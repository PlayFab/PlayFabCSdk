using System.Text.Json.Serialization;

namespace PfGameSaveUtil.Models;

/// <summary>
/// Extended manifest structure returned from blob storage.
/// Contains folder hierarchy and file extraction mappings.
/// </summary>
public class ExtendedManifest
{
    [JsonPropertyName("v1")]
    public ManifestV1? V1 { get; set; }
}

public class ManifestV1
{
    [JsonPropertyName("Folders")]
    public List<ManifestFolder>? Folders { get; set; }

    [JsonPropertyName("Files")]
    public List<ManifestFile>? Files { get; set; }
}

public class ManifestFolder
{
    [JsonPropertyName("Id")]
    public string? Id { get; set; }

    [JsonPropertyName("Name")]
    public string? Name { get; set; }
    
    [JsonPropertyName("Folders")]
    public List<ManifestFolder>? Folders { get; set; }
}

public class ManifestFile
{
    [JsonPropertyName("Name")]
    public string? Name { get; set; }
    
    [JsonPropertyName("FileId")]
    public string? FileId { get; set; }

    [JsonPropertyName("CompressSize")]
    public long CompressSize { get; set; }

    [JsonPropertyName("Compression")]
    public string? Compression { get; set; }

    [JsonPropertyName("Extract")]
    public List<ExtractEntry>? Extract { get; set; }
}

public class ExtractEntry
{
    [JsonPropertyName("Name")]
    public string? Name { get; set; }
    
    [JsonPropertyName("FileId")]
    public string? FileId { get; set; }

    [JsonPropertyName("FolderId")]
    public string? FolderId { get; set; }

    [JsonPropertyName("LastModified")]
    public string? LastModified { get; set; }
    
    [JsonPropertyName("Created")]
    public string? Created { get; set; }
    
    [JsonPropertyName("Size")]
    public long Size { get; set; }
    
    /// <summary>
    /// If true, this file entry is superseded by a newer version and should be skipped during extraction.
    /// </summary>
    [JsonPropertyName("SkipFile")]
    public bool SkipFile { get; set; }
}
