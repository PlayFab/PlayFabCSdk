using System.IO.Compression;
using PfGameSaveUtil.Models;

namespace PfGameSaveUtil;

/// <summary>
/// Extracts save files from compressed chunks based on the extended manifest.
/// </summary>
public class SaveExtractor
{
    private readonly string _outputDirectory;
    private bool _extractAll;

    public SaveExtractor(string outputDirectory)
    {
        _outputDirectory = outputDirectory;
        Directory.CreateDirectory(outputDirectory);
    }

    /// <summary>
    /// Extracts all files from a manifest, downloading chunks and reconstructing folder structure.
    /// </summary>
    public async Task ExtractAsync(
        ExtendedManifest manifest,
        Dictionary<string, byte[]> chunks,
        Action<string>? log = null,
        bool extractAll = false)
    {
        _extractAll = extractAll;
        var v1 = manifest.V1 ?? throw new InvalidOperationException("Manifest missing v1 section");
        var folders = v1.Folders ?? new List<ManifestFolder>();
        var files = v1.Files ?? new List<ManifestFile>();

        // Build folder ID -> name lookup (flatten nested folders)
        var folderLookup = BuildFolderLookup(folders);

        // Create folder structure
        foreach (var (folderId, folderName) in folderLookup)
        {
            if (!string.IsNullOrEmpty(folderName))
            {
                var folderPath = Path.GetFullPath(Path.Combine(_outputDirectory, folderName));

                // Ensure the folder path is within the output directory
                if (!folderPath.StartsWith(Path.GetFullPath(_outputDirectory)))
                {
                    log?.Invoke($"Warning: Skipping folder creation for path outside of output directory: {folderName}");
                    continue;
                }

                Directory.CreateDirectory(folderPath);
                log?.Invoke($"Created folder: {folderName}");
            }
        }

        // Process each compressed chunk
        foreach (var file in files)
        {
            if (string.IsNullOrEmpty(file.Name) || file.Extract == null)
                continue;

            if (!chunks.TryGetValue(file.Name, out var chunkData))
            {
                log?.Invoke($"Warning: Chunk not found: {file.Name}");
                continue;
            }

            await ExtractChunkAsync(file, chunkData, folderLookup, log);
        }
    }

    private Dictionary<string, string> BuildFolderLookup(List<ManifestFolder> folders, string prefix = "")
    {
        var lookup = new Dictionary<string, string>();
        foreach (var folder in folders)
        {
            if (!string.IsNullOrEmpty(folder.Id))
            {
                // Use forward slashes for zip paths (zip entries always use forward slashes)
                var fullPath = string.IsNullOrEmpty(prefix) 
                    ? folder.Name ?? "" 
                    : $"{prefix}/{folder.Name}";
                lookup[folder.Id] = fullPath;
                
                // Recurse into nested folders
                if (folder.Folders != null && folder.Folders.Count > 0)
                {
                    foreach (var nested in BuildFolderLookup(folder.Folders, fullPath))
                    {
                        lookup[nested.Key] = nested.Value;
                    }
                }
            }
        }
        return lookup;
    }

    private async Task ExtractChunkAsync(
        ManifestFile file,
        byte[] chunkData,
        Dictionary<string, string> folderLookup,
        Action<string>? log)
    {
        var compression = file.Compression?.ToLowerInvariant() ?? "none";

        switch (compression)
        {
            case "zip":
                await ExtractZipChunkAsync(file, chunkData, folderLookup, log);
                break;
            case "gzip":
                await ExtractGzipChunkAsync(file, chunkData, folderLookup, log);
                break;
            case "none":
                await ExtractUncompressedChunkAsync(file, chunkData, folderLookup, log);
                break;
            default:
                log?.Invoke($"Warning: Unknown compression type: {compression}");
                break;
        }
    }

    private async Task ExtractZipChunkAsync(
        ManifestFile file,
        byte[] chunkData,
        Dictionary<string, string> folderLookup,
        Action<string>? log)
    {
        using var memStream = new MemoryStream(chunkData);
        using var archive = new ZipArchive(memStream, ZipArchiveMode.Read);

        foreach (var extract in file.Extract ?? Enumerable.Empty<ExtractEntry>())
        {
            if (string.IsNullOrEmpty(extract.Name))
                continue;

            // Skip files that are marked as superseded (unless extractAll mode)
            if (extract.SkipFile && !_extractAll)
            {
                log?.Invoke($"Skipping superseded file: {extract.Name}");
                continue;
            }
            var folderName = "";
            if (!string.IsNullOrEmpty(extract.FolderId) && folderLookup.TryGetValue(extract.FolderId, out var foundFolder))
            {
                folderName = foundFolder;
            }
            var zipPath = string.IsNullOrEmpty(folderName) 
                ? extract.Name 
                : $"{folderName}/{extract.Name}";

            var entry = archive.GetEntry(zipPath) ?? archive.GetEntry(extract.Name);
            
            if (entry == null)
            {
                log?.Invoke($"Warning: Entry not found in zip: {zipPath}");
                continue;
            }

            var outputPath = GetOutputPath(extract, folderLookup);
            var fullOutputPath = Path.GetFullPath(outputPath);
            var fullOutputDirectory = Path.GetFullPath(_outputDirectory);

            if (!fullOutputPath.StartsWith(fullOutputDirectory + Path.DirectorySeparatorChar) ||
                Path.IsPathRooted(extract.Name) ||
                extract.Name.IndexOfAny(Path.GetInvalidPathChars()) >= 0)
            {
                log?.Invoke($"Warning: Invalid path detected for extraction: {extract.Name}");
                continue;
            }

            Directory.CreateDirectory(Path.GetDirectoryName(fullOutputPath)!);

            using var entryStream = entry.Open();
            using var outputStream = File.Create(fullOutputPath);
            await entryStream.CopyToAsync(outputStream);

            // Restore timestamp if available (ISO 8601 format)
            if (!string.IsNullOrEmpty(extract.LastModified) && DateTime.TryParse(extract.LastModified, out var timestamp))
            {
                File.SetLastWriteTime(outputPath, timestamp);
            }

            log?.Invoke($"Extracted: {extract.Name}");
        }
    }

    private async Task ExtractGzipChunkAsync(
        ManifestFile file,
        byte[] chunkData,
        Dictionary<string, string> folderLookup,
        Action<string>? log)
    {
        var extract = file.Extract?.FirstOrDefault();
        if (extract == null || string.IsNullOrEmpty(extract.Name))
            return;

        // Skip files that are marked as superseded (unless extractAll mode)
        if (extract.SkipFile && !_extractAll)
        {
            log?.Invoke($"Skipping superseded file: {extract.Name}");
            return;
        }

        var outputPath = GetOutputPath(extract, folderLookup);
        Directory.CreateDirectory(Path.GetDirectoryName(outputPath)!);

        using var memStream = new MemoryStream(chunkData);
        using var gzipStream = new GZipStream(memStream, CompressionMode.Decompress);
        using var outputStream = File.Create(outputPath);
        await gzipStream.CopyToAsync(outputStream);

        if (!string.IsNullOrEmpty(extract.LastModified) && DateTime.TryParse(extract.LastModified, out var timestamp))
        {
            File.SetLastWriteTime(outputPath, timestamp);
        }

        log?.Invoke($"Extracted (gzip): {extract.Name}");
    }

    private async Task ExtractUncompressedChunkAsync(
        ManifestFile file,
        byte[] chunkData,
        Dictionary<string, string> folderLookup,
        Action<string>? log)
    {
        var extract = file.Extract?.FirstOrDefault();
        if (extract == null || string.IsNullOrEmpty(extract.Name))
            return;

        // Skip files that are marked as superseded (unless extractAll mode)
        if (extract.SkipFile && !_extractAll)
        {
            log?.Invoke($"Skipping superseded file: {extract.Name}");
            return;
        }

        var outputPath = GetOutputPath(extract, folderLookup);
        Directory.CreateDirectory(Path.GetDirectoryName(outputPath)!);

        await File.WriteAllBytesAsync(outputPath, chunkData);

        if (!string.IsNullOrEmpty(extract.LastModified) && DateTime.TryParse(extract.LastModified, out var timestamp))
        {
            File.SetLastWriteTime(outputPath, timestamp);
        }

        log?.Invoke($"Wrote: {extract.Name}");
    }

    private string GetOutputPath(ExtractEntry extract, Dictionary<string, string> folderLookup)
    {
        var folderName = "";
        if (!string.IsNullOrEmpty(extract.FolderId) && folderLookup.TryGetValue(extract.FolderId, out var foundFolder))
        {
            folderName = foundFolder;
        }

        return Path.Combine(_outputDirectory, folderName, extract.Name!);
    }
}
