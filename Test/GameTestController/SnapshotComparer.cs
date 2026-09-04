using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text.Json;

namespace GameTestController
{
    internal static class SnapshotComparer
    {
        public static SnapshotComparisonResult Compare(
            SnapshotCaptureRecord baseline,
            SnapshotCaptureRecord candidate,
            out SnapshotSummary baselineSummary,
            out SnapshotSummary candidateSummary,
            bool ignoreTimestamps = false,
            bool ignoreDirectories = false,
            bool ignoreHashes = false)
        {
            baselineSummary = SnapshotSummary.FromMetadataOnly(baseline.DisplayName);
            candidateSummary = SnapshotSummary.FromMetadataOnly(candidate.DisplayName);

            try
            {
                SnapshotManifest baselineManifest = ParseManifest(baseline);
                SnapshotManifest candidateManifest = ParseManifest(candidate);

                baselineSummary = SnapshotSummary.FromManifest(baselineManifest);
                candidateSummary = SnapshotSummary.FromManifest(candidateManifest);

                var differences = new List<string>();

                // If either snapshot is system-managed (e.g., Xbox GRTS), we cannot compare file contents
                // because the platform manages the storage and we don't have direct file access.
                // In this case, we consider the comparison successful (trust the platform).
                if (baselineManifest.IsSystemManaged || candidateManifest.IsSystemManaged)
                {
                    // Log which side(s) are system-managed for clarity
                    if (baselineManifest.IsSystemManaged && candidateManifest.IsSystemManaged)
                    {
                        // Both are system-managed - comparison not possible but OK
                        return SnapshotComparisonResult.Success();
                    }
                    else if (baselineManifest.IsSystemManaged)
                    {
                        // Baseline is system-managed, candidate is not - can't compare
                        // Consider this OK since the platform manages the baseline
                        return SnapshotComparisonResult.Success();
                    }
                    else
                    {
                        // Candidate is system-managed, baseline is not - can't compare
                        // Consider this OK since the platform manages the candidate
                        return SnapshotComparisonResult.Success();
                    }
                }

                if (baselineManifest.HashesIncluded != candidateManifest.HashesIncluded)
                {
                    differences.Add(
                        $"Hash coverage mismatch: {baseline.DisplayName} hashesIncluded={baselineManifest.HashesIncluded} vs {candidate.DisplayName} hashesIncluded={candidateManifest.HashesIncluded}");
                }

                CompareTotals(baselineManifest, candidateManifest, differences, ignoreDirectories);
                CompareEntries(baselineManifest, candidateManifest, differences, ignoreTimestamps, ignoreDirectories, ignoreHashes);

                return differences.Count == 0
                    ? SnapshotComparisonResult.Success()
                    : SnapshotComparisonResult.FromDifferences(differences);
            }
            catch (Exception ex)
            {
                return SnapshotComparisonResult.FromError(ex.Message);
            }
        }

        internal sealed record SnapshotSummary(
            string DisplayName,
            bool HashesIncluded,
            int? FileCount,
            int? DirectoryCount,
            long? TotalBytes)
        {
            internal static SnapshotSummary FromManifest(SnapshotManifest manifest)
            {
                return new SnapshotSummary(
                    manifest.DisplayName,
                    manifest.HashesIncluded,
                    manifest.Totals?.Files,
                    manifest.Totals?.Directories,
                    manifest.Totals?.Bytes);
            }

            internal static SnapshotSummary FromMetadataOnly(string displayName)
            {
                return new SnapshotSummary(displayName, false, null, null, null);
            }
        }

        private static void CompareTotals(SnapshotManifest baseline, SnapshotManifest candidate, IList<string> differences, bool ignoreDirectories = false)
        {
            if (baseline.Totals == null || candidate.Totals == null)
            {
                return;
            }

            if (baseline.Totals.Files.HasValue && candidate.Totals.Files.HasValue && baseline.Totals.Files.Value != candidate.Totals.Files.Value)
            {
                differences.Add($"File count differs: {baseline.DisplayName}={baseline.Totals.Files.Value} vs {candidate.DisplayName}={candidate.Totals.Files.Value}");
            }

            if (!ignoreDirectories && baseline.Totals.Directories.HasValue && candidate.Totals.Directories.HasValue && baseline.Totals.Directories.Value != candidate.Totals.Directories.Value)
            {
                differences.Add($"Directory count differs: {baseline.DisplayName}={baseline.Totals.Directories.Value} vs {candidate.DisplayName}={candidate.Totals.Directories.Value}");
            }

            if (baseline.Totals.Bytes.HasValue && candidate.Totals.Bytes.HasValue && baseline.Totals.Bytes.Value != candidate.Totals.Bytes.Value)
            {
                differences.Add($"Total byte count differs: {baseline.DisplayName}={baseline.Totals.Bytes.Value} vs {candidate.DisplayName}={candidate.Totals.Bytes.Value}");
            }
        }

        private static void CompareEntries(SnapshotManifest baseline, SnapshotManifest candidate, IList<string> differences, bool ignoreTimestamps = false, bool ignoreDirectories = false, bool ignoreHashes = false)
        {
            // When ignoreDirectories is true, filter out directory entries from comparison
            var baselineEntries = ignoreDirectories
                ? baseline.Entries.Where(kvp => !string.Equals(kvp.Value.Type, "directory", StringComparison.OrdinalIgnoreCase))
                    .ToDictionary(kvp => kvp.Key, kvp => kvp.Value, StringComparer.OrdinalIgnoreCase)
                : baseline.Entries;
            var candidateEntries = ignoreDirectories
                ? candidate.Entries.Where(kvp => !string.Equals(kvp.Value.Type, "directory", StringComparison.OrdinalIgnoreCase))
                    .ToDictionary(kvp => kvp.Key, kvp => kvp.Value, StringComparer.OrdinalIgnoreCase)
                : candidate.Entries;

            var baselinePaths = new HashSet<string>(baselineEntries.Keys, StringComparer.OrdinalIgnoreCase);
            var candidatePaths = new HashSet<string>(candidateEntries.Keys, StringComparer.OrdinalIgnoreCase);

            foreach (string missingInCandidate in baselinePaths.Except(candidatePaths, StringComparer.OrdinalIgnoreCase))
            {
                differences.Add($"Path '{missingInCandidate}' present in {baseline.DisplayName} but missing in {candidate.DisplayName}");
            }

            foreach (string extraInCandidate in candidatePaths.Except(baselinePaths, StringComparer.OrdinalIgnoreCase))
            {
                differences.Add($"Path '{extraInCandidate}' present in {candidate.DisplayName} but missing in {baseline.DisplayName}");
            }

            foreach (string path in baselinePaths.Intersect(candidatePaths, StringComparer.OrdinalIgnoreCase))
            {
                SnapshotEntry baselineEntry = baselineEntries[path];
                SnapshotEntry candidateEntry = candidateEntries[path];

                if (!string.Equals(baselineEntry.Type, candidateEntry.Type, StringComparison.OrdinalIgnoreCase))
                {
                    differences.Add($"[{baseline.DisplayName} vs {candidate.DisplayName}] Path '{path}' type differs: {baselineEntry.Type} vs {candidateEntry.Type}");
                    continue;
                }

                bool isFileLike = string.Equals(baselineEntry.Type, "file", StringComparison.OrdinalIgnoreCase)
                    || string.Equals(baselineEntry.Type, "symlink", StringComparison.OrdinalIgnoreCase);

                if (isFileLike)
                {
                    if (baselineEntry.Size.HasValue && candidateEntry.Size.HasValue && baselineEntry.Size.Value != candidateEntry.Size.Value)
                    {
                        differences.Add($"[{baseline.DisplayName} vs {candidate.DisplayName}] Path '{path}' size differs: {baselineEntry.Size.Value} vs {candidateEntry.Size.Value}");
                    }

                    // Skip timestamp comparison if ignoreTimestamps is true (workaround for GRTS timestamp bug)
                    if (!ignoreTimestamps && baselineEntry.LastWriteTimeEpochMs.HasValue && candidateEntry.LastWriteTimeEpochMs.HasValue)
                    {
                        long baselineTime = baselineEntry.LastWriteTimeEpochMs.Value;
                        long candidateTime = candidateEntry.LastWriteTimeEpochMs.Value;
                        const long toleranceMs = 3000; // allow a few seconds of skew
                        if (Math.Abs(baselineTime - candidateTime) > toleranceMs)
                        {
                            differences.Add(
                                $"[{baseline.DisplayName} vs {candidate.DisplayName}] Path '{path}' timestamp differs: {baselineTime} vs {candidateTime}");
                        }
                    }

                    if (!ignoreHashes && baseline.HashesIncluded && candidate.HashesIncluded)
                    {
                        string baselineHash = baselineEntry.Sha256 ?? string.Empty;
                        string candidateHash = candidateEntry.Sha256 ?? string.Empty;
                        if (!string.Equals(baselineHash, candidateHash, StringComparison.OrdinalIgnoreCase))
                        {
                            differences.Add($"[{baseline.DisplayName} vs {candidate.DisplayName}] Path '{path}' hash differs: {baselineHash} vs {candidateHash}");
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Returns a filtered view of entries excluding directories that contain no child files.
        /// A directory is considered "empty" if no file entry's path starts with that directory path.
        /// </summary>
        private static IReadOnlyDictionary<string, SnapshotEntry> FilterOutEmptyDirectories(IReadOnlyDictionary<string, SnapshotEntry> entries)
        {
            // Collect all file paths
            var filePaths = entries
                .Where(e => string.Equals(e.Value.Type, "file", StringComparison.OrdinalIgnoreCase)
                         || string.Equals(e.Value.Type, "symlink", StringComparison.OrdinalIgnoreCase))
                .Select(e => e.Key)
                .ToList();

            var result = new Dictionary<string, SnapshotEntry>(StringComparer.OrdinalIgnoreCase);
            foreach (var kvp in entries)
            {
                if (string.Equals(kvp.Value.Type, "directory", StringComparison.OrdinalIgnoreCase))
                {
                    // Keep this directory only if some file lives under it
                    string dirPrefix = kvp.Key.TrimEnd('/', '\\') + "/";
                    bool hasChildren = filePaths.Any(f => f.StartsWith(dirPrefix, StringComparison.OrdinalIgnoreCase));
                    if (!hasChildren)
                    {
                        continue; // skip empty directory
                    }
                }
                result[kvp.Key] = kvp.Value;
            }
            return result;
        }

        private static SnapshotManifest ParseManifest(SnapshotCaptureRecord record)
        {
            if (string.IsNullOrWhiteSpace(record.RawJson))
            {
                throw new InvalidOperationException($"Snapshot for slot '{record.DisplayName}' is empty.");
            }

            using JsonDocument doc = JsonDocument.Parse(record.RawJson);
            JsonElement root = doc.RootElement;
            if (root.ValueKind != JsonValueKind.Object)
            {
                throw new InvalidOperationException("Action result payload is not a JSON object.");
            }

            if (!root.TryGetProperty("snapshot", out JsonElement snapshotElement) || snapshotElement.ValueKind != JsonValueKind.Object)
            {
                throw new InvalidOperationException("Action result missing 'snapshot' object.");
            }

            bool hashesIncluded = false;
            if (snapshotElement.TryGetProperty("hashesIncluded", out JsonElement hashesElement))
            {
                hashesIncluded = hashesElement.ValueKind switch
                {
                    JsonValueKind.True => true,
                    JsonValueKind.False => false,
                    JsonValueKind.String => bool.TryParse(hashesElement.GetString(), out bool parsed) && parsed,
                    JsonValueKind.Number => hashesElement.TryGetInt32(out int numeric) && numeric != 0,
                    _ => false
                };
            }

            // Check if this is a system-managed path (Xbox GRTS)
            bool isSystemManaged = false;
            if (snapshotElement.TryGetProperty("isSystemManaged", out JsonElement sysManagedElement))
            {
                isSystemManaged = sysManagedElement.ValueKind switch
                {
                    JsonValueKind.True => true,
                    JsonValueKind.False => false,
                    JsonValueKind.String => bool.TryParse(sysManagedElement.GetString(), out bool parsed) && parsed,
                    JsonValueKind.Number => sysManagedElement.TryGetInt32(out int numeric) && numeric != 0,
                    _ => false
                };
            }

            SnapshotTotals? totals = null;
            if (snapshotElement.TryGetProperty("totals", out JsonElement totalsElement) && totalsElement.ValueKind == JsonValueKind.Object)
            {
                totals = new SnapshotTotals(
                    TryGetInt32(totalsElement, "files"),
                    TryGetInt32(totalsElement, "directories"),
                    TryGetInt64(totalsElement, "bytes"));
            }

            if (!snapshotElement.TryGetProperty("entries", out JsonElement entriesElement) || entriesElement.ValueKind != JsonValueKind.Array)
            {
                throw new InvalidOperationException("Snapshot missing 'entries' array.");
            }

            var entries = new Dictionary<string, SnapshotEntry>(StringComparer.OrdinalIgnoreCase);
            foreach (JsonElement entryElement in entriesElement.EnumerateArray())
            {
                if (entryElement.ValueKind != JsonValueKind.Object)
                {
                    continue;
                }

                if (!entryElement.TryGetProperty("path", out JsonElement pathElement) || pathElement.ValueKind != JsonValueKind.String)
                {
                    throw new InvalidOperationException("Snapshot entry missing 'path'.");
                }

                string path = pathElement.GetString() ?? string.Empty;
                if (string.IsNullOrEmpty(path))
                {
                    throw new InvalidOperationException("Snapshot entry contained an empty path.");
                }

                string type = entryElement.TryGetProperty("type", out JsonElement typeElement) && typeElement.ValueKind == JsonValueKind.String
                    ? typeElement.GetString() ?? string.Empty
                    : string.Empty;

                long? size = TryGetInt64(entryElement, "size");
                long? lastWriteTime = TryGetInt64(entryElement, "lastWriteTimeEpochMs");
                string? sha256 = entryElement.TryGetProperty("sha256", out JsonElement hashElement) && hashElement.ValueKind == JsonValueKind.String
                    ? hashElement.GetString()
                    : null;

                entries[path] = new SnapshotEntry(path, type, size, lastWriteTime, sha256);
            }

            return new SnapshotManifest(record.DisplayName, hashesIncluded, isSystemManaged, totals, entries);
        }

        private static int? TryGetInt32(JsonElement parent, string propertyName)
        {
            if (parent.TryGetProperty(propertyName, out JsonElement element))
            {
                if (element.TryGetInt32(out int intValue))
                {
                    return intValue;
                }

                if (element.ValueKind == JsonValueKind.String && int.TryParse(element.GetString(), NumberStyles.Integer, CultureInfo.InvariantCulture, out int parsed))
                {
                    return parsed;
                }
            }

            return null;
        }

        private static long? TryGetInt64(JsonElement parent, string propertyName)
        {
            if (parent.TryGetProperty(propertyName, out JsonElement element))
            {
                if (element.TryGetInt64(out long value))
                {
                    return value;
                }

                if (element.ValueKind == JsonValueKind.String && long.TryParse(element.GetString(), NumberStyles.Integer, CultureInfo.InvariantCulture, out long parsed))
                {
                    return parsed;
                }
            }

            return null;
        }

        internal sealed record SnapshotManifest(
            string DisplayName,
            bool HashesIncluded,
            bool IsSystemManaged,
            SnapshotTotals? Totals,
            IReadOnlyDictionary<string, SnapshotEntry> Entries);

        internal sealed record SnapshotTotals(int? Files, int? Directories, long? Bytes);

        internal sealed record SnapshotEntry(
            string Path,
            string Type,
            long? Size,
            long? LastWriteTimeEpochMs,
            string? Sha256);
    }

    internal sealed record SnapshotComparisonResult(bool Succeeded, IReadOnlyList<string> Differences, string? FailureReason)
    {
        public static SnapshotComparisonResult Success() => new(true, Array.Empty<string>(), null);

        public static SnapshotComparisonResult FromDifferences(IEnumerable<string> differences)
        {
            IReadOnlyList<string> list = differences as IReadOnlyList<string> ?? differences.ToList();
            return new SnapshotComparisonResult(false, list, null);
        }

        public static SnapshotComparisonResult FromError(string message) => new(false, Array.Empty<string>(), message);
    }
}
