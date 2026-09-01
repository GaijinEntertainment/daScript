# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md` (the benchmark rig: `#one-benchmark-rig`). Planned work:
`../followup_vulkan.md` for anything about the Vulkan backend, `../PERF_LEDGER.md` for any
other performance followup, `../followup_general.md` for everything else.

**A diff that adds or changes an instrument whose timed body runs a forward pass through a
pipeline the dasLLAMA engine selected also calls `tune_gate()`
(`../performance/profile_common.das`) before that instrument's first timed rep.** A pipeline
the instrument compiled itself - or a reference tool's own runtime - is not one the engine
selected. Without the gate the instrument measures fallback kernels silently.

**A diff that adds or changes a race times both arms interleaved in one process - a Metal race
through `race_pair_ms`.** A race is an instrument that compares two implementations. Two
separate runs measure the box's drift between them as much as they measure the arms.

**A diff that adds or changes a race arm offered as the reason to adopt a change proves the
arm's output on its report line, by what the arm computes:** an arm producing no comparable
output carries the literal token `timing-only`; an arm computing the baseline's result in a
different precision prints a bounded-difference compare (against the baseline arm or the CPU
reference) plus the bound it passed; an arm computing it in the same precision prints the
bit-exact compare over the sampled region - the set of output elements the run compares - on
the report's "bit-exact vs ..." line.

**A diff that adds or changes a race also checks the race's baseline arm against a CPU
reference.** The baseline arm is the arm running the implementation already in use. The
reference check runs in the same process, on the same output elements the arms are judged on.
Two arms can agree and both be wrong; only the reference makes the winner right.

**A diff that adds or changes a knockout or sweep instrument carries the literal text
`ATTRIBUTION SWEEP` in the instrument's file header comment.** A knockout or sweep instrument
is one whose arms attribute cost across stages instead of selecting between two
implementations. The same header line also names what its arms attribute. Without that text a
reader takes the sweep's arms for an adoption decision it never made.

**A new instrument that times a served turn is a defect - add a board cell to
`../performance/gen_bench_records.das`, or a `lcpp_bench.das` cell with its own
`../PROFILE.md` section, instead.** A served turn is a whole prefill-plus-decode run. A
second instrument's numbers cannot be compared to any row the board already carries.

**An out-of-process observer never measures what the benchmark process can measure about
itself - that measurement goes inside the process instead.** An out-of-process observer is a
script that measures a benchmark process from outside.

**A timing instrument this checklist governs never writes the wall time of a binary this
repository does not build - a third-party reference tool - into
`../performance/records/<box>.json` or `../PERF_LEDGER.md`.** Such walls enter only through
the reference cells of `../performance/gen_bench_records.das`, the cells that time such a
tool on a board workload. A recovery file a reference leg writes (`asr/_pybench_rows.txt`)
is that leg's scratch - untracked, truncated before each spawn, never read by any other
tool's cell.

**A diff that adds or changes an instrument that prints a number formed by subtracting one
wall it also reports from another also prints both raw walls on that report line.** A plain
elapsed-time row (one clock pair, no attribution across stages) is not one.

**A diff that changes what a board cell times ships before/after rows for each affected cell
and corpus - or withdraws the affected rows and names the withdrawal and its reason in the PR
body.** A board cell is a timed cell of the published results board: one
`../performance/gen_bench_records.das` spawns, or a manual `lcpp_bench.das` cell with its own
`../PROFILE.md` section. What a cell times changes when a change to its code, to its input
corpus, or to the pinned reference build - `DEFAULT_REF_SHA` in `setup_lcpp_ref.das`, the
targets it builds, or a patch it applies - moves the measured quantity.

**A diff that adds or moves a row in `../harness/tune_kernels.das`'s bench list keeps
`dot_q8q8_laneq4x4` last.** That bench pins the repack backend for the rest of the process,
so a row after it races against the pinned backend instead of the one it selects.

**A timing instrument this checklist governs whose run can end with zero result rows exits
non-zero when it does - wrong flags, failed load, a device that declines.** A run that matched
nothing and reported success leaves a sidecar or a record untouched and a caller none the
wiser.
