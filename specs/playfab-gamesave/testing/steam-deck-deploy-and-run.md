# Steam Deck: Deploy, Launch, and Debug (Dev Operations)

Internal operations guide for getting a Windows build onto a physical Steam Deck,
launching it, and instrumenting it — without using the SteamOS Devkit GUI.

Scope: **dev-machine workflow**, not SDK behavior. For SDK/API guidance see
[`docs/game-saves/steam-deck-implementation.md`](../docs/game-saves/steam-deck-implementation.md).

Everything below was verified on real hardware (SteamOS `6.16.12-valve24.5`,
GDK 261000, Proton 11.0 + `SteamLinuxRuntime_4`) on 2026-09-03 while
investigating **AB#62819989**. Where something is inferred rather than observed,
it says so.

---

## 1. Connecting

The SteamOS Devkit Client already sets up passwordless SSH when you pair a Deck.
You do **not** need to run `passwd`, `ssh-copy-id`, or `sudo systemctl enable sshd` —
`sshd` is already `enabled` + `active`, and `sudo` requires a password anyway.

| Thing | Value |
|---|---|
| SSH key | `%LOCALAPPDATA%\steamos-devkit\steamos-devkit\devkit_rsa` |
| User | `deck` |
| Host | `steamdeck.local` (mDNS) — **prefer the raw IP**, mDNS resolution is flaky |
| Devkit GUI settings | `%USERPROFILE%\.devkit-client-gui\settings.pickle` (Python pickle) |
| Devkit discovery service | `http://<deck>:32000/properties.json` (on the **Deck**, not the PC) |

```powershell
$key = "$env:LOCALAPPDATA\steamos-devkit\steamos-devkit\devkit_rsa"
ssh -i $key deck@192.168.50.222 "grep '^ID=' /etc/os-release"   # -> ID=steamos
```

> **Always confirm `ID=steamos` before trusting any result.** Diagnostics run on
> the wrong machine are worse than no diagnostics.

**Find the Deck's IP** from the GUI's saved config, or scan; the devkit service
answers on 32000:

```powershell
Invoke-WebRequest http://<ip>:32000/properties.json -UseBasicParsing
# {"txtvers":1,"login":"deck","settings":"{}","devkit1":["devkit-1"]}
```

### Gotchas

- **The Deck auto-suspends when idle** and drops off the network entirely (not
  even in ARP). It is not crashed — wake it with the power button.
- **`wlan0` is usually the only route.** There is no wired fallback unless you
  dock it. Anything that touches Wi-Fi will kill your own SSH session — see §6.

---

## 2. What a "devkit title" actually is

The devkit GUI's *Title Upload* is just three steps you can replicate over SSH.
There is **no headless mode** — `devkit-gui.exe --help` exposes only
`--verbose`, `--logfile`, `--valve`, `--check-port-timeout`,
`--disable-rsync-compress`, `--with-conemu`, `--with-cmder`.

On the Deck:

```
~/devkit-game/<gameid>/                 # the payload
~/devkit-game/<gameid>-argv.json        # ["Game.exe"]  - the command
~/devkit-game/<gameid>-settings.json    # {"steam_play":"1", ...}
~/devkit-game/<gameid>-env.json         # optional env vars
~/devkit-utils/                         # helper scripts, pushed by the client
```

Registration creates an ordinary **non-Steam shortcut** in
`~/.steam/steam/userdata/<accountid>/config/shortcuts.vdf`, named
`Devkit Game: <gameid>`, tagged `Devkit`, with a `DevkitGameID` field.
Because it is an ordinary shortcut it has a **`LaunchOptions`** field and
supports `%command%` exactly like a normal game.

> ⚠️ **`~/devkit-utils/*` have CRLF shebangs** when pushed from the Windows
> client, so running them directly fails with:
> ```
> env: 'python3\r': No such file or directory
> ```
> Invoke them as `python3 <script>` to bypass the shebang.

---

## 3. Deploying a build over SSH

### 3a. Trim the payload

An MSBuild `x64\Debug` output directory is mostly PDBs and objs — in our case
**403 MB, of which only ~25 MB is runtime**. Note this sample sets
`IntDir == OutDir`, so `.obj`/`.tlog` land in the output dir too. Copy only
`*.exe`, `*.dll`, `*.png`, `*.config` (+ any data files).

### 3b. Push and register

```powershell
$key = "$env:LOCALAPPDATA\steamos-devkit\steamos-devkit\devkit_rsa"
$deck = "deck@192.168.50.222"
$gid  = "MyTitleDiag"

ssh -i $key $deck "python3 ~/devkit-utils/steamos-prepare-upload --gameid $gid"
scp -i $key -r "C:\stage\*" "${deck}:/home/deck/devkit-game/$gid/"
```

Then register with the **running** Steam client. Note `settings` is a *nested*
object — `save_settings()` only merges `data['settings']`:

```bash
cd ~/devkit-utils
cat > /tmp/parms.json <<'JSON'
{"gameid":"MyTitleDiag",
 "directory":"/home/deck/devkit-game/MyTitleDiag",
 "argv":["MyTitle.exe"],
 "env":{},
 "settings":{"steam_play":"1","steam_play_debug":"0"}}
JSON
python3 steam-client-create-shortcut --parms "$(cat /tmp/parms.json)"
# -> {"success": ""}
```

> 🔑 **`steam_play` must be `"1"` for a Windows `.exe`.** This is the GUI's
> *"The title requires Steam Play"* checkbox. With `"0"` Steam tries to exec the
> PE natively, and — critically — **no pressure-vessel container is created**,
> which silently invalidates any container-level instrumentation.

Steam must be running; the script talks to it over `~/.steam/steam.pipe` and
waits for a file response. If Steam is down you get
`"The Steam client is not running. Registration did not complete."`

### 3c. Confirm it went through Proton

A Proton launch creates a prefix. Its existence is the cheapest proof:

```bash
ls -d ~/.local/share/Steam/steamapps/compatdata/<appid_u32>
```

**First launch is slow** (ours built a 306 MB prefix, taking well over 40 s).
Warm it up once before any timed experiment.

Debug CRT builds do run under Proton, provided the post-build step copied
`ucrtbased.dll`, `msvcp140d.dll`, `vcruntime140d.dll`, `vcruntime140_1d.dll`
next to the exe.

---

## 4. Launching remotely

`~/.steam/steam.pipe` accepts `steam://` commands from any process running as
`deck`, so this works over SSH into a running Gaming Mode session:

```bash
steam "steam://rungameid/<rungameid>" &
```

### Computing `rungameid` for a non-Steam shortcut

`shortcuts.vdf` stores `appid` as a **signed** int32. Convert to unsigned, then:

```python
rungameid = (appid & 0xFFFFFFFF) << 32 | 0x02000000
```

Worked example: `appid = -1921441633` → u32 `2373525663` →
`rungameid = 10194215098835271680`.

`shortcuts.vdf` is binary VDF (`0x00` nested map, `0x01` string, `0x02` int32,
`0x08` end-of-map). No `vdf` Python module is installed on the Deck; a ~60-line
parser is enough. Useful fields: `AppName`, `Exe`, `StartDir`, `LaunchOptions`,
`DevkitGameID`, `appid`.

**Kill a running Proton title:** quitting from the Steam UI is cleanest. The
game process name is the `.exe`; `pgrep -f <Game>.exe` finds it, but beware
your own shell matching the pattern.

---

## 5. Launch Options — and the container boundary

> This is the single most important, least obvious thing in this document.

### 5a. Launch Options cannot be scripted while Steam runs

`LaunchOptions` lives in `shortcuts.vdf`, which Steam holds open and **rewrites
from memory on exit**. Any write while Steam is running is silently clobbered.
Exiting the *game* does not release it — only exiting *Steam* does, and
restarting Steam on a Deck in Gaming Mode risks stranding the device.

**Practical options:**
1. Set it once by hand in the Steam UI (⚙ → Properties → Launch Options). ~20 s.
2. Point Launch Options at a **wrapper script path** you control, then change the
   *script* instead of the Launch Options — no UI round-trips afterwards.

Option 2 is strongly preferred for iterating.

### 5b. A launch-options prefix runs on the HOST, not in the container

For a Proton title, `%command%` does **not** expand to the game. It expands to a
chain that *creates* the container partway through. Real captured expansion:

```
argv[0]  = .../ubuntu12_32/steam-launch-wrapper
argv[1]  = --oom-score-adjust
argv[2]  = 900
argv[3]  = --                                     <- host side
argv[4]  = .../ubuntu12_32/reaper
argv[5]  = SteamLaunch
argv[6]  = AppId=2373525663
argv[7]  = --                                     <- host side
argv[8]  = .../SteamLinuxRuntime_4/_v2-entry-point   <- creates the container
argv[9]  = --verb=waitforexitandrun
argv[10] = --                                     <- CONTAINER BOUNDARY
argv[11] = .../Proton 11.0/proton
argv[12] = waitforexitandrun
argv[13] = .../MyTitle.exe
```

So `mytool.sh %command%` runs `mytool.sh` **on the host**, before the container
exists. There are **three** bare `--` separators; only the one *following
`_v2-entry-point`* crosses into pressure-vessel.

To run something inside the container, re-split the argv and re-exec:

```bash
args=("$@"); n=${#args[@]}
idx_entry=-1
for ((i=0;i<n;i++)); do case "${args[$i]}" in *entry-point) idx_entry=$i;; esac; done
idx_sep=-1
for ((i=idx_entry+1;i<n;i++)); do [ "${args[$i]}" = "--" ] && { idx_sep=$i; break; }; done
exec "${args[@]:0:idx_sep+1}" /path/to/tool "${args[@]:idx_sep+1}"
```

**Symptom of getting this wrong:** your tool reports it is not in a container and
`/run/host` does not exist. It is *not* evidence that the title skipped Proton.

### 5c. What the container looks like from inside

| Signal | Value |
|---|---|
| Host filesystem | mounted at `/run/host` |
| Runtime | `PRESSURE_VESSEL_RUNTIME`, e.g. `steamrt4_platform_4.0.20260805.254769` |
| `$HOME` | still `/home/deck` (shared), so logs written to `~` land on the host |
| `/etc/resolv.conf` | **is** a mountpoint: `fstype overlay`, `root in src /resolv.conf` |

Because `/run/host` exposes the real host filesystem, a single process inside the
container can compare both views without correlating two log files.

---

## 6. Toggling Wi-Fi without stranding the Deck

`nmcli radio wifi off` / `on` works, **but your SSH session rides `wlan0`**, so
turning it off kills your control channel and you cannot turn it back on.

Always arm a deadman *first*, and run the sequence detached so it survives the
disconnect:

```bash
setsid nohup sh -c "sleep 460; nmcli radio wifi on" >/dev/null 2>&1 </dev/null &

systemd-run --user --unit=my-run --collect /home/deck/my-sequence.sh
```

`systemd-run --user` works fine over SSH and outlives the session.
Poll `systemctl --user is-active my-run.service` to know when it finishes.

If you instrument the container from launch options, remember the watcher is a
child of the container: **quitting the game tears the container down and kills
it**, often before it writes its summary. Outlast the measurement window.

---

## 7. Gotcha summary

| Symptom | Cause |
|---|---|
| `env: 'python3\r': No such file` | CRLF shebangs in `~/devkit-utils`; use `python3 <script>` |
| `bash: /path/x.sh^M: bad interpreter` | Script copied from Windows with CRLF. Repo `.gitattributes` enforces `*.sh eol=lf`; preserve it when staging |
| Title exits instantly / no container | `steam_play` is `"0"` — must be `"1"` for a Windows exe |
| Tool says "not in a container" | Launch-options prefix ran host-side; inject after the `_v2-entry-point` separator (§5b) |
| Launch Options edit disappears | Steam rewrote `shortcuts.vdf` on exit |
| Deck unreachable, not in ARP | Auto-suspend. Wake with power button |
| SSH dies mid-experiment | You turned off the Wi-Fi you were connected over (§6) |
| First launch takes minutes | Proton building the prefix under `compatdata/<appid_u32>` |
| `steamos-prepare-upload` "not running" | Steam client must be up for shortcut registration |

---

## 8. Case study — AB#62819989 (Steam Deck stays offline after Wi-Fi returns)

**Theory tested:** the container bind-mounts `/etc/resolv.conf`, pinning an
inode; when the host republishes by `rename()` the container stays on the stale,
empty file forever.

**Method:** [`Tools/SteamDeckNetworkDiag`](../../../Tools/SteamDeckNetworkDiag)
watcher injected into the container per §5b; Wi-Fi off → launch offline →
Wi-Fi on → hold ≥ watcher window.

**Result: `NO_LATCH`** — a real negative.

```
[14:24:44] me=1c:32987|127.0.0.53  dns=FAIL || host=1c:32987|127.0.0.53   <- offline
[14:25:12] me=1c:32987|127.0.0.53  dns=OK   || host=1c:33014|127.0.0.53
    *** CHANGE (host view) ***  was: 1c:32987  now: 1c:33014
[14:28:38] me=1c:32987|127.0.0.53  dns=OK   || host=1c:33014|127.0.0.53   <- recovered
```

Findings worth keeping:

- **NetworkManager does republish `/etc/resolv.conf` with a new inode**
  (`32987` → `33014`), so the atomic-replace premise holds. An in-place rewrite
  would have made the experiment unscoreable.
- **The container never followed the inode** — the pinning is real.
- **It is harmless on SteamOS.** `/etc/resolv.conf` only ever contains
  `nameserver 127.0.0.53`, the systemd-resolved stub. That address is invariant
  across network changes, so a stale copy is byte-identical to a fresh one, and
  resolution recovers the moment systemd-resolved has upstream DNS again.

**Conclusion:** the resolver-latch mechanism exists but is *not* the cause of the
reported bug. Look above the resolver — Wine/Proton DNS caching inside the
prefix, or XAL / libHttpClient holding a failed state after `E_XAL_NETWORK`.

> The host's `/etc/resolv.conf` is a plain NetworkManager-generated file — not a
> symlink and not a mountpoint — pointing at the systemd-resolved stub.
> Don't assume the usual `/run/systemd/resolve/stub-resolv.conf` symlink layout.
