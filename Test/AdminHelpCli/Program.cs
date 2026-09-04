using System.Diagnostics;
using System.IO.Pipes;
using System.Text;
using System.Text.Json;

namespace PlayFab.Test.AdminHelpCli;

/// <summary>
/// CLI client for AdminHelper. Sends a single action and prints the result.
///
/// Usage:
///   AdminHelpCli &lt;action&gt; [key=value ...]
///
/// Examples:
///   AdminHelpCli Ping
///   AdminHelpCli DisableNetwork
///   AdminHelpCli EnableNetwork
///   AdminHelpCli StopGamingServices
///   AdminHelpCli StartGamingServices
///   AdminHelpCli NetworkFlapping cycles=3 intervalMs=1000
///
/// Exit codes:
///   0 = success
///   1 = AdminHelper reported failure
///   2 = connection error (AdminHelper not running)
///   3 = usage error
/// </summary>
internal static class Program
{
    private const string PipeName = "PlayFabTestAdminHelper";
    private const string TrayPipeName = "AdminHelperLauncher";
    private const int ConnectTimeoutMs = 5_000;
    private const int ResponseTimeoutMs = 60_000;

    private static int Main(string[] args)
    {
        if (args.Length == 0 || args[0] is "-h" or "--help" or "/?" or "help")
        {
            PrintUsage();
            return 3;
        }

        string action = args[0];

        // Local commands (don't go through the pipe)
        if (action.Equals("start", StringComparison.OrdinalIgnoreCase))
            return StartAdminHelper();
        if (action.Equals("stop", StringComparison.OrdinalIgnoreCase))
            return StopAdminHelper();
        if (action.Equals("start-tray", StringComparison.OrdinalIgnoreCase))
            return StartTrayApp();

        Dictionary<string, object>? parameters = null;

        if (args.Length > 1)
        {
            parameters = new Dictionary<string, object>();
            for (int i = 1; i < args.Length; i++)
            {
                string arg = args[i];
                int eq = arg.IndexOf('=');
                if (eq <= 0)
                {
                    Console.Error.WriteLine($"ERROR: Invalid parameter '{arg}'. Expected key=value format.");
                    return 3;
                }
                string key = arg[..eq];
                string value = arg[(eq + 1)..];
                // Try to parse as int for numeric params
                if (int.TryParse(value, out int intVal))
                    parameters[key] = intVal;
                else
                    parameters[key] = value;
            }
        }

        return SendAction(action, parameters);
    }

    private static int SendAction(string action, Dictionary<string, object>? parameters)
    {
        NamedPipeClientStream? client = null;
        try
        {
            client = new NamedPipeClientStream(".", PipeName, PipeDirection.InOut);

            try
            {
                client.Connect(ConnectTimeoutMs);
            }
            catch (TimeoutException)
            {
                client.Dispose();

                // Auto-launch AdminHelper and retry with a fresh pipe
                Console.Error.WriteLine("AdminHelper not running — launching automatically...");
                int startResult = StartAdminHelper();
                if (startResult != 0)
                    return startResult;

                client = new NamedPipeClientStream(".", PipeName, PipeDirection.InOut);
                try
                {
                    client.Connect(ConnectTimeoutMs);
                }
                catch (TimeoutException)
                {
                    Console.Error.WriteLine("ERROR: AdminHelper launched but pipe still unavailable.");
                    return 2;
                }
            }

            // Send request
            var request = new { action, parameters };
            string json = JsonSerializer.Serialize(request);
            byte[] requestBytes = Encoding.UTF8.GetBytes(json + "\n");
            client.Write(requestBytes, 0, requestBytes.Length);
            client.Flush();

            // Read response
            var sb = new StringBuilder();
            var sw = Stopwatch.StartNew();
            int b;
            while ((b = client.ReadByte()) != -1)
            {
                if (b == '\n') break;
                sb.Append((char)b);
                if (sw.ElapsedMilliseconds > ResponseTimeoutMs)
                {
                    Console.Error.WriteLine("ERROR: AdminHelper response timed out.");
                    return 1;
                }
            }

            string responseLine = sb.ToString().TrimEnd('\r');
            if (string.IsNullOrWhiteSpace(responseLine))
            {
                Console.Error.WriteLine("ERROR: AdminHelper returned empty response.");
                return 1;
            }

            using var doc = JsonDocument.Parse(responseLine);
            bool success = doc.RootElement.GetProperty("success").GetBoolean();
            string? message = doc.RootElement.TryGetProperty("message", out var msgProp)
                ? msgProp.GetString()
                : null;

            if (success)
            {
                Console.WriteLine(message ?? "OK");
                return 0;
            }
            else
            {
                Console.Error.WriteLine($"FAILED: {message}");
                return 1;
            }
        }
        catch (IOException ex)
        {
            Console.Error.WriteLine($"ERROR: Cannot connect to AdminHelper — {ex.Message}");
            return 2;
        }
        finally
        {
            client?.Dispose();
        }
    }

    private static int StartAdminHelper()
    {
        // Check if already running
        try
        {
            using var probe = new NamedPipeClientStream(".", PipeName, PipeDirection.InOut);
            probe.Connect(500);
            Console.WriteLine("AdminHelper is already running.");
            return 0;
        }
        catch { }

        // Try the tray app first — no UAC prompt needed if it's running
        if (TryLaunchViaTrayApp())
        {
            Console.WriteLine("AdminHelper started via tray app (no UAC prompt).");
            return 0;
        }

        // Fall back to direct launch with UAC elevation
        string cliDir = AppContext.BaseDirectory;
        string adminHelperPath = Path.GetFullPath(Path.Combine(cliDir, "..", "AdminHelper", "AdminHelper.exe"));

        if (!File.Exists(adminHelperPath))
        {
            Console.Error.WriteLine($"ERROR: AdminHelper.exe not found at: {adminHelperPath}");
            return 2;
        }

        Console.WriteLine($"Launching AdminHelper (UAC elevation required)...");
        try
        {
            var psi = new ProcessStartInfo
            {
                FileName = adminHelperPath,
                UseShellExecute = true,
                Verb = "runas",
                WorkingDirectory = Path.GetDirectoryName(adminHelperPath)!
            };
            Process.Start(psi);
        }
        catch (System.ComponentModel.Win32Exception ex) when (ex.NativeErrorCode == 1223)
        {
            Console.Error.WriteLine("ERROR: UAC elevation was cancelled by user.");
            return 1;
        }

        // Wait for pipe to become available
        var sw = Stopwatch.StartNew();
        while (sw.ElapsedMilliseconds < 10_000)
        {
            Thread.Sleep(500);
            try
            {
                using var probe = new NamedPipeClientStream(".", PipeName, PipeDirection.InOut);
                probe.Connect(200);
                Console.WriteLine("AdminHelper started.");
                return 0;
            }
            catch { }
        }

        Console.Error.WriteLine("ERROR: AdminHelper launched but pipe not available after 10s.");
        return 1;
    }

    /// <summary>
    /// Attempts to launch AdminHelper via the AdminHelperTray app (already elevated).
    /// Returns true if the tray app handled the request and AdminHelper's pipe is ready.
    /// </summary>
    private static bool TryLaunchViaTrayApp()
    {
        try
        {
            using var client = new NamedPipeClientStream(".", TrayPipeName, PipeDirection.InOut);
            client.Connect(1_000);

            // Send LaunchAdminHelper request
            string json = JsonSerializer.Serialize(new { action = "LaunchAdminHelper" });
            byte[] requestBytes = Encoding.UTF8.GetBytes(json + "\n");
            client.Write(requestBytes, 0, requestBytes.Length);
            client.Flush();

            // Read response
            var sb = new StringBuilder();
            var sw = Stopwatch.StartNew();
            int b;
            while ((b = client.ReadByte()) != -1)
            {
                if (b == '\n') break;
                sb.Append((char)b);
                if (sw.ElapsedMilliseconds > 15_000)
                    return false;
            }

            string responseLine = sb.ToString().TrimEnd('\r');
            if (string.IsNullOrWhiteSpace(responseLine))
                return false;

            using var doc = JsonDocument.Parse(responseLine);
            return doc.RootElement.TryGetProperty("success", out var successProp) && successProp.GetBoolean();
        }
        catch
        {
            // Tray app not running — fall through to runas
            return false;
        }
    }

    private static int StopAdminHelper()
    {
        var processes = Process.GetProcessesByName("AdminHelper");
        if (processes.Length == 0)
        {
            Console.WriteLine("AdminHelper is not running.");
            return 0;
        }

        foreach (var proc in processes)
        {
            try
            {
                Console.WriteLine($"Stopping AdminHelper (PID {proc.Id})...");
                proc.Kill();
                proc.WaitForExit(5000);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"ERROR: Failed to stop PID {proc.Id} — {ex.Message}");
            }
            finally
            {
                proc.Dispose();
            }
        }

        Console.WriteLine("AdminHelper stopped.");
        return 0;
    }

    private static int StartTrayApp()
    {
        const string trayExePath = @"C:\git\PlayFab.C\Out\x64\Debug\AdminHelperTray\AdminHelperTray.exe";

        // Check if already running by probing its pipe
        try
        {
            using var probe = new NamedPipeClientStream(".", TrayPipeName, PipeDirection.InOut);
            probe.Connect(500);
            Console.WriteLine("AdminHelperTray is already running.");
            return 0;
        }
        catch { }

        if (!File.Exists(trayExePath))
        {
            Console.Error.WriteLine($"ERROR: AdminHelperTray.exe not found at: {trayExePath}");
            return 2;
        }

        Console.WriteLine("Launching AdminHelperTray (UAC elevation required)...");
        try
        {
            var psi = new ProcessStartInfo
            {
                FileName = trayExePath,
                UseShellExecute = true,
                Verb = "runas",
                WorkingDirectory = Path.GetDirectoryName(trayExePath)!
            };
            Process.Start(psi);
        }
        catch (System.ComponentModel.Win32Exception ex) when (ex.NativeErrorCode == 1223)
        {
            Console.Error.WriteLine("ERROR: UAC elevation was cancelled by user.");
            return 1;
        }

        // Wait for tray app's pipe to become available
        var sw = Stopwatch.StartNew();
        while (sw.ElapsedMilliseconds < 10_000)
        {
            Thread.Sleep(500);
            try
            {
                using var probe = new NamedPipeClientStream(".", TrayPipeName, PipeDirection.InOut);
                probe.Connect(500);
                Console.WriteLine("AdminHelperTray started.");
                return 0;
            }
            catch { }
        }

        Console.Error.WriteLine("ERROR: AdminHelperTray launched but pipe not available after 10s.");
        return 1;
    }

    private static void PrintUsage()
    {
        Console.WriteLine("""
            AdminHelpCli — CLI client for PlayFab Test AdminHelper

            Usage:
              AdminHelpCli <action> [key=value ...]

            Actions:
              start                         Launch AdminHelper (with UAC elevation)
              stop                          Kill AdminHelper process
              start-tray                    Launch AdminHelperTray (elevated, enables UAC-free AdminHelper starts)
              Ping                          Check if AdminHelper is alive
              DisableNetwork                Disable all physical network adapters
              EnableNetwork                 Enable all physical network adapters
              NetworkFlapping               Flap network (params: cycles=N intervalMs=N)
              StopGamingServices            Stop GamingServices (and dependents)
              StartGamingServices           Start GamingServices
              GameSaveMaintenance           Run gamesave-maintenance.ps1 elevated (param: options="--flag [--flag ...]")
                                            Flags: --fullclean --deletefolder --deletereg --restart
                                                   --inprocgamesaveson|off --inprocxgameruntimeon|off
                                                   --inprocxgameruntimelogson|off --collectlogs

            Examples:
              AdminHelpCli start
              AdminHelpCli Ping
              AdminHelpCli StopGamingServices
              AdminHelpCli GameSaveMaintenance options=--inprocgamesaveson
              AdminHelpCli stop

            Exit codes:
              0 = success
              1 = AdminHelper reported failure
              2 = connection error (AdminHelper not running)
              3 = usage error
            """);
    }
}
