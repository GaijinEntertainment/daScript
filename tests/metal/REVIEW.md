# dasMetal kernel tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/MASTERPLAN.md`.

**A CPU-oracle test builds a fresh instance of the kernel class for every thread id it
drives.** A CPU-oracle test produces its expected values by running the kernel method on the
CPU. Its driver loop sets `gl_GlobalInvocationID` once per thread id. Kernel members are
per-thread state, so one instance reused across ids carries the previous thread's values into
the next. Sharing one instance is a defect of the test, not of the kernel.
