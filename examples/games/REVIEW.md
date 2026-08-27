# examples/games Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**

**Weakening the card-wiring check in `REVIEW.das` (beside this file) is a defect.**

**A diff that adds a step to the deploy workflow naming the games it deploys writes that
list as a `for g in <ids>; do` loop.** The check beside this file reads the deployed game
list from those loops, so a list spelled any other way is one nothing cross-checks.
