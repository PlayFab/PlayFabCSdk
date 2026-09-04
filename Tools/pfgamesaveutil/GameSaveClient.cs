using System.Net;
using System.Net.Http.Headers;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using PfGameSaveUtil.Models;

namespace PfGameSaveUtil;

/// <summary>
/// Client for PlayFab Game Save REST APIs.
/// These endpoints are not in the C# SDK, so we call them directly.
/// </summary>
public class GameSaveClient : IDisposable
{
    // The Game Save service is inconsistent about number vs string representation
    // (e.g. int64 fields are serialized as strings). Allow numeric properties to be
    // read from JSON strings so deserialization tolerates either form.
    private static readonly JsonSerializerOptions s_responseOptions = new()
    {
        NumberHandling = JsonNumberHandling.AllowReadingFromString,
    };

    /// <summary>
    /// Shared client for blob (SAS URL) uploads. Deliberately has NO PlayFab default
    /// headers (e.g. X-EntityToken) so the PlayFab entity token is never sent to the
    /// storage endpoint when PUTting to a blob SAS URL.
    /// </summary>
    private static readonly HttpClient s_blobClient = new()
    {
        Timeout = Timeout.InfiniteTimeSpan // We handle progress/cancellation manually
    };

    private readonly HttpClient _httpClient;
    private readonly string _titleId;
    private string? _entityToken;

    public GameSaveClient(string titleId)
    {
        _titleId = titleId;
        _httpClient = new HttpClient
        {
            BaseAddress = new Uri($"https://{titleId}.playfabapi.com/"),
            Timeout = Timeout.InfiniteTimeSpan // We handle progress/cancellation manually
        };
    }

    public void SetEntityToken(string entityToken)
    {
        _entityToken = entityToken;
        _httpClient.DefaultRequestHeaders.Remove("X-EntityToken");
        _httpClient.DefaultRequestHeaders.Add("X-EntityToken", entityToken);
    }

    /// <summary>
    /// Lists all save manifests for a player.
    /// </summary>
    public async Task<ListManifestsResponse> ListManifestsAsync(string playerEntityId)
        => (await ListManifestsWithRawAsync(playerEntityId)).Parsed;

    /// <summary>
    /// Lists all save manifests for a player, also returning the raw service response JSON
    /// for diagnostics capture (the ListManifests response contains no secrets).
    /// </summary>
    public async Task<(ListManifestsResponse Parsed, string Raw)> ListManifestsWithRawAsync(string playerEntityId)
    {
        var request = new
        {
            Entity = new
            {
                Id = playerEntityId,
                Type = "title_player_account"
            },
            // Always include unavailable manifests (e.g. PendingDeletion) so they can be
            // inspected and downloaded by this tool.
            IncludeUnavailable = true
        };

        var raw = await PostRawAsync("/GameSave/ListManifests", request);
        var parsed = JsonSerializer.Deserialize<ListManifestsResponse>(raw, s_responseOptions)
            ?? throw new InvalidOperationException("Failed to deserialize response from /GameSave/ListManifests");
        return (parsed, raw);
    }

    /// <summary>
    /// Gets download URLs for all files in a manifest version.
    /// </summary>
    public async Task<GetManifestDownloadDetailsResponse> GetManifestDownloadDetailsAsync(
        string playerEntityId,
        string manifestVersion)
        => (await GetManifestDownloadDetailsWithRawAsync(playerEntityId, manifestVersion)).Parsed;

    /// <summary>
    /// Gets download URLs for all files in a manifest version, also returning the raw service
    /// response JSON (which includes the per-file SAS download URLs) for diagnostics capture.
    /// </summary>
    public async Task<(GetManifestDownloadDetailsResponse Parsed, string Raw)> GetManifestDownloadDetailsWithRawAsync(
        string playerEntityId,
        string manifestVersion)
    {
        var request = new
        {
            Entity = new
            {
                Id = playerEntityId,
                Type = "title_player_account"
            },
            Version = manifestVersion,
            // Admin download: lets a title-entity caller obtain download URLs for a manifest in
            // any state (e.g. PendingDeletion), not just Finalized. MainServer authorizes this
            // based on the calling entity being a `title` entity. During the service rollout this
            // can intermittently return GameSaveManifestVersionNotFinalized (20305) when a request
            // lands on a not-yet-updated instance, so we retry on that code.
            IsAdminCaller = true
        };

        var raw = await PostRawAsync(
            "/GameSave/GetManifestDownloadDetails", request,
            retryableGameSaveErrorCodes: s_manifestNotFinalizedRetryCodes);
        var parsed = JsonSerializer.Deserialize<GetManifestDownloadDetailsResponse>(raw, s_responseOptions)
            ?? throw new InvalidOperationException("Failed to deserialize response from /GameSave/GetManifestDownloadDetails");
        return (parsed, raw);
    }

    // GameSaveManifestVersionNotFinalized (20305) — retried for admin PendingDeletion downloads
    // to ride through the MainServer admin-download rollout (some instances not yet updated).
    private static readonly int[] s_manifestNotFinalizedRetryCodes = { 20305 };

    /// <summary>
    /// Deletes a specific manifest version.
    /// </summary>
    public async Task DeleteManifestAsync(string playerEntityId, string version)
    {
        var request = new
        {
            Entity = new
            {
                Id = playerEntityId,
                Type = "title_player_account"
            },
            Version = version
        };

        await PostAsync<object>("/GameSave/DeleteManifest", request);
    }

    /// <summary>
    /// Gets quota information for a player.
    /// </summary>
    public async Task<GetQuotaResponse> GetQuotaForPlayerAsync(string playerEntityId)
    {
        var request = new
        {
            Entity = new
            {
                Id = playerEntityId,
                Type = "title_player_account"
            }
        };

        return await PostAsync<GetQuotaResponse>("/GameSave/GetQuotaForPlayer", request);
    }

    /// <summary>
    /// Initializes a new manifest version (transitions to "Initialized" state).
    /// </summary>
    public async Task<InitializeManifestResponse> InitializeManifestAsync(
        string playerEntityId,
        string version,
        string baseVersion,
        ManifestMetadata metadata)
    {
        var request = new
        {
            Entity = new
            {
                Id = playerEntityId,
                Type = "title_player_account"
            },
            Version = version,
            BaseVersion = baseVersion,
            Metadata = new
            {
                metadata.DeviceId,
                metadata.DeviceName,
                metadata.DeviceType,
                metadata.DeviceVersion
            }
        };

        return await PostAsync<InitializeManifestResponse>("/GameSave/InitializeManifest", request);
    }

    /// <summary>
    /// Requests blob upload URLs for the given files (transitions to "Uploading" state).
    /// </summary>
    public async Task<InitiateUploadResponse> InitiateUploadAsync(
        string playerEntityId,
        string version,
        IEnumerable<string> fileNames)
    {
        var request = new
        {
            Entity = new
            {
                Id = playerEntityId,
                Type = "title_player_account"
            },
            Version = version,
            Files = fileNames.Select(f => new { FileName = f }).ToList()
        };

        return await PostAsync<InitiateUploadResponse>("/GameSave/InitiateUpload", request);
    }

    /// <summary>
    /// Uploads file bytes to a blob storage SAS URL using a PUT (BlockBlob).
    /// </summary>
    public async Task UploadBlobAsync(string url, byte[] content, Action<long, long>? onProgress = null)
    {
        using var requestContent = new ByteArrayContent(content);
        requestContent.Headers.Add("x-ms-blob-type", "BlockBlob");

        using var request = new HttpRequestMessage(HttpMethod.Put, url) { Content = requestContent };

        HttpLogger.Request("PUT", url,
            new[] { new KeyValuePair<string, string>("x-ms-blob-type", "BlockBlob") },
            $"[binary blob: {content.LongLength} bytes]");

        // Use a client without PlayFab default headers (e.g. X-EntityToken) so the
        // entity token is not leaked to the blob storage endpoint via the SAS URL.
        var sw = System.Diagnostics.Stopwatch.StartNew();
        using var response = await s_blobClient.SendAsync(request);

        if (!response.IsSuccessStatusCode)
        {
            var body = await response.Content.ReadAsStringAsync();
            HttpLogger.Response(url, (int)response.StatusCode, response.StatusCode.ToString(), sw.ElapsedMilliseconds, body: body);
            throw new HttpRequestException($"Blob upload failed: {response.StatusCode}\n{body}");
        }

        HttpLogger.Response(url, (int)response.StatusCode, response.StatusCode.ToString(), sw.ElapsedMilliseconds);
        onProgress?.Invoke(content.LongLength, content.LongLength);
    }

    /// <summary>
    /// Finalizes a manifest version (transitions to "Finalized" state).
    /// </summary>
    public async Task FinalizeManifestAsync(
        string playerEntityId,
        string version,
        IEnumerable<(string FileName, long FileSizeBytes)> filesToFinalize,
        string? manifestDescriptionBase64,
        bool markBaseAsKnownGood,
        bool force)
    {
        var request = new
        {
            Entity = new
            {
                Id = playerEntityId,
                Type = "title_player_account"
            },
            Version = version,
            FilesToFinalize = filesToFinalize
                .Select(f => new { FileName = f.FileName, FileSizeBytes = f.FileSizeBytes.ToString() })
                .ToList(),
            Force = force,
            ManifestDescription = manifestDescriptionBase64,
            MarkBaseAsKnownGood = markBaseAsKnownGood
        };

        await PostAsync<object>("/GameSave/FinalizeManifest", request);
    }

    /// <summary>
    /// Resets cloud save state by deleting all manifests for a player.
    /// </summary>
    public async Task ResetCloudAsync(string playerEntityId, Action<string>? log = null)
    {
        var response = await ListManifestsAsync(playerEntityId);
        var manifests = response.Data?.Manifests ?? new List<Models.ManifestInfo>();

        if (manifests.Count == 0)
        {
            log?.Invoke("No manifests to delete.");
            return;
        }

        var deletableStates = new[] { "Initialized", "Uploading", "Finalized" };
        var skippedStates = new[] { "Quarantined", "PendingDeletion" };
        int deleted = 0;
        int failed = 0;

        foreach (var manifest in manifests)
        {
            var status = manifest.Status ?? "unknown";
            
            if (skippedStates.Contains(status, StringComparer.OrdinalIgnoreCase))
            {
                log?.Invoke($"Skipping manifest version {manifest.Version} (status: {status} - not safe to delete)");
                continue;
            }
            
            if (!deletableStates.Contains(status, StringComparer.OrdinalIgnoreCase))
            {
                log?.Invoke($"Skipping manifest version {manifest.Version} (unknown status: {status})");
                continue;
            }

            try
            {
                log?.Invoke($"Deleting manifest version {manifest.Version} (status: {status})...");
                await DeleteManifestAsync(playerEntityId, manifest.Version!);
                deleted++;
            }
            catch (Exception ex)
            {
                log?.Invoke($"Warning: Failed to delete manifest version {manifest.Version}: {ex.Message}");
                failed++;
            }
        }

        log?.Invoke($"Cloud reset complete. Deleted: {deleted}, Failed: {failed}");
    }

    /// <summary>
    /// Downloads a file from a blob URL (used for manifest files and chunks).
    /// Reports progress via optional callback.
    /// </summary>
    public async Task<byte[]> DownloadBlobAsync(string url, string? displayName = null, Action<long, long?>? onProgress = null)
    {
        HttpLogger.Request("GET", url, headers: null, body: null);
        var sw = System.Diagnostics.Stopwatch.StartNew();
        // Use a client without PlayFab default headers (e.g. X-EntityToken) so the
        // entity token is not leaked to the blob storage endpoint via the SAS URL.
        using var request = new HttpRequestMessage(HttpMethod.Get, url);
        using var response = await s_blobClient.SendAsync(request, HttpCompletionOption.ResponseHeadersRead);
        var totalBytes = response.Content.Headers.ContentLength;
        HttpLogger.Response(url, (int)response.StatusCode, response.StatusCode.ToString(), sw.ElapsedMilliseconds,
            body: $"[binary blob: {(totalBytes.HasValue ? totalBytes.Value + " bytes" : "streaming")}]");
        response.EnsureSuccessStatusCode();

        using var contentStream = await response.Content.ReadAsStreamAsync();
        using var memoryStream = new MemoryStream();
        
        var buffer = new byte[81920]; // 80KB buffer
        long bytesRead = 0;
        int read;
        
        while ((read = await contentStream.ReadAsync(buffer, 0, buffer.Length)) > 0)
        {
            await memoryStream.WriteAsync(buffer, 0, read);
            bytesRead += read;
            onProgress?.Invoke(bytesRead, totalBytes);
        }
        
        return memoryStream.ToArray();
    }

    /// <summary>
    /// Downloads and parses the extended manifest JSON.
    /// </summary>
    public async Task<ExtendedManifest> DownloadExtendedManifestAsync(string url)
    {
        var bytes = await DownloadBlobAsync(url);
        return ParseExtendedManifest(bytes);
    }

    /// <summary>
    /// Parses extended manifest JSON from raw bytes (so callers that also need the raw
    /// bytes — e.g. for diagnostics — can download once and parse without a second request).
    /// </summary>
    public static ExtendedManifest ParseExtendedManifest(byte[] bytes)
    {
        var json = Encoding.UTF8.GetString(bytes);
        return JsonSerializer.Deserialize<ExtendedManifest>(json)
            ?? throw new InvalidOperationException("Failed to parse extended manifest");
    }

    // Transient HTTP statuses worth retrying (server-side / throttling).
    private static readonly HashSet<int> s_retryableStatusCodes = new()
    {
        408, // RequestTimeout
        429, // TooManyRequests
        500, // InternalServerError
        502, // BadGateway
        503, // ServiceUnavailable
        504, // GatewayTimeout
    };

    private const int MaxPostRetries = 4;

    // During the MainServer admin-download rollout, GetManifestDownloadDetails for a
    // PendingDeletion manifest only succeeds when the request lands on an updated instance.
    // Use a higher attempt count with capped backoff so the call rides through the rollout.
    private const int MaxGameSaveErrorRetries = 12;
    private const int MaxRetryDelayMs = 2000;

    private async Task<T> PostAsync<T>(string endpoint, object request, int[]? retryableGameSaveErrorCodes = null)
    {
        var responseJson = await PostRawAsync(endpoint, request, retryableGameSaveErrorCodes);
        return JsonSerializer.Deserialize<T>(responseJson, s_responseOptions)
            ?? throw new InvalidOperationException($"Failed to deserialize response from {endpoint}");
    }

    // Same as PostAsync but returns the raw response JSON (used for diagnostics capture).
    private async Task<string> PostRawAsync(string endpoint, object request, int[]? retryableGameSaveErrorCodes = null)
    {
        if (string.IsNullOrEmpty(_entityToken))
            throw new InvalidOperationException("Entity token not set. Call SetEntityToken first.");

        var json = JsonSerializer.Serialize(request);
        var fullUrl = new Uri(_httpClient.BaseAddress!, endpoint).ToString();

        int maxAttempts = retryableGameSaveErrorCodes != null
            ? Math.Max(MaxPostRetries, MaxGameSaveErrorRetries)
            : MaxPostRetries;

        HttpStatusCode lastStatus = 0;
        string lastBody = "";
        for (int attempt = 1; attempt <= maxAttempts; attempt++)
        {
            // StringContent must be recreated per attempt (its stream is consumed once).
            using var content = new StringContent(json, Encoding.UTF8, "application/json");

            HttpLogger.Request("POST", fullUrl, CollectRequestHeaders(content), json);

            var sw = System.Diagnostics.Stopwatch.StartNew();
            using var response = await _httpClient.PostAsync(endpoint, content);
            var responseJson = await response.Content.ReadAsStringAsync();
            HttpLogger.Response(fullUrl, (int)response.StatusCode, response.StatusCode.ToString(),
                sw.ElapsedMilliseconds, body: responseJson);

            if (response.IsSuccessStatusCode)
            {
                return responseJson;
            }

            lastStatus = response.StatusCode;
            lastBody = responseJson;

            // Retry transient server-side / throttling errors, or specific GameSave error codes
            // (e.g. 20305 during the admin-download rollout), with capped exponential backoff.
            bool retryable = s_retryableStatusCodes.Contains((int)response.StatusCode)
                || (retryableGameSaveErrorCodes != null
                    && TryGetGameSaveErrorCode(responseJson, out int code)
                    && retryableGameSaveErrorCodes.Contains(code));

            if (attempt < maxAttempts && retryable)
            {
                int delayMs = Math.Min(MaxRetryDelayMs, 500 * (int)Math.Pow(2, attempt - 1));
                Console.Error.WriteLine(
                    $"  {endpoint} returned {(int)response.StatusCode} {response.StatusCode}; " +
                    $"retrying in {delayMs}ms (attempt {attempt}/{maxAttempts - 1})...");
                await Task.Delay(delayMs);
                continue;
            }

            break;
        }

        throw new HttpRequestException($"PlayFab API error: {lastStatus}\n{lastBody}");
    }

    // Extracts the GameSave numeric errorCode from a PlayFab error response body, if present.
    private static bool TryGetGameSaveErrorCode(string responseJson, out int errorCode)
    {
        errorCode = 0;
        if (string.IsNullOrEmpty(responseJson)) return false;
        try
        {
            using var doc = JsonDocument.Parse(responseJson);
            if (doc.RootElement.TryGetProperty("errorCode", out var ec) && ec.TryGetInt32(out errorCode))
            {
                return true;
            }
        }
        catch (JsonException)
        {
        }
        return false;
    }

    // Collects the headers actually sent on a GameSave REST request (default client
    // headers + per-request content headers) for verbose logging. The X-EntityToken
    // value is redacted by HttpLogger.
    private IEnumerable<KeyValuePair<string, string>> CollectRequestHeaders(HttpContent content)
    {
        var list = new List<KeyValuePair<string, string>>();
        foreach (var h in _httpClient.DefaultRequestHeaders)
        {
            list.Add(new KeyValuePair<string, string>(h.Key, string.Join(", ", h.Value)));
        }
        foreach (var h in content.Headers)
        {
            list.Add(new KeyValuePair<string, string>(h.Key, string.Join(", ", h.Value)));
        }
        return list;
    }

    public void Dispose()
    {
        _httpClient.Dispose();
    }
}
