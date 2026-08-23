# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

**A per-PR gate step in `doc.yml` or `extended_checks.yml` keeps checking every tracked source
it checked before the diff - deleting the step, adding `continue-on-error`, narrowing its
`if:`, weakening its command, or shrinking the tracked-source set it covers is a defect.**
Each such step machine-enforces an invariant with no reviewer involved - 
`skills/internal/preflight.md` sec."doc.yml - the gates", sec."extended_checks.yml".

**A per-PR gate step in `doc.yml` or `extended_checks.yml` fails the lane on a defect in
what it checks, and only on such a defect.**
