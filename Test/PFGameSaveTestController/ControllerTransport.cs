using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace PFGameSaveTestController
{
    internal sealed class ControllerTransport
    {
        private readonly WebSocketServer _server;
        private readonly Action<string, bool, bool> _logger;

        public ControllerTransport(Action<string, bool, bool> logger)
        {
            _logger = logger;
            _server = new WebSocketServer
            {
                Logger = logger,
                TextMessageReceived = OnTextMessageReceived,
                BinaryMessageReceived = OnBinaryMessageReceived
            };
            _server.ConnectionsChanged += OnConnectionsChanged;
        }

        public event Action<IReadOnlyCollection<DeviceConnectionInfo>>? ConnectedDevicesChanged;
        public event Action<string, string>? TextReceived;
        public event Action<string, byte[]>? BinaryReceived;

        public Task StartAsync() => _server.StartAsync();

        public Task StopAsync() => _server.StopAsync();

        public Task BroadcastTextAsync(string payload) => _server.BroadcastTextAsync(payload);

        public Task SendTextToDeviceAsync(string deviceName, string payload) => _server.SendTextToDeviceAsync(deviceName, payload);

    public Task SendTextToDeviceAsync(Guid clientId, string payload) => _server.SendTextToDeviceAsync(clientId, payload);

        public Task BroadcastBinaryAsync(byte[] payload) => _server.BroadcastBinaryAsync(payload);

        public Task ResetDeviceAssignmentsAsync() => _server.ResetDeviceAssignmentsAsync();

        public IReadOnlyCollection<string> GetConnectedDeviceNames() => _server.GetConnectedDeviceNames();

        public IReadOnlyCollection<DeviceConnectionInfo> GetConnectedDeviceDetails() => _server.GetConnectedDeviceDetails();

        public bool IsDeviceConnected(string deviceName) => _server.IsDeviceConnected(deviceName);

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
