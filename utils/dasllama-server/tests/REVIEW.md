# dasllama-server control page tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `utils/dasllama-server/README.md` (repo root), `fixtures/README.md`.

**A captured response body under `fixtures/` (beside this file), whatever its extension -
`sse_expected.json` and `capture_catalog.tune.json` are derived, not captured - is never
hand-authored, and never edited beyond machine-local path normalization; re-capture it
instead.** `fixtures/README.md` gives the capture procedure and the path normalization.

**A request or response body in a Playwright `.spec.js` in this folder is a captured fixture,
or a clone of one with named fields overridden - a body authored from nothing is a defect.**

**A Playwright `.spec.js` in this folder never spells out a value the fixture carries - it
reads that value from the fixture; only page copy (a button label, a note, a heading) is
asserted literally.** A spelled-out fixture value is a second copy of the capture, and the next
capture moves it.

**A Playwright `.spec.js` in this folder never hard-codes a number a re-capture can move -
including one derived from a capture's length; it computes the number from the fixture.** A
capture carries the values of the machine it was taken on, so a re-capture elsewhere moves
them, and a spec that pins one goes red for a reason that has nothing to do with the page.
