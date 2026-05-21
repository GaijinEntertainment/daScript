---
slug: when-a-daslang-bench-reports-0-ns-op-for-a-splice-that-should-be-doing-real-work-is-it-dce-how-do-i-verify
title: When a daslang bench reports 0 ns/op for a splice that should be doing real work, is it DCE? How do I verify?
created: 2026-05-20
last_verified: 2026-05-20
links: []
---

**Not necessarily DCE — most likely the bench framework's reporting floor.** Verify with `mcp__daslang__ast_dump --mode source` before assuming the compiler optimized it away.

The bench framework (`dastest/testing.das` `run(b, name, n, body)`) reports `body_time / n_iters` as ns/op. If the body is fast enough that the per-iteration share rounds below 1 ns, you'll see "0 ns/op" — even though the body is doing real, observable work.

**Diagnosis:**

```bash
# Dump post-macro source for the bench file. Look for the lane's `def run_m4` (or m3f/etc) body.
mcp__daslang__ast_dump --file benchmarks/sql/your_bench.das --mode source
```

If you see something like:
```
def run_m4(...) {
    run(b, "m4_decs_fold/{n}", n, $():void {
        let rows <- invoke($() : array<...> {
            var decs_buf : array<...>
            var decs_takec : int = 0
            for_each_archetype_find(...) {
                ...if (decs_takec >= 1000) return true
                else { ++decs_takec; push_clone(decs_buf, ...) }
            }
            return <- decs_buf
        })
        accept(b, rows)
        if (empty(rows)) failNow()
    })
}
```

…then the splice IS firing and IS building a real array. The 0 ns/op is the measurement floor, not optimization.

**True DCE looks different in the dump:** the body would be missing entirely, or the `let rows` would be a constant, or `accept(b, ...)` would be called with a literal. If `accept(b, rows)` and `empty(rows)` both reference the computed result, the optimizer cannot DCE the whole chain — it might inline aggressively, but the work happens.

**Don't claim DCE based on benches alone.** Always verify with `ast_dump --mode source` first. Source-dump is the ground truth for what gets emitted.

**Cross-check against another lane:** if m3f (array-side splice) and m4 (decs-side splice) both round to 0 ns/op with the same chain shape, they're both at the floor — that's a strong signal the splices are equivalent fast paths, not that both got DCE'd. m1/m3 (eager) showing ns numbers > 0 with similar work confirms the floor is somewhere below the eager baseline.

**Common false alarm: `b |> accept(value)` and `b->failNow()` are anti-DCE primitives** (see `dastest/testing.das` `[sideeffects] def accept(...)`). Their presence in the bench body usually rules out compiler DCE.

**When to actually suspect DCE:**
- The bench has NO `accept(...)` and NO `if (empty(...)) failNow()` — just `let rows <- chain`
- ast_dump shows the lambda body has a missing return value or a literal
- The bench file's plan.md / M4_DECS_EXPANSION.md flags this lane as a known DCE issue (e.g., 11 such benches were flagged in May 2026)

Hit incident: PR #2770 (Slice 5a take/skip on decs) initially claimed "0 ns/op = DCE" in the PR body. ast_dump showed the splice was firing and pushing 1000 elements per iteration. Old eager bridge: 36 ns/op. New splice: rounded to 0. Real win ≈ 36×.

## Questions
- When a daslang bench reports 0 ns/op for a splice that should be doing real work, is it DCE? How do I verify?
