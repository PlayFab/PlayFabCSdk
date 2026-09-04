using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace GameTestController
{
    internal sealed class ControllerTransport
    {
        private readonly WebSocketServer _server;
        private readonly AgentServer? _agentServer;
        private readonly Action<string, bool, bool> _logger;

        public ControllerTransport(Action<string, bool, bool> logger, int devicePort = 15080, int agentPort = 0)
        {
            _logger = logger;
            _server = new WebSocketServer(devicePort)
            {
                Logger = logger,
                TextMessageReceived = OnTextMessageReceived,
                BinaryMessageReceived = OnBinaryMessageReceived
            };
            _server.ConnectionsChanged += OnConnectionsChanged;

            // Only start agent server if a port is configured (> 0)
            if (agentPort > 0)
            {
                _agentServer = new AgentServer(agentPort, logger);
                _agentServer.AgentConnected += info => AgentConnected?.Invoke(info);
                _agentServer.AgentDisconnected += info => AgentDisconnected?.Invoke(info);
            }
        }

        /// <summary>
        /// Port the device control channel listens on. Off-box devices already reach the
        /// controller here, so it is also the port to host any other device-facing endpoint on.
        /// </summary>
        public int DevicePort => _server.Port;

        public event Action<IReadOnlyCollection<DeviceConnectionInfo>>? ConnectedDevicesChanged;        public event Action<AgentConnectionInfo>? AgentConnected;
        public event Action<AgentConnectionInfo>? AgentDisconnected;
        public event Action<string, string>? TextReceived;
        public event Action<string, byte[]>? BinaryReceived;

        public async Task StartAsync()
        {
            await _server.StartAsync();
            if (_agentServer != null)
            {
                await _agentServer.StartAsync();
            }
        }

        public async Task StopAsync()
        {
            await _server.StopAsync();
            if (_agentServer != null)
            {
                await _agentServer.StopAsync();
            }
        }

        public Task BroadcastTextAsync(string payload) => _server.BroadcastTextAsync(payload);

        public Task SendTextToDeviceAsync(string deviceName, string payload) => _server.SendTextToDeviceAsync(deviceName, payload);

        public Task SendTextToDeviceAsync(Guid clientId, string payload) => _server.SendTextToDeviceAsync(clientId, payload);

        public Task BroadcastBinaryAsync(byte[] payload) => _server.BroadcastBinaryAsync(payload);

        public Task ResetDeviceAssignmentsAsync() => _server.ResetDeviceAssignmentsAsync();

        public void ForceDisconnectDevice(Guid clientId) => _server.ForceDisconnectDevice(clientId);

        public IReadOnlyCollection<string> GetConnectedDeviceNames() => _server.GetConnectedDeviceNames();

        public IReadOnlyCollection<DeviceConnectionInfo> GetConnectedDeviceDetails() => _server.GetConnectedDeviceDetails();

        public bool IsDeviceConnected(string deviceName) => _server.IsDeviceConnected(deviceName);

        // --- Agent operations ---

        /// <summary>
        /// Returns all connected agents. Empty if agent server is not configured.
        /// </summary>
        public IReadOnlyList<AgentConnectionInfo> GetConnectedAgents()
        {
            return _agentServer?.GetConnectedAgents() ?? Array.Empty<AgentConnectionInfo>();
        }

        /// <summary>
        /// Waits until the specified number of agents have connected and sent hello.
        /// </summary>
        public async Task<bool> WaitForAgentsAsync(int requiredCount, int timeoutSeconds)
        {
            if (_agentServer == null)
            {
                _logger("No agent server configured — cannot wait for agents.", false, false);
                return false;
            }

            DateTime deadline = DateTime.UtcNow.AddSeconds(timeoutSeconds);
            int lastCount = 0;

            while (DateTime.UtcNow <= deadline)
            {
                var agents = _agentServer.GetConnectedAgents();
                if (agents.Count != lastCount)
                {
                    lastCount = agents.Count;
                    _logger($"Agents connected: {agents.Count}/{requiredCount} ({string.Join(", ", agents.Select(a => a.MachineName))})", false, false);
                }

                if (agents.Count >= requiredCount)
                    return true;

                await Task.Delay(500).ConfigureAwait(false);
            }

            return _agentServer.GetConnectedAgents().Count >= requiredCount;
        }

        /// <summary>
        /// Sends launchTestApp to all connected agents and waits for acknowledgment.
        /// Returns true only if all agents successfully launched.
        /// </summary>
        public async Task<bool> LaunchAllAgentTestAppsAsync(string[]? args = null, int timeoutSeconds = 30)
        {
            if (_agentServer == null) return false;

            var agents = _agentServer.GetConnectedAgents();
            if (agents.Count == 0) return false;

            _logger($"Sending launchTestApp to {agents.Count} agent(s)...", false, false);

            int devicePort = _server.Port;
            var tasks = agents.Select(a => _agentServer.LaunchTestAppAsync(a.ClientId, args, devicePort, timeoutSeconds)).ToList();
            var results = await Task.WhenAll(tasks).ConfigureAwait(false);

            int successes = results.Count(r => r.Success);
            int failures = results.Length - successes;

            if (failures > 0)
            {
                foreach (var (success, error, _) in results.Where(r => !r.Success))
                {
                    _logger($"Agent launch failure: {error}", false, false);
                }
            }

            return failures == 0;
        }

        /// <summary>
        /// Sends killTestApp to all connected agents.
        /// </summary>
        public async Task KillAllAgentTestAppsAsync()
        {
            if (_agentServer != null)
            {
                await _agentServer.KillAllTestAppsAsync().ConfigureAwait(false);
            }
        }

        private void OnTextMessageReceived(string deviceName, string message)
        {
            if (TextReceived == null)
            {
                _logger($"[{deviceName}] WebSocket Text: {message}", true, false);
                return;
            }

            TextReceived.Invoke(deviceName, message);
        }

        private void OnBinaryMessageReceived(string deviceName, byte[] payload)
        {
            byte[] buffer = payload ?? Array.Empty<byte>();

            if (BinaryReceived == null)
            {
                _logger($"[{deviceName}] WebSocket Binary received ({buffer.Length} bytes)", true, false);
                return;
            }

            BinaryReceived.Invoke(deviceName, buffer);
        }

        private void OnConnectionsChanged(IReadOnlyCollection<DeviceConnectionInfo> devices)
        {
            ConnectedDevicesChanged?.Invoke(devices);
        }
    }
}
