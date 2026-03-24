using System;
using System.IO;
using System.Threading;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Runs chaos mode continuously until interrupted
    /// </summary>
    internal sealed class ChaosForeverCommand : ICliCommand
    {
        private readonly ControllerRuntime _runtime;
        private const string ChaosScenarioFileName = "chaos-single-device.yml";
        private static bool _stopRequested = false;

        public ChaosForeverCommand(ControllerRuntime runtime)
        {
            _runtime = runtime;
        }

        public string Name => "chaos-forever";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Runs chaos mode continuously until Ctrl+C";
        public string Usage => "chaos-forever";

        public async Task<CommandResult> ExecuteAsync(string[] args)
        {
            try
            {
                var chaosScenarioPath = Path.Combine(Environment.CurrentDirectory, "Scenarios", ChaosScenarioFileName);
                
                if (!File.Exists(chaosScenarioPath))
                {
                    return CommandResult.Error($"Chaos scenario not found: {chaosScenarioPath}");
                }

                Console.WriteLine("Starting continuous chaos mode...");
                Console.WriteLine($"Running scenario: {ChaosScenarioFileName}");
                Console.WriteLine("Press Ctrl+C to stop after current iteration completes");
                Console.WriteLine();

                _stopRequested = false;
                Console.CancelKeyPress += OnCancelKeyPress;

                int iteration = 0;
                int passedCount = 0;
                int failedCount = 0;

                try
                {
                    while (!_stopRequested)
                    {
                        iteration++;
                        Console.WriteLine($"[Iteration {iteration}] Starting...");

                        var outcome = await _runtime.LoadScenarioAsync(chaosScenarioPath, autoLaunchLocalDevices: false);

                        switch (outcome)
                        {
                            case ScenarioRunOutcome.Passed:
                                passedCount++;
                                Console.WriteLine($"[Iteration {iteration}] Passed (Total: {passedCount} passed, {failedCount} failed)");
                                break;
                            case ScenarioRunOutcome.Failed:
                                failedCount++;
                                Console.WriteLine($"[Iteration {iteration}] Failed (Total: {passedCount} passed, {failedCount} failed)");
                                Console.WriteLine("Stopping due to failure.");
                                _stopRequested = true;
                                break;
                            case ScenarioRunOutcome.NotStarted:
                                Console.WriteLine($"[Iteration {iteration}] Could not start.");
                                _stopRequested = true;
                                break;
                        }

                        if (!_stopRequested)
                        {
                            Console.WriteLine();
                        }
                    }
                }
                finally
                {
                    Console.CancelKeyPress -= OnCancelKeyPress;
                }

                Console.WriteLine();
                Console.WriteLine($"Chaos mode stopped after {iteration} iteration(s).");
                Console.WriteLine($"Results: {passedCount} passed, {failedCount} failed");

                return CommandResult.Ok("Chaos mode completed.");
            }
            catch (Exception ex)
            {
                return CommandResult.Error($"Failed to run chaos mode: {ex.Message}");
            }
        }

        private static void OnCancelKeyPress(object? sender, ConsoleCancelEventArgs e)
        {
            Console.WriteLine();
            Console.WriteLine("Ctrl+C detected. Stopping after current iteration...");
            _stopRequested = true;
            e.Cancel = true; // Prevent immediate termination
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return System.Array.Empty<string>();
        }
    }
}
