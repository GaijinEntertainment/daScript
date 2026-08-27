# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`.

**A diff to a file that requires a `dasllama/*` module applies `modules/dasLLAMA/REVIEW.md`
(repo root) too.**

**Never write or edit a fixture under `tests/fixtures/` by hand - capture the bytes of a real
server run and normalize the machine-local paths instead.** `tests/fixtures/README.md` gives
the capture steps.

**Weakening `REVIEW.das` (beside this file) is a defect** - dropping a check, narrowing what a
check scans, adding a name to a check's licensed set (the names that check does not flag), or
changing a finding text so it no longer names what failed. What the gate enforces is read from
the gate itself.

**A diff that adds or removes a route, or changes its path, method, or a request field the
server reads, updates every surface that documents or uses that route, in the same change**:
the `README.md` route row, the endpoint table in
`doc/source/reference/utils/dasllama_server.rst` (repo root), and `control.html` where the
page uses it.

**A diff that adds, removes, or renames a key in a response body re-captures that route's
fixtures under `tests/fixtures/` and updates that route's row in `README.md` (beside this
file) - a new or renamed key is listed there, a removed key is struck from it - in the same
change** - the fixtures are the recorded response shape, and the row is where a consumer learns
the key exists.
