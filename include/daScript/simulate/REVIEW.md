# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **No new work in a `SimNode::eval*` method — the ones in `simulate_nodes.h`, `runtime_*.h`,
  and their AOT twins in `aot.h` — or in the dispatchers `Context::callOrFastcall` /
  `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`); code that runs once per
  capacity change rather than once per evaluated node (table grow/rehash, array resize) is
  not in scope. A hot-path addition is a defect.** A load, branch, or counter on that path
  taxes every program on every step; a correctness-required addition is ledgered under
  `ARCHITECTURE.md`'s sanctioned hot-path additions.

- **A field added, removed, or reordered in a `debug_info.h` struct: the PR description
  states a per-consumer verdict (updated / no change needed / rebuild required) for the
  rtti binding (`src/builtin/module_builtin_rtti.cpp`), the das-side readers of the
  struct (enumerate with MCP `grep_usage` on the struct name), and external-module
  rebuilds (`skills/abi_break_sweep.md`).** A diff doing so with no such statement is a
  defect.
