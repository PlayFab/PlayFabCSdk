using System.Diagnostics;
using System.Net.WebSockets;
using System.Text;
using System.Text.Json;

namespace DeviceAgent;

/// <summary>
/// Core runtime for the DeviceAgent. Maintains a persistent WebSocket connection
/// to the GameTestController and executes lifecycle commands (launch/kill test app).
/// </summary>
internal sealed class AgentRuntime
{
    private readonly string _controllerHost;
    private readonly int _controllerPort;
    private readonly string _machineName;
    private string? _testAppPath;
    private Process? _managedProcess;
    private readonly object _processLock = new();

    private const int ReconnectDelayMs = 3000;
    private const int ReceiveBufferSize = 8192;

    public AgentRuntime(string controllerHost, int controllerPort, string machineName, string? testAppPath)
    {
        _controllerHost = controllerHost;
        _controllerPort = controllerPort;
        _machineName = machineName;
        _testAppPath = testAppPath;
    }

    public async Task RunAsync(CancellationToken cancellationToken)
    {
        while (!cancellationToken.IsCancellationRequested)
        {
            try
            {
                await ConnectAndRunAsync(cancellationToken);
            }
            catch (OperationCanceledException) when (cancellationToken.IsCancellationRequested)
            {
                break;
            }
            catch (Exception ex)
            {
                Log($"Connection error: {ex.Message}");
            }

            if (!cancellationToken.IsCancellationRequested)
            {
                Log($"Reconnecting in {ReconnectDelayMs / 1000}s...");
                await Task.Delay(ReconnectDelayMs, cancellationToken).ConfigureAwait(false);
            }
        }

        // Cleanup: kill any managed process on shutdown
        KillManagedProcess();
        Log("Agent stopped.");
    }

    private async Task ConnectAndRunAsync(CancellationToken cancellationToken)
    {
        using var ws = new ClientWebSocket();
        var uri = new Uri($"ws://{_controllerHost}:{_controllerPort}/agent/");

        Log($"Connecting to {uri}...");
        await ws.ConnectAsync(uri, cancellationToken).ConfigureAwait(false);
        Log("Connected.");

        // Send agent hello
        var hello = new AgentHello
        {
            Type = "agentHello",
            MachineName = _machineName,
            TestAppPath = _testAppPath
        };
        await SendJsonAsync(ws, hello, cancellationToken);
        Log("Sent agentHello.");

        // Receive loop
        var buffer = new byte[ReceiveBufferSize];
        var messageBuffer = new MemoryStream();
        while (ws.State == WebSocketState.Open && !cancellationToken.IsCancellationRequested)
        {
            var result = await ws.ReceiveAsync(new ArraySegment<byte>(buffer), cancellationToken).ConfigureAwait(false);

            if (result.MessageType == WebSocketMessageType.Close)
            {
                Log("Server closed connection.");
                break;
            }

            if (result.MessageType == WebSocketMessageType.Text || result.MessageType == WebSocketMessageType.Binary)
            {
                messageBuffer.Write(buffer, 0, result.Count);

                if (result.EndOfMessage)
                {
                    var message = Encoding.UTF8.GetString(messageBuffer.GetBuffer(), 0, (int)messageBuffer.Length);
                    messageBuffer.SetLength(0);

                    var response = await HandleMessageAsync(message);
                    if (response != null)
                    {
                        await SendJsonAsync(ws, response, cancellationToken);
                    }
                }
            }
        }
    }

    private async Task<object?> HandleMessageAsync(string rawJson)
    {
        try
        {
            using var doc = JsonDocument.Parse(rawJson);
            var root = doc.RootElement;

            if (!root.TryGetProperty("command", out var commandElement))
            {
                Log($"Received non-command message: {rawJson}");
                return null;
            }

            string command = commandElement.GetString() ?? "";
            string commandId = root.TryGetProperty("commandId", out var idEl) ? idEl.GetString() ?? "" : "";

            Log($"Command: {command} (id={commandId[..Math.Min(8, commandId.Length)]})");

            return command.ToLowerInvariant() switch
            {
                "ping" => HandlePing(commandId),
                "getstatus" => HandleGetStatus(commandId),
                "launchtestapp" => await HandleLaunchTestAppAsync(commandId, root),
                "killtestapp" => HandleKillTestApp(commandId),
                _ => MakeResponse(commandId, "failure", $"Unknown agent command: {command}")
            };
        }
        catch (JsonException ex)
        {
            Log($"Failed to parse message: {ex.Message}");
            return null;
        }
    }

    private object HandlePing(string commandId)
    {
        return MakeResponse(commandId, "success", null, new { machineName = _machineName });
    }

    private object HandleGetStatus(string commandId)
    {
        lock (_processLock)
        {
            bool isRunning = _managedProcess != null && !_managedProcess.HasExited;
            var data = new
            {
                machineName = _machineName,
                testAppRunning = isRunning,
                testAppPid = isRunning ? _managedProcess!.Id : (int?)null,
                testAppPath = _testAppPath
            };
            return MakeResponse(commandId, "success", null, data);
        }
    }

    private async Task<object> HandleLaunchTestAppAsync(string commandId, JsonElement root)
    {
        // Kill any existing process first
        KillManagedProcess();

        // Extract parameters
        string? appPath = null;
        string[]? args = null;
        int devicePort = 15080;

        if (root.TryGetProperty("parameters", out var paramsEl))
        {
            if (paramsEl.TryGetProperty("testAppPath", out var pathEl))
                appPath = pathEl.GetString();
            if (paramsEl.TryGetProperty("args", out var argsEl) && argsEl.ValueKind == JsonValueKind.Array)
                args = argsEl.EnumerateArray().Select(a => a.GetString() ?? "").ToArray();
            if (paramsEl.TryGetProperty("devicePort", out var portEl) && portEl.ValueKind == JsonValueKind.Number)
                devicePort = portEl.GetInt32();
        }

        // Use provided path or fall back to configured default
        string exePath = appPath ?? _testAppPath ?? "";
        if (string.IsNullOrWhiteSpace(exePath) || !File.Exists(exePath))
        {
            string error = string.IsNullOrWhiteSpace(exePath)
                ? "No test app path configured or provided"
                : $"Test app not found: {exePath}";
            Log($"ERROR: {error}");
            return MakeResponse(commandId, "failure", error);
        }

        // Update stored path if explicitly provided
        if (appPath != null)
            _testAppPath = appPath;

        try
        {
            // Build launch arguments: include orchestrator connection info so the game
            // connects back to the controller's device port on the controller host
            var launchArgs = new List<string>();
            launchArgs.Add($"-OrchestratorHost={_controllerHost}");
            launchArgs.Add($"-OrchestratorPort={devicePort}");
            launchArgs.Add("-log");
            launchArgs.Add("-nosplash");
            launchArgs.Add("-nosound");
            launchArgs.Add("-NullRHI");
            launchArgs.Add("-unattended");
            launchArgs.Add("-windowed");
            launchArgs.Add("-resx=800");
            launchArgs.Add("-resy=600");

            // Append any additional args from the command
            if (args != null && args.Length > 0)
                launchArgs.AddRange(args);

            var startInfo = new ProcessStartInfo
            {
                FileName = exePath,
                WorkingDirectory = Path.GetDirectoryName(exePath) ?? ".",
                UseShellExecute = false,
                Arguments = string.Join(" ", launchArgs),
                RedirectStandardOutput = true,
                RedirectStandardError = true
            };

            Log($"Launch details:");
            Log($"  Exe: {exePath}");
            Log($"  WorkDir: {startInfo.WorkingDirectory}");
            Log($"  Orchestrator: {_controllerHost}:{devicePort}");
            Log($"  Full args: {startInfo.Arguments}");

            lock (_processLock)
            {
                _managedProcess = Process.Start(startInfo);
            }

            if (_managedProcess == null)
            {
                return MakeResponse(commandId, "failure", "Process.Start returned null");
            }

            int pid = _managedProcess.Id;
            Log($"Launched test app (PID={pid})");

            // Capture stdout/stderr asynchronously for diagnostics
            _managedProcess.OutputDataReceived += (_, e) => { if (e.Data != null) Log($"[GameOut] {e.Data}"); };
            _managedProcess.ErrorDataReceived += (_, e) => { if (e.Data != null) Log($"[GameErr] {e.Data}"); };
            _managedProcess.BeginOutputReadLine();
            _managedProcess.BeginErrorReadLine();

            // Monitor for unexpected exit in the background
            _ = Task.Run(async () => await MonitorProcessAsync(_managedProcess));

            // Brief delay to detect immediate crashes
            await Task.Delay(2000);
            lock (_processLock)
            {
                if (_managedProcess != null && _managedProcess.HasExited)
                {
                    int exitCode = _managedProcess.ExitCode;
                    _managedProcess.Dispose();
                    _managedProcess = null;
                    Log($"WARNING: Test app exited immediately after launch (code={exitCode})");
                    return MakeResponse(commandId, "failure", $"Test app crashed on startup (exit code {exitCode})");
                }
            }

            return MakeResponse(commandId, "success", null, new { pid, exePath });
        }
        catch (Exception ex)
        {
            Log($"ERROR launching test app: {ex.Message}");
            return MakeResponse(commandId, "failure", ex.Message);
        }
    }

    private object HandleKillTestApp(string commandId)
    {
        lock (_processLock)
        {
            if (_managedProcess == null || _managedProcess.HasExited)
            {
                return MakeResponse(commandId, "success", null, new { wasRunning = false });
            }

            int pid = _managedProcess.Id;
            try
            {
                _managedProcess.Kill(entireProcessTree: true);
                _managedProcess.WaitForExit(5000);
                Log($"Killed test app (PID={pid}).");
                _managedProcess.Dispose();
                _managedProcess = null;
                return MakeResponse(commandId, "success", null, new { wasRunning = true, killedPid = pid });
            }
            catch (Exception ex)
            {
                Log($"ERROR killing PID {pid}: {ex.Message}");
                return MakeResponse(commandId, "failure", ex.Message);
            }
        }
    }

    private async Task MonitorProcessAsync(Process process)
    {
        try
        {
            await process.WaitForExitAsync();
            int exitCode = process.ExitCode;
            Log($"Test app exited (PID={process.Id}, code={exitCode}).");
        }
        catch
        {
            // Process may have been killed by us — ignore
        }
    }

    private void KillManagedProcess()
    {
        lock (_processLock)
        {
            if (_managedProcess != null && !_managedProcess.HasExited)
            {
                try
                {
                    _managedProcess.Kill(entireProcessTree: true);
                    _managedProcess.WaitForExit(5000);
                    Log($"Cleaned up managed process (PID={_managedProcess.Id}).");
                }
                catch { }
            }
            _managedProcess?.Dispose();
            _managedProcess = null;
        }
    }

    private static object MakeResponse(string commandId, string status, string? error, object? result = null)
    {
        return new AgentResponse
        {
            Type = "agentResponse",
            CommandId = commandId,
            Status = status,
            Error = error,
            Result = result
        };
    }

    private static async Task SendJsonAsync(ClientWebSocket ws, object payload, CancellationToken ct)
    {
        string json = JsonSerializer.Serialize(payload, new JsonSerializerOptions
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            DefaultIgnoreCondition = System.Text.Json.Serialization.JsonIgnoreCondition.WhenWritingNull
        });
        var bytes = Encoding.UTF8.GetBytes(json);
        await ws.SendAsync(new ArraySegment<byte>(bytes), WebSocketMessageType.Text, true, ct).ConfigureAwait(false);
    }

    private static void Log(string message)
    {
        Console.WriteLine($"[{DateTime.Now:HH:mm:ss}] {message}");
    }
}

// --- Message types ---

internal sealed class AgentHello
{
    public string Type { get; set; } = "agentHello";
    public string MachineName { get; set; } = "";
    public string? TestAppPath { get; set; }
}

internal sealed class AgentResponse
{
    public string Type { get; set; } = "agentResponse";
    public string CommandId { get; set; } = "";
    public string Status { get; set; } = "";
    public string? Error { get; set; }
    public object? Result { get; set; }
}
