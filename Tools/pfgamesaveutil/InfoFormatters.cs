using System.Text.Json;
using PfGameSaveUtil.Models;
using static PfGameSaveUtil.Utilities;

namespace PfGameSaveUtil;

public class ManifestDetail
{
    public string Name { get; set; } = "";
    public string Version { get; set; } = "";
    public string? BaseVersion { get; set; }
    public string Status { get; set; } = "";
    public bool? KnownGood { get; set; }
    public int FileCount { get; set; }
    public int FolderCount { get; set; }
    public int ChunkCount { get; set; }
    public long CompressedSize { get; set; }
    public long UncompressedSize { get; set; }
    public string? Created { get; set; }
    public string? LastModified { get; set; }
    // Decoded manifest description, or null if the manifest has no description.
    public string? Description { get; set; }
}

public class QuotaInfo
{
    public long TotalBytes { get; set; }
    public long AvailableBytes { get; set; }
    public long UsedBytes => TotalBytes - AvailableBytes;
    public double UsedPercent => TotalBytes > 0 ? (double)UsedBytes / TotalBytes * 100 : 0;
}

public static class InfoFormatters
{
    public static Dictionary<string, (long Size, DateTime? Modified)> BuildFileList(ExtendedManifest manifest, string source)
    {
        var result = new Dictionary<string, (long Size, DateTime? Modified)>();
        var folders = BuildFolderLookup(manifest.V1?.Folders ?? new List<ManifestFolder>());

        foreach (var file in manifest.V1?.Files ?? new List<ManifestFile>())
        {
            foreach (var extract in file.Extract ?? new List<ExtractEntry>())
            {
                // Skip files that are marked as superseded
                if (extract.SkipFile)
                    continue;
                    
                var folderPath = "";
                if (!string.IsNullOrEmpty(extract.FolderId) && folders.TryGetValue(extract.FolderId, out var fp))
                {
                    folderPath = fp;
                }

                var fullPath = string.IsNullOrEmpty(folderPath) 
                    ? extract.Name ?? "" 
                    : Path.Combine(folderPath, extract.Name ?? "");

                DateTime? modified = null;
                if (!string.IsNullOrEmpty(extract.LastModified) && DateTime.TryParse(extract.LastModified, out var dt))
                {
                    modified = dt;
                }

                result[fullPath] = (extract.Size, modified);
            }
        }
        return result;
    }

    public static Dictionary<string, string> BuildFolderLookup(List<ManifestFolder> folders, string prefix = "")
    {
        var lookup = new Dictionary<string, string>();
        foreach (var folder in folders)
        {
            if (string.IsNullOrEmpty(folder.Id))
                continue;
                
            var fullPath = string.IsNullOrEmpty(prefix)
                ? folder.Name ?? ""
                : Path.Combine(prefix, folder.Name ?? "");
            lookup[folder.Id] = fullPath;

            if (folder.Folders != null && folder.Folders.Count > 0)
            {
                foreach (var nested in BuildFolderLookup(folder.Folders, fullPath))
                {
                    lookup[nested.Key] = nested.Value;
                }
            }
        }
        return lookup;
    }

    public static void OutputSummary(string playerId, List<ManifestDetail> manifests, List<string> healthIssues, QuotaInfo? quota = null)
    {
        var statusCounts = manifests.GroupBy(m => m.Status)
            .ToDictionary(g => g.Key, g => g.Count());
        
        var totalCompressed = manifests.Sum(m => m.CompressedSize);
        var totalUncompressed = manifests.Sum(m => m.UncompressedSize);
        var totalFiles = manifests.Sum(m => m.FileCount);
        var totalChunks = manifests.Sum(m => m.ChunkCount);

        Console.WriteLine($"Player: {playerId}");
        
        var statusBreakdown = string.Join(", ", statusCounts.Select(kv => $"{kv.Value} {kv.Key}"));
        Console.WriteLine($"Manifests: {manifests.Count} ({statusBreakdown})");
        Console.WriteLine($"Total Files: {totalFiles}");
        Console.WriteLine($"Total Chunks: {totalChunks}");
        
        if (totalUncompressed > 0)
        {
            var ratio = (double)totalUncompressed / totalCompressed;
            Console.WriteLine($"Storage: {FormatSize(totalCompressed)} compressed / {FormatSize(totalUncompressed)} uncompressed ({ratio:F1}x ratio)");
        }

        if (quota != null)
        {
            Console.WriteLine();
            Console.WriteLine("Quota:");
            Console.WriteLine($"  Used: {FormatSize(quota.UsedBytes)} / {FormatSize(quota.TotalBytes)} ({quota.UsedPercent:F1}%)");
            Console.WriteLine($"  Available: {FormatSize(quota.AvailableBytes)}");
        }

        if (healthIssues.Count > 0)
        {
            Console.WriteLine();
            Console.WriteLine("Health Issues:");
            foreach (var issue in healthIssues)
            {
                Console.WriteLine($"  ⚠ {issue}");
            }
        }
    }

    public static void OutputVerbose(string playerId, List<ManifestDetail> manifests, List<string> healthIssues, QuotaInfo? quota = null)
    {
        OutputSummary(playerId, manifests, healthIssues, quota);

        if (manifests.Count == 0) return;

        Console.WriteLine();
        Console.WriteLine("Manifests (newest first):");
        foreach (var m in manifests)
        {
            var knownGood = m.KnownGood == true ? " (known good)" : "";
            Console.WriteLine();
            Console.WriteLine($"  Version {m.Version}  [{m.Status}]{knownGood}");

            if (!string.IsNullOrEmpty(m.BaseVersion) && m.BaseVersion != "0")
                Console.WriteLine($"    Base version: {m.BaseVersion}");

            Console.WriteLine($"    Description:  {(string.IsNullOrEmpty(m.Description) ? "(missing)" : m.Description)}");

            Console.WriteLine($"    Files:        {m.FileCount}");
            Console.WriteLine($"    Folders:      {m.FolderCount}");
            Console.WriteLine($"    Chunks:       {m.ChunkCount}");

            if (m.UncompressedSize > 0 || m.CompressedSize > 0)
            {
                var ratio = m.CompressedSize > 0 ? (double)m.UncompressedSize / m.CompressedSize : 0;
                Console.WriteLine($"    Size:         {FormatSize(m.CompressedSize)} compressed / {FormatSize(m.UncompressedSize)} uncompressed ({ratio:F1}x)");
            }
            else
            {
                Console.WriteLine($"    Size:         N/A (details available for finalized manifests only)");
            }

            if (!string.IsNullOrEmpty(m.Created))
                Console.WriteLine($"    Created:      {m.Created}");
            if (!string.IsNullOrEmpty(m.LastModified))
                Console.WriteLine($"    Finalized:    {m.LastModified}");
        }
    }

    public static void OutputJson(string playerId, List<ManifestDetail> manifests, List<string> healthIssues, QuotaInfo? quota = null)
    {
        var output = new
        {
            playerId,
            summary = new
            {
                manifestCount = manifests.Count,
                statusBreakdown = manifests.GroupBy(m => m.Status).ToDictionary(g => g.Key, g => g.Count()),
                totalFiles = manifests.Sum(m => m.FileCount),
                totalChunks = manifests.Sum(m => m.ChunkCount),
                totalCompressedBytes = manifests.Sum(m => m.CompressedSize),
                totalUncompressedBytes = manifests.Sum(m => m.UncompressedSize)
            },
            quota = quota != null ? new
            {
                totalBytes = quota.TotalBytes,
                usedBytes = quota.UsedBytes,
                availableBytes = quota.AvailableBytes,
                usedPercent = quota.UsedPercent
            } : null,
            manifests = manifests.Select(m => new
            {
                name = m.Name,
                version = m.Version,
                baseVersion = m.BaseVersion,
                status = m.Status,
                knownGood = m.KnownGood,
                description = m.Description,
                fileCount = m.FileCount,
                folderCount = m.FolderCount,
                chunkCount = m.ChunkCount,
                compressedBytes = m.CompressedSize,
                uncompressedBytes = m.UncompressedSize,
                created = m.Created,
                lastModified = m.LastModified
            }),
            healthIssues
        };

        var options = new JsonSerializerOptions { WriteIndented = true };
        Console.WriteLine(JsonSerializer.Serialize(output, options));
    }
}
