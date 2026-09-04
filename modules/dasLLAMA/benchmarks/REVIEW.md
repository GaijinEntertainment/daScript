# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md`. Planned work: `../followup_metal.md` for anything about
the Metal backend, `../followup_vulkan.md` for anything about the Vulkan backend,
`../PERF_LEDGER.md` for any other performance followup, `../followup_general.md` for
everything else.

**A GPU kernel timing arm - code that dispatches a kernel to measure it rather than to serve a
call - wherever the diff puts it, applies `../REVIEW_GPU_RACE.md` too.**

**A diff that adds or changes an instrument whose timed body runs a forward pass through a
pipeline the dasLLAMA engine selected also calls `tune_gate()`
(`../performance/profile_common.das`) before that instrument's first timed rep.** A pipeline
the instrument compiled itself - or a reference tool's own runtime - is not one the engine
selected. Without the gate the instrument measures fallback kernels silently. An instrument is a
script whose output is a measured wall or rate.

**A diff that adds or changes a race alternates its arms within one process - one timed round
per arm, best-of across rounds.** A race is an instrument that compares two implementations of
the same computation. Two separate runs measure how the machine changed between them as much as
they measure the arms.

**A diff that adds or changes a race arm proves the arm's output on its report line, by what the
arm computes:** an arm producing no comparable
output carries the literal token `timing-only`; an arm whose result is bit-identical to the
baseline's prints the bit-exact compare over the sampled region - the set of output elements
the run compares - on the report's "bit-exact vs ..." line; every other arm prints a
bounded-difference compare (against the baseline arm or the CPU reference) plus the bound it
passed. How the arm orders its sums, and whether its multiply-adds fuse, decide bit-identity -
not the declared precision.

**A diff that adds or changes a race also checks the race's baseline arm against a CPU
reference.** The baseline arm is the arm running the implementation already in use. The
reference check runs in the same process, on the same output elements the arms are judged on.
Two arms can agree and both be wrong; only the reference makes the winner right.

**A knockout or sweep arm - one that attributes cost across stages instead of selecting
between two implementations - carries the literal token `timing-only` on its report line.**

**An instrument all of whose arms attribute carries the literal text `ATTRIBUTION SWEEP` in its
file header comment, on a line that also names what its arms attribute.** Without the text a
reader takes the sweep's arms for an adoption decision it never made.

**A new instrument that puts its own clock around a served turn is a defect: add a board cell
to `../performance/gen_bench_records.das`, or a `lcpp_bench.das` cell with its own
`../PROFILE.md` section, instead.** A served turn is a whole prefill-plus-decode run. A
second instrument's numbers cannot be compared to any row the board already carries.

**An out-of-process observer never measures what the benchmark process can measure about
itself - that measurement goes inside the process instead.** An out-of-process observer is a
script that measures a benchmark process from outside.

**Only the reference cells of `../performance/gen_bench_records.das` - the cells that time a
binary this repository does not build on a board workload - write such a binary's wall time
into `../performance/records/<box>.json` or `../PERF_LEDGER.md`; only `../harness/mtp_ruler.das`
writes one into `../performance/records/mtp/`.**

**A file holding a third-party wall outside `../performance/records/` and `../PERF_LEDGER.md`
- a reference leg's recovery file, a pinned reference tsv an instrument
reads back - is scratch: untracked, owned by exactly one instrument, re-derivable from a
command written where the owning instrument documents its flags, and never an input to a
board cell.** A tracked or shared copy of a third-party wall becomes a stale baseline nobody
re-derives.

**A diff that adds or changes an instrument that prints the difference of two walls also
prints both of those walls on that report line.** A plain elapsed-time row - one clock pair,
no attribution across stages - is not a difference.

**A diff that changes what a board cell times ships before/after rows for each affected cell
and corpus - or withdraws the affected rows and names the withdrawal and its reason in the PR
body.** A board cell is a timed cell of the published results board: one
`../performance/gen_bench_records.das` spawns, or a manual `lcpp_bench.das` cell with its own
`../PROFILE.md` section. What a cell times changes when a change to its code, to its input
corpus, or to the pinned reference build - anything that decides which reference binary or
reference environment the run measures, `DEFAULT_REF_SHA` in `setup_lcpp_ref.das` included -
moves the measured quantity. The re-mint or withdrawal lands in
`../performance/records/<box>.json`, the file the affected rows live in.

**A diff that adds or changes an instrument makes it exit non-zero on a run that ends
with zero result rows - wrong flags, failed load, a device that declines.** A run that matched
nothing and reported success leaves a sidecar or a record untouched, and its caller cannot
tell.

**A diff that adds or changes an A/B arm - one of an instrument's timed runs, distinguished by a
named lever set to a value the paired run does not use, off/on or graded - makes that instrument
exit non-zero when the lever does not change what the run executes.** A lever is the flag or
environment switch that names the arm; a lever that silently no-ops prints a 1.00x row nobody
can tell from a real tie.

**A diff that adds or changes an A/B arm of an instrument over a prompt corpus makes that arm
report one row per prompt, never one aggregate ratio alone.** Prompts differ in how much the
lever helps, so a per-prompt loss hides inside a winning mean.
