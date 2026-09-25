# Source Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this
checklist.** Architecture doc: `ARCHITECTURE.md`.

- **A diff that changes the `require` syntax accepted by the parser changes
  `getAllRequireReq` (`ast/ast_parse.cpp`) to accept the same syntax, in the
  same change (`ARCHITECTURE.md` sec. 1).**

- **A diff that changes how require guard availability is decided keeps the
  collector's `guardAvailable` (`ast/ast_parse.cpp`), the parser's
  `ast_requireGuardAvailable` (`parser/parser_impl.cpp`), and
  `moduleGroupMemberAvailable` (`builtin/module_builtin_rtti.cpp`) consistent
  (`ARCHITECTURE.md` sec. 1).**

- **A diff that changes which members a module group expands to keeps
  `getAllRequireReq`, `ast_requireModuleGroup` (`parser/parser_impl.cpp`), and
  `rtti_module_group_for_each_member` (`builtin/module_builtin_rtti.cpp`) on
  the same member set, in the same change (`ARCHITECTURE.md` sec. 1).**
