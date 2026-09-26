# dasLLAMA Measurement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_MEASUREMENT.md`, `ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`,
`ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`. Planned work: `followup_metal.md` for the Metal
tier, `followup_vulkan.md` for the Vulkan tier, `followup_general.md` for everything else.

A figure rule below binds a figure this change writes or changes - in the repo outside `site*/`
(repo root), in a commit message, or in its PR body. It does not bind a figure a changed line carries unchanged. A reading
a `tests/` cell's tolerance is set from answers to `tests/REVIEW.md` here for the tolerance value
itself, and to this list for naming the box and flags it was read on.

The words these rules share:

- A figure is any number a run produced - a rate, a wall, a count, a byte size, or a ratio or
  percentage of two such numbers, produced by the runs its sides came from - or an error or
  agreement reading (a cosine, a max-abs error) a test's tolerance is set from, or that a change
  cites as evidence that a tolerance set for another cell, model or tier holds for the one it adds
  or alters.
- A served-turn leg is prefill, decode, a batched decode row, or a whole served request end to
  end; the wall of one internal stage (one model component's forward pass, one decoder block) is
  not a leg.
- An instrument is a script that times a run itself and reports the wall or rate as its own
  result; a harness is the `harness/` script that drove a run, where one did.
- The flags of a serving run are the tier (`-jit` or not), the `DAS_TUNE_POLICY` value in force,
  the kernel backend it served on, and, on a GPU backend, the device, and on Vulkan the coopmat
  mode (`DASLLAMA_COOPMAT`). The flags of an instrument run are the tier, the `DAS_TUNE_POLICY`
  value, the device, the coopmat mode where the backend has one, and the instrument's arguments.
- An environment override is an env variable given to the run that changes what it compiles,
  tunes, or serves, other than the run's flags.
- A board cell is a run whose reading lands as a row of `performance/records/<box>.json`: a
  spawned cell (a run `performance/gen_bench_records.das` spawns) or a manual cell (the
  `benchmarks/lcpp_bench.das` command a `PROFILE.md` section documents, run by hand, its reading
  written as such a row). An `--oracle` re-measure never writes the store and is not one; nor is
  a reading a `harness/` instrument produces, whether or not it writes a record.
- The `-jit` script is `benchmarks/lcpp_bench.das` run as a script under `-jit`, not as the
  released exe; a `-jit` reading is a wall or rate, or a ratio of two, that an instrument - the
  `-jit` script included - printed while running under `-jit`.

**The naming a figure rule asks for attaches to a figure only by a sentence, a table heading or
a provenance line that unambiguously covers it, or by a citation of a passage or board row whose
provenance covers it.** A provenance line is a paragraph whose whole content is provenance - the
harness, flags, box and exe; it covers the rows it names or, when it names none, the paragraphs
after it up to the next heading or the next provenance line.

**A tag (`external`, `direction-grade`, `out-of-process`, `debug-jit`) covers only the figures of
the sentence or bullet it sits in, or, on a provenance line, the figures that line covers.**

**A `PERF_LEDGER.md` entry states a turn wall (the wall of one whole served request, whatever the
modality) or a tok/s rate of the engine this repository builds only when
`benchmarks/lcpp_bench.das` produced it - as the released exe (`daspkg release`) or as the `-jit`
script - or a board cell did, or, for a modality `benchmarks/lcpp_bench.das` has no command-line
flag for, the `harness/` instrument that produced it, named with its command line.** A
served-turn wall or rate printed by any other instrument stays in that run's log: the ledger entry
carries what the instrument decided - served, declined, how many rows it counted - never that wall
or rate.

**A pair of `-jit` readings held against each other keeps each arm's (each side's) absolute wall
or rate in the commit message or PR body that states the pair.**

**A `PERF_LEDGER.md` entry stating a figure no cell, script, or exe of this repository produced
tags it `external`; a figure one of them produced carries no such tag, whatever binary the run
timed.**

**A difference, ratio, or percentage of two measured figures - a wall or a rate - written into
`PERF_LEDGER.md` carries the raw figure of every side no cell, script, or exe of this repository
produced.**

**A ratio the `-jit` script produced, written into `PERF_LEDGER.md`, names the arm - the side of
the compared pair - it is measured against.**

**A `PERF_LEDGER.md` entry tags its reading `direction-grade` when the reading compares across
two processes or two commits.**

**A `PERF_LEDGER.md` entry tags its reading `out-of-process` when the wall was measured from
outside the benchmark process.**

**A diff that writes a `-jit` reading outside `PERF_LEDGER.md` tags it `debug-jit`, and cites the
committed board row of the same model, served-turn leg and backend where one exists.**

**A figure a diff writes down - in `PERF_LEDGER.md` or any other checked-in text - that no cell,
script, or exe of this repository produced names where it came from: a published figure names
its source and the report; a figure a third-party program
(an exe or a script) produced names that program, its version, the model, the command line, and
any environment settings the command line does not show.** A citation of an architecture section
that spells that run - no placeholder in its exe, model or command line - names it.

**A diff never rests an adoption decision about what the engine serves on a figure from
another project - it rests on a measurement a cell or instrument of this repository took.**

**A dated `PERF_LEDGER.md` entry - one whose bold head, or the heading it sits under, carries a
date - never has its figures or conclusion edited; a new dated entry refutes it.** Repointing a
citation in it is not such an edit.

**A `PERF_LEDGER.md` entry never carries a timing that argues for code the same diff does not
land - that timing goes in the PR that lands the code.**

**A diff that routes a served turn, or a media encode a turn takes (an image, a video frame or an
audio chunk), onto a code path no board cell exercises mints that cell in the same change.** A
route is the end-to-end code path such a turn takes, including the default route - the one a run
given no command-line arguments and no environment overrides takes.

**A change that owes a board row no box of the author's can mint names instead, in the same
change, an artifact that shows the route ran end to end - a record, a gate output, an
instrument's print - or the profile output that shows the change is faster
(`benchmarks/lcpp_bench.das`'s `forward_profile` rows, or the tier's `DASLLAMA_GPU_PROF=1` token
ledger), with the flags that run took.**

**A box cannot mint a spawned cell when no rig leg - a child cell
`performance/gen_bench_records.das` spawns whose record row has `engine` `das`, named by that
row's `(backend, flavor)` pair whatever its `workload` - drives the path the owed row measures,
when the box refuses or skips it, or when `performance/gen_bench_records.das --catalog official`
does not carry the model; it can mint a manual cell when it runs the documented command's OS and
backend and holds its model.**

**A diff that changes `dasllama/dasllama_metal_kernels.das` (the kernel library
`metal_decode_init` compiles), `performance/profile_common.das`, or a `dasllama/` module
`benchmarks/lcpp_bench.das` requires directly copies into the PR body the `sanity:` lines and the
`tune gate:` line of an `lcpp_bench` run of the fat exe built from the diff's tree, or says that
run printed none.** A fat exe is what `daspkg release --fat <class>` builds (`DAS_TUNE_MODE=fat`,
`ARCHITECTURE_MEASUREMENT.md#fat-first-start`).

**A diff that claims to make an already-served path faster, where a rig leg (a spawned
`engine` `das` child cell) drives that path, re-mints a board row
(`performance/records/<box>.json`) that exercises it, in the same change, and names that row in
the PR body.** The board is the module's committed record of what serving costs; a kernel win
that never lands there is invisible to the next regression check.

**A rate or wall of any served-turn leg written down as a measurement rather than as a
prediction is a defect unless it cites the committed board row it came from, or names harness,
flags, environment overrides, box, the exe or script that ran it, and - for a figure aggregated
over more than one timed run or input - the number of runs or inputs and the spread of the
per-run figures: the standard deviation, or the min and max.**

**A diff that records a measured number a `harness/` instrument prints - a time, a rate, or a
figure computed from one - or changes what such a number measures, ships that number's
alternate in the same change: a row from the same instrument on the same fixture and input size,
with exactly one flag or environment override changed, or a third-party row - a figure from a
run of a third-party program serving the same model on the same input size - that cites the
architecture section holding its recipe.** A number with no alternate beside it cannot be
compared to anything.

**A diff that records a third-party row lands that row's recipe - the program, its version, the
model, the command line, and any environment settings the command line does not show - in the
same change, in the `ARCHITECTURE_MEASUREMENT*.md` section that describes the harness instrument
the row is compared with; where no section describes that instrument, the diff adds one.** The
next entry re-runs the reference from the section, not from the earlier entry's prose.

**A figure a run of this repository produced that is not a served-turn leg, whose value depends
on the box it ran on, names the harness, the flags, the environment overrides, the box and the
exe or script that produced it - or names the committed record file and the row or key it sits
under, when that record is a board record or its own fields name that harness, flags, box and
exe.**

**A figure whose value is the same on every box names the build, fixture, or command that
reproduces it.**

**A race a diff adds to a shipped exe's startup - a timed run-off between candidate
implementations whose winner sets a knob - runs on synthetic inputs only, never loading a model,
never spawning a child process.** A shipped exe carries no vehicle model (a real model file a
harness run drives) and no harness script, so a model or a child there is a hang or a silent
skip.

**A shipped exe's startup race races only a GPU pipeline-state twin - two pipeline-state objects
built from the same shader, raced to set a runtime knob - never a `[tune]` kernel family**
(`ARCHITECTURE_MEASUREMENT.md#fat-first-start`). A `[tune]` winner needs a recompiled clone the
shipped exe does not carry.

**A diff never adds a confirm outside `harness/` - it goes there instead.** A confirm is an
end-to-end A/B: it serves a vehicle model in a spawned child, runs two implementations of this
engine the run could adopt, and picks the winner.

**A diff that pins the kernel backend - `pin_kernel_backend`, `select_kernel_backend`, or
`DASLLAMA_PIN_BACKEND` - pins it before the load it governs.** The loader repacks weights into
the pinned backend's layout, so a pin set after the load never reaches them.

**A diff that adds or changes a kernel-backend pin path makes the run refuse or skip when the
pin names a backend the box has not registered - never fall through to the default.** A
misspelled pin that falls through measures the default backend under the pinned one's name.
