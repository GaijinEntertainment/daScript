# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_MEASUREMENT.md`, `../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`. Planned
work: `../followup_metal.md` for Metal, `../followup_vulkan.md` for Vulkan,
`../followup_general.md` otherwise.

An instrument is a file whose result is a time or rate it clocked itself (own clock or GPU
timestamps), printed or returned for printing; a serving path's profiler report beside its served
output is not one. Its timed body is the statements between its clock reads, and what they call.

A lever is a flag, an environment switch, a runtime setter or a profile key that picks what the
run executes - never an input's size or content. A form is one compiled variant of a kernel. A
GPU kernel emitter is code that generates a pipeline or a dispatch builder - a `[vk_dispatch]`
class, a `[metal_kernel]` body, a `[metal_dispatch]` builder, or a `*_msl` source global. A race
times two implementations of one computation in one process, each one either a form the engine
serves or a candidate a file under this folder defines - a function, or a kernel a GPU kernel
emitter generates (two values of one lever are not two implementations); an A/B is timed runs in
one process differing only in one lever's value. An arm is every timed run of one implementation
or one lever value; a round is one timed run of one arm, and a warmup round is a round the
instrument's header comment says the row drops; a compared arm is one whose output, from any run
of it, the run reads back and measures against another arm's output or a CPU reference over the
compare region, the output elements the instrument's header comment names; the baseline arm is
the one the header comment names as baseline. A run of an arm refuses when it produces no figure or
output, or runs on a backend other than the backend name the row records. A board cell is a run
whose reading lands as a row of `../performance/records/<box>.json` or a figure in
`../PERF_LEDGER.md`. A mode is one selectable run of the file, chosen by its own flag or argument;
a file with none is one mode.

**A diff that changes a board cell's input corpus or the pinned reference build
(`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`, or anything else deciding which reference binary or
environment the run measures) applies `../REVIEW_MEASUREMENT.md` too.**

**Code that dispatches a GPU kernel to measure it rather than to serve a call, wherever the diff
puts it, applies `../REVIEW_GPU_RACE.md` too.**

**A diff that adds or changes the timed body of an instrument that requires
`../performance/profile_common.das` and dispatches a kernel of a `[tune]` family or a `[tuned]`
kernel - one whose form the tune sidecar (the `.tune.json` beside the file or the box's) or
`DAS_TUNE_MANIFEST` picks - calls `tune_gate()` before its first round.** Without the gate the
instrument measures fallback kernels silently.

**A diff that adds or changes the timed body of an instrument that dispatches a kernel of a
`[tune]` family or a `[tuned]` kernel and does not require `../performance/profile_common.das`
stamps every row with the tune manifest `DAS_TUNE_MANIFEST` names or the class profile
(`../performance/defaults/<class>.tune-defaults.json`) the run compiled against.**

**A diff that adds a compared arm to an instrument, or changes a compared arm's run or report line,
makes the instrument's header comment name the baseline arm and the compare region.**

**A diff that adds or changes a race times its arms in turn - one round of each arm, then the
next round of each - never all of one arm's rounds before the next arm's.**

**A diff that adds or changes a race reports each arm's best round, warmup rounds dropped, as
that arm's row.**

**A diff that adds or changes a race reports each arm's row on its own - never two arms' numbers
on one row.**

**A diff that adds a compared arm other than the baseline, or changes its run or report line,
prints one compare on that line: `bit-exact vs <baseline>` when the arm's result over the compare
region is bit-identical to the baseline's, otherwise a bounded-difference compare against the
baseline arm or the CPU reference with the bound it passed.** How the arm orders its sums, and
whether its multiply-adds fuse, decide bit-identity - not the declared precision.

**A diff that adds a compared arm, or changes its run or report line, makes its row print no time
when the arm misses its bound - only the miss and the bound.**

**A diff that adds a race or an A/B with a compared arm, or changes its arms' runs or report
lines, checks its baseline arm against a CPU reference in the same process, over the compare
region, and prints that compare on the baseline's report line, bit-exact or bounded with the bound
it passed.** Two arms can agree and both be wrong; only the reference makes the winner right.

**A diff that adds a race arm that is not a compared arm, or changes its run or report line,
makes that arm carry the literal token `timing-only` on its report line.**

**A diff that adds a race or A/B mode with no compared arm - its arms run in one process, or
each in a process the mode spawns - or changes such a mode's arms, makes that mode carry the
literal text `ATTRIBUTION SWEEP` on its own header-comment line, naming the mode and what its
arms attribute.** Without the line a reader takes the mode's arms for an adoption decision it
never made.

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

**A diff that adds a GPU kernel emitter under this folder ships rows in `../PERF_LEDGER.md` for a
board cell or instrument that times the kernel.**

**A diff that changes a GPU kernel emitter under this folder either ships before/after rows in
`../PERF_LEDGER.md` for a board cell or instrument that times the kernel, or names in the PR body
the compare showing the emitted kernel code byte-identical before and after: the `*_msl` source
text, the AIR it builds into, or the SPIR-V words `DASLLAMA_VK_SPV_DUMP` writes.**

**A diff that adds a result-row mode - one reporting rows that carry a time, a rate, or a
per-kernel occupancy count - to an instrument, or changes how such a mode reports or exits, makes
every result-row mode of that instrument exit non-zero on a run that reports no such row, whatever
stopped it - wrong flags, a failed load, a device that declines.** A run that matched nothing and
reported success leaves a sidecar or a record untouched, and its caller cannot tell.

**A diff that adds an A/B arm, adds or changes a lever an instrument's A/B arm reads - a lever in
a file under this folder - or changes how such an arm reports or exits, makes that instrument exit
non-zero when the lever does not change what the run executes - or, when the check runs before the
arm, print a warning naming the inert lever.** A lever that silently no-ops prints a 1.00x row
nobody can tell from a real tie.

**A diff that adds an A/B arm of an instrument over a prompt corpus, or changes the lever such an
arm reads or how the arm reports, makes that arm report one row per prompt, never one aggregate
ratio alone.** Prompts differ in how much the lever helps, so a per-prompt loss hides inside a
winning mean.

**A diff that adds a row measured over rounds, or changes the instrument's timed body, an input
picking a round's backend, codec or session shape, or how the row is computed, states in the
instrument's header comment how many warmup rounds a row drops, zero included, and computes
every number the row reports over the rounds after them.**

**A diff that adds a row measured over rounds, or changes the instrument's timed body, an input
picking a round's backend, codec or session shape, or how the row is computed or reported, prints
no number on the row when any run of the row's arm refuses - a round, a warmup round, or the run
whose output the compare reads - only the refusal and its reason.** A partial row reads like a
measured one and is a different quantity.
