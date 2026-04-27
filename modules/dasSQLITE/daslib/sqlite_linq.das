options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module sqlite_linq shared public

require sqlite/sqlite_boost public
require daslib/sql public
require daslib/linq_boost public

require daslib/ast public
require daslib/ast_boost
require daslib/ast_match
require daslib/templates
require daslib/templates_boost
require daslib/macro_boost
require daslib/rtti
require strings public
require daslib/strings_convert

// ============================================================================
// _to_array passthrough
//
// Recognized by the `_sql` macro as a no-op terminal. In compatibility mode
// (no `_sql` wrapper) it forwards to the bare materialization done by
// `select_from` (which already returns `array<T>`).
// ============================================================================

[unused_argument(arr)]
def _to_array(var arr : array<auto(TT)>) : array<TT -const -#> {
    //! Passthrough terminal. In compat mode `select_from` has already materialized
    //! the array; this just yields it. Inside `_sql(...)` the macro recognizes
    //! the call and treats it as a no-op terminal.
    return <- arr
}

// ============================================================================
// _first_opt — Option<T>-returning sibling of linq.das's `first`.
//
// Inside `_sql(...)` the macro recognizes the call, emits `LIMIT 1`, and
// uses the OneOpt materializer. In compat mode (no `_sql` wrapper) the
// fallback inspects the materialized array.
// ============================================================================

def _first_opt(arr : array<auto(TT)>) : Option<TT -const -#> {
    //! Compat-mode fallback: returns ``some(arr[0])`` if non-empty, ``none`` otherwise.
    //! Inside `_sql(...)` the macro intercepts this call before evaluation
    //! and emits ` LIMIT 1` with the OneOpt materializer.
    if (length(arr) > 0) {
        return some(arr[0])
    }
    return none(type<TT -const -#>)
}

def _first(arr : array<auto(TT)>) : TT -const -& {
    //! Compat-mode fallback: delegates to ``daslib/linq.das`` ``first``,
    //! which panics with "sequence contains no elements" on empty and
    //! handles non-copyable ``TT`` via ``clone_to_move``. Inside `_sql(...)`
    //! the macro intercepts this call before evaluation and emits ` LIMIT 1`
    //! with the One materializer.
    return first(arr)
}


// ============================================================================
// SqlQuery — accumulator for chain analysis (compile-time only).
//
// Materializer and ProjectionShape are the framework discriminators. Each
// chain operator recognized in `analyze_chain` sets one or both, then the
// emit pass in `_sql.visit()` switches on them to choose runtime helper +
// row builder.
// ============================================================================

enum private Materializer {
    Array        //!< default: `array<T>` via run_select
    One          //!< exactly-one row (panic on 0); covers `_first`, `_count`
    OneOpt       //!< 0-or-1 row, returns `Option<T>`; covers `_first_opt`
}

enum private ProjectionShape {
    FullRow             //!< no `_select`: SELECT cols-of(rootT); reads via _::_sql_read_row
    SingleColumn        //!< `_select(_.Field)`: SELECT one col; reads via sqlite_read
    NamedTuple          //!< `_select((Name=_.Name, Price=_.Price))`: SELECT cols; reads via tuple builder
    Aggregate           //!< `_count()` etc.: raw SQL expression in SELECT; scalar reader
    GroupedNamedTuple   //!< `_select` after `_group_by`: tuple of group keys + group-scope aggregates
}

struct private SqlQuery {
    rootType    : TypeDeclPtr
    tableName   : string
    selectCols  : array<string>           //!< SingleColumn / FullRow / NamedTuple: column names (quoted in SQL)
    projRecordNames : array<string>       //!< NamedTuple: per-column tuple field names (parallel to selectCols)
    aggregateExpr : string                //!< Aggregate: raw SELECT-list text (e.g. "COUNT(*)")
    aggregateType : TypeDeclPtr           //!< Aggregate: resolved scalar result type; varies by aggregate expression (int for COUNT, double for AVG, column type for SUM/MIN/MAX)
    whereSql    : string
    bindExprs   : array<ExpressionPtr>    //!< WHERE binds, populated during analyze_chain
    limitBindExpr : ExpressionPtr         //!< take(n) bind expr (null = no LIMIT, except terminal-induced)
    offsetBindExpr : ExpressionPtr        //!< skip(n) bind expr (null = no OFFSET)
    distinctFlag : bool                   //!< distinct() seen → emits "SELECT DISTINCT"
    orderByCols : array<string>           //!< each entry like `"Col" ASC` / `"Col" DESC`
    groupByCols : array<string>           //!< each entry like `"Col"` (already SQL-quoted)
    havingSql : string                    //!< pre-formatted HAVING clause (without leading "HAVING ")
    havingBindExprs : array<ExpressionPtr>  //!< HAVING binds, kept separate so they sort after WHERE binds
    groupedSelectExprs : array<string>    //!< GroupedNamedTuple: raw SQL fragment per column
    groupedSelectTypes : array<TypeDeclPtr>  //!< GroupedNamedTuple: resolved scalar type per column
    matr        : Materializer = Materializer.Array
    proj        : ProjectionShape = ProjectionShape.FullRow
    seenSelect  : bool
    seenToArray : bool
    seenTerminal : bool                   //!< true once a single-row/scalar terminal has been peeled
    seenDistinct : bool                   //!< duplicate-stage guard for distinct()
    seenTake    : bool                    //!< duplicate-stage guard for take(n)
    seenSkip    : bool                    //!< duplicate-stage guard for skip(n)
    seenOrderBy : bool                    //!< duplicate-stage guard for _order_by
    seenGroupBy : bool                    //!< _group_by seen — group-scope context for downstream _select / _having
    seenHaving  : bool                    //!< duplicate-stage guard for _having
    inHaving    : bool                    //!< pred_to_sql writes binds to havingBindExprs while this is true
    dbExpr      : ExpressionPtr
}

// ============================================================================
// Chain analysis — walk a pipe-composed chain backwards.
//
// `_sql` runs as a normal call_macro (no `canVisitArgument` override), so by
// the time `analyze_chain` runs daslang has already expanded all inner
// call_macros bottom-up. The chain we see is the post-expansion shape:
//
//   _where(it, expr)  →  where_(it, $(_ : T) => expr)         [from linq_boost]
//   _select(it, expr) →  select(it,  $(_ : T) => expr)        [from linq_boost]
//   _first(it)        →  first(it)                             [linq.das fn]
//   _count(it)        →  count(it)                             [linq.das fn]
//   _to_array(arr)    →  _to_array(arr)                        [plain fn, stays]
//   _first_opt(arr)   →  _first_opt(arr)                       [plain fn, stays]
//   select_from(db, type<T>) → unchanged                       [plain fn]
//
// Because expansion is complete, `_` inside the lambda body is a real bound
// parameter with type T, captured vars carry their `_type`, and any
// user-defined wrapper macro (e.g. `when_its_cold(it)` → `_where(it, ...)`)
// has already cascaded into a `where_` / `select` we recognize.
// ============================================================================

[macro_function]
def private peel_lambda_body(var lam : ExpressionPtr) : ExpressionPtr {
    //! Peel `$(_ : T) => body` (post-expansion linq lambda) → body expression.
    //! Returns null if shape is unexpected. Mirrors `linq_boost.fold_linq_cond`.
    if (lam == null || !(lam is ExprMakeBlock)) {
        return null
    }
    var mblk = lam as ExprMakeBlock
    var blk = mblk._block as ExprBlock
    if (blk == null || blk.arguments |> length != 1 || blk.list |> length != 1) {
        return null
    }
    if (!(blk.list[0] is ExprReturn)) {
        return null
    }
    var ret = blk.list[0] as ExprReturn
    return ret.subexpr
}

[macro_function]
def private match_linq_call(var expr : ExpressionPtr; name : string) : ExprCall? {
    //! Match a fully-resolved call to `name` from module `linq`. Walks
    //! through `fromGeneric` so generic instances are recognized. Mirrors
    //! `linq_boost.is_call_or_generic`.
    if (expr == null || !(expr is ExprCall)) {
        return null
    }
    var call = expr as ExprCall
    if (call.func == null || call.func._module == null) {
        return null
    }
    if (call.func.name == name && call.func._module.name == "linq") {
        return call
    }
    if (call.func.fromGeneric != null
            && call.func.fromGeneric.name == name
            && call.func.fromGeneric._module != null
            && call.func.fromGeneric._module.name == "linq") {
        return call
    }
    return null
}

[macro_function]
def private match_module_call(var expr : ExpressionPtr; name : string; modName : string) : ExprCall? {
    //! Like match_linq_call but matches against any named module — used to
    //! recognize sqlite_linq's own plain functions like `_first_opt` after
    //! expansion (linq_boost recognizes `_first_opt` only as a fall-through
    //! to its underlying call; for `_sql` we want to peel the call here).
    if (expr == null || !(expr is ExprCall)) {
        return null
    }
    var call = expr as ExprCall
    if (call.func == null || call.func._module == null) {
        return null
    }
    if (call.func.name == name && call.func._module.name == modName) {
        return call
    }
    if (call.func.fromGeneric != null
            && call.func.fromGeneric.name == name
            && call.func.fromGeneric._module != null
            && call.func.fromGeneric._module.name == modName) {
        return call
    }
    return null
}

[macro_function]
def private peel_column_aggregate(var node : ExpressionPtr; var q : SqlQuery&;
                                  linqName : string; sqlOp : string; promote_to_double : bool;
                                  prog : ProgramPtr; at : LineInfo) : bool {
    //! Peel a column aggregate (sum/average/min/max) wrapping a SingleColumn
    //! projection. Returns true if this node matched (regardless of success);
    //! the caller treats `q.aggregateExpr != ""` as success and returns true,
    //! else propagates the macro_error already emitted.
    //!
    //! Strategy: recurse into the inner chain FIRST so the inner `_select` /
    //! `select_from` populates q.selectCols, then wrap selectCols[0] in
    //! `sqlOp(...)`. This lets the same slot serve any column aggregate.
    var aCall = match_linq_call(node, linqName)
    if (aCall == null) {
        return false
    }
    if (q.seenTerminal || q.seenToArray) {
        macro_error(prog, at, "_sql: only one terminal allowed per chain")
        return true
    }
    if (aCall.arguments |> length != 1) {
        macro_error(prog, at, "_sql: {linqName}(predicate) is not yet supported in `_sql` chains; use `_select(_.Col) |> {linqName}()` for column aggregates")
        return true
    }
    if (!analyze_chain(aCall.arguments[0], q, prog, at)) {
        return true
    }
    if (q.proj != ProjectionShape.SingleColumn || length(q.selectCols) != 1) {
        macro_error(prog, at, "_sql: {linqName}() needs an upstream `_select(_.Col)` projecting exactly one column (got proj={int(q.proj)}, cols={length(q.selectCols)})")
        return true
    }
    let col = q.selectCols[0]
    q.aggregateExpr = "{sqlOp}(\"{col}\")"
    if (promote_to_double) {
        q.aggregateType = new TypeDecl(at = at, baseType = Type.tDouble)
    } else {
        q.aggregateType = lookup_struct_field_type(q.rootType, col)
    }
    q.proj = ProjectionShape.Aggregate
    q.matr = Materializer.One
    q.seenTerminal = true
    return true
}

[macro_function]
def private analyze_chain(var node : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    if (node == null) {
        macro_error(prog, at, "_sql: empty chain")
        return false
    }

    // ---- Terminal recognition (must come before _select / _where peel) ----
    // These set the materializer and (sometimes) the projection, then recurse
    // into the inner chain. Only one terminal allowed per chain.

    // Peel _to_array(prev) — plain function in this module, no expansion.
    {
        var inner : ExpressionPtr
        if (qmatch(node, _to_array($e(inner))).matched) {
            if (q.seenToArray || q.seenTerminal) {
                macro_error(prog, at, "_sql: only one terminal allowed per chain")
                return false
            }
            // _to_array() must be the outermost chain element. analyze_chain
            // walks outermost-first, so any operator already peeled (any of
            // these flags / fields populated) means _to_array is buried under
            // a wrapper like _where(...) that would mean "filter after
            // materialize" — not translatable to SQL.
            if (q.seenSelect || q.whereSql != "") {
                macro_error(prog, at, "_sql: _to_array() must be the outermost chain element")
                return false
            }
            q.seenToArray = true
            q.matr = Materializer.Array
            return analyze_chain(inner, q, prog, at)
        }
    }

    // Peel first(prev) — written as either `|> first()` (linq.das `first`)
    // or `|> _first()` (sqlite_linq's underscore-form passthrough).
    {
        var fCall = match_linq_call(node, "first")
        if (fCall == null) {
            fCall = match_module_call(node, "_first", "sqlite_linq")
        }
        if (fCall != null) {
            if (q.seenTerminal || q.seenToArray) {
                macro_error(prog, at, "_sql: only one terminal allowed per chain")
                return false
            }
            if (fCall.arguments |> length != 1) {
                macro_error(prog, at, "_sql: _first(predicate) is chunk 4; chunk 3 supports `|> _first()` only")
                return false
            }
            q.matr = Materializer.One
            q.seenTerminal = true
            return analyze_chain(fCall.arguments[0], q, prog, at)
        }
    }

    // Peel _first_opt(prev) — plain function in this module.
    {
        var foCall = match_module_call(node, "_first_opt", "sqlite_linq")
        if (foCall != null) {
            if (q.seenTerminal || q.seenToArray) {
                macro_error(prog, at, "_sql: only one terminal allowed per chain")
                return false
            }
            q.matr = Materializer.OneOpt
            q.seenTerminal = true
            return analyze_chain(foCall.arguments[0], q, prog, at)
        }
    }

    // Peel count(prev) — post-expansion of `_count()` (linq.das `count`).
    // Sets MaterializeOne + ProjAggregate("COUNT(*)", int64). Canary for the
    // chunk-3 aggregate framework; chunk 4 broadens to _sum / _avg / etc.
    {
        var cCall = match_linq_call(node, "count")
        if (cCall != null) {
            if (q.seenTerminal || q.seenToArray) {
                macro_error(prog, at, "_sql: only one terminal allowed per chain")
                return false
            }
            if (cCall.arguments |> length != 1) {
                macro_error(prog, at, "_sql: _count(predicate) is chunk 4; chunk 3 supports `|> _count()` only")
                return false
            }
            q.matr = Materializer.One
            q.seenTerminal = true
            q.proj = ProjectionShape.Aggregate
            q.aggregateExpr = "COUNT(*)"
            // SQLite's COUNT(*) is int64 internally, but daslang's
            // `length(arr)` / `count(arr)` return `int`, so narrow via
            // `sqlite_read(stmt, 0, type<int>)`. Matches the grouped
            // form (`_._1 |> length`) which also lands as `int`. Any
            // plausible row count fits.
            q.aggregateType = new TypeDecl(at = at, baseType = Type.tInt)
            return analyze_chain(cCall.arguments[0], q, prog, at)
        }
    }

    // ---- Column aggregates: sum / average / min / max ----
    // Pattern: `... |> _select(_.Col) |> sum()` (or average/min/max).
    // Implementation note: recurse into the inner chain FIRST so selectCols
    // is populated, then wrap selectCols[0] in the SQL aggregate. This lets
    // the same emitter slot serve any column-aggregate primitive — adding a
    // new one is a one-line entry plus a column-type rule.
    {
        if (peel_column_aggregate(node, q, "sum",     "SUM", false, prog, at)) {
            return q.aggregateExpr != ""
        }
    }
    {
        if (peel_column_aggregate(node, q, "average", "AVG", true,  prog, at)) {
            return q.aggregateExpr != ""
        }
    }
    {
        if (peel_column_aggregate(node, q, "min",     "MIN", false, prog, at)) {
            return q.aggregateExpr != ""
        }
    }
    {
        if (peel_column_aggregate(node, q, "max",     "MAX", false, prog, at)) {
            return q.aggregateExpr != ""
        }
    }

    // ---- Stage operators ----

    // Peel distinct(prev) — linq.das's `distinct` plain function. Sets
    // SELECT DISTINCT on the emitted SQL. The SELECT-list itself comes from
    // the rest of the chain (FullRow / SingleColumn / NamedTuple); aggregates
    // pair with DISTINCT only via the function-internal form (e.g.
    // COUNT(DISTINCT col)) which is not yet wired — flag a guard for that.
    {
        var dCall = match_linq_call(node, "distinct")
        if (dCall != null) {
            if (q.seenDistinct) {
                macro_error(prog, at, "_sql: distinct() can appear at most once in the chain")
                return false
            }
            if (dCall.arguments |> length != 1) {
                macro_error(prog, at, "_sql: distinct() takes no explicit arguments (chain form: `|> distinct()`)")
                return false
            }
            q.distinctFlag = true
            q.seenDistinct = true
            return analyze_chain(dCall.arguments[0], q, prog, at)
        }
    }

    // Peel take(prev, n) — linq.das's `take(arr, int)`. Sets LIMIT n.
    // `n` is stashed on q.limitBindExpr; the bind statement is emitted in
    // emit_sql_macro_body AFTER the WHERE binds so the placeholder index
    // (LIMIT comes after WHERE in SQL) lines up correctly.
    {
        var tCall = match_linq_call(node, "take")
        if (tCall != null) {
            if (q.seenTake) {
                macro_error(prog, at, "_sql: take(n) can appear at most once in the chain")
                return false
            }
            if (tCall.arguments |> length != 2) {
                macro_error(prog, at, "_sql: only the `take(arr, n : int)` overload (chain form: `|> take(n)`) is supported in `_sql` chains; `take(arr, range)` is not translatable")
                return false
            }
            q.limitBindExpr = clone_expression(tCall.arguments[1])
            q.seenTake = true
            return analyze_chain(tCall.arguments[0], q, prog, at)
        }
    }

    // Peel order_by(prev, lambda) — post-expansion of `_order_by(prev, expr)`.
    // Single-column: body is `_.Col` → push `"Col" ASC`.
    // Multi-column: body is a tuple literal `(_.k1, _.k2)` → push each
    // `"k_i" ASC`. After `_group_by`, body is `_._0` (single key) or
    // `_._0._N` (multi-key). Mixed ASC/DESC across columns is out of
    // chunk-4 scope (use the raw-SQL escape hatch); see plan D2.
    //
    // Recurse FIRST so `q.seenGroupBy` / `q.groupByCols` are populated by
    // the time `collect_order_keys` runs, letting it route `_._0` to the
    // correct group-by column.
    {
        var obCall = match_linq_call(node, "order_by")
        if (obCall != null) {
            if (q.seenOrderBy) {
                macro_error(prog, at, "_sql: _order_by() can appear at most once in the chain (use a tuple key for multi-column ordering: `_order_by((_.k1, _.k2))`)")
                return false
            }
            var body = peel_lambda_body(obCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _order_by key lambda has unexpected shape")
                return false
            }
            if (!analyze_chain(obCall.arguments[0], q, prog, at)) {
                return false
            }
            if (!collect_order_keys(body, q, "ASC", prog, at)) {
                return false
            }
            q.seenOrderBy = true
            return true
        }
    }

    // Peel order_by_descending(prev, lambda) — post-expansion of
    // `_order_by_descending(prev, expr)`. Same shape as order_by; emits DESC.
    {
        var odCall = match_linq_call(node, "order_by_descending")
        if (odCall != null) {
            if (q.seenOrderBy) {
                macro_error(prog, at, "_sql: _order_by() can appear at most once in the chain (use a tuple key for multi-column ordering)")
                return false
            }
            var body = peel_lambda_body(odCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _order_by_descending key lambda has unexpected shape")
                return false
            }
            if (!analyze_chain(odCall.arguments[0], q, prog, at)) {
                return false
            }
            if (!collect_order_keys(body, q, "DESC", prog, at)) {
                return false
            }
            q.seenOrderBy = true
            return true
        }
    }

    // Peel skip(prev, n) — linq.das's `skip(arr, int)`. Sets OFFSET n.
    // OFFSET in SQL requires LIMIT; SQLite accepts `LIMIT -1 OFFSET n` to
    // skip-without-limit, which build_sql_string emits when seenSkip without
    // seenTake.
    {
        var sCall = match_linq_call(node, "skip")
        if (sCall != null) {
            if (q.seenSkip) {
                macro_error(prog, at, "_sql: skip(n) can appear at most once in the chain")
                return false
            }
            if (sCall.arguments |> length != 2) {
                macro_error(prog, at, "_sql: only the `skip(arr, n : int)` overload (chain form: `|> skip(n)`) is supported in `_sql` chains")
                return false
            }
            q.offsetBindExpr = clone_expression(sCall.arguments[1])
            q.seenSkip = true
            return analyze_chain(sCall.arguments[0], q, prog, at)
        }
    }

    // Peel select(prev, lambda) — post-expansion of `_select(prev, expr)`.
    // Recurses into the inner chain FIRST so any downstream `_group_by` has
    // populated `q.seenGroupBy` by the time we analyze the projection body.
    // The grouped path uses `analyze_grouped_projection` (mixed group keys +
    // group-scope aggregate primitives); the row-scope path uses
    // `analyze_projection`.
    {
        var sCall = match_linq_call(node, "select")
        if (sCall != null) {
            if (q.seenSelect) {
                macro_error(prog, at, "_sql: _select can only appear once in the chain")
                return false
            }
            if (q.proj == ProjectionShape.Aggregate) {
                // Soft pass-through: aggregate terminals (count/sum/...) emit their
                // own projection (e.g. COUNT(*)) and ignore any prior _select.
                // Mark seenSelect so select_from doesn't fill default columns —
                // the aggregate's projection wins. The inner _select's body is
                // still walked for side-effect-free typing but its column choice
                // does not affect the emitted SQL.
                q.seenSelect = true
                return analyze_chain(sCall.arguments[0], q, prog, at)
            }
            // Recurse first so q.seenGroupBy reflects whether a downstream
            // `_group_by` will fuse with this `_select`.
            if (!analyze_chain(sCall.arguments[0], q, prog, at)) {
                return false
            }
            var body = peel_lambda_body(sCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _select projection lambda has unexpected shape")
                return false
            }
            if (q.seenGroupBy) {
                if (!analyze_grouped_projection(body, q, prog, at)) {
                    return false
                }
            } else {
                if (!analyze_projection(body, q, prog, at)) {
                    return false
                }
            }
            q.seenSelect = true
            return true
        }
    }

    // Peel having_(prev, lambda) — post-expansion of `_having(prev, predicate)`.
    // Recurses into the inner chain FIRST so we can validate that `_group_by`
    // is downstream (HAVING without GROUP BY is meaningless in SQL). Binds
    // captured by the predicate go to `q.havingBindExprs` so they can be
    // appended after WHERE binds in the placeholder index sequence.
    {
        var hCall = match_linq_call(node, "having_")
        if (hCall != null) {
            if (hCall.arguments |> length != 2) {
                macro_error(prog, at, "_sql: _having has unexpected shape; expected having_(prev, predicate)")
                return false
            }
            let beforeGroupBy = q.seenGroupBy
            if (!analyze_chain(hCall.arguments[0], q, prog, at)) {
                return false
            }
            if (q.seenHaving) {
                macro_error(prog, at, "_sql: _having can appear at most once in the chain")
                return false
            }
            if (!q.seenGroupBy) {
                macro_error(prog, at, "_sql: _having requires a preceding _group_by; for row-level filters use _where")
                return false
            }
            if (beforeGroupBy) {
                // _group_by was set BEFORE we recursed → it sits OUTSIDE this
                // _having in source order = HAVING came before GROUP BY.
                macro_error(prog, at, "_sql: _having must appear AFTER _group_by in the chain")
                return false
            }
            var body = peel_lambda_body(hCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _having predicate lambda has unexpected shape")
                return false
            }
            q.inHaving = true
            let s = pred_to_sql(body, q, prog, at)
            q.inHaving = false
            if (empty(s)) {
                return false   // pred_to_sql already issued a macro_error
            }
            q.havingSql = s
            q.seenHaving = true
            return true
        }
    }

    // Peel group_by_lazy(prev, lambda) — post-expansion of both `_group_by`
    // and `_group_by_lazy` (linq_boost lowers both to the same backing call).
    // Inside `_sql` both forms emit GROUP BY; the IGrouping per-group
    // iterators never materialize at runtime because `_sql` rewrites the
    // entire chain into a `run_select` call before evaluation. Users who
    // want the actual IGrouping per-group iterator should not wrap in
    // `_sql(...)`.
    {
        var gbCall = match_linq_call(node, "group_by_lazy")
        if (gbCall != null) {
            if (q.seenGroupBy) {
                macro_error(prog, at, "_sql: _group_by can appear at most once in the chain")
                return false
            }
            if (gbCall.arguments |> length != 2) {
                macro_error(prog, at, "_sql: _group_by expects (chain, key)")
                return false
            }
            var body = peel_lambda_body(gbCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _group_by key lambda has unexpected shape")
                return false
            }
            if (!collect_group_keys(body, q, prog, at)) {
                return false
            }
            q.seenGroupBy = true
            return analyze_chain(gbCall.arguments[0], q, prog, at)
        }
    }

    // Peel where_(prev, lambda) — post-expansion of `_where(prev, predicate)`.
    {
        var wCall = match_linq_call(node, "where_")
        if (wCall != null) {
            var body = peel_lambda_body(wCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _where predicate lambda has unexpected shape")
                return false
            }
            if (!analyze_predicate(body, q, prog, at)) {
                return false
            }
            return analyze_chain(wCall.arguments[0], q, prog, at)
        }
    }

    // Bottom: select_from(db, type<T>) — plain function in sqlite_boost.
    {
        var dbE : ExpressionPtr
        var rootT : TypeDeclPtr
        if (qmatch(node, select_from($e(dbE), type<$t(rootT)>)).matched) {
            q.dbExpr = dbE
            q.rootType = rootT
            // Default projection if no _select was seen above
            if (!q.seenSelect && q.proj != ProjectionShape.Aggregate) {
                fill_default_columns(q, rootT, prog, at)
                q.proj = ProjectionShape.FullRow
            }
            q.tableName = sql_table_name_from_type(rootT)
            return true
        }
    }

    macro_error(prog, at, "_sql: unsupported chain root or operator. Got: {describe(node)}")
    return false
}

// ============================================================================
// Projection analysis (_select).
//
// Supported projection shapes:
//   _select(_.FieldName)                 -> single-column projection
//   _select((Name=_.X, Other=_.Y))       -> named-tuple projection (recordNames preserved)
//
// Struct-type projection (_select(type<T2>)) is deferred to chunk 4.
//
// `body` is the projection expression (e.g. `_.Name`) after Mode-2
// expansion has already lambda-wrapped it via `select(it, $(_:T) => body)`,
// so what we receive is the inner body extracted by `peel_lambda_body`.
// The typer wraps field reads in `ExprRef2Value` post-expansion, so we
// peel that adapter before pattern-matching the column shape.
// ============================================================================

[macro_function]
def private analyze_projection(var body : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    if (body == null) {
        macro_error(prog, at, "_sql: _select: missing projection")
        return false
    }
    // After Mode-2 expansion the typer may wrap field reads in ExprRef2Value
    // (the value-from-ref adapter). Peel it before pattern-matching.
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
        if (body == null) {
            macro_error(prog, at, "_sql: _select: ExprRef2Value with null subexpr")
            return false
        }
    }

    // Single-column form: _.FieldName
    {
        var fieldName : string
        if (qmatch(body, _.$f(fieldName)).matched) {
            q.selectCols |> clear
            q.selectCols |> push(fieldName)
            q.proj = ProjectionShape.SingleColumn
            return true
        }
    }

    // Named-tuple form: (Name=_.Name, Price=_.Price). recordNames live on
    // the inferred tuple type (TypeDecl.argNames) — the ExprMakeTuple's
    // own recordNames vector isn't exposed to daslang.
    if (body is ExprMakeTuple) {
        var mkTup = body as ExprMakeTuple
        var tupT = mkTup._type
        if (tupT != null && tupT.baseType == Type.tTuple
                && length(tupT.argNames) > 0
                && length(mkTup.values) == length(tupT.argNames)) {
            q.selectCols |> clear
            q.projRecordNames |> clear
            for (i in range(length(mkTup.values))) {
                var val = mkTup.values[i]
                if (val is ExprRef2Value) {
                    val = (val as ExprRef2Value).subexpr
                }
                var fieldName : string
                if (qmatch(val, _.$f(fieldName)).matched) {
                    q.selectCols |> push(fieldName)
                    q.projRecordNames |> push(string(tupT.argNames[i]))
                } else {
                    macro_error(prog, at, "_sql: _select named-tuple values must be `_.Field` shape; got: {describe(val)}")
                    return false
                }
            }
            q.proj = ProjectionShape.NamedTuple
            return true
        }
    }

    macro_error(prog, at, "_sql: _select: supported shapes are `_.Field` (single column) and `(Name=_.Name, Price=_.Price)` (named tuple); got: {describe(body)}")
    return false
}

// ============================================================================
// Grouped projection analysis (_select after _group_by) — IGrouping shape.
//
// `_group_by` lowers to ``group_by_lazy`` which yields per-bucket tuples
// of shape ``tuple<KeyType; array<RowType>>`` — index ``_0`` is the group
// key, index ``_1`` is the array of group elements. The user expresses
// SQL aggregates against that shape, and `_sql` translates the patterns
// to GROUP BY + the corresponding SQL aggregate function:
//
//   _._0                                    → group key column (single-key)
//   _._0._N                                 → multi-key Nth column
//   _._1 |> length                                 → COUNT(*)
//   _._1 |> count                                  → COUNT(*)
//   _._1 |> select($(u:T) => u.X) |> sum           → SUM("X")
//   _._1 |> select($(u:T) => u.X) |> average       → AVG("X")  (result type double)
//   _._1 |> select($(u:T) => u.X) |> min           → MIN("X")
//   _._1 |> select($(u:T) => u.X) |> max           → MAX("X")
//
// Required projection shape is a NAMED tuple literal so the result row
// schema (column names) is explicit. The same aggregate recognition is
// reused inside `_having` predicates by `try_translate_group_aggregate`.
// ============================================================================

[macro_function]
def private peel_ref2val(var node : ExpressionPtr) : ExpressionPtr {
    //! Strip a single ``ExprRef2Value`` wrapper that the typer inserts
    //! around field reads after Mode-2 inner expansion. Returns ``node``
    //! unchanged if not wrapped.
    if (node != null && node is ExprRef2Value) {
        var sub = (node as ExprRef2Value).subexpr
        if (sub != null) {
            return sub
        }
    }
    return node
}

[macro_function]
def private is_underscore_field(var node : ExpressionPtr; expectedField : string) : bool {
    //! True if ``node`` is the ExprField ``_.<expectedField>`` (the lambda
    //! parameter ``_`` followed by a specific tuple-index or struct field).
    //! Inlined manually rather than via qmatch — qmatch inside helper
    //! functions called from a macro_function body did not match reliably
    //! in this context.
    var n = peel_ref2val(node)
    if (n == null || !(n is ExprField)) {
        return false
    }
    var ef = n as ExprField
    if (ef.name != expectedField) {
        return false
    }
    var recv = peel_ref2val(ef.value)
    if (recv == null || !(recv is ExprVar)) {
        return false
    }
    return (recv as ExprVar).name == "_"
}

[macro_function]
def private group_key_column_name(q : SqlQuery; idx : int) : string {
    //! Return the unquoted column name of the idx-th group-by key.
    //! ``q.groupByCols[idx]`` is already SQL-quoted (e.g. ``"\"City\""``);
    //! strip the surrounding quotes.
    let raw = q.groupByCols[idx]
    let n = length(raw)
    if (n >= 2 && raw |> starts_with("\"") && raw |> ends_with("\"")) {
        return slice(raw, 1, n - 1)
    }
    return raw
}

[macro_function]
def private try_translate_group_aggregate(var node : ExpressionPtr; q : SqlQuery; prog : ProgramPtr; at : LineInfo;
                                          var sqlOut : string&; var typeOut : TypeDeclPtr&) : bool {
    //! Recognize one of the IGrouping-shape SQL aggregate patterns and
    //! fill ``sqlOut`` / ``typeOut``. Returns true on a recognized
    //! aggregate (even on column-resolution failure — caller checks
    //! ``empty(sqlOut)``); false if the node isn't one of the recognized
    //! aggregate forms.
    var n = peel_ref2val(node)
    if (n == null || !(n is ExprCall)) {
        return false
    }
    var call = n as ExprCall
    if (call.func == null) {
        return false
    }
    var fname = string(call.func.name)
    if (call.func.fromGeneric != null) {
        fname = string(call.func.fromGeneric.name)
    }
    let argc = user_arg_count(call)

    // length(_._1) / count(_._1) → COUNT(*)
    // Result type is `int` to match daslang's `length`/`count` return on
    // arrays. SQLite COUNT(*) returns int64 internally but sqlite_read
    // narrows to int via sqlite3_column_int — fine for any plausible row
    // count and matches the user's projection-tuple inferred type.
    if ((fname == "length" || fname == "count") && argc == 1
            && is_underscore_field(call.arguments[0], "_1")) {
        sqlOut = "COUNT(*)"
        typeOut = new TypeDecl(at = at, baseType = Type.tInt)
        return true
    }

    // sum / average / min / max applied to select(_._1, $(p:T) => p.X) → SUM("X") / etc.
    // The inner lambda's parameter name is intentionally NOT required to be
    // `_` — daslang flags `_` shadowing across nested lambdas as a compile
    // error (the outer lambda's `_` is the group tuple, the inner is a row),
    // so users name the inner parameter explicitly (`$(u : User) => u.Age`).
    if ((fname == "sum" || fname == "average" || fname == "min" || fname == "max") && argc == 1) {
        var inner : ExpressionPtr = call.arguments[0]
        var innerCall = match_linq_call(inner, "select")
        if (innerCall != null && innerCall.arguments |> length == 2
                && is_underscore_field(innerCall.arguments[0], "_1")) {
            var lamBody = peel_lambda_body(innerCall.arguments[1])
            if (lamBody == null) {
                macro_error(prog, at, "_sql: aggregate's inner select lambda has unexpected shape")
                return true
            }
            lamBody = peel_ref2val(lamBody)
            // Body must be `<param>.<field>` — an ExprField rooted at a
            // single ExprVar (the lambda's parameter, whatever its name).
            if (lamBody != null && lamBody is ExprField) {
                var ef = lamBody as ExprField
                var recv = peel_ref2val(ef.value)
                if (recv != null && recv is ExprVar) {
                    let col = string(ef.name)
                    var sqlOp = ""
                    if (fname == "sum") {
                        sqlOp = "SUM"
                    } elif (fname == "average") {
                        sqlOp = "AVG"
                    } elif (fname == "min") {
                        sqlOp = "MIN"
                    } else {
                        sqlOp = "MAX"
                    }
                    sqlOut = "{sqlOp}(\"{col}\")"
                    if (fname == "average") {
                        typeOut = new TypeDecl(at = at, baseType = Type.tDouble)
                    } else {
                        var colType = lookup_struct_field_type(q.rootType, col)
                        if (colType == null) {
                            macro_error(prog, at, "_sql: cannot resolve column '{col}' in {fname}(...)")
                            return true
                        }
                        typeOut = colType
                    }
                    return true
                }
            }
            macro_error(prog, at, "_sql: aggregate's inner select body must be `<param>.<field>`; got: {describe(lamBody)}")
            return true
        }
    }
    return false
}

[macro_function]
def private analyze_grouped_projection(var body : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    if (body == null) {
        macro_error(prog, at, "_sql: _select after _group_by: missing projection")
        return false
    }
    body = peel_ref2val(body)
    if (!(body is ExprMakeTuple)) {
        macro_error(prog, at, "_sql: _select after _group_by must be a named-tuple projection — `(Key=_._0, N=_._1 |> length, AvgX=_._1 |> select($(u:T) => u.X) |> average)`. Got: {describe(body)}")
        return false
    }
    var mkTup = body as ExprMakeTuple
    var tupT = mkTup._type
    if (tupT == null || tupT.baseType != Type.tTuple
            || length(tupT.argNames) == 0
            || length(tupT.argNames) != length(mkTup.values)) {
        macro_error(prog, at, "_sql: _select after _group_by must be a NAMED tuple — every entry needs `Name=expr`")
        return false
    }
    q.groupedSelectExprs |> clear
    q.groupedSelectTypes |> clear
    q.projRecordNames |> clear
    let nKeys = length(q.groupByCols)
    for (i in range(length(mkTup.values))) {
        var val = peel_ref2val(mkTup.values[i])

        // Inspect ExprField shapes — `_.<f>` (single-key key ref) and
        // `_._0._<n>` (multi-key key component). Both have an outer
        // ExprField; differentiate by recursing on the receiver.
        if (val != null && val is ExprField) {
            var ef = val as ExprField
            var recv = peel_ref2val(ef.value)
            let outerName = string(ef.name)
            // Case A: receiver is ExprVar("_") — direct `_.<f>` (so `_._0` for single-key).
            if (recv != null && recv is ExprVar
                    && (recv as ExprVar).name == "_"
                    && nKeys == 1
                    && outerName == "_0") {
                let col = group_key_column_name(q, 0)
                var fieldType = lookup_struct_field_type(q.rootType, col)
                if (fieldType == null) {
                    macro_error(prog, at, "_sql: cannot resolve type of group key column '{col}'")
                    return false
                }
                q.groupedSelectExprs |> push("\"{col}\"")
                q.groupedSelectTypes |> push(fieldType)
                q.projRecordNames |> push(string(tupT.argNames[i]))
                continue
            }
            // Case B: receiver is itself `_._0` — multi-key form `_._0._<n>`.
            if (nKeys > 1 && recv != null && recv is ExprField) {
                var inner = recv as ExprField
                var innerRecv = peel_ref2val(inner.value)
                if (innerRecv != null && innerRecv is ExprVar
                        && (innerRecv as ExprVar).name == "_"
                        && inner.name == "_0") {
                    if (length(outerName) > 1 && outerName |> starts_with("_")) {
                        let suffix = slice(outerName, 1, length(outerName))
                        let keyIdx = try_to_int(suffix) ?? -1
                        if (keyIdx >= 0 && keyIdx < nKeys) {
                            let col = group_key_column_name(q, keyIdx)
                            var fieldType = lookup_struct_field_type(q.rootType, col)
                            if (fieldType == null) {
                                macro_error(prog, at, "_sql: cannot resolve type of group key column '{col}'")
                                return false
                            }
                            q.groupedSelectExprs |> push("\"{col}\"")
                            q.groupedSelectTypes |> push(fieldType)
                            q.projRecordNames |> push(string(tupT.argNames[i]))
                            continue
                        }
                        macro_error(prog, at, "_sql: multi-key group has {nKeys} keys; `_._0.{outerName}` is out of range")
                        return false
                    }
                }
            }
            // Multi-key reject: `_._0` (whole key tuple) is ambiguous in projection.
            if (nKeys > 1 && recv != null && recv is ExprVar
                    && (recv as ExprVar).name == "_"
                    && outerName == "_0") {
                macro_error(prog, at, "_sql: multi-key group: `_._0` refers to the whole key tuple; use `_._0._N` to access the N-th key column")
                return false
            }
        }

        // Aggregate over the group's elements (`_._1`).
        var sqlFrag : string
        var resType : TypeDeclPtr
        if (try_translate_group_aggregate(val, q, prog, at, sqlFrag, resType)) {
            if (empty(sqlFrag)) {
                return false   // helper emitted macro_error
            }
            q.groupedSelectExprs |> push(sqlFrag)
            q.groupedSelectTypes |> push(resType)
            q.projRecordNames |> push(string(tupT.argNames[i]))
            continue
        }
        macro_error(prog, at, "_sql: _select after _group_by: each value must be a group key reference (`_._0` or `_._0._N` for multi-key) or an aggregate (`_._1 |> length`, `_._1 |> select($(u:T) => u.X) |> sum/average/min/max`); got: {describe(val)}")
        return false
    }
    q.proj = ProjectionShape.GroupedNamedTuple
    return true
}

// ============================================================================
// Predicate analysis (_where) — recursive AST walk emitting SQL.
//
// Bind capture: literals and free-variable references become `?` placeholders;
// each captured ExpressionPtr is appended to q.bindExprs in left-to-right order
// matching the placeholder index sequence.
//
// Capture-leak protocol: every qmatch attempt uses fresh capture variables in
// its own bare block so partial writes from a failed earlier attempt cannot
// leak into a later attempt.
// ============================================================================

[macro_function]
def private collect_order_keys(var body : ExpressionPtr; var q : SqlQuery&; direction : string; prog : ProgramPtr; at : LineInfo) : bool {
    //! Walk the body of an order_by lambda and push column entries onto
    //! q.orderByCols. Accepts a single `_.Field` reference (one column) or a
    //! tuple literal `(_.k1, _.k2)` (multi-column). All columns get the same
    //! direction (ASC or DESC) per chunk-4 scope; mixed-direction is plan D2.
    if (body == null) {
        macro_error(prog, at, "_sql: _order_by: missing key expression")
        return false
    }
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
        if (body == null) {
            macro_error(prog, at, "_sql: _order_by: ExprRef2Value with null subexpr")
            return false
        }
    }
    // Group-key form (after `_group_by`): `_._0` (single key) or `_._0._N`
    // (multi-key Nth column). The chain element type post-group is
    // `tuple<K; array<T>>` so plain `_.Field` no longer typechecks; users
    // sort by the group key explicitly.
    if (q.seenGroupBy && body is ExprField) {
        var ef = body as ExprField
        var recv = ef.value
        if (recv is ExprRef2Value) {
            recv = (recv as ExprRef2Value).subexpr
        }
        let outerName = string(ef.name)
        let nKeys = length(q.groupByCols)
        // Single-key: `_._0` → first group-by column.
        if (nKeys == 1 && recv != null && recv is ExprVar
                && (recv as ExprVar).name == "_"
                && outerName == "_0") {
            q.orderByCols |> push("{q.groupByCols[0]} {direction}")
            return true
        }
        // Multi-key: `_._0._N`.
        if (nKeys > 1 && recv != null && recv is ExprField) {
            var inner = recv as ExprField
            var innerRecv = inner.value
            if (innerRecv is ExprRef2Value) {
                innerRecv = (innerRecv as ExprRef2Value).subexpr
            }
            if (innerRecv != null && innerRecv is ExprVar
                    && (innerRecv as ExprVar).name == "_"
                    && inner.name == "_0"
                    && length(outerName) > 1 && outerName |> starts_with("_")) {
                let suffix = slice(outerName, 1, length(outerName))
                let keyIdx = try_to_int(suffix) ?? -1
                if (keyIdx >= 0 && keyIdx < nKeys) {
                    q.orderByCols |> push("{q.groupByCols[keyIdx]} {direction}")
                    return true
                }
            }
        }
    }
    // Single-column form: _.FieldName
    {
        var fieldName : string
        if (qmatch(body, _.$f(fieldName)).matched) {
            q.orderByCols |> push("\"{fieldName}\" {direction}")
            return true
        }
    }
    // Multi-column form: tuple literal `(_.k1, _.k2)`. Each entry must be
    // a `_.Field` ref (after ExprRef2Value peel). Anonymous tuple (no
    // recordNames) is the natural shape; named tuples also work.
    if (body is ExprMakeTuple) {
        var mkTup = body as ExprMakeTuple
        if (length(mkTup.values) == 0) {
            macro_error(prog, at, "_sql: _order_by: empty tuple key")
            return false
        }
        for (i in range(length(mkTup.values))) {
            var val = mkTup.values[i]
            if (val is ExprRef2Value) {
                val = (val as ExprRef2Value).subexpr
            }
            var fieldName : string
            if (qmatch(val, _.$f(fieldName)).matched) {
                q.orderByCols |> push("\"{fieldName}\" {direction}")
            } else {
                macro_error(prog, at, "_sql: _order_by: tuple-key entries must be `_.Field`; got: {describe(val)}")
                return false
            }
        }
        return true
    }
    macro_error(prog, at, "_sql: _order_by: key must be `_.Field` or a tuple of `_.Field`s; got: {describe(body)}")
    return false
}

[macro_function]
def private collect_group_keys(var body : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    //! Walk the body of a `_group_by` lambda and push column entries onto
    //! ``q.groupByCols``. Accepts a single ``_.Field`` reference or a tuple
    //! literal of ``_.Field`` references for multi-column grouping. SQL
    //! functions in the key (e.g. ``year(_.SignupDate)``) are not yet
    //! supported in chunk 4 — see API_REWORK § 19 / mockup section (c).
    if (body == null) {
        macro_error(prog, at, "_sql: _group_by: missing key expression")
        return false
    }
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
        if (body == null) {
            macro_error(prog, at, "_sql: _group_by: ExprRef2Value with null subexpr")
            return false
        }
    }
    // Single-column form: _.FieldName
    {
        var fieldName : string
        if (qmatch(body, _.$f(fieldName)).matched) {
            q.groupByCols |> push("\"{fieldName}\"")
            return true
        }
    }
    // Multi-column form: tuple literal `(_.k1, _.k2)`. Both anonymous and
    // named tuples accepted (the names don't affect SQL emission).
    if (body is ExprMakeTuple) {
        var mkTup = body as ExprMakeTuple
        if (length(mkTup.values) == 0) {
            macro_error(prog, at, "_sql: _group_by: empty tuple key")
            return false
        }
        for (i in range(length(mkTup.values))) {
            var val = mkTup.values[i]
            if (val is ExprRef2Value) {
                val = (val as ExprRef2Value).subexpr
            }
            var fieldName : string
            if (qmatch(val, _.$f(fieldName)).matched) {
                q.groupByCols |> push("\"{fieldName}\"")
            } else {
                macro_error(prog, at, "_sql: _group_by: tuple-key entries must be `_.Field`; got: {describe(val)}")
                return false
            }
        }
        return true
    }
    macro_error(prog, at, "_sql: _group_by: key must be `_.Field` or a tuple of `_.Field`s; got: {describe(body)}")
    return false
}

[macro_function]
def private analyze_predicate(var body : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    //! `body` is the predicate expression (e.g. `_.Id == 5`) extracted by
    //! `peel_lambda_body` from the post-expansion `where_(prev, $(_:T) => body)`.
    if (body == null) {
        macro_error(prog, at, "_sql: _where: missing predicate")
        return false
    }
    let s = pred_to_sql(body, q, prog, at)
    if (empty(s)) {
        return false   // pred_to_sql already issued a macro_error
    }
    if (empty(q.whereSql)) {
        q.whereSql = s
    } else {
        q.whereSql = "({q.whereSql}) AND ({s})"
    }
    return true
}

[macro_function]
def private pred_to_sql(var node : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : string {
    if (node == null) {
        macro_error(prog, at, "_sql: _where: null sub-expression")
        return ""
    }
    // Peel ExprRef2Value (value-from-ref adapter that the typer inserts
    // around field reads after Mode-2 inner expansion).
    if (node is ExprRef2Value) {
        node = (node as ExprRef2Value).subexpr
        if (node == null) {
            macro_error(prog, at, "_sql: _where: ExprRef2Value with null subexpr")
            return ""
        }
    }

    // Logical NOT  →  NOT (...)
    {
        var inner : ExpressionPtr
        if (qmatch(node, !$e(inner)).matched) {
            return "NOT ({pred_to_sql(inner, q, prog, at)})"
        }
    }
    // Logical AND
    {
        var lhs : ExpressionPtr
        var rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) && $e(rhs)).matched) {
            return "({pred_to_sql(lhs, q, prog, at)}) AND ({pred_to_sql(rhs, q, prog, at)})"
        }
    }
    // Logical OR
    {
        var lhs : ExpressionPtr
        var rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) || $e(rhs)).matched) {
            return "({pred_to_sql(lhs, q, prog, at)}) OR ({pred_to_sql(rhs, q, prog, at)})"
        }
    }
    // Comparison ops — six shapes, each in its own scope.
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) == $e(rhs)).matched) {
            return "{pred_to_sql(lhs, q, prog, at)} = {pred_to_sql(rhs, q, prog, at)}"
        }
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) != $e(rhs)).matched) {
            return "{pred_to_sql(lhs, q, prog, at)} <> {pred_to_sql(rhs, q, prog, at)}"
        }
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) < $e(rhs)).matched) {
            return "{pred_to_sql(lhs, q, prog, at)} < {pred_to_sql(rhs, q, prog, at)}"
        }
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) <= $e(rhs)).matched) {
            return "{pred_to_sql(lhs, q, prog, at)} <= {pred_to_sql(rhs, q, prog, at)}"
        }
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) > $e(rhs)).matched) {
            return "{pred_to_sql(lhs, q, prog, at)} > {pred_to_sql(rhs, q, prog, at)}"
        }
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) >= $e(rhs)).matched) {
            return "{pred_to_sql(lhs, q, prog, at)} >= {pred_to_sql(rhs, q, prog, at)}"
        }
    }

    // Column reference: _.ColName  (ExprField rooted at lambda param `_`)
    {
        var fieldName : string
        if (qmatch(node, _.$f(fieldName)).matched) {
            return "\"{fieldName}\""
        }
    }

    // Group-scope aggregate (IGrouping shape) in expression position —
    // typically inside HAVING (`_._1 |> length > 5l`). The same patterns
    // recognized by `analyze_grouped_projection`. The aggregate's SQL
    // fragment doesn't depend on WHERE/HAVING context, so we accept it in
    // either (SQLite will reject aggregates inside WHERE at runtime with a
    // clear "misuse of aggregate" error). The result type is discarded
    // here — the surrounding comparison handles type matching.
    {
        var aggSql : string
        var aggType : TypeDeclPtr
        if (try_translate_group_aggregate(node, q, prog, at, aggSql, aggType)) {
            if (empty(aggSql)) {
                return ""   // helper emitted macro_error
            }
            return aggSql
        }
    }

    // Function call → SQL function dispatch (table-driven entries below).
    // Handles `_.Name |> starts_with("F")` (post-pipe-expansion: starts_with(_.Name, "F")),
    // `length(_.Name)`, `_.Price |> abs()`, `to_lower(_.Name)`, etc.
    if (node is ExprCall) {
        let sql = fn_call_to_sql(node as ExprCall, q, prog, at)
        if (!empty(sql)) {
            return sql
        }
    }

    // Literal / captured var → bind param. Clone the *peeled* expression so
    // when we re-emit it as a `sqlite_bind` argument the runtime reads the
    // captured value cleanly (without re-wrapping into ExprRef2Value).
    // Inside `_having`, route to havingBindExprs so the placeholder index
    // matches the SQL emission order (WHERE → HAVING → LIMIT).
    if (is_const_or_captured_var(node)) {
        if (q.inHaving) {
            q.havingBindExprs |> push <| clone_expression(node)
        } else {
            q.bindExprs |> push <| clone_expression(node)
        }
        return "?"
    }

    macro_error(prog, at, "_sql: _where: unsupported expression. Got: {describe(node)}")
    return ""
}

// ============================================================================
// SQL function dispatch — daslang call → SQL expression
//
// Table-driven shape: each entry maps `(daslang_name, arity)` to a SQL
// template where ``$0`` / ``$1`` are placeholders for the recursively-
// translated arguments. Adding a new operator is a one-row change here
// plus a test.
//
// Per Appendix A of TUTORIALS.md: chunk 3 ships LIKE-pattern strings,
// case folding, length, and abs. Chunk 4 broadens with date/time and
// aggregates that can appear in projections.
// ============================================================================

[macro_function]
def private user_arg_count(call : ExprCall?) : int {
    //! Daslang builtins (string ops, etc.) inject hidden trailing args:
    //! ``__context__`` (ExprFakeContext), ``__line__`` (ExprFakeLineInfo).
    //! Count only user-supplied args by stripping the trailing builtin ones.
    if (call == null) {
        return 0
    }
    var n = length(call.arguments)
    while (n > 0) {
        let a = call.arguments[n - 1]
        let kind = string(a.__rtti)
        if (kind == "ExprFakeContext" || kind == "ExprFakeLineInfo") {
            n--
        } else {
            break
        }
    }
    return n
}

[macro_function]
def private fn_call_to_sql(var call : ExprCall?; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : string {
    //! Returns the SQL expression text for a recognized function call,
    //! or "" if the call name isn't in the dispatch table (caller falls
    //! through to literal/captured-var bind).
    if (call == null || call.func == null) {
        return ""
    }
    var fname = string(call.func.name)
    if (call.func.fromGeneric != null) {
        fname = string(call.func.fromGeneric.name)
    }
    let argc = user_arg_count(call)

    // ---- String predicates: LIKE patterns ----
    if (fname == "starts_with" && argc == 2) {
        let lhs = pred_to_sql(call.arguments[0], q, prog, at)
        let rhs = pred_to_sql(call.arguments[1], q, prog, at)
        return "{lhs} LIKE {rhs} || '%'"
    }
    if (fname == "ends_with" && argc == 2) {
        let lhs = pred_to_sql(call.arguments[0], q, prog, at)
        let rhs = pred_to_sql(call.arguments[1], q, prog, at)
        return "{lhs} LIKE '%' || {rhs}"
    }
    if (fname == "contains" && argc == 2) {
        // linq.das also has `contains(array, T)` — we only translate the
        // string form here; the array form (post-Mode-2-expansion of
        // `array._contains(x)`) doesn't reach this codepath.
        let lhs = pred_to_sql(call.arguments[0], q, prog, at)
        let rhs = pred_to_sql(call.arguments[1], q, prog, at)
        return "{lhs} LIKE '%' || {rhs} || '%'"
    }

    // ---- Case folding ----
    if (fname == "to_lower" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "LOWER({inner})"
    }
    if (fname == "to_upper" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "UPPER({inner})"
    }

    // ---- String length ----
    if (fname == "length" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "LENGTH({inner})"
    }

    // ---- Numeric scalar ----
    if (fname == "abs" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "ABS({inner})"
    }

    // ---- Option<T> nullability ----
    // `_.Col |> is_some()` / `_.Col |> is_none()` translate to SQL's
    // explicit null-check operators. Always use IS NULL / IS NOT NULL —
    // `Col = NULL` is never true under SQL's three-valued logic, so the
    // typesafe path here is also the semantically correct one.
    if (fname == "is_some" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "{inner} IS NOT NULL"
    }
    if (fname == "is_none" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "{inner} IS NULL"
    }
    // `_.Col |> unwrap_or(default)` collapses NULL to a fallback —
    // `COALESCE("Col", ?)`. The default goes through pred_to_sql so it
    // can be a literal, captured local, or another column reference.
    if (fname == "unwrap_or" && argc == 2) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        let dflt  = pred_to_sql(call.arguments[1], q, prog, at)
        return "COALESCE({inner}, {dflt})"
    }

    return ""
}

[macro_function]
def private is_const_or_captured_var(var node : ExpressionPtr) : bool {
    //! Recognize a literal or a captured-var read so the predicate walker can
    //! emit a `?` placeholder + bind. After Mode-2 inner expansion captured
    //! local-var reads can arrive wrapped in `ExprRef2Value` (the read-from-ref
    //! adapter); peel through it before classifying.
    if (node == null) {
        return false
    }
    if (node is ExprRef2Value) {
        node = (node as ExprRef2Value).subexpr
        if (node == null) {
            return false
        }
    }
    if (node is ExprConstInt || node is ExprConstInt64 || node is ExprConstUInt || node is ExprConstUInt64
        || node is ExprConstFloat || node is ExprConstDouble || node is ExprConstBool || node is ExprConstString) {
        return true
    }
    if (node is ExprVar) {
        let v = node as ExprVar
        // The lambda parameter `_` is not a captured var; it's the row.
        return v.name != "_"
    }
    return false
}

[macro_function]
def private fill_default_columns(var q : SqlQuery&; rootT : TypeDeclPtr; prog : ProgramPtr; at : LineInfo) : void {
    if (rootT == null || rootT.structType == null) {
        macro_error(prog, at, "_sql: select_from(type<T>): T must be a struct (got {describe(rootT)})")
        return
    }
    var st = rootT.structType
    for (field in st.fields) {
        q.selectCols |> push(string(field.name))
    }
}

[macro_function]
def private sql_table_name_from_type(rootT : TypeDeclPtr) : string {
    // At macro-expansion time we cannot directly call the user's
    // _::_sql_table_name helper because it lives in the user's module, not in
    // sqlite_linq's macro context. Instead, derive the table name here from
    // the struct metadata: use [sql_table(name="...")] when present, else
    // fall back to the struct name.
    if (rootT == null || rootT.structType == null) {
        return ""
    }
    var st = rootT.structType
    // Try to read the [sql_table(name="...")] override from the structure
    // annotation list. `[sql_table]` cannot appear twice on the same struct
    // (daslang rejects duplicate annotations), so once we find it and finish
    // scanning its arguments, break out — no other `sql_table` annotation
    // can exist in the list.
    for (annDecl in st.annotations) {
        if (annDecl.annotation.name == "sql_table") {
            for (arg in annDecl.arguments) {
                if (arg.name == "name" && arg.basicType == Type.tString) {
                    return string(arg.sValue)
                }
            }
            break
        }
    }
    return string(st.name)
}

// ============================================================================
// SQL string assembly
// ============================================================================

[macro_function]
def private build_sql_string(q : SqlQuery) : string {
    return build_string() $(var w) {
        w |> write("SELECT ")
        if (q.distinctFlag) {
            w |> write("DISTINCT ")
        }
        if (q.proj == ProjectionShape.Aggregate) {
            w |> write(q.aggregateExpr)
        } elif (q.proj == ProjectionShape.GroupedNamedTuple) {
            // Already-formatted SQL fragments (e.g. `"City"`, `COUNT(*)`,
            // `AVG("Age")`); emit verbatim, no extra quoting.
            for (i in range(length(q.groupedSelectExprs))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write(q.groupedSelectExprs[i])
            }
        } else {
            for (i in range(length(q.selectCols))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write("\"")
                w |> write(q.selectCols[i])
                w |> write("\"")
            }
        }
        w |> write(" FROM \"")
        w |> write(q.tableName)
        w |> write("\"")
        if (!empty(q.whereSql)) {
            w |> write(" WHERE ")
            w |> write(q.whereSql)
        }
        // GROUP BY / HAVING come between WHERE and ORDER BY in SQL clause
        // order. groupByCols entries are pre-quoted; havingSql is the
        // pre-formatted clause body (no leading "HAVING ").
        if (length(q.groupByCols) > 0) {
            w |> write(" GROUP BY ")
            for (i in range(length(q.groupByCols))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write(q.groupByCols[i])
            }
        }
        if (!empty(q.havingSql)) {
            w |> write(" HAVING ")
            w |> write(q.havingSql)
        }
        if (length(q.orderByCols) > 0) {
            w |> write(" ORDER BY ")
            for (i in range(length(q.orderByCols))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write(q.orderByCols[i])
            }
        }
        // LIMIT / OFFSET. Single-row terminals (One/OneOpt) force LIMIT 1
        // regardless of any user-supplied _take() — except aggregates, which
        // intrinsically return one row (LIMIT 1 would be redundant noise on
        // SELECT COUNT/SUM/AVG/MIN/MAX). User _take wins only when the
        // materializer is Array. SQLite requires LIMIT to precede OFFSET,
        // and OFFSET without LIMIT needs LIMIT -1 (= no upper bound).
        let forced_one_row = ((q.matr == Materializer.One || q.matr == Materializer.OneOpt)
                              && q.proj != ProjectionShape.Aggregate)
        if (forced_one_row) {
            w |> write(" LIMIT 1")
        } elif (q.limitBindExpr != null) {
            w |> write(" LIMIT ?")
        } elif (q.offsetBindExpr != null) {
            w |> write(" LIMIT -1")
        }
        if (!forced_one_row && q.offsetBindExpr != null) {
            w |> write(" OFFSET ?")
        }
    }
}

// ============================================================================
// _sql_text — emit the compiled SQL string at the call site.
// ============================================================================

[call_macro(name="_sql_text")]
class private SqlTextMacro : AstCallMacro {
    //! _sql_text(chain) -> string. Returns the SQL that _sql(chain) would emit,
    //! with `?` placeholders for binds. Used for testing the macro's output.
    //!
    //! Default `canVisitArgument=true` → daslang expands inner call_macros
    //! (`_where`, `_select`, user wrappers like `when_its_cold`) bottom-up
    //! before this fires. We then pattern-match the post-expansion shape.
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "_sql_text expects one argument: a chain expression")
        var argT = call.arguments[0]._type
        macro_verify(argT != null, prog, call.at, "_sql_text: chain argument is not inferred yet")
        macro_verify(!argT.isAutoOrAlias, prog, call.at, "_sql_text: chain type is auto/alias after inference")
        var q = SqlQuery()
        if (!analyze_chain(call.arguments[0], q, prog, call.at)) {
            return null
        }
        let sql = build_sql_string(q)
        var res = qmacro($v(sql))
        res.force_at(call.at)
        return res
    }
}

// ============================================================================
// _sql — emit the runtime materialization at the call site.
//
// The framework switches on (q.matr, q.proj):
//   - Materializer.Array  → _::run_select(...)            returns array<T>
//   - Materializer.One    → _::run_select_one(...)        returns T
//   - Materializer.OneOpt → _::run_select_one_opt(...)    returns Option<T>
//
// The row-builder block is determined by q.proj:
//   - FullRow      → _::_sql_read_row(stmt, type<T>)
//   - SingleColumn → sqlite_read(stmt, 0, type<FieldType>)
//   - NamedTuple   → tuple row builder for the projected fields
//   - Aggregate    → sqlite_read(stmt, 0, type<aggregateType>)
// ============================================================================

[macro_function]
def private lookup_struct_field_type(rootT : TypeDeclPtr; fieldName : string) : TypeDeclPtr {
    if (rootT == null || rootT.structType == null) {
        return null
    }
    var st = rootT.structType
    for (field in st.fields) {
        if (field.name == fieldName) {
            return clone_type(field._type)
        }
    }
    return null
}

[macro_function]
def private build_row_builder(q : SqlQuery; prog : ProgramPtr; at : LineInfo) : ExpressionPtr {
    //! Build the per-row materialization expression based on q.proj.
    //! Returns null on error (macro_error already emitted).
    if (q.proj == ProjectionShape.Aggregate) {
        let colIdx = 0
        var aggT = clone_type(q.aggregateType)
        return qmacro(sqlite_read(_sql_stmt, $v(colIdx), type<$t(aggT)>))
    }
    if (q.proj == ProjectionShape.SingleColumn) {
        let colIdx = 0
        var fieldType = lookup_struct_field_type(q.rootType, q.selectCols[0])
        if (fieldType == null) {
            macro_error(prog, at, "_sql: cannot resolve type of projected column '{q.selectCols[0]}'")
            return null
        }
        return qmacro(sqlite_read(_sql_stmt, $v(colIdx), type<$t(fieldType)>))
    }
    if (q.proj == ProjectionShape.NamedTuple) {
        // Build  (Name = sqlite_read(stmt, 0, type<...>), Price = sqlite_read(stmt, 1, type<...>))
        // recordNames live on the tuple TypeDecl's argNames — set those via
        // recordType so the typer reads them back into the ExprMakeTuple.
        let n = length(q.selectCols)
        var tupT = new TypeDecl(at = at, baseType = Type.tTuple)
        tupT.argNames.resize(n)
        var mkTup = new ExprMakeTuple(at = at)
        for (i in range(n)) {
            let colIdx = i
            var fieldType = lookup_struct_field_type(q.rootType, q.selectCols[i])
            if (fieldType == null) {
                macro_error(prog, at, "_sql: cannot resolve type of projected column '{q.selectCols[i]}'")
                return null
            }
            var readExpr = qmacro(sqlite_read(_sql_stmt, $v(colIdx), type<$t(fieldType)>))
            emplace(mkTup.values, readExpr)
            tupT.argTypes |> emplace_new(clone_type(fieldType))
            tupT.argNames[i] := q.projRecordNames[i]
        }
        mkTup.recordType = tupT
        return mkTup
    }
    if (q.proj == ProjectionShape.GroupedNamedTuple) {
        // Same shape as NamedTuple, but column types come from
        // `groupedSelectTypes` (filled at projection-analysis time —
        // mixes group-key column types with aggregate result types).
        let n = length(q.groupedSelectExprs)
        var tupT = new TypeDecl(at = at, baseType = Type.tTuple)
        tupT.argNames.resize(n)
        var mkTup = new ExprMakeTuple(at = at)
        for (i in range(n)) {
            let colIdx = i
            var fieldType = clone_type(q.groupedSelectTypes[i])
            var readExpr = qmacro(sqlite_read(_sql_stmt, $v(colIdx), type<$t(fieldType)>))
            emplace(mkTup.values, readExpr)
            tupT.argTypes |> emplace_new(clone_type(fieldType))
            tupT.argNames[i] := q.projRecordNames[i]
        }
        mkTup.recordType = tupT
        return mkTup
    }
    // FullRow (default)
    var rootType = clone_type(q.rootType)
    return qmacro(_::_sql_read_row(_sql_stmt, type<$t(rootType)>))
}

[macro_function]
def private emit_sql_macro_body(prog : ProgramPtr; var call : ExprCallMacro?; isTry : bool) : ExpressionPtr {
    //! Shared body for `_sql` and `_try_sql`. Difference: `isTry=true` emits
    //! the `try_run_*` variants returning `Result<..., string>`; `isTry=false`
    //! emits the strict `run_*` variants that panic on prepare/step failure.
    var argT = call.arguments[0]._type
    macro_verify(argT != null, prog, call.at, "_sql/_try_sql: chain argument is not inferred yet")
    macro_verify(!argT.isAutoOrAlias, prog, call.at, "_sql/_try_sql: chain type is auto/alias after inference")
    var q = SqlQuery()
    if (!analyze_chain(call.arguments[0], q, prog, call.at)) {
        return null
    }
    // `take(n)` / `skip(n)` upstream of an aggregate (sum / average /
    // min / max / count) is degenerate in SQL: the aggregate already
    // collapses to one row, so `LIMIT n` / `OFFSET n` become no-ops
    // (e.g. `SELECT SUM(p) FROM Cars LIMIT 2` returns the sum of
    // ALL rows). The user almost certainly wants "<agg> over the
    // first n rows", which requires a subquery
    // (`SELECT SUM(c) FROM (SELECT p AS c FROM Cars LIMIT 2)`) —
    // landing with joins / subqueries in chunk 5+. Reject loud now
    // so users see a clear pointer instead of silently-wrong sums.
    if (q.proj == ProjectionShape.Aggregate
            && (q.limitBindExpr != null || q.offsetBindExpr != null)) {
        macro_error(prog, call.at, "_sql: take(n)/skip(n) before an aggregate is not translatable — `LIMIT`/`OFFSET` after an aggregate has no effect (aggregate collapses to one row); use `_where(...)` for pre-aggregate filtering, or wait for subquery support in chunk 5+")
        return null
    }
    // `distinct()` upstream of an aggregate (sum / average / min /
    // max / count) is degenerate in SQL: `SELECT DISTINCT SUM(...)`
    // / `SELECT DISTINCT COUNT(*)` etc. dedupe a single-row result,
    // which is always a no-op. The meaningful form is
    // `COUNT(DISTINCT col)` (DISTINCT *inside* the aggregate), which
    // would need a separate analyzer path — not yet wired. Reject
    // loud rather than emit silently-meaningless SQL.
    if (q.proj == ProjectionShape.Aggregate && q.distinctFlag) {
        macro_error(prog, call.at, "_sql: distinct() before an aggregate is not translatable — `SELECT DISTINCT` over a single-row aggregate result is always a no-op; the meaningful form is `COUNT(DISTINCT col)` etc. (aggregate-distinct), which is not yet implemented")
        return null
    }
    // Append HAVING / LIMIT / OFFSET binds AFTER WHERE binds so the
    // placeholder index matches the SQL order
    // (WHERE … HAVING … LIMIT ? OFFSET ?). Single-row terminals
    // (`_first` / `_first_opt`) emit a literal `LIMIT 1` (no bind),
    // so skip the limit push in that case. Aggregate terminals
    // (`sum` / `average` / `min` / `max` / `count`) with upstream
    // `take(n)` / `skip(n)` are rejected at compile time above
    // (degenerate SQL — `LIMIT` after an aggregate is a no-op), so
    // there's no aggregate-specific bind push reachable here. The
    // `q.proj != Aggregate` exclusion below mirrors
    // `build_sql_string`'s `forced_one_row` check for hygiene.
    q.bindExprs |> reserve(length(q.bindExprs) + length(q.havingBindExprs) + 2)
    for (be in q.havingBindExprs) {
        q.bindExprs |> push(be)
    }
    let forced_one_row = ((q.matr == Materializer.One || q.matr == Materializer.OneOpt)
        && q.proj != ProjectionShape.Aggregate)
    if (!forced_one_row && q.limitBindExpr != null) {
        q.bindExprs |> push(q.limitBindExpr)
    }
    if (!forced_one_row && q.offsetBindExpr != null) {
        q.bindExprs |> push(q.offsetBindExpr)
    }
    let sql = build_sql_string(q)

    // Build bind statements: sqlite_bind(stmt, i+1, $e(boundExpr))
    var bind_stmts : array<ExpressionPtr>
    bind_stmts |> reserve(length(q.bindExprs))
    for (i in range(length(q.bindExprs))) {
        let bindIdx = i + 1
        var be = q.bindExprs[i]
        bind_stmts |> push <| qmacro_expr(${
            sqlite_bind(_sql_stmt, $v(bindIdx), $e(be));
        })
    }

    var rowBuilder = build_row_builder(q, prog, call.at)
    if (rowBuilder == null) {
        return null
    }

    var dbExpr = q.dbExpr
    var res : ExpressionPtr
    if (isTry) {
        if (q.matr == Materializer.One) {
            res = qmacro(_::try_run_select_one($e(dbExpr), $v(sql),
                $(var _sql_stmt : sqlite3_stmt?) {
                    $b(bind_stmts)
                },
                $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
        } elif (q.matr == Materializer.OneOpt) {
            res = qmacro(_::try_run_select_one_opt($e(dbExpr), $v(sql),
                $(var _sql_stmt : sqlite3_stmt?) {
                    $b(bind_stmts)
                },
                $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
        } else {
            res = qmacro(_::try_run_select($e(dbExpr), $v(sql),
                $(var _sql_stmt : sqlite3_stmt?) {
                    $b(bind_stmts)
                },
                $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
        }
    } else {
        if (q.matr == Materializer.One) {
            res = qmacro(_::run_select_one($e(dbExpr), $v(sql),
                $(var _sql_stmt : sqlite3_stmt?) {
                    $b(bind_stmts)
                },
                $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
        } elif (q.matr == Materializer.OneOpt) {
            res = qmacro(_::run_select_one_opt($e(dbExpr), $v(sql),
                $(var _sql_stmt : sqlite3_stmt?) {
                    $b(bind_stmts)
                },
                $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
        } else {
            res = qmacro(_::run_select($e(dbExpr), $v(sql),
                $(var _sql_stmt : sqlite3_stmt?) {
                    $b(bind_stmts)
                },
                $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
        }
    }
    res.force_at(call.at)
    return res
}

[call_macro(name="_sql")]
class private SqlMacro : AstCallMacro {
    //! _sql(chain) -> array<T> | T | Option<T>. Compile-time LINQ-to-SQL
    //! translator. Pattern-matches the chain, emits SQL + a runtime
    //! materialization via run_select / run_select_one / run_select_one_opt.
    //!
    //! **Default `canVisitArgument=true`.** Daslang's macro pass expands
    //! inner call_macros bottom-up *before* this `visit()` fires. By the
    //! time we run, `_where(it, expr)` has become `where_(it, $(_:T) => expr)`,
    //! `_select(it, expr)` has become `select(it, $(_:T) => expr)`, and any
    //! user-defined wrapper macro (e.g. `when_its_cold(it)` →
    //! `_where(it, _.temp < 0)`) has cascaded into the same shape.
    //!
    //! **Entry guard:** the chain argument's `_type` must be resolved before
    //! `_sql` runs — if it's null, inner call_macros didn't expand (typically a
    //! missing `require` for an operator/wrapper); if it's still auto/alias,
    //! type inference didn't finish. Both fail loudly via `macro_verify`.
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "_sql expects one argument: a chain expression")
        return emit_sql_macro_body(prog, call, false)
    }
}

[call_macro(name="_try_sql")]
class private TrySqlMacro : AstCallMacro {
    //! _try_sql(chain) -> Result<T, string> | Result<Option<T>, string> |
    //! Result<array<T>, string>. Non-panicking sibling of _sql. Same
    //! analyzer; emits `try_run_*` runtime helpers that wrap the result in
    //! `Result<..., string>` instead of panicking on prepare/step failure.
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "_try_sql expects one argument: a chain expression")
        return emit_sql_macro_body(prog, call, true)
    }
}
