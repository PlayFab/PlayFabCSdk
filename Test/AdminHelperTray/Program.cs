using System.Diagnostics;
using System.IO.Pipes;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Text.Json;
using System.Windows.Forms;

namespace PlayFab.Test.AdminHelperTray;

/// <summary>
/// Elevated system tray app that listens on a named pipe for requests to launch
/// AdminHelper.exe. Because this tray app runs elevated (via manifest), AdminHelper
/// inherits elevation without a UAC prompt.
///
/// Pipe protocol (JSON over named pipe, newline-delimited):
///   Request:  {"action":"LaunchAdminHelper"}\n
///   Response: {"success":true}\n  or  {"success":false,"error":"..."}\n
/// </summary>
internal static class Program
{
    private const string TrayPipeName = "AdminHelperLauncher";
    private const string AdminHelperPipeName = "PlayFabTestAdminHelper";
    private const string AdminHelperExeName = "AdminHelper.exe";

    private static string LogFilePath = null!;
    private static readonly object LogLock = new();

    [STAThread]
    static void Main()
    {
        // Set up log file next to the exe
        string baseDir = AppContext.BaseDirectory;
        LogFilePath = Path.Combine(baseDir, "AdminHelperTray.log");

        Log("=== AdminHelperTray starting ===");
        Log($"PID: {Environment.ProcessId}");
        Log($"Pipe: {TrayPipeName}");
        Log($"Base dir: {baseDir}");

        ApplicationConfiguration.Initialize();

        using var cts = new CancellationTokenSource();

        // Create tray icon
        using var trayIcon = CreateTrayIcon(cts);

        // Start pipe listener on background thread
        var pipeTask = Task.Run(() => PipeListenLoop(cts.Token));

        Application.Run();

        cts.Cancel();
        // Give pipe listener a moment to shut down
        pipeTask.Wait(TimeSpan.FromSeconds(2));

        Log("=== AdminHelperTray exiting ===");
    }

    private static NotifyIcon CreateTrayIcon(CancellationTokenSource cts)
    {
        var contextMenu = new ContextMenuStrip();
        contextMenu.Items.Add("Exit", null, (_, _) =>
        {
            cts.Cancel();
            Application.Exit();
        });

        var icon = new NotifyIcon
        {
            Icon = LoadTrayIcon(),
            Text = "AdminHelper Launcher (Elevated)",
            ContextMenuStrip = contextMenu,
            Visible = true
        };

        return icon;
    }

    private static System.Drawing.Icon LoadTrayIcon()
    {
        // Try to load the embedded .ico file; fall back to a system icon
        string icoPath = Path.Combine(AppContext.BaseDirectory, "app.ico");
        if (File.Exists(icoPath))
        {
            try { return new System.Drawing.Icon(icoPath); }
            catch { /* fall through */ }
        }

        // Fall back to the default application icon
        return SystemIcons.Shield;
    }

    // ────────────────────────────────────────────────────────────────────
    //  Named pipe listener
    // ────────────────────────────────────────────────────────────────────

    private static async Task PipeListenLoop(CancellationToken ct)
    {
        Log("Pipe listener started.");

        while (!ct.IsCancellationRequested)
        {
            try
            {
                // Allow non-elevated clients to connect to this elevated pipe
                var pipeSecurity = new PipeSecurity();
                pipeSecurity.AddAccessRule(new PipeAccessRule(
                    new SecurityIdentifier(WellKnownSidType.AuthenticatedUserSid, null),
                    PipeAccessRights.ReadWrite,
                    AccessControlType.Allow));

                using var server = NamedPipeServerStreamAcl.Create(
                    TrayPipeName,
                    PipeDirection.InOut,
                    1, // single instance
                    PipeTransmissionMode.Byte,
                    PipeOptions.Asynchronous,
                    0, 0,
                    pipeSecurity);

                Log("Waiting for pipe connection...");
                await server.WaitForConnectionAsync(ct);
                Log("Client connected.");

                await HandleClient(server, ct);
            }
            catch (OperationCanceledException)
            {
                break;
            }
            catch (Exception ex)
            {
                Log($"Pipe error: {ex.Message}");
                // Brief pause before retrying to avoid tight error loop
                try { await Task.Delay(1000, ct); } catch { break; }
            }
        }

        Log("Pipe listener stopped.");
    }

    private static async Task HandleClient(NamedPipeServerStream server, CancellationToken ct)
    {
        try
        {
            // Read request (newline-delimited JSON)
            string? requestLine = await ReadLineAsync(server, ct);
            if (string.IsNullOrWhiteSpace(requestLine))
            {
                await WriteResponse(server, false, "Empty request");
                return;
            }

            Log($"Request: {requestLine}");

            // Parse request
            string? action = null;
            try
            {
                using var doc = JsonDocument.Parse(requestLine);
                if (doc.RootElement.TryGetProperty("action", out var actionProp))
                    action = actionProp.GetString();
            }
            catch (JsonException ex)
            {
                await WriteResponse(server, false, $"Invalid JSON: {ex.Message}");
                return;
            }

            if (string.IsNullOrEmpty(action))
            {
                await WriteResponse(server, false, "Missing 'action' field");
                return;
            }

            // Dispatch
            if (action.Equals("LaunchAdminHelper", StringComparison.OrdinalIgnoreCase))
            {
                await HandleLaunchAdminHelper(server);
            }
            else
            {
                await WriteResponse(server, false, $"Unknown action: {action}");
            }
        }
        catch (Exception ex)
        {
            Log($"HandleClient error: {ex.Message}");
            try { await WriteResponse(server, false, ex.Message); } catch { }
        }
        finally
        {
            if (server.IsConnected)
                server.Disconnect();
        }
    }

    private static async Task HandleLaunchAdminHelper(NamedPipeServerStream server)
    {
        // Check if AdminHelper is already running by probing its pipe
        if (IsAdminHelperRunning())
        {
            Log("AdminHelper is already running.");
            await WriteResponse(server, true, "AdminHelper is already running");
            return;
        }

        // Find AdminHelper.exe
        string adminHelperPath = FindAdminHelper();
        if (adminHelperPath == null!)
        {
            string msg = "AdminHelper.exe not found";
            Log(msg);
            await WriteResponse(server, false, msg);
            return;
        }

        // Launch it — inherits our elevation, no UAC prompt
        Log($"Launching AdminHelper: {adminHelperPath}");
        try
        {
            var psi = new ProcessStartInfo
            {
                FileName = adminHelperPath,
                UseShellExecute = false, // No shell, inherits our elevation
                WorkingDirectory = Path.GetDirectoryName(adminHelperPath)!
            };
            Process.Start(psi);
        }
        catch (Exception ex)
        {
            string msg = $"Failed to launch AdminHelper: {ex.Message}";
            Log(msg);
            await WriteResponse(server, false, msg);
            return;
        }

        // Wait for AdminHelper's pipe to become available
        var sw = Stopwatch.StartNew();
        while (sw.ElapsedMilliseconds < 10_000)
        {
            await Task.Delay(500);
            if (IsAdminHelperRunning())
            {
                Log("AdminHelper launched and pipe is ready.");
                await WriteResponse(server, true, "AdminHelper launched successfully");
                return;
            }
        }

        string timeout = "AdminHelper launched but pipe not available after 10s";
        Log(timeout);
        await WriteResponse(server, false, timeout);
    }

    // ────────────────────────────────────────────────────────────────────
    //  Helpers
    // ────────────────────────────────────────────────────────────────────

    private static bool IsAdminHelperRunning()
    {
        try
        {
            using var probe = new NamedPipeClientStream(".", AdminHelperPipeName, PipeDirection.InOut);
            probe.Connect(500);
            return true;
        }
        catch
        {
            return false;
        }
    }

    private static string FindAdminHelper()
    {
        // Same directory as this tray app
        string baseDir = AppContext.BaseDirectory;
        string sameDirPath = Path.Combine(baseDir, AdminHelperExeName);
        if (File.Exists(sameDirPath))
            return sameDirPath;

        // Sibling directory (same output structure as AdminHelper project)
        string siblingPath = Path.GetFullPath(Path.Combine(baseDir, "..", "AdminHelper", AdminHelperExeName));
        if (File.Exists(siblingPath))
            return siblingPath;

        return null!;
    }

    private static async Task<string?> ReadLineAsync(NamedPipeServerStream server, CancellationToken ct)
    {
        var sb = new StringBuilder();
        var buffer = new byte[1];
        var sw = Stopwatch.StartNew();

        while (sw.ElapsedMilliseconds < 5_000)
        {
            ct.ThrowIfCancellationRequested();

            int bytesRead = await server.ReadAsync(buffer, 0, 1, ct);
            if (bytesRead == 0) break; // client disconnected

            char c = (char)buffer[0];
            if (c == '\n') break;
            sb.Append(c);
        }

        string result = sb.ToString().TrimEnd('\r');
        return string.IsNullOrEmpty(result) ? null : result;
    }

    private static async Task WriteResponse(NamedPipeServerStream server, bool success, string? message = null)
    {
        object response = success
            ? new { success = true, message }
            : new { success = false, error = message };

        string json = JsonSerializer.Serialize(response);
        Log($"Response: {json}");

        byte[] bytes = Encoding.UTF8.GetBytes(json + "\n");
        await server.WriteAsync(bytes, 0, bytes.Length);
        await server.FlushAsync();
    }

    private static void Log(string message)
    {
        string line = $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff}] {message}";
        lock (LogLock)
        {
            try
            {
                File.AppendAllText(LogFilePath, line + Environment.NewLine);
            }
            catch { /* don't crash on log failure */ }
        }
    }
}
