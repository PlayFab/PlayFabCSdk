using System;
using System.IO;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Starts chaos mode testing
    /// </summary>
    internal sealed class ChaosCommand : ICliCommand
    {
        private readonly ControllerRuntime _runtime;
        private const string ChaosScenarioFileName = "chaos-single-device.yml";

        public ChaosCommand(ControllerRuntime runtime)
        {
            _runtime = runtime ?? throw new ArgumentNullException(nameof(runtime), "ControllerRuntime cannot be null.");
        }

        public string Name => "chaos";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Starts chaos mode testing";
        public string Usage => "chaos";

        public async Task<CommandResult> ExecuteAsync(string[] args)
        {
            try
            {
                var chaosScenarioPath = Path.Combine(Environment.CurrentDirectory, "Scenarios", ChaosScenarioFileName);
                
                if (!File.Exists(chaosScenarioPath))
                {
                    return CommandResult.Error($"Chaos scenario not found: {chaosScenarioPath}");
                }

                Console.WriteLine("Starting chaos mode...");
                Console.WriteLine($"Running scenario: {ChaosScenarioFileName}");
                Console.WriteLine("Note: This will run one iteration. Use 'run' command for continuous chaos mode.");
                Console.WriteLine();

                var outcome = await _runtime.LoadScenarioAsync(chaosScenarioPath, autoLaunchLocalDevices: false);
                
                switch (outcome)
                {
                    case ScenarioRunOutcome.Passed:
                        return CommandResult.Ok("Chaos mode iteration completed successfully.");
                    case ScenarioRunOutcome.Failed:
                        return CommandResult.Error("Chaos mode iteration failed.");
                    case ScenarioRunOutcome.NotStarted:
                        return CommandResult.Error("Chaos mode could not be started.");
                    default:
                        return CommandResult.Error($"Chaos mode completed with unknown outcome: {outcome}");
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"Exception occurred: {ex}");
                return CommandResult.Error($"Failed to run chaos mode: {ex.Message}");
            }
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return System.Array.Empty<string>();
        }
    }
}
