# daslib linq family Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_LINQ.md`, `ARCHITECTURE_LINQ_FOLD.md`, `ARCHITECTURE_LINQ_FOLD_SOURCES.md`,
`ARCHITECTURE_SQL_LINQ.md`.

**Routed from `REVIEW.md`: a diff touching `linq*.das` or `sql_*.das` applies this list together
with it.**

**A fused emit that binds a terminator's default or compare argument, or a `take(N)` count,
lazily, more than once, or below the top of the generated invoke, every `*_or_default` decs lane
included, is a defect** - bind it once at the top, even on paths that never use it, because the
unfused `linq.das` overload evaluates it that way.

**A fused emit whose empty-source behavior differs from the unfused `linq.das` overload's -
panicking where the unfused overload yields the default, or yielding a default where it
panics - is a defect.** The unfused `first` panics on an empty source and `min` / `max` return a
zeroed default, so a fused `first` lane that computes through `min` / `max` checks its buffer for
empty and panics first.

**Never add `count` or `first` to the 2-arg reducer set (`is_bucket_reducer_call`) - it admits
only a reducer whose second argument is a selector.** `sum` / `min` / `max` / `average` take a
selector there; `count` / `long_count` take a predicate, and a predicate spliced into a
projection slot is silently wrong.

**Never gate the distinct lane's dedup insert on the terminator's `count(pred)` predicate - that
predicate gates the counter only.** `distinct[_by]` keeps the first occurrence per key, so an
insert under that predicate changes which occurrence represents the key.

**A diff that leaves `constant` unset on the table parameter type
`TableAdapter.invoke_param_type` returns, or on the srcB parameter `emit_array_join` passes in
probe mode (the join looks keys up in the srcB table itself instead of hashing it), is a
defect.** The const makes an insert or erase on the probed table a compile error; a rehash would
turn the borrowed `unsafe(tab?[k])` bind into a use-after-free.

**A per-field walk over a row-usage set that is neither sorted first nor ordered by an external
declaration-order list is a defect** - a membership test over the same set needs no order.
`collect_row_usage` / `collect_decs_tup_usage` return names in `table<string>` key order, so an
unsorted walk makes the emitted AST a function of hash order instead of the chain, which
AST-shape tests and AOT semantic hashes both pin.

**Two emit sites reachable at the same `at` that share one `qn` prefix are a defect - give each
site its own prefix.** `qn` keys only on (prefix, at), so a shared prefix shadows a bind instead
of failing to compile.

**A diff that changes one implementation of the composite-key ordering - `key_less` /
`less_masked` (`linq.das`), `try_make_inline_cmp_keys` (`linq_fold_common.das`),
`collect_order_by_keys` (`sql_linq.das`) - changes the others in the same change.** All read bit
`i` of the mask as key `i` descending (LSB = first key); the in-memory two also flip operand
order for descending and break ties in key order. One changed alone sorts a spliced, unfused, or
`_sql` chain differently from the others, with no error.

**A diff that changes the composite-key cap in one of its three homes - `try_make_inline_cmp_keys`'s
inline `_::less` if-chain (`linq_fold_common.das`), `less_masked`'s tuple overloads (`linq.das`),
linq_das's `ORDERBY_MAX_KEYS` - changes all three, in the same change.** A key tuple longer than
the inline cap declines the splice and sorts through `less_masked`, whose overloads stop at the
same cap, so one home raised alone admits a query the others cannot sort.

**Never reserve `n` in a `top_n*` lane over an iterator - let the bounded-heap fill phase grow
geometrically.** Cardinality is unknown there, so an `n` far above the element count allocates
the whole `n` upfront for no win.

**A linq call-macro that synthesizes the `_` block parameter without setting `can_shadow` on it
is a defect.** Chains nest, and the inner `_` has to shadow the outer one.

**Never move the `_sql` hand-off below `normalize_order_reverse` / `collapse_chained_*` or below
`if (empty(calls))`.** `_sql` re-expands the pristine expression on the next infer pass, and a
`[sql_table]` source with no chain ops at all is still SQL.

**A pattern row that emits when its captured `select` (the `srcsel` slot) cannot be wrapped into
a projected adapter is a defect - skip the row instead.** The row's emit assumes the projected
element, so running it on the un-projected adapter orders or dedups raw rows - a wrong result,
not a missed splice.

**A diff that reorders the registration calls in `register_all_linq_fold_rows` - the
`build_*_rows()` calls, the group call that registers the sources - without listing, in the PR
description, the chains whose matching arm changes is a defect.** Rows land in `splice_patterns` in call order, the
walker takes the first match, and that registrar is the only place the order exists.

**A diff that adds a row to a `build_*_rows` places it ahead, in `splice_patterns` order, of every
row whose chain, through its optional slots, matches every chain the new row matches.** `check_pattern_table_reachable`
reports only strict prefixes and runs only on synthetic tables, so a broader row declared first
takes the new row's chains with no error (`ARCHITECTURE_LINQ_FOLD.md#linq-fold-row-registration`).

**A diff that teaches a fused lane a new terminator adds that terminator to the `alias_table`
family the lane's row matches its `term` slot on (`loop_terminator_family`,
`distinct_terminator_family`), in the same change.** A name the family leaves out never matches
the terminator slot, and the chain falls to the tier-2 cascade with no diagnostic
(`ARCHITECTURE_LINQ_FOLD.md#linq-fold-walker`).

**A sql_linq column-ref arm that accepts a `$e(recv).$f(field)` qmatch without first proving
`recv is ExprVar` is a defect.** A nested receiver otherwise matches under a foreign name and
emits a wrong column, with no error.

**A sql_linq computed-expression arm that runs without an `is_sql_renderable_scalar` gate ahead
of it is a defect.** That type gate routes a whole-row carry var to the clean row-object reject
instead of to a macro-time crash in `pred_to_sql`.

**A call to a SQL-returning helper that checks only one of `q.hadError` and `empty(frag)` is a
defect - check both.** Operator arms wrap an empty child into a non-empty fragment and several
helpers return `""` without setting `hadError`, so one check alone emits malformed SQL in place
of a diagnostic.

**A path from `analyze_chain` to `build_sql_string` / `collect_query_binds` that skips
`maybe_finalize_distinct_by_passthrough` or `maybe_wrap_take_before_aggregate` is a defect.**
Skipping the first drops a `_distinct_by` dedup with no error; skipping the second leaves
LIMIT/OFFSET on the aggregate.

**Never install a wrap over an already-populated `q.fromSubquerySql` or `q.fromSubqueryBinds` -
each takes exactly one producer per query.** The wrap drops the earlier subquery and orphans its
binds - a bind/placeholder mismatch, with no error.

**A diff that adds a projection slot pushes one entry into every index-parallel projection
array - `selectCols`, `selectColAliases`, `selectColSqlFragments`, `selectColTypes`,
`projRecordNames` - in the same change.** `push_source_column` and `push_computed_proj_slot`
cover only part of the set and leave the rest to their caller; a partial push desyncs the SELECT
list from the row builder, with no error.

**A diff that adds a post-join `_.<alias>` consumer to sql_linq resolves the alias through
`find_projection_alias` + `render_projection_alias_sql`, and rejects a miss rather than
falling back to resolving the name against the base table.** After a join the alias lives in
the join projection snapshot, so base-table resolution answers with an unqualified
base-table column - a wrong column, with no error.

**Never pick a projection slot's SQL by `q.seenJoin` - pick it by which of that slot's own
entries is non-empty: SQL fragment first, then aliased column, then unqualified column.**

**A diff that adds a clause that can emit `?` also pushes that clause's binds at its SQL parse
position in `collect_query_binds`, in the same change.** `sql_to_frags_ex` re-scans the emitted
SQL text and pairs markers with that list in occurrence order, so a clause added to the emitter
alone binds every later placeholder to the wrong value, with no error.

**A diff that changes a generated SQL statement's column list changes its bind function in the
same change, and the reverse.** Bind indices are placeholder positions over bindable columns,
never struct field positions; drift is silently wrong data, not an error.

**A diff that adds or changes a `[sql_table]` helper pair registers its 2-arg form BEFORE the
1-arg form.** `find_struct_helper_fn` keeps the LAST match and `[sql_index]` rewrites the body of
the helper it returns, so a swapped pair attaches index DDL to the wrong overload.

**Never set `seenJoin` on a join's right-side `SqlQuery` (`subQ` in `process_join_call` /
`process_cross_join_call`) - give it `rootAlias = "t1"` only.** Column refs qualify on
`rootAlias`; `seenJoin` turns off the fallback that resolves an unbound `_where` parameter to
the side's only source, so a right-side filter over a named parameter is sent as a bound `?`
value instead of becoming a column.

**Never narrow or widen `find_kw_depth0`'s clause-keyword test - a clause keyword is a whole word
at bracket depth zero, outside every string literal (its `{...}` interpolation bodies included),
not preceded by `.` and not preceded by a `>` that ends `|>`, `=>` or `->`.** Narrowing lets an
in-body aggregate parse as a `select` clause or a keyword inside an interpolation end a stage;
widening the exclusion to any `>` hides a keyword that follows a generic bracket or a comparison.

**`substitute_idents`, `mentions_ident` and `rewrite_group_var` share one token model - what
counts as code, as verbatim text, and as a reference - and a diff that changes it in one changes
it in the others, in the same change** (the model: `ARCHITECTURE_LINQ.md#linq-das-token-model`). A
change in one scanner desyncs `mentions_ident` from the rewrite it gates, and the emitter then
renames a parameter the spliced projection still references.

**A `parse_one_stage` that reads a `where` or `orderby` position before it resolves the stage
terminal - the earliest `select`/`group` at or after `start` - is a defect.** The terminal bounds
the stage, so scanning clauses first lets a later stage's `where`, past this stage's `into`,
be taken as this stage's terminal.

**Never widen the `group ... into g` rewrite past its two spellings - bare `g` -> `g._1` and
`g.key` -> `g._0` - leave every other `g.<member>` verbatim.** Raw tuple access then keeps working
and a bad member errors as itself; a wider rewrite turns `g._0` into `g._1._0`.

**Never `return call` from a `FromInMacro` reject, the not-yet-inferred-source arm included -
report `macro_error` and `return null` instead.** Returning the call reports ast-changed on every
pass and repeats until the infer-pass limit stops the compile with error 30507.

**Accepting a decs chain whose calls are out of canonical order - head `where_` / `select` calls,
then skip -> skip_while -> take_while -> take, then at most one `where_` - is a defect - decline
the chain.**

**Emitting the decs range guards in any order other than take-cap -> skip counter -> skip_while
flag -> take_while break -> take bump (the take counter's increment) -> `postTakeWhereCond` (the
predicate of a `where_` after `take(N)`) -> per-element work is a defect** - the guards mirror
the array side's `wrap_with_ranges`. The bump follows the while-guards and precedes the
post-take `where_`, so only elements the while-guards pass count against `take(N)`, and that
`where_` filters among those N.

**A decs or `zip_general` row that accepts a `skip_while` / `take_while` with a `select` ahead
of it in the chain is a defect - decline the chain.** The predicate is inlined against the
source element (the decs source tuple, zip's `(a, b)` pair), so a select ahead of it changes the
element the predicate reads.

**Emitting a decs range counter or the take limit inside the archetype walk, rather than in the
prelude above it, is a defect.** That state is global to the query, across every archetype.

**A diff that drops a decs component slot erases it from every parallel `ExprFor` vector -
`sources`, `iterators`, `iteratorsAt`, `iteratorsAka`, `iteratorsTags`, back-to-front - and
clears `iteratorVariables` so the typer rebuilds it, in the same change.** A vector left
un-erased pairs a surviving slot with the wrong component.

**Never route `long_count` through the decs `arch.size` shortcut - walk the entities instead.**
The shortcut accumulates `acc += int(arch.size)`, so it is int-safe only.

**Never take a `select` projection's type from a body `peel_lambda_rename_var` or
`peel_lambda_replace_var` returns - take the projection lambda's return expression
(`peel_lambda_single_return`) in a zip chain or `build_join_pair_core`, the typer-resolved call
type on every other path, and prove it non-null before use.** The peel functions' invoke fallback
returns a body with a null `_type` at macro stage, and a zip or join select's call type can be an
unresolved `typedecl(result_selector(...))` formula.

**`emit_loop_or_count_lane_decs` that lets a `to_table` terminator reach the implicit-to_array
arm is a defect - decline `to_table` ahead of that arm.** decs has no to_table lane, so the
implicit arm emits an array for a table-typed expression.

**Never choose `aggregate`'s copy-or-move return (`aggregate_impl_const` and the public
`aggregate` overloads) by the element type `TT` - choose it by the accumulator type `AGG`.** The
two differ when the accumulator is a workhorse type (one `=` copies it, like `int`) and the
elements are not, such as an `int` sum over an array of structs.

**A diff that removes the `no_order_with_count` predicate from the `group_by` row's `requires`
list, or `join_no_select_with_count` from the `join_general` row's, makes that row's count lane
evaluate the captured trailing order key or select, in the same change.** Each predicate
declines a count whose chain carries that capture; a count lane that skips the capture drops the
lambda and its side effects with no error.

**A diff that adds a terminator to `emit_zip`'s accepted set whose answer depends on element
order also adds it to `zip_reverse_compatible_with_term`'s reject list, in the same change.** The
zip row ignores a trailing `reverse` for every terminator that predicate admits, so a missing
entry answers for the unreversed sequence, with no error.

**A diff that changes the arity of linq's `concat` overloads also changes the `foldedArgIndices`
list of the `concat` and `concat_to_array` entries in `linqCalls` (`linq_fold_common.das`), in
the same change.** The default fold wraps only the listed argument indices in a nested `_fold`,
so an index missing from the list leaves that source unfolded, with no error.

**A fused emit that passes a `*_or_default` user default through the terminal `_select`
projection is a defect - return it as bound.** The default already has the projected type, so
projecting it again applies the select twice.

**A fused emit that runs any later op of the chain - a `select` projection, a predicate, a
key - on an element an earlier `where_` rejected is a defect - nest each later op inside that
`where_`'s guard.** An op run on a rejected element fires its side effects, or faults on the
value the `where_` excluded.

**A hashed-join splice that admits a join key `is_primitive_join_key_type` rejects without
wrapping its insert and probe keys in `_::unique_key()`, as `join_impl` (`linq.das`) does, is a
defect - wrap them, or decline the splice so `fold_linq_default` folds the chain.** An unwrapped
key matches differently from the unfused join, with no error.
