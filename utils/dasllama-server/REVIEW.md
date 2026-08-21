# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `README.md`.

**A fixture under `tests/fixtures/` carries the bytes of a real server run with machine-local
paths normalized, and is never otherwise written or edited by hand** — the capture rail is
`tests/fixtures/README.md`; a schema change re-captures the file rather than editing it. A diff
that changes fixture VALUES without a capture behind it is a defect.

**Weakening `REVIEW.das` (beside this file) is a defect.** The gate machine-checks the
mechanical half of the fixture rule; each check's finding text states what it enforces.

**A route added, removed, or reshaped updates every surface that documents or consumes it in
the same change**: the `README.md` route row, the endpoint table in
`doc/source/reference/utils/dasllama_server.rst` (repo root), and `control.html` where the page
consumes it.

**A stats or streams wire key added, removed, or renamed updates those same surfaces, and its
fixtures under `tests/fixtures/` re-capture in the same change** — the fixtures are the
recorded wire shape.
