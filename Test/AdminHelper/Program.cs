using System.Diagnostics;
using System.IO.Pipes;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Text.Json;

namespace PlayFab.Test.AdminHelper;

/// <summary>
/// Elevated helper that listens on a named pipe for privileged test operations.
/// Run once as Administrator; the test controller connects as a non-admin client.
/// </summary>
internal static class Program
{
    internal const string PipeName = "PlayFabTestAdminHelper";
    private const int ConnectTimeoutMs = 500;
    private const int ReadTimeoutMs = 5_000;
    private const int WriteTimeoutMs = 5_000;
    private const int CommandTimeoutMs = 30_000;

    private static bool _networkDisabled = false;

    private static readonly HashSet<string> AllowedActions = new(StringComparer.OrdinalIgnoreCase)
    {
        "DisableNetwork",
        "EnableNetwork",
        "NetworkFlapping",
        "Ping",
        "StopGamingServices",
        "StartGamingServices",
        "GameSaveMaintenance",
    };

    private static readonly JsonSerializerOptions JsonOptions = new()
    {
        PropertyNameCaseInsensitive = true,
        PropertyNamingPolicy = JsonNamingPolicy.CamelCase
    };

    private static void Main(string[] args)
    {
        // Verify we're actually elevated
        using var identity = WindowsIdentity.GetCurrent();
        var principal = new WindowsPrincipal(identity);
        if (!principal.IsInRole(WindowsBuiltInRole.Administrator))
        {
            Console.Error.WriteLine("ERROR: AdminHelper must run as Administrator.");
            Environment.Exit(1);
        }

        Console.WriteLine($"PlayFab Test AdminHelper (elevated)");
        Console.WriteLine($"  Pipe: \\\\.\\pipe\\{PipeName}");
        Console.WriteLine($"  User: {identity.Name}");
        Console.WriteLine($"  PID:  {Environment.ProcessId}");
        Console.WriteLine($"  Allowed actions: {string.Join(", ", AllowedActions)}");
        Console.WriteLine("Listening... (Ctrl+C to stop)");
        Console.WriteLine();
        Console.WriteLine("Keyboard: [1] Toggle network  [e] Enable  [d] Disable  [s] Status  [p] Ping  [g] Restart GRTS  [?] Help");
        Console.WriteLine();

        using var cts = new CancellationTokenSource();
        Console.CancelKeyPress += (_, e) =>
        {
            e.Cancel = true;
            cts.Cancel();
            Console.WriteLine("Shutting down...");
        };

        // Safety: always re-enable network on exit
        AppDomain.CurrentDomain.ProcessExit += (_, _) =>
        {
            TryEnableNetworkSafely();
        };

        // Run pipe listener on background thread so main thread can handle keyboard
        var pipeTask = Task.Run(() => ListenLoop(cts.Token), cts.Token);
        KeyboardLoop(cts.Token);
        pipeTask.Wait(TimeSpan.FromSeconds(5));
    }

    private static void ListenLoop(CancellationToken cancel)
    {
        // Allow the current user (both elevated and non-elevated tokens) to connect.
        // This is more permissive than PipeOptions.CurrentUserOnly (which blocks
        // cross-elevation connections) but still restricts to the logged-in user.
        var pipeSecurity = new PipeSecurity();
        pipeSecurity.AddAccessRule(new PipeAccessRule(
            WindowsIdentity.GetCurrent().User!,
            PipeAccessRights.ReadWrite,
            AccessControlType.Allow));

        while (!cancel.IsCancellationRequested)
        {
            // One connection per request — client connects, sends command, gets response, disconnects.
            var server = NamedPipeServerStreamAcl.Create(
                PipeName,
                PipeDirection.InOut,
                maxNumberOfServerInstances: 1,
                PipeTransmissionMode.Byte,
                PipeOptions.None,
                inBufferSize: 4096,
                outBufferSize: 4096,
                pipeSecurity);

            try
            {
                // Synchronous wait since we're not using PipeOptions.Asynchronous.
                // To support cancellation, we use a background task.
                var connectTask = Task.Run(() => server.WaitForConnection(), cancel);
                connectTask.Wait(cancel);
            }
            catch (OperationCanceledException)
            {
                server.Dispose();
                break;
            }

            try
            {
                HandleConnection(server);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"[{Now()}] Connection error: {ex.Message}");
            }
            finally
            {
                if (server.IsConnected)
                    server.Disconnect();
                server.Dispose();
            }
        }
    }

    private static void KeyboardLoop(CancellationToken cancel)
    {
        while (!cancel.IsCancellationRequested)
        {
            // Poll for key press so we can check cancellation
            if (!Console.KeyAvailable)
            {
                Thread.Sleep(100);
                continue;
            }

            var key = Console.ReadKey(intercept: true);
            try
            {
                switch (key.KeyChar)
                {
                    case '1':
                        if (_networkDisabled)
                        {
                            RunNetworkAdapter(disable: false);
                            _networkDisabled = false;
                            WaitForNetworkReady();
                            Console.WriteLine($"[{Now()}] [KEY] Network ENABLED (toggled on)");
                        }
                        else
                        {
                            RunNetworkAdapter(disable: true);
                            _networkDisabled = true;
                            Console.WriteLine($"[{Now()}] [KEY] Network DISABLED (toggled off)");
                        }
                        break;

                    case 'e':
                    case 'E':
                        RunNetworkAdapter(disable: false);
                        _networkDisabled = false;
                        WaitForNetworkReady();
                        Console.WriteLine($"[{Now()}] [KEY] Network ENABLED");
                        break;

                    case 'd':
                    case 'D':
                        RunNetworkAdapter(disable: true);
                        _networkDisabled = true;
                        Console.WriteLine($"[{Now()}] [KEY] Network DISABLED");
                        break;

                    case 'p':
                    case 'P':
                        Console.WriteLine($"[{Now()}] [KEY] Pong — pipe server is alive, PID {Environment.ProcessId}");
                        break;

                    case 's':
                    case 'S':
                        string status = GetNetworkStatus();
                        Console.WriteLine($"[{Now()}] [KEY] {status}");
                        break;

                    case 'g':
                    case 'G':
                        Console.WriteLine($"[{Now()}] [KEY] Restarting GamingServices...");
                        StopGamingServicesAction();
                        Console.WriteLine($"[{Now()}] [KEY] GamingServices stopped.");
                        StartGamingServicesAction();
                        Console.WriteLine($"[{Now()}] [KEY] GamingServices started.");
                        break;

                    case '?':
                    case 'h':
                    case 'H':
                        Console.WriteLine();
                        Console.WriteLine("  [1] Toggle network (currently " + (_networkDisabled ? "DISABLED" : "ENABLED") + ")");
                        Console.WriteLine("  [e] Force enable network");
                        Console.WriteLine("  [d] Force disable network");
                        Console.WriteLine("  [g] Restart GamingServices (stop + start)");
                        Console.WriteLine("  [p] Self-ping");
                        Console.WriteLine("  [s] Show adapter status");
                        Console.WriteLine("  [?] This help");
                        Console.WriteLine();
                        break;
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"[{Now()}] [KEY] Error: {ex.Message}");
            }
        }
    }

    private static string GetNetworkStatus()
    {
        using var process = new Process();
        process.StartInfo.FileName = "powershell.exe";
        process.StartInfo.Arguments = "-NoProfile -NonInteractive -Command \"Get-NetAdapter -Physical | Select-Object Name, Status, LinkSpeed | Format-Table -AutoSize | Out-String\"";
        process.StartInfo.UseShellExecute = false;
        process.StartInfo.RedirectStandardOutput = true;
        process.StartInfo.CreateNoWindow = true;
        process.Start();
        string output = process.StandardOutput.ReadToEnd().Trim();
        process.WaitForExit(5000);
        return string.IsNullOrWhiteSpace(output) ? "No physical adapters found" : "\n" + output;
    }

    private static void HandleConnection(NamedPipeServerStream server)
    {
        // Note: ReadTimeout/WriteTimeout throw on async pipes, so we rely on
        // the ReadLine timeout logic and command-level timeout instead.

        // Read newline-delimited JSON request
        string? requestLine = ReadLine(server);
        if (string.IsNullOrWhiteSpace(requestLine))
        {
            WriteResponse(server, false, "Empty request");
            return;
        }

        AdminRequest? request;
        try
        {
            request = JsonSerializer.Deserialize<AdminRequest>(requestLine, JsonOptions);
        }
        catch (JsonException ex)
        {
            WriteResponse(server, false, $"Invalid JSON: {ex.Message}");
            return;
        }

        if (request == null || string.IsNullOrWhiteSpace(request.Action))
        {
            WriteResponse(server, false, "Missing 'action' field");
            return;
        }

        if (!AllowedActions.Contains(request.Action))
        {
            WriteResponse(server, false, $"Unknown action '{request.Action}'. Allowed: {string.Join(", ", AllowedActions)}");
            return;
        }

        Console.WriteLine($"[{Now()}] {request.Action} (params: {FormatParams(request.Parameters)})");

        try
        {
            string result = ExecuteAction(request);
            Console.WriteLine($"[{Now()}]   OK: {result}");
            WriteResponse(server, true, result);
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"[{Now()}]   FAIL: {ex.Message}");
            WriteResponse(server, false, ex.Message);
        }
    }

    private static string ExecuteAction(AdminRequest request)
    {
        switch (request.Action!.ToLowerInvariant())
        {
            case "disablenetwork":
                RunNetworkAdapter(disable: true);
                _networkDisabled = true;
                return "Network adapters disabled";

            case "enablenetwork":
                RunNetworkAdapter(disable: false);
                _networkDisabled = false;
                WaitForNetworkReady();
                return "Network adapters enabled and connectivity verified";

            case "networkflapping":
            {
                int cycles = GetIntParam(request.Parameters, "cycles", 5);
                int intervalMs = GetIntParam(request.Parameters, "intervalMs", 500);

                if (cycles < 1 || cycles > 100)
                    throw new ArgumentException($"cycles must be 1-100, got {cycles}");
                if (intervalMs < 100 || intervalMs > 30_000)
                    throw new ArgumentException($"intervalMs must be 100-30000, got {intervalMs}");

                try
                {
                    for (int i = 0; i < cycles; i++)
                    {
                        RunNetworkAdapter(disable: true);
                        Thread.Sleep(intervalMs);
                        RunNetworkAdapter(disable: false);
                        if (i < cycles - 1)
                            Thread.Sleep(intervalMs);
                    }
                }
                catch
                {
                    // Best-effort re-enable on failure
                    TryEnableNetworkSafely();
                    throw;
                }

                return $"Network flapping complete ({cycles} cycles, {intervalMs}ms interval)";
            }

            case "ping":
                return "pong";

            case "stopgamingservices":
                StopGamingServicesAction();
                return "GamingServices stopped";

            case "startgamingservices":
                StartGamingServicesAction();
                return "GamingServices started";

            // Runs Utilities\Scripts\gamesave-maintenance.ps1 with the given flags.
            //
            // Those actions all need elevation (HKLM writes, service restarts, deleting
            // C:\XboxGames\GameSave\pgs), which normally means a UAC prompt per run. This helper is
            // already elevated, so routing them through here lets the test loop do them unattended.
            //
            // Delegates to the script rather than reimplementing it, so the script stays the single
            // source of truth. Flags are validated against the script's own option list first: this
            // helper is reachable over a local pipe, so it must not become a way to run arbitrary
            // PowerShell.
            case "gamesavemaintenance":
            {
                string options = GetStringParam(request.Parameters, "options", "");
                return RunGameSaveMaintenance(options);
            }

            default:
                throw new InvalidOperationException($"Unhandled action: {request.Action}");
        }
    }

    // ──────────────────────────────────────────────────────────────────────
    //  Network adapter control
    // ──────────────────────────────────────────────────────────────────────

    private static readonly HashSet<string> AllowedMaintenanceOptions = new(StringComparer.OrdinalIgnoreCase)
    {
        "--fullclean",
        "--deletefolder",
        "--deletereg",
        "--restart",
        "--inprocgamesaveson",
        "--inprocgamesavesoff",
        "--inprocxgameruntimeon",
        "--inprocxgameruntimeoff",
        "--inprocxgameruntimelogson",
        "--inprocxgameruntimelogsoff",
        "--collectlogs",
    };

    private static string RunGameSaveMaintenance(string options)
    {
        string[] flags = options.Split(new[] { ' ', ',', ';' }, StringSplitOptions.RemoveEmptyEntries);
        if (flags.Length == 0)
        {
            throw new ArgumentException(
                $"No options given. Expected one or more of: {string.Join(", ", AllowedMaintenanceOptions)}");
        }

        foreach (string flag in flags)
        {
            if (!AllowedMaintenanceOptions.Contains(flag))
            {
                throw new ArgumentException(
                    $"Unsupported option '{flag}'. Allowed: {string.Join(", ", AllowedMaintenanceOptions)}");
            }
        }

        string scriptPath = ResolveMaintenanceScriptPath();
        string joinedFlags = string.Join(" ", flags);

        using var maintenance = new Process();
        maintenance.StartInfo.FileName = "powershell.exe";
        maintenance.StartInfo.Arguments =
            $"-NoProfile -NonInteractive -ExecutionPolicy Bypass -File \"{scriptPath}\" {joinedFlags}";
        maintenance.StartInfo.UseShellExecute = false;
        maintenance.StartInfo.RedirectStandardOutput = true;
        maintenance.StartInfo.RedirectStandardError = true;
        maintenance.StartInfo.CreateNoWindow = true;

        maintenance.Start();

        // Start draining both pipes before waiting. Reading stdout to completion first can
        // deadlock: if the script writes enough to stderr to fill that pipe's buffer, it blocks
        // waiting for stderr to be drained, while ReadToEnd() on stdout blocks waiting for output
        // that will never come. Neither side moves, and the CommandTimeoutMs below is never
        // reached because WaitForExit only runs after both reads return.
        Task<string> stdoutTask = maintenance.StandardOutput.ReadToEndAsync();
        Task<string> stderrTask = maintenance.StandardError.ReadToEndAsync();

        if (!maintenance.WaitForExit(CommandTimeoutMs))
        {
            try { maintenance.Kill(); } catch { }
            throw new TimeoutException($"gamesave-maintenance.ps1 timed out after {CommandTimeoutMs}ms");
        }

        // Safe now that the process has exited: both pipes are closed, so these complete.
        string maintStdout = stdoutTask.GetAwaiter().GetResult();
        string maintStderr = stderrTask.GetAwaiter().GetResult();

        if (maintenance.ExitCode != 0)
        {
            string errDetail = string.IsNullOrWhiteSpace(maintStderr) ? "(no stderr)" : maintStderr.Trim();
            throw new InvalidOperationException(
                $"gamesave-maintenance.ps1 failed (exit {maintenance.ExitCode}): {errDetail}");
        }

        string[] outLines = maintStdout.Split('\n', StringSplitOptions.RemoveEmptyEntries);
        string tail = string.Join(" | ", outLines.TakeLast(4).Select(l => l.Trim()));
        return $"gamesave-maintenance.ps1 {joinedFlags} completed. {tail}";
    }

    /// <summary>
    /// AdminHelper.exe runs from &lt;repo&gt;\Out\&lt;platform&gt;\&lt;config&gt;\AdminHelper\, so walk up
    /// to the repo root to find the script rather than hardcoding a path.
    /// </summary>
    private static string ResolveMaintenanceScriptPath()
    {
        var dir = new DirectoryInfo(AppContext.BaseDirectory);
        for (int i = 0; i < 6 && dir != null; i++, dir = dir.Parent)
        {
            string candidate = Path.Combine(dir.FullName, "Utilities", "Scripts", "gamesave-maintenance.ps1");
            if (File.Exists(candidate))
            {
                return candidate;
            }
        }

        throw new FileNotFoundException(
            $"Could not locate Utilities\\Scripts\\gamesave-maintenance.ps1 above '{AppContext.BaseDirectory}'.");
    }

    private static void RunNetworkAdapter(bool disable)
    {
        string script = disable
            ? "Get-NetAdapter -Physical | Where-Object { $_.Status -eq 'Up' } | Disable-NetAdapter -Confirm:$false"
            : "Get-NetAdapter -Physical | Where-Object { $_.Status -eq 'Disabled' } | Enable-NetAdapter -Confirm:$false";

        using var process = new Process();
        process.StartInfo.FileName = "powershell.exe";
        process.StartInfo.Arguments = $"-NoProfile -NonInteractive -ExecutionPolicy Bypass -Command \"{script}\"";
        process.StartInfo.UseShellExecute = false;
        process.StartInfo.RedirectStandardOutput = true;
        process.StartInfo.RedirectStandardError = true;
        process.StartInfo.CreateNoWindow = true;

        process.Start();

        string stdout = process.StandardOutput.ReadToEnd();
        string stderr = process.StandardError.ReadToEnd();

        if (!process.WaitForExit(CommandTimeoutMs))
        {
            try { process.Kill(); } catch { }
            throw new TimeoutException($"PowerShell command timed out after {CommandTimeoutMs}ms");
        }

        if (process.ExitCode != 0)
        {
            string errDetail = string.IsNullOrWhiteSpace(stderr) ? "(no stderr)" : stderr.Trim();
            throw new InvalidOperationException($"PowerShell failed (exit {process.ExitCode}): {errDetail}");
        }
    }

    /// <summary>
    /// After enabling adapters, polls DNS (8.8.8.8) until reachable or timeout.
    /// Ensures the caller doesn't race ahead before connectivity is actually restored.
    /// </summary>
    private static void WaitForNetworkReady(int timeoutMs = 15_000, int pollMs = 500)
    {
        var sw = Stopwatch.StartNew();
        using var ping = new System.Net.NetworkInformation.Ping();
        while (sw.ElapsedMilliseconds < timeoutMs)
        {
            try
            {
                var reply = ping.Send("8.8.8.8", 1000);
                if (reply.Status == System.Net.NetworkInformation.IPStatus.Success)
                {
                    Console.WriteLine($"[{Now()}]   Network ready ({sw.ElapsedMilliseconds}ms, rtt={reply.RoundtripTime}ms)");
                    return;
                }
            }
            catch { }
            Thread.Sleep(pollMs);
        }
        Console.WriteLine($"[{Now()}]   WARNING: Network not confirmed after {timeoutMs}ms — proceeding anyway");
    }

    private static void TryEnableNetworkSafely()
    {
        try
        {
            RunNetworkAdapter(disable: false);
            Console.WriteLine($"[{Now()}] Safety: re-enabled network adapters on exit.");
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"[{Now()}] Safety: failed to re-enable network: {ex.Message}");
        }
    }

    // ──────────────────────────────────────────────────────────────────────
    //  GamingServices control
    // ──────────────────────────────────────────────────────────────────────

    private static void StopGamingServicesAction()
    {
        // Stop-Service -Force also stops dependent services
        string script =
            "Stop-Service -Name GamingServices -Force -ErrorAction Stop; " +
            "$svc = Get-Service -Name GamingServices; " +
            "$svc.WaitForStatus('Stopped', [TimeSpan]::FromSeconds(30))";
        RunServiceCommand(script, "stop");
    }

    private static void StartGamingServicesAction()
    {
        // Start-Service auto-starts dependencies
        string script =
            "Start-Service -Name GamingServices -ErrorAction Stop; " +
            "$svc = Get-Service -Name GamingServices; " +
            "$svc.WaitForStatus('Running', [TimeSpan]::FromSeconds(30))";
        RunServiceCommand(script, "start");
    }

    private static void RunServiceCommand(string script, string verb)
    {
        using var process = new Process();
        process.StartInfo.FileName = "powershell.exe";
        process.StartInfo.Arguments = $"-NoProfile -NonInteractive -ExecutionPolicy Bypass -Command \"{script}\"";
        process.StartInfo.UseShellExecute = false;
        process.StartInfo.RedirectStandardOutput = true;
        process.StartInfo.RedirectStandardError = true;
        process.StartInfo.CreateNoWindow = true;

        process.Start();

        string stdout = process.StandardOutput.ReadToEnd();
        string stderr = process.StandardError.ReadToEnd();

        if (!process.WaitForExit(CommandTimeoutMs))
        {
            try { process.Kill(); } catch { }
            throw new TimeoutException($"GamingServices {verb} timed out after {CommandTimeoutMs}ms");
        }

        if (process.ExitCode != 0)
        {
            string errDetail = string.IsNullOrWhiteSpace(stderr) ? "(no stderr)" : stderr.Trim();
            throw new InvalidOperationException($"GamingServices {verb} failed (exit {process.ExitCode}): {errDetail}");
        }
    }

    // ──────────────────────────────────────────────────────────────────────
    //  Pipe I/O helpers (newline-delimited protocol)
    // ──────────────────────────────────────────────────────────────────────

    private static string? ReadLine(Stream stream)
    {
        var sb = new StringBuilder();
        int b;
        while ((b = stream.ReadByte()) != -1)
        {
            if (b == '\n') break;
            sb.Append((char)b);
        }

        return sb.Length > 0 ? sb.ToString().TrimEnd('\r') : null;
    }

    private static void WriteResponse(Stream stream, bool success, string? message)
    {
        var response = new AdminResponse { Success = success, Message = message };
        string json = JsonSerializer.Serialize(response, JsonOptions);
        byte[] bytes = Encoding.UTF8.GetBytes(json + "\n");
        stream.Write(bytes, 0, bytes.Length);
        stream.Flush();

        // Wait for the client to read all data before the caller disconnects the pipe.
        if (stream is NamedPipeServerStream pipeServer)
        {
            pipeServer.WaitForPipeDrain();
        }
    }

    // ──────────────────────────────────────────────────────────────────────
    //  Helpers
    // ──────────────────────────────────────────────────────────────────────

    private static int GetIntParam(Dictionary<string, object>? parameters, string key, int defaultValue)
    {
        if (parameters == null || !parameters.TryGetValue(key, out object? value))
            return defaultValue;

        return value switch
        {
            JsonElement je when je.ValueKind == JsonValueKind.Number => je.GetInt32(),
            JsonElement je when je.ValueKind == JsonValueKind.String && int.TryParse(je.GetString(), out int parsed) => parsed,
            int i => i,
            string s when int.TryParse(s, out int parsed) => parsed,
            _ => defaultValue
        };
    }

    private static string GetStringParam(Dictionary<string, object>? parameters, string key, string defaultValue)
    {
        if (parameters == null || !parameters.TryGetValue(key, out object? value))
            return defaultValue;

        return value switch
        {
            JsonElement je when je.ValueKind == JsonValueKind.String => je.GetString() ?? defaultValue,
            string s => s,
            null => defaultValue,
            _ => value.ToString() ?? defaultValue
        };
    }

    private static string FormatParams(Dictionary<string, object>? parameters)
    {
        if (parameters == null || parameters.Count == 0) return "none";
        return string.Join(", ", parameters.Select(kv => $"{kv.Key}={kv.Value}"));
    }

    private static string Now() => DateTime.Now.ToString("HH:mm:ss");
}

// ──────────────────────────────────────────────────────────────────────────
//  Protocol types
// ──────────────────────────────────────────────────────────────────────────

internal sealed class AdminRequest
{
    public string? Action { get; set; }
    public Dictionary<string, object>? Parameters { get; set; }
}

internal sealed class AdminResponse
{
    public bool Success { get; set; }
    public string? Message { get; set; }
}
