# src/builtin - extern bind flavors

Every C++ function a module registers becomes an interpreter call node. Two node
flavors exist, and the choice is per bind:

- **Shared per-signature node** (`addExtern`, `addExternEx`, `addExternTempRef`,
  `addExternProperty`, `addExternPropertyForType`): the node templates on the function
  SIGNATURE and carries the function pointer as a member, so every bind of one signature
  shares a single instantiation - this is what keeps module binaries small. The typed-eval
  matrix lives once per return carrier in `SimNode_ExtFuncCallRet`
  (`include/daScript/simulate/interop.h`), and a consumed extern costs one virtual call.
- **Per-function NTTP node** (`addExternInline`, `addExternInlineEx`): the function
  pointer is a template constant, so the callee can inline into the call node. Costs one
  node instantiation per function; buys direct-call/inlined dispatch. Functions bound this
  way carry `Function::moreFlags2.nttp`, which `daslib/only_nttp.das` and tooling key on.

The policy: **the modules `REVIEW.md` names bind NTTP; every other module binds
per-signature.** Those modules are on interpreter hot paths, their bind count is bounded,
and misjudgment is one-sided - a needlessly-NTTP cold function costs ~1KB of binary, a
missed hot one costs a few percent forever. The list lives in the rule and in the gate that
enforces it (`review_nttp.das`); repeating it here is how it goes stale. Exempt by
construction, because the NTTP entry points cannot express them: cmres binds (a
struct/array/tuple-shaped result the callee writes
into the caller's result slot - `SimNode_ExtFuncCallAndCopyOrMove`), ref-returning binds
(`SimNode_ExtFuncCallRef`), and interop binds (`addInterop`, raw argument array).

**Boundary case**: the generic container and equality helpers declared in
`include/daScript/ast/ast_handle.h` (repo root) - `das_vector_*`, `das_equ*`,
`das_nequ*`, `das_handle_equ*`, `das_handle_nequ*` - instantiate into whichever module
registers the handled type, from one shared call site. They bind NTTP everywhere: they are
the hottest container plumbing in every module, and a single call site cannot pick a
flavor by destination module. The gate (`review_nttp.das`) skips them by `cppName` prefix.

The gate scans the module registry of its own program, so only modules its
`require` list pulls in are covered - the require list is the coverage list.
