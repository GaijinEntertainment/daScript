# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. A diff changing a `debug_info.h` struct layout applies
`skills/internal/abi_break_sweep.md` too. A diff that changes or removes a name under this
folder that a `daslib/*.das` file spells out - a struct or member the AOT C++ emitter writes
into generated code, a flag or field a daslib predicate reads - applies `daslib/REVIEW.md`
too; checklist discovery walks changed paths only, so the C++ half never opens the daslib
checklist on its own.

- **A diff that adds a field to `CodeOfPolicies` (`code_of_policies.h`) adds it to
  `DAS_MODULE_CACHE_POLICY_FIELDS` in `src/builtin/module_builtin_ast_serialize.cpp`, in the
  same change** - that list drives both the module-cache record's policy stream and the compare
  that refuses a record written under other policies, so a field missing from it is a policy the
  cache silently ignores.

- **A diff that hashes a table key hashes a builtin key type - one in `heap.h`'s
  `makeTableKeyValueNode` list - as itself through `hash_function(context, key)` (`hash.h`),
  and a handled key type as the value type its annotation's `makeValueType()` returns.** A
  table grow rehashes every non-string key with `KeyHash` (`runtime_table.h`), so a site that
  hashes a key type differently loses every key of that type at the first grow.

- **A diff that changes `KeyHash` (`runtime_table.h`) or `WrapsBuiltinValue` (`cast.h`) states
  in its own PR description which key types change hash value.**

- **A diff that makes the hot path more expensive per evaluated expression is a defect - an
  added load, branch, call, copy, or counter, a direct call becoming indirect, a static
  dispatch becoming virtual, and an unboxed value becoming a boxed round-trip all count.**
  The hot path is a `SimNode::eval*` method, any helper such a method calls on every
  evaluation, the dispatchers `Context::callOrFastcall` / `callWithCopyOnReturn` /
  `invoke` / `invokeEx` (`simulate.h`), or an AOT-side function or template under this
  folder that generated code executes per evaluated expression. Such a diff - including one
  an optimized build flattens to nothing - lands its entry under `ARCHITECTURE.md`'s
  sanctioned hot-path additions in the same diff: what was added, where, why correctness
  required it, and the alternative that was rejected.

- **A diff that changes the layout of a `debug_info.h` struct - a field added, removed,
  reordered, or retyped, or a base changed - states a per-consumer verdict (updated / no
  change needed / rebuild required) in its own PR description: for the rtti binding
  (`src/builtin/module_builtin_rtti.cpp`), for the AST serializer
  (`src/builtin/module_builtin_ast_serialize.cpp`), for the das-side readers of the struct,
  and for external-module rebuilds.**

- **A `debug_info.h` layout change re-pins to the new layout every assertion in
  `tests-cpp/small/test_debug_info_layout_pin.cpp` it makes false, and adds an `offsetof`
  pin for each field it adds** - a field that lands in tail padding leaves `sizeof`
  unchanged, so no other assertion in that file fails.
