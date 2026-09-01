# web (the WASM build and its shells) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** A file
under this folder is served when the deploy (`.github/workflows/pages.yml`, repo root),
`daspkg release --web`, or a build step that feeds either copies it into a page a visitor
loads.

**A playground UI file - an `examples/ui/src` script or stylesheet, or an
`examples/ui/samples` bundle - answers to `examples/ui/REVIEW.md`, wherever the diff puts
it.**

**A diff that makes either `REVIEW.das` check read fewer files or report less than it did
before the change, with the same `ALLOWED_HOSTS` and `DEPLOYED_ROOTS`, is a defect** - the
cookie-and-host check bans `document.cookie` and reports any resource the files under
`DEPLOYED_ROOTS` load from a host `ALLOWED_HOSTS` does not name; the staged-tree check
reports a tree a staging step copies into a served page that `DEPLOYED_ROOTS` does not
cover.

**A diff that stages a tree under this folder into a served page by any step other than a
`cp` into `_site/` in `.github/workflows/pages.yml` (repo root) or a `-E copy` /
`copy_directory` into a `SITE_*` or `site/` destination in `CMakeLists.txt` (this folder)
also makes `REVIEW.das` parse that step's file, in the same change.** Those two shapes it
parses already, and fails on a tree they stage that `DEPLOYED_ROOTS` does not cover.

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
visitor sends that host and whether the host sets cookies.**
