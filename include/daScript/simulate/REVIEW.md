# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **No new work in a `SimNode::eval*` method — the ones in `simulate_nodes.h`, `runtime_*.h`,
  and their AOT twins in `aot.h` — or in the dispatchers `Context::callOrFastcall` /
  `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`); code that runs once per
  capacity change rather than once per evaluated node (table grow/rehash, array resize) is
  not in scope. A hot-path addition is a defect.** A load, branch, or counter on that path
  taxes every program on every step; an addition ledgered under `ARCHITECTURE.md`'s
  sanctioned hot-path additions still fires this rule, and the author answers "yes,
  ledgered."
