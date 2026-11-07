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
a `char *` file path. A file with a narrow call that compiles on Windows gets no row; that call
moves to the UTF-8 file API.**

**A diff that changes `REVIEW.das` (beside this file) anywhere but its `NARROW_PATH_FILES` rows
keeps it reporting every narrow call it reported before, each finding naming the call and the
API to use.**
