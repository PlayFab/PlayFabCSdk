using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
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
            Log("PFGameSaveTestController - Headless Mode");
            Log($"Start time: {DateTime.Now:yyyy-MM-dd HH:mm:ss}");
            Log("");

            // Create runtime
            var runtime = new ControllerRuntime(
                logger: (message, isError, skipTimestamp) =>
                {
                    var timestamp = skipTimestamp ? "" : $"[{DateTime.Now:HH:mm:ss}] ";
                    var fullMessage = $"{timestamp}{message}";
                    Log(fullMessage);
                }
            );

            // Start the WebSocket server
            await runtime.StartAsync().ConfigureAwait(false);

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

                    scenarioPaths = await runtime.FindScenariosByTagAsync(scenariosPath, options.RunTag).ConfigureAwait(false);

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

                    // If not auto-launching, wait for devices to connect externally
                    if (!options.AutoLaunchDevices)
                    {
                        Log($"Waiting up to {options.DeviceWaitTimeoutSeconds}s for devices to connect...");
                        bool devicesReady = await runtime.WaitForDevicesAsync(
                            requiredCount: 2,  // Most scenarios need at least 2 devices
                            timeoutSeconds: options.DeviceWaitTimeoutSeconds
                        ).ConfigureAwait(false);
                        
                        if (!devicesReady)
                        {
                            Log("Warning: Not all expected devices connected within timeout. Proceeding with available devices.");
                        }
                        Log("");
                    }

                    result = await runtime.RunScenariosAsync(scenarioPaths, options.AutoLaunchDevices).ConfigureAwait(false);
                    result.Tag = options.RunTag;
                }
                else if (!string.IsNullOrEmpty(options.RunScenario))
                {
                    var scenariosPath = options.GetEffectiveScenariosPath();
                    var scenarioPath = options.RunScenario;

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
                        }
                    }

                    if (!File.Exists(scenarioPath))
                    {
                        Log($"Error: Scenario file not found: {options.RunScenario}");
                        return ExitCodes.ConfigurationError;
                    }

                    Log($"Running scenario: {Path.GetFileName(scenarioPath)}");
                    Log("");

                    // If not auto-launching, wait for devices to connect externally
                    if (!options.AutoLaunchDevices)
                    {
                        Log($"Waiting up to {options.DeviceWaitTimeoutSeconds}s for devices to connect...");
                        bool devicesReady = await runtime.WaitForDevicesAsync(
                            requiredCount: 2,
                            timeoutSeconds: options.DeviceWaitTimeoutSeconds
                        ).ConfigureAwait(false);
                        
                        if (!devicesReady)
                        {
                            Log("Warning: Not all expected devices connected within timeout. Proceeding with available devices.");
                        }
                        Log("");
                    }

                    scenarioPaths = new List<string> { scenarioPath };
                    result = await runtime.RunScenariosAsync(scenarioPaths, options.AutoLaunchDevices).ConfigureAwait(false);
                }
                else
                {
                    Log("Error: No scenarios specified. Use --run-tag or --run-scenario.");
                    return ExitCodes.ConfigurationError;
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
                else
                {
                    Log("RESULT: No tests were executed");
                    return ExitCodes.ConfigurationError;
                }
            }
            finally
            {
                await runtime.StopAsync().ConfigureAwait(false);
            }
        }

        private void Log(string message)
        {
            Console.WriteLine(message);
            _logFileWriter?.WriteLine(message);
        }
    }
}
