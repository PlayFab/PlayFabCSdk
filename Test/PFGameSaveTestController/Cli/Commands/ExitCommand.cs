using System;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Exits the CLI
    /// </summary>
    internal sealed class ExitCommand : ICliCommand
    {
        public string Name => "exit";
        public string[] Aliases => new[] { "quit", "q" };
        public string Description => "Exits the CLI";
        public string Usage => "exit";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            return Task.FromResult(CommandResult.Exit());
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return System.Array.Empty<string>();
        }
    }
}
