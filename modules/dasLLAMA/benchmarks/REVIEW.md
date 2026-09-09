# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_MEASUREMENT.md`, `../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md`. Planned
work: `../followup_metal.md` for anything about the Metal backend, `../followup_vulkan.md` for
anything about the Vulkan backend, `../PERF_LEDGER.md` for any other performance followup,
`../followup_general.md` for everything else.

**A GPU kernel timing arm - code that dispatches a kernel to measure it rather than to serve a
call - wherever the diff puts it, applies `../REVIEW_GPU_RACE.md` too.**

**A diff that adds or changes an instrument that dispatches a `[tune]` kernel - one whose body
the engine's tune selection picks, not one the instrument compiled itself and not a reference
tool's own runtime - calls `tune_gate()` (`../performance/profile_common.das`) before that
instrument's first timed rep, or - where the instrument cannot require this module's
performance tree - stamps its rows with the tune manifest (`DAS_TUNE_MANIFEST`) or the class
profile (`../performance/defaults/<class>.tune-defaults.json`) the run compiled
against.** An instrument is a file that times a run and reports a wall-clock time or rate as
its result, printed or returned to a caller that prints it. Without the gate or the stamp the
instrument measures fallback kernels silently.

**A diff that adds or changes a race alternates its arms within one process - one timed round
per arm, best-of across rounds.** A race times two implementations of the same computation in
one process and compares them; an arm is one implementation's timed run. An instrument is
reviewed arm by arm.

**A diff that adds or changes a race arm that computes a comparable output proves that output
on its report line:** an arm whose result is bit-identical to the baseline's prints the
bit-exact compare over the sampled region - the set of output elements the run compares - on
the report's "bit-exact vs ..." line; every other arm prints a bounded-difference compare
(against the baseline arm or the CPU reference) plus the bound it passed. How the arm orders
its sums, and whether its multiply-adds fuse, decide bit-identity - not the declared precision.

**A diff that adds or changes a race also checks the race's baseline arm against a CPU
reference.** The baseline arm is the arm running the implementation already in use. The
reference check runs in the same process, on the same output elements the arms are judged on.
Two arms can agree and both be wrong; only the reference makes the winner right.

**An arm that compares no output carries the literal token `timing-only` on its report line.**

**A mode of an instrument that runs no race arm carries the literal text `ATTRIBUTION SWEEP` on
its own line of the file's header comment, naming the mode and what its arms attribute.** A
mode is one selectable run of the file, chosen by its own flag or argument. Without the line a
reader takes a sweep's arms for an adoption decision it never made.

**A new instrument that puts its own clock around a served turn is a defect: add a board cell
instead.** A served turn is a whole prefill-plus-decode run; a board cell is a timed cell of
the published results board - one `../performance/gen_bench_records.das` spawns, or a manual
`lcpp_bench.das` cell with its own `../PROFILE.md` section. A second instrument's numbers
cannot be compared to any row the board already carries.

**An out-of-process observer - a script watching a benchmark process from outside - never
measures what that process can measure about itself; that measurement goes inside the process
instead.**

**A file holding a third-party wall - a wall-clock time measured for a binary this repository
does not build - outside `../performance/records/` and `../PERF_LEDGER.md` is scratch:
untracked, owned by exactly one instrument, re-derivable from a command in that instrument's
header comment, and never an input to a board cell.** A tracked or shared copy of a third-party
wall becomes a stale baseline nobody re-derives.

**A diff that adds or changes an instrument that prints the difference of two walls also
prints both of those walls on that report line.** A plain elapsed-time row - one clock pair,
no attribution across stages - is not a difference.

**A diff that changes what a board cell times ships before/after rows for each affected cell
and corpus - or withdraws the affected rows and names the withdrawal and its reason in the PR
body.** What a cell times changes when a change inside its timed body, to its input corpus, or
to the pinned reference build (`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`, or anything else
deciding which reference binary or environment the run measures) moves the measured quantity; a
change outside the timed body - a flag, a require, the submit path - does not. The new rows or
the withdrawal land in `../performance/records/<box>.json`, the file the affected rows live in.

**A diff that adds an instrument, or changes how one reports or exits, makes every mode whose
purpose is to report result rows exit non-zero on a run that reports none - wrong flags, failed
load, a device that declines.** A result row is a row carrying a time or a rate. A run that
matched nothing and reported success leaves a sidecar or a record untouched, and its caller
cannot tell.

**A diff that adds an A/B arm, or changes how an arm reports or exits, makes that instrument
exit non-zero when the lever does not change what the run executes - or, when the instrument
runs that check before the arm, print a warning naming the inert lever.** An A/B arm is one of
an instrument's timed runs, distinguished by a named lever - the flag or environment switch
that names the arm - set to a value the paired run does not use, off/on or graded. A lever that
silently no-ops prints a 1.00x row nobody can tell from a real tie.

**A diff that adds or changes an A/B arm of an instrument over a prompt corpus makes that arm
report one row per prompt, never one aggregate ratio alone.** Prompts differ in how much the
lever helps, so a per-prompt loss hides inside a winning mean.
