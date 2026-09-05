# Simulate headers - architecture notes

## The hot-path model

The interpreter's throughput is a handful of functions: the `SimNode::eval*` methods
(`simulate_nodes.h`, `runtime_*.h`, the extern-call nodes in `interop.h`), their AOT twins
in `aot.h`, and the dispatchers
`Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`).
Work added there runs once per evaluated node, so a single load, branch, or counter taxes
every program on every step. Amortized container work - growth in `src/simulate/runtime_array.cpp`
(repo root) / `runtime_table.h`, reached from eval nodes but running once per capacity change - is outside
the hot set; its cost is judged against the allocate/copy/rehash it rides.

## Table key hashing

A table key hashes on every rail as the interpreter's table node hashes it. A builtin key type -
`heap.h`'s `makeTableKeyValueNode` list: scalars, the vectors, the ranges, strings, pointers -
hashes as itself, `hash_function(context, key)`: `hash_uint32`/`hash_uint64` for the scalar and
range specializations, `hash_blockz64` for strings, `hash_block64` over `sizeof(key)` bytes
otherwise (8 for an `int2`, 12 for a `float3`). A handled key - a `ManagedValueAnnotation<OT>`
such as `Time` or a module's id type - hashes as its workhorse, `WrapType<OT>::type`, because
that is the das value type the annotation declares (`makeValueType`) and what the compiler casts
the key to before the node hashes it; `EntityId` hashes as an `int32`, `BigEntityId` as a
`vec4f`. The interpreter's table nodes (`runtime_table_nodes.h`), the JIT helpers
(`src/builtin/module_jit.cpp`, repo root), the JSON scanner (`src/simulate/json_scan.cpp`, repo
root), rtti and the C API call `hash_function` on the key type they hold. `KeyHash`
(`runtime_table.h`) is the same hash for the callers that hold a key of a C++ type - AOT's
`TTable`, the `__builtin_table_*` templates in `aot.h`, and the rehash a grow performs on every
stored non-string key (a string table reuses its stored hashes): it takes the workhorse detour
for a handled type and hashes a builtin type as itself, telling them apart by
`WrapsBuiltinValue` (`cast.h`; `jit_abi.h` marks the vectors and ranges). The detour on a
builtin vector or range would hash a `vec4f`'s 16 bytes against the node's 8 or 12, and the raw
bytes of a handled type narrower than its workhorse (an 8-byte `ImVec2` wrapped to `vec4f`)
would miss the node's 16; either way every such key moves to another bucket at the first grow.
Non-string tables are open-addressed from their first slot (only string keys pack linearly up
to 8), so a disagreement shows on a one-key table as much as on a large one.

## Sanctioned hot-path additions

The ledger the checklist's hot-path rule routes to. Each entry: what was added, where, why
correctness required it, and the alternative that was rejected.

- **CRT scalar transcendentals** (`sim_policy.h`) - the scalar float arms of `Exp`, `Exp2`,
  `Log2` and `Pow` call the CRT; the `vec4f` arms stay on the vecmath polynomials, where
  four lanes amortize the setup. The lane trick's `v_set_x`/`v_extract_x` round-trip is a
  partial-register dependency chain scalar codegen does not break, so for these arms the
  CRT call is cheaper than the inlined polynomial - that is the rejected alternative. `Log2`
  is the one arm the swap does not speed up: it trades the `v_log2_est_p5` estimate for the
  exact answer the JIT already computes, so interp, AOT and JIT agree. `log`, `sin`, `cos`
  and `tan` stay on the lane, which is cheaper for them. The measurements behind the split:
  `plans/benchmark_followups.md` (repo root), the scalar-exp section.

- **`das_ordered2`** (`aot.h`) - a two-member aggregate the AOT emitter wraps around any
  binary op whose operands are not both side-effect-free, because braced aggregate init is
  the C++ construct that guarantees left-to-right evaluation; a plain call argument list or
  binary operator is unsequenced, and the interpreter and JIT both evaluate left-then-right.
  Optimized builds flatten the wrapper to nothing; an unoptimized AOT build pays a copy of
  both operand values
  plus an immediately-invoked lambda frame per wrapped op. Ops whose policy operands need a
  ref cast decline the wrapper and keep the plain unordered emission. Rejected alternative:
  hoisting operands to named temporaries in the emitter, which needs statement-position
  rewriting the textual visitor cannot do inside an expression.

- **Member `fn` on the extern-call nodes** (`interop.h`: `SimNode_ExtFuncCall`,
  `SimNode_ExtFuncCallAndCopyOrMove`, `SimNode_ExtFuncCallRef`) - the function pointer is
  a runtime member so all binds of one signature share a single node instantiation, which
  is what shrinks module binaries (bind-heavy shared modules measure -5..-24%) and halves
  per-bind compile time. Cost per evaluated extern call: one member load plus an indirect
  call in place of a direct, potentially inlined one - measured at 3-4.5% on loops that
  are nothing but extern calls, noise on real programs (dasProfile lane medians >=1.00).
  Binds that need the old profile opt into the NTTP flavor (`addExternInline` ->
  `SimNode_ExtFuncCallInline`; the modules `src/builtin/REVIEW.md` names bind that
  way by policy). The cmres and ref flavors have no NTTP twin yet, so those
  binds pay the indirect call unconditionally. Cross-slot and lattice typed reads route
  through the base as `cast<Carrier>::to(this->eval(ctx))` - a second virtual dispatch
  plus a vec4f round-trip on paths that are rare by construction (the compiler emits the
  matching slot; only uint-index reads and value reinterpret land here). Rejected
  alternative: keeping the per-function NTTP matrix everywhere - master's shape, ~24KB of
  object code and ~11ms of compile per bind, mostly duplicated typed-eval stubs.
