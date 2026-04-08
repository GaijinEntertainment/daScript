# GC Phase 2 Notes — Living Document

This is a knowledge capture document, NOT a task skill. Updated throughout Phase 1 (TypeDecl migration) with everything relevant to Phase 2: migrating Expression, Function, Structure, Variable, Enumeration from `smart_ptr` / `ptr_ref_count` to `gc_node`.

Read this at the start of Phase 2 planning.

## Patterns That Will Generalize

- **Uninitialized pointer fields**: Every `FooPtr field;` that was `smart_ptr<Foo>` needs `= nullptr`. Use regex `FooPtr\s+\w+;` to find them. This WILL be the #1 source of crashes in Phase 2.
- **`make_smart<T>` → `new T`**: Pure mechanical text replacement. ~500 per type.
- **`.get()` removal on direct members**: `firstType.get()` → `firstType`. Fields that were smart_ptr members now raw pointers.
- **`.reset()` → `= nullptr`**: Same pattern.
- **safebox template**: Add second type parameter for raw pointer types.
- **`const FooPtr &` params → `FooPtr`**: Pass by value since it's just a pointer.
- **Serializer `operator<<`**: Template needs updating for safebox new signature.
- **Parser .ypp + generated .cpp**: Fix both, don't regenerate with bison.

## Patterns That Won't Generalize

*TypeDecl-specific quirks that won't apply to other types.*

- TypeDecl has no custom destructor — relies on member auto-destruction. Other AST types (Expression, Function) have significant destructors.
- TypeDecl's `dimExpr` field holds `vector<ExpressionPtr>` — cascades via smart_ptr in Phase 1. In Phase 2, Expression becomes gc_node too, so this cascade changes.
- TypeDecl's `clone()` static method had a `use_count==1` optimization — unique to TypeDecl.

## Surprising Edge Cases

*Things that broke unexpectedly during Phase 1 that will likely break again.*

## safebox / Container Changes

*What worked, what didn't, how to adapt for other types.*

## gc_collect Walk Discoveries

*Cross-references found during implementation, circular dependency patterns, what needed the gc_type_owner marker and why.*

- Phase 1 adds `gc_root * gc_type_owner` as a temporary marker to Expression, Structure, Function, Variable (prevents infinite recursion during collect).
- In Phase 2, these types become real gc_nodes — `gc_type_owner` is replaced by `gc_owner` (inherited from gc_node). The cycle detection logic (`if (gc_owner == target) return`) naturally replaces `if (gc_type_owner == target) return`.

## Serializer Patterns

*How ast_serializer.cpp changed for TypeDecl, what the pattern is for other types.*

## Binding Layer Patterns

*How module_builtin_ast*.cpp bindings changed, what generalizes.*

## Performance Observations

*Did Debug build get slower/faster? Any hot paths?*

## gc_guard Usage Patterns

*Where guards were needed, what scope worked.*

## gc_type_owner → gc_node Migration Notes

In Phase 1, Expression/Structure/Function/Variable get a temporary `gc_root * gc_type_owner` field used solely as a "visited" marker during `gc_collect()`. In Phase 2:

- These types inherit from `gc_node`, gaining `gc_owner` naturally
- `gc_type_owner` field is removed
- Cycle detection changes from `gc_type_owner == target` to `gc_owner == target`
- The collect logic otherwise stays the same: walk children, move reachable nodes to module root

## Things We Deferred

*Any shortcuts taken for Phase 1 that Phase 2 must address.*

- `TypeDecl::clone()` use_count==1 optimization disabled — always allocates fresh. Revisit after Phase 1 to assess performance impact.
- Macro-stored TypeDecl reachability not solved generically — monitoring via gc_report(). May need `MacroModule::gc_collect()` virtual for Phase 2.

## Documentation Patterns

*What RST/tutorial changes were needed and how to find similar ones for other types.*
