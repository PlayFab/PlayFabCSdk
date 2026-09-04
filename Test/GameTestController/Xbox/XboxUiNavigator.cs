using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using OpenCvSharp;

namespace GameTestController.Xbox
{
    /// <summary>
    /// Monitors the Xbox screen for PFGameSave TCUI dialogs using screenshot capture
    /// and OpenCV template matching, then responds with virtual gamepad input.
    /// This is the Xbox equivalent of <see cref="GameSaveUiNavigator"/> (which uses
    /// Windows UI Automation for PC dialogs).
    /// </summary>
    internal sealed class XboxUiNavigator : IDisposable
    {
        private readonly string _xboxAddress;
        private readonly Action<string> _log;
        private readonly Func<string?>? _logDirectoryProvider;
        private readonly Dictionary<GameSaveUiNavigator.GameSaveDialogType, string> _responseActions = new();
        private readonly List<GameSaveUiNavigator.DialogEvent> _eventLog = new();
        private readonly object _eventLogLock = new();

        // Set true once we've saved an unmatched-dialog screenshot for this navigator
        // instance (one per test run), so the ~1.5s scan loop doesn't flood the log
        // folder with near-identical PNGs during a long hang.
        private bool _savedUnmatchedThisPass;

        /// <summary>
        /// Minimum best-candidate confidence for an unmatched screen to be worth capturing.
        /// A visible-but-unmatched dialog scores in a middle band (e.g. ~0.44); plain
        /// gameplay scores far lower, so this floor avoids dropping a useless PNG in every
        /// test's log folder while still catching genuine unmatched dialogs.
        /// </summary>
        private const double UnmatchedSaveFloor = 0.30;

        private XboxScreenshotCapture? _screenshotCapture;
        private XboxDialogDetector? _detector;
        private XboxGamepadInput? _gamepad;
        private Thread? _monitorThread;
        private CancellationTokenSource? _cts;
        private bool _disposed;

        // Suppress repeated log messages for same state
        private string? _lastDetectedState;

        /// <summary>
        /// Interval between screenshot scans in milliseconds. Xbox screenshots are heavier
        /// than local UI Automation scans, so we use a longer interval.
        /// </summary>
        public int ScanIntervalMs { get; set; } = 1500;

        /// <summary>
        /// Delay after sending gamepad input before resuming scan, to allow the UI to react.
        /// </summary>
        public int PostResponseDelayMs { get; set; } = 3000;

        /// <summary>
        /// Path to folder containing dialog template images ({DialogType}.png).
        /// </summary>
        public string TemplatesFolder { get; }

        public XboxUiNavigator(string xboxAddress, string templatesFolder, Action<string> log,
            Func<string?>? logDirectoryProvider = null)
        {
            _xboxAddress = xboxAddress ?? throw new ArgumentNullException(nameof(xboxAddress));
            TemplatesFolder = templatesFolder ?? throw new ArgumentNullException(nameof(templatesFolder));
            _log = log ?? throw new ArgumentNullException(nameof(log));
            _logDirectoryProvider = logDirectoryProvider;
        }

        /// <summary>
        /// Configures the auto-response action for a specific dialog type.
        /// </summary>
        public void SetResponse(GameSaveUiNavigator.GameSaveDialogType dialogType, string action)
        {
            _responseActions[dialogType] = action;
            _log($"XboxUiNavigator: Set response for {dialogType} → '{action}'");
        }

        /// <summary>
        /// Applies tracked auto-response settings from the scenario runner.
        /// </summary>
        public void ApplyTrackedResponses(string? role, IReadOnlyDictionary<string, string> trackedAutoResponses)
        {
            var mapping = new (GameSaveUiNavigator.GameSaveDialogType Type, string ShortKey)[]
            {
                (GameSaveUiNavigator.GameSaveDialogType.ActiveDeviceContention, "ActiveDeviceContention"),
                (GameSaveUiNavigator.GameSaveDialogType.Conflict, "Conflict"),
                (GameSaveUiNavigator.GameSaveDialogType.OutOfStorage, "OutOfStorage"),
                (GameSaveUiNavigator.GameSaveDialogType.SyncFailed, "SyncFailed"),
            };

            foreach (var (type, shortKey) in mapping)
            {
                // Try role-prefixed key first (e.g., "DeviceA:Conflict"), then global
                string? action = null;
                if (role != null)
                {
                    trackedAutoResponses.TryGetValue($"{role}:{shortKey}", out action);
                }
                if (action == null)
                {
                    trackedAutoResponses.TryGetValue(shortKey, out action);
                }

                if (action != null)
                {
                    SetResponse(type, action);
                }
            }
        }

        /// <summary>
        /// Starts the background monitoring thread.
        /// </summary>
        public void Start()
        {
            if (_monitorThread != null)
            {
                _log("XboxUiNavigator: Already running.");
                return;
            }

            // Initialize components
            _screenshotCapture = new XboxScreenshotCapture(_xboxAddress, _log);
            _detector = new XboxDialogDetector(TemplatesFolder, _log);
            _gamepad = new XboxGamepadInput(_xboxAddress, _log);

            if (!_gamepad.IsConnected)
            {
                _log("XboxUiNavigator: WARNING — virtual gamepad failed to connect. Will detect but cannot respond.");
            }

            _cts = new CancellationTokenSource();
            _monitorThread = new Thread(MonitorLoop)
            {
                Name = "XboxUiNavigator",
                IsBackground = true
            };
            _monitorThread.Start(_cts.Token);
            _log($"XboxUiNavigator: Started monitoring Xbox at {_xboxAddress} (interval={ScanIntervalMs}ms).");
        }

        /// <summary>
        /// Stops the background monitoring thread.
        /// </summary>
        public void Stop()
        {
            if (_cts == null || _monitorThread == null)
            {
                return;
            }

            _log("XboxUiNavigator: Stopping...");
            _cts.Cancel();

            if (_monitorThread.IsAlive)
            {
                _monitorThread.Join(TimeSpan.FromSeconds(5));
            }

            _cts.Dispose();
            _cts = null;
            _monitorThread = null;

            _gamepad?.Dispose();
            _gamepad = null;
            _screenshotCapture = null;
            _detector = null;

            _log("XboxUiNavigator: Stopped.");
        }

        /// <summary>
        /// Returns recorded dialog events for test verification.
        /// </summary>
        public IReadOnlyList<GameSaveUiNavigator.DialogEvent> GetEventLog()
        {
            lock (_eventLogLock)
            {
                return _eventLog.ToList();
            }
        }

        private void MonitorLoop(object? state)
        {
            var token = (CancellationToken)state!;

            try
            {
                while (!token.IsCancellationRequested)
                {
                    try
                    {
                        ScanAndRespond();
                    }
                    catch (Exception ex)
                    {
                        string errMsg = ex.Message;
                        if (_lastDetectedState != $"error:{errMsg}")
                        {
                            _log($"XboxUiNavigator: Scan error: {errMsg}");
                            _lastDetectedState = $"error:{errMsg}";
                        }
                    }

                    token.WaitHandle.WaitOne(ScanIntervalMs);
                }
            }
            catch (OperationCanceledException)
            {
                // Normal shutdown
            }
            catch (Exception ex)
            {
                _log($"XboxUiNavigator: Monitor thread exiting due to error: {ex.Message}");
            }
        }

        /// <summary>
        /// Single scan pass: capture screenshot → detect dialog → respond with gamepad.
        /// </summary>
        private void ScanAndRespond()
        {
            if (_screenshotCapture == null || _detector == null) return;

            using var screenshot = _screenshotCapture.Capture();
            if (screenshot == null)
            {
                // Capture failed (console rebooting, etc.) — suppress repeated logs
                if (_lastDetectedState != "capture-failed")
                {
                    _log("XboxUiNavigator: Screenshot capture failed (console may be rebooting).");
                    _lastDetectedState = "capture-failed";
                }
                return;
            }

            var match = _detector.Detect(screenshot);

            if (match == null)
            {
                // No template matched. If a dialog is nonetheless visible (best candidate
                // above the floor) but couldn't be identified, save one screenshot per run
                // so it can be inspected later (e.g. to author/refresh a template).
                MaybeSaveUnmatchedDialog(screenshot);

                if (_lastDetectedState != null && _lastDetectedState != "none")
                {
                    _log("XboxUiNavigator: Dialog dismissed (no longer detected).");
                }
                _lastDetectedState = "none";
                return;
            }

            // Dialog detected
            string stateKey = $"detected:{match.DialogType}";
            if (_lastDetectedState == stateKey)
            {
                // Already logged and attempted response for this dialog — don't spam
                return;
            }

            _log($"XboxUiNavigator: Detected {match.DialogType} dialog (confidence={match.Confidence:F3})");
            _lastDetectedState = stateKey;

            // Check if we have a configured response
            if (!_responseActions.TryGetValue(match.DialogType, out string? action))
            {
                _log($"XboxUiNavigator: No auto-response configured for {match.DialogType}. Ignoring.");
                RecordEvent(match.DialogType, "none", responded: false);
                return;
            }

            // Send gamepad response
            bool responded = false;
            if (_gamepad != null && _gamepad.IsConnected)
            {
                responded = _gamepad.RespondToDialog(match.DialogType, action);
                if (responded)
                {
                    _log($"XboxUiNavigator: Responded to {match.DialogType} with '{action}' via gamepad.");
                    // Wait for UI to react before next scan
                    Thread.Sleep(PostResponseDelayMs);
                    // Reset state so we can detect if dialog is still showing
                    _lastDetectedState = "responded";
                }
                else
                {
                    _log($"XboxUiNavigator: Failed to send gamepad response for {match.DialogType}.");
                }
            }
            else
            {
                _log($"XboxUiNavigator: Gamepad not connected — cannot respond to {match.DialogType}.");
            }

            RecordEvent(match.DialogType, action, responded);
        }

        /// <summary>
        /// Saves a single annotated screenshot of an on-screen dialog that failed to match
        /// any template, into the current run's log folder. Capped at one per navigator
        /// instance (one per test run) so a long hang doesn't flood the folder with PNGs.
        /// Skips plain gameplay frames via <see cref="UnmatchedSaveFloor"/>.
        /// </summary>
        private void MaybeSaveUnmatchedDialog(Mat screenshot)
        {
            if (_savedUnmatchedThisPass || _detector == null)
            {
                return;
            }

            var candidate = _detector.LastBestCandidate;
            if (candidate == null || candidate.Confidence < UnmatchedSaveFloor)
            {
                return;
            }

            string? logDir = _logDirectoryProvider?.Invoke();
            if (string.IsNullOrWhiteSpace(logDir))
            {
                // No log directory available (e.g. GUI run) — nothing to write to.
                return;
            }

            try
            {
                Directory.CreateDirectory(logDir!);
                string path = Path.Combine(logDir!, "xbox-unmatched-dialog.png");
                _detector.SaveUnmatched(screenshot, path);
                _savedUnmatchedThisPass = true;
                _log($"XboxUiNavigator: Saved unmatched-dialog screenshot (best={candidate.DialogType} " +
                     $"conf={candidate.Confidence:F3}) to {path}. Further captures suppressed for this run.");
            }
            catch (Exception ex)
            {
                _log($"XboxUiNavigator: Failed to save unmatched-dialog screenshot: {ex.Message}");
            }
        }

        private void RecordEvent(GameSaveUiNavigator.GameSaveDialogType dialogType, string action, bool responded)
        {
            lock (_eventLogLock)
            {
                _eventLog.Add(new GameSaveUiNavigator.DialogEvent(
                    dialogType, action, DateTimeOffset.UtcNow, responded));
            }
        }

        public void Dispose()
        {
            if (_disposed) return;
            _disposed = true;
            Stop();
        }
    }
}
