# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md` (it routes to its companions). Planned
work: `../followup_general.md`, `../followup_metal.md`, `../followup_vulkan.md`.

**A diff that places a race or bench in `tune_kernels.das` that calls `pin_kernel_backend`,
directly or through a helper, ahead of one that does not, is a defect - it moves after them.** The
pin holds for the rest of the process, so a timing after it runs against the pinned backend
instead of the one it would have picked.

**Weakening `check_last_bench_row` (`REVIEW.das` beside this file) is a defect.**

**Never set `@role = "alias"` or `"weight"` on a `[vk_dispatch]` binding in this folder whose
only read sits in another file - move the read into the binding's own file instead.** The lens -
the pass that collects a binding's accesses from its declaring file alone - reports such a
binding as never accessed, and `@role` silences the report.
