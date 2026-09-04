using System.Text.Json.Serialization;

namespace PfGameSaveUtil.Models;

/// <summary>
/// Response from POST /GameSave/ListManifests
/// </summary>
public class ListManifestsResponse
{
    [JsonPropertyName("code")]
    public int Code { get; set; }

    [JsonPropertyName("status")]
    public string? Status { get; set; }

    [JsonPropertyName("data")]
    public ListManifestsData? Data { get; set; }
}

public class ListManifestsData
{
    [JsonPropertyName("Manifests")]
    public List<ManifestInfo>? Manifests { get; set; }

    [JsonPropertyName("NextAvailableVersion")]
    public string? NextAvailableVersion { get; set; }
}

public class ManifestInfo
{
    [JsonPropertyName("Version")]
    public string? Version { get; set; }

    [JsonPropertyName("LastModified")]
    public DateTime LastModified { get; set; }

    [JsonPropertyName("Status")]
    public string? Status { get; set; }
    
    [JsonPropertyName("CreationTimestamp")]
    public DateTime? CreationTimestamp { get; set; }
    
    [JsonPropertyName("FinalizationTimestamp")]
    public DateTime? FinalizationTimestamp { get; set; }

    [JsonPropertyName("BaseVersion")]
    public string? BaseVersion { get; set; }

    [JsonPropertyName("KnownGood")]
    public bool? KnownGood { get; set; }

    [JsonPropertyName("ManifestDescription")]
    public string? ManifestDescription { get; set; }

    [JsonPropertyName("TotalFileSize")]
    public long? TotalFileSize { get; set; }
}

/// <summary>
/// Response from POST /GameSave/GetManifestDownloadDetails
/// </summary>
public class GetManifestDownloadDetailsResponse
{
    [JsonPropertyName("code")]
    public int Code { get; set; }

    [JsonPropertyName("status")]
    public string? Status { get; set; }

    [JsonPropertyName("data")]
    public ManifestDownloadDetailsData? Data { get; set; }
}

public class ManifestDownloadDetailsData
{
    [JsonPropertyName("Files")]
    public List<FileDownloadInfo>? Files { get; set; }
}

public class FileDownloadInfo
{
    [JsonPropertyName("FileName")]
    public string? FileName { get; set; }

    [JsonPropertyName("DownloadUrl")]
    public string? DownloadUrl { get; set; }
}

/// <summary>
/// Response from POST /GameSave/GetQuotaForPlayer
/// </summary>
public class GetQuotaResponse
{
    [JsonPropertyName("code")]
    public int Code { get; set; }

    [JsonPropertyName("status")]
    public string? Status { get; set; }

    [JsonPropertyName("data")]
    public QuotaData? Data { get; set; }
}

public class QuotaData
{
    [JsonPropertyName("AvailableBytes")]
    public string? AvailableBytes { get; set; }

    [JsonPropertyName("TotalBytes")]
    public string? TotalBytes { get; set; }
}

/// <summary>
/// Response from POST /GameSave/InitializeManifest
/// </summary>
public class InitializeManifestResponse
{
    [JsonPropertyName("code")]
    public int Code { get; set; }

    [JsonPropertyName("data")]
    public InitializeManifestData? Data { get; set; }
}

public class InitializeManifestData
{
    [JsonPropertyName("Manifest")]
    public ManifestInfo? Manifest { get; set; }
}

/// <summary>
/// Response from POST /GameSave/InitiateUpload
/// </summary>
public class InitiateUploadResponse
{
    [JsonPropertyName("code")]
    public int Code { get; set; }

    [JsonPropertyName("data")]
    public InitiateUploadData? Data { get; set; }
}

public class InitiateUploadData
{
    [JsonPropertyName("Files")]
    public List<AllocatedFile>? Files { get; set; }

    [JsonPropertyName("RecommendedChunkSizeBytes")]
    public long? RecommendedChunkSizeBytes { get; set; }
}

public class AllocatedFile
{
    [JsonPropertyName("FileName")]
    public string? FileName { get; set; }

    [JsonPropertyName("UploadUrl")]
    public string? UploadUrl { get; set; }

    [JsonPropertyName("AccessTokenExpirationTime")]
    public DateTime? AccessTokenExpirationTime { get; set; }
}

/// <summary>
/// Device metadata sent with InitializeManifest.
/// </summary>
public class ManifestMetadata
{
    public string? DeviceId { get; set; }
    public string? DeviceName { get; set; }
    public string? DeviceType { get; set; }
    public string? DeviceVersion { get; set; }
}
