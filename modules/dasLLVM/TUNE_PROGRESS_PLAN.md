# Tune UX rework — progress events and a single renderer

Design notes for replacing the tuner's current terminal output (a per-variant firehose that
mostly restates the sidecar JSON) with a progress display. Status: **design, not yet built.**

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
| `kernel.step` | inner index | the bench loop |
| `kernel.end` | kernel name, winner suffix, verdict (`holds` / `beats` / `rejected`) | each tuner half |
| `scope.end` | tuned count, elapsed | llvm_tune |
| `restart` | — | llvm_tune (see §5) |
| `error` | message | anywhere |

Renders as roughly:

```
[#####-------------]  9/25   dot_q8q8   47/128        1m12s
```

Elapsed comes free from `ref_time_ticks`. No ETA — the segments are too uneven (the 25 `[tuned]`
kernels are seconds each; the q8q8 grid and the confirm prefill dominate wall clock). The inner
counter is what keeps the long segments honest, which is why time-weighting is not needed.

The confirm pass gets its own `kernel.begin`/`end` pair so it is not an invisible 40-second stall
at `24/25`.

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

Three levels, carried by env because it has to reach four process levels and
`DAS_TUNE_MODE`/`DAS_TUNE_MANIFEST` already inherit that way with no forwarding code:

`DAS_TUNE_VERBOSITY=silent|normal|verbose`, set from `--tune-quiet` / `--tune-verbose` on the app
alongside the existing `--tune` / `--tune-fast`.

- **silent** — nothing. (Forwarding is unconditional, so a supervised silent run still feeds the
  watchdog; the renderer is simply drawing nothing. Verbosity gates the renderer only.)
- **normal** — the bar, then the winner summary.
- **verbose** — today's full per-variant output, no bar (a bar interleaved with scrolling lines is
  unreadable).

`TUNE_KERNEL_TIME` / `TUNE_GEN_TIME` are grep-only markers — nothing parses them, so they demote
to `verbose` safely. `CONFIRM_PP:` is parsed and must stay exactly as is.

## Open

- Prefix spelling for event lines.
- Whether the final winner summary is the current `===== summary =====` block or a tightened one.
- Whether `--tune-quiet`/`--tune-verbose` go through `daslib/clargs` (the tuner currently hand-rolls
  `get_command_line_arguments()`; `skills/clargs_usage.md` has the migration discipline).
