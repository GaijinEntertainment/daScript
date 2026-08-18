# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `skills/internal/preflight.md` (repo root).

**Removing or neutering a per-PR gate step in `doc.yml` or `extended_checks.yml` — deleting
the step, adding `continue-on-error`, narrowing its `if:`, or substituting a weaker target or
command — is a defect.** Each such step machine-enforces an invariant with no reviewer
involved (the gate map: `skills/internal/preflight.md` (repo root)).

**A per-PR gate step in `doc.yml` fails the lane on a doc defect and only on a doc defect.**
