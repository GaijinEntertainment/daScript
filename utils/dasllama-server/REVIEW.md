# dasllama-server Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. Planned work: `CONTROL_PAGE_PLAN.md`.

**A diff to a file that requires - directly or through another file in this folder - a
`dasllama/*` module, or to a `README.md` row stating dasLLAMA engine behavior or a measured
number, applies `modules/dasLLAMA/REVIEW.md` (repo root) too.**

**A diff that changes a flag's row in `README.md` also updates that flag's `@clarg_doc` in
`main.das`, in the same change - `--help` is the other copy a user reads.**

**A captured response body - a `.json`/`.txt` under `tests/fixtures/` - is never
hand-authored, and never edited beyond machine-local path normalization; a request or
response body in a Playwright `.spec.js` under `tests/` (beside this file) is a captured
fixture, or a clone of one with named fields overridden - a body authored from nothing is a
defect.** `tests/fixtures/README.md` gives the capture rails (a capture SCRIPT beside the
fixtures is code, not a fixture).

**A diff that re-captures a fixture under `tests/fixtures/`, or edits `control.html`, runs
the Playwright suite in `tests/` (beside this file) and ships green.**

**Weakening `REVIEW.das` (beside this file) is a defect** - dropping a check, narrowing what a
check scans, adding a name to a check's licensed set (the names that check does not flag), or
changing a finding text so it no longer names what failed. What the gate enforces is read from
the gate itself.

**A diff that adds a route, or changes a request field the server reads, updates
`control.html` where the page uses it, in the same change.**

**A diff that makes `control.html` or a Playwright `.spec.js` under `tests/` (beside this
file) read a response key no fixture under `tests/fixtures/` carries, or that changes the
set of items a route returns while the fixtures record the old set, is a defect - re-capture
the fixture and list a new key in that route's `README.md` row, in the same change.** The
fixtures are the recorded response shape, and the row is where a consumer learns the key
exists.

**A Playwright `.spec.js` under `tests/` (beside this file) never spells out a value the
fixture carries - it reads the value from the fixture.** A capture is box-shaped (the hardware
line, the detected GPU tier, model and tool paths, the models-dir listing), so a re-capture on
another box moves those values, and a spec that hard-codes one goes red for a reason that has
nothing to do with the page.
