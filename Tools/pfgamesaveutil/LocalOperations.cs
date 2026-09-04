using System.IO.Compression;
using System.Text;
using static PfGameSaveUtil.Utilities;
using static PfGameSaveUtil.RegistryHelpers;

namespace PfGameSaveUtil;

public static class LocalOperations
{
    public static string[] FindPgsBasePaths()
    {
        var results = new List<string>();
        foreach (var drive in DriveInfo.GetDrives())
        {
            if (drive.DriveType == DriveType.Fixed && drive.IsReady)
            {
                var pgsPath = Path.Combine(drive.Name, @"XboxGames\GameSave\pgs");
                if (Directory.Exists(pgsPath))
                {
                    results.Add(pgsPath);
                }
            }
        }
        return results.ToArray();
    }

    public static string[]? ResolveLocalFolders(string titleId, string localPath)
    {
        if (localPath.Equals("auto", StringComparison.OrdinalIgnoreCase))
        {
            var basePaths = FindPgsBasePaths();
            if (basePaths.Length == 0)
            {
                Console.Error.WriteLine("No PGS base folder found on any drive (looking for XboxGames\\GameSave\\pgs)");
                return null;
            }

            var allMatchingFolders = new List<string>();
            foreach (var basePath in basePaths)
            {
                var matchingFolders = Directory.GetDirectories(basePath, $"*_{titleId}", SearchOption.TopDirectoryOnly);
                allMatchingFolders.AddRange(matchingFolders);
            }

            if (allMatchingFolders.Count == 0)
            {
                Console.Error.WriteLine($"No local folder found matching title {titleId}");
                Console.Error.WriteLine($"  Searched: {string.Join(", ", basePaths)}");
                return null;
            }

            if (allMatchingFolders.Count > 1)
            {
                Console.WriteLine("Multiple matching local folders found:");
                foreach (var m in allMatchingFolders) Console.WriteLine($"  {m}");
                Console.Error.WriteLine("Please specify the exact path with --path");
                return null;
            }

            Console.WriteLine($"Auto-detected local folder: {allMatchingFolders[0]}");
            return allMatchingFolders.ToArray();
        }
        else
        {
            if (!Directory.Exists(localPath))
            {
                Console.Error.WriteLine($"Local folder not found: {localPath}");
                return null;
            }
            return new[] { localPath };
        }
    }

    public static void ResetLocalOnly(string titleId, string localPath, bool force)
    {
        Console.WriteLine("PlayFab Game Save - Reset Local");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine();

        var folders = ResolveLocalFolders(titleId, localPath);
        if (folders == null || folders.Length == 0)
        {
            Console.WriteLine("No local data to delete.");
            return;
        }

        Console.WriteLine($"Found {folders.Length} local folder(s):");
        foreach (var folder in folders)
        {
            var files = Directory.GetFiles(folder, "*", SearchOption.AllDirectories);
            var totalSize = files.Sum(f => new FileInfo(f).Length);
            Console.WriteLine($"  {folder} ({files.Length} files, {FormatSize(totalSize)})");
        }
        Console.WriteLine();

        if (!force)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
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

        foreach (var folder in folders)
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

        Console.WriteLine();
        Console.ForegroundColor = ConsoleColor.Green;
        Console.WriteLine("✓ Local reset complete");
        Console.ResetColor();
    }

    public static void ShowLocalStatus(string titleId, string localPath)
    {
        Console.WriteLine("PlayFab Game Save - Local Device Status");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine();
        Console.WriteLine("=" + new string('=', 70));
        Console.WriteLine();

        Console.ForegroundColor = ConsoleColor.Yellow;
        Console.WriteLine("PGS Folder Status:");
        Console.ResetColor();

        var foldersToShow = new List<string>();
        
        if (localPath.Equals("auto", StringComparison.OrdinalIgnoreCase))
        {
            var basePaths = FindPgsBasePaths();
            if (basePaths.Length == 0)
            {
                Console.ForegroundColor = ConsoleColor.DarkGray;
                Console.WriteLine("  No PGS base folder found on any drive");
                Console.ResetColor();
            }
            else
            {
                Console.WriteLine($"  Base path(s): {string.Join(", ", basePaths)}");
                foreach (var basePath in basePaths)
                {
                    var matches = Directory.GetDirectories(basePath, $"*_{titleId}", SearchOption.TopDirectoryOnly);
                    foldersToShow.AddRange(matches);
                }
                
                if (foldersToShow.Count == 0)
                {
                    Console.ForegroundColor = ConsoleColor.DarkGray;
                    Console.WriteLine($"  No folders found for title {titleId}");
                    Console.ResetColor();
                }
            }
        }
        else
        {
            if (Directory.Exists(localPath))
            {
                Console.WriteLine($"  Path: {localPath}");
                foldersToShow.Add(localPath);
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.DarkGray;
                Console.WriteLine($"  Path not found: {localPath}");
                Console.ResetColor();
            }
        }

        foreach (var folder in foldersToShow)
        {
            var folderName = Path.GetFileName(folder);
            var files = Directory.GetFiles(folder, "*", SearchOption.AllDirectories);
            var totalSize = files.Sum(f => new FileInfo(f).Length);
            
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"  {folderName}");
            Console.ResetColor();
            Console.WriteLine($"    Files: {files.Length}, Size: {FormatSize(totalSize)}");

            var manifests = Directory.GetFiles(folder, "extended-*-manifest.json");
            if (manifests.Length > 0)
            {
                var versions = manifests
                    .Select(m => Path.GetFileName(m).Replace("extended-", "").Replace("-manifest.json", ""))
                    .OrderByDescending(v => int.TryParse(v, out var n) ? n : 0)
                    .ToList();
                Console.WriteLine($"    Manifest versions: {string.Join(", ", versions)}");
            }

            var currentPath = Path.Combine(folder, "current");
            if (Directory.Exists(currentPath))
            {
                var currentFiles = Directory.GetFiles(currentPath, "*", SearchOption.AllDirectories);
                Console.WriteLine($"    current/ subfolder: {currentFiles.Length} files");
            }
        }

        Console.WriteLine();

        Console.ForegroundColor = ConsoleColor.Yellow;
        Console.WriteLine("Registry Status:");
        Console.ResetColor();

        ShowRegistryKey(@"HKLM:\SOFTWARE\Microsoft\XGameSaveStorage\PlayFab", "XGameSaveStorage\\PlayFab");

        Console.WriteLine();

        Console.ForegroundColor = ConsoleColor.Yellow;
        Console.WriteLine("GamingServices Settings:");
        Console.ResetColor();

        ShowRegistryValue(@"HKLM:\SOFTWARE\Microsoft\GamingServices", "ForceUseInprocGameSaves");
        ShowRegistryValue(@"HKLM:\SOFTWARE\Microsoft\GamingServices", "ForceUseLocalServices");
        ShowRegistryValue(@"HKLM:\SOFTWARE\Microsoft\GamingServices\Auth", "TraceToDebugger");

        Console.WriteLine();
        Console.ForegroundColor = ConsoleColor.DarkGray;
        Console.WriteLine("Tip: Modify registry settings with:");
        Console.WriteLine("  --inproc <true|false>          Set ForceUseInprocGameSaves");
        Console.WriteLine("  --local-services <true|false>  Set ForceUseLocalServices");
        Console.WriteLine("  --trace <true|false>           Set TraceToDebugger");
        Console.ResetColor();

        Console.WriteLine();
        Console.WriteLine("=" + new string('=', 70));
    }

    public static void CollectLocalPgsFolder(string titleId, string localPath)
    {
        Console.WriteLine("PlayFab Game Save - Collect Local Data");
        Console.WriteLine($"Title: {titleId}");
        Console.WriteLine();

        var folders = ResolveLocalFolders(titleId, localPath);
        if (folders == null || folders.Length == 0)
        {
            return;
        }
        var pgsFolder = folders[0];

        var files = Directory.GetFiles(pgsFolder, "*", SearchOption.AllDirectories);
        var totalSize = files.Sum(f => new FileInfo(f).Length);
        Console.WriteLine($"Found {files.Length} files ({FormatSize(totalSize)})");

        var statusInfo = GenerateLocalStatusInfo(titleId, pgsFolder);

        var timestamp = DateTime.Now.ToString("yyyyMMdd-HHmmss");
        var folderName = Path.GetFileName(pgsFolder);
        var zipFileName = $"PGS-{folderName}-{timestamp}.zip";
        var zipPath = Path.Combine(Directory.GetCurrentDirectory(), zipFileName);

        Console.WriteLine($"\nCreating archive: {zipPath}");

        try
        {
            if (File.Exists(zipPath))
            {
                File.Delete(zipPath);
            }

            ZipFile.CreateFromDirectory(pgsFolder, zipPath);

            using (var archive = ZipFile.Open(zipPath, ZipArchiveMode.Update))
            {
                var statusEntry = archive.CreateEntry("_device-status.txt");
                using var writer = new StreamWriter(statusEntry.Open());
                writer.Write(statusInfo);
            }

            var zipSize = new FileInfo(zipPath).Length;
            Console.WriteLine($"Archive created: {FormatSize(zipSize)}");
            Console.WriteLine();
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"✓ Local PGS data collected to: {zipPath}");
            Console.ResetColor();
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Failed to create archive: {ex.Message}");
            return;
        }

        Console.WriteLine();
        Console.WriteLine(statusInfo);
    }

    public static string GenerateLocalStatusInfo(string titleId, string pgsFolder)
    {
        var sb = new StringBuilder();
        sb.AppendLine("PlayFab Game Save - Device Status");
        sb.AppendLine($"Generated: {DateTime.Now:yyyy-MM-dd HH:mm:ss}");
        sb.AppendLine($"Title: {titleId}");
        sb.AppendLine($"Machine: {Environment.MachineName}");
        sb.AppendLine($"User: {Environment.UserName}");
        sb.AppendLine();
        sb.AppendLine(new string('=', 70));
        sb.AppendLine();

        sb.AppendLine("Folder Details:");
        sb.AppendLine($"  Path: {pgsFolder}");

        var files = Directory.GetFiles(pgsFolder, "*", SearchOption.AllDirectories);
        var totalSize = files.Sum(f => new FileInfo(f).Length);
        sb.AppendLine($"  Files: {files.Length}, Size: {FormatSize(totalSize)}");

        var manifests = Directory.GetFiles(pgsFolder, "extended-*-manifest.json");
        if (manifests.Length > 0)
        {
            var versions = manifests
                .Select(m => Path.GetFileName(m).Replace("extended-", "").Replace("-manifest.json", ""))
                .OrderByDescending(v => int.TryParse(v, out var n) ? n : 0)
                .ToList();
            sb.AppendLine($"  Manifest versions: {string.Join(", ", versions)}");
        }

        var currentPath = Path.Combine(pgsFolder, "current");
        if (Directory.Exists(currentPath))
        {
            var currentFiles = Directory.GetFiles(currentPath, "*", SearchOption.AllDirectories);
            sb.AppendLine($"  current/ subfolder: {currentFiles.Length} files");
        }

        sb.AppendLine();

        sb.AppendLine("Registry Status:");
        sb.AppendLine($"  XGameSaveStorage\\PlayFab: {GetRegistryKeyStatus(@"SOFTWARE\Microsoft\XGameSaveStorage\PlayFab")}");

        sb.AppendLine();

        sb.AppendLine("GamingServices Settings:");
        sb.AppendLine($"  ForceUseInprocGameSaves: {GetRegistryValueStatus(@"SOFTWARE\Microsoft\GamingServices", "ForceUseInprocGameSaves")}");
        sb.AppendLine($"  ForceUseLocalServices: {GetRegistryValueStatus(@"SOFTWARE\Microsoft\GamingServices", "ForceUseLocalServices")}");
        sb.AppendLine($"  TraceToDebugger: {GetRegistryValueStatus(@"SOFTWARE\Microsoft\GamingServices\Auth", "TraceToDebugger")}");

        sb.AppendLine();
        sb.AppendLine(new string('=', 70));

        return sb.ToString();
    }
}
