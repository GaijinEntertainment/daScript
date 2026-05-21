---
slug: when-two-linq-fold-splice-planners-e-g-plan-group-by-for-array-sources-plan-decs-group-by-for-the-decs-bridge-share-hundreds-of
title: When two linq_fold splice planners (e.g. plan_group_by for array sources + plan_decs_group_by for the decs bridge) share hundreds of LOC of state-table / dummy / reducer / having emit machinery, what's the right way to factor the shared core so adding a third source surface stays cheap?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

Extract the shared logic into `plan_X_core(calls, ..., adapter)` taking a small struct that encapsulates the **per-source differences**. The two surfaces (array `each(arr)` / decs `from_decs_template(type<T>)`) differ in only two things: how the per-element body is wrapped in a source loop, and how the final statement list is wrapped into the call site. Everything else (recognizing reducers, addr-compare miss detection, dummy assignment, having predicate rewrite, hidden-slot synthesis, output buffer build) is identical and belongs in `_core`.

**Adapter shape (group_by example, PR #2775 commit `cb72db296`):**

```das
struct private GroupBySourceAdapter {
    initialBindName : string         // name of the per-element tuple inside the inner loop
    initialElemType : TypeDeclPtr    // type of that tuple (strip_const_ref'd)
    namePrefix      : string         // gensym prefix: "grp" (array) vs "decs_grp" (decs)
    isDecs          : bool           // branch flag

    // array-source fields (populated when !isDecs)
    arrayTop    : Expression?
    arraySrcName : string

    // decs-source fields (populated when isDecs)
    decsBridge  : DecsBridgeShape?
}
```

**Two dispatch helpers consume the adapter** — they're the ONLY places where source-shape leaks into `_core`:

```das
// 1) wrap body in a source iteration loop
def adapter_emit_source_loop(adapter : GroupBySourceAdapter; body : ExpressionPtr; at : LineInfo) : ExpressionPtr {
    if (adapter.isDecs) {
        // for_each_archetype(req, erq, $(arch) { build_decs_inner_for(bridge, build_decs_tup_bind(...), body, at) })
        ...
    } else {
        // for ($i(adapter.arraySrcName) in $e(adapter.arrayTop)) { body }
        ...
    }
}

// 2) wrap final stmt list as a value at the call site
def adapter_finalize_emission(var adapter : GroupBySourceAdapter; stmts : ExpressionPtr; retType : TypeDeclPtr; at : LineInfo) : ExpressionPtr {
    if (adapter.isDecs) {
        // qmacro(invoke($() : $t(retType) { ...stmts }))
        ...
    } else {
        // finalize_emission_stmts(topClone, srcName, ...) — note: clone arrayTop FIRST
        var topClone = clone_expression(adapter.arrayTop)
        ...
    }
}
```

**Two specific gotchas the refactor surfaces:**

1. **`finalize_emission_stmts` takes `Expression? -const`.** The adapter field `arrayTop` is read as const-ref from the struct, so you can't pass it directly — `error[10210]: argument expects mutable Expression?`. Wrap with `var topClone = clone_expression(adapter.arrayTop)` before the call. This clone is required anyway because `finalize_emission_stmts` consumes its `top` argument into the emitted tree.

2. **`extract_decs_bridge` result must be assigned to `var bridge` (not `let bridge`).** The adapter's `decsBridge` field is non-const; `let bridge = extract_decs_bridge(...)` produces an `Expression? const` that the typer rejects when initializing the struct field. Plain `var bridge = extract_decs_bridge(...)` fixes it.

**Why this factoring works:** the planner entry points (`plan_group_by`, `plan_decs_group_by`) become ~30 LOC each — recognize the chain shape, validate, build the adapter, hand off to `plan_group_by_core`. The shared core is ~310 LOC of reducer machinery. Adding a third source surface (e.g. SQL or some future iterator factory) is just: add fields to the adapter, add a branch in each of the two dispatch helpers, add the planner entry. The reducer / state-table machinery stays untouched.

**Net diff in PR #2775 commit `cb72db296`:** 393 deletions, 161 insertions = -232 LOC across `daslib/linq_fold.das`. Two planners collapsed from ~300 LOC each to ~30 LOC each. Test suites (1280 linq AOT + 245 decs AOT) green; group_by bench numbers unchanged (refactor preserved the emit shape exactly).

**When to extract — heuristic:** if two planners diverge only in source-loop wrap and final-emission wrap (the chain recognition, reducer logic, having handling, output build are byte-identical), and the duplicated body is >100 LOC, the adapter pattern wins. Below that the duplication is cheaper than the abstraction.

## Questions

- When two linq_fold splice planners (e.g. plan_group_by for array sources + plan_decs_group_by for the decs bridge) share hundreds of LOC of state-table / dummy / reducer / having emit machinery, what's the right way to factor the shared core so adding a third source surface stays cheap?
- What's the GroupBySourceAdapter pattern in daslib/linq_fold.das and why does it exist?
- How do I share the state-table / reducer emit machinery between plan_group_by and plan_decs_group_by without duplicating ~300 LOC?
- Why does `finalize_emission_stmts(adapter.arrayTop, ...)` fail with "argument expects mutable Expression?" inside the adapter dispatch?

## See also

- [[when-adding-a-decs-specific-splice-planner-e-g-plan-decs-group-by-to-the-linq-fold-cascade-where-does-it-go-relative-to-the-arra]] — cascade ordering trap (companion concern when adding the second-source planner in the first place)
- [[in-a-shared-linq-fold-splice-helper-that-runs-over-both-array-sources-each-arr-and-iterator-sources-from-decs-template-how-do-i]] — iterator vs array return-type dispatch lives inside `_core`
- [[group-by-first-key-wins-single-hash-tab-dummy-addr-compare]] — the reducer emit shape the core preserves

## Questions
- When two linq_fold splice planners (e.g. plan_group_by for array sources + plan_decs_group_by for the decs bridge) share hundreds of LOC of state-table / dummy / reducer / having emit machinery, what's the right way to factor the shared core so adding a third source surface stays cheap?
