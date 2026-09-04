# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `CONTROL_PAGE_PLAN.md`.

**A diff to a file that requires - directly or through another file in this folder - a
`dasllama/*` module, or to a `README.md` row stating dasLLAMA engine behavior or a measured
number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff that changes a flag's row in `README.md` also updates that flag's `@clarg_doc` in
`main.das`, in the same change - `--help` is the other copy a user reads.**

**A captured body under `tests/fixtures/` is never hand-authored, and never edited beyond
machine-local path normalization - re-capture it instead.** `tests/fixtures/README.md` gives
the capture rails; a capture SCRIPT beside the fixtures is code, not a fixture.

**A body or SSE frame a Playwright `.spec.js` under `tests/` (beside this file) sends or
asserts is a fixture, or a clone of one with named fields overridden - one assembled from
scratch is a defect.**

**A diff that re-captures a fixture under `tests/fixtures/`, or edits `control.html`, runs
the Playwright suite in `tests/` (beside this file) and ships green.**

**Weakening `REVIEW.das` (beside this file) is a defect** - dropping a check, narrowing what a
check scans, adding a name to a check's licensed set (the names that check does not flag), or
changing a finding text so it no longer names what failed. What the gate enforces is read from
the gate itself.

**A diff that adds a route, or changes a request field the server reads, updates
`control.html` where the page uses it, in the same change.**

**A diff that changes what a route answers - an item, a field, or a value - re-captures every
fixture under `tests/fixtures/` that records that route, in the same change.** The fixtures are
the recorded response shape.

**A diff that makes `control.html` or a Playwright `.spec.js` under `tests/` (beside this file)
read a response key no fixture under `tests/fixtures/` carries is a defect - re-capture the
fixture for the route that answers with that key first.**

**A diff that adds a key to what a route answers lists it in that route's `README.md` row, in
the same change.** The row is where a consumer learns the key exists.

**A Playwright `.spec.js` under `tests/` (beside this file) never spells out a value a fixture
carries, and never hard-codes a number a re-capture can move - including one derived from a
capture's length. It reads the value, and computes the number, from the fixture.** A capture
carries the values of the machine it was taken on, so a re-capture elsewhere moves them, and a
spec that pins one goes red for a reason that has nothing to do with the page.
