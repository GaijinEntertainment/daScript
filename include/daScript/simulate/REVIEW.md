# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. A diff changing a `debug_info.h` struct layout applies
`skills/internal/abi_break_sweep.md` too.

- **No new work the hot path performs per evaluated expression — a load, branch, call,
  copy, or counter that survives optimization — anywhere under this folder: a
  `SimNode::eval*` method, any helper such a method calls per evaluation, the dispatchers
  `Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx`
  (`simulate.h`), or an AOT-side function or template under this folder (`aot.h`,
  `aot_builtin_*.h`) that generated code executes per evaluated expression. Code that runs
  once per capacity change rather than once per evaluated expression is not in scope. A
  hot-path addition is a defect.** Work on that path taxes every program on every evaluated
  expression.

- **A diff that adds a construct the hot path executes per evaluated expression — including
  one an optimized build flattens to nothing — lands its entry under `ARCHITECTURE.md`'s
  sanctioned hot-path additions in the same diff, stating the optimized cost and the
  unoptimized cost. Such a diff with no ledger entry is a defect.**

- **A change to the layout of a `debug_info.h` struct — a field added, removed, reordered,
  or retyped, or a base changed — carries a per-consumer verdict (updated / no change
  needed / rebuild required) in the PR description: for the rtti binding
  (`src/builtin/module_builtin_rtti.cpp`), for the das-side readers of the struct
  (enumerate with MCP `grep_usage` on the struct name), and for external-module rebuilds.
  Such a diff with no verdict statement is a defect.**

- **Weakening the `debug_info.h` layout pin is a defect: a diff that deletes or loosens an
  assertion in `tests-cpp/small/test_debug_info_layout_pin.cpp` instead of re-pinning it to
  the new layout.**
