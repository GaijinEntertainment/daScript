# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md`. Planned work: `../followup_metal.md` for anything about
the Metal backend, `../followup_vulkan.md` for anything about the Vulkan backend,
`../PERF_LEDGER.md` for any other performance followup, `../followup_general.md` for
everything else.

**A diff that adds or moves a timed unit in `tune_kernels.das` keeps `dot_q8q8_laneq4x4` the
LAST row of the `benches` list and adds no CPU timing after it; the Metal crown race after the
list is the one thing allowed there.** Running that bench pins one matmul backend for the rest of
the process, so a CPU timing after it runs against the pinned backend instead of the one it would
have picked.

**A diff that points a `dashv` call in this folder at the sidecar exchange - dasllama.io's
tune-sidecar service: sidecar lookup, download, or submit - is a defect; it calls the exchange
through `dasllama/dasllama_exchange` (the module `../dasllama/dasllama_exchange.das` declares)
instead.** A `dashv` call at a reference engine's server (a llama-server on localhost) is not
the exchange.

**A `[vk_dispatch]` class in this folder never silences the lens's never-accessed check with
`@role = "alias"` or `"weight"` on a binding its kernel reads through a `dasllama/` base's
body - it reads that binding in a body this folder's file declares.** The lens collects
accesses from this file's bodies only, and `@role` keeps the access out of the hazard masks, so
a base-read binding silenced here escapes the hazard rail.
