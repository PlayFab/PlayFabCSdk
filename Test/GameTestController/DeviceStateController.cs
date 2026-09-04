using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Pipes;
using System.Net;
using System.Net.NetworkInformation;
using System.Text;
using System.Text.Json;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace GameTestController
{
    /// <summary>
    /// Handles controller-side device state changes. For PC targets, uses PowerShell
    /// to manage network adapters. For Xbox targets, shells out to GDK xb* tools.
    /// Used by the ChangeTargetDeviceState controller command.
    ///
    /// Supported actions (see 2-automation-actions-needed.md for full details):
    ///   Network:    DisableNetwork, EnableNetwork, NetworkFlapping
    ///   Lifecycle:  Suspend, Resume, Terminate, EvictGame, Relaunch
    ///   Power:      Reboot, Shutdown, Standby, Wake, QueryPowerState, WaitForBoot
    ///   User:       SignOut, SignIn, SwitchUser, ListUsers, AddUser, DeleteUser, DeleteAllUsers
    ///   Storage:    DeleteStorage, ResetAllStorage, SimulateOutOfStorage, StopStorageSimulation
    ///   Services:   StopGamingServices, StartGamingServices, FlushGrtsAndGoOffline
    ///   QuickResume: QuickResumeSave, QuickResumeRestore, QuickResumeList, QuickResumeDelete
    /// </summary>
    internal static class DeviceStateController
    {
        private const int DefaultTimeoutMs = 60_000;

        /// <summary>
        /// Tracks launched device process PIDs by role name (e.g. "DeviceA" → 12345).
        /// Populated by <see cref="RegisterLaunchedDevice"/> when the controller auto-launches
        /// a local device, consumed by Terminate/Relaunch PC actions.
        /// </summary>
        private static readonly ConcurrentDictionary<string, int> s_launchedPids = new(StringComparer.OrdinalIgnoreCase);

        /// <summary>
        /// GRTS local save folder base path. GRTS stores saves under
        /// {drive}:\XboxGames\GameSave\pgs\u_{xuid}_{titleId}\.
        /// </summary>
        private const string GrtsSaveFolderBase = @"C:\XboxGames\GameSave\pgs";

        /// <summary>
        /// Registers the PID of a locally launched device so that Terminate can
        /// kill the correct process later. Called from ControllerRuntime after Process.Start.
        /// </summary>
        internal static void RegisterLaunchedDevice(string role, int processId)
        {
            s_launchedPids[role] = processId;
        }

        /// <summary>
        /// Checks whether physical network adapters are up. If any are disabled,
        /// sends an EnableNetwork command to AdminHelper to restore them.
        /// This is safe to call without admin — reading adapter status requires no elevation.
        /// Called at the start of each test to ensure a previous test's DisableNetwork
        /// didn't leave the machine offline.
        /// </summary>
        internal static void EnsureNetworkEnabled(Action<string> log)
        {
            try
            {
                var adapters = NetworkInterface.GetAllNetworkInterfaces();
                bool anyPhysicalDown = false;

                foreach (var nic in adapters)
                {
                    // Skip loopback, tunnel, and virtual adapters
                    if (nic.NetworkInterfaceType == NetworkInterfaceType.Loopback ||
                        nic.NetworkInterfaceType == NetworkInterfaceType.Tunnel)
                        continue;

                    // Focus on Ethernet and Wi-Fi (physical adapters)
                    if (nic.NetworkInterfaceType != NetworkInterfaceType.Ethernet &&
                        nic.NetworkInterfaceType != NetworkInterfaceType.Wireless80211 &&
                        nic.NetworkInterfaceType != NetworkInterfaceType.GigabitEthernet)
                        continue;

                    if (nic.OperationalStatus != OperationalStatus.Up)
                    {
                        log($"  Network adapter '{nic.Name}' is {nic.OperationalStatus} — will re-enable.");
                        anyPhysicalDown = true;
                    }
                }

                if (!anyPhysicalDown)
                    return;

                // Try AdminHelper first (preferred, no elevation needed for controller)
                if (TryRunViaAdminHelper("EnableNetwork", null, log))
                {
                    // Give the adapter a moment to come up
                    System.Threading.Thread.Sleep(3000);
                    log("  Network adapter re-enabled via AdminHelper.");
                    return;
                }

                // AdminHelper not available — warn but don't fail the test
                log("  WARNING: Network adapter is down but AdminHelper is not available to re-enable it.");
            }
            catch (Exception ex)
            {
                // Non-fatal — log and continue; the test will fail on its own if network is truly needed
                log($"  WARNING: EnsureNetworkEnabled check failed: {ex.Message}");
            }
        }

        /// <summary>
        /// Checks whether GamingServices is running. If stopped, starts it via AdminHelper.
        /// Called at the start of each test to recover from a prior test's
        /// StopGamingServices or FlushGrtsAndGoOffline.
        /// </summary>
        internal static void EnsureGamingServicesRunning(Action<string> log)
        {
            try
            {
                using var sc = new System.ServiceProcess.ServiceController("GamingServices");
                if (sc.Status == System.ServiceProcess.ServiceControllerStatus.Running)
                    return;

                log($"  GamingServices is {sc.Status} — will restart.");
                if (TryRunViaAdminHelper("StartGamingServices", null, log))
                {
                    log("  GamingServices restarted via AdminHelper.");
                    return;
                }

                log("  WARNING: GamingServices is not running but AdminHelper is not available to start it.");
            }
            catch (Exception ex)
            {
                log($"  WARNING: EnsureGamingServicesRunning check failed: {ex.Message}");
            }
        }

        /// <summary>
        /// Gets the tracked PID for a device role, or -1 if not tracked.
        /// </summary>
        internal static int GetLaunchedDevicePid(string role)
        {
            return s_launchedPids.TryGetValue(role, out int pid) ? pid : -1;
        }

        /// <summary>
        /// Removes the PID tracking entry for a role (after the process is killed).
        /// </summary>
        internal static void UnregisterLaunchedDevice(string role)
        {
            s_launchedPids.TryRemove(role, out _);
        }

        /// <summary>
        /// Stores the email of the last user signed out, for automatic sign-in recovery.
        /// </summary>
        private static string? _lastSignedOutEmail;

        /// <summary>
        /// Package Family Name for GameTestAppXbox, fixed by MicrosoftGame.Config.
        /// Used as the target for xbapp suspend/resume/terminate/launch commands.
        /// </summary>
        internal const string AppPFN = "41336MicrosoftATG.XboxLiveE2E_dspnxghe87tn0";

        /// <summary>
        /// Application User Model ID — PFN + "!Game" suffix.
        /// Used by xbapp launch and Quick Resume commands.
        /// </summary>
        internal const string AppAUMID = AppPFN + "!Game";

        /// <summary>
        /// Executes a device state change action. Routes to Xbox (xb* tools) or PC
        /// (PowerShell) based on the target device engine type.
        /// </summary>
        public static void Execute(CommandEnvelope envelope, DeviceAssignment assignment, Action<string> log)
        {
            IDictionary<string, object>? parameters = envelope.Parameters as IDictionary<string, object>;

            string action = string.Empty;
            if (parameters != null && parameters.TryGetValue("action", out object? actionValue))
            {
                action = actionValue?.ToString()?.Trim() ?? string.Empty;
            }

            if (string.IsNullOrEmpty(action))
            {
                throw new ArgumentException("ChangeTargetDeviceState requires an 'action' parameter.");
            }

            string engine = assignment.Engine?.ToLowerInvariant() ?? "unknown";

            if (engine == "xbox")
            {
                ExecuteXboxAction(action, parameters, assignment, log);
            }
            else if (engine.StartsWith("pc-"))
            {
                ExecutePcAction(action, parameters, assignment, log);
            }
            else
            {
                throw new InvalidOperationException(
                    $"ChangeTargetDeviceState: unsupported engine '{engine}' for device '{assignment.DisplayName}'. " +
                    $"Supported engines: xbox, pc-inproc, pc-inproc-gamesaves, pc-grts.");
            }
        }

        // ────────────────────────────────────────────────────────────────────
        //  Xbox actions — shell out to GDK xb* tools via /x:$ip
        // ────────────────────────────────────────────────────────────────────

        private static void ExecuteXboxAction(string action, IDictionary<string, object>? parameters, DeviceAssignment assignment, Action<string> log)
        {
            string ip = ExtractXboxIp(assignment);

            switch (action.ToLowerInvariant())
            {
                // --- Network ---
                case "disablenetwork":
                {
                    RunXbTool("xbstress.exe", $"simulate network=broken /x:{ip}", log);
                    log($"Xbox [{ip}]: network disabled (network=broken).");
                    break;
                }

                case "enablenetwork":
                    RunXbTool("xbstress.exe", $"stop /x:{ip}", log);
                    log($"Xbox [{ip}]: network enabled (xbstress stop).");
                    break;

                case "networkflapping":
                {
                    int cycles = GetIntParam(parameters, "cycles", 5);
                    int intervalMs = GetIntParam(parameters, "intervalMs", 500);
                    log($"Xbox [{ip}]: network flapping {cycles} cycles, {intervalMs}ms interval.");
                    for (int i = 0; i < cycles; i++)
                    {
                        RunXbTool("xbstress.exe", $"simulate network=broken /x:{ip}", log);
                        System.Threading.Thread.Sleep(intervalMs);
                        RunXbTool("xbstress.exe", $"stop /x:{ip}", log);
                        System.Threading.Thread.Sleep(intervalMs);
                    }
                    log($"Xbox [{ip}]: network flapping complete.");
                    break;
                }

                // --- Process Lifecycle ---
                case "suspend":
                    RunXbTool("xbapp.exe", $"/x:{ip} suspend {AppAUMID}", log);
                    log($"Xbox [{ip}]: suspended '{AppAUMID}'.");
                    break;

                case "resume":
                    RunXbTool("xbapp.exe", $"/x:{ip} resume {AppAUMID}", log);
                    log($"Xbox [{ip}]: resumed '{AppAUMID}'.");
                    break;

                case "terminate":
                    RunXbTool("xbapp.exe", $"/x:{ip} terminate {AppAUMID}", log);
                    log($"Xbox [{ip}]: terminated '{AppAUMID}'.");
                    break;

                case "evictgame":
                {
                    string stubPkg = GetStringParam(parameters, "stubPackageName");
                    RunXbTool("xbapp.exe", $"/x:{ip} launch {stubPkg}", log);
                    log($"Xbox [{ip}]: evicted current game by launching '{stubPkg}'.");
                    break;
                }

                case "relaunch":
                    RunXbTool("xbapp.exe", $"/x:{ip} launch {AppAUMID}", log);
                    log($"Xbox [{ip}]: launched '{AppAUMID}'.");
                    break;

                // --- Power State ---
                case "reboot":
                    RunXbTool("xbreboot.exe", $"/x:{ip}", log);
                    log($"Xbox [{ip}]: reboot initiated.");
                    break;

                case "shutdown":
                    RunXbTool("xbreboot.exe", $"/x:{ip} /S", log);
                    log($"Xbox [{ip}]: shutdown initiated.");
                    break;

                case "standby":
                    RunXbTool("xbreboot.exe", $"/x:{ip} /P", log);
                    log($"Xbox [{ip}]: entering connected standby.");
                    break;

                case "wake":
                    // sflash /resetcycle reliably wakes from Instant On standby;
                    // xbreboot /W alone cannot reach the console in deep standby.
                    RunExternalTool(@"D:\os\utilities\Xbox.Hardware.Utilities\sflash.exe", "/resetcycle", log);
                    log($"Xbox [{ip}]: SMC reset cycle issued (wake from standby).");
                    break;

                case "querypowerstate":
                    RunXbTool("xbreboot.exe", $"/x:{ip} /Q", log);
                    break;

                case "waitforboot":
                {
                    int timeoutSec = GetIntParam(parameters, "timeoutSeconds", 120);
                    RunXbTool("xbconnect.exe", $"{ip} /b /q /ws:{timeoutSec}", log, timeoutMs: (timeoutSec + 30) * 1000);
                    log($"Xbox [{ip}]: console is responsive after boot.");
                    break;
                }

                // --- User / Auth ---
                case "signout":
                {
                    string userArg = GetUserArg(parameters);
                    if (string.IsNullOrEmpty(userArg))
                    {
                        // No email/userId specified — query current signed-in user
                        string currentEmail = QueryCurrentXboxUserEmail(ip, log);
                        if (!string.IsNullOrEmpty(currentEmail))
                        {
                            userArg = $"/e:{currentEmail}";
                            log($"Xbox [{ip}]: resolved current user to '{currentEmail}'");
                        }
                        // else: try bare signout (may work on some devkits)
                    }
                    RunXbTool("xbuser.exe", $"/x:{ip} signout {userArg}".Trim(), log);
                    log($"Xbox [{ip}]: signed out user.");
                    break;
                }

                case "signin":
                {
                    string email = GetOptionalStringParam(parameters, "email");
                    if (string.IsNullOrEmpty(email))
                    {
                        // No email specified — try to use the last known user
                        email = _lastSignedOutEmail ?? string.Empty;
                        if (string.IsNullOrEmpty(email))
                        {
                            throw new ArgumentException("ChangeTargetDeviceState: SignIn requires 'email' parameter (no previously signed-out user to restore).");
                        }
                        log($"Xbox [{ip}]: using last signed-out user '{email}' for sign-in");
                    }
                    string password = GetOptionalStringParam(parameters, "password");
                    string args = $"/x:{ip} signin /e:{email}";
                    if (!string.IsNullOrEmpty(password))
                    {
                        args += $" /p:{password}";
                    }
                    RunXbTool("xbuser.exe", args, log);
                    log($"Xbox [{ip}]: signed in '{email}'.");
                    break;
                }

                case "switchuser":
                {
                    string signOutEmail = GetStringParam(parameters, "signOutEmail");
                    string signInEmail = GetStringParam(parameters, "signInEmail");
                    string password = GetOptionalStringParam(parameters, "password");
                    RunXbTool("xbuser.exe", $"/x:{ip} signout /e:{signOutEmail}", log);
                    string signInArgs = $"/x:{ip} signin /e:{signInEmail}";
                    if (!string.IsNullOrEmpty(password))
                    {
                        signInArgs += $" /p:{password}";
                    }
                    RunXbTool("xbuser.exe", signInArgs, log);
                    log($"Xbox [{ip}]: switched user from '{signOutEmail}' to '{signInEmail}'.");
                    break;
                }

                case "listusers":
                    RunXbTool("xbuser.exe", $"/x:{ip} list", log);
                    break;

                case "adduser":
                {
                    string email = GetStringParam(parameters, "email");
                    RunXbTool("xbuser.exe", $"/x:{ip} add /e:{email}", log);
                    log($"Xbox [{ip}]: added user '{email}'.");
                    break;
                }

                case "deleteuser":
                {
                    string email = GetStringParam(parameters, "email");
                    RunXbTool("xbuser.exe", $"/x:{ip} delete /e:{email}", log);
                    log($"Xbox [{ip}]: deleted user '{email}'.");
                    break;
                }

                case "deleteallusers":
                    RunXbTool("xbuser.exe", $"/x:{ip} deleteall", log);
                    log($"Xbox [{ip}]: deleted all users.");
                    break;

                // --- Storage ---
                case "deletestorage":
                {
                    string scid = GetOptionalStringParam(parameters, "scid");
                    string msa = GetOptionalStringParam(parameters, "msa");
                    bool machine = GetBoolParam(parameters, "machine", false);
                    string args = $"/x:{ip} delete /force";
                    if (machine)
                        args += " /machine";
                    else if (!string.IsNullOrEmpty(msa))
                        args += $" /msa:{msa}";
                    if (!string.IsNullOrEmpty(scid))
                        args += $" /scid:{scid}";
                    RunXbTool("xbstorage.exe", args, log);
                    log($"Xbox [{ip}]: deleted Connected Storage.");
                    break;
                }

                case "resetallstorage":
                    RunXbTool("xbstorage.exe", $"/x:{ip} reset /force", log);
                    log($"Xbox [{ip}]: factory reset all Connected Storage.");
                    break;

                case "simulateoutofstorage":
                {
                    string mode = GetOptionalStringParam(parameters, "mode");
                    string simArg = string.Equals(mode, "reserveRemaining", StringComparison.OrdinalIgnoreCase)
                        ? "/reserveremainingspace"
                        : "/forceoutoflocalstorage";
                    RunXbTool("xbstorage.exe", $"/x:{ip} simulate {simArg}", log);
                    log($"Xbox [{ip}]: simulating out-of-storage ({simArg}).");
                    break;
                }

                case "stopstoragesimulation":
                    RunXbTool("xbstorage.exe", $"/x:{ip} simulate /stop", log);
                    log($"Xbox [{ip}]: stopped storage simulation.");
                    break;

                // --- Quick Resume ---
                case "quickresumesave":
                {
                    string aumid = GetOptionalStringParam(parameters, "aumid");
                    if (string.IsNullOrEmpty(aumid))
                        aumid = AppAUMID;
                    string state = GetOptionalStringParam(parameters, "state");
                    string args = $"/x:{ip} save {aumid}";
                    if (!string.IsNullOrEmpty(state))
                        args += $" /State:{state}";
                    RunXbTool("xbapp.exe", args, log);
                    log($"Xbox [{ip}]: Quick Resume state saved.");
                    break;
                }

                case "quickresumerestore":
                {
                    string aumid = GetOptionalStringParam(parameters, "aumid");
                    if (string.IsNullOrEmpty(aumid))
                        aumid = AppAUMID;
                    string state = GetOptionalStringParam(parameters, "state");
                    string args = $"/x:{ip} restore {aumid}";
                    if (!string.IsNullOrEmpty(state))
                        args += $" /State:{state}";
                    RunXbTool("xbapp.exe", args, log);
                    log($"Xbox [{ip}]: Quick Resume state restored.");
                    break;
                }

                case "quickresumelist":
                    RunXbTool("xbapp.exe", $"/x:{ip} quickresume /List", log);
                    break;

                case "quickresumedelete":
                {
                    string state = GetStringParam(parameters, "state");
                    RunXbTool("xbapp.exe", $"/x:{ip} quickresume /Delete {state}", log);
                    log($"Xbox [{ip}]: Quick Resume state '{state}' deleted.");
                    break;
                }

                default:
                    throw new ArgumentException(
                        $"ChangeTargetDeviceState: unsupported Xbox action '{action}'. " +
                        $"See DeviceStateController.cs for the full list of supported actions.");
            }
        }

        // ────────────────────────────────────────────────────────────────────
        //  PC actions — PowerShell network adapter control (local only)
        // ────────────────────────────────────────────────────────────────────

        // Safety guard: PC NIC manipulation is OFF by default because it severs MCP connectivity,
        // remote test machine sessions, and any concurrent work on the controller PC. Tests that
        // legitimately need it must set ALLOW_PC_NIC_DISABLE=1 in the environment AND should
        // ideally be replaced with PFGameSaveFilesSetMockForceOfflineForDebug (SDK-side mock,
        // no real NIC change). See cluster-a-fix-summary in session notes for context.
        internal const string AllowPcNicDisableEnv = "ALLOW_PC_NIC_DISABLE";

        /// <summary>Returns true if PC NIC manipulation is allowed (env var ALLOW_PC_NIC_DISABLE=1).</summary>
        internal static bool IsPcNicManipulationAllowed()
        {
            string? optIn = Environment.GetEnvironmentVariable(AllowPcNicDisableEnv);
            return string.Equals(optIn, "1", StringComparison.Ordinal);
        }

        /// <summary>
        /// Actions that manipulate the PC's network adapter (DisableNetwork, EnableNetwork, NetworkFlapping)
        /// or composite actions that internally disable the NIC (FlushGrtsAndGoOffline).
        /// </summary>
        internal static readonly HashSet<string> PcActionsRequiringNic = new(StringComparer.OrdinalIgnoreCase)
        {
            "DisableNetwork",
            "EnableNetwork",
            "NetworkFlapping",
            "FlushGrtsAndGoOffline"
        };

        private static void ThrowIfPcNicManipulationDisallowed(string action, DeviceAssignment assignment)
        {
            if (!IsPcNicManipulationAllowed())
            {
                throw new InvalidOperationException(
                    $"ChangeTargetDeviceState '{action}' on PC target '{assignment.DisplayName}' (engine: {assignment.Engine}) " +
                    $"would manipulate the controller PC's network adapter. This is BLOCKED by default because it disconnects " +
                    $"the controller from MCP, remote test machines, and the user's session. " +
                    $"To opt in (NOT recommended on developer machines), set environment variable {AllowPcNicDisableEnv}=1. " +
                    $"Preferred alternative: register the device with engine: [xbox] only, or use the SDK mock-offline command " +
                    $"(PFGameSaveFilesSetMockForceOfflineForDebug) which simulates offline state without touching the NIC.");
            }
        }

        private static void ExecutePcAction(string action, IDictionary<string, object>? parameters, DeviceAssignment assignment, Action<string> log)
        {
            switch (action.ToLowerInvariant())
            {
                case "disablenetwork":
                    ThrowIfPcNicManipulationDisallowed(action, assignment);
                    ValidateDeviceIsLocal(assignment);
                    RunNetworkAction("DisableNetwork", null, log);
                    log($"PC: disabled network adapters on local machine for device '{assignment.DisplayName}'.");
                    break;

                case "enablenetwork":
                    ThrowIfPcNicManipulationDisallowed(action, assignment);
                    ValidateDeviceIsLocal(assignment);
                    RunNetworkAction("EnableNetwork", null, log);
                    log($"PC: enabled network adapters on local machine for device '{assignment.DisplayName}'.");
                    break;

                case "networkflapping":
                {
                    ValidateDeviceIsLocal(assignment);
                    int cycles = GetIntParam(parameters, "cycles", 5);
                    int intervalMs = GetIntParam(parameters, "intervalMs", 500);
                    log($"PC: network flapping {cycles} cycles, {intervalMs}ms interval for '{assignment.DisplayName}'.");

                    var flappingParams = new Dictionary<string, object>
                    {
                        ["cycles"] = cycles,
                        ["intervalMs"] = intervalMs
                    };
                    RunNetworkAction("NetworkFlapping", flappingParams, log);
                    log($"PC: network flapping complete ({cycles} cycles).");
                    break;
                }

                case "terminate":
                {
                    string role = assignment.DisplayName;
                    int pid = GetLaunchedDevicePid(role);
                    if (pid > 0)
                    {
                        try
                        {
                            var proc = Process.GetProcessById(pid);
                            log($"PC: terminating device '{role}' (PID {pid}).");
                            proc.Kill();
                            proc.WaitForExit(10_000);
                            UnregisterLaunchedDevice(role);
                            log($"PC: device '{role}' terminated.");
                        }
                        catch (ArgumentException)
                        {
                            log($"PC: device '{role}' (PID {pid}) already exited.");
                            UnregisterLaunchedDevice(role);
                        }
                    }
                    else
                    {
                        // Fallback: kill by process name (less precise but better than nothing)
                        log($"PC: no tracked PID for '{role}', searching by process name.");
                        var procs = Process.GetProcessesByName("GameTestAppWindows");
                        if (procs.Length == 0)
                        {
                            log($"PC: no GameTestAppWindows processes found to terminate.");
                        }
                        else if (procs.Length == 1)
                        {
                            log($"PC: terminating single GameTestAppWindows process (PID {procs[0].Id}).");
                            procs[0].Kill();
                            procs[0].WaitForExit(10_000);
                            log($"PC: device terminated.");
                        }
                        else
                        {
                            log($"PC: WARNING — {procs.Length} GameTestAppWindows processes found. Killing all of them.");
                            foreach (var p in procs)
                            {
                                try { p.Kill(); } catch { }
                            }
                        }
                        foreach (var p in procs) p.Dispose();
                    }
                    break;
                }

                case "relaunch":
                {
                    string role = assignment.DisplayName;
                    string deviceExecutable = ControllerRuntime.ResolveDeviceExecutablePath();
                    if (!File.Exists(deviceExecutable))
                    {
                        throw new FileNotFoundException(
                            $"Relaunch failed: device executable not found at '{deviceExecutable}'.", deviceExecutable);
                    }

                    bool forceInproc = assignment.Engine != null &&
                        (assignment.Engine.StartsWith("pc-inproc", StringComparison.OrdinalIgnoreCase));

                    var startInfo = new ProcessStartInfo
                    {
                        FileName = deviceExecutable,
                        WorkingDirectory = Path.GetDirectoryName(deviceExecutable) ?? Directory.GetCurrentDirectory(),
                        UseShellExecute = false
                    };

                    if (forceInproc)
                    {
                        startInfo.ArgumentList.Add("/forceinproc");
                    }

                    log($"PC: relaunching device '{role}' using '{deviceExecutable}'{(forceInproc ? " (forceinproc)" : "")}.");
                    Process? launched = Process.Start(startInfo);
                    if (launched == null)
                    {
                        throw new InvalidOperationException($"Relaunch failed: process did not start for role '{role}'.");
                    }

                    RegisterLaunchedDevice(role, launched.Id);
                    log($"PC: device '{role}' relaunched (PID {launched.Id}). Waiting for WebSocket reconnection...");
                    launched.Dispose();
                    break;
                }

                case "deletestorage":
                {
                    log($"PC: deleting GRTS save data for '{assignment.DisplayName}'.");
                    if (Directory.Exists(GrtsSaveFolderBase))
                    {
                        foreach (string dir in Directory.GetDirectories(GrtsSaveFolderBase))
                        {
                            try
                            {
                                Directory.Delete(dir, recursive: true);
                                log($"PC: deleted '{dir}'.");
                            }
                            catch (Exception ex)
                            {
                                log($"PC: WARNING — failed to delete '{dir}': {ex.Message}");
                            }
                        }

                        foreach (string file in Directory.GetFiles(GrtsSaveFolderBase))
                        {
                            try
                            {
                                File.Delete(file);
                            }
                            catch (Exception ex)
                            {
                                log($"PC: WARNING — failed to delete '{file}': {ex.Message}");
                            }
                        }

                        log($"PC: GRTS save data deleted from '{GrtsSaveFolderBase}'.");
                    }
                    else
                    {
                        log($"PC: GRTS save folder '{GrtsSaveFolderBase}' does not exist — nothing to delete.");
                    }
                    break;
                }

                case "stopgamingservices":
                {
                    ValidateDeviceIsLocal(assignment);
                    RunAdminAction("StopGamingServices", null, log);
                    log($"PC: GamingServices stopped for device '{assignment.DisplayName}'.");
                    break;
                }

                case "startgamingservices":
                {
                    ValidateDeviceIsLocal(assignment);
                    RunAdminAction("StartGamingServices", null, log);
                    log($"PC: GamingServices started for device '{assignment.DisplayName}'.");
                    break;
                }

                case "flushgrtsandgooffline":
                {
                    // Composite: stop GRTS → clear cache → disable network → start GRTS → relaunch device.
                    // Ensures GRTS starts cold with no cached state while offline,
                    // so AddUserWithUiAsync triggers SyncFailed instead of using cache.
                    // Relaunch is needed because stopping GamingServices kills the device app.
                    ValidateDeviceIsLocal(assignment);

                    log($"PC: FlushGrtsAndGoOffline — stopping GamingServices...");
                    RunAdminAction("StopGamingServices", null, log);

                    log($"PC: FlushGrtsAndGoOffline — clearing GRTS cache ({GrtsSaveFolderBase})...");
                    if (Directory.Exists(GrtsSaveFolderBase))
                    {
                        Directory.Delete(GrtsSaveFolderBase, recursive: true);
                        log($"PC: FlushGrtsAndGoOffline — cache deleted.");
                    }
                    else
                    {
                        log($"PC: FlushGrtsAndGoOffline — cache folder not found, nothing to delete.");
                    }

                    log($"PC: FlushGrtsAndGoOffline — disabling network...");
                    RunNetworkAction("DisableNetwork", null, log);

                    log($"PC: FlushGrtsAndGoOffline — starting GamingServices (cold, offline)...");
                    RunAdminAction("StartGamingServices", null, log);

                    // Relaunch the device app — stopping GamingServices kills it
                    string role = assignment.DisplayName;
                    string deviceExecutable = ControllerRuntime.ResolveDeviceExecutablePath();
                    bool forceInproc = assignment.Engine != null &&
                        assignment.Engine.StartsWith("pc-inproc", StringComparison.OrdinalIgnoreCase);
                    var startInfo = new ProcessStartInfo
                    {
                        FileName = deviceExecutable,
                        WorkingDirectory = Path.GetDirectoryName(deviceExecutable) ?? Directory.GetCurrentDirectory(),
                        UseShellExecute = false
                    };
                    if (forceInproc)
                        startInfo.ArgumentList.Add("/forceinproc");

                    log($"PC: FlushGrtsAndGoOffline — relaunching device '{role}'...");
                    Process? launched = Process.Start(startInfo);
                    if (launched != null)
                    {
                        RegisterLaunchedDevice(role, launched.Id);
                        launched.Dispose();
                    }

                    log($"PC: FlushGrtsAndGoOffline complete for '{assignment.DisplayName}'.");
                    break;
                }

                default:
                    log($"ChangeTargetDeviceState: action '{action}' is not supported on PC target '{assignment.DisplayName}' " +
                        $"(engine: {assignment.Engine}). This action may only be valid on Xbox.");
                    break;
            }
        }

        // ────────────────────────────────────────────────────────────────────
        //  Xbox tool execution
        // ────────────────────────────────────────────────────────────────────

        /// <summary>
        /// Resolves the full path to a GDK tool in %GameDK%\bin.
        /// </summary>
        internal static string ResolveGdkToolPath(string toolName)
        {
            string? gameDk = Environment.GetEnvironmentVariable("GameDK");
            if (string.IsNullOrEmpty(gameDk))
            {
                throw new InvalidOperationException(
                    $"Cannot find GDK tool '{toolName}': the GameDK environment variable is not set. " +
                    $"Ensure the Microsoft GDK is installed.");
            }

            string toolPath = Path.Combine(gameDk, "bin", toolName);
            if (!File.Exists(toolPath))
            {
                throw new FileNotFoundException(
                    $"GDK tool not found at '{toolPath}'. Ensure the Microsoft GDK is installed and the GameDK environment variable is correct.",
                    toolPath);
            }

            return toolPath;
        }

        /// <summary>
        /// Runs a GDK xb* tool and throws on non-zero exit code.
        /// </summary>
        internal static void RunXbTool(string toolName, string arguments, Action<string> log, int timeoutMs = DefaultTimeoutMs)
        {
            string toolPath = ResolveGdkToolPath(toolName);
            RunExternalTool(toolPath, arguments, log, timeoutMs);
        }

        /// <summary>
        /// Runs an external tool by full path and throws on non-zero exit code.
        /// </summary>
        internal static void RunExternalTool(string toolPath, string arguments, Action<string> log, int timeoutMs = DefaultTimeoutMs)
        {
            string toolName = Path.GetFileName(toolPath);
            log($"  Executing: {toolName} {RedactSensitiveArgs(arguments)}");

            using var process = new Process();
            process.StartInfo = new ProcessStartInfo
            {
                FileName = toolPath,
                Arguments = arguments,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                CreateNoWindow = true
            };

            process.Start();

            // Read stdout and stderr concurrently to avoid deadlocking when one of the
            // child's pipe buffers fills before we drain the other (PR feedback #4).
            Task<string> stdoutTask = process.StandardOutput.ReadToEndAsync();
            Task<string> stderrTask = process.StandardError.ReadToEndAsync();

            var sw = System.Diagnostics.Stopwatch.StartNew();
            if (!process.WaitForExit(timeoutMs))
            {
                try { process.Kill(); } catch { }
                throw new TimeoutException(
                    $"Tool '{toolName}' timed out after {timeoutMs / 1000}s. Args: {RedactSensitiveArgs(arguments)}");
            }

            // WaitForExit(int) does not guarantee async stream readers have completed;
            // wait for the read tasks with whatever time remains (minimum 5s).
            int remainingMs = Math.Max(5000, timeoutMs - (int)sw.ElapsedMilliseconds);
            try { Task.WaitAll(new Task[] { stdoutTask, stderrTask }, remainingMs); } catch { }
            string stdout = stdoutTask.IsCompletedSuccessfully ? stdoutTask.Result : string.Empty;
            string stderr = stderrTask.IsCompletedSuccessfully ? stderrTask.Result : string.Empty;

            if (!string.IsNullOrWhiteSpace(stdout))
            {
                log($"  {toolName} stdout: {stdout.Trim()}");
            }

            if (process.ExitCode != 0)
            {
                string errDetail = string.IsNullOrWhiteSpace(stderr) ? stdout.Trim() : stderr.Trim();
                throw new InvalidOperationException(
                    $"Tool '{toolName}' failed (exit code {process.ExitCode}): {errDetail}");
            }
        }

        // Redacts password-like values from command-line argument strings before logging.
        // Covers /p:<value>, /password:<value>, --password=<value>, -p <value> forms used by xbuser et al.
        private static readonly Regex s_sensitiveArgPattern = new Regex(
            @"(?ix)
              (?<key> (?:/|--|-) (?: p(?:assword)? ) (?: [:=] | \s+ ) )
              (?<val> \S+ )",
            RegexOptions.Compiled);

        internal static string RedactSensitiveArgs(string arguments)
        {
            if (string.IsNullOrEmpty(arguments)) return arguments;
            return s_sensitiveArgPattern.Replace(arguments, m => m.Groups["key"].Value + "***");
        }

        /// <summary>
        /// Runs a GDK xb* tool and returns (exitCode, stdout). Does not throw on non-zero exit.
        /// </summary>
        internal static (int ExitCode, string Stdout) RunXbToolQuiet(string toolName, string arguments, int timeoutMs = DefaultTimeoutMs)
        {
            string toolPath = ResolveGdkToolPath(toolName);

            using var process = new Process();
            process.StartInfo = new ProcessStartInfo
            {
                FileName = toolPath,
                Arguments = arguments,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                CreateNoWindow = true
            };

            process.Start();
            string stdout = process.StandardOutput.ReadToEnd();
            process.StandardError.ReadToEnd();

            if (!process.WaitForExit(timeoutMs))
            {
                try { process.Kill(); } catch { }
                return (-1, string.Empty);
            }

            return (process.ExitCode, stdout.Trim());
        }

        // ────────────────────────────────────────────────────────────────────
        //  AdminHelper availability (used by PrerequisiteChecker)
        // ────────────────────────────────────────────────────────────────────────

        /// <summary>
        /// PC <c>ChangeTargetDeviceState</c> actions that require AdminHelper
        /// (no fallback path — only AdminHelper can perform these).
        /// </summary>
        internal static readonly HashSet<string> PcActionsRequiringAdminHelper = new(StringComparer.OrdinalIgnoreCase)
        {
            "StopGamingServices",
            "StartGamingServices",
            "FlushGrtsAndGoOffline"
        };

        /// <summary>
        /// PC <c>ChangeTargetDeviceState</c> actions that require admin privilege.
        /// These prefer AdminHelper but can fall back to direct PowerShell when
        /// the controller is running elevated.
        /// </summary>
        internal static readonly HashSet<string> PcActionsRequiringAdminPrivilege = new(StringComparer.OrdinalIgnoreCase)
        {
            "DisableNetwork",
            "EnableNetwork",
            "NetworkFlapping"
        };

        /// <summary>
        /// Ensures AdminHelper is available, launching it if needed (via tray app
        /// or UAC elevation). Returns true if the AdminHelper pipe is reachable
        /// after the attempt. Used by <see cref="PrerequisiteChecker"/> to validate
        /// before scenario execution.
        /// </summary>
        internal static bool EnsureAdminHelperAvailable(Action<string> log)
        {
            log($"  [TRACE] EnsureAdminHelperAvailable called from:\n{Environment.StackTrace}");
            // Quick probe first — already running?
            if (IsAdminHelperPipeReachable())
            {
                log("  [TRACE] AdminHelper pipe already reachable — no launch needed.");
                return true;
            }

            // Try launch (tray → UAC fallback)
            log("  [TRACE] AdminHelper pipe not reachable — attempting launch...");
            return TryLaunchAdminHelper(log);
        }

        /// <summary>
        /// Returns true when the controller process is running with administrator
        /// privileges. Network actions can fall back to direct PowerShell in this case.
        /// </summary>
        internal static bool IsRunningAsAdministrator()
        {
            try
            {
                using var identity = System.Security.Principal.WindowsIdentity.GetCurrent();
                var principal = new System.Security.Principal.WindowsPrincipal(identity);
                return principal.IsInRole(System.Security.Principal.WindowsBuiltInRole.Administrator);
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// Probes the AdminHelper named pipe. Uses a longer timeout (2s) because the pipe
        /// is single-instance and may be briefly occupied by AdminHelpCli between tests.
        /// Falls back to process-existence check if the pipe probe times out.
        /// </summary>
        private static bool IsAdminHelperPipeReachable()
        {
            try
            {
                using var client = new NamedPipeClientStream(".", AdminHelperPipeName, PipeDirection.InOut);
                client.Connect(2_000);
                return true;
            }
            catch (TimeoutException)
            {
                // Pipe busy or not available — check if the process exists as fallback
                return IsAdminHelperProcessRunning();
            }
            catch (IOException) { return false; }
            catch (UnauthorizedAccessException) { return false; }
        }

        /// <summary>
        /// Checks if an AdminHelper process is running (fallback when pipe is busy).
        /// </summary>
        private static bool IsAdminHelperProcessRunning()
        {
            try
            {
                return Process.GetProcessesByName("AdminHelper").Length > 0;
            }
            catch
            {
                return false;
            }
        }

        // ────────────────────────────────────────────────────────────────────
        //  PC network control (AdminHelper pipe → direct PowerShell fallback)
        // ────────────────────────────────────────────────────────────────────────

        private const string AdminHelperPipeName = "PlayFabTestAdminHelper";
        private const string TrayPipeName = "AdminHelperLauncher";
        private const int PipeTimeoutMs = 30_000;

        /// <summary>
        /// Runs a network action via AdminHelper (preferred) or falls back to direct
        /// PowerShell if the helper isn't running and the controller is elevated.
        /// </summary>
        private static void RunNetworkAction(string action, Dictionary<string, object>? parameters, Action<string> log)
        {
            if (TryRunViaAdminHelper(action, parameters, log))
                return;

            // Fallback: direct PowerShell (requires admin)
            log($"  AdminHelper not available, falling back to direct PowerShell (requires admin).");
            ValidateAdministrator();

            switch (action.ToLowerInvariant())
            {
                case "disablenetwork":
                    RunNetworkAdapterPowerShell(disable: true, log);
                    break;
                case "enablenetwork":
                    RunNetworkAdapterPowerShell(disable: false, log);
                    break;
                case "networkflapping":
                {
                    int cycles = 5;
                    int intervalMs = 500;
                    if (parameters != null)
                    {
                        if (parameters.TryGetValue("cycles", out object? c) && c is int ci) cycles = ci;
                        if (parameters.TryGetValue("intervalMs", out object? i) && i is int ii) intervalMs = ii;
                    }
                    for (int j = 0; j < cycles; j++)
                    {
                        RunNetworkAdapterPowerShell(disable: true, log);
                        System.Threading.Thread.Sleep(intervalMs);
                        RunNetworkAdapterPowerShell(disable: false, log);
                        if (j < cycles - 1)
                            System.Threading.Thread.Sleep(intervalMs);
                    }
                    break;
                }
                default:
                    throw new InvalidOperationException($"Unknown network action for fallback: {action}");
            }
        }

        /// <summary>
        /// Runs a privileged action via AdminHelper. Unlike RunNetworkAction, has no
        /// direct PowerShell fallback — AdminHelper must be running.
        /// </summary>
        private static void RunAdminAction(string action, Dictionary<string, object>? parameters, Action<string> log)
        {
            if (!TryRunViaAdminHelper(action, parameters, log))
            {
                throw new InvalidOperationException(
                    $"AdminHelper is required for '{action}' but is not available. " +
                    "Start AdminHelper.exe as Administrator before running this test.");
            }
        }

        /// <summary>
        /// Attempts to send a command to the AdminHelper via named pipe.
        /// If the helper isn't running, launches it (triggers UAC) and retries.
        /// Returns true if the helper handled it, false if unavailable even after launch attempt.
        /// Throws on helper-reported errors.
        /// </summary>
        private static bool TryRunViaAdminHelper(string action, Dictionary<string, object>? parameters, Action<string> log)
        {
            if (TryRunViaAdminHelperOnce(action, parameters, log))
                return true;

            // Pipe not available — but is AdminHelper already running (pipe just busy)?
            if (IsAdminHelperProcessRunning())
            {
                log($"  [TRACE] AdminHelper process exists but pipe unavailable — retrying with longer wait...");
                System.Threading.Thread.Sleep(2_000);
                return TryRunViaAdminHelperOnce(action, parameters, log);
            }

            // Truly not running — try launching AdminHelper
            log($"  [TRACE] TryRunViaAdminHelper: pipe not available, attempting launch for '{action}'...");
            if (!TryLaunchAdminHelper(log))
                return false;

            // Retry after launch
            return TryRunViaAdminHelperOnce(action, parameters, log);
        }

        /// <summary>
        /// Single attempt to connect to AdminHelper via named pipe.
        /// </summary>
        private static bool TryRunViaAdminHelperOnce(string action, Dictionary<string, object>? parameters, Action<string> log)
        {
            try
            {
                using var client = new NamedPipeClientStream(".", AdminHelperPipeName, PipeDirection.InOut);
                client.Connect(5_000); // 5s — pipe is single-instance, may be busy with another request

                // Send newline-delimited JSON request
                var request = new { action, parameters };
                string json = JsonSerializer.Serialize(request);
                byte[] requestBytes = Encoding.UTF8.GetBytes(json + "\n");
                client.Write(requestBytes, 0, requestBytes.Length);
                client.Flush();

                // Read newline-delimited JSON response
                var sb = new StringBuilder();
                int b;
                var sw = Stopwatch.StartNew();
                while ((b = client.ReadByte()) != -1)
                {
                    if (b == '\n') break;
                    sb.Append((char)b);
                    if (sw.ElapsedMilliseconds > PipeTimeoutMs)
                        throw new TimeoutException($"AdminHelper response timed out after {PipeTimeoutMs}ms");
                }

                string responseLine = sb.ToString().TrimEnd('\r');
                if (string.IsNullOrWhiteSpace(responseLine))
                    throw new InvalidOperationException("AdminHelper returned empty response");

                using var doc = JsonDocument.Parse(responseLine);
                bool success = doc.RootElement.GetProperty("success").GetBoolean();
                string? message = doc.RootElement.TryGetProperty("message", out var msgProp) ? msgProp.GetString() : null;

                if (!success)
                    throw new InvalidOperationException($"AdminHelper error: {message}");

                log($"  AdminHelper: {message}");
                return true;
            }
            catch (TimeoutException)
            {
                // Helper not running — fall back
                return false;
            }
            catch (IOException)
            {
                // Pipe not found — fall back
                return false;
            }
        }

        /// <summary>
        /// Attempts to launch AdminHelper.exe. First tries the AdminHelperTray app
        /// (already elevated, no UAC prompt). Falls back to direct elevation with UAC.
        /// Returns true if the process was launched and the pipe became available.
        /// </summary>
        private static bool TryLaunchAdminHelper(Action<string> log)
        {
            // Try the tray app first — no UAC prompt needed if it's running
            if (TryLaunchViaTrayApp(log))
                return true;

            // Fall back to direct launch with UAC elevation
            string controllerDir = AppContext.BaseDirectory;
            string adminHelperPath = Path.Combine(controllerDir, "..", "AdminHelper", "AdminHelper.exe");
            adminHelperPath = Path.GetFullPath(adminHelperPath);

            if (!File.Exists(adminHelperPath))
            {
                log($"  AdminHelper not found at: {adminHelperPath}");
                return false;
            }

            log($"  Launching AdminHelper (will trigger UAC): {adminHelperPath}");
            try
            {
                var psi = new ProcessStartInfo
                {
                    FileName = adminHelperPath,
                    UseShellExecute = true,
                    Verb = "runas", // Triggers UAC elevation
                    WorkingDirectory = Path.GetDirectoryName(adminHelperPath)!
                };
                Process.Start(psi);

                // Wait for the pipe to become available (up to 10s for UAC + startup)
                var sw = Stopwatch.StartNew();
                while (sw.ElapsedMilliseconds < 10_000)
                {
                    System.Threading.Thread.Sleep(500);
                    try
                    {
                        using var probe = new NamedPipeClientStream(".", AdminHelperPipeName, PipeDirection.InOut);
                        probe.Connect(200);
                        log("  AdminHelper launched and pipe is ready.");
                        return true;
                    }
                    catch (TimeoutException) { }
                    catch (IOException) { }
                }

                log("  AdminHelper launched but pipe did not become available within 10s.");
                return false;
            }
            catch (Exception ex)
            {
                // UAC denied or other launch failure
                log($"  Failed to launch AdminHelper: {ex.Message}");
                return false;
            }
        }

        /// <summary>
        /// Attempts to launch AdminHelper via the AdminHelperTray app (already elevated).
        /// Returns true if the tray app handled the request and AdminHelper's pipe is ready.
        /// </summary>
        private static bool TryLaunchViaTrayApp(Action<string> log)
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
                    {
                        log("  Tray app response timed out.");
                        return false;
                    }
                }

                string responseLine = sb.ToString().TrimEnd('\r');
                if (string.IsNullOrWhiteSpace(responseLine))
                    return false;

                using var doc = JsonDocument.Parse(responseLine);
                bool success = doc.RootElement.TryGetProperty("success", out var successProp) && successProp.GetBoolean();

                if (success)
                {
                    log("  AdminHelper launched via tray app (no UAC prompt).");
                    return true;
                }

                string? error = doc.RootElement.TryGetProperty("error", out var errProp) ? errProp.GetString() : "unknown";
                log($"  Tray app failed to launch AdminHelper: {error}");
                return false;
            }
            catch (TimeoutException)
            {
                // Tray app not running
                return false;
            }
            catch (IOException)
            {
                // Tray app not running
                return false;
            }
            catch (Exception ex)
            {
                log($"  Tray app connection error: {ex.Message}");
                return false;
            }
        }

        private static void RunNetworkAdapterPowerShell(bool disable, Action<string> log)
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
            process.WaitForExit(30_000);

            if (!string.IsNullOrWhiteSpace(stdout))
            {
                log($"  PowerShell stdout: {stdout.Trim()}");
            }

            if (process.ExitCode != 0)
            {
                string errDetail = string.IsNullOrWhiteSpace(stderr) ? "(no stderr)" : stderr.Trim();
                throw new InvalidOperationException(
                    $"PowerShell network adapter command failed (exit code {process.ExitCode}): {errDetail}. " +
                    $"Ensure the controller process is running as Administrator.");
            }
        }

        // ────────────────────────────────────────────────────────────────────
        //  Validation helpers
        // ────────────────────────────────────────────────────────────────────

        private static void ValidateDeviceIsLocal(DeviceAssignment assignment)
        {
            string endpoint = assignment.RemoteEndpoint ?? string.Empty;
            string host = endpoint;
            int colonIndex = endpoint.LastIndexOf(':');
            if (colonIndex > 0)
            {
                host = endpoint.Substring(0, colonIndex);
            }

            host = host.Trim('[', ']');

            if (string.IsNullOrWhiteSpace(host))
            {
                throw new InvalidOperationException(
                    $"ChangeTargetDeviceState cannot determine whether device '{assignment.DisplayName}' is local " +
                    $"(remote endpoint is '{endpoint}'). This command only works on the same machine as the controller.");
            }

            if (IPAddress.TryParse(host, out IPAddress? address))
            {
                if (IPAddress.IsLoopback(address))
                    return;

                try
                {
                    var hostAddresses = Dns.GetHostAddresses(Dns.GetHostName());
                    foreach (var local in hostAddresses)
                    {
                        if (address.Equals(local))
                            return;
                    }
                }
                catch { }
            }

            throw new InvalidOperationException(
                $"ChangeTargetDeviceState only works on devices running on the same machine as the controller. " +
                $"Device '{assignment.DisplayName}' connected from '{endpoint}', which is not a local address.");
        }

        private static void ValidateAdministrator()
        {
            using var identity = System.Security.Principal.WindowsIdentity.GetCurrent();
            var principal = new System.Security.Principal.WindowsPrincipal(identity);
            if (!principal.IsInRole(System.Security.Principal.WindowsBuiltInRole.Administrator))
            {
                throw new InvalidOperationException(
                    "ChangeTargetDeviceState requires the controller process to be running as Administrator. " +
                    "Please restart the controller with elevated privileges.");
            }
        }

        // ────────────────────────────────────────────────────────────────────
        //  Parameter extraction helpers
        // ────────────────────────────────────────────────────────────────────

        /// <summary>
        /// Returns the console's IP for an xbox device assignment, derived from the endpoint the
        /// console connected to the controller from. Throws when the assignment carries no usable
        /// endpoint.
        /// </summary>
        internal static string ExtractXboxIp(DeviceAssignment assignment)
        {
            // For Xbox targets, RemoteEndpoint is the console IP:port from WebSocket.
            string endpoint = assignment.RemoteEndpoint ?? string.Empty;
            string host = endpoint;
            int colonIndex = endpoint.LastIndexOf(':');
            if (colonIndex > 0)
            {
                host = endpoint.Substring(0, colonIndex);
            }
            host = host.Trim('[', ']');

            if (string.IsNullOrWhiteSpace(host))
            {
                throw new InvalidOperationException(
                    $"Cannot determine Xbox IP for device '{assignment.DisplayName}' (endpoint: '{endpoint}').");
            }

            return host;
        }

        private static string GetStringParam(IDictionary<string, object>? parameters, string key)
        {
            if (parameters != null && parameters.TryGetValue(key, out object? value))
            {
                string? s = value?.ToString()?.Trim();
                if (!string.IsNullOrEmpty(s))
                    return s;
            }
            throw new ArgumentException($"ChangeTargetDeviceState: required parameter '{key}' is missing or empty.");
        }

        private static string GetOptionalStringParam(IDictionary<string, object>? parameters, string key)
        {
            if (parameters != null && parameters.TryGetValue(key, out object? value))
            {
                return value?.ToString()?.Trim() ?? string.Empty;
            }
            return string.Empty;
        }

        private static int GetIntParam(IDictionary<string, object>? parameters, string key, int defaultValue)
        {
            if (parameters != null && parameters.TryGetValue(key, out object? value))
            {
                if (value is int i) return i;
                if (value is long l) return (int)l;
                if (int.TryParse(value?.ToString(), out int parsed)) return parsed;
            }
            return defaultValue;
        }

        private static bool GetBoolParam(IDictionary<string, object>? parameters, string key, bool defaultValue)
        {
            if (parameters != null && parameters.TryGetValue(key, out object? value))
            {
                if (value is bool b) return b;
                if (bool.TryParse(value?.ToString(), out bool parsed)) return parsed;
            }
            return defaultValue;
        }

        /// <summary>
        /// Queries xbuser list to find the currently signed-in user's email on the Xbox.
        /// Returns the email if found, or empty string if no user is signed in.
        /// </summary>
        private static string QueryCurrentXboxUserEmail(string ip, Action<string> log)
        {
            try
            {
                var (exitCode, stdout) = RunXbToolQuiet("xbuser.exe", $"/x:{ip} list");
                if (exitCode == 0 && !string.IsNullOrEmpty(stdout))
                {
                    // xbuser list output format includes lines like:
                    // Email: user@example.com
                    // or the email appears on a line with user info
                    foreach (var line in stdout.Split('\n'))
                    {
                        var trimmed = line.Trim();
                        // Look for email pattern in the output
                        if (trimmed.Contains("@") && !trimmed.StartsWith("//"))
                        {
                            // Try to extract email from various formats
                            // Format: "  email@domain.com  SignedIn" or "Email: email@domain.com"
                            var parts = trimmed.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                            foreach (var part in parts)
                            {
                                if (part.Contains("@") && part.Contains("."))
                                {
                                    _lastSignedOutEmail = part;
                                    return part;
                                }
                            }
                        }
                    }
                }
                log($"Xbox [{ip}]: could not determine current user from xbuser list");
            }
            catch (Exception ex)
            {
                log($"Xbox [{ip}]: xbuser list failed: {ex.Message}");
            }
            return string.Empty;
        }

        /// <summary>
        /// Builds the xbuser identity argument from either "email" or "userId" parameter.
        /// </summary>
        private static string GetUserArg(IDictionary<string, object>? parameters)
        {
            string email = GetOptionalStringParam(parameters, "email");
            if (!string.IsNullOrEmpty(email))
                return $"/e:{email}";

            string userId = GetOptionalStringParam(parameters, "userId");
            if (!string.IsNullOrEmpty(userId))
                return $"/i:{userId}";

            // No email or userId — sign out current/default user
            return string.Empty;
        }
    }
}
