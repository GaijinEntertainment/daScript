# watchdog

One supervisor for any daslang program that needs to stay up. It restarts the child with
bounded backoff, captures crashes into bundles, reports startup progress, and polls health.
It supervises `utils/dasllama-server` (JIT) in-tree and the dictation bot in the das-telegram
package (a baked exe).

It ships as a static executable, `bin/watchdog` (`bin/Release/watchdog.exe` in an MSVC tree): a
`-ctx` standalone context on the full runtime with `libDaScript`, dasHV and dasStdDlg linked as
static archives. It compiles nothing at run time, loads no shared module, and holds no lock on any file
a deploy replaces, so a deploy can overwrite the runtime while the watchdog runs. The same code
runs under the interpreter for development: `daslang utils/watchdog/main.das -- --cwd <dir>`.

## Running it

In a deployed bundle, beside the program, with no arguments:

```
./watchdog
```

That works because the watchdog resolves what to supervise in this order, first match winning:

1. **A command-line flag** - `--program`, `--script`, `--name`, and the rest (`--help` lists them).
2. **`watchdog.json`** beside the executable - every key is a flag name with underscores, and
   sets the *default* for that flag, so a flag still overrides it. An unknown key is a hard error:
   silently ignoring a typo would supervise the program with the wrong wiring and nothing would
   say so. A `server_args` array is the default for what follows `--`.
3. **Layout discovery** - `main.das` means `daslang -jit main.das`, with the daslang found beside
   it (`bin/Release/daslang(.exe)`) or beside the watchdog itself, which is how `bin/watchdog`
   in a source tree finds `bin/daslang`; exactly one `*.exe` in the directory means that
   program. Anything ambiguous is an error, never a guess.

Everything after `--` goes to the child verbatim. From the source tree the watchdog does not sit
beside what it supervises, so pass `--cwd`:

```
bin/watchdog --cwd utils/dasllama-server
```

## Exit codes it acts on

| code | meaning |
|---|---|
| 0 | intentional shutdown - the watchdog stops too |
| 3 | tune bootstrap wrote the sidecar (the child printed "restart to apply the winners"); relaunch immediately (script mode only) |
| 4 | config restart requested by a control page; relaunch immediately |
| other | crash - report, notify, bundle, restart with bounded exponential backoff |

Exit 3 *without* the marker is a tuner abort (the noise gate found the box too loud to trust);
it restarts with backoff, because an immediate relaunch on a loud box just aborts again. A code
comes back from the child's `main`: a das `exit(N)` is an abnormal termination and reports 1.

## Stopping it

Ctrl-C, SIGTERM or the tray's `Shutdown` item asks the child to stop: through `--stop-file` (the path is handed to the child
in the `--stop-env` variable, `CADMUS_STOP_FILE` by default, and the file is created on the
request), through a POST to `--shutdown-url`, or with `--no-shutdown` by terminating it. The
stop is a ladder with an end: after `--stop-timeout` seconds the child is terminated, ten
seconds later killed, and ten seconds after that left behind (`child_unkillable`, exit 1).
Supervision otherwise ends with 0.

Two watchdogs would mean two children fighting over one port, so the pid file (`--pid-file`,
`logs/<name>-watchdog.pid` by default) both records and protects: a second start refuses while
the recorded pid is alive.

## Startup stages

A cold JIT start takes minutes (DLL cache miss, codegen, per-box tuning, model load), which
would otherwise look like a hang punctuated by health-check spam. The watchdog tracks ranked,
monotonic stages from the child's own log lines - `jit_cached`, `jit_codegen`, `jit_linked`,
`exchange_lookup`, `tuning`, `tune_restart`, `model_load`, `asr_init`, `ready` - and logs a
`stage` event on each forward move, with how long the previous stage took. Ranked and monotonic
matters: a tune runs many codegen/link cycles, and a naive matcher flaps between stages once
per kernel variant; `tune_restart` is the one legitimate rewind, since the process restarts to
apply the winners. Health is logged only on transition plus a heartbeat every five minutes, so a
long quiet run stays quiet.

`llvm_tune` emits structured progress as `@tune <kind> k=v ...` lines; the watchdog logs a
`tune` event at the kernel boundaries (`plan`, `end`, `abort`) and drops the hundreds of steps
between them.

## The log

`logs/<name>-watchdog.log` (`--log`), one JSON object per line, `{"ts", "event", ...}`, rotated
at 20 MB with five backups, and echoed to stdout. The events: `watchdog_started`,
`child_started`, `spawn_failed`, `child` (one per line the child wrote), `stage`, `tune`,
`health`, `health_heartbeat`, `recovered`, `child_exited`, `intentional_shutdown`,
`tune_bootstrap_complete`, `tune_incomplete`, `config_restart_relaunch`, `crash`,
`crash_bundle`, `stop_file_requested`, `shutdown_requested`, `shutdown_request_failed`,
`terminate_requested`, `kill_requested`, `child_unkillable`, `watchdog_already_running`,
`wer_ready` / `wer_not_ready` / `wer_installed` / `wer_install_failed`, `tray_started`,
`tray_unavailable`, `tray_open_requested`, `tray_open_failed`, `tray_shutdown_requested`,
`watchdog_stopped`.
In-tree readers: `smoke_test.cmake` and `tests/watchdog/test_watchdog.das`.

## Crash capture

Crash bundles collect the log, any WER minidump, the program's symbols, the tune sidecar and
the JIT artifacts the child named into `logs/crashes/<name>-<stamp>-pid<pid>/`, pruned to the
newest `--crash-bundles`. WER local dumps need a one-time elevated `--install-local-dumps`
(Windows); `--require-dumps` refuses to start without a policy, for a deployment where losing
the dump is not acceptable. See `examples/crash/README.md` for which failure families are
visible to which tier.

## The tray icon

`--tray` (the `tray` key in `watchdog.json`) puts a status icon in the notification area: a
disc, plain while the child serves, wearing an amber triangle while it starts or tunes and a red
square when it is unhealthy, crashed or waiting to restart. The tooltip and the menu's first row
carry one status line - `starting up - loading the model`, `tuning this box - 3/12 kernels`,
`serving - healthy 2h05m`, `unhealthy (ready)`, `restarting in 4s (exit 9)`. `Open <name>`, also
a left click, opens `--tray-url` (by default the health URL's origin) and is enabled only while
the health check is green; `Shutdown` enters the same stop ladder as Ctrl-C. Where no desktop,
session bus or backend can show an icon, the log says `tray_unavailable` and supervision runs
without one: the icon is never fatal. On Linux it is a StatusNotifierItem over the session bus,
so a systemd service with no `DBUS_SESSION_BUS_ADDRESS` gets none.

## Notifications

A crash, a recovery and a refused start raise a desktop notification: through the tray icon
while it is up, otherwise a PowerShell balloon on Windows, Notification Center through
`osascript` on macOS, `notify-send` on Linux, nothing where none exists. Never fatal.

## Shipping it

A package manifest names the tool once for every platform:

```das
release_include_tool("watchdog")         // bin/watchdog or bin/Release/watchdog.exe -> bundle root
release_include("watchdog.json")
```

A build without the executable fails the release rather than shipping a bundle quietly short
a supervisor.

## Layout

- `watchdog.das` - the library: configuration, discovery, the log, stages, crash capture, the
  tray, and `Supervisor`, a state machine the host ticks (`tick()` / `request_stop()` / `run()`).
- `main.das` - the entry for both hosts: `start` / `tick` / `request_stop` / `result` for the
  executable, `main` for the interpreter.
- `main.cpp` - the executable's `main`: argv, the pid, the signals, the loop.
- `smoke_test.cmake` - the `watchdog_smoke` ctest: the binary supervising a daslang child through
  a crash and a clean exit. The library's own tests are `tests/watchdog/`.
