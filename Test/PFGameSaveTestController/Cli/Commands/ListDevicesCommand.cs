using System;
using System.Linq;
using System.Threading.Tasks;

namespace PFGameSaveTestController.Cli
{
    /// <summary>
    /// Lists all connected devices
    /// </summary>
    internal sealed class ListDevicesCommand : ICliCommand
    {
        private readonly ControllerRuntime _runtime;

        public ListDevicesCommand(ControllerRuntime runtime)
        {
            _runtime = runtime;
        }

        public string Name => "list-devices";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Lists all connected test devices";
        public string Usage => "list-devices";

        public Task<CommandResult> ExecuteAsync(string[] args)
        {
            var devices = _runtime.Transport.GetConnectedDeviceDetails().ToList();

            if (devices.Count == 0)
            {
                return Task.FromResult(CommandResult.Ok("No devices connected."));
            }

            var message = new System.Text.StringBuilder();
            message.AppendLine($"Connected devices ({devices.Count}):");
            message.AppendLine();

            foreach (var device in devices.OrderBy(d => d.DisplayName ?? d.ClientId.ToString()))
            {
                var name = string.IsNullOrWhiteSpace(device.DisplayName) 
                    ? device.ClientId.ToString() 
                    : device.DisplayName;
                
                var status = device.IsConnected 
                    ? (device.CapabilitiesReady ? "Ready" : "Waiting for capabilities")
                    : "Disconnected";

                var engine = !string.IsNullOrWhiteSpace(device.Engine) 
                    ? $" [{device.Engine}]" 
                    : string.Empty;

                message.AppendLine($"  {name}{engine}");
                message.AppendLine($"    Endpoint: {device.RemoteEndpoint ?? "(unknown)"}");
                message.AppendLine($"    Status:   {status}");
                message.AppendLine();
            }

            return Task.FromResult(CommandResult.Ok(message.ToString()));
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            // No arguments to complete
            return Array.Empty<string>();
        }
    }
}
