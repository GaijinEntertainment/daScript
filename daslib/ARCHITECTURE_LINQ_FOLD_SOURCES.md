# daslib architecture notes - the linq_fold source adapters

Companion to `ARCHITECTURE.md` in this folder; the kernel these adapters plug into -
dispatch, the walker, the adapter contract, the generic lanes - is
`ARCHITECTURE_LINQ_FOLD.md`.

## linq_fold_array

### Zip fusion {#linq-fold-array-zip}

- **Zip binds the pair `it` only where a lane reads it.** `ZipAdapter.wrap_source_loop`
  emits `for (itA, itB in srcA, srcB)` and binds only the two per-source loop variables. It
  shares their gensym names, `ZipShape.itAName` / `itBName`, with `emit_zip`. The pair bind
  `let it = (itA, itB)` belongs to `emit_zip`, because only `emit_zip` knows whether a
  `where_`, a projection, a while-guard or a count predicate reads `it`. The accumulator and
  early-exit lanes always get the bind as `preCondStmts`. The buffer lane gets it, and the
  counter lane's loop body starts with it, only when something reads `it`. A bare zip into a
  buffer pushes `(itA, itB)` directly, with no bind. `emit_zip` builds the `ZipAdapter` before
  it picks a lane, so every lane runs over the same adapter.
- **A trailing `reverse` changes only what the buffer lane returns.** That lane reverses the
  collected buffer in place (`reverse_inplace`) before it returns it.
  `zip_reverse_compatible_with_term` declines the row when the terminator is `first` or
  `first_or_default`, because their answer depends on order. Every other zip terminator
  ignores the reverse: `count`, `any`, `all`, `contains`, `min` and `max` do not depend on
  order, and `sum` / `average` change only by floating-point rounding.
- **A `select` inside a zip chain takes its element type from the projection lambda's return
  expression** (`peel_lambda_single_return`), not from the select call's `_type.firstType`:
  inside `_fold` that call type is a `typedecl(result_selector(...))` formula that does not
  resolve.

### Group-by over a join {#linq-fold-group-by-over-join}

- **A `group_by_lazy` directly over a `join` groups the joined pairs without collecting them
  first.** The group_by row's optional `upstream_join` slot captures the join. The source's
  `build_group_by_adapter` then returns a join adapter in place of itself: `ArrayJoinAdapter`
  over two arrays, `DecsJoinAdapter` over two decs bridges, `JsonJoinAdapter` over a json
  source. That adapter's `wrap_source_loop` is the hash join: it collects srcB, the join's
  second argument, into a key -> `array<tupB>` table, probes the table with srcA, the chain
  top, and binds each `resultLam` value to `resBindName`, which is also the adapter's
  `bind_name`. So `plan_group_by_core` runs its per-element body once per joined pair,
  through the ordinary adapter loop. `decs_join_invariants` admits the slot only when there
  is no head `where_` / `select`, no `having_` and no trailing `where_`; the table adapter
  declines it.

## linq_fold_decs

### Column pruning {#decs-column-pruning}

- **Column pruning is a four-step pipeline over the `decs_tup` bind, and every step exists
  to reach bind elision.** Scan the body for `decs_tup.<field>` uses; if the bind is
  referenced as a WHOLE var, rewrite each whole-var use into a synthesized
  `(userName1 = iter1, ...)` literal - semantically identical - and re-scan; then flatten the
  surviving field reads to the iter vars and drop the bind entirely. Unused `get_ro` slots
  disappear along with one tuple-make plus N field reads per iteration. The unpruned bind
  stays for whole-var refs that survive the rewrite, for a body that reads no field, and for
  a field name that no bridge component carries - there the bind keeps the typer's
  unknown-field diagnostic on the named tuple, where a dropped bind would report an unknown
  identifier.
- **The usage scan maps each bare iter var back to its component** (`iterToUser`), so a
  component that trivial-select elision already reads through its iter var keeps its slot
  when the others are pruned.
- **Trivial-select elision removes the copy a `_select(_.userName)` would emit.** The
  pruned inner-for already keeps that component's iter var, so the synthetic bind would
  flatten to a pure copy; renaming the chain's `finalBind` to the iter var skips the binding
  entirely. Detecting the shape needs TWO `ExprRef2Value` peels: the typer wraps both the
  projection root and the `ExprVar` inside the `ExprField`.

### Random index and the archetype order {#decs-random-index}

- **The decs random-index fast paths rest on these source facts.** A plain `[decs_template]`
  field compiles to `get_ro` (an indexable array) while a default-init field compiles to
  `get_default_ro` - an iterator, not indexable - so `decs_can_random_index` returns false
  and the chain keeps the walk. `get_ro` is `[unsafe_outside_of_for]`, so each cloned source AND the index
  read are marked `alwaysSafe` (mirroring `decs_boost`'s `append_index_lookup`).
  `for_each_archetype` visits archetypes in order and skips empty ones, so a no-predicate
  `last()` overwriting a result per archetype ends holding the global last in
  O(num_archetypes), and `reverse |> take(N)` collects only the tail with the boundary
  archetype's head indexed past rather than walked.
- **`emit_decs_reverse_skip_into_tail` walks twice.** The first pass sums `arch.size`, which
  fixes where the tail starts. The collect pass then steps over each archetype that lies
  wholly before the tail by its size, without entering it, collects forward from the
  boundary, and reverses the buffer in place. A terminal `_select` projects only the K
  collected elements, after the reverse.

### The terminator walk {#decs-terminator-walk}

- **`DecsAdapter` runs in two modes, keyed on `chainInfo`.** The adapter that the dispatcher
  and `build_group_by_adapter` build carries no `chainInfo`: the group_by and distinct cores
  hand `wrap_source_loop` a finished per-element body, which it only prunes and frames. The
  adapter that `make_decs_term_adapter` builds for a terminator lane carries `chainInfo`,
  `rangeInfo` and `calls`, and its `wrap_source_loop` owns the whole walk: the in-loop range
  guards, the `where_` / `select` binds, column pruning, the hoisted range prelude, and the
  `for_each_archetype{,_find}` framing. The generic lanes see one bind. `bind_name` answers
  `chainInfo.finalBind` itself instead of minting a `qn`, and the term adapter takes
  `chainInfo.tupName`, so the name the lanes peel against is the name the walk binds.
  Because the chain runs inside the walk, the counter lane's body only bumps the counter, and
  every `select` - a side-effecting one included - still runs once per element.
- **Every decs walk wraps a clone of the bridge's inner `for` in an archetype lambda whose
  parameter is `bridge.archName`.** The cloned `get_ro` sources read the archetype by that
  name. `build_decs_inner_for` and `build_decs_inner_for_pruned` reuse the bridge's `for` as
  it is, so the lambda around it binds the same name.
- **Every decs early stop goes through `for_each_archetype_find`.** The `take` cap and the
  `take_while` break that `wrap_inner_for_with_decs_ranges` emits stop with `return true`
  from the archetype lambda. Only the bool lambda of `for_each_archetype_find` ends the walk
  that way; the plain `for_each_archetype` lambda returns nothing. So `effective_dispatch`
  promotes `Each` to `Find` when the chain carries `take` or `take_while`, and
  `emit_decs_min_max_by` picks its dispatch by the same test.
- **`emit_loop_or_count_lane_decs` classifies terminators itself, not through
  `classify_terminator`.** `count` and `long_count` share the counter lane, and `min_by` /
  `max_by` take `emit_decs_min_max_by`. `classify_terminator` files `long_count` and the
  min/max-by family under the accumulator lane.
- **The decs emits that build an array decline an iterator-typed chain.** The implicit
  `to_array` arm of `emit_loop_or_count_lane_decs` and the non-count arm of
  `emit_decs_join_impl` emit an `array` and apply no iterator wrap, so a chain whose type is
  an iterator falls to tier 2 there. `emit_decs_reverse_skip_into_tail` is the decs emit that
  wraps its array back into an iterator, via `finalize_decs_emission`'s `wrapToIter`.

### The decs join {#decs-join}

- **The decs join hook keeps hash-collect and probe inline rather than routing through
  `wrap_source_loop`** - that is what preserves the count-no-`where` fast path, which
  answers from bucket lengths instead of walking pairs. Its key lambdas are synthesized
  upstream by the LinqJoin macro as 1-arg single-return blocks, so the BLOCK's return type
  is the key type; a change to that synthesized shape breaks the decs join silently.
- **A decs `group_by` over an upstream join takes the other path.** `build_group_by_adapter`
  builds a `DecsJoinAdapter` over a `DecsJoinShape`. Its `wrap_source_loop` hashes the B
  side, probes from A through `build_join_adapter_pieces`, and binds one result per matched
  pair - the granularity the group_by core consumes - so that path has no bucket-length
  shortcut to keep.

### Un-inferred ExprFor {#linq-fold-uninferred-for}

- **An `ExprFor` the typer has not re-inferred carries an EMPTY `iteratorVariables`.** A
  decs bridge feeding a no-chain terminator (`from_decs_template(...).count()`) has no chain
  op to force the second inference pass, so the iterator names are recovered from the push
  tuple's values - each references its iter var by name under an `ExprRef2Value` wrap - and
  match what the loop binds once a later pass infers it.

## linq_fold_json

### Source recognition {#linq-fold-json-source}

- **`extract_json_source` claims a chain only when the call is a two-argument `from_json`
  returning an iterator AND argument 0 is a `json::JsonValue?`, matched by the struct name
  `JsonValue` and the module name `json` as strings.** linq_fold_json is required
  unconditionally and pulls in no json dependency. A match on the call name alone would
  therefore claim an unrelated user `from_json` and emit `read_json_field` / `from_JV` calls
  that resolve to nothing.

### Row materialization {#linq-fold-json-materialize}

- **`build_json_materializer` picks the per-element row shape from what the chain body
  reads off the row bind `it`.** If the body reads nothing, it runs alone. If it reads named
  fields only, each field is read once into a `jf_<f>` local by `read_json_field`, with the
  field of the hoisted default row `jdef` as the missing-key fallback. The body's `it.<f>`
  reads are rewritten to those locals, and the row struct is never built. If the body uses
  the whole row, a struct row is built by name, field by field, through `read_json_field`
  (`emit_full_row_by_name`). A non-struct element converts through `from_JV`.
- **A whole-row use confined to the true branch of a bare `if` (no `else`) whose condition
  reads only named fields is materialized under the guard**
  (`try_json_under_guard_materialize`). The condition's fields are read into locals as on
  the pruned path. The full by-name row is built inside the true branch, so only matching
  elements build it. The guard reads use plain `read_json_field`: the JSON tree is
  heap-owned, so JSON needs no borrowed peek read next to it, unlike the XML adapter's
  `peek_xml_field`.
- **`JsonAdapter` defers materialization.** The handle is the walk's per-element cursor:
  `current_handle_expr` names `jcur`, the cursor `build_pruned_array_walk` binds.
  `handle_type` is `JsonValue const?` to match the const elements the walk reads off its
  const source parameter. `materialize_handle` builds a survivor by name, as the whole-row
  arm does.

### Walk and invoke {#linq-fold-json-walk}

- **The JSON walk is one flat `for` over `jsrc.value as _array`, guarded by
  `jsrc != null && jsrc.value is _array`** (`build_json_array_walk`). The unfused
  `from_json` in `json_boost.das` uses the same guard, so a null source or a non-array value
  is an empty sequence on both paths. The two guards are a pair, and nothing fails when one
  side moves alone. The walk has no name filter (the source IS the array, unlike XML's
  named-child form) and no second level (unlike decs's archetype walk or XML's DOM child
  while-walk). A `continue` emitted for skip / skip_while lands on the next element. For
  distinct + take, a `break if (guard)` leads the element body.
- **The fused invoke takes the JSON source as a const parameter: the source's type with ref
  stripped and const forced.** The source `jv` is often a `let` (a const `JsonValue?`), and
  a non-const argument converts to const cleanly, so one invoke shape binds either.
  `wrap_invoke` and the join invoke `build_json_join_invoke` both bind it this way.

### Joins {#linq-fold-json-join}

- **A standalone join over a JSON source is a hashed equi-join** (`JsonAdapter.emit_join_hook`).
  srcB, an array-shaped in-memory argument, is collected into a `table<key; array<row>>`.
  srcA, the JSON value, is then probed through `wrap_source_loop`, the same field-pruned
  walk a plain chain gets. The key-A and result lambdas are peeled onto the walk's row bind
  `it`, so the materializer prunes srcA to the fields they read. The shape mirrors
  `emit_array_join`, with the JSON value in place of an array parameter.
- **An iterator-typed join result (no count terminator) is built as the whole field-pruned
  result array and then wrapped by `to_sequence_move`.** The fused single-pass build
  measures faster than a streaming generator, and the wrap keeps the iterator contract.
- **`join |> group_by` over JSON runs through `JsonJoinAdapter`, which
  `JsonAdapter.build_group_by_adapter` returns on the upstream-join arm.** `plan_group_by_core`
  drives the bucket fill through `JsonJoinAdapter.wrap_source_loop`. That loop collects srcB
  into a hash and probes from the field-pruned array walk, so srcA gets the same field
  pruning with no join-specific code. It mirrors pugixml's `XmlJoinAdapter`.

## linq_fold_table

### The table source {#linq-fold-table-source}

- **The kv lane walks only the iterators the body reads.** `build_table_walk` scans the body
  for reads of the row binder. A body that reads only `it.key` walks `keys(tab)`, one that
  reads only `it.value` walks `values(tab)`, and one that reads both walks `keys` and
  `values` in parallel. Each field read becomes a read of that loop's own local. A body that
  uses the row as a whole, or reads any other field, gets the full `(key = k, value = v)` row
  over the parallel walk (`needs_full_row_bind`). A body that reads neither side, such as a
  bare count walk, walks `keys(tab)`, the cheaper iterator.
- **The table lane's key and value binds are asymmetric on purpose.** `keys(tab)` yields
  non-const elements (writable temp copies), so the key lanes rebind the loop var through a
  `let` before the body sees it; `values(tab)` over the const table param already yields
  `V& const` and binds directly. Keys are workhorse types, so the copy is free, and
  downstream `==const` composition (a `push_clone` of a bare projected key) needs the const.
- **`TableAdapter` emits one flat `for` over `keys` / `values` of the invoke's only
  parameter, the table.** No callback wraps that loop, so a `return` inside it leaves the
  invoke. Because the loop is flat, the table can use the shared lanes without
  table-specific code. A join whose lead is a table runs `emit_array_join`, which gets its
  probe loop from `wrap_source_loop`. `group_by` fills its buckets in `plan_group_by_core`,
  which also calls `wrap_source_loop`. Both therefore get the kv pruning. A `group_by` that
  follows a join over a table lead (the `upstream_join` capture) is declined:
  `build_group_by_adapter` returns null and the chain runs at tier 2.
- **For a table, `loop_source_expr` turns on the reserve hint and nothing that indexes.**
  `length(tab)` is O(1), so `can_reserve_by_length` answers true, and `type_has_length`
  accepts the table `loop_source_expr` returns, so the shared lanes reserve their output by
  the table's length. The backward-index reverse lanes index the source through
  `loop_source_name`, but they are gated on `array_source`. The dispatcher hands them the
  `each_kv` / `keys` / `values` call as the match top, and that call is iterator-typed, so
  the gate is false and those lanes never index a table.
- **The invoke's table parameter is always const.** `invoke_param_type` strips const and ref
  from the source's type, then sets `constant`. The source table is often a `let`, and a
  `let` binds only to a const parameter, while a non-const table converts to const cleanly.
  So one parameter type accepts every source.
- **`extract_table_source` matches on name, module and argument type.** It accepts `each_kv`
  / `keys` / `values` only when the call resolves to the `builtin` module, returns an
  iterator, and takes exactly one argument whose type is a table. The module and type checks
  stop a user function that is only named `keys` or `values` from being folded as a table
  walk.
- **A plain `distinct` over raw keys or kv rows is dropped before the splice.** Table keys
  are unique, so the dedup set never rejects anything. `drop_distinct_over_unique_keys`
  removes the first `distinct` only when every call before it keeps elements unique:
  `where_`, `skip`, `take`, `skip_while`, `take_while` and `reverse`. A `select` before it
  can map two keys to one value, so in that case the `distinct` stays. `distinct_by` is never
  dropped, because its key is not the table key. The values lane never drops a `distinct`,
  since values can repeat, and the dispatcher skips the call for that lane. A chain whose
  only call is `distinct` keeps it, so the splice still has a call to match.

### Point-lookup folds {#table-point-lookup}

- **A key-equality filter over a table folds to one hash probe that gives the scan's exact
  result.** `try_table_point_lookup` rewrites `where(it.key == X [&& rest]) |> <terminator>`
  over `each_kv(tab)`, and `where(k == X [&& rest])` over `keys(tab)`. `any` and `contains`
  become `key_exists`. `count` becomes `key_exists ? 1 : 0`. `first` and `first_or_default`,
  with or without one `select`, probe `tab?[X]` on the kv lane and `key_exists` on the keys
  lane. A one-call chain whose terminator carries the predicate itself - `any(pred)`,
  `count(pred)`, `contains(x)` on the keys lane - folds the same way, with the predicate as the
  filter. On a miss the fold returns what the terminator returns for an empty sequence: `false`, `0`, the default argument, or the
  `first` panic (`make_probe_miss`). The extra conjuncts (`rest`) run only on the probed
  element, and when they are false the fold takes the same miss path. The `values` lane
  never probes, and neither does a chain whose result is an iterator.
- **The probe matches the scan only for these key shapes.** `match_key_probe_side` accepts
  an `X` that does not read the row binder and has no side effects, because the scan
  evaluates `X` once per element and the probe evaluates it once in total.
  `extract_key_probe` takes the key equality only as the leftmost conjunct. Keys are unique, so a conjunct to its right runs
  at most once in both the scan and the probe and needs no purity check. A conjunct to its
  left would run on every element in the scan but only once in the probe, so that shape is
  declined. `contains(x)` on the keys lane evaluates `x` exactly once on both paths, so it
  needs neither check.
- **A run of `where` calls reaches the probe as one call.** `try_splice_patterns` runs
  `collapse_chained_wheres` before `try_table_point_lookup`. That is why the lookup accepts
  at most three calls: one `where_`, an optional `select`, and the terminator. If the lookup
  ran before that pass, a chain with several `where` calls would not probe.

## linq_fold_sql

### The SQL hand-off {#linq-fold-sql-source}

- **`extract_sql_source` walks the call's own `arguments[0]` spine, not linq_fold's
  flattened `top`** - the SQL DSL ops are not `linqCalls`, so `flatten_linq` stops at the
  first one and never reaches the `select_from` source. `sql_linq_loaded(prog)` then gates
  on `sql_linq`, the module that defines `_sql`, being in THIS program: a consumer can get
  `select_from` from a provider boost without `daslib/sql_linq`, and that chain falls
  through to the in-memory path.
- **The source is recognized by annotation, not by name.** `is_sql_table_select` accepts a
  `select_from` call only when its type is an array of a struct annotated `[sql_table]`,
  `[sql_view]` or `[sql_fts5]`. A `select_from` of any other type ends the walk with null, so
  a function that only shares the name `select_from` folds in memory and never reaches
  `_sql`.
