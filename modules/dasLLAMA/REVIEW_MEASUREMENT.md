# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `followup_metal.md` for the Metal tier,
`followup_vulkan.md` for the Vulkan tier, `followup_general.md` for everything else.

A figure is any number a run produced - a rate, a wall, a count, a byte size. A figure rule below
binds a figure this change writes or changes - in the repo, in a commit message, or in its PR
body. It does not bind a figure a changed line carries unchanged. A figure on a page a `site*/`
or `utils/` folder checklist owns (repo root) answers to that checklist instead of this one. The
naming a figure rule asks for sits in the figure's own sentence, in a table heading that covers
the table's rows, in a section-level provenance line that covers the paragraphs under it, or in
a citation of the passage whose provenance line covers it. An arm is one side of a pair held
against the other. Flags are the tier (`-jit` or not), the `DAS_TUNE_POLICY` value in force, and
the kernel backend the run served on; for a run served on a GPU backend, the device and the
coopmat mode in force (`DASLLAMA_COOPMAT` - which cooperative-matrix path the run compiled). An
environment override is an env variable a run was given that changes what it compiles, tunes, or
serves, other than the ones named above.

**A `PERF_LEDGER.md` entry states a served-turn figure of the engine this repository builds - a
tok/s rate or a turn wall - only when the released `lcpp_bench` exe (`benchmarks/lcpp_bench.das`
built by `daspkg release`) or a board cell produced it.** A served turn is one whole request an
engine serves, whatever the modality; a turn wall is its wall.

**A `-jit` A/B pair's arms keep their absolute rates in the run's report.** The `-jit` script is `benchmarks/lcpp_bench.das` run as a script under
`-jit` rather than as the released exe.

**A `PERF_LEDGER.md` entry stating a figure no cell, script, or exe of this repository produced
tags it `external`; a figure one of them produced carries no such tag, whatever binary the run
timed - a ratio with either side produced by a cell, script or exe of this repository included.**

**A `PERF_LEDGER.md` ratio against a third-party engine names that engine's exe and the command
line it ran under.**

**A difference, ratio, or percentage of two measured figures - a wall or a rate - written into
`PERF_LEDGER.md` carries the raw figure of every side the `-jit` script did not produce; only a
pair with both sides from the `-jit` script enters as its ratio alone.**

**A `-jit` ratio written into `PERF_LEDGER.md` names the arm it is measured against.**

**A diff that adds a `PERF_LEDGER.md` entry whose reading no board cell produced names the
instrument that produced it - the script or exe whose output is that wall or rate.** A board
cell is a run `performance/gen_bench_records.das` spawns, or a manual
`benchmarks/lcpp_bench.das` cell its `PROFILE.md` section documents, whose reading lands as a
row of `performance/records/<box>.json`; an `--oracle` re-measure is not one - it never writes
the store. A ruler record (`performance/records/mtp/*.json`, written by
`harness/mtp_ruler.das`) is not a board cell.

**A ruler record's third-party row is written by the same `harness/mtp_ruler.das` run that
wrote the das row it pairs with.** A wall pasted in from another run measures a different
prompt, drafter and settle state, and the acceptance rate it implies is not the pair's.

**A `PERF_LEDGER.md` entry tags its reading `direction-grade` when the reading compares across
two processes or two commits, and `out-of-process` when the wall was measured from outside the
benchmark process.**

**A checked-in document that states a `--for-debug-purposes` reading covered by a committed board
row - one matching on all three of model, served-turn leg (prefill, decode, or the turn end to
end) and backend - cites that row and marks the reading `debug-jit`.** A `--for-debug-purposes`
row is the `-jit` script's own output.

**A figure tagged `external` in `PERF_LEDGER.md` names the source and the report it came from.**

**A diff never rests an adoption decision about what the engine serves on a figure from
another project - it rests on a measurement a cell or instrument of this repository took.**

**A diff that adds an entry to `PERF_LEDGER.md` never records a selection timing - a timing
whose losing arm was never committed.** That timing settles its adoption decision in the report
of the run that took it and in the PR that lands the kernel. The winner enters the ledger only
through a re-measured board cell.

**A diff that routes a whole prefill-plus-decode pass onto a code path no board cell exercises
mints that cell in the same change.** A route is the end-to-end code path such a pass takes,
including the path a run with no flags and no environment overrides takes.

**A change that owes a board cell for a route `performance/gen_bench_records.das` cannot mint a
row for - no rig leg (the backend arm a cell runs, the set `stored_row_leg` in
`performance/profile_common.das` admits: plain cpu, `accel`, `metal`) drives it, the author's box
refuses or skips the leg that would, or the model is one `--catalog official` does not carry,
whose rows a sweep can only write to a store outside `records/` - names instead, in the same
change, the record or gate output that proves the route ran end to end.**

**A diff that makes the fat exe - a shipped exe carrying its tune profile
(`ARCHITECTURE_MEASUREMENT.md` sec.2.42a) - run end to end names `tune_gate`'s fat-world report
(`performance/profile_common.das`) and the `sanity:` lines of its `lcpp_bench` run.**

**A diff that claims to make an already-served path faster, from an author whose box mints
that path, re-mints a board row (`performance/records/<box>.json`) that exercises that path, in
the same change, and names that row in the PR body.** A box mints a path when
`performance/gen_bench_records.das` mints a row for it on that box - a rig leg its `stored_row_leg`
(`performance/profile_common.das`) admits - rather than refusing or skipping it. The board is
the module's committed record of what serving costs; a kernel win that never lands there is
invisible to the next regression check.

**A rate or wall of any leg of a served turn - prefill, decode, or the turn end to end -
written down as a measurement rather than as a prediction, is a defect without either a board
cell behind it or a provenance line naming harness, flags, environment overrides, box, and the
exe or script that ran it.**

**A figure a run of this repository produced that is not a served-turn leg, whose value depends
on the box it ran on, names the harness, the flags, the environment overrides, the box and the
exe or script that produced it.** A figure a committed board cell or ruler record produced names
the record and row instead.

**A figure whose value is the same on every box names the build, fixture, or command that
reproduces it.**

**A diff that adds a race - a timed run-off between candidate implementations at startup whose
winner sets a knob - to a shipped exe's startup races on synthetic inputs only, never loading a
model, never spawning a child process.** A shipped exe carries no vehicle model - a real model
file a harness run drives - and no harness script, so a model or a child there is a hang or a
silent skip.

**A shipped exe's startup race races only a GPU pipeline-state twin, which sets a runtime knob -
never a `[tune]` kernel family** (`ARCHITECTURE_MEASUREMENT.md` sec.2.42a). A `[tune]` winner needs a recompiled clone the
shipped exe does not carry.

**A diff never adds a confirm - an end-to-end A/B served on a vehicle model in a spawned
child - outside `harness/`.**

**A diff that pins the kernel backend - `pin_kernel_backend`, `select_kernel_backend`, or
`DASLLAMA_PIN_BACKEND` - pins it before the load it governs.** The loader repacks weights into
the pinned backend's layout, so a pin set after the load never reaches them.

**A pin naming a backend the box has not registered refuses or skips the run rather than
falling through to the default.** A misspelled pin that falls through measures the default
backend under the pinned one's name.
