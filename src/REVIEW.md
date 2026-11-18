# src Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`.

**Subfolders of this folder carry their own `REVIEW.md` checklists.**

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

**A diff after which the compile rejects source it accepted before, or leaves a different AST
for unchanged source - a node, name, type or flag any compile pass adds, removes or changes -
bumps `AstSerializer::getVersion()` (`include/daScript/ast/ast_serializer.h`, repo root) in the
same change.** A cached module skips the compile, so without the bump a rebuilt
`daslang` serves what the old compile accepted or produced.

**A diff under this folder that adds an object with static storage duration that code writes
outside its initializer or a global constructor, or adds or changes code that reads or writes
such an object, makes it a `thread_local`, a field reached through `daScriptEnvironment`, a
lock, an atomic, or an object read and written only under one named lock
(`ARCHITECTURE.md#static-storage-homes`).**

**A diff that changes which text the grammar's `require_module`, `require_module_name` or
`optional_require_guard` productions (`parser/ds2_parser.ypp`) accept as a `require`, or which
text the source-text collector `getAllRequireReq` (`ast/ast_parse.cpp`) reads as one, makes both
accept the same text, in the same change (`ARCHITECTURE.md#require-sites`).**

**A diff that changes how a require guard is evaluated - the `?m` in `require ?m x`, or the guard
a member of a `require [group]` carries - makes the same change in all three evaluators, in the
same change: the collector `getAllRequireReq` (`ast/ast_parse.cpp`), the parser's
`ast_requireGuardAvailable` (`parser/parser_impl.cpp`), and the member walk
`moduleGroupMemberAvailable` (`builtin/module_builtin_rtti.cpp`)
(`ARCHITECTURE.md#require-sites`).**

**A diff that adds or changes an `addExtern*` call under `builtin/`, or the signature of a
function such a call binds, wherever it is declared, states in the PR description that
`builtin/REVIEW.das` ran on a binary built from the diff** - the bind-flavor scan in
`builtin/review_nttp.das` reads the binds compiled into the running binary, so an older binary
passes binds the diff broke.
