# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

A per-PR gate step is a workflow step that runs on `pull_request` and fails the lane when it
finds a defect; each one enforces its rule automatically, with no reviewer involved
(`skills/internal/preflight.md` sec."doc.yml - the gates", sec."extended_checks.yml").

**A diff that weakens a per-PR gate step - deletes it, stops its failure failing the lane
(`continue-on-error`, a trailing `|| true`, a swallowed exit code, a narrowed `if:`), or
shrinks what it checks - is a defect; a step the diff adds or changes makes its failure the
lane's failure.** The one admitted narrowing is the budget's: a per-PR job fits 35 minutes
(`plans/ci_preflight_budget.md`), so a step that leaves the per-PR path moves to the nightly
cron (`github.event_name == 'schedule' || github.event_name == 'workflow_dispatch'`), never
out of the workflow, and the diff names the preflight gate that keeps its check per PR
(`skills/internal/preflight.md` sec."extended_checks.yml"); a step with no local mirror stays
per PR.

**A step in `extended_checks.yml` that runs in one per-PR role spells its condition
`matrix.role != '<the other role>'`, never `== '<its role>'`.** The nightly job runs with role
`all` and is the only run of the steps too slow for a PR; a step conditioned on its own role
would skip there and run nowhere in full. `ci/test_ci_matrix.py` reads the workflow and fails
any other spelling; the cells themselves are data in `ci/ci_matrix.py`, which the same test
pins per event.

**A diff that adds or changes a per-PR gate step states a run of the command the diff adds or
changes, on the lane's platform, in its PR body or commit message; a green run of that lane on
the PR's head commit is that evidence.** A step that fails for a non-defect turns a green
branch red for everyone.

**A diff that adds or changes a step in `pages.yml` that names the deployed games writes
that list as a `for g in <ids>; do` loop.** `examples/games/REVIEW.das` (repo root) reads
the deployed game list from those loops, so a list spelled any other way is one nothing
cross-checks.
