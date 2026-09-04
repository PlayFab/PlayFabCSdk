namespace PfGameSaveUtil;

public static class Utilities
{
    public static string FormatSize(long bytes)
    {
        if (bytes < 1024) return $"{bytes} B";
        if (bytes < 1024 * 1024) return $"{bytes / 1024.0:F1} KB";
        if (bytes < 1024 * 1024 * 1024) return $"{bytes / (1024.0 * 1024):F1} MB";
        return $"{bytes / (1024.0 * 1024 * 1024):F1} GB";
    }

    public static int CountFolders(List<Models.ManifestFolder>? folders)
    {
        if (folders == null) return 0;
        int count = folders.Count;
        foreach (var folder in folders)
        {
            count += CountFolders(folder.Folders);
        }
        return count;
    }
}
