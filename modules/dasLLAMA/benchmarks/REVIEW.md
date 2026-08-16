# benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**No new file produces record-grade model timing — a benchmark's self-measured time that
enters `../performance/records/<box>.json` or `../PERF_LEDGER.md`.** Model-level time is measured by
`../performance/gen_profile.das` and `../performance/gen_bench_records.das`, both spawning
`lcpp_bench.das` here; a one-off measurement script or a revived rig is a defect.

**A kernel A/B lab answers which of two kernel variants to adopt, by timing both interleaved
in one process with one instrument.** Before any timing counts, both arms must produce
bit-identical output — a lab without that cross-arm assertion is a defect. A lab's numbers
stay out of `../performance/records/<box>.json` and `../PERF_LEDGER.md`.

**An out-of-process observer — a script that measures a benchmark process from outside —
measures only what a process cannot measure about itself; those numbers may enter
`../PERF_LEDGER.md`.** Wall-clock timing of a foreign reference binary does not qualify —
the observer license covers gaps in a benchmark's self-measurement, and a foreign binary is
not one of this folder's benchmarks. An estimate formed by subtracting one measured wall
from another prints both raw walls, not only the difference.
