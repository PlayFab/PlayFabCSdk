using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading;
using System.Threading.Tasks;

namespace GameTestController
{
    internal sealed class ControllerRuntime
    {
        private readonly Action<string, bool, bool> _logger;
        private readonly ControllerTransport _transport;
        private readonly CommandProcessor _commandProcessor;
        private readonly ActionResultHandler _actionResultHandler;
        private readonly ScenarioManifestService _scenarioService;
    private readonly ScenarioRunner _scenarioRunner;
    private readonly Func<ChaosModeScenarioParameters>? _chaosSettingsProvider;
    private readonly Func<string?>? _sourceDataFolderProvider;
        private string? _defaultLogDirectory;
        private readonly JsonSerializerOptions _jsonOptions = new JsonSerializerOptions
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull,
            WriteIndented = false
        };

        // Xbox auto-launch session state: deploy once, launch per-scenario
        private string? _xboxConsoleAddress;
        private string? _xboxLayoutDir;
        private bool _xboxDeployed;

        /// <summary>
        /// When non-empty, only these engines are allowed for auto-launch.
        /// Auto-launch will skip any engine not in this list (e.g., "xbox" when only "pc-grts" is allowed).
        /// </summary>
        public List<string> AllowedEngines { get; set; } = new List<string>();

        // Tracks whether we already waited for a device to reconnect and it didn't.
        // Once exhausted, skip immediately instead of waiting 15s per scenario.
        private bool _deviceReconnectExhausted;
        public event Action<IReadOnlyCollection<DeviceConnectionInfo>>? ConnectedDevicesChanged;

        public ControllerTransport Transport => _transport;

        public ControllerRuntime(Action<string, bool, bool> logger, Func<ChaosModeScenarioParameters>? chaosSettingsProvider = null, Func<string?>? sourceDataFolderProvider = null, int devicePort = 15080, int agentPort = 15081)
        {
            _logger = logger;
            _chaosSettingsProvider = chaosSettingsProvider;
            _sourceDataFolderProvider = sourceDataFolderProvider;
            _transport = new ControllerTransport(Log, devicePort, agentPort);
            _transport.TextReceived += OnTextMessageReceived;
            _transport.BinaryReceived += OnBinaryMessageReceived;
            _transport.ConnectedDevicesChanged += devices => ConnectedDevicesChanged?.Invoke(devices);
            _commandProcessor = new CommandProcessor(Log);
            _actionResultHandler = new ActionResultHandler(_commandProcessor, Log);
            _scenarioService = new ScenarioManifestService(Log);
            _scenarioRunner = new ScenarioRunner(_commandProcessor, _transport, _jsonOptions, Log, _chaosSettingsProvider, () => _defaultLogDirectory, _sourceDataFolderProvider);
        }

        public async Task StartAsync()
        {
            try
            {
                await _transport.StartAsync();
            }
            catch (HttpListenerException ex) when (ex.Message.Contains("Access is denied"))
            {
                Log($"WebSocket server failed: {ex.Message}");
                Log("To fix, either:");
                Log("  1. Run as Administrator, OR");
                Log("  2. Register URL ACL (run as Admin once): netsh http add urlacl url=http://+:15080/ws/ user=Everyone");
            }
            catch (Exception ex)
            {
                Log($"WebSocket server failed: {ex.Message}");
            }
        }

        public Task StopAsync()
        {
            return _transport.StopAsync();
        }

        /// <summary>
        /// Sends killTestApp to all connected agents (cleanup between scenarios or on exit).
        /// </summary>
        public Task KillAllAgentTestAppsAsync()
        {
            return _transport.KillAllAgentTestAppsAsync();
        }

        /// <summary>
        /// Waits for the specified number of remote DeviceAgents to connect (without launching apps).
        /// </summary>
        public Task<bool> WaitForAgentsAsync(int requiredCount, int timeoutSeconds)
        {
            return _transport.WaitForAgentsAsync(requiredCount, timeoutSeconds);
        }

        public IReadOnlyCollection<DeviceConnectionInfo> GetConnectedDevices()
        {
            return _transport.GetConnectedDeviceDetails();
        }

        public async Task<string> LaunchNextLocalDeviceAsync(TimeSpan? timeout = null)
        {
            string? role = GetNextAvailableLocalDeviceRole();
            if (role == null)
            {
                throw new InvalidOperationException("DeviceA and DeviceB are already connected. Disconnect a device before launching another local device.");
            }

            await LaunchLocalDeviceAsync(role, timeout ?? TimeSpan.FromSeconds(30)).ConfigureAwait(false);
            return role;
        }

        public void UpdateDefaultLogDirectory(string? path)
        {
            if (string.IsNullOrWhiteSpace(path))
            {
                _defaultLogDirectory = null;
                return;
            }

            try
            {
                Directory.CreateDirectory(path);
                _defaultLogDirectory = path;
            }
            catch (Exception ex)
            {
                Log($"Failed to set default log directory '{path}': {ex.Message}");
                _defaultLogDirectory = null;
            }
        }

        public async Task SendTextAsync()
        {
            try
            {
                await _transport.BroadcastTextAsync("hello");
                Log("Broadcast text 'hello' to connected devices");
            }
            catch (Exception ex)
            {
                Log($"Send text failed: {ex.Message}");
            }
        }

        public async Task SendBinaryAsync()
        {
            try
            {
                byte[] payload = new byte[10];
                for (int i = 0; i < payload.Length; i++)
                {
                    payload[i] = (byte)(i + 1);
                }

                await _transport.BroadcastBinaryAsync(payload);
                Log("Broadcast binary message (10 bytes)");
            }
            catch (Exception ex)
            {
                Log($"Send binary failed: {ex.Message}");
            }
        }

        public async Task SendCommandAsync()
        {
            CommandEnvelope envelope = _commandProcessor.CreateNextCommand();
            int manualTimeoutSeconds = envelope.TimeoutSeconds > 0 ? envelope.TimeoutSeconds : 30;
            try
            {
                ActionResult result = await _commandProcessor.SendCommandAsync(
                    envelope,
                    payload => _transport.BroadcastTextAsync(payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(manualTimeoutSeconds),
                    CancellationToken.None).ConfigureAwait(false);

                Log($"Manual command '{envelope.Command}' completed with status {result.Status}.");
            }
            catch (TimeoutException)
            {
                Log($"Manual command '{envelope.Command}' timed out after {manualTimeoutSeconds} seconds.");
            }
            catch (Exception ex)
            {
                Log($"Send command failed: {ex.Message}");
            }
        }

        public async Task<ActionResult> SendManualCommandAsync(string deviceIdentifier, string commandName, Dictionary<string, object>? customParameters = null)
        {
            if (string.IsNullOrWhiteSpace(deviceIdentifier))
            {
                throw new ArgumentException("Device identifier is required.", nameof(deviceIdentifier));
            }

            if (string.IsNullOrWhiteSpace(commandName))
            {
                throw new ArgumentException("Command name is required.", nameof(commandName));
            }

            // Find the target device
            var devices = _transport.GetConnectedDeviceDetails();
            DeviceConnectionInfo? targetDevice = devices.FirstOrDefault(d => 
                d.ClientId.ToString() == deviceIdentifier || 
                d.DisplayName == deviceIdentifier);

            if (targetDevice == null)
            {
                return new ActionResult
                {
                    Command = commandName,
                    Status = "Error",
                    ErrorMessage = $"Device '{deviceIdentifier}' not found."
                };
            }

            // Create command envelope with custom or default parameters
            var envelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = commandName,
                TimeoutSeconds = 180,
                Parameters = customParameters ?? GetDefaultParametersForCommand(commandName)
            };

            int timeoutSeconds = envelope.TimeoutSeconds > 0 ? envelope.TimeoutSeconds : 180;

            try
            {
                ActionResult result = await _commandProcessor.SendCommandAsync(
                    envelope,
                    payload => _transport.SendTextToDeviceAsync(targetDevice.Value.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(timeoutSeconds),
                    CancellationToken.None).ConfigureAwait(false);

                return result;
            }
            catch (TimeoutException)
            {
                return new ActionResult
                {
                    Command = commandName,
                    Status = "Error",
                    ErrorMessage = $"Command timed out after {timeoutSeconds} seconds."
                };
            }
            catch (Exception ex)
            {
                return new ActionResult
                {
                    Command = commandName,
                    Status = "Error",
                    ErrorMessage = ex.Message
                };
            }
        }

        private Dictionary<string, object>? GetDefaultParametersForCommand(string commandName)
        {
            return commandName switch
            {
                "PFServiceConfigCreateHandle" => new Dictionary<string, object>
                {
                    ["endpoint"] = "https://E18D7.playfabapi.com",
                    ["titleId"] = "E18D7"
                },
                "XTaskQueueCreate" => new Dictionary<string, object>
                {
                    ["workMode"] = "ThreadPool",
                    ["completionMode"] = "ThreadPool",
                    ["setAsProcessQueue"] = true
                },
                "PFGameSaveFilesInitialize" => new Dictionary<string, object>
                {
                    ["saveFolder"] = "C:\\gamesave"
                },
                "PFGameSaveFilesSetUiCallbacks" => new Dictionary<string, object>
                {
                    ["enable"] = true
                },
                "PFLocalUserCreateHandleWithPersistedLocalId" => new Dictionary<string, object>
                {
                    ["persistedLocalId"] = "ManualApiUser",
                    ["customId"] = "ManualApiUser",
                    ["createAccount"] = true
                },
                "WriteGameSaveData" => new Dictionary<string, object>
                {
                    ["operations"] = new[]
                    {
                        new Dictionary<string, object>
                        {
                            ["verb"] = "CreateBinaryFile",
                            ["relativePath"] = "test/data.bin",
                            ["bytes"] = 1024,
                            ["pattern"] = new[] { 0xAA, 0xBB, 0xCC, 0xDD }
                        }
                    }
                },
                "PFGameSaveFilesUploadWithUiAsync" => new Dictionary<string, object>
                {
                    ["mode"] = "ReleaseDeviceAsActive"
                },
                "CaptureSaveContainerSnapshot" => new Dictionary<string, object>
                {
                    ["slot"] = "left"
                },
                "DeleteSaveRoot" => new Dictionary<string, object>
                {
                    ["preserveManifest"] = false
                },
                "XblInitialize" => new Dictionary<string, object>
                {
                    ["scid"] = "00000000-0000-0000-0000-000076029b4d"
                },
                _ => null
            };
        }

        public async Task ResetDevicesAsync()
        {
            try
            {
                await _transport.ResetDeviceAssignmentsAsync();
                Log("Device assignments reset");
            }
            catch (Exception ex)
            {
                Log($"Device reset failed: {ex.Message}");
            }
        }

        public async Task GatherLogsAsync(string logDirectory, CancellationToken cancellationToken = default)
        {
            if (string.IsNullOrWhiteSpace(logDirectory))
            {
                throw new ArgumentException("Log directory must be provided.", nameof(logDirectory));
            }

            Directory.CreateDirectory(logDirectory);
            _defaultLogDirectory = logDirectory;

            IReadOnlyCollection<DeviceConnectionInfo> devices = _transport.GetConnectedDeviceDetails();
            if (devices == null || devices.Count == 0)
            {
                Log("GatherLogs requested, but no devices are connected.");
                return;
            }

            foreach (DeviceConnectionInfo device in devices)
            {
                cancellationToken.ThrowIfCancellationRequested();

                if (!device.IsConnected)
                {
                    string disconnectedName = string.IsNullOrWhiteSpace(device.DisplayName)
                        ? device.ClientId.ToString("N")
                        : device.DisplayName;
                    Log($"Skipping log collection for '{disconnectedName}' because the device is not connected.", true);
                    continue;
                }

                await GatherLogsForDeviceAsync(device, logDirectory, cancellationToken).ConfigureAwait(false);
            }
        }

        public async Task GatherSnapshotsAsync(string outputDirectory, CancellationToken cancellationToken = default)
        {
            if (string.IsNullOrWhiteSpace(outputDirectory))
            {
                throw new ArgumentException("Output directory must be provided.", nameof(outputDirectory));
            }

            Directory.CreateDirectory(outputDirectory);
            _defaultLogDirectory = outputDirectory;

            IReadOnlyCollection<DeviceConnectionInfo> devices = _transport.GetConnectedDeviceDetails();
            if (devices == null || devices.Count == 0)
            {
                Log("GatherSnapshots requested, but no devices are connected.");
                return;
            }

            foreach (DeviceConnectionInfo device in devices)
            {
                cancellationToken.ThrowIfCancellationRequested();

                if (!device.IsConnected)
                {
                    string disconnectedName = string.IsNullOrWhiteSpace(device.DisplayName)
                        ? device.ClientId.ToString("N")
                        : device.DisplayName;
                    Log($"Skipping snapshot collection for '{disconnectedName}' because the device is not connected.", true);
                    continue;
                }

                await GatherSnapshotForDeviceAsync(device, outputDirectory, cancellationToken).ConfigureAwait(false);
            }
        }

        public async Task<ScenarioRunOutcome> LoadScenarioAsync(string path, bool autoLaunchLocalDevices = false, string? customIdPrefix = null)
        {
            try
            {
                string? effectivePrefix = string.IsNullOrWhiteSpace(customIdPrefix) ? null : customIdPrefix.Trim();
                ScenarioPlan? plan = await _scenarioService.LoadAsync(path).ConfigureAwait(false);
                if (plan == null)
                {
                    return ScenarioRunOutcome.NotStarted;
                }

                if (autoLaunchLocalDevices)
                {
                    // Reset reconnect-exhausted flag so a fresh auto-launch gets
                    // a fresh chance (the flag is for batch-run cascade prevention,
                    // not for blocking individual manual runs).
                    _deviceReconnectExhausted = false;

                    // Auto-launch handles both PC and Xbox devices, including
                    // deploying/launching the Xbox app when an engine constraint
                    // requires it. This must run before the engine compatibility
                    // check so that auto-launched Xbox devices are available.
                    bool devicesReady = await EnsureRequiredDevicesAsync(plan).ConfigureAwait(false);
                    if (!devicesReady)
                    {
                        Log("Auto-launch failed to prepare required devices.");
                    }
                }

                // After auto-launch attempt, check if engine constraints are met.
                // If auto-launch is disabled (--no-auto-launch), this is the first
                // check and will skip scenarios that need unavailable engines.
                // If the check fails, wait briefly for devices to reconnect — a
                // WebSocket drop between scenarios should not cascade-skip the
                // entire remaining run. However, if we already waited once and the
                // device didn't come back, skip immediately on subsequent checks.
                string? engineSkipReason = CheckEngineCompatibility(plan);
                if (engineSkipReason != null)
                {
                    if (!_deviceReconnectExhausted)
                    {
                        // Wait up to 15s for a recently-disconnected device to reconnect
                        // and re-announce capabilities before giving up.
                        Log("Engine check failed, waiting up to 15s for device reconnection...");
                        for (int waited = 0; waited < 15; waited++)
                        {
                            await Task.Delay(1000).ConfigureAwait(false);
                            engineSkipReason = CheckEngineCompatibility(plan);
                            if (engineSkipReason == null)
                            {
                                Log("Device reconnected — engine check passed.");
                                break;
                            }
                        }

                        if (engineSkipReason != null)
                        {
                            // Device didn't come back — don't wait again for subsequent scenarios
                            _deviceReconnectExhausted = true;
                            Log($"Device did not reconnect within 15s — skipping this and remaining incompatible scenarios immediately.");
                        }
                    }

                    if (engineSkipReason != null)
                    {
                        _scenarioRunner.LastSkipReason = engineSkipReason;
                        return ScenarioRunOutcome.NotStarted;
                    }
                }

                var outcome = await _scenarioRunner.RunAsync(plan, effectivePrefix).ConfigureAwait(false);

                // When running a single scenario (not part of a batch run),
                // gather logs here since there is no outer RunScenariosAsync to
                // do it.  The ScenarioRunner only gathers on failure, so
                // successful single-scenario runs would otherwise lose logs.
                if (outcome == ScenarioRunOutcome.Passed && !string.IsNullOrWhiteSpace(_defaultLogDirectory))
                {
                    try
                    {
                        await GatherLogsAsync(_defaultLogDirectory!).ConfigureAwait(false);
                    }
                    catch (Exception ex)
                    {
                        Log($"Post-scenario log gathering failed: {ex.Message}");
                    }
                }

                return outcome;
            }
            catch (Exception ex)
            {
                Log($"Scenario load failed: {ex.Message}");
                return ScenarioRunOutcome.Failed;
            }
        }


        private void OnTextMessageReceived(string deviceName, string message)
        {
            if (_actionResultHandler.TryHandle(deviceName, message))
            {
                return;
            }

            if (TryHandleDeviceLog(deviceName, message))
            {
                return;
            }

            Log($"[{deviceName}] WebSocket Text: {message}", verbose: true);
        }

        /// <summary>
        /// Handles {"type":"deviceLog","message":"..."} messages pushed from the device.
        /// The device decides what to log; the controller just displays it.
        /// </summary>
        private bool TryHandleDeviceLog(string deviceName, string message)
        {
            if (string.IsNullOrWhiteSpace(message) || message[0] != '{')
            {
                return false;
            }

            try
            {
                using JsonDocument doc = JsonDocument.Parse(message);
                JsonElement root = doc.RootElement;

                if (!root.TryGetProperty("type", out JsonElement typeEl) ||
                    typeEl.ValueKind != JsonValueKind.String ||
                    !string.Equals(typeEl.GetString(), "deviceLog", StringComparison.OrdinalIgnoreCase))
                {
                    return false;
                }

                string text = root.TryGetProperty("message", out JsonElement msgEl) && msgEl.ValueKind == JsonValueKind.String
                    ? msgEl.GetString() ?? ""
                    : "";

                Log($"[{deviceName}] {text}");
                return true;
            }
            catch (JsonException)
            {
                return false;
            }
        }

        private void OnBinaryMessageReceived(string deviceName, byte[] payload)
        {
            Log(FormatBinaryPreview(deviceName, payload), verbose: true);
        }

        private static string FormatBinaryPreview(string deviceName, byte[] payload)
        {
            if (payload == null || payload.Length == 0)
            {
                return $"[{deviceName}] WebSocket Binary: size=0";
            }

            const int previewCount = 8;
            var sb = new StringBuilder();
            sb.Append($"[{deviceName}] WebSocket Binary: size={payload.Length} bytes=[");
            int count = Math.Min(payload.Length, previewCount);
            for (int i = 0; i < count; i++)
            {
                if (i > 0)
                {
                    sb.Append(' ');
                }
                sb.Append(payload[i].ToString("X2"));
            }
            if (payload.Length > previewCount)
            {
                sb.Append(" ...");
            }
            sb.Append(']');
            return sb.ToString();
        }

        private void Log(string message, bool verbose = false, bool chaosLog = false)
        {
            _logger(message, verbose, chaosLog);
        }

        private async Task GatherLogsForDeviceAsync(DeviceConnectionInfo device, string logDirectory, CancellationToken cancellationToken)
        {
            string deviceName = string.IsNullOrWhiteSpace(device.DisplayName)
                ? device.ClientId.ToString("N")
                : device.DisplayName;

            Log($"Requesting logs from '{deviceName}'.");

            var envelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "GatherLogs",
                TimeoutSeconds = 15
            };

            try
            {
                ActionResult result = await _commandProcessor.SendCommandAsync(
                    envelope,
                    payload => _transport.SendTextToDeviceAsync(device.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(envelope.TimeoutSeconds),
                    cancellationToken).ConfigureAwait(false);

                if (!string.Equals(result.Status, "succeeded", StringComparison.OrdinalIgnoreCase))
                {
                    string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                    string hrText = string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim();
                    string errorSuffix = string.IsNullOrWhiteSpace(result.ErrorMessage)
                        ? string.Empty
                        : $" Error: {result.ErrorMessage.Trim()}";
                    Log($"GatherLogs for '{deviceName}' failed with status '{statusText}' (hr={hrText}).{errorSuffix}");
                    return;
                }

                if (!TryExtractGatherLogsMetadata(result.RawJson, out GatherLogsMetadata metadata))
                {
                    Log($"GatherLogs for '{deviceName}' returned an unexpected payload; no log was saved.");
                    return;
                }

                string sanitizedDeviceName = SanitizeFileComponent(deviceName, "device");

                // Fetch and save main log in chunks
                if (metadata.BytesToTransfer > 0 && !string.IsNullOrWhiteSpace(metadata.LogPath))
                {
                    // Derive fetched filename from source path to preserve timestamped names
                    string sourceFileName = Path.GetFileName(metadata.LogPath);
                    string outputFileName = sourceFileName.EndsWith("-log.txt", StringComparison.OrdinalIgnoreCase)
                        ? sourceFileName.Replace("-log.txt", "-fetched-log.txt")
                        : $"device-{sanitizedDeviceName}-fetched-log.txt";
                    string outputPath = Path.Combine(logDirectory, outputFileName);

                    await FetchLogChunksAsync(device, metadata.LogPath, metadata.FileStartOffset, metadata.BytesToTransfer, outputPath, cancellationToken).ConfigureAwait(false);

                    string truncatedSuffix = metadata.Truncated ? " (truncated)" : string.Empty;
                    string message = $"GatherLogs saved {metadata.BytesToTransfer} of {metadata.FileSize} bytes from '{deviceName}' to '{outputPath}'{truncatedSuffix}.";
                    if (!string.IsNullOrWhiteSpace(metadata.LogPath))
                    {
                        message += $" Source: {metadata.LogPath}.";
                    }

                    Log(message);
                }

                // Fetch and save summary log in chunks
                if (metadata.SummaryBytesToTransfer > 0 && !string.IsNullOrWhiteSpace(metadata.SummaryLogPath))
                {
                    // Derive fetched filename from source path to preserve timestamped names
                    string summarySourceFileName = Path.GetFileName(metadata.SummaryLogPath);
                    string summaryOutputFileName = summarySourceFileName.EndsWith("-summary.txt", StringComparison.OrdinalIgnoreCase)
                        ? summarySourceFileName.Replace("-summary.txt", "-fetched-summary.txt")
                        : $"device-{sanitizedDeviceName}-fetched-summary.txt";
                    string summaryOutputPath = Path.Combine(logDirectory, summaryOutputFileName);

                    await FetchLogChunksAsync(device, metadata.SummaryLogPath, metadata.SummaryFileStartOffset, metadata.SummaryBytesToTransfer, summaryOutputPath, cancellationToken).ConfigureAwait(false);

                    string summaryTruncatedSuffix = metadata.SummaryTruncated ? " (truncated)" : string.Empty;
                    string summaryMessage = $"GatherLogs saved summary {metadata.SummaryBytesToTransfer} of {metadata.SummaryFileSize} bytes from '{deviceName}' to '{summaryOutputPath}'{summaryTruncatedSuffix}.";
                    if (!string.IsNullOrWhiteSpace(metadata.SummaryLogPath))
                    {
                        summaryMessage += $" Source: {metadata.SummaryLogPath}.";
                    }

                    Log(summaryMessage);
                }
            }
            catch (TimeoutException)
            {
                Log($"GatherLogs for '{deviceName}' timed out.");
            }
            catch (OperationCanceledException)
            {
                Log("GatherLogs operation canceled.");
                throw;
            }
            catch (Exception ex)
            {
                Log($"GatherLogs for '{deviceName}' failed: {ex.Message}");
            }
        }

        private async Task FetchLogChunksAsync(
            DeviceConnectionInfo device,
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
                    payload => _transport.SendTextToDeviceAsync(device.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(chunkTimeoutSeconds),
                    cancellationToken).ConfigureAwait(false);

                if (!string.Equals(chunkResult.Status, "succeeded", StringComparison.OrdinalIgnoreCase))
                {
                    string hrText = string.IsNullOrWhiteSpace(chunkResult.HResult) ? "<none>" : chunkResult.HResult!.Trim();
                    throw new InvalidOperationException($"GatherLogsChunk failed at offset {offset} (hr={hrText}).");
                }

                if (!TryExtractGatherLogsChunkData(chunkResult.RawJson, out string chunkContent, out long bytesRead))
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

        private async Task GatherSnapshotForDeviceAsync(DeviceConnectionInfo device, string outputDirectory, CancellationToken cancellationToken)
        {
            string deviceName = string.IsNullOrWhiteSpace(device.DisplayName)
                ? device.ClientId.ToString("N")
                : device.DisplayName;

            Log($"Requesting snapshot from '{deviceName}'.");

            var envelope = new CommandEnvelope
            {
                CommandId = Guid.NewGuid().ToString("N"),
                Command = "GatherSnapshot",
                TimeoutSeconds = 45
            };

            try
            {
                ActionResult result = await _commandProcessor.SendCommandAsync(
                    envelope,
                    payload => _transport.SendTextToDeviceAsync(device.ClientId, payload),
                    _jsonOptions,
                    TimeSpan.FromSeconds(envelope.TimeoutSeconds),
                    cancellationToken).ConfigureAwait(false);

                if (!string.Equals(result.Status, "succeeded", StringComparison.OrdinalIgnoreCase))
                {
                    string statusText = string.IsNullOrWhiteSpace(result.Status) ? "<none>" : result.Status;
                    string hrText = string.IsNullOrWhiteSpace(result.HResult) ? "<none>" : result.HResult!.Trim();
                    string errorSuffix = string.IsNullOrWhiteSpace(result.ErrorMessage)
                        ? string.Empty
                        : $" Error: {result.ErrorMessage.Trim()}";
                    Log($"GatherSnapshot for '{deviceName}' failed with status '{statusText}' (hr={hrText}).{errorSuffix}");
                    return;
                }

                if (!TryExtractGatheredSnapshotData(result.RawJson, out GatheredSnapshotData data))
                {
                    Log($"GatherSnapshot for '{deviceName}' returned an unexpected payload; no archive was saved.");
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
                        Log($"GatherSnapshot for '{deviceName}' returned invalid base64 content; no archive was saved.");
                        return;
                    }
                }

                string sanitizedDeviceName = SanitizeFileComponent(deviceName, "device");
                string sanitizedArchiveName = SanitizeFileComponent(data.ArchiveFileName, "snapshot.zip");
                if (!sanitizedArchiveName.EndsWith(".zip", StringComparison.OrdinalIgnoreCase))
                {
                    sanitizedArchiveName += ".zip";
                }

                string outputFileName = $"snapshot-{sanitizedDeviceName}-{sanitizedArchiveName}";
                string outputPath = Path.Combine(outputDirectory, outputFileName);

                await File.WriteAllBytesAsync(outputPath, archiveBytes, cancellationToken).ConfigureAwait(false);

                string truncatedSuffix = data.Truncated ? " (truncated)" : string.Empty;
                string message = $"GatherSnapshot saved {archiveBytes.LongLength} of {data.ArchiveSize} bytes from '{deviceName}' to '{outputPath}'{truncatedSuffix}.";
                if (!string.IsNullOrWhiteSpace(data.SaveFolder))
                {
                    message += $" Source folder: {data.SaveFolder}.";
                }

                Log(message);
            }
            catch (TimeoutException)
            {
                Log($"GatherSnapshot for '{deviceName}' timed out.");
            }
            catch (OperationCanceledException)
            {
                Log("GatherSnapshot operation canceled.");
                throw;
            }
            catch (Exception ex)
            {
                Log($"GatherSnapshot for '{deviceName}' failed: {ex.Message}");
            }
        }

        private async Task<bool> EnsureRequiredDevicesAsync(ScenarioPlan plan)
        {
            IReadOnlyCollection<string> required = plan.RequiredDevices;
            if (required == null || required.Count == 0)
            {
                return true;
            }

            // Determine how many Xbox users this scenario needs (default 1).
            // Check all device specs for the maximum xboxUsers prerequisite.
            int requiredXboxUsers = 1;
            foreach (var kvp in plan.Manifest.Devices)
            {
                if (kvp.Value?.Prerequisites?.XboxUsers is int xu && xu > requiredXboxUsers)
                {
                    requiredXboxUsers = xu;
                }
            }

            // Build launch order sorted by role name (DeviceA before DeviceB).
            // The WebSocket server assigns names FCFS — first connection gets "DeviceA",
            // second gets "DeviceB". Launch order MUST match this naming convention
            // regardless of the scenario's execution order.
            List<string> launchOrder = BuildLaunchOrder(required);
            if (launchOrder.Count == 0)
            {
                return true;
            }

            // Log the pre-planned launch order with engine requirements for diagnostics.
            foreach (string role in launchOrder)
            {
                string engineNote = "";
                if (plan.Manifest.Devices.TryGetValue(role, out ScenarioDevice? spec) && spec != null && spec.HasEngineConstraint)
                {
                    engineNote = $" (requires engine: {spec.EngineDisplay})";
                }
                Log($"Auto-launch plan: {role}{engineNote}", true);
            }

            // Preflight: check for impossible constraints before launching anything.
            // If a role requires Xbox-only and no Xbox is available, fail fast.
            foreach (string role in launchOrder)
            {
                if (!plan.Manifest.Devices.TryGetValue(role, out ScenarioDevice? spec) || spec == null || !spec.HasEngineConstraint)
                {
                    continue;
                }

                bool hasXbox = spec.EngineMatches("xbox");
                bool hasLocal = spec.EngineMatches("pc-grts") || spec.EngineMatches("pc-inproc") || spec.EngineMatches("pc-inproc-gamesaves");
                bool xboxAllowed = AllowedEngines.Count == 0 || AllowedEngines.Any(e => e.Equals("xbox", StringComparison.OrdinalIgnoreCase));

                if (hasXbox && !hasLocal && !xboxAllowed)
                {
                    Log($"Auto-launch failed: role '{role}' requires engine 'xbox' but Xbox is not in allowed engines.");
                    return false;
                }

                if (hasXbox && !hasLocal && xboxAllowed)
                {
                    // Xbox-only constraint — EnsureXboxDeviceAsync will discover the console,
                    // deploy, prepare users, and launch. If it fails, the scenario will be skipped.
                }
            }

            // Launch devices in name-sorted order to match WebSocket server FCFS naming.
            // Track whether Xbox has been assigned to a role this scenario.
            // Xbox can only run one app instance — if a second role also accepts Xbox,
            // it must fall back to PC to avoid killing the first role's app.
            bool xboxAssignedToRole = false;

            foreach (string role in launchOrder)
            {
                plan.Manifest.Devices.TryGetValue(role, out ScenarioDevice? spec);
                bool isConstrained = spec?.HasEngineConstraint ?? false;

                try
                {
                    if (IsDeviceSessionPresent(role))
                    {
                        Log($"Device '{role}' already connected.", true);
                        // Check if this device is on Xbox (for tracking purposes)
                        if (IsXboxDeviceReadyForRole(role))
                        {
                            xboxAssignedToRole = true;
                        }
                        continue;
                    }

                    if (isConstrained)
                    {
                        bool hasXbox = spec!.EngineMatches("xbox");
                        bool hasLocal = spec.EngineMatches("pc-grts") || spec.EngineMatches("pc-inproc") || spec.EngineMatches("pc-inproc-gamesaves");

                        // Apply allowed-engines filter: suppress Xbox if not in allowed list
                        bool xboxAllowed = AllowedEngines.Count == 0 || AllowedEngines.Any(e => e.Equals("xbox", StringComparison.OrdinalIgnoreCase));

                        if (hasXbox && !hasLocal && !xboxAllowed)
                        {
                            Log($"Auto-launch skipped for role '{role}': requires 'xbox' but Xbox is not in allowed engines.");
                            return false;
                        }

                        if (hasXbox && !hasLocal && xboxAllowed)
                        {
                            // Xbox-only constraint
                            await EnsureXboxDeviceAsync(role, TimeSpan.FromSeconds(60), requiredXboxUsers).ConfigureAwait(false);
                            xboxAssignedToRole = true;
                        }
                        else if (hasXbox && hasLocal)
                        {
                            // Accepts Xbox or local — prefer Xbox if available, but only
                            // if Xbox hasn't already been assigned to another role.
                            bool xboxLaunched = false;
                            if (!xboxAssignedToRole)
                            {
                                if (IsXboxDeviceReady() || XboxDeviceLauncher.GetDefaultConsoleAddress() != null)
                                {
                                    // Xbox is reachable — launch on Xbox (no fallback on failure)
                                    await EnsureXboxDeviceAsync(role, TimeSpan.FromSeconds(60), requiredXboxUsers).ConfigureAwait(false);
                                    xboxLaunched = true;
                                    xboxAssignedToRole = true;
                                }
                            }
                            else
                            {
                                Log($"Xbox already assigned to another role; using local device for '{role}'.");
                            }

                            if (!xboxLaunched)
                            {
                                await LaunchLocalDeviceAsync(role, TimeSpan.FromSeconds(30), spec.RequiresInproc).ConfigureAwait(false);
                            }
                        }
                        else
                        {
                            // Local-only constraint (pc-grts, pc-inproc, etc.)
                            await LaunchLocalDeviceAsync(role, TimeSpan.FromSeconds(30), spec.RequiresInproc).ConfigureAwait(false);
                        }
                    }
                    else
                    {
                        // Unconstrained — launch local device
                        await LaunchLocalDeviceAsync(role, TimeSpan.FromSeconds(30)).ConfigureAwait(false);
                    }
                }
                catch (Exception ex)
                {
                    Log(ex.Message);
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Ensures an Xbox device is connected and ready for the given role.
        /// If no Xbox is connected, attempts to discover the default console,
        /// deploy the app (once per session), and launch it.
        /// </summary>
        private async Task EnsureXboxDeviceAsync(string role, TimeSpan timeout, int requiredXboxUsers = 1)
        {
            // If an Xbox device for this specific role is already connected with capabilities, we're done
            if (IsXboxDeviceReadyForRole(role))
            {
                Log($"Xbox device already connected for role '{role}'.", true);
                return;
            }

            // Discover the default console address (cached for session, with retry)
            if (_xboxConsoleAddress == null)
            {
                for (int attempt = 0; attempt < 6; attempt++)
                {
                    _xboxConsoleAddress = XboxDeviceLauncher.GetDefaultConsoleAddress();
                    if (_xboxConsoleAddress != null) break;

                    if (attempt == 0)
                        Log("Xbox auto-launch: no default console found, retrying (console may be booting)...");
                    System.Threading.Thread.Sleep(5000);
                }

                if (_xboxConsoleAddress == null)
                {
                    throw new InvalidOperationException(
                        $"Xbox auto-launch: role '{role}' requires engine 'xbox' but no default Xbox console is configured. " +
                        $"Run 'xbconnect <ip>' to set a default console.");
                }

                Log($"Xbox auto-launch: default console is '{_xboxConsoleAddress}'.");

                // Quick reachability check — if the console is off/unreachable, fail fast
                // instead of spending 60+ seconds on deploy/launch timeouts.
                if (!await IsConsoleReachableAsync(_xboxConsoleAddress, TimeSpan.FromSeconds(2)).ConfigureAwait(false))
                {
                    _xboxConsoleAddress = null; // Reset so next scenario can retry
                    throw new TimeoutException(
                        $"Xbox auto-launch: console is not reachable (2s ping timeout). Is it powered on?");
                }
            }

            // Deploy once per session
            if (!_xboxDeployed)
            {
                // Verify/set sandbox before first deploy (may reboot the console)
                EnsureXboxSandbox(_xboxConsoleAddress);

                if (_xboxLayoutDir == null)
                {
                    _xboxLayoutDir = XboxDeviceLauncher.ResolveXboxLayoutPath();
                    if (_xboxLayoutDir == null)
                    {
                        throw new FileNotFoundException(
                            $"Xbox auto-launch: GameTestAppXbox layout not found. " +
                            $"Build the Gaming.Xbox.Scarlett.x64 target first.");
                    }

                    Log($"Xbox auto-launch: layout dir is '{_xboxLayoutDir}'.");
                }

                // Write the controller's IP into the layout so the Xbox app
                // knows where to connect
                string? serverIp = XboxDeviceLauncher.GetLocalServerIp(_xboxConsoleAddress);
                if (string.IsNullOrEmpty(serverIp))
                {
                    throw new InvalidOperationException(
                        "Xbox auto-launch: could not determine this PC's IP address for controllerip.txt.");
                }

                XboxDeviceLauncher.WriteControllerIp(_xboxLayoutDir, serverIp, msg => Log(msg));
                XboxDeviceLauncher.Deploy(_xboxLayoutDir, _xboxConsoleAddress, msg => Log(msg));
                _xboxDeployed = true;
            }

            // Launch the app (every scenario — the app may have been terminated)
            // Prepare Xbox users: ensure exactly the required count are signed in.
            // Reuses already-signed-in users when possible to avoid sign-in failures
            // from expired passwords. With only 1 user signed in, AddDefaultUserSilently
            // succeeds without "Who are you?" dialog.
            PrepareXboxUsers(_xboxConsoleAddress!, requiredXboxUsers);

            XboxDeviceLauncher.Launch(_xboxConsoleAddress!, msg => Log(msg));

            // Wait for an Xbox device to connect and report capabilities
            bool ready = await WaitForXboxDeviceReadyAsync(timeout).ConfigureAwait(false);
            if (!ready)
            {
                throw new TimeoutException(
                    $"Xbox auto-launch: app launched on '{_xboxConsoleAddress}' but no device with engine 'xbox' " +
                    $"connected within {timeout.TotalSeconds:0} seconds.");
            }

            Log($"Xbox device connected and ready for role '{role}'.", true);
        }

        /// <summary>
        /// TCP connect probe to check if the Xbox console is reachable.
        /// Uses port 11443 (Xbox Device Portal) as a lightweight check.
        /// </summary>
        private static async Task<bool> IsConsoleReachableAsync(string address, TimeSpan timeout)
        {
            try
            {
                using var tcp = new System.Net.Sockets.TcpClient();
                var connectTask = tcp.ConnectAsync(address, 11443);
                if (await Task.WhenAny(connectTask, Task.Delay(timeout)).ConfigureAwait(false) == connectTask)
                {
                    return tcp.Connected;
                }
                return false;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// Returns true if any connected device reports engine "xbox" with capabilities ready.
        /// </summary>
        private bool IsXboxDeviceReady()
        {
            return _transport.GetConnectedDeviceDetails().Any(d =>
                d.IsConnected && d.CapabilitiesReady
                && string.Equals(d.Engine, "xbox", StringComparison.OrdinalIgnoreCase));
        }

        /// <summary>
        /// Returns true if the device for the specified role reports engine "xbox" with capabilities ready.
        /// Unlike <see cref="IsXboxDeviceReady"/>, this checks the specific role to avoid
        /// skipping deployment for DeviceB when DeviceA is already connected.
        /// </summary>
        private bool IsXboxDeviceReadyForRole(string role)
        {
            return _transport.GetConnectedDeviceDetails().Any(d =>
                d.IsConnected && d.CapabilitiesReady
                && string.Equals(d.Engine, "xbox", StringComparison.OrdinalIgnoreCase)
                && string.Equals(d.DisplayName, role, StringComparison.OrdinalIgnoreCase));
        }

        /// <summary>
        /// Waits until a device with engine "xbox" is connected and has reported capabilities,
        /// or the timeout expires.
        /// </summary>
        private async Task<bool> WaitForXboxDeviceReadyAsync(TimeSpan timeout)
        {
            DateTime deadline = DateTime.UtcNow + timeout;
            while (DateTime.UtcNow <= deadline)
            {
                if (IsXboxDeviceReady())
                {
                    return true;
                }

                await Task.Delay(1000).ConfigureAwait(false);
            }

            return IsXboxDeviceReady();
        }

        /// <summary>
        /// Resets the Xbox device after a scenario failure to prevent cascade failures.
        /// Terminates the (possibly broken) Xbox app, disconnects the stale WebSocket session,
        /// relaunches the app, and waits for it to reconnect.
        /// </summary>
        private async Task ResetXboxDeviceForNextScenarioAsync()
        {
            if (_xboxConsoleAddress == null)
                return; // No Xbox was ever used this session

            // Overall timeout: if reset takes longer than 120s, something is fundamentally broken
            using var resetCts = new CancellationTokenSource(TimeSpan.FromSeconds(120));
            try
            {
                await ResetXboxDeviceInternalAsync(resetCts.Token).ConfigureAwait(false);
            }
            catch (OperationCanceledException) when (resetCts.IsCancellationRequested)
            {
                Log("ERROR: Xbox reset timed out after 120 seconds. Marking device as exhausted.");
                _deviceReconnectExhausted = true;
            }
        }


        private async Task ResetXboxDeviceInternalAsync(CancellationToken cancellationToken)
        {
            // Check if any Xbox device is currently connected (broken or healthy).
            // DeviceConnectionInfo is a record struct, so use a list+Count check
            // (FirstOrDefault on a value type returns default(T), not null).
            var xboxMatches = _transport.GetConnectedDeviceDetails()
                .Where(d => string.Equals(d.Engine, "xbox", StringComparison.OrdinalIgnoreCase))
                .ToList();

            if (xboxMatches.Count == 0)
            {
                // Xbox already disconnected — just need to relaunch
                Log("Xbox device already disconnected. Relaunching for next scenario...");
            }
            else
            {
                var xboxDevice = xboxMatches[0];
                Log($"Resetting Xbox device '{xboxDevice.DisplayName}' for next scenario...");

                // 1. Terminate the Xbox app process
                try
                {
                    DeviceStateController.RunXbToolQuiet(
                        "xbapp.exe",
                        $"/x:{_xboxConsoleAddress} terminate {DeviceStateController.AppAUMID}");
                }
                catch (Exception ex)
                {
                    Log($"Xbox terminate failed (non-fatal): {ex.Message}");
                }

                // 2. Force-disconnect the stale WebSocket session by its ClientId
                _transport.ForceDisconnectDevice(xboxDevice.ClientId);
            }

            // 2.5. Reset Connected Storage to clear stale locks.
            // Uses fire-and-forget process with hard kill after timeout to avoid
            // the ReadToEnd() deadlock that RunXbToolQuiet suffers from.
            try
            {
                string xbstoragePath = DeviceStateController.ResolveGdkToolPath("xbstorage.exe");
                using var csProc = new System.Diagnostics.Process();
                csProc.StartInfo.FileName = xbstoragePath;
                csProc.StartInfo.Arguments = $"/X:{_xboxConsoleAddress} reset /force";
                csProc.StartInfo.UseShellExecute = false;
                csProc.StartInfo.RedirectStandardOutput = false;
                csProc.StartInfo.RedirectStandardError = false;
                csProc.StartInfo.CreateNoWindow = true;
                csProc.Start();
                if (!csProc.WaitForExit(30_000))
                {
                    try { csProc.Kill(); } catch { }
                    Log("Connected Storage reset timed out after 30s (killed).");
                }
                else
                {
                    Log("Connected Storage reset completed.");
                }
            }
            catch (Exception ex)
            {
                Log($"Connected Storage reset failed (non-fatal): {ex.Message}");
            }

            // 3. Brief pause for cleanup
            await Task.Delay(3000, cancellationToken).ConfigureAwait(false);

            // 4. Prepare Xbox users for relaunch with retry logic.
            // Sign out all, sign in primary only. Next scenario's EnsureXboxDeviceAsync adjusts if needed.
            const int maxUserPrepRetries = 3;
            bool userPrepSucceeded = false;
            for (int attempt = 1; attempt <= maxUserPrepRetries; attempt++)
            {
                try
                {
                    PrepareXboxUsers(_xboxConsoleAddress!, 1);
                    userPrepSucceeded = true;
                    break;
                }
                catch (Exception ex)
                {
                    Log($"Xbox user preparation failed (attempt {attempt}/{maxUserPrepRetries}): {ex.Message}");
                    if (attempt < maxUserPrepRetries)
                    {
                        await Task.Delay(1000 * attempt, cancellationToken).ConfigureAwait(false);
                    }
                }
            }

            if (!userPrepSucceeded)
            {
                Log("WARNING: Xbox user preparation failed after all retries. Continuing with relaunch anyway.");
            }

            // 5. Relaunch the app
            try
            {
                XboxDeviceLauncher.Launch(_xboxConsoleAddress!, msg => Log(msg));
            }
            catch (Exception ex)
            {
                Log($"ERROR: Xbox relaunch failed: {ex.Message}");
                _deviceReconnectExhausted = true;
                return;
            }

            // 6. Wait for the Xbox to reconnect
            bool ready = await WaitForXboxDeviceReadyAsync(TimeSpan.FromSeconds(60)).ConfigureAwait(false);
            if (ready)
            {
                Log("Xbox device reconnected after reset.");
                _deviceReconnectExhausted = false;
            }
            else
            {
                Log("ERROR: Xbox device did not reconnect after reset. Marking as exhausted.");
                _deviceReconnectExhausted = true;
            }
        }

        /// <summary>
        /// Cleans up PC (local) devices between tests. Force-disconnects stale WebSocket
        /// sessions and kills orphaned GameTestAppWindows processes so each test starts fresh.
        /// </summary>
        private void ResetPcDevicesForNextScenario()
        {
            try
            {
                // Force-disconnect any remaining PC WebSocket sessions
                var pcDevices = _transport.GetConnectedDeviceDetails()
                    .Where(d => !string.Equals(d.Engine, "xbox", StringComparison.OrdinalIgnoreCase))
                    .ToList();

                foreach (var device in pcDevices)
                {
                    try
                    {
                        _transport.ForceDisconnectDevice(device.ClientId);
                        Log($"Force-disconnected PC device '{device.DisplayName}' (ClientId={device.ClientId}).", true);
                    }
                    catch (Exception ex)
                    {
                        Log($"Failed to disconnect PC device '{device.DisplayName}': {ex.Message}", true);
                    }
                }

                // Kill any remaining GameTestAppWindows processes
                KillOrphanedPcAppProcesses();
            }
            catch (Exception ex)
            {
                Log($"PC device cleanup error (non-fatal): {ex.Message}", true);
            }
        }

        /// <summary>
        /// Manages Xbox user sign-in state for a clean environment.
        /// Signs out all users first, then signs in exactly the required number.
        /// This prevents the "Who are you?" dialog (caused by multiple signed-in users
        /// with no auto-signin set) and ensures tests start from a known state.
        /// </summary>
        /// <param name="xboxIp">Console IP address</param>
        /// <param name="requiredUserCount">Number of users to sign in (default 1 = primary only)</param>
        private void PrepareXboxUsers(string xboxIp, int requiredUserCount = 1)
        {
            var (exitCode, stdout) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} list");
            if (exitCode != 0)
            {
                throw new InvalidOperationException($"Failed to list Xbox users (exit code {exitCode}).");
            }

            // Parse all users with their sign-in state and email addresses
            var userIdMatches = System.Text.RegularExpressions.Regex.Matches(stdout, @"UserId:\s*(\d+)");
            var emailMatches = System.Text.RegularExpressions.Regex.Matches(stdout, @"Email Address:\s*(\S+)");
            var signedInFlags = System.Text.RegularExpressions.Regex.Matches(stdout, @"Signed in:\s*(Yes|No)", System.Text.RegularExpressions.RegexOptions.IgnoreCase);

            if (userIdMatches.Count != signedInFlags.Count)
            {
                Log($"WARNING: User list parse mismatch: {userIdMatches.Count} UserIds vs {signedInFlags.Count} sign-in flags.");
            }

            var allUsers = new List<(string Id, string Email)>();
            for (int i = 0; i < userIdMatches.Count && i < signedInFlags.Count; i++)
            {
                string email = i < emailMatches.Count ? emailMatches[i].Groups[1].Value : "";
                allUsers.Add((userIdMatches[i].Groups[1].Value, email));
            }

            // Auto-provision users from testAccountConfig.json when none exist (e.g., after recovery)
            if (allUsers.Count == 0)
            {
                Log("No Xbox users found on console — attempting auto-provision from testAccountConfig.json...");
                AutoProvisionXboxUsers(xboxIp, requiredUserCount);

                // Re-list after provisioning
                var (rc2, stdout2) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} list");
                if (rc2 != 0)
                    throw new InvalidOperationException($"Failed to list Xbox users after provisioning (exit code {rc2}).");

                userIdMatches = System.Text.RegularExpressions.Regex.Matches(stdout2, @"UserId:\s*(\d+)");
                emailMatches = System.Text.RegularExpressions.Regex.Matches(stdout2, @"Email Address:\s*(\S+)");
                signedInFlags = System.Text.RegularExpressions.Regex.Matches(stdout2, @"Signed in:\s*(Yes|No)", System.Text.RegularExpressions.RegexOptions.IgnoreCase);
                allUsers.Clear();
                for (int i = 0; i < userIdMatches.Count && i < signedInFlags.Count; i++)
                {
                    string em = i < emailMatches.Count ? emailMatches[i].Groups[1].Value : "";
                    allUsers.Add((userIdMatches[i].Groups[1].Value, em));
                }

                if (allUsers.Count == 0)
                    throw new InvalidOperationException("No Xbox users found even after auto-provisioning. Check testAccountConfig.json and account provisioning in Partner Center.");
            }

            // Count how many users are already signed in.
            var signedInUsers = new List<(string Id, string Email)>();
            var signedOutUsers = new List<(string Id, string Email)>();
            for (int i = 0; i < allUsers.Count && i < signedInFlags.Count; i++)
            {
                bool isSignedIn = signedInFlags[i].Groups[1].Value.Equals("Yes", StringComparison.OrdinalIgnoreCase);
                if (isSignedIn)
                    signedInUsers.Add(allUsers[i]);
                else
                    signedOutUsers.Add(allUsers[i]);
            }

            // If exactly the right number of users are already signed in, skip the
            // sign-out/sign-in cycle entirely. The test just needs N signed-in users,
            // not specific accounts. This avoids failures from expired test account
            // passwords or locked accounts when a usable user is already present.
            if (signedInUsers.Count == requiredUserCount)
            {
                Log($"Xbox user state: {signedInUsers.Count} user(s) already signed in (required: {requiredUserCount}). Skipping sign-in cycle.");
                return;
            }

            // If more users are signed in than required, sign out only the excess
            // to maintain the "exactly N users" invariant that prevents the
            // "Who are you?" dialog when AddDefaultUserSilently is called.
            if (signedInUsers.Count > requiredUserCount)
            {
                int excessCount = signedInUsers.Count - requiredUserCount;
                Log($"Xbox user state: {signedInUsers.Count} signed in but only {requiredUserCount} required — signing out {excessCount} excess user(s).");
                for (int i = signedInUsers.Count - 1; i >= requiredUserCount; i--)
                {
                    var (soRc, _) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} signout /i:{signedInUsers[i].Id}", timeoutMs: 10000);
                    if (soRc == 0)
                    {
                        Log($"Signed out excess Xbox UserId {signedInUsers[i].Id}.");
                    }
                    else
                    {
                        Log($"WARNING: Failed to sign out excess Xbox UserId {signedInUsers[i].Id} (exit code {soRc}).");
                    }
                }

                Log($"Xbox user state: kept {requiredUserCount} user(s) signed in.");
                return;
            }

            // Fewer than required users are signed in — try to sign in more.
            // Build email-to-password map for password-retry fallback.
            var passwordMap = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            var config = TestAccountConfig.Instance;
            if (config != null)
            {
                if (!string.IsNullOrEmpty(config.XboxPrimaryEmail) && !string.IsNullOrEmpty(config.XboxPrimaryPassword))
                    passwordMap[config.XboxPrimaryEmail] = config.XboxPrimaryPassword;
                if (!string.IsNullOrEmpty(config.XboxSecondaryEmail) && !string.IsNullOrEmpty(config.XboxSecondaryPassword))
                    passwordMap[config.XboxSecondaryEmail] = config.XboxSecondaryPassword;
            }

            int successfulSignins = signedInUsers.Count;
            foreach (var (userId, email) in signedOutUsers)
            {
                if (successfulSignins >= requiredUserCount)
                    break;

                // Try sign-in without password first (works when credentials are cached on console)
                var (rc, _) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} signin /i:{userId}", timeoutMs: 15000);
                if (rc == 0)
                {
                    successfulSignins++;
                    Log($"Signed in Xbox UserId {userId} ({successfulSignins}/{requiredUserCount}).");
                    continue;
                }

                // If that failed and we have a password, retry with password
                if (passwordMap.TryGetValue(email, out string? password) && !string.IsNullOrEmpty(password))
                {
                    Log($"Sign-in without password failed for UserId {userId} — retrying with password...");
                    var (rc2, _2) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} signin /i:{userId} /p:{password}", timeoutMs: 15000);
                    if (rc2 == 0)
                    {
                        successfulSignins++;
                        Log($"Signed in Xbox UserId {userId} with password ({successfulSignins}/{requiredUserCount}).");
                        continue;
                    }
                }

                Log($"WARNING: Failed to sign in Xbox UserId {userId} ({email}).");
            }

            // If we now have more than required (e.g., had 1, signed in 1 more for
            // a total of 2 but only needed 1), sign out the excess.
            if (successfulSignins > requiredUserCount)
            {
                Log($"Xbox user state: {successfulSignins} signed in, trimming to {requiredUserCount}.");
                // Re-list to get current state and sign out extras
                var (rc3, stdout3) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} list");
                if (rc3 == 0)
                {
                    var ids3 = System.Text.RegularExpressions.Regex.Matches(stdout3, @"UserId:\s*(\d+)");
                    var flags3 = System.Text.RegularExpressions.Regex.Matches(stdout3, @"Signed in:\s*(Yes|No)", System.Text.RegularExpressions.RegexOptions.IgnoreCase);
                    int kept = 0;
                    for (int i = 0; i < ids3.Count && i < flags3.Count; i++)
                    {
                        if (flags3[i].Groups[1].Value.Equals("Yes", StringComparison.OrdinalIgnoreCase))
                        {
                            kept++;
                            if (kept > requiredUserCount)
                            {
                                DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} signout /i:{ids3[i].Groups[1].Value}", timeoutMs: 10000);
                            }
                        }
                    }
                }
            }

            if (successfulSignins < requiredUserCount)
            {
                // Don't throw — let the app launch anyway. If no user is signed in,
                // the test scenario's XUserAddAsync step will fail with a clear error
                // rather than blocking the entire auto-launch. A user may also sign in
                // manually on the devkit before the test step executes.
                Log($"WARNING: Only {successfulSignins} of {requiredUserCount} required Xbox users signed in. " +
                    $"The app will launch anyway — XUserAddAsync may prompt or fail if no user is available.");
                return;
            }

            Log($"Xbox user state: {successfulSignins} of {allUsers.Count} users signed in (required: {requiredUserCount}).");
        }

        /// <summary>
        /// Auto-provisions Xbox test accounts from testAccountConfig.json when the console
        /// has no users (e.g., after a factory reset or recovery).
        /// Waits for the console to be fully ready before adding users.
        /// </summary>
        private void AutoProvisionXboxUsers(string xboxIp, int requiredUserCount)
        {
            var config = TestAccountConfig.Instance;
            if (config == null)
            {
                throw new InvalidOperationException(
                    "No Xbox users on console and no testAccountConfig.json found. " +
                    "Either add users manually (xbuser /X <ip> add /e:<email>) or create Test/testAccountConfig.json.");
            }

            // Collect account emails in priority order: primary first, then secondary
            var emails = new List<string>();
            string? primary = config.XboxPrimaryEmail;
            string? secondary = config.XboxSecondaryEmail;

            if (!string.IsNullOrEmpty(primary)) emails.Add(primary);
            if (!string.IsNullOrEmpty(secondary)) emails.Add(secondary);

            if (emails.Count == 0)
            {
                throw new InvalidOperationException(
                    "testAccountConfig.json has no Xbox account emails configured. " +
                    "Add xboxPrimary and/or xboxSecondary accounts.");
            }

            // Wait for console to be fully ready for user operations (post-recovery can take time)
            WaitForConsoleReady(xboxIp);

            int toAdd = Math.Max(requiredUserCount, emails.Count);
            for (int i = 0; i < toAdd && i < emails.Count; i++)
            {
                Log($"Auto-provisioning Xbox user: {emails[i]}...");
                var (rc, output) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} add /e:{emails[i]}", timeoutMs: 30000);
                if (rc == 0)
                {
                    Log($"Added Xbox user '{emails[i]}' successfully.");
                }
                else
                {
                    Log($"WARNING: Failed to add Xbox user '{emails[i]}' (exit code {rc}): {output}");
                }
            }

            // Verify users were actually added
            var (verifyRc, verifyOut) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} list", timeoutMs: 15000);
            if (verifyRc != 0)
            {
                throw new InvalidOperationException($"Failed to verify Xbox users after provisioning (exit code {verifyRc}).");
            }

            var addedCount = System.Text.RegularExpressions.Regex.Matches(verifyOut, @"UserId:\s*\d+").Count;
            if (addedCount == 0)
            {
                throw new InvalidOperationException(
                    "Auto-provisioning completed but no users appeared on console. " +
                    "The console may not be fully ready. Try again or add users manually.");
            }

            Log($"Auto-provisioning verified: {addedCount} user(s) now on console.");
        }

        /// <summary>
        /// Waits for the Xbox console to be fully ready to accept user operations.
        /// After recovery or reboot, there's a window where xbconfig works but xbuser doesn't.
        /// </summary>
        private void WaitForConsoleReady(string xboxIp)
        {
            Log("Checking Xbox console readiness...");
            for (int attempt = 0; attempt < 24; attempt++) // up to ~120 seconds
            {
                var (rc, stdout) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} list", timeoutMs: 10000);
                if (rc == 0 && !stdout.Contains("not yet ready", StringComparison.OrdinalIgnoreCase))
                {
                    Log("Xbox console is ready for user operations.");
                    return;
                }

                if (attempt == 0)
                    Log("Xbox console not yet ready — waiting...");

                System.Threading.Thread.Sleep(5000);
            }

            throw new TimeoutException("Xbox console did not become ready for user operations within 120 seconds.");
        }

        /// <summary>
        /// Ensures the Xbox console is in the correct sandbox. Sets it and reboots if needed.
        /// Call before deploying the app.
        /// </summary>
        private void EnsureXboxSandbox(string xboxIp)
        {
            var config = TestAccountConfig.Instance;
            string? requiredSandbox = config?.Sandbox;
            if (string.IsNullOrEmpty(requiredSandbox))
            {
                Log("No sandbox configured in testAccountConfig.json — skipping sandbox check.");
                return;
            }

            var (exitCode, stdout) = DeviceStateController.RunXbToolQuiet("xbconfig.exe", $"/X {xboxIp} SandboxId");
            if (exitCode != 0)
            {
                Log($"WARNING: Could not query Xbox sandbox (exit code {exitCode}). Proceeding without verification.");
                return;
            }

            var match = System.Text.RegularExpressions.Regex.Match(stdout, @"SandboxId\s*[=:]\s*(\S+)", System.Text.RegularExpressions.RegexOptions.IgnoreCase);
            string actualSandbox = match.Success ? match.Groups[1].Value : stdout.Trim();

            if (string.Equals(actualSandbox, requiredSandbox, StringComparison.OrdinalIgnoreCase))
            {
                Log($"Xbox sandbox is correct: {actualSandbox}");
                return;
            }

            Log($"Xbox sandbox mismatch: '{actualSandbox}' → setting to '{requiredSandbox}' and rebooting...");
            var (setRc, setOut) = DeviceStateController.RunXbToolQuiet("xbconfig.exe", $"/X {xboxIp} SandboxId={requiredSandbox}", timeoutMs: 15000);
            if (setRc != 0)
            {
                throw new InvalidOperationException(
                    $"Failed to set Xbox sandbox to '{requiredSandbox}' (exit code {setRc}): {setOut}");
            }

            // Reboot required for sandbox change to take effect
            Log("Rebooting Xbox for sandbox change...");
            var (rebootRc, _) = DeviceStateController.RunXbToolQuiet("xbreboot.exe", $"/X {xboxIp}", timeoutMs: 10000);
            if (rebootRc != 0)
            {
                Log($"WARNING: Reboot command returned exit code {rebootRc}. Console may need manual reboot.");
            }

            // Wait for the console to come back online — verify with user operations too
            Log("Waiting for Xbox to come back online after reboot...");
            bool online = false;
            for (int attempt = 0; attempt < 24; attempt++) // ~120 seconds
            {
                System.Threading.Thread.Sleep(5000);
                var (pingRc, pingOut) = DeviceStateController.RunXbToolQuiet("xbconfig.exe", $"/X {xboxIp} SandboxId", timeoutMs: 10000);
                if (pingRc == 0 && pingOut.Contains(requiredSandbox, StringComparison.OrdinalIgnoreCase))
                {
                    // Also verify user operations work (xbconfig can succeed before xbuser is ready)
                    var (userRc, userOut) = DeviceStateController.RunXbToolQuiet("xbuser.exe", $"/X {xboxIp} list", timeoutMs: 10000);
                    if (userRc == 0 && !userOut.Contains("not yet ready", StringComparison.OrdinalIgnoreCase))
                    {
                        online = true;
                        break;
                    }
                }
            }

            if (!online)
            {
                throw new TimeoutException(
                    $"Xbox did not come back online within 120 seconds after sandbox change reboot.");
            }

            Log($"Xbox is back online with sandbox '{requiredSandbox}'.");
        }

        private async Task LaunchLocalDeviceAsync(string role, TimeSpan timeout, bool forceInproc = false)
        {
            if (string.IsNullOrWhiteSpace(role))
            {
                throw new ArgumentException("Device role must be provided.", nameof(role));
            }

            if (IsDeviceSessionPresent(role))
            {
                bool alreadyReady = await WaitForDeviceConnectionAsync(role, timeout).ConfigureAwait(false);
                if (!alreadyReady)
                {
                    throw new TimeoutException($"Device '{role}' is connected but did not become ready within {timeout.TotalSeconds:0} seconds.");
                }

                Log($"Device '{role}' connected.", true);
                return;
            }

            // Kill any orphaned PC app processes before launching a new one.
            // This prevents zombie accumulation when WebSocket connections drop
            // but the process stays alive (e.g., after a timeout or crash).
            KillOrphanedPcAppProcesses();

            string deviceExecutable = ResolveDeviceExecutablePath();
            if (!File.Exists(deviceExecutable))
            {
                throw new FileNotFoundException($"Auto-launch enabled, but device executable was not found at '{deviceExecutable}'.", deviceExecutable);
            }

            Log($"Auto-launching local device for role '{role}' using '{deviceExecutable}'{(forceInproc ? " (forceinproc)" : "")}.");

            try
            {
                var startInfo = new ProcessStartInfo
                {
                    FileName = deviceExecutable,
                    WorkingDirectory = Path.GetDirectoryName(deviceExecutable) ?? ResolveBaseDirectory(),
                    UseShellExecute = false
                };

                if (forceInproc)
                {
                    startInfo.ArgumentList.Add("/forceinproc");
                }

                Process? launched = Process.Start(startInfo);
                if (launched == null)
                {
                    throw new InvalidOperationException($"Failed to auto-launch device for role '{role}': process did not start.");
                }

                DeviceStateController.RegisterLaunchedDevice(role, launched.Id);
                launched.Dispose();
            }
            catch (Exception ex) when (ex is not InvalidOperationException)
            {
                throw new InvalidOperationException($"Failed to auto-launch device for role '{role}': {ex.Message}", ex);
            }

            bool connected = await WaitForDeviceConnectionAsync(role, timeout).ConfigureAwait(false);
            if (!connected)
            {
                throw new TimeoutException($"Auto-launched device for role '{role}' did not connect within {timeout.TotalSeconds:0} seconds.");
            }

            Log($"Device '{role}' connected.", true);
        }

        /// <summary>
        /// Kills any GameTestAppWindows processes that are not connected via WebSocket.
        /// Prevents zombie process accumulation when connections drop but processes stay alive.
        /// </summary>
        private void KillOrphanedPcAppProcesses()
        {
            try
            {
                var pcProcesses = Process.GetProcessesByName("GameTestAppWindows");
                if (pcProcesses.Length == 0)
                    return;

                // Check which PC devices are still connected
                var connectedPcDevices = _transport.GetConnectedDeviceDetails()
                    .Where(d => d.IsConnected && !string.Equals(d.Engine, "xbox", StringComparison.OrdinalIgnoreCase))
                    .ToList();

                if (connectedPcDevices.Count > 0)
                {
                    // Some PC devices are still connected — don't kill processes
                    // (we can't reliably match PIDs to WebSocket sessions)
                    foreach (var p in pcProcesses) p.Dispose();
                    return;
                }

                // No PC devices connected but processes exist — kill them all
                Log($"Killing {pcProcesses.Length} orphaned GameTestAppWindows process(es).");
                foreach (var proc in pcProcesses)
                {
                    try
                    {
                        if (!proc.HasExited)
                        {
                            proc.Kill();
                            Log($"Killed orphaned PC app process (PID {proc.Id}).", true);
                        }
                    }
                    catch (Exception ex)
                    {
                        Log($"Failed to kill PC app process (PID {proc.Id}): {ex.Message}", true);
                    }
                    finally
                    {
                        proc.Dispose();
                    }
                }
            }
            catch (Exception ex)
            {
                Log($"Error checking for orphaned PC app processes: {ex.Message}", true);
            }
        }

        private async Task<bool> WaitForDeviceConnectionAsync(string deviceName, TimeSpan timeout)
        {
            DateTime deadline = DateTime.UtcNow + timeout;
            while (DateTime.UtcNow <= deadline)
            {
                if (_transport.IsDeviceConnected(deviceName))
                {
                    return true;
                }

                await Task.Delay(500).ConfigureAwait(false);
            }

            return _transport.IsDeviceConnected(deviceName);
        }

        /// <summary>
        /// Restarts devices that were force-disconnected during a scenario timeout.
        /// For PC devices: kills the hung process, launches a fresh instance.
        /// For Xbox devices: terminates and relaunches via xbapp.
        /// Waits for WebSocket reconnection before returning.
        /// This prevents subsequent scenarios from being skipped due to missing devices.
        /// </summary>
        private async Task RestartForcedDisconnectedDevicesAsync(
            IReadOnlyList<(string Role, string Engine)> devices, CancellationToken cancellationToken = default)
        {
            foreach (var (role, engine) in devices)
            {
                cancellationToken.ThrowIfCancellationRequested();
                Log($"Restarting force-disconnected device '{role}' (engine={engine})...");

                bool isXbox = engine.StartsWith("xbox", StringComparison.OrdinalIgnoreCase);

                if (isXbox)
                {
                    await RestartXboxDeviceAsync(role, cancellationToken).ConfigureAwait(false);
                }
                else
                {
                    await RestartLocalDeviceAsync(role, engine, cancellationToken).ConfigureAwait(false);
                }
            }
        }

        private async Task RestartXboxDeviceAsync(string role, CancellationToken cancellationToken)
        {
            // Terminate then relaunch via xbapp using the cached console address
            if (_xboxConsoleAddress == null)
            {
                _xboxConsoleAddress = XboxDeviceLauncher.GetDefaultConsoleAddress();
            }

            if (_xboxConsoleAddress == null)
            {
                Log($"Cannot restart Xbox device '{role}': no console address configured.");
                return;
            }

            try
            {
                XboxDeviceLauncher.Terminate(_xboxConsoleAddress, msg => Log(msg));
                await Task.Delay(2000, cancellationToken).ConfigureAwait(false);
                XboxDeviceLauncher.Launch(_xboxConsoleAddress, msg => Log(msg));
            }
            catch (Exception ex)
            {
                Log($"Failed to restart Xbox device '{role}': {ex.Message}");
                return;
            }

            bool connected = await WaitForXboxDeviceReadyAsync(TimeSpan.FromSeconds(30)).ConfigureAwait(false);
            if (connected)
            {
                Log($"Xbox device '{role}' reconnected successfully.");
            }
            else
            {
                Log($"WARNING: Xbox device '{role}' did not reconnect within 30 seconds.");
            }
        }

        private async Task RestartLocalDeviceAsync(string role, string engine, CancellationToken cancellationToken)
        {
            // Kill the hung process if we have a tracked PID
            int pid = DeviceStateController.GetLaunchedDevicePid(role);
            if (pid > 0)
            {
                try
                {
                    var proc = Process.GetProcessById(pid);
                    Log($"Killing hung device '{role}' (PID {pid}).");
                    proc.Kill();
                    proc.WaitForExit(10_000);
                }
                catch (ArgumentException)
                {
                    Log($"Device '{role}' (PID {pid}) already exited.", true);
                }
                catch (Exception ex)
                {
                    Log($"Failed to kill device '{role}' (PID {pid}): {ex.Message}");
                }
                DeviceStateController.UnregisterLaunchedDevice(role);
            }

            // Relaunch the device process
            string deviceExecutable = ResolveDeviceExecutablePath();
            if (!File.Exists(deviceExecutable))
            {
                Log($"Cannot restart device '{role}': executable not found at '{deviceExecutable}'.");
                return;
            }

            bool forceInproc = engine.StartsWith("pc-inproc", StringComparison.OrdinalIgnoreCase);

            try
            {
                var startInfo = new ProcessStartInfo
                {
                    FileName = deviceExecutable,
                    WorkingDirectory = Path.GetDirectoryName(deviceExecutable) ?? ResolveBaseDirectory(),
                    UseShellExecute = false
                };

                if (forceInproc)
                {
                    startInfo.ArgumentList.Add("/forceinproc");
                }

                Process? launched = Process.Start(startInfo);
                if (launched == null)
                {
                    Log($"Failed to restart device '{role}': process did not start.");
                    return;
                }

                DeviceStateController.RegisterLaunchedDevice(role, launched.Id);
                Log($"Device '{role}' relaunched (PID {launched.Id}){(forceInproc ? " (forceinproc)" : "")}. Waiting for reconnection...");
                launched.Dispose();
            }
            catch (Exception ex)
            {
                Log($"Failed to restart device '{role}': {ex.Message}");
                return;
            }

            // Wait for the new device to connect and complete capability handshake
            bool connected = await WaitForDeviceConnectionAsync(role, TimeSpan.FromSeconds(30)).ConfigureAwait(false);
            if (connected)
            {
                Log($"Device '{role}' reconnected successfully.");
            }
            else
            {
                Log($"WARNING: Device '{role}' did not reconnect within 30 seconds.");
            }
        }

        private string? GetNextAvailableLocalDeviceRole()
        {
            if (!IsDeviceSessionPresent("DeviceA"))
            {
                return "DeviceA";
            }

            if (!IsDeviceSessionPresent("DeviceB"))
            {
                return "DeviceB";
            }

            return null;
        }

        private bool IsDeviceSessionPresent(string deviceName)
        {
            return _transport.GetConnectedDeviceDetails().Any(
                device => device.IsConnected && string.Equals(device.DisplayName, deviceName, StringComparison.OrdinalIgnoreCase));
        }

        /// <summary>
        /// Waits for a specified number of devices to connect with capabilities ready.
        /// </summary>
        /// <param name="requiredCount">Minimum number of devices to wait for.</param>
        /// <param name="timeoutSeconds">Maximum time to wait in seconds.</param>
        /// <returns>True if required number of devices connected, false if timeout.</returns>
        public async Task<bool> WaitForDevicesAsync(int requiredCount, int timeoutSeconds)
        {
            DateTime deadline = DateTime.UtcNow.AddSeconds(timeoutSeconds);
            int lastCount = 0;

            while (DateTime.UtcNow <= deadline)
            {
                var devices = _transport.GetConnectedDeviceDetails()
                    .Where(d => d.IsConnected && d.CapabilitiesReady)
                    .ToList();

                if (devices.Count != lastCount)
                {
                    lastCount = devices.Count;
                    Log($"Devices connected: {devices.Count} ({string.Join(", ", devices.Select(d => d.DisplayName))})");
                }

                if (devices.Count >= requiredCount)
                {
                    return true;
                }

                await Task.Delay(500).ConfigureAwait(false);
            }

            var finalDevices = _transport.GetConnectedDeviceDetails()
                .Where(d => d.IsConnected && d.CapabilitiesReady)
                .ToList();
            
            return finalDevices.Count >= requiredCount;
        }

        /// <summary>
        /// Launches remote test apps via connected agents and waits for the required number
        /// of game devices to connect. If an agent reconnects during the wait (e.g., after
        /// a transient network drop), re-issues the launchTestApp command so that the remote
        /// game can still connect within the timeout.
        /// </summary>
        private async Task LaunchRemoteAppsAndWaitForDevicesAsync(int requiredDevices, int timeoutSeconds, CancellationToken cancellationToken)
        {
            // Track which agents we've already sent launch commands to
            var launchedAgentIds = new HashSet<Guid>();

            // Initial launch to all currently connected agents
            var agents = _transport.GetConnectedAgents();
            if (agents.Count > 0)
            {
                Log("Launching remote test apps via agents...");
                bool remoteLaunched = await _transport.LaunchAllAgentTestAppsAsync().ConfigureAwait(false);
                if (!remoteLaunched)
                {
                    Log("WARNING: Failed to launch one or more remote test apps.");
                }
                foreach (var a in agents)
                    launchedAgentIds.Add(a.ClientId);
            }

            Log($"Waiting up to {timeoutSeconds}s for {requiredDevices} device(s) to connect...");
            DateTime deadline = DateTime.UtcNow.AddSeconds(timeoutSeconds);

            while (DateTime.UtcNow <= deadline)
            {
                cancellationToken.ThrowIfCancellationRequested();

                var devices = _transport.GetConnectedDeviceDetails()
                    .Where(d => d.IsConnected && d.CapabilitiesReady)
                    .ToList();

                if (devices.Count >= requiredDevices)
                {
                    Log($"All {requiredDevices} device(s) connected.");
                    return;
                }

                // Check if any new agents have connected that we haven't launched on yet
                var currentAgents = _transport.GetConnectedAgents();
                var newAgents = currentAgents.Where(a => !launchedAgentIds.Contains(a.ClientId)).ToList();
                if (newAgents.Count > 0)
                {
                    Log($"Agent(s) reconnected ({string.Join(", ", newAgents.Select(a => a.MachineName))}). Re-launching remote test apps...");
                    bool launched = await _transport.LaunchAllAgentTestAppsAsync().ConfigureAwait(false);
                    if (!launched)
                    {
                        Log("WARNING: Failed to launch on reconnected agent(s).");
                    }
                    foreach (var a in currentAgents)
                        launchedAgentIds.Add(a.ClientId);
                }

                await Task.Delay(500, cancellationToken).ConfigureAwait(false);
            }

            Log($"WARNING: Not all devices connected within {timeoutSeconds}s timeout.");
        }

        /// <summary>
        /// Quick pre-flight check: are there any engine constraints in the scenario
        /// that cannot possibly be met by the currently connected devices?
        /// Returns a skip reason string if incompatible, or null if OK (or unknown).
        /// </summary>
        private string? CheckEngineCompatibility(ScenarioPlan plan)
        {
            if (plan.Manifest.Devices == null || plan.Manifest.Devices.Count == 0)
            {
                return null;
            }

            var connectedEngines = _transport.GetConnectedDeviceDetails()
                .Where(d => d.IsConnected)
                .Select(d => d.Engine)
                .Where(e => !string.IsNullOrEmpty(e))
                .ToHashSet(StringComparer.OrdinalIgnoreCase);

            foreach (KeyValuePair<string, ScenarioDevice> entry in plan.Manifest.Devices)
            {
                ScenarioDevice? spec = entry.Value;
                if (spec == null || !spec.HasEngineConstraint)
                {
                    continue;
                }

                if (!spec.Engine!.Any(e => connectedEngines.Contains(e)))
                {
                    string reason = $"Role '{entry.Key}' requires engine '{spec.EngineDisplay}' but no connected device has that engine. Connected engines: [{string.Join(", ", connectedEngines)}].";
                    Log($"SKIPPED scenario '{plan.Manifest.Id}': {reason}");
                    return reason;
                }
            }

            return null;
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

        /// <summary>
        /// Builds the device launch order sorted by role name (alphabetical).
        /// The WebSocket server assigns device names in FCFS order (DeviceA first,
        /// DeviceB second), so launch order must match to ensure correct role assignment.
        /// </summary>
        private static List<string> BuildLaunchOrder(IReadOnlyCollection<string> required)
        {
            return required
                .Where(r => !string.IsNullOrWhiteSpace(r))
                .Distinct(StringComparer.OrdinalIgnoreCase)
                .OrderBy(r => r, StringComparer.OrdinalIgnoreCase)
                .ToList();
        }

        internal static bool TryExtractGatherLogsMetadata(string rawJson, out GatherLogsMetadata data)
        {
            data = default;
            if (string.IsNullOrWhiteSpace(rawJson))
            {
                return false;
            }

            using JsonDocument doc = JsonDocument.Parse(rawJson);
            JsonElement root = doc.RootElement;
            if (root.ValueKind != JsonValueKind.Object)
            {
                return false;
            }

            string? logPath = null;
            if (root.TryGetProperty("logPath", out JsonElement pathElement) && pathElement.ValueKind == JsonValueKind.String)
            {
                logPath = pathElement.GetString();
            }

            long fileSize = GetInt64Property(root, "fileSize");
            long bytesToTransfer = GetInt64Property(root, "bytesToTransfer");
            long fileStartOffset = GetInt64Property(root, "fileStartOffset");
            bool truncated = GetBooleanProperty(root, "truncated");

            string? summaryLogPath = null;
            if (root.TryGetProperty("summaryLogPath", out JsonElement summaryPathElement) && summaryPathElement.ValueKind == JsonValueKind.String)
            {
                summaryLogPath = summaryPathElement.GetString();
            }

            long summaryFileSize = GetInt64Property(root, "summaryFileSize");
            long summaryBytesToTransfer = GetInt64Property(root, "summaryBytesToTransfer");
            long summaryFileStartOffset = GetInt64Property(root, "summaryFileStartOffset");
            bool summaryTruncated = GetBooleanProperty(root, "summaryTruncated");

            data = new GatherLogsMetadata(logPath, fileSize, bytesToTransfer, fileStartOffset, truncated,
                summaryLogPath, summaryFileSize, summaryBytesToTransfer, summaryFileStartOffset, summaryTruncated);
            return true;
        }

        internal static bool TryExtractGatherLogsChunkData(string rawJson, out string content, out long bytesRead)
        {
            content = string.Empty;
            bytesRead = 0;
            if (string.IsNullOrWhiteSpace(rawJson))
            {
                return false;
            }

            using JsonDocument doc = JsonDocument.Parse(rawJson);
            JsonElement root = doc.RootElement;
            if (root.ValueKind != JsonValueKind.Object)
            {
                return false;
            }

            if (!root.TryGetProperty("content", out JsonElement contentElement) || contentElement.ValueKind != JsonValueKind.String)
            {
                return false;
            }

            content = contentElement.GetString() ?? string.Empty;
            bytesRead = GetInt64Property(root, "bytesRead");
            return true;
        }

        internal static bool TryExtractGatheredSnapshotData(string rawJson, out GatheredSnapshotData data)
        {
            data = default;
            if (string.IsNullOrWhiteSpace(rawJson))
            {
                return false;
            }

            using JsonDocument doc = JsonDocument.Parse(rawJson);
            JsonElement root = doc.RootElement;
            if (root.ValueKind != JsonValueKind.Object)
            {
                return false;
            }

            if (!root.TryGetProperty("base64Content", out JsonElement contentElement) || contentElement.ValueKind != JsonValueKind.String)
            {
                return false;
            }

            string base64Content = contentElement.GetString() ?? string.Empty;
            string? archiveFileName = null;
            if (root.TryGetProperty("archiveFileName", out JsonElement archiveElement) && archiveElement.ValueKind == JsonValueKind.String)
            {
                archiveFileName = archiveElement.GetString();
            }

            string? saveFolder = null;
            if (root.TryGetProperty("saveFolder", out JsonElement saveElement) && saveElement.ValueKind == JsonValueKind.String)
            {
                saveFolder = saveElement.GetString();
            }

            long archiveSize = GetInt64Property(root, "archiveSize");
            long bytesRead = GetInt64Property(root, "bytesRead");
            bool truncated = GetBooleanProperty(root, "truncated");

            data = new GatheredSnapshotData(base64Content, archiveFileName, saveFolder, archiveSize, bytesRead, truncated);
            return true;
        }

        private static long GetInt64Property(JsonElement root, string propertyName)
        {
            if (root.TryGetProperty(propertyName, out JsonElement element))
            {
                if (element.ValueKind == JsonValueKind.Number && element.TryGetInt64(out long numeric))
                {
                    return numeric;
                }

                if (element.ValueKind == JsonValueKind.String)
                {
                    string? text = element.GetString();
                    if (!string.IsNullOrWhiteSpace(text) && long.TryParse(text, NumberStyles.Integer, CultureInfo.InvariantCulture, out long parsed))
                    {
                        return parsed;
                    }
                }
            }

            return 0;
        }

        private static bool GetBooleanProperty(JsonElement root, string propertyName)
        {
            if (root.TryGetProperty(propertyName, out JsonElement element))
            {
                return element.ValueKind switch
                {
                    JsonValueKind.True => true,
                    JsonValueKind.False => false,
                    JsonValueKind.String => bool.TryParse(element.GetString(), out bool parsed) && parsed,
                    _ => false
                };
            }

            return false;
        }

        internal static string SanitizeFileComponent(string? value, string fallback)
        {
            if (string.IsNullOrWhiteSpace(value))
            {
                return fallback;
            }

            string trimmed = value.Trim();
            char[] invalid = Path.GetInvalidFileNameChars();
            var builder = new StringBuilder(trimmed.Length);
            foreach (char ch in trimmed)
            {
                if (Array.IndexOf(invalid, ch) >= 0 || char.IsControl(ch))
                {
                    builder.Append('_');
                }
                else
                {
                    builder.Append(ch);
                }
            }

            string sanitized = builder.ToString().Trim();
            if (string.IsNullOrWhiteSpace(sanitized))
            {
                return fallback;
            }

            while (!string.IsNullOrEmpty(sanitized) && sanitized[^1] == '.')
            {
                sanitized = sanitized[..^1];
            }

            if (string.IsNullOrWhiteSpace(sanitized))
            {
                return fallback;
            }

            return sanitized;
        }

        internal readonly record struct GatherLogsMetadata(
            string? LogPath,
            long FileSize,
            long BytesToTransfer,
            long FileStartOffset,
            bool Truncated,
            string? SummaryLogPath,
            long SummaryFileSize,
            long SummaryBytesToTransfer,
            long SummaryFileStartOffset,
            bool SummaryTruncated);

        internal readonly record struct GatheredSnapshotData(
            string Base64Content,
            string? ArchiveFileName,
            string? SaveFolder,
            long ArchiveSize,
            long BytesRead,
            bool Truncated);

        internal static string ResolveDeviceExecutablePath()
        {
            string baseDirectory = ResolveBaseDirectory();
            
            // Candidate relative paths in priority order (Release before Debug, new x64 layout before legacy Gaming.Desktop.x64)
            string[] candidateRelPaths = new[]
            {
                Path.Combine("..", "..", "..", "x64", "Release", "GameTestAppWindows", "GameTestAppWindows.exe"),
                Path.Combine("..", "..", "..", "x64", "Debug", "GameTestAppWindows", "GameTestAppWindows.exe"),
                Path.Combine("..", "..", "..", "Gaming.Desktop.x64", "Release", "GameTestAppWindows", "GameTestAppWindows.exe"),
                Path.Combine("..", "..", "..", "Gaming.Desktop.x64", "Debug", "GameTestAppWindows", "GameTestAppWindows.exe"),
            };

            // Resolve to absolute and collect those that exist
            List<string> found = new();
            foreach (string relPath in candidateRelPaths)
            {
                string fullPath = Path.GetFullPath(Path.Combine(baseDirectory, relPath));
                if (File.Exists(fullPath))
                {
                    found.Add(fullPath);
                }
            }

            if (found.Count == 0)
            {
                // Return the first candidate so the caller gets a meaningful "not found" path in the error message
                return Path.GetFullPath(Path.Combine(baseDirectory, candidateRelPaths[0]));
            }

            if (found.Count == 1)
            {
                return found[0];
            }

            // Multiple candidates exist — pick the most recently built one
            return found.OrderByDescending(p => File.GetLastWriteTimeUtc(p)).First();
        }

        internal static string ResolveBaseDirectory()
        {
            string? baseDirectory = AppContext.BaseDirectory;
            if (string.IsNullOrEmpty(baseDirectory))
            {
                baseDirectory = AppDomain.CurrentDomain.BaseDirectory;
            }

            if (string.IsNullOrEmpty(baseDirectory))
            {
                baseDirectory = Environment.CurrentDirectory;
            }

            return baseDirectory!;
        }

        /// <summary>
        /// Finds all scenario files in the specified directory that have the given tag.
        /// </summary>
        public async Task<List<string>> FindScenariosByTagAsync(string scenariosPath, string tag, IReadOnlyList<string>? excludeTags = null, string? platformFilter = null)
        {
            if (string.IsNullOrWhiteSpace(scenariosPath))
            {
                throw new ArgumentException("Scenarios path must be provided", nameof(scenariosPath));
            }

            if (string.IsNullOrWhiteSpace(tag))
            {
                throw new ArgumentException("Tag must be provided", nameof(tag));
            }

            if (!Directory.Exists(scenariosPath))
            {
                Log($"Scenarios directory not found: {scenariosPath}");
                return new List<string>();
            }

            var matchingScenarios = new List<string>();
            var yamlFiles = Directory.GetFiles(scenariosPath, "*.yml", SearchOption.AllDirectories)
                .Concat(Directory.GetFiles(scenariosPath, "*.yaml", SearchOption.AllDirectories))
                .Distinct()
                .OrderBy(f => f);

            var loader = new ScenarioManifestLoader();
            bool usePlatformStatus = !string.IsNullOrEmpty(platformFilter);

            foreach (var file in yamlFiles)
            {
                try
                {
                    var manifest = await loader.LoadAsync(file, (_, _, _) => { }).ConfigureAwait(false);
                    if (manifest == null)
                    {
                        continue;
                    }

                    bool matches;
                    bool excluded;

                    if (usePlatformStatus)
                    {
                        // When --platform-filter is set, match/exclude based on platform status
                        string status = manifest.GetPlatformStatus(platformFilter!);
                        matches = string.Equals(status, tag, StringComparison.OrdinalIgnoreCase);
                        excluded = excludeTags != null && excludeTags.Count > 0 &&
                            excludeTags.Any(et => string.Equals(status, et, StringComparison.OrdinalIgnoreCase));
                    }
                    else
                    {
                        // Legacy behavior: match/exclude based on tags list
                        matches = manifest.Tags != null &&
                            manifest.Tags.Any(t => string.Equals(t, tag, StringComparison.OrdinalIgnoreCase));
                        excluded = excludeTags != null && excludeTags.Count > 0 && manifest.Tags != null &&
                            excludeTags.Any(et =>
                                manifest.Tags.Any(t => string.Equals(t, et, StringComparison.OrdinalIgnoreCase)));
                    }

                    if (matches)
                    {
                        if (excluded)
                        {
                            var matchedExclude = excludeTags!.FirstOrDefault(et =>
                                usePlatformStatus
                                    ? string.Equals(manifest.GetPlatformStatus(platformFilter!), et, StringComparison.OrdinalIgnoreCase)
                                    : manifest.Tags!.Any(t => string.Equals(t, et, StringComparison.OrdinalIgnoreCase)));
                            Log($"  Excluding ('{matchedExclude}'): {Path.GetFileName(file)}");
                            continue;
                        }
                        matchingScenarios.Add(file);
                    }
                }
                catch
                {
                    // Skip files that can't be parsed
                }
            }

            return matchingScenarios;
        }

        /// <summary>
        /// Runs multiple scenarios and returns aggregated results.
        /// </summary>
        public async Task<TestRunResult> RunScenariosAsync(
            IEnumerable<string> scenarioPaths,
            bool autoLaunchDevices = true,
            bool stopOnFirstError = false,
            int skipScenarios = 0,
            string? customIdPrefix = null,
            string? localAppPath = null,
            string? localAppArgs = null,
            string? platformFilter = null,
            CancellationToken cancellationToken = default)
        {
            var result = new TestRunResult
            {
                StartTime = DateTime.UtcNow
            };

            bool perScenarioLifecycle = !string.IsNullOrWhiteSpace(localAppPath);
            bool hasRemoteAgents = _transport.GetConnectedAgents().Count > 0;

            var paths = scenarioPaths.ToList();
            if (skipScenarios > 0)
            {
                Log($"Skipping first {skipScenarios} scenario(s)...");
            }
            Log($"Running {paths.Count} scenario(s){(skipScenarios > 0 ? $" (skipping first {skipScenarios})" : "")}{(perScenarioLifecycle ? " [per-scenario app lifecycle]" : "")}...");
            Log("");

            _deviceReconnectExhausted = false;

            int scenarioIndex = 0;
            foreach (var path in paths)
            {
                scenarioIndex++;
                cancellationToken.ThrowIfCancellationRequested();

                if (scenarioIndex <= skipScenarios)
                {
                    // Load manifest to get name for skip reporting
                    var skippedResult = new ScenarioResult
                    {
                        Id = Path.GetFileNameWithoutExtension(path),
                        Status = TestStatus.Skipped,
                        Error = "Skipped (--skip-scenarios)"
                    };
                    try
                    {
                        var skipLoader = new ScenarioManifestLoader();
                        var skipManifest = await skipLoader.LoadAsync(path, (_, _, _) => { }).ConfigureAwait(false);
                        if (skipManifest != null)
                        {
                            skippedResult.Id = skipManifest.Id ?? skippedResult.Id;
                            skippedResult.Name = skipManifest.Name;
                        }
                    }
                    catch { /* best effort */ }
                    result.Scenarios.Add(skippedResult);
                    result.Summary.Total++;
                    result.Summary.Skipped++;
                    Log($"[{scenarioIndex}/{paths.Count}] SKIPPED: {skippedResult.Name ?? skippedResult.Id} (--skip-scenarios)");
                    continue;
                }

                var scenarioResult = new ScenarioResult
                {
                    Id = Path.GetFileNameWithoutExtension(path),
                    StartTime = DateTime.UtcNow
                };

                var stopwatch = Stopwatch.StartNew();

                // Declared outside try so the finally block can access them for cleanup
                Process? localAppProcess = null;
                HashSet<Guid>? preScenarioDeviceIds = null;
                bool scenarioLifecycleStarted = false;

                try
                {
                    // Load manifest to get metadata
                    var loader = new ScenarioManifestLoader();
                    var manifest = await loader.LoadAsync(path, (_, _, _) => { }).ConfigureAwait(false);
                    if (manifest != null)
                    {
                        scenarioResult.Id = manifest.Id ?? scenarioResult.Id;
                        scenarioResult.Name = manifest.Name;
                        scenarioResult.Tags = manifest.Tags?.ToList();
                    }

                    string scenarioDisplayName = scenarioResult.Name ?? scenarioResult.Id;

                    // Check if scenario is explicitly marked as skip
                    if (manifest != null && manifest.Skip == true)
                    {
                        stopwatch.Stop();
                        scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;
                        scenarioResult.Status = TestStatus.Skipped;
                        scenarioResult.Error = "Marked skip: true in YAML";
                        result.Summary.Skipped++;
                        result.Summary.Total++;
                        result.Scenarios.Add(scenarioResult);
                        Log($"[{scenarioIndex}/{paths.Count}] SKIPPED: {scenarioDisplayName} — skip: true");
                        continue;
                    }

                    if (manifest != null &&
                        !string.IsNullOrWhiteSpace(platformFilter) &&
                        string.Equals(manifest.GetPlatformStatus(platformFilter), "ignore", StringComparison.OrdinalIgnoreCase))
                    {
                        stopwatch.Stop();
                        scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;
                        scenarioResult.Status = TestStatus.Skipped;
                        scenarioResult.Error = $"Platform '{platformFilter}' is marked ignore in YAML";
                        result.Summary.Skipped++;
                        result.Summary.Total++;
                        result.Scenarios.Add(scenarioResult);
                        Log($"[{scenarioIndex}/{paths.Count}] SKIPPED: {scenarioDisplayName} — {platformFilter}: ignore");
                        continue;
                    }

                    // Check if scenario requires more devices than are available.
                    // Only applies when using per-scenario lifecycle or remote agents, where
                    // the available device count is known upfront. In traditional mode (no
                    // --local-app-path, no --remote-devices), devices connect externally and
                    // autoLaunchDevices handles discovery — skip this check.
                    if (perScenarioLifecycle || hasRemoteAgents)
                    {
                        int requiredDeviceCount = manifest?.Devices.Count ?? 1;
                        int availableDeviceCount = (perScenarioLifecycle ? 1 : 0) + _transport.GetConnectedAgents().Count;
                        if (requiredDeviceCount > availableDeviceCount)
                        {
                            stopwatch.Stop();
                            scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;
                            scenarioResult.Status = TestStatus.Skipped;
                            scenarioResult.Error = $"Requires {requiredDeviceCount} device(s) but only {availableDeviceCount} available";
                            result.Summary.Skipped++;
                            result.Summary.Total++;
                            result.Scenarios.Add(scenarioResult);
                            Log($"[{scenarioIndex}/{paths.Count}] SKIPPED: {scenarioDisplayName} — requires {requiredDeviceCount} device(s), {availableDeviceCount} available");
                            continue;
                        }
                    }

                    Log($"[{scenarioIndex}/{paths.Count}] Starting: {scenarioDisplayName}");

                    // Per-scenario app lifecycle: launch local + remote games before each scenario
                    if (perScenarioLifecycle || hasRemoteAgents)
                    {
                        scenarioLifecycleStarted = true;

                        // Snapshot device IDs before launch so we can identify scenario-owned devices later
                        preScenarioDeviceIds = _transport.GetConnectedDeviceDetails()
                            .Select(d => d.ClientId).ToHashSet();
                    }

                    if (perScenarioLifecycle)
                    {
                        // Launch local app
                        Log($"Launching local app: {localAppPath}");
                        var startInfo = new ProcessStartInfo
                        {
                            FileName = localAppPath!,
                            WorkingDirectory = Path.GetDirectoryName(localAppPath!) ?? ".",
                            UseShellExecute = false
                        };
                        if (!string.IsNullOrWhiteSpace(localAppArgs))
                        {
                            startInfo.Arguments = localAppArgs;
                        }
                        localAppProcess = Process.Start(startInfo);
                        if (localAppProcess == null)
                        {
                            stopwatch.Stop();
                            scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;
                            Log("ERROR: Failed to launch local app.");
                            scenarioResult.Status = TestStatus.Failed;
                            scenarioResult.Error = "Failed to launch local app";
                            result.Summary.Failed++;
                            result.Summary.Total++;
                            result.Scenarios.Add(scenarioResult);
                            continue;
                        }
                        Log($"Local app launched (PID {localAppProcess.Id}).");
                    }

                    if (hasRemoteAgents)
                    {
                        // Launch remote apps via agents and wait for all devices to connect.
                        // If an agent reconnects during the wait (e.g., after a transient disconnect),
                        // re-issue the launch command so the remote game can still join in time.
                        int requiredDevices = manifest?.Devices.Count ?? 1;
                        await LaunchRemoteAppsAndWaitForDevicesAsync(requiredDevices, 60, cancellationToken).ConfigureAwait(false);
                    }
                    else if (perScenarioLifecycle)
                    {
                        // No remote agents — wait for the locally launched app to connect.
                        int requiredDevices = manifest?.Devices.Count ?? 1;
                        Log($"Waiting up to 60s for {requiredDevices} device(s) to connect...");
                        bool devicesConnected = await WaitForDevicesAsync(requiredDevices, 60).ConfigureAwait(false);
                        if (devicesConnected)
                        {
                            Log($"All {requiredDevices} device(s) connected.");
                        }
                    }

                    // Per-scenario customId isolation: append the scenario index so each scenario
                    // gets a fresh PlayFab entity for any customId-based Login. This prevents
                    // cross-scenario cloud-state contamination (e.g., leaked "active device" claims
                    // from a failed scenario breaking subsequent AddUserWithUi calls).
                    string? perScenarioPrefix = string.IsNullOrWhiteSpace(customIdPrefix)
                        ? customIdPrefix
                        : $"{customIdPrefix}_s{scenarioIndex:D3}_";

                    // Run the scenario
                    var outcome = await LoadScenarioAsync(path, autoLaunchDevices && !perScenarioLifecycle, perScenarioPrefix).ConfigureAwait(false);

                    stopwatch.Stop();
                    scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;

                    switch (outcome)
                    {
                        case ScenarioRunOutcome.Passed:
                            scenarioResult.Status = TestStatus.Passed;
                            result.Summary.Passed++;
                            Log($"[{scenarioIndex}/{paths.Count}] PASSED: {scenarioResult.Name ?? scenarioResult.Id} ({scenarioResult.DurationSeconds:F1}s)");

                            // Reset Xbox device after success if it was used, to prevent
                            // state leakage (Connected Storage, save folders) between tests.
                            if (_scenarioRunner.LastScenarioUsedXboxDevice)
                            {
                                await ResetXboxDeviceForNextScenarioAsync().ConfigureAwait(false);
                            }
                            break;
                        case ScenarioRunOutcome.Failed:
                            scenarioResult.Status = TestStatus.Failed;
                            scenarioResult.Error = "Scenario failed";
                            result.Summary.Failed++;
                            Log($"[{scenarioIndex}/{paths.Count}] FAILED: {scenarioResult.Name ?? scenarioResult.Id} ({scenarioResult.DurationSeconds:F1}s)");

                            // Reset Xbox device after failure to prevent cascade failures.
                            // A failed scenario may leave the Xbox app in a broken state
                            // (e.g., corrupted runtime, hung user session) that would cause
                            // all subsequent Xbox tests to timeout and fail.
                            await ResetXboxDeviceForNextScenarioAsync().ConfigureAwait(false);
                            break;
                        case ScenarioRunOutcome.NotStarted:
                            scenarioResult.Status = TestStatus.Skipped;
                            string skipReason = _scenarioRunner.LastSkipReason ?? "Scenario could not be started";
                            scenarioResult.Error = skipReason;
                            result.Summary.Skipped++;
                            Log($"[{scenarioIndex}/{paths.Count}] SKIPPED: {scenarioResult.Name ?? scenarioResult.Id} — {skipReason}");
                            break;
                    }

                    // When per-scenario lifecycle is active, devices are already killed above.
                    // Otherwise, PC devices are left running between tests for reuse.
                }
                catch (OperationCanceledException)
                {
                    stopwatch.Stop();
                    scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;
                    scenarioResult.Status = TestStatus.Skipped;
                    scenarioResult.Error = "Cancelled";
                    result.Summary.Skipped++;
                    Log($"[{scenarioIndex}/{paths.Count}] CANCELLED: {scenarioResult.Name ?? scenarioResult.Id}");
                    throw;
                }
                catch (Exception ex)
                {
                    stopwatch.Stop();
                    scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;
                    scenarioResult.Status = TestStatus.Failed;
                    scenarioResult.Error = ex.Message;
                    result.Summary.Failed++;
                    Log($"[{scenarioIndex}/{paths.Count}] ERROR: {scenarioResult.Name ?? scenarioResult.Id}: {ex.Message}");

                    // Reset Xbox after exception too (e.g., TimeoutException from EnsureXboxDeviceAsync)
                    await ResetXboxDeviceForNextScenarioAsync().ConfigureAwait(false);
                }
                finally
                {
                    // Per-scenario app lifecycle: always kill local + remote games after each scenario,
                    // even if the scenario threw an exception or was cancelled.
                    if (scenarioLifecycleStarted && hasRemoteAgents)
                    {
                        // Kill remote apps via agents
                        if (_transport.GetConnectedAgents().Count > 0)
                        {
                            Log("Killing remote test apps...");
                            try { await KillAllAgentTestAppsAsync().ConfigureAwait(false); } catch { }
                        }
                    }

                    if (scenarioLifecycleStarted && perScenarioLifecycle && localAppProcess != null)
                    {
                        // Kill local app (entire process tree to catch UE child processes)
                        if (!localAppProcess.HasExited)
                        {
                            Log($"Killing local app (PID {localAppProcess.Id})...");
                            try
                            {
                                localAppProcess.Kill(entireProcessTree: true);
                                localAppProcess.WaitForExit(5000);
                            }
                            catch { }
                        }
                        localAppProcess.Dispose();
                    }

                    if (scenarioLifecycleStarted && (perScenarioLifecycle || hasRemoteAgents))
                    {
                        // Force-disconnect devices that connected during this scenario
                        var currentDevices = _transport.GetConnectedDeviceDetails();
                        foreach (var device in currentDevices)
                        {
                            if (preScenarioDeviceIds == null || !preScenarioDeviceIds.Contains(device.ClientId))
                            {
                                _transport.ForceDisconnectDevice(device.ClientId);
                            }
                        }

                        // Reset device-role assignments from the finished scenario
                        try { await _transport.ResetDeviceAssignmentsAsync().ConfigureAwait(false); } catch { }

                        // Poll until scenario devices are fully cleared (max 5s)
                        for (int waitMs = 0; waitMs < 5000; waitMs += 250)
                        {
                            var remaining = _transport.GetConnectedDeviceDetails()
                                .Where(d => d.IsConnected)
                                .Where(d => preScenarioDeviceIds == null || !preScenarioDeviceIds.Contains(d.ClientId))
                                .ToList();
                            if (remaining.Count == 0) break;
                            try { await Task.Delay(250, cancellationToken).ConfigureAwait(false); } catch { break; }
                        }
                    }
                }

                result.Scenarios.Add(scenarioResult);
                result.Summary.Total++;

                // If any devices were force-disconnected during this scenario, restart them
                // so subsequent scenarios don't get skipped due to missing devices.
                // Skip in per-scenario lifecycle mode or when remote agents manage device
                // launches — apps are already killed and relaunched each scenario.
                if (_scenarioRunner.LastForcedDisconnectedDevices.Count > 0 && !perScenarioLifecycle && !hasRemoteAgents)
                {
                    await RestartForcedDisconnectedDevicesAsync(
                        _scenarioRunner.LastForcedDisconnectedDevices, cancellationToken).ConfigureAwait(false);
                }

                if (stopOnFirstError && scenarioResult.Status == TestStatus.Failed)
                {
                    Log($"Stopping early (--stop-on-first-error). Skipping remaining {paths.Count - scenarioIndex} scenario(s).");
                    // Mark remaining scenarios as skipped
                    for (int remaining = scenarioIndex; remaining < paths.Count; remaining++)
                    {
                        var skippedPath = paths[remaining];
                        var skipped = new ScenarioResult
                        {
                            Id = Path.GetFileNameWithoutExtension(skippedPath),
                            Status = TestStatus.Skipped,
                            Error = "Skipped (stop-on-first-error)"
                        };
                        // Try to load the name from manifest
                        try
                        {
                            var loader2 = new ScenarioManifestLoader();
                            var manifest2 = await loader2.LoadAsync(skippedPath, (_, _, _) => { }).ConfigureAwait(false);
                            if (manifest2 != null)
                            {
                                skipped.Id = manifest2.Id ?? skipped.Id;
                                skipped.Name = manifest2.Name;
                            }
                        }
                        catch { /* best effort */ }
                        result.Scenarios.Add(skipped);
                        result.Summary.Total++;
                        result.Summary.Skipped++;
                    }
                    break;
                }
            }

            // Gather device logs once at the end of the full run instead of
            // after every scenario.  Per-scenario gathering still happens on
            // failure so that logs are available for diagnosis even if the run
            // is aborted early.
            if (!string.IsNullOrWhiteSpace(_defaultLogDirectory) && result.Summary.Failed == 0)
            {
                try
                {
                    Log("Gathering device logs at end of test run...");
                    await GatherLogsAsync(_defaultLogDirectory!, cancellationToken).ConfigureAwait(false);
                }
                catch (OperationCanceledException) when (!cancellationToken.IsCancellationRequested)
                {
                    Log("End-of-run log gathering was cancelled.");
                }
                catch (Exception ex)
                {
                    Log($"End-of-run log gathering failed: {ex.Message}");
                }
            }

            Log("");
            result.EndTime = DateTime.UtcNow;
            return result;
        }
    }
}
