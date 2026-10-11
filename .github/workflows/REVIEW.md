# Workflows Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`skills/internal/preflight.md` (repo root).

A per-PR lane is a workflow job that runs - directly or through a workflow that calls it - on every
pull request, or every push to a branch other than `master`, that its path filter matches, when it
has one; a job that runs on a pull request only when the author opts in - a label, a tag in the PR
body - is not one. A per-PR check is a step or a matrix cell that a per-PR lane runs on such a pull
request or push and that fails the lane when the tree or a published artifact is wrong; the job's
trigger and the job-level keys that decide whether, where, and how long it runs - `if:`, `needs:`,
`runs-on`, `timeout-minutes` - are part of every check the job runs. A provisioning step only
prepares the runner and checks nothing - a checkout, a toolchain install, a version pin - and is not
itself a per-PR check.

**A diff that weakens a per-PR check is a defect: deleting it, or a step it depends on, while
no per-PR lane still runs its cases, stopping its failure from failing the lane
(`continue-on-error`, a trailing `|| true`, a swallowed exit code), shrinking what it checks, or, on a
`pull_request` lane, narrowing its condition to anything but a `matrix.role` condition that
still runs it on every pull request or the nightly-cron condition (`github.event_name ==
'schedule' || github.event_name == 'workflow_dispatch'`).**

**A per-PR check the diff adds fails the lane when it finds a defect.**

**A workflow the diff adds declares a `concurrency` group and `permissions` naming only the
scopes its own steps use.**

**A diff that makes `REVIEW.das` (beside this file) read fewer workflow files or jobs, report
fewer cases than before, or word a finding so it no longer names what failed is a defect.**

**A workflow the diff adds or deletes, or whose trigger or matrix the diff adds, changes, or
removes, adds, corrects, or deletes in the same change its row in sec."What CI runs (per-PR +
nightly)" of `skills/internal/preflight.md` (repo root) - naming every event that triggers it,
each with its path filter when it has one, and what the lane runs - and its section there,
naming its local mirror or saying it has none. Its section is the heading beginning
`## <workflow>.yml`; a workflow with one such heading per job answers in the section of each job
the diff changes, and a trigger change corrects every section that names the trigger.** A lane
the table does not list, or lists wrong, is one nobody mirrors before a push.

**A step the diff adds, renames, or deletes, other than a provisioning step, adds, corrects, or
deletes its entry, in the same change, in the section of `skills/internal/preflight.md` (repo
root) for the workflow that runs it - the job's section when the workflow carries one per job, a
new section when it has none. A job the diff adds is entered once, by its job id, and that entry
covers its steps; a step added, renamed, or deleted in an existing job is entered by its
`name:`.**

**A step with a local mirror that the diff adds, renames, or deletes adds, corrects, or deletes
its row in that section's mirror table, in the same change, adding the table when the section
has none.**

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
