# examples/games Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**

**Weakening the card-wiring check in `REVIEW.das` (beside this file) is a defect.**

**A `.das` under `web/examples/ui/samples/examples/<game>/` that differs from its
`examples/games/<game>/` twin, or has no twin, is a defect - the staged copy is the desktop
file verbatim.** The wasm playground runtime resolves the same `live/*` requires the desktop
uses, so the copy carries no port edits; the check beside this file compares every file.

**A diff that adds a step to the deploy workflow naming the games it deploys writes that
list as a `for g in <ids>; do` loop.** The check beside this file reads the deployed game
list from those loops, so a list spelled any other way is one nothing cross-checks.
