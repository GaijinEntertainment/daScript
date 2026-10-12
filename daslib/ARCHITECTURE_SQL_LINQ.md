# daslib architecture notes - sql_linq and sql_migrate

Companion to `ARCHITECTURE.md` in this folder. `sql_linq` lowers a linq chain over a
`[sql_table]` source to one SQL statement; `sql_migrate` is the provider-neutral migration
rail. The in-memory linq family and the `_fold` splice are `ARCHITECTURE_LINQ.md` and
`ARCHITECTURE_LINQ_FOLD.md`.

## sql_linq

### Eval phases {#sql-eval-phases}

- **Equal phase numbers are deliberate.** `PHASE_ORDER_BY == PHASE_SELECT` - ORDER BY
  commutes with the projection in SQL (it may reference projected aliases or source
  columns) - and `PHASE_GROUP_BY == PHASE_DISTINCT == PHASE_SET_OP`. Giving either member
  its own number spuriously diverts the canonical `_group_by |> _order_by |> _select` into
  a nested SELECT.

### The analyze_chain peel order {#sql-chain-peel-order}

- **`analyze_chain` peels a chain from its outermost call inward and recurses on
  `arguments[0]`, so state flows both ways and each arm's position relative to its recursion
  is part of its contract.** An arm whose state the inner chain must see sets it before
  recursing: a terminal sets `matr` / `seenTerminal` first, which lets `process_join_call`
  and `process_cross_join_call` spot a terminal inside their own left side by the flag
  changing across the recursion; `count` / `long_count` set `proj = Aggregate` first, an
  `_select` peeled under them only marks `seenSelect`, and the `select_from` arm then skips
  its default full-row columns. An arm that needs state the inner chain sets recurses first
  and reads its lambda after: `_where`, `_group_by` and the order-by keys need `q.rootType`
  and its field annotations (`@sql_json`, `@sql_blob`); `_select` and the order-by keys need
  `seenGroupBy` and the group keys to route `_._0` / `_._1`; `_having` compares
  `seenGroupBy` before and after its recursion to prove `_group_by` sits inside it.
- **`peel_to_array_terminal` is its own function, not an inline arm, so its match locals
  stay out of `analyze_chain`'s recursive frame.** `to_array` / `_to_array` is a no-op
  terminal on a SQL chain - the result is already an array - which is why linq_das appends
  `|> to_array()` to every chain whatever its source.

### Multi-Q lowering and the snapshot wrap {#sql-multi-q-lowering}

- **A chain op whose eval phase is above the lowest phase already in the query
  (`q.minPhaseSeen`) cannot share one SELECT with it.** Peeling runs outermost-first, so
  "already in" means later in the chain. `maybe_divert` hands the op to `divert_to_inner`,
  which re-analyzes the same node in a fresh `SqlQuery`. The fresh query has no phase
  recorded, so the op does not divert again. Its SQL and binds become `q.fromSubquerySql` /
  `q.fromSubqueryBinds`, the outer FROM renders `(<sql>) AS t0`, and the outer `_` resolves
  through `q.fromRowType`, the inner's projected row type. A named-tuple inner passes its
  aliases to the outer as plain columns (`apply_passthrough_projection`); a `SingleColumn` or
  `Aggregate` inner is rejected, because the outer has no named row to filter.
- **`_select` diverts only behind an outer `_where`.** It passes `max_outer_phase =
  PHASE_WHERE` to `maybe_divert`. A SQL WHERE filters source rows, while a linq `_where` after
  `_select` filters projected rows; distinct, set ops, order-by, take and skip read projected
  rows in both, so they share the SELECT (`SELECT DISTINCT proj`).
- **`_where`, `_group_by` and `_having` never divert.** They only lower `q.minPhaseSeen`, so
  a later-phase op peeled inside them is the one that diverts.
- **`snapshot_q_to_subquery_wrap` wraps in place**: it renders the query's current state as
  the FROM subquery, resets the query, and restores the outer-terminal state. It serves the
  cases where the query is already populated when the wrap is decided: a multi-join left
  side, a RIGHT / FULL OUTER join whose left side carries a WHERE, a `_where` over a join's
  named-tuple `into` (or over a join aggregate), and take / skip below an aggregate.
- **Take / skip below an aggregate apply to the rows before aggregation.**
  `maybe_wrap_take_before_aggregate` snapshots with `keepLimitInner`, so LIMIT / OFFSET stay
  on the inner subquery and the aggregate renders on the outer SELECT. Outside a join the
  inner re-emits every source column and drops any `_select` together with its SELECT-list
  binds: that select already lives inside the saved aggregate expression, which reads the
  inner's columns by name.
- **When a wrap leaves `q.rootType` null, `q.fromRowType` carries the row type of source 0.**
  `source_rootType_for_idx` / `source_rootType_for_alias` and the join null probe fall back
  to it, and `lookup_struct_field_type` resolves a named-tuple `fromRowType` by its
  `argNames`.

### Emitted SQL template {#sql-in-band-markers}

- **Emitted SQL is a template over two in-band markers.** `?` is a bind, `\x01` a runtime
  identifier (a dynamic `_order_by` column name); `next_placeholder` scans outside
  single-quoted literals and always resumes just past a placeholder, so the scan starts
  with no quote open. `sql_to_frags_ex` pairs each marker with `orderedBinds` /
  `orderedInlineIds` in occurrence order and preserves a marker as text once its list is
  exhausted. Nothing reaches the text unquoted: a compile-time constant folds in with SQL
  doubling, a non-constant emits a runtime `sql_quote_id` / `sql_quote_lit` call, and every
  other runtime value stays a `?`.
- **A string-typed order key that does not read the row is a runtime column name.**
  `collect_one_order_key` tests it with `mentions_source_arg`: a key that reads nothing of the
  row goes to `q.orderByInlineExprs` and emits the `\x01` marker; a string key that reads the
  row (`to_lower(_.Name)`) is a computed key instead.

### Bind order {#sql-bind-order}

- **Placeholders are numbered in SQL text order, so each bind list is assembled in its
  clause's parse order.** A computed projection slot renders through `pred_to_sql`, which
  pushes into `q.bindExprs`; `analyze_projection` moves the binds its slots pushed into
  `q.projBindExprs`, because the SELECT list parses before FROM, ON and WHERE. `_sql_update`
  renders the SET values in a `SqlQuery` of their own, so SET binds stay apart from the
  WHERE's, and binds SET before WHERE.

### Key contexts and inlined constants {#sql-inlined-key-constants}

- **Key contexts render with constants inlined because their fragment is re-used.** A
  computed `_group_by` / `_order_by` key fragment is emitted at several SQL positions
  (SELECT, GROUP BY, ORDER BY) while a placeholder can be bound only once, so
  `render_inlined_key_sql` sets `q.inlineConstants`, which renders `ExprConst*` literals
  inline (strings through `sql_quote_lit`), and rejects any key that still pushed a bind.

### Group-key representation {#sql-group-keys}

- **A `_group_by` key lives at index `i` of three parallel arrays.** `groupByKeyExprs[i] ==
  null` marks a plain `_.Field` key: `groupByCols[i]` holds the quoted daslang field name, and
  each emitter maps it through `@sql_column` (`group_key_column_name` strips the quotes). A
  non-null `groupByKeyExprs[i]` marks a rendered key: `groupByCols[i]` is a parenthesized SQL
  fragment - a computed key, or a post-join `_.<alias>` resolved to the join slot's qualified
  column - emitted verbatim in GROUP BY, in the grouped SELECT for `_._0` / `_._0._N`, and in
  ORDER BY. Every reader tests `groupByKeyExprs[i]` to tell the two apart.

### The _distinct_by wrap {#sql-distinct-by-wrap}

- **`_distinct_by(K)` is dialect-routed on `caps.distinct_on`.** Without it, SQLite's
  bare-aggregate form `SELECT *, MIN/MAX(pk) FROM t GROUP BY K` keeps the `*` columns of
  the min-pk (or max-pk) row per K - SQLite-only semantics; strict engines reject bare
  columns beside GROUP BY. With it, `SELECT DISTINCT ON (K) * ... ORDER BY K, pk [DESC]` - a
  PostgreSQL extension DuckDB also implements. MIN/ASC is linq's "first row per K", MAX/DESC
  is `reverse() |> _distinct_by(K)` "last row per K" - both only while pk is monotonic with
  insertion order.
- **The wrap is finalized on one of two rails.** An aggregate terminator finalizes it inside
  its own peel - `count(P)` / `long_count(P)` as `SELECT COUNT(*) FROM (<wrap>) AS t0 WHERE
  P`, `_select(_.F) |> sum/min/max/average` as that aggregate over `F` with no outer WHERE -
  and its gate, `check_distinct_by_v1_gating`, admits no other chain op. A row-returning
  chain (`to_array`, `first`) finalizes after analysis in
  `maybe_finalize_distinct_by_passthrough`: the outer SELECT keeps the source columns over
  the wrap and carries any trailing `_order_by` / `take` / `skip`, and its gate admits
  nothing else. A 1-arg `count` directly over `_distinct_by(_.K)` takes neither rail and
  emits `COUNT(DISTINCT K)`.

### Whole-row group pick {#sql-group-first-row}

- **`_._1 |> first()` in a grouped `_select` is a whole-row pick, and it lowers onto the
  `_distinct_by` passthrough rail.** `analyze_grouped_projection` moves the single `_.<field>`
  key into `q.distinctByCol` and clears the group-by state, so the inner subquery is the
  bare-aggregate or DISTINCT ON wrap (first row per key) and the outer SELECT carries no
  GROUP BY. Only a single-key group over a field key qualifies. The entry is pushed as `*`
  with `groupedSelectIsRow[i]` set: `build_sql_select` expands it to every source column, and
  `build_row_builder` rebuilds the struct from that run of consecutive columns with
  `build_row_make_struct`, one `read_via_adapter` per field at `baseCol + j`.
  `is_first_of_group_call` matches the call by its resolved name (`first` / `_first`),
  because the qmatch pattern `_._1.first()` does not match the pipe call form.

### Column resolution in pred_to_sql {#sql-column-resolution}

- **`process_join_call` and `process_cross_join_call` set `q.rootAlias = "t0"` before
  analyzing the left source, and analyze the right source in a fresh `SqlQuery` with
  `rootAlias = "t1"` and `seenJoin` unset.** A column ref is alias-qualified whenever
  `rootAlias` is non-empty - not when `seenJoin` is set - so each side's `_where` emits
  `"t0"."col"` / `"t1"."col"`.
- **Until the join's `into` binds `argNames`, a source's `_where` names its row with the
  user's own parameter (`c`), which `lookup_arg_source` cannot resolve.** Only one source
  exists at that point, so while `seenJoin` is unset an unbound receiver resolves to source 0
  - in `pred_to_sql`'s column-ref and JSON-path arms and in `is_nullable_column_ref`.
  `mentions_source_arg` applies the same fallback to a field receiver (`asReceiver`).
- **`pred_to_sql` ends in a bind fallthrough**: an expression that reaches its last arm
  becomes `?`, with the expression itself as the bind - a captured runtime value. An
  expression that reads the row cannot be bound, because the bind would reference the
  lambda's row variable, which does not exist where the query runs; `mentions_source_arg`
  sends it to a reject first.

### Membership and existence lowering {#sql-membership-lowering}

- **`_in`, `_not_in`, `_any` and `_none` reach `pred_to_sql` in two forms.** Expanded,
  `x._in(c)` is `contains(c, x)` and `_not_in` is `!contains(c, x)`, which the NOT arm peels
  into NOT IN; `_any` / `_none` are `any` / `none` calls that `fn_call_to_sql` lowers to
  EXISTS / NOT EXISTS. Inside an operator's operands, which call-macro expansion does not
  reach first, they are still `ExprCallMacro` nodes, dispatched by name with arguments
  `(element, collection-or-subquery)`. `contains` is membership only when its first argument
  is an array or iterator; with a string first argument it is a substring test, lowered to
  LIKE (GLOB on a `caps.ci_like` provider).

### The join into-projection snapshot {#sql-join-projection-snapshot}

- **A join's `into` projection registry is snapshotted because the outer projection
  clobbers the live one.** `process_join_call` copies the live projection into
  `joinProjRecordNames` + the `joinSelectCol*` arrays, which survive
  `analyze_grouped_projection`'s clear-and-repopulate. Post-join `_.<alias>` resolves
  through that snapshot in every post-join consumer - `pred_to_sql`'s column-ref arm,
  `collect_one_order_key`, `push_group_key`, `try_translate_group_aggregate`,
  `try_translate_global_aggregate` - all through `find_projection_alias` +
  `render_projection_alias_sql`. A registry miss is rejected loudly everywhere alike: falling
  back to base-table resolution would leak the unqualified base-table namespace into post-join
  predicates.

### Whole-source join projection {#sql-whole-source-projection}

- **A join `into` that returns one source variable whole (`$(l, r) => r`) projects that
  source's struct.** `analyze_projection_impl` expands it to the source's alias-qualified
  columns and records the struct in `q.projectedRowType`; `q.rootType` stays the FROM root
  (source 0). Every FullRow consumer - `build_row_builder`, and `inner_projection_type` when
  the query is wrapped as a subquery - reads `projectedRowType` first. Single-source identity
  (`$(x) => x`), scalar or tuple identity, and a nullable outer-join side are rejected.

### Single-source argument names {#sql-single-source-arg-names}

- **`normalize_single_source_arg_names` exists for the linq_das front end.** `_sql`
  resolves a single source against the placeholder `_`, but LINQ-syntax lowering splices
  the user's range variable verbatim (`$(c) => c.field`), so a single-parameter chain-op
  lambda's bound variable is renamed to `_`. Gated on a pure single-source chain - join and
  set-op key lambdas bind distinct sources and are left alone.

### DML macros read raw arguments {#sql-dml-raw-args}

- **The DML call macros keep every argument after the db - for upsert, after the row - raw.**
  `OuterArgZeroMacro.canVisitArgument` admits argument 0 only, and `OuterArgTwoMacro`
  (upsert) admits the db and the row. The free `_` in the where and set lambdas therefore
  never reaches the typer, and `pred_to_sql` reads the untyped AST. A raw SET tuple carries
  its column names on `ExprMakeTuple.recordNames`, a typed one on its type's `argNames`;
  `analyze_set_clause` and `analyze_upsert_set_clause` read either.

### Upsert primary-key dispatch {#sql-upsert-pk-dispatch}

- **`emit_upsert_body` builds two INSERT statements, with and without the primary-key
  column, and picks one per row at run time on `_sql_pk_is_unset(row)`.** An unset integer
  primary key has to be left out of the INSERT so the provider's own auto-assignment runs.

## sql_migrate

### Migration rail shape {#sql-migrate-rail}

- **The audit table is provider-neutral by construction**: client-side epoch seconds, in a
  BIGINT column.
- **Duplicate versions are caught in two layers, because neither sees everything**: the
  `[sql_migration]` annotation at compile time, within one program; `_duplicate_version_error`
  at run time, across every module that registered.
- **The `struct_convert_field` overload set is a specificity ladder** - deleting a
  "duplicate" silently re-routes conversions.
