using System;
using System.Threading.Tasks;

namespace GameTestController.Cli
{
    /// <summary>
    /// Launches the next local test device.
    /// </summary>
    internal sealed class LaunchCommand : ICliCommand
    {
        private readonly ControllerRuntime _runtime;

        public LaunchCommand(ControllerRuntime runtime)
        {
            _runtime = runtime;
        }

        public string Name => "launch";
        public string[] Aliases => Array.Empty<string>();
        public string Description => "Launches the next local test device (DeviceA, then DeviceB)";
        public string Usage => "launch";

        public async Task<CommandResult> ExecuteAsync(string[] args)
        {
            if (args.Length != 0)
            {
                return CommandResult.Error($"Usage: {Usage}");
            }

            try
            {
                string role = await _runtime.LaunchNextLocalDeviceAsync().ConfigureAwait(false);
                return CommandResult.Ok($"Launched local device for {role}.");
            }
            catch (Exception ex)
            {
                return CommandResult.Error(ex.Message);
            }
        }

        public string[] GetCompletions(string[] args, int cursorPosition)
        {
            return Array.Empty<string>();
        }
    }
}
