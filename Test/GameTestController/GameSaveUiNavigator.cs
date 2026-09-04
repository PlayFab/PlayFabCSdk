using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Automation;

namespace GameTestController
{
    /// <summary>
    /// Monitors the local desktop for PFGameSave UI dialogs and automatically
    /// responds to them based on the auto-response actions configured in the scenario.
    /// Uses Windows UI Automation (System.Windows.Automation) to detect and interact with dialogs.
    /// </summary>
    internal sealed class GameSaveUiNavigator : IDisposable
    {
        private readonly Action<string> _log;
        private readonly Dictionary<GameSaveDialogType, string> _responseActions = new();
        private readonly Dictionary<GameSaveDialogType, int> _maxRetries = new();       // -1 = unlimited
        private readonly Dictionary<GameSaveDialogType, int> _retryCount = new();       // current count
        private readonly Dictionary<GameSaveDialogType, string> _fallbackActions = new(); // after maxRetries
        private Thread? _monitorThread;
        private CancellationTokenSource? _cts;
        private bool _disposed;

        // Tracks last-logged state per window to suppress duplicate messages
        private readonly Dictionary<string, string> _lastLoggedState = new();

        // Counts dialogs actually DISMISSED per type, so scenarios can assert how many times a
        // dialog was raised (e.g. ADO 63185188: the offline sync-failure dialog must not re-appear
        // on every save). Distinct from _lastLoggedState, which only de-dupes log spam.
        private readonly Dictionary<GameSaveDialogType, int> _dismissedCount = new();
        private readonly object _dismissedCountLock = new();

        /// <summary>Number of dialogs of <paramref name="type"/> dismissed since the last reset.</summary>
        public int GetDismissedCount(GameSaveDialogType type)
        {
            lock (_dismissedCountLock)
            {
                return _dismissedCount.TryGetValue(type, out int n) ? n : 0;
            }
        }

        /// <summary>Clears all dismissed-dialog counters.</summary>
        public void ResetDismissedCounts()
        {
            lock (_dismissedCountLock)
            {
                _dismissedCount.Clear();
            }
        }

        /// <summary>
        /// The five PFGameSave UI dialog types that can appear during sync operations.
        /// </summary>
        internal enum GameSaveDialogType
        {
            ActiveDeviceContention,
            Conflict,
            OutOfStorage,
            SyncFailed,
            Progress,
            StopOrKeepSyncing
        }

        /// <summary>
        /// Snapshot of a detected dialog — what was found and how we responded.
        /// </summary>
        internal readonly record struct DialogEvent(
            GameSaveDialogType DialogType,
            string Action,
            DateTimeOffset Timestamp,
            bool Responded);

        private readonly List<DialogEvent> _eventLog = new();
        private readonly object _eventLogLock = new();

        public GameSaveUiNavigator(Action<string> log)
        {
            _log = log ?? throw new ArgumentNullException(nameof(log));
        }

        /// <summary>
        /// Configures the auto-response action for a specific dialog type.
        /// Call this before <see cref="Start"/> to set up the desired responses.
        /// </summary>
        /// <param name="dialogType">Which dialog type to configure.</param>
        /// <param name="action">The button action to take (e.g., "Retry", "Cancel").</param>
        /// <param name="maxRetries">Max times to use <paramref name="action"/> before switching to
        /// <paramref name="fallbackAction"/>. -1 means unlimited (default).</param>
        /// <param name="fallbackAction">Action to use after maxRetries is exhausted. Defaults to "Cancel".</param>
        public void SetResponse(GameSaveDialogType dialogType, string action, int maxRetries = -1, string fallbackAction = "Cancel")
        {
            _responseActions[dialogType] = action;
            _maxRetries[dialogType] = maxRetries;
            _fallbackActions[dialogType] = fallbackAction;
            _retryCount[dialogType] = 0;
        }

        /// <summary>
        /// Configures responses from the tracked auto-response state in ScenarioRunner.
        /// Only sets responses that were explicitly configured via YML commands.
        /// Dialogs with no configured response will be detected but not acted on.
        /// </summary>
        public void ApplyTrackedResponses(string? role, IReadOnlyDictionary<string, string> trackedAutoResponses,
            IReadOnlyDictionary<string, int>? trackedMaxRetries = null)
        {
            var mapping = new (GameSaveDialogType Type, string ShortKey)[]
            {
                (GameSaveDialogType.ActiveDeviceContention, "ActiveDeviceContention"),
                (GameSaveDialogType.Conflict,              "Conflict"),
                (GameSaveDialogType.OutOfStorage,          "OutOfStorage"),
                (GameSaveDialogType.SyncFailed,            "SyncFailed"),
                (GameSaveDialogType.Progress,              "Progress"),
            };

            foreach (var (type, shortKey) in mapping)
            {
                string lookupKey = $"{role}:{shortKey}";
                if (role != null && trackedAutoResponses.TryGetValue(lookupKey, out string? action))
                {
                    int maxRetry = -1;
                    if (trackedMaxRetries != null && trackedMaxRetries.TryGetValue(lookupKey, out int tracked))
                    {
                        maxRetry = tracked;
                    }
                    SetResponse(type, action, maxRetry);
                }
            }
        }

        /// <summary>
        /// Starts the background monitor thread that scans for UI dialogs every second.
        /// </summary>
        public void Start()
        {
            if (_monitorThread != null)
            {
                _log("GameSaveUiNavigator: Monitor already running.");
                return;
            }

            _cts = new CancellationTokenSource();
            _monitorThread = new Thread(MonitorLoop)
            {
                Name = "GameSaveUiNavigator",
                IsBackground = true
            };
            _monitorThread.Start(_cts.Token);
            _log("GameSaveUiNavigator: Monitor thread started.");
        }

        /// <summary>
        /// Stops the background monitor thread and waits for it to exit.
        /// </summary>
        public void Stop()
        {
            if (_cts == null || _monitorThread == null)
            {
                return;
            }

            _log("GameSaveUiNavigator: Stopping monitor thread...");
            _cts.Cancel();

            if (_monitorThread.IsAlive)
            {
                _monitorThread.Join(TimeSpan.FromSeconds(5));
            }

            _cts.Dispose();
            _cts = null;
            _monitorThread = null;
            _log("GameSaveUiNavigator: Monitor thread stopped.");
        }

        /// <summary>
        /// Returns a snapshot of all dialog events recorded during monitoring.
        /// </summary>
        public IReadOnlyList<DialogEvent> GetEventLog()
        {
            lock (_eventLogLock)
            {
                return _eventLog.ToList();
            }
        }

        /// <summary>
        /// Background thread loop: scans for PFGameSave UI dialogs every 250 ms.
        /// </summary>
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
                        string errKey = $"scan-error:{ex.Message}";
                        if (!_lastLoggedState.TryGetValue(errKey, out _))
                        {
                            _log($"GameSaveUiNavigator: Scan error: {ex.Message}");
                            _lastLoggedState[errKey] = "logged";
                        }
                    }

                    // Wait between scans
                    token.WaitHandle.WaitOne(250);
                }
            }
            catch (OperationCanceledException)
            {
                // Normal shutdown
            }
            catch (Exception ex)
            {
                _log($"GameSaveUiNavigator: Monitor thread exiting due to error: {ex.Message}");
            }
        }

        /// <summary>
        /// Performs a single scan pass: enumerates top-level windows, identifies
        /// PFGameSave dialogs, and clicks the appropriate response button.
        /// </summary>
        private void ScanAndRespond()
        {
            var desktop = AutomationElement.RootElement;

            // Find all top-level windows
            var windows = FindAll(desktop, TreeScope.Children,
                new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Window));

            foreach (var window in windows)
            {
                string windowName = SafeName(window);
                if (string.IsNullOrWhiteSpace(windowName) || windowName == "(error)" || windowName == "(null)")
                {
                    continue;
                }

                GameSaveDialogType? dialogType = IdentifyDialog(window, windowName);
                if (dialogType == null)
                {
                    continue;
                }

                // Build a state key to suppress duplicate log messages
                int pid = SafePid(window);
                string windowKey = $"{pid}:{dialogType.Value}";

                if (!_responseActions.TryGetValue(dialogType.Value, out string? action))
                {
                    string noActionState = "no-action";
                    if (!_lastLoggedState.TryGetValue(windowKey, out string? prev) || prev != noActionState)
                    {
                        _log($"GameSaveUiNavigator: Detected {dialogType.Value} dialog but no response action configured. Skipping.");
                        _lastLoggedState[windowKey] = noActionState;
                    }
                    continue;
                }

                // Apply maxRetries: after exhausting retries, switch to fallback action.
                if (_maxRetries.TryGetValue(dialogType.Value, out int maxRetry) && maxRetry >= 0)
                {
                    _retryCount.TryGetValue(dialogType.Value, out int currentCount);
                    if (currentCount >= maxRetry)
                    {
                        string fallback = _fallbackActions.TryGetValue(dialogType.Value, out string? fb) ? fb : "Cancel";
                        if (action != fallback)
                        {
                            _log($"GameSaveUiNavigator: maxRetries ({maxRetry}) exhausted for {dialogType.Value}. Switching from '{action}' to '{fallback}'.");
                            action = fallback;
                        }
                    }
                    else
                    {
                        _retryCount[dialogType.Value] = currentCount + 1;
                        _log($"GameSaveUiNavigator: {dialogType.Value} retry {currentCount + 1}/{maxRetry}.");
                    }
                }

                string currentState = $"responding:{action}";
                if (!_lastLoggedState.TryGetValue(windowKey, out string? lastState) || lastState != currentState)
                {
                    _log($"GameSaveUiNavigator: Detected {dialogType.Value} dialog \"{windowName}\". Responding with action={action}.");
                    _lastLoggedState[windowKey] = currentState;

                    // Dump UI tree on first detection to diagnose button names
                    try
                    {
                        DumpWindowTree(window, windowName);
                    }
                    catch (Exception dumpEx)
                    {
                        _log($"GameSaveUiNavigator: Error dumping UI tree: {dumpEx.Message}");
                    }
                }

                bool responded = TryClickResponseButton(window, dialogType.Value, action);

                if (responded)
                {
                    // InvokeOrClick already verified dismissal per-strategy.
                    // Double-check: is the Gaming UI window actually gone now?
                    IntPtr hwnd = FindGamingUiWindowHandle();
                    bool dismissed = (hwnd == IntPtr.Zero || !IsWindowVisible(hwnd));

                    if (!dismissed)
                    {
                        _log($"GameSaveUiNavigator: Dialog still present after clicking \"{action}\". Counting as consumed retry (dialog may have reappeared).");
                        _lastLoggedState[windowKey] = $"click-fired:{action}";
                        continue;
                    }

                    _lastLoggedState[windowKey] = $"responded:{action}";
                    lock (_dismissedCountLock)
                    {
                        _dismissedCount[dialogType.Value] =
                            (_dismissedCount.TryGetValue(dialogType.Value, out int prior) ? prior : 0) + 1;
                        _log($"GameSaveUiNavigator: ✓ Dismissed {dialogType.Value} dialog with \"{action}\" (total {dialogType.Value} dismissals: {_dismissedCount[dialogType.Value]}).");
                    }
                }
                else
                {
                    if (lastState != $"failed:{action}")
                    {
                        _log($"GameSaveUiNavigator: Failed to click response button for {dialogType.Value} dialog.");
                    }
                    _lastLoggedState[windowKey] = $"failed:{action}";
                }
            }
        }

        /// <summary>
        /// Identifies whether a window is a PFGameSave UI dialog and which type it is.
        /// Detection strategy: Gaming UI windows contain a CoreWindow child with a Group
        /// element whose AutomationId="CurrentPage" and Name="TCUI.Pages.XxxPage" — the
        /// page name uniquely identifies the dialog type.
        /// </summary>
        private GameSaveDialogType? IdentifyDialog(AutomationElement window, string windowName)
        {
            // All PFGameSave dialogs appear as "Gaming UI" windows
            if (!windowName.Equals("Gaming UI", StringComparison.OrdinalIgnoreCase))
            {
                return null;
            }

            // Look for the CurrentPage group inside the CoreWindow child.
            // Structure: Window > CoreWindow > Group[AutomationId="CurrentPage"]
            string? pageName = null;
            try
            {
                var children = FindAll(window, TreeScope.Children, Condition.TrueCondition);
                foreach (var child in children)
                {
                    // The CoreWindow has ClassName="Windows.UI.Core.CoreWindow"
                    string childClass = SafeClassName(child);
                    if (!childClass.Contains("CoreWindow", StringComparison.OrdinalIgnoreCase))
                    {
                        continue;
                    }

                    // Find the Group with AutomationId="CurrentPage"
                    var pageGroups = FindAll(child, TreeScope.Children, Condition.TrueCondition);
                    foreach (var group in pageGroups)
                    {
                        string autoId = SafeAutoIdStr(group);
                        if (autoId.Equals("CurrentPage", StringComparison.OrdinalIgnoreCase))
                        {
                            pageName = SafeName(group);
                            if (pageName == "(error)" || pageName == "(null)") pageName = null;
                            break;
                        }
                    }
                    break; // Only one CoreWindow expected
                }
            }
            catch (Exception ex)
            {
                _log($"GameSaveUiNavigator: Error inspecting window \"{windowName}\": {ex.Message}");
                return null;
            }

            if (string.IsNullOrEmpty(pageName))
            {
                return null;
            }

            // Map TCUI page names to dialog types.
            // Known pages (discovered via UI Automation tree inspection):
            //   "TCUI.Pages.UnsyncedProgressPage"           => ActiveDeviceContention
            //   "TCUI.Pages.LockContentionWithContextPage"   => ActiveDeviceContention (variant)
            //   (other pages will be added here as they are identified)
            if (pageName.Contains("UnsyncedProgressPage", StringComparison.OrdinalIgnoreCase) ||
                pageName.Contains("LockContentionWithContextPage", StringComparison.OrdinalIgnoreCase))
            {
                return GameSaveDialogType.ActiveDeviceContention;
            }

            // StopSyncConfirmationPage is a follow-up confirmation after clicking
            // "STOP SYNC & CONTINUE" — always auto-click "YES, STOP SYNC".
            if (pageName.Contains("StopSyncConfirmationPage", StringComparison.OrdinalIgnoreCase))
            {
                string confirmKey = $"confirm:StopSync";
                if (!_lastLoggedState.TryGetValue(confirmKey, out _))
                {
                    _log("GameSaveUiNavigator: StopSyncConfirmationPage detected — auto-clicking \"YES, STOP SYNC\".");
                    _lastLoggedState[confirmKey] = "logged";
                }

                var buttons = FindAll(window, TreeScope.Descendants,
                    new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Button));
                // Confirm button varies: "YES, STOP SYNC" or "CONTINUE FROM LAST CLOUD DATA"
                // Pick the first non-GoBack, non-Close button as the confirm action.
                var confirmButton = buttons.FirstOrDefault(b =>
                    SafeName(b).Contains("YES, STOP SYNC", StringComparison.OrdinalIgnoreCase))
                    ?? buttons.FirstOrDefault(b =>
                    SafeName(b).Contains("CONTINUE FROM LAST CLOUD DATA", StringComparison.OrdinalIgnoreCase))
                    ?? buttons.FirstOrDefault(b =>
                    {
                        string name = SafeName(b);
                        string autoId = SafeAutoIdStr(b);
                        return !autoId.Equals("GoBackButton", StringComparison.OrdinalIgnoreCase)
                            && !autoId.Equals("Close", StringComparison.OrdinalIgnoreCase)
                            && name != "(error)" && name != "(null)" && !string.IsNullOrEmpty(name);
                    });
                if (confirmButton != null && SafeIsEnabled(confirmButton))
                {
                    InvokeOrClick(confirmButton);
                    _log($"GameSaveUiNavigator: Clicked \"{SafeName(confirmButton)}\".");
                }
                else
                {
                    _log($"GameSaveUiNavigator: Could not find/click confirm button. Available: [{string.Join(", ", buttons.Select(b => $"\"{SafeName(b)}\""))}]");
                }
                return null; // Not a configurable dialog type
            }

            // TCUI.Pages.ConflictResolutionWithContextPage => Conflict
            // Buttons: TBD — button names will be captured from logs
            if (pageName.Contains("ConflictResolution", StringComparison.OrdinalIgnoreCase))
            {
                return GameSaveDialogType.Conflict;
            }

            // TCUI.Pages.SyncFailurePage => SyncFailed
            // Buttons: "Play offline" (AutoId=UseOfflineButton), "Try again" (AutoId=TryAgainButton)
            if (pageName.Contains("SyncFailure", StringComparison.OrdinalIgnoreCase) ||
                pageName.Contains("SyncFailed", StringComparison.OrdinalIgnoreCase))
            {
                return GameSaveDialogType.SyncFailed;
            }

            // TCUI.Pages.SyncProgressPage => Progress
            // Buttons: "Cancel" (AutoId=SyncCancelButton)
            if (pageName.Contains("SyncProgressPage", StringComparison.OrdinalIgnoreCase))
            {
                return GameSaveDialogType.Progress;
            }

            // TCUI.Pages.StopOrKeepSyncingPage => StopOrKeepSyncing
            // Buttons: "Stop syncing" (AutoId=StopSycningButton [sic]), "Continue syncing" (AutoId=KeepSyncingButton)
            if (pageName.Contains("StopOrKeepSyncing", StringComparison.OrdinalIgnoreCase))
            {
                return GameSaveDialogType.StopOrKeepSyncing;
            }

            // Content-based fallback: identify the conflict dialog by its distinctive content in
            // case the page AutomationId differs by GRTS build. The screenshot-confirmed page
            // offers a "Local save" and a "Cloud save" choice under "Which one do you want to use?".
            if ((WindowContainsText(window, "Local save") && WindowContainsText(window, "Cloud save"))
                || WindowContainsText(window, "Which one do you want to use"))
            {
                string conflictKey = $"conflict-by-content:{pageName}";
                if (!_lastLoggedState.TryGetValue(conflictKey, out _))
                {
                    _log($"GameSaveUiNavigator: Page \"{pageName}\" identified as Conflict by content (Local save / Cloud save).");
                    DumpWindowTree(window, pageName);
                    _lastLoggedState[conflictKey] = "logged";
                }
                return GameSaveDialogType.Conflict;
            }

            string unknownKey = $"unknown:{pageName}";
            if (!_lastLoggedState.TryGetValue(unknownKey, out _))
            {
                _log($"GameSaveUiNavigator: Unknown TCUI page \"{pageName}\" on \"Gaming UI\" window — not a recognized dialog.");
                DumpWindowTree(window, pageName);
                _lastLoggedState[unknownKey] = "logged";
            }
            return null;
        }

        /// <summary>
        /// Dumps the UI automation tree of a window to the log for later analysis.
        /// Called once per unknown TCUI page so we can add support later from logs.
        /// </summary>
        private void DumpWindowTree(AutomationElement window, string pageName)
        {
            try
            {
                _log($"GameSaveUiNavigator: --- UI Tree Dump for \"{pageName}\" ---");
                DumpElement(window, 0, 6);
                _log($"GameSaveUiNavigator: --- End UI Tree Dump ---");
            }
            catch (Exception ex)
            {
                _log($"GameSaveUiNavigator: Error dumping UI tree: {ex.Message}");
            }
        }

        private void DumpElement(AutomationElement element, int depth, int maxDepth)
        {
            if (depth > maxDepth) return;

            string indent = new string(' ', depth * 2);
            string name = SafeName(element);
            string controlType = SafeControlType(element);
            string autoId = SafeAutoIdStr(element);
            string className = SafeClassName(element);

            var parts = new List<string> { $"[{controlType}]" };
            if (!string.IsNullOrEmpty(name) && name != "(null)")
                parts.Add($"Name=\"{(name.Length > 80 ? name.Substring(0, 80) + "..." : name)}\"");
            if (!string.IsNullOrEmpty(autoId))
                parts.Add($"AutoId=\"{autoId}\"");
            if (!string.IsNullOrEmpty(className))
                parts.Add($"Class=\"{className}\"");

            _log($"GameSaveUiNavigator:   {indent}{string.Join(" ", parts)}");

            try
            {
                var children = FindAll(element, TreeScope.Children, Condition.TrueCondition);
                foreach (var child in children)
                {
                    DumpElement(child, depth + 1, maxDepth);
                }
            }
            catch { }
        }

        private static bool SafeAutoId(AutomationElement e, string expected)
        {
            try
            {
                return (e.Current.AutomationId ?? "").Equals(expected, StringComparison.OrdinalIgnoreCase);
            }
            catch { return false; }
        }

        private static string SafeAutoIdStr(AutomationElement e)
        {
            try { return e.Current.AutomationId ?? ""; }
            catch { return "(error)"; }
        }

        private static string SafeName(AutomationElement e)
        {
            try { return e.Current.Name ?? "(null)"; }
            catch { return "(error)"; }
        }

        private static string SafeClassName(AutomationElement e)
        {
            try { return e.Current.ClassName ?? ""; }
            catch { return ""; }
        }

        private static bool SafeIsEnabled(AutomationElement e)
        {
            try { return e.Current.IsEnabled; }
            catch { return false; }
        }

        private static string SafeControlType(AutomationElement e)
        {
            try
            {
                var ct = e.Current.ControlType;
                return ct?.ProgrammaticName?.Replace("ControlType.", "") ?? "?";
            }
            catch { return "?"; }
        }

        /// <summary>
        /// Attempts to click the appropriate button on the dialog for the given action.
        /// ActiveDeviceContention has known button mappings; other dialog types still use
        /// generic name-matching until their buttons are identified.
        /// </summary>
        /// <summary>
        /// Collects candidate clickable elements. TCUI pages expose some choices (e.g. the
        /// conflict-resolution save tiles) as ListItem/Group/Custom rather than Button, so a
        /// Button-only search silently finds nothing and the dialog is left on screen.
        /// </summary>
        private AutomationElement[] FindClickableElements(AutomationElement window)
        {
            var types = new[]
            {
                ControlType.Button, ControlType.ListItem, ControlType.Custom,
                ControlType.Group, ControlType.Hyperlink, ControlType.Image, ControlType.Text
            };

            var list = new List<AutomationElement>();
            foreach (var t in types)
            {
                try
                {
                    list.AddRange(FindAll(window, TreeScope.Descendants,
                        new PropertyCondition(AutomationElement.ControlTypeProperty, t)));
                }
                catch { /* control type not present */ }
            }
            return list.ToArray();
        }

        /// <summary>True if the element can actually be activated (not just a label).</summary>
        private static bool IsInvokable(AutomationElement e)
        {
            try
            {
                if (e.TryGetCurrentPattern(InvokePattern.Pattern, out _)) return true;
                if (e.TryGetCurrentPattern(SelectionItemPattern.Pattern, out _)) return true;
                if (e.TryGetCurrentPattern(TogglePattern.Pattern, out _)) return true;

                var ct = e.Current.ControlType;
                return ct == ControlType.Button || ct == ControlType.ListItem || ct == ControlType.Hyperlink;
            }
            catch { return false; }
        }

        /// <summary>
        /// Resolves a matched element to something actually clickable. Labels frequently live in a
        /// non-invokable child: the conflict tiles expose "Local save"/"Cloud save" as a TextBlock
        /// inside an UNNAMED Button (AutoId=LocalSaveData/CloudSaveData). Matching on the visible
        /// text therefore lands on the TextBlock, where Invoke is a no-op — so walk up to the
        /// nearest invokable ancestor (the tile Button).
        /// </summary>
        private static AutomationElement ResolveClickTarget(AutomationElement e)
        {
            try
            {
                if (IsInvokable(e)) return e;

                var walker = TreeWalker.ControlViewWalker;
                AutomationElement? cur = walker.GetParent(e);
                for (int depth = 0; cur != null && depth < 6; depth++)
                {
                    if (IsInvokable(cur)) return cur;
                    cur = walker.GetParent(cur);
                }
            }
            catch { /* fall through to the original element */ }
            return e;
        }

        /// <summary>Finds an element by exact AutomationId first, then by name fragments.</summary>
        private static AutomationElement? FindByAutoIdOrName(
            AutomationElement[] elements, string autoId, params string[] nameFragments)
        {
            var byId = elements.FirstOrDefault(e =>
                SafeAutoIdStr(e).Equals(autoId, StringComparison.OrdinalIgnoreCase));
            if (byId != null) return ResolveClickTarget(byId);

            foreach (string frag in nameFragments)
            {
                // Prefer an enabled, directly-invokable match; otherwise accept any name match and
                // resolve it up to its clickable ancestor rather than leaving the dialog up.
                var match = elements.FirstOrDefault(e =>
                        SafeName(e).Contains(frag, StringComparison.OrdinalIgnoreCase)
                        && SafeIsEnabled(e) && IsInvokable(e))
                    ?? elements.FirstOrDefault(e =>
                        SafeName(e).Contains(frag, StringComparison.OrdinalIgnoreCase) && SafeIsEnabled(e))
                    ?? elements.FirstOrDefault(e =>
                        SafeName(e).Contains(frag, StringComparison.OrdinalIgnoreCase));
                if (match != null) return ResolveClickTarget(match);
            }
            return null;
        }

        /// <summary>True if any descendant's name contains <paramref name="text"/>.</summary>
        private bool WindowContainsText(AutomationElement window, string text)
        {
            try
            {
                return FindAll(window, TreeScope.Descendants, Condition.TrueCondition)
                    .Any(e => SafeName(e).Contains(text, StringComparison.OrdinalIgnoreCase));
            }
            catch { return false; }
        }

        private bool TryClickResponseButton(
            AutomationElement window,
            GameSaveDialogType dialogType,
            string action)
        {
            var buttons = FindAll(window, TreeScope.Descendants,
                new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Button));

            AutomationElement? target = null;

            if (dialogType == GameSaveDialogType.ActiveDeviceContention)
            {
                // Known buttons for UnsyncedProgressPage:
                //   "STOP SYNC & CONTINUE" → SyncLastSavedData (takes over as active device)
                //   "QUIT GAME"            → Cancel
                // For Retry, there is no visible button — the dialog doesn't offer retry.
                target = action.ToUpperInvariant() switch
                {
                    "SYNCLASTSAVEDDATA" or "SYNCLASTSAVED" or "STOPSYNC" =>
                        buttons.FirstOrDefault(b => SafeName(b).Contains("STOP SYNC", StringComparison.OrdinalIgnoreCase)),
                    "CANCEL" or "QUIT" or "QUITGAME" =>
                        buttons.FirstOrDefault(b => SafeName(b).Contains("QUIT GAME", StringComparison.OrdinalIgnoreCase)),
                    "RETRY" =>
                        buttons.FirstOrDefault(b => SafeName(b).Contains("Close Gaming UI", StringComparison.OrdinalIgnoreCase)),
                    _ => null
                };

                if (target == null)
                {
                    _log($"GameSaveUiNavigator: ActiveDeviceContention — no button mapped for action '{action}'. " +
                         $"Available: [{string.Join(", ", buttons.Select(b => $"\"{SafeName(b)}\""))}]");
                    return false;
                }
            }
            else if (dialogType == GameSaveDialogType.Progress)
            {
                // Known buttons for SyncProgressPage:
                //   "Cancel" (AutoId=SyncCancelButton) → Cancel
                target = action.ToUpperInvariant() switch
                {
                    "CANCEL" =>
                        buttons.FirstOrDefault(b =>
                            SafeAutoIdStr(b).Equals("SyncCancelButton", StringComparison.OrdinalIgnoreCase))
                        ?? buttons.FirstOrDefault(b => SafeName(b).Equals("Cancel", StringComparison.OrdinalIgnoreCase)),
                    _ => null
                };

                if (target == null)
                {
                    _log($"GameSaveUiNavigator: Progress — no button mapped for action '{action}'. " +
                         $"Available: [{string.Join(", ", buttons.Select(b => $"\"{SafeName(b)}\""))}]");
                    return false;
                }
            }
            else if (dialogType == GameSaveDialogType.Conflict)
            {
                // ConflictResolutionWithContextPage ("Which one do you want to use?"):
                //   tile  AutoId="LocalSaveData"     Name="Local save"  -> TakeLocal / UseLocal
                //   tile  AutoId="CloudSaveData"     Name="Cloud save"  -> TakeCloud / UseCloud
                //   button AutoId="StopSyncingButton" Name="Play offline" -> Cancel
                // The two save choices are TILES and are NOT reliably exposed as ControlType.Button,
                // so search the broader clickable set - a Button-only search matched nothing and the
                // dialog was left on screen unclicked.
                var clickables = FindClickableElements(window);

                target = action.ToUpperInvariant() switch
                {
                    "TAKELOCAL" or "USELOCAL" or "LOCAL" =>
                        FindByAutoIdOrName(clickables, "LocalSaveData", "Local save", "Local"),
                    "TAKECLOUD" or "USECLOUD" or "USEREMOTE" or "CLOUD" =>
                        FindByAutoIdOrName(clickables, "CloudSaveData", "Cloud save", "Cloud"),
                    "CANCEL" or "PLAYOFFLINE" or "USEOFFLINE" =>
                        FindByAutoIdOrName(clickables, "StopSyncingButton", "Play offline"),
                    _ => null
                };

                if (target == null)
                {
                    _log($"GameSaveUiNavigator: Conflict — no element mapped for action '{action}'. " +
                         $"Available: [{string.Join(", ", clickables.Take(25).Select(b => $"\"{SafeName(b)}\" (AutoId={SafeAutoIdStr(b)}, type={SafeControlType(b)})"))}]");

                    // Never leave the conflict dialog on screen: fall back to "Play offline",
                    // which is a real Button on this page and always dismisses it.
                    target = FindByAutoIdOrName(clickables, "StopSyncingButton", "Play offline")
                          ?? FindByAutoIdOrName(clickables, "UseOfflineButton", "Play offline");
                    if (target == null)
                    {
                        return false;
                    }
                    _log($"GameSaveUiNavigator: Conflict — falling back to \"{SafeName(target)}\" to dismiss the dialog.");
                }
            }
            else if (dialogType == GameSaveDialogType.SyncFailed)
            {
                // Known buttons for SyncFailurePage:
                //   AutoId="UseOfflineButton"  Name="Play offline" → Cancel / PlayOffline
                //   AutoId="TryAgainButton"    Name="Try again"    → Retry
                // Default: "Play offline" to dismiss the dialog and let the test continue.
                target = action.ToUpperInvariant() switch
                {
                    "CANCEL" or "PLAYOFFLINE" or "USEOFFLINE" =>
                        buttons.FirstOrDefault(b => SafeAutoId(b, "UseOfflineButton"))
                        ?? buttons.FirstOrDefault(b => SafeName(b).Contains("Play offline", StringComparison.OrdinalIgnoreCase)),
                    "RETRY" or "TRYAGAIN" =>
                        buttons.FirstOrDefault(b => SafeAutoId(b, "TryAgainButton"))
                        ?? buttons.FirstOrDefault(b => SafeName(b).Contains("Try again", StringComparison.OrdinalIgnoreCase)),
                    _ =>
                        // Unknown action — default to "Play offline" to avoid hanging
                        buttons.FirstOrDefault(b => SafeAutoId(b, "UseOfflineButton"))
                        ?? buttons.FirstOrDefault(b => SafeName(b).Contains("Play offline", StringComparison.OrdinalIgnoreCase))
                };

                if (target == null)
                {
                    _log($"GameSaveUiNavigator: SyncFailed — no button mapped for action '{action}'. " +
                         $"Available: [{string.Join(", ", buttons.Select(b => $"\"{SafeName(b)}\" (AutoId={SafeAutoIdStr(b)})"))}]");
                    return false;
                }
            }
            else if (dialogType == GameSaveDialogType.StopOrKeepSyncing)
            {
                // Known buttons for StopOrKeepSyncingPage:
                //   AutoId="StopSycningButton" [sic — typo in GRTS]  Name="Stop syncing"  → Stop / Cancel
                //   AutoId="KeepSyncingButton"                       Name="Continue syncing" → Continue / Retry
                // Default: "Stop syncing" so the test doesn't hang waiting for network.
                target = action.ToUpperInvariant() switch
                {
                    "STOP" or "CANCEL" or "STOPSYNCING" =>
                        buttons.FirstOrDefault(b => SafeAutoId(b, "StopSycningButton") || SafeAutoId(b, "StopSyncingButton"))
                        ?? buttons.FirstOrDefault(b => SafeName(b).Contains("Stop syncing", StringComparison.OrdinalIgnoreCase)),
                    "CONTINUE" or "RETRY" or "KEEPSYNCING" =>
                        buttons.FirstOrDefault(b => SafeAutoId(b, "KeepSyncingButton"))
                        ?? buttons.FirstOrDefault(b => SafeName(b).Contains("Continue syncing", StringComparison.OrdinalIgnoreCase)),
                    _ =>
                        // Unknown action — default to "Stop syncing" to avoid hanging
                        buttons.FirstOrDefault(b => SafeAutoId(b, "StopSycningButton") || SafeAutoId(b, "StopSyncingButton"))
                        ?? buttons.FirstOrDefault(b => SafeName(b).Contains("Stop syncing", StringComparison.OrdinalIgnoreCase))
                };

                if (target == null)
                {
                    _log($"GameSaveUiNavigator: StopOrKeepSyncing — no button mapped for action '{action}'. " +
                         $"Available: [{string.Join(", ", buttons.Select(b => $"\"{SafeName(b)}\" (AutoId={SafeAutoIdStr(b)})"))}]");
                    return false;
                }
            }
            else
            {
                // Generic fallback for other dialog types (will be replaced as each is identified)
                target = buttons.FirstOrDefault(b =>
                    string.Equals(SafeName(b), action, StringComparison.OrdinalIgnoreCase));
                if (target == null)
                {
                    target = buttons.FirstOrDefault(b =>
                        SafeName(b).Contains(action, StringComparison.OrdinalIgnoreCase));
                }
                if (target == null)
                {
                    _log($"GameSaveUiNavigator: No button matching action '{action}' found. " +
                         $"Available: [{string.Join(", ", buttons.Select(b => $"\"{SafeName(b)}\""))}]");
                    return false;
                }
            }

            // Tiles like the conflict-resolution save choices expose no Name (their label lives in a
            // child TextBlock), so fall back to AutomationId to keep the log useful.
            string targetLabel = SafeName(target);
            if (string.IsNullOrEmpty(targetLabel) || targetLabel == "(null)" || targetLabel == "(error)")
            {
                targetLabel = $"(AutoId={SafeAutoIdStr(target)})";
            }
            _log($"GameSaveUiNavigator: Clicking {SafeControlType(target)} \"{targetLabel}\" (IsEnabled={SafeIsEnabled(target)}).");

            if (!SafeIsEnabled(target))
            {
                _log($"GameSaveUiNavigator: Button \"{SafeName(target)}\" is disabled — cannot click.");
                return false;
            }

            InvokeOrClick(target);

            return true;
        }

        /// <summary>
        /// Returns true if the given endpoint resolves to a local address.
        /// </summary>
        internal static bool IsDeviceLocal(string? remoteEndpoint)
        {
            string endpoint = remoteEndpoint ?? string.Empty;
            string host = endpoint;
            int colonIndex = endpoint.LastIndexOf(':');
            if (colonIndex > 0)
            {
                host = endpoint.Substring(0, colonIndex);
            }

            host = host.Trim('[', ']');

            if (string.IsNullOrWhiteSpace(host))
            {
                return false;
            }

            if (IPAddress.TryParse(host, out IPAddress? address))
            {
                if (IPAddress.IsLoopback(address))
                    return true;

                try
                {
                    var hostAddresses = Dns.GetHostAddresses(Dns.GetHostName());
                    foreach (var local in hostAddresses)
                    {
                        if (address.Equals(local))
                            return true;
                    }
                }
                catch { }
            }

            return false;
        }

        public void Dispose()
        {
            if (!_disposed)
            {
                Stop();
                _disposed = true;
            }
        }

        /// <summary>
        /// Result of a single-scan pass for use by the UI debug runner.
        /// </summary>
        internal sealed class ScanResult
        {
            public GameSaveDialogType? DialogType { get; set; }
            public string? PageName { get; set; }
            public string? WindowName { get; set; }
            public int ProcessId { get; set; }
            public List<ButtonInfo> Buttons { get; set; } = new();
        }

        internal sealed class ButtonInfo
        {
            public string Name { get; set; } = "";
            public string AutomationId { get; set; } = "";
            public bool IsEnabled { get; set; }
            public bool HasInvoke { get; set; }
        }

        /// <summary>
        /// Performs a single scan of the desktop and returns structured results
        /// without taking any action. Used by the --ui-debug CLI mode.
        /// Also dumps the full UI tree to the log for each Gaming UI window found.
        /// </summary>
        public List<ScanResult> ScanOnce()
        {
            var results = new List<ScanResult>();
            var desktop = AutomationElement.RootElement;

            var windows = FindAll(desktop, TreeScope.Children,
                new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Window));

            foreach (var window in windows)
            {
                string windowName = SafeName(window);
                if (string.IsNullOrWhiteSpace(windowName) || windowName == "(error)" || windowName == "(null)")
                    continue;

                if (!windowName.Equals("Gaming UI", StringComparison.OrdinalIgnoreCase))
                    continue;

                var result = new ScanResult
                {
                    WindowName = windowName,
                    ProcessId = SafePid(window),
                };

                // Identify page
                try
                {
                    var children = FindAll(window, TreeScope.Children, Condition.TrueCondition);
                    foreach (var child in children)
                    {
                        string childClass = SafeClassName(child);
                        if (!childClass.Contains("CoreWindow", StringComparison.OrdinalIgnoreCase))
                            continue;

                        var pageGroups = FindAll(child, TreeScope.Children, Condition.TrueCondition);
                        foreach (var group in pageGroups)
                        {
                            string autoId = SafeAutoIdStr(group);
                            if (autoId.Equals("CurrentPage", StringComparison.OrdinalIgnoreCase))
                            {
                                string name = SafeName(group);
                                if (name != "(error)" && name != "(null)")
                                    result.PageName = name;
                                break;
                            }
                        }
                        break;
                    }
                }
                catch (Exception ex)
                {
                    _log($"ScanOnce: Error inspecting window: {ex.Message}");
                }

                // Identify dialog type
                result.DialogType = IdentifyDialog(window, windowName);

                // Enumerate buttons
                var buttons = FindAll(window, TreeScope.Descendants,
                    new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Button));
                foreach (var btn in buttons)
                {
                    var info = new ButtonInfo { Name = SafeName(btn), AutomationId = SafeAutoIdStr(btn) };
                    try { info.IsEnabled = SafeIsEnabled(btn); } catch { }
                    try { info.HasInvoke = btn.TryGetCurrentPattern(InvokePattern.Pattern, out _); } catch { }
                    result.Buttons.Add(info);
                }

                // Dump full tree to log
                _log("--- UI Tree Dump ---");
                DumpElement(window, 0, 8);
                _log("--- End UI Tree Dump ---");

                results.Add(result);
            }

            return results;
        }

        /// <summary>
        /// Clicks a button by name or AutomationId on the first Gaming UI window found.
        /// Returns true if the button was found and clicked.
        /// </summary>
        public bool ClickButton(string buttonNameOrAutoId)
        {
            var desktop = AutomationElement.RootElement;

            var windows = FindAll(desktop, TreeScope.Children,
                new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Window));
            foreach (var window in windows)
            {
                string windowName = SafeName(window);
                if (!windowName.Equals("Gaming UI", StringComparison.OrdinalIgnoreCase))
                    continue;

                var buttons = FindAll(window, TreeScope.Descendants,
                    new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Button));

                var target = buttons.FirstOrDefault(b =>
                    SafeName(b).Equals(buttonNameOrAutoId, StringComparison.OrdinalIgnoreCase));
                target ??= buttons.FirstOrDefault(b =>
                    SafeName(b).Contains(buttonNameOrAutoId, StringComparison.OrdinalIgnoreCase));
                target ??= buttons.FirstOrDefault(b =>
                    SafeAutoIdStr(b).Equals(buttonNameOrAutoId, StringComparison.OrdinalIgnoreCase));

                if (target == null)
                {
                    _log($"ClickButton: No button matching \"{buttonNameOrAutoId}\" found. " +
                         $"Available: [{string.Join(", ", buttons.Select(b => $"\"{SafeName(b)}\""))}]");
                    return false;
                }

                if (!SafeIsEnabled(target))
                {
                    _log($"ClickButton: Button \"{SafeName(target)}\" found but is disabled.");
                    return false;
                }

                InvokeOrClick(target);

                _log($"ClickButton: Clicked \"{SafeName(target)}\".");
                return true;
            }

            _log("ClickButton: No Gaming UI window found.");
            return false;
        }

        private static int SafePid(AutomationElement e)
        {
            try { return e.Current.ProcessId; } catch { return 0; }
        }

        /// <summary>
        /// Wraps FindAll to return a typed array and handles exceptions from stale elements.
        /// </summary>
        private static AutomationElement[] FindAll(AutomationElement parent, TreeScope scope, Condition condition)
        {
            try
            {
                var collection = parent.FindAll(scope, condition);
                if (collection == null || collection.Count == 0)
                    return Array.Empty<AutomationElement>();
                return collection.Cast<AutomationElement>().ToArray();
            }
            catch { return Array.Empty<AutomationElement>(); }
        }

        /// <summary>
        /// Activates a TCUI button using keyboard-first approach (DPI-independent).
        /// Strategy order:
        ///   1. SetForeground → UIA SetFocus → keybd_event Space
        ///   2. InvokePattern (UIA native)
        ///   3. DPI-corrected mouse click
        /// </summary>
        private void InvokeOrClick(AutomationElement element)
        {
            string buttonName = "?";
            try { buttonName = element.Current.Name; } catch { }

            IntPtr windowHandle = FindGamingUiWindowHandle();

            // Strategy 1 (primary): Keyboard — SetForeground + UIA SetFocus + keybd_event Space
            // Proven to deliver input to UWP TCUI windows regardless of DPI scaling.
            try
            {
                if (windowHandle != IntPtr.Zero)
                {
                    SetForegroundWindow(windowHandle);
                    Thread.Sleep(200);
                }
                element.SetFocus();
                Thread.Sleep(150);
                keybd_event(VK_SPACE, 0x39, 0, UIntPtr.Zero);
                Thread.Sleep(50);
                keybd_event(VK_SPACE, 0x39, KEYEVENTF_KEYUP, UIntPtr.Zero);
                _log($"  [click] Keyboard Space on \"{buttonName}\": fired");
                if (WaitForWindowDismissed(windowHandle, 1500))
                {
                    _log($"  [click] Keyboard Space: ✓ dialog dismissed");
                    return;
                }
                _log($"  [click] Keyboard Space: dialog still present");
            }
            catch (Exception ex) { _log($"  [click] Keyboard Space error: {ex.Message}"); }

            // Strategy 2: InvokePattern on the target button
            try
            {
                if (element.TryGetCurrentPattern(InvokePattern.Pattern, out object? pattern))
                {
                    ((InvokePattern)pattern).Invoke();
                    _log($"  [click] InvokePattern on \"{buttonName}\": fired");
                    if (WaitForWindowDismissed(windowHandle, 1500))
                    {
                        _log($"  [click] InvokePattern: ✓ dialog dismissed");
                        return;
                    }
                }
            }
            catch (Exception ex) { _log($"  [click] InvokePattern error: {ex.Message}"); }

            // Strategy 3: DPI-corrected mouse click
            try
            {
                double dpiScale = 1.0;
                if (windowHandle != IntPtr.Zero)
                {
                    int windowDpi = SafeGetDpiForWindow(windowHandle);
                    if (windowDpi > 96)
                        dpiScale = windowDpi / 96.0;
                }

                var rect = element.Current.BoundingRectangle;
                if (!rect.IsEmpty && !double.IsInfinity(rect.Width) && !double.IsInfinity(rect.Height))
                {
                    int cx = (int)((rect.Left + rect.Width / 2) / dpiScale);
                    int cy = (int)((rect.Top + rect.Height / 2) / dpiScale);
                    SetCursorPos(cx, cy);
                    Thread.Sleep(100);
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, UIntPtr.Zero);
                    Thread.Sleep(80);
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, UIntPtr.Zero);
                    _log($"  [click] Mouse click on \"{buttonName}\" at ({cx},{cy}) dpi={dpiScale:F2}: fired");
                    if (WaitForWindowDismissed(windowHandle, 1500))
                    {
                        _log($"  [click] Mouse click: ✓ dialog dismissed");
                        return;
                    }
                }
            }
            catch (Exception ex) { _log($"  [click] Mouse click error: {ex.Message}"); }

            _log($"  [click] All strategies exhausted for \"{buttonName}\"");
        }

        /// <summary>
        /// Checks whether the Gaming UI window has been dismissed (closed/hidden).
        /// Polls every 100ms up to timeoutMs.
        /// </summary>
        private static bool WaitForWindowDismissed(IntPtr windowHandle, int timeoutMs)
        {
            if (windowHandle == IntPtr.Zero)
                return false;

            var sw = System.Diagnostics.Stopwatch.StartNew();
            while (sw.ElapsedMilliseconds < timeoutMs)
            {
                Thread.Sleep(100);
                // Check if the window is still visible
                if (!IsWindowVisible(windowHandle))
                    return true;
                // Also check if a fresh FindWindow returns null (window destroyed)
                if (FindWindow("ApplicationFrameWindow", "Gaming UI") == IntPtr.Zero)
                    return true;
            }
            return false;
        }

        [DllImport("user32.dll")]
        private static extern void keybd_event(byte bVk, byte bScan, uint dwFlags, UIntPtr dwExtraInfo);

        private const byte VK_SPACE = 0x20;
        private const uint KEYEVENTF_KEYUP = 0x0002;

        [DllImport("user32.dll")]
        private static extern bool SetCursorPos(int X, int Y);

        [DllImport("user32.dll")]
        private static extern void mouse_event(uint dwFlags, int dx, int dy, uint dwData, UIntPtr dwExtraInfo);

        [DllImport("user32.dll")]
        private static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        private static extern IntPtr FindWindow(string? lpClassName, string? lpWindowName);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsWindowVisible(IntPtr hWnd);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern int GetDpiForWindow(IntPtr hwnd);

        /// <summary>
        /// Safely calls GetDpiForWindow, returning 96 (no scaling) if the API is unavailable.
        /// GetDpiForWindow requires Windows 10 1607+.
        /// </summary>
        private static int SafeGetDpiForWindow(IntPtr hwnd)
        {
            try
            {
                int dpi = GetDpiForWindow(hwnd);
                return dpi > 0 ? dpi : 96;
            }
            catch
            {
                return 96;
            }
        }

        private const uint MOUSEEVENTF_LEFTDOWN = 0x0002;
        private const uint MOUSEEVENTF_LEFTUP = 0x0004;

        private static IntPtr FindGamingUiWindowHandle()
        {
            return FindWindow("ApplicationFrameWindow", "Gaming UI");
        }

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        private const uint WM_CLOSE = 0x0010;

        /// <summary>
        /// Dismisses any outstanding PFGameSave "Gaming UI" dialogs that may be lingering
        /// from a previous test. Call this at the start of each test scenario before the
        /// UiNavigator monitor thread is started.
        /// Sends WM_CLOSE to close the dialog window; safe to call when no dialog exists.
        /// </summary>
        internal static void DismissStaleDialogs(Action<string> log)
        {
            // Retry a few times since closing one dialog might reveal another underneath
            for (int attempt = 0; attempt < 3; attempt++)
            {
                IntPtr hwnd = FindWindow("ApplicationFrameWindow", "Gaming UI");
                if (hwnd == IntPtr.Zero || !IsWindowVisible(hwnd))
                {
                    if (attempt == 0)
                        return; // No dialog found on first check — nothing to dismiss
                    break;
                }

                log($"  DismissStaleDialogs: Found lingering 'Gaming UI' dialog (attempt {attempt + 1}). Closing...");
                SendMessage(hwnd, WM_CLOSE, IntPtr.Zero, IntPtr.Zero);
                Thread.Sleep(1000); // Give it a moment to close
            }

            // Final verification
            IntPtr finalCheck = FindWindow("ApplicationFrameWindow", "Gaming UI");
            if (finalCheck != IntPtr.Zero && IsWindowVisible(finalCheck))
            {
                log("  DismissStaleDialogs: WARNING — dialog still present after close attempts.");
            }
            else
            {
                log("  DismissStaleDialogs: Stale dialog(s) dismissed.");
            }
        }
    }
}
