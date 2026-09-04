using System;
using System.Net;

namespace GameTestController
{
    /// <summary>
    /// Shared binding logic for the controller's device-facing test servers
    /// (<see cref="HttpTestServer"/> and <see cref="WebSocketEchoServer"/>).
    ///
    /// These servers exist to give scenarios an endpoint they fully control. A device app running
    /// on this machine can reach a localhost bind, but an Xbox devkit is a separate machine and
    /// must reach the controller over the LAN, which requires a wildcard bind - and a wildcard
    /// bind requires an HTTP.sys URL ACL.
    ///
    /// URL ACL reservations are prefix-based, so hosting under the device control channel's
    /// existing "http://+:15080/ws/" reservation binds without admin. An arbitrary dynamically
    /// chosen port has no reservation and is denied, so callers that need off-box reachability
    /// should host under that prefix; see ScenarioRunner's server start commands.
    /// </summary>
    internal static class TestServerBinding
    {
        /// <summary>
        /// Creates and starts an <see cref="HttpListener"/> for <paramref name="port"/> and
        /// <paramref name="path"/>.
        ///
        /// When <paramref name="preferWildcard"/> is false the listener binds localhost directly,
        /// which is all a device app on this machine needs and avoids a wildcard attempt that is
        /// expected to be denied.
        ///
        /// When true, a wildcard bind is attempted first and falls back to localhost if HTTP.sys
        /// denies it. The fallback keeps things running on machines without a matching URL ACL;
        /// only off-box devices lose reachability, which callers can detect via
        /// <paramref name="boundToWildcard"/>.
        /// </summary>
        internal static HttpListener StartListener(int port, string path, string serverName, bool preferWildcard, Action<string>? logger, out bool boundToWildcard)
        {
            // A path that trims to nothing is the server root. Naively re-wrapping it would
            // produce the invalid prefix "//".
            string trimmed = path.Trim('/');
            string normalizedPath = trimmed.Length == 0 ? "/" : "/" + trimmed + "/";
            string localhostPrefix = $"http://localhost:{port}{normalizedPath}";

            if (!preferWildcard)
            {
                var localListener = new HttpListener();
                localListener.Prefixes.Add(localhostPrefix);
                localListener.Start();
                boundToWildcard = false;
                return localListener;
            }

            string wildcardPrefix = $"http://+:{port}{normalizedPath}";

            var listener = new HttpListener();
            try
            {
                listener.Prefixes.Add(wildcardPrefix);
                listener.Start();
                boundToWildcard = true;
                return listener;
            }
            catch (HttpListenerException ex)
            {
                logger?.Invoke($"{serverName}: bind of '{wildcardPrefix}' denied ({ex.Message}); falling back to localhost. Off-box devices will not be able to reach this endpoint.");

                // A failed Start() disposes the listener, so the fallback needs a fresh one.
                listener.Close();

                listener = new HttpListener();
                listener.Prefixes.Add(localhostPrefix);
                listener.Start();
                boundToWildcard = false;
                return listener;
            }
        }
    }
}
