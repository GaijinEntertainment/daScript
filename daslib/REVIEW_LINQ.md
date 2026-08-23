# daslib linq family Code Review Checklist

**A diff touching the linq family - `linq*.das`, `sql_*.das` (the SQL lowering and its
bind-side helpers, `sql_boost.das` and `sql_provider.das` included) - applies this checklist
together with `REVIEW.md`.** `REVIEW_COMMON.md` (repo root) binds this file too.
Architecture doc: `ARCHITECTURE.md`.

**A fused emit binds a terminator's default / compare argument ONCE, at the top of the
generated invoke, eagerly** - even on paths that never use it, because the tier-2 `linq.das`
overload evaluates it that way. This includes every `*_or_default` decs lane: an empty-tail
fast path that evaluates the default lazily diverges from the walk lane and the iterator
fallback.

**A fused emit reproduces the tier-2 `linq.das` overload's empty-source behavior: it panics
or yields the default exactly where tier-2 does.** A fused `first` over a prefilter buffer
panics BEFORE reaching `min`/`max` - those return an uninitialized reference on an empty
array, so the guard is load-bearing, not defensive.

**`count` / `long_count`'s second argument is a PREDICATE; `sum` / `min` / `max` /
`average`'s is a SELECTOR.** The 2-arg reducer set (`is_bucket_reducer_call`) admits only
the latter - adding `count` or `first` to it splices a predicate into a projection slot.

**In the distinct lane the dedup insert is not gated by the terminator's `count(pred)`
predicate - only the counter is.** A `where_` filter gating the whole per-element body is
expected; `distinct[_by]` keeps the FIRST occurrence per key, so moving the insert under the
terminator predicate changes which occurrence represents the key.

**A fold-emitted table probe binds by reference off `unsafe(tab?[k])` - a borrowed pointer
into table storage - so the generated invoke never inserts into or erases from the probed
table.** A rehash dangles the bind into a use-after-free. Both the join probe
(`build_join_probe_pieces`) and the point-lookup probe (`try_table_point_lookup`) rest on
this.

**`TableAdapter.invoke_param_type` returns the table parameter type with `constant` set.**
That const is what proves the point-lookup probe's borrowed bind survives the invoke; a
param that loses it turns the bind into a use-after-free.

**A per-field WALK over a row-usage set is sorted first (or ordered by an external
declaration-order list); a membership test needs no order.** `collect_row_usage` /
`collect_decs_tup_usage` return names in `table<string>` key order, so an unsorted walk
makes the emitted AST a function of hash order instead of the chain - AST-shape tests and
AOT semantic hashes both key on that shape.

**Two emit sites reachable at the same `at` use different `qn` prefixes.** `qn` keys only on
(prefix, at), so a shared prefix shadows a bind instead of colliding loudly - a `group_by` +
`first`/`order`/`distinct`/`take` chain reaches both the array-walk materializer and the
handle materializer at one `at`.

**Every implementation of the composite-key ordering changes with the others.** `key_less`
/ `less_masked` (`linq.das`) sort eagerly; `try_make_inline_cmp_keys` (`linq_fold_common.das`)
emits an inline `_::less` if-chain for the same chain. Both spell bit `i` of `mask` as "key
`i` descending" (LSB = first key), flip operand order for descending, and break ties in key
order - a change on one side makes a spliced chain and its tier-2 fallback sort differently,
silently.

**The inline `_::less` if-chain's key cap and `less_masked`'s tuple overloads change
together.** A key tuple longer than the cap declines the splice and sorts eagerly; raising
the cap without adding the matching `less_masked` overload leaves a spliced chain with no
tier-2 twin to agree with.

**`top_n*` over an ITERATOR never reserves `n`.** Cardinality is unknown, so a caller
passing `n` far above the element count would allocate the whole `n` upfront for no win; the
bounded-heap fill phase relies on geometric growth.

**Every linq call-macro that synthesizes the `_` block parameter sets `can_shadow` on it.**
Chains nest (`_where(_.x._in(arr |> _select(_.y)))`) and the inner `_` has to shadow the
outer one.

**The `_sql` hand-off precedes chain normalization AND the empty-chain bail.** `_sql`
re-expands the pristine expression on the next infer pass, and a `[sql_table]` source with
no chain ops at all is still SQL - moving the check below `normalize_order_reverse` /
`collapse_chained_*`, or below `if (empty(calls))`, routes those chains to the in-memory
tier instead.

**A pattern row whose captured `select` (the `srcsel` slot) cannot be wrapped into a
projected adapter skips the row - it never emits.** The row's emit assumes the projected
element; running it on the un-projected adapter orders or dedups raw rows - a wrong result
rather than a missed splice.

**The call order in `register_all_linq_fold_rows` IS pattern priority.** Rows land in
`splice_patterns` in call order and the walker takes the first match, so reordering the
`build_*_rows()` calls changes which arm claims a chain. Separate modules run their
`[_macro]`s in separate macro contexts that cannot coordinate - that single registrar is the
only place the order exists.

**A sql_linq column-ref arm accepts a `$e(recv).$f(field)` qmatch only when
`recv is ExprVar`.** A nested receiver (`l.opt.X`, `outer.Brand`) otherwise matches with a
foreign name and emits a silently wrong column.

**A sql_linq computed-expression arm runs only behind `is_sql_renderable_scalar`.** That
type gate routes a whole-row carry var to the clean row-object reject instead of a
macro-time crash in `pred_to_sql`.

**A SQL-returning helper is checked on BOTH channels - `q.hadError || empty(frag)`.**
Neither half suffices: operator arms wrap empty children into non-empty junk
(`lhs = rhs` -> `" = ?"`), and several helpers return `""` without setting `hadError`.
Dropping either half emits malformed SQL instead of a diagnostic.

**Both `maybe_finalize_distinct_by_passthrough` and `maybe_wrap_take_before_aggregate` run
between `analyze_chain` and the `build_sql_string` / `collect_query_binds` that turn its
result into SQL.** Skipping the first silently drops a `_distinct_by` dedup; skipping the
second leaves LIMIT/OFFSET on the aggregate.

**`q.innerSql` and `q.innerBindExprs` have exactly one producer per query.** Installing a
wrap over an already-populated `innerSql` drops the earlier subquery and orphans its binds - 
a bind/placeholder mismatch, not an error.

**Every projection slot pushes one entry into each of the five index-parallel projection
arrays - `selectCols`, `selectColAliases`, `selectColSqlFragments`, `selectColTypes`,
`projRecordNames`.** A helper covering only part of the set (`push_source_column`,
`push_computed_proj_slot`) leaves the rest to its caller; a partial push desyncs the SELECT
list from the row builder silently.

**The sql_linq emitter picks each projection slot's SQL by which of that slot's own entries
is non-empty - SQL fragment first, then aliased column, then unqualified column - never by
`q.seenJoin`.**

**A clause that can emit `?` pushes its binds at its SQL parse position in
`collect_query_binds`.** `sql_to_frags_ex` re-scans the emitted SQL text and pairs markers
with that list in occurrence order, so push order must equal textual order - a bind-carrying
clause added to the emitter alone binds every later placeholder to the wrong value, with no
error.

**A linq_das clause keyword is a depth-0 whole word not preceded by `.` and not preceded by
a `>` that tails `|>`, `=>` or `->`.** Narrowing that exclusion lets an in-body aggregate
(`g |> select(...) |> sum`) parse as a `select` clause; widening it to any `>` stops a clause
keyword that legitimately follows a generic bracket or a comparison from being found at all.

**Every substituting linq_das scanner shares one token model and changes with the
others** - 
`substitute_idents`, `mentions_ident`, `rewrite_group_var`: plain `"..."` content is verbatim,
a `{...}` interpolation body is CODE (scanned and substituted), one level of nested string
literal inside an interpolation is verbatim again, and identifier POSITION classifies the
same way everywhere (after-`.` field access and before-single-`=` labels are not
references - `is_label_position` is the shared judge). A model change in one scanner
desyncs `mentions_ident` from the rewrite it gates, and the emitter then renames a
parameter the spliced projection still references.

**`find_kw_depth0` skips a whole string literal, interpolation bodies included.** That
asymmetry against the substituting scanners is load-bearing: a clause keyword inside `"{...}"`
must never claim a stage terminal, so giving this scanner the interp-as-code model is a
defect, not a unification.

**`parse_one_stage` resolves the stage terminal - the earliest `select`/`group` at or after
`start` - before any `where`/`orderby` position is considered.** The terminal bounds the
stage; scanning clauses first lets a later stage's `where` (past this stage's `into`) hijack
this stage's terminal lookup.

**The `group ... into g` rewrite touches exactly two spellings: bare `g` -> `g._1`, `g.key` ->
`g._0`.** Every other `g.<member>` is left verbatim, so raw tuple access keeps working and a
bad member errors as itself. Widening the rewrite turns `g._0` into `g._1._0`.

**Every `FromInMacro` reject - including the not-yet-inferred-source arm - is
`macro_error` + `return null`, never `return call`.** Returning the call reports
ast-changed on every pass and churns to the infer-pass cap (30507); null lets infer
stabilize, and because errors clear per pass, the not-yet-inferred arm's error survives
only when the source never infers, which is what makes that arm a deferral.

**decs range slots are accepted in canonical chain order: skip -> skip_while -> take_while ->
take, all after any `where_`.**

**decs range guards emit take-cap -> skip counter -> skip_while flag -> take_while break ->
take bump, mirroring the array side's `wrap_with_ranges`.** The bump is LAST so an element
the while-guards rejected does not eat the `take(N)` budget.

**A predicate-driven decs range requires a select-free prefix** - its predicate peels
against the SOURCE tuple, so a select ahead of it changes the element the predicate reads.

**decs range counters and the take limit are hoisted into the prelude above the archetype
walk.** The state is global to the query, and the user's `take(N)` expression must evaluate
exactly once.

**Dropping a decs component slot erases from all parallel `ExprFor` vectors and clears
`iteratorVariables`** - `sources`, `iterators`, `iteratorsAt`, `iteratorsAka`,
`iteratorsTags`, erased back-to-front, with `iteratorVariables` cleared so the typer
rebuilds it (the `soa.das` pattern). A vector left un-erased pairs a surviving slot with the
wrong component.

**`long_count` never rides the decs `arch.size` shortcut.** The shortcut accumulates
`acc += int(arch.size)`, so it is int-safe only; an int64-safe total has to walk the
entities.

**`emit_decs_reverse_skip_into_tail` takes its projection type from the typer-resolved call
type, never from the peeled lambda body.** `peel_lambda_rename_var`'s invoke fallback
carries a null `_type` at macro stage, so deriving the type from the peel mis-classifies
those chains; the caller proves the type non-null before using it.

**`emit_loop_or_count_lane_decs` declines `to_table` before the implicit-to_array arm.**
decs has no to_table lane, and reaching the implicit arm emits an array for a table-typed
expression.
