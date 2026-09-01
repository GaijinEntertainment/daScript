# web (the WASM build and its shells) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** A file
under this folder is served when the deploy (`.github/workflows/pages.yml`, repo root) or
`daspkg release --web` copies it into a page a visitor loads.

**A playground UI file - an `examples/ui/src` script or stylesheet, or an
`examples/ui/samples` bundle - answers to `examples/ui/REVIEW.md`, wherever the diff puts
it.**

**A diff that makes `REVIEW.das`'s cookie-and-host check read fewer files, or report less
than it does today with the same `ALLOWED_HOSTS`, is a defect** - the gate bans
`document.cookie` and reports any resource the files under its root list load from a host
the list does not name.

**A diff that stages a tree under this folder into a served page by a route other than
`.github/workflows/pages.yml` (repo root) or `CMakeLists.txt` (this folder) also teaches
`REVIEW.das` to read that route, in the same change.** Those two routes it reads already: a
tree they stage that no root-list entry covers is a red gate.

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
visitor sends that host and whether the host sets cookies.**
