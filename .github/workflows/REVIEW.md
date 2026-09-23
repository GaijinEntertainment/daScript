# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`skills/internal/preflight.md` (repo root).

A per-PR check is a step, a matrix cell, or a workflow trigger whose failure means the tree or a
published artifact is wrong, and that turns the lane red before a human merges or ships it - a
`pull_request` lane's step and a branch-push lane's smoke alike. A provisioning step only
prepares the runner and checks nothing - a checkout, a toolchain install, a version pin - and is
not itself a per-PR check.

**A diff that weakens a per-PR check is a defect: deleting it, or a step it depends on, while
no per-PR lane still runs its cases, stopping its failure from failing the lane
(`continue-on-error`, a trailing `|| true`, a swallowed exit code), shrinking what it checks, or, on a
`pull_request` lane, narrowing its condition to anything but a `matrix.role` condition that
still runs it on every pull request or the nightly-cron condition (`github.event_name ==
'schedule' || github.event_name == 'workflow_dispatch'`).**

**A per-PR check the diff adds fails the lane when it finds a defect.**

**A workflow the diff adds declares `timeout-minutes` on every job, a `concurrency` group, and
`permissions` naming only the scopes its own steps use.** A job with no timeout holds its
runners until GitHub's six-hour ceiling on one hung step.

**A workflow the diff adds or deletes, or whose trigger or matrix the diff adds, changes, or
removes, adds, corrects, or deletes in the same change its row in sec."What CI runs (per-PR +
nightly)" of `skills/internal/preflight.md` (repo root) - naming every event that triggers it,
each with its path filter when it has one, and what the lane runs - and its section there,
naming its local mirror or saying it has none. Its section is the heading beginning
`## <workflow>.yml`; a workflow with one such heading per job answers in the section of each job
the diff changes, and a trigger change corrects every section that names the trigger.** A lane
the table does not list, or lists wrong, is one nobody mirrors before a push.

**A step the diff adds, renames, or deletes, other than a provisioning step, adds, corrects, or
deletes its name, in the same change, in the section of `skills/internal/preflight.md` (repo
root) for the workflow that runs it - adding that section when the workflow has none, and using
the section for the job that runs it when the workflow carries one per job; when the step has a
local mirror, the same change also adds, corrects, or deletes its row in that section's mirror
table, adding the table when the section has none.**

**A per-PR check leaves the per-PR path only to the nightly cron (`github.event_name ==
'schedule' || github.event_name == 'workflow_dispatch'`), and the diff either names the
preflight gate - a check `preflight` runs locally before a push - that keeps it per PR
(`skills/internal/preflight.md` sec."extended_checks.yml") or states the platform no per-PR
cell has.** A job on a `pull_request` lane fits 35 minutes; what does not fit moves.

**A diff that adds or changes a per-PR check, or adds, changes, or removes a step a per-PR
check depends on, states in its PR body or commit message a run of that check's command on one
of the lane's platforms, naming which; an author with none of the lane's platforms states that
in the PR body instead, with the URL of the lane's green run on the PR's head commit. A green
run of that lane on the PR's head commit covers the lane's other platforms.** A check that fails
for a non-defect turns a green branch red for everyone.

**A step in `pages.yml` that names more than one id under `examples/games/` spells them as a
`for g in <ids>; do` loop, never inline.** `examples/games/REVIEW.das` (repo root) reads the
deployed list from those loops; an inline list beside a surviving loop is one nothing
cross-checks. An example outside `examples/games/` never goes in that loop - the step names it on
its own.
