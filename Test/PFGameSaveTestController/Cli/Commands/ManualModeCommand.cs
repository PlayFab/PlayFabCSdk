using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli.Commands
{
    internal class ManualModeCommand : ICliCommand
    {
        private readonly CliEngine _cliEngine;

        public ManualModeCommand(CliEngine cliEngine)
        {
            _cliEngine = cliEngine;
        }

        public string Name => "manual-mode";
        public string[] Aliases => new[] { "manual" };
        public string Description => "Enter manual API mode to execute individual commands";
        public string Usage => "manual-mode";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            _cliEngine.EnterManualMode();
            return Task.FromResult(CommandResult.Ok("Entered manual API mode. Type 'manual-mode-exit' or 'exit' to return to normal mode."));
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return System.Array.Empty<string>();
        }
    }
}
