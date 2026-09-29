# src/ast architecture notes - gc roots

Companion to `ARCHITECTURE.md` in this folder.

## Type remap at module finalize (`ast_typedecl.cpp`, `ast_gc_collect.cpp`, `ast_parse.cpp`) {#type-remap}

A dependency module that ends with no errors - inferred and finalized, built, or read from the
module cache - shares its equal types. `ModuleGcFinalize` collects the module's live nodes into
a fresh root whose `typeRemap` points at a `TypeDeclRemap` table for the length of that collect.
Only that collect has the fresh root as its target, so no other collect sees the table. The
collect walk reaches every holder of a type, and each holder hands its slot to `gc_collect_type`
by reference. `gc_collect_type` collects the type's child types first, then looks the type up in
the table. When the table holds an
equal type, the slot is pointed at that type, and the node it held stays on the old root, which
the finalize sweeps. Otherwise the type is collected and joins the table. A type the walk reaches
through a plain `TypeDecl::gc_collect` joins the table too, but its holder keeps its own node.

Two types are equal when every field matches - base type, flags, fixed dim, structure,
enumeration, annotation, module, alias and argument names - the child types are the same
pointers, and all five fields of `at` match. Children are remapped before their parent, so equal
child pointers mean equal content and equal locations all the way down. `at` is part of the key
because a declared type reports at its own position (`x : int`): a shared node without it would
move diagnostics and editor ranges. The alias-cache bits (`TypeDecl::aliasCacheFlags`) are not
compared - they are derived from the content. A type that holds expressions (`fixedDimExpr`,
type-macro arguments) is never shared.

The remap waits for the module's end because inference writes flags into types in place
(`expr->type->constant = ...`), and a shared node would carry that write to every holder. A
finished dependency module is only read: later modules clone its types, and simulation and the
module-cache writer read them. Two finalizes do not remap. The main program's module still gets
`finalizeCompiledProgram` after its finalize. A module parsed without inference
(`parseDaScriptNoInfer`) is inferred later.

A replaced node is freed with the old root, so a pointer to it that the collect walk does not
see dangles - a lookup cache, or a macro's global. `clearAllFunctionLookups` runs at the same
point for the lookup caches. A type slot the walk collects with a plain `gc_collect` stays
correct: its node survives, it is just not shared. `ExprTypeInfo::typeexpr` is collected that
way: `typeinfo ast_typedecl` bakes that exact node into the simulated macro code
(`SimNode_AstGetTypeDecl`) and into `Program::astTypeInfo`, and the macro reads it after its
module ends.

The table pays because most live types are flat primitives on expressions - `const int`, `void`,
`const uint` - and each distinct type with its location has about six holders.
