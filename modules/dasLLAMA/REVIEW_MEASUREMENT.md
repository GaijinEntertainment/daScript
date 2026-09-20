# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `followup_metal.md` for the Metal tier,
`followup_vulkan.md` for the Vulkan tier, `followup_general.md` for everything else.

A figure is any number a run produced - a rate, a wall, a count, a byte size, or a ratio or
percentage of two such numbers, which counts as produced by the runs its sides came from - or an
error or agreement reading (a cosine, a max-abs error) a test's tolerance is set from. A figure
rule below binds a figure this change writes or changes - in the repo, in a commit message, or in
its PR body. It does not bind a figure a changed line carries unchanged. A figure on a page a
`site*/` or `utils/` folder checklist owns (repo root) answers to that checklist instead of this
one; a reading a `tests/` cell's tolerance is set from answers to `tests/REVIEW.md` here for the
tolerance value itself, and to this list for naming the box and flags it was read on.

The naming a figure rule asks for is attached to the figure by a sentence, a table heading or a
provenance line that unambiguously covers it, or by a citation of a passage or board row whose
provenance covers it. A tag (`external`, `direction-grade`, `out-of-process`, `debug-jit`)
covers the figures of the sentence or bullet it sits in, or, on a provenance line, the figures
of the paragraphs that line sits under or names.

A served-turn leg is prefill, decode, a batched decode row, or the turn end to end. A box mints a
path when `performance/gen_bench_records.das` mints a board row (`performance/records/<box>.json`)
for it on that box - a rig leg drives it - rather than refusing or skipping it.

An arm is one side of a pair held against the other. An instrument is a script that times a run
itself and reports the wall or rate as its own result. The flags of a serving run are the tier
(`-jit` or not), the `DAS_TUNE_POLICY` value in force, and the kernel backend the run served on;
for a run served on a GPU backend, the device, and on a Vulkan backend the coopmat mode in force
(`DASLLAMA_COOPMAT` - which cooperative-matrix path the run compiled). The flags of an instrument
run are the tier, the `DAS_TUNE_POLICY` value in force, the device, the coopmat mode where the
backend has one, and the instrument's own arguments.
An environment override is an env variable a run was given that changes what it compiles, tunes,
or serves, other than that run's own flags named above.

A board cell is a run `performance/gen_bench_records.das` spawns, or a manual
`benchmarks/lcpp_bench.das` cell its `PROFILE.md` section documents, whose reading lands as a row
of `performance/records/<box>.json`; an `--oracle` re-measure is not one - it never writes the
store. A reading an instrument under `harness/` produces is not a board cell, whether or not it
writes a record. The `-jit` script is `benchmarks/lcpp_bench.das` run as a script under `-jit`
rather than as the released exe. A rig leg is the backend arm a board cell runs - one the set
`stored_row_leg` in `performance/profile_common.das` admits. A vehicle model is a real model file
a harness run drives; a harness is the `harness/` script that drove a run, where one did.

**A `PERF_LEDGER.md` entry states a turn wall or a tok/s rate of the engine this repository
builds only when `benchmarks/lcpp_bench.das` produced it - as the released exe (`daspkg release`)
or as the `-jit` script, the two serving one code path and reading alike - or a board cell did.**
A served turn is one whole request an engine serves, whatever the modality; a turn wall is its
wall.

**A `-jit` A/B pair's arms keep their absolute rates in the commit message or PR body that states
the pair.**

**A `PERF_LEDGER.md` entry stating a figure no cell, script, or exe of this repository produced
tags it `external`; a figure one of them produced carries no such tag, whatever binary the run
timed.**

**A difference, ratio, or percentage of two measured figures - a wall or a rate - written into
`PERF_LEDGER.md` carries the raw figure of every side no cell, script, or exe of this repository
produced.**

**A ratio the `-jit` script produced, written into `PERF_LEDGER.md`, names the arm it is
measured against.**

**A `PERF_LEDGER.md` entry tags its reading `direction-grade` when the reading compares across
two processes or two commits, and `out-of-process` when the wall was measured from outside the
benchmark process.**

**A checked-in document other than `PERF_LEDGER.md` that states a `--for-debug-purposes` reading
covered by a committed board row - one matching on all three of model, served-turn leg and
backend - cites that row and marks the reading `debug-jit`.**
A `--for-debug-purposes` reading is the `-jit` script's own output.

**A figure tagged `external` in `PERF_LEDGER.md` names where it came from: a published figure
names its source and the report; a figure a third-party exe produced names that exe, its build,
and the command line it ran under.**

**A diff never rests an adoption decision about what the engine serves on a figure from
another project - it rests on a measurement a cell or instrument of this repository took.**

**A diff that adds an entry to `PERF_LEDGER.md` never records a selection timing - a timing
recorded to justify adopting code the change does not land.** That timing settles its adoption
decision in the PR that lands the kernel. A negative result whose winner is the committed path is
an entry, not a selection timing: it records what was tried and why the tree stays as it is.

**A diff that routes a whole prefill-plus-decode pass onto a code path no board cell exercises
mints that cell in the same change.** A route is the end-to-end code path such a pass takes,
including the path a run with no flags and no environment overrides takes.

**A change that owes a board row for a route or a path no box of the author's mints - no rig leg
drives it, the box refuses or skips the leg that would, or the model is one `--catalog official`
does not carry, so its rows can only go to a store outside `records/` - names instead, in the
same change, the record or gate output that proves the route ran end to end, or the profile
output that shows the win (`benchmarks/lcpp_bench.das`'s `forward_profile` rows, or the tier's
`DASLLAMA_GPU_PROF=1` token ledger) with the flags that run took.**

**A diff that makes the fat exe - a shipped exe carrying its tune profile
(`ARCHITECTURE_MEASUREMENT.md` sec.2.42a) - run end to end names `tune_gate`'s fat-world report
(`performance/profile_common.das`) and the `sanity:` lines of its `lcpp_bench` run.**

**A diff that claims to make an already-served path faster re-mints a board row
(`performance/records/<box>.json`) that exercises that path, in the same change, and names that
row in the PR body.** The board is the module's committed record of what serving costs; a kernel
win that never lands there is invisible to the next regression check.

**A rate or wall of any served-turn leg written down outside `PERF_LEDGER.md` as a measurement rather than as a
prediction, is a defect without either a board cell behind it or a naming of harness, flags,
environment overrides, box, and the exe or script that ran it.**

**A figure a run of this repository produced that is not a served-turn leg, whose value depends
on the box it ran on, names the harness, the flags, the environment overrides, the box and the
exe or script that produced it.** A figure a committed board cell produced, or a committed record
an instrument under `harness/` wrote, names the record and row instead.

**A figure whose value is the same on every box names the build, fixture, or command that
reproduces it.**

**A race a diff adds to a shipped exe's startup - a timed run-off between candidate
implementations whose winner sets a knob - runs on synthetic inputs only, never loading a model,
never spawning a child process.** A shipped exe carries no vehicle model and no harness script, so
a model or a child there is a hang or a silent skip.

**A shipped exe's startup race races only a GPU pipeline-state twin - two pipeline-state objects
built from the same shader, raced to set a runtime knob - never a `[tune]` kernel family**
(`ARCHITECTURE_MEASUREMENT.md` sec.2.42a). A `[tune]` winner needs a recompiled clone the shipped
exe does not carry.

**A diff never adds a confirm - an end-to-end A/B served on a vehicle model in a spawned
child, both arms implementations of this engine the run could adopt and the run picking the
winner - outside `harness/`.**

**A diff that pins the kernel backend - `pin_kernel_backend`, `select_kernel_backend`, or
`DASLLAMA_PIN_BACKEND` - pins it before the load it governs.** The loader repacks weights into
the pinned backend's layout, so a pin set after the load never reaches them.

**A diff that adds or changes a pin path makes a pin naming a backend the box has not registered
refuse or skip the run - never fall through to the default.** A misspelled pin that falls through
measures the default backend under the pinned one's name.
