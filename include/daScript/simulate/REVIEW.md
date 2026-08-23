# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`. A diff changing a `debug_info.h` struct layout applies
`skills/internal/abi_break_sweep.md` too.

- **The hot path must not become more expensive per evaluated expression - new work is a
  defect, and so is degrading existing work: a direct call becoming indirect, a static
  dispatch becoming virtual, or an unboxed value becoming a boxed round-trip counts the
  same as an added load, branch, call, copy, or counter.** The hot path is a
  `SimNode::eval*` method, any helper such a method calls on every evaluation, the
  dispatchers `Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx`
  (`simulate.h`), or an AOT-side function or template under this folder that generated
  code executes per evaluated expression. A diff that adds or amplifies such a construct -
  including one an optimized build flattens to nothing - lands its entry under
  `ARCHITECTURE.md`'s sanctioned hot-path additions in the same diff; the entry's required
  fields are that ledger's to define. Work on this path taxes every program on every
  evaluated expression.

- **A change to the layout of a `debug_info.h` struct - a field added, removed, reordered,
  or retyped, or a base changed - carries a per-consumer verdict (updated / no change
  needed / rebuild required) in the PR description: for the rtti binding
  (`src/builtin/module_builtin_rtti.cpp`), for the das-side readers of the struct, and for
  external-module rebuilds.** Such a diff with no verdict statement is a defect.

- **A `debug_info.h` layout change that deletes or loosens an assertion in
  `tests-cpp/small/test_debug_info_layout_pin.cpp` instead of re-pinning it to the new
  layout is a defect.**
