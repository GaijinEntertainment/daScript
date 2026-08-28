# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

**A diff that deletes a per-PR gate step - a workflow step that runs on `pull_request` and
fails the lane when it finds a defect - adds `continue-on-error` to one, narrows its `if:`,
weakens its command, or shrinks the set of tracked source files it checks is a defect.** Each
such step enforces its rule automatically, with no reviewer involved -
`skills/internal/preflight.md` sec."doc.yml - the gates", sec."extended_checks.yml".

**A diff that adds or changes a per-PR gate step - a workflow step that runs on
`pull_request` and fails the lane when it finds a defect - makes that step red the lane
whenever the artifact the step names fails.**

**A diff that adds or changes a per-PR gate step states a run of that step's command on the
lane's platform in its PR body or commit message - or the PR's own green lane run stands as
that evidence.** A step that fails for a non-defect turns a green branch red for everyone.

**A diff that adds or changes a step in `pages.yml` that names the deployed games writes
that list as a `for g in <ids>; do` loop.** `examples/games/REVIEW.das` (repo root) reads
the deployed game list from those loops, so a list spelled any other way is one nothing
cross-checks.
