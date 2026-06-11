# LINQ → TABLE — arc plan

Sibling of [LINQ.md](LINQ.md) / [LINQ_TO_DECS.md](LINQ_TO_DECS.md). Plan of record for
`table<K;V>` / `table<K>` as the 6th `_fold` source, plus the `to_table` sink.
Edited in-place as PRs land.

Status: **stage 2 committed** (TableAdapter + m7; stage 1 = `each_kv` builtin, 8751bb9ba).

Stage 2 findings:
- m7 INTERP profile (2026-06-10 sweep): pruned scans sit between array and XML — `sum_aggregate`
  13.4 ns/elem (array 2.1, XML 54.3, JSON 146.7), `contains_match` 6.6 via the keys-pruned walk,
  pure-select `count` hits the O(1) shortcut (0.0). Deferred markers: `groupby_count` 162.6 /
  `groupby_sum` 192.8 / `join_count` 195.0 / `join_where_count` 229.1 / `reverse_take` 58.7 —
  the tier-2 cells stages 4–5 erase.
- The qmacro grammar only allows `$i()` in the FIRST iterator slot of a multi-source `for` — the
  kv zip header uses literal `_tab_kv_key_` / `_tab_kv_value_` names (ZipAdapter's itA/itB trade).
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

- **Key-as-handle deferred materialization**: for `order_by` over kv with large (copyable)
  values, buffer `(orderKey, key)` surrogates and materialize survivors via `tab?[key]` — K
  probes instead of N value copies. The table handle is its key; clean fit for the existing
  4-hook surface. Revisit once m7 numbers show whether it matters.
- Set-ops probe (`except`/`intersect` where the *other* side is a `table<K>`) — rides the
  engine-wide set-ops edge.
- Fused-kv-over-non-copyable values (loosening the uniform gate) — only if a real use case
  begs.
- Dim-array-valued tables (`table<K; V[N]>`) in `each_kv` — `keys`/`values` carry dedicated
  overloads; add an `each_kv` one only on demand.
