# Repository Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. This checklist holds the rules whose trigger is any file in the tree.

**Removing `REVIEW.das`'s check that no two git-tracked `.das` files declare the same `shared`
module name, or letting a name past it without `TOLERATED_SHARED_TWINS` rows naming every file
that may declare it and stating why no process compiles two of them by accident, is a defect.**
The first file a process compiles that declares `module X shared` becomes that process's module
`X`, and every later file declaring or requiring `X` in the same process gets that module - so any
process compiling both gives the second file the first's module.
