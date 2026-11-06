# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md`. Planned work: `../followup_general.md`,
`../followup_metal.md`, `../followup_vulkan.md`.

**In `tune_kernels.das`, never time a CPU kernel after any call that pins a kernel backend
(`pin_kernel_backend`, `pin_kernel_backend_env`, or a helper that calls either) and before
`restore_kernel_backend` - pin inside `with_kernel_backend` instead, or move the pin after every
CPU race and bench.** `clear_kernel_backend_pin` alone leaves the
pinned backend active, so every CPU timing after it still measures that backend.

**Weakening `check_last_bench_row` (`REVIEW.das` beside this file) is a defect.**

**Never set `@role = "alias"` or `"weight"` on a binding of a dispatch-macro class
(`[vk_dispatch]`, `[cuda_dispatch]`, `[metal_dispatch]`) in this folder when the binding's only
read sits in another file - move the read into the binding's own file instead.** Those macros
refuse a binding that the file declaring the class never reads, and `@role` silences the refusal.
