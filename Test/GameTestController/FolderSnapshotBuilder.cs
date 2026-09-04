using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;

namespace GameTestController
{
    /// <summary>
    /// Builds a <see cref="SnapshotCaptureRecord"/> from an on-disk folder using the
    /// same JSON schema the device emits for CaptureSaveContainerSnapshot. This lets a
    /// cloud download (fetched via pfgamesaveutil) be fed straight into
    /// <see cref="SnapshotComparer"/> and compared against a device-captured snapshot slot.
    ///
    /// Schema produced (matches Test/GameTestAppShared/TestHarness/CaptureSaveContainerSnapshot.cpp):
    ///   { "snapshot": { "hashesIncluded": true, "isSystemManaged": false,
    ///                   "totals": { "files", "directories", "bytes" },
    ///                   "entries": [ { "path", "type", "size", "sha256" }, ... ] } }
    /// Paths are relative to the folder root and use forward slashes (generic form),
    /// matching the device's PathToGenericString output.
    /// </summary>
    internal static class FolderSnapshotBuilder
    {
        /// <summary>
        /// Walks <paramref name="rootPath"/> and returns a snapshot record under the
        /// given <paramref name="slot"/> display name. Top-level reserved components
        /// (e.g. "cloudsync") are skipped to mirror the device-side capture, which the
        /// cloud round-trip does not round-trip.
        /// </summary>
        public static SnapshotCaptureRecord Build(string rootPath, string slot, bool includeHashes = true)
        {
            var root = new DirectoryInfo(rootPath);
            var entries = new List<(string Path, string Type, long? Size, string? Sha256)>();

            long totalBytes = 0;
            int fileCount = 0;
            int dirCount = 0;

            if (root.Exists)
            {
                foreach (FileSystemInfo info in EnumerateEntries(root))
                {
                    string relative = MakeRelativeGeneric(root.FullName, info.FullName);
                    if (relative.Length == 0 || IsRootLevelReserved(relative))
                    {
                        continue;
                    }

                    bool isDir = (info.Attributes & FileAttributes.Directory) == FileAttributes.Directory;
                    if (isDir)
                    {
                        dirCount++;
                        entries.Add((relative, "directory", null, null));
                    }
                    else
                    {
                        var file = (FileInfo)info;
                        fileCount++;
                        totalBytes += file.Length;
                        string? hash = includeHashes ? ComputeSha256(file.FullName) : null;
                        entries.Add((relative, "file", file.Length, hash));
                    }
                }
            }

            entries.Sort((a, b) => string.CompareOrdinal(a.Path, b.Path));

            string json = BuildJson(entries, includeHashes, fileCount, dirCount, totalBytes);

            return new SnapshotCaptureRecord(
                Slot: slot,
                DeviceId: "pfgamesaveutil",
                CommandId: string.Empty,
                Status: "succeeded",
                HResult: "0x00000000",
                Timestamp: DateTimeOffset.UtcNow,
                RawJson: json);
        }

        private static IEnumerable<FileSystemInfo> EnumerateEntries(DirectoryInfo root)
        {
            // Recurse depth-first; both directories and files are reported so the
            // schema matches the device snapshot (which records directory entries too).
            var stack = new Stack<DirectoryInfo>();
            stack.Push(root);
            while (stack.Count > 0)
            {
                DirectoryInfo dir = stack.Pop();
                FileSystemInfo[] children;
                try
                {
                    children = dir.GetFileSystemInfos();
                }
                catch (IOException)
                {
                    continue;
                }
                catch (UnauthorizedAccessException)
                {
                    continue;
                }

                foreach (FileSystemInfo child in children)
                {
                    yield return child;
                    if ((child.Attributes & FileAttributes.Directory) == FileAttributes.Directory)
                    {
                        stack.Push((DirectoryInfo)child);
                    }
                }
            }
        }

        private static string MakeRelativeGeneric(string rootFullPath, string entryFullPath)
        {
            string rel = Path.GetRelativePath(rootFullPath, entryFullPath);
            return rel.Replace('\\', '/').Trim('/');
        }

        /// <summary>Skips a top-level "cloudsync" component (reserved by the SDK).</summary>
        private static bool IsRootLevelReserved(string relativeGenericPath)
        {
            int slash = relativeGenericPath.IndexOf('/');
            string firstComponent = slash < 0 ? relativeGenericPath : relativeGenericPath.Substring(0, slash);
            return string.Equals(firstComponent, "cloudsync", StringComparison.OrdinalIgnoreCase);
        }

        private static string ComputeSha256(string filePath)
        {
            using FileStream stream = File.OpenRead(filePath);
            using var sha = SHA256.Create();
            byte[] hash = sha.ComputeHash(stream);
            var sb = new StringBuilder(hash.Length * 2);
            foreach (byte b in hash)
            {
                sb.Append(b.ToString("x2", CultureInfo.InvariantCulture));
            }
            return sb.ToString();
        }

        private static string BuildJson(
            List<(string Path, string Type, long? Size, string? Sha256)> entries,
            bool hashesIncluded,
            int fileCount,
            int dirCount,
            long totalBytes)
        {
            using var ms = new MemoryStream();
            using (var writer = new Utf8JsonWriter(ms))
            {
                writer.WriteStartObject();
                writer.WritePropertyName("snapshot");
                writer.WriteStartObject();

                writer.WriteBoolean("hashesIncluded", hashesIncluded);
                writer.WriteBoolean("isSystemManaged", false);

                writer.WritePropertyName("totals");
                writer.WriteStartObject();
                writer.WriteNumber("files", fileCount);
                writer.WriteNumber("directories", dirCount);
                writer.WriteNumber("bytes", totalBytes);
                writer.WriteEndObject();

                writer.WritePropertyName("entries");
                writer.WriteStartArray();
                foreach (var entry in entries)
                {
                    writer.WriteStartObject();
                    writer.WriteString("path", entry.Path);
                    writer.WriteString("type", entry.Type);
                    if (entry.Size.HasValue)
                    {
                        writer.WriteNumber("size", entry.Size.Value);
                    }
                    if (entry.Sha256 != null)
                    {
                        writer.WriteString("sha256", entry.Sha256);
                    }
                    writer.WriteEndObject();
                }
                writer.WriteEndArray();

                writer.WriteEndObject(); // snapshot
                writer.WriteEndObject(); // root
            }

            return Encoding.UTF8.GetString(ms.ToArray());
        }
    }
}
