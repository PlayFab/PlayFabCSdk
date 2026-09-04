using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace GameTestController
{
    /// <summary>
    /// Validates environment prerequisites for a scenario before execution begins.
    /// Uses controller-side GDK tools (xbuser, xbconfig) for Xbox device checks.
    /// Provides clear remediation guidance when prerequisites are not met.
    /// </summary>
    internal sealed class PrerequisiteChecker
    {
        private readonly Action<string> _log;

        // Cache sandbox per device IP for the duration of a batch run (sandbox rarely changes)
        private readonly Dictionary<string, string> _sandboxCache = new(StringComparer.OrdinalIgnoreCase);

        // Cache sflash.exe availability (doesn't change during a run)
        private bool? _sflashAvailable;

        public PrerequisiteChecker(Action<string> log)
        {
            _log = log ?? throw new ArgumentNullException(nameof(log));
        }

        /// <summary>
        /// Checks prerequisites for all roles in a scenario. Returns null if all pass,
        /// or a human-readable skip reason with remediation guidance if any fail.
        /// </summary>
        public string? CheckPrerequisites(
            ScenarioManifest manifest,
            IReadOnlyList<ScenarioCommandInvocation> commands,
            IReadOnlyList<ScenarioCleanupInvocation> cleanupCommands,
            IReadOnlyDictionary<string, DeviceAssignment> roleAssignments)
        {
            var failures = new List<string>();

            foreach (var (role, device) in manifest.Devices)
            {
                if (device.Prerequisites == null)
                    continue;

                if (!roleAssignments.TryGetValue(role, out DeviceAssignment assignment))
                    continue;

                var roleFailures = CheckRolePrerequisites(role, device.Prerequisites, assignment);
                failures.AddRange(roleFailures);
            }

            // Scan commands + cleanup for ChangeTargetDeviceState actions that need AdminHelper
            string? adminHelperFailure = CheckAdminHelperRequired(commands, cleanupCommands, roleAssignments);
            if (adminHelperFailure != null)
                failures.Add(adminHelperFailure);

            // Check if any PC NIC manipulation actions are present but ALLOW_PC_NIC_DISABLE is not set
            string? nicFailure = CheckPcNicManipulationAllowed(commands, cleanupCommands, roleAssignments);
            if (nicFailure != null)
                failures.Add(nicFailure);

            if (failures.Count == 0)
                return null;

            var sb = new StringBuilder();
            sb.AppendLine("Prerequisites not met:");
            foreach (var failure in failures)
            {
                sb.AppendLine(failure);
            }
            return sb.ToString().TrimEnd();
        }

        private List<string> CheckRolePrerequisites(
            string role,
            ScenarioPrerequisites prereqs,
            DeviceAssignment assignment)
        {
            var failures = new List<string>();
            bool isXbox = string.Equals(assignment.Engine, "xbox", StringComparison.OrdinalIgnoreCase);

            // Check xboxUsers requirement
            if (prereqs.XboxUsers.HasValue && prereqs.XboxUsers.Value > 1 && isXbox)
            {
                string? userFailure = CheckXboxUsers(role, prereqs.XboxUsers.Value, assignment);
                if (userFailure != null)
                    failures.Add(userFailure);
            }

            // Check sandbox requirement
            if (!string.IsNullOrWhiteSpace(prereqs.Sandbox) && isXbox)
            {
                string? sandboxFailure = CheckSandbox(role, prereqs.Sandbox!, assignment);
                if (sandboxFailure != null)
                    failures.Add(sandboxFailure);
            }

            // Check sflash requirement (for standby/shutdown/power-loss tests)
            if (prereqs.Sflash == true)
            {
                string? sflashFailure = CheckSflash(role);
                if (sflashFailure != null)
                    failures.Add(sflashFailure);
            }

            return failures;
        }

        private string? CheckXboxUsers(string role, int requiredCount, DeviceAssignment assignment)
        {
            string xboxIp = ExtractIp(assignment);
            if (string.IsNullOrEmpty(xboxIp))
                return $"  [{role}] ✗ xboxUsers: Cannot determine Xbox IP to verify user count.";

            try
            {
                var (exitCode, stdout) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} list");
                if (exitCode != 0)
                {
                    return $"  [{role}] ✗ xboxUsers: Failed to query Xbox users (xbuser exit code {exitCode}).";
                }

                // Count lines matching "Signed in: Yes"
                int signedInCount = Regex.Matches(stdout, @"Signed in:\s*Yes", RegexOptions.IgnoreCase).Count;

                // Load account config once for hints and overlap checks
                var accountConfig = TestAccountConfig.Instance;

                if (signedInCount < requiredCount)
                {
                    // Auto-sign-in: find users that are present but not signed in, sign them in
                    var userIdMatches = Regex.Matches(stdout, @"UserId:\s*(\d+)");
                    var signedInFlags = Regex.Matches(stdout, @"Signed in:\s*(Yes|No)", RegexOptions.IgnoreCase);
                    var signedInUserIds = new List<string>();

                    for (int i = 0; i < userIdMatches.Count && i < signedInFlags.Count; i++)
                    {
                        string userId = userIdMatches[i].Groups[1].Value;
                        bool isSignedIn = signedInFlags[i].Groups[1].Value.Equals("Yes", StringComparison.OrdinalIgnoreCase);
                        if (!isSignedIn)
                        {
                            _log($"  [{role}] Auto-signing in UserId {userId} on Xbox {xboxIp}...");
                            var (signInExit, signInOut) = DeviceStateController.RunXbToolQuiet(
                                "xbuser.exe", $"/X {xboxIp} signin /i:{userId}", timeoutMs: 15000);
                            if (signInExit == 0)
                            {
                                _log($"  [{role}] ✓ UserId {userId} signed in successfully.");
                                signedInUserIds.Add(userId);
                            }
                            else
                            {
                                _log($"  [{role}] ✗ Failed to sign in UserId {userId}: {signInOut.Trim()}");
                            }
                        }
                        else
                        {
                            signedInUserIds.Add(userId);
                        }
                    }

                    // Re-check signed-in count after auto-sign-in
                    int newSignedInCount = signedInUserIds.Count;
                    if (newSignedInCount < requiredCount)
                    {
                        var sb = new StringBuilder();
                        sb.AppendLine($"  [{role}] ✗ xboxUsers: {newSignedInCount} user(s) signed in after auto-sign-in, but {requiredCount} required.");
                        sb.AppendLine($"    → Sign in additional Xbox test account(s) on the devkit:");

                        if (accountConfig != null)
                        {
                            foreach (string email in accountConfig.GetXboxSignInHints())
                            {
                                sb.AppendLine($"      xbuser /X {xboxIp} signin {email}");
                            }
                        }
                        else
                        {
                            sb.AppendLine($"      xbuser /X {xboxIp} signin <xbox-test-account-email>");
                            sb.AppendLine($"    → Configure accounts in Test/testAccountConfig.json");
                        }

                        sb.AppendLine($"    → Or use the Xbox Manager app to sign in users on the console.");
                        return sb.ToString().TrimEnd();
                    }

                    _log($"  [{role}] ✓ xboxUsers: {newSignedInCount} user(s) now signed in after auto-sign-in (required: {requiredCount})");
                }

                _log($"  [{role}] ✓ xboxUsers: {signedInCount} user(s) signed in (required: {requiredCount})");

                // Check for account overlap: warn if any Xbox user matches the PC account
                if (accountConfig != null)
                {
                    var emailMatches = Regex.Matches(stdout, @"Email:\s*(\S+@\S+)", RegexOptions.IgnoreCase);
                    foreach (Match emailMatch in emailMatches)
                    {
                        string xboxEmail = emailMatch.Groups[1].Value;
                        if (accountConfig.IsOverlappingWithPc(xboxEmail))
                        {
                            return $"  [{role}] ✗ xboxUsers: Account '{xboxEmail}' is signed in on both Xbox and PC.\n" +
                                   $"    → This will cause 'signed in on another device' dialogs during two-device tests.\n" +
                                   $"    → Remove this account from the Xbox: xbuser /X {xboxIp} signout {xboxEmail}\n" +
                                   $"    → Then sign in a non-overlapping account (see Test/testAccountConfig.json).";
                        }
                    }
                }

                return null;
            }
            catch (Exception ex)
            {
                return $"  [{role}] ✗ xboxUsers: Error checking users — {ex.Message}";
            }
        }

        private string? CheckSandbox(string role, string requiredSandbox, DeviceAssignment assignment)
        {
            string xboxIp = ExtractIp(assignment);
            if (string.IsNullOrEmpty(xboxIp))
                return $"  [{role}] ✗ sandbox: Cannot determine Xbox IP to verify sandbox.";

            try
            {
                // Check cache first
                if (_sandboxCache.TryGetValue(xboxIp, out string? cached))
                {
                    if (string.Equals(cached, requiredSandbox, StringComparison.OrdinalIgnoreCase))
                    {
                        _log($"  [{role}] ✓ sandbox: {cached} (cached)");
                        return null;
                    }

                    return FormatSandboxFailure(role, cached, requiredSandbox, xboxIp);
                }

                var (exitCode, stdout) = DeviceStateController.RunXbToolQuiet("xbconfig.exe", $"/X {xboxIp} SandboxId");
                if (exitCode != 0)
                {
                    return $"  [{role}] ✗ sandbox: Failed to query sandbox (xbconfig exit code {exitCode}).";
                }

                // Parse "SandboxId: XDKS.1"
                var match = Regex.Match(stdout, @"SandboxId:\s*(\S+)", RegexOptions.IgnoreCase);
                string actualSandbox = match.Success ? match.Groups[1].Value : stdout.Trim();

                // Cache result
                _sandboxCache[xboxIp] = actualSandbox;

                if (!string.Equals(actualSandbox, requiredSandbox, StringComparison.OrdinalIgnoreCase))
                {
                    return FormatSandboxFailure(role, actualSandbox, requiredSandbox, xboxIp);
                }

                _log($"  [{role}] ✓ sandbox: {actualSandbox}");
                return null;
            }
            catch (Exception ex)
            {
                return $"  [{role}] ✗ sandbox: Error checking sandbox — {ex.Message}";
            }
        }

        private static string FormatSandboxFailure(string role, string actual, string required, string xboxIp)
        {
            var sb = new StringBuilder();
            sb.AppendLine($"  [{role}] ✗ sandbox: Device is in sandbox '{actual}', but '{required}' is required.");
            sb.AppendLine($"    → Change sandbox and reboot:");
            sb.AppendLine($"      xbconfig /X {xboxIp} SandboxId={required}");
            sb.AppendLine($"      xbreboot /X {xboxIp}");
            return sb.ToString().TrimEnd();
        }

        private static string ExtractIp(DeviceAssignment assignment)
        {
            string endpoint = assignment.RemoteEndpoint ?? string.Empty;
            string host = endpoint;
            int colonIndex = endpoint.LastIndexOf(':');
            if (colonIndex > 0)
            {
                host = endpoint.Substring(0, colonIndex);
            }
            return host.Trim('[', ']');
        }

        /// <summary>
        /// Checks whether sflash.exe is available on the PATH.
        /// Required for tests that simulate connected standby, shutdown, or power-loss
        /// via hardware-level Xbox device control.
        /// </summary>
        private string? CheckSflash(string role)
        {
            if (_sflashAvailable.HasValue)
            {
                if (_sflashAvailable.Value)
                {
                    _log($"  [{role}] ✓ sflash: sflash.exe available (cached)");
                    return null;
                }
                return FormatSflashFailure(role);
            }

            try
            {
                var (exitCode, _) = DeviceStateController.RunXbToolQuiet("sflash.exe", "/?", timeoutMs: 5000);
                _sflashAvailable = (exitCode == 0);
            }
            catch
            {
                // sflash.exe not found on PATH
                _sflashAvailable = false;
            }

            if (_sflashAvailable.Value)
            {
                _log($"  [{role}] ✓ sflash: sflash.exe available");
                return null;
            }

            return FormatSflashFailure(role);
        }

        private static string FormatSflashFailure(string role)
        {
            var sb = new StringBuilder();
            sb.AppendLine($"  [{role}] ✗ sflash: sflash.exe not found on PATH.");
            sb.AppendLine($"    → This test requires sflash.exe for hardware-level device control");
            sb.AppendLine($"      (connected standby, shutdown, or power-loss simulation).");
            sb.AppendLine($"    → Ensure sflash.exe is installed and available on the system PATH.");
            sb.AppendLine($"    → See Xbox devkit documentation for sflash.exe installation.");
            return sb.ToString().TrimEnd();
        }

        // ────────────────────────────────────────────────────────────────────
        //  AdminHelper availability check (auto-detected from command scan)
        // ────────────────────────────────────────────────────────────────────

        // Cache per batch run — AdminHelper availability doesn't change within a run
        private bool? _adminHelperAvailable;
        private bool? _isElevated;

        /// <summary>
        /// Scans all commands (execution + cleanup) for <c>ChangeTargetDeviceState</c>
        /// actions on PC devices that require AdminHelper or admin elevation.
        /// Returns null if no admin actions are needed or if AdminHelper is available;
        /// returns a failure message with remediation guidance otherwise.
        /// </summary>
        private string? CheckAdminHelperRequired(
            IReadOnlyList<ScenarioCommandInvocation> commands,
            IReadOnlyList<ScenarioCleanupInvocation> cleanupCommands,
            IReadOnlyDictionary<string, DeviceAssignment> roleAssignments)
        {
            var adminHelperActions = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            var adminPrivilegeActions = new HashSet<string>(StringComparer.OrdinalIgnoreCase);

            // Scan execution commands
            foreach (var cmd in commands)
            {
                ClassifyAdminAction(cmd.Step, cmd.Role, roleAssignments, adminHelperActions, adminPrivilegeActions);
            }

            // Scan cleanup commands
            foreach (var cleanup in cleanupCommands)
            {
                ClassifyAdminAction(cleanup.Step, cleanup.Role, roleAssignments, adminHelperActions, adminPrivilegeActions);
            }

            if (adminHelperActions.Count == 0 && adminPrivilegeActions.Count == 0)
                return null;

            // Check AdminHelper availability (cached)
            if (!_adminHelperAvailable.HasValue)
            {
                _adminHelperAvailable = DeviceStateController.EnsureAdminHelperAvailable(_log);
            }

            if (_adminHelperAvailable.Value)
            {
                var allActions = new HashSet<string>(adminHelperActions, StringComparer.OrdinalIgnoreCase);
                allActions.UnionWith(adminPrivilegeActions);
                _log($"  ✓ AdminHelper: available (needed for {string.Join(", ", allActions)})");
                return null;
            }

            // AdminHelper not available — check if elevation alone is sufficient
            if (adminHelperActions.Count == 0)
            {
                // Only network-type actions — check if running elevated
                if (!_isElevated.HasValue)
                {
                    _isElevated = DeviceStateController.IsRunningAsAdministrator();
                }

                if (_isElevated.Value)
                {
                    _log($"  ✓ AdminHelper: not running, but controller is elevated — " +
                         $"network actions ({string.Join(", ", adminPrivilegeActions)}) will use direct PowerShell.");
                    return null;
                }
            }

            // Build failure message
            return FormatAdminHelperFailure(adminHelperActions, adminPrivilegeActions);
        }

        /// <summary>
        /// Extracts the action from a <c>ChangeTargetDeviceState</c> step and classifies
        /// it into the appropriate admin requirement set if the role targets a PC engine.
        /// </summary>
        private static void ClassifyAdminAction(
            ScenarioStep step,
            string role,
            IReadOnlyDictionary<string, DeviceAssignment> roleAssignments,
            HashSet<string> adminHelperActions,
            HashSet<string> adminPrivilegeActions)
        {
            if (!string.Equals(step.Command, "ChangeTargetDeviceState", StringComparison.OrdinalIgnoreCase))
                return;

            string? action = null;
            if (step.Parameters != null && step.Parameters.TryGetValue("action", out object? actionVal))
                action = actionVal?.ToString()?.Trim();

            if (string.IsNullOrEmpty(action))
                return;

            // Only relevant for PC engines
            if (!roleAssignments.TryGetValue(role, out DeviceAssignment assignment))
                return;

            string engine = assignment.Engine?.ToLowerInvariant() ?? "";
            if (!engine.StartsWith("pc-"))
                return;

            if (DeviceStateController.PcActionsRequiringAdminHelper.Contains(action))
                adminHelperActions.Add(action);
            else if (DeviceStateController.PcActionsRequiringAdminPrivilege.Contains(action))
                adminPrivilegeActions.Add(action);
        }

        /// <summary>
        /// Checks if any PC NIC manipulation actions (DisableNetwork, EnableNetwork,
        /// FlushGrtsAndGoOffline, etc.) are present and ALLOW_PC_NIC_DISABLE is not set.
        /// Returns a skip reason if blocked, null if OK.
        /// </summary>
        private string? CheckPcNicManipulationAllowed(
            IReadOnlyList<ScenarioCommandInvocation> commands,
            IReadOnlyList<ScenarioCleanupInvocation> cleanupCommands,
            IReadOnlyDictionary<string, DeviceAssignment> roleAssignments)
        {
            if (DeviceStateController.IsPcNicManipulationAllowed())
                return null;

            var nicActions = new HashSet<string>(StringComparer.OrdinalIgnoreCase);

            foreach (var cmd in commands)
                CollectNicAction(cmd.Step, cmd.Role, roleAssignments, nicActions);
            foreach (var cleanup in cleanupCommands)
                CollectNicAction(cleanup.Step, cleanup.Role, roleAssignments, nicActions);

            if (nicActions.Count == 0)
                return null;

            _log($"  [SKIP] NIC manipulation blocked: scenario uses {string.Join(", ", nicActions)} on a local PC target " +
                 $"but {DeviceStateController.AllowPcNicDisableEnv} is not set.");
            return $"  SKIP (NIC disabled): test requires {string.Join(", ", nicActions)} -- " +
                   $"set {DeviceStateController.AllowPcNicDisableEnv}=1 to enable";
        }

        private static void CollectNicAction(
            ScenarioStep step,
            string role,
            IReadOnlyDictionary<string, DeviceAssignment> roleAssignments,
            HashSet<string> nicActions)
        {
            if (!string.Equals(step.Command, "ChangeTargetDeviceState", StringComparison.OrdinalIgnoreCase))
                return;

            string? action = null;
            if (step.Parameters != null && step.Parameters.TryGetValue("action", out object? actionVal))
                action = actionVal?.ToString()?.Trim();

            if (string.IsNullOrEmpty(action))
                return;

            if (!roleAssignments.TryGetValue(role, out DeviceAssignment assignment))
                return;

            string engine = assignment.Engine?.ToLowerInvariant() ?? "";
            if (!engine.StartsWith("pc-"))
                return;

            if (DeviceStateController.PcActionsRequiringNic.Contains(action))
                nicActions.Add(action);
        }

        private static string FormatAdminHelperFailure(
            HashSet<string> adminHelperActions,
            HashSet<string> adminPrivilegeActions)
        {
            var sb = new StringBuilder();
            var allActions = new HashSet<string>(adminHelperActions, StringComparer.OrdinalIgnoreCase);
            allActions.UnionWith(adminPrivilegeActions);

            sb.AppendLine($"  ✗ AdminHelper: not available but required for: {string.Join(", ", allActions)}");
            sb.AppendLine($"    → Start AdminHelperTray.exe (runs in system tray, no UAC popups)");
            sb.AppendLine($"    → Or start AdminHelper.exe as Administrator");

            if (adminHelperActions.Count == 0)
            {
                // Only network actions — elevation is an alternative
                sb.AppendLine($"    → Or run the controller as Administrator (direct PowerShell fallback for network actions)");
            }
            else if (adminPrivilegeActions.Count > 0)
            {
                sb.AppendLine($"    Note: {string.Join(", ", adminHelperActions)} strictly require AdminHelper;");
                sb.AppendLine($"    {string.Join(", ", adminPrivilegeActions)} could also work if controller runs as Administrator.");
            }

            return sb.ToString().TrimEnd();
        }
    }
}
