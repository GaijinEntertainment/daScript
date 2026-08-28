# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

**A diff that deletes a per-PR gate step - a workflow step that runs on `pull_request` and
fails the lane when it finds a defect - adds `continue-on-error` to one, narrows its `if:`,
weakens its command, or shrinks the set of tracked source files it checks is a defect.** Each
such step enforces its rule automatically, with no reviewer involved -
`skills/internal/preflight.md` sec."doc.yml - the gates", sec."extended_checks.yml".

**A diff that adds or changes a per-PR gate step - a workflow step that runs on
`pull_request` and fails the lane when it finds a defect - makes that step fail on every
defect in what it checks, and on nothing else.**

**A diff that adds a step to the deploy workflow (`pages.yml`) naming the games it deploys
writes that list as a `for g in <ids>; do` loop.** `examples/games/REVIEW.das` (repo root)
reads the deployed game list from those loops, so a list spelled any other way is one
nothing cross-checks.
