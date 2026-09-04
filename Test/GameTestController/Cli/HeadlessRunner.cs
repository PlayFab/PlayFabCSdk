using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace GameTestController.Cli
{
    /// <summary>
    /// Runs test scenarios in headless mode for ADO pipeline integration.
    /// </summary>
    internal sealed class HeadlessRunner
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool FreeConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool AttachConsole(int dwProcessId);

        private const int ATTACH_PARENT_PROCESS = -1;

        private StreamWriter? _logFileWriter;

        /// <summary>
        /// Runs the controller in headless mode with the specified options.
        /// </summary>
        public int Run(HeadlessOptions options)
        {
            // Try to attach to parent console first, allocate new one if needed
            if (!AttachConsole(ATTACH_PARENT_PROCESS))
            {
                AllocConsole();
            }

            try
            {
                // Setup log file if specified
                if (!string.IsNullOrEmpty(options.LogFile))
                {
                    try
                    {
                        var logDir = Path.GetDirectoryName(options.LogFile);
                        if (!string.IsNullOrEmpty(logDir) && !Directory.Exists(logDir))
                        {
                            Directory.CreateDirectory(logDir);
                        }
                        _logFileWriter = new StreamWriter(options.LogFile, append: false) { AutoFlush = true };
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine($"Warning: Could not open log file: {ex.Message}");
                    }
                }

                // Validate options
                if (!options.IsValid)
                {
                    Log(options.ErrorMessage ?? "Invalid command-line arguments");
                    return ExitCodes.ConfigurationError;
                }

                return RunAsync(options).GetAwaiter().GetResult();
            }
            catch (Exception ex)
            {
                Log($"Fatal error: {ex.Message}");
                Log($"Stack trace: {ex.StackTrace}");
                return ExitCodes.FatalError;
            }
            finally
            {
                _logFileWriter?.Dispose();
                _logFileWriter = null;
                FreeConsole();
            }
        }

        /// <summary>
        /// Shows the help text to the console.
        /// </summary>
        public static void ShowHelp()
        {
            if (!AttachConsole(ATTACH_PARENT_PROCESS))
            {
                AllocConsole();
            }
            Console.WriteLine(HeadlessOptions.GetHelpText());
        }

        private async Task<int> RunAsync(HeadlessOptions options)
        {
            Log("GameTestController - Headless Mode");
            Log($"Start time: {DateTime.Now:yyyy-MM-dd HH:mm:ss}");

            // Require custom ID prefix — without it, concurrent test runs on shared titles will collide
            if (string.IsNullOrWhiteSpace(options.CustomIdPrefix))
            {
                Log("ERROR: --custom-id-prefix is required. Without it, concurrent test runs on the same PlayFab");
                Log("       title will share the same user account and interfere with each other. Set --custom-id-prefix");
                Log("       to a unique value (e.g., machine name) to isolate test runs.");
                return ExitCodes.ConfigurationError;
            }
            Log($"Custom ID prefix: {options.CustomIdPrefix}");

            // Validate test account configuration constraints at startup
            var accountConfig = TestAccountConfig.Instance;
            if (accountConfig != null)
            {
                var violations = accountConfig.ValidateConstraints();
                if (violations.Count > 0)
                {
                    Log("WARNING: Test account configuration issues detected (Test/testAccountConfig.json):");
                    foreach (string v in violations)
                        Log($"  ⚠ {v}");
                    Log("");
                }
            }
            else
            {
                Log("NOTE: No testAccountConfig.json found. Account hints in prerequisite checks will be generic.");
                Log("      Create Test/testAccountConfig.json to codify your test account arrangement.");
            }
            Log("");

            // Create runtime
            var runtime = new ControllerRuntime(
                logger: (message, isError, skipTimestamp) =>
                {
                    var timestamp = skipTimestamp ? "" : $"[{DateTime.Now:HH:mm:ss}] ";
                    var fullMessage = $"{timestamp}{message}";
                    Log(fullMessage);
                },
                devicePort: options.DevicePort,
                agentPort: options.RemoteDevices > 0 ? options.AgentPort : 0
            );

            // Set default log directory for gathering device logs
            // Use the directory containing the results file, or the log file directory
            string? logDirectory = null;
            if (!string.IsNullOrEmpty(options.ResultsFile))
            {
                logDirectory = Path.GetDirectoryName(options.ResultsFile);
            }
            else if (!string.IsNullOrEmpty(options.LogFile))
            {
                logDirectory = Path.GetDirectoryName(options.LogFile);
            }
            if (!string.IsNullOrEmpty(logDirectory))
            {
                runtime.UpdateDefaultLogDirectory(logDirectory);
                Log($"Device logs will be saved to: {logDirectory}");
            }

            // Apply allowed engines constraint
            if (options.AllowedEngines.Count > 0)
            {
                runtime.AllowedEngines = options.AllowedEngines;
                Log($"Allowed engines: {string.Join(", ", options.AllowedEngines)}");
            }

            // Start the WebSocket server(s)
            await runtime.StartAsync().ConfigureAwait(false);

            bool allScenariosIgnored = false;
            try
            {
                TestRunResult result;
                List<string> scenarioPaths;

                // Find scenarios to run
                if (!string.IsNullOrEmpty(options.RunTag))
                {
                    var scenariosPath = options.GetEffectiveScenariosPath();
                    Log($"Searching for scenarios with tag '{options.RunTag}' in: {scenariosPath}");

                    if (!Directory.Exists(scenariosPath))
                    {
                        Log($"Error: Scenarios directory not found: {scenariosPath}");
                        return ExitCodes.ConfigurationError;
                    }

                    scenarioPaths = await runtime.FindScenariosByTagAsync(scenariosPath, options.RunTag, options.ExcludeTags, options.PlatformFilter).ConfigureAwait(false);

                    if (scenarioPaths.Count == 0)
                    {
                        Log($"Error: No scenarios found with tag '{options.RunTag}'");
                        return ExitCodes.ConfigurationError;
                    }

                    Log($"Found {scenarioPaths.Count} scenario(s) with tag '{options.RunTag}':");
                    foreach (var path in scenarioPaths)
                    {
                        Log($"  - {Path.GetFileName(path)}");
                    }
                    Log("");

                    allScenariosIgnored = await AreAllScenariosIgnoredForPlatformAsync(scenarioPaths, options.PlatformFilter).ConfigureAwait(false);
                    if (!await EnsureRemoteAgentsReadyAsync(runtime, options, allScenariosIgnored).ConfigureAwait(false))
                    {
                        return ExitCodes.DeviceTimeout;
                    }

                    // If not auto-launching and no per-scenario lifecycle, wait for devices to connect externally
                    if (!allScenariosIgnored && !options.AutoLaunchDevices && string.IsNullOrWhiteSpace(options.LocalAppPath))
                    {
                        Log($"Waiting up to {options.DeviceWaitTimeoutSeconds}s for devices to connect...");
                        bool devicesReady = await runtime.WaitForDevicesAsync(
                            requiredCount: options.ExpectedDevices ?? 1,  // Wait for at least 1 device; scenarios that need more will wait at run time
                            timeoutSeconds: options.DeviceWaitTimeoutSeconds
                        ).ConfigureAwait(false);
                        
                        if (!devicesReady)
                        {
                            Log("Warning: Not all expected devices connected within timeout. Proceeding with available devices.");
                        }
                        Log("");
                    }

                    result = await runtime.RunScenariosAsync(scenarioPaths, options.AutoLaunchDevices, customIdPrefix: options.CustomIdPrefix, localAppPath: options.LocalAppPath, localAppArgs: options.LocalAppArgs, platformFilter: options.PlatformFilter).ConfigureAwait(false);
                    result.Tag = options.RunTag;
                }
                else if (options.RunScenarios.Count > 0)
                {
                    var scenariosPath = options.GetEffectiveScenariosPath();
                    var scenarioEntries = options.RunScenarios;
                    scenarioPaths = new List<string>();

                    foreach (var entry in scenarioEntries)
                    {
                        var scenarioPath = entry;

                        // Resolve scenario path
                        if (!Path.IsPathRooted(scenarioPath))
                        {
                            var inScenariosFolder = Path.Combine(scenariosPath, scenarioPath);
                            if (File.Exists(inScenariosFolder))
                            {
                                scenarioPath = inScenariosFolder;
                            }
                            else if (!File.Exists(scenarioPath))
                            {
                                // Try with .yml extension
                                var withExtension = scenarioPath.EndsWith(".yml", StringComparison.OrdinalIgnoreCase)
                                    ? scenarioPath
                                    : scenarioPath + ".yml";
                                var inFolderWithExt = Path.Combine(scenariosPath, withExtension);
                                if (File.Exists(inFolderWithExt))
                                {
                                    scenarioPath = inFolderWithExt;
                                }
                                else
                                {
                                    // Search recursively in subfolders
                                    var searchName = withExtension;
                                    var matches = Directory.GetFiles(scenariosPath, searchName, SearchOption.AllDirectories);
                                    if (matches.Length == 1)
                                    {
                                        scenarioPath = matches[0];
                                    }
                                    else if (matches.Length > 1)
                                    {
                                        Log($"Error: Ambiguous scenario '{entry}' matches {matches.Length} files. Use area/filename to disambiguate.");
                                        return ExitCodes.ConfigurationError;
                                    }
                                }
                            }
                        }

                        if (!File.Exists(scenarioPath))
                        {
                            Log($"Error: Scenario file not found: {entry}");
                            return ExitCodes.ConfigurationError;
                        }

                        scenarioPaths.Add(scenarioPath);
                    }

                    Log($"Running scenario(s): {string.Join(", ", scenarioPaths.Select(p => Path.GetFileName(p)))}");
                    Log("");

                    allScenariosIgnored = await AreAllScenariosIgnoredForPlatformAsync(scenarioPaths, options.PlatformFilter).ConfigureAwait(false);
                    if (!await EnsureRemoteAgentsReadyAsync(runtime, options, allScenariosIgnored).ConfigureAwait(false))
                    {
                        return ExitCodes.DeviceTimeout;
                    }

                    // If not auto-launching and no per-scenario lifecycle, wait for devices to connect externally
                    if (!allScenariosIgnored && !options.AutoLaunchDevices && string.IsNullOrWhiteSpace(options.LocalAppPath))
                    {
                        Log($"Waiting up to {options.DeviceWaitTimeoutSeconds}s for devices to connect...");
                        bool devicesReady = await runtime.WaitForDevicesAsync(
                            requiredCount: options.ExpectedDevices ?? 2,
                            timeoutSeconds: options.DeviceWaitTimeoutSeconds
                        ).ConfigureAwait(false);
                        
                        if (!devicesReady)
                        {
                            Log("Warning: Not all expected devices connected within timeout. Proceeding with available devices.");
                        }
                        Log("");
                    }

                    scenarioPaths = new List<string>(scenarioPaths);
                    result = await runtime.RunScenariosAsync(scenarioPaths, options.AutoLaunchDevices, customIdPrefix: options.CustomIdPrefix, localAppPath: options.LocalAppPath, localAppArgs: options.LocalAppArgs, platformFilter: options.PlatformFilter).ConfigureAwait(false);
                }
                else
                {
                    // No explicit scenarios — run all .yaml/.yml files from --scenarios-path
                    var scenariosPath = options.GetEffectiveScenariosPath();
                    if (!Directory.Exists(scenariosPath))
                    {
                        Log($"Error: Scenarios directory not found: {scenariosPath}");
                        return ExitCodes.ConfigurationError;
                    }

                    scenarioPaths = Directory.GetFiles(scenariosPath, "*.yaml", SearchOption.TopDirectoryOnly)
                        .Concat(Directory.GetFiles(scenariosPath, "*.yml", SearchOption.TopDirectoryOnly))
                        .OrderBy(p => p, StringComparer.OrdinalIgnoreCase)
                        .ToList();

                    if (scenarioPaths.Count == 0)
                    {
                        Log($"Error: No scenario files found in: {scenariosPath}");
                        return ExitCodes.ConfigurationError;
                    }

                    Log($"Running all {scenarioPaths.Count} scenario(s) from: {scenariosPath}");
                    foreach (var path in scenarioPaths)
                    {
                        Log($"  - {Path.GetFileName(path)}");
                    }
                    Log("");

                    allScenariosIgnored = await AreAllScenariosIgnoredForPlatformAsync(scenarioPaths, options.PlatformFilter).ConfigureAwait(false);
                    if (!await EnsureRemoteAgentsReadyAsync(runtime, options, allScenariosIgnored).ConfigureAwait(false))
                    {
                        return ExitCodes.DeviceTimeout;
                    }

                    result = await runtime.RunScenariosAsync(scenarioPaths, options.AutoLaunchDevices, customIdPrefix: options.CustomIdPrefix, localAppPath: options.LocalAppPath, localAppArgs: options.LocalAppArgs, platformFilter: options.PlatformFilter).ConfigureAwait(false);
                }

                // Print summary
                Log("");
                Log("═══════════════════════════════════════════════════════════════");
                Log($"  TEST RUN SUMMARY");
                Log($"  Total: {result.Summary.Total}  Passed: {result.Summary.Passed}  Failed: {result.Summary.Failed}  Skipped: {result.Summary.Skipped}");
                Log($"  Duration: {result.TotalDurationSeconds:F1} seconds");
                Log("═══════════════════════════════════════════════════════════════");
                Log("");

                // Print detailed results table with durations
                if (result.Scenarios.Count > 0)
                {
                    Log("  SCENARIO RESULTS:");
                    Log("  ─────────────────────────────────────────────────────────────");
                    Log($"  {"Status",-8} {"Duration",-10} {"Scenario"}");
                    Log("  ─────────────────────────────────────────────────────────────");
                    foreach (var scenario in result.Scenarios)
                    {
                        string statusIcon = scenario.Status switch
                        {
                            TestStatus.Passed => "✓ PASS",
                            TestStatus.Failed => "✗ FAIL",
                            _ => "○ SKIP"
                        };
                        string duration = $"{scenario.DurationSeconds:F1}s";
                        string name = scenario.Name ?? scenario.Id ?? "(unknown)";
                        Log($"  {statusIcon,-8} {duration,-10} {name}");
                    }
                    Log("  ─────────────────────────────────────────────────────────────");
                    Log("");
                }

                // Write results files
                if (!string.IsNullOrEmpty(options.ResultsFile))
                {
                    try
                    {
                        TestResultsWriter.WriteJsonResults(result, options.ResultsFile);
                        Log($"JSON results written to: {options.ResultsFile}");
                    }
                    catch (Exception ex)
                    {
                        Log($"Warning: Failed to write JSON results: {ex.Message}");
                    }
                }

                if (!string.IsNullOrEmpty(options.JUnitFile))
                {
                    try
                    {
                        TestResultsWriter.WriteJUnitResults(result, options.JUnitFile);
                        Log($"JUnit XML results written to: {options.JUnitFile}");
                    }
                    catch (Exception ex)
                    {
                        Log($"Warning: Failed to write JUnit results: {ex.Message}");
                    }
                }

                // Determine exit code
                if (result.Summary.Failed > 0)
                {
                    Log($"RESULT: {result.Summary.Failed} test(s) FAILED");
                    return ExitCodes.TestsFailed;
                }
                else if (result.Summary.Passed > 0)
                {
                    Log("RESULT: All tests PASSED");
                    return ExitCodes.Success;
                }
                else if (result.Summary.Total > 0 && result.Summary.Skipped == result.Summary.Total)
                {
                    if (allScenariosIgnored)
                    {
                        Log("RESULT: All selected tests were SKIPPED due to platform ignore");
                        return ExitCodes.Success;
                    }

                    Log("RESULT: All selected tests were SKIPPED without executing");
                    return ExitCodes.ConfigurationError;
                }
                else
                {
                    Log("RESULT: No tests were executed");
                    return ExitCodes.ConfigurationError;
                }
            }
            finally
            {
                // Kill remote test apps before stopping
                if (options.RemoteDevices > 0 && !allScenariosIgnored)
                {
                    await runtime.KillAllAgentTestAppsAsync().ConfigureAwait(false);
                }
                await runtime.StopAsync().ConfigureAwait(false);
            }
        }

        private async Task<bool> AreAllScenariosIgnoredForPlatformAsync(IReadOnlyList<string> scenarioPaths, string? platformFilter)
        {
            if (scenarioPaths.Count == 0 || string.IsNullOrWhiteSpace(platformFilter))
            {
                return false;
            }

            var loader = new ScenarioManifestLoader();
            foreach (string path in scenarioPaths)
            {
                try
                {
                    ScenarioManifest? manifest = await loader.LoadAsync(path, (_, _, _) => { }).ConfigureAwait(false);
                    if (manifest == null ||
                        !string.Equals(manifest.GetPlatformStatus(platformFilter), "ignore", StringComparison.OrdinalIgnoreCase))
                    {
                        return false;
                    }
                }
                catch (IOException ex)
                {
                    Log($"Warning: Could not evaluate platform status for '{path}': {ex.Message}");
                    return false;
                }
                catch (YamlDotNet.Core.YamlException ex)
                {
                    Log($"Warning: Could not evaluate platform status for '{path}': {ex.Message}");
                    return false;
                }
            }

            return true;
        }

        private async Task<bool> EnsureRemoteAgentsReadyAsync(
            ControllerRuntime runtime,
            HeadlessOptions options,
            bool allScenariosIgnored)
        {
            if (options.RemoteDevices <= 0)
            {
                return true;
            }

            if (allScenariosIgnored)
            {
                Log($"All selected scenarios are ignored for platform '{options.PlatformFilter}'.");
                Log("Skipping remote-agent preflight because no scenario will execute.");
                Log("");
                return true;
            }

            Log($"Expecting {options.RemoteDevices} remote device agent(s) on port {options.AgentPort}...");
            Log($"Waiting up to {options.DeviceWaitTimeoutSeconds}s for {options.RemoteDevices} remote agent(s) to connect...");
            bool agentsReady = await runtime.WaitForAgentsAsync(options.RemoteDevices, options.DeviceWaitTimeoutSeconds).ConfigureAwait(false);
            if (!agentsReady)
            {
                Log("Error: Not all remote device agents connected within timeout.");
                return false;
            }

            Log($"All {options.RemoteDevices} agent(s) connected. Apps will be launched per-scenario.");
            Log("");
            return true;
        }

        private void Log(string message)
        {
            Console.WriteLine(message);
            _logFileWriter?.WriteLine(message);
        }
    }
}
