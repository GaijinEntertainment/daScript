# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `CONTROL_PAGE_PLAN.md`.

**A diff to a file that requires a `dasllama/*` module applies `modules/dasLLAMA/REVIEW.md`
(repo root) too.**

**Never hand-write or hand-edit a fixture under `tests/fixtures/`, or a wire body in a spec
under `tests/` - capture the bytes of a real server run and normalize the machine-local paths
instead.** `tests/fixtures/README.md` gives the capture steps.

**Weakening `REVIEW.das` (beside this file) is a defect** - dropping a check, narrowing what a
check scans, adding a name to a check's licensed set (the names that check does not flag), or
changing a finding text so it no longer names what failed. What the gate enforces is read from
the gate itself.

**A diff that adds a route, or changes a request field the server reads, updates
`control.html` where the page uses it, in the same change.**

**A diff that makes `control.html` or a spec under `tests/` read a response key that no
fixture under `tests/fixtures/` carries is a defect - re-capture the fixture and list the key
in that route's `README.md` row, in the same change.** The fixtures are the recorded response
shape, and the row is where a consumer learns the key exists.
