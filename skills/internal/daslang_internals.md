# daslang compiler internals (repo-only)

Internals a repo maintainer needs that no user-facing doc carries: how the compiler implements
something, or where it is known to be wrong. Language, runtime and stdlib truth is
`skills/daslang/`; house policy is `CLAUDE.md`.

## The container collect banner

`erase` / `erase_if` / `clear` / shrinking `resize` / `pop` (arrays) and `erase` / `clear`
(tables) COLLECT the elements they drop, via `builtin_collect_local` — the GcPod walk that
frees owned array/table heap and cannot execute user code by construction (the no-memset
sibling of `builtin_collect_local_and_zero`). Gated on `typeinfo needs_container_finalize`,
i.e. `force_inscope_pod && !is_const && is_pod_delete && is_safe_to_delete`, so the collect
half rides the **inscope-pod** policy (default OFF) exactly like move-assign and scope-exit
collection. Container CONSTRUCT (`default_init_containers`, default ON) is a separate knob.

`is_pod_delete` (`InferTypes::isPodDelete`) is true only when collection is fully GENERATED and
frees owned heap — false for POD structs (nothing to free), raw and owning pointers, lambdas,
blocks, iterators, temporaries, and any type carrying a USER finalizer. That is why the banner
never runs user code, why clear-then-`delete` on `array<T?>` stays the borrowed-pointer idiom,
and why slot-erase of pointer-carrying tuples never deletes pointees. `delete` is ownership
teardown (runs user finalizers); `erase` / `clear` are release (generated heap-free only). The
asymmetry is the design.

## Options that are not really options

A `CodeOfPolicies` field is settable from `options` ONLY if its declaration in
`include/daScript/ast/ast.h` carries the `/*option*/` marker. Without it the field exists, is
bound to rtti, and reads plausibly in docs, but `options that_field = true` in a `.das` silently
does nothing — only a C++ embedder setting the policy can reach it. Check the marker before
believing an option is live: a policy-gated pass with no marker has effectively never run.
(probe-verified 2026-08-09)

## Table index rewrite

Under `default_init_containers` infer rewrites a non-store `tab[key]` into
`*_table_index_and_init(tab, key)`, which fills a freshly inserted slot with `default<VT>`.
Direct stores (`=` / `<-` / `:=`) and `addr(tab[k])` keep the raw zeroed slot, since the value
is about to be written. It is `default<T>` semantics, not deep-initialize — a struct gets its
field initializers, a tuple or variant whose members carry initializers stays zeroed.

## Fixed arrays: AST versus runtime

The AST is structural (`Type::tFixedArray`, one `TypeDecl` per dimension, size in `fixedDim`),
but runtime `TypeInfo` is still FLATTENED (`dim[]` / `dimSize`). C++ `TypeDecl::isArray()` means
"is a fixed array" in both pre- and post-rework daslang, which is what lets an external module
compile against both.

## Lattice vectors have no constants

The 16/8-bit lattice vector types (`half2..8`, `short2..8`, `ushort*`, `byte*`, `ubyte*`) carry
NO `ExprConst` nodes by design: `isFoldable` is false and `Program::makeConst` returns null, so
every caller must handle that null. Conformance harness: `tests/type_lattice/` — GENERATED,
regenerate with `daslang utils/dasgen/gen_type_conformance.das`.

## addr<T?> desugaring

`addr<T?>(x)` desugars to `reinterpret<T?>(addr(x))`. The generated reinterpret carries
`castFlags.fromAddrSugar`, and infer's preVisit pushes `alwaysSafe` down onto the generated
inner `addr` — that is what makes one `unsafe` cover both halves, and it is also how STYLE034
exempts the sugar's own output from its own rule.

## Workhorse types

"Passes by value" is `isWorkhorseType` on the C++ side — a predicate, not a hand-kept list. A
binding or macro that must branch on it should call the predicate rather than enumerate scalars.

## AOT-emitted C++

`#pragma optimize` in AOT-generated code must be wrapped in `#ifdef _MSC_VER` — Clang warns on
unknown pragmas. The emitter is `daslib/aot_cpp.das`.

## Distinct types, C++ side

Registering a distinct type from C++ takes `DistinctTypeAnnotation` plus
`MAKE_DISTINCT_TYPE_FACTORY(TYPE, CTYPE)` and `makeType`-registered externs. Worked example:
`NativeId` in dasUnitTest.

## Known defect: generic instance mangling collision

Instantiations of one generic that differ only in inner element-const (`iterator<int -&>` versus
`iterator<int const -&>`) hash-collide in the instance registry, producing
`error[50609]: multiple instances of …` when both arise in one module. Library-level workaround:
declare the iterator overload `iterator<auto(TT) const>` so both source flavors converge on a
single instance — element-const variance is pointer-like, mutable flows into const and not the
reverse. The cost is that `it` is const inside the body, so it cannot be moved from or mutated,
which is why `daslib/linq.das` constifies only `all` and `contains`. Did not reproduce on
2026-08-16 and may already be fixed; keep the workaround until someone re-probes deliberately.

## Lint interaction worth knowing

The `daslib/builtin.das` shape for a bare `var x : TT` inside a generic —
`static_if (typeinfo is_unsafe_when_uninitialized(type<TT>)) { unsafe { ... } } else { ... }` —
needs `// nolint:STYLE025` on the `unsafe {` line: STYLE025 sees exactly one statement needing
unsafe at any single instantiation and cannot reason across the static_if branches.
