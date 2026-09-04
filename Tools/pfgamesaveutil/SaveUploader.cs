using System.IO.Compression;
using System.Text;
using System.Text.Json.Nodes;

namespace PfGameSaveUtil;

/// <summary>
/// A single compressed (zip) chunk blob to upload.
/// </summary>
public class UploadChunk
{
    public string FileName { get; set; } = "";
    public byte[] Bytes { get; set; } = Array.Empty<byte>();
}

/// <summary>
/// The compressed chunk(s) + extended manifest blobs to upload for a single save version.
/// Large saves are split across multiple zip chunks so no single zip exceeds the
/// console's 64MB limit.
/// </summary>
public class UploadPackage
{
    public List<UploadChunk> Chunks { get; set; } = new();
    public string ExtendedManifestFileName { get; set; } = "";
    public byte[] ExtendedManifestBytes { get; set; } = Array.Empty<byte>();
    public int FileCount { get; set; }
    public long TotalUncompressedBytes { get; set; }

    public long TotalCompressedBytes => Chunks.Sum(c => c.Bytes.LongLength);
}

/// <summary>
/// Packages a local folder into the blobs required for a Game Save upload:
///   1. One or more compressed (zip) chunks containing the files. Files are split
///      across chunks so no single zip exceeds the configured limit (the console
///      cannot handle zips larger than 64MB).
///   2. An "extended manifest" JSON describing the folder tree and per-file
///      extraction mapping, matching the format the SDK writes (see
///      Source/PlayFabGameSave/Source/Types/ExtendedManifest.cpp). Each chunk is a
///      separate entry in the manifest's "Files" array with its own "Extract" list.
/// </summary>
public static class SaveUploader
{
    // The SDK uses the all-zeros GUID to represent the save root folder.
    private const string RootFolderId = "{00000000-0000-0000-0000-000000000000}";

    // The console cannot extract zips larger than 64MB, so chunks are capped below this.
    public const long DefaultMaxZipBytes = 64L * 1024 * 1024;

    private record PendingFile(
        string FullPath,
        string Rel,
        string FolderRel,
        FileInfo Info,
        long StandaloneZipSize);

    public static UploadPackage BuildPackage(
        string sourceFolder,
        ulong version,
        Action<string>? log = null,
        long maxZipBytes = DefaultMaxZipBytes)
    {
        sourceFolder = Path.GetFullPath(sourceFolder);
        if (!Directory.Exists(sourceFolder))
            throw new DirectoryNotFoundException($"Source folder not found: {sourceFolder}");

        // Skip the cloudsync metadata folder written by DownloadSingleSaveAsync so a
        // previously downloaded save folder doesn't re-upload service manifests / info
        // files as if they were game save files.
        static bool IsCloudSyncFile(string relPath)
        {
            var rel = relPath.Replace('\\', '/');
            return rel.Equals("cloudsync", StringComparison.OrdinalIgnoreCase) ||
                   rel.StartsWith("cloudsync/", StringComparison.OrdinalIgnoreCase);
        }

        var filePaths = Directory
            .EnumerateFiles(sourceFolder, "*", SearchOption.AllDirectories)
            .Where(fullPath => !IsCloudSyncFile(Path.GetRelativePath(sourceFolder, fullPath)))
            .ToArray();
        if (filePaths.Length == 0)
            throw new InvalidOperationException($"No files to upload in: {sourceFolder}");

        if (maxZipBytes <= 0)
            maxZipBytes = DefaultMaxZipBytes;

        // Every distinct (relative, forward-slash) folder path gets a stable GUID.
        var folderIds = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

        // Pass 1: gather files and measure each file's standalone compressed size.
        // A file's standalone zip size is an upper bound for its contribution to a
        // combined zip (a combined zip only saves the per-file end-of-central-directory
        // record), so packing by standalone size guarantees each chunk stays under the limit.
        var pending = new List<PendingFile>();
        long totalUncompressed = 0;
        foreach (var fullPath in filePaths.OrderBy(p => p, StringComparer.OrdinalIgnoreCase))
        {
            var rel = Path.GetRelativePath(sourceFolder, fullPath).Replace('\\', '/');
            var folderRel = Path.GetDirectoryName(rel)?.Replace('\\', '/') ?? "";

            if (!string.IsNullOrEmpty(folderRel))
                RegisterFolderPath(folderRel, folderIds);

            var info = new FileInfo(fullPath);
            totalUncompressed += info.Length;

            var standaloneSize = MeasureZippedSize(fullPath, rel);
            if (standaloneSize > maxZipBytes)
            {
                log?.Invoke($"  WARNING: '{rel}' compresses to {FormatBytes(standaloneSize)}, which exceeds the " +
                            $"{FormatBytes(maxZipBytes)} zip limit. It will be uploaded in its own chunk, but the " +
                            $"console may be unable to extract it.");
            }

            pending.Add(new PendingFile(fullPath, rel, folderRel, info, standaloneSize));
        }

        // Pass 2: greedily bin-pack files into chunks that stay under the size limit.
        var bins = new List<List<PendingFile>>();
        var current = new List<PendingFile>();
        long currentSize = 0;
        foreach (var pf in pending)
        {
            if (current.Count > 0 && currentSize + pf.StandaloneZipSize > maxZipBytes)
            {
                bins.Add(current);
                current = new List<PendingFile>();
                currentSize = 0;
            }
            current.Add(pf);
            currentSize += pf.StandaloneZipSize;
        }
        if (current.Count > 0)
            bins.Add(current);

        // Pass 3: build a real zip for each bin and a manifest "Files" entry per chunk.
        var filesArray = new JsonArray();
        var chunks = new List<UploadChunk>();

        foreach (var bin in bins)
        {
            var chunkId = Guid.NewGuid().ToString();
            var chunkFileName = $"{chunkId}.zip";

            var extractEntries = new JsonArray();
            long chunkUncompressed = 0;

            using var zipStream = new MemoryStream();
            using (var archive = new ZipArchive(zipStream, ZipArchiveMode.Create, leaveOpen: true))
            {
                foreach (var pf in bin)
                {
                    // Zip entry name is the full relative path with forward slashes; this is what
                    // the extractor reconstructs as "{folderPath}/{fileName}" when restoring.
                    chunkUncompressed += pf.Info.Length;

                    var entry = archive.CreateEntry(pf.Rel, CompressionLevel.Optimal);
                    entry.LastWriteTime = pf.Info.LastWriteTime;
                    using (var entryStream = entry.Open())
                    using (var fileStream = pf.Info.OpenRead())
                    {
                        fileStream.CopyTo(entryStream);
                    }

                    var folderId = string.IsNullOrEmpty(pf.FolderRel) ? RootFolderId : folderIds[pf.FolderRel];

                    extractEntries.Add(new JsonObject
                    {
                        ["Name"] = Path.GetFileName(pf.Rel),
                        ["FileId"] = Guid.NewGuid().ToString(),
                        ["FolderId"] = folderId,
                        ["Size"] = pf.Info.Length,
                        ["SkipFile"] = false,
                        ["LastModified"] = ToIso8601(pf.Info.LastWriteTimeUtc),
                        ["Created"] = ToIso8601(pf.Info.CreationTimeUtc)
                    });

                    log?.Invoke($"  + {pf.Rel} ({pf.Info.Length} bytes) -> {chunkFileName}");
                }
            }

            var chunkBytes = zipStream.ToArray();
            chunks.Add(new UploadChunk { FileName = chunkFileName, Bytes = chunkBytes });

            // One compressed file entry per chunk, holding the files extracted from it.
            filesArray.Add(new JsonObject
            {
                ["Name"] = chunkFileName,
                ["FileId"] = chunkId,
                ["CompressSize"] = chunkBytes.LongLength,
                ["Size"] = chunkUncompressed,
                ["LastModified"] = ToIso8601(DateTime.UtcNow),
                ["Compression"] = "zip",
                ["Extract"] = extractEntries
            });
        }

        var v1 = new JsonObject { ["Files"] = filesArray };

        // Only top-level subfolders are written (the root itself is implicit).
        var foldersArray = BuildFoldersJson(folderIds);
        if (foldersArray.Count > 0)
            v1["Folders"] = foldersArray;

        var root = new JsonObject { ["v1"] = v1 };

        var manifestName = $"extended-{version}-manifest.json";
        var manifestBytes = Encoding.UTF8.GetBytes(root.ToJsonString());

        return new UploadPackage
        {
            Chunks = chunks,
            ExtendedManifestFileName = manifestName,
            ExtendedManifestBytes = manifestBytes,
            FileCount = filePaths.Length,
            TotalUncompressedBytes = totalUncompressed
        };
    }

    /// <summary>
    /// Compresses a single file into a one-entry zip and returns the resulting byte size.
    /// Used to size files for bin-packing without committing them to a chunk yet.
    /// </summary>
    private static long MeasureZippedSize(string fullPath, string entryName)
    {
        using var ms = new MemoryStream();
        using (var archive = new ZipArchive(ms, ZipArchiveMode.Create, leaveOpen: true))
        {
            var entry = archive.CreateEntry(entryName, CompressionLevel.Optimal);
            using var entryStream = entry.Open();
            using var fileStream = File.OpenRead(fullPath);
            fileStream.CopyTo(entryStream);
        }
        return ms.Length;
    }

    private static string FormatBytes(long bytes)
    {
        string[] units = { "B", "KB", "MB", "GB" };
        double size = bytes;
        int unit = 0;
        while (size >= 1024 && unit < units.Length - 1)
        {
            size /= 1024;
            unit++;
        }
        return $"{size:0.#} {units[unit]}";
    }

    /// <summary>
    /// Registers a folder path and all of its ancestors, assigning each a GUID.
    /// </summary>
    private static void RegisterFolderPath(string folderRel, Dictionary<string, string> folderIds)
    {
        var parts = folderRel.Split('/', StringSplitOptions.RemoveEmptyEntries);
        var current = "";
        foreach (var part in parts)
        {
            current = string.IsNullOrEmpty(current) ? part : $"{current}/{part}";
            if (!folderIds.ContainsKey(current))
                folderIds[current] = Guid.NewGuid().ToString();
        }
    }

    /// <summary>
    /// Builds the nested Folders JSON array from the flat folder-path -> GUID map.
    /// </summary>
    private static JsonArray BuildFoldersJson(Dictionary<string, string> folderIds)
    {
        JsonArray BuildLevel(string parent)
        {
            var arr = new JsonArray();
            foreach (var kvp in folderIds)
            {
                var path = kvp.Key;
                var idx = path.LastIndexOf('/');
                var thisParent = idx < 0 ? "" : path.Substring(0, idx);
                var name = idx < 0 ? path : path.Substring(idx + 1);
                if (string.Equals(thisParent, parent, StringComparison.OrdinalIgnoreCase))
                {
                    arr.Add(new JsonObject
                    {
                        ["Name"] = name,
                        ["Id"] = kvp.Value,
                        ["Folders"] = BuildLevel(path)
                    });
                }
            }
            return arr;
        }

        return BuildLevel("");
    }

    private static string ToIso8601(DateTime utc) => utc.ToString("yyyy-MM-ddTHH:mm:ssZ");
}
