using System;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace GameTestController
{
    /// <summary>
    /// Thin wrapper around the <c>pfgamesaveutil</c> command-line tool so the
    /// controller can pull a player's cloud save back down and verify what was
    /// actually uploaded (as opposed to only inspecting the local save folder).
    ///
    /// Credentials come from the environment, mirroring Test/pfgamesaveutilTests:
    ///   PFSECRETKEY   PlayFab title secret key (REQUIRED — when unset, callers skip).
    ///   PFTITLEID     PlayFab title ID (optional; defaults to E18D7, the test title).
    ///   PFGSUTIL_EXE  Full path to pfgamesaveutil.exe (optional; defaults to the
    ///                 published binary under Tools/pfgamesaveutil/bin/publish/).
    /// </summary>
    internal static class PfGameSaveUtil
    {
        internal const string DefaultTitleId = "E18D7";

        /// <summary>Returns the PlayFab secret key, or null when PFSECRETKEY is unset/empty.</summary>
        public static string? GetSecretKey()
        {
            string? key = Environment.GetEnvironmentVariable("PFSECRETKEY");
            return string.IsNullOrWhiteSpace(key) ? null : key;
        }

        /// <summary>Returns the title id from PFTITLEID, or the default test title.</summary>
        public static string GetTitleId()
        {
            string? titleId = Environment.GetEnvironmentVariable("PFTITLEID");
            return string.IsNullOrWhiteSpace(titleId) ? DefaultTitleId : titleId.Trim();
        }

        /// <summary>
        /// Resolves the path to pfgamesaveutil.exe. Honors PFGSUTIL_EXE, otherwise
        /// falls back to the published binary relative to the repo root. Returns null
        /// when no executable can be located.
        /// </summary>
        public static string? ResolveExePath()
        {
            string? overridePath = Environment.GetEnvironmentVariable("PFGSUTIL_EXE");
            if (!string.IsNullOrWhiteSpace(overridePath) && File.Exists(overridePath))
            {
                return overridePath;
            }

            // Controller runs out of Out/x64/Debug/GameTestController; repo root is 4 levels up.
            string baseDir = AppContext.BaseDirectory;
            string? repoRoot = FindRepoRoot(baseDir);
            if (repoRoot != null)
            {
                string published = Path.Combine(
                    repoRoot, "Tools", "pfgamesaveutil", "bin", "publish", "pfgamesaveutil.exe");
                if (File.Exists(published))
                {
                    return published;
                }
            }

            return null;
        }

        /// <summary>
        /// Walks up from <paramref name="startDir"/> looking for a recognizable repo
        /// root (a directory that contains the Tools/pfgamesaveutil folder).
        /// </summary>
        private static string? FindRepoRoot(string startDir)
        {
            DirectoryInfo? dir = new DirectoryInfo(startDir);
            for (int i = 0; i < 8 && dir != null; i++)
            {
                if (Directory.Exists(Path.Combine(dir.FullName, "Tools", "pfgamesaveutil")))
                {
                    return dir.FullName;
                }
                dir = dir.Parent;
            }
            return null;
        }

        /// <summary>Outcome of running a pfgamesaveutil verb.</summary>
        internal sealed record RunResult(bool Success, int ExitCode, string Stdout, string Stderr)
        {
            public string Combined => string.IsNullOrEmpty(Stderr) ? Stdout : $"{Stdout}\n{Stderr}";
        }

        /// <summary>
        /// Runs <c>pfgamesaveutil &lt;verb&gt; --title-id .. --secret-key .. --title-player-id .. --path ..</c>.
        /// The secret key is never echoed back to the caller (redacted in any thrown text).
        /// </summary>
        /// <param name="verb">download or downloadall.</param>
        /// <param name="titleId">PlayFab title id.</param>
        /// <param name="secretKey">PlayFab secret key.</param>
        /// <param name="titlePlayerId">title_player_account entity id (16 hex chars).</param>
        /// <param name="destPath">Destination folder for the download.</param>
        /// <param name="exePath">Path to pfgamesaveutil.exe.</param>
        /// <param name="timeout">Maximum time to allow the process to run.</param>
        public static RunResult RunDownload(
            string verb,
            string titleId,
            string secretKey,
            string titlePlayerId,
            string destPath,
            string exePath,
            TimeSpan timeout)
        {
            var psi = new ProcessStartInfo
            {
                FileName = exePath,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true,
            };
            psi.ArgumentList.Add(verb);
            psi.ArgumentList.Add("--title-id");
            psi.ArgumentList.Add(titleId);
            psi.ArgumentList.Add("--secret-key");
            psi.ArgumentList.Add(secretKey);
            psi.ArgumentList.Add("--title-player-id");
            psi.ArgumentList.Add(titlePlayerId);
            psi.ArgumentList.Add("--path");
            psi.ArgumentList.Add(destPath);

            var stdout = new StringBuilder();
            var stderr = new StringBuilder();

            using var process = new Process { StartInfo = psi };
            process.OutputDataReceived += (_, e) => { if (e.Data != null) stdout.AppendLine(e.Data); };
            process.ErrorDataReceived += (_, e) => { if (e.Data != null) stderr.AppendLine(e.Data); };

            process.Start();
            process.BeginOutputReadLine();
            process.BeginErrorReadLine();

            if (!process.WaitForExit((int)Math.Min(timeout.TotalMilliseconds, int.MaxValue)))
            {
                try { process.Kill(entireProcessTree: true); } catch { /* best effort */ }
                return new RunResult(false, -1,
                    stdout.ToString(),
                    $"pfgamesaveutil {verb} timed out after {timeout.TotalSeconds:0}s.");
            }

            // Ensure async buffers are flushed.
            process.WaitForExit();

            return new RunResult(process.ExitCode == 0, process.ExitCode, stdout.ToString(), stderr.ToString());
        }
    }
}
