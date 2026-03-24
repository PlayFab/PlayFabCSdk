using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Displays help information
    /// </summary>
    internal sealed class HelpCommand : ICliCommand
    {
        private readonly Dictionary<string, ICliCommand> _commands;

        public HelpCommand(Dictionary<string, ICliCommand> commands)
        {
            _commands = commands;
        }

        public string Name => "help";
        public string[] Aliases => new[] { "?", "h" };
        public string Description => "Displays help information";
        public string Usage => "help [command]";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length == 0)
            {
                // Show all commands
                var message = new System.Text.StringBuilder();
                message.AppendLine("Available commands:");
                message.AppendLine();

                var uniqueCommands = _commands.Values
                    .GroupBy(c => c.Name)
                    .Select(g => g.First())
                    .OrderBy(c => c.Name);

                foreach (var cmd in uniqueCommands)
                {
                    var aliasText = cmd.Aliases.Length > 0 
                        ? $" (aliases: {string.Join(", ", cmd.Aliases)})" 
                        : string.Empty;
                    message.AppendLine($"  {cmd.Name,-20} {cmd.Description}{aliasText}");
                }

                message.AppendLine();
                message.AppendLine("Type 'help <command>' for detailed usage information.");

                return Task.FromResult(CommandResult.Ok(message.ToString()));
            }
            else
            {
                // Show help for specific command
                var commandName = args[0];
                if (!_commands.TryGetValue(commandName, out var command))
                {
                    return Task.FromResult(CommandResult.Error($"Unknown command: {commandName}"));
                }

                var message = new System.Text.StringBuilder();
                message.AppendLine($"Command: {command.Name}");
                message.AppendLine($"Description: {command.Description}");
                if (command.Aliases.Length > 0)
                {
                    message.AppendLine($"Aliases: {string.Join(", ", command.Aliases)}");
                }
                message.AppendLine($"Usage: {command.Usage}");

                return Task.FromResult(CommandResult.Ok(message.ToString()));
            }
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            if (args.Length <= 1)
            {
                // Complete command names
                var prefix = args.Length > 0 ? args[0] : string.Empty;
                return _commands.Keys
                    .Distinct(StringComparer.OrdinalIgnoreCase)
                    .Where(cmd => cmd.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                    .OrderBy(cmd => cmd)
                    .ToArray();
            }

            return Array.Empty<string>();
        }
    }
}
