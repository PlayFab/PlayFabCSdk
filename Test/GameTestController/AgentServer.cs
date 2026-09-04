using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.WebSockets;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

namespace GameTestController
{
    /// <summary>
    /// Information about a connected DeviceAgent.
    /// </summary>
    internal sealed record AgentConnectionInfo(
        Guid ClientId,
        string MachineName,
        string RemoteEndpoint,
        string? TestAppPath,
        bool IsConnected,
        bool TestAppRunning,
        int? TestAppPid);

    /// <summary>
    /// WebSocket server that accepts DeviceAgent connections on a dedicated port.
    /// Handles the agent lifecycle protocol (hello, launchTestApp, killTestApp, getStatus).
    /// </summary>
    internal sealed class AgentServer : IDisposable
    {
        private readonly CancellationTokenSource _cts = new();
        private readonly int _port;
        private readonly HttpListener _listener;
        private readonly string _uriPrefix;
        private readonly ConcurrentDictionary<Guid, AgentSession> _agents = new();
        private readonly Action<string, bool, bool> _logger;

        public event Action<AgentConnectionInfo>? AgentConnected;
        public event Action<AgentConnectionInfo>? AgentDisconnected;

        private sealed class AgentSession : IDisposable
        {
            public Guid Id { get; }
            public WebSocket Socket { get; }
            public string MachineName { get; set; } = "";
            public string RemoteEndpoint { get; }
            public string? TestAppPath { get; set; }
            public bool TestAppRunning { get; set; }
            public int? TestAppPid { get; set; }
            public bool HelloReceived { get; set; }

            // Tracks pending command responses
            public ConcurrentDictionary<string, TaskCompletionSource<JsonElement>> PendingCommands { get; } = new();

            public AgentSession(Guid id, WebSocket socket, string remoteEndpoint)
            {
                Id = id;
                Socket = socket;
                RemoteEndpoint = remoteEndpoint;
            }

            public void Dispose() => Socket.Dispose();
        }

        public AgentServer(int port, Action<string, bool, bool> logger)
        {
            _port = port;
            _logger = logger;

            var path = "/agent/";
            _uriPrefix = $"http://+:{port}{path}";
            _listener = new HttpListener();
            _listener.Prefixes.Add(_uriPrefix);
        }

        public Task StartAsync()
        {
            _listener.Start();
            _ = Task.Run(async () => await AcceptLoopAsync(_cts.Token));
            Log($"Agent server started at {_uriPrefix}");
            return Task.CompletedTask;
        }

        public async Task StopAsync()
        {
            _cts.Cancel();
            if (_listener.IsListening) _listener.Stop();
            _listener.Close();

            foreach (var pair in _agents)
            {
                try
                {
                    if (pair.Value.Socket.State == WebSocketState.Open)
                    {
                        await pair.Value.Socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Server stopping", CancellationToken.None);
                    }
                }
                catch { }
                pair.Value.Dispose();
            }
            _agents.Clear();
            Log("Agent server stopped.");
        }

        /// <summary>
        /// Returns all currently connected agents.
        /// </summary>
        public IReadOnlyList<AgentConnectionInfo> GetConnectedAgents()
        {
            return _agents.Values
                .Where(a => a.Socket.State == WebSocketState.Open && a.HelloReceived)
                .Select(a => new AgentConnectionInfo(
                    a.Id, a.MachineName, a.RemoteEndpoint, a.TestAppPath,
                    IsConnected: true, a.TestAppRunning, a.TestAppPid))
                .ToList();
        }

        /// <summary>
        /// Sends a launchTestApp command to a specific agent and waits for acknowledgment.
        /// </summary>
        public async Task<(bool Success, string? Error, int? Pid)> LaunchTestAppAsync(
            Guid agentId, string[]? args = null, int devicePort = 5000, int timeoutSeconds = 60)
        {
            if (!_agents.TryGetValue(agentId, out var session))
                return (false, "Agent not found", null);

            var commandId = Guid.NewGuid().ToString("N")[..12];
            var command = new
            {
                commandId,
                command = "launchTestApp",
                parameters = new { args, devicePort }
            };

            var tcs = new TaskCompletionSource<JsonElement>(TaskCreationOptions.RunContinuationsAsynchronously);
            session.PendingCommands[commandId] = tcs;

            try
            {
                await SendJsonAsync(session.Socket, command).ConfigureAwait(false);
                Log($"Sent launchTestApp to agent '{session.MachineName}' (commandId={commandId})");

                using var cts = new CancellationTokenSource(TimeSpan.FromSeconds(timeoutSeconds));
                cts.Token.Register(() => tcs.TrySetCanceled());

                var result = await tcs.Task.ConfigureAwait(false);

                string status = result.TryGetProperty("status", out var statusEl) ? statusEl.GetString() ?? "" : "";
                if (status == "success")
                {
                    int? pid = null;
                    if (result.TryGetProperty("result", out var resultEl) &&
                        resultEl.TryGetProperty("pid", out var pidEl))
                    {
                        pid = pidEl.GetInt32();
                    }
                    session.TestAppRunning = true;
                    session.TestAppPid = pid;
                    Log($"Agent '{session.MachineName}' launched test app (PID={pid})");
                    return (true, null, pid);
                }
                else
                {
                    string error = result.TryGetProperty("error", out var errEl) ? errEl.GetString() ?? "Unknown error" : "Unknown error";
                    Log($"Agent '{session.MachineName}' failed to launch test app: {error}");
                    return (false, error, null);
                }
            }
            catch (OperationCanceledException)
            {
                Log($"Timeout waiting for launchTestApp response from agent '{session.MachineName}'");
                return (false, "Timeout waiting for agent response", null);
            }
            catch (Exception ex)
            {
                Log($"Error launching test app on agent '{session.MachineName}': {ex.Message}");
                return (false, $"Agent communication error: {ex.Message}", null);
            }
            finally
            {
                session.PendingCommands.TryRemove(commandId, out _);
            }
        }

        /// <summary>
        /// Sends a killTestApp command to a specific agent.
        /// </summary>
        public async Task<bool> KillTestAppAsync(Guid agentId, int timeoutSeconds = 10)
        {
            if (!_agents.TryGetValue(agentId, out var session))
                return false;

            var commandId = Guid.NewGuid().ToString("N")[..12];
            var command = new { commandId, command = "killTestApp" };

            var tcs = new TaskCompletionSource<JsonElement>(TaskCreationOptions.RunContinuationsAsynchronously);
            session.PendingCommands[commandId] = tcs;

            try
            {
                await SendJsonAsync(session.Socket, command).ConfigureAwait(false);

                using var cts = new CancellationTokenSource(TimeSpan.FromSeconds(timeoutSeconds));
                cts.Token.Register(() => tcs.TrySetCanceled());

                var result = await tcs.Task.ConfigureAwait(false);

                string status = result.TryGetProperty("status", out var statusEl) ? statusEl.GetString() ?? "" : "";
                if (status == "success")
                {
                    session.TestAppRunning = false;
                    session.TestAppPid = null;
                    Log($"Agent '{session.MachineName}' killed test app.");
                    return true;
                }
                else
                {
                    string error = result.TryGetProperty("error", out var errEl) ? errEl.GetString() ?? "Unknown error" : "Unknown error";
                    Log($"Agent '{session.MachineName}' failed to kill test app: {error}");
                    return false;
                }
            }
            catch (OperationCanceledException)
            {
                Log($"Timeout waiting for killTestApp response from agent '{session.MachineName}'");
                return false;
            }
            catch
            {
                return false;
            }
            finally
            {
                session.PendingCommands.TryRemove(commandId, out _);
            }
        }

        /// <summary>
        /// Sends killTestApp to all connected agents.
        /// </summary>
        public async Task KillAllTestAppsAsync()
        {
            var tasks = _agents.Values
                .Where(a => a.Socket.State == WebSocketState.Open && a.TestAppRunning)
                .Select(a => KillTestAppAsync(a.Id))
                .ToList();
            await Task.WhenAll(tasks).ConfigureAwait(false);
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
                catch (HttpListenerException) { break; }
                catch (ObjectDisposedException) { break; }

                if (!context.Request.IsWebSocketRequest)
                {
                    context.Response.StatusCode = 400;
                    context.Response.Close();
                    continue;
                }

                _ = Task.Run(async () => await HandleAgentAsync(context));
            }
        }

        private async Task HandleAgentAsync(HttpListenerContext context)
        {
            string remoteEndpoint = context.Request.RemoteEndPoint?.ToString() ?? "unknown";
            try
            {
                var wsContext = await context.AcceptWebSocketAsync(subProtocol: null);
                var socket = wsContext.WebSocket;
                var agentId = Guid.NewGuid();
                var session = new AgentSession(agentId, socket, remoteEndpoint);
                _agents[agentId] = session;

                Log($"Agent connection from {remoteEndpoint} (id={agentId.ToString()[..8]})");

                await ReceiveLoopAsync(session);
            }
            catch (Exception ex)
            {
                context.Response.StatusCode = 500;
                context.Response.Close();
                Log($"Agent connection failed ({remoteEndpoint}): {ex.Message}");
            }
        }

        private async Task ReceiveLoopAsync(AgentSession session)
        {
            var buffer = new byte[8192];
            var messageBuffer = new System.IO.MemoryStream();
            try
            {
                while (session.Socket.State == WebSocketState.Open && !_cts.IsCancellationRequested)
                {
                    var result = await session.Socket.ReceiveAsync(new ArraySegment<byte>(buffer), _cts.Token);

                    if (result.MessageType == WebSocketMessageType.Close)
                    {
                        await session.Socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
                        break;
                    }

                    if (result.MessageType == WebSocketMessageType.Text)
                    {
                        messageBuffer.Write(buffer, 0, result.Count);

                        if (result.EndOfMessage)
                        {
                            var message = Encoding.UTF8.GetString(messageBuffer.GetBuffer(), 0, (int)messageBuffer.Length);
                            messageBuffer.SetLength(0);
                            HandleMessage(session, message);
                        }
                    }
                    else
                    {
                        // Discard non-text frames
                        messageBuffer.SetLength(0);
                    }
                }
            }
            catch (OperationCanceledException) { }
            catch (Exception ex)
            {
                Log($"Agent receive error ({session.MachineName}/{session.RemoteEndpoint}): {ex.Message}");
            }
            finally
            {
                messageBuffer.Dispose();
                _agents.TryRemove(session.Id, out _);
                var info = new AgentConnectionInfo(
                    session.Id, session.MachineName, session.RemoteEndpoint,
                    session.TestAppPath, IsConnected: false, false, null);
                Log($"Agent disconnected: {session.MachineName} ({session.RemoteEndpoint})");
                AgentDisconnected?.Invoke(info);
                session.Dispose();
            }
        }

        private void HandleMessage(AgentSession session, string rawJson)
        {
            try
            {
                Log($"Agent '{session.MachineName}' message: {rawJson[..Math.Min(200, rawJson.Length)]}");

                using var doc = JsonDocument.Parse(rawJson);
                var root = doc.RootElement;

                string type = root.TryGetProperty("type", out var typeEl) ? typeEl.GetString() ?? "" : "";

                switch (type)
                {
                    case "agentHello":
                        HandleHello(session, root);
                        break;

                    case "agentResponse":
                        HandleResponse(session, root);
                        break;

                    default:
                        Log($"Agent '{session.MachineName}' sent unknown message type: '{type}'");
                        break;
                }
            }
            catch (JsonException ex)
            {
                Log($"Failed to parse agent message from '{session.MachineName}': {ex.Message} — raw: {rawJson[..Math.Min(100, rawJson.Length)]}");
            }
        }

        private void HandleHello(AgentSession session, JsonElement root)
        {
            session.MachineName = root.TryGetProperty("machineName", out var nameEl)
                ? nameEl.GetString() ?? "unknown" : "unknown";
            session.TestAppPath = root.TryGetProperty("testAppPath", out var pathEl)
                ? pathEl.GetString() : null;
            session.HelloReceived = true;

            Log($"Agent registered: '{session.MachineName}' from {session.RemoteEndpoint}" +
                (session.TestAppPath != null ? $" (app: {session.TestAppPath})" : ""));

            var info = new AgentConnectionInfo(
                session.Id, session.MachineName, session.RemoteEndpoint,
                session.TestAppPath, IsConnected: true, false, null);
            AgentConnected?.Invoke(info);
        }

        private void HandleResponse(AgentSession session, JsonElement root)
        {
            string commandId = root.TryGetProperty("commandId", out var idEl) ? idEl.GetString() ?? "" : "";
            string status = root.TryGetProperty("status", out var statusEl) ? statusEl.GetString() ?? "" : "";
            Log($"Agent '{session.MachineName}' response: commandId={commandId}, status={status}, pending={session.PendingCommands.Count}");

            if (session.PendingCommands.TryRemove(commandId, out var tcs))
            {
                tcs.TrySetResult(root.Clone());
            }
            else
            {
                Log($"Agent '{session.MachineName}' sent response for unknown commandId: {commandId} (pending keys: {string.Join(",", session.PendingCommands.Keys)})");
            }
        }

        private async Task SendJsonAsync(WebSocket ws, object payload)
        {
            string json = JsonSerializer.Serialize(payload, new JsonSerializerOptions
            {
                PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
                DefaultIgnoreCondition = System.Text.Json.Serialization.JsonIgnoreCondition.WhenWritingNull
            });
            var bytes = Encoding.UTF8.GetBytes(json);
            await ws.SendAsync(new ArraySegment<byte>(bytes), WebSocketMessageType.Text, true, _cts.Token).ConfigureAwait(false);
        }

        private void Log(string message)
        {
            _logger(message, false, false);
        }

        public void Dispose()
        {
            try
            {
                _cts.Cancel();
                StopAsync().GetAwaiter().GetResult();
            }
            catch { }
            _cts.Dispose();
        }
    }
}
