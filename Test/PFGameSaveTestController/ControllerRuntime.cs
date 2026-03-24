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

namespace PFGameSaveTestController
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

        public event Action<IReadOnlyCollection<DeviceConnectionInfo>>? ConnectedDevicesChanged;

        public ControllerTransport Transport => _transport;

        public ControllerRuntime(Action<string, bool, bool> logger, Func<ChaosModeScenarioParameters>? chaosSettingsProvider = null, Func<string?>? sourceDataFolderProvider = null)
        {
            _logger = logger;
            _chaosSettingsProvider = chaosSettingsProvider;
            _sourceDataFolderProvider = sourceDataFolderProvider;
            _transport = new ControllerTransport(Log);
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
                Log("  2. Register URL ACL (run as Admin once): netsh http add urlacl url=http://+:5000/ws/ user=Everyone");
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

        public IReadOnlyCollection<DeviceConnectionInfo> GetConnectedDevices()
        {
            return _transport.GetConnectedDeviceDetails();
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
                    ["scid"] = "00000000-0000-0000-0000-000076b1590e"
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
                    bool devicesReady = await EnsureRequiredDevicesAsync(plan).ConfigureAwait(false);
                    if (!devicesReady)
                    {
                        Log("Auto-launch failed to prepare required devices.");
                    }
                }

                return await _scenarioRunner.RunAsync(plan, effectivePrefix).ConfigureAwait(false);
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

            Log($"[{deviceName}] WebSocket Text: {message}", verbose: true);
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

                if (!TryExtractGatheredLogData(result.RawJson, out GatheredLogData data))
                {
                    Log($"GatherLogs for '{deviceName}' returned an unexpected payload; no log was saved.");
                    return;
                }

                string sanitizedDeviceName = SanitizeFileComponent(deviceName, "device");
                string outputFileName = $"pfgamesaves-device-{sanitizedDeviceName}-log.txt";
                string outputPath = Path.Combine(logDirectory, outputFileName);

                await File.WriteAllTextAsync(outputPath, data.LogContent, cancellationToken).ConfigureAwait(false);

                string truncatedSuffix = data.Truncated ? " (truncated)" : string.Empty;
                string message = $"GatherLogs saved {data.BytesRead} of {data.FileSize} bytes from '{deviceName}' to '{outputPath}'{truncatedSuffix}.";
                if (!string.IsNullOrWhiteSpace(data.SourcePath))
                {
                    message += $" Source: {data.SourcePath}.";
                }

                Log(message);

                // Save summary log if available
                if (!string.IsNullOrWhiteSpace(data.SummaryLogContent))
                {
                    string summaryOutputFileName = $"pfgamesaves-device-{sanitizedDeviceName}-summary.txt";
                    string summaryOutputPath = Path.Combine(logDirectory, summaryOutputFileName);

                    await File.WriteAllTextAsync(summaryOutputPath, data.SummaryLogContent, cancellationToken).ConfigureAwait(false);

                    string summaryTruncatedSuffix = data.SummaryTruncated ? " (truncated)" : string.Empty;
                    string summaryMessage = $"GatherLogs saved summary {data.SummaryBytesRead} of {data.SummaryFileSize} bytes from '{deviceName}' to '{summaryOutputPath}'{summaryTruncatedSuffix}.";
                    if (!string.IsNullOrWhiteSpace(data.SummarySourcePath))
                    {
                        summaryMessage += $" Source: {data.SummarySourcePath}.";
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

                string outputFileName = $"pfgamesaves-device-{sanitizedDeviceName}-{sanitizedArchiveName}";
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

            List<string> launchOrder = BuildRoleOrder(plan, required);
            if (launchOrder.Count == 0)
            {
                return true;
            }

            string deviceExecutable = ResolveDeviceExecutablePath();
            if (!File.Exists(deviceExecutable))
            {
                Log($"Auto-launch enabled, but device executable was not found at '{deviceExecutable}'.");
                return false;
            }

            foreach (string role in launchOrder)
            {
                if (string.IsNullOrWhiteSpace(role))
                {
                    continue;
                }

                if (_transport.IsDeviceConnected(role))
                {
                    continue;
                }

                Log($"Auto-launching local device for role '{role}' using '{deviceExecutable}'.");

                try
                {
                    var startInfo = new ProcessStartInfo
                    {
                        FileName = deviceExecutable,
                        WorkingDirectory = Path.GetDirectoryName(deviceExecutable) ?? ResolveBaseDirectory(),
                        UseShellExecute = false
                    };
                    Process? launched = Process.Start(startInfo);
                    if (launched == null)
                    {
                        Log($"Failed to auto-launch device for role '{role}': process did not start.");
                        return false;
                    }
                    launched.Dispose();
                }
                catch (Exception ex)
                {
                    Log($"Failed to auto-launch device for role '{role}': {ex.Message}");
                    return false;
                }

                bool connected = await WaitForDeviceConnectionAsync(role, TimeSpan.FromSeconds(30)).ConfigureAwait(false);
                if (!connected)
                {
                    Log($"Auto-launched device for role '{role}' did not connect within the timeout.");
                    return false;
                }

                Log($"Device '{role}' connected.", true);
            }

            return true;
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

        internal static bool TryExtractGatheredLogData(string rawJson, out GatheredLogData data)
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

            if (!root.TryGetProperty("logContent", out JsonElement contentElement) || contentElement.ValueKind != JsonValueKind.String)
            {
                return false;
            }

            string logContent = contentElement.GetString() ?? string.Empty;
            string? sourcePath = null;
            if (root.TryGetProperty("logPath", out JsonElement pathElement) && pathElement.ValueKind == JsonValueKind.String)
            {
                sourcePath = pathElement.GetString();
            }

            long fileSize = GetInt64Property(root, "fileSize");
            long bytesRead = GetInt64Property(root, "bytesRead");
            bool truncated = GetBooleanProperty(root, "truncated");

            // Extract summary log fields (optional)
            string? summaryLogContent = null;
            if (root.TryGetProperty("summaryLogContent", out JsonElement summaryContentElement) && summaryContentElement.ValueKind == JsonValueKind.String)
            {
                summaryLogContent = summaryContentElement.GetString();
            }

            string? summarySourcePath = null;
            if (root.TryGetProperty("summaryLogPath", out JsonElement summaryPathElement) && summaryPathElement.ValueKind == JsonValueKind.String)
            {
                summarySourcePath = summaryPathElement.GetString();
            }

            long summaryFileSize = GetInt64Property(root, "summaryFileSize");
            long summaryBytesRead = GetInt64Property(root, "summaryBytesRead");
            bool summaryTruncated = GetBooleanProperty(root, "summaryTruncated");

            data = new GatheredLogData(logContent, fileSize, bytesRead, truncated, sourcePath,
                summaryLogContent, summaryFileSize, summaryBytesRead, summaryTruncated, summarySourcePath);
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

        internal readonly record struct GatheredLogData(
            string LogContent,
            long FileSize,
            long BytesRead,
            bool Truncated,
            string? SourcePath,
            string? SummaryLogContent,
            long SummaryFileSize,
            long SummaryBytesRead,
            bool SummaryTruncated,
            string? SummarySourcePath);

        internal readonly record struct GatheredSnapshotData(
            string Base64Content,
            string? ArchiveFileName,
            string? SaveFolder,
            long ArchiveSize,
            long BytesRead,
            bool Truncated);

        private static string ResolveDeviceExecutablePath()
        {
            string baseDirectory = ResolveBaseDirectory();
            
            // Check Release build first (newer or equal), fallback to Debug
            string releaseRelPath = Path.Combine("..", "..", "..", "Gaming.Desktop.x64", "Release", "PFGameSaveTestDeviceWindows", "PFGameSaveTestDeviceWindows.exe");
            string debugRelPath = Path.Combine("..", "..", "..", "Gaming.Desktop.x64", "Debug", "PFGameSaveTestDeviceWindows", "PFGameSaveTestDeviceWindows.exe");
            
            string releasePath = Path.GetFullPath(Path.Combine(baseDirectory, releaseRelPath));
            string debugPath = Path.GetFullPath(Path.Combine(baseDirectory, debugRelPath));
            
            // If both exist, compare timestamps and default to Release if similar
            if (File.Exists(releasePath) && File.Exists(debugPath))
            {
                var releaseTime = File.GetLastWriteTimeUtc(releasePath);
                var debugTime = File.GetLastWriteTimeUtc(debugPath);
                
                // Use Release if newer or within 10 seconds (default to Release)
                return (releaseTime >= debugTime.AddSeconds(-10)) ? releasePath : debugPath;
            }
            
            // Use whichever exists, prefer Release
            return File.Exists(releasePath) ? releasePath : debugPath;
        }

        private static string ResolveBaseDirectory()
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
        public async Task<List<string>> FindScenariosByTagAsync(string scenariosPath, string tag)
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

            foreach (var file in yamlFiles)
            {
                try
                {
                    var manifest = await loader.LoadAsync(file, (_, _, _) => { }).ConfigureAwait(false);
                    if (manifest != null && manifest.Tags != null &&
                        manifest.Tags.Any(t => string.Equals(t, tag, StringComparison.OrdinalIgnoreCase)))
                    {
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
            CancellationToken cancellationToken = default)
        {
            var result = new TestRunResult
            {
                StartTime = DateTime.UtcNow
            };

            var paths = scenarioPaths.ToList();
            Log($"Running {paths.Count} scenario(s)...");
            Log("");

            int scenarioIndex = 0;
            foreach (var path in paths)
            {
                scenarioIndex++;
                cancellationToken.ThrowIfCancellationRequested();

                var scenarioResult = new ScenarioResult
                {
                    Id = Path.GetFileNameWithoutExtension(path),
                    StartTime = DateTime.UtcNow
                };

                var stopwatch = Stopwatch.StartNew();

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
                    Log($"[{scenarioIndex}/{paths.Count}] Starting: {scenarioDisplayName}");

                    // Run the scenario
                    var outcome = await LoadScenarioAsync(path, autoLaunchDevices).ConfigureAwait(false);

                    stopwatch.Stop();
                    scenarioResult.DurationSeconds = stopwatch.Elapsed.TotalSeconds;

                    switch (outcome)
                    {
                        case ScenarioRunOutcome.Passed:
                            scenarioResult.Status = TestStatus.Passed;
                            result.Summary.Passed++;
                            Log($"[{scenarioIndex}/{paths.Count}] PASSED: {scenarioResult.Name ?? scenarioResult.Id} ({scenarioResult.DurationSeconds:F1}s)");
                            break;
                        case ScenarioRunOutcome.Failed:
                            scenarioResult.Status = TestStatus.Failed;
                            scenarioResult.Error = "Scenario failed";
                            result.Summary.Failed++;
                            Log($"[{scenarioIndex}/{paths.Count}] FAILED: {scenarioResult.Name ?? scenarioResult.Id} ({scenarioResult.DurationSeconds:F1}s)");
                            break;
                        case ScenarioRunOutcome.NotStarted:
                            scenarioResult.Status = TestStatus.Skipped;
                            scenarioResult.Error = "Scenario could not be started";
                            result.Summary.Skipped++;
                            Log($"[{scenarioIndex}/{paths.Count}] SKIPPED: {scenarioResult.Name ?? scenarioResult.Id}");
                            break;
                    }
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
                }

                result.Scenarios.Add(scenarioResult);
                result.Summary.Total++;
            }

            Log("");
            result.EndTime = DateTime.UtcNow;
            return result;
        }
    }
}
