# dasMetal kernel tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/ARCHITECTURE.md`.

**A CPU-oracle test - one computing its expected values on the CPU from the kernel's own code - of a
kernel whose class has a member neither `@ssbo` nor `@workgroup` and whose body calls no `barrier()`
builds a fresh class instance for every thread id; reusing one is a defect.** Such a member is
per-thread state, so a reused instance carries the previous thread's values into the next.

**A CPU-oracle test of a kernel whose body calls `barrier()` is a defect unless every thread id of a
workgroup finishes one barrier-separated phase before any starts the next, each phase repeating that
phase's statements of the kernel body with the thread id in place of each invocation-id read, each
workgroup on its own `@workgroup` values, and each thread id on its own instance's per-thread
members (members neither `@ssbo` nor `@workgroup`), kept across that thread's phases.** A CPU run
of the method cannot stop at a barrier.

**A diff that adds or changes a `[test]` in this folder that creates any Metal object asserts
`metal_live_object_count() == 0` before that test returns.**
