# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `skills/internal/preflight.md` (repo root).

**Weakening a per-PR gate in `doc.yml` — any step that fails the lane on a doc defect, including
the `-W` warnings-as-errors flag carried by the `build_doc` target the build step invokes — is a
defect.** Each gate enforces a doc invariant with no reviewer involved (the gate map:
`skills/internal/preflight.md` (repo root), "doc.yml — the five gates").
