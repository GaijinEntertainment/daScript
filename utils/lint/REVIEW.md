# lint Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff that shrinks the set of rule ids `REVIEW.das` (beside this file) scans is a
defect** - by editing the gate or deleting an id's last `"<ID>:` (quote, id, colon; comments
count) from the files in the gate's `RULE_MODULES`. The gate checks each scanned id for a
fixture and a `doc/source/reference/language/lint.rst` (repo root) section.

**A diff that makes a file emit a rule id the gate `REVIEW.das` (beside this file) does not
scan adds that file to `RULE_MODULES` in the gate, in the same change.** A file emits an id
when its own code prints it; a fixture asserting on that text does not emit.
