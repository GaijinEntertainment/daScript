# Repository Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`.

**Subfolders of this repository carry their own `REVIEW.md` checklists - a diff applies the
`REVIEW.md` of every folder between this one and each file it changes.**

**A file also answers, by kind, to the checklists the folder walk finds starting from a second
folder: a `.h` or `.inc` header under `include/daScript/<dir>/` from `src/<dir>/`; a `[test]`
file outside `modules/`, wherever the diff puts it, from `tests/`; a `.cpp` that includes doctest
from `tests-cpp/small/`.** A diff that changes this list changes `routed_paths` in
`utils/internal/review-md/scan.das` to match, in the same change.

**A `.das` outside `modules/` whose requires reach - directly or through the modules they
require - modules under exactly one `modules/<M>/` folder, or a `.cpp` outside `modules/` whose
includes reach - directly or through the headers they include - headers under exactly one
`modules/<M>/` folder, answers to `modules/<M>/REVIEW.md` too, when that file exists.**
`utils/internal/review-md/scan.das` does not apply this route, so the reviewer does.

**A guard on the `dasCUDA` package in a file outside `modules/dasCUDA/` tests for the `cuda`
module - the module `modules/dasCUDA/src/dasCUDA.cpp` registers, built only when the build
configures the package - through `require ?cuda <target>`, `has_module("cuda")` or
`builtin_module_exists(cuda)`, never through a `cuda/` path.** The `cuda/` files of the package
are on disk in a build configured without it, so a path guard reads that build as having CUDA.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check it runs, narrowing
what a check walks, or rewording a finding so it no longer names what failed.**

**A diff that adds or changes a `TOLERATED_SHARED_TWINS` row of `REVIEW.das` (beside this file)
names in its `why` the test compiling the files under its name together, or what keeps other
processes from compiling two of them.** Two files declaring one `shared` module name compiled
together share the first's module.

**A diff after which one process compiles two files that `TOLERATED_SHARED_TWINS` in `REVIEW.das`
(beside this file) lists under one name, and no `why` among that name's rows names that process,
is a defect - rename the module one of them declares.**
