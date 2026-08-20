# detect-dupe Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**Decision record: detect-dupe's `DAS_UTILS_SHIPPED_EXES` membership is deliberate** — it
ships as a prebuilt exe. `utils/REVIEW.das` machine-checks the membership itself; a diff
removing the entry records the removal decision here, per `utils/REVIEW.md`.
