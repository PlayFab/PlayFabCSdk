using System;
using System.Linq;
using System.Threading.Tasks;

namespace GameTestController.Cli.Commands
{
    /// <summary>
    /// Sets or displays the targeted device for API commands.
    /// </summary>
    internal sealed class TargetCommand : ICliCommand
    {
        private readonly CliEngine _cliEngine;
        private readonly ControllerRuntime _runtime;

        public TargetCommand(CliEngine cliEngine, ControllerRuntime runtime)
        {
            _cliEngine = cliEngine;
            _runtime = runtime;
        }

        public string Name => "target";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Set or show the targeted device for API commands";
        public string Usage => "target [DeviceA|DeviceB]";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length == 0)
            {
                var current = _cliEngine.TargetDeviceName ?? "(first connected)";
                return Task.FromResult(CommandResult.Ok($"Current target: {current}"));
            }

            var requested = args[0];

            // Allow clearing the target
            if (string.Equals(requested, "auto", StringComparison.OrdinalIgnoreCase) ||
                string.Equals(requested, "first", StringComparison.OrdinalIgnoreCase))
            {
                _cliEngine.TargetDeviceName = null;
                return Task.FromResult(CommandResult.Ok("Target reset to first connected device."));
            }

            // Validate the device exists
            var devices = _runtime.Transport.GetConnectedDeviceDetails();
            var match = devices.FirstOrDefault(d =>
                string.Equals(d.DisplayName, requested, StringComparison.OrdinalIgnoreCase));

            if (string.IsNullOrWhiteSpace(match.DisplayName))
            {
                var available = devices.Any()
                    ? string.Join(", ", devices.Select(d => d.DisplayName))
                    : "none connected";
                return Task.FromResult(CommandResult.Error(
                    $"No device named '{requested}'. Connected: {available}"));
            }

            _cliEngine.TargetDeviceName = match.DisplayName;
            return Task.FromResult(CommandResult.Ok($"Target set to {match.DisplayName} [{match.Engine}]."));
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            if (args.Length <= 1)
            {
                var prefix = args.Length > 0 ? args[0] : string.Empty;
                var devices = _runtime.Transport.GetConnectedDeviceDetails();
                return devices
                    .Select(d => d.DisplayName)
                    .Where(n => !string.IsNullOrWhiteSpace(n))
                    .Append("auto")
                    .Where(n => n.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                    .OrderBy(n => n)
                    .ToArray();
            }

            return Array.Empty<string>();
        }
    }
}
