# benchmarks Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `modules/dasLLAMA/PROFILE.md` (the measurement rigs this folder feeds).

**No new record-grade timing harness.** Model-level time is measured by the rigs `PROFILE.md`
documents — `modules/dasLLAMA/performance/gen_profile.das` and `gen_bench_records.das`, both
spawning `lcpp_bench.das` here. A new file that times a model and feeds a board, a one-off
measurement script, or a revived rig is a defect; the one sanctioned sub-model instrument is
the kernel A/B lab.

**A kernel A/B lab is a decision instrument, not a record rig.** It answers a kernel-join
question by interleaved same-instrument A/B under a bit-exact cross-arm gate; hand-rolled bind
lists are legal in a lab, and a binding renumber fixes or deletes the arms it rots. Adoption
deletes the pair; numbers never enter the record stores or `PERF_LEDGER.md`.

**An out-of-process observer measures only what no in-process rig can observe about itself, and
its numbers are ledger-grade** — today that is `asr/mem_census.sh` alone. Wall-clock timing of
a FOREIGN reference binary is not one; a subtraction-based estimate prints both raw walls.

**A measuring entry point calls `tune_gate()`
(`modules/dasLLAMA/performance/profile_common.das`) before its first timed rep**, or it
measures fallback kernels silently. A/B labs dispatch through their own arms and the `--tok`
cell dispatches none — neither is a measuring entry point in this sense.
