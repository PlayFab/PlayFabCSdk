using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Command-line options for headless/automated test execution.
    /// </summary>
    internal sealed class HeadlessOptions
    {
        /// <summary>
        /// Run in headless mode (no GUI, exit when complete).
        /// </summary>
        public bool Headless { get; set; }

        /// <summary>
        /// Run all scenarios with the specified tag.
        /// </summary>
        public string? RunTag { get; set; }

        /// <summary>
        /// Run a specific scenario by ID or filename.
        /// </summary>
        public string? RunScenario { get; set; }

        /// <summary>
        /// Exit process when all tests complete.
        /// </summary>
        public bool ExitOnComplete { get; set; }

        /// <summary>
        /// Path to write JSON test results.
        /// </summary>
        public string? ResultsFile { get; set; }

        /// <summary>
        /// Path to write JUnit XML test results (for ADO integration).
        /// </summary>
        public string? JUnitFile { get; set; }

        /// <summary>
        /// Override path to scenarios folder.
        /// </summary>
        public string? ScenariosPath { get; set; }

        /// <summary>
        /// Path to write controller log file.
        /// </summary>
        public string? LogFile { get; set; }

        /// <summary>
        /// Timeout in seconds for waiting for devices to connect.
        /// </summary>
        public int DeviceWaitTimeoutSeconds { get; set; } = 30;

        /// <summary>
        /// Auto-launch local devices for scenarios that need them.
        /// </summary>
        public bool AutoLaunchDevices { get; set; } = true;

        /// <summary>
        /// Whether parsing was successful.
        /// </summary>
        public bool IsValid { get; set; } = true;

        /// <summary>
        /// Error message if parsing failed.
        /// </summary>
        public string? ErrorMessage { get; set; }

        /// <summary>
        /// Whether any headless-related options were specified.
        /// </summary>
        public bool HasHeadlessOptions => Headless || !string.IsNullOrEmpty(RunTag) || !string.IsNullOrEmpty(RunScenario);

        /// <summary>
        /// Parse command-line arguments into HeadlessOptions.
        /// </summary>
        public static HeadlessOptions Parse(string[] args)
        {
            var options = new HeadlessOptions();
            var argsList = new List<string>(args);

            for (int i = 0; i < argsList.Count; i++)
            {
                var arg = argsList[i];

                switch (arg.ToLowerInvariant())
                {
                    case "--headless":
                    case "-headless":
                        options.Headless = true;
                        options.ExitOnComplete = true; // Implied by headless
                        break;

                    case "--run-tag":
                    case "-run-tag":
                        if (i + 1 < argsList.Count)
                        {
                            options.RunTag = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--run-tag requires a tag value";
                        }
                        break;

                    case "--run-scenario":
                    case "-run-scenario":
                        if (i + 1 < argsList.Count)
                        {
                            options.RunScenario = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--run-scenario requires a scenario ID or path";
                        }
                        break;

                    case "--exit-on-complete":
                    case "-exit-on-complete":
                        options.ExitOnComplete = true;
                        break;

                    case "--results-file":
                    case "-results-file":
                        if (i + 1 < argsList.Count)
                        {
                            options.ResultsFile = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--results-file requires a file path";
                        }
                        break;

                    case "--junit-file":
                    case "-junit-file":
                        if (i + 1 < argsList.Count)
                        {
                            options.JUnitFile = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--junit-file requires a file path";
                        }
                        break;

                    case "--scenarios-path":
                    case "-scenarios-path":
                        if (i + 1 < argsList.Count)
                        {
                            options.ScenariosPath = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--scenarios-path requires a directory path";
                        }
                        break;

                    case "--log-file":
                    case "-log-file":
                        if (i + 1 < argsList.Count)
                        {
                            options.LogFile = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--log-file requires a file path";
                        }
                        break;

                    case "--device-wait-timeout":
                    case "-device-wait-timeout":
                        if (i + 1 < argsList.Count && int.TryParse(argsList[i + 1], out int timeout))
                        {
                            i++;
                            options.DeviceWaitTimeoutSeconds = timeout;
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--device-wait-timeout requires a number of seconds";
                        }
                        break;

                    case "--no-auto-launch":
                    case "-no-auto-launch":
                        options.AutoLaunchDevices = false;
                        break;

                    case "--help":
                    case "-help":
                    case "-h":
                    case "-?":
                        options.IsValid = false;
                        options.ErrorMessage = GetHelpText();
                        break;

                    case "-cli":
                        // Legacy CLI mode flag - ignored in headless parsing
                        break;

                    default:
                        // Ignore unknown arguments for forward compatibility
                        break;
                }

                if (!options.IsValid)
                {
                    break;
                }
            }

            // Validate that we have something to run in headless mode
            if (options.Headless && string.IsNullOrEmpty(options.RunTag) && string.IsNullOrEmpty(options.RunScenario))
            {
                options.IsValid = false;
                options.ErrorMessage = "Headless mode requires --run-tag or --run-scenario to specify which tests to run";
            }

            return options;
        }

        /// <summary>
        /// Gets the effective scenarios path, defaulting to ./Scenarios if not specified.
        /// </summary>
        public string GetEffectiveScenariosPath()
        {
            if (!string.IsNullOrEmpty(ScenariosPath))
            {
                return ScenariosPath;
            }

            // Try relative to current directory
            var relativePath = Path.Combine(Environment.CurrentDirectory, "Scenarios");
            if (Directory.Exists(relativePath))
            {
                return relativePath;
            }

            // Try relative to executable
            var exeDir = Path.GetDirectoryName(typeof(HeadlessOptions).Assembly.Location);
            if (!string.IsNullOrEmpty(exeDir))
            {
                var exeRelativePath = Path.Combine(exeDir, "Scenarios");
                if (Directory.Exists(exeRelativePath))
                {
                    return exeRelativePath;
                }
            }

            // Ensure the relative path exists before returning
            if (Directory.Exists(relativePath))
            {
                return relativePath;
            }

            // Fallback to current directory if all else fails
            return Environment.CurrentDirectory;
        }

        public static string GetHelpText()
        {
            return @"
PFGameSaveTestController - PlayFab GameSave Test Automation

Usage:
  PFGameSaveTestController.exe [options]

Options:
  --headless                Run without GUI, exit when complete
  --run-tag <tag>           Run all scenarios with the specified tag (e.g., 'passing')
  --run-scenario <id>       Run a specific scenario by ID or filename
  --exit-on-complete        Exit process when all tests finish
  --results-file <path>     Write JSON test results to file
  --junit-file <path>       Write JUnit XML test results to file (for ADO)
  --scenarios-path <path>   Override scenarios folder location
  --log-file <path>         Write controller log to file
  --device-wait-timeout <s> Timeout in seconds waiting for devices (default: 30)
  --no-auto-launch          Don't auto-launch local device processes
  -cli                      Run in interactive CLI mode (legacy)
  --help, -h                Show this help message

Exit Codes:
  0  All tests passed
  1  One or more tests failed
  2  Configuration error (invalid arguments, missing scenarios)
  3  Timeout waiting for devices
  4  Fatal error during execution

Examples:
  # Run all 'passing' tagged tests in headless mode
  PFGameSaveTestController.exe --headless --run-tag passing --results-file results.json

  # Run a specific scenario
  PFGameSaveTestController.exe --headless --run-scenario scenario-01-single-device-golden-path.yml

  # Run with JUnit output for ADO pipeline
  PFGameSaveTestController.exe --headless --run-tag smoke --junit-file test-results.xml
";
        }
    }
}
