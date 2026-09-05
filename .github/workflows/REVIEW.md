# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

**A diff that weakens a per-PR gate step - a workflow step that runs on `pull_request` and
fails the lane when it finds a defect - is a defect: deleting it, stopping its failure failing
the lane (`continue-on-error`, a trailing `|| true`, a swallowed exit code), narrowing its
`if:` to anything but the nightly cron condition, or shrinking what it checks.**

**A per-PR gate step the diff adds or changes makes its failure the lane's failure - no
`continue-on-error`, no trailing `|| true`, no swallowed exit code.**

**A per-PR gate step whose job runs past 35 minutes moves to the nightly cron
(`github.event_name == 'schedule' || github.event_name == 'workflow_dispatch'`), never out of
the workflow, and the diff names the preflight gate that keeps its check per PR
(`skills/internal/preflight.md` sec."extended_checks.yml"); a step with no such gate stays per
PR.**

**Weakening `ci/test_ci_matrix.py` - dropping its role-condition assertion or its per-event
matrix-cell assertions - is a defect.** The nightly `extended_checks` job runs with role `all`
and is the only run of the steps too slow for a PR, so a step conditioned on its own role
would run nowhere in full; that test is what keeps every condition `matrix.role != '<other>'`.

**A diff that adds or changes a per-PR gate step states a run of the command the diff adds or
changes, on the lane's platform, in its PR body or commit message; a green run of that lane on
the PR's head commit is that evidence.** A step that fails for a non-defect turns a green
branch red for everyone.

**A diff that adds or changes a step in `pages.yml` that names the deployed games writes
that list as a `for g in <ids>; do` loop.** `examples/games/REVIEW.das` (repo root) reads
the deployed game list from those loops, so a list spelled any other way is one nothing
cross-checks.
