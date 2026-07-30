# daslang environment variables

Every environment variable daslang itself reads. Module-specific knobs live with their module —
dasLLAMA's ~130 are in `modules/dasLLAMA/ENVIRONMENT.md`, generated from its own registry.

Nothing here is required for normal use. These are diagnostic and tuning levers: reach for one when
you are measuring, bisecting, or working around a box, not as part of a build.

## Threading — the job queue

`DAS_JOBQUE_THREADS=N` means **N total compute lanes**, not N spawned workers: the queue creates
`N-1` workers and the calling thread is the Nth lane. It overrides `set_jobque_threads_cap()` and
the stock default (physical cores minus one). Set it before the queue is created; changing it later
has no effect.

| Variable | Type | Effect |
|---|---|---|
| `DAS_JOBQUE_THREADS` | number | Total compute lanes. Overrides the cap and the stock default. |

Not an env var despite the name: `DAS_MAX_HW_JOBS` is a **build-time** `-D` define, **wasm-only**
since 2026-07-02 — desktop gets `cores-1`. A pre-fix binary caps at 4 workers, so every threaded
number it produced is a 4-thread number.
| `DAS_JOBQUE_AFFINITY` | number | Worker affinity: `0` off, `1` ideal-processor hint, `2` hard mask. On a big SMT box the placement lottery can land two compute lanes on one physical core's SMT pair; `2` prevents it. |
| `DAS_JOBQUE_LIMIT_ORDER` | flag | Constrain job ordering — a determinism aid when chasing a race, not a speed knob. |
| `DAS_JOBQUE_TEAM_EAGER_EXIT` | flag | Team workers leave as soon as their share is done rather than waiting at the barrier. |
| `DAS_JOBQUE_TEAM_RANK_GATE` | number | Team-dispatch rank gate. When set, it takes precedence over a box profile's own `team_rank_gate`. |
| `DAS_TEAM_PROF` | flag | Per-team dispatch profiling counters. |

`set_jobque_affinity()` and `set_jobque_threads_cap()` are the programmatic equivalents; the
environment wins over both, which is what makes them usable for an A/B without touching source.

## JIT

| Variable | Type | Effect |
|---|---|---|
| `DAS_JIT_X64_FORCE_FEATURES` | text | Comma-separated x64 CPU features to force on (e.g. `avx2,f16c`), bypassing detection. |
| `DAS_JIT_ARM64_FORCE_FEATURES` | text | The arm64 twin (e.g. `dotprod,i8mm`). |

Both exist to reproduce another box's codegen locally, or to check that a feature-gated kernel is
actually the one being measured. Forcing a feature the CPU lacks produces an illegal instruction at
run time, not a diagnostic.

They also override `cpu_supports`, which is what `requires=` on a `[tune_perm]` and the
`suffix:requires` entries of a `[tuned]` fallback chain are matched against — so forcing a feature
is how you check that a per-ISA default resolves the way you expect without owning that silicon.
Names are the LLVM target-feature spellings on both architectures (`avx2`, `amx-int8`; `dotprod`,
`i8mm`, `fullfp16`).

## Kernel tuning

`DAS_TUNE_MODE`, `DAS_TUNE_MANIFEST` and `DAS_TUNE_POLICY` drive the `[tune]` framework. They are
documented where the framework is — see `skills/tune.md`. The one worth repeating here:
`DAS_TUNE_MANIFEST` points at the sidecar to read and write, which is how you tune when the
application directory is read-only.

| Variable | Type | Effect |
|---|---|---|
| `DAS_TUNE_VERBOSITY` | text | What a tune shows: `silent`, `normal` (default), `verbose`. Anything unrecognized reads as `normal`, so a typo never silences a tune. |

`--tune-quiet` / `--tune-verbose` on the application set it, and it inherits down the whole tuner
process chain — the measuring code is two `popen`s below the process the user typed the flag at.
It gates only what a human sees: progress *events* are forwarded to a capturing parent regardless,
so `silent` under a supervisor still yields a full event stream in the log.

## Lint

| Variable | Type | Effect |
|---|---|---|
| `DAS_LINT_CONFIG_PATH` | path | Read this file instead of `<das root>/.lint_config`. |
| `DAS_LINT_DISABLE` | text | Comma-separated rule codes to turn off for one run (`DAS_LINT_DISABLE=PERF026,LINT017`). Applied last, so it overrides the config file. |

`DAS_LINT_DISABLE` is the "let me add a log line while I debug this" hatch: no source edit, no
config change, and it reverts the moment the variable goes away. Surrounding whitespace is ignored
and unknown codes are harmless. The `-no-lint` command-line flag skips the lint pass entirely.

## Diagnostics

| Variable | Type | Effect |
|---|---|---|
| `DAS_GC_STAGE_REPORT` | flag | Report gc_node deltas per compilation stage — the first thing to reach for on a `GC APP LEAK` at exit. |
| `DAS_GC_BREAK_ON_ID` | number | Break when the gc_node with this id is allocated. Pair it with the id from a leak report. |
| `DAS_TRACE_MODULE_LOAD` | flag | Log every module as it loads, with its resolved path — the fastest way to see which of two same-named modules actually won. |

## Ambient variables daslang reads but does not own

`NO_COLOR` and `TERM` gate ANSI output (`daslib/ansi_colors`). `TMPDIR`, `HOME`,
`XDG_CONFIG_HOME`, `APPDATA` and `LOCALAPPDATA` are used for scratch and config paths in the usual
platform way. On Windows, `OS`, `PROCESSOR_ARCHITECTURE` and `NUMBER_OF_PROCESSORS` are read for
platform detection. Setting any of these changes daslang's behaviour only as far as the platform
convention implies.

## Adding one

A new environment variable is a support burden: it is invisible, it has no type, and it survives in
someone's shell long after they have forgotten setting it. Before adding one, check whether a
programmatic setter would do — most of daslang's knobs have both, and the API form is testable.

If a variable is genuinely warranted:

1. **Read it once**, at init or behind a one-time gate. `get_env_variable` calls `getenv` *and*
   allocates the result in the context heap, so a per-step read is both a lookup and a leak into the
   heap until the next reset. This is what `PERF027` exists to catch — see `skills/perf_lint.md`.
2. **Give it a default that means "unset"**, and make garbage fall back to that default rather than
   to zero. Plain `to_int` answers `0` on garbage, which silently reads as "explicitly zero".
3. **Document it in the same commit.** A knob that exists only in the source is a knob nobody can
   use and nobody can remove.
