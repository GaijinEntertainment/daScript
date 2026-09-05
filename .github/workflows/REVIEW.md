# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

A per-PR gate step is a workflow step that runs on `pull_request` and fails the lane when it
finds a defect; each one enforces its rule automatically, with no reviewer involved
(`skills/internal/preflight.md` sec."doc.yml - the gates", sec."extended_checks.yml").

**A diff that weakens a per-PR gate step - deletes it, stops its failure failing the lane
(`continue-on-error`, a trailing `|| true`, a swallowed exit code, a narrowed `if:`), or
shrinks what it checks - is a defect; a step the diff adds or changes makes its failure the
lane's failure.**

**A diff that adds or changes a per-PR gate step states a run of the command the diff adds or
changes, on the lane's platform, in its PR body or commit message; a green run of that lane on
the PR's head commit is that evidence.** A step that fails for a non-defect turns a green
branch red for everyone.

**A diff that adds or changes a step in `pages.yml` that names the deployed games writes
that list as a `for g in <ids>; do` loop.** `examples/games/REVIEW.das` (repo root) reads
the deployed game list from those loops, so a list spelled any other way is one nothing
cross-checks.
