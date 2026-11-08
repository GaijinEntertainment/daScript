# dasLLAMA Gate Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md` and its `ARCHITECTURE_*.md` companions. Planned work:
`followup_general.md`.

**An edit to a `REVIEW.das` under this folder never drops a check while code it guards remains in
the tree and no lint takes it over - fix the flagged code instead.**

**An edit to a `REVIEW.das` under this folder never narrows what a check walks (a file or folder
it stops reading) - fix the flagged code instead.**

**An edit to a `REVIEW.das` under this folder changes a literal a check compares the tree
against only when that check's finding text says to, and each condition the text sets for the
change holds - shown by the change the text names, in the same diff, or by a PR-body claim with
its evidence; otherwise fix the flagged code.**

**An edit to a `REVIEW.das` under this folder never leaves a check passing code its finding text
still names as a defect - fix the flagged code instead.**

**A finding text names what failed; a diff that rewords one keeps it naming what failed.**

**A diff that adds a finding condition to a `REVIEW.das` under this folder, or adds or removes a
name in a check's licensed set - the names the check lets pass, or the closed set of roles or kinds
it makes every file take one of - makes every finding text that condition or check emits name the
`ARCHITECTURE_*.md#<anchor>` section saying which file owns the feature it guards, never
`ARCHITECTURE.md`; that section gets, in the same change, the check's line: the check, the names it
licenses and the property they share, or that it licenses none.**

**A diff that creates or extends a check's licensed set adds a name only when the finding text
names the property the licensed names share - one a reviewer can test a name against without
consulting the set or its ledger - and the new name has it; otherwise fix the flagged code.** A
licensed set is the names a check lets pass, or the closed set of roles or kinds it makes every
file take one of; for a closed set, the property says what any one role or kind is.
