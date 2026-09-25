# src - shared front-end contracts

Contract: `ARCHITECTURE_COMMON.md` (repo root).

## 1. Require resolution across compiler phases {#require-resolution}

`getAllRequireReq` reads a source file before the parser so the prerequisite
walk can compile every module the file will need. The collector and the parser
therefore accept the same `require` syntax. They also make the same guard
decision: an empty guard is available, a path guard resolves through the
compile's `FileAccess` relative to the file being compiled, and a module guard
uses `guardModuleAvailable`, which includes a dynamic module waiting in a
manifest row.

A module group expands through `getModuleGroupMembers` in the collector and
the parser. Both apply the group's guard and each member's guard. The
`module_group_for_each_member` runtime binding applies the same member filter
when `daslib/module_group` emits the calls, so the modules compiled for a group
and the modules called through it are the same set.
