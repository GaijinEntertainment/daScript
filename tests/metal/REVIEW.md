# dasMetal kernel tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/ARCHITECTURE.md`.

**A CPU-oracle test that reuses one kernel-class instance across thread ids is a defect of the
test, not of the kernel - build a fresh instance for every thread id.** A CPU-oracle test
produces its expected values by running the kernel method on the CPU, in a driver loop that
sets `gl_GlobalInvocationID` once per thread id. Kernel members are per-thread state, so a
reused instance carries the previous thread's values into the next.

**A diff that adds or changes a `[test]` in this folder that creates any Metal object asserts
`metal_live_object_count() == 0` before that test returns.**
