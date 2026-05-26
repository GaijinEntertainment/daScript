# linq_fold.das refactor — masterplan

Living document. Update **Status** + **Decision log** as phases ship.

## Status

- [x] **PR A** — Foundation + first migrations (plan_reverse, plan_distinct) — branch `bbatkin/linq-fold-patterns-foundation`
- [ ] **PR B** — Array core (plan_loop_or_count, plan_order_family)
- [ ] **PR C** — SourceAdapter + decs mirrors (plan_decs_reverse / _distinct / _order_family / _unroll)
- [ ] **PR D** — Group-by + special cases (plan_group_by family, plan_zip, plan_decs_join, reducer-spec data table)

## Goal

Split `_fold` splice machinery into two layers:

- **Pattern recognition** — declared via a data table (`splice_patterns`). Each row names a chain shape, a list of requires-predicates, and a target emit fn.
- **Code generation** — reusable emit archetypes parameterized by a `SourceAdapter` (array / decs / decs-find, plus future zip / decs-join variants).

Today's 13 `plan_*` functions (~3300 LOC across the file) re-implement the same boilerplate: `flatten_linq → declare 8-30 tracking flags → giant for-loop with if/elif on op name → ad-hoc co-occurrence guards → giant final bail → emit dispatch`. 70-80% of plan_* code is recognition state + co-occurrence checking, not codegen. Adding a splice arm means patching 1-3 plan_* cores at 5-8 sites each, hunting through if/elif walls.

End state: adding an arm = adding a row to `splice_patterns`. Coverage gaps surface as missing rows, not as negative bails buried at the bottom of a function.

Estimated savings: **~-1750 LOC** across the 4 PRs (~-25% of the file).

## Today's situation (input to the refactor)

13 `plan_*` functions cover all splice cases. From the census:

| Plan | LOC | Anchor | Emit shapes |
|---|---|---|---|
| plan_order_family | 543 | `order*` | 11 (3 archetypes × variants) |
| plan_loop_or_count | 208 | terminator dispatch | 6 (already lane-factored) |
| plan_reverse | 284 | `reverse` | 5 |
| plan_distinct | 223 | `distinct*` | 1 archetype × 7 returns |
| plan_group_by_core | 364 | (pre-stripped contract) | 8 + 12-arm reducer table |
| plan_group_by | 62 | `group_by_lazy` | delegate |
| plan_decs_unroll | 61 | decs + terminator | 7 (already dispatcher) |
| plan_decs_group_by | 103 | `group_by_lazy` + decs | delegate |
| plan_decs_order_family | 384 | `order*` + decs | ~12 (near-mirror of array sibling) |
| plan_decs_reverse | 288 | `reverse` + decs | 4 (near-mirror) |
| plan_decs_distinct | 254 | `distinct*` + decs | 1 × 4 × 2 (near-mirror) |
| plan_decs_join | 189 | `join` + 2 decs bridges | 1 × 4 (no array sibling) |
| plan_zip | 352 | `zip` | 8 |

Decs-side plans are near-mirrors of their array-side siblings modulo source-loop wrap. `GroupBySourceAdapter` (existing) is the proof-case: `plan_group_by_core` is fully source-agnostic, with the array-side and decs-side wrappers each ~60-100 LOC.

## Grammar kernel (lives inline at top of linq_fold.das)

```das
variant private SlotMatcher {
    literal      : string;         // exact name match
    one_of       : array<string>;  // any-of name set
    alias        : string;         // named group looked up in alias_table
}

variant private SlotCardinality {
    one          : void;           // required, exactly 1
    optional     : void;           // 0 or 1
}

struct private Slot {
    matcher      : SlotMatcher;
    cardinality  : SlotCardinality;
    capture_name : string = "";    // "" = don't capture
    arity        : int = -1;       // -1 = any; positive = require N args on the matched call
}

typedef Captures = table<string; tuple<ExprCall?; LinqCall?>>
typedef RequiresPredicate = function<(c : Captures; top : Expression?) : bool>
typedef EmitFn = function<(c : Captures; top : Expression?; src : SourceAdapter; at : LineInfo) : Expression?>

variant private SourceAdapter {
    Array        : tuple<Expression?; string>;   // (top, srcName) — PR A scope
    // PR C widens: Decs(DecsBridgeShape), DecsFind(DecsBridgeShape)
    // PR D widens: Zip(...), DecsJoin(...)
}

struct private SplicePattern {
    name         : string;         // for debug / lint diagnostics
    chain        : array<Slot>;
    requires     : array<RequiresPredicate>;  // all must hold
    emit         : EmitFn;
}

var private splice_patterns : array<SplicePattern>   // tried in declaration order; first match wins
```

### Walker contract

```das
def match_pattern(pattern : SplicePattern;
                  var calls : array<tuple<ExprCall?; LinqCall?>>;
                  top : Expression?) : Captures?
```

Walks `calls` left-to-right. For each slot:

- `one` — current call must match (name + arity if specified); both cursors advance.
- `optional` — if current call matches, both cursors advance; otherwise the slot is skipped without consuming.

After all slots, no unconsumed calls remain. If any of the above fails → null.

Then each `RequiresPredicate` in `pattern.requires` is evaluated against the populated `Captures`. All must return true. If any fails → null.

Returns the populated `Captures` (keyed by `capture_name`, omitting slots with empty capture_name) on full success, null otherwise.

### Alias table (named op-name groups)

```das
var alias_table : table<string; array<string>> <- {
    "order_family"          => ["order", "order_descending", "order_by", "order_by_descending"],
    "distinct_family"       => ["distinct", "distinct_by"],
    "first_family"          => ["first", "first_or_default"],
    "count_family"          => ["count", "long_count"],
    "accum_family"          => ["sum", "min", "max", "average"],
    "range_op_family"       => ["skip", "skip_while", "take_while", "take"],
    "any_terminator_family" => [/* full union — built incrementally */]
}
```

### Predicate library

Module-level named `RequiresPredicate` constants for reuse across patterns:

| Name | Meaning |
|---|---|
| `array_source` | `peel_each(top)` succeeds |
| `array_random_access` | `array_source && top._type.isGoodArrayType` |
| `decs_source` | `extract_decs_bridge(top) != null` |
| `inline_cmp_available(cap_name)` | `try_make_inline_cmp` succeeds on captured order op |
| `take_arg_is_int(cap_name)` | captured take's 2nd arg `_type.baseType == Type.tInt` |
| `arity_eq(cap, n)` / `arity_ge(cap, n)` | structural checks on captured calls |
| `implicit_to_array` | no terminator captured (final optional slot empty) |
| `is_primitive_key(cap, argIdx)` | `is_primitive_join_key_type` on captured arg |

Inline closures (`@@(c, top) => …`) acceptable for one-off pattern-specific checks. **Rule of thumb:** promote to named predicate on second use.

## Migration phases

| PR | Phase | Scope | Status |
|---|---|---|---|
| **A** | 0 — Foundation | Kernel types + walker + alias_table + predicate library + per-archetype unit tests. `splice_patterns` empty initially (safe state — all cascades unchanged). | not started |
| **A** | 1 — First migrations | `plan_reverse` (5 rows: Ra/Rb/R6/R-2a/R1-R4), `plan_distinct` (2 rows + return-shape switch in emit). Archetypes: `emit_counter_array`, `emit_walk_overwrite_scalar`, `emit_backward_walk`, `emit_buffer_reverse_inplace`, `emit_hashtable_dedup`. **Hard-delete imperative bodies.** | not started |
| **B** | 2 — Array core | `plan_loop_or_count` (1 row + lane dispatch — preserves existing factoring), `plan_order_family` (3-4 rows: streaming-min / bounded-heap / fused-prefilter / buffer-helper-dispatch). Archetypes: `emit_streaming_min`, `emit_bounded_heap`, `emit_fused_prefilter`, `emit_buffer_helper_dispatch`, shared `emit_terminal_select_project`. **Hard-delete imperative bodies.** | not started |
| **C** | 3 — SourceAdapter + decs mirrors | Widen `SourceAdapter` to multi-variant + methods. Migrate `plan_decs_reverse / _distinct / _order_family / _unroll` — **reuse array-side rows + emit fns** modulo adapter swap. **Hard-delete decs imperative bodies.** | not started |
| **D** | 4 — Group-by + special cases | Reconcile `GroupBySourceAdapter` with `SourceAdapter`. `plan_group_by` + `plan_decs_group_by` → thin pattern rows delegating to existing `plan_group_by_core` (which stays as a sub-codegen). `plan_zip` (1-2 rows, possibly `SourceAdapter::Zip`). `plan_decs_join` (1 row, `SourceAdapter::DecsJoin` or special-case emit). Migrate `emit_reducer_branches` 12-arm if/elif into a `ReducerSpec` data table. | not started |

**Net at end: ~-1750 LOC.**

## Migration mechanics

During migration, today's planner cascade at lines 6302-6337 stays unchanged. Each migrated `plan_X` function body becomes a 3-line stub:

```das
def private plan_reverse(var expr : Expression?) : Expression? {
    for (p in plan_reverse_patterns) {  // subset filtered by owner_plan_id
        let captures = match_pattern(p, calls, top)
        if (captures != null) return p.emit(captures, top, source_adapter, at)
    }
    return null
}
```

After PR D, collapse all stubs + cascade into one flat walk over `splice_patterns` (no `owner_plan_id` filtering needed once all are rows).

## What we KEEP from today's code

All shared helpers stay as building blocks for emit archetypes:

- `flatten_linq`, `peel_each`, `extract_decs_bridge`
- `normalize_order_reverse`, `collapse_chained_selects`
- `peel_lambda_rename_var`, `peel_lambda_replace_var`, `peel_lambda_rename_2vars`
- `merge_where_cond`, `wrap_with_condition`, `wrap_with_ranges`
- `try_make_inline_cmp`, `make_inline_less_call`
- `buffer_return`, `finalize_emission_stmts`, `finalize_decs_emission`
- `qn`, `clone_expression`, `clone_type`, `strip_const_ref`
- `has_sideeffects`, `type_has_length`, `classify_terminator`
- `plan_group_by_core` (stays as sub-codegen; only its wrapper plans migrate)
- `GroupBySourceAdapter` (PR D folds it into the new `SourceAdapter`)

## Tests / exports philosophy

Public surface (drop `private` annotations) for testability:

- `match_pattern(...)` walker
- Each `emit_*` archetype
- `SourceAdapter` constructors + methods (once they exist in PR C)
- `alias_table` (so tests can register synthetic patterns)

Per-archetype unit tests: call emit fns directly with synthetic `Captures` + `SourceAdapter`, AST-dump-assert the resulting Expression. Avoids going through the full `_fold` macro pipeline. End-to-end behavioral tests stay in `tests/linq/test_linq_fold_*`.

## Naming (decided)

| Name | Role |
|---|---|
| `splice_patterns` | The master table — `array<SplicePattern>` |
| `SplicePattern` | Per-row struct |
| `Slot` | Chain slot |
| `SlotMatcher`, `SlotCardinality` | Variant types |
| `Captures` | `table<string; tuple<ExprCall?; LinqCall?>>` typedef |
| `RequiresPredicate`, `EmitFn` | Function-typedef types |
| `SourceAdapter` | Source-loop abstraction variant |
| `alias_table` | Named op-name groups |
| `match_pattern(...)` | Walker function |
| `plan_<X>_patterns` | Per-plan filtered subset (only during migration; deleted in PR D) |

## Risks

1. **Pattern ordering hazard.** Pattern A's chain being a strict prefix of B's means A wins. Discipline: more-specific patterns declared first; add a lint pass that walks the table at module-init time and flags prefix conflicts.
2. **Hidden cross-plan helpers.** Some emit branches consume helpers used by other plans. These stay as-is — emit archetypes call them like the imperative code did.
3. **Mid-flight `SourceAdapter` redesign.** PR C is the highest-uncertainty phase. If `wrap_per_element` doesn't generalize cleanly to `DecsFind`, we either widen the interface or special-case. **Mid-flight redesign approved.**
4. **Bench refresh per [feedback-living-results-md]:** each PR re-runs INTERP+JIT for any bench shape it touches and refreshes `results.md`. Goal is **byte-identical or strictly faster** at each phase (refactor, not perf change).
5. **RST refresh per [feedback-living-linq-fold-patterns-rst]:** each promoted arm's row in `doc/source/reference/linq_fold_patterns.rst` gets touched — phrasing changes from "plan_X handles …" to "pattern `<name>` (archetype `<emit_fn>`) handles …".

## Decision log

- **2026-05-25** — Hybrid (declared patterns table + reusable archetypes) over pure EDSL or pure data-table.
- **2026-05-25** — Inline kernel in `linq_fold.das` (not a separate module file).
- **2026-05-25** — Bundle PR A foundation with first migrations (`plan_reverse`, `plan_distinct`) — foundation-only PR is grounded in nothing.
- **2026-05-25** — Stop and align between phases; mid-flight redesign approved.
- **2026-05-25** — Hard-cutover (no legacy imperative alongside the table).
- **2026-05-25** — `Captures` as `table<string; tuple<ExprCall?; LinqCall?>>`; emit fns use `is` / `as` / `?as` to dig out call shape.
- **2026-05-25** — `SourceAdapter` stub (Array-only) in PR A; widened in PR C.
- **2026-05-25** — Per-plan stubs during migration; flat walk after PR D.
- **2026-05-25** — `arity` lives on `Slot` (structural check), not requires-predicate.
- **2026-05-25** — Named predicates over inline closures by default; inline acceptable for one-off, promote on second use.
- **2026-05-25 (PR A impl)** — `let` is const, `var` is non-const. Walker stub binds `var result = invoke(p.emit, …)` to receive non-const Expression?. Lint LINT005 misreports the required reinterpret as redundant — known asymmetry; tracked via this entry instead of suppressing in code.
- **2026-05-25 (PR A impl)** — `ExprCall.name` is the mangled generic-instance name (e.g. `__::linq\`distinct_by\`<hash>`); the user-facing name lives at the root of the `func.fromGeneric` chain. Helper `call_norm_name(ExprCall?)` walks the chain and normalizes through `linqCalls`.
- **2026-05-25 (PR A impl)** — Variant construction in gen2 uses the named-field constructor form (`SlotMatcher(literal = "x")`, `MatchResult(matched <- captures)`), same syntax as struct init. Helpers `m_literal`/`m_alias`/`c_one`/`c_opt` keep pattern rows compact.
- **2026-05-25 (PR A impl)** — Empty typed array literal: `array<T>()`. The bare `[]` lacks the element-type inference base.
- **2026-05-25 (PR A impl)** — `array_source` predicate gates only patterns that need indexed access (R-2a backward-walk, R6 backward-index). Patterns using `for (it in src)` body (Ra / Rb / R1-R4 / distinct_*) work on iterator sources too and have no source-shape gate.
- **2026-05-25 (PR A impl)** — `take_arg_is_int` predicate is vacuously true when no `take` capture is present (so it's safe on patterns with optional take). Same pattern applies for any future capture-conditional predicate.
- **2026-05-25 (PR A impl)** — PR A is a pure refactor (no arm add/extend/tighten). Per `[[feedback-living-linq-fold-patterns-rst]]` and `[[feedback-living-results-md]]`, RST and bench refresh are skipped — both are arm-shape-tracking docs, not implementation-tracking docs.

## Open questions

- **Prefix-conflict lint pass** — in PR A scope or deferred? Lean PR A so it grows with the table.
- **`plan_zip` / `plan_decs_join` SourceAdapter shape** — defer until PR D scoping. They feel special-case.
- **Reducer-spec data table** — exact shape (miss/hit template per row) — design during PR D.
- **`SourceAdapter` method surface** — `wrap_per_element(body, allows_early_exit)` is the minimal contract. Whether `finalize(stmts, retType)` belongs on the adapter or stays as a separate `finalize_*_emission` family — decide during PR C.

## See also

- `doc/source/reference/linq_fold_patterns.rst` — user-facing splice-pattern reference (refreshed per arm-touching PR).
- `benchmarks/sql/linq_fold_chain_audit.md` — closed-out audit that drove Themes 1-8 (PRs #2851 / #2852 / #2857 / #2861 / #2862 / #2865 / #2866 / #2874 / #2875).
- `benchmarks/sql/results.md` — INTERP+JIT matrix refreshed per splice-touching PR.
