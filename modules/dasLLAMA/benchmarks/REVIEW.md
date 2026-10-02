# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_MEASUREMENT.md`, `../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`. Planned
work: `../followup_metal.md` for Metal, `../followup_vulkan.md` for Vulkan,
`../followup_general.md` otherwise.

An instrument is a file that times a run itself and reports the wall or rate as its result,
printed or returned to a caller that prints it; a file reading a child process's clock, and a
serving path's profiler-gated report (a run whose result is the served output, the numbers a side
report), are not one.

A race times two implementations of one computation in one process, either of which the run
could adopt - two values of one lever are not two implementations.

An A/B is two or more timed runs an instrument makes in ONE process that differ only in one lever
- a flag, an environment switch, a runtime setter or a profile key - set to a different value in
each, off/on or graded.

An arm is every timed run of one implementation in a race or of one lever value in an A/B; an
A/B arm is an arm of an A/B; a compared arm is one whose output - what a run of the same
implementation or lever value wrote, timed or not - the run reads back and measures against
another arm's output or a CPU reference; the baseline arm is the arm running the implementation,
or the lever value, already in use.

A board cell is a run whose reading lands as a row of `../performance/records/<box>.json` or as a
figure in `../PERF_LEDGER.md`.

An instrument's timed body is the statements between its clock reads, and what they call.

**A diff that changes a board cell's input corpus or the pinned reference build
(`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`, or anything else deciding which reference binary or
environment the run measures) applies `../REVIEW_MEASUREMENT.md` too.**

**Code that dispatches a GPU kernel to measure it rather than to serve a call, wherever the diff
puts it, applies `../REVIEW_GPU_RACE.md` too.**

**A diff that adds or changes the timed body of an instrument dispatching a `[tune]` kernel - one
whose body the engine's tune selection picks, not one the instrument compiled itself or a
reference tool's own runtime - calls `tune_gate()` (`../performance/profile_common.das`) before
its first timed rep, or - where the instrument cannot require this module's
performance tree - stamps its rows with the tune manifest (`DAS_TUNE_MANIFEST`) or the class
profile (`../performance/defaults/<class>.tune-defaults.json`) the run compiled
against.** Without the gate or the stamp the instrument measures fallback kernels silently.

**A diff that adds or changes a race alternates its arms - one timed round per arm, best-of
across rounds.**

**A diff that adds or changes a race reports each arm's row on its own - never two arms' numbers
on one row.**

**A diff that adds a compared arm other than the baseline, or changes its run or report line,
prints on that line the bit-exact compare over the sampled region (the output elements the run
compares), on a `bit-exact vs ...` line, when the arm's result is bit-identical to the
baseline's, or a bounded-difference compare against the baseline arm or the CPU reference plus
the bound it passed.** How the arm orders its sums, and whether its multiply-adds fuse, decide
bit-identity - not the declared precision.

**A diff that adds a race or an A/B with a compared arm, or changes its arms' runs or report
lines, also checks its baseline arm against a CPU reference, and prints that compare on the
baseline's report line, bit-exact or bounded with the bound it passed.** The reference check runs
in the same process, on the same output elements the arms are judged on. Two arms can agree and
both be wrong; only the reference makes the winner right.

**A diff that adds a race arm that is not a compared arm, or changes its run or report line,
makes that arm carry the literal token `timing-only` on its report line.**

**A diff that adds a mode that times its arms without reading their outputs back and comparing
them, or changes such a mode's arms, makes that mode carry the literal text `ATTRIBUTION SWEEP`
on its own header-comment line, naming the mode and what its arms attribute.** A mode is one
selectable run of the file, chosen by its own flag or argument; a file with none is one mode.
Without the line a reader takes the mode's arms for an adoption decision it never made.

**A new instrument that puts its own clock around a served turn - one whole request the engine
serves, a prefill-plus-decode run, or a transcription or synthesis end to end - is a defect: add a
run `../performance/gen_bench_records.das` spawns, or a `lcpp_bench.das` command a `../PROFILE.md`
section documents, instead; where `lcpp_bench.das` has no flag for the modality, the instrument
lives under `../harness/`.** A second instrument's numbers cannot be compared to any row the board
already carries.

**An out-of-process observer - a script watching a benchmark process from outside - never
measures what that process can measure about itself; that measurement goes inside the process
instead.**

**A diff that adds or changes a data file an instrument reads or writes that holds a third-party
wall - a wall-clock time measured for a binary this repository does not build - outside
`../performance/records/` and `../PERF_LEDGER.md` keeps that file untracked, owned by exactly one
instrument, re-derivable from a command in that instrument's header comment, and never an input
to a board cell.** A tracked or shared copy of a third-party wall becomes a stale baseline nobody
re-derives.

**A diff that adds an instrument that prints the difference of two wall-clock times, or changes
what it times or that report line, also prints both of those times on that report line.** A
plain elapsed-time row - one clock pair, no attribution across stages - is not a difference.

**A diff that changes a GPU kernel emitter under this folder - a `[vk_dispatch]` or
`[metal_kernel]` body or a `*_msl` source global - either ships before/after rows for a board
cell or instrument that times the changed kernel, or names in the PR body the compare showing
the emitted kernel code byte-identical before and after: the `*_msl` source text, the AIR it
builds into, or the SPIR-V words `DASLLAMA_VK_SPV_DUMP` writes** (the engine's own emitters
answer to `../REVIEW_GPU.md`).

**A diff that adds a result-row mode - one reporting rows that carry a time, a rate, or a
per-kernel occupancy count - to an instrument, or changes how such a mode reports or exits, makes
every result-row mode of that instrument exit non-zero on a run that reports no such row, whatever
stopped it - wrong flags, a failed load, a device that
declines.** A run that matched nothing and reported success leaves a sidecar or a record
untouched, and its caller cannot tell.

**A diff that adds an A/B arm, adds or changes a lever an instrument's A/B arm reads - a lever in
a file under this folder or one `lcpp_bench.das` requires directly - or changes how such an arm
reports or exits, makes that instrument exit non-zero when the lever does not change what the run
executes - or, when the check runs before the arm, print a warning naming the inert lever.** A
lever that silently no-ops prints a 1.00x row nobody can tell from a real tie.

**A diff that adds an A/B arm of an instrument over a prompt corpus, or changes the lever such an
arm reads or how the arm reports, makes that arm report one row per prompt, never one aggregate
ratio alone.** Prompts differ in how much the lever helps, so a per-prompt loss hides inside a
winning mean.

**A diff that adds a row measured over reps, or changes what its reps run or how the row is
computed, computes every number the row reports over all the reps after the warmup reps the
instrument's header comment names.**

**A diff that adds a row measured over reps, or changes what its reps run (an input picking a
rep's backend, codec or session shape included) or how the row is computed or reported, prints
no number on the row when any rep refuses, only the refusal and its reason.** A rep refuses when
it produces no figure or runs on a backend other than the row's backend stamp, the backend name
the row records. A partial row reads like a measured one and is a different quantity.
