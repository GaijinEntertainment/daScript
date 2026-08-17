# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `skills/internal/preflight.md` (repo root).

**A per-PR gate step in `doc.yml` must fail the lane on a doc defect and only on a doc defect —
removing a step, neutering it, or renaming or deleting the script or target it invokes is a
defect.** Each gate enforces a doc invariant with no reviewer involved (the gate map:
`skills/internal/preflight.md` (repo root), the doc.yml section).

**Weakening the `all_utils_exe` build step in `extended_checks.yml` is a defect.** It is what
machine-enforces that every shipped tool exe stays `daslang -exe`-compilable.
