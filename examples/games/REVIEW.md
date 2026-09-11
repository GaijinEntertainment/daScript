# examples/games Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff that deletes a check in `REVIEW.das` (beside this file), narrows what it walks, or
drops a finding it used to report is a defect** - updating a constant so a check keeps
matching the tree is not.

- **A module a game in this folder requires is a plain sibling module, never `shared`** - a
  shared module survives a live reload as the promoted copy, and the late-bound GL calls
  inside it fail their second compile.

- **A shader or post-process pass a diff adds to a game in this folder uses only GLSL ES 3.00
  features, and probes any colour format above RGBA8 at creation with an RGBA8 fallback** -
  the same file runs in the playground on WebGL2, which does not guarantee a float colour
  attachment.

