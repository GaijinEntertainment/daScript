# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_MEASUREMENT.md`, `../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`. Planned
work: `../followup_metal.md` for Metal, `../followup_vulkan.md` for Vulkan,
`../followup_general.md` otherwise.

An instrument is a file that times a run itself and reports a wall-clock time or rate as its
result, printed or returned to a caller that prints it; a file that reads a child process's clock
is not one, and a serving path's profiler-gated report (a run whose result is the served output,
the numbers a side report) is not one. A race times two candidates for one computation in one
process, either of which the run could adopt; an arm is one candidate's timed run; a compared arm
is one whose output the run reads back and measures against another arm's output or a CPU
reference; the baseline arm is the arm running the implementation already in use. A served turn
is one whole request the engine serves - a prefill-plus-decode run, or a transcription or
synthesis end to end; a board cell is a timed cell whose rows land in `../performance/records/`
or `../PERF_LEDGER.md`. A result row is a row carrying a time, a rate, or a per-kernel occupancy
count. An A/B arm is one of two timed runs an instrument makes in ONE process that differ only in
one flag or environment switch - the lever - set to a different value in each; off/on or graded.

**Code that dispatches a GPU kernel to measure it rather than to serve a call, wherever the diff
puts it, applies `../REVIEW_GPU_RACE.md` too.**

**A diff that adds or changes an instrument that dispatches a `[tune]` kernel - one whose body
the engine's tune selection picks, not one the instrument compiled itself and not a reference
tool's own runtime - calls `tune_gate()` (`../performance/profile_common.das`) before that
instrument's first timed rep, or - where the instrument cannot require this module's
performance tree - stamps its rows with the tune manifest (`DAS_TUNE_MANIFEST`) or the class
profile (`../performance/defaults/<class>.tune-defaults.json`) the run compiled
against.** Without the gate or the stamp the instrument measures fallback kernels silently.

**A diff that adds or changes a race alternates its arms - one timed round per arm, best-of
across rounds.**

**A diff that adds or changes a race reports each arm's row on its own - never two arms' numbers
on one row.**

**A diff that adds or changes a compared arm prints on its report line either the bit-exact
compare over the sampled region (the output elements the run compares), on a `bit-exact vs ...`
line, when the arm's result is bit-identical to the baseline's, or else a bounded-difference
compare against the baseline arm or the CPU reference plus the bound it passed.** How the arm
orders its sums, and whether its multiply-adds fuse, decide bit-identity - not the declared
precision.

**A diff that adds or changes a race with a compared arm also checks the race's baseline arm
against a CPU reference.** The reference check runs in the same process, on the same output
elements the arms are judged on. Two arms can agree and both be wrong; only the reference makes
the winner right.

**A diff that adds or changes a race arm that is not a compared arm makes that arm carry the
literal token `timing-only` on its report line.**

**A diff that adds or changes a mode that times its arms without reading their outputs back and
comparing them makes that mode carry the literal text `ATTRIBUTION SWEEP` on its own line of the
file's header comment, naming the mode and what its arms attribute.** A mode is one selectable
run of the file, chosen by its own flag or argument; a file with no mode flag is one mode. Without
the line a reader takes the mode's arms for an adoption decision it never made.

**A new instrument that puts its own clock around a served turn is a defect: add a board cell
instead.** A second instrument's numbers cannot be compared to any row the board already carries.

**An out-of-process observer - a script watching a benchmark process from outside - never
measures what that process can measure about itself; that measurement goes inside the process
instead.**

**A diff that adds or changes a data file an instrument reads or writes that holds a third-party
wall - a wall-clock time measured for a binary this repository does not build - outside
`../performance/records/` and `../PERF_LEDGER.md` keeps that file untracked, owned by exactly one
instrument, re-derivable from a command in that instrument's header comment, and never an input
to a board cell.** A tracked or shared copy of a third-party wall becomes a stale baseline nobody
re-derives.

**A diff that adds or changes an instrument that prints the difference of two wall-clock times
also prints both of those times on that report line.** A plain elapsed-time row - one clock pair,
no attribution across stages - is not a difference.

**A diff that changes what a board cell times ships before/after rows for each affected cell
and corpus - or withdraws the affected rows and names the withdrawal and its reason in the PR
body.** What a cell times changes when a change inside its timed body, to its input corpus, or
to the pinned reference build (`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`, or anything else
deciding which reference binary or environment the run measures) moves the measured quantity; a
change that alters nothing the timed body loads, runs, or counts does not. The new rows or
the withdrawal land in the file the affected rows live in - `../performance/records/<box>.json`
or `../PERF_LEDGER.md`.

**A diff that changes a GPU kernel emitter under this folder - a `[vk_dispatch]` or
`[metal_kernel]` body or a `*_msl` source global - and ships no before/after rows for a board
cell or instrument that times the changed kernel names, in the PR body, the compare showing the
emitted kernel code byte-identical before and after: the `*_msl` source text, the AIR it builds
into, or the SPIR-V words `DASLLAMA_VK_SPV_DUMP` writes** (the engine's own emitters answer to
`../REVIEW_GPU.md`).

**A diff that adds a result-row mode - to a new or an existing instrument - or changes how such
a mode reports or exits, makes every result-row mode of that instrument exit non-zero on a run
that reports no result row, whatever stopped it - wrong flags, a failed load, a device that
declines.** A run that matched nothing and reported success leaves a sidecar or a record
untouched, and its caller cannot tell.

**A diff that adds an A/B arm, adds or changes a lever an instrument's A/B arm reads - a lever in
a file under this folder or one `lcpp_bench.das` requires directly - or changes how such an arm
reports or exits, makes that instrument exit non-zero when the lever does not change what the run
executes - or, when the check runs before the arm, print a warning naming the inert lever.** A
lever that silently no-ops prints a 1.00x row nobody can tell from a real tie.

**A diff that adds or changes an A/B arm of an instrument over a prompt corpus makes that arm
report one row per prompt, never one aggregate ratio alone.** Prompts differ in how much the
lever helps, so a per-prompt loss hides inside a winning mean.

**A diff that adds or changes a row measured over reps reports one number over ALL of them. A rep
refuses when it produced no figure, or when it ran on a backend other than the row's backend
stamp - the backend name the row records as having served the row. One refusing rep drops the
whole row, and the row reports the refusal and its reason and no number.** A partial row reads
like a measured one and is a different quantity.
