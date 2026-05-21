---
slug: in-a-qmacro-template-can-i-use-i-name-in-the-iter-var-position-of-a-multi-iterator-for-loop-e-g-for-i-a-i-b-in
title: In a qmacro template, can I use `$i(name)` in the iter-var position of a multi-iterator for-loop (e.g. `for ($i(a), $i(b) in ...)`)?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**No.** The qmacro parser rejects `$i(...)` splice in iter-var position of a for-loop header — fails with `syntax error, unexpected $i, expecting name or '('`. It works fine in source-expression position (`for (x in $i(srcName))`), just not in iter-name position.

**Why:** the for-loop grammar at the iter-var position takes a raw `NAME` token (per the bison grammar in `src/parser/`). The `$i(...)` splice is only accepted at expression-position productions.

**Workarounds:**

1. **Use literal iter names**. If your splice planner controls the calling convention, hardcode the iter var names (e.g. `itA`, `itB` for 2-iter zip emission) and emit:
   ```das
   bodyStmts |> push <| qmacro_expr() {
       for (itA, itB in $i(srcAName), $i(srcBName)) {
           $e(loopBody)
       }
   }
   ```
   This is what `plan_zip` in `daslib/linq_fold.das:3160` does. Callers that need to reference the iter values from outside the for-loop (e.g. via `let it = (itA, itB)` body bind in `preCondStmts`) just use the same literal names.

2. **Hand-build the ExprFor AST node** directly via `new_expr_for(...)` if you genuinely need dynamic-arity iter vars (e.g. for an 8-ary zip planner without 8 hardcoded branches). More verbose; only worth it when literal names won't scale.

**Real example:** the multi-source generalization of `emit_accumulator_lane` / `emit_early_exit_lane` in PR for Phase 2B+ branches on `length(srcNames)` (1 vs 2) and emits two different `qmacro_expr() { for (...) { ... } }` blocks — one with `$i(itName)` (single iter), one with literal `itA, itB`. Dynamic-arity beyond 2 would require option 2.

**Don't try:** `for ($i(itName0) in ...)` with a hardcoded loop unroll — `$i` in iter-name position fails for any arity (even 1, when the name itself comes from a splice). Single-source helpers use `$i(itName)` in iter position because that's the canonical "single iter via splice" pattern and the parser does accept it... actually wait, let me re-check.

Re-checking: `for ($i(itName) in $i(srcName))` (single iter, single source, both spliced) DOES work in plan_loop_or_count's `emit_accumulator_lane` at line 770. The parser accepts `$i(name)` in iter-var position for **single-iter** for-loops. The failure was specifically the **multi-iter** form `for ($i(a), $i(b) in ...)`. So the rule is: **`$i` in iter-var position works only when there's exactly one iter var.** Multi-iter must use literal names.

Why: the multi-iter grammar production expects `NAME (',' NAME)+` — the `,` separator requires the parser to lookahead for a raw NAME token, and `$i` doesn't satisfy that lookahead.

**Bottom line for planners:** if you're emitting a single-iter for-loop, use `$i(name)`. If you need multi-iter (zip, archetype walks, etc.), use literal iter names and have the planner control the calling convention so the iter names are predictable (`itA`, `itB`, `itC`, ... up to your arity ceiling).

## Questions
- In a qmacro template, can I use `$i(name)` in the iter-var position of a multi-iterator for-loop (e.g. `for ($i(a), $i(b) in ...)`)?
