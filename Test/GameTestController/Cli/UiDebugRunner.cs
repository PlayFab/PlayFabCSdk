using System;
using System.IO;
using System.Runtime.InteropServices;
using GameTestController.Xbox;

namespace GameTestController.Cli
{
    /// <summary>
    /// One-shot UI automation debugger. Uses <see cref="GameSaveUiNavigator"/> (the same
    /// code that test scenarios use via AutoNavigateGameSaveUi) to scan for PFGameSave
    /// dialogs, log what it finds, and optionally click a button.
    /// 
    /// Platform-aware — mirrors the engine logic in ExecuteAutoNavigateGameSaveUi:
    ///   pc-grts (default) → UI Automation scan of local desktop
    ///   pc-inproc          → no UI (inproc doesn't show TCUI dialogs)
    ///   xbox               → XTF screenshot + OpenCV template matching
    ///
    /// Invoked with: GameTestController.exe --ui-debug [--platform pc|xbox] [--address IP] [--click "ButtonName"]
    /// </summary>
    internal sealed class UiDebugRunner
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool FreeConsole();

        /// <summary>
        /// Parse --ui-debug and optional --click / --platform / --address from command line args.
        /// Returns null if --ui-debug is not present.
        /// </summary>
        public static UiDebugOptions? TryParse(string[] args)
        {
            bool found = false;
            string? clickTarget = null;
            string? platform = null;
            string? address = null;

            for (int i = 0; i < args.Length; i++)
            {
                if (string.Equals(args[i], "--ui-debug", StringComparison.OrdinalIgnoreCase))
                {
                    found = true;
                }
                else if (string.Equals(args[i], "--click", StringComparison.OrdinalIgnoreCase) && i + 1 < args.Length)
                {
                    clickTarget = args[++i];
                }
                else if (string.Equals(args[i], "--platform", StringComparison.OrdinalIgnoreCase) && i + 1 < args.Length)
                {
                    platform = args[++i];
                }
                else if (string.Equals(args[i], "--address", StringComparison.OrdinalIgnoreCase) && i + 1 < args.Length)
                {
                    address = args[++i];
                }
            }

            return found ? new UiDebugOptions { ClickTarget = clickTarget, Platform = platform, XboxAddress = address } : null;
        }

        public int Run(UiDebugOptions options)
        {
            AllocConsole();
            Console.SetOut(new System.IO.StreamWriter(Console.OpenStandardOutput()) { AutoFlush = true });
            Console.SetError(new System.IO.StreamWriter(Console.OpenStandardError()) { AutoFlush = true });
            try
            {
                int result = RunCore(options);
                Console.WriteLine();
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
                return result;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"ERROR: {ex.Message}");
                Console.WriteLine(ex.StackTrace);
                Console.WriteLine();
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
                return 1;
            }
            finally
            {
                FreeConsole();
            }
        }

        private static int RunCore(UiDebugOptions options)
        {
            string engine = NormalizeEngine(options.Platform);

            Log("=== PFGameSave UI Debug Scanner ===");
            Log($"Engine:       {engine}");
            Log($"Click target: {(options.ClickTarget != null ? $"\"{options.ClickTarget}\"" : "(none — scan only)")}");
            if (engine == "xbox")
                Log($"Xbox address: {options.XboxAddress ?? "(default)"}");
            Log("");

            switch (engine)
            {
                case "pc-inproc":
                case "pc-inproc-gamesaves":
                    Log("Automated UI navigation is not supported for inproc engines.");
                    Log("Inproc mode bypasses GRTS and never shows TCUI dialogs.");
                    return 0;

                case "xbox":
                    return RunXboxScan(options);

                case "pc-grts":
                    break;

                default:
                    Log($"Unknown engine '{engine}'. Treating as pc-grts.");
                    break;
            }

            return RunPcScan(options);
        }

        private static int RunPcScan(UiDebugOptions options)
        {
            using var navigator = new GameSaveUiNavigator(msg => Log(msg));

            Log("Scanning desktop for Gaming UI windows...");
            Log("");

            var results = navigator.ScanOnce();

            if (results.Count == 0)
            {
                Log("No \"Gaming UI\" windows found. No PFGameSave dialog is currently visible.");
            }
            else
            {
                foreach (var result in results)
                {
                    Log($">>> Gaming UI window (PID={result.ProcessId})");
                    Log($"  CurrentPage: {result.PageName ?? "(not found)"}");
                    Log($"  Dialog type: {result.DialogType?.ToString() ?? "Unknown"}");
                    Log($"  Buttons ({result.Buttons.Count}):");
                    foreach (var btn in result.Buttons)
                    {
                        Log($"    - \"{btn.Name}\" AutoId=\"{btn.AutomationId}\" Enabled={btn.IsEnabled} Invoke={btn.HasInvoke}");
                    }
                    Log("");
                }

                Log($"Inspected {results.Count} Gaming UI window(s).");
            }

            if (options.ClickTarget != null)
            {
                Log("");
                Log($"Attempting to click button matching \"{options.ClickTarget}\"...");
                bool clicked = navigator.ClickButton(options.ClickTarget);
                if (!clicked)
                {
                    Log("Click failed — see messages above.");
                    return 1;
                }
            }

            Log("");
            Log("=== Done ===");
            return 0;
        }

        private static int RunXboxScan(UiDebugOptions options)
        {
            // Resolve Xbox address
            string? address = options.XboxAddress;
            if (string.IsNullOrWhiteSpace(address))
            {
                Log("No --address specified, resolving default Xbox from XTF...");
                address = XboxScreenshotCapture.GetDefaultAddress();
                if (string.IsNullOrWhiteSpace(address))
                {
                    Log("ERROR: Could not resolve default Xbox address.");
                    Log("Use --address <ip> or set default console with xbconnect.");
                    return 1;
                }
                Log($"Resolved default Xbox: {address}");
            }

            // Output folder for screenshots
            string outputDir = Path.Combine(Environment.CurrentDirectory, "XboxUiDebug");
            Directory.CreateDirectory(outputDir);
            string timestamp = DateTime.Now.ToString("yyyyMMdd-HHmmss");

            // Capture screenshot
            Log($"Capturing screenshot from {address}...");
            var capture = new XboxScreenshotCapture(address, msg => Log(msg));
            using var screenshot = capture.Capture();
            if (screenshot == null)
            {
                Log("ERROR: Failed to capture screenshot. Is the Xbox reachable?");
                return 1;
            }

            string screenshotPath = Path.Combine(outputDir, $"screenshot-{timestamp}.png");
            OpenCvSharp.Cv2.ImWrite(screenshotPath, screenshot);
            Log($"Screenshot saved: {screenshotPath} ({screenshot.Width}x{screenshot.Height})");
            Log("");

            // Locate templates folder
            string templatesFolder = FindTemplatesFolder();
            Log($"Templates folder: {templatesFolder}");

            // Detect dialog
            var detector = new XboxDialogDetector(templatesFolder, msg => Log(msg));
            var match = detector.Detect(screenshot);

            // Save annotated screenshot for debugging
            string annotatedPath = Path.Combine(outputDir, $"annotated-{timestamp}.png");
            detector.SaveAnnotated(screenshot, match, annotatedPath);

            Log("");
            if (match != null)
            {
                Log($"=== DETECTED: {match.DialogType} (confidence={match.Confidence:F4}) ===");
                Log($"  Match location: ({match.MatchLocation.X}, {match.MatchLocation.Y})");
                Log($"  Template size:  {match.TemplateSize.Width}x{match.TemplateSize.Height}");

                // Send gamepad response if --click specifies an action
                if (options.ClickTarget != null)
                {
                    Log("");
                    var sequence = XboxGamepadInput.GetResponseSequence(match.DialogType, options.ClickTarget);
                    if (sequence == null)
                    {
                        Log($"ERROR: No gamepad sequence mapped for {match.DialogType} action '{options.ClickTarget}'.");
                        Log("Valid actions:");
                        Log("  Conflict:   TakeLocal|UseLocal|Local, TakeCloud|UseCloud|Cloud|Remote, Cancel|PlayOffline");
                        Log("  SyncFailed: Retry, Cancel|Offline|UseOffline");
                    }
                    else
                    {
                        Log($"Sending gamepad response for action '{options.ClickTarget}'...");
                        using var gamepad = new XboxGamepadInput(address, msg => Log(msg));
                        if (!gamepad.IsConnected)
                        {
                            Log("ERROR: Failed to connect virtual gamepad.");
                        }
                        else
                        {
                            bool sent = gamepad.RespondToDialog(match.DialogType, options.ClickTarget);
                            Log(sent ? "Gamepad response sent successfully." : "ERROR: Failed to send gamepad response.");
                        }
                    }
                }
            }
            else
            {
                Log("=== No dialog detected ===");
                Log($"Screenshot saved for analysis: {screenshotPath}");
                Log("To improve detection, crop a distinctive region of the dialog from the");
                Log("screenshot and save it as {DialogType}.png in the templates folder:");
                Log($"  {templatesFolder}");

                if (options.ClickTarget != null)
                {
                    Log("");
                    Log($"Cannot respond with '{options.ClickTarget}' — no dialog detected.");
                }
            }

            Log("");
            Log("Output files:");
            Log($"  Screenshot:  {screenshotPath}");
            Log($"  Annotated:   {annotatedPath}");
            Log("");
            Log("=== Done ===");
            return 0;
        }

        /// <summary>
        /// Finds the XboxDialogTemplates folder, checking relative to exe and cwd.
        /// </summary>
        private static string FindTemplatesFolder()
        {
            // Relative to executable
            string? exeDir = Path.GetDirectoryName(typeof(UiDebugRunner).Assembly.Location);
            if (!string.IsNullOrEmpty(exeDir))
            {
                string exePath = Path.Combine(exeDir, "XboxDialogTemplates");
                if (Directory.Exists(exePath))
                    return exePath;
            }

            // Relative to cwd
            string cwdPath = Path.Combine(Environment.CurrentDirectory, "XboxDialogTemplates");
            if (Directory.Exists(cwdPath))
                return cwdPath;

            // Source tree location
            string sourcePath = Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..",
                "Test", "GameTestController", "XboxDialogTemplates");
            if (Directory.Exists(sourcePath))
                return Path.GetFullPath(sourcePath);

            // Fallback — return the exe-relative path even if it doesn't exist yet
            return exeDir != null
                ? Path.Combine(exeDir, "XboxDialogTemplates")
                : cwdPath;
        }

        private static string NormalizeEngine(string? platform)
        {
            if (string.IsNullOrWhiteSpace(platform))
                return "pc-grts";

            return platform.Trim().ToLowerInvariant() switch
            {
                "pc" or "pc-grts" or "grts" => "pc-grts",
                "inproc" or "pc-inproc" => "pc-inproc",
                "inproc-gamesaves" or "pc-inproc-gamesaves" => "pc-inproc-gamesaves",
                "xbox" => "xbox",
                _ => platform.Trim().ToLowerInvariant()
            };
        }

        private static void Log(string message)
        {
            Console.WriteLine(message);
        }
    }

    internal sealed class UiDebugOptions
    {
        public string? ClickTarget { get; set; }

        /// <summary>
        /// Platform/engine target. Maps to the same engine names used by
        /// AutoNavigateGameSaveUi: pc-grts (default), pc-inproc, xbox.
        /// </summary>
        public string? Platform { get; set; }

        /// <summary>
        /// Xbox dev kit IP address. If null, uses the XTF default console.
        /// Only used when Platform is "xbox".
        /// </summary>
        public string? XboxAddress { get; set; }
    }
}
