using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace GameTestController.Xbox
{
    /// <summary>
    /// Sends virtual gamepad input to an Xbox dev kit via the XTF Input API.
    /// Used to navigate TCUI dialogs (A to confirm, B to cancel, DPad to move).
    /// </summary>
    internal sealed class XboxGamepadInput : IDisposable
    {
        private readonly Action<string> _log;
        private IXtfInputClient? _client;
        private ulong _controllerId;
        private bool _connected;

        /// <summary>
        /// Default hold time for button presses in milliseconds.
        /// </summary>
        public int HoldTimeMs { get; set; } = 100;

        public XboxGamepadInput(string xboxAddress, Action<string> log)
        {
            _log = log ?? throw new ArgumentNullException(nameof(log));
            GdkDllResolver.EnsureLoaded();

            try
            {
                var iid = typeof(IXtfInputClient).GUID;
                int hr = NativeMethods.XtfCreateInputClient(xboxAddress, ref iid, out _client);
                if (hr < 0 || _client == null)
                {
                    _log($"XboxGamepadInput: XtfCreateInputClient failed (0x{hr:X8}).");
                    return;
                }

                hr = _client.ConnectGamepad(out _controllerId);
                if (hr < 0)
                {
                    _log($"XboxGamepadInput: ConnectGamepad failed (0x{hr:X8}).");
                    return;
                }

                _connected = true;
                _log($"XboxGamepadInput: Connected virtual gamepad (id={_controllerId}). Waiting 500ms for Xbox to register...");
                Thread.Sleep(500);
            }
            catch (DllNotFoundException ex)
            {
                _log($"XboxGamepadInput: XTF DLL not found — is the GDK installed? {ex.Message}");
            }
            catch (Exception ex)
            {
                _log($"XboxGamepadInput: Error initializing: {ex.Message}");
            }
        }

        public bool IsConnected => _connected;

        /// <summary>
        /// Press and release a button combination.
        /// </summary>
        public bool PressButtons(GamepadButtons buttons)
        {
            if (!_connected || _client == null) return false;

            _log($"XboxGamepadInput: SendGamepadReport buttons=0x{(ushort)buttons:X4} ({buttons})...");
            var report = new GamepadReport { Buttons = (ushort)buttons };
            int hr = _client.SendGamepadReport(_controllerId, report);
            if (hr < 0)
            {
                _log($"XboxGamepadInput: SendGamepadReport (press) failed (0x{hr:X8}).");
                return false;
            }
            _log($"XboxGamepadInput: Press sent (hr=0x{hr:X8}), holding {HoldTimeMs}ms...");

            Thread.Sleep(HoldTimeMs);

            var release = new GamepadReport();
            hr = _client.SendGamepadReport(_controllerId, release);
            if (hr < 0)
            {
                _log($"XboxGamepadInput: SendGamepadReport (release) failed (0x{hr:X8}).");
                return false;
            }
            _log($"XboxGamepadInput: Released (hr=0x{hr:X8}).");

            return true;
        }

        public bool PressA() => PressButtons(GamepadButtons.A);
        public bool PressB() => PressButtons(GamepadButtons.B);
        public bool PressDPadUp() => PressButtons(GamepadButtons.DpadUp);
        public bool PressDPadDown() => PressButtons(GamepadButtons.DpadDown);
        public bool PressDPadLeft() => PressButtons(GamepadButtons.DpadLeft);
        public bool PressDPadRight() => PressButtons(GamepadButtons.DpadRight);

        /// <summary>
        /// Delay between sequential button presses in a navigation sequence.
        /// </summary>
        public int NavigationDelayMs { get; set; } = 1000;

        /// <summary>
        /// Sends a sequence of button presses with delays between them.
        /// </summary>
        public bool SendSequence(params GamepadButtons[] sequence)
        {
            foreach (var button in sequence)
            {
                _log($"XboxGamepadInput: Pressing {button}...");
                if (!PressButtons(button))
                    return false;
                Thread.Sleep(NavigationDelayMs);
            }
            return true;
        }

        /// <summary>
        /// Responds to a detected dialog using the appropriate gamepad sequence.
        /// Returns true if the sequence was sent successfully.
        /// </summary>
        public bool RespondToDialog(
            GameSaveUiNavigator.GameSaveDialogType dialogType,
            string action)
        {
            var sequence = GetResponseSequence(dialogType, action);
            if (sequence == null)
            {
                _log($"XboxGamepadInput: No gamepad sequence for {dialogType} action '{action}'.");
                return false;
            }

            _log($"XboxGamepadInput: Responding to {dialogType} with action '{action}' → [{string.Join(", ", sequence)}]");
            return SendSequence(sequence);
        }

        /// <summary>
        /// Returns the gamepad button sequence for a given dialog type and action.
        /// Returns null if the combination is not mapped.
        /// </summary>
        public static GamepadButtons[]? GetResponseSequence(
            GameSaveUiNavigator.GameSaveDialogType dialogType,
            string action)
        {
            string normalizedAction = action.Trim().ToUpperInvariant();

            return dialogType switch
            {
                GameSaveUiNavigator.GameSaveDialogType.Conflict => normalizedAction switch
                {
                    // Choose local save data
                    "TAKELOCAL" or "USELOCAL" or "LOCAL"
                        => new[] { GamepadButtons.DpadDown, GamepadButtons.DpadUp, GamepadButtons.A },
                    // Choose cloud/remote save data
                    "TAKECLOUD" or "USECLOUD" or "CLOUD" or "REMOTE" or "TAKEREMOTE" or "USEREMOTE"
                        => new[] { GamepadButtons.DpadDown, GamepadButtons.DpadRight, GamepadButtons.A },
                    // Cancel / play offline. Aliases kept in sync with the PC navigator's Conflict
                    // mapping (GameSaveUiNavigator.TryClickResponseButton), where the same action
                    // clicks the "Play offline" button (AutoId=StopSyncingButton) on the
                    // ConflictResolutionWithContextPage.
                    "CANCEL" or "PLAYOFFLINE" or "USEOFFLINE" or "OFFLINE"
                        => new[] { GamepadButtons.DpadDown, GamepadButtons.A },
                    _ => null
                },

                GameSaveUiNavigator.GameSaveDialogType.SyncFailed => normalizedAction switch
                {
                    // Retry sync
                    "RETRY"
                        => new[] { GamepadButtons.DpadLeft, GamepadButtons.A },
                    // Use offline / cancel
                    "CANCEL" or "OFFLINE" or "USEOFFLINE"
                        => new[] { GamepadButtons.DpadRight, GamepadButtons.A },
                    _ => null
                },

                GameSaveUiNavigator.GameSaveDialogType.ActiveDeviceContention => normalizedAction switch
                {
                    // Buttons on the UnsyncedProgressPage ("Your previous device is still
                    // uploading your game save") are laid out left-to-right as:
                    //     [QUIT GAME]  [STOP SYNC & CONTINUE]
                    // with QUIT GAME focused by default. The semantics are documented by the PC
                    // navigator, which drives the same TCUI page via UI Automation
                    // (GameSaveUiNavigator.TryClickResponseButton):
                    //     "STOP SYNC & CONTINUE" -> SyncLastSavedData (takes over as active device)
                    //     "QUIT GAME"            -> Cancel
                    // The previous mapping had these inverted: SyncLastSavedData pressed A on the
                    // focused button, which is QUIT GAME, so a takeover would have quit the game.
                    // DpadRight is used rather than a bare A because it deterministically lands on
                    // the rightmost button of this two-button row (it is a no-op if focus is
                    // already there), so the sequence does not depend on which button has focus.
                    "SYNCLASTSAVEDDATA" or "SYNC" or "SYNCLAST"
                        => new[] { GamepadButtons.DpadRight, GamepadButtons.A },
                    // Cancel = QUIT GAME, the default-focused (leftmost) button.
                    "CANCEL"
                        => new[] { GamepadButtons.A },
                    _ => null
                },

                GameSaveUiNavigator.GameSaveDialogType.OutOfStorage => normalizedAction switch
                {
                    // Free up space (opens storage management)
                    "FREEUPSPACE" or "MANAGE"
                        => new[] { GamepadButtons.A },
                    // Cancel
                    "CANCEL"
                        => new[] { GamepadButtons.DpadRight, GamepadButtons.A },
                    _ => null
                },

                _ => null
            };
        }

        public void Dispose()
        {
            if (_connected && _client != null)
            {
                _client.DisconnectGamepad(_controllerId);
                _log("XboxGamepadInput: Disconnected virtual gamepad.");
                _connected = false;
            }

            if (_client != null && Marshal.IsComObject(_client))
            {
                Marshal.ReleaseComObject(_client);
            }
            _client = null;
        }

        #region Gamepad types and COM interop

        [Flags]
        internal enum GamepadButtons : ushort
        {
            None           = 0x0000,
            Enroll         = 0x0001,
            Nexus          = 0x0002,
            Menu           = 0x0004,
            View           = 0x0008,
            A              = 0x0010,
            B              = 0x0020,
            X              = 0x0040,
            Y              = 0x0080,
            DpadUp         = 0x0100,
            DpadDown       = 0x0200,
            DpadLeft       = 0x0400,
            DpadRight      = 0x0800,
            LeftShoulder   = 0x1000,
            RightShoulder  = 0x2000,
            LeftThumbstick = 0x4000,
            RightThumbstick = 0x8000,
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct GamepadReport
        {
            public ushort Buttons;
            public ushort LeftTrigger;
            public ushort RightTrigger;
            public short LeftThumbstickX;
            public short LeftThumbstickY;
            public short RightThumbstickX;
            public short RightThumbstickY;
        }

        [ComImport]
        [Guid("167B5DB0-F32C-487C-AEFD-8AF3DA6284FA")]
        [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        internal interface IXtfInputClient
        {
            [PreserveSig]
            int ConnectGamepad(out ulong controllerId);

            [PreserveSig]
            int DisconnectAllGamepads();

            [PreserveSig]
            int DisconnectGamepad(ulong controllerId);

            [PreserveSig]
            int SendGamepadReport(ulong controllerId, [In] GamepadReport report);
        }

        private static class NativeMethods
        {
            [DllImport("XtfInput.dll", PreserveSig = true)]
            public static extern int XtfCreateInputClient(
                [MarshalAs(UnmanagedType.LPWStr)] string address,
                ref Guid riid,
                [MarshalAs(UnmanagedType.Interface)] out IXtfInputClient ppvObject);
        }

        #endregion
    }
}
