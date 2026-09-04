using System.Text;
using System.Text.Json;
using PlayFab;
using PlayFab.AuthenticationModels;
using PfGameSaveUtil.Models;
using static PfGameSaveUtil.Utilities;
using static PfGameSaveUtil.LocalOperations;

namespace PfGameSaveUtil;

public static class CloudOperations
{
    public static async Task ResetAsync(string titleId, string secretKey, string playerId, bool force, bool doCloud, bool doLocal, string localPath)
    {
        var scope = (doCloud, doLocal) switch
        {
            (true, true) => "Cloud + Local",
            (true, false) => "Cloud",
            (false, true) => "Local",
            _ => "None"
        };

        Console.WriteLine($"PlayFab Game Save - Reset ({scope})");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine($"Player: {playerId}");
        Console.WriteLine();

        string[]? localFolders = null;
        if (doLocal)
        {
            localFolders = ResolveLocalFolders(titleId, localPath);
            if (localFolders != null && localFolders.Length > 0)
            {
                Console.WriteLine("Local folders to be deleted:");
                foreach (var folder in localFolders)
                {
                    var files = Directory.GetFiles(folder, "*", SearchOption.AllDirectories);
                    var totalSize = files.Sum(f => new FileInfo(f).Length);
                    Console.WriteLine($"  {folder} ({files.Length} files, {FormatSize(totalSize)})");
                }
            }
            Console.WriteLine();
        }

        if (!force)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            if (doCloud && doLocal)
                Console.WriteLine("WARNING: This will permanently delete ALL cloud AND local save data.");
            else if (doCloud)
                Console.WriteLine("WARNING: This will permanently delete ALL cloud save data for this player.");
            else
                Console.WriteLine("WARNING: This will permanently delete ALL local save data for this title.");
            Console.ResetColor();
            Console.Write("Are you sure you want to continue? [y/N] ");
            
            var response = Console.ReadLine()?.Trim().ToLowerInvariant();
            if (response != "y" && response != "yes")
            {
                Console.WriteLine("Reset cancelled.");
                return;
            }
            Console.WriteLine();
        }

        if (doCloud)
        {
            Console.WriteLine("Authenticating with PlayFab...");
            PlayFabSettings.staticSettings.TitleId = titleId;
            PlayFabSettings.staticSettings.DeveloperSecretKey = secretKey;

            var authRequest = new GetEntityTokenRequest();
            var authResult = await PlayFabAuthenticationAPI.GetEntityTokenAsync(authRequest);

            if (authResult.Error != null)
            {
                Console.Error.WriteLine($"Authentication failed: {authResult.Error.ErrorMessage}");
                return;
            }

            var entityToken = authResult.Result.EntityToken;
            Console.WriteLine($"Authenticated as: {authResult.Result.Entity?.Type} ({authResult.Result.Entity?.Id})");

            using var client = new GameSaveClient(titleId);
            client.SetEntityToken(entityToken);

            Console.WriteLine("\nResetting cloud save state...");
            await client.ResetCloudAsync(playerId, msg => Console.WriteLine($"  {msg}"));
        }

        if (doLocal && localFolders != null && localFolders.Length > 0)
        {
            Console.WriteLine("\nResetting local save state...");
            foreach (var folder in localFolders)
            {
                try
                {
                    Directory.Delete(folder, recursive: true);
                    Console.WriteLine($"  Deleted: {folder}");
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine($"  Failed to delete {folder}: {ex.Message}");
                }
            }
        }

        Console.WriteLine();
        Console.ForegroundColor = ConsoleColor.Green;
        Console.WriteLine($"✓ Reset complete ({scope})");
        Console.ResetColor();
    }

    public static async Task DownloadAllSavesAsync(string titleId, string secretKey, string playerId, string output, string? version)
    {
        Console.WriteLine($"PlayFab Game Save Downloader");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine($"Player: {playerId}");
        Console.WriteLine($"Output: {Path.GetFullPath(output)}");
        Console.WriteLine();

        Console.WriteLine("Authenticating with PlayFab...");
        PlayFabSettings.staticSettings.TitleId = titleId;
        PlayFabSettings.staticSettings.DeveloperSecretKey = secretKey;

        var authRequest = new GetEntityTokenRequest();
        var authResult = await PlayFabAuthenticationAPI.GetEntityTokenAsync(authRequest);

        if (authResult.Error != null)
        {
            Console.Error.WriteLine($"Authentication failed: {authResult.Error.ErrorMessage}");
            return;
        }

        var entityToken = authResult.Result.EntityToken;
        Console.WriteLine($"Authenticated as: {authResult.Result.Entity?.Type} ({authResult.Result.Entity?.Id})");

        using var client = new GameSaveClient(titleId);
        client.SetEntityToken(entityToken);

        Console.WriteLine("\nFetching manifests...");
        var (manifestsResponse, manifestsRaw) = await client.ListManifestsWithRawAsync(playerId);

        if (manifestsResponse.Data?.Manifests == null || manifestsResponse.Data.Manifests.Count == 0)
        {
            Console.WriteLine("No save manifests found for this player.");
            return;
        }

        var manifests = FilterManifests(manifestsResponse.Data.Manifests, includePendingDeletion: false)
            .OrderByDescending(m => m.Version).ToList();
        Console.WriteLine($"Found {manifests.Count} manifest(s):");
        foreach (var m in manifests)
        {
            Console.WriteLine($"  - Version {m.Version} (status: {m.Status ?? "unknown"}, finalized: {m.FinalizationTimestamp})");
        }

        // Get quota info
        QuotaData? quotaData = null;
        try
        {
            var quotaResponse = await client.GetQuotaForPlayerAsync(playerId);
            quotaData = quotaResponse.Data;
        }
        catch { /* non-fatal */ }

        // Fixed layout: clean extracted saves live under output/v{ver}/extracted/, and all
        // diagnostics live under a diag/ subfolder (root diag/ for the list responses, and a
        // per-version v{ver}/diag/ for that version's manifest, download details, extended
        // manifest, and raw chunk zips).
        Directory.CreateDirectory(output);
        var rootDiag = Path.Combine(output, "diag");
        Directory.CreateDirectory(rootDiag);

        // Raw ListManifests service response (no secrets) for diagnostics.
        await WriteRawJsonPrettyAsync(Path.Combine(rootDiag, "list-manifests.json"), manifestsRaw);

        // Write summary.json (summary list + quota)
        var listManifestObj = new
        {
            downloadTimestamp = DateTime.UtcNow.ToString("o"),
            titleId,
            playerId,
            quota = quotaData != null ? new
            {
                totalBytes = quotaData.TotalBytes,
                availableBytes = quotaData.AvailableBytes
            } : null,
            manifests = manifests.Select(m => new
            {
                version = m.Version,
                status = m.Status,
                creationTimestamp = m.CreationTimestamp?.ToString("o"),
                finalizationTimestamp = m.FinalizationTimestamp?.ToString("o")
            })
        };
        var listManifestPath = Path.Combine(rootDiag, "summary.json");
        await File.WriteAllTextAsync(listManifestPath, JsonSerializer.Serialize(listManifestObj, new JsonSerializerOptions { WriteIndented = true }));
        Console.WriteLine($"\nSaved: diag/summary.json");

        // Determine which versions to download
        List<ManifestInfo> toDownload;
        if (version != null)
        {
            var target = manifests.FirstOrDefault(m => m.Version == version);
            if (target == null)
            {
                Console.Error.WriteLine($"Version {version} not found.");
                return;
            }
            toDownload = new List<ManifestInfo> { target };
        }
        else
        {
            // Download all versions (create folder for each)
            toDownload = manifests;
        }

        foreach (var manifest in toDownload)
        {
            var ver = manifest.Version!;

            // Per-version layout: output/v{ver}/extracted/ holds the clean save files, and
            // output/v{ver}/diag/ holds all diagnostics for that version (manifest entry,
            // download details, extended manifest, and the raw chunk zips).
            var versionFolder = Path.Combine(output, $"v{ver}");
            var diagVersionFolder = Path.Combine(versionFolder, "diag");
            Directory.CreateDirectory(diagVersionFolder);

            // Capture the per-version manifest entry from the list response.
            await WriteJsonAsync(Path.Combine(diagVersionFolder, "manifest.json"), manifest);

            // Finalized → downloadable directly; PendingDeletion → downloadable via admin path.
            var isDownloadable = EnsureDownloadable(manifest);

            if (!isDownloadable)
            {
                // For non-downloadable versions (e.g. Initialized/Uploading/Quarantined),
                // write a status-only marker.
                var statusObj = new
                {
                    version = ver,
                    status = manifest.Status,
                    creationTimestamp = manifest.CreationTimestamp?.ToString("o"),
                    finalizationTimestamp = manifest.FinalizationTimestamp?.ToString("o"),
                    note = $"Version is {manifest.Status} — no files available for download"
                };
                await WriteJsonAsync(Path.Combine(diagVersionFolder, "status.json"), statusObj);
                Console.WriteLine($"\n[v{ver}] Status: {manifest.Status} (not downloadable)");
                continue;
            }

            Console.WriteLine($"\n[v{ver}] Downloading...");

            GetManifestDownloadDetailsResponse downloadDetails;
            string downloadDetailsRaw;
            try
            {
                (downloadDetails, downloadDetailsRaw) = await client.GetManifestDownloadDetailsWithRawAsync(playerId, ver);
            }
            catch (HttpRequestException ex)
            {
                Console.Error.WriteLine($"  Failed to get download details: {ex.Message}");
                continue;
            }

            var files = downloadDetails.Data?.Files ?? new List<FileDownloadInfo>();
            if (files.Count == 0)
            {
                Console.WriteLine($"  No files in manifest.");
                continue;
            }

            // Save the full download-details response, including the per-file SAS download URLs.
            await WriteRawJsonPrettyAsync(Path.Combine(diagVersionFolder, "download-details.json"), downloadDetailsRaw);

            var extendedManifestFile = files.FirstOrDefault(f =>
                f.FileName?.StartsWith("extended-") == true && f.FileName.EndsWith("-manifest.json"));

            if (extendedManifestFile?.DownloadUrl == null)
            {
                Console.Error.WriteLine($"  Extended manifest not found in download details.");
                continue;
            }

            // Download the extended manifest as raw bytes, save the true artifact, then parse.
            Console.WriteLine($"  Downloading extended manifest...");
            var extendedManifestBytes = await client.DownloadBlobAsync(extendedManifestFile.DownloadUrl);
            await WriteRawJsonPrettyAsync(Path.Combine(diagVersionFolder, "extended-manifest.json"), extendedManifestBytes);
            var extendedManifest = GameSaveClient.ParseExtendedManifest(extendedManifestBytes);

            // Download chunks into the diagnostics zips/ folder (raw zips are debug material).
            var zipsFolder = Path.Combine(diagVersionFolder, "zips");
            Directory.CreateDirectory(zipsFolder);

            var chunkFiles = files.Where(f => f.FileName != extendedManifestFile.FileName).ToList();
            Console.WriteLine($"  Downloading {chunkFiles.Count} chunk(s)...");

            var chunks = new Dictionary<string, byte[]>();
            var chunkIndex = 0;

            foreach (var file in chunkFiles)
            {
                if (file.FileName == null || file.DownloadUrl == null) continue;
                chunkIndex++;

                void ShowProgress(long bytesRead, long? totalBytes)
                {
                    var progress = totalBytes.HasValue
                        ? $"{FormatSize(bytesRead)} / {FormatSize(totalBytes.Value)}"
                        : FormatSize(bytesRead);
                    Console.Write($"\r  [{chunkIndex}/{chunkFiles.Count}] {file.FileName}: {progress}    ");
                }

                var data = await client.DownloadBlobAsync(file.DownloadUrl, file.FileName, ShowProgress);
                Console.WriteLine();

                // Save raw zip to zips/ folder
                await File.WriteAllBytesAsync(Path.Combine(zipsFolder, file.FileName), data);
                chunks[file.FileName] = data;
            }

            // Extract the clean save files into v{ver}/extracted/.
            var extractedFolder = Path.Combine(versionFolder, "extracted");
            Directory.CreateDirectory(extractedFolder);
            Console.WriteLine($"  Extracting to: v{ver}/extracted/");
            var extractor = new SaveExtractor(extractedFolder);
            await extractor.ExtractAsync(extendedManifest, chunks, msg => Console.WriteLine($"    {msg}"));
        }

        Console.WriteLine("\nDone!");
    }

    // Downloads a single manifest version and extracts it directly to the output folder.
    // If no version is specified, the latest finalized manifest is used.
    // When metadataPath is set, diagnostic material (raw service responses, the extended
    // manifest, and the raw chunk zips) is written there, keeping the output folder clean.
    public static async Task DownloadSingleSaveAsync(string titleId, string secretKey, string playerId, string output, string? version, string? metadataPath = null)
    {
        Console.WriteLine($"PlayFab Game Save Downloader");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine($"Player: {playerId}");
        Console.WriteLine($"Output: {Path.GetFullPath(output)}");
        Console.WriteLine();

        Console.WriteLine("Authenticating with PlayFab...");
        PlayFabSettings.staticSettings.TitleId = titleId;
        PlayFabSettings.staticSettings.DeveloperSecretKey = secretKey;

        var authRequest = new GetEntityTokenRequest();
        var authResult = await PlayFabAuthenticationAPI.GetEntityTokenAsync(authRequest);

        if (authResult.Error != null)
        {
            Console.Error.WriteLine($"Authentication failed: {authResult.Error.ErrorMessage}");
            return;
        }

        var entityToken = authResult.Result.EntityToken;
        Console.WriteLine($"Authenticated as: {authResult.Result.Entity?.Type} ({authResult.Result.Entity?.Id})");

        using var client = new GameSaveClient(titleId);
        client.SetEntityToken(entityToken);

        Console.WriteLine("\nFetching manifests...");
        var (manifestsResponse, manifestsRaw) = await client.ListManifestsWithRawAsync(playerId);

        if (manifestsResponse.Data?.Manifests == null || manifestsResponse.Data.Manifests.Count == 0)
        {
            Console.WriteLine("No save manifests found for this player.");
            return;
        }

        var manifests = FilterManifests(manifestsResponse.Data.Manifests, includePendingDeletion: false)
            .OrderByDescending(m => ParseVersion(m.Version)).ToList();
        Console.WriteLine($"Found {manifests.Count} manifest(s):");
        foreach (var m in manifests)
        {
            Console.WriteLine($"  - Version {m.Version} (status: {m.Status ?? "unknown"}, finalized: {m.FinalizationTimestamp})");
        }

        // Default selection uses Finalized manifests (directly downloadable). An explicit
        // --version may still target a PendingDeletion one, which is handled below.
        var downloadableManifests = manifests.Where(m => IsDownloadableStatus(m.Status)).ToList();

        if (downloadableManifests.Count == 0 && version == null)
        {
            Console.WriteLine("\nNo downloadable (Finalized) manifests found.");
            return;
        }

        var targetVersion = version ?? downloadableManifests.First().Version!;

        // Look up an explicit --version in the full (unfiltered) list so a PendingDeletion
        // target still produces the helpful "restore via Game Manager" message rather than
        // a misleading "not found".
        var allManifests = manifestsResponse.Data.Manifests;
        var targetManifest = allManifests.FirstOrDefault(m => m.Version == targetVersion);
        if (targetManifest == null)
        {
            Console.Error.WriteLine($"\nVersion {targetVersion} not found.");
            return;
        }
        // Finalized → download directly; PendingDeletion → not downloadable (logs why).
        if (!EnsureDownloadable(targetManifest))
        {
            Console.Error.WriteLine($"\nVersion {targetVersion} is not downloadable (status: {targetManifest.Status}).");
            return;
        }
        Console.WriteLine($"\nDownloading manifest version {targetVersion}...");

        // Prepare the diagnostics folder up front (if requested).
        if (!string.IsNullOrWhiteSpace(metadataPath))
        {
            Directory.CreateDirectory(metadataPath);
            Console.WriteLine($"Diagnostics folder: {Path.GetFullPath(metadataPath)}");

            // Raw ListManifests service response (no secrets) + the specific target entry.
            await WriteRawJsonPrettyAsync(Path.Combine(metadataPath, "list-manifests.json"), manifestsRaw);
            await WriteJsonAsync(Path.Combine(metadataPath, "manifest.json"), targetManifest);
        }

        var (downloadDetails, downloadDetailsRaw) = await client.GetManifestDownloadDetailsWithRawAsync(playerId, targetVersion);
        var files = downloadDetails.Data?.Files ?? new List<FileDownloadInfo>();

        if (files.Count == 0)
        {
            Console.WriteLine("No files in manifest.");
            return;
        }

        Console.WriteLine($"Manifest contains {files.Count} file(s)");

        var extendedManifestFile = files.FirstOrDefault(f =>
            f.FileName?.StartsWith("extended-") == true && f.FileName.EndsWith("-manifest.json"));

        if (extendedManifestFile?.DownloadUrl == null)
        {
            Console.Error.WriteLine("Extended manifest not found in download details.");
            return;
        }

        // Save the full download-details response, including the per-file SAS download URLs.
        if (!string.IsNullOrWhiteSpace(metadataPath))
        {
            await WriteRawJsonPrettyAsync(Path.Combine(metadataPath, "download-details.json"), downloadDetailsRaw);
        }

        Console.WriteLine("\nDownloading extended manifest...");
        var extendedManifestBytes = await client.DownloadBlobAsync(extendedManifestFile.DownloadUrl);
        var extendedManifest = GameSaveClient.ParseExtendedManifest(extendedManifestBytes);

        // Save the raw extended manifest (true service bytes) to diagnostics.
        if (!string.IsNullOrWhiteSpace(metadataPath))
        {
            await WriteRawJsonPrettyAsync(Path.Combine(metadataPath, "extended-manifest.json"), extendedManifestBytes);
        }

        Console.WriteLine("\nDownloading chunks...");
        var chunks = new Dictionary<string, byte[]>();
        var chunkFiles = files.Where(f => f.FileName != extendedManifestFile.FileName).ToList();
        var chunkIndex = 0;

        // Raw chunk zips are diagnostic material; keep them out of the clean output folder.
        string? zipsFolder = null;
        if (!string.IsNullOrWhiteSpace(metadataPath))
        {
            zipsFolder = Path.Combine(metadataPath, "zips");
            Directory.CreateDirectory(zipsFolder);
        }

        foreach (var file in chunkFiles)
        {
            if (file.FileName == null || file.DownloadUrl == null) continue;
            chunkIndex++;

            // Progress callback that updates the same line
            void ShowProgress(long bytesRead, long? totalBytes)
            {
                var progress = totalBytes.HasValue
                    ? $"{FormatSize(bytesRead)} / {FormatSize(totalBytes.Value)}"
                    : FormatSize(bytesRead);
                Console.Write($"\r  [{chunkIndex}/{chunkFiles.Count}] {file.FileName}: {progress}    ");
            }

            chunks[file.FileName] = await client.DownloadBlobAsync(file.DownloadUrl, file.FileName, ShowProgress);
            Console.WriteLine(); // Move to next line after download completes

            if (zipsFolder != null)
            {
                await File.WriteAllBytesAsync(Path.Combine(zipsFolder, file.FileName), chunks[file.FileName]);
            }
        }

        Console.WriteLine($"\nExtracting to: {Path.GetFullPath(output)}");
        var extractor = new SaveExtractor(output);
        await extractor.ExtractAsync(extendedManifest, chunks, msg => Console.WriteLine($"  {msg}"));

        if (!string.IsNullOrWhiteSpace(metadataPath))
        {
            Console.WriteLine($"\nDiagnostics written to: {Path.GetFullPath(metadataPath)}");
        }

        Console.WriteLine("\nDone!");
    }

    /// <summary>
    /// Uploads a local folder as a new save version: compresses the files, builds the
    /// extended manifest, and runs the InitializeManifest -> InitiateUpload -> PUT blobs
    /// -> FinalizeManifest sequence.
    /// </summary>
    public static async Task UploadSaveAsync(
        string titleId,
        string secretKey,
        string playerId,
        string sourceFolder,
        string? description,
        bool force,
        long maxZipBytes = SaveUploader.DefaultMaxZipBytes)
    {
        Console.WriteLine("PlayFab Game Save Uploader");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine($"Player: {playerId}");
        Console.WriteLine($"Source: {Path.GetFullPath(sourceFolder)}");
        Console.WriteLine();

        if (!Directory.Exists(sourceFolder))
        {
            Console.Error.WriteLine($"Error: Source folder not found: {sourceFolder}");
            return;
        }

        Console.WriteLine("Authenticating with PlayFab...");
        PlayFabSettings.staticSettings.TitleId = titleId;
        PlayFabSettings.staticSettings.DeveloperSecretKey = secretKey;

        var authResult = await PlayFabAuthenticationAPI.GetEntityTokenAsync(new GetEntityTokenRequest());
        if (authResult.Error != null)
        {
            Console.Error.WriteLine($"Authentication failed: {authResult.Error.ErrorMessage}");
            return;
        }

        using var client = new GameSaveClient(titleId);
        client.SetEntityToken(authResult.Result.EntityToken);
        Console.WriteLine($"Authenticated as: {authResult.Result.Entity?.Type} ({authResult.Result.Entity?.Id})");

        // Determine the new version (and its base) from the existing manifests.
        Console.WriteLine("\nFetching existing manifests...");
        var manifestsResponse = await client.ListManifestsAsync(playerId);
        var manifests = manifestsResponse.Data?.Manifests ?? new List<ManifestInfo>();

        ulong baseVersion = manifests
            .Where(m => string.Equals(m.Status, "Finalized", StringComparison.OrdinalIgnoreCase))
            .Select(m => ParseVersion(m.Version))
            .DefaultIfEmpty(0UL)
            .Max();

        ulong newVersion = 0;
        if (ulong.TryParse(manifestsResponse.Data?.NextAvailableVersion, out var nav) && nav > 0)
        {
            newVersion = nav;
        }
        if (newVersion == 0)
        {
            ulong maxVisible = manifests.Select(m => ParseVersion(m.Version)).DefaultIfEmpty(0UL).Max();
            newVersion = maxVisible + 1;
        }
        if (newVersion <= baseVersion)
        {
            newVersion = baseVersion + 1;
        }

        Console.WriteLine($"  Base version: {baseVersion}");
        Console.WriteLine($"  New version:  {newVersion}");

        // Package the folder into compressed chunk(s) + extended manifest.
        Console.WriteLine("\nCompressing files...");
        var package = SaveUploader.BuildPackage(sourceFolder, newVersion, msg => Console.WriteLine(msg), maxZipBytes);
        Console.WriteLine($"  {package.FileCount} file(s), {FormatSize(package.TotalUncompressedBytes)} uncompressed " +
                          $"-> {FormatSize(package.TotalCompressedBytes)} compressed in {package.Chunks.Count} chunk(s)");

        // Quota check (best-effort).
        try
        {
            var quota = await client.GetQuotaForPlayerAsync(playerId);
            if (long.TryParse(quota.Data?.AvailableBytes, out var available))
            {
                Console.WriteLine($"  Available quota: {FormatSize(available)}");
                if (package.TotalCompressedBytes + package.ExtendedManifestBytes.LongLength > available)
                {
                    Console.Error.WriteLine("Error: Upload exceeds available quota.");
                    return;
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"  (Quota check skipped: {ex.Message})");
        }

        if (!force)
        {
            Console.Write($"\nUpload version {newVersion} for this player? [y/N] ");
            var answer = Console.ReadLine();
            if (!string.Equals(answer?.Trim(), "y", StringComparison.OrdinalIgnoreCase))
            {
                Console.WriteLine("Aborted.");
                return;
            }
        }

        // 1. InitializeManifest — reserve the new version.
        Console.WriteLine("\nInitializing manifest...");
        var metadata = new ManifestMetadata
        {
            DeviceId = GetDeviceId(),
            DeviceName = Environment.MachineName,
            DeviceType = "Windows",
            DeviceVersion = Environment.OSVersion.Version.ToString()
        };
        await client.InitializeManifestAsync(playerId, newVersion.ToString(), baseVersion.ToString(), metadata);

        // 2. InitiateUpload — request blob URLs for all chunks + extended manifest.
        Console.WriteLine("Requesting upload URLs...");
        var fileNames = package.Chunks.Select(c => c.FileName)
            .Append(package.ExtendedManifestFileName)
            .ToArray();
        var initiate = await client.InitiateUploadAsync(playerId, newVersion.ToString(), fileNames);
        var allocated = initiate.Data?.Files ?? new List<AllocatedFile>();

        string? UrlFor(string name) =>
            allocated.FirstOrDefault(f => f.FileName == name)?.UploadUrl;

        var manifestUrl = UrlFor(package.ExtendedManifestFileName);
        if (string.IsNullOrEmpty(manifestUrl) || package.Chunks.Any(c => string.IsNullOrEmpty(UrlFor(c.FileName))))
        {
            Console.Error.WriteLine("Error: Service did not return upload URLs for all files.");
            return;
        }

        // 3. PUT the blobs.
        var chunkIndex = 0;
        foreach (var chunk in package.Chunks)
        {
            chunkIndex++;
            Console.WriteLine($"Uploading chunk {chunkIndex}/{package.Chunks.Count} ({FormatSize(chunk.Bytes.LongLength)})...");
            await client.UploadBlobAsync(UrlFor(chunk.FileName)!, chunk.Bytes);
        }
        Console.WriteLine("Uploading extended manifest...");
        await client.UploadBlobAsync(manifestUrl, package.ExtendedManifestBytes);

        // 4. FinalizeManifest — make the version downloadable.
        Console.WriteLine("Finalizing manifest...");
        var filesToFinalize = package.Chunks
            .Select(c => (c.FileName, c.Bytes.LongLength))
            .Append((package.ExtendedManifestFileName, package.ExtendedManifestBytes.LongLength))
            .ToArray();
        var descriptionBase64 = Convert.ToBase64String(Encoding.UTF8.GetBytes(description ?? ""));
        await client.FinalizeManifestAsync(
            playerId,
            newVersion.ToString(),
            filesToFinalize,
            descriptionBase64,
            markBaseAsKnownGood: baseVersion > 0,
            force: false);

        Console.WriteLine($"\nDone! Uploaded and finalized version {newVersion}.");
    }

    private static ulong ParseVersion(string? version) =>
        ulong.TryParse(version, out var v) ? v : 0UL;

    // The GameSave service only returns download URLs (GetManifestDownloadDetails) for
    // Finalized manifests. PendingDeletion is listed (ListManifests is called with
    // IncludeUnavailable=true) but is not downloadable; it must be restored to Finalized
    // via the PlayFab Game Manager first — see EnsureDownloadable.
    private static bool IsDownloadableStatus(string? status) =>
        string.Equals(status, "Finalized", StringComparison.OrdinalIgnoreCase);

    private static bool IsPendingDeletion(string? status) =>
        string.Equals(status, "PendingDeletion", StringComparison.OrdinalIgnoreCase);

    // PendingDeletion manifests cannot be downloaded, so they are hidden by default.
    // Only the `info --pending-delete` path opts to include them.
    private static List<ManifestInfo> FilterManifests(IEnumerable<ManifestInfo> manifests, bool includePendingDeletion)
    {
        var list = manifests.ToList();
        if (includePendingDeletion)
        {
            return list;
        }
        return list.Where(m => !IsPendingDeletion(m.Status)).ToList();
    }

    // Returns true if the manifest can be downloaded. Finalized manifests download via the
    // normal path. PendingDeletion manifests are downloadable too via the admin path
    // (GetManifestDownloadDetails authorizes title-entity callers for any-state manifests);
    // ListManifests already surfaces them (IncludeUnavailable=true). Other transient states
    // (Initialized/Uploading/Quarantined) have no downloadable files.
    private static bool EnsureDownloadable(ManifestInfo manifest)
    {
        if (IsDownloadableStatus(manifest.Status) || IsPendingDeletion(manifest.Status))
        {
            return true;
        }
        return false;
    }

    // Writes an object as indented JSON to the given path (diagnostics helper).
    private static Task WriteJsonAsync(string path, object obj) =>
        File.WriteAllTextAsync(path, JsonSerializer.Serialize(obj, new JsonSerializerOptions { WriteIndented = true }));

    // Writes raw JSON (as returned by the service, often minified) re-formatted with indentation
    // so it's readable in any JSON viewer. Falls back to the original text if it can't be parsed.
    private static async Task WriteRawJsonPrettyAsync(string path, string rawJson)
    {
        try
        {
            using var doc = JsonDocument.Parse(rawJson);
            var pretty = JsonSerializer.Serialize(doc.RootElement, new JsonSerializerOptions { WriteIndented = true });
            await File.WriteAllTextAsync(path, pretty);
        }
        catch (JsonException)
        {
            await File.WriteAllTextAsync(path, rawJson);
        }
    }

    // Same as WriteRawJsonPrettyAsync but for raw JSON bytes (e.g. a downloaded blob).
    private static Task WriteRawJsonPrettyAsync(string path, byte[] rawJsonBytes) =>
        WriteRawJsonPrettyAsync(path, Encoding.UTF8.GetString(rawJsonBytes));

    // The manifest description is stored base64-encoded (UTF-8). Returns the decoded text,
    // or null if the manifest has no description. If the value isn't valid base64, the raw
    // value is returned so nothing is silently lost.
    private static string? DecodeManifestDescription(string? descriptionBase64)
    {
        if (string.IsNullOrEmpty(descriptionBase64))
        {
            return null;
        }

        try
        {
            var decoded = Encoding.UTF8.GetString(Convert.FromBase64String(descriptionBase64));
            return string.IsNullOrWhiteSpace(decoded) ? null : decoded;
        }
        catch (FormatException)
        {
            return descriptionBase64;
        }
    }

    private static string GetDeviceId()
    {
        // Stable per-machine identifier (informational metadata only).
        var bytes = System.Security.Cryptography.MD5.HashData(Encoding.UTF8.GetBytes(Environment.MachineName));
        return new Guid(bytes).ToString();
    }

    private static async Task<string> BuildInfoJsonAsync(GameSaveClient client, string playerId, List<ManifestInfo> manifests)
    {
        var manifestDetails = new List<ManifestDetail>();
        var healthIssues = new List<string>();

        foreach (var manifest in manifests.OrderByDescending(m => m.Version))
        {
            var detail = new ManifestDetail
            {
                Name = manifest.Version ?? "unnamed",
                Version = manifest.Version ?? "?",
                BaseVersion = manifest.BaseVersion,
                Status = manifest.Status ?? "unknown",
                KnownGood = manifest.KnownGood,
                Created = manifest.CreationTimestamp?.ToString("o"),
                LastModified = manifest.FinalizationTimestamp?.ToString("o"),
                Description = DecodeManifestDescription(manifest.ManifestDescription)
            };

            if (string.Equals(manifest.Status, "Quarantined", StringComparison.OrdinalIgnoreCase))
            {
                healthIssues.Add($"Manifest v{detail.Version} is quarantined");
            }

            if (IsDownloadableStatus(manifest.Status))
            {
                try
                {
                    var downloadDetails = await client.GetManifestDownloadDetailsAsync(playerId, manifest.Version!);
                    var files = downloadDetails.Data?.Files ?? new List<FileDownloadInfo>();

                    var extManifestFile = files.FirstOrDefault(f =>
                        f.FileName?.StartsWith("extended-") == true && f.FileName.EndsWith("-manifest.json"));

                    if (extManifestFile?.DownloadUrl != null)
                    {
                        var extManifest = await client.DownloadExtendedManifestAsync(extManifestFile.DownloadUrl);
                        var manifestFiles = extManifest.V1?.Files ?? new List<ManifestFile>();

                        detail.FileCount = manifestFiles.Sum(f => f.Extract?.Count(e => !e.SkipFile) ?? 0);
                        detail.CompressedSize = manifestFiles.Sum(f => f.CompressSize);
                        detail.UncompressedSize = manifestFiles.Sum(f => f.Extract?.Where(e => !e.SkipFile).Sum(e => e.Size) ?? 0);
                        detail.FolderCount = CountFolders(extManifest.V1?.Folders);
                    }

                    detail.ChunkCount = files.Count(f => !f.FileName?.Contains("manifest") ?? false);
                }
                catch
                {
                    // Ignore errors - we already have the manifest info from the download
                }
            }

            manifestDetails.Add(detail);
        }

        // Get quota
        QuotaInfo? quota = null;
        try
        {
            var quotaResponse = await client.GetQuotaForPlayerAsync(playerId);
            if (quotaResponse.Data != null)
            {
                quota = new QuotaInfo
                {
                    TotalBytes = long.TryParse(quotaResponse.Data.TotalBytes, out var total) ? total : 0,
                    AvailableBytes = long.TryParse(quotaResponse.Data.AvailableBytes, out var avail) ? avail : 0
                };
            }
        }
        catch
        {
            // Ignore quota errors
        }

        var output = new
        {
            playerId,
            timestamp = DateTime.UtcNow.ToString("o"),
            summary = new
            {
                manifestCount = manifestDetails.Count,
                statusBreakdown = manifestDetails.GroupBy(m => m.Status).ToDictionary(g => g.Key, g => g.Count()),
                totalFiles = manifestDetails.Sum(m => m.FileCount),
                totalChunks = manifestDetails.Sum(m => m.ChunkCount),
                totalCompressedBytes = manifestDetails.Sum(m => m.CompressedSize),
                totalUncompressedBytes = manifestDetails.Sum(m => m.UncompressedSize)
            },
            quota = quota != null ? new
            {
                totalBytes = quota.TotalBytes,
                usedBytes = quota.UsedBytes,
                availableBytes = quota.AvailableBytes,
                usedPercent = quota.UsedPercent
            } : null,
            manifests = manifestDetails.Select(m => new
            {
                name = m.Name,
                version = m.Version,
                status = m.Status,
                fileCount = m.FileCount,
                chunkCount = m.ChunkCount,
                compressedBytes = m.CompressedSize,
                uncompressedBytes = m.UncompressedSize,
                lastModified = m.LastModified
            }),
            healthIssues
        };

        return JsonSerializer.Serialize(output, new JsonSerializerOptions { WriteIndented = true });
    }

    public static async Task ShowInfoAsync(string titleId, string secretKey, string playerId, bool verbose, bool jsonOutput, bool includePendingDeletion)
    {
        PlayFabSettings.staticSettings.TitleId = titleId;
        PlayFabSettings.staticSettings.DeveloperSecretKey = secretKey;

        var authResult = await PlayFabAuthenticationAPI.GetEntityTokenAsync(new GetEntityTokenRequest());
        if (authResult.Error != null)
        {
            Console.Error.WriteLine($"Authentication failed: {authResult.Error.ErrorMessage}");
            return;
        }

        using var client = new GameSaveClient(titleId);
        client.SetEntityToken(authResult.Result.EntityToken);

        var manifestsResponse = await client.ListManifestsAsync(playerId);
        var manifests = FilterManifests(manifestsResponse.Data?.Manifests ?? new List<ManifestInfo>(), includePendingDeletion);

        // Get quota information
        var quotaResponse = await client.GetQuotaForPlayerAsync(playerId);
        var quotaData = quotaResponse.Data;

        var manifestDetails = new List<ManifestDetail>();
        var healthIssues = new List<string>();

        foreach (var manifest in manifests.OrderByDescending(m => m.Version))
        {
            var detail = new ManifestDetail
            {
                Name = manifest.Version ?? "unnamed",
                Version = manifest.Version ?? "?",
                BaseVersion = manifest.BaseVersion,
                Status = manifest.Status ?? "unknown",
                KnownGood = manifest.KnownGood,
                Created = manifest.CreationTimestamp?.ToString("o"),
                LastModified = manifest.FinalizationTimestamp?.ToString("o")
            };

            detail.Description = DecodeManifestDescription(manifest.ManifestDescription);

            if (string.Equals(manifest.Status, "Quarantined", StringComparison.OrdinalIgnoreCase))
            {
                healthIssues.Add($"Manifest v{detail.Version} is quarantined");
            }

            if (IsDownloadableStatus(manifest.Status))
            {
                try
                {
                    var downloadDetails = await client.GetManifestDownloadDetailsAsync(playerId, manifest.Version!);
                    var files = downloadDetails.Data?.Files ?? new List<FileDownloadInfo>();

                    var extManifestFile = files.FirstOrDefault(f =>
                        f.FileName?.StartsWith("extended-") == true && f.FileName.EndsWith("-manifest.json"));

                    if (extManifestFile?.DownloadUrl != null)
                    {
                        var extManifest = await client.DownloadExtendedManifestAsync(extManifestFile.DownloadUrl);
                        var manifestFiles = extManifest.V1?.Files ?? new List<ManifestFile>();
                        
                        // Only count files that are not marked as skipped
                        detail.FileCount = manifestFiles.Sum(f => f.Extract?.Count(e => !e.SkipFile) ?? 0);
                        detail.CompressedSize = manifestFiles.Sum(f => f.CompressSize);
                        detail.UncompressedSize = manifestFiles.Sum(f => f.Extract?.Where(e => !e.SkipFile).Sum(e => e.Size) ?? 0);
                        detail.FolderCount = CountFolders(extManifest.V1?.Folders);
                    }

                    detail.ChunkCount = files.Count(f => !f.FileName?.Contains("manifest") ?? false);
                }
                catch (HttpRequestException ex)
                {
                    // Network or API error - log but continue with other manifests
                    if (!jsonOutput)
                    {
                        Console.Error.WriteLine($"Warning: Failed to get details for manifest {manifest.Version}: {ex.Message}");
                    }
                }
                catch (InvalidOperationException ex)
                {
                    // Parse error - log but continue
                    if (!jsonOutput)
                    {
                        Console.Error.WriteLine($"Warning: Failed to parse manifest {manifest.Version}: {ex.Message}");
                    }
                }
            }

            manifestDetails.Add(detail);
        }

        // Convert quota data
        QuotaInfo? quota = null;
        if (quotaData != null)
        {
            quota = new QuotaInfo
            {
                TotalBytes = long.TryParse(quotaData.TotalBytes, out var total) ? total : 0,
                AvailableBytes = long.TryParse(quotaData.AvailableBytes, out var avail) ? avail : 0
            };
        }

        if (jsonOutput)
        {
            InfoFormatters.OutputJson(playerId, manifestDetails, healthIssues, quota);
        }
        else if (verbose)
        {
            InfoFormatters.OutputVerbose(playerId, manifestDetails, healthIssues, quota);
        }
        else
        {
            InfoFormatters.OutputSummary(playerId, manifestDetails, healthIssues, quota);
        }
    }

    public static async Task CompareWithLocalAsync(string titleId, string secretKey, string playerId, string localPath, string? version)
    {
        Console.WriteLine("PlayFab Game Save - Cloud vs Local Comparison");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine($"Player: {playerId}");
        Console.WriteLine();

        var folders = ResolveLocalFolders(titleId, localPath);
        if (folders == null || folders.Length == 0)
        {
            return;
        }
        var pgsFolder = folders[0];

        PlayFabSettings.staticSettings.TitleId = titleId;
        PlayFabSettings.staticSettings.DeveloperSecretKey = secretKey;

        var authResult = await PlayFabAuthenticationAPI.GetEntityTokenAsync(new GetEntityTokenRequest());
        if (authResult.Error != null)
        {
            Console.Error.WriteLine($"Authentication failed: {authResult.Error.ErrorMessage}");
            return;
        }

        using var client = new GameSaveClient(titleId);
        client.SetEntityToken(authResult.Result.EntityToken);

        Console.WriteLine("\nFetching cloud manifests...");
        var manifestsResponse = await client.ListManifestsAsync(playerId);
        var manifests = manifestsResponse.Data?.Manifests ?? new List<ManifestInfo>();

        var finalizedManifests = manifests
            .Where(m => string.Equals(m.Status, "Finalized", StringComparison.OrdinalIgnoreCase))
            .OrderByDescending(m => m.Version)
            .ToList();

        if (finalizedManifests.Count == 0)
        {
            Console.WriteLine("No finalized cloud manifests found.");
            return;
        }

        var targetVersion = version ?? finalizedManifests.First().Version!;
        Console.WriteLine($"Comparing cloud version: {targetVersion}");

        var downloadDetails = await client.GetManifestDownloadDetailsAsync(playerId, targetVersion);
        var files = downloadDetails.Data?.Files ?? new List<FileDownloadInfo>();

        var extManifestFile = files.FirstOrDefault(f =>
            f.FileName?.StartsWith("extended-") == true && f.FileName.EndsWith("-manifest.json"));

        if (extManifestFile?.DownloadUrl == null)
        {
            Console.Error.WriteLine("Cloud extended manifest not found.");
            return;
        }

        var cloudManifest = await client.DownloadExtendedManifestAsync(extManifestFile.DownloadUrl);

        var localManifestPath = Path.Combine(pgsFolder, $"extended-{targetVersion}-manifest.json");
        ExtendedManifest? localManifest = null;
        string? localVersion = null;

        if (File.Exists(localManifestPath))
        {
            localVersion = targetVersion;
            var localJson = await File.ReadAllTextAsync(localManifestPath);
            localManifest = JsonSerializer.Deserialize<ExtendedManifest>(localJson);
        }
        else
        {
            var localManifests = Directory.GetFiles(pgsFolder, "extended-*-manifest.json");
            if (localManifests.Length > 0)
            {
                var latestLocal = localManifests
                    .Select(p => new { Path = p, Version = Path.GetFileName(p).Replace("extended-", "").Replace("-manifest.json", "") })
                    .OrderByDescending(x => int.TryParse(x.Version, out var v) ? v : 0)
                    .First();
                
                localVersion = latestLocal.Version;
                var localJson = await File.ReadAllTextAsync(latestLocal.Path);
                localManifest = JsonSerializer.Deserialize<ExtendedManifest>(localJson);
                Console.WriteLine($"Local version found: {localVersion} (differs from cloud)");
            }
        }

        Console.WriteLine();
        Console.WriteLine("=" + new string('=', 70));
        Console.WriteLine("                              COMPARISON RESULTS");
        Console.WriteLine("=" + new string('=', 70));
        Console.WriteLine();

        Console.WriteLine("Versions:");
        Console.WriteLine($"  Cloud:  {targetVersion}");
        if (localVersion == null)
        {
            Console.WriteLine("  Local:  N/A (comparing files directly)");
        }
        else if (localVersion != targetVersion)
        {
            Console.WriteLine($"  Local:  {localVersion}");
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("  ⚠ Version mismatch!");
            Console.ResetColor();
        }
        else
        {
            Console.WriteLine($"  Local:  {localVersion}");
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("  ✓ Versions match");
            Console.ResetColor();
        }
        Console.WriteLine();

        var cloudFiles = InfoFormatters.BuildFileList(cloudManifest, "cloud");
        var localFiles = localManifest != null ? InfoFormatters.BuildFileList(localManifest, "local") : new Dictionary<string, (long Size, DateTime? Modified)>();

        var currentFolder = Path.Combine(pgsFolder, "current");
        var diskRoot = Directory.Exists(currentFolder) ? currentFolder : pgsFolder;
        
        var diskFiles = new Dictionary<string, (long Size, DateTime Modified)>();
        foreach (var file in Directory.GetFiles(diskRoot, "*", SearchOption.AllDirectories))
        {
            var fileName = Path.GetFileName(file);
            if (fileName.EndsWith("-manifest.json") || fileName.EndsWith(".json"))
                continue;
                
            var relativePath = Path.GetRelativePath(diskRoot, file);
            
            // Skip cloudsync folder (contains downloaded manifests)
            if (relativePath.StartsWith("cloudsync" + Path.DirectorySeparatorChar) || relativePath == "cloudsync")
                continue;
            
            // Skip chunk files (named as version numbers in subfolders like "143\file.zip")
            if (!string.IsNullOrEmpty(relativePath) && 
                relativePath.Length > 0 &&
                char.IsDigit(relativePath[0]) && 
                relativePath.Contains(Path.DirectorySeparatorChar))
                continue;
                
            var fi = new FileInfo(file);
            diskFiles[relativePath] = (fi.Length, fi.LastWriteTimeUtc);
        }

        Console.WriteLine("Files:");
        Console.WriteLine($"  Cloud manifest:  {cloudFiles.Count} files");
        Console.WriteLine($"  Local manifest:  {localFiles.Count} files");
        Console.WriteLine($"  Local disk:      {diskFiles.Count} files");
        Console.WriteLine();

        var allFiles = cloudFiles.Keys.Union(localFiles.Keys).Union(diskFiles.Keys).OrderBy(f => f).ToList();
        
        var matchCount = 0;
        var cloudOnly = new List<string>();
        var localOnly = new List<string>();
        var sizeMismatch = new List<(string File, long CloudSize, long LocalSize)>();

        foreach (var file in allFiles)
        {
            var inCloud = cloudFiles.TryGetValue(file, out var cloudInfo);
            var onDisk = diskFiles.TryGetValue(file, out var diskInfo);

            if (inCloud && onDisk)
            {
                if (cloudInfo.Size == diskInfo.Size)
                {
                    matchCount++;
                }
                else
                {
                    sizeMismatch.Add((File: file, CloudSize: cloudInfo.Size, LocalSize: diskInfo.Size));
                }
            }
            else if (inCloud && !onDisk)
            {
                cloudOnly.Add(file);
            }
            else if (!inCloud && onDisk)
            {
                localOnly.Add(file);
            }
        }

        Console.WriteLine("Comparison:");
        if (matchCount > 0)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"  ✓ {matchCount} files match");
            Console.ResetColor();
        }

        if (cloudOnly.Count > 0)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine($"  ⚠ {cloudOnly.Count} files in cloud only:");
            Console.ResetColor();
            foreach (var f in cloudOnly.Take(10))
                Console.WriteLine($"      + {f}");
            if (cloudOnly.Count > 10)
                Console.WriteLine($"      ... and {cloudOnly.Count - 10} more");
        }

        if (localOnly.Count > 0)
        {
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.WriteLine($"  ℹ {localOnly.Count} files on local disk only:");
            Console.ResetColor();
            foreach (var f in localOnly.Take(10))
                Console.WriteLine($"      - {f}");
            if (localOnly.Count > 10)
                Console.WriteLine($"      ... and {localOnly.Count - 10} more");
        }

        if (sizeMismatch.Count > 0)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"  ✗ {sizeMismatch.Count} files with size mismatch:");
            Console.ResetColor();
            foreach (var (file, cloudSize, localSize) in sizeMismatch.Take(10))
                Console.WriteLine($"      {file}: cloud={FormatSize(cloudSize)}, local={FormatSize(localSize)}");
            if (sizeMismatch.Count > 10)
                Console.WriteLine($"      ... and {sizeMismatch.Count - 10} more");
        }

        if (cloudOnly.Count == 0 && localOnly.Count == 0 && sizeMismatch.Count == 0 && matchCount > 0)
        {
            Console.WriteLine();
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("  ✓ Cloud and local are in sync!");
            Console.ResetColor();
        }

        Console.WriteLine();
    }
}
