# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md` (the benchmark rig is sec.2.5). Planned work: `../PERF_LEDGER.md` for
a performance followup, `../followup_general.md` for everything else.

**A diff that adds or changes an instrument whose timed body runs a forward pass through a
pipeline the model runtime selected also calls `tune_gate()`
(`../performance/profile_common.das`) before that instrument's first timed rep.** A pipeline
the instrument compiled itself is not one the runtime selected. Without the gate the instrument
measures fallback kernels silently.

**A diff that adds or changes a race times both arms interleaved in one process - a Metal race
through `race_pair_ms`.** A race is an instrument that compares two implementations. Two
separate runs measure the box's drift between them as much as they measure the arms.

**A diff that adds or changes a race arm offered as the reason to adopt a change, where the arm
produces no comparable output, carries the literal token `timing-only` in the arm's report
line.**

**A diff that adds or changes a race arm offered as the reason to adopt a change, where the arm
computes the baseline's result in a different precision, prints a bounded-difference compare.**
The compare runs against the baseline arm or against the CPU reference. The report line also
prints the bound the arm passed.

**A diff that adds or changes a race arm offered as the reason to adopt a change, where the arm
computes the baseline's result in the same precision, prints the bit-exact compare over the
sampled region.** The sampled region is the set of output elements the run compares. The
compare prints on the report's "bit-exact vs ..." line.

**A diff that adds or changes a race also checks the race's baseline arm against a CPU
reference.** The baseline arm is the arm running the implementation already in use. The
reference check runs in the same process, on the same output elements the arms are judged on.
Two arms can agree and both be wrong; only the reference makes the winner right.

**A diff that adds or changes a knockout or sweep instrument carries the literal text
`ATTRIBUTION SWEEP` in the instrument's file header comment.** A knockout or sweep instrument
is one whose arms attribute cost across stages instead of selecting between two
implementations. The same header line also names what its arms attribute. Without that text a
reader takes the sweep's arms for an adoption decision it never made.

**An out-of-process observer never measures what the benchmark process can measure about
itself - that measurement goes inside the process instead.** An out-of-process observer is a
script that measures a benchmark process from outside.

**A timing instrument this checklist governs never writes the wall time of a binary this
repository does not build - a third-party reference tool - into
`../performance/records/<box>.json` or `../PERF_LEDGER.md`.** Such walls enter only through
the reference cells of `../performance/gen_bench_records.das`, the cells that time such a
tool on a board workload.

**A diff that adds or changes an instrument that prints a number formed by subtracting one
measured wall from another also prints both raw walls on that report line.**

**A diff that changes what a board cell times ships before/after rows for each affected cell
and corpus.** A board cell is a timed cell of the published results board: one
`../performance/gen_bench_records.das` spawns, or a manual `lcpp_bench.das` cell with its own
`../PROFILE.md` section. What a cell times changes when a change to its code, to its input
corpus, or to the pinned reference build (`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`) moves the
measured quantity.
