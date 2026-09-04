# Xbox Dialog Templates

Reference images for OpenCV template matching against Xbox TCUI dialogs.

## Naming Convention

Files are named `{DialogType}.png` matching `GameSaveDialogType` values:
- `ActiveDeviceContention.png`
- `Conflict.png`
- `OutOfStorage.png`
- `SyncFailed.png`
- `Progress.png`

## How to Capture

1. Trigger the dialog on Xbox (e.g., via a test scenario)
2. Capture a screenshot: `XbInputCSharp.exe /X <address> /SCREENSHOT dialog.bmp`
3. Crop the distinctive region (title bar, icon, or unique text area)
4. Save as PNG in this folder

## Guidelines

- Crop to a **distinctive region** — not the full screen. A dialog title or unique
  icon works best. Avoid regions that change (progress bars, timestamps).
- Use the **native Xbox resolution** (typically 1920x1080).
- Template matching uses `MatchTemplate(TM_CCOEFF_NORMED)` with a configurable
  threshold (default 0.8). Smaller, more distinctive templates match better.
