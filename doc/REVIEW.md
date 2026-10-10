# Doc Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `skills/internal/documentation_rst.md` (repo root). The prose itself is
`skills/internal/tutorial_prose.md` (repo root); how a page reads is reviewed there, not here.
A handmade stdlib stub - an `enumeration-`, `structure-`, `structure_annotation-`, `class-`,
`typedef-` or `function-` file, wherever the diff puts it - answers to
`source/stdlib/handmade/REVIEW.md` as well.

**Weakening `REVIEW.das` (beside this file) is a defect - a diff that makes it read fewer
files, or report less than it did before the change with the same `ALLOWED_HOSTS`, is weaker.**

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
reader sends that host and whether the host sets cookies.**

**A diff that adds to an authored `.rst` under `source`, or to a stub under
`source/stdlib/handmade`, anything a reader is told to fetch, run, or type that the file did not
carry before states in the PR body, for each such item, where it was checked to exist and work
at merge, the job of a release-tag workflow in `.github/workflows/` that creates and smoke-tests
it, or - for a name that exists only after a package-manager install - a `ci/` test (repo root)
that asserts the name in the package manifest that ships it** - the build proves the page
renders, never that what it points a reader at is there.

**A diff that changes a generator under `reflections/` (a file there, its `group_by_regex`
calls included) states, in the PR body, that every name the generator newly places on a page
resolves in the tree, and where that was checked** - a name the generator lists is a name a
reader will type.
