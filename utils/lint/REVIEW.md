# lint Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**lint ships as a prebuilt exe: it stays in `DAS_UTILS` and `DAS_UTILS_SHIPPED_EXES`
(`utils/CMakeLists.txt`)** - dropping either entry breaks the release bundle.

**The set of rule ids `REVIEW.das` (beside this file) discovers never shrinks** - the gate
checks that every rule id emitted by a lint module has a fixture and a section in
`doc/source/reference/language/lint.rst` (repo root). A change that drops an id from the
scan - an edit to the gate, or a source change that removes the id's only scannable
spelling - silently retires that id's fixture-and-rst check.
