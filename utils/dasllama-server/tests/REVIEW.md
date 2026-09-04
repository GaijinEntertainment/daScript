# dasllama-server control page tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `utils/dasllama-server/README.md` (repo root).

**A captured response body - a `.json`/`.txt` under `fixtures/` (beside this file) - is never
hand-authored, and never edited beyond machine-local path normalization.**
`fixtures/README.md` gives the capture procedure.

**A request or response body in a Playwright `.spec.js` in this folder is a captured fixture,
or a clone of one with named fields overridden - a body authored from nothing is a defect.**

**A Playwright `.spec.js` in this folder never spells out a value a fixture carries, and never
hard-codes a number a re-capture can move - including one derived from a capture's length. It
reads the value, and computes the number, from the fixture.** A capture carries the values of
the machine it was taken on, so a re-capture elsewhere moves them, and a spec that pins one
goes red for a reason that has nothing to do with the page.
