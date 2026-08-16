# Simulate headers — architecture notes

## The hot-path model

The interpreter's throughput is a handful of functions: the `SimNode::eval*` methods
(`simulate_nodes.h`, `runtime_*.h`), their AOT twins in `aot.h`, and the dispatchers
`Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`).
Work added there runs once per evaluated node, so a single load, branch, or counter taxes
every program on every step. Amortized container work — growth in `runtime_array.cpp` /
`runtime_table.h`, reached from eval nodes but running once per capacity change — is outside
the hot set; its cost is judged against the allocate/copy/rehash it rides.

## Sanctioned hot-path additions

The ledger the checklist's hot-path rule routes to. Each entry: what was added, where, why
correctness required it, and the alternative that was rejected.

(none)
