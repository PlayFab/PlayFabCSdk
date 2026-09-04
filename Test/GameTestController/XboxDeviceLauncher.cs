using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;

namespace GameTestController
{
    /// <summary>
    /// Handles auto-discovery, deployment, and launch of GameTestAppXbox on an
    /// Xbox devkit. Used by <see cref="ControllerRuntime"/> when auto-launch is
    /// enabled and a scenario requires engine "xbox".
    ///
    /// Reuses GDK tool helpers from <see cref="DeviceStateController"/>.
    /// </summary>
    internal static class XboxDeviceLauncher
    {
        private const int DeployTimeoutMs = 120_000; // xbapp deploy can be slow

        /// <summary>
        /// Queries the default Xbox devkit address via <c>xbconnect /S</c> (bare System IP).
        /// Returns the IP/hostname string, or null if no default console is configured.
        /// </summary>
        public static string? GetDefaultConsoleAddress()
        {
            try
            {
                var (exitCode, stdout) = DeviceStateController.RunXbToolQuiet("xbconnect.exe", "/S");

                // /S returns just the System address on success (exit 0).
                // Exit 1 with empty/help text means no console configured.
                if (exitCode != 0 || string.IsNullOrWhiteSpace(stdout))
                {
                    return null;
                }

                // The bare output may contain just an IP or hostname, one line.
                string address = stdout.Split('\n', '\r')[0].Trim();
                return string.IsNullOrEmpty(address) ? null : address;
            }
            catch
            {
                // GDK not installed or xbconnect not found — no Xbox available
                return null;
            }
        }

        /// <summary>
        /// Finds this PC's non-loopback IPv4 address that an Xbox devkit can reach.
        /// Mirrors the logic in Step-GetServerIp.ps1.
        /// </summary>
        /// <param name="consoleAddress">
        /// Optional devkit address. When supplied, the address the OS would actually use as the
        /// source when routing to that console is preferred. This matters on machines with more
        /// than one "up" adapter (multi-NIC, or a VPN through which the devkit is reached): the
        /// first enumerated adapter is frequently on a private LAN the devkit cannot route back
        /// to, which makes the app launch but never connect to the controller.
        /// </param>
        public static string? GetLocalServerIp(string? consoleAddress = null)
        {
            string? routedIp = GetRoutedSourceIp(consoleAddress);
            if (!string.IsNullOrEmpty(routedIp))
            {
                return routedIp;
            }

            try
            {
                // Prefer non-virtual, non-loopback interfaces that are up
                var candidates = NetworkInterface.GetAllNetworkInterfaces()
                    .Where(nic => nic.OperationalStatus == OperationalStatus.Up
                        && nic.NetworkInterfaceType != NetworkInterfaceType.Loopback
                        && !nic.Description.Contains("Virtual", StringComparison.OrdinalIgnoreCase)
                        && !nic.Name.Contains("vEthernet", StringComparison.OrdinalIgnoreCase))
                    .SelectMany(nic => nic.GetIPProperties().UnicastAddresses)
                    .Where(addr => addr.Address.AddressFamily == AddressFamily.InterNetwork
                        && !IPAddress.IsLoopback(addr.Address))
                    .Select(addr => addr.Address.ToString())
                    .ToList();

                return candidates.FirstOrDefault();
            }
            catch
            {
                return null;
            }
        }

        /// <summary>
        /// Asks the routing table which local IPv4 address would be used to reach
        /// <paramref name="consoleAddress"/>. Connecting a UDP socket performs no traffic but
        /// binds the socket to the source address the OS selected for that destination.
        /// </summary>
        private static string? GetRoutedSourceIp(string? consoleAddress)
        {
            if (string.IsNullOrWhiteSpace(consoleAddress))
            {
                return null;
            }

            try
            {
                // The address may be a hostname; resolve to IPv4 first.
                IPAddress? destination = null;
                if (!IPAddress.TryParse(consoleAddress, out destination))
                {
                    destination = Dns.GetHostAddresses(consoleAddress)
                        .FirstOrDefault(a => a.AddressFamily == AddressFamily.InterNetwork);
                }

                if (destination == null || destination.AddressFamily != AddressFamily.InterNetwork)
                {
                    return null;
                }

                using var probe = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                probe.Connect(destination, 15080);
                if (probe.LocalEndPoint is IPEndPoint local
                    && !IPAddress.IsLoopback(local.Address)
                    && !local.Address.Equals(IPAddress.Any))
                {
                    return local.Address.ToString();
                }
            }
            catch
            {
                // Fall back to adapter enumeration below.
            }

            return null;
        }

        /// <summary>
        /// Locates the GameTestAppXbox loose-file layout directory, searching
        /// both local build output (<c>Out\Gaming.Xbox.Scarlett.x64\{cfg}\GameTestAppXbox\</c>)
        /// and the controller's sibling directories (artifact mode).
        /// </summary>
        public static string? ResolveXboxLayoutPath()
        {
            string baseDirectory = AppContext.BaseDirectory ?? AppDomain.CurrentDomain.BaseDirectory;

            // Local build mode: controller is at Out\x64\{cfg}\GameTestController\
            // Xbox layout is at Out\Gaming.Xbox.Scarlett.x64\{cfg}\GameTestAppXbox\
            string outDir = Path.GetFullPath(Path.Combine(baseDirectory, "..", "..", ".."));

            string[] candidateRelPaths = new[]
            {
                Path.Combine("Gaming.Xbox.Scarlett.x64", "Debug", "GameTestAppXbox"),
                Path.Combine("Gaming.Xbox.Scarlett.x64", "Release", "GameTestAppXbox"),
            };

            // Pick the most recently built layout
            string? best = null;
            DateTime bestTime = DateTime.MinValue;

            foreach (string relPath in candidateRelPaths)
            {
                string fullPath = Path.GetFullPath(Path.Combine(outDir, relPath));
                string exePath = Path.Combine(fullPath, "GameTestAppXbox.exe");
                if (File.Exists(exePath))
                {
                    DateTime writeTime = File.GetLastWriteTimeUtc(exePath);
                    if (writeTime > bestTime)
                    {
                        best = fullPath;
                        bestTime = writeTime;
                    }
                }
            }

            if (best != null)
            {
                return best;
            }

            // Artifact mode: controller may be in a flat directory alongside
            // the Xbox layout. Look for GameTestAppXbox.exe in sibling dirs.
            string? parentDir = Path.GetDirectoryName(baseDirectory.TrimEnd(Path.DirectorySeparatorChar));
            if (parentDir != null && Directory.Exists(parentDir))
            {
                foreach (string dir in Directory.GetDirectories(parentDir))
                {
                    string candidate = Path.Combine(dir, "GameTestAppXbox.exe");
                    if (File.Exists(candidate))
                    {
                        return dir;
                    }
                }
            }

            return null;
        }

        /// <summary>
        /// Writes controllerip.txt into the Xbox layout directory so the app
        /// knows where to connect on launch.
        /// </summary>
        public static void WriteControllerIp(string layoutDir, string serverIp, Action<string> log)
        {
            string controllerIpFile = Path.Combine(layoutDir, "controllerip.txt");
            File.WriteAllText(controllerIpFile, serverIp);
            log($"Xbox auto-launch: wrote server IP '{serverIp}' to '{controllerIpFile}'.");
        }

        /// <summary>
        /// Deploys the Xbox app layout to the devkit via <c>xbapp deploy</c>.
        /// </summary>
        public static void Deploy(string layoutDir, string consoleAddress, Action<string> log)
        {
            log($"Xbox auto-launch: deploying '{layoutDir}' to console '{consoleAddress}'...");
            DeviceStateController.RunXbTool(
                "xbapp.exe",
                $"/x:{consoleAddress} deploy \"{layoutDir}\"",
                log,
                timeoutMs: DeployTimeoutMs);
            log($"Xbox auto-launch: deploy succeeded.");
        }

        /// <summary>
        /// Launches the test app on the Xbox devkit via <c>xbapp launch</c>.
        /// </summary>
        public static void Launch(string consoleAddress, Action<string> log)
        {
            log($"Xbox auto-launch: launching {DeviceStateController.AppAUMID} on '{consoleAddress}'...");
            DeviceStateController.RunXbTool(
                "xbapp.exe",
                $"/x:{consoleAddress} launch {DeviceStateController.AppAUMID}",
                log);
            log($"Xbox auto-launch: app launched.");
        }

        /// <summary>
        /// Terminates the test app on the Xbox devkit via <c>xbapp terminate</c>.
        /// Used to kill a hung device before relaunching.
        /// </summary>
        public static void Terminate(string consoleAddress, Action<string> log)
        {
            log($"Xbox auto-launch: terminating {DeviceStateController.AppPFN} on '{consoleAddress}'...");
            DeviceStateController.RunXbTool(
                "xbapp.exe",
                $"/x:{consoleAddress} terminate {DeviceStateController.AppPFN}",
                log);
            log($"Xbox auto-launch: app terminated.");
        }
    }
}
