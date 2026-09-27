# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE_MEASUREMENT.md`, `../ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md`. Planned
work: `../followup_metal.md` for anything about the Metal backend, `../followup_vulkan.md` for
anything about the Vulkan backend, `../followup_general.md` for everything else.

**A diff to `tune_kernels.das` that adds a race or bench of a kernel that no `benches` row times
runs it before the `benches` sweep starts - never after the sweep.** Running `dot_q8q8_laneq4x4`
pins one matmul backend for the rest of the process, so a timing after it runs against the pinned
backend instead of the one it would have picked.

**Weakening `check_last_bench_row` (`REVIEW.das` beside this file) is a defect.**

**Never set `@role = "alias"` or `"weight"` on a `[vk_dispatch]` binding in this folder whose
only read sits in another file - move the read into the binding's own file instead.** The lens -
the pass that collects a binding's accesses from its declaring file alone - reports such a
binding as never accessed, and `@role` silences the report.
