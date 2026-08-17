# benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**A kernel A/B lab answers which of two kernel variants to adopt, by timing both interleaved
in one process with one instrument.** Before any timing counts, both arms must produce
bit-identical output — a lab without that cross-arm assertion is a defect. A lab's numbers
stay out of `../performance/records/<box>.json` and `../PERF_LEDGER.md`.

**An out-of-process observer — a script that measures a benchmark process from outside —
measures only what a process cannot measure about itself; those numbers may enter
`../PERF_LEDGER.md`.** The license covers gaps in the self-measurement of this folder's own
benchmarks; a foreign binary is not one of them.

**Foreign reference timing — the wall time of a binary that is not one of this folder's
benchmarks — enters `../performance/records/<box>.json` or `../PERF_LEDGER.md` only through
`../performance/gen_bench_records.das`'s reference passes.** Any other foreign-binary wall
time in a record or the ledger is a defect.

**A number derived by subtracting one measured wall from another prints both raw walls, not
only the difference.**
