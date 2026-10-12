# daslib architecture notes - the linq_fold splice kernel

Companion to `ARCHITECTURE.md` in this folder. `_fold` fuses a linq chain into one generated
invoke; this document is the kernel - layout, dispatch, the walker, the adapter contract, the
generic lanes. The adapters are `ARCHITECTURE_LINQ_FOLD_SOURCES.md`; the family is `ARCHITECTURE_LINQ.md`.

## Module layout

### File charters {#linq-fold-file-charters}

- **`linq_fold`** - dispatch only: source recognition (`try_splice_patterns`), adapter
  construction, the tier-2 `fold_linq_default` cascade, the `LinqFold` call macro and the single
  registrar `register_all_linq_fold_rows`; no emit lane, no row.
- **`linq_fold_common`** - what no source can influence: the pattern-table types, the walker, the
  predicate library, the chain pre-passes, the generic emit lanes, the `splice_patterns` registry
  and the abstract `SourceAdapter`; no source recognizer.
- **`linq_fold_array`** - the array, zip and array-join adapters and the zip rows.
- **`linq_fold_decs`**, **`linq_fold_json`**, **`linq_fold_table`** - one adapter each, and never
  a sibling source: a source file requires `linq_fold_common` only.
- **`linq_fold_sql`** - the exception: it recognizes a `[sql_table]` source and hands the whole
  chain to the `_sql` macro, so it has no adapter and no emit.
- pugixml's **`linq_fold_xml`** - the xml adapter, outside daslib.
- **A daslib source joins `linq_fold` in two places**: `linq_fold` requires its module `public`,
  and `try_splice_patterns` gets a recognizer arm at the priority the source needs; it adds no
  rows, because rows are per operator family. A source outside daslib (pugixml's `linq_fold_xml`)
  joins the `linq_fold_source` group from its descriptor and its C++ module's constructor, and
  registers a `LinqFoldSourceRecognizer` from its `register_linq_fold_source` entry.

### Dispatch {#linq-fold-dispatch}

- **`try_splice_patterns` tries recognizers in a fixed order** - sql, decs, the registered
  `linq_fold_sources` in registration order, json, table - and the array arm runs last with
  no recognizer of its own: it is what claims a chain nobody else claimed.
- **An arm that builds an adapter returns its result, null included.** Every arm that
  constructs an adapter returns what `run_splice_adapter` gives back. A null goes back to
  `LinqFold`, which runs the tier-2 `fold_linq_default` cascade; the array arm never sees a
  chain another source claimed. An unclaimed `from_decs_template` chain is the case `_fold`'s
  perf warning reports: the bridge fills a temporary array and tier 2 runs over it.
- **The json and table arms recognize their source by name, so `linq_fold` requires them
  unconditionally.** `extract_json_source` matches a `from_json` call over a `JsonValue?` and
  compares the struct and module names as strings, and the json adapter emits its walk by
  name, so `linq_fold_json` requires no json module. `extract_table_source` matches a builtin
  `each_kv` / `keys` / `values` call over a table-typed argument. In a program with no such
  source the recognizer returns null and the chain moves on to the next arm.
- **`_fold` waits for a fully inferred chain type.** `LinqFold.visit` refuses a chain whose
  type is still auto or an alias, as happens with a `select` whose declared `typedecl` return
  is not yet instanced. `macro_verify` reports the refusal as a macro error, and macro errors
  clear on every infer pass, so the refusal lasts only until the type resolves. Firing earlier
  would write the unresolved `typedecl` into the emitted buffer declaration.

### Row registration and the shadow check {#linq-fold-row-registration}

- **Rows are per operator family, not per source.** The seven `build_*_rows` fns
  (order_family, loop_or_count, reverse, distinct, group_by, join, zip) emplace 17 rows into
  the one `splice_patterns` table; only zip's builder lives outside `linq_fold_common`.
  Source gating happens inside a row - through a `RequiresPredicate` that asks the adapter
  (`can_group_by_source`, `can_join_source`, `array_source`, `non_array_source`) or through
  a hook that returns null. Registration order is match priority: the dispatcher walks the
  table top to bottom and the first row whose chain matches and whose emit returns non-null
  wins, so more specific chains are declared first.
- **Rows are built by one `[_macro]` in `linq_fold`, and exist only in its macro context.**
  Each module runs its `[_macro]`s in its own macro context, and those contexts cannot order
  themselves against each other, so `register_all_linq_fold_rows` calls every `build_*_rows`
  and then the `linq_fold_source` group in one place, and that call order is the row order.
  It returns unless linq_fold's own macros are compiling, and every `build_*_rows` is a
  `[macro_function]`: the `@@<EmitFn>` addresses the rows carry name `[macro_function]` emit
  fns whose `quote()` bodies the LLVM JIT cannot lower, so no runtime the JIT compiles holds
  them.
- **Predicates and emit fns are named module-level functions, referenced as
  `@@<RequiresPredicate> name` / `@@<EmitFn> name`.** An anonymous `@@(...)` lambda compiles
  to a `_localfunction_*` symbol the LLVM JIT pass cannot resolve; a named function has a
  stable address.
- **`chain_prefix_of` reports a strict structural prefix only.** It compares matcher (a
  `one_of` set order-insensitively), cardinality and arity, and ignores capture names. Equal-
  length chains are not a shadow: two rows with one chain and different `requires` are both
  reachable, because a failed `requires` falls through to the next row. An earlier row whose
  optional slots let it match every chain a later row matches is not reported, so declaring
  specific rows first is a discipline nothing checks. `check_pattern_table_reachable` answers
  by return value only, and only the walker tests call it, on synthetic tables.

## The walker

### Slot matching {#linq-fold-walker}

- **The walker consumes the whole call list or fails.** `match_pattern` walks the flattened
  calls left to right against the row's slots: `c_one` must match and advances both cursors,
  `c_opt` advances both on a match and skips the slot otherwise, `c_chain` matches greedily
  while the call is in the slot's name set and always succeeds - including with zero matches,
  which still creates the (empty) `captures.many` entry, so an emit fn can rely on the key
  existing. Any call left unconsumed after the last slot is a no-match. The `requires`
  predicates run last, against the populated captures.
- **`Captures.single_name` exists because a pre-pass renames the op without touching the
  AST.** `normalize_order_reverse` folds a trailing `reverse` into the order op by swapping
  the tuple's `LinqCall` record, leaving the `ExprCall` alone; deriving the name back from
  the `ExprCall` (`call_norm_name`) would silently undo the swap. The walker therefore
  records the `LinqCall.name` at match time, and any emit fn that cares about the
  post-normalize name reads `single_name`.
- **`alias_table` names the op-name groups a `m_alias` slot admits, and a missing name fails
  silently.** A terminator the lanes handle but `loop_terminator_family` omits never matches
  the terminator slot, so the chain leaves the splice and lands on the tier-2 cascade with no
  diagnostic - the symptom is a slow chain, not an error.

### Chain pre-passes {#linq-fold-chain-prepasses}

- **The dispatcher rewrites the flattened call list before any row matches** -
  `normalize_order_reverse`, `collapse_chained_selects`, `collapse_chained_wheres`, in that
  order. They edit list entries and lambda arguments, never a surviving call's
  `ExprCall.func`, so after a collapse a call can name an overload whose input no longer
  matches its rewired source. That stays unobservable because emit fns read the op's name and
  its lambda, never the overload the call resolved to.
- **`normalize_order_reverse` needs no AST rewrite**, because each ascending/descending order
  pair takes identical arguments; `order_by_keys` carries its directions in a mask argument
  and is never flipped. Repeated reverses fold pairwise.
- **Adjacent `select`s compose by substitution, adjacent `where_`s by `&&`.** Substituting
  the inner body for each use of the outer's parameter would run an impure inner body zero or
  several times per element, so `compose_select_bodies` declines one with side effects. `&&`
  keeps the chain's order and short-circuit exactly - the inner predicate always runs, the
  outer only when the inner passes - so `compose_where_bodies` needs no purity gate. The
  composed lambda is a clone of the inner one, so its parameter keeps the source element
  type. The where collapse is what lets a row's single `where_` slot cover a run of `where_`
  calls.

## The adapter contract

### Abstract methods, hooks and capabilities {#linq-fold-adapter-contract}

- **The adapter contract is four abstract methods; everything else on the base has a
  default.** `bind_name`, `element_type`, `wrap_source_loop` and `wrap_invoke` are abstract,
  so every source answers them. The per-operation hooks (`emit_loop_or_count`,
  `emit_reverse_skip_into_tail`, `emit_reverse_last_backward`, `emit_distinct_take_loop`,
  `build_group_by_adapter`, `emit_join_hook`) return null on the base, and what a null means
  is per hook (below). The capability methods (`can_group_by`, `can_join`,
  `can_reserve_by_length`, `has_own_loop_or_count_lane`, `supports_direct_return`,
  `effective_dispatch`, `defers_materialization`, `count_shortcut`, `invoke_param_type`,
  `name_prefix`) are the source answering questions about itself. daslang classes have no
  `is`/`as` downcast, so an emit fn holding a `SourceAdapter?` reaches source-specific
  behavior only through these methods - there is no place to write a per-source branch, and
  adding a source needs no edit to a central enum or switch.
- **The capability defaults are fail-closed.** A wrong `true` from `supports_direct_return`
  emits a mid-loop `return` that escapes a nested-callback walk (decs's
  `for_each_archetype`); a wrong `false` only costs the slower state-var + find-stop + tail
  path.
- **`loop_source_expr` / `loop_source_name` is what lights up the array-shaped lanes.** The
  shared lanes spell their loop and their `length(...)` reads with the name, and read the
  expr for compile-time facts - the `type_has_length` reserve gate and the join's srcA
  element-type check. Array and table override both, xml and json override the name only,
  decs overrides neither.

### What a null adapter hook means {#linq-fold-hook-null}

- **A null from a per-operation hook means "take the generic path" for two hooks and
  "decline the row" for four.** `emit_reverse_skip_into_tail` null - `emit_reverse_buffer_inplace`
  buffers through its catch-all; `emit_reverse_last_backward` null - the reverse-first walk
  stays forward. A null from `emit_loop_or_count` on a source with
  `has_own_loop_or_count_lane` (the array lane assumes an array-shaped source), from
  `emit_distinct_take_loop` (no generic distinct-take loop exists), from
  `build_group_by_adapter` (it is the source and upstream-join gate) or from `emit_join_hook`
  (`emit_join` only routes) declines the row: the dispatcher tries the later rows, then the
  tier-2 cascade.

### Direct return from a fused loop {#linq-fold-direct-return}

- **A source answers `supports_direct_return` true only when its `wrap_source_loop` puts a
  real loop directly in the generated invoke, so a `return` from the per-element body leaves
  the invoke.** This holds for `ArrayAdapter`'s `for (it in source)`, `ZipAdapter`'s
  `for (itA, itB in srcA, srcB)`, `ArrayJoinAdapter`'s probe loop over srcA, the table
  adapter's flat `for` over `keys` / `values`, and the two json and two pugixml xml adapters
  (the json array `for` or the xml DOM `while` walk, and for a join the srcB collect `for`
  followed by that probe walk).

### Loop dispatch over nested-callback walks {#linq-fold-loop-dispatch}

- **`LoopDispatch` is the loop-framing knob a lane hands `wrap_source_loop`, and only a
  nested-callback source reads it.** A direct-return source frames a plain loop, where the
  body's own `return` or `break` leaves the invoke. decs walks inside `for_each_archetype`,
  where a `return` leaves only the per-archetype lambda, so it maps the arms onto its walk
  variants: `Each` - plain walk, the tail runs after it; `Find` - stoppable walk whose result
  is discarded, the tail answers from state vars; `FindReturn` - stoppable walk whose stop
  flag is the answer (`any`, `contains`); `FindReturnNegated` - the same, negated, for `all`,
  whose lambda stops on the first counterexample.
- **`effective_dispatch` lets a source raise the lane's arm.** decs turns `Each` into `Find`
  when its in-loop `take` / `take_while` emits an early-exit `return true`.
- **Under a range op the stop signal is shared, so a stop is not an answer.** On decs with
  `take` / `take_while`, the range cap's `return true` and a real match (an `any` hit, an
  `all` counterexample, a `contains` hit) both stop the walk; `emit_early_exit_lane` routes
  the match through a found flag, uses `Find` only to stop, and lets the tail answer.
  `FindReturn` / `FindReturnNegated` are taken only when no range op is present.

### Deferred materialization {#linq-fold-deferred-materialization}

- **A source that builds every row as it walks can hand a buffered reducer an element handle
  instead of the row.** XML and json answer `defers_materialization()` true: `handle_type()`
  is the handle's type, `current_handle_expr()` the handle of the element in scope inside
  `wrap_source_loop`'s walk, and `materialize_handle(h, out)` fills an already-declared
  `var out = default<Row>` from a handle. The reducer keeps handles and builds rows only for
  its survivors. Array, decs and table keep the base default (off): their elements already
  exist, and a reducer copies only what it keeps. `ProjectedSourceAdapter` keeps the default
  too, so a leading `select` turns deferral off for xml and json.
- **The order emits buffer a surrogate `tuple<key; handle>`** (`build_surrogate_type`: key
  at `._0`, handle at `._1`), order it with `build_surrogate_cmp` - a plain `_::less` on
  `._0`, operands flipped for descending - and rebuild the survivors in
  `build_surrogate_materialize_loop`, which also applies a terminal `_select`. `where` and
  `distinct` run during the walk on field reads and never enter the surrogate; a distinct
  gate's dedup key still reads the source element, so its witness type stays the full row
  type.
- **A handle stands in for the element only where nothing else needs the element.** Deferral
  needs a single-key order whose key peels - `build_surrogate_cmp` carries no per-key
  direction mask, so `order_by_keys` never defers. In `emit_fused_prefilter` only the
  distinct arm with no take and no first defers. `reverse |> take` defers with no pre-reverse
  projection and buffers bare handles. `reverse |> distinct_by` defers; plain `distinct` does
  not, because its key is the whole element. A full-walk `last` defers with no projection. A
  group's whole-element `first` defers only while the group's bind is the raw element: an
  upstream `select` rebinds it, and the handle yields the raw row, not the projection.

### Row-usage field pruning {#linq-fold-row-usage}

- **The XML, json and table adapters build only the row fields the chain reads.**
  `collect_row_usage` scans the per-element body for `<bind>.<field>` reads, through the
  typer's `ExprRef2Value` wrap; any other reference to the bind - a `to_array` push of the
  element, an identity `select`, the element passed to a function - sets `usesWholeRow`, and the
  walk builds the whole row.
- **`ProjectedSourceAdapter` absorbs a leading source projection and keeps the pruning.**
  When a row captures the optional `srcsel` slot (`source |> _select(f) |> order/distinct/take`),
  the dispatcher wraps the real adapter in this decorator: it binds `projName = f(rawElem)`
  on top of the per-element body and delegates loop and invoke to the inner adapter, so every
  emit sees the projected element unchanged, and the inner scan sees `f`'s field reads rather
  than a whole-element use. It leaves `loop_source_expr` / `loop_source_name` /
  `can_reserve_by_length` at the base defaults, which disables the source-direct fast paths -
  those would bypass the projection.
- **`hoist_prelude` hand-builds a flat block on purpose.** The materializer prelude - the
  hoisted default-row declaration on the field-prune path - must be a SIBLING of the walk; a
  `$b(prelude)` splice nests it in its own scope, where the per-element reads cannot see it.

## The generic lanes

### Order-family rows {#linq-fold-order-rows}

- **A `distinct` after the sort splices only as plain `distinct`.** `order_then_plain_distinct`
  reuses `emit_fused_prefilter`: dedup during the walk, sort after. Plain distinct compares
  whole elements, so deduping before the sort keeps the same elements that sorting then
  deduping keeps. A `distinct_by` after a sort keeps the first element per key in sorted
  order, which a pre-sort dedup cannot choose, so that chain cascades.
- **`make_inline_less_call` is the direct-call twin of `try_make_inline_cmp`.** The
  per-element admit tests of the bounded heap and the streaming min spell
  `less(KEY[a], KEY[b])` inline rather than invoking the comparator block, a call the hot walk
  would pay on every element.

### The fused to_table sink {#linq-fold-to-table}

- **`to_table` is a materializing terminator on the ARRAY lane.** `emit_loop_or_count_lane`
  rides `emit_fold_array_lane`, with `bufDeclStmt` replacing the array buffer by
  `var acc : table<...>`; map or set follows from the terminator call's resolved type (a void
  value type is a set). The selector form `to_table(key, elementSelector)` keeps the tier-2
  path.
- **Each side evaluates once.** A `(k => v)` tuple projection splits, so the key and the
  value are each evaluated once and no tuple temporary is built; any other projection binds
  to a local once, then indexes it. With no projection the kv access is spelled with the
  element tuple's real field names (`.key` / `.value`): the kv usage-pruner maps named
  fields, and a positional `._0` would not bind - so a bare `each_kv(tab) |> to_table()` is a
  pruned, reserved table copy. Duplicate keys keep the last occurrence (das insert
  semantics), as the `linq.das` `to_table` overloads document.
- **The table is reserved only on an unfiltered walk.** A where-thinned table over-reserves
  hash buckets, which costs more than an array's slack, so this gate is stricter than the
  array arm's.

### Hashed join and the table probe {#linq-fold-hashed-join}

- **A group join is outer, and the lead `where_` wraps outside its empty-group arm.** When
  `get` misses, the result runs with an empty group, as `linq.das`'s `group_join_impl` does.
  The lead `where_` wraps the whole probe, so a left row it rejects yields no row at all, not
  an empty-group row. The lead filter fuses into the probe loop, so no filtered left-side
  array is built; `build_join_probe_pieces` applies the same wrap.
- **The table probe relies on a pure key.** When srcB is `each_kv(tab)` / `keys(tab)` joined
  on its bare key, the probe replaces the build loop and the bucket walk, so keyb is never
  evaluated per right row - which no one can observe only because a bare key read has no
  side effects. Any other keyb, and every group join (its result takes the whole bucket),
  keeps the hashed build. A count with no where, or a key-only use, probes `key_exists`;
  value reads bind by reference off the probed pointer; a whole-pair use binds the kv tuple.

### group_by planner {#linq-fold-group-by}

- **group_by's per-key update is a dummy plus an address compare, not a `key_exists`
  probe.** `entry &= tab?[uk] ?? dummy` costs one hash op per element on hits;
  `addr(entry) == addr(dummy)` identifies the miss, which then fills the dummy, inserts it,
  and re-defaults it - the second hash op is paid on misses only. Average slots hold a
  `(sum, count)` accumulator and divide with no zero guard: sound because a key enters the
  table only on its first element.
- **A single-return group key lambda is peeled onto the element bind; any other key is
  invoked.** A peeled body reads the bind's fields directly: there is no per-element invoke,
  and a pruning source (decs's tuple pruner, the row-usage scanner) drops the fields the key
  does not read, which a whole-element lambda argument would pin.
- **The having rewrite is targeted, and a scan backs it.** `rewrite_having_pred` substitutes
  the reducer shapes it recognizes and leaves the rest. `plan_group_by_core` then scans the
  result with `expr_uses_var`, and any surviving reference to the having bind - from a node
  the rewrite does not recurse into, such as `ExprAt` or an `ExprInvoke` from a non-peelable
  lambda - declines the splice instead of emitting an unbound name.
- **One `at` can reach two materializers.** A `group_by` chain closed by
  `first`/`order`/`distinct`/`take` emits through both the array-walk materializer and the
  handle materializer at the same source location, so `qn` - which keys on (prefix, at) -
  keeps the two sites' binds apart only while each site carries its own prefix.
