# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md`. Planned work: `../followup_general.md`,
`../followup_metal.md`, `../followup_vulkan.md`.

**In `tune_kernels.das`, every race or bench that calls `pin_kernel_backend`, directly or through a
helper, runs after every race and bench that does not; a diff that runs one earlier is a defect.** A race
times kernel variants against each other. The pin lasts for the rest of the process, so every
timing after it measures the pinned backend.

**Weakening `check_last_bench_row` (`REVIEW.das` beside this file) is a defect.**

**Never set `@role = "alias"` or `"weight"` on a `[vk_dispatch]` binding in this folder whose
only read sits in another file - move the read into the binding's own file instead.** The lens -
the pass that collects a binding's accesses from its declaring file alone - reports such a
binding as never accessed, and `@role` silences the report.
