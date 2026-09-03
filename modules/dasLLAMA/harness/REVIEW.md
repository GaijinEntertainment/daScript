# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md`. Planned work: `../followup_metal.md` for anything about
the Metal backend, `../followup_vulkan.md` for anything about the Vulkan backend,
`../PERF_LEDGER.md` for any other performance followup, `../followup_general.md` for
everything else.

**A diff that adds or moves a row in `tune_kernels.das`'s `benches` list - the name/function
pairs `main` declares up front, or any later mutation of that list - keeps `dot_q8q8_laneq4x4`
last.** Running that bench pins one matmul backend for the rest of the process, so a row after
it is timed against the pinned backend instead of the one it would have picked.

**A diff that points a `dashv` call in this folder at the sidecar exchange is a defect - require
`../dasllama/dasllama_exchange.das` and call it instead.**
