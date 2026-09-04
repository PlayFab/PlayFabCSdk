using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GameTestController.Cli
{
    /// <summary>
    /// Displays help information for built-in commands.
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
        public string Description => "Displays help information for built-in commands";
        public string Usage => "help [command]";

        // Logical grouping order for help display
        private static readonly string[] CommandOrder = new[]
        {
            "launch", "list-devices", "target",
            "run", "chaos", "chaos-forever", "set-chaos",
            "batch", "paste", "apihelp",
            "help", "exit"
        };

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length == 0)
            {
                var message = new System.Text.StringBuilder();
                message.AppendLine("Available commands:");
                message.AppendLine();

                var uniqueCommands = _commands.Values
                    .GroupBy(c => c.Name)
                    .Select(g => g.First())
                    .ToList();

                // Sort by explicit order, then alphabetically for anything unlisted
                var orderMap = CommandOrder
                    .Select((name, index) => (name, index))
                    .ToDictionary(x => x.name, x => x.index, StringComparer.OrdinalIgnoreCase);

                var sorted = uniqueCommands
                    .OrderBy(c => orderMap.TryGetValue(c.Name, out var idx) ? idx : 1000)
                    .ThenBy(c => c.Name);

                foreach (var cmd in sorted)
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
                var commandName = args[0];
                if (!_commands.TryGetValue(commandName, out var command))
                {
                    return Task.FromResult(CommandResult.Error($"Unknown command: {commandName}. Use 'apihelp' to search API commands."));
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
