# Simulate headers — architecture notes

## The hot-path model

The interpreter's throughput is a handful of functions: the `SimNode::eval*` methods
(`simulate_nodes.h`, `runtime_*.h`), their AOT twins in `aot.h`, and the dispatchers
`Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`).
Work added there runs once per evaluated node, so a single load, branch, or counter taxes
every program on every step. Amortized container work — growth in `src/simulate/runtime_array.cpp`
(repo root) / `runtime_table.h`, reached from eval nodes but running once per capacity change — is outside
the hot set; its cost is judged against the allocate/copy/rehash it rides.

## Sanctioned hot-path additions

The ledger the checklist's hot-path rule routes to. Each entry: what was added, where, why
correctness required it, and the alternative that was rejected.

- **`das_ordered2`** (`aot.h`) — a two-member aggregate the AOT emitter wraps around any
  binary op whose operands are not both side-effect-free, because braced aggregate init is
  the C++ construct that guarantees left-to-right evaluation; a plain call argument list or
  binary operator is unsequenced, and the interpreter and JIT both evaluate left-then-right.
  Optimized builds flatten the wrapper to nothing (full-corpus A/B: regen + compile of all
  AOT TUs is timing-neutral); an unoptimized AOT build pays a copy of both operand values
  plus an immediately-invoked lambda frame per wrapped op. Ops whose policy operands need a
  ref cast decline the wrapper and keep the plain unordered emission. Rejected alternative:
  hoisting operands to named temporaries in the emitter, which needs statement-position
  rewriting the textual visitor cannot do inside an expression.
