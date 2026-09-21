# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_MEASUREMENT.md`, `../ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`. Planned
work: `../followup_metal.md` for anything about the Metal backend, `../followup_vulkan.md` for
anything about the Vulkan backend, `../followup_general.md` for everything else. A measured rate
of the engine this repository builds is recorded under `../REVIEW_MEASUREMENT.md`, not by this
checklist.

**A diff that adds or moves a timing in `tune_kernels.das` keeps `dot_q8q8_laneq4x4` the LAST
row of the `benches` list, and adds no CPU timing that runs after it.** Running that bench pins one
matmul backend for the rest of the process, so a CPU timing after it runs against the pinned
backend instead of the one it would have picked.

**A diff that points a `dashv` call in this folder at the sidecar exchange - dasllama.io's
tune-sidecar service: sidecar lookup, download, or submit - is a defect; it calls the exchange
through `dasllama/dasllama_exchange` (the module `../dasllama/dasllama_exchange.das` declares)
instead.**

**Never set `@role = "alias"` or `"weight"` on a `[vk_dispatch]` binding in this folder whose
only read sits in another file - move the read into the binding's own file instead.** The lens -
the pass that collects a binding's accesses from its declaring file alone - reports such a
binding as never accessed, and `@role` silences the report.

**A diff that adds a measured number - a time, a rate, or a figure computed from one - that a
file in this folder prints, or changes what an already-printed number measures, ships that
number's alternate in the same change - another row of the same run at the same shape, or the
reference exe's row (the third-party engine this module measures against) for the same shape,
with the command that produced it.** A number with no alternate beside it ranks nothing.

**A reference exe's row a diff records as the alternate of a number a file in this folder
printed lands in `../ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md` sec.2.5a when the number was
printed by `vk_gemm_probe.das` or `vk_gemv_probe.das`, and in `../ARCHITECTURE_MEASUREMENT.md`
sec.2.5 otherwise.**
