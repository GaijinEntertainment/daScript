# Simulate Headers Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md` (repo root).

- **No new work on the hot evaluation paths — call/fastcall dispatch, invoke, loop and
  condition nodes, the per-node `eval` methods — unless correctness requires it.** The
  interpreter's throughput is these few functions, and any added load, branch, or counter
  there taxes every program. A correctness-required addition is FLAGGED for human review
  in the PR description, naming the alternative that was rejected; an unflagged hot-path
  addition is a defect.
