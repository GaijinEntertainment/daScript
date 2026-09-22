# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `followup_metal.md` for the Metal tier,
`followup_vulkan.md` for the Vulkan tier, `followup_general.md` for everything else.

A figure is any number a run produced - a rate, a wall, a count, a byte size, or a ratio or
percentage of two such numbers, which counts as produced by the runs its sides came from - or an
error or agreement reading (a cosine, a max-abs error) a test's tolerance is set from. A figure
rule below binds a figure this change writes or changes - in the repo, in a commit message, or in
its PR body. It does not bind a figure a changed line carries unchanged. A figure on a page a
`site*/` folder checklist owns (repo root) answers to that checklist instead of this one; a
reading a `tests/` cell's tolerance is set from answers to `tests/REVIEW.md` here for the
tolerance value itself, and to this list for naming the box and flags it was read on.

The naming a figure rule asks for is attached to the figure by a sentence, a table heading or a
provenance line that unambiguously covers it, or by a citation of a passage or board row whose
provenance covers it. A provenance line is a paragraph whose whole content is provenance - the
harness, flags, box and exe. It covers the rows it names or, when it names none, the paragraphs
after it up to the next heading or the next provenance line. A tag (`external`,
`direction-grade`, `out-of-process`, `debug-jit`) covers the figures of the sentence or bullet it
sits in, or, on a provenance line, the figures that line covers.

A served-turn leg is prefill, decode, a batched decode row, or the turn end to end.

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
rather than as the released exe. A `-jit` reading is a wall or rate, or a ratio of two, that an
instrument - the `-jit` script included - printed while running under `-jit`. A rig leg is the backend arm a board cell runs - one the set
`stored_row_leg` in `performance/profile_common.das` admits. A vehicle model is a real model file
a harness run drives; a harness is the `harness/` script that drove a run, where one did.

**A served-turn leg's rate written in `PERF_LEDGER.md` names the flags of the run that produced
it.**

**A `PERF_LEDGER.md` entry states a turn wall or a tok/s rate of the engine this repository
builds only when `benchmarks/lcpp_bench.das` produced it - as the released exe (`daspkg release`)
or as the `-jit` script - or a board cell did.** A served turn is one whole request an engine
serves, whatever the modality; a turn wall is its wall. A served-turn rate an instrument under
`harness/` prints stays in that run's log: the ledger entry carries what the instrument decided -
served, declined, how many rows it counted - never that rate.

**A pair of `-jit` readings held against each other keeps each arm's absolute wall or rate in the
commit message or PR body that states the pair.**

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

**A diff that writes a `-jit` reading outside `PERF_LEDGER.md` tags it `debug-jit`, and cites the
committed board row of the same model, served-turn leg and backend where one exists.**

**A figure tagged `external` in `PERF_LEDGER.md` names where it came from: a published figure
names its source and the report; a figure a third-party exe produced names that exe, its build,
and the command line it ran under.**

**A diff never rests an adoption decision about what the engine serves on a figure from
another project - it rests on a measurement a cell or instrument of this repository took.**

**A dated `PERF_LEDGER.md` row is never edited to correct it - a new dated row refutes it.**

**A diff that adds an entry to `PERF_LEDGER.md` never records a selection timing - a timing
recorded to justify adopting code the change does not land.** That timing settles its adoption
decision in the PR that lands the kernel. A negative result whose winner is the committed path is
an entry, not a selection timing.

**A diff that routes a whole prefill-plus-decode pass onto a code path no board cell exercises
mints that cell in the same change.** A route is the end-to-end code path such a pass takes,
including the path a run with no flags and no environment overrides takes.

**A change that owes a board row no box of the author's can mint names instead, in the same
change, an artifact that shows the route ran end to end - a record, a gate output, an
instrument's print - or the profile output that shows the change is faster
(`benchmarks/lcpp_bench.das`'s `forward_profile` rows, or the tier's `DASLLAMA_GPU_PROF=1` token
ledger), with the flags that run took.** A box cannot mint the row when no rig leg drives the
leg, when the box refuses or skips it, or when `performance/gen_bench_records.das --catalog
official` does not carry the model.

**A diff that changes what the fat exe - a `DAS_TUNE_MODE=fat` exe
(`ARCHITECTURE_MEASUREMENT.md` sec.2.42a) - runs at startup or while serving names, in the PR
body, the `sanity:` lines of its `lcpp_bench` run, and the `tune gate:` line that run printed -
or says the run printed none.**

**A diff that claims to make an already-served path faster, where a rig leg drives that path,
re-mints a board row (`performance/records/<box>.json`) that exercises it, in the same change,
and names that row in the PR body.** The board is the module's committed record of what serving costs; a kernel
win that never lands there is invisible to the next regression check.

**A rate or wall of any served-turn leg written down as a measurement rather than as a
prediction is a defect unless it cites the committed board row it came from, or names harness,
flags, environment overrides, box, the exe or script that ran it, and - for a figure computed
over repeated timings - the repetition count and their spread: the standard deviation, or the
min and max.**

**A diff that records a measured number a `harness/` instrument prints - a time, a rate, or a
figure computed from one - or changes what such a number measures, ships that number's
alternate in the same change: another row of the same run at the same shape, or a
reference-build row - a figure from a run of the third-party engine this module measures against
- at the same shape, named with the command that produced it.** A number with no alternate
beside it ranks nothing.

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

**A diff never adds a confirm outside `harness/` - it goes there instead.** A confirm is an
end-to-end A/B: it serves a vehicle model in a spawned child, runs two implementations of this
engine the run could adopt, and picks the winner.

**A diff that pins the kernel backend - `pin_kernel_backend`, `select_kernel_backend`, or
`DASLLAMA_PIN_BACKEND` - pins it before the load it governs.** The loader repacks weights into
the pinned backend's layout, so a pin set after the load never reaches them.

**A diff that adds or changes a kernel-backend pin path makes the run refuse or skip when the
pin names a backend the box has not registered - never fall through to the default.** A
misspelled pin that falls through measures the default backend under the pinned one's name.
