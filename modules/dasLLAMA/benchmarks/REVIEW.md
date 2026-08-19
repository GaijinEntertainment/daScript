# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**A kernel A/B lab answers which of two kernel variants to adopt, by timing both interleaved
in one process with one instrument.** An arm whose timing is reported as adoptable evidence is
compared bit-identical to the baseline arm — the production kernel every variant is measured
against — before its timing counts; a timing-only arm says so in its report line, and its
number never becomes adoption evidence.

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
