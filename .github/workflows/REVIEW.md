# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

**A diff that deletes a per-PR gate step in `doc.yml` or `extended_checks.yml`, adds
`continue-on-error` to it, narrows its `if:`, weakens its command, or shrinks the set of
tracked source files it checks is a defect.** Each such step enforces its rule automatically,
with no reviewer involved - `skills/internal/preflight.md` sec."doc.yml - the gates",
sec."extended_checks.yml".

**A diff that adds or changes a per-PR gate step in `doc.yml` or `extended_checks.yml` makes
that step fail the lane on every defect in what it checks, and on nothing else.**
