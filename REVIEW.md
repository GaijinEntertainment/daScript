# Repository Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`.

**Subfolders of this repository carry their own `REVIEW.md` checklists - a diff applies the
`REVIEW.md` of every folder between this one and each file it changes.** A header under
`include/daScript/<dir>/` also answers to `src/<dir>/REVIEW.md`; a `[test]` file, wherever the
diff puts it, answers to `tests/REVIEW.md`; a doctest `.cpp` answers to
`tests-cpp/small/REVIEW.md`.

**Removing or weakening `REVIEW.das`'s check that no two git-tracked `.das` files declare the
same `shared` module name is a defect.**

**A `TOLERATED_SHARED_TWINS` row's `why` names either the test that compiles the pair on
purpose or what keeps every process from compiling two of its name's files; a `why` that names
nothing, or that a build or test run contradicts, is a defect.** The first file a process compiles that declares `module X shared` becomes that
process's module `X`, and every later file declaring or requiring `X` in the same process gets
that module - so any process compiling both gives the second file the first's module.
