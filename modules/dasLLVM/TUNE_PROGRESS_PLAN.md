# Tune UX rework — progress events and a single renderer

Design notes for replacing the tuner's current terminal output (a per-variant firehose that
mostly restates the sidecar JSON) with a progress display.

**Status.** §1 (live relay), §2/§3 (event protocol + renderer), §4 (`daslib/tty` + binding) and
the §9 up-front promise are **built and tested**. Still open: §7 (clargs flags), §8
(`gen_tune_probe` honouring `--tune-fast`), wiring the emitters into the three real dasLLAMA
tuners, the §5 watchdog migration, and the doc updates.

Framework reference: `skills/llvm_tune.md` (internals), `skills/tune.md` (user-facing).

## The shape today

Four process levels, each spawned by the one above:

```
app  (llvm_tune guard prepended to main)      llvm_tune.das:1045 tune_auto_prepare
 └─ dasllama_tuner.das                        the scope tuner named by [tune_scope]
     ├─ gen_tune_probe.das                    the [tune] generator grid
     │   └─ tune_confirm_prefill.das          end-to-end confirm (parsed, not relayed)
     └─ tune_kernels.das                      the [tuned] loop-hint grid + runtime knobs
```

Every measurement print lives at level 3. The user only ever looks at level 1.

## 1. The blocker — level 1 buffers instead of relaying

`run_scope_tuner` ([llvm_tune.das:1021](daslib/llvm_tune.das)) redirects the tuner to a temp
file with an **empty** popen callback, then prints the file after the child exits:

```das
let cmd = "{tuner_cmd(scope)} > \"{tmp}\" 2>&1"
unsafe(popen(cmd) $(_f) {})
let outp = fread(tmp)
```

So on the default auto policy — the user-facing path — there is no output at all for the whole
tune (timeout is 3600 s), then a wall of text. No progress display is possible until this is a
live relay. `dasllama_tuner.das:59-67` and `tune_auto_reexec` ([llvm_tune.das:1079](daslib/llvm_tune.das))
already do it correctly (`while (!feof(f)) { print(fgets(f)) }`) — copy that shape and drop the
temp file.

**This is worth landing on its own**, ahead of any bar.

## 2. Event protocol

Events everywhere, one renderer. The vocabulary and the renderer both live in
`modules/dasLLVM/daslib/llvm_tune.das` — beside the `[tune]` macro — so any library's tuner gets
the display for free, not just dasLLAMA's.

Transport is **prefixed lines on stdout**: the levels are separate processes, so a pipe is the
only channel that exists. Prefix must be unmistakable and must not collide with `CONFIRM_PP:`
(a live contract between `gen_tune_probe` and `tune_confirm_prefill`).

Vocabulary (two-level counters, no percentage — an outer kernel count and an inner variant
count, per Boris):

| event | payload | emitted by |
|---|---|---|
| `scope.begin` | scope name, sidecar path | llvm_tune (level 1) |
| `plan` | outer total for this half | each tuner half |
| `kernel.begin` | kernel name, inner total (variant count) | each tuner half |
| `kernel.step` | inner index, pass name, live-candidate count | the bench loop |
| `kernel.end` | kernel name, winner suffix, verdict (`holds` / `beats` / `rejected`) | each tuner half |
| `scope.end` | tuned count, elapsed | llvm_tune |
| `restart` | — | llvm_tune (see §5) |
| `error` | message | anywhere |

Renders as roughly:

```
[#####-------------]  9/25   dot_q8q8   finalists 47/80   4 live   1m12s
```

**No duration estimate, ever.** We genuinely do not know it — the segments are wildly uneven (the
25 `[tuned]` kernels are seconds each; the q8q8 grid and the confirm prefill dominate wall clock)
and a wrong ETA is worse than none. Everything displayed is *observed*, never predicted. Elapsed
is fine on those terms and comes free from `ref_time_ticks`. This is also why time-weighting the
bar is out and the two-level counter is in.

The three honest axes, beyond the outer `n/m`:

- **inner `n/m`** — rounds done against this kernel's round budget.
- **pass name** — `tune_kernels` really does run distinct passes (`tune_candidate_active`,
  tune_kernels.das:127): full-grid `screening` through `SCREEN_ROUNDS`, a fast-only `narrowing`
  phase (top 8 / within 10%) through `FAST_MID_ROUNDS`, then `finalists` (top 4 / within 5%) to
  `ROUNDS`. Naming the pass explains why the inner counter's cost per round drops partway through.
- **live-candidate count** — the grid narrows as passes advance; `4 live` out of a 28-row grid is
  the single most informative "something extra" available, and `screening_keep` already computes
  it per round.

The confirm pass gets its own `kernel.begin`/`end` pair so it is not an invisible stall at `24/25`.

## 3. Renderer vs forwarder — one rule

A process **renders** if its own stdout is a terminal; otherwise it **forwards events upward
verbatim** and renders nothing. That single rule gives:

- interactive run → level 1 has a tty, renders the bar; levels 2-3 are piped, so they forward.
- under `watchdog.py` → level 1 is also a pipe, so it forwards too, and the watchdog receives the
  full event stream (§5).
- redirected to a file / CI → no tty anywhere, no `\r` garbage in the log, events still present.

Forwarders pass everything through untouched. Only the renderer filters by verbosity, so there is
exactly one place that decides what a human sees.

Exception: `gen_tune_probe` *parses* `tune_confirm_prefill` rather than relaying it — that level
emits its own events for the confirm stage instead of forwarding the child's output.

## 4. `daslib/tty` — new module, needs a binding

There is **no tty detection in daslang today**. `isatty` appears only in vendored
doctest/fmt/gtest C++; nothing in `include/daScript` or `src/`.

`daslib/ansi_colors.das` exists and is fine for *styling*, but its detection is a `TERM`/`NO_COLOR`
heuristic, not an fd probe — `TERM=xterm-256color` is still set when you pipe to a file, so it
guesses wrong on exactly the case that matters here. Keep the two modules separate: colors =
styling, tty = capability probe.

New `daslib/tty`:

- `is_terminal(fd)` — the renderer/forwarder decision above
- `terminal_width()` — bar sizing; fall back to 80

Binding goes in `src/builtin/module_builtin_fio.cpp` (`isatty`/`_isatty`, plus `TIOCGWINSZ` /
`GetConsoleScreenBufferInfo` for the width). Optionally `init_ansi_colors` grows to consult it
later — adjacent win, not required for this work.

## 5. `watchdog.py` is a live consumer — do not break it

`utils/watchdog/watchdog.py` supervises `dasllama-server` and reads the child's stdout line by
line into a rotating log (`stdout=PIPE`, `stderr=STDOUT`, `bufsize=1`). Two llvm_tune strings are
load-bearing:

| string | used for | site |
|---|---|---|
| `llvm_tune: tuning scope` | startup stage `tuning` (rank 3) | watchdog.py:527 |
| `llvm_tune: restart to apply the winners` | stage `tune_restart` **and** `tune_restart_seen.set()` | watchdog.py:528, 578 |

The second one gates the exit-code-3 decision at watchdog.py:979 — whether a tune bootstrap exit
counts as expected or as a crash. Drop that string and a normal first-run tune reads as a crash
and enters backoff restart.

The watchdog comment at watchdog.py:518-521 also notes it already has to defend against tuner
output volume ("the JIT lines fire hundreds of times mid-tune and would otherwise flap the
stage"). So it is currently regexing human prose to recover structure the tuner never emitted —
the event stream is the proper fix, and migrating `STARTUP_STAGES` onto it is part of this work,
not a follow-up.

**Migration order.** `daspkg release` ships `watchdog.py` beside the program, so a deployed old
watchdog can meet a new daslang. Emit both events *and* the two legacy strings this round, move
the watchdog to events in the same PR, drop the legacy strings a release later.

## 6. Verbosity

Three levels. **`daslib/clargs` is the front door; env is the transport** — the flag is declared
once in a clargs spec, and level 1 exports `DAS_TUNE_VERBOSITY=silent|normal|verbose` so it
reaches all four process levels the same way `DAS_TUNE_MODE`/`DAS_TUNE_MANIFEST` already do, with
no forwarding code. These are not competing mechanisms: nothing below level 1 parses the flag.

New flags `--tune-quiet` / `--tune-verbose`, alongside the existing `--tune` / `--tune-fast`.

- **silent** — nothing. (Forwarding is unconditional, so a supervised silent run still feeds the
  watchdog; the renderer is simply drawing nothing. Verbosity gates the renderer only.)
- **normal** — the bar, then the winner summary.
- **verbose** — today's full per-variant output, no bar (a bar interleaved with scrolling lines is
  unreadable).

`TUNE_KERNEL_TIME` / `TUNE_GEN_TIME` are grep-only markers — nothing parses them, so they demote
to `verbose` safely. `CONFIRM_PP:` is parsed and must stay exactly as is.

## 7. clargs migration

Every flag in this rail goes through `daslib/clargs` — including migrating the hand-rolled
parsers already there, per the standing rule in `skills/clargs_usage.md` ("when you edit any
in-tree tool that still parses `get_command_line_arguments()` directly, migrate it in the same
PR"). This work edits all of them.

The producer side is *already* clargs — `daspkg` declares `--tune-fast` as a real field
(`utils/daspkg/commands.das:68-70`) and emits it at commands.das:1434. Only the consumers
hand-roll. That asymmetry is the whole argument.

Sites:

| site | flag | note |
|---|---|---|
| `llvm_tune.das:991` `cli_forces_tune` | `--tune` | macro-time; shares the app's argv |
| `dasllama_tuner.das:39` `tune_fast_requested` | `--tune-fast` | |
| `tune_kernels.das:137` `tune_fast_requested` | `--tune-fast` | verbatim duplicate of the above |
| `gen_tune_probe.das` | `--tune-fast` | has **no** parser today — add one (§8) |

**Accessor: `get_user_args()`.** Mode-aware — `argv[1..]` under a standalone `-exe`, the post-`--`
slice otherwise (clargs.das:70). `cli_forces_tune` currently hand-walks for `--` then `--tune`,
which is only ever right for the interpreted case; the accessor makes the exe case correct for
free. (The policy rail is dead under `-exe` today, so this is not a live bug.)

**Sharing argv with the app.** llvm_tune parses an argv it does not own, so it needs the two-arg
`parse_args(type<TuneCliArgs>, argv)` form — same as `ReleaseDepsArgs` in
`modules/dasLLVM/daslib/llvm_exe.das:791`, which `skills/clargs_usage.md` names as *the* precedent
for this exact case, and which lives in this module already.

One hazard to record: clargs skips unknown flag-shaped tokens **without consuming their value**
(clargs.das:320-326). So an app invoked as `... -- --model foo.gguf --tune` leaves `foo.gguf` in
the positional list. Harmless here — the tune spec is bool fields only and never reads
positionals — but **the tune spec must never grow a positional or a value-taking flag whose value
could be confused with an app token.**

Two smaller points:

- The single-dash `-tune-fast` form the hand-rolled parsers also accept is never produced by
  anything (daspkg, dasllama_tuner, and the docs all emit `--tune-fast`). clargs drops it; safe.
- `--tune` stays stripped from the re-exec ([llvm_tune.das:1072](daslib/llvm_tune.das)) so the
  child converges. `--tune-quiet` / `--tune-verbose` must *survive* it — a quiet run must stay
  quiet across the relaunch.
- Help flag is `-?`, not `-h`: the daslang host intercepts `-h`/`--help` before forwarding script
  args.

## 8. `gen_tune_probe` must honour `--tune-fast`

`dasllama_tuner.das:51` passes `-- --tune-fast` to **both** halves and banners `mode = FAST`, but
`gen_tune_probe.das` has no parser for it — the flag is silently dropped, and the banner's 20/80
and 4/8/20 budgets are `tune_kernels`' numbers only. That half is a flat best-of-`ROUNDS = 6`
(gen_tune_probe.das:49, a bare constant with no justifying comment) across three family loops
(gen_tune_probe.das:929, :1470, and the q51 loop).

**What fast should mean here.** There is no screening phase to narrow — the only dial is the flat
round count. `tune_kernels` fast cuts its total budget 80 → 20 (4×) and its full-grid screening
20 → 4 (5×). The proportionate move is **`ROUNDS` 6 → 3 under fast** (6 → 2 if we want to match
the 3× more closely). Don't port the screening machinery: it pays when the grid is large and the
round count is high, which is why `tune_kernels` has it and a best-of-6 loop does not.

**Do not touch the confirm pass under fast.** `CONFIRM_ROUNDS = 2` is already minimal, and the
confirm is a *correctness* gate, not a measurement budget — cutting it ships an unconfirmed crown,
which is the exact failure it exists to prevent ("the SPR amx lesson", gen_tune_probe.das:1310).
Note its cost is mostly the unconditional `discover_fallback_stamp()` compile-only child; the
model-prefill arms only run when the winner diverges from the fallback *and*
`DASLLAMA_CONFIRM_MODEL` is set, so on most boxes they never run at all.

The parser goes in as clargs from the start rather than hand-rolled-then-migrated, so this is part
of §7 rather than a separate change.

## 9. The supervised scenario — this is the goal

Start `dasllama-server` under the watchdog. If it is untuned it should say *tuning, this will take
a while, we will restart when it is done*, show progress, finish, restart, then run normally.

That narrative is the `restart` policy rail, and `utils/dasllama-server/main.das:882` is
`[tune_policy(missing = "restart")]` — the only one of the four entry points that is (`ask.das`
and `wav2txt.das` are `auto`). The skeleton is already correct end to end:

| step | mechanism today | gap |
|---|---|---|
| start | watchdog spawns `daslang -jit main.das` | — |
| "tuning" | `tune_restart_needed` → `run_scope_tuner` prints `llvm_tune: tuning scope` → watchdog stage `tuning` (rank 3) | **no up-front "this will take a while, we will restart" promise** |
| progress | — | **nothing — `run_scope_tuner` buffers the whole tune (§1)** |
| "done" | `llvm_tune: scope '<x>' tuned -> <path>` | present, but buried in the buffered wall |
| "we will restart" | `llvm_tune: restart to apply the winners` → stage `tune_restart` (rank 4) + `tune_restart_seen.set()` | present |
| restart | main returns 3 → `TUNE_RESTART_EXIT` → relaunch, no backoff | present |
| normal | stages `model_load` → `asr_init` → `ready` | present |

So this is wiring, not new architecture. Two gaps only: the up-front promise, and the silence.

**§1 is the direct cause of the thing the stage machinery was written to paper over.** watchdog.py
comments at :514-516 that "startup is several minutes of work with no single progress signal ...
the log is only `health ok:false` repeated for the whole window, which says the server is down but
never what it is doing." That window *is* the buffered relay. Fix §1 and the window has content.

`advance_stage` is rank-monotonic with `tune_restart` as the one legitimate rewind — it resets the
rank so the sequence replays after the relaunch (watchdog.py:519-522, :563). That is exactly right
for this narrative; no change needed.

### Consequence: the watchdog is the renderer here

`stream_child` re-emits **every** child line as its own JSON record to both the rotating log and
watchdog's console (`emit(logger, "child", ..., message=message)`, watchdog.py:590). So:

- A `\r`-drawn bar from the daslang side is garbage here — every redraw becomes a JSON record.
  The §3 rule already prevents it (the server's stdout is a pipe, so it forwards and never draws),
  but it means **the drawing code exists twice**: once in llvm_tune, once in watchdog.py. That is
  the honest cost of "single source of draw" — single *per process tree*, with the event protocol
  as the contract that lets either end be the drawer.
- Today the tuner's hundreds of per-variant lines all land in that log wrapped in JSON envelopes.
  A supervised run should default to a quiet verbosity so the log carries ~25 informative event
  records instead of hundreds of noise lines.

### Third surface: `STATE` / the control page

`STATE` (watchdog.py:60-67) is published for the optional `watchdog_control.py` HTTP control
plugin, and carries a coarse `stage` string. For a *server*, nobody watches the terminal — the
control page is the primary display. The tune counters should land in `STATE` alongside `stage`
so the page can render real progress. Cheap: `set_state` is already called on every stage move.

## Open

- Prefix spelling for event lines.
- Whether the final winner summary is the current `===== summary =====` block or a tightened one.
- `ROUNDS` 6 → 3 or 6 → 2 under fast (§8).
- Wording of the up-front promise. It states *that* we will restart, never *when* — no duration,
  estimated or otherwise (§2).
