using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace GameTestController.Xbox
{
    /// <summary>
    /// Ensures the GDK bin directory is on the DLL search path so that XTF DLLs
    /// (and their transitive dependencies) can be loaded at runtime.
    /// Call <see cref="EnsureLoaded"/> before any XTF P/Invoke calls.
    /// </summary>
    internal static class GdkDllResolver
    {
        private static bool _initialized;
        private static readonly object _lock = new();
        private static string? _gdkBinDir;

        /// <summary>
        /// Adds the GDK bin directory to the process DLL search path and registers
        /// a NativeLibrary resolver for XTF DLLs. Safe to call multiple times.
        /// </summary>
        public static void EnsureLoaded()
        {
            if (_initialized) return;
            lock (_lock)
            {
                if (_initialized) return;

                _gdkBinDir = FindGdkBinDirectory();
                if (_gdkBinDir != null && Directory.Exists(_gdkBinDir))
                {
                    // Add GDK bin to the DLL search path for transitive native deps
                    AddDllDirectory(_gdkBinDir);

                    // LOAD_LIBRARY_SEARCH_DEFAULT_DIRS includes: system, AddDllDirectory, and app dir
                    SetDefaultDllDirectories(0x1000);

                    // Also pre-load critical XTF DLLs to ensure they're available
                    // before .NET P/Invoke tries to resolve them
                    PreloadXtfDlls();
                }

                _initialized = true;
            }
        }

        /// <summary>
        /// Returns the resolved GDK bin directory, or null if not found.
        /// </summary>
        public static string? GdkBinDirectory => _gdkBinDir;

        private static void PreloadXtfDlls()
        {
            // Load in dependency order: XtfInternal first, then others that depend on it
            string[] dlls = new[]
            {
                "xbtp.dll",
                "XtfInternal.dll",
                "XtfFileIO.dll",
                "XtfProvision.dll",
                "xtfremotevideo.dll",
                "XtfRemoteRun.dll",
                "XtfConsoleManager.dll",
                "XtfConsoleControl.dll",
                "XtfInput.dll",
            };

            foreach (string dll in dlls)
            {
                string fullPath = Path.Combine(_gdkBinDir!, dll);
                if (File.Exists(fullPath))
                {
                    IntPtr handle = LoadLibraryEx(
                        fullPath, IntPtr.Zero,
                        LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
                    // Intentionally not checking result — best-effort preload
                }
            }
        }

        private static string? FindGdkBinDirectory()
        {
            // Try registry (GDK install path)
            try
            {
                using var key = Microsoft.Win32.Registry.LocalMachine.OpenSubKey(
                    @"SOFTWARE\Microsoft\GDK", false);
                string? installPath = key?.GetValue("InstallPath") as string;
                if (!string.IsNullOrEmpty(installPath))
                {
                    string bin = Path.Combine(installPath, "bin");
                    if (Directory.Exists(bin)) return bin;
                }
            }
            catch { }

            // Fallback to well-known path
            const string defaultPath = @"C:\Program Files (x86)\Microsoft GDK\bin";
            if (Directory.Exists(defaultPath)) return defaultPath;

            return null;
        }

        private const uint LOAD_LIBRARY_SEARCH_DEFAULT_DIRS = 0x1000;
        private const uint LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR = 0x0100;

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr AddDllDirectory(string newDirectory);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool SetDefaultDllDirectories(uint directoryFlags);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr LoadLibraryEx(string lpFileName, IntPtr hFile, uint dwFlags);
    }
}
