# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md`. Planned work: `../followup_general.md`,
`../followup_metal.md`, `../followup_vulkan.md`.

**In `tune_kernels.das`, a diff that runs a race or bench calling `pin_kernel_backend` (directly
or through a helper) before a race or bench that does not is a defect - move it after them.** A
race times kernel variants against each other. The pin lasts for the rest of the process, so
every timing after it measures the pinned backend.

**Weakening `check_last_bench_row` (`REVIEW.das` beside this file) is a defect.**

**Never set `@role = "alias"` or `"weight"` on a binding of a `[vk_dispatch]` or
`[cuda_dispatch]` class in this folder when the binding's only read sits in another file - move
the read into the binding's own file instead.** Both lenses read only the class's own file, so
they report such a binding as never accessed, and `@role` silences the report.
