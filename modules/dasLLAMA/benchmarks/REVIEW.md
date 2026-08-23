# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**A timed rep that runs model or tower kernels - any `eval*` / `generate*` / `forward*` call
on a `Model`, or any `encode_image*` / `encode_audio*` call on a vision or audio embedder or
tower, all under `../dasllama/` - calls `tune_gate()` (`../performance/profile_common.das`)
before its first timed rep**, or it measures fallback kernels silently. Tokenizing and
detokenizing (`encode` / `encode_` / `decode` / `decode_`, on a `Model` or a `Tokenizer`)
are not forward passes.

**A kernel A/B lab - a rig whose output selects between two implementations of the same
compute - times both variants interleaved in one process with one instrument.** A board bench
cell that picks a kernel tier by flag is not an A/B lab.

**An A/B arm whose timing is reported as adoptable evidence is first compared bit-identical to
the baseline arm** - the production kernel every variant is measured against; an arm reported
as evidence without that compare is a defect.

**An A/B arm that skips the bit-identical compare - a timing-only arm - says so in its report
line.**

**A knockout or sweep instrument - one whose arms ATTRIBUTE cost across stages rather than
select between two implementations - says so in its header: a line naming it an attribution
sweep and naming what its arms attribute.** Without it the instrument reads as a lab and is
deleted with a decision it never made.

**A kernel A/B lab's numbers stay out of `../performance/records/<box>.json` and
`../PERF_LEDGER.md`** - they settle the adoption decision in the lab's own report and the PR
that lands the kernel; the board learns the winner only through a re-measured cell row.

**An out-of-process observer - a script that measures a benchmark process from outside - 
measures only what a process cannot measure about itself; its numbers may enter
`../PERF_LEDGER.md` and no record file.**

**A timing instrument this checklist governs never writes the wall time of a binary this
repository does not build - a third-party reference tool - into
`../performance/records/<box>.json` or `../PERF_LEDGER.md`**; such walls enter only through
the reference cells of `../performance/gen_bench_records.das`, the cells that time such a
tool on a board workload.

**A number derived by subtracting one measured wall from another prints both raw walls, not
only the difference.**

**A change to the corpus input of the `--tok` measurement cell (`lcpp_bench.das`) ships
before/after `--tok` rows from `lcpp_bench.das --tok` for each affected corpus, or a
statement that the corpus bytes are unchanged.**
