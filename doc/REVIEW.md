# doc (the Sphinx manual) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/documentation_rst.md` (repo root). The prose itself is
`skills/internal/tutorial_prose.md` (repo root); how a page reads is reviewed there, not here.

**Weakening `REVIEW.das` (beside this file) is a defect - a diff that makes it read fewer
files, or report less than it did before the change with the same `ALLOWED_HOSTS`, is weaker.**

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
reader sends that host and whether the host sets cookies.**

**A diff that adds a download address, an asset name, or a command a reader runs to any text
that reaches a built page - an authored `.rst` under `source`, a stub under
`source/stdlib/handmade`, or page text a `reflections` generator writes - states, in the PR
body, that each exists and works at merge, and where that was checked** - the build proves the
page renders, never that what it points a reader at is there.
