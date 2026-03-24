using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

namespace PFGameSaveTestController
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
            ScenarioRunOutcome finalOutcome = ScenarioRunOutcome.NotStarted;
            OperationCanceledException? cancellationException = null;
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

                if (!TryBuildRoleAssignments(plan, out Dictionary<string, DeviceAssignment> assignments))
                {
                    finalOutcome = ScenarioRunOutcome.NotStarted;
                    return finalOutcome;
                }

                roleAssignments = assignments;

                Log($"Starting scenario '{plan.Manifest.Id}' ({plan.Commands.Count} command(s)).");

                int defaultTimeoutSeconds = plan.DefaultTimeoutSeconds > 0 ? plan.DefaultTimeoutSeconds : 30;
                int commandIndex = 0;
                ScenarioRunOutcome outcome = ScenarioRunOutcome.Passed;

                foreach (ScenarioCommandInvocation invocation in plan.Commands)
                {
                    commandIndex++;
                    string commandName = invocation.Step.Command ?? string.Empty;
                    int timeoutSeconds = invocation.Step.TimeoutSeconds ?? defaultTimeoutSeconds;
                    if (timeoutSeconds <= 0)
                    {
                        timeoutSeconds = defaultTimeoutSeconds;
                    }

                    ControllerCommandType controllerCommandType = GetControllerCommandType(commandName);
                    bool isControllerCommand = controllerCommandType != ControllerCommandType.None;

                    object? normalizedParameters = NormalizeParameters(invocation.Step.Parameters);
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

                    if (!string.IsNullOrEmpty(effectivePrefix) &&
                        string.Equals(commandName, "PFLocalUserCreateHandleWithPersistedLocalId", StringComparison.OrdinalIgnoreCase))
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
                            result = ExecuteControllerCommand(controllerCommandType, envelope, assignment, roleAssignments, cancellationToken);
                        }
                        else
                        {
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

                        bool hasHresult = TryParseHResult(result.HResult, out int parsedHresult, out string hresultDisplay);
                        bool hresultIndicatesFailure = hasHresult && parsedHresult < 0;
                        LogActionResultJson(commandName, result, isControllerCommand ? "controller" : deviceName);
                        
                        // Check if this command expects failure
                        bool expectFailure = false;
                        int? expectedHr = null;
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
                            
                            // Check if a specific HRESULT is expected
                            if (paramDict.ContainsKey("expectedHr"))
                            {
                                if (paramDict["expectedHr"] is string hrStr && TryParseHResult(hrStr, out int parsedExpectedHr, out _))
                                {
                                    expectedHr = parsedExpectedHr;
                                }
                            }
                        }
                        
                        // If expectedHr is specified and matches the actual HR, treat as success
                        bool expectedHrMatched = expectedHr.HasValue && hasHresult && parsedHresult == expectedHr.Value;
                        bool actuallyFailed = !IsSuccessStatus(result.Status) || (hresultIndicatesFailure && !expectedHrMatched);
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
                        await CaptureSnapshotsForFailedScenarioAsync(
                            plan,
                            roleAssignments,
                            pfGameSaveInitializedClients,
                            scenarioIdContext,
                            scenarioNameContext,
                            cancellationToken).ConfigureAwait(false);
                    }

                    if (roleAssignments != null && plan.CleanupCommands.Count > 0)
                    {
                        int startingIndex = plan.Commands.Count;
                        await RunCleanupAsync(
                            plan,
                            roleAssignments,
                            debugStatsEligibleClients,
                            finalOutcome,
                            scenarioIdContext,
                            scenarioNameContext,
                            startingIndex,
                            cancellationToken).ConfigureAwait(false);
                    }

                    // Gather logs from all devices at the end of every scenario
                    if (roleAssignments != null)
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

            if (commandName.Equals("CompareSaveContainerSnapshots", StringComparison.OrdinalIgnoreCase))
            {
                return ControllerCommandType.CompareSaveContainerSnapshots;
            }

            return ControllerCommandType.None;
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

            // Parse optional ignoreTimestamps parameter (workaround for GRTS timestamp bug)
            bool ignoreTimestamps = false;
            if (envelope.Parameters is IDictionary<string, object> parameters && parameters.TryGetValue("ignoreTimestamps", out object? ignoreTimestampsValue))
            {
                ignoreTimestamps = ignoreTimestampsValue switch
                {
                    bool b => b,
                    string s => bool.TryParse(s, out bool parsed) && parsed,
                    _ => false
                };
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
                SnapshotComparisonResult comparison = SnapshotComparer.Compare(leftRecord, rightRecord, out SnapshotComparer.SnapshotSummary left, out SnapshotComparer.SnapshotSummary right, ignoreTimestamps);
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
            if (success)
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
                    return;
                }

                if (!ControllerRuntime.TryExtractGatheredLogData(result.RawJson, out ControllerRuntime.GatheredLogData data))
                {
                    Log($"GatherLogs for '{deviceLabel}' returned an unexpected payload; no log was saved.");
                    return;
                }

                string sanitizedDeviceName = ControllerRuntime.SanitizeFileComponent(deviceLabel, "device");
                string outputFileName = $"pfgamesaves-device-{sanitizedDeviceName}-log.txt";
                string outputPath = Path.Combine(outputDirectory, outputFileName);

                await File.WriteAllTextAsync(outputPath, data.LogContent, cancellationToken).ConfigureAwait(false);

                string truncatedSuffix = data.Truncated ? " (truncated)" : string.Empty;
                string message = $"GatherLogs saved {data.BytesRead} of {data.FileSize} bytes from '{deviceLabel}' to '{outputPath}'{truncatedSuffix}.";
                if (!string.IsNullOrWhiteSpace(data.SourcePath))
                {
                    message += $" Source: {data.SourcePath}.";
                }

                Log(message);

                // Save summary log if available
                if (!string.IsNullOrWhiteSpace(data.SummaryLogContent))
                {
                    string summaryOutputFileName = $"pfgamesaves-device-{sanitizedDeviceName}-summary.txt";
                    string summaryOutputPath = Path.Combine(outputDirectory, summaryOutputFileName);

                    await File.WriteAllTextAsync(summaryOutputPath, data.SummaryLogContent, cancellationToken).ConfigureAwait(false);

                    string summaryTruncatedSuffix = data.SummaryTruncated ? " (truncated)" : string.Empty;
                    string summaryMessage = $"GatherLogs saved summary {data.SummaryBytesRead} of {data.SummaryFileSize} bytes from '{deviceLabel}' to '{summaryOutputPath}'{summaryTruncatedSuffix}.";
                    if (!string.IsNullOrWhiteSpace(data.SummarySourcePath))
                    {
                        summaryMessage += $" Source: {data.SummarySourcePath}.";
                    }

                    Log(summaryMessage);
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
                string outputFileName = $"pfgamesaves-failure-{scenarioComponent}-{roleComponent}-{timestamp}-{sanitizedArchiveName}";
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
            ScenarioRunOutcome outcome,
            string scenarioId,
            string? scenarioName,
            int startingStepIndex,
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

                CommandEnvelope envelope = new CommandEnvelope
                {
                    CommandId = Guid.NewGuid().ToString("N"),
                    Command = commandName,
                    TimeoutSeconds = timeoutSeconds,
                    Parameters = NormalizeParameters(step.Parameters),
                    ScenarioContext = new ScenarioContext
                    {
                        ScenarioId = scenarioId,
                        ScenarioName = scenarioName,
                        Role = cleanup.Role,
                        StepIndex = startingStepIndex + cleanupIndex
                    }
                };

                Log($"[cleanup {cleanupIndex}/{total}] Sending '{commandName}' to '{assignment.DisplayName}' (timeout={timeoutSeconds}s).", true);

                try
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

                    ActionResult result = await _commandProcessor.SendCommandAsync(
                        envelope,
                        payload => _transport.SendTextToDeviceAsync(assignment.ClientId, payload),
                        _jsonOptions,
                        TimeSpan.FromSeconds(timeoutSeconds),
                        cancellationToken).ConfigureAwait(false);

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

        private bool TryBuildRoleAssignments(ScenarioPlan plan, out Dictionary<string, DeviceAssignment> assignments)
        {
            assignments = new Dictionary<string, DeviceAssignment>(StringComparer.OrdinalIgnoreCase);
            IReadOnlyCollection<string> required = plan.RequiredDevices;
            if (required.Count == 0)
            {
                return true;
            }

            List<string> orderedRoles = BuildRoleOrder(plan, required);

            IReadOnlyCollection<DeviceConnectionInfo> connectedDetails = _transport.GetConnectedDeviceDetails();

            IReadOnlyList<string> pendingCapabilityDevices = connectedDetails
                .Where(device => device.IsConnected && !device.CapabilitiesReady)
                .Select(device => string.IsNullOrWhiteSpace(device.DisplayName) ? device.ClientId.ToString() : device.DisplayName)
                .ToArray();

            if (pendingCapabilityDevices.Count > 0)
            {
                Log($"Waiting for capability handshake from: {string.Join(", ", pendingCapabilityDevices)}.", true);
            }

            List<DeviceConnectionInfo> available = connectedDetails
                .Where(device => device.IsConnected && device.CapabilitiesReady)
                .OrderBy(device => device.DisplayName, StringComparer.OrdinalIgnoreCase)
                .ToList();

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

            var usedClients = new HashSet<Guid>();
            var lookupByName = available
                .GroupBy(device => device.DisplayName, StringComparer.OrdinalIgnoreCase)
                .ToDictionary(group => group.Key, group => group.First(), StringComparer.OrdinalIgnoreCase);

            foreach (string role in orderedRoles)
            {
                if (string.IsNullOrWhiteSpace(role))
                {
                    continue;
                }

                DeviceConnectionInfo? selected = null;

                if (lookupByName.TryGetValue(role, out DeviceConnectionInfo match) && !usedClients.Contains(match.ClientId))
                {
                    selected = match;
                }

                if (selected == null)
                {
                    selected = available.FirstOrDefault(device => !usedClients.Contains(device.ClientId));
                }

                if (selected == null)
                {
                    Log($"Scenario '{plan.Manifest.Id}' requires role '{role}', but no unassigned device is available.");
                    assignments.Clear();
                    return false;
                }

                assignments[role] = new DeviceAssignment(selected.Value.ClientId, selected.Value.DisplayName);
                usedClients.Add(selected.Value.ClientId);
            }

            if (assignments.Count == 0)
            {
                Log($"Scenario '{plan.Manifest.Id}' requires device assignments but none could be established.");
                return false;
            }

            Log($"Assigned roles: {string.Join(", ", assignments.Select(pair => $"{pair.Key}->{pair.Value.DisplayName}"))}.", true);
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

    internal readonly record struct DeviceAssignment(Guid ClientId, string DisplayName);

    internal enum ControllerCommandType
    {
        None = 0,
        CompareSaveContainerSnapshots
    }

    internal enum ScenarioRunOutcome
    {
        NotStarted,
        Passed,
        Failed
    }
}
