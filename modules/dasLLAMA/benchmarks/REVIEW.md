# dasLLAMA benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `../PROFILE.md`.

**A kernel A/B lab answers which of two kernel variants to adopt, by timing both interleaved
in one process with one instrument.** An arm whose timing is reported as adoptable evidence is
compared bit-identical to v0 before its timing counts; a timing-only arm says so in its report
line, and its number never becomes adoption evidence.

**Numbers enter `../performance/records/<box>.json` and `../PERF_LEDGER.md` by exactly one door
each:** a lab's numbers enter neither; an out-of-process observer — a script that measures a
benchmark process from outside — measures only what a process cannot measure about itself, and
those numbers may enter `../PERF_LEDGER.md`; the wall time of a binary that is not one of this
folder's benchmarks enters either only through `../performance/gen_bench_records.das`'s
reference passes. Any other route is a defect.

**A number derived by subtracting one measured wall from another prints both raw walls, not
only the difference.**
