using System;
using System.Collections.Concurrent;
using System.IO;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GameTestController
{
    /// <summary>
    /// Lightweight HTTP server for test scenarios. Supports configurable routes
    /// with static or generated response bodies, and an optional per-route response delay.
    /// </summary>
    internal sealed class HttpTestServer : IDisposable
    {
        private HttpListener _listener = new();
        private readonly CancellationTokenSource _cts = new();
        private readonly ConcurrentDictionary<string, RouteConfig> _routes = new(StringComparer.OrdinalIgnoreCase);
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

        private int _inFlight;
        private int _maxInFlight;

        /// <summary>
        /// Highest number of requests this server was handling simultaneously since the last
        /// <see cref="ResetConcurrencyStats"/>.
        /// </summary>
        /// <remarks>
        /// This is the only place concurrency can actually be observed. The client side can prove a
        /// request was queued and later completed, but it cannot prove the cap was enforced - if
        /// admission control were removed entirely, every request would still complete and a
        /// client-side assertion would still pass. Counting arrivals here, while the route's
        /// <c>delayMs</c> holds each one open, is what makes over-concurrency detectable.
        /// </remarks>
        public int MaxInFlight => Volatile.Read(ref _maxInFlight);

        /// <summary>Requests currently being held by the server.</summary>
        public int InFlight => Volatile.Read(ref _inFlight);

        public void ResetConcurrencyStats()
        {
            Interlocked.Exchange(ref _maxInFlight, 0);
            Logger?.Invoke("HttpTestServer concurrency stats reset.");
        }

        /// <param name="port">Port to listen on.</param>
        /// <param name="path">
        /// Base path to serve from, e.g. "/" or "/ws/http". Routes are configured relative to this.
        /// </param>
        public HttpTestServer(int port, string path = "/", bool preferWildcard = false)
        {
            Port = port;
            _path = path;
            _preferWildcard = preferWildcard;
        }

        /// <summary>
        /// Binds and starts the listener. See <see cref="TestServerBinding"/> for why the wildcard
        /// bind matters and when it falls back to localhost.
        /// </summary>
        public void Start()
        {
            _listener = TestServerBinding.StartListener(Port, _path, nameof(HttpTestServer), _preferWildcard, Logger, out bool boundToWildcard);
            BoundToWildcard = boundToWildcard;

            _acceptLoop = Task.Run(() => AcceptLoopAsync(_cts.Token));
            Logger?.Invoke($"HttpTestServer started on port {Port} at '{_path}' (wildcard={BoundToWildcard}).");
        }

        public async Task StopAsync()
        {
            _cts.Cancel();
            if (_listener.IsListening)
            {
                _listener.Stop();
            }
            _listener.Close();

            if (_acceptLoop != null)
            {
                try { await _acceptLoop.ConfigureAwait(false); } catch { /* expected */ }
            }

            _routes.Clear();
            Logger?.Invoke("HttpTestServer stopped.");
        }

        /// <param name="delayMs">
        /// How long to hold the request before sending the response. Lets a scenario keep a request
        /// genuinely in flight for a known window - needed to test behavior at the moment a request
        /// is outstanding (suspend teardown, concurrency limits), which a fast-completing request
        /// cannot do reliably because it finishes before the event under test arrives.
        /// </param>
        public void ConfigureRoute(string method, string path, int statusCode, string? contentType, byte[]? body, int generateBodySizeBytes, int delayMs = 0)
        {
            string key = $"{method.ToUpperInvariant()}:{NormalizePath(path)}";
            _routes[key] = new RouteConfig(statusCode, contentType, body, generateBodySizeBytes, delayMs);
            Logger?.Invoke($"HttpTestServer route configured: {key} -> {statusCode} ({(body != null ? body.Length : generateBodySizeBytes)} bytes, delay {delayMs}ms)");
        }

        public void Dispose()
        {
            _cts.Cancel();
            if (_listener.IsListening)
            {
                _listener.Stop();
            }
            _listener.Close();
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

                _ = Task.Run(() => HandleRequestAsync(context));
            }
        }

        private async Task HandleRequestAsync(HttpListenerContext context)
        {
            try
            {
                string method = context.Request.HttpMethod.ToUpperInvariant();
                string path = NormalizePath(StripBasePath(context.Request.Url?.AbsolutePath ?? "/"));
                string key = $"{method}:{path}";

                if (!_routes.TryGetValue(key, out RouteConfig? route))
                {
                    context.Response.StatusCode = 404;
                    byte[] notFound = Encoding.UTF8.GetBytes($"No route configured for {key}");
                    context.Response.ContentType = "text/plain";
                    context.Response.ContentLength64 = notFound.Length;
                    await context.Response.OutputStream.WriteAsync(notFound).ConfigureAwait(false);
                    context.Response.Close();
                    return;
                }

                // Count only routed requests, and only from the moment the server takes ownership
                // until the response is written. That window is what the route's delayMs widens,
                // so it is the interval during which a concurrency cap is observable.
                int current = Interlocked.Increment(ref _inFlight);
                int observedMax = Volatile.Read(ref _maxInFlight);
                while (current > observedMax)
                {
                    int prior = Interlocked.CompareExchange(ref _maxInFlight, current, observedMax);
                    if (prior == observedMax)
                    {
                        break;
                    }
                    observedMax = prior;
                }

                try
                {
                    // Hold the request open before responding, so the caller has a request that is
                    // genuinely in flight for a known window. Honors cancellation so a server stop
                    // during a delay does not block teardown.
                    if (route.DelayMs > 0)
                    {
                        try
                        {
                            await Task.Delay(route.DelayMs, _cts.Token).ConfigureAwait(false);
                        }
                        catch (OperationCanceledException)
                        {
                            try { context.Response.Abort(); } catch { }
                            return;
                        }
                    }

                    context.Response.StatusCode = route.StatusCode;
                    if (!string.IsNullOrEmpty(route.ContentType))
                    {
                        context.Response.ContentType = route.ContentType;
                    }

                    byte[] responseBody;
                    if (route.Body != null)
                    {
                        responseBody = route.Body;
                    }
                    else if (route.GenerateBodySizeBytes > 0)
                    {
                        responseBody = GenerateBody(route.GenerateBodySizeBytes);
                    }
                    else
                    {
                        responseBody = Array.Empty<byte>();
                    }

                    context.Response.ContentLength64 = responseBody.Length;
                    if (responseBody.Length > 0)
                    {
                        await context.Response.OutputStream.WriteAsync(responseBody).ConfigureAwait(false);
                    }
                    context.Response.Close();
                }
                finally
                {
                    Interlocked.Decrement(ref _inFlight);
                }
            }
            catch (Exception ex)
            {
                Logger?.Invoke($"HttpTestServer error handling request: {ex.Message}");
                try { context.Response.Abort(); } catch { }
            }
        }

        private static byte[] GenerateBody(int size)
        {
            var buffer = new byte[size];
            // Fill with a repeating pattern for verifiability
            for (int i = 0; i < size; i++)
            {
                buffer[i] = (byte)(i % 256);
            }
            return buffer;
        }

        /// <summary>
        /// Removes the server's base path from an incoming request path, so routes are configured
        /// and matched relative to the mount point regardless of where the server is hosted.
        /// </summary>
        private string StripBasePath(string absolutePath)
        {
            string basePath = "/" + _path.Trim('/');
            if (basePath == "/")
            {
                return absolutePath;
            }

            if (absolutePath.StartsWith(basePath, StringComparison.OrdinalIgnoreCase))
            {
                string remainder = absolutePath.Substring(basePath.Length);
                return string.IsNullOrEmpty(remainder) ? "/" : remainder;
            }

            return absolutePath;
        }

        private static string NormalizePath(string path)
        {
            if (string.IsNullOrEmpty(path) || path == "/")
            {
                return "/";
            }
            return path.TrimEnd('/');
        }

        private sealed record RouteConfig(int StatusCode, string? ContentType, byte[]? Body, int GenerateBodySizeBytes, int DelayMs);

        /// <summary>
        /// Finds an available TCP port by binding to port 0.
        /// </summary>
        public static int FindAvailablePort()
        {
            var listener = new System.Net.Sockets.TcpListener(IPAddress.Loopback, 0);
            listener.Start();
            int port = ((IPEndPoint)listener.LocalEndpoint).Port;
            listener.Stop();
            return port;
        }
    }
}
