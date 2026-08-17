# benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**Record-grade model timing — a self-measured time entering `../performance/records/<box>.json`
or `../PERF_LEDGER.md` — comes only from `lcpp_bench.das` spawned by
`../performance/gen_profile.das` or `../performance/gen_bench_records.das`.** Any other
producer, new or edited, is a defect.

**A kernel A/B lab answers which of two kernel variants to adopt, by timing both interleaved
in one process with one instrument.** Before any timing counts, both arms must produce
bit-identical output — a lab without that cross-arm assertion is a defect. A lab's numbers
stay out of `../performance/records/<box>.json` and `../PERF_LEDGER.md`.

**An out-of-process observer — a script that measures a benchmark process from outside —
measures only what a process cannot measure about itself; those numbers may enter
`../PERF_LEDGER.md`.** The observer license covers gaps in a benchmark's self-measurement.

**Foreign reference timing enters `../performance/records/<box>.json` only through
`../performance/gen_bench_records.das`'s reference passes;** any other foreign-binary wall
time in a record or the ledger is a defect.

**A number derived by subtracting one measured wall from another prints both raw walls, not
only the difference.**
