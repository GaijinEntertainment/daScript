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
// SqlQuery — accumulator for chain analysis (compile-time only).
// ============================================================================

struct private SqlQuery {
    rootType    : TypeDeclPtr
    tableName   : string
    selectCols  : array<string>
    whereSql    : string
    bindExprs   : array<ExpressionPtr>
    projType    : TypeDeclPtr
    seenSelect  : bool
    seenToArray : bool
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
//   _to_array(arr)    →  _to_array(arr)                        [plain fn, stays]
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
def private analyze_chain(var node : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    if (node == null) {
        macro_error(prog, at, "_sql: empty chain")
        return false
    }

    // Peel _to_array(prev) — plain function in this module, no expansion.
    // Must be outermost.
    {
        var inner : ExpressionPtr
        if (qmatch(node, _to_array($e(inner))).matched) {
            if (q.seenToArray) {
                macro_error(prog, at, "_sql: _to_array() can only appear once and must be the outermost operator")
                return false
            }
            q.seenToArray = true
            return analyze_chain(inner, q, prog, at)
        }
    }

    // Peel select(prev, lambda) — post-expansion of `_select(prev, expr)`.
    {
        var sCall = match_linq_call(node, "select")
        if (sCall != null) {
            if (q.seenSelect) {
                macro_error(prog, at, "_sql: _select can only appear once in the chain")
                return false
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
            if (!q.seenSelect) {
                fill_default_columns(q, rootT, prog, at)
                q.projType = clone_type(rootT)
            }
            q.tableName = sql_table_name_from_type(rootT)
            return true
        }
    }

    macro_error(prog, at, "_sql: unsupported chain root or operator. chunk 2 supports `select_from(type<T>)` at the root, with optional `_select` / `_where` / `_to_array`. Got: {describe(node)}")
    return false
}

// ============================================================================
// Projection analysis (_select).
//
// Chunk 2 supports a single projection shape:
//   _select(_.FieldName)   -> single-column projection
//
// Struct-constructor / named-tuple projection is deferred to chunk 3.
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
            // projType becomes the field's type — defer to inference; leave
            // q.projType null. The macro emission path for chunk 2 currently
            // reuses select_from for bare cases; once it emits invoke blocks
            // we'll set projType from the struct's field metadata.
            return true
        }
    }

    macro_error(prog, at, "_sql: _select: chunk 2 supports `_.Field` (single column) only; got: {describe(body)}")
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

    // Literal / captured var → bind param. Clone the *peeled* expression so
    // when we re-emit it as a `sqlite_bind` argument the runtime reads the
    // captured value cleanly (without re-wrapping into ExprRef2Value).
    if (is_const_or_captured_var(node)) {
        q.bindExprs |> push <| clone_expression(node)
        return "?"
    }

    macro_error(prog, at, "_sql: _where: unsupported expression. Chunk 2 supports field refs (_.Col), constants, captured vars, comparisons (=, <>, <, <=, >, >=), &&/||, !. Got: {describe(node)}")
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
        for (i in range(length(q.selectCols))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write("\"")
            w |> write(q.selectCols[i])
            w |> write("\"")
        }
        w |> write(" FROM \"")
        w |> write(q.tableName)
        w |> write("\"")
        if (!empty(q.whereSql)) {
            w |> write(" WHERE ")
            w |> write(q.whereSql)
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
        var q : SqlQuery
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
// Chunk 2 minimum: bare `select_from(type<T>)` -> array<T>. Builds an invoke
// block that prepares the SQL, steps, materializes via _::_sql_read_row.
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

[call_macro(name="_sql")]
class private SqlMacro : AstCallMacro {
    //! _sql(chain) -> array<T>. Compile-time LINQ-to-SQL translator. Pattern-matches
    //! the chain, emits SQL + a runtime materialization via run_select.
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
        var argT = call.arguments[0]._type
        macro_verify(argT != null, prog, call.at, "_sql: chain argument is not inferred yet")
        macro_verify(!argT.isAutoOrAlias, prog, call.at, "_sql: chain type is auto/alias after inference")
        var q : SqlQuery
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

        // Build row materializer based on projection shape.
        var rootType = clone_type(q.rootType)
        var rowBuilder : ExpressionPtr
        if (q.seenSelect && length(q.selectCols) == 1) {
            // single-column projection: emit sqlite_read(stmt, 0, type<FieldType>)
            let colIdx = 0
            var fieldType = lookup_struct_field_type(q.rootType, q.selectCols[0])
            if (fieldType == null) {
                macro_error(prog, call.at, "_sql: cannot resolve type of projected column '{q.selectCols[0]}'")
                return null
            }
            rowBuilder = qmacro(sqlite_read(_sql_stmt, $v(colIdx), type<$t(fieldType)>))
        } else {
            // full row: emit _::_sql_read_row(stmt, type<T>)
            rowBuilder = qmacro(_::_sql_read_row(_sql_stmt, type<$t(rootType)>))
        }

        var dbExpr = q.dbExpr
        var res = qmacro(_::run_select($e(dbExpr), $v(sql),
            $(var _sql_stmt : sqlite3_stmt?) {
                $b(bind_stmts)
            },
            $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
        res.force_at(call.at)
        return res
    }
}
