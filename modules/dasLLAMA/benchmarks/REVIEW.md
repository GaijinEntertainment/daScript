# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**A diff that also lands a row in `../performance/records/<box>.json` applies
`../performance/REVIEW.md`, and one that also lands an entry in `../PERF_LEDGER.md` applies
`../REVIEW.md`.**

**A timed rep that dispatches a pipeline the model runtime selected - not one the lab
compiled itself - calls `tune_gate()` (`../performance/profile_common.das`) before its first
timed rep**, or it measures fallback kernels silently. Tokenizing and detokenizing (`encode` /
`encode_` / `decode` / `decode_`, on a `Model` or a `Tokenizer`) run no forward pass, so
nothing there is gated.

**A race in this folder that compares two implementations times both arms interleaved in one
process (`race_pair_ms`).** Two separate runs measure the box's drift between them as much as
the arms.

**An A/B arm reported as adoptable evidence shows the compare its form calls for: an arm
whose form differs from the baseline only in timing carries the literal token `timing-only`
in its report line; an arm whose form differs in PRECISION prints a bounded-difference
compare against the baseline arm or the CPU reference and the bound it passed; an arm of the
same form prints the bit-exact compare over the sampled region (the report's "bit-exact
vs ..." line).** An arm reported as evidence showing none of the three is a defect.

**A lab's baseline arm is compared against a CPU reference over the sampled region, in the
same run.** The arm-vs-baseline compare proves the two agree, not that either is right.

**A knockout or sweep instrument - one whose arms ATTRIBUTE cost across stages rather than
select between two implementations - carries the literal text `ATTRIBUTION SWEEP` in its
file header comment, on a line that also names what its arms attribute.** Without it the
instrument reads as a lab and is deleted with a decision it never made.

**An out-of-process observer - a script that measures a benchmark process from outside -
measures only what a process cannot measure about itself.**

**A timing instrument this checklist governs never writes the wall time of a binary this
repository does not build - a third-party reference tool - into
`../performance/records/<box>.json` or `../PERF_LEDGER.md`**; such walls enter only through
the reference cells of `../performance/gen_bench_records.das`, the cells that time such a
tool on a board workload.

**An instrument under this folder that prints a number formed by subtracting one measured
wall from another prints both raw walls on that report line.**

**A change to what a board cell times - its code, its input corpus, or the pinned reference
build (`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`) - ships before/after rows for each affected
cell and corpus, or a statement that the measured quantity is unchanged.**
