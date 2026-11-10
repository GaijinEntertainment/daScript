# src Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `misc/ARCHITECTURE_DAS_ROOT.md`.

Subfolders of this folder carry their own `REVIEW.md` checklists - a diff applies the
`REVIEW.md` of every folder between this one and each file it changes.

**A diff whose C++ under this folder, in code that compiles on Windows, opens, stats, lists,
renames or removes a file or directory whose path is not a string literal does it through the
UTF-8 file API in `include/daScript/misc/sysos.h`, never through a C runtime or OS call, which
on Windows reads a `char *` path in the ANSI codepage.**

**A diff that adds or changes a `NARROW_PATH_FILES` row of `REVIEW.das` (beside this file) names
in its `why` the platforms that compile the file's narrow calls - C runtime or OS calls that take
a `char *` file path - and a row whose `why` lists Windows among the compiling platforms is a
defect: those calls move to the UTF-8 file API in `include/daScript/misc/sysos.h` instead.**

**A diff that changes `REVIEW.das` (beside this file) keeps it reporting every narrow call - a C
runtime or OS call that takes a `char *` file path - it reported before in each file the changed
`REVIEW.das` leaves without a `NARROW_PATH_FILES` row, each finding naming the call and the API
to use.**

**A diff that makes the compile reject source it accepted before - a new or tightened compile
error does - or changes the AST the compile produces from unchanged source, bumps
`AstSerializer::getVersion()` (`include/daScript/ast/ast_serializer.h`, repo root) in the same
change.** A cached module never runs through the compile again, and the cache key misses a
rebuild of the runtime library alone (a `-module-cache <path>` file records no binary at all),
so without the bump a rebuilt `daslang` serves modules the old compile accepted or produced.
