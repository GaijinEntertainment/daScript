# dasMetal kernel tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/MASTERPLAN.md`.

**A CPU-oracle test - one that produces its expected values by running the kernel method on the
CPU, a driver loop setting `gl_GlobalInvocationID` once per thread id - builds a fresh instance
of the kernel class for each of those ids.** Kernel members are per-thread state, so one
instance reused across ids carries the previous thread's values into the next; sharing is a
defect of the test, not of the kernel.
