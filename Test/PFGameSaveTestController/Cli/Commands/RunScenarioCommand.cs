using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Runs a test scenario
    /// </summary>
    internal sealed class RunScenarioCommand : ICliCommand
    {
        private readonly ControllerRuntime _runtime;

        public RunScenarioCommand(ControllerRuntime runtime)
        {
            _runtime = runtime;
        }

        public string Name => "run";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Runs a test scenario file";
        public string Usage => "run <scenario-file>";

        public async Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length == 0)
            {
                return CommandResult.Error($"Usage: {Usage}");
            }

            var scenarioPath = args[0];

            // If the path doesn't exist and doesn't contain a path separator, try looking in Scenarios folder
            if (!File.Exists(scenarioPath) && !scenarioPath.Contains(Path.DirectorySeparatorChar) && !scenarioPath.Contains(Path.AltDirectorySeparatorChar))
            {
                var scenariosPath = Path.Combine(Environment.CurrentDirectory, "Scenarios", scenarioPath);
                if (File.Exists(scenariosPath))
                {
                    scenarioPath = scenariosPath;
                }
            }

            if (!File.Exists(scenarioPath))
            {
                return CommandResult.Error($"Scenario file not found: {scenarioPath}");
            }

            Console.WriteLine($"Running scenario: {scenarioPath}");
            Console.WriteLine();

            try
            {
                var outcome = await _runtime.LoadScenarioAsync(scenarioPath, autoLaunchLocalDevices: false);
                
                switch (outcome)
                {
                    case ScenarioRunOutcome.Passed:
                        return CommandResult.Ok($"Scenario passed: {Path.GetFileName(scenarioPath)}");
                    case ScenarioRunOutcome.Failed:
                        return CommandResult.Error($"Scenario failed: {Path.GetFileName(scenarioPath)}");
                    case ScenarioRunOutcome.NotStarted:
                        return CommandResult.Error($"Scenario could not be started: {Path.GetFileName(scenarioPath)}");
                    default:
                        return CommandResult.Error($"Scenario completed with unknown outcome: {outcome}");
                }
            }
            catch (Exception ex)
            {
                return CommandResult.Error($"Scenario error: {ex.Message}");
            }
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            if (args.Length <= 1)
            {
                // Complete scenario file paths - default to Scenarios folder
                try
                {
                    var prefix = args.Length > 0 ? args[0] : string.Empty;
                    var directory = Path.Combine(Environment.CurrentDirectory, "Scenarios");

                    if (!Directory.Exists(directory))
                    {
                        return Array.Empty<string>();
                    }

                    var files = Directory.GetFiles(directory, "*.yml", SearchOption.AllDirectories)
                        .Select(f => Path.GetRelativePath(directory, f).Replace('\\', '/'))
                        .Where(f => string.IsNullOrEmpty(prefix) || f.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                        .OrderBy(f => f)
                        .ToArray();

                    return files;
                }
                catch
                {
                    return Array.Empty<string>();
                }
            }

            return Array.Empty<string>();
        }
    }
}
