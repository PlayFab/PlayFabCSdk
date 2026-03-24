using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading;
using System.Threading.Tasks;

namespace PFGameSaveTestController
{
    internal sealed class CommandProcessor
    {
    private readonly Action<string, bool, bool> _logger;
    private readonly ConcurrentDictionary<string, TaskCompletionSource<ActionResult>> _pendingResults = new ConcurrentDictionary<string, TaskCompletionSource<ActionResult>>(StringComparer.OrdinalIgnoreCase);
    private readonly ConcurrentDictionary<string, SnapshotCaptureRecord> _snapshotSlots = new ConcurrentDictionary<string, SnapshotCaptureRecord>(StringComparer.OrdinalIgnoreCase);
    private readonly ConcurrentDictionary<string, string> _snapshotSlotAssignments = new ConcurrentDictionary<string, string>(StringComparer.OrdinalIgnoreCase);
        private int _commandCounter;

        public CommandProcessor(Action<string, bool, bool> logger)
        {
            _logger = logger;
        }

        public CommandEnvelope CreateNextCommand()
        {
            int id = Interlocked.Increment(ref _commandCounter);
            string scenarioCommand = (id % 3) switch
            {
                1 => "warmUp",
                2 => "syncState",
                _ => "ping"
            };

            return new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = scenarioCommand,
                TimeoutSeconds = 30
            };
        }

        public async Task<ActionResult> SendCommandAsync(
            CommandEnvelope envelope,
            Func<string, Task> sendTextAsync,
            JsonSerializerOptions serializerOptions,
            TimeSpan timeout,
            CancellationToken cancellationToken)
        {
            if (envelope == null)
            {
                throw new ArgumentNullException(nameof(envelope));
            }

            if (sendTextAsync == null)
            {
                throw new ArgumentNullException(nameof(sendTextAsync));
            }

            if (string.IsNullOrWhiteSpace(envelope.Command))
            {
                throw new ArgumentException("Command name must be provided.", nameof(envelope));
            }

            if (string.IsNullOrEmpty(envelope.CommandId))
            {
                envelope.CommandId = Guid.NewGuid().ToString("N");
            }

            var tcs = new TaskCompletionSource<ActionResult>(TaskCreationOptions.RunContinuationsAsynchronously);
            if (!_pendingResults.TryAdd(envelope.CommandId, tcs))
            {
                throw new InvalidOperationException($"A pending command with id '{envelope.CommandId}' already exists.");
            }

            TimeSpan effectiveTimeout = timeout > TimeSpan.Zero
                ? timeout
                : TimeSpan.FromSeconds(envelope.TimeoutSeconds > 0 ? envelope.TimeoutSeconds : 30);

            string? snapshotSlot = null;
            bool slotRegistered = false;
            if (string.Equals(envelope.Command, "CaptureSaveContainerSnapshot", StringComparison.OrdinalIgnoreCase))
            {
                snapshotSlot = ExtractSnapshotSlot(envelope.Parameters);
                if (snapshotSlot == null)
                {
                    throw new InvalidOperationException("CaptureSaveContainerSnapshot requires a 'slot' parameter with value 'left' or 'right'.");
                }

                _snapshotSlotAssignments[envelope.CommandId] = snapshotSlot;
                slotRegistered = true;
            }

            try
            {
                #pragma warning disable IL2026 // Command payload serialization used only in test controller runtime.
                #pragma warning disable IL3050 // AOT and trimming are not targeted scenarios for test controller.
                string payload = JsonSerializer.Serialize(envelope, serializerOptions);
                #pragma warning restore IL3050
                #pragma warning restore IL2026
                await sendTextAsync(payload).ConfigureAwait(false);
                _logger($"Sent command '{envelope.CommandId}' ({envelope.Command})", true, false);

                return await tcs.Task.WaitAsync(effectiveTimeout, cancellationToken).ConfigureAwait(false);
            }
            catch (TimeoutException)
            {
                _logger($"Command '{envelope.CommandId}' ({envelope.Command}) timed out after {effectiveTimeout.TotalSeconds} seconds.", false, false);
                throw;
            }
            finally
            {
                _pendingResults.TryRemove(envelope.CommandId, out _);
                if (slotRegistered)
                {
                    _snapshotSlotAssignments.TryRemove(envelope.CommandId, out _);
                }
            }
        }

        public bool HandleResult(ActionResult? result)
        {
            if (result == null)
            {
                return false;
            }

            MaybeRecordSnapshot(result);

            bool matched = false;

            if (!string.IsNullOrEmpty(result.CommandId) && _pendingResults.TryGetValue(result.CommandId, out TaskCompletionSource<ActionResult>? pending))
            {
                matched = pending.TrySetResult(result);
            }

            if (!matched)
            {
                LogUnmatchedResult(result);
            }

            return matched;
        }

        public bool TryGetLatestSnapshot(string slot, out SnapshotCaptureRecord? record)
        {
            if (string.IsNullOrWhiteSpace(slot))
            {
                record = null;
                return false;
            }

            return _snapshotSlots.TryGetValue(slot, out record);
        }

        private void LogUnmatchedResult(ActionResult result)
        {
            string status = string.IsNullOrEmpty(result.Status) ? "<none>" : result.Status;
            string hresult = string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!;
            string errorSuffix = string.Empty;
            if (!string.IsNullOrWhiteSpace(result.ErrorMessage))
            {
                string sanitized = result.ErrorMessage.Replace('\r', ' ').Replace('\n', ' ').Trim();
                if (!string.IsNullOrEmpty(sanitized))
                {
                    errorSuffix = $", error=\"{sanitized}\"";
                }
            }

            _logger($"Untracked action result received: commandId={result.CommandId}, command={result.Command}, status={status}, hResult={hresult}, deviceId={result.DeviceId}{errorSuffix}", false, false);
        }

        private static string? ExtractSnapshotSlot(object? parameters)
        {
            if (parameters == null)
            {
                return null;
            }

            if (parameters is IDictionary<string, object?> typedDict)
            {
                if (typedDict.TryGetValue("slot", out object? slotValue))
                {
                    return NormalizeSnapshotSlotValue(slotValue);
                }
            }

            if (parameters is IDictionary dictionary)
            {
                foreach (DictionaryEntry entry in dictionary)
                {
                    string? key = entry.Key?.ToString();
                    if (!string.IsNullOrWhiteSpace(key) && key.Equals("slot", StringComparison.OrdinalIgnoreCase))
                    {
                        return NormalizeSnapshotSlotValue(entry.Value);
                    }
                }
            }

            if (parameters is JsonElement element && element.ValueKind == JsonValueKind.Object)
            {
                if (element.TryGetProperty("slot", out JsonElement slotElement))
                {
                    return NormalizeSnapshotSlotValue(slotElement);
                }
            }

            return NormalizeSnapshotSlotValue(parameters);
        }

        private static string? NormalizeSnapshotSlotValue(object? value)
        {
            if (value == null)
            {
                return null;
            }

            if (value is string stringValue)
            {
                return NormalizeSnapshotSlotString(stringValue);
            }

            if (value is JsonElement element)
            {
                switch (element.ValueKind)
                {
                    case JsonValueKind.String:
                        return NormalizeSnapshotSlotString(element.GetString());
                    case JsonValueKind.Null:
                    case JsonValueKind.Undefined:
                        return null;
                    case JsonValueKind.Object:
                        if (element.TryGetProperty("slot", out JsonElement nestedSlot))
                        {
                            return NormalizeSnapshotSlotValue(nestedSlot);
                        }
                        break;
                    default:
                        string? text = element.ToString();
                        if (!string.IsNullOrWhiteSpace(text))
                        {
                            return NormalizeSnapshotSlotString(text);
                        }
                        break;
                }
            }

            return NormalizeSnapshotSlotString(value.ToString());
        }

        private static string? NormalizeSnapshotSlotString(string? slot)
        {
            if (string.IsNullOrWhiteSpace(slot))
            {
                return null;
            }

            string trimmed = slot.Trim();
            if (trimmed.Equals("left", StringComparison.OrdinalIgnoreCase))
            {
                return "left";
            }

            if (trimmed.Equals("right", StringComparison.OrdinalIgnoreCase))
            {
                return "right";
            }

            return null;
        }

        private void MaybeRecordSnapshot(ActionResult result)
        {
            if (!string.Equals(result.Command, "CaptureSaveContainerSnapshot", StringComparison.OrdinalIgnoreCase))
            {
                return;
            }

            if (!_snapshotSlotAssignments.TryRemove(result.CommandId, out string? slot) || string.IsNullOrWhiteSpace(slot))
            {
                _logger($"Snapshot result for command '{result.CommandId}' arrived without a recorded slot; ignoring entry.", false, false);
                return;
            }

            if (!string.Equals(result.Status, "succeeded", StringComparison.OrdinalIgnoreCase))
            {
                string deviceId = string.IsNullOrWhiteSpace(result.DeviceId) ? "<unknown>" : result.DeviceId;
                _logger($"Snapshot result for slot '{slot}' from device '{deviceId}' did not succeed; skipping cache update.", false, false);
                return;
            }

            var record = new SnapshotCaptureRecord(
                slot,
                result.DeviceId,
                result.CommandId,
                result.Status,
                result.HResult,
                result.Timestamp,
                result.RawJson);

            _snapshotSlots[slot] = record;
            string sourceDevice = string.IsNullOrWhiteSpace(result.DeviceId) ? "<unknown>" : result.DeviceId;
            _logger($"Stored CaptureSaveContainerSnapshot result in slot '{slot}' from device '{sourceDevice}'.", true, false);
        }
    }

    internal sealed class CommandEnvelope
    {
        public string CommandId { get; set; } = string.Empty;

        public string Command { get; set; } = string.Empty;

        public int TimeoutSeconds { get; set; }

        public object? Parameters { get; set; }

        public ScenarioContext? ScenarioContext { get; set; }
    }

    internal sealed class ScenarioContext
    {
        public string? ScenarioId { get; set; }

        public string? ScenarioName { get; set; }

        public string? Role { get; set; }

        public int? StepIndex { get; set; }
    }

    internal sealed class ActionResult
    {
        public string Type { get; set; } = string.Empty;

        public string CommandId { get; set; } = string.Empty;

        public string DeviceId { get; set; } = string.Empty;

        public string Status { get; set; } = string.Empty;

        public int ElapsedMs { get; set; }

        public DateTimeOffset Timestamp { get; set; }

        public string Command { get; set; } = string.Empty;

        [JsonPropertyName("hresult")]
        public string? HResult { get; set; }

        public string? ErrorMessage { get; set; }

        public string RawJson { get; set; } = string.Empty;
    }

    internal sealed record SnapshotCaptureRecord(
        string Slot,
        string DeviceId,
        string CommandId,
        string Status,
        string? HResult,
        DateTimeOffset Timestamp,
        string RawJson)
    {
        public string DisplayName => string.IsNullOrWhiteSpace(DeviceId)
            ? Slot
            : $"{Slot} ({DeviceId})";
    }
}
