using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli.Commands
{
    internal class ManualModeExitCommand : ICliCommand
    {
        private readonly CliEngine _cliEngine;

        public ManualModeExitCommand(CliEngine cliEngine)
        {
            _cliEngine = cliEngine;
        }

        public string Name => "manual-mode-exit";
        public string[] Aliases => System.Array.Empty<string>();
        public string Description => "Exit manual API mode and return to normal CLI mode";
        public string Usage => "manual-mode-exit";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            _cliEngine.ExitManualMode();
            return Task.FromResult(CommandResult.Ok("Exited manual API mode."));
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return System.Array.Empty<string>();
        }
    }
}
