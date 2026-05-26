# linq_fold.das refactor — masterplan

Living document. Update **Status** + **Decision log** as phases ship.

## Status

- [x] **PR A** — Foundation + first migrations (plan_reverse, plan_distinct) — branch `bbatkin/linq-fold-patterns-foundation`
- [x] **PR B1** — KR-1 closure (`collapse_chained_wheres`) + `c_chain` cardinality + `Captures` wrapper struct + `plan_loop_or_count` migration — branch `bbatkin/linq-fold-pattern-table-prb`
- [ ] **PR B2** — `plan_order_family` migration (5 emit archetypes + 5 rows) — deferred follow-up; foundation (aliases / predicates / c_chain) shipped in B1
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

Visibility follows the rule in **Tests / exports philosophy** below: default `private`, public only what walker tests must name (`Slot` / `SlotMatcher` / `SlotCardinality` + their `m_*`/`c_*` constructors, `SplicePattern`, `Captures` / `EmitCtx` / `EmitFn` typedefs, `chain_prefix_of`, `check_pattern_table_reachable`, `alias_table`). The snippet below omits the `private` keyword for readability; the implementation in `linq_fold.das` carries it on everything not in that public list.

```das
variant SourceAdapter {
    Array        : tuple<Expression?; string>   // (top, srcName) — PR A scope
    // PR C widens: Decs(DecsBridgeShape), DecsFind(DecsBridgeShape)
    // PR D widens: Zip(...), DecsJoin(...)
}

variant SlotMatcher {
    literal      : string                       // exact name match
    one_of       : array<string>                // any-of name set
    alias        : string                       // named group looked up in alias_table
}

variant SlotCardinality {
    one          : void?                        // required, exactly 1
    optional     : void?                        // 0 or 1
    chain        : void?                        // 0 or more (greedy); captures as array<ExprCall?> via Captures.many — PR B1
}

struct Slot {
    matcher      : SlotMatcher
    cardinality  : SlotCardinality
    capture_name : string = ""                  // "" = don't capture
    arity        : int = -1                     // -1 = any; positive = require N args
}

// PR B1 — Captures is a wrapper struct: `single` for c_one/c_opt slots, `many` for c_chain slots.
struct Captures {
    single : table<string; ExprCall?>
    many   : table<string; array<ExprCall?>>
}

variant MatchResult {
    no_match : void?                            // daslang-idiomatic Option<Captures>
    matched  : Captures
}

typedef RequiresPredicate = function<(var c : Captures; var top : Expression?) : bool>

// Fold-time context passed to every emit archetype. Carries the peeled source expression, source adapter,
// and the outer `_fold(...)` expression's iterator-ness (drives `buffer_return` wrap).
struct EmitCtx {
    top              : Expression?              // peel_each'd; stubs pre-clone per invoke
    src              : SourceAdapter
    expr_is_iterator : bool
}

typedef EmitFn = function<(var c : Captures; var ctx : EmitCtx; at : LineInfo) : Expression?>

struct SplicePattern {
    name     : string                           // for debug / lint diagnostics
    chain    : array<Slot>
    requires : array<RequiresPredicate>         // all must hold
    emit     : EmitFn
}

var plan_reverse_patterns : array<SplicePattern>
var plan_distinct_patterns : array<SplicePattern>
var plan_loop_or_count_patterns : array<SplicePattern>   // PR B1
var splice_patterns : array<SplicePattern>     // PR D: collapsed from per-plan tables; first match wins
```

Predicates and emit archetypes are NAMED module-level `def` functions wrapped at use sites with `@@<RequiresPredicate>` / `@@<EmitFn>` (anonymous `@@(...)` lambdas produce `_localfunction_*` symbols that the LLVM JIT pass can't resolve — named functions take a stable address).

### Walker contract

```das
def match_pattern(p : SplicePattern;
                  var calls : array<tuple<ExprCall?; LinqCall?>>;
                  var top : Expression?) : MatchResult
```

Walks `calls` left-to-right. For each slot:

- `one` — current call must match (name + arity if specified); both cursors advance.
- `optional` — if current call matches, both cursors advance; otherwise the slot is skipped without consuming.
- `chain` (PR B1) — greedy match-while-in-set. Captured as `array<ExprCall?>` into `captures.many[capture_name]`. Always succeeds (0+); empty match still creates the `many` entry so emit fns can rely on `c.many |> key_exists("…")`. Pairs with `m_one_of` via the `slot_chain_of(names, cap)` convenience constructor.

After all slots, no unconsumed calls remain. If any of the above fails → `MatchResult(no_match = null)`.

Then each `RequiresPredicate` in `p.requires` is evaluated against the populated `Captures` and the peeled `top`. All must return true. If any fails → `MatchResult(no_match = null)`.

Returns `MatchResult(matched <- captures)` on full success (move semantics — `Captures` is a table). Caller binds `var r <- match_pattern(...)` and reads via `if (r is matched) { let c & = r as matched; … }`.

### Alias table (named op-name groups)

The snippet below is the projected end-state at PR D. The authoritative live list is the `alias_table` literal in [daslib/linq_fold.das](linq_fold.das). Status reflects what's populated through PR B1.

```das
// projected end-state at PR D
var alias_table : table<string; array<string>> <- {
    "order_family"               => ["order", "order_descending", "order_by", "order_by_descending"],  // PR B1 ✓
    "distinct_family"            => ["distinct", "distinct_by"],                                       // PR A ✓
    "first_family"               => ["first", "first_or_default"],                                     // PR A ✓
    "count_family"               => ["count", "long_count"],                                           // PR A ✓
    "accum_family"               => ["sum", "min", "max", "average", "aggregate",                      // PR B1 ✓
                                     "min_by", "max_by", "min_max", "min_max_by",
                                     "min_max_average", "min_max_average_by", "long_count"],
    "early_exit_family"          => ["any", "all", "contains", "first", "first_or_default"],           // PR B1 ✓
    "range_op_family"            => ["skip", "skip_while", "take_while", "take"],                      // PR B1 ✓
    "loop_terminator_family"     => union of count + accum + early_exit + last/single/element_at,      // PR B1 ✓ (loop_or_count terminator slot)
    "distinct_terminator_family" => ["count", "long_count", "sum"]                                     // PR A ✓ — narrow to terminators emit_hashtable_dedup actually handles
}
```

### Predicate library

Module-level named `RequiresPredicate` constants for reuse across patterns. As with `alias_table`, this table shows the projected end-state — see [daslib/linq_fold.das](linq_fold.das) for what's actually defined today (PR A: `array_source`, `take_arg_is_int`, `no_terminator`).

| Name | Status | Meaning |
|---|---|---|
| `array_source` | PR A ✓ | `top._type.isGoodArrayType \|\| top._type.isArray` (after `peel_each`) |
| `array_random_access` | planned | `array_source && top._type.isGoodArrayType` |
| `decs_source` | planned | `extract_decs_bridge(top) != null` |
| `inline_cmp_available` | PR B1 ✓ | `try_make_inline_cmp` succeeds on `c.single["order"]` (only `order_by[_descending]` with inline-splice-able key). Hard-wired to `"order"` capture key; promote to factory on second use |
| `has_where_or_distinct` | PR B1 ✓ | `c.single \|> key_exists("where") \|\| c.single \|> key_exists("distinct")`. For `order_fused_prefilter` row to distinguish from bare `buffer_helper_dispatch` |
| `take_arg_is_int` | PR A ✓ | captured `take`'s 2nd arg `_type.baseType == Type.tInt`; vacuous if no `take` slot |
| `arity_eq(cap, n)` / `arity_ge(cap, n)` | planned (factory) | structural checks on captured calls |
| `no_terminator` | PR A ✓ | no terminator captured (final optional slot empty); return shape decided by `ctx.expr_is_iterator` in the emit fn |
| `is_primitive_key(cap, argIdx)` | planned (factory) | `is_primitive_join_key_type` on captured arg |

PR A's `take_arg_is_int` is hard-wired to the `"take"` capture key (not a factory) because every PR A consumer uses that name. Promote to a `make_arity_eq(cap, n)` / `make_arg_type_is(cap, idx, type)` factory shape on second use, per the masterplan rule of thumb.

Inline closures (`@@(c, top) => …`) acceptable for one-off pattern-specific checks. **Rule of thumb:** promote to named predicate (or factory) on second use.

## Migration phases

| PR | Phase | Scope | Status |
|---|---|---|---|
| **A** | 0 — Foundation | Kernel types + walker + alias_table + predicate library + per-archetype unit tests. `splice_patterns` empty initially (safe state — all cascades unchanged). | complete |
| **A** | 1 — First migrations | `plan_reverse` (5 rows: Ra/Rb/R6/R-2a/R1-R4), `plan_distinct` (2 rows + return-shape switch in emit). Archetypes: `emit_counter_array`, `emit_walk_overwrite_scalar`, `emit_backward_walk`, `emit_buffer_reverse_inplace`, `emit_hashtable_dedup`. **Hard-delete imperative bodies.** | complete |
| **B1** | 2a — Array core (`plan_loop_or_count`) | `c_chain` cardinality + `Captures` wrapper struct (`single` / `many`) + `slot_chain_of(names, cap)` constructor. `collapse_chained_wheres` pre-pass (KR-1 fix). `plan_loop_or_count` migration (1 row + lane dispatch — preserves existing factoring; head c_chain matches `["where_", "select"]` greedy). | complete |
| **B2** | 2b — Array core (`plan_order_family`) | `plan_order_family` (5 rows: streaming-min / bounded-heap / fused-prefilter / buffer-helper-dispatch / order_then_plain_distinct). Archetypes: `emit_streaming_min`, `emit_bounded_heap`, `emit_fused_prefilter`, `emit_buffer_helper_dispatch`, shared `emit_terminal_select_project`. **Hard-delete imperative body.** | not started |
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

Default private; promote ONLY what a synthetic-input test must name. PR A's actual public surface is narrow:

- Slot construction: `Slot`, `SlotMatcher`, `SlotCardinality`, `m_literal`, `m_alias`, `c_one`, `c_opt`, `c_chain`, `slot_chain_of`
- Pattern row: `SplicePattern`
- Struct/typedefs used in test fn signatures: `Captures` (struct), `EmitCtx`, `EmitFn`
- Lint helpers tests assert on: `chain_prefix_of`, `check_pattern_table_reachable`
- `alias_table` (so tests can read which aliases populated)

Everything else stays private: the walker (`match_pattern`), the per-plan tables, the predicate library, all `emit_*` archetypes, all populate fns, `SourceAdapter` / `MatchResult` / `RequiresPredicate` / `LinqCall`. They're only called inside this module — bare names compose without cross-module visibility.

Per-archetype unit testing via direct calls is impractical anyway: emit fns are `[macro_function]` whose bodies contain `quote()` nodes the runtime can't lower (LLVM JIT bail). End-to-end behavioral tests carry the per-archetype coverage in `tests/linq/test_linq_fold_*` (each user chain exercises one or more archetypes through the splice).

## Naming (decided)

| Name | Role |
|---|---|
| `splice_patterns` | The master table — `array<SplicePattern>` |
| `SplicePattern` | Per-row struct |
| `Slot` | Chain slot |
| `SlotMatcher`, `SlotCardinality` | Variant types |
| `Captures` | Struct `{ single : table<string;ExprCall?>; many : table<string;array<ExprCall?>> }`. `single` for c_one/c_opt slots, `many` for c_chain slots. The `LinqCall` record is accessible separately via `linqCalls[call_norm_name(c)]` |
| `MatchResult` | Variant `no_match : void? \| matched : Captures` — walker return type |
| `c_chain` / `slot_chain_of(names, cap)` | Greedy run cardinality + (matcher = m_one_of(names), cardinality = c_chain()) convenience constructor — PR B1 |
| `RequiresPredicate`, `EmitFn` | Function-typedef types — see kernel snippet for current signatures |
| `EmitCtx` | Struct `{ top; src; expr_is_iterator }` passed to every emit archetype |
| `SourceAdapter` | Source-loop abstraction variant |
| `alias_table` | Named op-name groups |
| `match_pattern(...)` | Walker function |
| `plan_<X>_patterns` | Per-plan filtered subset (only during migration; deleted in PR D) |

## PR B1 (shipped) + PR B2 (planned) sketch

### PR B1 — shipped

**Branch:** `bbatkin/linq-fold-pattern-table-prb`

**Scope (delivered):** KR-1 closure (`collapse_chained_wheres`) + `c_chain` cardinality + `Captures` wrapper struct (`single` / `many`) + `slot_chain_of(names, cap)` constructor + `plan_loop_or_count` migration (1 row, replaces 210 LOC imperative). PR A's 6 emit fns + 5 predicates mechanically migrated to `c.single[…]` (~47 sites).

### PR B2 — planned

**Scope:** `plan_order_family` migration (5 rows). All foundation (aliases / predicates / `c_chain`) shipped in B1; B2 is row + emit-archetype work only.

### Pre-pass (PR B1 ✓)

- `collapse_chained_wheres(calls)` — mirrors `collapse_chained_selects` modulo composition: clone inner where lambda, rename param to fresh name, build composed body via `merge_where_cond(innerBodyFresh, outerBodyFresh)` (which is `inner && outer`), rewire chain backlink, erase inner from `calls`. **No `has_sideeffects` bail** — composition uses cloned ASTs and AND-merge preserves left-to-right evaluation order with short-circuit semantics identical to the imperative cascade. Called from `plan_reverse`, `plan_distinct`, `plan_loop_or_count` stubs. **KR-1 fix; load-bearing for plan_loop_or_count row.**

### Pattern row shipped (PR B1)

**`plan_loop_or_count`** — 1 row using the new `c_chain` cardinality for the head:

```das
SplicePattern(
    name = "loop_or_count_general",
    chain = [
        slot_chain_of(["where_", "select"], "head"),                   // c_chain — 0+ contiguous where/select
        Slot(m_literal("skip"),       c_opt(), "skip"),
        Slot(m_literal("skip_while"), c_opt(), "skip_while"),
        Slot(m_literal("take_while"), c_opt(), "take_while"),
        Slot(m_literal("take"),       c_opt(), "take"),
        Slot(m_literal("where_"),     c_opt(), "post_take_where"),     // Theme 2 5c
        Slot(m_alias("loop_terminator_family"), c_opt(), "term")
    ],
    requires = [],   // intrinsic — chain shape carries the constraints
    emit = @@<EmitFn> emit_loop_or_count_lane)
```

`emit_loop_or_count_lane` walks `c.many["head"]` left-to-right applying the same where_/select arms (AND-merge, chained-select rebinding, where-after-select projection-replace) the imperative loop did. Range ops + post-take-where + terminator come from `c.single[…]`. Pre-dispatch fast paths: `emit_length_shortcut`, `emit_any_empty_shortcut`. Lane dispatch: `classify_terminator(call_norm_name(c.single["term"]))` → `emit_counter_lane` / `emit_array_lane` / `emit_accumulator_lane` / `emit_early_exit_lane`. `emit_array_lane` refactored to take `isIter : bool` directly (was `expr : Expression?` just to read `.isIterator`) so the new emit fn can pass `ctx.expr_is_iterator` cleanly.

### Predicates added (PR B1 ✓)

- `inline_cmp_available(c, top)` — `try_make_inline_cmp(c.single["order"].arguments[1], …)`. For PR B2's `order_streaming_min` + `order_bounded_heap` rows.
- `has_where_or_distinct(c, top)` — `c.single |> key_exists("where") || c.single |> key_exists("distinct")`. For PR B2's `order_fused_prefilter` row.

### PR B2 — planned rows

**`plan_order_family`** — 5 rows, priority order 1 → 5:

```das
// Row 1 — streaming_min: inline-cmp + first[_or_default]
SplicePattern(
    name = "order_streaming_min",
    chain = [
        Slot(m_literal("where_"),       c_opt(), "where"),
        Slot(m_alias("distinct_family"), c_opt(), "distinct"),
        Slot(m_alias("order_family"),    c_one(), "order"),
        Slot(m_alias("first_family"),    c_one(), "term"),
        Slot(m_literal("select"),        c_opt(), "termsel"),
    ],
    requires = [@@<RequiresPredicate> inline_cmp_available],
    emit = @@<EmitFn> emit_streaming_min)

// Row 2 — bounded_heap: inline-cmp + take(N)
SplicePattern(
    name = "order_bounded_heap",
    chain = [
        Slot(m_literal("where_"),       c_opt(), "where"),
        Slot(m_alias("distinct_family"), c_opt(), "distinct"),
        Slot(m_alias("order_family"),    c_one(), "order"),
        Slot(m_literal("take"),          c_one(), "take"),
        Slot(m_literal("select"),        c_opt(), "termsel"),
    ],
    requires = [@@<RequiresPredicate> inline_cmp_available, @@<RequiresPredicate> take_arg_is_int],
    emit = @@<EmitFn> emit_bounded_heap)

// Row 3 — fused_prefilter: where or distinct present, no inline-cmp shortcut
SplicePattern(
    name = "order_fused_prefilter",
    chain = [
        Slot(m_literal("where_"),       c_opt(), "where"),
        Slot(m_alias("distinct_family"), c_opt(), "distinct"),
        Slot(m_alias("order_family"),    c_one(), "order"),
        Slot(m_literal("take"),          c_opt(), "take"),
        Slot(m_alias("first_family"),    c_opt(), "term"),
        Slot(m_literal("select"),        c_opt(), "termsel"),
    ],
    requires = [@@<RequiresPredicate> has_where_or_distinct, @@<RequiresPredicate> take_arg_is_int],
    emit = @@<EmitFn> emit_fused_prefilter)

// Row 4 — buffer_helper_dispatch: bare order, direct call to daslib helpers
SplicePattern(
    name = "order_buffer_helper_dispatch",
    chain = [
        Slot(m_alias("order_family"), c_one(), "order"),
        Slot(m_literal("take"),       c_opt(), "take"),
        Slot(m_alias("first_family"), c_opt(), "term"),
    ],
    requires = [@@<RequiresPredicate> take_arg_is_int],
    emit = @@<EmitFn> emit_buffer_helper_dispatch)

// Row 5 — order_then_plain_distinct: `order + distinct (plain)` accepted by master imperative
// (whole-tuple equality is position-invariant). distinct_by AFTER order_by would NOT be safe
// (distinct_by picks an arbitrary K1 representative regardless of sort order). distinct is
// literal "distinct" only (no alias) to forbid distinct_by here.
SplicePattern(
    name = "order_then_plain_distinct",
    chain = [
        Slot(m_alias("order_family"), c_one(), "order"),
        Slot(m_literal("distinct"),   c_one(), "distinct_after"),
        Slot(m_literal("take"),       c_opt(), "take"),
        Slot(m_alias("first_family"), c_opt(), "term"),
        Slot(m_literal("select"),     c_opt(), "termsel"),
    ],
    requires = [@@<RequiresPredicate> take_arg_is_int],
    emit = @@<EmitFn> emit_fused_prefilter)   // reuses emit_fused_prefilter with distinct_after capture
```

### Emit archetypes

| Name | Source lines | LOC | Notes |
|---|---|---|---|
| `emit_streaming_min` | 1662-1727 | ~65 | Single-best state, per-element less-test |
| `emit_bounded_heap` | 1729-1855 | ~125 | Size-N heap during walk; distinct gate variant (Theme 3 Phase 3); terminal `_select` variant |
| `emit_fused_prefilter` | 1928-2107 | ~180 | Walk into buffer with where/distinct gate; sort/min/top_n on buffer; terminal `_select` variant; internal dispatch on take/first/bare |
| `emit_buffer_helper_dispatch` | 1857-1927 | ~70 | Direct call to `order` / `top_n*` / `min_max` helpers; 4 sub-paths |
| `emit_loop_or_count_lane` | 2243-2317 + recognition state | ~150 | Single row with internal `classify_terminator` dispatch into 4 existing lane emit fns |
| `emit_terminal_select_project` | NEW shared helper | ~30 | Used by `emit_bounded_heap` + `emit_fused_prefilter` for `outBuf` projection-from-`buf` |

### Co-occurrence audit (to verify during implementation)

The imperative code has a few subtle co-occurrence rules that may not map cleanly onto the pattern table:

- **`order + distinct (plain)`**: imperative `plan_order_family` accepts `distinct` (not `distinct_by`) AFTER `order_by` because whole-tuple equality is position-invariant. **Decision (2026-05-26)**: add row 5 `order_then_plain_distinct` so PR B has byte-equivalent splice coverage to master. The row's `distinct_after` slot is `m_literal("distinct")` (not the alias), structurally forbidding `distinct_by`. Emit reuses `emit_fused_prefilter` with the new capture name.
- **`select` mid-chain in plan_loop_or_count**: chained selects need `intermediateBinds` for side-effect ordering. Already handled by emit-fn-internal recognition; pattern row captures via single `select` slot post-collapse.
- **`where` after `select` in plan_loop_or_count**: imperative does `peel_lambda_replace_var(predicate, projection)` to rebind the where pred to the projection result. Critical correctness — emit fn must replicate (the recognition state in `emit_loop_or_count_lane` covers this).

### LOC budget

| Component | Delta |
|---|---|
| New: `collapse_chained_wheres` | +30 |
| New: 5 emit archetypes (lifted from imperative) | +610 |
| New: shared `emit_terminal_select_project` | +30 |
| New: 5 pattern rows | +60 |
| New: 2 populate `[_macro]` fns | +30 |
| New: 2 predicates + 5 aliases | +25 |
| Delete: imperative `plan_loop_or_count` body | -208 |
| Delete: imperative `plan_order_family` body | -543 |
| New: stubs + KR-1 wiring | +30 |
| New: tests (`collapse_chained_wheres` + per-archetype + regression) | +200 |
| **Net** | **~+264 LOC** (refactor, code redistributed; tests dominate) |

### Test plan (additions to existing per-archetype + walker integrity)

- `test_linq_fold_collapse_chained_wheres.das` — N=2, N=3 chains; side-effect bail; on plan_reverse + plan_distinct + plan_loop_or_count + plan_order_family
- `test_linq_fold_order_streaming_min.das` — inline-cmp + first / first_or_default; with/without where; with/without distinct; with terminal `_select`
- `test_linq_fold_order_bounded_heap.das` — inline-cmp + take(N); distinct gate; terminal `_select`
- `test_linq_fold_order_fused_prefilter.das` — where + order + take/first; distinct + order + take/first; terminal `_select`
- `test_linq_fold_order_buffer_helper.das` — bare order; order + take; order + first
- `test_linq_fold_loop_or_count_terminators.das` — all 4 lanes (counter / accumulator / early_exit / array); fast paths; range ops; chained wheres post-collapse

## Known regressions to address in follow-ups

| # | Surface | Symptom | Severity | Owner PR |
|---|---|---|---|---|
| KR-1 | `plan_reverse` + `plan_distinct` pattern rows allow a single optional `where_` slot; pre-PR-A imperative `plan_*` accepted N consecutive `where_` calls and `&&`-merged via `merge_where_cond`. | `..._where(p1)._where(p2).reverse()...` and `..._where(p1)._where(p2)._distinct()...` no longer spliced; fell back to cascade. | medium | **CLOSED in PR B1** — `collapse_chained_wheres` pre-pass mirroring `collapse_chained_selects` (~50 LOC + 18 sub-runs). Called from `plan_reverse` / `plan_distinct` / `plan_loop_or_count` stubs; will be called from `plan_order_family` in PR B2. |

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
- **2026-05-25** — `Captures` as `table<string; ExprCall?>`; emit fns reach the `LinqCall` registry record on demand via `linqCalls[call_norm_name(c)]` (avoids carrying the per-call pair in every capture entry). Initial sketch carried `tuple<ExprCall?; LinqCall?>` but it bought nothing — emit fns mostly read terminator/call arg shape from the `ExprCall`, and `call_norm_name` is the canonical way to derive the registry key anyway.
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
- **2026-05-26 (PR A R3)** — Intentional extension over master in `plan_reverse`: chains with BOTH a pre-reverse `_select(f)` AND a post-reverse `_select(g)` now splice (R1-R4 + Rb patterns) where master's imperative code had a `!seenSelect` guard that bailed to cascade. The two selects compose cleanly — pre-projection feeds `pushExpr`, post-projection projects the reversed survivors at return. Strictly faster, semantics preserved. Covered by `test_reverse_pre_and_post_select_array` / `_first` in `test_linq_fold_terminal_select.das`.
- **2026-05-26 (PR B1)** — Split PR B into B1 (KR-1 + `c_chain` + `plan_loop_or_count`) and B2 (`plan_order_family`). The c_chain cardinality is a kernel extension that's load-bearing for plan_loop_or_count's variable-shape head (`[where_*][select*]` interleaved); without it the row would explode into N positional optional slots and still not cover everything. Bundling kernel + first user of kernel in one PR keeps the kernel grounded.
- **2026-05-26 (PR B1)** — `Captures` migrated from `typedef Captures = table<string; ExprCall?>` to `struct Captures { single : table<string;ExprCall?>; many : table<string;array<ExprCall?>> }`. Alternatives considered: (a) overload one table with sentinel encoding — ugly and type-unsafe; (b) store all captures as `array<ExprCall?>` and index `[0]` for c_one/c_opt — fixed-shape callsites pay an awkward bracket tax. The split struct is mechanical for emit fns (`c["x"]` → `c.single["x"]`, ~47 sites swept) and leaves room for future cardinality types (`c_repeat_n`, etc.) to land in their own table.
- **2026-05-26 (PR B1)** — `c_chain` walker rule: empty match still creates an entry in `captures.many[name]` (empty array). Emit fns can rely on `c.many |> key_exists("…")` instead of branching on the array's length being > 0. Mirrors how `c_opt` slots that miss still leave `c.single |> key_exists` returning false — predictable existence semantics for emit-fn reads.
- **2026-05-26 (PR B1)** — `slot_chain_of(names, cap)` convenience constructor takes `var names : array<string>` and moves it into the SlotMatcher via `<-`. `array<string>` is non-copyable; pass-by-value-and-copy would require an explicit clone. Move-consume is the more honest signature.
- **2026-05-26 (PR B1)** — `collapse_chained_wheres` does NOT gate on `has_sideeffects` (whereas `collapse_chained_selects` does for one specific case). Reason: AND-composing two `where_` predicates preserves left-to-right short-circuit semantics — `inner(x) && outer(x)` evaluates `inner` first and short-circuits, identical to the imperative `if(inner) { if(outer) { … } }` cascade. Side effects in `inner` always fire (per element); side effects in `outer` fire only when `inner` returns true. Cascade and composition match exactly.
- **2026-05-26 (PR B1)** — `loop_terminator_family` alias must include ALL terminators `classify_terminator` returns non-UNKNOWN for. First B1 cut missed `last`/`single`/`element_at` × `_or_default` (6 EARLY_EXIT terminators); matrix run caught it via `test_linq_fold_ast` "expected 1 for-loop, got 0" failures (terminator wasn't matching the alias → planner cascaded to tier-2 imperative which emits 2 loops). Single-line fix: extend the alias. Lesson: any new alias for a c_opt terminator slot needs an audit against `classify_terminator`'s domain.
- **2026-05-26 (PR B1)** — `emit_array_lane` signature refactored: `var expr : Expression?` → `isIter : bool`. The only thing the original `expr` parameter was used for was reading `expr._type.isIterator`. The new `EmitCtx.expr_is_iterator` already carries that bool, so the refactor flows cleanly. Single callsite update (imperative caller computed `expr._type != null && expr._type.isIterator` inline before the call).

## Open questions

- **Prefix-conflict lint pass** — in PR A scope or deferred? Lean PR A so it grows with the table.
- **`plan_zip` / `plan_decs_join` SourceAdapter shape** — defer until PR D scoping. They feel special-case.
- **Reducer-spec data table** — exact shape (miss/hit template per row) — design during PR D.
- **`SourceAdapter` method surface** — `wrap_per_element(body, allows_early_exit)` is the minimal contract. Whether `finalize(stmts, retType)` belongs on the adapter or stays as a separate `finalize_*_emission` family — decide during PR C.

## See also

- `doc/source/reference/linq_fold_patterns.rst` — user-facing splice-pattern reference (refreshed per arm-touching PR).
- `benchmarks/sql/linq_fold_chain_audit.md` — closed-out audit that drove Themes 1-8 (PRs #2851 / #2852 / #2857 / #2861 / #2862 / #2865 / #2866 / #2874 / #2875).
- `benchmarks/sql/results.md` — INTERP+JIT matrix refreshed per splice-touching PR.
