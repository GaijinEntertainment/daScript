# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**An instrument whose timed body dispatches a pipeline the model runtime selected - not one
the instrument compiled itself - calls `tune_gate()` (`../performance/profile_common.das`)
before its first timed rep**, or it measures fallback kernels silently. Tokenizing and
detokenizing (`encode` / `encode_` / `decode` / `decode_`, on a `Model` or a `Tokenizer`) run
no forward pass, so nothing there is gated.

**A race this checklist governs - an instrument that compares two implementations - times
both arms interleaved in one process; a Metal race does that through `race_pair_ms`.** Two
separate runs measure the box's drift between them as much as the arms.

**An A/B arm reported as adoptable evidence that produces no comparable output - it exists
only to time work - carries the literal token `timing-only` in its report line.**

**An A/B arm reported as adoptable evidence that computes the baseline's result in a
different precision prints a bounded-difference compare against the baseline arm or the CPU
reference, and the bound it passed.**

**An A/B arm reported as adoptable evidence that computes the baseline's result in the same
precision prints the bit-exact compare over the sampled region - the output elements the run
compares - on the report's "bit-exact vs ..." line.**

**An instrument that races two implementations to pick one compares its baseline arm - the
arm it treats as already correct - against a CPU reference over the same output region it
compares the arms over, in the same run.** The arm-vs-baseline compare proves the two agree,
not that either is right.

**A knockout or sweep instrument - one whose arms ATTRIBUTE cost across stages rather than
select between two implementations - carries the literal text `ATTRIBUTION SWEEP` in its
file header comment, on a line that also names what its arms attribute.** Without it a reader
takes the sweep's arms for an adoption decision it never made.

**An out-of-process observer - a script that measures a benchmark process from outside -
measures only what a process cannot measure about itself.**

**A timing instrument this checklist governs never writes the wall time of a binary this
repository does not build - a third-party reference tool - into
`../performance/records/<box>.json` or `../PERF_LEDGER.md`**; such walls enter only through
the reference cells of `../performance/gen_bench_records.das`, the cells that time such a
tool on a board workload.

**An instrument this checklist governs that prints a number formed by subtracting one measured
wall from another prints both raw walls on that report line.**

**A change to what a board cell times - a timed cell of the published results board, one
`../performance/gen_bench_records.das` spawns or a manual `lcpp_bench.das` cell
`../PROFILE.md` documents - its code, its input corpus, or the pinned reference build
(`DEFAULT_REF_SHA` in `setup_lcpp_ref.das`) ships before/after rows for each affected cell
and corpus, or a statement that the measured quantity is unchanged.**
