# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**
Architecture doc: `skills/internal/preflight.md` (repo root). A per-PR check is a step, a
matrix cell, or a workflow trigger whose failure means the tree or a published artifact is
wrong, and that turns the lane red before a human merges or ships it - a `pull_request` lane's
step and a branch-push lane's smoke alike. A provisioning step - a toolchain install, a version
pin - is not itself a per-PR check.

**A diff that weakens a per-PR check is a defect: deleting it, or a step it depends on, while
no per-PR lane still runs its cases, stopping its failure from failing the lane
(`continue-on-error`, a trailing `|| true`, a swallowed exit code), shrinking what it checks, or
narrowing its condition to anything but a `matrix.role` condition that still runs it on every
pull request.**

**A per-PR check the diff adds fails the lane when it finds a defect.**

**A workflow the diff adds declares `timeout-minutes` on every job, a `concurrency` group, and
`permissions` naming only the scopes its own steps use.** A job with no timeout holds its
runners until GitHub's six-hour ceiling on one hung step.

**A workflow the diff adds, or whose trigger, matrix, or local mirror the diff changes, adds or
corrects its row in sec."What CI runs (per-PR + nightly)" of `skills/internal/preflight.md`
(repo root) in the same change: the row names its trigger and what the lane runs.** A lane the
table does not list, or lists wrong, is one nobody mirrors before a push.

**The same diff adds or corrects that workflow's own section there - the heading beginning
`## <workflow>.yml` - which names its local mirror or says it has none; a step the diff adds,
whatever role runs it, is named in that section's step list and gets a row in the section's
mirror table when it has a local mirror.** A workflow carrying one section per job
(`build.yml`) gets the section for the job the diff changes.

**A per-PR check leaves the per-PR path only to the nightly cron (`github.event_name ==
'schedule' || github.event_name == 'workflow_dispatch'`), and the diff either names the
preflight gate - a check `preflight` runs locally before a push - that keeps it per PR
(`skills/internal/preflight.md` sec."extended_checks.yml") or states the platform no per-PR
cell has.** A per-PR job fits 35 minutes; what does not fit moves.

**A diff that adds or changes a per-PR check, or adds, changes, or removes a step a per-PR
check depends on, states a run of that check's command on one of the lane's platforms, naming
which, in its PR body or commit message; a green run of that lane on the PR's head commit
covers the lane's other platforms.** A check that fails for a non-defect turns a green branch
red for everyone.

**A step in `pages.yml` that names more than one id under `examples/games/` spells them as a
`for g in <ids>; do` loop, never inline.** `examples/games/REVIEW.das` (repo root) reads the
deployed list from those loops; an inline list beside a surviving loop is one nothing
cross-checks. An example outside `examples/games/` (a graphics showcase, a dasLLAMA app) is not
on that list and is named on its own.
