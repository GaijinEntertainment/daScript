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

A table key has one hash on every rail: `hash_function(context, key)` on the key's own type -
`hash_uint32`/`hash_uint64` for the scalar specializations, `hash_blockz64` for strings, and
`hash_block64` over `sizeof(key)` bytes for everything else, so an `int2` hashes 8 bytes, a
`float3` 12, a `range` through its 8-byte specialization. The interpreter's table nodes
(`runtime_table_nodes.h`), the JIT helpers (`src/builtin/module_jit.cpp`, repo root), the JSON
scanner (`src/simulate/json_scan.cpp`), rtti and the C API all call it directly. `KeyHash`
(`runtime_table.h`) is the same hash for the callers that hold a key of a wrapped C++ type -
AOT's `TTable`, the `__builtin_table_*` templates in `aot.h`, and the rehash a grow performs
on every stored key: it takes the workhorse detour (`Time` to `int64`, a handle to `uint64`,
a smart pointer to its raw pointer, a 16-byte vector to `vec4f`) only when the workhorse has
the key's byte width, because those specializations hash the same bytes; a 2- or 3-lane vector
or a range is hashed as itself, since widened to `vec4f` it would hash 16 bytes and every key
would move to another bucket at the first grow. Non-string tables are open-addressed from
their first slot (only string keys pack linearly up to 8), so a disagreement shows on a
one-key table as much as on a large one.

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
