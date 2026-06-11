# LINQ → TABLE — arc plan

Sibling of [LINQ.md](LINQ.md) / [LINQ_TO_DECS.md](LINQ_TO_DECS.md). Plan of record for
`table<K;V>` / `table<K>` as the 6th `_fold` source, plus the `to_table` sink.
Edited in-place as PRs land.

Status: **stage 5 committed** (join probe + table-lead joins; stage 4 = point-lookup folds,
ac441c4a0; stage 3 = `%linq!` table sources, 29d23baf6; stage 2 = TableAdapter + m7, 571fe879e;
stage 1 = `each_kv` builtin, 8751bb9ba).

Stage 5 findings:
- **`emit_array_join` generalized instead of a parallel `emit_table_join`**: the lead loop, bind
  name, and lead invoke-param spelling now come from the adapter (`wrap_source_loop` /
  `bind_name` / new `invoke_param_type` capability), so `TableAdapter.emit_join_hook` just routes
  to `emit_array_join` and the kv usage-pruner sees the whole probe body for free — a table-lead
  join touching only `c.value.*` walks `values(tab)` alone. Any future direct-return loop source
  joins the same way; decs/xml/json keep their own hooks (nested-callback walks).
- **srcB probe**: `join_srcb_table_call` (each_kv/keys over a table in the srcb slot) +
  `join_keyb_is_bare_key` (peeled keyb is bare `d.key` / bare set element) switch the emitter to
  `build_join_probe_pieces` — srcB binds the user's table (const param), no internal
  `table<KEY; array<TUPB>>`, no build loop; the per-A probe usage-prunes like the point lookup
  (count-no-where / key-only → `key_exists`, value shapes → by-ref bind off `tab?[k]`, whole-pair
  → kv tuple bind). Skipping keyb's per-B evaluation is unobservable (a bare field read is pure
  by construction — no `has_sideeffects` gate needed, unlike stage 4's X).
- **Shared per-pair core**: `build_join_pair_core` factored out of `build_join_standalone_pieces`
  (which keeps the group-join arm + bucket wrap); both builders emit identical per-pair
  statements, so hash-mode AST is unchanged for the decs/xml/json callers of the standalone
  builder. Group joins never probe — their result consumes the whole bucket.
- The `_join` predicate splitter is **position-based** (`<a-side> == <b-side>`); a flipped
  `d.key == a` fails to compile for any source (pre-existing). The probe matcher therefore only
  sees keyb on the b-side.
- m7 (2026-06-11 sweep): table-lead joins leave tier-2 — `join_count` 195.0 → 65.6 ns/elem
  INTERP (33.1 JIT), `join_where_count` 229.1 → 81.4 (37.9 JIT). The probe A/B pair:
  `join_probe` 47.3 vs `join_probe_build` 79.1 INTERP (24.2 vs 38.1 JIT) — skipping the
  internal hash is ~1.7× on identical rows.

Stage 4 findings:
- `try_table_point_lookup` (linq_fold_table.das) runs in the dispatcher arm BEFORE pattern dispatch;
  shapes per plan — where(key==X)+any/count/first/first_or_default(±select), predicate-form
  any(p)/count(p), keys-lane contains — all emit through `TableAdapter.wrap_invoke` (probe inside
  the same 1-param const-table invoke as the walks).
- **Invariance alone is not enough**: X must also be side-effect free (`has_sideeffects`) — the scan
  evaluates X per element, a probe once; an impure X (e.g. a counter bump) would change observable
  behavior. Covered by a regression test asserting per-element evaluation is preserved.
- Table safe-index `tab?[k]` is **unsafe** (31034 — the pointer dangles on rehash); the generated
  probe wraps it (the invoke never mutates the table). Deref after the null check is plain `*p`.
- Scan-semantics mirroring: `first` panics "sequence contains no elements"; `first_or_default`
  binds its default eagerly before the probe (same order as the early-exit lane / linq.das).
- `collapse_chained_wheres` runs before dispatch, so `where(key==X)|>where(p)` arrives as one
  `&&` body → correctly declined (compound predicates keep the scan). Conjunct extraction
  (probe + residual predicate on the probed element) is a named deferred edge below.
- m7 INTERP (2026-06-11 sweep): `point_lookup` 0.0 ns/elem (O(1) probe) vs `point_lookup_scan`
  (the same query forced through the walk via a second always-true where) at full scan cost.

Stage 3 findings:
- The untyped `from c in <src>` now emits the **1-arg `from_in(src)`** for every source (the reader
  can't tell an array from a table); FromInMacro dispatches at infer time — `table<K;V>` →
  `each_kv`, `table<K>` set → `keys`, anything else → `each` (arrays land on the identical fused
  emission as before, ast-verified). The `unsafe($c(...))` qmacro form puts `alwaysSafe` on the call
  itself (templates_boost `carry_tag_safe_flags`), so extractors/peel_each still see a bare ExprCall.
- **Call-macro reject/defer idiom**: macro_error + `return null` (the `_sql` idiom). Returning
  `call` after an error report-ast-changes every pass and churns to the 50-pass infer cap (30507).
  All FromInMacro rejects switched to null; the "source type is not inferred" arm (now also gated on
  `isAutoOrAlias`) doubles as the DEFER — errors clear per pass while other inference progresses
  (a local `var tab <- {...}` source reaches the visit before its own type settles), and only stick
  if the source genuinely never infers.
- A rejecting `from_in` leaves the chain head unresolved, so `_fold`'s "expecting linq expression"
  verify lands on the same generated line with the same cerr — the error report collapses the pair
  to ONE 50503 (`+1 more on this line`); failed-test `expect` counts are post-collapse.
- **Joins over tables already work on either side** (tier-2; the kv pair is that side's row) —
  tested both directions. Stage 5's probe will optimize the table-as-srcB case.
- The non-copyable-value gate composes through the reader unchanged: fused dispatch declines,
  tier-2 instantiates the real `each_kv`, one clean 31400.

Stage 2 findings:
- m7 INTERP profile (2026-06-10 sweep): pruned scans sit between array and XML — `sum_aggregate`
  13.4 ns/elem (array 2.1, XML 54.3, JSON 146.7), `contains_match` 6.6 via the keys-pruned walk,
  pure-select `count` hits the O(1) shortcut (0.0). Deferred markers: `groupby_count` 162.6 /
  `groupby_sum` 192.8 / `join_count` 195.0 / `join_where_count` 229.1 / `reverse_take` 58.7 —
  the tier-2 cells stages 4–5 erase.
- The qmacro grammar only allows `$i()` in the FIRST iterator slot of a multi-source `for` — the
  kv zip header uses literal `_tab_kv_key_` / `_tab_kv_value_` names (ZipAdapter's itA/itB trade).
  Filed as [#3096](https://github.com/GaijinEntertainment/daScript/issues/3096) (grammar fix
  and/or a templates_boost loop-builder helper).
- `keys()` yields NON-const elements (writable temp copies) — the engine-visible bind is a `let`
  rebind (workhorse copy, free); push_clone's `==const` composition needs it.
- `keys`/`each_kv` spell their element `-const` (iterator variance); the dispatcher clears
  `removeConstant` on the cloned types or `array<tuple<…> -const>` buffer spellings break
  push_clone unification.
- Bare `<src>.to_array()` is not a recognized chain for ANY source (only suffix variants like
  `where_to_array` exist) — a keys-snapshot needs an op in the chain. Shared engine edge.

**Branch strategy (Boris, 2026-06-10):** the ENTIRE arc stays on `bbatkin/linq-table-each-kv`
as stacked stage commits — no per-stage PRs. A major fixed-array rework is in flight on master;
merging that INTO this branch once (after it lands) beats making every rework merge fight this
work. Cut the PR only after the rework has landed and been merged in here. At that merge,
re-validate the `each_kv` dim-array-value reject overload and `auto(valT)[]` matching — fixed
arrays are exactly what is being reworked.

**Merge done (2026-06-11, after stage 5):** rework (#3095) merged in; one conflict in
`daslib/builtin.das` — master deleted the dim-array `values()` overloads (plain `auto(valT)`
now binds the whole `T[N]`), our `each_kv` block kept. Re-validation: `auto(valT)[]` in table
value position still matches dim-valued tables (the reject overload fires its 31400), and the
plain `each_kv` generic still does NOT match `table<K; V[N]>` (table-position generic matching
doesn't bind fixed-array values), so the explicit rejects remain the right design — without
them the dim case would be a cryptic 30341, not a workable path. The dim-array-valued each_kv
deferred edge is therefore engine-gated (table generic matcher), not ours. Gates green: full
INTERP 10965/10971 (0 failed, 6 skipped), AOT linq 1949/1949, JIT linq 1949/1949.

PR1 findings:
- **Pre-existing generator-lowering bug, fixed in PR1**: the yield-for lowering emitted
  `loop &&= _builtin_iterator_first(...)` per source — short-circuiting `first()` on later
  sources when an earlier one came up empty, while the end-of-loop path closes ALL sources.
  Closing a never-opened container iterator unlocks a container whose lock magic was already
  cleared → "table/array magic mismatch on unlock". Reachable before each_kv (any generator
  zipping two lockable containers, first one empty). Fix: `loop = first(...) && loop`
  (ast_generate.cpp), matching SimNode_ForWithIterator. Regression:
  `tests/language/generator_zip_empty.das`.
- `each_kv` needs explicit reject overloads for `table<K>` (void values → "iterate keys()
  instead") and dim-array values — the bare generic otherwise mis-binds (valT drops the dim)
  and cascades confusing errors from inside builtin.das.
- Tier-2 chain heads need `unsafe(each_kv(tab))` — same `[unsafe_outside_of_for]` contract as
  `each(arr)`; fused chains (PR2) rewrite the head before inference so the wrap disappears.
- builtin module documents via handmade RST (it is a `get_module` C++-flow module in das2rst),
  so each_kv has both `//!` in-source docs and a filled handmade file.

## Settled decisions

- **kv surface** = `kv.key` / `kv.value` named tuple, **read-only** (a by-value tuple has no
  write-through; in-place mutation stays the domain of `for (k, v in keys(t), values(t))`).
- **Pipe head** = `each_kv(tab)`; `keys(tab)` / `values(tab)` are recognized as table sources too.
- **`each_kv` is pure daslib** with a strict `can_copy` gate on the value type — no clone
  fallback, ever (a hidden per-element `clone` of an `array<…>` value is the exact sadness the
  gate bans). Matches existing language ergonomics: plain `insert` already concept-asserts
  `can_copy` on values ([builtin.das ~921](../../daslib/builtin.das)), so non-copyable-valued
  tables only arise via `insert_clone` / `tab[k] <- v`.
- **Uniform gate enforcement falls out free**: for non-copyable values `extract_table_source`
  returns null → the chain defers to tier-2 → the real `each_kv` instantiates → `concept_assert`
  fires (error 31400). One error source; deferral never silently changes semantics.
- Shape (probe-validated): two const/var overloads mirroring `keys`,
  `generator<tuple<key:keyT; value:valT> -const> capture(<- kit, <- vit)` zipping the two builtin
  slot-walk iterators. Multi-source `for` + `yield` works in *generators*; iterator
  *comprehensions* reject it ("can't yield from inside the block") — hence the generator form.
- No profiling pre-PR; straight to m7 bench lanes. Scan lanes before the join probe. Sink in
  this arc.

## Stage sequence (commits on this branch)

1. **`each_kv` in builtin.das** — the validated shape next to `keys`/`values`
   (`[unsafe_outside_of_for, nodiscard]`); das2rst "Containers" group; tests
   (`tests/language/table_each_kv.das` + `failed_` can_copy compile-fail); INTERP/AOT/JIT.
   Standalone value: a kv iterator for plain `for` loops.
2. **`TableAdapter` core (`daslib/linq_fold_table.das`) + m7.** `extract_table_source`
   name-matches `each_kv`/`keys`/`values` at the spine head, **type-gated on the arg being a
   table** (names too generic to trust bare). Three lanes: keys (by value), values (by ref),
   kv — `wrap_source_loop` emits `for (k, v in keys(t), values(t))`, `RowFieldFlattener`
   rewrites the field reads, and **usage pruning** drops to a keys-only / values-only
   single-iterator walk when the body touches one side (the table analog of XML field-pruning).
   Capabilities: `can_reserve_by_length` / `supports_direct_return` = true, `count_shortcut` →
   `length(tab)`, any/empty → `!empty(tab)`, `distinct` on keys/kv → identity (keys unique by
   construction; values-lane distinct stays real). `can_group_by`/`can_join` = false → tier-2.
   New `benchmarks/sql/table.das` with `<family>_m7` runners (fixture `table<int; Car>`;
   expected values order-insensitive — slot order ≠ insertion order), results.md re-sweep,
   linq_fold_patterns.rst rows, linq_fold.md module-layout update, fused-vs-tier-2 agreement
   tests.
3. **`%linq!` `from_in` arm.** `from kv in tab` → `each_kv(tab)` (table-typed value dispatch,
   no annotation needed — like arrays); set form `from k in s` over `table<K>` → `keys(s)`.
   linq_das.rst update.
4. **Point-lookup folds.** `where(kv.key == X)` + terminator, X loop-invariant:
   `any`/`contains` → `key_exists`, `first`/`first_or_default` (± trailing
   `select(kv.value…)`) → `tab?[X]` probe, `count` → `key_exists ? 1 : 0`; set-form
   `contains(x)` → `key_exists`. The table analog of the JSON const-key fold. m7 point-lookup
   bench lane vs linear scan.
5. **Join probe.** `emit_join_hook`: when srcB is `each_kv(tab)`/`keys(tab)` and the b-key
   selector is bare `kv.key`, probe the user's table instead of building the join's internal
   `table<key;…>`. Semantics are exactly inner-equi-join with unique B keys — which a das table
   guarantees. Bench vs the build-side baseline.
6. **`to_table` / `to_table_move` terminators.** Chain of `tuple<K;V>` (incl. kv elements) →
   `table<K;V>`; chain of bare hashable K → `table<K>` set. Selector-free — key/value shaping
   composes via a preceding `select(k => v)`, matching the existing `to_table` vocabulary over
   tuple arrays ([builtin.das ~1664](../../daslib/builtin.das)). Tier-2 generic in linq.das +
   fused insert-loop emit (reserve when count is known). Duplicate-key policy: das `insert`
   semantics (last-wins), documented — not C#'s throw.

End of arc: `skills/linq.md` + linq docs mention the table source.

## Risks / watch items

- **Mangler ICE 50609** (iterator element-const collision) — `each_kv` yields `-const` non-ref
  tuples; the known footgun lives in iterator-typed generic params on the tier-2 side;
  mitigation (const-qualify) is known.
- **Lock semantics unchanged**: fused loops use the same builtin iterators as hand code —
  mutating the table mid-chain panics exactly as today.
- `values()` on `table<K>` already concept-asserts, so set-form `each_kv` errors cleanly for
  free.

## Deferred edges (named, not built)

- **Point-lookup conjunct extraction**: `where(kv.key == X && <residual>)` (incl. the collapsed
  multi-where form) could probe and evaluate the residual on the probed element only. The matcher
  currently declines compound predicates; add when a real chain wants it.
- **Multiple-`from` (cross / SelectMany) over tables**: the unfused `_cross_join` arm passes the
  bare source text so the array×array overload resolves without an `each` unsafe trip; a table
  there has no overload (confusing 30303 cascade). `cross_join` has iterator overloads, so routing
  the unfused untyped sources through `from_in` would work — but it changes overload selection for
  every existing untyped array cross query. Documented as unsupported (join a table instead);
  revisit on demand.
- **Key-as-handle deferred materialization**: for `order_by` over kv with large (copyable)
  values, buffer `(orderKey, key)` surrogates and materialize survivors via `tab?[key]` — K
  probes instead of N value copies. The table handle is its key; clean fit for the existing
  4-hook surface. Revisit once m7 numbers show whether it matters.
- **decs/xml/json lead × table srcB probe**: those leads keep their own `emit_join_hook`
  (nested-callback walks) and hash a table srcB like any iterator. Correct, just unprobed —
  port `build_join_probe_pieces` into their hooks if a real chain wants it.
- **Group-join probe**: a table srcB group join could bind a 0/1-element bucket from the probe
  instead of hashing; the result lambda consumes `array<B>`, so it needs a synthesized
  one-element array per hit. Hashed build is correct; revisit on demand.
- Set-ops probe (`except`/`intersect` where the *other* side is a `table<K>`) — rides the
  engine-wide set-ops edge.
- Fused-kv-over-non-copyable values (loosening the uniform gate) — only if a real use case
  begs.
- Dim-array-valued tables (`table<K; V[N]>`) in `each_kv` — `keys`/`values` carry dedicated
  overloads; add an `each_kv` one only on demand.
