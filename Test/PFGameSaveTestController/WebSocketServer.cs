using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Net;
using System.Net.WebSockets;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;
using System.IO;

namespace PFGameSaveTestController
{
    public sealed class WebSocketServer : IDisposable
    {
        private readonly CancellationTokenSource _cts = new();
        private sealed class ClientSession : IDisposable
        {
            public ClientSession(Guid id, WebSocket socket, string? name, string remoteEndpoint)
            {
                Id = id;
                Socket = socket;
                Name = name;
                RemoteEndpoint = remoteEndpoint;
                Engine = "unknown";
            }

            public Guid Id { get; }
            public WebSocket Socket { get; }
            public string? Name { get; set; }
            public string RemoteEndpoint { get; }
            public string Engine { get; set; }
            public bool CapabilitiesReceived { get; set; }

            public string DisplayName => Name ?? Id.ToString();

            public void Dispose()
            {
                Socket.Dispose();
            }
        }

        private readonly ConcurrentDictionary<Guid, ClientSession> _clients = new();
        private readonly object _assignmentLock = new();
        private Guid? _deviceAClientId;
        private Guid? _deviceBClientId;
        private readonly string _uriPrefix;
        private readonly HttpListener _listener;

        public Action<string, bool, bool>? Logger { get; set; }
        public Action<string, string>? TextMessageReceived { get; set; }
        public Action<string, byte[]>? BinaryMessageReceived { get; set; }
    public event Action<IReadOnlyCollection<DeviceConnectionInfo>>? ConnectionsChanged;

        public WebSocketServer(int port = 5000, string path = "/ws/")
        {
            var normalizedPath = path;
            if (!normalizedPath.StartsWith("/", StringComparison.Ordinal))
            {
                normalizedPath = "/" + normalizedPath;
            }
            if (!normalizedPath.EndsWith("/", StringComparison.Ordinal))
            {
                normalizedPath += "/";
            }

            // Use + to bind to all interfaces (allows connections from other machines)
            // This requires admin privileges or URL ACL: netsh http add urlacl url=http://+:5000/ws/ user=Everyone
            _uriPrefix = $"http://+:{port}{normalizedPath}";

            _listener = new HttpListener();
            _listener.Prefixes.Add(_uriPrefix);
        }

        public async Task StartAsync()
        {
            _listener.Start();

            _ = Task.Run(async () => await AcceptLoopAsync(_cts.Token));
            Log($"WebSocket server started at {_uriPrefix}");
            NotifyConnectionsChanged();
        }

        public async Task StopAsync()
        {
            _cts.Cancel();

            if (_listener.IsListening)
            {
                _listener.Stop();
            }

            _listener.Close();
            Log("WebSocket server stopping...");

            foreach (var pair in _clients)
            {
                try
                {
                    await pair.Value.Socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Server shutting down", CancellationToken.None);
                }
                catch
                {
                    // Ignore cleanup failures.
                }
                finally
                {
                    pair.Value.Dispose();
                }
            }

            _clients.Clear();
            lock (_assignmentLock)
            {
                _deviceAClientId = null;
                _deviceBClientId = null;
            }
            Log("WebSocket server stopped.");
            NotifyConnectionsChanged();
        }

        public async Task BroadcastTextAsync(string message, CancellationToken cancellationToken = default)
        {
            var buffer = Encoding.UTF8.GetBytes(message);
            string? commandName = TryExtractCommandName(message);
            foreach (var client in _clients.Values)
            {
                if (client.Socket.State == WebSocketState.Open)
            {
                if (!string.IsNullOrEmpty(commandName))
                {
                    //// Only log [Started] for async commands
                    //if (commandName.Contains("Async", StringComparison.OrdinalIgnoreCase))
                    //{
                    //    Log($"{client.DisplayName}: {commandName} [Started]");
                    //}
                    Log($"{client.DisplayName}: {message}", true);
                }
                else
                {
                    Log($"{client.DisplayName}: {message}");
                }                    await client.Socket.SendAsync(buffer.AsMemory(), WebSocketMessageType.Text, true, cancellationToken);
                }
            }
        }

        public Task SendTextToDeviceAsync(Guid clientId, string message, CancellationToken cancellationToken = default)
        {
            if (!_clients.TryGetValue(clientId, out ClientSession? target))
            {
                throw new InvalidOperationException($"Device '{clientId}' is not connected.");
            }

            if (target.Socket.State != WebSocketState.Open)
            {
                throw new InvalidOperationException($"Device '{target.DisplayName}' connection is not open.");
            }

            return SendMessageAsync(target, message, cancellationToken);
        }

        public Task SendTextToDeviceAsync(string deviceName, string message, CancellationToken cancellationToken = default)
        {
            if (string.IsNullOrWhiteSpace(deviceName))
            {
                throw new ArgumentException("Device name must be provided.", nameof(deviceName));
            }

            ClientSession? target = FindClientByName(deviceName);
            if (target == null)
            {
                throw new InvalidOperationException($"Device '{deviceName}' is not connected.");
            }

            if (target.Socket.State != WebSocketState.Open)
            {
                throw new InvalidOperationException($"Device '{target.DisplayName}' connection is not open.");
            }

            return SendMessageAsync(target, message, cancellationToken);
        }

        public async Task BroadcastBinaryAsync(byte[] payload, CancellationToken cancellationToken = default)
        {
            string preview = FormatBinaryPreview(payload);
            foreach (var client in _clients.Values)
            {
                if (client.Socket.State == WebSocketState.Open)
                {
                    Log($"Sending binary to {client.DisplayName}: {preview}", true);
                    await client.Socket.SendAsync(payload.AsMemory(), WebSocketMessageType.Binary, true, cancellationToken);
                }
            }
        }

        private async Task SendMessageAsync(ClientSession target, string message, CancellationToken cancellationToken)
        {
            byte[] buffer = Encoding.UTF8.GetBytes(message);
            string? commandName = TryExtractCommandName(message);
            if (!string.IsNullOrEmpty(commandName))
            {
                //// Only log [Started] for async commands
                //if (commandName.Contains("Async", StringComparison.OrdinalIgnoreCase))
                //{
                //    Log($"{target.DisplayName}: {commandName} [Started]");
                //}
                Log($"Sending text to {target.DisplayName}: {message}", true);
            }
            else
            {
                Log($"Sending text to {target.DisplayName}: {message}");
            }

            await target.Socket.SendAsync(buffer.AsMemory(), WebSocketMessageType.Text, true, cancellationToken);
        }

        public IReadOnlyCollection<string> GetConnectedDeviceNames()
        {
            var results = new List<string>();

            foreach (ClientSession session in _clients.Values)
            {
                if (session.Socket.State != WebSocketState.Open)
                {
                    continue;
                }

                if (!string.IsNullOrWhiteSpace(session.Name))
                {
                    results.Add(session.Name);
                }
            }

            return results;
        }

        public bool IsDeviceConnected(string deviceName)
        {
            if (string.IsNullOrWhiteSpace(deviceName))
            {
                return false;
            }

            ClientSession? session = FindClientByName(deviceName);
            return session != null && session.Socket.State == WebSocketState.Open && session.CapabilitiesReceived;
        }

        public async Task ResetDeviceAssignmentsAsync(CancellationToken cancellationToken = default)
        {
            ClientSession? deviceASession = null;
            ClientSession? deviceBSession = null;

            lock (_assignmentLock)
            {
                if (_deviceAClientId.HasValue && _clients.TryGetValue(_deviceAClientId.Value, out var existingA))
                {
                    deviceASession = existingA;
                }

                if (_deviceBClientId.HasValue && _clients.TryGetValue(_deviceBClientId.Value, out var existingB))
                {
                    deviceBSession = existingB;
                }

                _deviceAClientId = null;
                _deviceBClientId = null;
            }

            foreach (var session in new[] { deviceASession, deviceBSession })
            {
                if (session == null)
                {
                    continue;
                }

                try
                {
                    Log($"Closing {session.DisplayName} due to reset.");
                    await session.Socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Scenario reset", cancellationToken);
                }
                catch (Exception ex)
                {
                    Log($"Close failed for {session.DisplayName}: {ex.Message}");
                }
                finally
                {
                    _clients.TryRemove(session.Id, out _);
                    session.Dispose();
                }
            }

            Log("Device assignments cleared.");
            NotifyConnectionsChanged();
        }

        public IReadOnlyCollection<DeviceConnectionInfo> GetConnectedDeviceDetails()
        {
            return CaptureConnectedDevices();
        }

        private IReadOnlyCollection<DeviceConnectionInfo> CaptureConnectedDevices()
        {
            var snapshot = new List<DeviceConnectionInfo>(_clients.Count);

            foreach (ClientSession session in _clients.Values)
            {
                bool isConnected = session.Socket.State == WebSocketState.Open;
                string displayName = session.DisplayName;
                string endpoint = string.IsNullOrWhiteSpace(session.RemoteEndpoint) ? "unknown" : session.RemoteEndpoint;
                string engine = string.IsNullOrWhiteSpace(session.Engine) ? "unknown" : session.Engine;
                snapshot.Add(new DeviceConnectionInfo(session.Id, displayName, endpoint, isConnected, session.CapabilitiesReceived, engine));
            }

            return snapshot;
        }

        private void NotifyConnectionsChanged()
        {
            IReadOnlyCollection<DeviceConnectionInfo> snapshot = CaptureConnectedDevices();
            ConnectionsChanged?.Invoke(snapshot);
        }
    

        private async Task AcceptLoopAsync(CancellationToken cancellationToken)
        {
            while (!cancellationToken.IsCancellationRequested)
            {
                HttpListenerContext context;
                try
                {
                    context = await _listener.GetContextAsync();
                }
                catch (HttpListenerException)
                {
                    break; // Listener stopped.
                }
                catch (ObjectDisposedException)
                {
                    break;
                }

                if (!context.Request.IsWebSocketRequest)
                {
                    string remoteEp = context.Request.RemoteEndPoint?.ToString() ?? "unknown";
                    Log($"Rejected non-WebSocket request from {remoteEp}: {context.Request.HttpMethod} {context.Request.RawUrl} (Upgrade: {context.Request.Headers["Upgrade"]}, Connection: {context.Request.Headers["Connection"]})");
                    context.Response.StatusCode = 400;
                    context.Response.Close();
                    continue;
                }

                _ = Task.Run(async () => await HandleClientAsync(context));
            }
        }

        private async Task HandleClientAsync(HttpListenerContext context)
        {
            string remoteEndpoint = context.Request.RemoteEndPoint?.ToString() ?? "unknown";
            WebSocketContext? webSocketContext = null;
            try
            {
                webSocketContext = await context.AcceptWebSocketAsync(subProtocol: null);
                var socket = webSocketContext.WebSocket;
                var clientId = Guid.NewGuid();
                string? assignedName = AssignDeviceName(clientId);
                var session = new ClientSession(clientId, socket, assignedName, remoteEndpoint);
                _clients[clientId] = session;

                if (!string.IsNullOrEmpty(assignedName))
                {
                    Log($"Client connected: {assignedName} ({remoteEndpoint})");
                    await SendAssignmentMessageAsync(session, assignedName).ConfigureAwait(false);
                }
                else
                {
                    Log($"Client connected: {clientId} ({remoteEndpoint})");
                }

                NotifyConnectionsChanged();

                await ReceiveLoopAsync(session);
            }
            catch (Exception ex)
            {
                context.Response.StatusCode = 500;
                context.Response.Close();
                Log($"Client connection failed ({remoteEndpoint}): {ex.Message}");
            }
        }

        private async Task ReceiveLoopAsync(ClientSession session)
        {
            var buffer = new byte[4096];
            using var messageStream = new MemoryStream();
            try
            {
                while (session.Socket.State == WebSocketState.Open)
                {
                    ValueWebSocketReceiveResult result = await session.Socket.ReceiveAsync(buffer.AsMemory(), _cts.Token);
                    if (result.MessageType == WebSocketMessageType.Close)
                    {
                        await session.Socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
                        continue;
                    }

                    if (result.MessageType == WebSocketMessageType.Text || result.MessageType == WebSocketMessageType.Binary)
                    {
                        if (result.Count > 0)
                        {
                            messageStream.Write(buffer, 0, result.Count);
                        }

                        if (result.EndOfMessage)
                        {
                            byte[] payloadBytes = messageStream.Length > 0
                                ? messageStream.ToArray()
                                : Array.Empty<byte>();

                            DispatchMessage(session, result.MessageType, payloadBytes);
                            messageStream.SetLength(0);
                            messageStream.Position = 0;
                        }
                    }
                    else
                    {
                        messageStream.SetLength(0);
                        messageStream.Position = 0;
                    }
                }
            }
            catch (Exception ex)
            {
                Log($"Receive loop exception for {session.DisplayName} ({session.RemoteEndpoint}): {ex.Message}");
            }
            finally
            {
                _clients.TryRemove(session.Id, out _);
                Log($"Client disconnected: {session.DisplayName} ({session.RemoteEndpoint})");
                ClearAssignment(session.Id);
                session.Dispose();
                NotifyConnectionsChanged();
            }
        }

        public void Dispose()
        {
            _ = StopAsync();
            _cts.Dispose();
        }

        private void Log(string message, bool verbose = false, bool chaosLog = false)
        {
            Logger?.Invoke(message, verbose, chaosLog);
        }

        private void DispatchMessage(ClientSession session, WebSocketMessageType messageType, byte[] payload)
        {
            string deviceName = session.Name ?? session.Id.ToString();
            switch (messageType)
            {
                case WebSocketMessageType.Text:
                    string text = payload.Length == 0 ? string.Empty : Encoding.UTF8.GetString(payload);
                    if (!TryHandleCapabilitiesMessage(session, text))
                    {
                        TextMessageReceived?.Invoke(deviceName, text);
                    }
                    break;
                case WebSocketMessageType.Binary:
                    byte[] copy = payload.Length == 0 ? Array.Empty<byte>() : payload;
                    Log($"Received binary from {deviceName}: {FormatBinaryPreview(copy)}", true);
                    BinaryMessageReceived?.Invoke(deviceName, copy);
                    break;
            }
        }

        private bool TryHandleCapabilitiesMessage(ClientSession session, string payload)
        {
            if (string.IsNullOrWhiteSpace(payload) || payload[0] != '{')
            {
                return false;
            }

            try
            {
                using JsonDocument doc = JsonDocument.Parse(payload);
                if (!doc.RootElement.TryGetProperty("type", out JsonElement typeElement) || typeElement.ValueKind != JsonValueKind.String)
                {
                    return false;
                }

                string? typeValue = typeElement.GetString();
                if (!string.Equals(typeValue, "capabilities", StringComparison.OrdinalIgnoreCase))
                {
                    return false;
                }

                string engine = "unknown";
                bool knownEngine = false;
                if (doc.RootElement.TryGetProperty("engine", out JsonElement engineElement) && engineElement.ValueKind == JsonValueKind.String)
                {
                    engine = NormalizeEngine(engineElement.GetString(), out knownEngine);
                }

                bool changed = !session.CapabilitiesReceived || !string.Equals(session.Engine, engine, StringComparison.OrdinalIgnoreCase);
                session.Engine = engine;
                session.CapabilitiesReceived = true;

                if (changed)
                {
                    Log($"Capabilities received from {session.DisplayName}: engine={engine}");
                    if (!knownEngine)
                    {
                        Log($"Unrecognized engine value supplied by {session.DisplayName}.", true);
                    }

                    NotifyConnectionsChanged();
                }

                return true;
            }
            catch (JsonException ex)
            {
                Log($"Failed to parse capabilities payload from {session.DisplayName}: {ex.Message}", true);
                return false;
            }
        }

        private static string NormalizeEngine(string? engine, out bool recognized)
        {
            recognized = true;
            if (string.IsNullOrWhiteSpace(engine))
            {
                recognized = false;
                return "unknown";
            }

            string normalized = engine.Trim().ToLowerInvariant();
            switch (normalized)
            {
                case "pc-grts":
                    return "pc-grts";
                case "pc-inproc":
                    return "pc-inproc";
                case "pc-inproc-gamesaves":
                    return "pc-inproc-gamesaves";
                case "xbox":
                    return "xbox";
                case "psx":
                    return "psx";
                case "unknown":
                    return "unknown";
                default:
                    recognized = false;
                    return "unknown";
            }
        }

        private async Task SendAssignmentMessageAsync(ClientSession session, string deviceName)
        {
            try
            {
                if (session.Socket.State != WebSocketState.Open)
                {
                    return;
                }

                string payload = $"{{\"type\":\"deviceAssignment\",\"deviceName\":\"{deviceName}\"}}";
                byte[] buffer = Encoding.UTF8.GetBytes(payload);
                await session.Socket.SendAsync(buffer.AsMemory(), WebSocketMessageType.Text, true, _cts.Token).ConfigureAwait(false);
            }
            catch (Exception ex)
            {
                Log($"Failed to send assignment to {session.DisplayName}: {ex.Message}");
            }
        }

        private string? AssignDeviceName(Guid clientId)
        {
            lock (_assignmentLock)
            {
                if (!_deviceAClientId.HasValue)
                {
                    _deviceAClientId = clientId;
                    return "DeviceA";
                }

                if (!_deviceBClientId.HasValue)
                {
                    _deviceBClientId = clientId;
                    return "DeviceB";
                }
            }

            return null;
        }

        private ClientSession? FindClientByName(string deviceName)
        {
            foreach (ClientSession session in _clients.Values)
            {
                if (session.Socket.State != WebSocketState.Open)
                {
                    continue;
                }

                if (!string.IsNullOrWhiteSpace(session.Name) && string.Equals(session.Name, deviceName, StringComparison.OrdinalIgnoreCase))
                {
                    return session;
                }

                if (string.Equals(session.DisplayName, deviceName, StringComparison.OrdinalIgnoreCase))
                {
                    return session;
                }
            }

            return null;
        }

        private void ClearAssignment(Guid clientId)
        {
            lock (_assignmentLock)
            {
                if (_deviceAClientId == clientId)
                {
                    _deviceAClientId = null;
                }
                else if (_deviceBClientId == clientId)
                {
                    _deviceBClientId = null;
                }
            }
        }

        private static string? TryExtractCommandName(string payload)
        {
            if (string.IsNullOrWhiteSpace(payload) || payload[0] != '{')
            {
                return null;
            }

            try
            {
                using JsonDocument doc = JsonDocument.Parse(payload);
                if (doc.RootElement.TryGetProperty("command", out JsonElement commandElement) && commandElement.ValueKind == JsonValueKind.String)
                {
                    string? command = commandElement.GetString();
                    return string.IsNullOrWhiteSpace(command) ? null : command;
                }
            }
            catch (JsonException)
            {
                // Ignore parse errors; caller will fall back to verbose payload logging.
            }

            return null;
        }

        private static string FormatBinaryPreview(IReadOnlyList<byte> payload)
        {
            if (payload.Count == 0)
            {
                return "[size=0]";
            }

            const int previewCount = 8;
            var sb = new StringBuilder();
            sb.Append("[size=");
            sb.Append(payload.Count);
            sb.Append("] bytes=[");

            int count = Math.Min(payload.Count, previewCount);
            for (int i = 0; i < count; i++)
            {
                if (i > 0)
                {
                    sb.Append(' ');
                }
                sb.Append(payload[i].ToString("X2"));
            }

            if (payload.Count > previewCount)
            {
                sb.Append(" ...");
            }

            sb.Append(']');
            return sb.ToString();
        }
    }

    public readonly record struct DeviceConnectionInfo(Guid ClientId, string DisplayName, string RemoteEndpoint, bool IsConnected, bool CapabilitiesReady, string Engine);
}
