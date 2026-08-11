# watchdog

One Python supervisor for any daslang program that needs to stay up. It restarts the child with
bounded backoff, captures crashes into bundles, reports startup progress, and optionally exposes a
per-program control page.

It supervises two things in-tree today — `utils/dasllama-server` (JIT) and
`examples/telegram/dictation` (a baked exe) — and was merged from the two forks those grew.

## Running it

In a deployed bundle, with no arguments:

```
python watchdog.py
```

That works because the watchdog resolves what to supervise in this order, first match winning:

1. **A command-line flag** — `--program`, `--script`, `--name`, and the rest (`--help` lists them).
2. **`watchdog.json`** beside `watchdog.py` — every key sets the *default* for the flag of the same
   name, so a flag still overrides it. An unknown key is a hard error: silently ignoring a typo
   would supervise the program with the wrong wiring and nothing would say so.
3. **Layout discovery** — `main.das` next to `bin/Release/daslang.exe` means `daslang -jit main.das`;
   exactly one `*.exe` in the directory means that program. Anything ambiguous is an error, never a
   guess.

Both in-tree programs are discoverable, so their `watchdog.json` only pins the identity that the
log, pid file and notifications key on (`cadmus`, `dasllama`) plus whatever discovery cannot know —
that the bot has no HTTP health endpoint, for instance.

From the source tree the watchdog no longer sits beside what it supervises, so pass `--cwd`:

```
python utils/watchdog/watchdog.py --cwd utils/dasllama-server
```

## Exit codes it acts on

| code | meaning |
|---|---|
| 0 | intentional shutdown — the watchdog stops too |
| 3 | tune bootstrap wrote the sidecar (local tuner OR an exchange download — both print a "restart to apply the winners" marker); relaunch immediately (JIT only) |
| 4 | config restart requested by a control page; relaunch immediately |
| other | crash — report, notify, bundle, restart with bounded exponential backoff |

Exit 3 *without* the marker is a tuner abort (noise gate, or a tray-requested stop). A noise
abort restarts with backoff — an immediate relaunch on a loud box just aborts again. A
tray-requested stop relaunches immediately with the policy the user picked (below).

## Startup stages

A cold JIT start takes minutes (DLL cache miss, codegen, per-box tuning, model load), which used to
look like a hang punctuated by health-check spam. The watchdog now tracks ranked, monotonic stages —
`jit_cached`, `jit_codegen`, `jit_linked`, `exchange_lookup`, `tuning`, `tune_restart`,
`model_load`, `asr_init`, `ready` — and logs a `stage` event on each forward move, with how long
the previous stage took.

Ranked and monotonic matters: a tune runs many codegen/link cycles, and a naive matcher flaps
between stages once per kernel variant. Health is logged only on transition plus a heartbeat, so a
long quiet run stays quiet.

Stages are still detected from the child's own log prose, which is brittle by nature. The planned
replacement is an explicit `das-stage: <name>` token emitted by the program, with these regexes
kept as fallback.

### Tune progress

The one part no longer inferred from prose. `llvm_tune` emits structured progress as
`@tune <kind> k=v ...` lines, and a supervised child never owns a terminal, so it *forwards* those
events rather than drawing a progress bar — which is what makes them ours to consume.

The watchdog folds them into `STATE["tune"]` (`scope`, `done`/`total` kernels, `kernel`,
`round`/`rounds`, `phase`, `live`) for the control plugin's status route, and logs a `tune` event
only at `plan` and `end` — a real tune is hundreds of steps, and logging each one would recreate
the flood these events exist to replace. On a measured run that is **26 log records from 551
events**, where the whole raw tuner output used to be JSON-wrapped into the log line by line.

Every field is a counter. Nothing is an estimate: the tuner cannot know how long it has left, so
no ETA is published and none should be synthesized from these numbers.

### The exchange (`@sidecar` events, the tray's tune actions)

dasllama's exchange client emits `@sidecar <kind> k=v ...` on the same contract (`lookup`,
`none`, `offer`, `apply`, `pending_submit`, `submitted`); the watchdog folds them into
`STATE["sidecar"]` and logs each one. Balloons announce a decision but never carry the action —
the tray menu and the control page do:

- **Use available sidecar instead (stops tuning)** — visible while a tune is in flight AND an
  unverified offer exists. Writes the tune-control file (the tuner polls `DAS_TUNE_CONTROL` at
  kernel-family boundaries, so the measurement in flight always completes), then relaunches with
  a ONE-SHOT `DASLLAMA_EXCHANGE_ACCEPT=any` — consent that never outlives the click.
- **Stop tuning, run untuned** — visible while a tune is in flight. Same stop file, then
  `DAS_TUNE_POLICY=fallback` STICKY for the rest of the watchdog session, so a later
  crash-restart cannot surprise the box with a 20-minute tune. Untuned serving is legitimate.
- **Share this box's tune…** — visible while a `pending_submit` offer stands and the server is
  healthy; opens the control page at the exchange card.

The tray menu is a pure function of `STATE`: pystray re-evaluates the item callables when the
menu opens, and the supervision tick calls `update_menu()` on a state transition so an
already-open menu cannot go stale.

## Control plugin

If a `watchdog_control.py` sits beside `watchdog.py`, the watchdog imports it and calls
`start(logger, args)`, injecting `host` (the watchdog module) first so the plugin can use
`host.emit(...)` and `host.read_state()` rather than re-declaring them and drifting.

Supervision stays generic; anything that knows what a *particular* program is lives in the plugin.
`examples/telegram/dictation/watchdog_control.py` is the worked example: it owns `dictation.toml`,
the prompt set, generation defaults and the activity feed, and is ~480 lines that have no business
in a supervisor. A plugin that fails to import is reported and skipped, never fatal — keeping the
program alive outranks being able to reconfigure it.

`host.read_state()` publishes supervision state (`child_pid`, `child_started_at`,
`child_exit_code`, `restart_delay`, `stage`, `tune`, `sidecar`) for the plugin's status route.

## Shipping it

`watchdog.py` lives outside every package, so manifests reach it with `release_include_from`, which
sources relative to the daslang root:

```das
release_include_from("utils/watchdog/watchdog.py")
release_include("watchdog_control.py")   // package-local, if the program has one
release_include("watchdog.json")
```

A missing source fails the release rather than shipping a bundle quietly short a file.

## Crash capture

Crash bundles collect the log, any WER minidump, the program's symbols, the tune sidecar and the JIT
artifact cache into `logs/crashes/<name>-<stamp>-pid<pid>/`, pruned to the newest N. WER local dumps
need a one-time elevated `--install-local-dumps`; `--require-dumps` refuses to start without a
policy, for a deployment where losing the dump is not acceptable. See `examples/crash/README.md` for
which failure families are visible to which tier.
