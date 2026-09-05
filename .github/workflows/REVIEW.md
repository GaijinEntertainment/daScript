# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root).

**A diff that weakens a per-PR check - a step, a matrix cell, or a workflow's `pull_request`
trigger that runs on every pull request and fails the lane when it finds a defect, whether the
diff finds it or adds it - is a defect: deleting it, stopping its failure from failing the lane
(`continue-on-error`, a trailing `|| true`, a swallowed exit code), shrinking what it checks,
or narrowing its condition to anything but a role that still runs it on every pull request or
the nightly cron condition of the next rule.**

**A per-PR check leaves the per-PR path only to the nightly cron (`github.event_name ==
'schedule' || github.event_name == 'workflow_dispatch'`), and the diff either names the
preflight gate - a check `preflight` runs locally before a push - that keeps it per PR
(`skills/internal/preflight.md` sec."extended_checks.yml") or states the platform no per-PR
cell has, the one reason no gate can.** A per-PR job fits 35 minutes; what does not fit moves,
and preflight is where the check keeps running per PR.

**A diff that adds or changes a per-PR check states a run of the command the diff adds or
changes, on the lane's platform, in its PR body or commit message; a green run of that lane on
the PR's head commit is that evidence.** A step that fails for a non-defect turns a green
branch red for everyone.

**A step in `pages.yml` that names the deployed games spells the list as a `for g in <ids>; do`
loop, never inline.** `examples/games/REVIEW.das` (repo root) reads the deployed list from
those loops and cannot see one spelled any other way.
