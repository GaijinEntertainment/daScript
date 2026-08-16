# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **No new work in a `SimNode::eval*` method (`simulate_nodes.h`, `runtime_*.h`), in the
  dispatchers `Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx`
  (`simulate.h`), or in any `aot.h` function or template that generated code executes per
  evaluated expression; on any of these surfaces, code that runs once per capacity change
  rather than once per evaluated expression is not in scope. A hot-path addition is a
  defect.** A load, branch, or counter on that path taxes every program on every evaluated
  expression; a correctness-required addition is ledgered under `ARCHITECTURE.md`'s
  sanctioned hot-path additions.

- **A field added, removed, or reordered in a `debug_info.h` struct: the PR description
  states a per-consumer verdict (updated / no change needed / rebuild required) for the
  rtti binding (`src/builtin/module_builtin_rtti.cpp`), the das-side readers of the
  struct (enumerate with MCP `grep_usage` on the struct name), and external-module
  rebuilds (`skills/internal/abi_break_sweep.md`); such a diff with no verdict statement is a
  defect.** The layout pin (`tests-cpp/small/test_debug_info_layout_pin.cpp`) pins the
  size and every field offset of every `debug_info.h` struct; a diff weakening the pin
  is a defect.
