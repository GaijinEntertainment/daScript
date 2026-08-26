# dasMetal kernel tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`modules/dasMetal/MASTERPLAN.md`.

**A CPU-oracle test builds a fresh kernel instance per simulated thread.** Kernel state is
per-thread; sharing one instance across simulated threads is a defect of the test, not of the
kernel.
