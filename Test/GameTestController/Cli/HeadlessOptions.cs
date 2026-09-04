using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace GameTestController.Cli
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
        /// Exclude scenarios that have this tag, even if they match RunTag.
        /// </summary>
        public List<string> ExcludeTags { get; set; } = new();

        /// <summary>
        /// Run specific scenario(s) by ID or filename.
        /// </summary>
        public List<string> RunScenarios { get; set; } = new List<string>();

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
        /// Number of devices to wait for before starting scenarios.
        /// When set, overrides the default heuristic (1 for tag runs, 2 for specific scenarios).
        /// </summary>
        public int? ExpectedDevices { get; set; }

        /// <summary>
        /// Auto-launch local devices for scenarios that need them.
        /// </summary>
        public bool AutoLaunchDevices { get; set; } = true;

        /// <summary>
        /// Stop running scenarios after the first failure.
        /// Remaining scenarios are marked as skipped in results.
        /// </summary>
        public bool StopOnFirstError { get; set; }

        /// <summary>
        /// Number of scenarios to skip from the beginning.
        /// Used to resume after fixing a failure found with StopOnFirstError.
        /// </summary>
        public int SkipScenarios { get; set; }

        /// <summary>
        /// Whether parsing was successful.
        /// </summary>
        public bool IsValid { get; set; } = true;

        /// <summary>
        /// Error message if parsing failed.
        /// </summary>
        public string? ErrorMessage { get; set; }

        /// <summary>
        /// Prefix prepended to customId in PFLocalUserCreateHandleWithPersistedLocalId commands.
        /// Used to isolate test runs on shared PlayFab titles (e.g., per-machine prefix).
        /// </summary>
        public string? CustomIdPrefix { get; set; }

        /// <summary>
        /// Port for game device WebSocket connections (default: 5000).
        /// </summary>
        public int DevicePort { get; set; } = 15080;

        /// <summary>
        /// Port for DeviceAgent WebSocket connections (default: 5001).
        /// Set to 0 to disable the agent server.
        /// </summary>
        public int AgentPort { get; set; } = 15081;

        /// <summary>
        /// Number of remote devices (DeviceAgents) expected to connect before starting scenarios.
        /// When > 0, the controller waits for this many agents, sends launchTestApp, then
        /// waits for the corresponding game devices to connect.
        /// </summary>
        public int RemoteDevices { get; set; }

        /// <summary>
        /// Platform key used for scenario platform status (e.g., "pc-inproc", "SteamPlayFab").
        /// Scenarios marked "ignore" for this platform are skipped. When --run-tag is also set,
        /// it matches against the platform status instead of the tags list.
        /// </summary>
        public string? PlatformFilter { get; set; }

        /// <summary>
        /// Path to the local test app executable to launch per-scenario.
        /// When set, the controller launches/kills this app for each scenario.
        /// </summary>
        public string? LocalAppPath { get; set; }

        /// <summary>
        /// Additional arguments to pass to the local test app when launching.
        /// </summary>
        public string? LocalAppArgs { get; set; }

        /// <summary>
        /// Comma-separated list of engines allowed for auto-launch (e.g., "pc-grts,pc-inproc").
        /// When set, auto-launch will only attempt engines in this list, skipping others (e.g., xbox).
        /// </summary>
        public List<string> AllowedEngines { get; set; } = new List<string>();

        /// <summary>
        /// Whether any headless-related options were specified.
        /// </summary>
        public bool HasHeadlessOptions => Headless || !string.IsNullOrEmpty(RunTag) || RunScenarios.Count > 0;

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

                    case "--exclude-tag":
                    case "-exclude-tag":
                        if (i + 1 < argsList.Count)
                        {
                            options.ExcludeTags.Add(argsList[++i]);
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--exclude-tag requires a tag value";
                        }
                        break;

                    case "--run-scenario":
                    case "-run-scenario":
                        if (i + 1 < argsList.Count)
                        {
                            options.RunScenarios.Add(argsList[++i]);
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

                    case "--expected-devices":
                    case "-expected-devices":
                        if (i + 1 < argsList.Count && int.TryParse(argsList[i + 1], out int expectedDevices))
                        {
                            i++;
                            options.ExpectedDevices = expectedDevices;
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--expected-devices requires a number";
                        }
                        break;

                    case "--stop-on-first-error":
                    case "-stop-on-first-error":
                        options.StopOnFirstError = true;
                        break;

                    case "--skip-scenarios":
                    case "-skip-scenarios":
                        if (i + 1 < argsList.Count && int.TryParse(argsList[i + 1], out int skipCount))
                        {
                            i++;
                            options.SkipScenarios = skipCount;
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--skip-scenarios requires a number";
                        }
                        break;

                    case "--custom-id-prefix":
                    case "-custom-id-prefix":
                        if (i + 1 < argsList.Count)
                        {
                            options.CustomIdPrefix = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--custom-id-prefix requires a prefix string";
                        }
                        break;

                    case "--device-port":
                    case "-device-port":
                        if (i + 1 < argsList.Count && int.TryParse(argsList[i + 1], out int devicePort))
                        {
                            i++;
                            options.DevicePort = devicePort;
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--device-port requires a port number";
                        }
                        break;

                    case "--agent-port":
                    case "-agent-port":
                        if (i + 1 < argsList.Count && int.TryParse(argsList[i + 1], out int agentPort))
                        {
                            i++;
                            options.AgentPort = agentPort;
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--agent-port requires a port number";
                        }
                        break;

                    case "--remote-devices":
                    case "-remote-devices":
                        if (i + 1 < argsList.Count && int.TryParse(argsList[i + 1], out int remoteDevices))
                        {
                            i++;
                            options.RemoteDevices = remoteDevices;
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--remote-devices requires a number";
                        }
                        break;

                    case "--platform-filter":
                    case "-platform-filter":
                        if (i + 1 < argsList.Count)
                        {
                            options.PlatformFilter = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--platform-filter requires a platform key (e.g., pc-inproc, xbox, psx)";
                        }
                        break;

                    case "--local-app-path":
                        if (i + 1 < argsList.Count)
                        {
                            options.LocalAppPath = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--local-app-path requires a path to the test app executable";
                        }
                        break;

                    case "--local-app-args":
                        if (i + 1 < argsList.Count)
                        {
                            options.LocalAppArgs = argsList[++i];
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--local-app-args requires an argument string";
                        }
                        break;

                    case "--allowed-engines":
                    case "-allowed-engines":
                        if (i + 1 < argsList.Count)
                        {
                            var engines = argsList[++i].Split(',', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                            options.AllowedEngines.AddRange(engines);
                        }
                        else
                        {
                            options.IsValid = false;
                            options.ErrorMessage = "--allowed-engines requires a comma-separated list (e.g., pc-grts,pc-inproc)";
                        }
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
            if (options.Headless && string.IsNullOrEmpty(options.RunTag) && options.RunScenarios.Count == 0
                && string.IsNullOrEmpty(options.ScenariosPath))
            {
                options.IsValid = false;
                options.ErrorMessage = "Headless mode requires --run-tag, --run-scenario, or --scenarios-path to specify which tests to run";
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
Usage:
  GameTestController.exe [options]

Options:
  --headless                Run without GUI, exit when complete
  --run-tag <tag>           Run all scenarios with the specified tag (e.g., 'passing')
  --exclude-tag <tag>       Exclude scenarios with this tag (e.g., 'failing')
  --run-scenario <id>       Run a specific scenario by ID or filename
  --exit-on-complete        Exit process when all tests finish
  --results-file <path>     Write JSON test results to file
  --junit-file <path>       Write JUnit XML test results to file (for ADO)
  --scenarios-path <path>   Override scenarios folder location
  --log-file <path>         Write controller log to file
  --device-wait-timeout <s> Timeout in seconds waiting for devices (default: 30)
  --expected-devices <n>    Number of devices to wait for before starting (default: auto-detect)
  --remote-devices <n>      Number of remote DeviceAgents expected (controller waits + sends launchTestApp)
  --device-port <port>      Port for game device connections (default: 5000)
  --agent-port <port>       Port for DeviceAgent connections (default: 5001, 0 to disable)
  --no-auto-launch          Don't auto-launch local device processes
  --stop-on-first-error     Stop after the first failing scenario
  --skip-scenarios <n>      Skip the first N scenarios (resume after a fix)
  --custom-id-prefix <str>  Prefix for customId to isolate concurrent test runs (default: machine name)
  --platform-filter <key>   Filter scenarios by platform status (e.g., pc-inproc, xbox, psx).
                            When set, --run-tag/--exclude-tag match the platform status instead of tags.
  -cli                      Run in interactive CLI mode (legacy)
  --ui-debug                Scan desktop for PFGameSave UI dialogs and dump UI tree
  --platform <name>         (with --ui-debug) Target platform: pc, xbox (default: pc)
  --address <ip>            (with --ui-debug --platform xbox) Xbox dev kit IP address
  --click <name>            (with --ui-debug) Click a button by name or AutomationId
  --help, -h                Show this help message

Exit Codes:
  0  All tests passed
  1  One or more tests failed
  2  Configuration error (invalid arguments, missing scenarios)
  3  Timeout waiting for devices
  4  Fatal error during execution

Examples:
  # Run all 'passing' tagged tests in headless mode
  GameTestController.exe --headless --run-tag passing --results-file results.json

  # Run all scenarios passing on pc-inproc platform
  GameTestController.exe --headless --run-tag passing --platform-filter pc-inproc --results-file results.json

  # Run a specific scenario
  GameTestController.exe --headless --run-scenario scenario-01-single-device-golden-path.yml

  # Run with JUnit output for ADO pipeline
  GameTestController.exe --headless --run-tag smoke --junit-file test-results.xml

  # Debug UI automation - scan for dialogs
  GameTestController.exe --ui-debug

  # Debug UI automation - scan and click a button
  GameTestController.exe --ui-debug --click ""STOP SYNC & CONTINUE""
";
        }
    }
}
