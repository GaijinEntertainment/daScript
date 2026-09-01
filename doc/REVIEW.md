# doc (the Sphinx manual) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** RST
mechanics are `skills/internal/documentation_rst.md` (repo root) and the prose itself is
`skills/internal/tutorial_prose.md` (repo root); how a page reads is reviewed there, not
here.

**A diff that makes `REVIEW.das`'s cookie-and-host check read fewer files, or report less
than it did before the change with the same `ALLOWED_HOSTS`, is a defect** - the gate bans
`document.cookie` and reports any resource loaded from a host the list does not name, both
in what `source/` serves and in the absolute URLs `source/conf.py` declares.

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
reader sends that host and whether the host sets cookies.**
