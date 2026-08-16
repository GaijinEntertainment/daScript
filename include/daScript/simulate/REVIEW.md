# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md` (repo root).

- **No new work in code executed once per evaluation step — the `SimNode::eval*` methods
  (`simulate_nodes.h`, `runtime_*.h`, their AOT twins in `aot.h`) and the dispatchers
  `Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`) —
  unless correctness requires it.** The interpreter's throughput is these few functions, and
  any added load, branch, or counter there taxes every program. A correctness-required
  addition is FLAGGED for human review in the PR description, naming the alternative that
  was rejected; an unflagged hot-path addition is a defect.

- **A field added, removed, or reordered in a `debug_info.h` struct: the PR description
  states a per-consumer verdict (updated / no change needed / rebuild required) for the
  rtti binding (`src/builtin/module_builtin_rtti.cpp`), the das-side readers of the
  struct (enumerate with MCP `grep_usage` on the struct name), and external-module
  rebuilds (`skills/abi_break_sweep.md`).** A diff doing so with no such statement is a
  defect.
