# Doc Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** A diff to an
`.rst` under `source` is also reviewed with `skills/internal/documentation_rst.md` (repo root)
for RST mechanics and with `skills/internal/tutorial_prose.md` (repo root) for how the page
reads. A handmade stdlib stub - an `.rst` named `<kind>-<module>-...` whose text
`reflections/das2rst.das` splices into a generated stdlib page - answers to
`source/stdlib/handmade/REVIEW.md` as well, wherever the diff puts it.

**Weakening `REVIEW.das` (beside this file) is a defect - a diff that makes it read fewer
files, or report less than it did before the change with the same `ALLOWED_HOSTS`, is weaker.**

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
reader sends that host and whether the host sets cookies.**

**A diff that adds to an `.rst` under `source` that no `reflections/` generator writes a URL,
download, command, or third-party package or library name the tree does not ship, which a reader
is told to fetch, run, or type, names for each, in the PR body, one of: where it was checked to
exist and work at merge; the job of a release-tag workflow in `.github/workflows/` that creates
and smoke-tests it; or, for a name that exists only after a package-manager install, the `ci/`
test (repo root) that asserts the name in the package manifest that ships it.** The build proves
the page renders, never that what it points a reader at is there.

**A diff that adds to an `.rst` under `source` that no `reflections/` generator writes a name,
syntax, option, or command the tree ships, shown for a reader to use, names in the PR body the
test that runs it.** The build proves the page renders, never that what it shows works.

**A diff that changes a file under `reflections/` states, in the PR body, that every name the
change newly places on a page resolves in the tree, and where that was checked** - a name the
generator lists is a name a reader will type.
