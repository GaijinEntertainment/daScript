# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root). A per-PR check is a step, a
matrix cell, or a workflow trigger whose failure turns a branch or a published artifact red
before a human merges or ships it - a `pull_request` lane's step and a branch-push lane's
smoke alike.

**A diff that weakens a per-PR check is a defect: deleting it while no per-PR lane still
runs its cases, stopping its failure from failing the lane (`continue-on-error`, a trailing
`|| true`, a swallowed exit code), shrinking what it checks, or narrowing its condition to
anything but a `matrix.role` condition that still runs it on every pull request or the
nightly cron.**

**A per-PR check the diff adds fails the lane when it finds a defect.**

**A workflow the diff adds declares `timeout-minutes` on every job, a `concurrency` group, and
`permissions` naming only the scopes its own steps use.** A job with no timeout holds its
runners until GitHub's six-hour ceiling on one hung step.

**A workflow the diff adds, or whose trigger, matrix, or local mirror the diff changes, gets
its row in sec."What CI runs (per-PR + nightly)" of `skills/internal/preflight.md` (repo root)
added or corrected in the same change - its trigger, and its local mirror or the words that it
has none.** A lane the table does not list, or lists wrong, is one nobody mirrors before a push.

**A per-PR check leaves the per-PR path only to the nightly cron (`github.event_name ==
'schedule' || github.event_name == 'workflow_dispatch'`), and the diff either names the
preflight gate - a check `preflight` runs locally before a push - that keeps it per PR
(`skills/internal/preflight.md` sec."extended_checks.yml") or states the platform no per-PR
cell has.** A per-PR job fits 35 minutes; what does not fit moves.

**A diff that adds or changes a per-PR check states a run of that check's command, on the
lane's platform, in its PR body or commit message; a green run of that lane on the PR's head
commit is that evidence.** A check that fails for a non-defect turns a green branch red for
everyone.

**A step in `pages.yml` that names more than one id under `examples/games/` spells them as a
`for g in <ids>; do` loop, never inline.** `examples/games/REVIEW.das` (repo root) reads the
deployed list from those loops; an inline list beside a surviving loop is one nothing
cross-checks. An example outside `examples/games/` (a graphics showcase, a dasLLAMA app) is not
on that list and is named on its own.
