# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **No new work in code executed once per evaluation step — the `SimNode::eval*` methods
  (`simulate_nodes.h`, `runtime_*.h`, their AOT twins in `aot.h`) and the dispatchers
  `Context::callOrFastcall` / `callWithCopyOnReturn` / `invoke` / `invokeEx` (`simulate.h`) —
  unless correctness requires it.** The interpreter's throughput is these few functions, and
  any added load, branch, or counter there taxes every program. A correctness-required
  addition is FLAGGED for human review in the PR description, naming the alternative that
  was rejected; an unflagged hot-path addition is a defect.
