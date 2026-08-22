# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `README.md`.

**A diff calling any `modules/dasLLAMA` module symbol (a `dasllama_*` def) or naming one of
its structs in this folder applies `modules/dasLLAMA/REVIEW.md` (repo root) too** — its gate
polices the family seams here.

**A fixture under `tests/fixtures/` carries the bytes of a real server run with machine-local
paths normalized, and is never otherwise written or edited by hand** — the capture rail is
`tests/fixtures/README.md`. A diff that changes fixture VALUES without a capture behind it is
a defect.

**Weakening `REVIEW.das` (beside this file) is a defect.** The gate machine-checks the
mechanical half of "never otherwise written or edited by hand"; each check's finding text
states what it enforces.

**A route added or removed, or its path, method, or a request field the server reads changed,
updates every surface that documents or consumes it in the same change**: the `README.md`
route row, the endpoint table in `doc/source/reference/utils/dasllama_server.rst` (repo root),
and `control.html` where the page consumes it.

**A response wire key added, removed, or renamed re-captures its fixtures under
`tests/fixtures/` in the same change and lands its `README.md` route-row mention** — the
fixtures are the recorded wire shape, and the row is where a consumer learns the key exists.
