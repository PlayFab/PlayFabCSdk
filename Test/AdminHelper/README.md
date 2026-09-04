# AdminHelper — Elevated Named Pipe Server

Lightweight tool that runs as Administrator and provides privileged operations (network control, etc.) to the non-admin test controller via a named pipe.

## Why

Some test scenarios need to disable/enable network adapters, which requires admin privileges. Instead of running the entire test controller as admin, AdminHelper runs separately as admin and the controller talks to it over a named pipe. If AdminHelper isn't running, the controller falls back to direct PowerShell (which requires the controller itself to be elevated).

## Quick Start

```powershell
# Build
dotnet build Test\AdminHelper\AdminHelper.csproj -c Debug

# Run as admin (right-click → Run as Administrator, or from an admin terminal)
.\Out\x64\Debug\AdminHelper\AdminHelper.exe
```

The tool prints its pipe name, PID, and available actions, then listens for connections. Press **Ctrl+C** to stop. Network adapters are always re-enabled on exit (safety net).

## Interactive Keyboard Shortcuts

While running, press these keys for quick actions:

| Key | Action |
|-----|--------|
| `1` | Toggle network (disable if up, enable if down) |
| `e` | Force enable network |
| `d` | Force disable network |
| `p` | Self-ping (verify pipe server is alive) |
| `s` | Show current network adapter status |
| `?` | Show help |

This is useful if a test leaves your network disabled — just press `1` or `e` to recover.

## Named Pipe Protocol

- **Pipe name**: `PlayFabTestAdminHelper`
- **Direction**: InOut (bidirectional)
- **Security**: Current user SID only (works across elevation levels)
- **Pattern**: One request per connection (connect → send → receive → close)
- **Format**: Newline-delimited JSON

### Request

```json
{"action":"DisableNetwork"}\n
{"action":"EnableNetwork"}\n
{"action":"NetworkFlapping","parameters":{"cycles":5,"intervalMs":500}}\n
{"action":"Ping"}\n
```

### Response

```json
{"success":true,"message":"Network adapters disabled"}\n
{"success":false,"message":"PowerShell failed (exit 1): ..."}\n
```

## Supported Actions

| Action | Parameters | Description |
|--------|-----------|-------------|
| `Ping` | — | Returns `pong`. Health check. |
| `DisableNetwork` | — | Disables all physical network adapters |
| `EnableNetwork` | — | Enables all physical network adapters |
| `NetworkFlapping` | `cycles` (1-100), `intervalMs` (100-30000) | Rapid disable/enable cycles. Always re-enables on failure. |

## Helper Scripts

PowerShell scripts in this folder for quick manual testing:

| Script | What it does |
|--------|-------------|
| `Send-Ping.ps1` | Ping AdminHelper to check it's running |
| `Send-DisableNetwork.ps1` | Disable network via AdminHelper |
| `Send-EnableNetwork.ps1` | Enable network via AdminHelper |
| `Send-Action.ps1 -Action <name>` | Send any action with optional parameters |

## Integration

The test controller (`DeviceStateController.cs`) automatically tries AdminHelper first when handling `DisableNetwork`, `EnableNetwork`, or `NetworkFlapping` actions. If the pipe connection fails (AdminHelper not running), it falls back to direct PowerShell with admin validation.

## Safety

- **ProcessExit handler**: Always re-enables network on exit (even crash/Ctrl+C)
- **NetworkFlapping try/finally**: Re-enables on mid-flap failure
- **Cleanup blocks**: All network test YAMLs include `EnableNetwork` in their cleanup section
- **Keyboard shortcut**: Press `1` or `e` to manually re-enable if something goes wrong
