# dasLLAMA Gate Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Planned work: `followup_general.md`.

**An edit to a `REVIEW.das` under this folder that drops a check while code it guards remains in
the tree and no lint takes the check over, rewrites a finding text so it no longer names what
failed, or leaves a check passing code its finding text still names as a defect is a defect - fix
the flagged code instead.** Narrowing what a check walks (a file or folder it stops reading), or
re-stamping a pinned hash, count or list the finding text does not tell the author to re-stamp,
leaves flagged code passing.

**A diff adds a name to a check's licensed set - the names that check does not flag - only when
the check's finding text names the property the licensed names share and the check's line in the
`ARCHITECTURE_*.md` section that finding text cites names that property; otherwise it fixes the
flagged code.**

**A new check in any `REVIEW.das` under this folder, or a check whose licensed set gains a name,
names in its finding text the `ARCHITECTURE_*.md#<anchor>` section carrying the charter of the
feature the check guards - never `ARCHITECTURE.md` - and ships its line in that section, in the
same change.** The line names the check and the names it licenses; when the check licenses no
names, the line says so.
