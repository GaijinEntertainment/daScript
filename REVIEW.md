# Repository Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`.

**Removing `REVIEW.das`'s check that no two git-tracked `.das` files declare the same `shared`
module name is a defect.**

**A diff that lets two git-tracked `.das` files declare the same `shared` module name adds
`TOLERATED_SHARED_TWINS` rows naming every file that may declare it, and states in those rows why
no process compiles two of them by accident.** The first file a process compiles that declares `module X shared` becomes that process's module
`X`, and every later file declaring or requiring `X` in the same process gets that module - so any
process compiling both gives the second file the first's module.
