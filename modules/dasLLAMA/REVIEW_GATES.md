# dasLLAMA Gate Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Planned work: `followup_general.md`.

**An edit to a `REVIEW.das` under this folder never weakens a check: drops it while code it
guards remains in the tree and no lint takes it over, narrows what it walks (a file or folder it
stops reading), edits the fixed value a check compares against - a hash, a count or a list -
where the finding text does not tell the author to, or leaves it passing code its finding text
still names as a defect - fix the flagged code instead.**

**A finding text names what failed; a diff that rewords one keeps it naming what failed.**

**A new check in any `REVIEW.das` under this folder, or a check whose licensed set - the names
it does not flag - gains a name, names in its finding text the `ARCHITECTURE_*.md#<anchor>`
section carrying the charter of the feature it guards (the section that says which file owns the
feature) - never `ARCHITECTURE.md` - and ships its line in that section in the same change. The
line names the check, the names it licenses, and the property those names share, or says the
check licenses no names. A name whose shared property the finding text does not name stays out -
fix the flagged code instead.**
