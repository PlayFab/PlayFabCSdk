using System;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameTestController.Cli.Commands
{
    /// <summary>
    /// Lists available API commands from connected devices, with optional substring filter.
    /// </summary>
    internal sealed class ApiHelpCommand : ICliCommand
    {
        private readonly ControllerRuntime? _runtime;

        public ApiHelpCommand(ControllerRuntime? runtime)
        {
            _runtime = runtime;
        }

        public string Name => "apihelp";
        public string[] Aliases => new[] { "api" };
        public string Description => "Lists available API commands (use 'apihelp <filter>' to search)";
        public string Usage => "apihelp [filter]";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            var apiCommands = ManualApiCommand.GetAllCommandNames(_runtime);

            if (apiCommands.Length == 0)
            {
                return Task.FromResult(CommandResult.Error(
                    "No device connected yet — API commands will be available once a device connects."));
            }

            var message = new StringBuilder();

            if (args.Length > 0)
            {
                var filter = args[0];
                var matched = apiCommands
                    .Where(cmd => cmd.Contains(filter, StringComparison.OrdinalIgnoreCase))
                    .ToArray();

                if (matched.Length == 0)
                {
                    return Task.FromResult(CommandResult.Error($"No API commands matching '{filter}'."));
                }

                message.AppendLine($"API commands matching '{filter}' ({matched.Length}):");
                foreach (var cmd in matched)
                {
                    message.AppendLine($"  {cmd}");
                }
            }
            else
            {
                message.AppendLine($"Available API commands ({apiCommands.Length}):");
                foreach (var cmd in apiCommands)
                {
                    message.AppendLine($"  {cmd}");
                }
            }

            message.AppendLine();
            message.AppendLine("Usage: <CommandName> [--param=value ...]");

            return Task.FromResult(CommandResult.Ok(message.ToString()));
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            if (args.Length <= 1)
            {
                var prefix = args.Length > 0 ? args[0] : string.Empty;
                return ManualApiCommand.GetAllCommandNames(_runtime)
                    .Where(cmd => cmd.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                    .OrderBy(cmd => cmd)
                    .ToArray();
            }

            return Array.Empty<string>();
        }
    }
}
