# dasLLAMA Gate Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Planned work: `followup_general.md`.

**An edit to a `REVIEW.das` under this folder never weakens a check: drops it while code it
guards remains in the tree and no lint takes it over, narrows what it walks (a file or folder it
stops reading), edits the fixed value a check compares against - a hash, a count or a list -
where the finding text does not tell the author to or the condition it states for doing so
does not hold, or leaves it passing code its finding text
still names as a defect - fix the flagged code instead.**

**A finding text names what failed; a diff that rewords one keeps it naming what failed.**

**A diff that adds a finding condition to a `REVIEW.das` under this folder - in a new gate
function or an existing one - or a name to the licensed set of a check (the names the check
does not flag), names, in every finding text that condition or check emits, the
`ARCHITECTURE_*.md#<anchor>` section carrying the charter of the feature it guards (the section
that says which file owns the feature), never `ARCHITECTURE.md`.**

**A diff that adds a finding condition to a `REVIEW.das` under this folder - in a new gate
function or an existing one - or a name to a check's licensed set, ships in the section carrying
the charter of the feature the check guards, in the same change, the check's line: the check,
the names it licenses, and the property those names share, or that the check licenses no
names.**

**A diff that creates or extends a check's licensed set adds a name only when the finding text
names the property the licensed names share - one a reviewer can test a name against without
consulting the set or its ledger - and the new name has it; otherwise fix the flagged code.**
