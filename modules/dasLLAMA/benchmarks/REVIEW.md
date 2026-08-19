# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**A kernel A/B lab — a rig whose output selects between two implementations of the same
compute — times both variants interleaved in one process with one instrument.** A board bench
cell that picks a kernel tier by flag is not an A/B lab.

**An A/B arm whose timing is reported as adoptable evidence is first compared bit-identical to
the baseline arm** — the production kernel every variant is measured against; an arm reported
as evidence without that compare is a defect.

**An A/B arm that skips the bit-identical compare — a timing-only arm — says so in its report
line, and its number never becomes adoption evidence.**

**A kernel A/B lab's numbers stay out of `../performance/records/<box>.json` and
`../PERF_LEDGER.md`.**

**An out-of-process observer — a script that measures a benchmark process from outside —
measures only what a process cannot measure about itself; its numbers may enter
`../PERF_LEDGER.md` and no record file.**

**The wall time of a binary that is not one of this folder's benchmarks enters
`../performance/records/<box>.json` or `../PERF_LEDGER.md` only through
`../performance/gen_bench_records.das`'s reference passes.** Any other foreign wall time in a
record or the ledger is a defect.

**A number derived by subtracting one measured wall from another prints both raw walls, not
only the difference.**
