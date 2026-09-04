using System;
using System.Collections.Concurrent;
using System.Net;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GameTestController
{
    /// <summary>
    /// WebSocket echo server for test scenarios. Echoes text and binary messages
    /// back to the sender. Supports server-initiated close.
    /// </summary>
    internal sealed class WebSocketEchoServer : IDisposable
    {
        private HttpListener _listener = new();
        private readonly CancellationTokenSource _cts = new();
        private readonly ConcurrentDictionary<Guid, WebSocket> _clients = new();
        private readonly string _path;
        private readonly bool _preferWildcard;
        private Task? _acceptLoop;

        public int Port { get; }

        /// <summary>
        /// True when the listener bound a wildcard prefix and is therefore reachable from off-box
        /// devices; false when it fell back to a localhost-only bind.
        /// </summary>
        public bool BoundToWildcard { get; private set; }

        public Action<string>? Logger { get; set; }

        /// <param name="port">Port to listen on.</param>
        /// <param name="path">
        /// Absolute path to serve the echo endpoint at, e.g. "/echo" or "/ws/echo".
        /// </param>
        public WebSocketEchoServer(int port, string path = "/echo", bool preferWildcard = false)
        {
            Port = port;
            string trimmed = path.Trim('/');
            _path = trimmed.Length == 0 ? "/" : "/" + trimmed + "/";
            _preferWildcard = preferWildcard;
        }

        /// <summary>
        /// Binds and starts the listener. See <see cref="TestServerBinding"/> for why the wildcard
        /// bind matters and when it falls back to localhost.
        /// </summary>
        public void Start()
        {
            _listener = TestServerBinding.StartListener(Port, _path, nameof(WebSocketEchoServer), _preferWildcard, Logger, out bool boundToWildcard);
            BoundToWildcard = boundToWildcard;

            _acceptLoop = Task.Run(() => AcceptLoopAsync(_cts.Token));
            Logger?.Invoke($"WebSocketEchoServer started on port {Port} at '{_path}' (wildcard={BoundToWildcard}).");
        }

        public async Task StopAsync()
        {
            _cts.Cancel();
            if (_listener.IsListening)
            {
                _listener.Stop();
            }
            _listener.Close();

            foreach (var kvp in _clients)
            {
                try
                {
                    if (kvp.Value.State == WebSocketState.Open)
                    {
                        await kvp.Value.CloseAsync(WebSocketCloseStatus.NormalClosure, "Server stopping", CancellationToken.None).ConfigureAwait(false);
                    }
                }
                catch { }
                finally
                {
                    kvp.Value.Dispose();
                }
            }
            _clients.Clear();

            if (_acceptLoop != null)
            {
                try { await _acceptLoop.ConfigureAwait(false); } catch { /* expected */ }
            }

            Logger?.Invoke("WebSocketEchoServer stopped.");
        }

        /// <summary>
        /// Initiates a server-side close on all connected clients.
        /// </summary>
        public async Task CloseAllClientsAsync(WebSocketCloseStatus status = WebSocketCloseStatus.NormalClosure, string description = "Server closing connection")
        {
            foreach (var kvp in _clients)
            {
                try
                {
                    if (kvp.Value.State == WebSocketState.Open)
                    {
                        await kvp.Value.CloseOutputAsync(status, description, CancellationToken.None).ConfigureAwait(false);
                        Logger?.Invoke($"WebSocketEchoServer: initiated close for client {kvp.Key}.");
                    }
                }
                catch (Exception ex)
                {
                    Logger?.Invoke($"WebSocketEchoServer: error closing client {kvp.Key}: {ex.Message}");
                }
            }
        }

        public void Dispose()
        {
            _cts.Cancel();
            if (_listener.IsListening)
            {
                _listener.Stop();
            }
            _listener.Close();
            foreach (var kvp in _clients)
            {
                kvp.Value.Dispose();
            }
            _clients.Clear();
            _cts.Dispose();
        }

        private async Task AcceptLoopAsync(CancellationToken ct)
        {
            while (!ct.IsCancellationRequested)
            {
                HttpListenerContext context;
                try
                {
                    context = await _listener.GetContextAsync().ConfigureAwait(false);
                }
                catch (HttpListenerException) { break; }
                catch (ObjectDisposedException) { break; }
                catch (OperationCanceledException) { break; }

                if (!context.Request.IsWebSocketRequest)
                {
                    context.Response.StatusCode = 400;
                    context.Response.Close();
                    continue;
                }

                _ = Task.Run(() => HandleClientAsync(context));
            }
        }

        private async Task HandleClientAsync(HttpListenerContext context)
        {
            var clientId = Guid.NewGuid();
            try
            {
                var wsContext = await context.AcceptWebSocketAsync(subProtocol: null).ConfigureAwait(false);
                var socket = wsContext.WebSocket;
                _clients[clientId] = socket;
                Logger?.Invoke($"WebSocketEchoServer: client {clientId} connected.");

                var buffer = new byte[8192];
                while (socket.State == WebSocketState.Open && !_cts.IsCancellationRequested)
                {
                    WebSocketReceiveResult result;
                    try
                    {
                        result = await socket.ReceiveAsync(new ArraySegment<byte>(buffer), _cts.Token).ConfigureAwait(false);
                    }
                    catch (OperationCanceledException) { break; }
                    catch (WebSocketException) { break; }

                    if (result.MessageType == WebSocketMessageType.Close)
                    {
                        try
                        {
                            await socket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Echo close", CancellationToken.None).ConfigureAwait(false);
                        }
                        catch { }
                        break;
                    }

                    // Echo the message back
                    await socket.SendAsync(
                        new ArraySegment<byte>(buffer, 0, result.Count),
                        result.MessageType,
                        result.EndOfMessage,
                        CancellationToken.None).ConfigureAwait(false);
                }
            }
            catch (Exception ex)
            {
                Logger?.Invoke($"WebSocketEchoServer: client {clientId} error: {ex.Message}");
            }
            finally
            {
                if (_clients.TryRemove(clientId, out var removed))
                {
                    removed.Dispose();
                }
                Logger?.Invoke($"WebSocketEchoServer: client {clientId} disconnected.");
            }
        }
    }
}
