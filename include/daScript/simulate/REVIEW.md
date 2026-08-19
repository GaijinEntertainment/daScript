# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **No new code that runs once per evaluated expression, anywhere under this folder — a
  `SimNode::eval*` method, any helper such a method calls per evaluation (a table lookup,
  a `SimPolicy` op, a hash), the dispatchers `Context::callOrFastcall` /
  `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`), or an AOT-side function or
  template under this folder (`aot.h`, `aot_builtin_*.h`) that generated code executes per
  evaluated expression. Code that runs once per
  capacity change rather than once per evaluated expression is not in scope. A hot-path
  addition is a defect.** A load, branch, or counter on that path taxes every program on every
  evaluated expression; a correctness-required addition is ledgered under
  `ARCHITECTURE.md`'s sanctioned hot-path additions.

- **A field added, removed, or reordered in a `debug_info.h` struct: the PR description
  states a per-consumer verdict (updated / no change needed / rebuild required) for the
  rtti binding (`src/builtin/module_builtin_rtti.cpp`), the das-side readers of the
  struct (enumerate with MCP `grep_usage` on the struct name), and external-module
  rebuilds (`skills/internal/abi_break_sweep.md`); such a diff with no verdict statement is a
  defect — and the same diff updates the layout pin
  (`tests-cpp/small/test_debug_info_layout_pin.cpp`) to the new layout rather than
  relaxing it.**
