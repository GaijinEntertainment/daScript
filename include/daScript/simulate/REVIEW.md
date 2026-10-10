# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. A layout change to a struct or class is a member added, removed, reordered,
renamed or retyped, or a base changed. A diff that makes a layout change to a public struct or
class under this folder, any struct declared in `debug_info.h` included, applies
`skills/internal/abi_break_sweep.md` too. A diff that changes the signature, return type,
overload set or existence of a C++ name `daslib/aot_cpp.das` (repo root) writes into generated
code, whole or built from a prefix, or what that name does or returns, applies `daslib/REVIEW.md`
too; so does a diff that renames, retypes or removes a flag or field a function under `daslib/`
(repo root) reads. A diff that adds, reorders or retypes a member of a C++ type das binds through
an annotation applies `src/builtin/REVIEW.md` too. Checklist discovery walks changed paths only,
so it never opens either checklist on its own.

**A diff that adds a field to `CodeOfPolicies` (`code_of_policies.h`) adds it to
`DAS_MODULE_CACHE_POLICY_FIELDS` in `src/builtin/module_builtin_ast_serialize.cpp`, in the
same change** - that list drives both the module-cache record's policy stream and the compare
that refuses a record written under other policies, so a field missing from it is a policy the
cache silently ignores.

**A diff that adds a data member to a C++ type under this folder that das binds through an
annotation appends it after that type's last member, or bumps `LLVM_JIT_CODEGEN_VERSION`
(`modules/dasLLVM/daslib/llvm_jit_plan.das`) in the same change.** A cached JIT DLL binds the
members it read by offset; a member inserted before them moves every later one under code that
still uses the old offsets.

**A diff that hashes a table key hashes a builtin key type - one in `heap.h`'s
`makeTableKeyValueNode` list - as itself through `hash_function(context, key)` (`hash.h`),
and a handled key type as the value type its annotation's `makeValueType()` returns.** A
table grow rehashes every non-string key with `KeyHash` (`runtime_table.h`), so a site that
hashes a key type differently loses every key of that type at the first grow.

**A diff that changes `KeyHash` (`runtime_table.h`) or `WrapsBuiltinValue` (`cast.h`) states
in its own PR description which key types change hash value.**

**A diff that adds or changes a `cvt_*` inline in `aot.h` returns `vec4f` from it and gives it
a name no other `cvt_*` has - never an overload.** `vec4f` is the SIMD register; the
`vec2`/`vec3`/`vec4` types are structs of scalars, so a concrete return spills the lanes
through `v_extract_*` and the next conversion reloads them, and an overload set fed a `vec4f`
result is ambiguous - the emitter writes `cvt_pass(cvt_uint3(..))` for `x |> uint3 |> int3`.

**A diff that makes a hot-path body that already answered correctly before the diff cost more per evaluated expression in
the build the repo ships is a defect.** The hot path is a `SimNode::eval*` method, any helper
such a method calls on every evaluation, the dispatchers `Context::callOrFastcall` /
`callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`), or an AOT-side function or
template under this folder that generated code runs for every evaluated expression
(`ARCHITECTURE.md#hot-path-model`).

**A diff that adds hot-path work - an added load, branch, call, copy or counter, a direct call
made indirect, a static dispatch made virtual, or an unboxed value made a boxed round-trip, in the
hot path this checklist defines - lands its entry under
`ARCHITECTURE.md#sanctioned-hot-path-additions` in the same diff (what was added, where, why
correctness requires it, the rejected alternative), even when the optimizer of the shipped build
removes it, and names in its PR description its emitted codegen unchanged or a measurement
against the code it replaces.**

**A diff whose added hot-path work costs more per evaluated expression only under a relaxed-math
or other non-default compiler flag states in its PR description which flag and how much.**

**A diff that makes a layout change to a `debug_info.h` struct states a per-consumer verdict
(updated / no change needed / rebuild required) in its own PR description: for the rtti binding
(`src/builtin/module_builtin_rtti.cpp`), for the AST serializer
(`src/builtin/module_builtin_ast_serialize.cpp`), for the das-side readers of the struct,
and for external-module rebuilds.**

**A `debug_info.h` layout change re-pins to the new layout every assertion in
`tests-cpp/small/test_debug_info_layout_pin.cpp` it makes false, and adds an `offsetof`
pin for each field it adds** - a field that lands in tail padding leaves `sizeof`
unchanged, so no other assertion in that file fails.
