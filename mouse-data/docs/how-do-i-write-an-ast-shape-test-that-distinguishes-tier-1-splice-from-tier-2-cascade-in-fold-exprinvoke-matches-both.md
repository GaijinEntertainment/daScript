---
slug: how-do-i-write-an-ast-shape-test-that-distinguishes-tier-1-splice-from-tier-2-cascade-in-fold-exprinvoke-matches-both
title: How do I write an AST-shape test that distinguishes tier 1 splice from tier 2 cascade in `_fold`? `ExprInvoke` matches both.
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**`body_expr is ExprInvoke` is the WEAKEST possible assertion** — both tier-1 splice emissions (in `plan_loop_or_count` / `plan_order_family` / `emit_*_lane`) AND tier-2 `fold_linq_default` emissions wrap their result in `invoke($block, $src)` form. Asserting just `ExprInvoke` passes for both tiers and tells you nothing.

**Strong-form distinguishers** (use 2-3 of these together):

```
t |> equal(1, count_inner_for_loops(body_expr), "single fused for-loop")
t |> equal(0, count_call(body_expr, "where_"),  "where_ should be inlined, not called")
t |> equal(0, count_call(body_expr, "select"),  "select should be inlined, not called")
```

Tier 1 splice emits a single `for` loop with the predicate/projection inlined into the body — zero residual `where_(...)` / `select(...)` calls. Tier 2 `fold_linq_default` emits a multi-statement block:

```
var pass_0 = where_to_array(src, pred)
var pass_1 = select_inplace(pass_0, proj)
delete pass_0
return <- pass_1
```

Multiple `var` decls, `_to_array` / `_inplace` suffixed calls, explicit `delete` between stages. The pattern is distinguishable by counting calls that survive vs. inline.

**Canonical template:** [tests/linq/test_linq_fold_ast.das:1377 (`test_select_where_count_emits_fused_loop`)](tests/linq/test_linq_fold_ast.das#L1377). Use as the model when adding new tier-1 AST-shape assertions.

**Why this matters:** Phase 3d (PR #2712) added the select+where splice arm. The pre-Phase-3d tests asserted `body_expr is ExprInvoke` expecting tier-2 fallback, but those chains now splice tier 1. The tests kept passing (both tiers wrap in invoke) but were stale-named and meaningless. Copilot review caught it; the fix was deleting the redundant tests and replacing with strong-form assertions.

## Questions
- How do I write an AST-shape test that distinguishes tier 1 splice from tier 2 cascade in `_fold`? `ExprInvoke` matches both.
- Why does my `body_expr is ExprInvoke` assertion pass for both tier 1 splice and tier 2 cascade in `_fold` AST tests?
- What's the canonical splice-emission shape I should assert in `test_linq_fold_ast.das` for a new splice arm?

## Questions
- How do I write an AST-shape test that distinguishes tier 1 splice from tier 2 cascade in `_fold`? `ExprInvoke` matches both.
