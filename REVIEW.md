# Repository Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`.

**Subfolders of this repository carry their own `REVIEW.md` checklists - a diff applies the
`REVIEW.md` of every folder between this one and each file it changes.**

**A file also answers, by kind, to the checklists the folder walk finds starting from a second
folder: a `.h` or `.inc` header under `include/daScript/<dir>/` from `src/<dir>/`; a `[test]`
file outside `modules/`, wherever the diff puts it, from `tests/`; a `.cpp` that includes doctest
from `tests-cpp/small/`.**

**A diff that changes the by-kind routing rule above changes `routed_paths` in
`utils/internal/review-md/scan.das` to match, in the same change.**

**A `.das` outside `modules/` that requires modules from `modules/<M>/`, or a `.cpp` outside
`modules/` that includes a header from `modules/<M>/`, and from no other `modules/` folder,
answers to `modules/<M>/REVIEW.md` too, when that file exists - the reviewer applies this route,
because the review walk does not.**

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check it runs, narrowing
what a check walks, or rewording a finding so it no longer names what failed.**

**A row of `REVIEW.das`'s `TOLERATED_SHARED_TWINS` table names in its `why` either the test that
compiles the pair on purpose or what keeps every process from compiling two of the files that
row lists; a `why` that names nothing, or that a build or test run contradicts, is a defect.** A
process that compiles two files declaring one `shared` module name gives the second file the first
file's module.

**A diff that adds or changes syntax in `tree-sitter-daslang/grammar.js` also adds a case for that
syntax to `tree-sitter-daslang/test/corpus/`, in the same change.**

**A diff that adds syntax to `tree-sitter-daslang/grammar.js` also adds a section exercising it to
`modules/dasImgui/tests/test_grammar_canary.das`, in the same change.**
