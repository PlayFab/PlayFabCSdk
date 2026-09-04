using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace GameTestController
{
    internal sealed class ScenarioRunner
    {
        private readonly CommandProcessor _commandProcessor;
        private readonly ControllerTransport _transport;
        private readonly JsonSerializerOptions _jsonOptions;
        private readonly Action<string, bool, bool> _logger;
        private readonly SemaphoreSlim _runLock = new SemaphoreSlim(1, 1);
        private readonly Func<ChaosModeScenarioParameters>? _chaosSettingsProvider;
        private readonly Func<string?>? _logDirectoryProvider;
        private readonly Func<string?>? _sourceDataFolderProvider;
        private readonly PrerequisiteChecker _prerequisiteChecker;

        // Test infrastructure servers (managed per-scenario)
        private HttpTestServer? _httpTestServer;
        private WebSocketEchoServer? _wsEchoServer;
        private GameSaveUiNavigator? _uiNavigator;
        private Xbox.XboxUiNavigator? _xboxUiNavigator;
        private string? _uiNavigatorRole;
        private readonly Dictionary<string, string> _templateVariables = new(StringComparer.OrdinalIgnoreCase);

        /// <summary>
        /// Stores per-role result data from command executions to support cross-role variable
        /// references in scenario parameters. E.g., "${PlayerB.playerId}" resolves to the
        /// "playerId" field from PlayerB's last command result that contained it.
        /// Key: "Role.field" (case-insensitive). Value: the string value of that field.
        /// </summary>
        private readonly Dictionary<string, string> _roleResultVariables = new(StringComparer.OrdinalIgnoreCase);


        /// <summary>
        /// Tracks auto-response actions set by device commands during scenario execution.
        /// Key: "{Role}:{NormalizedCommandName}" (e.g., "DeviceA:Conflict"). Value: the action string.
        /// Populated as PFGameSaveFilesSetUi*AutoResponse commands are dispatched to devices.
        /// </summary>
        private readonly Dictionary<string, string> _trackedAutoResponses = new(StringComparer.OrdinalIgnoreCase);
        private readonly Dictionary<string, int> _trackedMaxRetries = new(StringComparer.OrdinalIgnoreCase);

        /// <summary>
        /// Latest title_player_account entity id (16 hex chars) seen flowing back from a
        /// device during the run. Captured from action-result JSON (e.g. PFEntityGetEntityKey's
        /// "entityId" field, or any "title_player_account!XXXX" reference). Used to drive
        /// pfgamesaveutil cloud downloads. Null until a player has logged in.
        /// </summary>
        private string? _capturedTitlePlayerId;

        /// <summary>
        /// Destination folder of the most recent PfGameSaveUtilDownload, so a following
        /// VerifyGameSaveUtilDownload can default to it without repeating the path.
        /// </summary>
        private string? _lastUtilDownloadPath;

        /// <summary>
        /// Authoritative ring-buffer manifest captured from the latest StopRingBufferWriter
        /// result. Used as the data-loss ground truth by VerifyRingBufferIntegrity. Null
        /// until a ring writer has been stopped.
        /// </summary>
        private RingManifest? _ringManifest;

        /// <summary>Matches a 16-hex title_player_account id, either bare ("entityId":"..")
        /// or embedded in a service URL ("title_player_account!XXXX" / "%21XXXX").</summary>
        private static readonly Regex TitlePlayerIdRegex = new(
            "title_player_account(?:!|%21)(?<id>[A-Fa-f0-9]{16})",
            RegexOptions.Compiled);

        private static readonly Regex EntityIdFieldRegex = new(
            "\"entityId\"\\s*:\\s*\"(?<id>[A-Fa-f0-9]{16})\"",
            RegexOptions.Compiled);


        /// <summary>
        /// Maps auto-response command names to a short key used in <see cref="_trackedAutoResponses"/>.
        /// Includes both the "AutoResponse" and "Response" variants.
        /// </summary>
        private static readonly Dictionary<string, string> AutoResponseCommandKeys = new(StringComparer.OrdinalIgnoreCase)
        {
            ["PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse"] = "ActiveDeviceContention",
            ["PFGameSaveFilesSetUiActiveDeviceContentionResponse"] = "ActiveDeviceContention",
            ["PFGameSaveFilesSetUiConflictAutoResponse"] = "Conflict",
            ["PFGameSaveFilesSetUiConflictResponse"] = "Conflict",
            ["PFGameSaveFilesSetUiOutOfStorageAutoResponse"] = "OutOfStorage",
            ["PFGameSaveFilesSetUiOutOfStorageResponse"] = "OutOfStorage",
            ["PFGameSaveFilesSetUiSyncFailedAutoResponse"] = "SyncFailed",
            ["PFGameSaveFilesSetUiSyncFailedResponse"] = "SyncFailed",
            ["PFGameSaveFilesSetUiProgressAutoResponse"] = "Progress",
            ["PFGameSaveFilesSetUiProgressResponse"] = "Progress",
        };

        /// <summary>
        /// After <see cref="RunAsync"/> returns <see cref="ScenarioRunOutcome.NotStarted"/>,
        /// this property contains a human-readable reason (e.g., engine mismatch).
        /// Reset at the start of each run. May also be set by the caller for pre-flight skips.
        /// </summary>
        public string? LastSkipReason { get; set; }

        /// <summary>
        /// After <see cref="RunAsync"/> completes, contains the role names (e.g., "DeviceA")
        /// and engine types of devices that were force-disconnected due to timeout. The caller
        /// can use this to restart those devices before the next scenario.
        /// Reset at the start of each run.
        /// </summary>
        public IReadOnlyList<(string Role, string Engine)> LastForcedDisconnectedDevices { get; private set; } = Array.Empty<(string, string)>();

        /// <summary>
        /// True if the last scenario that ran assigned an Xbox device to any role.
        /// Used to determine whether a device reset is needed between tests.
        /// </summary>
        public bool LastScenarioUsedXboxDevice { get; private set; }

        public ScenarioRunner(
            CommandProcessor commandProcessor,
            ControllerTransport transport,
            JsonSerializerOptions jsonOptions,
            Action<string, bool, bool> logger,
            Func<ChaosModeScenarioParameters>? chaosSettingsProvider = null,
            Func<string?>? logDirectoryProvider = null,
            Func<string?>? sourceDataFolderProvider = null)
        {
            _commandProcessor = commandProcessor;
            _transport = transport;
            _jsonOptions = jsonOptions;
            _logger = logger;
            _chaosSettingsProvider = chaosSettingsProvider;
            _logDirectoryProvider = logDirectoryProvider;
            _sourceDataFolderProvider = sourceDataFolderProvider;
            _prerequisiteChecker = new PrerequisiteChecker(msg => Log(msg));
        }

    public async Task<ScenarioRunOutcome> RunAsync(ScenarioPlan plan, string? customIdPrefix = null, CancellationToken cancellationToken = default)
        {
            if (plan == null)
            {
                throw new ArgumentNullException(nameof(plan));
            }

            if (!await _runLock.WaitAsync(0, cancellationToken).ConfigureAwait(false))
            {
                Log("Scenario execution already in progress.");
                return ScenarioRunOutcome.NotStarted;
            }

            Dictionary<string, DeviceAssignment>? roleAssignments = null;
            var debugStatsEligibleClients = new HashSet<Guid>();
            var pfGameSaveInitializedClients = new HashSet<Guid>();
            var forcedDisconnectedClients = new HashSet<Guid>();
            ScenarioRunOutcome finalOutcome = ScenarioRunOutcome.NotStarted;
            LastSkipReason = null;
            LastForcedDisconnectedDevices = Array.Empty<(string, string)>();
            OperationCanceledException? cancellationException = null;
            _trackedAutoResponses.Clear();
            _trackedMaxRetries.Clear();
            string scenarioIdContext = DetermineScenarioId(plan);
            string? scenarioNameContext = string.IsNullOrWhiteSpace(plan.Manifest.Name) ? null : plan.Manifest.Name;

            string? effectivePrefix = string.IsNullOrWhiteSpace(customIdPrefix) ? null : customIdPrefix;

            try
            {
                if (plan.Commands.Count == 0)
                {
                    Log($"Scenario '{plan.Manifest.Id}' has no commands to execute.");
                    finalOutcome = ScenarioRunOutcome.NotStarted;
                    return finalOutcome;
                }

                var assignments = new Dictionary<string, DeviceAssignment>(StringComparer.OrdinalIgnoreCase);
                if (!await TryBuildRoleAssignmentsAsync(plan, assignments, cancellationToken).ConfigureAwait(false))
                {
                    finalOutcome = ScenarioRunOutcome.NotStarted;
                    return finalOutcome;
                }

                // Track whether this scenario uses an Xbox device (for reset decisions)
                LastScenarioUsedXboxDevice = assignments.Values.Any(a =>
                    string.Equals(a.Engine, "xbox", StringComparison.OrdinalIgnoreCase));

                // Validate environment prerequisites (e.g., Xbox users signed in, sandbox, AdminHelper)
                // before executing any commands. Provides clear guidance on what's needed.
                string? prereqFailure = _prerequisiteChecker.CheckPrerequisites(plan.Manifest, plan.Commands, plan.CleanupCommands, assignments);
                if (prereqFailure != null)
                {
                    Log(prereqFailure);
                    LastSkipReason = prereqFailure;
                    finalOutcome = ScenarioRunOutcome.NotStarted;
                    return finalOutcome;
                }

                roleAssignments = assignments;

                // Only run AdminHelper-dependent recovery checks for GRTS/Xbox engines.
                // Inproc tests never call DisableNetwork or StopGamingServices, so they
                // don't need these checks and shouldn't trigger AdminHelper launch.
                bool usesGrts = assignments.Values.Any(a =>
                    string.Equals(a.Engine, "pc-grts", StringComparison.OrdinalIgnoreCase) ||
                    string.Equals(a.Engine, "xbox", StringComparison.OrdinalIgnoreCase));

                if (usesGrts)
                {
                    // Ensure network is up (recovers from prior test's DisableNetwork)
                    DeviceStateController.EnsureNetworkEnabled(msg => Log(msg));

                    // Ensure GamingServices is running (recovers from prior test's StopGamingServices/FlushGrtsAndGoOffline)
                    DeviceStateController.EnsureGamingServicesRunning(msg => Log(msg));

                    // Dismiss any stale GRTS "Gaming UI" dialogs left over from a prior test
                    GameSaveUiNavigator.DismissStaleDialogs(msg => Log(msg));
                }

                // Always start the Xbox screenshot/gamepad UI navigator for any xbox device.
                // Xbox surfaces conflict / sync-failed / contention as SYSTEM TCUI dialogs, and
                // the in-app auto-response callback cannot always handle them (e.g., console-flash
                // recovery raises conflicts with UserContext=0, which never reaches the game). The
                // navigator is the reliable backstop. Per-scenario Set*AutoResponse commands
                // live-update it via TrackAutoResponseCommand, so each test still drives its own
                // conflict choice; this only guarantees the navigator is running to receive them.
                AutoStartXboxUiNavigator(assignments);

                Log($"Starting scenario '{plan.Manifest.Id}' ({plan.Commands.Count} command(s)).");

                int defaultTimeoutSeconds = plan.DefaultTimeoutSeconds > 0 ? plan.DefaultTimeoutSeconds : 30;
                int commandIndex = 0;
                ScenarioRunOutcome outcome = ScenarioRunOutcome.Passed;

                // Track commands whose results are handled by offline batch processing.
                // When the controller detects a DisableNetwork on an Xbox device, it sends
                // all subsequent device commands as a batch (BeginOfflineBatch) before actually
                // disabling the network. After re-enabling and reconnecting, it retrieves
                // the batch results. Commands in this set are skipped in the main loop.
                HashSet<int> offlineBatchedIndices = new HashSet<int>();

                for (int cmdIdx = 0; cmdIdx < plan.Commands.Count; cmdIdx++)
                {
                    ScenarioCommandInvocation invocation = plan.Commands[cmdIdx];
                    commandIndex = cmdIdx + 1; // 1-based for display
                    string commandName = invocation.Step.Command ?? string.Empty;
                    int timeoutSeconds = invocation.Step.TimeoutSeconds ?? defaultTimeoutSeconds;
                    if (timeoutSeconds <= 0)
                    {
                        timeoutSeconds = defaultTimeoutSeconds;
                    }

                    ControllerCommandType controllerCommandType = GetControllerCommandType(commandName);
                    bool isControllerCommand = controllerCommandType != ControllerCommandType.None;

                    // Skip commands that were batched for offline execution.
                    // Their results are validated when the batch results are retrieved.
                    if (offlineBatchedIndices.Contains(cmdIdx))
                    {
                        Log($"[{commandIndex}/{plan.Commands.Count}] Skipping '{commandName}' (handled by offline batch).", true);
                        continue;
                    }

                    object? normalizedParameters = NormalizeParameters(invocation.Step.Parameters);
                    normalizedParameters = SubstituteTemplateVariables(normalizedParameters);
                    if (string.Equals(commandName, "DoChaosMode", StringComparison.OrdinalIgnoreCase))
                    {
                        normalizedParameters = ApplyChaosScenarioOverrides(normalizedParameters);
                    }
                    if (string.Equals(commandName, "CopyTargetFolderToSaveFolder", StringComparison.OrdinalIgnoreCase))
                    {
                        normalizedParameters = ApplySourceDataFolderOverride(normalizedParameters);
                    }

                    CommandEnvelope envelope = new CommandEnvelope
                    {
                        CommandId = Guid.NewGuid().ToString("N"),
                        Command = commandName,
                        TimeoutSeconds = timeoutSeconds,
                        Parameters = normalizedParameters
                    };

                    // NOTE: Per-scenario customId prefix is NOT applied to commands like
                    // PFAuthenticationLoginWithCustomIDAsync because Xbox tests also call
                    // PFAccountManagementClientLinkXboxAccountAsync(forceLink, unlinkIfNeeded).
                    // The single physical Xbox user can only be linked to one PlayFab entity
                    // at a time, so per-scenario PlayFab entities would require unlink/relink
                    // every test, causing cloud-side ownership state to be in flux and the
                    // first scenario's Upload to fail with 0x8083000D (sync error). The prefix
                    // is therefore restricted to PFLocalUserCreateHandleWithPersistedLocalId,
                    // which is a PC-only command. See ScenarioRunner regression note in
                    // checkpoint 015 for full root-cause analysis.
                    if (!string.IsNullOrEmpty(effectivePrefix) &&
                        (string.Equals(commandName, "PFLocalUserCreateHandleWithPersistedLocalId", StringComparison.OrdinalIgnoreCase) ||
                         string.Equals(commandName, "PFAuthenticationLoginWithCustomIDAsync", StringComparison.OrdinalIgnoreCase)))
                    {
                        TryApplyCustomIdPrefix(envelope.Parameters, effectivePrefix);
                    }

                    if (!roleAssignments.TryGetValue(invocation.Role, out DeviceAssignment assignment))
                    {
                        Log($"Scenario '{plan.Manifest.Id}' failed: no device assignment found for role '{invocation.Role}'.");
                        outcome = ScenarioRunOutcome.Failed;
                        break;
                    }

                    envelope.ScenarioContext = new ScenarioContext
                    {
                        ScenarioId = scenarioIdContext,
                        ScenarioName = scenarioNameContext,
                        Role = invocation.Role,
                        StepIndex = commandIndex
                    };

                    string deviceName = assignment.DisplayName;
                    if (isControllerCommand)
                    {
                        Log($"[{commandIndex}/{plan.Commands.Count}] Executing controller-managed command '{commandName}' locally (role '{invocation.Role}').", true);
                    }
                    else
                    {
                        Log($"[{commandIndex}/{plan.Commands.Count}] Sending '{commandName}' to '{deviceName}' with timeout={timeoutSeconds}s.", true);
                    }

                    try
                    {
                        cancellationToken.ThrowIfCancellationRequested();

                        ActionResult result;
                        if (isControllerCommand)
                        {
                            // Offline batch detection: when we're about to DisableNetwork on an
                            // Xbox device, look ahead and batch all device commands until the next
                            // EnableNetwork. This lets the device execute commands autonomously
                            // while the network is down (xbstress network=broken kills the WebSocket).
                            if (controllerCommandType == ControllerCommandType.ChangeTargetDeviceState &&
                                IsDisableNetworkAction(envelope.Parameters) &&
                                IsXboxEngine(assignment))
                            {
                                var batchResult = await ExecuteOfflineBatchBlockAsync(
                                    plan,
                                    cmdIdx,
                                    invocation.Role,
                                    assignment,
                                    roleAssignments,
                                    offlineBatchedIndices,
                                    debugStatsEligibleClients,
                                    scenarioIdContext,
                                    scenarioNameContext ?? string.Empty,
                                    defaultTimeoutSeconds,
                                    cancellationToken).ConfigureAwait(false);

                                if (!batchResult.Success)
                                {
                                    Log($"Scenario '{plan.Manifest.Id}' failed: offline batch execution failed. {batchResult.ErrorMessage}");
                                    outcome = ScenarioRunOutcome.Failed;
                                    break;
                                }

                                // Validate each batch result against its step expectations
                                if (batchResult.StepResults != null)
                                {
                                    bool anyStepFailed = false;
                                    foreach (var (stepIdx, stepResult) in batchResult.StepResults)
                                    {
                                        var batchInvocation = plan.Commands[stepIdx];
                                        string batchCmdName = batchInvocation.Step.Command ?? string.Empty;
                                        bool stepFailed = ValidateCommandResult(
                                            plan, batchInvocation, stepResult, batchCmdName,
                                            deviceName, assignment.ClientId,
                                            debugStatsEligibleClients, pfGameSaveInitializedClients);
                                        if (stepFailed)
                                        {
                                            anyStepFailed = true;
                                            break;
                                        }
                                    }
                                    if (anyStepFailed)
                                    {
                                        outcome = ScenarioRunOutcome.Failed;
                                        break;
                                    }
                                }

                                // DisableNetwork itself is complete — continue to next non-batched command
                                Log($"Offline batch complete for role '{invocation.Role}' — {offlineBatchedIndices.Count} commands executed offline.");
                                continue;
                            }

                            result = ExecuteControllerCommand(controllerCommandType, envelope, assignment, roleAssignments, cancellationToken);

                            // After lifecycle actions (Relaunch, Resume, Wake), the device may
                            // reconnect with a new ClientId. Wait for the new connection and update
                            // roleAssignments so subsequent commands target the correct WebSocket session.
                            if (controllerCommandType == ControllerCommandType.ChangeTargetDeviceState &&
                                IsDeviceReconnectionAction(envelope.Parameters))
                            {
                                // Force-close the old WebSocket to prevent stale connection from being
                                // matched as "still connected" during reconnection detection.
                                _transport.ForceDisconnectDevice(assignment.ClientId);

                                DeviceAssignment updatedAssignment = await WaitForDeviceReconnectionAsync(
                                    invocation.Role,
                                    assignment,
                                    timeoutMs: 45000,
                                    cancellationToken).ConfigureAwait(false);

                                if (updatedAssignment.ClientId != assignment.ClientId)
                                {
                                    roleAssignments[invocation.Role] = updatedAssignment;
                                    debugStatsEligibleClients.Remove(assignment.ClientId);
                                    Log($"Device '{updatedAssignment.DisplayName}' reconnected for role '{invocation.Role}' (new ClientId: {updatedAssignment.ClientId:N}).");
                                }
                            }
                        }
                        else
                        {
                            // If the device has been terminated and not yet relaunched, handle
                            // delay commands locally instead of trying to send to a dead device.
                            bool deviceTerminated = !_transport.GetConnectedDeviceDetails()
                                .Any(d => d.ClientId == assignment.ClientId && d.IsConnected);

                            if (deviceTerminated && IsSmokeDelayCommand(commandName))
                            {
                                int delayMs = 5000;
                                if (envelope.Parameters is IDictionary<string, object> delayParams &&
                                    delayParams.TryGetValue("durationMs", out object? durationObj))
                                {
                                    int.TryParse(durationObj?.ToString(), out delayMs);
                                }
                                Log($"Device '{invocation.Role}' is terminated — executing SmokeDelay as controller Sleep ({delayMs}ms).");
                                Thread.Sleep(delayMs);
                                result = MakeControllerResult(envelope, assignment, true, Stopwatch.StartNew());
                            }
                            else
                            {
                                TrackAutoResponseCommand(invocation.Role, commandName, envelope.Parameters);

                                if (debugStatsEligibleClients.Contains(assignment.ClientId) &&
                                    (IsLocalUserCloseHandleCommand(commandName) || IsGameSaveUninitializeCommand(commandName)))
                                {
                                    await GatherDebugStatsForAssignmentAsync(
                                        plan,
                                        invocation.Role,
                                        assignment,
                                        debugStatsEligibleClients,
                                        scenarioIdContext,
                                        scenarioNameContext,
                                        cancellationToken).ConfigureAwait(false);
                                }

                                result = await _commandProcessor.SendCommandAsync(
                                    envelope,
                                    payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                                    _jsonOptions,
                                    TimeSpan.FromSeconds(timeoutSeconds),
                                    cancellationToken).ConfigureAwait(false);
                            }
                        }

                        bool hasHresult = TryParseHResult(result.HResult, out int parsedHresult, out string hresultDisplay);
                        bool hresultIndicatesFailure = hasHresult && parsedHresult < 0;
                        LogActionResultJson(commandName, result, isControllerCommand ? "controller" : deviceName);
                        TryCaptureTitlePlayerId(result.RawJson);
                        TryCaptureRingManifest(result.RawJson);
                        
                        // Check if this command expects failure (step-level or parameter-level)
                        bool expectFailure = invocation.Step.ExpectFailure == true;
                        int? expectedHr = null;
                        List<int>? acceptedHrs = null;
                        if (envelope.Parameters is Dictionary<string, object> paramDict)
                        {
                            if (paramDict.ContainsKey("expectFailure"))
                            {
                                if (paramDict["expectFailure"] is bool boolValue)
                                {
                                    expectFailure = boolValue;
                                }
                                else if (paramDict["expectFailure"] is string strValue)
                                {
                                    expectFailure = bool.TryParse(strValue, out bool parsed) && parsed;
                                }
                            }
                            
                            // Check if specific HRESULT(s) are expected (comma-separated)
                            if (paramDict.ContainsKey("expectedHr"))
                            {
                                if (paramDict["expectedHr"] is string hrStr)
                                {
                                    acceptedHrs = new List<int>();
                                    foreach (var part in hrStr.Split(','))
                                    {
                                        if (TryParseHResult(part.Trim(), out int parsedPart, out _))
                                        {
                                            acceptedHrs.Add(parsedPart);
                                        }
                                    }
                                    if (acceptedHrs.Count > 0)
                                    {
                                        expectedHr = acceptedHrs[0];
                                    }
                                    else
                                    {
                                        acceptedHrs = null;
                                    }
                                }
                            }
                        }
                        
                        // If expectedHr is specified, the actual HR must match one of the accepted values
                        bool expectedHrMatched = acceptedHrs != null && hasHresult && acceptedHrs.Contains(parsedHresult);
                        bool actuallyFailed;
                        if (acceptedHrs != null)
                        {
                            // When expectedHr is set, only matching one of the accepted HRs counts as success
                            actuallyFailed = !expectedHrMatched;
                        }
                        else
                        {
                            actuallyFailed = !IsSuccessStatus(result.Status) || hresultIndicatesFailure;
                        }
                        bool shouldReportFailure = expectFailure ? !actuallyFailed : actuallyFailed;
                        
                        if (shouldReportFailure)
                        {
                            string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                            string hrText = string.IsNullOrEmpty(hresultDisplay)
                                ? (string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim())
                                : hresultDisplay;
                            string errorSuffix = string.IsNullOrWhiteSpace(result.ErrorMessage) ? string.Empty : $" Error: {SanitizeForLog(result.ErrorMessage)}";
                            
                            if (expectFailure)
                            {
                                Log($"Scenario '{plan.Manifest.Id}' failed: command '{commandName}' was expected to fail but succeeded with status '{statusText}' (hr={hrText}).");
                            }
                            else
                            {
                                Log($"Scenario '{plan.Manifest.Id}' failed: command '{commandName}' returned status '{statusText}' (hr={hrText}).{errorSuffix}");
                            }
                            outcome = ScenarioRunOutcome.Failed;
                            break;
                        }

                        string successHrSource = string.IsNullOrEmpty(hresultDisplay)
                            ? (string.IsNullOrWhiteSpace(result.HResult) ? string.Empty : result.HResult!.Trim())
                            : hresultDisplay;
                        string successHrText = string.IsNullOrEmpty(successHrSource) ? "hr=0x00000000" : $"hr={successHrSource}";
                        
                        if (expectedHrMatched)
                        {
                            Log($"{deviceName}: {commandName} [Completed with expected HR] {successHrText}, {result.ElapsedMs} ms");
                        }
                        else if (expectFailure)
                        {
                            Log($"{deviceName}: {commandName} [Failed as expected] {successHrText}, {result.ElapsedMs} ms");
                        }
                        else
                        {
                            Log($"{deviceName}: {commandName} [Completed] {successHrText}, {result.ElapsedMs} ms");
                        }

                        // Store result fields as role-scoped variables for cross-role parameter resolution
                        StoreRoleResultVariables(invocation.Role, result);

                        // Special handling for XUserGetId - log the userId prominently
                        if (string.Equals(commandName, "XUserGetId", StringComparison.OrdinalIgnoreCase) &&
                            !string.IsNullOrWhiteSpace(result.RawJson))
                        {
                            try
                            {
                                using var jsonDoc = System.Text.Json.JsonDocument.Parse(result.RawJson);
                                if (jsonDoc.RootElement.TryGetProperty("userId", out var userIdElement))
                                {
                                    ulong userId = userIdElement.GetUInt64();
                                    Log($"{deviceName}: XUserGetId returned userId: 0x{userId:X16} ({userId})");
                                }
                            }
                            catch
                            {
                                // Ignore JSON parsing errors
                            }
                        }

                        // Special handling for PFGameSaveFilesGetFolder - log the folder path
                        if (string.Equals(commandName, "PFGameSaveFilesGetFolder", StringComparison.OrdinalIgnoreCase) &&
                            !string.IsNullOrWhiteSpace(result.RawJson))
                        {
                            try
                            {
                                using var jsonDoc = System.Text.Json.JsonDocument.Parse(result.RawJson);
                                if (jsonDoc.RootElement.TryGetProperty("folder", out var folderElement))
                                {
                                    string? folder = folderElement.GetString();
                                    if (!string.IsNullOrWhiteSpace(folder))
                                    {
                                        Log($"{deviceName}: PFGameSaveFilesGetFolder returned folder: {folder}");
                                    }
                                }
                            }
                            catch
                            {
                                // Ignore JSON parsing errors
                            }
                        }

                        if (!isControllerCommand && IsGameSaveAddUserCommand(commandName))
                        {
                            debugStatsEligibleClients.Add(assignment.ClientId);
                        }

                        if (!isControllerCommand && IsPFGameSaveFilesInitializeCommand(commandName))
                        {
                            pfGameSaveInitializedClients.Add(assignment.ClientId);
                        }
                    }
                    catch (TimeoutException)
                    {
                        Log($"Scenario '{plan.Manifest.Id}' failed: command '{commandName}' timed out after {timeoutSeconds} seconds.");
                        Log($"Force-disconnecting device '{deviceName}' to unblock hung process.");
                        _transport.ForceDisconnectDevice(assignment.ClientId);
                        forcedDisconnectedClients.Add(assignment.ClientId);
                        outcome = ScenarioRunOutcome.Failed;
                        break;
                    }
                    catch (OperationCanceledException ex)
                    {
                        Log($"Scenario '{plan.Manifest.Id}' cancelled.");
                        cancellationException = ex;
                        outcome = ScenarioRunOutcome.Failed;
                        break;
                    }
                    catch (Exception ex)
                    {
                        Log($"Scenario '{plan.Manifest.Id}' failed: {ex.Message}");
                        outcome = ScenarioRunOutcome.Failed;
                        break;
                    }
                }

                if (outcome == ScenarioRunOutcome.Passed)
                {
                    Log($"Scenario '{plan.Manifest.Id}' passed.");
                }

                finalOutcome = outcome;
            }
            finally
            {
                Exception? cleanupFailure = null;
                try
                {
                    if (roleAssignments != null && finalOutcome == ScenarioRunOutcome.Failed)
                    {
                        // NOTE: Snapshot capture disabled — not currently useful in practice.
                        // Uncomment to re-enable automatic failure snapshot collection.
                        // await CaptureSnapshotsForFailedScenarioAsync(
                        //     plan,
                        //     roleAssignments,
                        //     pfGameSaveInitializedClients,
                        //     scenarioIdContext,
                        //     scenarioNameContext,
                        //     cancellationToken).ConfigureAwait(false);
                    }

                    if (roleAssignments != null && plan.CleanupCommands.Count > 0)
                    {
                        int startingIndex = plan.Commands.Count;
                        await RunCleanupAsync(
                            plan,
                            roleAssignments,
                            debugStatsEligibleClients,
                            forcedDisconnectedClients,
                            finalOutcome,
                            scenarioIdContext,
                            scenarioNameContext,
                            startingIndex,
                            effectivePrefix,
                            cancellationToken).ConfigureAwait(false);
                    }

                    // Gather logs from devices only when the scenario failed.
                    // On success, log gathering is deferred to the end of the full
                    // test run to avoid the per-scenario overhead of streaming the
                    // (ever-growing) device log over WebSocket in 64 KB chunks.
                    if (roleAssignments != null && finalOutcome == ScenarioRunOutcome.Failed)
                    {
                        await GatherLogsFromAllDevicesAsync(
                            roleAssignments,
                            cancellationToken).ConfigureAwait(false);
                    }
                }
                catch (OperationCanceledException ex)
                {
                    cleanupFailure = ex;
                }
                finally
                {
                    StopTestInfrastructure();
                    ResetPcNetwork();
                    ResetXboxNetworkStress(roleAssignments);
                    ResetXboxStorageSimulation(roleAssignments);
                    _runLock.Release();
                }

                if (cleanupFailure is OperationCanceledException cancelDuringCleanup)
                {
                    throw cancelDuringCleanup;
                }

                if (cancellationException != null)
                {
                    throw cancellationException;
                }

                // Expose force-disconnected role names so the caller can restart those devices.
                if (forcedDisconnectedClients.Count > 0 && roleAssignments != null)
                {
                    LastForcedDisconnectedDevices = roleAssignments
                        .Where(pair => forcedDisconnectedClients.Contains(pair.Value.ClientId))
                        .Select(pair => (pair.Key, pair.Value.Engine))
                        .ToList();
                }
            }

            return finalOutcome;
        }

        private object? ApplySourceDataFolderOverride(object? parameters)
        {
            if (_sourceDataFolderProvider == null)
            {
                return parameters;
            }

            string? sourceDataFolder;
            try
            {
                sourceDataFolder = _sourceDataFolderProvider();
            }
            catch (Exception ex)
            {
                Log($"Failed to acquire source data folder setting: {ex.Message}");
                return parameters;
            }

            if (string.IsNullOrWhiteSpace(sourceDataFolder))
            {
                return parameters;
            }

            Dictionary<string, object?> dictionary = ExtractParameterDictionary(parameters);
            dictionary["sourceFolder"] = sourceDataFolder;
            return dictionary;
        }

        private object? ApplyChaosScenarioOverrides(object? parameters)
        {
            if (_chaosSettingsProvider == null)
            {
                return parameters;
            }

            ChaosModeScenarioParameters settings;
            try
            {
                settings = _chaosSettingsProvider();
            }
            catch (Exception ex)
            {
                Log($"Failed to acquire chaos mode settings: {ex.Message}");
                return parameters;
            }

            if (settings == null)
            {
                return parameters;
            }

            Dictionary<string, object?> dictionary = ExtractParameterDictionary(parameters);
            settings.ApplyTo(dictionary);
            return dictionary;
        }

        private static Dictionary<string, object?> ExtractParameterDictionary(object? parameters)
        {
            if (parameters is Dictionary<string, object?> typed)
            {
                return typed;
            }

            if (parameters is IDictionary<string, object?> generic)
            {
                return new Dictionary<string, object?>(generic, StringComparer.OrdinalIgnoreCase);
            }

            if (parameters is IDictionary<object, object> map)
            {
                var converted = new Dictionary<string, object?>(StringComparer.OrdinalIgnoreCase);
                foreach (KeyValuePair<object, object> entry in map)
                {
                    string key = entry.Key?.ToString() ?? string.Empty;
                    if (string.IsNullOrWhiteSpace(key))
                    {
                        continue;
                    }

                    converted[key] = entry.Value;
                }

                return converted;
            }

            if (parameters == null)
            {
                return new Dictionary<string, object?>(StringComparer.OrdinalIgnoreCase);
            }

            return new Dictionary<string, object?>(StringComparer.OrdinalIgnoreCase)
            {
                ["value"] = parameters
            };
        }

        private static bool IsSuccessStatus(string? status)
        {
            if (string.IsNullOrWhiteSpace(status))
            {
                return true;
            }

            return status.Equals("success", StringComparison.OrdinalIgnoreCase)
                || status.Equals("succeeded", StringComparison.OrdinalIgnoreCase)
                || status.Equals("ok", StringComparison.OrdinalIgnoreCase)
                || status.Equals("completed", StringComparison.OrdinalIgnoreCase);
        }

        private static bool IsGameSaveAddUserCommand(string commandName)
        {
            return commandName.Equals("PFGameSaveFilesAddUserWithUiAsync", StringComparison.OrdinalIgnoreCase);
        }

        private static bool IsPFGameSaveFilesInitializeCommand(string commandName)
        {
            return commandName.Equals("PFGameSaveFilesInitialize", StringComparison.OrdinalIgnoreCase);
        }

        private static bool IsGameSaveUninitializeCommand(string commandName)
        {
            return commandName.Equals("PFGameSaveFilesUninitializeAsync", StringComparison.OrdinalIgnoreCase);
        }

        private static bool IsLocalUserCloseHandleCommand(string commandName)
        {
            return commandName.Equals("PFLocalUserCloseHandle", StringComparison.OrdinalIgnoreCase);
        }

        /// <summary>
        /// Returns true for SmokeDelay — a device-side delay that can be handled locally
        /// when the target device has been terminated.
        /// </summary>
        private static bool IsSmokeDelayCommand(string commandName)
        {
            return commandName.Equals("SmokeDelay", StringComparison.OrdinalIgnoreCase);
        }

        private static bool TryParseHResult(string? input, out int value, out string display)
        {
            value = 0;
            display = string.Empty;

            if (string.IsNullOrWhiteSpace(input))
            {
                return false;
            }

            string trimmed = input.Trim();
            string parseTarget = trimmed;
            if (parseTarget.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
            {
                parseTarget = parseTarget.Substring(2);
            }

            if (int.TryParse(parseTarget, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out int hexValue))
            {
                value = hexValue;
                display = $"0x{hexValue:X8}";
                return true;
            }

            if (int.TryParse(trimmed, NumberStyles.Integer, CultureInfo.InvariantCulture, out int decimalValue))
            {
                value = decimalValue;
                display = $"0x{decimalValue:X8}";
                return true;
            }

            return false;
        }

        private static object? NormalizeParameters(IDictionary<string, object?>? parameters)
        {
            if (parameters == null || parameters.Count == 0)
            {
                return null;
            }

            var normalized = new Dictionary<string, object?>(StringComparer.OrdinalIgnoreCase);
            foreach (KeyValuePair<string, object?> kv in parameters)            {
                normalized[kv.Key] = NormalizeValue(kv.Value);
            }

            return normalized;
        }

        private static object? NormalizeValue(object? value)
        {
            if (value == null)
            {
                return null;
            }

            switch (value)
            {
                case IDictionary<string, object?> dict:
                    return dict.ToDictionary(kv => kv.Key, kv => NormalizeValue(kv.Value), StringComparer.OrdinalIgnoreCase);
                case IDictionary<object, object> map:
                    var converted = new Dictionary<string, object?>(StringComparer.OrdinalIgnoreCase);
                    foreach (KeyValuePair<object, object> entry in map)
                    {
                        string key = entry.Key?.ToString() ?? string.Empty;
                        converted[key] = NormalizeValue(entry.Value);
                    }
                    return converted;
                case IEnumerable<object?> enumerable:
                    return enumerable.Select(NormalizeValue).ToList();
                case IEnumerable sequence when value is not string:
                    return sequence.Cast<object?>().Select(NormalizeValue).ToList();
                default:
                    return value;
            }
        }

        private static void TryApplyCustomIdPrefix(object? parameters, string prefix)
        {
            if (string.IsNullOrEmpty(prefix) || parameters == null)
            {
                return;
            }

            if (parameters is IDictionary<string, object?> dictionary)
            {
                ApplyCustomIdPrefix(dictionary, prefix);
            }
        }

        private static void ApplyCustomIdPrefix(IDictionary<string, object?> parameters, string prefix)
        {
            if (!parameters.TryGetValue("customId", out object? value))
            {
                return;
            }

            string? current = ExtractString(value);
            if (string.IsNullOrWhiteSpace(current))
            {
                return;
            }

            if (current.StartsWith(prefix, StringComparison.Ordinal))
            {
                return;
            }

            parameters["customId"] = prefix + current;
        }

        private static string? ExtractString(object? value)
        {
            if (value == null)
            {
                return null;
            }

            return value switch
            {
                string text => text,
                JsonElement element when element.ValueKind == JsonValueKind.String => element.GetString(),
                JsonElement element when element.ValueKind == JsonValueKind.Number => element.ToString(),
                _ => value.ToString()
            };
        }

        private static string DetermineScenarioId(ScenarioPlan plan)
        {
            if (!string.IsNullOrWhiteSpace(plan.Manifest.Id))
            {
                return plan.Manifest.Id!;
            }

            if (!string.IsNullOrWhiteSpace(plan.Manifest.Name))
            {
                return plan.Manifest.Name!;
            }

            return "Scenario";
        }

        private ControllerCommandType GetControllerCommandType(string commandName)
        {
            if (string.IsNullOrWhiteSpace(commandName))
            {
                return ControllerCommandType.None;
            }

            return commandName switch
            {
                _ when commandName.Equals("CompareSaveContainerSnapshots", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.CompareSaveContainerSnapshots,
                _ when commandName.Equals("StartHttpTestServer", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.StartHttpTestServer,
                _ when commandName.Equals("StopHttpTestServer", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.StopHttpTestServer,
                _ when commandName.Equals("ConfigureHttpRoute", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.ConfigureHttpRoute,
                _ when commandName.Equals("AssertHttpMaxConcurrency", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.AssertHttpMaxConcurrency,
                _ when commandName.Equals("StartWebSocketTestServer", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.StartWebSocketTestServer,
                _ when commandName.Equals("StopWebSocketTestServer", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.StopWebSocketTestServer,
                _ when commandName.Equals("WebSocketServerClose", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.WebSocketServerClose,
                _ when commandName.Equals("Sleep", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.Sleep,
                _ when commandName.Equals("SmokeDelay", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.Sleep,
                _ when commandName.Equals("ChangeTargetDeviceState", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.ChangeTargetDeviceState,
                _ when commandName.Equals("AutoNavigateGameSaveUi", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.AutoNavigateGameSaveUi,
                _ when commandName.Equals("AssertGameSaveUiDialogCount", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.AssertGameSaveUiDialogCount,
                _ when commandName.Equals("WaitForGameSaveSync", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.WaitForGameSaveSync,
                _ when commandName.Equals("PfGameSaveUtilDownload", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.PfGameSaveUtilDownload,
                _ when commandName.Equals("VerifyGameSaveUtilDownload", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.VerifyGameSaveUtilDownload,
                _ when commandName.Equals("VerifyRingBufferIntegrity", StringComparison.OrdinalIgnoreCase) => ControllerCommandType.VerifyRingBufferIntegrity,
                _ => ControllerCommandType.None
            };
        }

        private ActionResult ExecuteControllerCommand(
            ControllerCommandType commandType,
            CommandEnvelope envelope,
            DeviceAssignment assignment,
            IReadOnlyDictionary<string, DeviceAssignment> roleAssignments,
            CancellationToken cancellationToken)
        {
            return commandType switch
            {
                ControllerCommandType.CompareSaveContainerSnapshots => ExecuteCompareSnapshots(envelope, assignment, roleAssignments, cancellationToken),
                ControllerCommandType.StartHttpTestServer => ExecuteStartHttpTestServer(envelope, assignment),
                ControllerCommandType.StopHttpTestServer => ExecuteStopHttpTestServer(envelope, assignment),
                ControllerCommandType.ConfigureHttpRoute => ExecuteConfigureHttpRoute(envelope, assignment),
                ControllerCommandType.AssertHttpMaxConcurrency => ExecuteAssertHttpMaxConcurrency(envelope, assignment),
                ControllerCommandType.StartWebSocketTestServer => ExecuteStartWebSocketTestServer(envelope, assignment),
                ControllerCommandType.StopWebSocketTestServer => ExecuteStopWebSocketTestServer(envelope, assignment),
                ControllerCommandType.WebSocketServerClose => ExecuteWebSocketServerClose(envelope, assignment),
                ControllerCommandType.Sleep => ExecuteSleep(envelope, assignment),
                ControllerCommandType.ChangeTargetDeviceState => ExecuteChangeTargetDeviceState(envelope, assignment),
                ControllerCommandType.AutoNavigateGameSaveUi => ExecuteAutoNavigateGameSaveUi(envelope, assignment),
                ControllerCommandType.AssertGameSaveUiDialogCount => ExecuteAssertGameSaveUiDialogCount(envelope, assignment),
                ControllerCommandType.WaitForGameSaveSync => ExecuteWaitForGameSaveSync(envelope, assignment),
                ControllerCommandType.PfGameSaveUtilDownload => ExecutePfGameSaveUtilDownload(envelope, assignment),
                ControllerCommandType.VerifyGameSaveUtilDownload => ExecuteVerifyGameSaveUtilDownload(envelope, assignment),
                ControllerCommandType.VerifyRingBufferIntegrity => ExecuteVerifyRingBufferIntegrity(envelope, assignment),
                ControllerCommandType.None => throw new InvalidOperationException("No controller command to execute."),
                _ => throw new InvalidOperationException($"Unsupported controller command '{envelope.Command}'.")
            };
        }

        private ActionResult ExecuteCompareSnapshots(
            CommandEnvelope envelope,
            DeviceAssignment assignment,
            IReadOnlyDictionary<string, DeviceAssignment> roleAssignments,
            CancellationToken cancellationToken)
        {
            cancellationToken.ThrowIfCancellationRequested();

            var stopwatch = Stopwatch.StartNew();
            var differences = new List<string>();

            _ = roleAssignments;

            var paramDict = envelope.Parameters as IDictionary<string, object>;

            // Parse optional ignoreTimestamps parameter (workaround for GRTS timestamp bug)
            bool ignoreTimestamps = false;
            bool expectMismatch = false;
            bool ignoreEmptyDirectories = false;
            if (envelope.Parameters is IDictionary<string, object> parameters)
            {
                if (parameters.TryGetValue("ignoreTimestamps", out object? ignoreTimestampsValue))
                {
                    ignoreTimestamps = ignoreTimestampsValue switch
                    {
                        bool b => b,
                        string s => bool.TryParse(s, out bool parsed) && parsed,
                        _ => false
                    };
                }

                // Parse optional expectMismatch parameter (test expects snapshots to differ)
                if (parameters.TryGetValue("expectMismatch", out object? expectMismatchValue))
                {
                    expectMismatch = expectMismatchValue switch
                    {
                        bool b => b,
                        string s => bool.TryParse(s, out bool parsed) && parsed,
                        _ => false
                    };
                }

                // Parse optional ignoreEmptyDirectories parameter (workaround for GRTS/inproc materialization difference)
                if (parameters.TryGetValue("ignoreEmptyDirectories", out object? ignoreEmptyDirsValue))
                {
                    ignoreEmptyDirectories = ignoreEmptyDirsValue switch
                    {
                        bool b => b,
                        string s => bool.TryParse(s, out bool parsed) && parsed,
                        _ => false
                    };
                }
            }

            // Parse optional ignoreDirectories parameter (for cross-platform comparisons where directory structure differs)
            bool ignoreDirectories = false;
            if (paramDict != null && paramDict.TryGetValue("ignoreDirectories", out object? ignoreDirectoriesValue))
            {
                ignoreDirectories = ignoreDirectoriesValue switch
                {
                    bool b => b,
                    string s => bool.TryParse(s, out bool parsed) && parsed,
                    _ => false
                };
            }

            // Parse optional ignoreHashes parameter (for rollback tests where Xbox CS restores correct size but different content)
            bool ignoreHashes = false;
            if (paramDict != null && paramDict.TryGetValue("ignoreHashes", out object? ignoreHashesValue))
            {
                ignoreHashes = ignoreHashesValue switch
                {
                    bool b => b,
                    string s => bool.TryParse(s, out bool parsed) && parsed,
                    _ => false
                };
            }

            // when either of the devices is a psx, ignore timestamps.
            const string PsxEngine = "psx"; // defined in PlayFab.C\Test\GameTestAppShared\Misc\DeviceWebSocketConnection.cpp
            if (roleAssignments.Values.Any(assignment => assignment.Engine == PsxEngine))
            {
                ignoreTimestamps = true;
            }

            // When devices use different engines (e.g., Xbox GRTS vs PC Win32), directory structure
            // differs by platform — auto-enable ignoreDirectories for cross-platform comparisons.
            var distinctEngines = roleAssignments.Values.Select(a => a.Engine).Where(e => !string.IsNullOrEmpty(e)).Distinct().ToList();
            if (distinctEngines.Count > 1)
            {
                ignoreDirectories = true;
            }

            _commandProcessor.TryGetLatestSnapshot("left", out SnapshotCaptureRecord? leftRecord);
            _commandProcessor.TryGetLatestSnapshot("right", out SnapshotCaptureRecord? rightRecord);
            SnapshotComparer.SnapshotSummary? leftSummary = null;
            SnapshotComparer.SnapshotSummary? rightSummary = null;

            if (leftRecord == null || string.IsNullOrWhiteSpace(leftRecord.RawJson))
            {
                differences.Add("Snapshot slot 'left' is empty. Run CaptureSaveContainerSnapshot with slot='left'.");
            }

            if (rightRecord == null || string.IsNullOrWhiteSpace(rightRecord.RawJson))
            {
                differences.Add("Snapshot slot 'right' is empty. Run CaptureSaveContainerSnapshot with slot='right'.");
            }

            if (leftRecord != null && rightRecord != null && !string.IsNullOrWhiteSpace(leftRecord.RawJson) && !string.IsNullOrWhiteSpace(rightRecord.RawJson))
            {
                SnapshotComparisonResult comparison = SnapshotComparer.Compare(leftRecord, rightRecord, out SnapshotComparer.SnapshotSummary left, out SnapshotComparer.SnapshotSummary right, ignoreTimestamps, ignoreDirectories, ignoreHashes);
                leftSummary = left;
                rightSummary = right;
                if (!comparison.Succeeded)
                {
                    if (!string.IsNullOrWhiteSpace(comparison.FailureReason))
                    {
                        differences.Add($"[{leftRecord.DisplayName} vs {rightRecord.DisplayName}] {comparison.FailureReason}");
                    }

                    foreach (string diff in comparison.Differences)
                    {
                        differences.Add(diff);
                    }
                }
            }

            leftSummary ??= leftRecord != null
                ? SnapshotComparer.SnapshotSummary.FromMetadataOnly(leftRecord.DisplayName)
                : SnapshotComparer.SnapshotSummary.FromMetadataOnly("left");

            rightSummary ??= rightRecord != null
                ? SnapshotComparer.SnapshotSummary.FromMetadataOnly(rightRecord.DisplayName)
                : SnapshotComparer.SnapshotSummary.FromMetadataOnly("right");

            stopwatch.Stop();

            bool success = differences.Count == 0;

            // When expectMismatch is true, invert the result: differences are expected
            if (expectMismatch)
            {
                success = !success;
                if (success)
                {
                    string leftLabel = leftRecord?.DisplayName ?? "left";
                    string rightLabel = rightRecord?.DisplayName ?? "right";
                    Log($"CompareSaveContainerSnapshots confirmed expected mismatch between slots '{leftLabel}' and '{rightLabel}'.", true);
                    foreach (string diff in differences)
                    {
                        Log("  " + diff);
                    }
                }
                else
                {
                    Log("CompareSaveContainerSnapshots expected mismatch but snapshots matched.");
                }
            }
            else if (success)
            {
                string leftLabel = leftRecord?.DisplayName ?? "left";
                string rightLabel = rightRecord?.DisplayName ?? "right";
                Log($"CompareSaveContainerSnapshots matched between slots '{leftLabel}' and '{rightLabel}'.", true);
            }
            else
            {
                Log("CompareSaveContainerSnapshots detected mismatches:");
                foreach (string diff in differences)
                {
                    Log("  " + diff);
                }
            }

            LogSnapshotSummary(leftSummary);
            LogSnapshotSummary(rightSummary);

            string[] comparedSlots =
            {
                leftRecord?.DisplayName ?? "left",
                rightRecord?.DisplayName ?? "right"
            };

            var summaries = new[] { leftSummary, rightSummary };

            var actionResult = new ActionResult
            {
                Type = "actionResult",
                CommandId = envelope.CommandId,
                Command = envelope.Command,
                DeviceId = assignment.DisplayName,
                Status = success ? "succeeded" : "failed",
                ElapsedMs = (int)Math.Min(stopwatch.ElapsedMilliseconds, int.MaxValue),
                Timestamp = DateTimeOffset.UtcNow,
                HResult = success ? null : "0x80004005",
                RawJson = BuildComparisonSummaryJson(comparedSlots, differences, success, summaries)
            };

            return actionResult;
        }

        private string BuildComparisonSummaryJson(
            IReadOnlyList<string> devices,
            IReadOnlyList<string> differences,
            bool success,
            IReadOnlyList<SnapshotComparer.SnapshotSummary?> summaries)
        {
            var payload = new
            {
                type = "snapshotComparison",
                success,
                devices,
                differences,
                summaries = summaries.Select(summary => summary == null
                    ? null
                    : new
                    {
                        displayName = summary.DisplayName,
                        hashesIncluded = summary.HashesIncluded,
                        files = summary.FileCount,
                        directories = summary.DirectoryCount,
                        bytes = summary.TotalBytes
                    })
            };

#pragma warning disable IL2026
#pragma warning disable IL3050
            string json = JsonSerializer.Serialize(payload, _jsonOptions);
#pragma warning restore IL3050
#pragma warning restore IL2026
            return json;
        }

        private void LogSnapshotSummary(SnapshotComparer.SnapshotSummary? summary)
        {
            if (summary == null)
            {
                return;
            }

            string filesText = summary.FileCount.HasValue ? summary.FileCount.Value.ToString(CultureInfo.InvariantCulture) : "?";
            string directoriesText = summary.DirectoryCount.HasValue ? summary.DirectoryCount.Value.ToString(CultureInfo.InvariantCulture) : "?";
            string bytesText = summary.TotalBytes.HasValue ? summary.TotalBytes.Value.ToString("N0", CultureInfo.InvariantCulture) : "?";
            string hashText = summary.HashesIncluded ? "hashes included" : "hashes not included";

            Log($"Snapshot {summary.DisplayName}: files={filesText}, folders={directoriesText}, bytes={bytesText}, {hashText}.", chaosLog: true);
        }

        #region Test Infrastructure Controller Commands

        private ActionResult ExecuteStartHttpTestServer(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                if (_httpTestServer != null)
                {
                    Log("HttpTestServer already running; stopping previous instance.");
                    _httpTestServer.StopAsync().GetAwaiter().GetResult();
                    _httpTestServer.Dispose();
                }

                // Where to host, and what the target device should dial. Mirrors
                // StartWebSocketTestServer: an Xbox devkit is a separate machine and needs a
                // LAN-reachable wildcard bind, which is only permitted under the control channel's
                // existing "/ws/" URL ACL reservation.
                int port;
                string path;
                string httpHost;
                if (IsXboxEngine(assignment))
                {
                    string? serverIp = XboxDeviceLauncher.GetLocalServerIp(DeviceStateController.ExtractXboxIp(assignment));
                    if (string.IsNullOrEmpty(serverIp))
                    {
                        throw new InvalidOperationException(
                            "StartHttpTestServer: could not determine this PC's IP address for an xbox device. " +
                            "The console cannot reach a localhost endpoint.");
                    }
                    port = _transport.DevicePort;
                    path = "/ws/http";
                    httpHost = serverIp;
                }
                else
                {
                    port = HttpTestServer.FindAvailablePort();
                    path = "/";
                    httpHost = "localhost";
                }

                _httpTestServer = new HttpTestServer(port, path, preferWildcard: IsXboxEngine(assignment)) { Logger = msg => Log(msg) };
                _httpTestServer.Start();

                if (IsXboxEngine(assignment) && !_httpTestServer.BoundToWildcard)
                {
                    throw new InvalidOperationException(
                        $"StartHttpTestServer: server fell back to a localhost-only bind on {port}{path}, " +
                        $"so the console cannot reach it. Reserve the URL ACL (run as admin): " +
                        $"netsh http add urlacl url=http://+:{port}{path}/ user=Everyone");
                }

                string httpTestUrl = $"http://{httpHost}:{port}{path.TrimEnd('/')}";
                _templateVariables["httpTestPort"] = port.ToString(CultureInfo.InvariantCulture);
                _templateVariables["httpTestHost"] = httpHost;
                _templateVariables["httpTestUrl"] = httpTestUrl;

                Log($"StartHttpTestServer: listening on {port}{path}, devices will dial '{httpTestUrl}'.");
                sw.Stop();
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                Log($"StartHttpTestServer failed: {ex.Message}");
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        private ActionResult ExecuteStopHttpTestServer(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                if (_httpTestServer != null)
                {
                    _httpTestServer.StopAsync().GetAwaiter().GetResult();
                    _httpTestServer.Dispose();
                    _httpTestServer = null;
                }
                _templateVariables.Remove("httpTestPort");
                _templateVariables.Remove("httpTestHost");
                _templateVariables.Remove("httpTestUrl");
                sw.Stop();
                Log("StopHttpTestServer: server stopped.");
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        private ActionResult ExecuteConfigureHttpRoute(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                if (_httpTestServer == null)
                {
                    throw new InvalidOperationException("HttpTestServer is not running. Call StartHttpTestServer first.");
                }

                string method = "GET";
                string path = "/";
                int statusCode = 200;
                string? contentType = null;
                byte[]? body = null;
                int generateBodySizeBytes = 0;
                int delayMs = 0;

                if (envelope.Parameters is IDictionary<string, object> p)
                {
                    if (p.TryGetValue("method", out object? m) && m is string ms) method = ms;
                    if (p.TryGetValue("path", out object? pa) && pa is string ps) path = ps;
                    if (p.TryGetValue("statusCode", out object? sc))
                    {
                        statusCode = sc switch
                        {
                            int i => i,
                            long l => (int)l,
                            string s => int.TryParse(s, out int v) ? v : 200,
                            _ => 200
                        };
                    }
                    if (p.TryGetValue("contentType", out object? ct) && ct is string cts) contentType = cts;
                    if (p.TryGetValue("body", out object? b) && b is string bs) body = System.Text.Encoding.UTF8.GetBytes(bs);
                    if (p.TryGetValue("generateBodySizeBytes", out object? gb))
                    {
                        generateBodySizeBytes = gb switch
                        {
                            int i => i,
                            long l => (int)l,
                            string s => int.TryParse(s, out int v) ? v : 0,
                            _ => 0
                        };
                    }
                    if (p.TryGetValue("delayMs", out object? dm))
                    {
                        delayMs = dm switch
                        {
                            int i => i,
                            long l => (int)l,
                            string s => int.TryParse(s, out int v) ? v : 0,
                            _ => 0
                        };
                    }
                }

                _httpTestServer.ConfigureRoute(method, path, statusCode, contentType, body, generateBodySizeBytes, delayMs);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                Log($"ConfigureHttpRoute failed: {ex.Message}");
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        /// <summary>
        /// Asserts the peak number of simultaneously in-flight requests the test server observed.
        /// This is the assertion that makes a request-limit scenario able to fail: without it the
        /// scenario only proves every request eventually completed, which stays true even if
        /// admission control is removed entirely.
        /// </summary>
        private ActionResult ExecuteAssertHttpMaxConcurrency(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                if (_httpTestServer == null)
                {
                    throw new InvalidOperationException("HttpTestServer is not running. Call StartHttpTestServer first.");
                }

                int? expectedMax = null;
                int? atLeast = null;
                bool reset = false;

                if (envelope.Parameters is IDictionary<string, object> p)
                {
                    static int? AsInt(object? v) => v switch
                    {
                        int i => i,
                        long l => (int)l,
                        string s => int.TryParse(s, out int parsed) ? parsed : null,
                        _ => null
                    };

                    if (p.TryGetValue("expectedMax", out object? em)) expectedMax = AsInt(em);
                    if (p.TryGetValue("atLeast", out object? al)) atLeast = AsInt(al);
                    if (p.TryGetValue("reset", out object? r))
                    {
                        reset = r switch
                        {
                            bool b => b,
                            string s => bool.TryParse(s, out bool parsed) && parsed,
                            _ => false
                        };
                    }
                }

                int observed = _httpTestServer.MaxInFlight;
                Log($"AssertHttpMaxConcurrency: observed peak in-flight = {observed} (expectedMax={expectedMax?.ToString() ?? "-"}, atLeast={atLeast?.ToString() ?? "-"}).");

                if (expectedMax.HasValue && observed > expectedMax.Value)
                {
                    throw new InvalidOperationException(
                        $"AssertHttpMaxConcurrency: peak in-flight was {observed}, which exceeds the cap of {expectedMax.Value}. " +
                        "The concurrent request limit was not enforced.");
                }

                // Guards the inverse failure: if the burst never actually overlapped, a cap of N is
                // trivially satisfied and the scenario would pass without exercising queueing.
                if (atLeast.HasValue && observed < atLeast.Value)
                {
                    throw new InvalidOperationException(
                        $"AssertHttpMaxConcurrency: peak in-flight was only {observed}, below the required {atLeast.Value}. " +
                        "The requests did not overlap, so this run did not exercise the limit.");
                }

                if (reset)
                {
                    _httpTestServer.ResetConcurrencyStats();
                }

                sw.Stop();
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                Log($"AssertHttpMaxConcurrency failed: {ex.Message}");
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        private ActionResult ExecuteStartWebSocketTestServer(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                if (_wsEchoServer != null)
                {
                    Log("WebSocketEchoServer already running; stopping previous instance.");
                    _wsEchoServer.StopAsync().GetAwaiter().GetResult();
                    _wsEchoServer.Dispose();
                }

                // Where to host the echo endpoint, and what the target device should dial.
                //
                // A device app on this machine can use a private port on localhost. An Xbox devkit
                // is a separate machine, so the endpoint must be reachable over the LAN, which
                // needs a wildcard bind and therefore an HTTP.sys URL ACL. Rather than require a
                // new reservation for an arbitrary port, host it under the control channel's
                // existing "/ws/" reservation, on the port the console already connects to.
                int port;
                string path;
                string wsHost;
                if (IsXboxEngine(assignment))
                {
                    string? serverIp = XboxDeviceLauncher.GetLocalServerIp(DeviceStateController.ExtractXboxIp(assignment));
                    if (string.IsNullOrEmpty(serverIp))
                    {
                        throw new InvalidOperationException(
                            "StartWebSocketTestServer: could not determine this PC's IP address for an xbox device. " +
                            "The console cannot reach a localhost echo endpoint.");
                    }
                    port = _transport.DevicePort;
                    path = "/ws/echo";
                    wsHost = serverIp;
                }
                else
                {
                    port = HttpTestServer.FindAvailablePort();
                    path = "/echo";
                    wsHost = "localhost";
                }

                _wsEchoServer = new WebSocketEchoServer(port, path, preferWildcard: IsXboxEngine(assignment)) { Logger = msg => Log(msg) };
                _wsEchoServer.Start();

                if (IsXboxEngine(assignment) && !_wsEchoServer.BoundToWildcard)
                {
                    throw new InvalidOperationException(
                        $"StartWebSocketTestServer: echo server fell back to a localhost-only bind on {port}{path}, " +
                        $"so the console cannot reach it. Reserve the URL ACL (run as admin): " +
                        $"netsh http add urlacl url=http://+:{port}{path}/ user=Everyone");
                }

                string wsTestUrl = $"ws://{wsHost}:{port}{path}";
                _templateVariables["wsTestPort"] = port.ToString(CultureInfo.InvariantCulture);
                _templateVariables["wsTestHost"] = wsHost;
                _templateVariables["wsTestUrl"] = wsTestUrl;

                Log($"StartWebSocketTestServer: listening on {port}{path}, devices will dial '{wsTestUrl}'.");
                sw.Stop();
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                Log($"StartWebSocketTestServer failed: {ex.Message}");
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        private ActionResult ExecuteStopWebSocketTestServer(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                if (_wsEchoServer != null)
                {
                    _wsEchoServer.StopAsync().GetAwaiter().GetResult();
                    _wsEchoServer.Dispose();
                    _wsEchoServer = null;
                }
                _templateVariables.Remove("wsTestPort");
                _templateVariables.Remove("wsTestHost");
                _templateVariables.Remove("wsTestUrl");
                sw.Stop();
                Log("StopWebSocketTestServer: server stopped.");
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        private ActionResult ExecuteWebSocketServerClose(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                if (_wsEchoServer == null)
                {
                    throw new InvalidOperationException("WebSocketEchoServer is not running.");
                }

                _wsEchoServer.CloseAllClientsAsync().GetAwaiter().GetResult();
                sw.Stop();
                Log("WebSocketServerClose: initiated close on all clients.");
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        private static readonly Random s_sleepRandom = new Random();

        private ActionResult ExecuteSleep(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            int durationMs = 1000;
            var p = envelope.Parameters as IDictionary<string, object>;

            // Random timing (minMs/maxMs) lets soak loops land the suspend/resume at a
            // different point in the upload window each iteration, hunting the race.
            int minMs = GetIntParam(p, "minMs", -1);
            int maxMs = GetIntParam(p, "maxMs", -1);
            if (minMs >= 0 && maxMs >= minMs)
            {
                lock (s_sleepRandom)
                {
                    durationMs = s_sleepRandom.Next(minMs, maxMs + 1);
                }
                Thread.Sleep(durationMs);
                sw.Stop();
                Log($"Sleep: waited {durationMs} ms (random {minMs}-{maxMs}).");
                return MakeControllerResult(envelope, assignment, true, sw);
            }

            if (p != null && p.TryGetValue("durationMs", out object? d))
            {
                durationMs = d switch
                {
                    int i => i,
                    long l => (int)l,
                    string s => int.TryParse(s, out int v) ? v : 1000,
                    _ => 1000
                };
            }

            Thread.Sleep(durationMs);
            sw.Stop();
            Log($"Sleep: waited {durationMs} ms.");
            return MakeControllerResult(envelope, assignment, true, sw);
        }

        /// <summary>
        /// Polls the local GRTS enumerator until the specified scid (and optionally xuid) shows InSync=true,
        /// or until timeoutSeconds elapses. Requires XGameSaveEnumAPI.dll next to the controller.
        /// Parameters:
        ///   scid (string, required) — the service config ID (e.g. "E18D7")
        ///   xuid (string, optional) — the Xbox user ID; if omitted, matches any user with the given scid
        ///   timeoutSeconds (int, default 60) — max wait time
        ///   pollIntervalMs (int, default 1000) — polling interval in milliseconds
        /// </summary>
        private ActionResult ExecuteWaitForGameSaveSync(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            string? xuid = null;
            string? scid = null;
            int timeoutSeconds = 600;
            int pollIntervalMs = 1000;

            if (envelope.Parameters is IDictionary<string, object> p)
            {
                if (p.TryGetValue("xuid", out object? x))
                    xuid = x?.ToString();
                if (p.TryGetValue("scid", out object? s))
                    scid = s?.ToString();
                if (p.TryGetValue("timeoutSeconds", out object? t))
                    timeoutSeconds = t switch { int i => i, long l => (int)l, string sv => int.TryParse(sv, out int v) ? v : 60, _ => 60 };
                if (p.TryGetValue("pollIntervalMs", out object? pi))
                    pollIntervalMs = pi switch { int i => i, long l => (int)l, string sv => int.TryParse(sv, out int v) ? v : 1000, _ => 1000 };
            }

            if (string.IsNullOrWhiteSpace(scid))
            {
                string error = "WaitForGameSaveSync: 'scid' parameter is required.";
                Log(error);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, error);
            }

            string target = string.IsNullOrWhiteSpace(xuid) ? $"scid={scid}" : $"xuid={xuid}, scid={scid}";
            Log($"WaitForGameSaveSync: Polling GRTS for {target} (timeout={timeoutSeconds}s, interval={pollIntervalMs}ms).");

            XGameSaveEnumInterop? enumerator = null;
            try
            {
                enumerator = new XGameSaveEnumInterop(msg => Log(msg));

                // Log initial state for diagnostics
                enumerator.LogAllProviders();

                var deadline = DateTime.UtcNow.AddSeconds(timeoutSeconds);
                int pollCount = 0;

                while (DateTime.UtcNow < deadline)
                {
                    pollCount++;
                    enumerator.Refresh();

                    XGameSaveEnumInterop.XGameSaveProviderStatus? status;
                    if (!string.IsNullOrWhiteSpace(xuid))
                    {
                        status = enumerator.FindProvider(xuid, scid);
                    }
                    else
                    {
                        status = FindProviderByScid(enumerator, scid);
                    }

                    if (status == null)
                    {
                        Log($"WaitForGameSaveSync: Poll #{pollCount} — provider not found for {target}. Will retry.");
                    }
                    else if (status.Value.InSync != 0)
                    {
                        sw.Stop();
                        Log($"WaitForGameSaveSync: InSync=true after {pollCount} poll(s), {sw.ElapsedMilliseconds} ms. " +
                            $"xuid={status.Value.Xuid}, totalBytes={status.Value.TotalBytes}, uploadedBytes={status.Value.UploadedBytes}, " +
                            $"lastSyncHr=0x{status.Value.LastSyncHr:X8}.");
                        return MakeControllerResult(envelope, assignment, true, sw);
                    }
                    else
                    {
                        if (pollCount <= 3 || pollCount % 10 == 0)
                        {
                            Log($"WaitForGameSaveSync: Poll #{pollCount} — InSync=false, IsActive={status.Value.IsActive}, " +
                                $"xuid={status.Value.Xuid}, totalBytes={status.Value.TotalBytes}, uploadedBytes={status.Value.UploadedBytes}, " +
                                $"lastSyncHr=0x{status.Value.LastSyncHr:X8}.");
                        }
                    }

                    Thread.Sleep(pollIntervalMs);
                }

                // Timeout — log final state
                sw.Stop();
                enumerator.Refresh();
                Log($"WaitForGameSaveSync: Timed out after {timeoutSeconds}s ({pollCount} polls). Final state:");
                enumerator.LogAllProviders();
                string timeoutError = $"WaitForGameSaveSync timed out after {timeoutSeconds}s waiting for {target} to reach InSync.";
                Log(timeoutError);
                return MakeControllerResult(envelope, assignment, false, sw, timeoutError);
            }
            catch (Exception ex)
            {
                sw.Stop();
                string error = $"WaitForGameSaveSync failed: {ex.GetType().Name}: {ex.Message}";
                Log(error);
                return MakeControllerResult(envelope, assignment, false, sw, error);
            }
            finally
            {
                enumerator?.Dispose();
            }
        }

        /// <summary>
        /// Finds the first provider matching the given scid (any xuid).
        /// </summary>
        private static XGameSaveEnumInterop.XGameSaveProviderStatus? FindProviderByScid(
            XGameSaveEnumInterop enumerator, string scid)
        {
            var providers = enumerator.GetAllProviders();
            foreach (var p in providers)
            {
                if (string.Equals(p.Scid, scid, StringComparison.OrdinalIgnoreCase))
                {
                    return p;
                }
            }
            return null;
        }

        /// <summary>
        /// Executes a controller-side state change on the target device's environment.
        /// Delegates to <see cref="DeviceStateController"/> for the actual work.
        /// </summary>
        private ActionResult ExecuteChangeTargetDeviceState(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            try
            {
                DeviceStateController.Execute(envelope, assignment, msg => Log(msg));
                sw.Stop();
                return MakeControllerResult(envelope, assignment, true, sw);
            }
            catch (Exception ex)
            {
                sw.Stop();
                Log($"ChangeTargetDeviceState failed: {ex.Message}");
                return MakeControllerResult(envelope, assignment, false, sw, ex.Message);
            }
        }

        /// <summary>
        /// Determines whether a ChangeTargetDeviceState action may cause device disconnection
        /// and reconnection. This includes any action that kills or restarts the app, or
        /// wakes/resumes the console from a suspended state where the WebSocket may have dropped.
        /// </summary>
        private static bool IsDeviceReconnectionAction(object? parameters)
        {
            if (parameters is IDictionary<string, object> dict &&
                dict.TryGetValue("action", out object? actionValue) &&
                actionValue is string action)
            {
                return action.ToLowerInvariant() switch
                {
                    "relaunch" => true,       // App restarted — new WebSocket session
                    "resume" => true,          // App resumed after suspend — WebSocket may have dropped
                    // Quick Resume restores the title from a disk snapshot, which tears down the
                    // network stack: the app comes back with a new WebSocket session exactly like
                    // relaunch. Without rebinding here the role keeps pointing at the pre-snapshot
                    // ClientId and every later step fails with "Device is not connected", even
                    // though the device has in fact reconnected.
                    "quickresumerestore" => true,
                    // Note: "wake" is NOT a reconnection action. After wake, the console boots
                    // and the app must be relaunched explicitly. The test handles this via
                    // WaitForBoot + Relaunch steps (which IS a reconnection action).
                    "signin" => true,          // User sign-in may trigger app reconnection
                    "switchuser" => true,      // User switch may trigger app reconnection
                    "flushgrtsandgooffline" => true, // Stops GamingServices (kills app), relaunches
                    _ => false
                };
            }

            return false;
        }

        private static bool IsDisableNetworkAction(object? parameters)
        {
            if (parameters is IDictionary<string, object> dict &&
                dict.TryGetValue("action", out object? actionValue) &&
                actionValue is string action)
            {
                return string.Equals(action, "DisableNetwork", StringComparison.OrdinalIgnoreCase);
            }
            return false;
        }

        private static bool IsEnableNetworkAction(object? parameters)
        {
            if (parameters is IDictionary<string, object> dict &&
                dict.TryGetValue("action", out object? actionValue) &&
                actionValue is string action)
            {
                return string.Equals(action, "EnableNetwork", StringComparison.OrdinalIgnoreCase);
            }
            return false;
        }

        private static bool IsXboxEngine(DeviceAssignment assignment)
        {
            return string.Equals(assignment.Engine, "xbox", StringComparison.OrdinalIgnoreCase);
        }

        /// <summary>
        /// Result of an offline batch block execution.
        /// </summary>
        private sealed class OfflineBatchBlockResult
        {
            public bool Success { get; set; }
            public string? ErrorMessage { get; set; }
            /// <summary>Map of 0-based plan.Commands index → ActionResult for each batched command.</summary>
            public List<(int StepIndex, ActionResult Result)>? StepResults { get; set; }
        }

        /// <summary>
        /// Orchestrates offline batch execution for a DisableNetwork block on Xbox:
        /// 1. Scans ahead to find all device commands until the next EnableNetwork
        /// 2. Sends BeginOfflineBatch to the device
        /// 3. Executes DisableNetwork (xbstress)
        /// 4. Waits for estimated batch duration
        /// 5. Executes EnableNetwork (xbstress stop)
        /// 6. Waits for device reconnection
        /// 7. Retrieves batch results via GetOfflineBatchResults
        /// </summary>
        private async Task<OfflineBatchBlockResult> ExecuteOfflineBatchBlockAsync(
            ScenarioPlan plan,
            int disableNetworkIdx,
            string role,
            DeviceAssignment assignment,
            Dictionary<string, DeviceAssignment> roleAssignments,
            HashSet<int> offlineBatchedIndices,
            HashSet<Guid> debugStatsEligibleClients,
            string scenarioIdContext,
            string scenarioNameContext,
            int defaultTimeoutSeconds,
            CancellationToken cancellationToken)
        {
            // Step 1: Scan ahead to find device commands to batch and the EnableNetwork endpoint.
            var batchCommands = new List<(int Index, ScenarioCommandInvocation Invocation)>();
            int enableNetworkIdx = -1;

            for (int i = disableNetworkIdx + 1; i < plan.Commands.Count; i++)
            {
                var futureInvocation = plan.Commands[i];
                if (futureInvocation.Role != role)
                    continue; // Commands for other roles are not part of this batch

                string futureCmdName = futureInvocation.Step.Command ?? string.Empty;
                ControllerCommandType futureType = GetControllerCommandType(futureCmdName);

                // If we hit EnableNetwork for the same role, that's our endpoint
                if (futureType == ControllerCommandType.ChangeTargetDeviceState &&
                    IsEnableNetworkAction(NormalizeParameters(futureInvocation.Step.Parameters)))
                {
                    enableNetworkIdx = i;
                    break;
                }

                // If we hit another controller command (Suspend, Resume, EvictGame, etc.),
                // this scenario has interleaved controller actions — can't batch it simply.
                // However, if no device commands have been collected yet, this means there are
                // no commands to batch — fall through to the empty-batch handler below.
                if (futureType != ControllerCommandType.None)
                {
                    if (batchCommands.Count == 0)
                    {
                        Log($"Offline batch: controller command '{futureCmdName}' at index {i + 1} before any device commands — skipping offline batch (no commands to batch).");
                        // No device commands to batch — just execute DisableNetwork normally
                        var normalResult = ExecuteControllerCommand(
                            ControllerCommandType.ChangeTargetDeviceState,
                            new CommandEnvelope
                            {
                                CommandId = Guid.NewGuid().ToString("N"),
                                Command = "ChangeTargetDeviceState",
                                TimeoutSeconds = 30,
                                Parameters = NormalizeParameters(plan.Commands[disableNetworkIdx].Step.Parameters)
                            },
                            assignment, roleAssignments, cancellationToken);
                        // Mark this index as batched so the main loop skips only DisableNetwork
                        offlineBatchedIndices.Add(disableNetworkIdx);
                        return new OfflineBatchBlockResult { Success = true };
                    }

                    Log($"Offline batch: interleaved controller command '{futureCmdName}' at index {i + 1} — cannot batch this scenario.");
                    return new OfflineBatchBlockResult
                    {
                        Success = false,
                        ErrorMessage = $"Interleaved controller command '{futureCmdName}' prevents offline batching."
                    };
                }

                batchCommands.Add((i, futureInvocation));
            }

            if (batchCommands.Count == 0)
            {
                Log("Offline batch: no device commands found between DisableNetwork and EnableNetwork.");
                // Just execute DisableNetwork normally (no batching needed)
                var normalResult = ExecuteControllerCommand(
                    ControllerCommandType.ChangeTargetDeviceState,
                    new CommandEnvelope
                    {
                        CommandId = Guid.NewGuid().ToString("N"),
                        Command = "ChangeTargetDeviceState",
                        TimeoutSeconds = 30,
                        Parameters = NormalizeParameters(plan.Commands[disableNetworkIdx].Step.Parameters)
                    },
                    assignment, roleAssignments, cancellationToken);

                if (!IsSuccessStatus(normalResult.Status))
                {
                    return new OfflineBatchBlockResult
                    {
                        Success = false,
                        ErrorMessage = $"DisableNetwork failed (non-batch fallback): {normalResult.ErrorMessage}"
                    };
                }
                return new OfflineBatchBlockResult { Success = true };
            }

            Log($"Offline batch: found {batchCommands.Count} device commands to batch (indices {batchCommands.First().Index + 1}–{batchCommands.Last().Index + 1}), EnableNetwork at index {(enableNetworkIdx >= 0 ? enableNetworkIdx + 1 : -1)}.");

            // Step 2: Build the batch command envelope
            var batchCommandsJson = new List<Dictionary<string, object?>>();
            int totalEstimatedSeconds = 0;

            foreach (var (idx, inv) in batchCommands)
            {
                int stepTimeout = inv.Step.TimeoutSeconds ?? defaultTimeoutSeconds;
                totalEstimatedSeconds += stepTimeout;

                object? normalizedParams = NormalizeParameters(inv.Step.Parameters);
                normalizedParams = SubstituteTemplateVariables(normalizedParams);

                batchCommandsJson.Add(new Dictionary<string, object?>
                {
                    ["commandId"] = Guid.NewGuid().ToString("N"),
                    ["command"] = inv.Step.Command ?? string.Empty,
                    ["parameters"] = normalizedParams ?? new Dictionary<string, object>(),
                    ["timeoutSeconds"] = stepTimeout
                });
            }

            var batchEnvelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "BeginOfflineBatch",
                TimeoutSeconds = 30,
                Parameters = new Dictionary<string, object>
                {
                    ["commands"] = batchCommandsJson,
                    ["startDelayMs"] = 5000
                },
                ScenarioContext = new ScenarioContext
                {
                    ScenarioId = scenarioIdContext,
                    ScenarioName = scenarioNameContext,
                    Role = role,
                    StepIndex = disableNetworkIdx + 1
                }
            };

            // Step 3: Send BeginOfflineBatch to the device (network is still up)
            Log($"Offline batch: sending BeginOfflineBatch with {batchCommands.Count} commands to '{assignment.DisplayName}'.");

            ActionResult batchAck = await _commandProcessor.SendCommandAsync(
                batchEnvelope,
                payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                _jsonOptions,
                TimeSpan.FromSeconds(30),
                cancellationToken).ConfigureAwait(false);

            if (!IsSuccessStatus(batchAck.Status))
            {
                return new OfflineBatchBlockResult
                {
                    Success = false,
                    ErrorMessage = $"BeginOfflineBatch failed: {batchAck.Status} — {batchAck.ErrorMessage}"
                };
            }

            Log($"Offline batch: device acknowledged batch ({batchCommands.Count} commands queued, 5s start delay).");

            // Step 4: Execute DisableNetwork (xbstress network=broken)
            var disableEnvelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "ChangeTargetDeviceState",
                TimeoutSeconds = 30,
                Parameters = NormalizeParameters(plan.Commands[disableNetworkIdx].Step.Parameters)
            };
            ExecuteControllerCommand(ControllerCommandType.ChangeTargetDeviceState, disableEnvelope, assignment, roleAssignments, cancellationToken);

            // Mark all batched commands so the main loop skips them
            foreach (var (idx, _) in batchCommands)
            {
                offlineBatchedIndices.Add(idx);
            }

            // Step 5: Wait for the batch to complete.
            // The device waits 5s (startDelayMs) then executes commands sequentially.
            // Most commands complete in seconds; use a reasonable fixed wait rather than
            // summing worst-case timeouts. If the batch isn't done, GetOfflineBatchResults
            // will retry with backoff.
            int waitSeconds = 5 + Math.Min(batchCommands.Count * 15, 180);
            Log($"Offline batch: waiting {waitSeconds}s for batch execution ({batchCommands.Count} commands).");
            await Task.Delay(TimeSpan.FromSeconds(waitSeconds), cancellationToken).ConfigureAwait(false);

            // Step 6: Execute EnableNetwork if we found one
            if (enableNetworkIdx >= 0)
            {
                offlineBatchedIndices.Add(enableNetworkIdx); // Skip EnableNetwork in main loop too
                var enableEnvelope = new CommandEnvelope
                {
                    CommandId = Guid.NewGuid().ToString("N"),
                    Command = "ChangeTargetDeviceState",
                    TimeoutSeconds = 30,
                    Parameters = NormalizeParameters(plan.Commands[enableNetworkIdx].Step.Parameters)
                };
                ExecuteControllerCommand(ControllerCommandType.ChangeTargetDeviceState, enableEnvelope, assignment, roleAssignments, cancellationToken);
                Log("Offline batch: network re-enabled.");

                // Give the Xbox networking stack time to fully restore after xbstress stop.
                // The device-side websocket watchdog needs ~15s to detect a stuck connect
                // attempt, force-reset it, and start a fresh connection.
                Log("Offline batch: waiting 20s for Xbox networking to stabilize...");
                await Task.Delay(TimeSpan.FromSeconds(20), cancellationToken).ConfigureAwait(false);
            }

            // Step 7: Wait for device reconnection (60s timeout gives ample time for
            // the device-side connect watchdog to cycle and establish a new connection)
            DeviceAssignment updatedAssignment = await WaitForDeviceReconnectionAsync(
                role,
                assignment,
                timeoutMs: 60000,
                cancellationToken).ConfigureAwait(false);

            if (updatedAssignment.ClientId != assignment.ClientId)
            {
                roleAssignments[role] = updatedAssignment;
                debugStatsEligibleClients.Remove(assignment.ClientId);
                Log($"Device '{updatedAssignment.DisplayName}' reconnected after offline batch (new ClientId: {updatedAssignment.ClientId:N}).");
                assignment = updatedAssignment;
            }

            // Step 8: Retrieve batch results
            var getResultsEnvelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "GetOfflineBatchResults",
                TimeoutSeconds = 30,
                Parameters = new Dictionary<string, object>(),
                ScenarioContext = new ScenarioContext
                {
                    ScenarioId = scenarioIdContext,
                    ScenarioName = scenarioNameContext,
                    Role = role,
                    StepIndex = disableNetworkIdx + 1
                }
            };

            // Retry GetOfflineBatchResults a few times in case the batch is still finishing
            ActionResult? batchResults = null;
            for (int attempt = 0; attempt < 3; attempt++)
            {
                batchResults = await _commandProcessor.SendCommandAsync(
                    getResultsEnvelope,
                    payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(30),
                    cancellationToken).ConfigureAwait(false);

                if (IsSuccessStatus(batchResults.Status))
                    break;

                Log($"Offline batch: GetOfflineBatchResults attempt {attempt + 1} returned '{batchResults.Status}', retrying in 10s...");
                await Task.Delay(10_000, cancellationToken).ConfigureAwait(false);
            }

            if (batchResults == null || !IsSuccessStatus(batchResults.Status))
            {
                return new OfflineBatchBlockResult
                {
                    Success = false,
                    ErrorMessage = $"GetOfflineBatchResults failed: {batchResults?.Status ?? "null"} — {batchResults?.ErrorMessage}"
                };
            }

            // Step 9: Parse batch results and map back to step indices
            var stepResults = new List<(int StepIndex, ActionResult Result)>();
            try
            {
                if (!string.IsNullOrWhiteSpace(batchResults.RawJson))
                {
                    using var jsonDoc = System.Text.Json.JsonDocument.Parse(batchResults.RawJson);
                    if (jsonDoc.RootElement.TryGetProperty("batchResults", out var batchResultsArray) &&
                        batchResultsArray.ValueKind == System.Text.Json.JsonValueKind.Array)
                    {
                        int resultIdx = 0;
                        foreach (var resultElement in batchResultsArray.EnumerateArray())
                        {
                            if (resultIdx >= batchCommands.Count)
                                break;

                            var (stepIndex, _) = batchCommands[resultIdx];
                            string rawResult = resultElement.GetRawText();

                            // Parse manually (same approach as ActionResultHandler.ParseActionResult)
                            // to handle hresult being either string or number.
                            var actionResult = new ActionResult { RawJson = rawResult };

                            if (resultElement.TryGetProperty("type", out var typeProp) && typeProp.ValueKind == System.Text.Json.JsonValueKind.String)
                                actionResult.Type = typeProp.GetString() ?? string.Empty;

                            if (resultElement.TryGetProperty("commandId", out var cmdIdProp) && cmdIdProp.ValueKind == System.Text.Json.JsonValueKind.String)
                                actionResult.CommandId = cmdIdProp.GetString() ?? string.Empty;

                            if (resultElement.TryGetProperty("command", out var cmdProp) && cmdProp.ValueKind == System.Text.Json.JsonValueKind.String)
                                actionResult.Command = cmdProp.GetString() ?? string.Empty;

                            if (resultElement.TryGetProperty("deviceId", out var devIdProp) && devIdProp.ValueKind == System.Text.Json.JsonValueKind.String)
                                actionResult.DeviceId = devIdProp.GetString() ?? string.Empty;

                            if (resultElement.TryGetProperty("status", out var statusProp) && statusProp.ValueKind == System.Text.Json.JsonValueKind.String)
                                actionResult.Status = statusProp.GetString() ?? string.Empty;

                            if (resultElement.TryGetProperty("elapsedMs", out var elapsedProp))
                            {
                                if (elapsedProp.TryGetInt32(out int elapsed))
                                    actionResult.ElapsedMs = elapsed;
                            }

                            if (resultElement.TryGetProperty("hresult", out var hrProp))
                            {
                                if (hrProp.ValueKind == System.Text.Json.JsonValueKind.String)
                                    actionResult.HResult = hrProp.GetString();
                                else if (hrProp.ValueKind == System.Text.Json.JsonValueKind.Number && hrProp.TryGetInt64(out long hrVal))
                                    actionResult.HResult = string.Format(System.Globalization.CultureInfo.InvariantCulture, "0x{0:X8}", unchecked((int)hrVal));
                            }

                            if (resultElement.TryGetProperty("errorMessage", out var errProp))
                            {
                                if (errProp.ValueKind == System.Text.Json.JsonValueKind.String)
                                    actionResult.ErrorMessage = errProp.GetString();
                            }

                            stepResults.Add((stepIndex, actionResult));
                            resultIdx++;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Log($"Offline batch: failed to parse batch results: {ex.Message}");
                return new OfflineBatchBlockResult
                {
                    Success = false,
                    ErrorMessage = $"Failed to parse batch results: {ex.Message}"
                };
            }

            Log($"Offline batch: retrieved {stepResults.Count} results for {batchCommands.Count} commands.");

            return new OfflineBatchBlockResult
            {
                Success = true,
                StepResults = stepResults
            };
        }

        /// <summary>
        /// Validates a single command result against step expectations (expectedHr, expectFailure).
        /// Returns true if the command failed validation (scenario should fail).
        /// </summary>
        private bool ValidateCommandResult(
            ScenarioPlan plan,
            ScenarioCommandInvocation invocation,
            ActionResult result,
            string commandName,
            string deviceName,
            Guid clientId,
            HashSet<Guid> debugStatsEligibleClients,
            HashSet<Guid> pfGameSaveInitializedClients)
        {
            bool hasHresult = TryParseHResult(result.HResult, out int parsedHresult, out string hresultDisplay);
            bool hresultIndicatesFailure = hasHresult && parsedHresult < 0;
            LogActionResultJson(commandName, result, deviceName);

            bool expectFailure = invocation.Step.ExpectFailure == true;
            int? expectedHr = null;
            List<int>? acceptedHrs = null;

            object? normalizedParams = NormalizeParameters(invocation.Step.Parameters);
            normalizedParams = SubstituteTemplateVariables(normalizedParams);
            if (normalizedParams is Dictionary<string, object> paramDict)
            {
                if (paramDict.ContainsKey("expectFailure"))
                {
                    if (paramDict["expectFailure"] is bool boolValue)
                        expectFailure = boolValue;
                    else if (paramDict["expectFailure"] is string strValue)
                        expectFailure = bool.TryParse(strValue, out bool parsed) && parsed;
                }

                if (paramDict.ContainsKey("expectedHr") && paramDict["expectedHr"] is string hrStr)
                {
                    acceptedHrs = new List<int>();
                    foreach (var part in hrStr.Split(','))
                    {
                        if (TryParseHResult(part.Trim(), out int parsedPart, out _))
                            acceptedHrs.Add(parsedPart);
                    }
                    if (acceptedHrs.Count > 0)
                        expectedHr = acceptedHrs[0];
                    else
                        acceptedHrs = null;
                }
            }

            bool expectedHrMatched = acceptedHrs != null && hasHresult && acceptedHrs.Contains(parsedHresult);
            bool actuallyFailed;
            if (acceptedHrs != null)
                actuallyFailed = !expectedHrMatched;
            else
                actuallyFailed = !IsSuccessStatus(result.Status) || hresultIndicatesFailure;

            bool shouldReportFailure = expectFailure ? !actuallyFailed : actuallyFailed;

            if (shouldReportFailure)
            {
                string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                string hrText = string.IsNullOrEmpty(hresultDisplay)
                    ? (string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim())
                    : hresultDisplay;
                string errorSuffix = string.IsNullOrWhiteSpace(result.ErrorMessage) ? string.Empty : $" Error: {SanitizeForLog(result.ErrorMessage)}";

                if (expectFailure)
                    Log($"Scenario '{plan.Manifest.Id}' failed (offline batch): command '{commandName}' was expected to fail but succeeded with status '{statusText}' (hr={hrText}).");
                else
                    Log($"Scenario '{plan.Manifest.Id}' failed (offline batch): command '{commandName}' returned status '{statusText}' (hr={hrText}).{errorSuffix}");

                return true; // validation failed
            }

            string successHrSource = string.IsNullOrEmpty(hresultDisplay)
                ? (string.IsNullOrWhiteSpace(result.HResult) ? string.Empty : result.HResult!.Trim())
                : hresultDisplay;
            string successHrText = string.IsNullOrEmpty(successHrSource) ? "hr=0x00000000" : $"hr={successHrSource}";

            if (expectedHrMatched)
                Log($"{deviceName}: {commandName} [Completed with expected HR (offline)] {successHrText}, {result.ElapsedMs} ms");
            else if (expectFailure)
                Log($"{deviceName}: {commandName} [Failed as expected (offline)] {successHrText}, {result.ElapsedMs} ms");
            else
                Log($"{deviceName}: {commandName} [Completed (offline)] {successHrText}, {result.ElapsedMs} ms");

            if (IsGameSaveAddUserCommand(commandName))
                debugStatsEligibleClients.Add(clientId);
            if (IsPFGameSaveFilesInitializeCommand(commandName))
                pfGameSaveInitializedClients.Add(clientId);

            return false; // validation passed
        }
        /// Polls connected devices until the device is available. If the device is still connected
        /// with the original ClientId (e.g., short suspend), returns the existing assignment.
        /// If a new client appears, returns the updated assignment.
        /// </summary>
        private async Task<DeviceAssignment> WaitForDeviceReconnectionAsync(
            string roleName,
            DeviceAssignment previousAssignment,
            int timeoutMs,
            CancellationToken cancellationToken)
        {
            var sw = Stopwatch.StartNew();
            Guid oldClientId = previousAssignment.ClientId;
            string displayName = previousAssignment.DisplayName;

            Log($"Waiting for '{displayName}' (role '{roleName}') to reconnect (timeout: {timeoutMs}ms)...", true);

            while (sw.ElapsedMilliseconds < timeoutMs)
            {
                cancellationToken.ThrowIfCancellationRequested();

                IReadOnlyCollection<DeviceConnectionInfo> devices = _transport.GetConnectedDeviceDetails();

                // First pass: match by display name (preferred — exact same device identity)
                foreach (DeviceConnectionInfo device in devices)
                {
                    if (string.Equals(device.DisplayName, displayName, StringComparison.OrdinalIgnoreCase) &&
                        device.IsConnected &&
                        device.CapabilitiesReady)
                    {
                        // Device found — could be same session (survived suspend) or new session
                        return new DeviceAssignment(
                            device.ClientId,
                            device.DisplayName,
                            device.Engine,
                            device.RemoteEndpoint);
                    }
                }

                // Second pass: match by IP address + engine type (handles reconnection with new client ID
                // where the display name doesn't match because the device got a UUID-based name)
                if (previousAssignment.RemoteEndpoint != null)
                {
                    string previousIp = previousAssignment.RemoteEndpoint.Contains(":")
                        ? previousAssignment.RemoteEndpoint.Substring(0, previousAssignment.RemoteEndpoint.LastIndexOf(':'))
                        : previousAssignment.RemoteEndpoint;

                    foreach (DeviceConnectionInfo device in devices)
                    {
                        if (device.IsConnected &&
                            device.CapabilitiesReady &&
                            device.ClientId != oldClientId &&
                            !string.Equals(device.DisplayName, displayName, StringComparison.OrdinalIgnoreCase))
                        {
                            string deviceIp = device.RemoteEndpoint?.Contains(":") == true
                                ? device.RemoteEndpoint.Substring(0, device.RemoteEndpoint.LastIndexOf(':'))
                                : (device.RemoteEndpoint ?? string.Empty);

                            if (string.Equals(deviceIp, previousIp, StringComparison.OrdinalIgnoreCase) &&
                                string.Equals(device.Engine, previousAssignment.Engine, StringComparison.OrdinalIgnoreCase))
                            {
                                Log($"Device reconnected with new identity '{device.DisplayName}' (matched by IP {previousIp} + engine {device.Engine}).");
                                return new DeviceAssignment(
                                    device.ClientId,
                                    device.DisplayName,
                                    device.Engine,
                                    device.RemoteEndpoint ?? string.Empty);
                            }
                        }
                    }
                }

                await Task.Delay(500, cancellationToken).ConfigureAwait(false);
            }

            throw new TimeoutException(
                $"Device '{displayName}' (role '{roleName}') did not reconnect within {timeoutMs}ms. " +
                $"Previous ClientId: {oldClientId:N}.");
        }

        /// <summary>
        /// Automated UI navigation command. For pc-grts local devices, starts a background
        /// thread that monitors for PFGameSave UI dialogs and responds automatically.
        /// For non-local or unsupported engines, logs the status.
        /// </summary>
        /// <summary>
        /// Asserts how many PFGameSave stock-UI dialogs of a given type the PC UI navigator has
        /// had to dismiss. Regression guard for ADO 63185188 ("sync failure UI every time the game
        /// saves"): while offline, the sync-failure dialog must be raised for the initial add-user
        /// sync only, NOT once per subsequent offline save.
        ///
        /// Parameters:
        ///   dialogType  SyncFailed (default) | Conflict | Progress | ActiveDeviceContention | OutOfStorage
        ///   maxCount    assertion fails if dismissals exceed this
        ///   expectedCount  assertion fails unless dismissals equal this exactly
        ///   reset       when true, zero the counter AFTER evaluating (default false)
        /// </summary>
        private ActionResult ExecuteAssertGameSaveUiDialogCount(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            var p = envelope.Parameters as IDictionary<string, object>;

            string dialogTypeName = GetStringParam(p, "dialogType") ?? "SyncFailed";
            if (!Enum.TryParse<GameSaveUiNavigator.GameSaveDialogType>(dialogTypeName, true, out var dialogType))
            {
                string err = $"AssertGameSaveUiDialogCount: unknown dialogType '{dialogTypeName}'.";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            if (_uiNavigator == null)
            {
                string err = "AssertGameSaveUiDialogCount: no PC UI navigator is running (call AutoNavigateGameSaveUi first).";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            int actual = _uiNavigator.GetDismissedCount(dialogType);

            int? maxCount = null;
            if (p != null && p.TryGetValue("maxCount", out object? maxRaw) && maxRaw != null &&
                int.TryParse(maxRaw.ToString(), out int maxParsed))
            {
                maxCount = maxParsed;
            }

            int? expectedCount = null;
            if (p != null && p.TryGetValue("expectedCount", out object? expRaw) && expRaw != null &&
                int.TryParse(expRaw.ToString(), out int expParsed))
            {
                expectedCount = expParsed;
            }

            bool reset = GetBoolParam(p, "reset", false);

            string? failure = null;
            if (expectedCount.HasValue && actual != expectedCount.Value)
            {
                failure = $"AssertGameSaveUiDialogCount: expected exactly {expectedCount.Value} '{dialogType}' dialog(s) but the navigator dismissed {actual}.";
            }
            else if (maxCount.HasValue && actual > maxCount.Value)
            {
                failure = $"AssertGameSaveUiDialogCount: expected at most {maxCount.Value} '{dialogType}' dialog(s) but the navigator dismissed {actual}. " +
                          "This is ADO 63185188 - the offline sync-failure UI is being raised on every save instead of only for the initial sync.";
            }

            if (reset)
            {
                _uiNavigator.ResetDismissedCounts();
            }

            sw.Stop();
            if (failure != null)
            {
                Log(failure);
                return MakeControllerResult(envelope, assignment, false, sw, failure);
            }

            Log($"AssertGameSaveUiDialogCount: '{dialogType}' dismissals={actual}" +
                (maxCount.HasValue ? $" (max {maxCount.Value})" : string.Empty) +
                (expectedCount.HasValue ? $" (expected {expectedCount.Value})" : string.Empty) +
                (reset ? " [counter reset]" : string.Empty) + " - OK.");
            return MakeControllerResult(envelope, assignment, true, sw);
        }

        private ActionResult ExecuteAutoNavigateGameSaveUi(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            string engine = assignment.Engine?.ToLowerInvariant() ?? "unknown";

            switch (engine)
            {
                case "pc-inproc":
                case "pc-inproc-gamesaves":
                    Log($"AutoNavigateGameSaveUi: Automated UI navigation is not supported for engine '{engine}'.");
                    break;

                case "xbox":
                    {
                        string endpoint = assignment.RemoteEndpoint ?? "";
                        int colonIndex = endpoint.LastIndexOf(':');
                        string ip = colonIndex > 0 ? endpoint.Substring(0, colonIndex).Trim('[', ']') : endpoint.Trim('[', ']');

                        if (string.IsNullOrWhiteSpace(ip))
                        {
                            Log($"AutoNavigateGameSaveUi: Cannot determine Xbox IP from endpoint '{endpoint}'.");
                            break;
                        }

                        LogAutoResponseSettings(envelope.ScenarioContext?.Role);
                        StartXboxUiNavigator(ip, envelope.ScenarioContext?.Role);
                        break;
                    }

                case "pc-grts":
                    if (!GameSaveUiNavigator.IsDeviceLocal(assignment.RemoteEndpoint))
                    {
                        Log($"AutoNavigateGameSaveUi: UI automation is not supported on non-local devices (endpoint='{assignment.RemoteEndpoint}').");
                        break;
                    }

                    LogAutoResponseSettings(envelope.ScenarioContext?.Role);
                    StartUiNavigator(envelope.ScenarioContext?.Role);
                    break;

                default:
                    Log($"AutoNavigateGameSaveUi: Unknown engine '{engine}'. No action taken.");
                    break;
            }

            sw.Stop();
            return MakeControllerResult(envelope, assignment, true, sw);
        }

        /// <summary>
        /// Creates and starts the <see cref="GameSaveUiNavigator"/> monitor thread.
        /// If one is already running, stops it first so response settings are refreshed.
        /// </summary>
        private void StartUiNavigator(string? role)
        {
            if (_uiNavigator != null)
            {
                Log("AutoNavigateGameSaveUi: Stopping previous UI navigator.");
                try
                {
                    _uiNavigator.Dispose();
                }
                finally
                {
                    _uiNavigator = null;
                }
            }

            _uiNavigator = new GameSaveUiNavigator(msg => Log(msg));
            _uiNavigatorRole = role;
            _uiNavigator.ApplyTrackedResponses(role, _trackedAutoResponses, _trackedMaxRetries);

            // Default SyncFailed to Cancel ("Play offline") so network-down dialogs
            // never hang the test even if no explicit auto-response was configured.
            if (role == null || !_trackedAutoResponses.ContainsKey($"{role}:SyncFailed"))
            {
                _uiNavigator.SetResponse(GameSaveUiNavigator.GameSaveDialogType.SyncFailed, "Cancel");
            }

            // Default Conflict to UseLocal so unexpected conflict dialogs don't hang tests.
            if (role == null || !_trackedAutoResponses.ContainsKey($"{role}:Conflict"))
            {
                _uiNavigator.SetResponse(GameSaveUiNavigator.GameSaveDialogType.Conflict, "UseLocal");
            }

            // Default StopOrKeepSyncing to Stop so cancel-confirmation dialogs don't hang tests.
            _uiNavigator.SetResponse(GameSaveUiNavigator.GameSaveDialogType.StopOrKeepSyncing, "Stop");

            _uiNavigator.Start();
        }

        /// <summary>
        /// Creates and starts the <see cref="Xbox.XboxUiNavigator"/> for Xbox TCUI automation.
        /// Uses XTF screenshot capture + OpenCV template matching + virtual gamepad input.
        /// </summary>
        private void StartXboxUiNavigator(string xboxIp, string? role)
        {
            if (_xboxUiNavigator != null)
            {
                Log("AutoNavigateGameSaveUi: Stopping previous Xbox UI navigator.");
                try
                {
                    _xboxUiNavigator.Dispose();
                }
                finally
                {
                    _xboxUiNavigator = null;
                }
            }

            string templatesFolder = FindXboxTemplatesFolder();
            _xboxUiNavigator = new Xbox.XboxUiNavigator(xboxIp, templatesFolder, msg => Log(msg), _logDirectoryProvider);
            _uiNavigatorRole = role;
            _xboxUiNavigator.ApplyTrackedResponses(role, _trackedAutoResponses);

            // Seed safe fallback defaults (matching the PC navigator) so that an unexpected
            // system dialog never hangs the test for 240s even if the scenario declared no
            // explicit auto-response. Any Set*AutoResponse the scenario declares is tracked and
            // live-updates the navigator (see TrackAutoResponseCommand), overriding these.
            if (role == null || !_trackedAutoResponses.ContainsKey($"{role}:SyncFailed"))
            {
                _xboxUiNavigator.SetResponse(GameSaveUiNavigator.GameSaveDialogType.SyncFailed, "Cancel");
            }
            if (role == null || !_trackedAutoResponses.ContainsKey($"{role}:Conflict"))
            {
                _xboxUiNavigator.SetResponse(GameSaveUiNavigator.GameSaveDialogType.Conflict, "UseLocal");
            }

            _xboxUiNavigator.Start();
        }

        /// <summary>
        /// Auto-starts the <see cref="Xbox.XboxUiNavigator"/> for the (first) Xbox device in the
        /// scenario, if any. Called at the start of every scenario so the screenshot + gamepad
        /// navigator is always running for gamesave-xbox tests — no per-scenario
        /// <c>AutoNavigateGameSaveUi</c> command required. The navigator idles harmlessly when no
        /// dialog appears, and per-scenario auto-responses live-update its response actions.
        /// </summary>
        private void AutoStartXboxUiNavigator(Dictionary<string, DeviceAssignment> assignments)
        {
            string? xboxRole = null;
            DeviceAssignment xboxAssignment = default;
            foreach (KeyValuePair<string, DeviceAssignment> kvp in assignments)
            {
                if (string.Equals(kvp.Value.Engine, "xbox", StringComparison.OrdinalIgnoreCase))
                {
                    xboxRole = kvp.Key;
                    xboxAssignment = kvp.Value;
                    break;
                }
            }

            if (xboxRole == null)
            {
                return; // No Xbox device in this scenario — nothing to do.
            }

            string endpoint = xboxAssignment.RemoteEndpoint ?? "";
            int colonIndex = endpoint.LastIndexOf(':');
            string ip = colonIndex > 0
                ? endpoint.Substring(0, colonIndex).Trim('[', ']')
                : endpoint.Trim('[', ']');

            if (string.IsNullOrWhiteSpace(ip))
            {
                Log($"AutoNavigateGameSaveUi: Cannot auto-start Xbox UI navigator for role '{xboxRole}' — no IP in endpoint '{endpoint}'.");
                return;
            }

            Log($"AutoNavigateGameSaveUi: Auto-starting Xbox UI navigator for role '{xboxRole}' at {ip} (always-on for gamesave-xbox).");
            StartXboxUiNavigator(ip, xboxRole);
        }

        /// <summary>
        /// Locates the XboxDialogTemplates folder containing reference dialog screenshots.
        /// </summary>
        private static string FindXboxTemplatesFolder()
        {
            // Relative to executable
            string? exeDir = Path.GetDirectoryName(typeof(ScenarioRunner).Assembly.Location);
            if (!string.IsNullOrEmpty(exeDir))
            {
                string exePath = Path.Combine(exeDir, "XboxDialogTemplates");
                if (Directory.Exists(exePath))
                    return exePath;
            }

            // Relative to cwd
            string cwdPath = Path.Combine(Environment.CurrentDirectory, "XboxDialogTemplates");
            if (Directory.Exists(cwdPath))
                return cwdPath;

            // Source tree location (when running via dotnet run)
            string sourcePath = Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..",
                "Test", "GameTestController", "XboxDialogTemplates");
            if (Directory.Exists(sourcePath))
                return Path.GetFullPath(sourcePath);

            // Fallback
            return exeDir != null
                ? Path.Combine(exeDir, "XboxDialogTemplates")
                : cwdPath;
        }

        /// <summary>
        /// Records an auto-response action when a PFGameSaveFilesSetUi*AutoResponse (or *Response)
        /// command is dispatched to a device. The value is stored per-role so that
        /// <see cref="ExecuteAutoNavigateGameSaveUi"/> can report the effective setting.
        /// </summary>
        private void TrackAutoResponseCommand(string role, string commandName, object? parameters)
        {
            if (!AutoResponseCommandKeys.TryGetValue(commandName, out string? shortKey))
            {
                return;
            }

            string action = "(unknown)";
            int maxRetries = -1; // unlimited by default
            string fallbackAction = "Cancel";

            if (parameters is IDictionary<string, object> p)
            {
                if (p.TryGetValue("action", out object? actionValue) &&
                    actionValue is string actionStr &&
                    !string.IsNullOrWhiteSpace(actionStr))
                {
                    action = actionStr;
                }

                if (p.TryGetValue("maxRetries", out object? maxRetriesValue))
                {
                    if (maxRetriesValue is int intVal)
                        maxRetries = intVal;
                    else if (maxRetriesValue is long longVal)
                        maxRetries = (int)longVal;
                    else if (maxRetriesValue is string strVal && int.TryParse(strVal, out int parsed))
                        maxRetries = parsed;
                }

                if (p.TryGetValue("fallbackAction", out object? fallbackValue) &&
                    fallbackValue is string fallbackStr &&
                    !string.IsNullOrWhiteSpace(fallbackStr))
                {
                    fallbackAction = fallbackStr;
                }
            }

            _trackedAutoResponses[$"{role}:{shortKey}"] = action;

            // Also track maxRetries for the Navigator
            if (maxRetries >= 0)
            {
                _trackedMaxRetries[$"{role}:{shortKey}"] = maxRetries;
            }

            // Push live update to running navigator if it matches the navigator's role
            if (_uiNavigator != null && string.Equals(role, _uiNavigatorRole, StringComparison.OrdinalIgnoreCase)
                && Enum.TryParse<GameSaveUiNavigator.GameSaveDialogType>(shortKey, ignoreCase: true, out var dialogType))
            {
                _uiNavigator.SetResponse(dialogType, action, maxRetries, fallbackAction);
                Log($"AutoNavigateGameSaveUi: Live-updated {shortKey} response to '{action}'" +
                    (maxRetries >= 0 ? $" (maxRetries={maxRetries}, fallback={fallbackAction})" : ""));
            }
            if (_xboxUiNavigator != null && string.Equals(role, _uiNavigatorRole, StringComparison.OrdinalIgnoreCase)
                && Enum.TryParse<GameSaveUiNavigator.GameSaveDialogType>(shortKey, ignoreCase: true, out var xboxDialogType))
            {
                _xboxUiNavigator.SetResponse(xboxDialogType, action);
                Log($"AutoNavigateGameSaveUi: Live-updated Xbox {shortKey} response to '{action}'");
            }
        }

        /// <summary>
        /// Logs the auto-response actions that have been tracked for the given role so far.
        /// Uses built-in defaults for any type not yet set by a prior command.
        /// </summary>
        private void LogAutoResponseSettings(string? role)
        {
            var autoResponseTypes = new (string CommandName, string ShortKey, string DefaultAction)[]
            {
                ("PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse", "ActiveDeviceContention", "Retry"),
                ("PFGameSaveFilesSetUiConflictAutoResponse", "Conflict", "UseLocal"),
                ("PFGameSaveFilesSetUiOutOfStorageAutoResponse", "OutOfStorage", "Retry"),
                ("PFGameSaveFilesSetUiSyncFailedAutoResponse", "SyncFailed", "Cancel"),
                ("PFGameSaveFilesSetUiProgressAutoResponse", "Progress", "Cancel"),
            };

            Log("  Auto-response actions:");

            foreach (var (commandName, shortKey, defaultAction) in autoResponseTypes)
            {
                string lookupKey = $"{role}:{shortKey}";
                if (role != null && _trackedAutoResponses.TryGetValue(lookupKey, out string? trackedAction))
                {
                    Log($"    {commandName}: action={trackedAction} (set)");
                }
                else
                {
                    Log($"    {commandName}: action={defaultAction} (default)");
                }
            }
        }

        private static ActionResult MakeControllerResult(CommandEnvelope envelope, DeviceAssignment assignment, bool success, Stopwatch sw, string? errorMessage = null)
        {
            return new ActionResult
            {
                Type = "actionResult",
                CommandId = envelope.CommandId,
                Command = envelope.Command,
                DeviceId = assignment.DisplayName,
                Status = success ? "succeeded" : "failed",
                ElapsedMs = (int)Math.Min(sw.ElapsedMilliseconds, int.MaxValue),
                Timestamp = DateTimeOffset.UtcNow,
                HResult = success ? "0x00000000" : "0x80004005",
                ErrorMessage = errorMessage
            };
        }

        /// <summary>
        /// Scans an action-result JSON payload for a title_player_account entity id and
        /// remembers the most recent one. Cheap no-op when nothing matches.
        /// </summary>
        private void TryCaptureTitlePlayerId(string? rawJson)
        {
            if (string.IsNullOrEmpty(rawJson))
            {
                return;
            }

            Match m = EntityIdFieldRegex.Match(rawJson);
            if (!m.Success)
            {
                m = TitlePlayerIdRegex.Match(rawJson);
            }

            if (m.Success)
            {
                string id = m.Groups["id"].Value.ToUpperInvariant();
                if (!string.Equals(id, _capturedTitlePlayerId, StringComparison.OrdinalIgnoreCase))
                {
                    _capturedTitlePlayerId = id;
                    Log($"Captured title_player_account id for cloud verification: {id}", true);
                }
            }
        }

        /// <summary>
        /// Controller command: pulls the player's cloud save down via pfgamesaveutil so a
        /// later VerifyGameSaveUtilDownload can confirm what actually reached the cloud.
        ///
        /// Parameters (all optional):
        ///   path            destination folder (default: &lt;logDir&gt;/pfgsutil-download or Out/pfgsutil-download).
        ///   titlePlayerId   override the captured entity id.
        ///   titleId         override PFTITLEID / default title.
        ///   downloadAll     when true, uses the 'downloadall' verb (per-version layout).
        ///   timeoutSeconds  process timeout (default 300).
        ///
        /// When PFSECRETKEY is unset the step logs and is skipped (returns success), matching
        /// the opt-in behavior of Test/pfgamesaveutilTests.
        /// </summary>
        private ActionResult ExecutePfGameSaveUtilDownload(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            var p = envelope.Parameters as IDictionary<string, object>;

            string? secretKey = PfGameSaveUtil.GetSecretKey();
            if (secretKey == null)
            {
                Log("PfGameSaveUtilDownload: PFSECRETKEY is not set — skipping cloud download/verify step.");
                sw.Stop();
                return MakeControllerResult(envelope, assignment, true, sw);
            }

            string? exePath = PfGameSaveUtil.ResolveExePath();
            if (exePath == null)
            {
                string err = "PfGameSaveUtilDownload: pfgamesaveutil.exe not found. Publish it " +
                             "(Tools/pfgamesaveutil/publish.cmd) or set PFGSUTIL_EXE.";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            string? titlePlayerId = GetStringParam(p, "titlePlayerId") ?? _capturedTitlePlayerId;
            if (string.IsNullOrWhiteSpace(titlePlayerId))
            {
                string err = "PfGameSaveUtilDownload: no title_player_account id available. Add a " +
                             "PFEntityGetEntityKey step after login, or pass 'titlePlayerId'.";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            string titleId = GetStringParam(p, "titleId") ?? PfGameSaveUtil.GetTitleId();
            bool downloadAll = GetBoolParam(p, "downloadAll", false);
            string verb = downloadAll ? "downloadall" : "download";
            int timeoutSeconds = GetIntParam(p, "timeoutSeconds", 300);

            string destPath = GetStringParam(p, "path") ?? DefaultUtilDownloadPath(assignment);

            try
            {
                if (Directory.Exists(destPath))
                {
                    Directory.Delete(destPath, recursive: true);
                }
                Directory.CreateDirectory(destPath);
            }
            catch (Exception ex)
            {
                string err = $"PfGameSaveUtilDownload: could not prepare destination '{destPath}': {ex.Message}";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            Log($"PfGameSaveUtilDownload: {verb} title={titleId} player={titlePlayerId} -> {destPath}", true);

            PfGameSaveUtil.RunResult run = PfGameSaveUtil.RunDownload(
                verb, titleId, secretKey, titlePlayerId, destPath, exePath,
                TimeSpan.FromSeconds(timeoutSeconds));

            sw.Stop();

            // Log tool output (never contains the secret — it's only passed as an arg).
            foreach (string line in run.Combined.Split('\n'))
            {
                string trimmed = line.TrimEnd('\r');
                if (!string.IsNullOrWhiteSpace(trimmed))
                {
                    Log($"  [pfgamesaveutil] {trimmed}", true);
                }
            }

            if (!run.Success)
            {
                string err = $"PfGameSaveUtilDownload: {verb} failed (exit {run.ExitCode}).";
                Log(err);
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            _lastUtilDownloadPath = destPath;
            Log($"PfGameSaveUtilDownload: {verb} completed into '{destPath}'.", true);
            return MakeControllerResult(envelope, assignment, true, sw);
        }

        /// <summary>
        /// Controller command: verifies a pfgamesaveutil download matches a device-captured
        /// snapshot slot, proving the cloud copy equals what the device wrote/uploaded.
        ///
        /// Parameters (all optional):
        ///   path              downloaded folder (default: last PfGameSaveUtilDownload path).
        ///   slot              snapshot slot to compare against (default: "left").
        ///   ignoreTimestamps  default true (cloud round-trip does not preserve mtimes).
        ///   ignoreDirectories default true (download layout may differ from local dirs).
        ///   expectMismatch    when true, a difference is the success condition.
        ///
        /// When PFSECRETKEY is unset the verify is skipped (returns success), so a scenario
        /// can run cloudless on machines without credentials.
        /// </summary>
        private ActionResult ExecuteVerifyGameSaveUtilDownload(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            var p = envelope.Parameters as IDictionary<string, object>;

            if (PfGameSaveUtil.GetSecretKey() == null)
            {
                Log("VerifyGameSaveUtilDownload: PFSECRETKEY is not set — skipping cloud verification.");
                sw.Stop();
                return MakeControllerResult(envelope, assignment, true, sw);
            }

            string? downloadPath = GetStringParam(p, "path") ?? _lastUtilDownloadPath;
            if (string.IsNullOrWhiteSpace(downloadPath) || !Directory.Exists(downloadPath))
            {
                string err = $"VerifyGameSaveUtilDownload: download folder not found ('{downloadPath}'). " +
                             "Run PfGameSaveUtilDownload first or pass 'path'.";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            string slot = GetStringParam(p, "slot") ?? "left";
            bool ignoreTimestamps = GetBoolParam(p, "ignoreTimestamps", true);
            bool ignoreDirectories = GetBoolParam(p, "ignoreDirectories", true);
            bool expectMismatch = GetBoolParam(p, "expectMismatch", false);

            if (!_commandProcessor.TryGetLatestSnapshot(slot, out SnapshotCaptureRecord? expected)
                || expected == null || string.IsNullOrWhiteSpace(expected.RawJson))
            {
                string err = $"VerifyGameSaveUtilDownload: snapshot slot '{slot}' is empty. " +
                             "Capture the expected state with CaptureSaveContainerSnapshot before uploading.";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            SnapshotCaptureRecord downloaded = FolderSnapshotBuilder.Build(downloadPath, "cloud-download");

            SnapshotComparisonResult comparison = SnapshotComparer.Compare(
                expected, downloaded, out _, out _,
                ignoreTimestamps: ignoreTimestamps,
                ignoreDirectories: ignoreDirectories,
                ignoreHashes: false);

            var differences = new List<string>();
            if (!comparison.Succeeded)
            {
                if (!string.IsNullOrWhiteSpace(comparison.FailureReason))
                {
                    differences.Add(comparison.FailureReason!);
                }
                differences.AddRange(comparison.Differences);
            }

            bool matched = comparison.Succeeded;
            bool success = expectMismatch ? !matched : matched;

            sw.Stop();

            if (success)
            {
                Log(expectMismatch
                    ? $"VerifyGameSaveUtilDownload: confirmed expected cloud mismatch vs slot '{slot}'."
                    : $"VerifyGameSaveUtilDownload: cloud download matches slot '{slot}' — upload verified.", true);
            }
            else
            {
                Log($"VerifyGameSaveUtilDownload: cloud copy does NOT match slot '{slot}':");
                foreach (string diff in differences)
                {
                    Log("  " + diff);
                }
            }

            return MakeControllerResult(envelope, assignment, success, sw,
                success ? null : $"Cloud download did not match snapshot slot '{slot}'.");
        }

        /// <summary>Default per-role download folder, preferring the active log directory.</summary>
        private string DefaultUtilDownloadPath(DeviceAssignment assignment)
        {
            string roleLeaf = string.IsNullOrWhiteSpace(assignment.DisplayName) ? "device" : assignment.DisplayName;
            string? logDir = _logDirectoryProvider?.Invoke();
            string baseDir = !string.IsNullOrWhiteSpace(logDir)
                ? logDir!
                : Path.Combine(AppContext.BaseDirectory, "pfgsutil-download");
            return Path.Combine(baseDir, "pfgsutil-download", roleLeaf);
        }

        private static string? GetStringParam(IDictionary<string, object>? p, string key)
        {
            if (p != null && p.TryGetValue(key, out object? v) && v != null)
            {
                string s = v.ToString() ?? string.Empty;
                return string.IsNullOrWhiteSpace(s) ? null : s;
            }
            return null;
        }

        private static bool GetBoolParam(IDictionary<string, object>? p, string key, bool fallback)
        {
            if (p != null && p.TryGetValue(key, out object? v) && v != null)
            {
                return v switch
                {
                    bool b => b,
                    string s => bool.TryParse(s, out bool parsed) ? parsed : fallback,
                    _ => fallback
                };
            }
            return fallback;
        }

        private static int GetIntParam(IDictionary<string, object>? p, string key, int fallback)
        {
            if (p != null && p.TryGetValue(key, out object? v) && v != null)
            {
                return v switch
                {
                    int i => i,
                    long l => (int)l,
                    string s => int.TryParse(s, out int parsed) ? parsed : fallback,
                    _ => fallback
                };
            }
            return fallback;
        }

        /// <summary>Authoritative ring-buffer ground truth captured from StopRingBufferWriter.</summary>
        private sealed record RingManifest(
            int FileCount,
            long BytesPerFile,
            string RelativeDir,
            long WritesCompleted,
            IReadOnlyList<RingSlot> Slots);

        private sealed record RingSlot(string Path, long Size, int LastByte);

        /// <summary>
        /// Parses a StopRingBufferWriter action result and remembers its ring manifest as
        /// the data-loss ground truth for VerifyRingBufferIntegrity. No-op otherwise.
        /// </summary>
        private void TryCaptureRingManifest(string? rawJson)
        {
            if (string.IsNullOrEmpty(rawJson) || rawJson.IndexOf("ringManifest", StringComparison.OrdinalIgnoreCase) < 0)
            {
                return;
            }

            try
            {
                using JsonDocument doc = JsonDocument.Parse(rawJson);
                if (!doc.RootElement.TryGetProperty("ringManifest", out JsonElement m) || m.ValueKind != JsonValueKind.Object)
                {
                    return;
                }

                int fileCount = m.TryGetProperty("fileCount", out JsonElement fc) && fc.TryGetInt32(out int fcv) ? fcv : 0;
                long bytesPerFile = m.TryGetProperty("bytesPerFile", out JsonElement bp) && bp.TryGetInt64(out long bpv) ? bpv : 0;
                string relativeDir = m.TryGetProperty("relativeDir", out JsonElement rd) && rd.ValueKind == JsonValueKind.String ? rd.GetString() ?? "ring" : "ring";
                long writes = m.TryGetProperty("writesCompleted", out JsonElement wc) && wc.TryGetInt64(out long wcv) ? wcv : 0;

                var slots = new List<RingSlot>();
                if (m.TryGetProperty("slots", out JsonElement slotsEl) && slotsEl.ValueKind == JsonValueKind.Array)
                {
                    foreach (JsonElement s in slotsEl.EnumerateArray())
                    {
                        string path = s.TryGetProperty("path", out JsonElement pe) && pe.ValueKind == JsonValueKind.String ? pe.GetString() ?? "" : "";
                        long size = s.TryGetProperty("size", out JsonElement se) && se.TryGetInt64(out long sv) ? sv : 0;
                        int lastByte = s.TryGetProperty("lastByte", out JsonElement lb) && lb.TryGetInt32(out int lbv) ? lbv : -1;
                        if (!string.IsNullOrEmpty(path))
                        {
                            slots.Add(new RingSlot(path.Replace('\\', '/'), size, lastByte));
                        }
                    }
                }

                _ringManifest = new RingManifest(fileCount, bytesPerFile, relativeDir, writes, slots);
                Log($"Captured ring manifest: {slots.Count} slot(s), {writes} writes, {bytesPerFile} bytes/file.", true);
            }
            catch
            {
                // Not a ring manifest payload — ignore.
            }
        }

        /// <summary>
        /// Controller command: verifies the ring-buffer save data survived an event with NO
        /// data loss, against the authoritative manifest from StopRingBufferWriter.
        ///
        /// For every ring slot the manifest recorded, the target must contain that file at
        /// exactly bytesPerFile, filled uniformly with the slot's last byte. A missing file,
        /// a short/truncated file, or any non-uniform/wrong content is reported as DATA LOSS —
        /// the signature of the resume-with-missing-data bug.
        ///
        /// Parameters:
        ///   source  "local" (a CaptureSaveContainerSnapshot slot) or "cloud" (a pfgamesaveutil
        ///           download folder). Default: "cloud" when a download exists, else "local".
        ///   slot    snapshot slot name when source=local (default "left").
        ///   path    download folder when source=cloud (default last PfGameSaveUtilDownload).
        /// </summary>
        private ActionResult ExecuteVerifyRingBufferIntegrity(CommandEnvelope envelope, DeviceAssignment assignment)
        {
            var sw = Stopwatch.StartNew();
            var p = envelope.Parameters as IDictionary<string, object>;

            if (_ringManifest == null || _ringManifest.Slots.Count == 0)
            {
                string err = "VerifyRingBufferIntegrity: no ring manifest captured. Run StopRingBufferWriter first.";
                Log(err);
                sw.Stop();
                return MakeControllerResult(envelope, assignment, false, sw, err);
            }

            string source = (GetStringParam(p, "source") ?? (_lastUtilDownloadPath != null ? "cloud" : "local")).ToLowerInvariant();

            // Resolve the actual (path -> (size, sha256)) map of whatever we're verifying.
            Dictionary<string, (long Size, string? Sha256)>? actual;
            string sourceLabel;
            if (source == "cloud")
            {
                if (PfGameSaveUtil.GetSecretKey() == null)
                {
                    Log("VerifyRingBufferIntegrity: PFSECRETKEY unset — skipping cloud integrity check.");
                    sw.Stop();
                    return MakeControllerResult(envelope, assignment, true, sw);
                }

                string? downloadPath = GetStringParam(p, "path") ?? _lastUtilDownloadPath;
                if (string.IsNullOrWhiteSpace(downloadPath) || !Directory.Exists(downloadPath))
                {
                    string err = $"VerifyRingBufferIntegrity: download folder not found ('{downloadPath}').";
                    Log(err);
                    sw.Stop();
                    return MakeControllerResult(envelope, assignment, false, sw, err);
                }
                sourceLabel = $"cloud download '{downloadPath}'";
                actual = BuildPathMapFromFolder(downloadPath);
            }
            else
            {
                string slot = GetStringParam(p, "slot") ?? "left";
                if (!_commandProcessor.TryGetLatestSnapshot(slot, out SnapshotCaptureRecord? rec) || rec == null || string.IsNullOrWhiteSpace(rec.RawJson))
                {
                    string err = $"VerifyRingBufferIntegrity: snapshot slot '{slot}' is empty.";
                    Log(err);
                    sw.Stop();
                    return MakeControllerResult(envelope, assignment, false, sw, err);
                }
                sourceLabel = $"local snapshot slot '{slot}'";
                actual = BuildPathMapFromSnapshot(rec.RawJson);
            }

            var dataLoss = new List<string>();
            foreach (RingSlot slot in _ringManifest.Slots)
            {
                long expectedSize = _ringManifest.BytesPerFile;
                string expectedSha = ExpectedUniformSha256(slot.LastByte, expectedSize);

                if (!actual.TryGetValue(slot.Path, out (long Size, string? Sha256) got))
                {
                    dataLoss.Add($"DATA LOSS: '{slot.Path}' is MISSING (expected {expectedSize} bytes of 0x{slot.LastByte:X2}).");
                    continue;
                }
                if (got.Size != expectedSize)
                {
                    dataLoss.Add($"DATA LOSS: '{slot.Path}' size {got.Size} != expected {expectedSize} (truncated/partial).");
                    continue;
                }
                if (got.Sha256 != null && !string.Equals(got.Sha256, expectedSha, StringComparison.OrdinalIgnoreCase))
                {
                    dataLoss.Add($"DATA LOSS: '{slot.Path}' content mismatch (not uniform 0x{slot.LastByte:X2}).");
                }
            }

            sw.Stop();
            bool success = dataLoss.Count == 0;
            if (success)
            {
                Log($"VerifyRingBufferIntegrity: {sourceLabel} intact — all {_ringManifest.Slots.Count} ring slot(s) present, full-size, uniform. No data loss.", true);
            }
            else
            {
                Log($"VerifyRingBufferIntegrity: DATA LOSS detected in {sourceLabel}:");
                foreach (string d in dataLoss)
                {
                    Log("  " + d);
                }
            }

            return MakeControllerResult(envelope, assignment, success, sw,
                success ? null : $"Ring-buffer data loss detected in {sourceLabel} ({dataLoss.Count} slot(s)).");
        }

        /// <summary>Computes the sha256 of <paramref name="size"/> bytes all equal to <paramref name="byteValue"/>.</summary>
        private static string ExpectedUniformSha256(int byteValue, long size)
        {
            using var sha = System.Security.Cryptography.SHA256.Create();
            byte b = (byte)byteValue;
            const int chunk = 1 << 20; // 1 MB
            byte[] buffer = new byte[(int)Math.Min(chunk, Math.Max(size, 1))];
            Array.Fill(buffer, b);
            long remaining = size;
            while (remaining > 0)
            {
                int take = (int)Math.Min(buffer.Length, remaining);
                sha.TransformBlock(buffer, 0, take, null, 0);
                remaining -= take;
            }
            sha.TransformFinalBlock(Array.Empty<byte>(), 0, 0);
            var sb = new System.Text.StringBuilder(sha.Hash!.Length * 2);
            foreach (byte hb in sha.Hash!)
            {
                sb.Append(hb.ToString("x2", System.Globalization.CultureInfo.InvariantCulture));
            }
            return sb.ToString();
        }

        /// <summary>Reads files under a folder into a (relative-posix-path -> size, sha256) map, skipping cloudsync.</summary>
        private static Dictionary<string, (long Size, string? Sha256)> BuildPathMapFromFolder(string root)
        {
            var map = new Dictionary<string, (long, string?)>(StringComparer.OrdinalIgnoreCase);
            var rootInfo = new DirectoryInfo(root);
            if (!rootInfo.Exists)
            {
                return map;
            }
            foreach (FileInfo file in rootInfo.EnumerateFiles("*", SearchOption.AllDirectories))
            {
                string rel = Path.GetRelativePath(rootInfo.FullName, file.FullName).Replace('\\', '/').Trim('/');
                int slash = rel.IndexOf('/');
                string first = slash < 0 ? rel : rel.Substring(0, slash);
                if (string.Equals(first, "cloudsync", StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }
                string sha;
                using (FileStream fs = file.OpenRead())
                using (var s = System.Security.Cryptography.SHA256.Create())
                {
                    byte[] h = s.ComputeHash(fs);
                    var sb = new System.Text.StringBuilder(h.Length * 2);
                    foreach (byte hb in h) sb.Append(hb.ToString("x2", System.Globalization.CultureInfo.InvariantCulture));
                    sha = sb.ToString();
                }
                map[rel] = (file.Length, sha);
            }
            return map;
        }

        /// <summary>Parses a device snapshot JSON into a (path -> size, sha256) map of file entries.</summary>
        private static Dictionary<string, (long Size, string? Sha256)> BuildPathMapFromSnapshot(string rawJson)
        {
            var map = new Dictionary<string, (long, string?)>(StringComparer.OrdinalIgnoreCase);
            try
            {
                using JsonDocument doc = JsonDocument.Parse(rawJson);
                if (!doc.RootElement.TryGetProperty("snapshot", out JsonElement snap) ||
                    !snap.TryGetProperty("entries", out JsonElement entries) || entries.ValueKind != JsonValueKind.Array)
                {
                    return map;
                }
                foreach (JsonElement e in entries.EnumerateArray())
                {
                    string type = e.TryGetProperty("type", out JsonElement te) && te.ValueKind == JsonValueKind.String ? te.GetString() ?? "" : "";
                    if (!string.Equals(type, "file", StringComparison.OrdinalIgnoreCase))
                    {
                        continue;
                    }
                    string path = e.TryGetProperty("path", out JsonElement pe) && pe.ValueKind == JsonValueKind.String ? pe.GetString() ?? "" : "";
                    if (string.IsNullOrEmpty(path))
                    {
                        continue;
                    }
                    long size = e.TryGetProperty("size", out JsonElement se) && se.TryGetInt64(out long sv) ? sv : -1;
                    string? sha = e.TryGetProperty("sha256", out JsonElement she) && she.ValueKind == JsonValueKind.String ? she.GetString() : null;
                    map[path.Replace('\\', '/')] = (size, sha);
                }
            }
            catch
            {
                // Malformed snapshot — return whatever parsed.
            }
            return map;
        }

        /// <summary>
        /// Replaces {{variableName}} placeholders in all string values of the parameter dictionary.
        /// </summary>
        private object? SubstituteTemplateVariables(object? parameters)
        {
            if (parameters == null)
            {
                return parameters;
            }

            // Skip only if no template sources exist AND no ${...} references could be present
            // (we can't cheaply check for ${} without iterating, so only skip when both dicts are empty)
            if (_templateVariables.Count == 0 && _roleResultVariables.Count == 0)
            {
                return parameters;
            }

            if (parameters is Dictionary<string, object?> dict)
            {
                var result = new Dictionary<string, object?>(dict.Count, StringComparer.OrdinalIgnoreCase);
                foreach (var kv in dict)
                {
                    result[kv.Key] = SubstituteValue(kv.Value);
                }
                return result;
            }

            return parameters;
        }

        private object? SubstituteValue(object? value)
        {
            if (value is string s)
            {
                // Handle {{templateVar}} substitution
                foreach (var kv in _templateVariables)
                {
                    s = s.Replace($"{{{{{kv.Key}}}}}", kv.Value, StringComparison.OrdinalIgnoreCase);
                }

                // Handle ${Role.field} cross-role variable references
                if (s.Contains("${"))
                {
                    s = ResolveRoleVariableReferences(s);
                }

                return s;
            }
            return value;
        }

        private static readonly System.Text.RegularExpressions.Regex s_roleVarPattern =
            new(@"\$\{(\w+)\.(\w+)\}", System.Text.RegularExpressions.RegexOptions.Compiled);

        /// <summary>
        /// Resolves ${Role.field} references using stored command result data from prior steps.
        /// </summary>
        private string ResolveRoleVariableReferences(string input)
        {
            return s_roleVarPattern.Replace(input, match =>
            {
                string key = $"{match.Groups[1].Value}.{match.Groups[2].Value}";
                if (_roleResultVariables.TryGetValue(key, out string? resolved))
                {
                    return resolved;
                }
                // Leave unresolved references as-is (will fail at runtime with clear error)
                Log($"WARNING: Unresolved variable reference '${{{key}}}' — no prior command result for this role/field.");
                return match.Value;
            });
        }

        /// <summary>
        /// Extracts fields from a command result's "result" object and stores them
        /// as role-scoped variables for cross-role parameter resolution.
        /// </summary>
        private void StoreRoleResultVariables(string role, ActionResult result)
        {
            if (string.IsNullOrWhiteSpace(result.RawJson) || string.IsNullOrWhiteSpace(role))
            {
                return;
            }

            try
            {
                using var jsonDoc = System.Text.Json.JsonDocument.Parse(result.RawJson);

                // Prefer fields under "result" object; fall back to root-level scalars
                // so commands that return their fields (e.g. {someId: "..."}) at the root
                // are also captured for cross-role variable resolution.
                System.Text.Json.JsonElement source = jsonDoc.RootElement;
                if (source.TryGetProperty("result", out var resultObj) &&
                    resultObj.ValueKind == System.Text.Json.JsonValueKind.Object)
                {
                    source = resultObj;
                }

                if (source.ValueKind == System.Text.Json.JsonValueKind.Object)
                {
                    foreach (var prop in source.EnumerateObject())
                    {
                        string? value = prop.Value.ValueKind switch
                        {
                            System.Text.Json.JsonValueKind.String => prop.Value.GetString(),
                            System.Text.Json.JsonValueKind.Number => prop.Value.GetRawText(),
                            System.Text.Json.JsonValueKind.True => "true",
                            System.Text.Json.JsonValueKind.False => "false",
                            _ => null
                        };

                        if (value != null)
                        {
                            _roleResultVariables[$"{role}.{prop.Name}"] = value;
                        }
                    }
                }
            }
            catch
            {
                // Non-fatal — variable resolution is best-effort
            }
        }

        /// <summary>
        /// Stops any test infrastructure servers that are still running (called during scenario cleanup).
        /// </summary>
        private void StopTestInfrastructure()
        {
            if (_uiNavigator != null)
            {
                try { _uiNavigator.Dispose(); } catch { }
                _uiNavigator = null;
                _uiNavigatorRole = null;
            }
            if (_xboxUiNavigator != null)
            {
                try { _xboxUiNavigator.Dispose(); } catch { }
                _xboxUiNavigator = null;
            }
            if (_httpTestServer != null)
            {
                try { _httpTestServer.StopAsync().GetAwaiter().GetResult(); } catch { }
                _httpTestServer.Dispose();
                _httpTestServer = null;
            }
            if (_wsEchoServer != null)
            {
                try { _wsEchoServer.StopAsync().GetAwaiter().GetResult(); } catch { }
                _wsEchoServer.Dispose();
                _wsEchoServer = null;
            }
            _templateVariables.Clear();
            _roleResultVariables.Clear();
        }

        /// <summary>
        /// After each scenario, stop any xbstress simulations on Xbox devices so a
        /// failed scenario that disabled the network doesn't leave the console in a
        /// broken state for subsequent scenarios.
        /// </summary>
        private void ResetPcNetwork()
        {
            // Safety guard: always re-enable PC network adapters in case a test
            // disabled them (via Disable-NetAdapter) and failed before cleanup.
            // This runs as a local PowerShell command — no WebSocket needed.
            try
            {
                using var process = new Process();
                process.StartInfo.FileName = "powershell.exe";
                process.StartInfo.Arguments = "-NoProfile -NonInteractive -ExecutionPolicy Bypass -Command \"" +
                    "Get-NetAdapter -Physical | Where-Object { $_.Status -eq 'Disabled' } | Enable-NetAdapter -Confirm:$false\"";
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.CreateNoWindow = true;

                process.Start();
                process.WaitForExit(15_000);

                if (process.ExitCode == 0)
                {
                    Log("Safety: re-enabled any disabled PC network adapters.");
                }
            }
            catch
            {
                // Best-effort — may fail if not running as admin, which is fine
            }
        }

        /// <summary>
        /// Best-effort reset of any Xbox network stress simulations that may still be running.
        /// Runs in the finally block to ensure stress is stopped even if the scenario fails.
        /// </summary>
        private void ResetXboxNetworkStress(Dictionary<string, DeviceAssignment>? roleAssignments)
        {
            if (roleAssignments == null)
                return;

            foreach (DeviceAssignment assignment in roleAssignments.Values)
            {
                string engine = assignment.Engine?.ToLowerInvariant() ?? "";
                if (engine != "xbox")
                    continue;

                try
                {
                    string endpoint = assignment.RemoteEndpoint ?? "";
                    int colonIndex = endpoint.LastIndexOf(':');
                    string ip = colonIndex > 0 ? endpoint.Substring(0, colonIndex).Trim('[', ']') : endpoint.Trim('[', ']');
                    if (string.IsNullOrWhiteSpace(ip))
                        continue;

                    DeviceStateController.RunXbToolQuiet("xbstress.exe", $"stop /x:{ip}");
                }
                catch
                {
                    // Best-effort — don't let cleanup failures break the test run
                }
            }
        }

        private void ResetXboxStorageSimulation(Dictionary<string, DeviceAssignment>? roleAssignments)
        {
            if (roleAssignments == null)
                return;

            foreach (DeviceAssignment assignment in roleAssignments.Values)
            {
                string engine = assignment.Engine?.ToLowerInvariant() ?? "";
                if (engine != "xbox")
                    continue;

                try
                {
                    string endpoint = assignment.RemoteEndpoint ?? "";
                    int colonIndex = endpoint.LastIndexOf(':');
                    string ip = colonIndex > 0 ? endpoint.Substring(0, colonIndex).Trim('[', ']') : endpoint.Trim('[', ']');
                    if (string.IsNullOrWhiteSpace(ip))
                        continue;

                    DeviceStateController.RunXbToolQuiet("xbstorage.exe", $"/x:{ip} simulate /stop");
                }
                catch
                {
                    // Best-effort — don't let cleanup failures break the test run
                }
            }
        }

        #endregion

        private async Task CaptureSnapshotsForFailedScenarioAsync(
            ScenarioPlan plan,
            IReadOnlyDictionary<string, DeviceAssignment> assignments,
            IReadOnlySet<Guid> pfGameSaveInitializedClients,
            string scenarioId,
            string? scenarioName,
            CancellationToken cancellationToken)
        {
            if (_logDirectoryProvider == null)
            {
                Log("Automatic snapshot capture skipped: log directory provider not configured.");
                return;
            }

            string? outputDirectory = _logDirectoryProvider();
            if (string.IsNullOrWhiteSpace(outputDirectory))
            {
                Log("Automatic snapshot capture skipped: log directory unavailable.");
                return;
            }

            try
            {
                Directory.CreateDirectory(outputDirectory);
            }
            catch (Exception ex)
            {
                Log($"Automatic snapshot capture skipped: failed to ensure output directory '{outputDirectory}': {ex.Message}");
                return;
            }

            Log($"Scenario '{plan.Manifest.Id}' failed; capturing snapshots before cleanup.");

            var visitedClients = new HashSet<Guid>();
            int snapshotIndex = 0;

            foreach (KeyValuePair<string, DeviceAssignment> entry in assignments)
            {
                cancellationToken.ThrowIfCancellationRequested();

                DeviceAssignment assignment = entry.Value;
                if (!visitedClients.Add(assignment.ClientId))
                {
                    continue;
                }

                // Skip GatherSnapshot for devices where PFGameSaves wasn't initialized
                if (!pfGameSaveInitializedClients.Contains(assignment.ClientId))
                {
                    string deviceLabel = string.IsNullOrWhiteSpace(assignment.DisplayName)
                        ? assignment.ClientId.ToString("N")
                        : assignment.DisplayName;
                    Log($"[failure snapshot] Skipping GatherSnapshot for role '{entry.Key}' ({deviceLabel}): PFGameSaves not initialized.", true);
                    continue;
                }

                snapshotIndex++;

                await GatherSnapshotForFailureAsync(
                    plan,
                    entry.Key,
                    assignment,
                    outputDirectory,
                    scenarioId,
                    scenarioName,
                    snapshotIndex,
                    cancellationToken).ConfigureAwait(false);
            }
        }

        private async Task GatherLogsFromAllDevicesAsync(
            IReadOnlyDictionary<string, DeviceAssignment> assignments,
            CancellationToken cancellationToken)
        {
            if (_logDirectoryProvider == null)
            {
                Log("Automatic log gathering skipped: log directory provider not configured.", true);
                return;
            }

            string? outputDirectory = _logDirectoryProvider();
            if (string.IsNullOrWhiteSpace(outputDirectory))
            {
                Log("Automatic log gathering skipped: log directory unavailable.", true);
                return;
            }

            try
            {
                Directory.CreateDirectory(outputDirectory);
            }
            catch (Exception ex)
            {
                Log($"Automatic log gathering skipped: failed to ensure output directory '{outputDirectory}': {ex.Message}");
                return;
            }

            var visitedClients = new HashSet<Guid>();

            foreach (KeyValuePair<string, DeviceAssignment> entry in assignments)
            {
                cancellationToken.ThrowIfCancellationRequested();

                DeviceAssignment assignment = entry.Value;
                if (!visitedClients.Add(assignment.ClientId))
                {
                    continue;
                }

                await GatherLogsForDeviceAsync(assignment, outputDirectory, cancellationToken).ConfigureAwait(false);
            }
        }

        private async Task GatherLogsForDeviceAsync(
            DeviceAssignment assignment,
            string outputDirectory,
            CancellationToken cancellationToken)
        {
            const int timeoutSeconds = 15;

            string deviceLabel = string.IsNullOrWhiteSpace(assignment.DisplayName)
                ? assignment.ClientId.ToString("N")
                : assignment.DisplayName;

            Log($"Requesting logs from '{deviceLabel}'.", true);

            string sanitizedDeviceName = ControllerRuntime.SanitizeFileComponent(deviceLabel, "device");
            bool primarySucceeded = false;

            var envelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "GatherLogs",
                TimeoutSeconds = timeoutSeconds
            };

            try
            {
                ActionResult result = await _commandProcessor.SendCommandAsync(
                    envelope,
                    payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(timeoutSeconds),
                    cancellationToken).ConfigureAwait(false);

                if (!IsSuccessStatus(result.Status))
                {
                    string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                    string hrText = string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim();
                    string errorSuffix = string.IsNullOrWhiteSpace(result.ErrorMessage)
                        ? string.Empty
                        : $" Error: {SanitizeForLog(result.ErrorMessage)}";
                    Log($"GatherLogs for '{deviceLabel}' failed with status '{statusText}' (hr={hrText}).{errorSuffix}");
                }
                else if (!ControllerRuntime.TryExtractGatherLogsMetadata(result.RawJson, out ControllerRuntime.GatherLogsMetadata metadata))
                {
                    Log($"GatherLogs for '{deviceLabel}' returned an unexpected payload; no log was saved.");
                }
                else
                {
                    // Fetch and save main log in chunks
                    if (metadata.BytesToTransfer > 0 && !string.IsNullOrWhiteSpace(metadata.LogPath))
                    {
                        // Derive fetched filename from source path to preserve timestamped names
                        string sourceFileName = Path.GetFileName(metadata.LogPath);
                        string outputFileName = sourceFileName.EndsWith("-log.txt", StringComparison.OrdinalIgnoreCase)
                            ? sourceFileName.Replace("-log.txt", "-fetched-log.txt")
                            : $"device-{sanitizedDeviceName}-fetched-log.txt";
                        string outputPath = Path.Combine(outputDirectory, outputFileName);

                        await FetchLogChunksAsync(assignment.ClientId, metadata.LogPath, metadata.FileStartOffset, metadata.BytesToTransfer, outputPath, cancellationToken).ConfigureAwait(false);

                        string truncatedSuffix = metadata.Truncated ? " (truncated)" : string.Empty;
                        string message = $"GatherLogs saved {metadata.BytesToTransfer} of {metadata.FileSize} bytes from '{deviceLabel}' to '{outputPath}'{truncatedSuffix}.";
                        if (!string.IsNullOrWhiteSpace(metadata.LogPath))
                        {
                            message += $" Source: {metadata.LogPath}.";
                        }

                        Log(message);
                        primarySucceeded = true;
                    }

                    // Fetch and save summary log in chunks
                    if (metadata.SummaryBytesToTransfer > 0 && !string.IsNullOrWhiteSpace(metadata.SummaryLogPath))
                    {
                        // Derive fetched filename from source path to preserve timestamped names
                        string summarySourceFileName = Path.GetFileName(metadata.SummaryLogPath);
                        string summaryOutputFileName = summarySourceFileName.EndsWith("-summary.txt", StringComparison.OrdinalIgnoreCase)
                            ? summarySourceFileName.Replace("-summary.txt", "-fetched-summary.txt")
                            : $"device-{sanitizedDeviceName}-fetched-summary.txt";
                        string summaryOutputPath = Path.Combine(outputDirectory, summaryOutputFileName);

                        await FetchLogChunksAsync(assignment.ClientId, metadata.SummaryLogPath, metadata.SummaryFileStartOffset, metadata.SummaryBytesToTransfer, summaryOutputPath, cancellationToken).ConfigureAwait(false);

                        string summaryTruncatedSuffix = metadata.SummaryTruncated ? " (truncated)" : string.Empty;
                        string summaryMessage = $"GatherLogs saved summary {metadata.SummaryBytesToTransfer} of {metadata.SummaryFileSize} bytes from '{deviceLabel}' to '{summaryOutputPath}'{summaryTruncatedSuffix}.";
                        if (!string.IsNullOrWhiteSpace(metadata.SummaryLogPath))
                        {
                            summaryMessage += $" Source: {metadata.SummaryLogPath}.";
                        }

                        Log(summaryMessage);
                    }
                }
            }
            catch (TimeoutException)
            {
                Log($"GatherLogs for '{deviceLabel}' timed out.");
            }
            catch (OperationCanceledException)
            {
                Log("GatherLogs operation canceled.");
                throw;
            }
            catch (Exception ex)
            {
                Log($"GatherLogs for '{deviceLabel}' failed: {ex.Message}");
            }

            // Fallback: if primary method failed, try direct copy based on platform
            if (!primarySucceeded)
            {
                await TryFallbackLogGatherAsync(assignment, sanitizedDeviceName, outputDirectory, cancellationToken).ConfigureAwait(false);
            }
        }

        /// <summary>
        /// Fallback log gathering when the WebSocket-based GatherLogs command fails.
        /// For PC devices: copies logs from local Out\logs directory.
        /// For Xbox devices: uses xbcp to copy logs from D:\ on the console.
        /// </summary>
        private async Task TryFallbackLogGatherAsync(
            DeviceAssignment assignment,
            string sanitizedDeviceName,
            string outputDirectory,
            CancellationToken cancellationToken)
        {
            string deviceLabel = string.IsNullOrWhiteSpace(assignment.DisplayName)
                ? assignment.ClientId.ToString("N")
                : assignment.DisplayName;

            string engine = assignment.Engine?.ToLowerInvariant() ?? "unknown";
            Log($"Attempting fallback log gather for '{deviceLabel}' (engine: {engine}).");

            try
            {
                if (engine == "xbox")
                {
                    await TryFallbackLogGatherXboxAsync(sanitizedDeviceName, outputDirectory, cancellationToken).ConfigureAwait(false);
                }
                else if (engine.StartsWith("pc-") || engine == "unknown")
                {
                    TryFallbackLogGatherPc(sanitizedDeviceName, outputDirectory);
                }
                else
                {
                    Log($"Fallback log gather: unsupported engine '{engine}'.");
                }
            }
            catch (OperationCanceledException)
            {
                throw;
            }
            catch (Exception ex)
            {
                Log($"Fallback log gather for '{deviceLabel}' failed: {ex.Message}");
            }
        }

        /// <summary>
        /// Copies logs from local Out\logs directory for PC devices.
        /// </summary>
        private void TryFallbackLogGatherPc(string sanitizedDeviceName, string outputDirectory)
        {
            // Find repo root by looking for Out directory
            string? repoRoot = FindRepoRoot();
            if (repoRoot == null)
            {
                Log("Fallback log gather (PC): could not find repo root.");
                return;
            }

            string localLogsDir = Path.Combine(repoRoot, "Out", "logs");
            if (!Directory.Exists(localLogsDir))
            {
                Log($"Fallback log gather (PC): local logs directory not found: {localLogsDir}");
                return;
            }

            // Glob for all log files matching this device name (handles timestamped filenames).
            // Pattern matches both legacy "device-{name}-log.txt" and new "device-{name}-HHMMSS-mmm-PID-log.txt"
            bool copiedAny = false;

            foreach (string localPath in Directory.GetFiles(localLogsDir, $"device-{sanitizedDeviceName}-*-log.txt"))
            {
                string fileName = Path.GetFileName(localPath);
                string destName = fileName.Replace("-log.txt", "-fetched-log.txt");
                string destPath = Path.Combine(outputDirectory, destName);
                File.Copy(localPath, destPath, overwrite: true);
                var fi = new FileInfo(destPath);
                Log($"Fallback log gather (PC): copied {fi.Length} bytes from '{localPath}' to '{destPath}'.");
                copiedAny = true;
            }

            foreach (string localPath in Directory.GetFiles(localLogsDir, $"device-{sanitizedDeviceName}-*-summary.txt"))
            {
                string fileName = Path.GetFileName(localPath);
                string destName = fileName.Replace("-summary.txt", "-fetched-summary.txt");
                string destPath = Path.Combine(outputDirectory, destName);
                File.Copy(localPath, destPath, overwrite: true);
                var fi = new FileInfo(destPath);
                Log($"Fallback log gather (PC): copied {fi.Length} bytes from '{localPath}' to '{destPath}'.");
                copiedAny = true;
            }

            // Also check for legacy non-timestamped files
            string legacyLogPath = Path.Combine(localLogsDir, $"device-{sanitizedDeviceName}-log.txt");
            if (File.Exists(legacyLogPath))
            {
                string destLogPath = Path.Combine(outputDirectory, $"device-{sanitizedDeviceName}-fetched-log.txt");
                File.Copy(legacyLogPath, destLogPath, overwrite: true);
                var fi = new FileInfo(destLogPath);
                Log($"Fallback log gather (PC): copied {fi.Length} bytes from '{legacyLogPath}' to '{destLogPath}'.");
                copiedAny = true;
            }

            string legacySummaryPath = Path.Combine(localLogsDir, $"device-{sanitizedDeviceName}-summary.txt");
            if (File.Exists(legacySummaryPath))
            {
                string destSummaryPath = Path.Combine(outputDirectory, $"device-{sanitizedDeviceName}-fetched-summary.txt");
                File.Copy(legacySummaryPath, destSummaryPath, overwrite: true);
                var fi = new FileInfo(destSummaryPath);
                Log($"Fallback log gather (PC): copied {fi.Length} bytes from '{legacySummaryPath}' to '{destSummaryPath}'.");
                copiedAny = true;
            }

            if (!copiedAny)
            {
                Log($"Fallback log gather (PC): no matching log files found in '{localLogsDir}'.");
            }
        }

        /// <summary>
        /// Uses xbcp to copy logs from Xbox D:\ drive.
        /// Note: This fallback uses legacy non-timestamped filenames. When Xbox
        /// devices are relaunched, the old process's timestamped log file survives
        /// on D:\ but this fallback won't find it. The primary WebSocket-based
        /// GatherLogs path handles timestamped names correctly.
        /// </summary>
        private async Task TryFallbackLogGatherXboxAsync(string sanitizedDeviceName, string outputDirectory, CancellationToken cancellationToken)
        {
            // Xbox logs are stored on D:\ (scratch drive)
            string xboxLogPath = $"xd:\\device-{sanitizedDeviceName}-log.txt";
            string xboxSummaryPath = $"xd:\\device-{sanitizedDeviceName}-summary.txt";
            string destLogPath = Path.Combine(outputDirectory, $"device-{sanitizedDeviceName}-fetched-log.txt");
            string destSummaryPath = Path.Combine(outputDirectory, $"device-{sanitizedDeviceName}-fetched-summary.txt");

            bool copiedAny = false;

            // Try to copy main log
            if (await TryXbcpCopyAsync(xboxLogPath, destLogPath, cancellationToken).ConfigureAwait(false))
            {
                copiedAny = true;
            }

            // Try to copy summary log
            if (await TryXbcpCopyAsync(xboxSummaryPath, destSummaryPath, cancellationToken).ConfigureAwait(false))
            {
                copiedAny = true;
            }

            if (!copiedAny)
            {
                Log($"Fallback log gather (Xbox): no log files found on Xbox.");
            }
        }

        /// <summary>
        /// Executes xbcp to copy a file from Xbox to local machine.
        /// </summary>
        private async Task<bool> TryXbcpCopyAsync(string xboxPath, string localPath, CancellationToken cancellationToken)
        {
            try
            {
                var psi = new ProcessStartInfo
                {
                    FileName = "xbcp.exe",
                    Arguments = $"\"{xboxPath}\" \"{localPath}\"",
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    CreateNoWindow = true
                };

                using var process = new Process { StartInfo = psi };
                process.Start();

                // Read output asynchronously
                Task<string> stdoutTask = process.StandardOutput.ReadToEndAsync();
                Task<string> stderrTask = process.StandardError.ReadToEndAsync();

                // Wait for process with timeout
                using var timeoutCts = CancellationTokenSource.CreateLinkedTokenSource(cancellationToken);
                timeoutCts.CancelAfter(TimeSpan.FromSeconds(30));

                try
                {
                    await process.WaitForExitAsync(timeoutCts.Token).ConfigureAwait(false);
                }
                catch (OperationCanceledException) when (!cancellationToken.IsCancellationRequested)
                {
                    // Timeout
                    try { process.Kill(); } catch { }
                    Log($"Fallback log gather (Xbox): xbcp timed out for '{xboxPath}'.");
                    return false;
                }

                string stderr = await stderrTask.ConfigureAwait(false);

                if (process.ExitCode == 0 && File.Exists(localPath))
                {
                    var fi = new FileInfo(localPath);
                    Log($"Fallback log gather (Xbox): copied {fi.Length} bytes from '{xboxPath}' to '{localPath}'.");
                    return true;
                }
                else
                {
                    // Don't log error for file not found - that's expected if device crashed before logging
                    if (!stderr.Contains("0x80070002") && !stderr.Contains("cannot find"))
                    {
                        Log($"Fallback log gather (Xbox): xbcp failed for '{xboxPath}': {stderr.Trim()}");
                    }
                    return false;
                }
            }
            catch (Exception ex) when (ex is not OperationCanceledException)
            {
                Log($"Fallback log gather (Xbox): xbcp error for '{xboxPath}': {ex.Message}");
                return false;
            }
        }

        /// <summary>
        /// Finds the repository root by looking for the Out directory.
        /// </summary>
        private static string? FindRepoRoot()
        {
            string? current = AppContext.BaseDirectory;
            while (!string.IsNullOrEmpty(current))
            {
                string outDir = Path.Combine(current, "Out");
                if (Directory.Exists(outDir))
                {
                    return current;
                }

                string? parent = Path.GetDirectoryName(current);
                if (parent == current)
                {
                    break;
                }
                current = parent;
            }
            return null;
        }

        private async Task FetchLogChunksAsync(
            Guid clientId,
            string logPath,
            long fileStartOffset,
            long bytesToTransfer,
            string outputPath,
            CancellationToken cancellationToken)
        {
            const int chunkSize = 64 * 1024;
            const int chunkTimeoutSeconds = 15;

            using var stream = new FileStream(outputPath, FileMode.Create, FileAccess.Write, FileShare.None);
            using var writer = new StreamWriter(stream, leaveOpen: true);

            long offset = 0;
            while (offset < bytesToTransfer)
            {
                cancellationToken.ThrowIfCancellationRequested();

                int requestSize = (int)Math.Min(chunkSize, bytesToTransfer - offset);

                var chunkEnvelope = new CommandEnvelope
                {
                    CommandId = Guid.NewGuid().ToString("N"),
                    Command = "GatherLogsChunk",
                    TimeoutSeconds = chunkTimeoutSeconds,
                    Parameters = new Dictionary<string, object>
                    {
                        { "logPath", logPath },
                        { "fileOffset", fileStartOffset + offset },
                        { "size", requestSize }
                    }
                };

                ActionResult chunkResult = await _commandProcessor.SendCommandAsync(
                    chunkEnvelope,
                    payload => _transport.SendTextToDeviceAsync(clientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(chunkTimeoutSeconds),
                    cancellationToken).ConfigureAwait(false);

                if (!IsSuccessStatus(chunkResult.Status))
                {
                    string hrText = string.IsNullOrWhiteSpace(chunkResult.HResult) ? "<none>" : chunkResult.HResult!.Trim();
                    throw new InvalidOperationException($"GatherLogsChunk failed at offset {offset} (hr={hrText}).");
                }

                if (!ControllerRuntime.TryExtractGatherLogsChunkData(chunkResult.RawJson, out string chunkContent, out long bytesRead))
                {
                    throw new InvalidOperationException($"GatherLogsChunk returned unexpected payload at offset {offset}.");
                }

                if (bytesRead <= 0)
                {
                    break;
                }

                await writer.WriteAsync(chunkContent).ConfigureAwait(false);
                offset += bytesRead;
            }

            await writer.FlushAsync(cancellationToken).ConfigureAwait(false);
        }

        private async Task GatherSnapshotForFailureAsync(
            ScenarioPlan plan,
            string role,
            DeviceAssignment assignment,
            string outputDirectory,
            string scenarioId,
            string? scenarioName,
            int sequenceIndex,
            CancellationToken cancellationToken)
        {
            const int timeoutSeconds = 45;

            string deviceLabel = string.IsNullOrWhiteSpace(assignment.DisplayName)
                ? assignment.ClientId.ToString("N")
                : assignment.DisplayName;

            Log($"[failure snapshot] Requesting GatherSnapshot from role '{role}' ({deviceLabel}).", true);

            var envelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "GatherSnapshot",
                TimeoutSeconds = timeoutSeconds,
                ScenarioContext = new ScenarioContext
                {
                    ScenarioId = scenarioId,
                    ScenarioName = scenarioName,
                    Role = role,
                    StepIndex = plan.Commands.Count + plan.CleanupCommands.Count + sequenceIndex
                }
            };

            try
            {
                ActionResult result = await _commandProcessor.SendCommandAsync(
                    envelope,
                    payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(timeoutSeconds),
                    cancellationToken).ConfigureAwait(false);

                LogActionResultJson(envelope.Command, result, deviceLabel);

                if (!IsSuccessStatus(result.Status))
                {
                    string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                    string hrText = string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim();
                    string errorSuffix = string.IsNullOrWhiteSpace(result.ErrorMessage) ? string.Empty : $" Error: {SanitizeForLog(result.ErrorMessage)}";
                    Log($"Automatic GatherSnapshot for role '{role}' failed with status '{statusText}' (hr={hrText}).{errorSuffix}");
                    return;
                }

                if (!ControllerRuntime.TryExtractGatheredSnapshotData(result.RawJson, out ControllerRuntime.GatheredSnapshotData data))
                {
                    Log($"Automatic GatherSnapshot for role '{role}' returned an unexpected payload; no archive was saved.");
                    return;
                }

                cancellationToken.ThrowIfCancellationRequested();

                byte[] archiveBytes = Array.Empty<byte>();
                if (!string.IsNullOrEmpty(data.Base64Content))
                {
                    try
                    {
                        archiveBytes = Convert.FromBase64String(data.Base64Content);
                    }
                    catch (FormatException)
                    {
                        Log($"Automatic GatherSnapshot for role '{role}' returned invalid base64 content; no archive was saved.");
                        return;
                    }
                }

                string scenarioComponent = ControllerRuntime.SanitizeFileComponent(plan.Manifest.Id, "scenario");
                string roleComponent = ControllerRuntime.SanitizeFileComponent(role, "role");
                string deviceComponent = ControllerRuntime.SanitizeFileComponent(deviceLabel, "device");
                string sanitizedArchiveName = ControllerRuntime.SanitizeFileComponent(data.ArchiveFileName, "snapshot.zip");
                if (!sanitizedArchiveName.EndsWith(".zip", StringComparison.OrdinalIgnoreCase))
                {
                    sanitizedArchiveName += ".zip";
                }

                string timestamp = DateTime.UtcNow.ToString("yyyyMMdd-HHmmssfff", CultureInfo.InvariantCulture);
                string outputFileName = $"failure-{scenarioComponent}-{roleComponent}-{timestamp}-{sanitizedArchiveName}";
                string outputPath = Path.Combine(outputDirectory, outputFileName);

                await File.WriteAllBytesAsync(outputPath, archiveBytes, cancellationToken).ConfigureAwait(false);

                string truncatedSuffix = data.Truncated ? " (truncated)" : string.Empty;
                string message = $"Automatic GatherSnapshot saved {archiveBytes.LongLength} of {data.ArchiveSize} bytes for role '{role}' ({deviceLabel}) to '{outputPath}'{truncatedSuffix}.";
                if (!string.IsNullOrWhiteSpace(data.SaveFolder))
                {
                    message += $" Source folder: {data.SaveFolder}.";
                }

                Log(message);
            }
            catch (TimeoutException)
            {
                Log($"Automatic GatherSnapshot for role '{role}' timed out after {timeoutSeconds} seconds.");
            }
            catch (OperationCanceledException)
            {
                Log("Automatic GatherSnapshot operation canceled.");
                throw;
            }
            catch (Exception ex)
            {
                Log($"Automatic GatherSnapshot for role '{role}' failed: {ex.Message}");
            }
        }

        private async Task RunCleanupAsync(
            ScenarioPlan plan,
            IReadOnlyDictionary<string, DeviceAssignment> assignments,
            ISet<Guid> debugStatsEligibleClients,
            ISet<Guid> forcedDisconnectedClients,
            ScenarioRunOutcome outcome,
            string scenarioId,
            string? scenarioName,
            int startingStepIndex,
            string? customIdPrefix,
            CancellationToken cancellationToken)
        {
            if (plan.CleanupCommands.Count == 0)
            {
                return;
            }

            Log($"Beginning cleanup for scenario '{plan.Manifest.Id}' (outcome={outcome}).");

            int cleanupIndex = 0;
            int total = plan.CleanupCommands.Count;

            foreach (ScenarioCleanupInvocation cleanup in plan.CleanupCommands)
            {
                cancellationToken.ThrowIfCancellationRequested();

                cleanupIndex++;
                ScenarioStep step = cleanup.Step;
                string commandName = step.Command ?? string.Empty;

                int timeoutSeconds = step.TimeoutSeconds ?? plan.DefaultTimeoutSeconds;
                if (timeoutSeconds <= 0)
                {
                    timeoutSeconds = plan.DefaultTimeoutSeconds > 0 ? plan.DefaultTimeoutSeconds : 30;
                }

                if (!assignments.TryGetValue(cleanup.Role, out DeviceAssignment assignment))
                {
                    Log($"Cleanup command '{commandName}' skipped: no assignment for role '{cleanup.Role}'.", false);
                    continue;
                }

                if (forcedDisconnectedClients.Contains(assignment.ClientId))
                {
                    Log($"Cleanup command '{commandName}' skipped: device '{assignment.DisplayName}' was force-disconnected after timeout.");
                    continue;
                }

                CommandEnvelope envelope = new CommandEnvelope
                {
                    CommandId = Guid.NewGuid().ToString("N"),
                    Command = commandName,
                    TimeoutSeconds = timeoutSeconds,
                    Parameters = SubstituteTemplateVariables(NormalizeParameters(step.Parameters)),
                    ScenarioContext = new ScenarioContext
                    {
                        ScenarioId = scenarioId,
                        ScenarioName = scenarioName,
                        Role = cleanup.Role,
                        StepIndex = startingStepIndex + cleanupIndex
                    }
                };

                // Per-scenario customId prefix application is restricted to
                // PFLocalUserCreateHandleWithPersistedLocalId for the same reason as the main
                // loop (see comment near PFLocalUserCreateHandleWithPersistedLocalId check
                // in RunAsync). Cleanup steps that login via PFAuthenticationLoginWithCustomID
                // intentionally use the shared entity to avoid Xbox link/unlink churn.
                if (!string.IsNullOrEmpty(customIdPrefix) &&
                    string.Equals(commandName, "PFLocalUserCreateHandleWithPersistedLocalId", StringComparison.OrdinalIgnoreCase))
                {
                    TryApplyCustomIdPrefix(envelope.Parameters, customIdPrefix);
                }

                ControllerCommandType cleanupControllerCommandType = GetControllerCommandType(commandName);
                bool isCleanupControllerCommand = cleanupControllerCommandType != ControllerCommandType.None;

                Log($"[cleanup {cleanupIndex}/{total}] {(isCleanupControllerCommand ? "Executing" : "Sending")} '{commandName}' {(isCleanupControllerCommand ? "locally" : $"to '{assignment.DisplayName}'")} (timeout={timeoutSeconds}s).", true);

                try
                {
                    ActionResult result;
                    if (isCleanupControllerCommand)
                    {
                        result = ExecuteControllerCommand(cleanupControllerCommandType, envelope, assignment, assignments, cancellationToken);
                    }
                    else
                    {
                    if (debugStatsEligibleClients.Contains(assignment.ClientId) &&
                        (IsLocalUserCloseHandleCommand(commandName) || IsGameSaveUninitializeCommand(commandName)))
                    {
                        await GatherDebugStatsForAssignmentAsync(
                            plan,
                            cleanup.Role,
                            assignment,
                            debugStatsEligibleClients,
                            scenarioId,
                            scenarioName,
                            cancellationToken).ConfigureAwait(false);
                    }

                    result = await _commandProcessor.SendCommandAsync(
                        envelope,
                        payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                        _jsonOptions,
                        TimeSpan.FromSeconds(timeoutSeconds),
                        cancellationToken).ConfigureAwait(false);
                    }

                    bool hasHresult = TryParseHResult(result.HResult, out int parsedHresult, out string hresultDisplay);
                    bool hresultFailure = hasHresult && parsedHresult < 0;
                    LogActionResultJson(commandName, result, assignment.DisplayName);
                    if (!IsSuccessStatus(result.Status) || hresultFailure)
                    {
                        string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                        string hrText = string.IsNullOrEmpty(hresultDisplay)
                            ? (string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim())
                            : hresultDisplay;
                        string errorSuffix = string.IsNullOrWhiteSpace(result.ErrorMessage) ? string.Empty : $" Error: {SanitizeForLog(result.ErrorMessage)}";
                        Log($"Cleanup command '{commandName}' returned status '{statusText}' (hr={hrText}).{errorSuffix} Continuing cleanup.");
                    }
                    else
                    {
                        string hrText = string.IsNullOrEmpty(hresultDisplay)
                            ? (string.IsNullOrWhiteSpace(result.HResult) ? string.Empty : result.HResult!.Trim())
                            : hresultDisplay;
                        string hrSuffix = string.IsNullOrEmpty(hrText) ? string.Empty : $" (hr={hrText})";
                        Log($"Cleanup command '{commandName}' succeeded in {result.ElapsedMs} ms{hrSuffix}.", true);

                        if (IsGameSaveAddUserCommand(commandName))
                        {
                            debugStatsEligibleClients.Add(assignment.ClientId);
                        }
                    }
                }
                catch (TimeoutException)
                {
                    Log($"Cleanup command '{commandName}' timed out after {timeoutSeconds} seconds. Continuing cleanup.");
                }
                catch (OperationCanceledException)
                {
                    Log("Cleanup cancelled.");
                    throw;
                }
                catch (Exception ex)
                {
                    Log($"Cleanup command '{commandName}' failed: {ex.Message}. Continuing cleanup.");
                }
            }

            Log($"Cleanup for scenario '{plan.Manifest.Id}' finished.");
        }

        private void LogActionResultJson(string commandName, ActionResult result, string defaultSource)
        {
            if (result == null)
            {
                return;
            }

            if (string.IsNullOrWhiteSpace(result.RawJson))
            {
                return;
            }

            string source = !string.IsNullOrWhiteSpace(result.DeviceId) ? result.DeviceId : defaultSource;
            if (string.IsNullOrWhiteSpace(source))
            {
                source = "controller";
            }

            Log($"[{source}] Command '{commandName}' action result JSON: {result.RawJson}", true);
        }

        private async Task GatherDebugStatsForAssignmentAsync(
            ScenarioPlan plan,
            string role,
            DeviceAssignment assignment,
            ISet<Guid> debugStatsEligibleClients,
            string scenarioIdContext,
            string? scenarioNameContext,
            CancellationToken cancellationToken)
        {
            const int timeoutSeconds = 30;

            cancellationToken.ThrowIfCancellationRequested();

            debugStatsEligibleClients.Remove(assignment.ClientId);

            Log($"[DebugStats] Collecting stats from role '{role}' ({assignment.DisplayName}).", true);

            var envelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "GetDebugStats",
                TimeoutSeconds = timeoutSeconds,
                ScenarioContext = new ScenarioContext
                {
                    ScenarioId = scenarioIdContext,
                    ScenarioName = scenarioNameContext,
                    Role = role
                }
            };

            try
            {
                ActionResult result = await _commandProcessor.SendCommandAsync(
                    envelope,
                    payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(timeoutSeconds),
                    cancellationToken).ConfigureAwait(false);

                bool hasHresult = TryParseHResult(result.HResult, out int parsedHresult, out string hresultDisplay);
                bool hresultFailure = hasHresult && parsedHresult < 0;
                if (!IsSuccessStatus(result.Status) || hresultFailure)
                {
                    string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                    string hrText = string.IsNullOrEmpty(hresultDisplay)
                        ? (string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim())
                        : hresultDisplay;
                    Log($"[DebugStats] Role '{role}' ({assignment.DisplayName}) returned status '{statusText}' (hr={hrText}).", true);
                    return;
                }

                LogActionResultJson("GetDebugStats", result, assignment.DisplayName);

                if (!TryExtractDebugStatsData(result.RawJson, out string deviceName, out string statsJson))
                {
                    Log($"[DebugStats] Role '{role}' ({assignment.DisplayName}) did not return stats payload.", true);
                    return;
                }

                string resolvedName = string.IsNullOrWhiteSpace(deviceName) ? assignment.DisplayName : deviceName;
                DebugStatsLog formatted = BuildDebugStatsLog(statsJson);
                Log($"[DebugStats] Scenario '{plan.Manifest.Id}' role '{role}' ({resolvedName}) stats: {formatted.Summary}", chaosLog: true);
                foreach (string detail in formatted.Details)
                {
                    Log($"[DebugStats] Scenario '{plan.Manifest.Id}' role '{role}' ({resolvedName}) detail: {detail}", true, true);
                }
            }
            catch (TimeoutException)
            {
                Log($"[DebugStats] Timed out retrieving stats for role '{role}' ({assignment.DisplayName}).", true);
            }
            catch (OperationCanceledException)
            {
                Log("[DebugStats] Stats collection cancelled.");
                throw;
            }
            catch (Exception ex)
            {
                Log($"[DebugStats] Failed to retrieve stats for role '{role}' ({assignment.DisplayName}): {SanitizeForLog(ex.Message)}", true);
            }
        }

        private static bool TryExtractDebugStatsData(string? rawJson, out string deviceName, out string statsJson)
        {
            deviceName = string.Empty;
            statsJson = string.Empty;

            if (string.IsNullOrWhiteSpace(rawJson))
            {
                return false;
            }

            try
            {
                using JsonDocument document = JsonDocument.Parse(rawJson);
                JsonElement root = document.RootElement;

                if (root.TryGetProperty("deviceName", out JsonElement deviceElement))
                {
                    deviceName = deviceElement.ValueKind switch
                    {
                        JsonValueKind.String => deviceElement.GetString() ?? string.Empty,
                        JsonValueKind.Null => string.Empty,
                        _ => deviceElement.GetRawText()
                    };
                }

                if (!root.TryGetProperty("statsJson", out JsonElement statsElement))
                {
                    return false;
                }

                statsJson = statsElement.ValueKind switch
                {
                    JsonValueKind.String => statsElement.GetString() ?? string.Empty,
                    JsonValueKind.Null => string.Empty,
                    _ => statsElement.GetRawText()
                };

                return true;
            }
            catch (JsonException)
            {
                return false;
            }
        }

        private static DebugStatsLog BuildDebugStatsLog(string statsJson)
        {
            if (string.IsNullOrWhiteSpace(statsJson))
            {
                return new DebugStatsLog("<empty>", Array.Empty<string>());
            }

            try
            {
                using JsonDocument document = JsonDocument.Parse(statsJson);
                JsonElement root = document.RootElement;

                var summarySegments = new List<string>();
                var detailLines = new List<string>();

                DebugFileAggregate downloads = ExtractFileAggregate(root, "FilesToDownload");
                summarySegments.Add(downloads.HasEntries
                    ? $"downloads={downloads.Count}{FormatOptionalSize(downloads.TotalSize)}"
                    : "downloads=none");
                if (downloads.HasEntries)
                {
                    detailLines.Add($"files to download: {FormatFileEntries(downloads.Entries)}");
                }

                DebugFileAggregate uploads = ExtractFileAggregate(root, "FilesToUpload");
                summarySegments.Add(uploads.HasEntries
                    ? $"uploads={uploads.Count}{FormatOptionalSize(uploads.TotalSize)}"
                    : "uploads=none");
                if (uploads.HasEntries)
                {
                    detailLines.Add($"files to upload: {FormatFileEntries(uploads.Entries)}");
                }

                DebugFileAggregate compressedDownloads = ExtractFileAggregate(
                    root,
                    "CompressedFilesToDownload",
                    sizePropertyName: "Size",
                    compressedSizePropertyName: "CompressedSize",
                    cachedPropertyName: "DownloadedLocally");
                summarySegments.Add(compressedDownloads.HasEntries
                    ? $"compressedDownloads={compressedDownloads.Count}{FormatOptionalSize(compressedDownloads.TotalCompressedSize ?? compressedDownloads.TotalSize)}"
                    : "compressedDownloads=none");
                if (compressedDownloads.HasEntries)
                {
                    detailLines.Add($"compressed downloads: {FormatFileEntries(compressedDownloads.Entries)}");
                }

                DebugFileAggregate compressedUploads = ExtractFileAggregate(
                    root,
                    "CompressedFilesToUpload",
                    sizePropertyName: "Size",
                    compressedSizePropertyName: "CompressedSize");
                summarySegments.Add(compressedUploads.HasEntries
                    ? $"compressedUploads={compressedUploads.Count}{FormatOptionalSize(compressedUploads.TotalCompressedSize ?? compressedUploads.TotalSize)}"
                    : "compressedUploads=none");
                if (compressedUploads.HasEntries)
                {
                    detailLines.Add($"compressed uploads: {FormatFileEntries(compressedUploads.Entries)}");
                }

                IReadOnlyList<string> skipped = ExtractNameList(root, "SkippedFiles");
                summarySegments.Add(skipped.Count > 0 ? $"skipped={skipped.Count}" : "skipped=none");
                if (skipped.Count > 0)
                {
                    detailLines.Add($"skipped files: {FormatNameList(skipped)}");
                }

                long? manifestFiles = TryGetInt64(root, "Upload", "NumFilesInFinalizedManifest");
                if (manifestFiles.HasValue)
                {
                    summarySegments.Add($"manifestFiles={manifestFiles.Value}");
                }

                string summary = string.Join("; ", summarySegments.Where(segment => !string.IsNullOrWhiteSpace(segment)));
                return new DebugStatsLog(summary, detailLines);
            }
            catch (JsonException)
            {
                return new DebugStatsLog(SanitizeForLog(statsJson), Array.Empty<string>());
            }
        }

        private static DebugFileAggregate ExtractFileAggregate(
            JsonElement root,
            string propertyName,
            string? sizePropertyName = "Size",
            string? compressedSizePropertyName = null,
            string? cachedPropertyName = null)
        {
            if (!root.TryGetProperty(propertyName, out JsonElement arrayElement) || arrayElement.ValueKind != JsonValueKind.Array)
            {
                return DebugFileAggregate.Empty;
            }

            var entries = new List<DebugFileEntry>();
            long totalSize = 0;
            long totalCompressedSize = 0;
            bool hasCompressed = false;

            foreach (JsonElement item in arrayElement.EnumerateArray())
            {
                if (item.ValueKind == JsonValueKind.Null)
                {
                    continue;
                }

                string name = TryGetString(item, "Name") ?? item.ToString();
                long size = sizePropertyName != null ? TryGetInt64(item, sizePropertyName) ?? 0 : 0;
                long? compressedSize = compressedSizePropertyName != null ? TryGetInt64(item, compressedSizePropertyName) : null;
                bool? cached = cachedPropertyName != null ? TryGetBoolean(item, cachedPropertyName) : null;

                entries.Add(new DebugFileEntry(name, size, compressedSize, cached));

                totalSize += size;
                if (compressedSize.HasValue)
                {
                    totalCompressedSize += compressedSize.Value;
                    hasCompressed = true;
                }
            }

            return entries.Count == 0
                ? DebugFileAggregate.Empty
                : new DebugFileAggregate(entries.Count, totalSize, hasCompressed ? totalCompressedSize : null, entries);
        }

        private static IReadOnlyList<string> ExtractNameList(JsonElement root, string propertyName)
        {
            if (!root.TryGetProperty(propertyName, out JsonElement element) || element.ValueKind != JsonValueKind.Array)
            {
                return Array.Empty<string>();
            }

            var names = new List<string>();
            foreach (JsonElement item in element.EnumerateArray())
            {
                string? name = item.ValueKind switch
                {
                    JsonValueKind.String => item.GetString(),
                    JsonValueKind.Object => TryGetString(item, "Name"),
                    _ => null
                };

                if (!string.IsNullOrWhiteSpace(name))
                {
                    names.Add(name);
                }
            }

            return names.Count == 0 ? Array.Empty<string>() : names;
        }

        private static string FormatFileEntries(IReadOnlyList<DebugFileEntry> entries)
        {
            if (entries.Count == 0)
            {
                return "none";
            }

            const int maxItems = 3;
            var segments = new List<string>();
            for (int index = 0; index < entries.Count && index < maxItems; index++)
            {
                DebugFileEntry entry = entries[index];
                string sizeText = entry.Size > 0 ? FormatSize(entry.Size) : "0 B";
                string text = string.IsNullOrWhiteSpace(entry.Name) ? "<unnamed>" : entry.Name;
                text += $" ({sizeText}";
                if (entry.CompressedSize.HasValue)
                {
                    text += $", compressed {FormatSize(entry.CompressedSize.Value)}";
                }

                if (entry.DownloadedLocally.HasValue)
                {
                    text += entry.DownloadedLocally.Value ? ", cached" : ", not cached";
                }

                text += ")";
                segments.Add(text);
            }

            if (entries.Count > maxItems)
            {
                segments.Add($"+{entries.Count - maxItems} more");
            }

            return string.Join(", ", segments);
        }

        private static string FormatNameList(IReadOnlyList<string> names)
        {
            if (names.Count == 0)
            {
                return "none";
            }

            const int maxItems = 5;
            var segments = new List<string>();
            for (int index = 0; index < names.Count && index < maxItems; index++)
            {
                segments.Add(names[index]);
            }

            if (names.Count > maxItems)
            {
                segments.Add($"+{names.Count - maxItems} more");
            }

            return string.Join(", ", segments);
        }

        private static string FormatOptionalSize(long? bytes)
        {
            if (!bytes.HasValue || bytes.Value <= 0)
            {
                return string.Empty;
            }

            return $" ({FormatSize(bytes.Value)})";
        }

        private static string FormatSize(long bytes)
        {
            if (bytes <= 0)
            {
                return "0 B";
            }

            string[] units = new[] { "B", "KB", "MB", "GB", "TB" };
            double value = bytes;
            int index = 0;
            while (value >= 1024 && index < units.Length - 1)
            {
                value /= 1024;
                index++;
            }

            return value >= 10 || index == 0 ? $"{value:0} {units[index]}" : $"{value:0.0} {units[index]}";
        }

        private static long? TryGetInt64(JsonElement element, string propertyName)
        {
            if (!element.TryGetProperty(propertyName, out JsonElement value))
            {
                return null;
            }

            return ConvertToInt64(value);
        }

        private static long? TryGetInt64(JsonElement element, string objectPropertyName, string nestedPropertyName)
        {
            if (!element.TryGetProperty(objectPropertyName, out JsonElement nested) || nested.ValueKind != JsonValueKind.Object)
            {
                return null;
            }

            return TryGetInt64(nested, nestedPropertyName);
        }

        private static long? ConvertToInt64(JsonElement value)
        {
            switch (value.ValueKind)
            {
                case JsonValueKind.Number when value.TryGetInt64(out long intValue):
                    return intValue;
                case JsonValueKind.String:
                    {
                        string? text = value.GetString();
                        if (long.TryParse(text, NumberStyles.Integer, CultureInfo.InvariantCulture, out long parsed))
                        {
                            return parsed;
                        }

                        return null;
                    }
                default:
                    return null;
            }
        }

        private static bool? TryGetBoolean(JsonElement element, string propertyName)
        {
            if (!element.TryGetProperty(propertyName, out JsonElement value))
            {
                return null;
            }

            return value.ValueKind switch
            {
                JsonValueKind.True => true,
                JsonValueKind.False => false,
                JsonValueKind.String => bool.TryParse(value.GetString(), out bool parsed) ? parsed : (bool?)null,
                _ => null
            };
        }

        private static string? TryGetString(JsonElement element, string propertyName)
        {
            if (!element.TryGetProperty(propertyName, out JsonElement value))
            {
                return null;
            }

            return value.ValueKind switch
            {
                JsonValueKind.String => value.GetString(),
                JsonValueKind.Null => null,
                _ => value.GetRawText()
            };
        }

        private readonly record struct DebugStatsLog(string Summary, IReadOnlyList<string> Details);

        private readonly record struct DebugFileAggregate(int Count, long TotalSize, long? TotalCompressedSize, IReadOnlyList<DebugFileEntry> Entries)
        {
            public bool HasEntries => Count > 0;

            public static DebugFileAggregate Empty { get; } = new DebugFileAggregate(0, 0, null, Array.Empty<DebugFileEntry>());
        }

        private readonly record struct DebugFileEntry(string Name, long Size, long? CompressedSize, bool? DownloadedLocally);

        private static string SanitizeForLog(string? text)
        {
            if (string.IsNullOrWhiteSpace(text))
            {
                return string.Empty;
            }

            return text.Replace('\r', ' ').Replace('\n', ' ').Trim();
        }

        private void Log(string message, bool verbose = false, bool chaosLog = false)
        {
            _logger(message, verbose, chaosLog);
        }

        private async Task<bool> TryBuildRoleAssignmentsAsync(ScenarioPlan plan, Dictionary<string, DeviceAssignment> assignments, CancellationToken cancellationToken)
        {
            assignments.Clear();
            IReadOnlyCollection<string> required = plan.RequiredDevices;
            if (required.Count == 0)
            {
                return true;
            }

            List<string> orderedRoles = BuildRoleOrder(plan, required);

            // Allow up to 15 seconds for pending capability handshakes to complete.
            // Devices may have connected but not yet reported their engine/commands.
            const int maxWaitMs = 15000;
            const int pollIntervalMs = 250;
            int elapsed = 0;

            while (true)
            {
                IReadOnlyCollection<DeviceConnectionInfo> connectedDetails = _transport.GetConnectedDeviceDetails();

                IReadOnlyList<string> pendingCapabilityDevices = connectedDetails
                    .Where(device => device.IsConnected && !device.CapabilitiesReady)
                    .Select(device => string.IsNullOrWhiteSpace(device.DisplayName) ? device.ClientId.ToString() : device.DisplayName)
                    .ToArray();

                List<DeviceConnectionInfo> available = connectedDetails
                    .Where(device => device.IsConnected && device.CapabilitiesReady)
                    .OrderBy(device => device.DisplayName, StringComparer.OrdinalIgnoreCase)
                    .ToList();

                if (pendingCapabilityDevices.Count > 0 && available.Count < orderedRoles.Count && elapsed < maxWaitMs)
                {
                    if (elapsed == 0)
                    {
                        Log($"Waiting for capability handshake from: {string.Join(", ", pendingCapabilityDevices)}.", true);
                    }
                    await Task.Delay(pollIntervalMs, cancellationToken).ConfigureAwait(false);
                    elapsed += pollIntervalMs;
                    continue;
                }

                if (pendingCapabilityDevices.Count > 0 && elapsed >= maxWaitMs)
                {
                    Log($"Capability handshake timed out for: {string.Join(", ", pendingCapabilityDevices)}. Proceeding with {available.Count} ready device(s).");
                }

                return TryAssignFromAvailable(plan, orderedRoles, available, connectedDetails, assignments);
            }
        }

        private bool TryAssignFromAvailable(
            ScenarioPlan plan,
            List<string> orderedRoles,
            List<DeviceConnectionInfo> available,
            IReadOnlyCollection<DeviceConnectionInfo> connectedDetails,
            Dictionary<string, DeviceAssignment> assignments)
        {

            if (available.Count == 0)
            {
                Log($"Scenario '{plan.Manifest.Id}' requires {orderedRoles.Count} device(s): {string.Join(", ", orderedRoles)}. No devices ready yet (waiting for capability announcements).");
                return false;
            }

            int uniqueRoleCount = orderedRoles.Count;
            if (available.Count < uniqueRoleCount)
            {
                Log($"Scenario '{plan.Manifest.Id}' requires {uniqueRoleCount} device(s): {string.Join(", ", orderedRoles)}. Connected: {string.Join(", ", available.Select(device => device.DisplayName))}. Waiting for additional device(s).");
                return false;
            }

            // Two-pass assignment: constrained roles (with engine requirement) first,
            // then unconstrained roles. This prevents an unconstrained role from
            // consuming the only device that satisfies a later engine constraint.
            var constrainedRoles = new List<string>();
            var unconstrainedRoles = new List<string>();
            foreach (string role in orderedRoles)
            {
                if (string.IsNullOrWhiteSpace(role))
                {
                    continue;
                }

                bool isConstrained = false;
                if (plan.Manifest.Devices.TryGetValue(role, out ScenarioDevice? deviceSpec))
                {
                    isConstrained = deviceSpec?.HasEngineConstraint ?? false;
                }

                if (isConstrained)
                {
                    constrainedRoles.Add(role);
                }
                else
                {
                    unconstrainedRoles.Add(role);
                }
            }

            var usedClients = new HashSet<Guid>();
            var lookupByName = available
                .GroupBy(device => device.DisplayName, StringComparer.OrdinalIgnoreCase)
                .ToDictionary(group => group.Key, group => group.First(), StringComparer.OrdinalIgnoreCase);

            // Pass 1: assign engine-constrained roles
            foreach (string role in constrainedRoles)
            {
                ScenarioDevice spec = plan.Manifest.Devices[role];
                string engineDisplay = spec.EngineDisplay;

                DeviceConnectionInfo? selected = null;

                // Prefer name-match that also satisfies engine
                if (lookupByName.TryGetValue(role, out DeviceConnectionInfo nameMatch)
                    && !usedClients.Contains(nameMatch.ClientId)
                    && spec.EngineMatches(nameMatch.Engine))
                {
                    selected = nameMatch;
                }

                // Fall back to any available device matching the engine.
                // Note: DeviceConnectionInfo is a record struct, so FirstOrDefault returns
                // default(DeviceConnectionInfo) (Guid.Empty) when no match — not null.
                // Cast the result to nullable explicitly to get proper null semantics.
                if (selected == null)
                {
                    selected = available
                        .Cast<DeviceConnectionInfo?>()
                        .FirstOrDefault(device =>
                            device.HasValue
                            && !usedClients.Contains(device.Value.ClientId)
                            && spec.EngineMatches(device.Value.Engine));
                }

                if (selected == null)
                {
                    // Check if a matching device is still completing capability handshake
                    bool pendingMatch = connectedDetails.Any(device =>
                        device.IsConnected && !device.CapabilitiesReady
                        && spec.EngineMatches(device.Engine));

                    if (pendingMatch)
                    {
                        string reason = $"Role '{role}' requires engine '{engineDisplay}' — a matching device is connected but capability handshake is pending.";
                        Log($"Scenario '{plan.Manifest.Id}': {reason}");
                        LastSkipReason = reason;
                    }
                    else
                    {
                        string engines = string.Join(", ", available.Where(d => !usedClients.Contains(d.ClientId)).Select(d => $"{d.DisplayName}({d.Engine})"));
                        string reason = $"Role '{role}' requires engine '{engineDisplay}' but no compatible device is available. Connected: [{engines}].";
                        Log($"SKIPPED scenario '{plan.Manifest.Id}': {reason}");
                        LastSkipReason = reason;
                    }

                    assignments.Clear();
                    return false;
                }

                assignments[role] = new DeviceAssignment(selected.Value.ClientId, selected.Value.DisplayName, selected.Value.Engine, selected.Value.RemoteEndpoint);
                usedClients.Add(selected.Value.ClientId);
            }

            // Pass 2: assign unconstrained roles from remaining devices
            foreach (string role in unconstrainedRoles)
            {
                DeviceConnectionInfo? selected = null;

                if (lookupByName.TryGetValue(role, out DeviceConnectionInfo match) && !usedClients.Contains(match.ClientId))
                {
                    selected = match;
                }

                // Note: DeviceConnectionInfo is a record struct, so FirstOrDefault returns
                // default(DeviceConnectionInfo) (Guid.Empty) when no match — not null.
                if (selected == null)
                {
                    selected = available
                        .Cast<DeviceConnectionInfo?>()
                        .FirstOrDefault(device =>
                            device.HasValue
                            && !usedClients.Contains(device.Value.ClientId));
                }

                if (selected == null)
                {
                    Log($"Scenario '{plan.Manifest.Id}' requires role '{role}', but no unassigned device is available.");
                    assignments.Clear();
                    return false;
                }

                assignments[role] = new DeviceAssignment(selected.Value.ClientId, selected.Value.DisplayName, selected.Value.Engine, selected.Value.RemoteEndpoint);
                usedClients.Add(selected.Value.ClientId);
            }

            if (assignments.Count == 0)
            {
                Log($"Scenario '{plan.Manifest.Id}' requires device assignments but none could be established.");
                return false;
            }

            Log($"Assigned roles: {string.Join(", ", assignments.Select(pair => $"{pair.Key}->{pair.Value.DisplayName} (engine={pair.Value.Engine})"))}.", true);
            return true;
        }

        private static List<string> BuildRoleOrder(ScenarioPlan plan, IReadOnlyCollection<string> required)
        {
            var orderedRoles = new List<string>();

            if (plan.Manifest.ExecutionOrder != null)
            {
                foreach (ScenarioExecutionOrder? entry in plan.Manifest.ExecutionOrder)
                {
                    string? role = entry?.Role;
                    if (string.IsNullOrWhiteSpace(role))
                    {
                        continue;
                    }

                    if (!orderedRoles.Any(existing => string.Equals(existing, role, StringComparison.OrdinalIgnoreCase)))
                    {
                        orderedRoles.Add(role);
                    }
                }
            }

            foreach (string role in required)
            {
                if (string.IsNullOrWhiteSpace(role))
                {
                    continue;
                }

                if (!orderedRoles.Any(existing => string.Equals(existing, role, StringComparison.OrdinalIgnoreCase)))
                {
                    orderedRoles.Add(role);
                }
            }

            return orderedRoles;
        }
    }

    internal readonly record struct DeviceAssignment(Guid ClientId, string DisplayName, string Engine, string RemoteEndpoint);

    internal enum ControllerCommandType
    {
        None = 0,
        CompareSaveContainerSnapshots,
        StartHttpTestServer,
        StopHttpTestServer,
        ConfigureHttpRoute,
        AssertHttpMaxConcurrency,
        StartWebSocketTestServer,
        StopWebSocketTestServer,
        WebSocketServerClose,
        Sleep,
        ChangeTargetDeviceState,
        AutoNavigateGameSaveUi,
        AssertGameSaveUiDialogCount,
        WaitForGameSaveSync,
        PfGameSaveUtilDownload,
        VerifyGameSaveUtilDownload,
        VerifyRingBufferIntegrity
    }

    internal enum ScenarioRunOutcome
    {
        NotStarted,
        Passed,
        Failed
    }
}
