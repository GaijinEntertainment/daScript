# daslib Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`daslib/ARCHITECTURE.md`. Planned work: `daslib/followup_comment_sweep.md`.

**A lint report path (`perf_warning` / `style_warning` / `lint_error`) checks suppression BEFORE
its dedup insert.** A nolint'd finding that takes the (rule, location) slot silences a
co-located sibling rule.

**The rule code stays part of the dedup key.** A key without the code collides across rules.

**PERF026-028 honor a `// nolint` at ANY frame of the reported chain.** The sink often
bottoms out in daslib while the actionable line is an intermediate call site — narrowing the
check to the sink line is a defect.

**Moving an `in_closure` / `in_deferred` guard inside a lint visitor method is a semantic
change, never cleanup.** The guard's position encodes which rules are meaningful inside a
lambda; the per-rule policy is in `ARCHITECTURE.md` § perf_lint.

**A lint warning anchors at the code its fix rewrites.** A remedy that deletes a statement
reports the statement; a remedy that edits one variable's initializer reports the variable —
an anchor chosen for implementation convenience instead is a defect.

**Visitor state scoped to a construct is reset or restored on every exit path of that
construct.**

**Per-loop visitor state is a stack, never a bare scalar.** A scalar survives into the
sibling loop's exit path and unbalances its counter.

**Per-function visitor state resets in `preVisitFunction`, ahead of any early return.**
A latch that skips the template early-return poisons the next function.

**A daslib predicate that replicates a C++ compiler decision changes in lockstep with its
C++ side.** The pairs: `lint022_optimized` / `lint022_calls_may_be_inlined` ↔
`Program::getOptimize` / `Program::patchInline`; `stale_scan_line` ↔
`rtti_is_nolint_suppressed`; `is_inline_temp_name` ↔ `INLINE_TEMP_PREFIX`; the
STYLE024/025 unsafe map ↔ infer's `unsafe_*` rules; `style036_inert_contract` ↔ infer's
contract clearing. Nothing fails when one side moves alone.

**Weakening the nolint-window tests is a defect** — `tests/lint/test_nolint_suppression.das`
pins that a string literal, a URL, and a mid-comment `nolint:` do not suppress while a
first-token directive after `//` or `//!` does, and `tests/lint/test_stale_nolint.das` pins
that a `lint-skip-file` past the header window is prose.

**`build_lint_macro_disabled` layers four sources in this order: defaults, repo `off`, repo
`on`, environment.** Env last lets a one-run `DAS_LINT_DISABLE` beat a `CODE = true`.

**`options _enable_default_off_rules` skips BOTH the default seeding and the repo `off`
directives.** Repo policy must not silence the rule a fixture exists to exercise.

**Weakening `tests/lint/test_lint_config.das` is a defect** — it pins that each `[format]`
key resolves nearest-wins independently up to the `.git` root, so a nearer `.lint_config`
declaring one key does not reset the other.

**Weakening the kept-comment cases in `utils/mcp/test_tools.das` is a defect** — they pin
the formatter's kept set.

**A daslib module that emits a lint rule id joins `RULE_MODULES` in `utils/lint/REVIEW.das`
in the same change.** An unlisted module's ids are never scanned, so its fixture-and-rst
check never runs.

**LINT010 records a store in the POST-visit of `ExprCopy`/`ExprClone`/`ExprMove`, never in
`preVisitExprVar`.** The LHS's variable fires before the RHS is walked; an early record
lets the RHS's own read clear the store it just recorded.

**A rule identifying a callee by name matches the root generic, not the instance** —
instantiation mangles names and strips witness arguments.

**A rule never fires inside the module whose idiom it advertises** — a rule advertising
module M's idiom firing on M's own dispatch is a defect.

**A collapse suggestion compiles and preserves semantics for the exact shape reported** —
gate on receiver type, arity, cloneability, and duplicate constant keys.

**Exactly one sibling rule fires on a shape several collapse rules can match.** Overlaps
stay partitioned: STYLE021 owns the const-key run over STYLE031, STYLE032 owns fresh-empty
declarations over STYLE033, and STYLE033's chain-receiver check leaves a plain-variable
receiver to STYLE033's variable-form check, so the two never double-fire.

**The lint message text is the rule-code transport: the code is everything before the
FIRST colon.** Suppression, disable lists, and dedup all parse it from the message —
rewording a message so the code is not the leading colon-delimited token makes the rule
unsuppressable and self-colliding.

**A `canVisit*` override in style_lint ships with the unsafe-frame push/pop taught about
the subtree it skips.** The walk pushes one frame per expression and pops it, so a skipped
subtree unbalances the count; the balance panic in each entry point is the tripwire.

**The arity caps mirror overload sets that live elsewhere** — `MAX_CONCAT_ARITY` ↔ linq's
`concat` variadics, `MAX_VARIADIC_PUSH_ARITY` ↔ builtin `push_from`/`push_clone_from`;
growing either overload set without the cap is a silently missed finding, the reverse a
suggestion that does not compile.

**AOT emit is fail-closed: every entry point tests `macroException`/`failToCompile` BEFORE
materializing output.** A codegen exception mid-visit leaves partial C++.

**An unreachable emit state writes `#error` into the output, never `panic`** —
`runMacroFunction` swallows a panic, so the emitter never reports through it.

**`aotStructName` and the `VarInfo` emitter's inline `aotSuffixNameEx(info.name, "_S", ...)`
build the same C++ identifier, and `buildStructEnumCollisions` is seeded before either
emits.** An emit path that skips the seeding, or a suffix rule changed on one side, produces
`offsetof`s naming a struct declared under a different identifier.

**`match_error` stores a BORROWED `LineInfo` pointer — pass a pattern node's location,
never a synthesized access expression's.** Access nodes are cloned per field inside a bare
scope and die with it.

**A macro that lowers lambdas reports through `macro_sticky_error`, never `macro_error`.**
A later pass lowers the lambda to a plain function and clears non-sticky errors.

**UTF-8 byte-class tables are indexed through `uint(uint8(ch))`** — `for (ch in string)`
yields a SIGNED byte under JIT; a raw index reads out of bounds for every byte >= 0x80.

**A parser result that borrows a view into a tree the parser allocated names the owning
field and ships the scope-ender that frees it.** Nothing else can free it — the tree must
outlive the caller's read, and daslang finalizes neither a raw pointer field nor a local
container at scope exit — so a borrowed view with no named owner leaks the whole document
per call, invisibly until the process is long-lived.

**A conversion that throws on out-of-range input is a panic on untrusted bytes.** `int64` /
`uint64` / `double` on a string throw; a lexer or decoder reachable from a file, a socket,
or a model reaches for the non-throwing `to_*` twin and reports through its own error
channel.

**A parser never re-derives control flow from the text of its own diagnostics.** A message
carries user data, so `starts_with` on an error string is an input-controlled branch; the
token that caused the failure is what the decision reads.

**Every flatten_opt rewrite arm ships a read-only residual predicate** — the oracle the
residual visitors call to prove the pass complete.

**A residual oracle mirrors its arm's gate exactly and never calls the transform.** A
narrower oracle is a false pass, a wider one a false miss; calling the transform from an
oracle aliases the live tree. A gate the transform takes as a parameter is threaded into the
oracle too — re-spelling its default as a constant makes every non-default run a false pass,
and the tests that call the oracle pass the same value.

**Every new fold/fuse arm declares its float class.** Inf/NaN/rounding/association changes
are fast-math-only; bit-exact per-lane rewrites are never gated. An arm added without that
decision silently changes output under `_flatten_no_fast_math`.

**A fuse arm emits a call only after proving the target module can resolve it**, and the
miss path falls back to the unfused shape — the pass must never turn a shader that compiled
into an unresolvable call on a narrower backend.

**A new store spelling joins `MutCollect` in the same change.** CSE reads a name outside the
mutable set as constant for the whole block, so an uncollected store is a silently shared
subexpression.

**A statement the lowering cannot predicate is refused, never dropped.** `lower_stmt`'s
fall-through drop is licensed only because every call that survives lowering is pure.

**`flatten_function` runs its passes in this order — dse → copy-prop → mask-const-prop →
dse → ssa-rename — and a diff that reorders them is a defect.** Each pass produces the next
one's input; a reorder leaves scaffolding in the generated `<name>_flat` twin, which must
come out branchless and call-free.

**`lift_expr` and the lowering it drives use plain recursion, never `make_visitor`** —
inlining recurses back through the pipeline, and a nested visitor traversal corrupts the
visitor machinery.

**A macro that splices the same subexpression more than once pre-binds it to a local ref
first** — re-splicing re-evaluates a call once per splice; a lock/unlock pair then releases
a different temporary than it took.

**Every `[sql_table]` helper pair registers its 2-arg form BEFORE the 1-arg form** —
`find_struct_helper_fn` keeps the LAST match, and the finish pass rewrites the helper it
returns; swapped, index DDL attaches to the wrong overload.

**A generated SQL statement's column list and its bind function change in one edit.** Bind
indices are placeholder positions over bindable columns, never struct field positions;
drift is silently wrong data, not an error.

**A new daslib spelling that reads the environment joins the env_registry marker lists in
the same change.** The scanners match daslang source text; a missing spelling makes every
enforcement test pass vacuously.

**An `ast_verify` check names, in `ARCHITECTURE.md` § ast_verify, the C++ `::visit` site
that dereferences the slot unguarded.** A check with no such site invents an invariant the
compiler does not hold, and the two passes disagree — a slot infer fills in is checkable
only post-infer.

**An `ast_verify` check repairs the slot it reports, not only reports it.** A broken slot
left in place trips every later pass that walks the same node.

**64-bit range sums prove both operands non-negative BEFORE summing as `uint64`**
(`erase(at, count)`): a signed sum wraps, the bounds test passes, and the walk leaves the
array silently.

**Macro-built AST carries no branch a macro-time value can decide** — nothing folds at
macro-application time, so a generated `if ($v(flag))` keeps its dead arm and type-checks
it. Branch in daslang and emit only the taken arm.

**Every buffer-I/O overload returns before taking `addr(buf[0])` on an empty buffer** —
the address is out of bounds and the call sits inside `unsafe`; a new overload without the
guard passes every non-empty test.

**`apply`'s inline path decides lvalue-ness by node KIND, never by a ref flag** — an rvalue
stays materialized, because a reference to a temporary dangles.

**A `let` generated by `apply`'s inline path carries `alwaysSafe`** — that flag, not an
`unsafe` block, licenses the variant access.

**A swizzle rewrite reuses each source node once and clones every repeat** — the first
appearance moves, a second output lane MUST clone, or one node gets two parents.

**The RST label and topic key are computed twice — pre-infer and post-infer — and must
agree byte-for-byte**, or the page prints a bare signature and the symbol re-stubs.

**A fused emit binds a terminator's default / compare argument ONCE, at the top of the
generated invoke, eagerly** — even on paths that never use it, because the tier-2 `linq.das`
overload evaluates it that way. This includes every `*_or_default` decs lane: an empty-tail
fast path that evaluates the default lazily diverges from the walk lane and the iterator
fallback.

**A fused emit reproduces the tier-2 `linq.das` overload's empty-source behavior: it panics
or yields the default exactly where tier-2 does.** A fused `first` over a prefilter buffer
panics BEFORE reaching `min`/`max` — those return an uninitialized reference on an empty
array, so the guard is load-bearing, not defensive.

**`count` / `long_count`'s second argument is a PREDICATE; `sum` / `min` / `max` /
`average`'s is a SELECTOR.** The 2-arg reducer set (`is_bucket_reducer_call`) admits only
the latter — adding `count` or `first` to it splices a predicate into a projection slot.

**In the distinct lane the dedup insert is not gated by the terminator's `count(pred)`
predicate — only the counter is.** A `where_` filter gating the whole per-element body is
expected; `distinct[_by]` keeps the FIRST occurrence per key, so moving the insert under the
terminator predicate changes which occurrence represents the key.

**A fold-emitted table probe binds by reference off `unsafe(tab?[k])` — a borrowed pointer
into table storage, sound only while the generated invoke never inserts into or erases from
that table** (a rehash dangles it). The join probe (`build_join_probe_pieces`) and the
point-lookup probe (`try_table_point_lookup`) both rest on this; what licenses the latter is
the const-forced table param (`TableAdapter.invoke_param_type`) — an emit arm that mutates
the source, or a param that loses the const, turns the bind into a use-after-free.

**A per-field WALK over a row-usage set is sorted first (or ordered by an external
declaration-order list); a membership test needs no order.** `collect_row_usage` /
`collect_decs_tup_usage` return names in `table<string>` key order, so an unsorted walk
makes the emitted AST a function of hash order instead of the chain — AST-shape tests and
AOT semantic hashes both key on that shape.

**Two emit sites reachable at the same `at` use different `qn` prefixes.** `qn` keys only on
(prefix, at), so a shared prefix shadows a bind instead of colliding loudly — a `group_by` +
`first`/`order`/`distinct`/`take` chain reaches both the array-walk materializer and the
handle materializer at one `at`.

**The composite-key ordering has two implementations and they change together.** `key_less`
/ `less_masked` (`linq.das`) sort eagerly; `try_make_inline_cmp_keys` (`linq_fold_common.das`)
emits an inline `_::less` if-chain for the same chain. Both spell bit `i` of `mask` as "key
`i` descending" (LSB = first key), flip operand order for descending, and break ties in key
order — a change on one side makes a spliced chain and its tier-2 fallback sort differently,
silently.

**The inline `_::less` if-chain is capped at 4 keys, matching `less_masked`'s 1..4-tuple
overloads.** A longer key tuple declines the splice and sorts eagerly; raising the cap
without adding the matching `less_masked` overload leaves a spliced chain with no tier-2
twin to agree with.

**`top_n*` over an ITERATOR never reserves `n`.** Cardinality is unknown, so a caller
passing `n` far above the element count would allocate the whole `n` upfront for no win; the
bounded-heap fill phase relies on geometric growth.

**Every linq call-macro that synthesizes the `_` block parameter sets `can_shadow` on it.**
Chains nest (`_where(_.x._in(arr |> _select(_.y)))`) and the inner `_` has to shadow the
outer one.

**The `_sql` hand-off precedes chain normalization AND the empty-chain bail.** `_sql`
re-expands the pristine expression on the next infer pass, and a `[sql_table]` source with
no chain ops at all is still SQL — moving the check below `normalize_order_reverse` /
`collapse_chained_*`, or below `if (empty(calls))`, routes those chains to the in-memory
tier instead.

**A pattern row whose captured `select` (the `srcsel` slot) cannot be wrapped into a
projected adapter skips the row — it never emits.** The row's emit assumes the projected
element; running it on the un-projected adapter orders or dedups raw rows — a wrong result
rather than a missed splice.

**The call order in `register_all_linq_fold_rows` IS pattern priority.** Rows land in
`splice_patterns` in call order and the walker takes the first match, so reordering the
`build_*_rows()` calls changes which arm claims a chain. Separate modules run their
`[_macro]`s in separate macro contexts that cannot coordinate — that single registrar is the
only place the order exists.

**A sql_linq column-ref arm accepts a `$e(recv).$f(field)` qmatch only when
`recv is ExprVar`.** A nested receiver (`l.opt.X`, `outer.Brand`) otherwise matches with a
foreign name and emits a silently wrong column.

**A sql_linq computed-expression arm runs only behind `is_sql_renderable_scalar`.** That
type gate routes a whole-row carry var to the clean row-object reject instead of a
macro-time crash in `pred_to_sql`.

**A SQL-returning helper is checked on BOTH channels — `q.hadError || empty(frag)`.**
Neither half suffices: operator arms wrap empty children into non-empty junk
(`lhs = rhs` → `" = ?"`), and several helpers return `""` without setting `hadError`.
Dropping either half emits malformed SQL instead of a diagnostic.

**Every `analyze_chain` consumer runs `maybe_finalize_distinct_by_passthrough` and
`maybe_wrap_take_before_aggregate` before `build_sql_string` / `collect_query_binds`.**
Skipping the first pass silently drops a `_distinct_by` dedup; skipping the second leaves
LIMIT/OFFSET on the aggregate.

**`q.innerSql` and `q.innerBindExprs` have exactly one producer per query.** Installing a
wrap over an already-populated `innerSql` drops the earlier subquery and orphans its binds —
a bind/placeholder mismatch, not an error.

**The sql_linq projection arrays are index-parallel and always pushed together** —
`selectCols`, `selectColAliases`, `selectColSqlFragments`, `selectColTypes`,
`projRecordNames`, through `push_source_column` / `push_computed_proj_slot` /
`reserve_projection`. A partial push desyncs the SELECT list from the row builder silently.

**The sql_linq emitter picks each projection slot's SQL by which of that slot's own entries
is non-empty — SQL fragment first, then aliased column, then unqualified column — never by
`q.seenJoin`.**

**A clause that can emit `?` pushes its binds at its SQL parse position in
`collect_query_binds`.** `sql_to_frags_ex` re-scans the emitted SQL text and pairs markers
with that list in occurrence order, so push order must equal textual order — a bind-carrying
clause added to the emitter alone binds every later placeholder to the wrong value, with no
error.

**A linq_das clause keyword is a depth-0 whole word not preceded by `.` and not preceded by
a `>` that tails `|>`, `=>` or `->`.** Narrowing that exclusion lets an in-body aggregate
(`g |> select(…) |> sum`) parse as a `select` clause; widening it to any `>` stops a clause
keyword that legitimately follows a generic bracket or a comparison from being found at all.

**The three substituting linq_das scanners share one string model and change together** —
`substitute_idents`, `mentions_ident`, `rewrite_group_var`: plain `"…"` content is verbatim,
a `{…}` interpolation body is CODE (scanned and substituted), and one level of nested string
literal inside an interpolation is verbatim again. A model change in one scanner desyncs
`mentions_ident` from the rewrite it gates, and the emitter then renames a parameter the
spliced projection still references.

**`find_kw_depth0` skips a whole string literal, interpolation bodies included.** That
asymmetry against the substituting scanners is load-bearing: a clause keyword inside `"{…}"`
must never claim a stage terminal, so giving this scanner the interp-as-code model is a
defect, not a unification.

**`parse_one_stage` resolves the stage terminal — the earliest `select`/`group` at or after
`start` — before any `where`/`orderby` position is considered.** The terminal bounds the
stage; scanning clauses first lets a later stage's `where` (past this stage's `into`) hijack
this stage's terminal lookup.

**The `group … into g` rewrite touches exactly two spellings: bare `g` → `g._1`, `g.key` →
`g._0`.** Every other `g.<member>` is left verbatim, so raw tuple access keeps working and a
bad member errors as itself. Widening the rewrite turns `g._0` into `g._1._0`.

**Every `FromInMacro` reject — including the not-yet-inferred-source arm — is
`macro_error` + `return null`, never `return call`.** Returning the call reports
ast-changed on every pass and churns to the infer-pass cap (30507); null lets infer
stabilize, and because errors clear per pass, the not-yet-inferred arm's error survives
only when the source never infers, which is what makes that arm a deferral.

**decs range slots are accepted in canonical chain order: skip → skip_while → take_while →
take, all after any `where_`.**

**decs range guards emit take-cap → skip counter → skip_while flag → take_while break →
take bump, mirroring the array side's `wrap_with_ranges`.** The bump is LAST so an element
the while-guards rejected does not eat the `take(N)` budget.

**A predicate-driven decs range requires a select-free prefix** — its predicate peels
against the SOURCE tuple, so a select ahead of it changes the element the predicate reads.

**decs range counters and the take limit are hoisted into the prelude above the archetype
walk.** The state is global to the query, and the user's `take(N)` expression must evaluate
exactly once.

**Dropping a decs component slot erases from all parallel `ExprFor` vectors and clears
`iteratorVariables`** — `sources`, `iterators`, `iteratorsAt`, `iteratorsAka`,
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
