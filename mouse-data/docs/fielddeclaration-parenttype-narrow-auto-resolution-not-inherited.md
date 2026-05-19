---
slug: fielddeclaration-parenttype-narrow-auto-resolution-not-inherited
title: What does the parentType bit on FieldDeclaration actually mean? Is it set on fields inherited from a parent class?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**No.** `parentType` is NOT "this field came from a parent class". It is a narrow auto-type-resolution flag and is almost always false after inference.

## Actual semantic

`parentType` is set in exactly two places:

1. `src/parser/parser_impl.cpp:408` — when a derived class declaration overrides a parent field and the override uses `auto` (so the type comes from the parent at infer time):
   ```cpp
   oldFd->parentType = oldFd->type->isAuto();
   ```
2. `src/ast/ast_generate.cpp:2113,2136` — when `makeClassRtti` / `makeClassFinalize` rewrites the cloned `__rtti` / `__finalize` field in a derived class. Same `fd->type->isAuto()` check — only true for `__finalize` (declared as `tAutoinfer`), false for `__rtti` (declared as `pvoid`).

## When it gets cleared

`src/ast/ast_infer_type.cpp:247-258` reads `parentType=true`, clones the parent's resolved type into `decl.type`, and sets `parentType = false`. So once the typer runs the bit goes back to false.

## What this means for macros

Most consumers reading `!fld.flags.parentType` (e.g. `daslib/rst.das`, `utils/mcp/subtools/list_types.das`, `daslib/aot_cpp.das`) run post-inference, where the bit is effectively always false. The check is vestigial / defensive against the rare pre-resolution case.

If you want "this field is inherited from a parent class and not (re)declared here", use the `inherited` bit instead (added in PR #2736, bit 0x800). That bit is sticky — set on clone, cleared only when the derived class declares or overrides the field locally.

If you want to know if a field's type still needs parent resolution (pre-infer macros only), use `parentType`.

## Naming trap

The name reads like "this field's type belongs to the parent" which most people interpret as "inherited from parent class". That broader interpretation is wrong but tempting — at one point even the daslib usages were written under it. PR #2736 added the explicit `inherited` bit to remove the ambiguity. Don't add new code that reads `parentType` to mean "inherited"; use `inherited` (or `!implemented`, which means the same thing pre/post inference).

## Questions
- What does the parentType bit on FieldDeclaration actually mean? Is it set on fields inherited from a parent class?
