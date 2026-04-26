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
    FullRow      //!< no `_select`: SELECT cols-of(rootT); reads via _::_sql_read_row
    SingleColumn //!< `_select(_.Field)`: SELECT one col; reads via sqlite_read
    NamedTuple   //!< `_select((Name=_.Name, Price=_.Price))`: SELECT cols; reads via tuple builder
    Aggregate    //!< `_count()` etc.: raw SQL expression in SELECT; scalar reader
}

struct private SqlQuery {
    rootType    : TypeDeclPtr
    tableName   : string
    selectCols  : array<string>           //!< SingleColumn / FullRow / NamedTuple: column names (quoted in SQL)
    projRecordNames : array<string>       //!< NamedTuple: per-column tuple field names (parallel to selectCols)
    aggregateExpr : string                //!< Aggregate: raw SELECT-list text (e.g. "COUNT(*)")
    aggregateType : TypeDeclPtr           //!< Aggregate: result scalar type (e.g. int64 for COUNT)
    whereSql    : string
    bindExprs   : array<ExpressionPtr>
    sqlSuffix   : string                  //!< " LIMIT 1" for One/OneOpt
    matr        : Materializer = Materializer.Array
    proj        : ProjectionShape = ProjectionShape.FullRow
    seenSelect  : bool
    seenToArray : bool
    seenTerminal : bool                   //!< true once a single-row/scalar terminal has been peeled
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
            q.sqlSuffix = " LIMIT 1"
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
            q.sqlSuffix = " LIMIT 1"
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
            q.aggregateType = new TypeDecl(at = at, baseType = Type.tInt64)
            return analyze_chain(cCall.arguments[0], q, prog, at)
        }
    }

    // ---- Stage operators ----

    // Peel select(prev, lambda) — post-expansion of `_select(prev, expr)`.
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
            var body = peel_lambda_body(sCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _select projection lambda has unexpected shape")
                return false
            }
            if (!analyze_projection(body, q, prog, at)) {
                return false
            }
            q.seenSelect = true
            return analyze_chain(sCall.arguments[0], q, prog, at)
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
    if (is_const_or_captured_var(node)) {
        q.bindExprs |> push <| clone_expression(node)
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
        if (q.proj == ProjectionShape.Aggregate) {
            w |> write(q.aggregateExpr)
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
        if (!empty(q.sqlSuffix)) {
            w |> write(q.sqlSuffix)
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
