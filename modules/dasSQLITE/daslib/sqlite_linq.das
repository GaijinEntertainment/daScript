options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

// Intentionally opt in for user-facing docs: keep comments concise and actionable.
options _comment_hygiene = true

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

[unused_argument(arr)]
def _to_array(var arr : array<auto(TT)>) : array<TT -const -#> {
    //! Passthrough terminal. In compat mode `select_from` has already materialized
    //! the array; this just yields it. Inside `_sql(...)` the macro recognizes
    //! the call and treats it as a no-op terminal.
    return <- arr
}

def _first_opt(arr : array<auto(TT)>) : Option<TT -const -#> {
    //! Compat-mode fallback: returns ``some(arr[0])`` if non-empty, ``none`` otherwise.
    //! Inside `_sql(...)` the macro intercepts this call before evaluation
    //! and emits ` LIMIT 1` with the OneOpt materializer.
    if (!empty(arr)) return some(arr[0])
    return none(type<TT -const -#>)
}

def _first(arr : array<auto(TT)>) : TT -const -& {
    //! Compat-mode fallback to ``linq.first`` (panics on empty). Inside ``_sql(...)``
    //! the macro intercepts and emits ``LIMIT 1`` with the One materializer.
    return first(arr)
}

// ===== to_sql_literal — runtime stringifier for `_create_view` body inlining =====
// `_::` resolution lets users extend with a one-line overload in their own module
// (`def to_sql_literal(s : Status) : string => "{int(s)}"`).

def to_sql_literal(v : int)    : string => "{v}"
def to_sql_literal(v : int8)   : string => "{v}"
def to_sql_literal(v : int16)  : string => "{v}"
def to_sql_literal(v : int64)  : string => "{v}"
def to_sql_literal(v : uint)   : string => "{v}"
def to_sql_literal(v : uint8)  : string => "{v}"
def to_sql_literal(v : uint16) : string => "{v}"
def to_sql_literal(v : uint64) : string => "{v}"
def to_sql_literal(v : float)  : string => fmt(":.9g",  v)
def to_sql_literal(v : double) : string => fmt(":.17g", v)
def to_sql_literal(v : bool)   : string => v ? "1" : "0"
def to_sql_literal(v : string) : string => sql_quote_lit(v)

def to_sql_literal(value : auto(TT)) : string {
    //! Catch-all for `_create_view` body inlining. Enums emit their underlying
    //! integer; user types must supply their own `def to_sql_literal(v : YourType) : string`
    //! overload (the concrete overload beats this generic at the user's call site via _::).
    static_if (typeinfo is_enum(value)) {
        return "{int(value)}"
    } else {
        concept_assert(false, "to_sql_literal: unsupported type for `_create_view` body inlining. Define `def to_sql_literal(v : YourType) : string` in YourType's module.")
        return ""
    }
}

// ===== SqlQuery — accumulator for chain analysis =====

let private INT_MAX_PHASE : int = int(0x7fffffff)

// nolint:STYLE014
// Phase numbers match SQL eval order: lower = earlier in eval = innermost in SQL nesting.
// Multiple ops at the SAME phase coexist in one Q (e.g. two `_where` AND-fold).
// On peel: P > q.minPhaseSeen → push inner subQ (this op runs AFTER something already
// in q per chain order, but its eval phase says BEFORE — preserve linq positional
// semantics by lowering to a nested SELECT).
let private PHASE_SOURCE     : int = 0
let private PHASE_JOIN       : int = 1
let private PHASE_WHERE      : int = 2
let private PHASE_GROUP_BY   : int = 3
let private PHASE_DISTINCT   : int = 3
let private PHASE_SET_OP     : int = 3
let private PHASE_HAVING     : int = 4
let private PHASE_SELECT     : int = 5
let private PHASE_ORDER_BY   : int = 5   // same phase as SELECT — they commute in SQL (ORDER BY can reference projected aliases or source columns); prevents spurious divert on canonical `_group_by |> _order_by |> _select`
let private PHASE_SKIP       : int = 7
let private PHASE_TAKE       : int = 8
let private PHASE_TERMINAL   : int = 9   // _first / _first_opt / _to_array / aggregate

enum private Materializer {
    Array
    One
    OneOpt
}

enum private ProjectionShape {
    FullRow
    SingleColumn
    NamedTuple
    Aggregate
    GroupedNamedTuple
}

enum private JoinKind {
    Inner
    Left
    Right
    FullOuter
    Cross
}

enum private SetOpKind {
    None
    Union
    Intersect
    Except
}

struct private JoinSpec {
    kind        : JoinKind
    rootType    : TypeDeclPtr
    tableName   : string
    alias       : string
    leftCol     : string
    leftAlias   : string
    rightCol    : string
    rightWhereSql     : string
    rightOnBindExprs  : array<ExpressionPtr>
    // RIGHT/FULL OUTER w/ RHS WHERE: tableName is `(SELECT * FROM ... WHERE ...)` raw SQL,
    // not a quoted identifier — filters BEFORE the join to preserve null-padded rows.
    tableSqlIsRaw     : bool
}

struct private SqlQuery {
    rootType    : TypeDeclPtr
    tableName   : string
    rootAlias   : string
    selectCols  : array<string>
    selectColAliases : array<string>
    selectColTypes : array<TypeDeclPtr>
    selectColSqlFragments : array<string>
    projRecordNames : array<string>
    aggregateExpr : string
    aggregateType : TypeDeclPtr
    whereSql    : string
    bindExprs   : array<ExpressionPtr>
    limitBindExpr : ExpressionPtr
    offsetBindExpr : ExpressionPtr
    distinctFlag : bool
    orderByCols : array<string>
    orderByInlineExprs : array<ExpressionPtr>
    groupByCols : array<string>
    havingSql : string
    havingBindExprs : array<ExpressionPtr>
    groupedSelectExprs : array<string>
    groupedSelectTypes : array<TypeDeclPtr>
    matr        : Materializer = Materializer.Array
    proj        : ProjectionShape = ProjectionShape.FullRow
    seenSelect  : bool
    seenToArray : bool
    seenTerminal : bool
    seenDistinct : bool
    seenTake    : bool
    seenSkip    : bool
    seenOrderBy : bool
    seenGroupBy : bool
    seenHaving  : bool
    inHaving    : bool
    seenJoin    : bool
    joins       : array<JoinSpec>
    setOpKind     : SetOpKind = SetOpKind.None
    setOpRhsSql   : string
    setOpRhsBinds : array<ExpressionPtr>   // RHS placeholders, parsed AFTER LHS in `<lhs> UNION <rhs>`
    seenSetOp     : bool
    argNames    : array<string>
    argSourceIdx : array<int>
    dbExpr      : ExpressionPtr
    upsertMode  : bool
    inView      : bool
    hadError    : bool
    lastError   : string
    // Multi-Q lowering: phase-order conflict (e.g. `take(n) |> _where(p)`) snapshots
    // the inner subtree's SQL+binds here; outer FROM renders as `(<innerSql>) AS t0`.
    innerSql       : string                 // null/empty = base-table FROM; else nested SELECT body
    innerBindExprs : array<ExpressionPtr>   // FROM-subquery placeholders, parsed first
    minPhaseSeen   : int = int(0x7fffffff)  // INT_MAX_PHASE sentinel; lowest phase added to this Q
    fromRowType    : TypeDeclPtr            // `_` row type for outer-Q lambdas (= inner's projected output)
}

// ===== SqlFrag fragment stream =====

variant private SqlFrag {
    text       : string
    bind       : ExpressionPtr
    inline_id  : ExpressionPtr
    inline_lit : ExpressionPtr
}

[macro_function]
def private push_text(var out : array<SqlFrag>; s : string) {
    if (empty(s)) return
    let n = length(out)
    if (n > 0 && out[n - 1] is text) {
        out[n - 1] = SqlFrag(text = string(out[n - 1] as text) + s)
        return
    }
    out |> push(SqlFrag(text = s))
}

[macro_function]
def private push_bind(var out : array<SqlFrag>; e : ExpressionPtr) {
    out |> push(SqlFrag(bind = clone_expression(e)))
}

[macro_function]
def private push_inline_id(var out : array<SqlFrag>; e : ExpressionPtr) {
    out |> push(SqlFrag(inline_id = clone_expression(e)))
}

[macro_function]
def private push_inline_lit(var out : array<SqlFrag>; e : ExpressionPtr) {
    out |> push(SqlFrag(inline_lit = clone_expression(e)))
}

[macro_function]
def private push_frags(var dst : array<SqlFrag>; var src : array<SqlFrag>) {
    for (f in src) {
        dst |> push(f) // nolint:PERF006 macro-function, runs once per compile
    }
}

[macro_function]
def private has_bind_frag(var frags : array<SqlFrag>) : bool {
    for (f in frags) {
        if (f is bind) return true
    }
    return false
}

[macro_function]
def private extract_const_string(e : ExpressionPtr) : tuple<bool; string> {
    if (e == null || !(e is ExprConstString)) return (false, "")
    let cs = e as ExprConstString
    return (true, string(cs.value))
}

[macro_function]
def private fold_to_string(var frags : array<SqlFrag>; var hadError : bool&; prog : ProgramPtr; at : LineInfo) : string {
    return build_string() $(var w) {
        for (f in frags) {
            if (f is text) {
                w |> write(f as text)
            } elif (f is bind) {
                w |> write("?")
            } elif (f is inline_id) {
                let r = extract_const_string(f as inline_id)
                if (!r._0) {
                    hadError = true
                    macro_error(prog, at, "_sql_text / _create_view requires compile-time identifiers; use _sql / query for runtime-built SQL")
                    return
                }
                w |> write("\"")
                w |> write(r._1 |> replace("\"", "\"\""))
                w |> write("\"")
            } elif (f is inline_lit) {
                let r = extract_const_string(f as inline_lit)
                if (!r._0) {
                    hadError = true
                    macro_error(prog, at, "_sql_text / _create_view requires compile-time literals; use _sql / query for runtime-built SQL")
                    return
                }
                w |> write("'")
                w |> write(r._1 |> replace("'", "''"))
                w |> write("'")
            }
        }
    }
}

[macro_function]
def private fold_to_builder(var frags : array<SqlFrag>; at : LineInfo;
                            var sqlExpr : ExpressionPtr&;
                            var bindExprs : array<ExpressionPtr>) {
    var sb = new ExprStringBuilder(at = at)
    var pending = ""
    // macro-function: short-burst string accumulation, flushed into ExprConstString nodes
    for (f in frags) {
        if (f is text) {
            pending += string(f as text) // nolint:PERF001
        } elif (f is bind) {
            pending += "?" // nolint:PERF001
            var be : ExpressionPtr = f as bind
            bindExprs |> push(be)
        } elif (f is inline_id) {
            let r = extract_const_string(f as inline_id)
            if (r._0) {
                pending += "\"" // nolint:PERF001
                pending += r._1 |> replace("\"", "\"\"") // nolint:PERF001
                pending += "\"" // nolint:PERF001
            } else {
                if (!empty(pending)) {
                    sb.elements |> push <| new ExprConstString(at = at, value := pending)
                    pending = ""
                }
                sb.elements |> push <| qmacro(_::sql_quote_id($e(f as inline_id)))
            }
        } elif (f is inline_lit) {
            let r = extract_const_string(f as inline_lit)
            if (r._0) {
                pending += "'" // nolint:PERF001
                pending += r._1 |> replace("'", "''") // nolint:PERF001
                pending += "'" // nolint:PERF001
            } else {
                if (!empty(pending)) {
                    sb.elements |> push <| new ExprConstString(at = at, value := pending)
                    pending = ""
                }
                sb.elements |> push <| qmacro(_::sql_quote_lit($e(f as inline_lit)))
            }
        }
    }
    if (!empty(pending)) {
        sb.elements |> push <| new ExprConstString(at = at, value := pending)
    }
    sqlExpr = sb
}

[macro_function]
def private sql_to_frags(sql : string; var orderedBinds : array<ExpressionPtr>; var out : array<SqlFrag>) {
    var noInlineIds : array<ExpressionPtr>
    sql_to_frags_ex(sql, orderedBinds, noInlineIds, out)
}

// Splits `?` (binds) / `\x01` (runtime ids) into frags; pulls in occurrence order, preserves markers when list exhausted.
[macro_function]
def private sql_to_frags_ex(sql : string;
                            var orderedBinds : array<ExpressionPtr>;
                            var orderedInlineIds : array<ExpressionPtr>;
                            var out : array<SqlFrag>) {
    var rest = sql
    var bindIdx = 0
    var idIdx = 0
    while (true) {
        let p = next_placeholder(rest)
        if (p._0 < 0) break
        if (p._1 == uint8('?')) {
            if (bindIdx >= length(orderedBinds)) {
                out |> push_text(rest |> slice(0, p._0 + 1))
                rest = rest |> slice(p._0 + 1)
                continue
            }
            if (p._0 > 0) {
                out |> push_text(rest |> slice(0, p._0))
            }
            out |> push_bind(orderedBinds[bindIdx])
            bindIdx++
        } else {
            if (idIdx >= length(orderedInlineIds)) {
                out |> push_text(rest |> slice(0, p._0 + 1))
                rest = rest |> slice(p._0 + 1)
                continue
            }
            if (p._0 > 0) {
                out |> push_text(rest |> slice(0, p._0))
            }
            out |> push_inline_id(orderedInlineIds[idIdx])
            idIdx++
        }
        rest = rest |> slice(p._0 + 1)
    }
    if (!empty(rest)) {
        out |> push_text(rest)
    }
}

// ===== Multi-source column qualification helpers =====

[macro_function]
def private source_alias_for_idx(q : SqlQuery; idx : int) : string {
    if (empty(q.rootAlias)) return ""
    if (idx == 0) return q.rootAlias
    return q.joins[idx - 1].alias
}

[macro_function]
def private source_rootType_for_idx(var q : SqlQuery&; idx : int) : TypeDeclPtr {
    if (idx == 0) {
        // Multi-join LHS subquery: rootType is null, fromRowType holds the inner's projected named tuple.
        if (q.rootType == null && q.fromRowType != null) return q.fromRowType
        return q.rootType
    }
    return q.joins[idx - 1].rootType
}

[macro_function]
def private source_rootType_for_alias(var q : SqlQuery&; alias : string) : TypeDeclPtr {
    if (alias == q.rootAlias) {
        if (q.rootType == null && q.fromRowType != null) return q.fromRowType
        return q.rootType
    }
    for (j in q.joins) {
        if (j.alias == alias) return j.rootType
    }
    return q.rootType
}

[macro_function]
def private column_sql_for_alias(alias : string; rootT : TypeDeclPtr; field : string) : string {
    // [sql_fts5] @sql_fts_rank → emit FTS5's hidden `rank` unquoted (quoting names a real column).
    if (is_fts_rank_field(rootT, field)) return "rank"
    let sqlCol = field_to_column_name(rootT, field)
    if (empty(alias)) return "\"{sqlCol}\""
    return "\"{alias}\".\"{sqlCol}\""
}

[macro_function]
def private is_fts_rank_field(rootT : TypeDeclPtr; fieldName : string) : bool {
    if (rootT == null || rootT.structType == null) return false
    var st = rootT.structType
    for (field in st.fields) {
        if (field.name == fieldName) {
            for (annDecl in field.annotation) {
                if (annDecl.name == "sql_fts_rank") return true
            }
            return false
        }
    }
    return false
}

[macro_function]
def private render_json_extract(rootT : TypeDeclPtr; alias : string; col : string; path : array<string>) : string {
    let p = build_string() $(var w) {
        w |> write("'$")
        for (s in path) {
            w |> write(".")
            w |> write(s)
        }
        w |> write("'")
    }
    let target = column_sql_for_alias(alias, rootT, col)
    return "json_extract({target}, {p})"
}

[macro_function]
def private lookup_arg_source(q : SqlQuery; argName : string) : int {
    let n = length(q.argNames)
    for (i in range(n)) {
        if (q.argNames[i] == argName) return q.argSourceIdx[i]
    }
    return -1
}

[macro_function]
def private call_resolved_name(var call : ExprCall?) : string {
    if (call == null || call.func == null) return ""
    if (call.func.fromGeneric != null) return string(call.func.fromGeneric.name)
    return string(call.func.name)
}

[macro_function]
def private emit_in_collection_json_bridge(var collArg : ExpressionPtr; var q : SqlQuery&;
                                           xSql : string; negated : bool; nullable : bool) : string {
    // Captured-collection lowering for `_in` / `_not_in`: encode arr as JSON TEXT,
    // single bind, query via SQLite's table-valued json_each. EF Core 8's default.
    var rawArg : ExpressionPtr = clone_expression(collArg)
    var wrapped : ExpressionPtr = qmacro(_::write_json(_::JV($e(rawArg))))
    if (q.inHaving) {
        q.havingBindExprs |> push(wrapped)
    } else {
        q.bindExprs |> push(wrapped)
    }
    if (negated) {
        // Three-valued logic: `x != NULL` is unknown, so naive NOT IN drops rows
        // whose column is NULL. Defensive form preserves them on nullable columns.
        if (nullable) return "({xSql} NOT IN (SELECT value FROM json_each(?)) OR {xSql} IS NULL)"
        return "{xSql} NOT IN (SELECT value FROM json_each(?))"
    }
    return "{xSql} IN (SELECT value FROM json_each(?))"
}

[macro_function]
def private looks_like_subquery_chain(var arg : ExpressionPtr) : bool {
    // Walks `arg.arguments[0]` repeatedly; chain shape bottoms out at `select_from(...)`.
    // Captured arrays / inline literals / user functions returning array<T> never reach it.
    // Handles both expanded ExprCall (e.g. `select(...)`) and unexpanded ExprCallMacro
    // (`_select(...)`, `_where(...)`) on intermediate chain links — Mode-2 doesn't deep-walk
    // into `_in`'s arguments, so we may see either form depending on local expansion order.
    var cur = arg
    let max_depth = 32
    for (_ in range(max_depth)) {
        if (cur == null) return false
        if (cur is ExprCall) {
            var call = cur as ExprCall
            if (call_resolved_name(call) == "select_from") return true
            if (empty(call.arguments)) return false
            cur = call.arguments[0]
        } elif (cur is ExprCallMacro) {
            var mcall = cur as ExprCallMacro
            if (mcall.name == "select_from") return true
            if (empty(mcall.arguments)) return false
            cur = mcall.arguments[0]
        } else {
            return false
        }
    }
    return false
}

[macro_function]
def private is_nullable_column_ref(var node : ExpressionPtr; var q : SqlQuery&) : bool {
    // Matches `_.<Field>` / `<alias>._.<Field>` (mirrors the column-ref qmatch arm in
    // pred_to_sql) and consults is_option_field_type on the matching struct field.
    // Returns true on shapes we don't recognize so the caller emits the safer NOT IN
    // form by default.
    if (node is ExprRef2Value) {
        node = (node as ExprRef2Value).subexpr
    }
    if (node == null) return true
    var receiver : ExpressionPtr
    var fieldName : string
    if (!qmatch(node, $e(receiver).$f(fieldName)).matched
            || receiver == null || !(receiver is ExprVar)) return true
    let argName = string((receiver as ExprVar).name)
    var srcIdx = -1
    if (!empty(q.rootAlias)) {
        srcIdx = lookup_arg_source(q, argName)
        if (srcIdx < 0 && argName == "_") {
            srcIdx = 0
        }
    } elif (argName == "_") {
        srcIdx = 0
    }
    if (srcIdx < 0) return true
    let rootT = source_rootType_for_idx(q, srcIdx)
    if (rootT == null || rootT.structType == null) return true
    var st = rootT.structType
    for (field in st.fields) {
        if (field.name == fieldName) return is_option_field_type(field._type)
    }
    return true
}

// ===== Chain analysis (walk pipe-composed chain backwards) =====

[macro_function]
def private peel_lambda_body(var lam : ExpressionPtr) : ExpressionPtr {
    if (lam == null || !(lam is ExprMakeBlock)) return null
    var mblk = lam as ExprMakeBlock
    var blk = mblk._block as ExprBlock
    if (blk == null || blk.arguments |> length != 1 || blk.list |> length != 1 || !(blk.list[0] is ExprReturn)) return null
    var ret = blk.list[0] as ExprReturn
    return ret.subexpr
}

[macro_function]
def private match_linq_call(var expr : ExpressionPtr; name : string) : ExprCall? {
    if (expr == null || !(expr is ExprCall)) return null
    var call = expr as ExprCall
    if (call.func == null || call.func._module == null) return null
    if ((call.func.name == name && call.func._module.name == "linq")
            || (call.func.fromGeneric != null
                && call.func.fromGeneric.name == name
                && call.func.fromGeneric._module != null
                && call.func.fromGeneric._module.name == "linq")) return call
    return null
}

[macro_function]
def private match_module_call(var expr : ExpressionPtr; name : string; modName : string) : ExprCall? {
    if (expr == null || !(expr is ExprCall)) return null
    var call = expr as ExprCall
    if (call.func == null || call.func._module == null) return null
    if ((call.func.name == name && call.func._module.name == modName)
            || (call.func.fromGeneric != null
                && call.func.fromGeneric.name == name
                && call.func.fromGeneric._module != null
                && call.func.fromGeneric._module.name == modName)) return call
    return null
}

[macro_function]
def private peel_column_aggregate(var node : ExpressionPtr; var q : SqlQuery&;
                                  linqName : string; sqlOp : string; promote_to_double : bool;
                                  prog : ProgramPtr; at : LineInfo) : bool {
    var aCall = match_linq_call(node, linqName)
    if (aCall == null) return false
    if (q.seenTerminal || q.seenToArray) {
        macro_error(prog, at, "_sql: only one terminal allowed per chain")
        return true
    }
    if (aCall.arguments |> length != 1) {
        macro_error(prog, at, "_sql: {linqName}(predicate) is not yet supported in `_sql` chains; use `_select(_.Col) |> {linqName}()` for column aggregates")
        return true
    }
    if (!analyze_chain(aCall.arguments[0], q, prog, at)) return true
    if (q.proj != ProjectionShape.SingleColumn || length(q.selectCols) != 1) {
        macro_error(prog, at, "_sql: {linqName}() needs an upstream `_select(_.Col)` projecting exactly one column (got proj={int(q.proj)}, cols={length(q.selectCols)})")
        return true
    }
    let col = q.selectCols[0]
    let sqlCol = field_to_column_name(q.rootType, col)
    q.aggregateExpr = "{sqlOp}(\"{sqlCol}\")"
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
def private extract_key_selector_field(var keyLambda : ExpressionPtr; expectedArgName : string;
                                       prog : ProgramPtr; at : LineInfo) : string {
    if (keyLambda == null || !(keyLambda is ExprMakeBlock)) {
        macro_error(prog, at, "_sql: _join key selector has unexpected shape (not a lambda)")
        return ""
    }
    var mblk = keyLambda as ExprMakeBlock
    var blk = mblk._block as ExprBlock
    if (blk == null || blk.arguments |> length != 1 || blk.list |> length != 1) {
        macro_error(prog, at, "_sql: _join key selector lambda must take exactly one arg with a single return")
        return ""
    }
    if (!(blk.list[0] is ExprReturn)) {
        macro_error(prog, at, "_sql: _join key selector lambda body must be a single return")
        return ""
    }
    var ret = blk.list[0] as ExprReturn
    var receiver : ExpressionPtr
    var fieldName : string
    // Pin receiver to ExprVar so nested chains (`l.opt.X`) don't match silently with a meaningless argName.
    if (!qmatch(ret.subexpr, $e(receiver).$f(fieldName)).matched
            || receiver == null || !(receiver is ExprVar)) {
        macro_error(prog, at, "_sql: _join key selector body must be `<arg>.Field` (column ref); computed key expressions are not yet translatable to SQL — use raw SQL or restructure the join")
        return ""
    }
    let argName = string((receiver as ExprVar).name)
    if (argName != expectedArgName) {
        macro_error(prog, at, "_sql: _join key selector references arg `{argName}`, expected `{expectedArgName}` — the on-clause must read `(l, r) => l.X == r.Y` referring to the join's own params, not outer captures")
        return ""
    }
    return fieldName
}

// Multi-join LHS wrap: snapshot q's inner-join chain into a subquery (innerSql + binds),
// then reset q so the outer join installs its own state. Outer's `into` resolves against q.fromRowType.
[macro_function]
def private snapshot_q_to_subquery_wrap(var q : SqlQuery&; prog : ProgramPtr; at : LineInfo;
                                        fillPassthrough : bool = false) : bool {
    // nolint:STYLE015
    // NOTE: don't gate on q.seenTerminal / q.seenToArray here — analyze_chain peels
    // outermost-first, so by the time snapshot is called, those flags reflect the OUTER
    // chain's terminal (which is fine) and there's no way to distinguish that from an
    // inner-chain terminal. The "inner LHS can't end in a terminal" check is hoisted
    // to the call sites that can do save-and-compare across analyze_chain (currently
    // process_join_call's srca recursion).
    // Aggregate is acceptable here: outer-peeled `_count`/`_sum` set proj=Aggregate before
    // recursing; we save/clear/restore it below so the inner subquery emits projection columns
    // and the outer SELECT renders the aggregate. SingleColumn rejection still stands because
    // inner_projection_type expects a named tuple.
    if (q.proj == ProjectionShape.SingleColumn) {
        macro_error(prog, at, "_sql: multi-join LHS must project a named row (use `into` to produce a tuple); got scalar projection — reshape so each LHS column is named")
        return false
    }
    // nolint:STYLE015
    // Save outer-terminal state pre-set by outer-peeled ops (`_first` / `_count` / `_take` /
    // `_skip` / `_order_by` / `_distinct` etc.). Without this, `build_sql_string(q, true)` below
    // serializes those clauses into the INNER subquery (e.g. `LIMIT 1` lands BEFORE the outer
    // WHERE filter → wrong row), and the reset further down drops them entirely so the OUTER
    // SELECT loses them too. Save → clear → build inner → reset → restore.
    let savedMatr = q.matr
    var savedLimitBindExpr = q.limitBindExpr
    var savedOffsetBindExpr = q.offsetBindExpr
    let savedDistinctFlag = q.distinctFlag
    var savedOrderByCols <- q.orderByCols
    var savedOrderByInlineExprs <- q.orderByInlineExprs
    let savedSeenTake = q.seenTake
    let savedSeenSkip = q.seenSkip
    let savedSeenOrderBy = q.seenOrderBy
    let savedSeenDistinct = q.seenDistinct
    let savedSeenTerminal = q.seenTerminal
    let savedSeenToArray = q.seenToArray
    // Aggregate state save: inner subquery emits projection cols, outer renders COUNT/SUM.
    let savedAggExpr := q.aggregateExpr
    var savedAggType = q.aggregateType
    // Clear them on q so the inner SQL emits without outer-only clauses.
    q.matr = Materializer.Array
    q.limitBindExpr = null
    q.offsetBindExpr = null
    q.distinctFlag = false
    q.seenTake = false
    q.seenSkip = false
    q.seenOrderBy = false
    q.seenDistinct = false
    q.seenTerminal = false
    q.seenToArray = false
    q.aggregateExpr := ""
    q.aggregateType = null
    if (savedAggExpr != "") {
        // Force NamedTuple so inner emits selectCols (analyze_projection populated them); empty aggregateExpr would emit nothing.
        q.proj = ProjectionShape.NamedTuple
    }
    var savedDb = q.dbExpr
    let savedInView = q.inView
    let savedHadError = q.hadError
    let innerSql = build_sql_string(q, true)
    var innerBinds : array<ExpressionPtr>
    collect_query_binds(q, innerBinds)
    var projType = inner_projection_type(q, at)
    // Reset q to wrap state. Clear all join/where/projection state so outer can re-fill.
    q.rootType = null
    q.tableName := ""
    q.rootAlias := ""
    q.selectCols |> clear
    q.selectColAliases |> clear
    q.selectColTypes |> clear
    q.selectColSqlFragments |> clear
    q.projRecordNames |> clear
    q.aggregateExpr := ""
    q.aggregateType = null
    q.whereSql := ""
    q.bindExprs |> clear
    q.limitBindExpr = null
    q.offsetBindExpr = null
    q.distinctFlag = false
    q.orderByCols |> clear
    q.orderByInlineExprs |> clear
    q.groupByCols |> clear
    q.havingSql := ""
    q.havingBindExprs |> clear
    q.groupedSelectExprs |> clear
    q.groupedSelectTypes |> clear
    q.proj = ProjectionShape.FullRow
    q.seenSelect = false
    q.seenToArray = false
    q.seenTerminal = false
    q.seenDistinct = false
    q.seenTake = false
    q.seenSkip = false
    q.seenOrderBy = false
    q.seenGroupBy = false
    q.seenHaving = false
    q.inHaving = false
    q.seenJoin = false
    q.joins |> clear
    q.setOpKind = SetOpKind.None
    q.setOpRhsSql := ""
    q.setOpRhsBinds |> clear
    q.seenSetOp = false
    q.argNames |> clear
    q.argSourceIdx |> clear
    q.upsertMode = false
    q.minPhaseSeen = INT_MAX_PHASE
    // Install wrap state.
    q.innerSql := innerSql
    q.innerBindExprs <- innerBinds
    q.fromRowType = projType
    q.dbExpr = savedDb
    q.inView = savedInView
    q.hadError = savedHadError
    // Restore outer-terminal state — LIMIT/ORDER BY/DISTINCT belong on the outer SELECT post-wrap.
    q.matr = savedMatr
    q.limitBindExpr = savedLimitBindExpr
    q.offsetBindExpr = savedOffsetBindExpr
    q.distinctFlag = savedDistinctFlag
    q.orderByCols <- savedOrderByCols
    q.orderByInlineExprs <- savedOrderByInlineExprs
    q.seenTake = savedSeenTake
    q.seenSkip = savedSeenSkip
    q.seenOrderBy = savedSeenOrderBy
    q.seenDistinct = savedSeenDistinct
    q.seenTerminal = savedSeenTerminal
    q.seenToArray = savedSeenToArray
    q.aggregateExpr := savedAggExpr
    q.aggregateType = savedAggType
    if (savedAggExpr != "") {
        // Override FullRow installed by reset; aggregate outer ignores fillPassthrough.
        q.proj = ProjectionShape.Aggregate
    } elif (fillPassthrough) {
        // Terminal wrap: outer projects passthrough columns from inner's named-tuple aliases.
        if (!apply_passthrough_projection(q, projType, prog, at)) return false
    }
    return true
}

// Replace q's projection with passthrough column refs against named-tuple projType (= NamedTuple).
[macro_function]
def private apply_passthrough_projection(var q : SqlQuery&; projType : TypeDeclPtr;
                                         prog : ProgramPtr; at : LineInfo) : bool {
    if (projType == null || projType.baseType != Type.tTuple) {
        macro_error(prog, at, "_sql multi-Q: passthrough projection expected named-tuple type (internal)")
        return false
    }
    let n = length(projType.argNames)
    if (n != length(projType.argTypes)) {
        macro_error(prog, at, "_sql multi-Q: passthrough projection alias count {n} != type count {length(projType.argTypes)} (internal)")
        return false
    }
    q.selectCols |> clear
    q.selectColAliases |> clear
    q.selectColSqlFragments |> clear
    q.selectColTypes |> clear
    q.projRecordNames |> clear
    q.selectCols |> reserve(n)
    q.selectColAliases |> reserve(n)
    q.selectColSqlFragments |> reserve(n)
    q.selectColTypes |> reserve(n)
    q.projRecordNames |> reserve(n)
    for (i in range(n)) {
        let alias = string(projType.argNames[i])
        q.selectCols |> push(alias) // nolint:PERF006 reserved above
        q.selectColAliases |> push("") // nolint:PERF006 reserved above
        q.selectColSqlFragments |> push("\"{alias}\"") // nolint:PERF006 reserved above
        q.selectColTypes |> push(clone_type(projType.argTypes[i])) // nolint:PERF006 reserved above
        q.projRecordNames |> push(alias) // nolint:PERF006 reserved above
    }
    q.proj = ProjectionShape.NamedTuple
    return true
}

[macro_function]
def private process_join_call(var jCall : ExprCall?; var q : SqlQuery&;
                              kind : JoinKind; prog : ProgramPtr; at : LineInfo) : bool {
    if (q.seenJoin) {
        macro_error(prog, at, "_sql: internal — process_join_call entered with q.seenJoin already true (chain shape error)")
        return false
    }
    if (length(jCall.arguments) != 5) {
        macro_error(prog, at, "_sql: _join expected 5 args (srca, srcb, keya, keyb, into) post-expansion; got {length(jCall.arguments)}")
        return false
    }
    // Pre-set rootAlias = "t0" so srca's inner `_where` qualifies cols as "t0"."col" (v1 path).
    q.rootAlias = "t0"
    // nolint:STYLE015
    // 1. Recurse into srca FIRST. If srca itself is a `_join` chain (multi-join), the inner
    // process_join_call will set q.seenJoin / fill q.joins[]. Detect that on return below.
    // Capture seenTerminal/seenToArray pre-recursion: if srca itself peels a terminal, we
    // reject — the LHS of a join can't terminate in `_first`/`_count`/`_to_array`. This is
    // the discrimination snapshot_q_to_subquery_wrap can't do (it sees the OUTER terminal
    // flag indistinguishably set in chains like `_join(...) |> _first()`).
    let savedSeenTerminal = q.seenTerminal
    let savedSeenToArray = q.seenToArray
    if (!analyze_chain(jCall.arguments[0], q, prog, at)) return false
    if ((q.seenTerminal && !savedSeenTerminal) || (q.seenToArray && !savedSeenToArray)) {
        macro_error(prog, at, "_sql: _join LHS chain cannot end in a terminal (`_first`, `_to_array`, `_count`, etc.); restructure so terminals are outermost")
        return false
    }
    // nolint:STYLE015 — ordering invariant: must run BEFORE snapshot below, which clears these flags.
    // V1 path: srca was a simple chain. Reject _select/_group_by which would conflict with this
    // join's `into` projection.
    if (!q.seenJoin) {
        if (q.seenSelect) {
            macro_error(prog, at, "_sql: _join must precede `_select` in source order — the join's `into` lambda is the projection. Drop the trailing `_select(...)` or restructure the chain.")
            return false
        }
        if (q.seenGroupBy) {
            macro_error(prog, at, "_sql: _join cannot follow `_group_by` (joining grouped rows is not yet supported); group AFTER the join instead")
            return false
        }
    }
    // nolint:STYLE015 — table of cases where snapshot-and-wrap is required; the (b) branch
    // documents non-obvious NULL-on-preserved-side semantics.
    // 2. Snapshot q as an inner subquery and reset for outer wrap when:
    //    (a) srca contained a _join (multi-join chain); OR
    //    (b) kind ∈ {Right, FullOuter} AND q.whereSql is non-empty — preserved-side LHS
    //        columns are NULL after the join, so outer WHERE on them would drop the row;
    //        wrap as `(SELECT * FROM Users WHERE …) AS t0` to filter BEFORE the join.
    let needsLhsWrapForPreservedSide = ((kind == JoinKind.Right || kind == JoinKind.FullOuter)
                                        && !empty(q.whereSql))
    if (q.seenJoin || needsLhsWrapForPreservedSide) {
        if (!snapshot_q_to_subquery_wrap(q, prog, at)) return false
    }
    q.seenJoin = true
    q.rootAlias = "t0"
    // 3. Build sub-q for srcb. rootAlias="t1" drives column qualification; do NOT set seenJoin (it gates other paths we already reject post-hoc).
    var subQ = SqlQuery(rootAlias = "t1", inView = q.inView)
    if (!analyze_chain(jCall.arguments[1], subQ, prog, at)) return false
    // Reject right-side stages we don't yet support.
    if (subQ.seenGroupBy || !empty(subQ.havingSql)) {
        macro_error(prog, at, "_sql: _join right side cannot use `_group_by` / `_having`")
        return false
    }
    if (subQ.seenDistinct) {
        macro_error(prog, at, "_sql: _join right side cannot use `distinct()` (apply to the joined result instead)")
        return false
    }
    if (subQ.seenSelect) {
        macro_error(prog, at, "_sql: _join right side cannot use `_select(...)` — the join's `into` lambda projects from both sources")
        return false
    }
    if (subQ.seenTerminal || subQ.seenToArray) {
        macro_error(prog, at, "_sql: _join right side cannot have a terminal (`_first`, `_count`, `_to_array`, etc.)")
        return false
    }
    if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
        macro_error(prog, at, "_sql: _join right side cannot use `take(n)` / `skip(n)`")
        return false
    }
    if (subQ.seenOrderBy) {
        macro_error(prog, at, "_sql: _join right side cannot use `_order_by` (apply to the joined result instead)")
        return false
    }
    if (subQ.seenJoin && !empty(subQ.joins)) {
        macro_error(prog, at, "_sql: _join right side cannot itself contain a join (3+ table joins not yet supported)")
        return false
    }
    // 4. Construct JoinSpec.
    let leftCol = extract_key_selector_field(jCall.arguments[2], "__la__", prog, at)
    return false if (empty(leftCol))
    let rightCol = extract_key_selector_field(jCall.arguments[3], "__lb__", prog, at)
    return false if (empty(rightCol))
    // nolint:STYLE015 — explains why RIGHT/FULL OUTER need RHS-WHERE wrapping while LEFT/INNER don't.
    // For RIGHT/FULL OUTER with RHS WHERE: bake into a derived-table subquery so the filter
    // applies BEFORE the join (preserved-side semantics). LEFT/INNER are fine via ON-AND
    // attachment since unmatched RHS becomes NULL regardless.
    let needsRhsWrapForPreservedSide = ((kind == JoinKind.Right || kind == JoinKind.FullOuter)
                                        && !empty(subQ.whereSql))
    var rhsTableName : string
    var rhsRightWhereSql : string
    var rhsTableSqlIsRaw = false
    if (needsRhsWrapForPreservedSide) {
        // nolint:STYLE015 — explains why inner AS "t1" is required (alias-qualified refs in subQ.whereSql).
        // SELECT * propagates underlying column names; outer references t1.<col> by name.
        // Inner AS "t1" is required because subQ.whereSql may carry refs like `"t1"."Total"`
        // built when subQ.rootAlias was "t1". Outer-most "t1" shadows inner via parens-scope.
        rhsTableName := "(SELECT * FROM \"{subQ.tableName}\" AS \"t1\" WHERE {subQ.whereSql})"
        rhsRightWhereSql := ""
        rhsTableSqlIsRaw = true
    } else {
        rhsTableName := subQ.tableName
        rhsRightWhereSql := subQ.whereSql
    }
    var spec <- JoinSpec(
        kind = kind,
        rootType = subQ.rootType,
        tableName = rhsTableName,
        alias = "t1",
        leftCol = leftCol,
        leftAlias = "t0",
        rightCol = rightCol,
        rightWhereSql = rhsRightWhereSql,
        tableSqlIsRaw = rhsTableSqlIsRaw)
    // nolint:STYLE015 — bind-order invariant across wrapped vs unwrapped RHS paths.
    // ON binds live on JoinSpec (not q.bindExprs); runtime concats per-join ON before outer
    // WHERE. Wrapped-RHS binds emit at the t1 subquery body — same SQL position as unwrapped.
    spec.rightOnBindExprs |> reserve(length(subQ.bindExprs))
    for (b in subQ.bindExprs) {
        spec.rightOnBindExprs |> push <| clone_expression(b)
    }
    q.joins |> emplace(spec)
    // 5. Process `into` projection — 2-arg lambda; bind args → sources 0/1, run analyze_projection.
    var intoLambda = jCall.arguments[4]
    if (intoLambda == null || !(intoLambda is ExprMakeBlock)) {
        macro_error(prog, at, "_sql: _join `into` projection has unexpected shape (not a lambda)")
        return false
    }
    var imblk = intoLambda as ExprMakeBlock
    var iblk = imblk._block as ExprBlock
    if (iblk == null || iblk.arguments |> length != 2 || iblk.list |> length != 1
            || !(iblk.list[0] is ExprReturn)) {
        macro_error(prog, at, "_sql: _join `into` lambda must take 2 args with a single return")
        return false
    }
    var iret = iblk.list[0] as ExprReturn
    let arg0Name = string(iblk.arguments[0].name)
    let arg1Name = string(iblk.arguments[1].name)
    q.argNames |> push(arg0Name)
    q.argSourceIdx |> push(0)
    q.argNames |> push(arg1Name)
    q.argSourceIdx |> push(1)
    // nolint:STYLE015 — save/restore explanation: analyze_projection clobbers proj=Aggregate
    // set by outer-peeled `_count`/`_sum`/etc. We still need it to populate selectCols for
    // the inner subquery, so run normally then restore. Invariant: aggregateExpr non-empty ⇒ proj=Aggregate.
    let savedAggExpr := q.aggregateExpr
    var savedAggType = q.aggregateType
    let savedAggProj = q.proj == ProjectionShape.Aggregate
    let projOk = analyze_projection(iret.subexpr, q, prog, at)
    q.argNames |> clear
    q.argSourceIdx |> clear
    if (!projOk) return false
    if (savedAggExpr != "") {
        q.aggregateExpr := savedAggExpr
        q.aggregateType = savedAggType
        if (savedAggProj) {
            q.proj = ProjectionShape.Aggregate
        }
    }
    q.seenSelect = true
    return true
}

[macro_function]
def private process_cross_join_call(var jCall : ExprCall?; var q : SqlQuery&;
                                    prog : ProgramPtr; at : LineInfo) : bool {
    // 3-arg shape (srca, srcb, into); mirrors process_join_call's reject + projection handling.
    if (q.seenJoin) {
        macro_error(prog, at, "_sql: internal — process_cross_join_call entered with q.seenJoin already true (chain shape error)")
        return false
    }
    if (length(jCall.arguments) != 3) {
        macro_error(prog, at, "_sql: _cross_join expected 3 args (srca, srcb, into); got {length(jCall.arguments)}")
        return false
    }
    q.rootAlias = "t0"
    let savedSeenTerminal = q.seenTerminal
    let savedSeenToArray = q.seenToArray
    if (!analyze_chain(jCall.arguments[0], q, prog, at)) return false
    if ((q.seenTerminal && !savedSeenTerminal) || (q.seenToArray && !savedSeenToArray)) {
        macro_error(prog, at, "_sql: _cross_join LHS chain cannot end in a terminal (`_first`, `_to_array`, `_count`, etc.); restructure so terminals are outermost")
        return false
    }
    if (q.seenJoin) {
        if (!snapshot_q_to_subquery_wrap(q, prog, at)) return false
    } else {
        if (q.seenSelect) {
            macro_error(prog, at, "_sql: _cross_join must precede `_select` in source order — the cross's `into` lambda is the projection. Drop the trailing `_select(...)` or restructure the chain.")
            return false
        }
        if (q.seenGroupBy) {
            macro_error(prog, at, "_sql: _cross_join cannot follow `_group_by` (joining grouped rows is not yet supported); group AFTER the join instead")
            return false
        }
    }
    q.seenJoin = true
    q.rootAlias = "t0"
    var subQ = SqlQuery(rootAlias = "t1", inView = q.inView)
    if (!analyze_chain(jCall.arguments[1], subQ, prog, at)) return false
    if (subQ.seenGroupBy || !empty(subQ.havingSql)) {
        macro_error(prog, at, "_sql: _cross_join right side cannot use `_group_by` / `_having`")
        return false
    }
    if (subQ.seenDistinct) {
        macro_error(prog, at, "_sql: _cross_join right side cannot use `distinct()` (apply to the joined result instead)")
        return false
    }
    if (subQ.seenSelect) {
        macro_error(prog, at, "_sql: _cross_join right side cannot use `_select(...)` — the cross's `into` lambda projects from both sources")
        return false
    }
    if (subQ.seenTerminal || subQ.seenToArray) {
        macro_error(prog, at, "_sql: _cross_join right side cannot have a terminal (`_first`, `_count`, `_to_array`, etc.)")
        return false
    }
    if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
        macro_error(prog, at, "_sql: _cross_join right side cannot use `take(n)` / `skip(n)`")
        return false
    }
    if (subQ.seenOrderBy) {
        macro_error(prog, at, "_sql: _cross_join right side cannot use `_order_by` (apply to the joined result instead)")
        return false
    }
    if (subQ.seenJoin && !empty(subQ.joins)) {
        macro_error(prog, at, "_sql: _cross_join right side cannot itself contain a join (3+ table joins not yet supported)")
        return false
    }
    var spec <- JoinSpec(
        kind = JoinKind.Cross,
        rootType = subQ.rootType,
        tableName = subQ.tableName,
        alias = "t1",
        leftCol = "",
        leftAlias = "t0",
        rightCol = "",
        rightWhereSql = "")
    // Hoist any RHS WHERE into the outer q.whereSql — Cross has no ON clause to attach it to.
    if (!empty(subQ.whereSql)) {
        if (empty(q.whereSql)) {
            q.whereSql := subQ.whereSql
        } else {
            q.whereSql := "({q.whereSql}) AND ({subQ.whereSql})"
        }
        q.bindExprs |> reserve(q.bindExprs |> length + length(subQ.bindExprs))
        for (b in subQ.bindExprs) {
            q.bindExprs |> push <| clone_expression(b)
        }
    }
    q.joins |> emplace(spec)
    var intoLambda = jCall.arguments[2]
    if (intoLambda == null || !(intoLambda is ExprMakeBlock)) {
        macro_error(prog, at, "_sql: _cross_join `into` projection has unexpected shape (not a lambda)")
        return false
    }
    var imblk = intoLambda as ExprMakeBlock
    var iblk = imblk._block as ExprBlock
    if (iblk == null || iblk.arguments |> length != 2 || iblk.list |> length != 1
            || !(iblk.list[0] is ExprReturn)) {
        macro_error(prog, at, "_sql: _cross_join `into` lambda must take 2 args with a single return")
        return false
    }
    var iret = iblk.list[0] as ExprReturn
    let arg0Name = string(iblk.arguments[0].name)
    let arg1Name = string(iblk.arguments[1].name)
    q.argNames |> push(arg0Name)
    q.argSourceIdx |> push(0)
    q.argNames |> push(arg1Name)
    q.argSourceIdx |> push(1)
    let savedAggExpr := q.aggregateExpr
    var savedAggType = q.aggregateType
    let savedAggProj = q.proj == ProjectionShape.Aggregate
    let projOk = analyze_projection(iret.subexpr, q, prog, at)
    q.argNames |> clear
    q.argSourceIdx |> clear
    if (!projOk) return false
    if (savedAggExpr != "") {
        q.aggregateExpr := savedAggExpr
        q.aggregateType = savedAggType
        if (savedAggProj) {
            q.proj = ProjectionShape.Aggregate
        }
    }
    q.seenSelect = true
    return true
}

// ===== Multi-Q lowering — phase model + bind collection =====

[macro_function]
def private collect_query_binds(var q : SqlQuery&; var out : array<ExpressionPtr>&) {
    // nolint:STYLE015
    // Placeholder order matches SQL parse order, depth-first inner-first:
    //   1. Inner-Q (FROM subquery) binds — innermost SELECT parsed first
    //   2. Per-join ON binds — within outer FROM clause
    //   3. WHERE binds
    //   4. HAVING binds
    //   5. Set-op RHS binds (compound `<lhs> UNION <rhs>` — RHS parsed before any outer LIMIT)
    //   6. LIMIT bind (compound applies to combined result, parsed last)
    //   7. OFFSET bind
    var total = length(q.innerBindExprs) + length(q.bindExprs) + length(q.havingBindExprs) + length(q.setOpRhsBinds) + 2
    for (j in q.joins) {
        total += length(j.rightOnBindExprs)
    }
    out |> reserve(out |> length + total)
    for (b in q.innerBindExprs) {
        out |> push(b)
    }
    for (j in q.joins) {
        for (b in j.rightOnBindExprs) {
            out |> push(b)
        }
    }
    for (b in q.bindExprs) {
        out |> push(b)
    }
    for (b in q.havingBindExprs) {
        out |> push(b)
    }
    for (b in q.setOpRhsBinds) {
        out |> push(b)
    }
    let forced_one_row = ((q.matr == Materializer.One || q.matr == Materializer.OneOpt)
                          && q.proj != ProjectionShape.Aggregate)
    if (!forced_one_row && q.limitBindExpr != null) {
        out |> push(q.limitBindExpr)
    }
    if (!forced_one_row && q.offsetBindExpr != null) {
        out |> push(q.offsetBindExpr)
    }
}

// True when SQL fragment is exactly the quoted identifier `"<ident>"` — used to skip the
// redundant `AS "<ident>"` on pass-through group-key projections like `(City=_._0)` where
// the fragment is `"City"` and the alias is also "City". Skipping keeps snapshot SQL stable.
[macro_function]
def private sql_fragment_is_quoted_ident(frag : string; ident : string) : bool {
    let want = "\"{ident}\""
    return frag == want
}

// Look up an alias name in a synthesized fromRowType (named-tuple) — used by outer-Q
// pred_to_sql to validate `_.alias` references against the inner's projection.
[macro_function]
def private fromrow_alias_exists(rowT : TypeDeclPtr; name : string) : bool {
    if (rowT == null || rowT.baseType != Type.tTuple) return false
    for (an in rowT.argNames) {
        if (an == name) return true
    }
    return false
}

[macro_function]
def private fromrow_alias_list(rowT : TypeDeclPtr) : string {
    if (rowT == null || rowT.baseType != Type.tTuple || empty(rowT.argNames)) return "(none)"
    return build_string() $(var w) {
        for (i in range(length(rowT.argNames))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write("`{rowT.argNames[i]}`")
        }
    }
}

// nolint:STYLE014
// Inner Q's projection-output type, used by the outer Q's lambdas to resolve `_.Field`.
//   FullRow              → rootType (struct); outer's `_.Col` resolves through the source struct's columns.
//   NamedTuple           → synthesized tuple<col0type;col1type;…> with argNames = projRecordNames; outer resolves `_.alias` by argNames lookup.
//   GroupedNamedTuple    → same shape from groupedSelectTypes.
//   SingleColumn / Aggregate → returned as a 1-arg tuple (alias = projRecordNames[0] when set, else "_0"); outer reference is `_._0` or `_.alias`.
// fromRowType is the witness pred_to_sql consults to know "is this an over-subquery outer Q, and what aliases does it expose?"
[macro_function]
def private inner_projection_type(var inner : SqlQuery&; at : LineInfo) : TypeDeclPtr {
    if (inner.proj == ProjectionShape.FullRow) return inner.rootType
    if (inner.proj == ProjectionShape.NamedTuple) {
        let n = length(inner.selectCols)
        var tupT = new TypeDecl(at = at, baseType = Type.tTuple)
        tupT.argNames.resize(n)
        for (i in range(n)) {
            tupT.argTypes |> emplace_new(clone_type(inner.selectColTypes[i]))
            if (i < length(inner.projRecordNames)) {
                tupT.argNames[i] := inner.projRecordNames[i]
            }
        }
        return tupT
    }
    if (inner.proj == ProjectionShape.GroupedNamedTuple) {
        let n = length(inner.groupedSelectExprs)
        var tupT = new TypeDecl(at = at, baseType = Type.tTuple)
        tupT.argNames.resize(n)
        for (i in range(n)) {
            tupT.argTypes |> emplace_new(clone_type(inner.groupedSelectTypes[i]))
            if (i < length(inner.projRecordNames)) {
                tupT.argNames[i] := inner.projRecordNames[i]
            }
        }
        return tupT
    }
    // SingleColumn / Aggregate: not a valid divert source — caller rejects via shape check.
    return inner.rootType
}

// Phase-conflict diversion: rest of chain → fresh sub-Q, snapshotted as SQL+binds and
// attached to outer's FROM. Same node re-passed; sub-Q starts fresh (no phase conflict).
[macro_function]
def private divert_to_inner(var node : ExpressionPtr; var q : SqlQuery&;
                            prog : ProgramPtr; at : LineInfo) : bool {
    var subQ = SqlQuery(inView = q.inView)
    if (!analyze_chain(node, subQ, prog, at)) return false
    if (subQ.seenTerminal || subQ.seenToArray) {
        macro_error(prog, at, "_sql: terminals (`_first`, `_to_array`, etc.) cannot appear inside a subquery — keep the terminal at the outermost chain position")
        return false
    }
    if (subQ.proj == ProjectionShape.SingleColumn || subQ.proj == ProjectionShape.Aggregate) {
        // SingleColumn / Aggregate inner has no row for the outer to filter — reject both shapes.
        macro_error(prog, at, "_sql: cannot lower a chain whose inner subquery projects a single value (`_select(_.X)` or an aggregate) into an outer query — there is no named row to filter on. Restructure the chain so the divergent op runs before the projection, or wrap the projection in a named tuple `(X=_.X)`.")
        return false
    }
    // nolint:STYLE015 force_aliases=true: outer references inner cols by alias. Without AS, SQLite
    // names the column as the raw expr (e.g. `COUNT(*)`); outer can't bind `_.alias` → `"alias"`.
    let innerSql = build_sql_string(subQ, true)
    var innerBinds : array<ExpressionPtr>
    collect_query_binds(subQ, innerBinds)
    q.innerSql := innerSql
    q.innerBindExprs <- innerBinds
    q.fromRowType = inner_projection_type(subQ, at)
    if (empty(q.tableName)) {
        q.tableName := subQ.tableName
    }
    if (q.dbExpr == null) {
        q.dbExpr = subQ.dbExpr
    }
    if (subQ.proj == ProjectionShape.FullRow) {
        // FullRow inner: outer treats wrapped subquery as the original table; v1 path — unchanged.
        if (q.rootType == null) {
            q.rootType = subQ.rootType
        }
        if (q.proj == ProjectionShape.FullRow && empty(q.selectCols) && q.rootType != null) {
            fill_default_columns(q, q.rootType, prog, at)
        }
    } else {
        // NamedTuple/Grouped: passthrough aliases as SQL frags → `SELECT "a1", ... FROM (<innerSql>) AS t0`.
        if (!apply_passthrough_projection(q, q.fromRowType, prog, at)) return false
    }
    return true
}

// nolint:STYLE014
// Phase-divert dispatcher used by every peel that participates in multi-Q lowering.
// Encodes the standard "this op runs in eval phase `my_phase` but appears later in the
// chain than something at a lower phase — divert OR track" pattern.
//
// `max_outer_phase`: only divert when the already-pinned outer phase is ≤ this value.
// Default is unbounded (always divert when `my_phase > minPhaseSeen`). SELECT passes
// `PHASE_WHERE` so it diverts only behind an outer WHERE (other lower-phase ops fuse
// in canonical SQL form: `SELECT DISTINCT proj`, `SELECT proj LIMIT n`, etc.).
//
// Returns: 0 = no divert (caller continues normal processing).
//          1 = divert succeeded (caller should `return true`).
//         -1 = divert errored (caller should `return false`).
[macro_function]
def private maybe_divert(var q : SqlQuery&; my_phase : int; var node : ExpressionPtr;
                         prog : ProgramPtr; at : LineInfo;
                         max_outer_phase : int = INT_MAX_PHASE) : int {
    if (q.minPhaseSeen != INT_MAX_PHASE && my_phase > q.minPhaseSeen
            && q.minPhaseSeen <= max_outer_phase) return divert_to_inner(node, q, prog, at) ? 1 : -1
    if (my_phase < q.minPhaseSeen) { q.minPhaseSeen = my_phase; }
    return 0
}

[macro_function]
def private analyze_chain(var node : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    if (node == null) {
        macro_error(prog, at, "_sql: empty chain")
        return false
    }
    // ---- Terminals: one per chain, must precede _select / _where peel; _to_array sets materializer flag.
    {
        var inner : ExpressionPtr
        if (qmatch(node, _to_array($e(inner))).matched) {
            if (q.seenToArray || q.seenTerminal) {
                macro_error(prog, at, "_sql: only one terminal allowed per chain")
                return false
            }
            q.seenToArray = true
            q.matr = Materializer.Array
            return analyze_chain(inner, q, prog, at)
        }
    }
    // Peel first(prev) — `|> first()` or `|> _first()`.
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
                macro_error(prog, at, "_sql: `_first(predicate)` is not yet supported — use `|> _where(predicate) |> _first()` instead")
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
    // Peel count(prev) — `|> _count()` → SELECT COUNT(*) ...
    {
        var cCall = match_linq_call(node, "count")
        if (cCall != null) {
            if (q.seenTerminal || q.seenToArray) {
                macro_error(prog, at, "_sql: only one terminal allowed per chain")
                return false
            }
            if (cCall.arguments |> length != 1) {
                macro_error(prog, at, "_sql: `_count(predicate)` is not yet supported — use `|> _where(predicate) |> _count()` instead")
                return false
            }
            q.matr = Materializer.One
            q.seenTerminal = true
            q.proj = ProjectionShape.Aggregate
            q.aggregateExpr = "COUNT(*)"
            // Narrow SQLite int64 to daslang int — matches length()/count() return type.
            q.aggregateType = new TypeDecl(at = at, baseType = Type.tInt)
            return analyze_chain(cCall.arguments[0], q, prog, at)
        }
    }
    // ---- Column aggregates: sum / average / min / max ----
    {
        if (peel_column_aggregate(node, q, "sum",     "SUM", false, prog, at)) return q.aggregateExpr != ""
    }
    {
        if (peel_column_aggregate(node, q, "average", "AVG", true,  prog, at)) return q.aggregateExpr != ""
    }
    {
        if (peel_column_aggregate(node, q, "min",     "MIN", false, prog, at)) return q.aggregateExpr != ""
    }
    {
        if (peel_column_aggregate(node, q, "max",     "MAX", false, prog, at)) return q.aggregateExpr != ""
    }
    // ---- Stage operators: peel distinct(prev) — emits SELECT DISTINCT.
    {
        var dCall = match_linq_call(node, "distinct")
        if (dCall != null) {
            let dr = maybe_divert(q, PHASE_DISTINCT, node, prog, at)
            if (dr != 0) return dr > 0
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
    // Peel take(prev, n) — sets LIMIT n; bind emitted after WHERE so placeholder index lines up.
    {
        var tCall = match_linq_call(node, "take")
        if (tCall != null) {
            let dr = maybe_divert(q, PHASE_TAKE, node, prog, at)
            if (dr != 0) return dr > 0
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
    // Peel order_by(prev, lambda) — body `_.Col` → `"Col" ASC`. Recurse first so groupBy state is in place for `_._N`.
    {
        var obCall = match_linq_call(node, "order_by")
        if (obCall != null) {
            let dr = maybe_divert(q, PHASE_ORDER_BY, node, prog, at)
            if (dr != 0) return dr > 0
            if (q.seenOrderBy) {
                macro_error(prog, at, "_sql: _order_by() can appear at most once in the chain (use a tuple key for multi-column ordering: `_order_by((_.k1, _.k2))`)")
                return false
            }
            var body = peel_lambda_body(obCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _order_by key lambda has unexpected shape")
                return false
            }
            if (!analyze_chain(obCall.arguments[0], q, prog, at)
                    || !collect_order_keys(body, q, "ASC", prog, at)) return false
            q.seenOrderBy = true
            return true
        }
    }
    // Peel order_by_descending — same shape as order_by, emits DESC.
    {
        var odCall = match_linq_call(node, "order_by_descending")
        if (odCall != null) {
            let dr = maybe_divert(q, PHASE_ORDER_BY, node, prog, at)
            if (dr != 0) return dr > 0
            if (q.seenOrderBy) {
                macro_error(prog, at, "_sql: _order_by() can appear at most once in the chain (use a tuple key for multi-column ordering)")
                return false
            }
            var body = peel_lambda_body(odCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _order_by_descending key lambda has unexpected shape")
                return false
            }
            if (!analyze_chain(odCall.arguments[0], q, prog, at)
                    || !collect_order_keys(body, q, "DESC", prog, at)) return false
            q.seenOrderBy = true
            return true
        }
    }
    // Peel skip(prev, n) → OFFSET n. build_sql_string emits `LIMIT -1 OFFSET n` when no take.
    {
        var sCall = match_linq_call(node, "skip")
        if (sCall != null) {
            let dr = maybe_divert(q, PHASE_SKIP, node, prog, at)
            if (dr != 0) return dr > 0
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
    // Peel select(prev, lambda) — recurse first so q.seenGroupBy is set before projection analysis routes grouped vs row-scope.
    {
        var sCall = match_linq_call(node, "select")
        if (sCall != null) {
            // nolint:STYLE015 v2 divert only behind outer WHERE (phase 2) — SQL `WHERE` filters source rows,
            // but linq `_select |> _where` filters PROJECTED rows; wrap pushes _select into a subquery so the
            // outer WHERE can reference projection aliases. DISTINCT / TAKE / SKIP / ORDER_BY / set-op apply
            // to projected output in both SQL and linq positional — same semantics, no wrap needed.
            let dr = maybe_divert(q, PHASE_SELECT, node, prog, at, PHASE_WHERE)
            if (dr != 0) return dr > 0
            if (q.seenSelect) {
                macro_error(prog, at, "_sql: _select can only appear once in the chain")
                return false
            }
            if (q.proj == ProjectionShape.Aggregate) {
                // Aggregate terminal wins; mark seenSelect so select_from doesn't fill defaults.
                q.seenSelect = true
                return analyze_chain(sCall.arguments[0], q, prog, at)
            }
            // Recurse first so q.seenGroupBy reflects any downstream `_group_by` fusing with this `_select`.
            if (!analyze_chain(sCall.arguments[0], q, prog, at)) return false
            var body = peel_lambda_body(sCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _select projection lambda has unexpected shape")
                return false
            }
            if (q.seenGroupBy) {
                if (!analyze_grouped_projection(body, q, prog, at)) return false
            } else {
                if (!analyze_projection(body, q, prog, at)) return false
            }
            q.seenSelect = true
            return true
        }
    }
    // Peel _having — recurse first to validate that _group_by sits inside; HAVING binds land in q.havingBindExprs (after WHERE).
    {
        var hCall = match_linq_call(node, "having_")
        if (hCall != null) {
            // v1: no divert on _having (paired with _group_by below; v2 handles divert when needed).
            if (PHASE_HAVING < q.minPhaseSeen) { q.minPhaseSeen = PHASE_HAVING; }
            if (hCall.arguments |> length != 2) {
                macro_error(prog, at, "_sql: _having has unexpected shape; expected having_(prev, predicate)")
                return false
            }
            let beforeGroupBy = q.seenGroupBy
            if (!analyze_chain(hCall.arguments[0], q, prog, at)) return false
            if (q.seenHaving) {
                macro_error(prog, at, "_sql: _having can appear at most once in the chain")
                return false
            }
            if (!q.seenGroupBy) {
                macro_error(prog, at, "_sql: _having requires a preceding _group_by; for row-level filters use _where")
                return false
            }
            if (beforeGroupBy) {
                // _group_by set before recursion → it sits OUTSIDE this _having (HAVING came first in source order).
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
            if (q.hadError || empty(s)) return false   // pred_to_sql emitted macro_error, OR returned "" without setting hadError
            q.havingSql = s
            q.seenHaving = true
            return true
        }
    }
    // Peel _group_by / _group_by_lazy — both lower to group_by_lazy; emits GROUP BY.
    {
        var gbCall = match_linq_call(node, "group_by_lazy")
        if (gbCall != null) {
            // v1: no divert on _group_by; v2 handles when paired with post-aggregate filter.
            if (PHASE_GROUP_BY < q.minPhaseSeen) { q.minPhaseSeen = PHASE_GROUP_BY; }
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
            if (!collect_group_keys(body, q, prog, at)) return false
            q.seenGroupBy = true
            return analyze_chain(gbCall.arguments[0], q, prog, at)
        }
    }
    // Peel _where — recurse first so q.rootType is set before pred_to_sql runs (needs it for @sql_json/@sql_blob descent).
    {
        var wCall = match_linq_call(node, "where_")
        if (wCall != null) {
            // _where is broadly commutative with JOIN / GROUP / SELECT in SQL — no divert; just track min phase.
            if (PHASE_WHERE < q.minPhaseSeen) { q.minPhaseSeen = PHASE_WHERE; }
            var body = peel_lambda_body(wCall.arguments[1])
            if (body == null) {
                macro_error(prog, at, "_sql: _where predicate lambda has unexpected shape")
                return false
            }
            if (!analyze_chain(wCall.arguments[0], q, prog, at)) return false
            // nolint:STYLE015 v2.1: outer WHERE on JOIN's named-tuple `into` filters PROJECTED rows;
            // pred_to_sql against base scope emits `t0.alias` which doesn't exist. Snapshot in place so
            // WHERE renders as `... FROM (joinSql) AS "t0" WHERE "alias" = ...`, resolved through
            // q.fromRowType. The `_select |> _where` case is handled by SELECT peel's divert (no seenJoin).
            // Use snapshot helper, NOT divert_to_inner — re-analyzing re-instantiates the generic chain
            // and tickles a typer cascade in linq Mode-2/3.
            // Aggregate-with-WHERE (`_join | _where | _count`) needs same wrap for alias reference;
            // snapshot preserves aggregate state on the outer.
            if (q.seenJoin && (q.proj == ProjectionShape.NamedTuple || q.aggregateExpr != "")) {
                if (!snapshot_q_to_subquery_wrap(q, prog, at, [fillPassthrough = true])) return false
            }
            return analyze_predicate(body, q, prog, at)
        }
    }
    // Peel set operations (union/intersect/except). One set-op per chain; chained set-ops are deferred.
    {
        var soKind = SetOpKind.None
        var soCall : ExprCall?
        var uCall = match_linq_call(node, "union")
        if (uCall != null) {
            soCall = uCall
            soKind = SetOpKind.Union
        } else {
            var iCall = match_linq_call(node, "intersect")
            if (iCall != null) {
                soCall = iCall
                soKind = SetOpKind.Intersect
            } else {
                var eCall = match_linq_call(node, "except")
                if (eCall != null) {
                    soCall = eCall
                    soKind = SetOpKind.Except
                }
            }
        }
        if (soCall != null) {
            if (q.seenSetOp) {
                macro_error(prog, at, "_sql: only one set-op (UNION / INTERSECT / EXCEPT) per chain (chained set-ops are deferred)")
                return false
            }
            if (soCall.arguments |> length != 2) {
                macro_error(prog, at, "_sql: union/intersect/except expected 2 args (lhs, rhs) post-expansion")
                return false
            }
            // Recurse into LHS — fills q with the left query.
            if (!analyze_chain(soCall.arguments[0], q, prog, at)) return false
            // SQLite syntax-errors on per-side ORDER BY / LIMIT in compound queries — reject both sides.
            if (q.seenOrderBy) {
                macro_error(prog, at, "_sql: set-op LHS cannot use `_order_by` (apply to the combined set-op result instead)")
                return false
            }
            if (q.limitBindExpr != null || q.offsetBindExpr != null) {
                macro_error(prog, at, "_sql: set-op LHS cannot use `take(n)` / `skip(n)` (apply to the combined set-op result instead)")
                return false
            }
            // Recurse into RHS in a fresh sub-q; reject set-ops within set-ops.
            var subQ = SqlQuery(inView = q.inView)
            if (!analyze_chain(soCall.arguments[1], subQ, prog, at)) return false
            if (subQ.seenSetOp || subQ.setOpKind != SetOpKind.None) {
                macro_error(prog, at, "_sql: set-ops cannot themselves contain set-ops")
                return false
            }
            if (subQ.seenTerminal || subQ.seenToArray) {
                macro_error(prog, at, "_sql: set-op RHS cannot have terminals (`_first`, `_to_array`, etc.)")
                return false
            }
            if (subQ.seenOrderBy) {
                macro_error(prog, at, "_sql: set-op RHS cannot use `_order_by` (apply to the combined set-op result instead)")
                return false
            }
            if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
                macro_error(prog, at, "_sql: set-op RHS cannot use `take(n)` / `skip(n)` (apply to the combined set-op result instead)")
                return false
            }
            let rhsSql = build_sql_string(subQ)
            // RHS binds spliced (by collect_query_binds) after LHS WHERE/HAVING, before outer LIMIT.
            var rhsBinds : array<ExpressionPtr>
            collect_query_binds(subQ, rhsBinds)
            var rhsClones : array<ExpressionPtr>
            rhsClones |> reserve(length(rhsBinds))
            for (b in rhsBinds) {
                rhsClones |> push <| clone_expression(b)
            }
            q.setOpRhsBinds <- rhsClones
            q.setOpKind = soKind
            q.setOpRhsSql = rhsSql
            q.seenSetOp = true
            return true
        }
    }
    // Peel _join (linq_boost expands `on` into keya/keyb selectors).
    {
        var jCall = match_linq_call(node, "join")
        if (jCall != null) return process_join_call(jCall, q, JoinKind.Inner, prog, at)
    }
    // Peel _left_join — same shape as `join`, right side becomes NULL on no-match.
    {
        var ljCall = match_linq_call(node, "left_join")
        if (ljCall != null) return process_join_call(ljCall, q, JoinKind.Left, prog, at)
    }
    // Peel _right_join — mirror of left_join, left side becomes NULL on no-match. SQLite ≥3.39.
    {
        var rjCall = match_linq_call(node, "right_join")
        if (rjCall != null) return process_join_call(rjCall, q, JoinKind.Right, prog, at)
    }
    // Peel _full_outer_join — both sides surface; unmatched pair with NULL. SQLite ≥3.39.
    {
        var foCall = match_linq_call(node, "full_outer_join")
        if (foCall != null) return process_join_call(foCall, q, JoinKind.FullOuter, prog, at)
    }
    // Peel _cross_join — Cartesian; 3-arg shape (no on lambda).
    {
        var cxCall = match_linq_call(node, "cross_join")
        if (cxCall != null) return process_cross_join_call(cxCall, q, prog, at)
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

// ===== Projection analysis (_select) =====

[macro_function]
def private try_join_null_probe(var node : ExpressionPtr; q : SqlQuery;
                                var argName : string&; positive : bool) : string {
    if (!q.seenJoin || empty(q.joins)) return ""
    // Require a bare ExprVar — `$i` would over-match on ExprField/ExprCall receivers, capturing foreign names.
    var n = node
    if (n is ExprRef2Value) {
        n = (n as ExprRef2Value).subexpr
    }
    if (n == null || !(n is ExprVar)) return ""
    argName = string((n as ExprVar).name)
    let srcIdx = lookup_arg_source(q, argName)
    if (srcIdx < 0) return ""
    unsafe {
        let spec & = q.joins[length(q.joins) - 1]
        var probeAlias : string
        var sqlCol : string
        if (spec.kind == JoinKind.Left && srcIdx == 1) {
            probeAlias = string(spec.alias)
            sqlCol = field_to_column_name(spec.rootType, spec.rightCol)
        } elif (spec.kind == JoinKind.Right && srcIdx == 0) {
            // LHS owner type: q.rootType pre-wrap, q.fromRowType post multi-join wrap.
            let lhsOwner = q.rootType != null ? q.rootType : q.fromRowType
            probeAlias = string(spec.leftAlias)
            sqlCol = field_to_column_name(lhsOwner, spec.leftCol)
        } elif (spec.kind == JoinKind.FullOuter && srcIdx == 0) {
            let lhsOwner = q.rootType != null ? q.rootType : q.fromRowType
            probeAlias = string(spec.leftAlias)
            sqlCol = field_to_column_name(lhsOwner, spec.leftCol)
        } elif (spec.kind == JoinKind.FullOuter && srcIdx == 1) {
            probeAlias = string(spec.alias)
            sqlCol = field_to_column_name(spec.rootType, spec.rightCol)
        } else {
            return ""
        }
        if (positive) return "\"{probeAlias}\".\"{sqlCol}\" IS NOT NULL"
        return "\"{probeAlias}\".\"{sqlCol}\" IS NULL"
    }
}

[macro_function]
def private try_recognize_json_path_proj(val : ExpressionPtr; var q : SqlQuery&;
                                         var outFrag : string&; var outLeafType : TypeDeclPtr&;
                                         prog : ProgramPtr; at : LineInfo) : int {
    if (val == null || !(val is ExprField)) return 0
    var ef0 = val as ExprField
    var recv0 = ef0.value
    if (recv0 is ExprRef2Value) {
        recv0 = (recv0 as ExprRef2Value).subexpr
    }
    if (recv0 == null || !(recv0 is ExprField)) return 0
    var chain : array<string>
    chain |> push(string(ef0.name))
    var cur : ExpressionPtr = recv0
    while (cur != null && cur is ExprField) {
        var efc = cur as ExprField
        chain |> push(string(efc.name))
        cur = efc.value
        if (cur is ExprRef2Value) {
            cur = (cur as ExprRef2Value).subexpr
        }
    }
    if (cur == null || !(cur is ExprVar)) return 0
    let argName = string((cur as ExprVar).name)
    // Silent resolution (mirror of pred_to_sql): unrecognized names fall through, preserving sibling diagnostics.
    var srcIdx = -1
    if (q.seenJoin) {
        srcIdx = lookup_arg_source(q, argName)
        if (srcIdx < 0 && argName == "_") {
            srcIdx = 0
        }
    } elif (argName == "_") {
        srcIdx = 0
    }
    if (srcIdx < 0) return 0
    let nChain = length(chain)
    let colName = chain[nChain - 1]
    let srcType = source_rootType_for_idx(q, srcIdx)
    if (has_field_annotation(srcType, colName, "sql_blob")) {
        macro_error(prog, at, "_sql: cannot project @sql_blob column path '{colName}.…'; round-trip the row and access the field in daslang, or use raw SQL")
        return -1
    }
    if (!has_field_annotation(srcType, colName, "sql_json")) return 0
    var jsonPath : array<string>
    jsonPath |> reserve(nChain - 1)
    for (i in range(nChain - 1)) {
        jsonPath |> push(chain[nChain - 2 - i])
    }
    var leaf = resolve_nested_field_type(srcType, colName, jsonPath)
    if (leaf == null) {
        macro_error(prog, at, "_sql: JSON path on @sql_json column '{colName}' walks into a field that isn't a struct/tuple member at depth {length(jsonPath)}")
        return -1
    }
    outFrag = render_json_extract(srcType, source_alias_for_idx(q, srcIdx), colName, jsonPath)
    outLeafType = leaf
    return 1
}

[macro_function]
def private try_recognize_sql_function_proj(val : ExpressionPtr; var q : SqlQuery&;
                                            var outFrag : string&; var outLeafType : TypeDeclPtr&;
                                            prog : ProgramPtr; at : LineInfo) : int {
    if (val == null || !(val is ExprCall)) return 0
    var call = val as ExprCall
    if (call.func == null) return 0
    let info = sql_function_info_from_func(call.func)
    if (empty(info.name)) return 0
    // fn_call_to_sql handles directonly+inView guard and recursive arg emission.
    let frag = fn_call_to_sql(call, q, prog, at)
    if (q.hadError) return -1
    outFrag = frag
    outLeafType = call.func.result
    return 1
}

[macro_function]
def private push_computed_proj_slot(var q : SqlQuery&; frag : string;
                                    var leafType : TypeDeclPtr; recordName : string) {
    q.selectCols |> push("")
    q.selectColAliases |> push("")
    q.selectColSqlFragments |> push(frag)
    q.selectColTypes |> push(leafType)
    if (!empty(recordName)) {
        q.projRecordNames |> push(recordName)
    }
}

[macro_function]
def private resolve_proj_arg_to_source(q : SqlQuery; argName : string; prog : ProgramPtr; at : LineInfo) : int {
    if (!q.seenJoin) {
        if (argName == "_") return 0
        macro_error(prog, at, "_sql: _select projection refers to unbound arg `{argName}`; expected `_` in single-source mode")
        return -1
    }
    let idx = lookup_arg_source(q, argName)
    if (idx < 0) {
        macro_error(prog, at, "_sql: _select projection refers to unbound arg `{argName}`; the `into` lambda's parameter names are the only valid receivers")
        return -1
    }
    return idx
}

[macro_function]
def private analyze_projection(var body : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
    if (body == null) {
        macro_error(prog, at, "_sql: _select: missing projection")
        return false
    }
    // Peel ExprRef2Value — `is ExprMakeTuple` dispatch needs the unwrapped node.
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
        if (body == null) {
            macro_error(prog, at, "_sql: _select: ExprRef2Value with null subexpr")
            return false
        }
    }
    // Multi-level field chain `<arg>.Col.path…` — descends via json_extract for @sql_json.
    {
        var frag : string
        var leafType : TypeDeclPtr
        let m = try_recognize_json_path_proj(body, q, frag, leafType, prog, at)
        if (m < 0) return false
        if (m > 0) {
            q.selectCols |> clear
            q.selectColAliases |> clear
            q.selectColTypes |> clear
            q.selectColSqlFragments |> clear
            push_computed_proj_slot(q, frag, leafType, "")
            q.proj = ProjectionShape.SingleColumn
            return true
        }
    }
    // [sql_function] call — emits `<name>(args...)` so SQLite computes per-row.
    {
        var frag : string
        var leafType : TypeDeclPtr
        let m = try_recognize_sql_function_proj(body, q, frag, leafType, prog, at)
        if (m < 0) return false
        if (m > 0) {
            q.selectCols |> clear
            q.selectColAliases |> clear
            q.selectColTypes |> clear
            q.selectColSqlFragments |> clear
            push_computed_proj_slot(q, frag, leafType, "")
            q.proj = ProjectionShape.SingleColumn
            return true
        }
    }
    // Single-column form: <argname>.FieldName. Receiver must be ExprVar (else nested chains slip through).
    {
        var receiver : ExpressionPtr
        var fieldName : string
        if (qmatch(body, $e(receiver).$f(fieldName)).matched
                && receiver != null && receiver is ExprVar) {
            let argName = string((receiver as ExprVar).name)
            let srcIdx = resolve_proj_arg_to_source(q, argName, prog, at)
            if (srcIdx < 0) return false
            q.selectCols |> clear
            q.selectColAliases |> clear
            q.selectColTypes |> clear
            q.selectColSqlFragments |> clear
            q.selectCols |> push(fieldName)
            // Parallel arrays always populated — emitter discriminates on emptiness, not q.seenJoin.
            q.selectColAliases |> push(source_alias_for_idx(q, srcIdx))
            q.selectColSqlFragments |> push("")
            let srcType = source_rootType_for_idx(q, srcIdx)
            q.selectColTypes |> push(lookup_struct_field_type(srcType, fieldName))
            q.proj = ProjectionShape.SingleColumn
            return true
        }
    }
    // Named-tuple form: `<arg>.Field` cols or multi-source `is_some/is_none` LEFT JOIN probes.
    if (body is ExprMakeTuple) {
        var mkTup = body as ExprMakeTuple
        var tupT = mkTup._type
        if (tupT != null && tupT.baseType == Type.tTuple
                && !empty(tupT.argNames)
                && length(mkTup.values) == length(tupT.argNames)) {
            q.selectCols |> clear
            q.selectColAliases |> clear
            q.selectColTypes |> clear
            q.selectColSqlFragments |> clear
            q.projRecordNames |> clear
            for (i in range(length(mkTup.values))) {
                var val = mkTup.values[i]
                if (val is ExprRef2Value) {
                    val = (val as ExprRef2Value).subexpr
                }
                // JSON-path descent — must run before single-level qmatch.
                {
                    var jfrag : string
                    var jleaf : TypeDeclPtr
                    let m = try_recognize_json_path_proj(val, q, jfrag, jleaf, prog, at)
                    if (m < 0) return false
                    if (m > 0) {
                        push_computed_proj_slot(q, jfrag, jleaf, string(tupT.argNames[i]))
                        continue
                    }
                }
                // [sql_function] call as a tuple slot — same fragment-emit rules as single-col form.
                {
                    var sfrag : string
                    var sleaf : TypeDeclPtr
                    let m = try_recognize_sql_function_proj(val, q, sfrag, sleaf, prog, at)
                    if (m < 0) return false
                    if (m > 0) {
                        push_computed_proj_slot(q, sfrag, sleaf, string(tupT.argNames[i]))
                        continue
                    }
                }
                var receiver : ExpressionPtr
                var fieldName : string
                if (qmatch(val, $e(receiver).$f(fieldName)).matched
                        && receiver != null && receiver is ExprVar) {
                    let argName = string((receiver as ExprVar).name)
                    let srcIdx = resolve_proj_arg_to_source(q, argName, prog, at)
                    if (srcIdx < 0) return false
                    q.selectCols |> push(fieldName)
                    q.projRecordNames |> push(string(tupT.argNames[i]))
                    q.selectColAliases |> push(source_alias_for_idx(q, srcIdx))
                    q.selectColSqlFragments |> push("")
                    let srcType = source_rootType_for_idx(q, srcIdx)
                    q.selectColTypes |> push(lookup_struct_field_type(srcType, fieldName))
                    continue
                }
                // is_some / is_none on a LEFT JOIN's Option<TB> arg → bool computed projection.
                if (q.seenJoin && val is ExprCall) {
                    var fcall = val as ExprCall
                    // fcall.func may be null pre-inference — fall back to textual name.
                    var fname = string(fcall.name)
                    if (fcall.func != null) {
                        fname = string(fcall.func.name)
                        if (fcall.func.fromGeneric != null) {
                            fname = string(fcall.func.fromGeneric.name)
                        }
                    }
                    let argc = user_arg_count(fcall)
                    if ((fname == "is_some" || fname == "is_none") && argc == 1) {
                        var probeArg : string
                        let frag = try_join_null_probe(fcall.arguments[0], q, probeArg, fname == "is_some")
                        if (!empty(frag)) {
                            push_computed_proj_slot(q, frag,
                                new TypeDecl(at = at, baseType = Type.tBool),
                                string(tupT.argNames[i]))
                            continue
                        }
                    }
                }
                macro_error(prog, at, "_sql: _select named-tuple values must be `<arg>.Field` (column ref) or, after `_left_join`, `<arg> |> is_some` / `<arg> |> is_none` on the Option<TB> arg; got: {describe(val)}")
                return false
            }
            q.proj = ProjectionShape.NamedTuple
            return true
        }
    }
    macro_error(prog, at, "_sql: _select: supported shapes are `<arg>.Field` (single column) and `(Name=<arg>.Name, Price=<arg>.Price)` (named tuple); got: {describe(body)}")
    return false
}

// ===== Grouped projection analysis (_select after _group_by) =====

[macro_function]
def private group_key_column_name(q : SqlQuery; idx : int) : string {
    let raw = q.groupByCols[idx]
    let n = length(raw)
    if (n >= 2 && raw |> starts_with("\"") && raw |> ends_with("\"")) return slice(raw, 1, n - 1)
    return raw
}

[macro_function]
def private try_translate_group_aggregate(var node : ExpressionPtr; q : SqlQuery; prog : ProgramPtr; at : LineInfo;
                                          var sqlOut : string&; var typeOut : TypeDeclPtr&) : bool {
    var n = node
    if (n is ExprRef2Value) {
        n = (n as ExprRef2Value).subexpr
    }
    if (n == null || !(n is ExprCall)) return false
    var call = n as ExprCall
    if (call.func == null) return false
    var fname = string(call.func.name)
    if (call.func.fromGeneric != null) {
        fname = string(call.func.fromGeneric.name)
    }
    let argc = user_arg_count(call)
    // length(_._1) / count(_._1) → COUNT(*) (narrowed to int).
    if ((fname == "length" || fname == "count") && argc == 1
            && qmatch(call.arguments[0], _._1).matched) {
        sqlOut = "COUNT(*)"
        typeOut = new TypeDecl(at = at, baseType = Type.tInt)
        return true
    }
    // sum/average/min/max over `_._1 |> select($(p:T)=>p.X)` → SUM("X") etc.
    if ((fname == "sum" || fname == "average" || fname == "min" || fname == "max") && argc == 1) {
        var inner : ExpressionPtr = call.arguments[0]
        var innerCall = match_linq_call(inner, "select")
        if (innerCall != null && innerCall.arguments |> length == 2
                && qmatch(innerCall.arguments[0], _._1).matched) {
            var lamBody = peel_lambda_body(innerCall.arguments[1])
            if (lamBody == null) {
                macro_error(prog, at, "_sql: aggregate's inner select lambda has unexpected shape")
                return true
            }
            // Receiver-gate ExprVar: without it, `u.opt.X` over-matches as col="X" and emits silently-wrong SUM("X").
            var receiver : ExpressionPtr
            var col : string
            if (qmatch(lamBody, $e(receiver).$f(col)).matched
                    && receiver != null && receiver is ExprVar) {
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
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
        if (body == null) {
            macro_error(prog, at, "_sql: _select after _group_by: ExprRef2Value with null subexpr")
            return false
        }
    }
    if (!(body is ExprMakeTuple)) {
        macro_error(prog, at, "_sql: _select after _group_by must be a named-tuple projection — `(Key=_._0, N=_._1 |> length, AvgX=_._1 |> select($(u:T) => u.X) |> average)`. Got: {describe(body)}")
        return false
    }
    var mkTup = body as ExprMakeTuple
    var tupT = mkTup._type
    if (tupT == null || tupT.baseType != Type.tTuple
            || empty(tupT.argNames)
            || length(tupT.argNames) != length(mkTup.values)) {
        macro_error(prog, at, "_sql: _select after _group_by must be a NAMED tuple — every entry needs `Name=expr`")
        return false
    }
    q.groupedSelectExprs |> clear
    q.groupedSelectTypes |> clear
    q.projRecordNames |> clear
    let nKeys = length(q.groupByCols)
    for (i in range(length(mkTup.values))) {
        var val = mkTup.values[i]
        // single-key `_._0` — direct group-key reference.
        if (nKeys == 1 && qmatch(val, _._0).matched) {
            let col = group_key_column_name(q, 0)
            var fieldType = lookup_struct_field_type(q.rootType, col)
            if (fieldType == null) {
                macro_error(prog, at, "_sql: cannot resolve type of group key column '{col}'")
                return false
            }
            q.groupedSelectExprs |> push("\"{field_to_column_name(q.rootType, col)}\"")
            q.groupedSelectTypes |> push(fieldType)
            q.projRecordNames |> push(string(tupT.argNames[i]))
            continue
        }
        // multi-key `_._0._<n>` — N-th key column.
        if (nKeys > 1) {
            var outerName : string
            if (qmatch(val, _._0.$f(outerName)).matched) {
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
                        q.groupedSelectExprs |> push("\"{field_to_column_name(q.rootType, col)}\"")
                        q.groupedSelectTypes |> push(fieldType)
                        q.projRecordNames |> push(string(tupT.argNames[i]))
                        continue
                    }
                    macro_error(prog, at, "_sql: multi-key group has {nKeys} keys; `_._0.{outerName}` is out of range")
                    return false
                }
            }
            // Multi-key reject: `_._0` alone (whole key tuple) is ambiguous in projection.
            if (qmatch(val, _._0).matched) {
                macro_error(prog, at, "_sql: multi-key group: `_._0` refers to the whole key tuple; use `_._0._N` to access the N-th key column")
                return false
            }
        }
        // Aggregate over the group's elements (`_._1`).
        var sqlFrag : string
        var resType : TypeDeclPtr
        if (try_translate_group_aggregate(val, q, prog, at, sqlFrag, resType)) {
            if (empty(sqlFrag)) return false   // helper emitted macro_error
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

// ===== Predicate analysis (_where) =====

[macro_function]
def private collect_order_keys(var body : ExpressionPtr; var q : SqlQuery&; direction : string; prog : ProgramPtr; at : LineInfo) : bool {
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
    // Group-key form: `_._0` (single key) or `_._0._N` (multi-key).
    if (q.seenGroupBy && body is ExprField) {
        var ef = body as ExprField
        var recv = ef.value
        if (recv is ExprRef2Value) {
            recv = (recv as ExprRef2Value).subexpr
        }
        let outerName = string(ef.name)
        let nKeys = length(q.groupByCols)
        // Single-key: `_._0` → first group-by column (groupByCols stores daslang names; translate here).
        if (nKeys == 1 && recv != null && recv is ExprVar
                && (recv as ExprVar).name == "_"
                && outerName == "_0") {
            let dasName = group_key_column_name(q, 0)
            let sqlCol = field_to_column_name(q.rootType, dasName)
            q.orderByCols |> push("\"{sqlCol}\" {direction}")
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
                    let dasName = group_key_column_name(q, keyIdx)
                    let sqlCol = field_to_column_name(q.rootType, dasName)
                    q.orderByCols |> push("\"{sqlCol}\" {direction}")
                    return true
                }
            }
        }
    }
    // Single-column form: _.FieldName
    {
        var fieldName : string
        if (qmatch(body, _.$f(fieldName)).matched) {
            let sqlCol = field_to_column_name(q.rootType, fieldName)
            q.orderByCols |> push("\"{sqlCol}\" {direction}")
            return true
        }
    }
    // Multi-column tuple `(_.k1, _.k2)` — each entry picks compile-time `_.Field` or runtime string column.
    if (body is ExprMakeTuple) {
        var mkTup = body as ExprMakeTuple
        if (empty(mkTup.values)) {
            macro_error(prog, at, "_sql: _order_by: empty tuple key")
            return false
        }
        for (i in range(length(mkTup.values))) {
            var val = mkTup.values[i]
            var fieldName : string
            if (qmatch(val, _.$f(fieldName)).matched) {
                let sqlCol = field_to_column_name(q.rootType, fieldName)
                q.orderByCols |> push("\"{sqlCol}\" {direction}")
            } elif (val._type != null && val._type.baseType == Type.tString) {
                q.orderByInlineExprs |> push <| clone_expression(val)
                q.orderByCols |> push("\x01 {direction}")
            } else {
                macro_error(prog, at, "_sql: _order_by: tuple-key entries must be `_.Field` or a `string` expression; got: {describe(val)}")
                return false
            }
        }
        return true
    }
    // Runtime form: string-typed expression naming the column; quoted via sql_quote_id at emission.
    if (body._type != null && body._type.baseType == Type.tString) {
        q.orderByInlineExprs |> push <| clone_expression(body)
        q.orderByCols |> push("\x01 {direction}")
        return true
    }
    macro_error(prog, at, "_sql: _order_by: key must be `_.Field`, a `string` expression, or a tuple of either; got: {describe(body)}")
    return false
}

[macro_function]
def private collect_group_keys(var body : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : bool {
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
    // Single-column form: _.FieldName. groupByCols stores DASLANG names; SQL translation deferred to emission.
    {
        var fieldName : string
        if (qmatch(body, _.$f(fieldName)).matched) {
            q.groupByCols |> push("\"{fieldName}\"")
            return true
        }
    }
    // Multi-column form: tuple literal `(_.k1, _.k2)`.
    if (body is ExprMakeTuple) {
        var mkTup = body as ExprMakeTuple
        if (empty(mkTup.values)) {
            macro_error(prog, at, "_sql: _group_by: empty tuple key")
            return false
        }
        for (i in range(length(mkTup.values))) {
            var val = mkTup.values[i]
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
    if (body == null) {
        macro_error(prog, at, "_sql: _where: missing predicate")
        return false
    }
    let s = pred_to_sql(body, q, prog, at)
    // operator handlers wrap empty children (`lhs = rhs` -> `" = ?"`); empty(s) alone isn't reliable
    if (q.hadError || empty(s)) return false
    if (empty(q.whereSql)) {
        q.whereSql = s
    } else {
        q.whereSql = "({q.whereSql}) AND ({s})"
    }
    return true
}

[macro_function]
def private pred_fail(var q : SqlQuery&; prog : ProgramPtr; at : LineInfo; msg : string) : string {
    q.hadError = true
    if (empty(q.lastError)) {
        q.lastError = msg
    }
    macro_error(prog, at, msg)
    return ""
}

[macro_function]
def private pred_to_sql(var node : ExpressionPtr; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : string {
    if (node == null) return pred_fail(q, prog, at, "_sql: _where: null sub-expression")
    if (node is ExprRef2Value) {
        node = (node as ExprRef2Value).subexpr
        if (node == null) return pred_fail(q, prog, at, "_sql: _where: ExprRef2Value with null subexpr")
    }
    // Logical NOT — `_not_in` lowers to `!contains(subq, x)`; dispatch on textual name (pre-typer).
    {
        var inner : ExpressionPtr
        if (qmatch(node, !$e(inner)).matched) { // nolint:STYLE016 — structural NOT-peel, not a guard chain
            if (inner != null && inner is ExprCall) {
                var icall = inner as ExprCall
                let iname = string(icall.name)
                let iargc = user_arg_count(icall)
                if (iname == "contains" && iargc == 2) {
                    // Disambiguate string contains vs iterator contains by first-arg inferred type.
                    var firstT = icall.arguments[0]._type
                    if (firstT != null && (firstT.isIterator || firstT.isGoodArrayType)) {
                        let xSql = pred_to_sql(icall.arguments[1], q, prog, at)
                        return "" if (q.hadError)
                        // Captured-collection form: arg1 is `array<T>` and not a chain → JSON bridge.
                        if (!looks_like_subquery_chain(icall.arguments[0])
                                && firstT.isGoodArrayType) {
                            let nullable = is_nullable_column_ref(icall.arguments[1], q)
                            return emit_in_collection_json_bridge(icall.arguments[0], q, xSql, true, nullable)
                        }
                        var binds : array<ExpressionPtr>
                        let subSql = analyze_subquery(icall.arguments[0], binds, prog, at, true, q.inView)
                        if (empty(subSql)) {
                            q.hadError = true   // analyze_subquery emitted macro_error
                            return ""
                        }
                        for (b in binds) {
                            if (q.inHaving) {
                                q.havingBindExprs |> push <| clone_expression(b)
                            } else {
                                q.bindExprs |> push <| clone_expression(b)
                            }
                        }
                        return "{xSql} NOT IN ({subSql})"
                    }
                }
            }
            return "NOT ({pred_to_sql(inner, q, prog, at)})"
        }
    }
    // Logical AND
    {
        var lhs : ExpressionPtr
        var rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) && $e(rhs)).matched) return "({pred_to_sql(lhs, q, prog, at)}) AND ({pred_to_sql(rhs, q, prog, at)})"
    }
    // Logical OR
    {
        var lhs : ExpressionPtr
        var rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) || $e(rhs)).matched) return "({pred_to_sql(lhs, q, prog, at)}) OR ({pred_to_sql(rhs, q, prog, at)})"
    }
    // Comparison ops.
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) == $e(rhs)).matched) {
            // `== none()` would silently bind a 0-byte placeholder; nudge to is_some/is_none.
            if (is_none_call(lhs) || is_none_call(rhs)) return pred_fail(q, prog, at, "_sql: `_.Col == none()` is not translated. Use `_.Col |> is_none()` (or `is_some()` for the negation) — `none()` returns Option<T>, equality against it would silently bind an empty placeholder.")
            return "{pred_to_sql(lhs, q, prog, at)} = {pred_to_sql(rhs, q, prog, at)}"
        }
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) != $e(rhs)).matched) {
            if (is_none_call(lhs) || is_none_call(rhs)) return pred_fail(q, prog, at, "_sql: `_.Col != none()` is not translated. Use `_.Col |> is_some()` (the inverse of is_none).")
            return "{pred_to_sql(lhs, q, prog, at)} <> {pred_to_sql(rhs, q, prog, at)}"
        }
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) < $e(rhs)).matched) return "{pred_to_sql(lhs, q, prog, at)} < {pred_to_sql(rhs, q, prog, at)}"
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) <= $e(rhs)).matched) return "{pred_to_sql(lhs, q, prog, at)} <= {pred_to_sql(rhs, q, prog, at)}"
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) > $e(rhs)).matched) return "{pred_to_sql(lhs, q, prog, at)} > {pred_to_sql(rhs, q, prog, at)}"
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) >= $e(rhs)).matched) return "{pred_to_sql(lhs, q, prog, at)} >= {pred_to_sql(rhs, q, prog, at)}"
    }
    // Arithmetic — parens preserve precedence in emitted SQL.
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) + $e(rhs)).matched) return "({pred_to_sql(lhs, q, prog, at)}) + ({pred_to_sql(rhs, q, prog, at)})"
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) - $e(rhs)).matched) return "({pred_to_sql(lhs, q, prog, at)}) - ({pred_to_sql(rhs, q, prog, at)})"
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) * $e(rhs)).matched) return "({pred_to_sql(lhs, q, prog, at)}) * ({pred_to_sql(rhs, q, prog, at)})"
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) / $e(rhs)).matched) return "({pred_to_sql(lhs, q, prog, at)}) / ({pred_to_sql(rhs, q, prog, at)})"
    }
    {
        var lhs, rhs : ExpressionPtr
        if (qmatch(node, $e(lhs) % $e(rhs)).matched) return "({pred_to_sql(lhs, q, prog, at)}) % ({pred_to_sql(rhs, q, prog, at)})"
    }
    // Multi-level field chain `<arg>.Col.path…` — descends @sql_json via json_extract; @sql_blob is a compile error.
    if (node is ExprField) {
        var ef0 = node as ExprField
        var recv0 = ef0.value
        if (recv0 is ExprRef2Value) {
            recv0 = (recv0 as ExprRef2Value).subexpr
        }
        // Need at least two ExprField levels to be a multi-level chain.
        if (recv0 != null && recv0 is ExprField) {
            // Walk receiver chain outer-to-inner; innermost name is the SQL column, earlier names form the JSON path.
            var chain : array<string>
            chain |> push(string(ef0.name))
            var cur : ExpressionPtr = recv0
            while (cur != null && cur is ExprField) {
                var efc = cur as ExprField
                chain |> push(string(efc.name))
                cur = efc.value
                if (cur is ExprRef2Value) {
                    cur = (cur as ExprRef2Value).subexpr
                }
            }
            if (cur != null && cur is ExprVar) {
                let argName = string((cur as ExprVar).name)
                var srcIdx = -1
                if (!empty(q.rootAlias)) {
                    srcIdx = lookup_arg_source(q, argName)
                    if (srcIdx < 0 && argName == "_") {
                        srcIdx = 0
                    }
                } elif (argName == "_") {
                    srcIdx = 0
                }
                if (srcIdx >= 0) {
                    let nChain = length(chain)
                    let colName = chain[nChain - 1]
                    let srcType = source_rootType_for_idx(q, srcIdx)
                    if (has_field_annotation(srcType, colName, "sql_blob")) return pred_fail(q, prog, at, "_sql: cannot descend into @sql_blob column '{colName}'; filter in daslang after round-trip, or use raw SQL")
                    if (has_field_annotation(srcType, colName, "sql_json")) {
                        // Reverse outer-to-inner chain into column-out path (drop the column name).
                        var jsonPath : array<string>
                        jsonPath |> reserve(nChain - 1)
                        for (i in range(nChain - 1)) {
                            jsonPath |> push(chain[nChain - 2 - i])
                        }
                        // Validate every path segment — the typer never sees `_where`'s lambda body so silent NULL-on-typo is the failure mode.
                        let leaf = resolve_nested_field_type(srcType, colName, jsonPath)
                        if (leaf == null) return pred_fail(q, prog, at, "_sql: JSON path on @sql_json column '{colName}' walks into a field that isn't a struct/tuple member at depth {length(jsonPath)}")
                        return render_json_extract(srcType, source_alias_for_idx(q, srcIdx), colName, jsonPath)
                    }
                }
            }
        }
    }
    // Column reference: <arg>.Col. Pin receiver to ExprVar so nested chains can't slip through.
    {
        var receiver : ExpressionPtr
        var fieldName : string
        if (qmatch(node, $e(receiver).$f(fieldName)).matched
                && receiver != null && receiver is ExprVar) {
            let argName = string((receiver as ExprVar).name)
            // Alias-qualification is driven by rootAlias != "", NOT seenJoin (right-side subQ runs with rootAlias="t1" but seenJoin=false).
            if (!empty(q.rootAlias)) {
                var srcIdx = lookup_arg_source(q, argName)
                // Pre-join `_where` references root through `_` — arg-binding stack only populated inside `into` projection.
                if (srcIdx < 0 && argName == "_") {
                    srcIdx = 0
                }
                if (srcIdx >= 0) {
                    let rootT = source_rootType_for_idx(q, srcIdx)
                    return column_sql_for_alias(source_alias_for_idx(q, srcIdx), rootT, fieldName)
                }
                // Not bound — fall through (e.g. `_._0` group-key handled later).
            } else {
                // Single-source: only `_` is bound.
                if (argName == "_") {
                    // nolint:STYLE015 Multi-Q outer over named-tuple inner: rootType is null, resolve `_.alias`
                    // against fromRowType.argNames; FROM is `(<innerSql>) AS t0` so unqualified suffices.
                    if (q.rootType == null && q.fromRowType != null) {
                        if (!fromrow_alias_exists(q.fromRowType, fieldName)) return pred_fail(q, prog, at, "_sql: column `{fieldName}` is not a projection alias of the inner subquery; valid aliases are {fromrow_alias_list(q.fromRowType)}")
                        return "\"{fieldName}\""
                    }
                    return column_sql_for_alias("", q.rootType, fieldName)
                }
                // `_excluded.Col` inside upsertMode → SQLite's `excluded.col` (proposed-row sentinel).
                if (q.upsertMode && argName == "_excluded") {
                    let sqlCol = field_to_column_name(q.rootType, fieldName)
                    return "excluded.\"{sqlCol}\""
                }
            }
        }
    }
    // Group-scope aggregate in expression position (typically HAVING).
    {
        var aggSql : string
        var aggType : TypeDeclPtr
        if (try_translate_group_aggregate(node, q, prog, at, aggSql, aggType)) {
            if (empty(aggSql)) {
                q.hadError = true
                return ""
            }
            return aggSql
        }
    }
    // Unexpanded `_in`/`_not_in`/`_any`/`_none` — Mode-2 doesn't deep-walk into operator children, so dispatch by name.
    if (node is ExprCallMacro) {
        var mcall = node as ExprCallMacro
        let mname = string(mcall.name)
        if ((mname == "_in" || mname == "_not_in") && length(mcall.arguments) == 2) {
            // arg order: (element, collection-or-subquery)
            let xSql = pred_to_sql(mcall.arguments[0], q, prog, at)
            return "" if (q.hadError)
            var collArg : ExpressionPtr = mcall.arguments[1]
            // Subquery form first: `select_from(...) |> ...` chains have array type
            // post-typer, so isGoodArrayType alone can't distinguish them from a
            // captured runtime array — pick by AST shape (chain bottoms out at
            // `select_from`).
            if (looks_like_subquery_chain(collArg)) {
                var binds : array<ExpressionPtr>
                let subSql = analyze_subquery(collArg, binds, prog, at, true, q.inView)
                if (empty(subSql)) {
                    q.hadError = true   // analyze_subquery emitted macro_error
                    return ""
                }
                for (b in binds) {
                    if (q.inHaving) {
                        q.havingBindExprs |> push <| clone_expression(b)
                    } else {
                        q.bindExprs |> push <| clone_expression(b)
                    }
                }
                if (mname == "_not_in") return "{xSql} NOT IN ({subSql})"
                return "{xSql} IN ({subSql})"
            }
            // Captured-collection form: arg2 is `array<T>` (variable, function result,
            // literal). Bind JSON-encoded TEXT and use SQLite's table-valued json_each —
            // single bind, any size, plan-cache-friendly (matches EF Core 8's default).
            if (collArg._type != null && collArg._type.isGoodArrayType) {
                let negated = mname == "_not_in"
                let nullable = negated && is_nullable_column_ref(mcall.arguments[0], q)
                return emit_in_collection_json_bridge(collArg, q, xSql, negated, nullable)
            }
            // Neither shape — let the existing subquery analyzer surface the diagnostic.
            var binds : array<ExpressionPtr>
            let subSql = analyze_subquery(collArg, binds, prog, at, true, q.inView)
            if (empty(subSql)) {
                q.hadError = true
                return ""
            }
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            if (mname == "_not_in") return "{xSql} NOT IN ({subSql})"
            return "{xSql} IN ({subSql})"
        }
        if ((mname == "_any" || mname == "_none") && !empty(mcall.arguments)) {
            // 1-arg `subq._any()/_none()` or 2-arg `_any(pred)/_none(pred)` (pred bare; call_macro hasn't lambda-wrapped).
            var subSql : string
            var binds : array<ExpressionPtr>
            if (length(mcall.arguments) == 1) {
                subSql = analyze_subquery(mcall.arguments[0], binds, prog, at, false, q.inView)
            } else {
                // Build subquery, then run analyze_predicate on the bare pred (skips lambda wrapping).
                var subQ = SqlQuery(inView = q.inView)
                if (!analyze_chain(mcall.arguments[0], subQ, prog, at)) {
                    q.hadError = true   // analyze_chain emitted macro_error on subQ
                    return ""
                }
                if (subQ.seenJoin || !empty(subQ.joins)) return pred_fail(q, prog, at, "_sql: subqueries cannot themselves contain joins")
                if (subQ.seenGroupBy || !empty(subQ.havingSql) || subQ.seenOrderBy
                        || subQ.distinctFlag || subQ.limitBindExpr != null
                        || subQ.offsetBindExpr != null || subQ.seenTerminal
                        || subQ.seenToArray) return pred_fail(q, prog, at, "_sql: _any/_none subquery must be a `select_from(...)` (chain stages and terminals are not yet allowed)")
                if (!analyze_predicate(mcall.arguments[1], subQ, prog, at)) {
                    q.hadError = true   // analyze_predicate emitted macro_error on subQ
                    return ""
                }
                subSql = build_sql_string(subQ)
                binds |> reserve(length(binds) + length(subQ.bindExprs))
                for (b in subQ.bindExprs) {
                    binds |> push <| clone_expression(b)
                }
            }
            if (empty(subSql)) {
                q.hadError = true   // sub-helper emitted macro_error
                return ""
            }
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            if (mname == "_none") return "NOT EXISTS ({subSql})"
            return "EXISTS ({subSql})"
        }
    }
    // Function call → SQL function dispatch (table-driven below).
    if (node is ExprCall) {
        let sql = fn_call_to_sql(node as ExprCall, q, prog, at)
        if (!empty(sql)) return sql
        // fn_call_to_sql may have emitted a targeted macro_error via pred_fail (e.g. text_match on non-FTS5 struct); stop here so the generic "unsupported expression" doesn't double-fire.
        if (q.hadError) return ""
    }
    // nolint:STYLE015 Detect post-typer `operator==(a, none())` for is_none/is_some fixit. Must run
    // BEFORE the bind fallthrough, otherwise `none()` would silently bind an empty placeholder.
    if (node is ExprCall) {
        let c = node as ExprCall
        let cname = string(c.name)
        if (cname |> find("eq") >= 0 && length(c.arguments) >= 2) {
            for (a in c.arguments) {
                if (is_none_call(a)) {
                    let positive = cname |> find("not") < 0
                    return pred_fail(q, prog, at,
                        positive
                            ? "_sql: `_.Col == none()` is not translated. Use `_.Col |> is_none()` (or `is_some()` for the negation) — `none()` returns Option<T>, equality against it would silently bind an empty placeholder."
                            : "_sql: `_.Col != none()` is not translated. Use `_.Col |> is_some()` (the inverse of is_none).")
                }
            }
        }
    }
    // Bind fallthrough: unrecognized exprs become `?`; sql_bind catch-all + concept_assert error if not bindable.
    if (q.inHaving) {
        q.havingBindExprs |> push <| clone_expression(node)
    } else {
        q.bindExprs |> push <| clone_expression(node)
    }
    return "?"
}

// ===== SQL function dispatch (daslang call -> SQL expression) =====

[macro_function]
def private user_arg_count(call : ExprCall?) : int {
    if (call == null) return 0
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
def private analyze_subquery(var subqExpr : ExpressionPtr; var binds : array<ExpressionPtr>&;
                             prog : ProgramPtr; at : LineInfo;
                             requireSingleColumn : bool = false; inView : bool = false) : string {
    var subQ = SqlQuery(inView = inView)
    if (!analyze_chain(subqExpr, subQ, prog, at)) return ""
    if (subQ.seenJoin || !empty(subQ.joins)) {
        macro_error(prog, at, "_sql: subqueries cannot themselves contain joins")
        return ""
    }
    if (subQ.seenGroupBy || !empty(subQ.havingSql)) {
        macro_error(prog, at, "_sql: subqueries cannot use `_group_by` / `_having`")
        return ""
    }
    if (subQ.seenOrderBy || subQ.distinctFlag) {
        macro_error(prog, at, "_sql: subqueries cannot use `_order_by` / `distinct()`")
        return ""
    }
    if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
        macro_error(prog, at, "_sql: subqueries cannot use `take(n)` / `skip(n)`")
        return ""
    }
    if (subQ.seenTerminal || subQ.seenToArray) {
        macro_error(prog, at, "_sql: subqueries cannot have terminals (`_first`, `_to_array`, `_count`, etc.)")
        return ""
    }
    if (requireSingleColumn && subQ.proj != ProjectionShape.SingleColumn) {
        macro_error(prog, at, "_sql: _in / _not_in subquery must project exactly one column via `_select(_.Col)` (got: {subQ.proj}); SQLite rejects multi-column subqueries on the right of IN")
        return ""
    }
    let sql = build_sql_string(subQ)
    binds |> reserve(length(binds) + length(subQ.bindExprs))
    for (b in subQ.bindExprs) {
        binds |> push <| clone_expression(b)
    }
    return sql
}

[macro_function]
def private analyze_subquery_with_pred(var subqExpr : ExpressionPtr; var predLambda : ExpressionPtr;
                                       var binds : array<ExpressionPtr>&;
                                       prog : ProgramPtr; at : LineInfo;
                                       inView : bool = false) : string {
    var subQ = SqlQuery(inView = inView)
    if (!analyze_chain(subqExpr, subQ, prog, at)) return ""
    if (subQ.seenJoin || !empty(subQ.joins)) {
        macro_error(prog, at, "_sql: subqueries cannot themselves contain joins")
        return ""
    }
    if (subQ.seenGroupBy || !empty(subQ.havingSql)) {
        macro_error(prog, at, "_sql: subqueries cannot use `_group_by` / `_having`")
        return ""
    }
    if (subQ.seenOrderBy || subQ.distinctFlag) {
        macro_error(prog, at, "_sql: subqueries cannot use `_order_by` / `distinct()`")
        return ""
    }
    if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
        macro_error(prog, at, "_sql: subqueries cannot use `take(n)` / `skip(n)`")
        return ""
    }
    if (subQ.seenTerminal || subQ.seenToArray) {
        macro_error(prog, at, "_sql: subqueries cannot have terminals (`_first`, `_to_array`, `_count`, etc.)")
        return ""
    }
    var body = peel_lambda_body(predLambda)
    if (body == null) {
        macro_error(prog, at, "_sql: _any/_none predicate lambda has unexpected shape")
        return ""
    }
    if (!analyze_predicate(body, subQ, prog, at)) return ""
    let sql = build_sql_string(subQ)
    binds |> reserve(length(binds) + length(subQ.bindExprs))
    for (b in subQ.bindExprs) {
        binds |> push <| clone_expression(b)
    }
    return sql
}

[macro_function]
def private fn_call_to_sql(var call : ExprCall?; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : string {
    if (call == null) return ""
    var fname : string
    if (call.func != null) {
        fname = string(call.func.name)
        if (call.func.fromGeneric != null) {
            fname = string(call.func.fromGeneric.name)
        }
    } else {
        fname = string(call.name)
    }
    let argc = user_arg_count(call)
    // ---- String predicates: LIKE patterns — RHS wrapped via `_::like_escape_*` so user `%`/`_`/`\` match literally.
    if (fname == "starts_with" && argc == 2) {
        let lhs = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        var rawArg : ExpressionPtr = clone_expression(call.arguments[1])
        var wrapped : ExpressionPtr = qmacro(_::like_escape_prefix($e(rawArg)))
        if (q.inHaving) {
            q.havingBindExprs |> push(wrapped)
        } else {
            q.bindExprs |> push(wrapped)
        }
        return "{lhs} LIKE ? ESCAPE '\\'"
    }
    if (fname == "ends_with" && argc == 2) {
        let lhs = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        var rawArg : ExpressionPtr = clone_expression(call.arguments[1])
        var wrapped : ExpressionPtr = qmacro(_::like_escape_suffix($e(rawArg)))
        if (q.inHaving) {
            q.havingBindExprs |> push(wrapped)
        } else {
            q.bindExprs |> push(wrapped)
        }
        return "{lhs} LIKE ? ESCAPE '\\'"
    }
    if (fname == "contains" && argc == 2) {
        // String contains → LIKE; iterator/array contains → IN subquery (handled below).
        var firstT = call.arguments[0]._type
        if (firstT != null && (firstT.isIterator || firstT.isGoodArrayType)) {
            // Fall through to subquery branch.
        } else {
            let lhs = pred_to_sql(call.arguments[0], q, prog, at)
            return "" if (q.hadError)
            var rawArg : ExpressionPtr = clone_expression(call.arguments[1])
            var wrapped : ExpressionPtr = qmacro(_::like_escape_substring($e(rawArg)))
            if (q.inHaving) {
                q.havingBindExprs |> push(wrapped)
            } else {
                q.bindExprs |> push(wrapped)
            }
            return "{lhs} LIKE ? ESCAPE '\\'"
        }
    }
    // ---- FTS5 (tut 40): text_match(_.Body, q) on [sql_fts5] → `Body MATCH ?`; non-FTS5 is a compile error.
    if (fname == "text_match" && argc == 2) {
        if (!has_struct_annotation(q.rootType, "sql_fts5")) return pred_fail(q, prog, at, "_sql: text_match() requires an [sql_fts5] struct, got '{describe(q.rootType)}'. Options: use contains(_.Col, q) for LIKE-based substring match, or annotate the struct with [sql_fts5] for indexed full-text search.")
        let lhs = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        if (q.inHaving) {
            q.havingBindExprs |> push <| clone_expression(call.arguments[1])
        } else {
            q.bindExprs |> push <| clone_expression(call.arguments[1])
        }
        return "{lhs} MATCH ?"
    }
    // ---- Case folding ----
    if (fname == "to_lower" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        return "LOWER({inner})"
    }
    if (fname == "to_upper" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        return "UPPER({inner})"
    }
    // ---- String length ----
    if (fname == "length" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        return "LENGTH({inner})"
    }
    // ---- Numeric scalar ----
    if (fname == "abs" && argc == 1) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        return "ABS({inner})"
    }
    // ---- Option<T> nullability — left/right/full-outer join: bare ExprVar of Option<T> arg probes the join key NULL.
    if (fname == "is_some" && argc == 1) {
        var argName : string
        let nullProbe = try_join_null_probe(call.arguments[0], q, argName, true)
        if (!empty(nullProbe)) return nullProbe
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        return "{inner} IS NOT NULL"
    }
    if (fname == "is_none" && argc == 1) {
        var argName : string
        let nullProbe = try_join_null_probe(call.arguments[0], q, argName, false)
        if (!empty(nullProbe)) return nullProbe
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        return "{inner} IS NULL"
    }
    // `_.Col |> unwrap_or(d)` → `COALESCE("Col", ?)`; default routes through pred_to_sql (literal/local/column).
    if (fname == "unwrap_or" && argc == 2) {
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "" if (q.hadError)
        let dflt  = pred_to_sql(call.arguments[1], q, prog, at)
        return "" if (q.hadError)
        return "COALESCE({inner}, {dflt})"
    }
    // ---- Subqueries: `x._in(subq)` → `contains(subq, x)`; `_not_in` → `!contains(...)` (leading `!` peeled).
    if (fname == "contains" && argc == 2) {
        let xSql = pred_to_sql(call.arguments[1], q, prog, at)
        return "" if (q.hadError)
        // Captured-collection form: arg0 is `array<T>` and not a chain → JSON bridge.
        var firstT = call.arguments[0]._type
        if (firstT != null && firstT.isGoodArrayType
                && !looks_like_subquery_chain(call.arguments[0])) return emit_in_collection_json_bridge(call.arguments[0], q, xSql, false, false)
        var binds : array<ExpressionPtr>
        let subSql = analyze_subquery(call.arguments[0], binds, prog, at, true, q.inView)
        if (empty(subSql)) {
            q.hadError = true   // analyze_subquery emitted macro_error
            return ""
        }
        for (b in binds) {
            if (q.inHaving) {
                q.havingBindExprs |> push <| clone_expression(b)
            } else {
                q.bindExprs |> push <| clone_expression(b)
            }
        }
        return "{xSql} IN ({subSql})"
    }
    // `_any` → EXISTS (...), `_any(pred)` → EXISTS (... WHERE pred).
    if (fname == "any") {
        if (argc == 1) {
            var binds : array<ExpressionPtr>
            let subSql = analyze_subquery(call.arguments[0], binds, prog, at, false, q.inView)
            if (empty(subSql)) return ""
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            return "EXISTS ({subSql})"
        }
        if (argc == 2) {
            var binds : array<ExpressionPtr>
            let subSql = analyze_subquery_with_pred(call.arguments[0], call.arguments[1], binds, prog, at, q.inView)
            if (empty(subSql)) return ""
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            return "EXISTS ({subSql})"
        }
    }
    // `subquery._none()` / `_none(pred)` — same as `_any`, NOT EXISTS.
    if (fname == "none") {
        if (argc == 1) {
            var binds : array<ExpressionPtr>
            let subSql = analyze_subquery(call.arguments[0], binds, prog, at, false, q.inView)
            if (empty(subSql)) return ""
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            return "NOT EXISTS ({subSql})"
        }
        if (argc == 2) {
            var binds : array<ExpressionPtr>
            let subSql = analyze_subquery_with_pred(call.arguments[0], call.arguments[1], binds, prog, at, q.inView)
            if (empty(subSql)) return ""
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            return "NOT EXISTS ({subSql})"
        }
    }
    // ---- User-defined [sql_function] — emit `<name>(args...)` so SQLite calls the registered UDF.
    if (call.func != null) {
        let info = sql_function_info_from_func(call.func)
        if (!empty(info.name)) {
            if (info.directonly && q.inView) return pred_fail(q, prog, at, "_sql: [sql_function(directonly=true)] '{info.name}' cannot be used in `_create_view` body — SQLite forbids SQLITE_DIRECTONLY functions in views (drop directonly=true, or call the function only from application SQL)")
            let sql = build_string() $(var w) {
                w |> write(info.name)
                w |> write("(")
                for (i in range(argc)) {
                    if (i > 0) {
                        w |> write(", ")
                    }
                    w |> write(pred_to_sql(call.arguments[i], q, prog, at))
                }
                w |> write(")")
            }
            return "" if (q.hadError)
            return sql
        }
    }
    return ""
}

[macro_function]
def private sql_function_info_from_func(func : FunctionPtr) : tuple<name : string; directonly : bool> {
    // Reads `[sql_function]` annotation. Returns ("", false) if not tagged.
    for (ann in func.annotations) {
        if (ann != null && ann.annotation.name == "sql_function") {
            var name = string(func.name)
            let nameArg = ann.arguments |> find_arg("name")
            if (nameArg is tString) {
                name = nameArg as tString
            }
            var directonly = false
            let donArg = ann.arguments |> find_arg("directonly")
            if (donArg is tBool) {
                directonly = donArg as tBool
            }
            return (name = name, directonly = directonly)
        }
    }
    return (name = "", directonly = false)
}

[macro_function]
def private is_none_call(node : ExpressionPtr) : bool {
    if (node == null) return false
    var peeled = node
    if (peeled is ExprRef2Value) {
        peeled = (peeled as ExprRef2Value).subexpr
        if (peeled == null) return false
    }
    if (peeled is ExprCall) {
        let c = peeled as ExprCall
        let cname = string(c.name)
        if (cname == "none") return true
        // Reified template form: contains both "option" and "none".
        return cname |> find("option") >= 0 && cname |> find("none") >= 0
    }
    return false
}

[macro_function]
def private next_bind_placeholder(sql : string) : int {
    return next_placeholder(sql)._0
}

// Walks the SQL string honoring single-quote literals; returns (position, marker).
// Markers: '?' = bind, '\x01' = runtime inline_id (column name supplied at runtime).
[macro_function]
def private next_placeholder(sql : string) : tuple<int; uint8> {
    var found_pos = -1
    var found_ch = uint8(0)
    peek_data(sql) $(bytes) {
        let n = length(bytes)
        var i = 0
        var in_quote = false
        while (i < n) {
            let ch = bytes[i]
            if (ch == uint8('\'')) {
                if (in_quote && i + 1 < n && bytes[i + 1] == uint8('\'')) {
                    i += 2
                    continue
                }
                in_quote = !in_quote
                i++
                continue
            }
            if (!in_quote && (ch == uint8('?') || ch == uint8(0x01))) {
                found_pos = i
                found_ch = ch
                return
            }
            i++
        }
    }
    return (found_pos, found_ch)
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
    // Read [sql_table]/[sql_view]/[sql_fts5] name from struct metadata directly — we can't reach the user's _::_sql_table_name from this module's macro context.
    if (rootT == null || rootT.structType == null) return ""
    var st = rootT.structType
    for (annDecl in st.annotations) {
        if (annDecl.annotation.name == "sql_table"
                || annDecl.annotation.name == "sql_view"
                || annDecl.annotation.name == "sql_fts5") {
            for (arg in annDecl.arguments) {
                if (arg.name == "name" && arg.basicType == Type.tString) return string(arg.sValue)
            }
            break
        }
    }
    return string(st.name)
}

[macro_function]
def private has_struct_annotation(rootT : TypeDeclPtr; name : string) : bool {
    if (rootT == null || rootT.structType == null) return false
    for (annDecl in rootT.structType.annotations) {
        if (annDecl.annotation.name == name) return true
    }
    return false
}

// ===== SQL string assembly =====

[macro_function]
def private build_sql_string(var q : SqlQuery&; force_aliases : bool = false) : string {
    let inner = build_sql_select(q, force_aliases)
    if (q.setOpKind == SetOpKind.None) return inner
    var op : string
    if (q.setOpKind == SetOpKind.Union) {
        op = "UNION"
    } elif (q.setOpKind == SetOpKind.Intersect) {
        op = "INTERSECT"
    } else {
        op = "EXCEPT"
    }
    return "{inner} {op} {q.setOpRhsSql}"
}

[macro_function]
def private build_sql_select(var q : SqlQuery&; force_aliases : bool = false) : string {
    return build_string() $(var w) {
        w |> write("SELECT ")
        if (q.distinctFlag) {
            w |> write("DISTINCT ")
        }
        if (q.proj == ProjectionShape.Aggregate) {
            w |> write(q.aggregateExpr)
        } elif (q.proj == ProjectionShape.GroupedNamedTuple) {
            // nolint:STYLE015 Pre-formatted frags emit verbatim; AS "<alias>" only when force_aliases
            // (multi-Q outer resolution) or when alias differs from natural identifier (renamed projection).
            for (i in range(length(q.groupedSelectExprs))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write(q.groupedSelectExprs[i])
                if (force_aliases && i < length(q.projRecordNames) && !empty(q.projRecordNames[i])
                        && !sql_fragment_is_quoted_ident(q.groupedSelectExprs[i], q.projRecordNames[i])) {
                    w |> write(" AS \"")
                    w |> write(q.projRecordNames[i])
                    w |> write("\"")
                }
            }
        } else {
            for (i in range(length(q.selectCols))) {
                if (i > 0) {
                    w |> write(", ")
                }
                // Emit precedence: SQL fragment > aliased col > unqualified col. Discriminator is array-emptiness, not q.seenJoin.
                let hasFrag = i < length(q.selectColSqlFragments) && !empty(q.selectColSqlFragments[i])
                var natColName = ""
                if (hasFrag) {
                    w |> write(q.selectColSqlFragments[i])
                    // Computed fragment has no natural name; AS will fire (rename != natural).
                } elif (i < length(q.selectColAliases) && !empty(q.selectColAliases[i])) {
                    let owningT = source_rootType_for_alias(q, q.selectColAliases[i])
                    w |> write(column_sql_for_alias(q.selectColAliases[i], owningT, q.selectCols[i]))
                    natColName = field_to_column_name(owningT, q.selectCols[i])
                } else {
                    w |> write(column_sql_for_alias("", q.rootType, q.selectCols[i]))
                    natColName = field_to_column_name(q.rootType, q.selectCols[i])
                }
                if (force_aliases && i < length(q.projRecordNames) && !empty(q.projRecordNames[i])
                        && q.projRecordNames[i] != natColName) {
                    w |> write(" AS \"")
                    w |> write(q.projRecordNames[i])
                    w |> write("\"")
                }
            }
        }
        // FROM clause: alias root only with joins; with innerSql renders `(<innerSql>) AS t0` instead of base-table.
        if (q.seenJoin) {
            // Multi-join LHS subquery: outer `FROM (<innerSql>) AS "t0"` instead of base table.
            if (!empty(q.innerSql)) {
                w |> write(" FROM (")
                w |> write(q.innerSql)
                w |> write(") AS \"")
                w |> write(q.rootAlias)
                w |> write("\"")
            } else {
                w |> write(" FROM \"")
                w |> write(q.tableName)
                w |> write("\" AS \"")
                w |> write(q.rootAlias)
                w |> write("\"")
            }
            for (j in q.joins) {
                // SQLite ≥3.39 (2022) for RIGHT / FULL OUTER; bundled is 3.41.2.
                if (j.kind == JoinKind.Inner) {
                    w |> write(" INNER JOIN ")
                } elif (j.kind == JoinKind.Left) {
                    w |> write(" LEFT JOIN ")
                } elif (j.kind == JoinKind.Right) {
                    w |> write(" RIGHT JOIN ")
                } elif (j.kind == JoinKind.FullOuter) {
                    w |> write(" FULL OUTER JOIN ")
                } else {
                    w |> write(" CROSS JOIN ")
                }
                if (j.tableSqlIsRaw) {
                    // RHS pre-wrapped as `(SELECT * FROM … WHERE …)` for preserved-side joins.
                    w |> write(j.tableName)
                } else {
                    w |> write("\"")
                    w |> write(j.tableName)
                    w |> write("\"")
                }
                w |> write(" AS \"")
                w |> write(j.alias)
                w |> write("\"")
                if (j.kind != JoinKind.Cross) {
                    w |> write(" ON \"")
                    w |> write(j.leftAlias)
                    w |> write("\".\"")
                    // Translate daslang names via @sql_column on each side's owning struct.
                    let leftOwnerT = source_rootType_for_alias(q, j.leftAlias)
                    w |> write(field_to_column_name(leftOwnerT, j.leftCol))
                    w |> write("\" = \"")
                    w |> write(j.alias)
                    w |> write("\".\"")
                    w |> write(field_to_column_name(j.rootType, j.rightCol))
                    w |> write("\"")
                    // Right-side _where in ON clause (LEFT/RIGHT/FULL OUTER JOIN: row-level WHERE filter would drop unmatched rows).
                    if (!empty(j.rightWhereSql)) {
                        w |> write(" AND (")
                        w |> write(j.rightWhereSql)
                        w |> write(")")
                    }
                }
                // Cross joins have no ON / no rightWhereSql (process_cross_join_call hoists into outer WHERE).
            }
        } elif (!empty(q.innerSql)) {
            // Outer wrapping phase-conflict-pushed subquery; inner's default-row projection feeds outer's `_.<field>`.
            w |> write(" FROM (")
            w |> write(q.innerSql)
            w |> write(") AS \"t0\"")
        } else {
            w |> write(" FROM \"")
            w |> write(q.tableName)
            w |> write("\"")
            // nolint:STYLE015 Inner-subquery snapshot wrap: q.whereSql may carry `"t0"."Id"` refs built
            // when rootAlias was set; without AS here they'd reference an undefined alias inside the parens.
            if (force_aliases && !empty(q.rootAlias)) {
                w |> write(" AS \"")
                w |> write(q.rootAlias)
                w |> write("\"")
            }
        }
        if (!empty(q.whereSql)) {
            w |> write(" WHERE ")
            w |> write(q.whereSql)
        }
        // groupByCols hold daslang field names (quoted); translate to SQL identifier here.
        if (!empty(q.groupByCols)) {
            w |> write(" GROUP BY ")
            for (i in range(length(q.groupByCols))) {
                if (i > 0) {
                    w |> write(", ")
                }
                let dasName = group_key_column_name(q, i)
                let sqlCol = field_to_column_name(q.rootType, dasName)
                w |> write("\"")
                w |> write(sqlCol)
                w |> write("\"")
            }
        }
        if (!empty(q.havingSql)) {
            w |> write(" HAVING ")
            w |> write(q.havingSql)
        }
        if (!empty(q.orderByCols)) {
            w |> write(" ORDER BY ")
            for (i in range(length(q.orderByCols))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write(q.orderByCols[i])
            }
        }
        // LIMIT/OFFSET. One/OneOpt force LIMIT 1 (skip for Aggregate — intrinsically 1-row). OFFSET-only needs LIMIT -1.
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

[call_macro(name="_sql_text")]
class private SqlTextMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "_sql_text expects one argument: a chain expression")
        var argT = call.arguments[0]._type
        macro_verify(argT != null, prog, call.at, "_sql_text: chain argument is not inferred yet")
        macro_verify(!argT.isAutoOrAlias, prog, call.at, "_sql_text: chain type is auto/alias after inference")
        var q = SqlQuery()
        if (!analyze_chain(call.arguments[0], q, prog, call.at)) return null
        let sql = build_sql_string(q)
        var noBinds : array<ExpressionPtr>
        var frags : array<SqlFrag>
        sql_to_frags_ex(sql, noBinds, q.orderByInlineExprs, frags)
        var sqlExpr : ExpressionPtr
        var bindExprs : array<ExpressionPtr>
        fold_to_builder(frags, call.at, sqlExpr, bindExprs)
        sqlExpr.force_at(call.at)
        return sqlExpr
    }
}

// ===== _create_view =====

[call_macro(name="_create_view")]
class private SqlCreateViewMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 3, prog, call.at,
            "_create_view expects (db, type<V>, chain) — got {length(call.arguments)} args")
        var viewT : TypeDeclPtr
        var typeE = call.arguments[1]
        if (!qmatch(typeE, type<$t(viewT)>).matched) {
            macro_error(prog, call.at, "_create_view: second argument must be a `type<V>` literal")
            return null
        }
        if (viewT == null || viewT.structType == null) {
            macro_error(prog, call.at, "_create_view: type<V> must be a struct (got {describe(viewT)})")
            return null
        }
        if (!has_struct_annotation(viewT, "sql_view")) {
            macro_error(prog, call.at,
                "_create_view: type<V> must carry [sql_view] — got '{viewT.structType.name}' without [sql_view]. Add `[sql_view(name=\"…\")]` to the struct.")
            return null
        }
        // View name from `[sql_view(name="…")]`, fallback to struct name.
        var view_name = string(viewT.structType.name)
        for (annDecl in viewT.structType.annotations) {
            if (annDecl.annotation.name == "sql_view") {
                for (arg in annDecl.arguments) {
                    if (arg.name == "name" && arg.basicType == Type.tString) {
                        view_name = string(arg.sValue)
                    }
                }
                break
            }
        }
        // Analyze the SELECT chain.
        var argChainT = call.arguments[2]._type
        macro_verify(argChainT != null, prog, call.at, "_create_view: chain argument is not inferred yet")
        macro_verify(!argChainT.isAutoOrAlias, prog, call.at, "_create_view: chain type is auto/alias after inference")
        var q = SqlQuery()
        q.inView = true
        if (!analyze_chain(call.arguments[2], q, prog, call.at)) {
            // nolint:STYLE015 Deeper pred_fail errors get eaten by the AST cascade after call_macro
            // returns null (free `_` refs confuse the typer); re-emit captured first error.
            if (!empty(q.lastError)) {
                macro_error(prog, call.at, q.lastError)
            }
            return null
        }
        if (q.seenToArray) {
            macro_error(prog, call.at, "_create_view: chain ends in `_to_array()` — views are by definition arrays of rows; drop the `_to_array()` terminal.")
            return null
        }
        // CREATE VIEW DDL is run once; baking a runtime column name at view-creation time is surprising. Reject.
        if (!empty(q.orderByInlineExprs)) {
            macro_error(prog, call.at, "_create_view: `_order_by(<runtime string>)` not supported — the runtime column name would be baked in at CREATE VIEW time. Use a compile-time `_.Field` ordering inside the view, or build a chain with `_sql(...)` outside.")
            return null
        }
        // Each bind goes through `_::to_sql_literal(...)`: evaluated once at CREATE VIEW DDL time, inlined into view body.
        var ordered_binds : array<ExpressionPtr>
        collect_query_binds(q, ordered_binds)
        // Validate projection column count vs V's fields.
        let nFields = length(viewT.structType.fields)
        var nProj = 0
        if (q.proj == ProjectionShape.Aggregate) {
            nProj = 1
        } elif (q.proj == ProjectionShape.GroupedNamedTuple) {
            nProj = length(q.groupedSelectExprs)
        } else {
            nProj = length(q.selectCols)
        }
        if (nProj != nFields) {
            macro_error(prog, call.at, "_create_view: column count mismatch — projection has {nProj} columns, [sql_view] '{viewT.structType.name}' has {nFields} fields")
            return null
        }
        // Per-position type check via describe().
        for (i in range(nFields)) {
            var projType : TypeDeclPtr
            if (q.proj == ProjectionShape.Aggregate) {
                projType = q.aggregateType
            } elif (q.proj == ProjectionShape.GroupedNamedTuple) {
                projType = q.groupedSelectTypes[i]
            } elif (i < length(q.selectColTypes) && q.selectColTypes[i] != null) {
                projType = q.selectColTypes[i]
            } else {
                projType = lookup_struct_field_type(q.rootType, q.selectCols[i])
            }
            if (projType == null) {
                macro_error(prog, call.at, "_create_view: cannot resolve type of projected column #{i+1}")
                return null
            }
            let fieldT = viewT.structType.fields[i]._type
            if (fieldT == null) continue
            let projDesc = describe(projType)
            let fieldDesc = describe(fieldT)
            if (projDesc != fieldDesc) {
                macro_error(prog, call.at, "_create_view: column #{i+1} type mismatch — projection emits '{projDesc}', [sql_view] '{viewT.structType.name}.{viewT.structType.fields[i].name}' is '{fieldDesc}'")
                return null
            }
        }
        // Build the column-name list using V's @sql_column-renamed names.
        let col_list = build_string() $(var w) {
            for (i in range(nFields)) {
                if (i > 0) {
                    w |> write(", ")
                }
                var col_name = string(viewT.structType.fields[i].name)
                let col_anno = find_arg(viewT.structType.fields[i].annotation, "sql_column")
                if (col_anno is tString) {
                    col_name = string(col_anno as tString)
                }
                w |> write("\"")
                w |> write(col_name)
                w |> write("\"")
            }
        }
        // nolint:STYLE015 ExprStringBuilder of text frags + `_::to_sql_literal(<bind>)` calls; `_::` resolves
        // at user call site so user-defined to_sql_literal overloads override module defaults.
        let select_sql_template = build_sql_string(q)
        var sb = new ExprStringBuilder(at = call.at)
        var pending = "CREATE VIEW \"{view_name}\"({col_list}) AS "
        var rest = select_sql_template
        var bind_idx = 0
        // macro_function: short-burst string accumulation, flushed into ExprConstString nodes
        while (true) {
            let pos = next_bind_placeholder(rest)
            if (pos < 0) {
                pending += rest // nolint:PERF001
                break
            }
            pending += rest |> slice(0, pos) // nolint:PERF001
            if (!empty(pending)) {
                sb.elements |> push <| new ExprConstString(at = call.at, value := pending)
                pending = ""
            }
            sb.elements |> push <| qmacro(_::to_sql_literal($e(ordered_binds[bind_idx])))
            rest = rest |> slice(pos + 1)
            bind_idx++
        }
        if (!empty(pending)) {
            sb.elements |> push <| new ExprConstString(at = call.at, value := pending)
        }
        // Pipe-rewrite turns `db |> _create_view(...)` into `_create_view(db, ...)` —
        // arg 0 is exactly what the user wrote.
        var dbExpr = clone_expression(call.arguments[0])
        var res = qmacro(_::exec($e(dbExpr), $e(sb)))
        res.force_at(call.at)
        return res
    }
}

// ===== _sql_pragma / _sql_vacuum_into =====

[macro_function]
def private emit_pragma_or_vacuum(prog : ProgramPtr; var call : ExprCallMacro?;
                                  isPragma : bool) : ExpressionPtr {
    let macroName = isPragma ? "_sql_pragma" : "_sql_vacuum_into"
    let argCount = isPragma ? 3 : 2
    let argShape = isPragma ? "(db, name : string, value : string)" : "(db, path : string)"
    if (call.arguments |> length != argCount) {
        macro_error(prog, call.at, "{macroName} expects {argCount} args {argShape} — got {length(call.arguments)}")
        return null
    }
    if (isPragma) {
        let nameT = call.arguments[1]._type
        let valueT = call.arguments[2]._type
        if (nameT == null || nameT.baseType != Type.tString) {
            macro_error(prog, call.at, "_sql_pragma: name must be `string` — got '{describe(nameT)}'")
            return null
        }
        if (valueT == null || valueT.baseType != Type.tString) {
            macro_error(prog, call.at, "_sql_pragma: value must be `string` — for int / bool pragmas use `try_set_pragma` (or `set_pragma`)")
            return null
        }
    } else {
        let pathT = call.arguments[1]._type
        if (pathT == null || pathT.baseType != Type.tString) {
            macro_error(prog, call.at, "_sql_vacuum_into: path must be `string` — got '{describe(pathT)}'")
            return null
        }
    }
    var frags : array<SqlFrag>
    if (isPragma) {
        frags |> push_text("PRAGMA ")
        frags |> push_inline_id(clone_expression(call.arguments[1]))
        frags |> push_text(" = ")
        frags |> push_inline_lit(clone_expression(call.arguments[2]))
    } else {
        frags |> push_text("VACUUM INTO ")
        frags |> push_inline_lit(clone_expression(call.arguments[1]))
    }
    var sqlExpr : ExpressionPtr
    var bindExprs : array<ExpressionPtr>
    fold_to_builder(frags, call.at, sqlExpr, bindExprs)
    var dbExpr = clone_expression(call.arguments[0])
    var res = qmacro(_::try_exec($e(dbExpr), $e(sqlExpr)))
    res.force_at(call.at)
    return res
}

[call_macro(name="_sql_pragma")]
class private SqlPragmaMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        return emit_pragma_or_vacuum(prog, call, true)
    }
}

[call_macro(name="_sql_vacuum_into")]
class private SqlVacuumIntoMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        return emit_pragma_or_vacuum(prog, call, false)
    }
}

// ===== _sql runtime materialization =====

[macro_function]
def private lookup_struct_field_type(rootT : TypeDeclPtr; fieldName : string) : TypeDeclPtr {
    if (rootT == null) return null
    // Named tuple — used by multi-Q outer Q (rootType=null, fromRowType=projected named tuple).
    if (rootT.baseType == Type.tTuple) {
        for (i in range(length(rootT.argNames))) {
            if (rootT.argNames[i] == fieldName && i < length(rootT.argTypes)) return clone_type(rootT.argTypes[i])
        }
        return null
    }
    if (rootT.structType == null) return null
    var st = rootT.structType
    for (field in st.fields) {
        if (field.name == fieldName) return clone_type(field._type)
    }
    return null
}

[macro_function]
def private has_field_annotation(rootT : TypeDeclPtr; fieldName : string; annoName : string) : bool {
    if (rootT == null || rootT.structType == null) return false
    var st = rootT.structType
    for (field in st.fields) {
        if (field.name == fieldName) {
            let v = find_arg(field.annotation, annoName)
            return v is tBool && (v as tBool)
        }
    }
    return false
}

[macro_function]
def private find_field_annotation_string(rootT : TypeDeclPtr; fieldName : string;
                                         annoName : string; defaultV : string) : string {
    if (rootT == null || rootT.structType == null) return defaultV
    var st = rootT.structType
    for (field in st.fields) {
        if (field.name == fieldName) {
            let v = find_arg(field.annotation, annoName)
            if (v is tString) return string(v as tString)
            return defaultV
        }
    }
    return defaultV
}

[macro_function]
def private field_to_column_name(rootT : TypeDeclPtr; fieldName : string) : string {
    return find_field_annotation_string(rootT, fieldName, "sql_column", fieldName)
}

[macro_function]
def private resolve_nested_field_type(rootT : TypeDeclPtr; colName : string; jsonPath : array<string>) : TypeDeclPtr {
    var cur = lookup_struct_field_type(rootT, colName)
    if (cur == null) return null
    for (seg in jsonPath) {
        if (cur == null) return null
        if (cur.baseType == Type.tStructure && cur.structType != null) {
            var found : TypeDeclPtr
            for (field in cur.structType.fields) {
                if (field.name == seg) {
                    found = clone_type(field._type)
                    break
                }
            }
            if (found == null) return null
            cur = found
        } elif (cur.baseType == Type.tTuple && !empty(cur.argNames)) {
            var idx = -1
            for (i in range(length(cur.argNames))) {
                if (cur.argNames[i] == seg) {
                    idx = i
                    break
                }
            }
            if (idx < 0) return null
            cur = clone_type(cur.argTypes[idx])
        } else {
            return null
        }
    }
    return cur
}

[macro_function]
def private build_row_builder(var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : ExpressionPtr {
    if (q.proj == ProjectionShape.Aggregate) {
        let colIdx = 0
        var aggT = clone_type(q.aggregateType)
        return qmacro(read_via_adapter(_sql_stmt, $v(colIdx), type<$t(aggT)>))
    }
    if (q.proj == ProjectionShape.SingleColumn) {
        let colIdx = 0
        var fieldType : TypeDeclPtr
        if (!empty(q.selectColTypes)) {
            fieldType = q.selectColTypes[0]
        } else {
            fieldType = lookup_struct_field_type(q.rootType, q.selectCols[0])
        }
        if (fieldType == null) {
            macro_error(prog, at, "_sql: cannot resolve type of projected column '{q.selectCols[0]}'")
            return null
        }
        return qmacro(read_via_adapter(_sql_stmt, $v(colIdx), type<$t(fieldType)>))
    }
    if (q.proj == ProjectionShape.NamedTuple) {
        // (Name=read_via_adapter(stmt,0,type<…>), …); recordNames go on tupT.argNames via recordType.
        let n = length(q.selectCols)
        var tupT = new TypeDecl(at = at, baseType = Type.tTuple)
        tupT.argNames.resize(n)
        var mkTup = new ExprMakeTuple(at = at)
        for (i in range(n)) {
            let colIdx = i
            var fieldType : TypeDeclPtr
            if (i < length(q.selectColTypes)) {
                fieldType = q.selectColTypes[i]
            } else {
                fieldType = lookup_struct_field_type(q.rootType, q.selectCols[i])
            }
            if (fieldType == null) {
                macro_error(prog, at, "_sql: cannot resolve type of projected column '{q.selectCols[i]}'")
                return null
            }
            var readExpr = qmacro(read_via_adapter(_sql_stmt, $v(colIdx), type<$t(fieldType)>))
            emplace(mkTup.values, readExpr)
            tupT.argTypes |> emplace_new(clone_type(fieldType))
            tupT.argNames[i] := q.projRecordNames[i]
        }
        mkTup.recordType = tupT
        return mkTup
    }
    if (q.proj == ProjectionShape.GroupedNamedTuple) {
        // Same shape as NamedTuple; column types come from groupedSelectTypes.
        let n = length(q.groupedSelectExprs)
        var tupT = new TypeDecl(at = at, baseType = Type.tTuple)
        tupT.argNames.resize(n)
        var mkTup = new ExprMakeTuple(at = at)
        for (i in range(n)) {
            let colIdx = i
            var fieldType = clone_type(q.groupedSelectTypes[i])
            var readExpr = qmacro(read_via_adapter(_sql_stmt, $v(colIdx), type<$t(fieldType)>))
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
    var argT = call.arguments[0]._type
    macro_verify(argT != null, prog, call.at, "_sql/_try_sql: chain argument is not inferred yet")
    macro_verify(!argT.isAutoOrAlias, prog, call.at, "_sql/_try_sql: chain type is auto/alias after inference")
    var q = SqlQuery()
    if (!analyze_chain(call.arguments[0], q, prog, call.at)) return null
    // take(n)/skip(n) before an aggregate is degenerate in SQL — aggregate collapses to one row, LIMIT/OFFSET no-op; reject loudly.
    if (q.proj == ProjectionShape.Aggregate
            && (q.limitBindExpr != null || q.offsetBindExpr != null)) {
        macro_error(prog, call.at, "_sql: take(n)/skip(n) before an aggregate is not translatable — `LIMIT`/`OFFSET` after an aggregate has no effect (aggregate collapses to one row); use `_where(...)` for pre-aggregate filtering")
        return null
    }
    // distinct() upstream of an aggregate is a no-op in SQL — meaningful form is COUNT(DISTINCT col), not yet implemented.
    if (q.proj == ProjectionShape.Aggregate && q.distinctFlag) {
        macro_error(prog, call.at, "_sql: distinct() before an aggregate is not translatable — `SELECT DISTINCT` over a single-row aggregate result is always a no-op; the meaningful form is `COUNT(DISTINCT col)` etc. (aggregate-distinct), which is not yet implemented")
        return null
    }
    // Bind order (centralized in collect_query_binds): inner-Q FROM → per-join ON → WHERE → HAVING → set-op-RHS → LIMIT → OFFSET.
    var allBinds : array<ExpressionPtr>
    collect_query_binds(q, allBinds)
    let sql = build_sql_string(q)
    // Convert (sqlString, allBinds) → ExprStringBuilder + binds; const-fold keeps AOT byte-identical to the legacy $v(sql) path.
    var frags : array<SqlFrag>
    sql_to_frags_ex(sql, allBinds, q.orderByInlineExprs, frags)
    var sqlExpr : ExpressionPtr
    var bindExprs : array<ExpressionPtr>
    fold_to_builder(frags, call.at, sqlExpr, bindExprs)
    // Build bind statements: sql_bind_to_stmt(stmt, i+1, $e(boundExpr))
    var bind_stmts : array<ExpressionPtr>
    bind_stmts |> reserve(length(bindExprs))
    for (i in range(length(bindExprs))) {
        let bindIdx = i + 1
        var be = bindExprs[i]
        bind_stmts |> push <| qmacro_expr(${
            sql_bind_to_stmt(_sql_stmt, $v(bindIdx), $e(be));
        })
    }
    var rowBuilder = build_row_builder(q, prog, call.at)
    if (rowBuilder == null) return null
    var dbExpr = q.dbExpr
    let suffix = (q.matr == Materializer.One ? "_one"
                  : (q.matr == Materializer.OneOpt ? "_one_opt" : ""))
    let fname = (isTry ? "try_run_select" : "run_select") + suffix
    var res = qmacro($c(fname)($e(dbExpr), $e(sqlExpr),
        $(var _sql_stmt : sqlite3_stmt?) {
            $b(bind_stmts)
        },
        $(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
    res.force_at(call.at)
    return res
}

[call_macro(name="_sql")]
class private SqlMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "_sql expects one argument: a chain expression")
        return emit_sql_macro_body(prog, call, false)
    }
}

[call_macro(name="_try_sql")]
class private TrySqlMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "_try_sql expects one argument: a chain expression")
        return emit_sql_macro_body(prog, call, true)
    }
}

[macro_function]
def private emit_each_sql_macro_body(prog : ProgramPtr; var call : ExprCallMacro?) : ExpressionPtr {
    var argT = call.arguments[0]._type
    macro_verify(argT != null, prog, call.at, "_each_sql: chain argument is not inferred yet")
    macro_verify(!argT.isAutoOrAlias, prog, call.at, "_each_sql: chain type is auto/alias after inference")
    var q = SqlQuery()
    if (!analyze_chain(call.arguments[0], q, prog, call.at)) return null
    // Reject terminals (iterator, not scalar/row/array). Order matters: aggregates set both Aggregate+One, so dispatch on proj first.
    if (q.seenToArray) {
        macro_error(prog, call.at, "_each_sql: chain ends in `_to_array()` — _each_sql streams rows lazily; drop the terminal, or switch to `_sql(...)` for materialization")
        return null
    }
    if (q.proj == ProjectionShape.Aggregate) {
        macro_error(prog, call.at, "_each_sql: chain ends in an aggregate (_count/_sum/_avg/_min/_max) — aggregates collapse to one scalar; iteration is degenerate. Use `_sql(...)` for the scalar value, or drop the aggregate to iterate over rows.")
        return null
    }
    if (q.matr == Materializer.One || q.matr == Materializer.OneOpt) {
        macro_error(prog, call.at, "_each_sql: chain ends in `_first()` / `_first_opt()` — those terminals materialize a single row. Use `_sql(...)` for the same chain, or drop the terminal to iterate.")
        return null
    }
    // Same bind ordering as emit_sql_macro_body via collect_query_binds. take/skip binds OK here (lazy stream).
    var allBinds : array<ExpressionPtr>
    collect_query_binds(q, allBinds)
    let sql = build_sql_string(q)
    var frags : array<SqlFrag>
    sql_to_frags_ex(sql, allBinds, q.orderByInlineExprs, frags)
    var sqlExpr : ExpressionPtr
    var bindExprs : array<ExpressionPtr>
    fold_to_builder(frags, call.at, sqlExpr, bindExprs)
    var bind_stmts : array<ExpressionPtr>
    bind_stmts |> reserve(length(bindExprs))
    for (i in range(length(bindExprs))) {
        let bindIdx = i + 1
        var be = bindExprs[i]
        bind_stmts |> push <| qmacro_expr(${
            sql_bind_to_stmt(_sql_stmt, $v(bindIdx), $e(be));
        })
    }
    var rowBuilder = build_row_builder(q, prog, call.at)
    if (rowBuilder == null) return null
    var dbExpr = q.dbExpr
    // bind_blk = block (one-shot, before the generator); row_blk = capturing lambda (outlives run_each_select).
    var res = qmacro(_::run_each_select($e(dbExpr), $e(sqlExpr),
        $(var _sql_stmt : sqlite3_stmt?) {
            $b(bind_stmts)
        },
        @(_sql_stmt : sqlite3_stmt?) => $e(rowBuilder)))
    res.force_at(call.at)
    return res
}

[call_macro(name="_each_sql")]
class private EachSqlMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at, "_each_sql expects one argument: a chain expression")
        return emit_each_sql_macro_body(prog, call)
    }
}

// ===== _sql_update / _sql_delete — predicate-based mutations =====

[macro_function]
def private analyze_set_clause(var setExpr : ExpressionPtr; var q : SqlQuery&;
                               var setSqlClauses : array<string>&;
                               var setBindExprs : array<ExpressionPtr>&;
                               prog : ProgramPtr; at : LineInfo) : bool {
    if (setExpr == null) {
        macro_error(prog, at, "_sql_update: set clause is null")
        return false
    }
    var body = setExpr
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
    }
    if (!(body is ExprMakeTuple)) {
        macro_error(prog, at, "_sql_update: set clause must be a named-tuple literal `(Col1=expr1, Col2=expr2, ...)`; got: {describe(body)}")
        return false
    }
    var mkTup = body as ExprMakeTuple
    if (empty(mkTup.values)) {
        macro_error(prog, at, "_sql_update: set clause is empty — UPDATE with no columns is invalid SQL. Pass at least one (Col=expr) pair.")
        return false
    }
    // recordNames may live on tupT.argNames (typed) or mkTup.recordNames (raw, canVisitArgument=false).
    var tupT = mkTup._type
    var recordNames : array<string>
    if (tupT != null && tupT.baseType == Type.tTuple
            && length(tupT.argNames) == length(mkTup.values)) {
        recordNames |> reserve(length(tupT.argNames))
        for (i in range(length(tupT.argNames))) {
            recordNames |> push(string(tupT.argNames[i]))
        }
    } elif (length(mkTup.recordNames) == length(mkTup.values)) {
        recordNames |> reserve(length(mkTup.recordNames))
        for (i in range(length(mkTup.recordNames))) {
            recordNames |> push(string(mkTup.recordNames[i]))
        }
    } else {
        macro_error(prog, at, "_sql_update: set clause must be a NAMED-tuple literal `(Col=expr, ...)`; could not extract per-element column names")
        return false
    }
    for (i in range(length(recordNames))) {
        if (empty(recordNames[i])) {
            macro_error(prog, at, "_sql_update: set clause: positional element at index {i} has no field name; use `(Col=expr, ...)` form")
            return false
        }
    }
    // Validate each SET column against T's fields (no typos, no duplicates).
    if (q.rootType != null && q.rootType.structType != null) {
        var st = q.rootType.structType
        var seenCols : table<string; bool>
        for (colName in recordNames) {
            if (key_exists(seenCols, colName)) {
                macro_error(prog, at, "_sql_update: duplicate SET column \"{colName}\"")
                return false
            }
            seenCols[colName] = true
            var found = false
            for (f in st.fields) {
                if (f.name == colName) {
                    found = true
                    break
                }
            }
            if (!found) {
                macro_error(prog, at, "_sql_update: SET column \"{colName}\" is not a field of {st.name}")
                return false
            }
        }
    }
    // Translate via pred_to_sql; binds collected SET-local for proper interleave with WHERE.
    var setQ = SqlQuery(
        rootType = q.rootType,
        tableName = q.tableName,
        argNames <- ["_"],
        argSourceIdx <- [0]
    )
    setSqlClauses |> reserve(length(setSqlClauses) + length(mkTup.values))
    for (i in range(length(mkTup.values))) {
        let colName = recordNames[i]
        let sqlCol = field_to_column_name(q.rootType, colName)
        var val = mkTup.values[i]
        let valSql = pred_to_sql(val, setQ, prog, at)
        // pred_to_sql emitted macro_error, OR returned "" without setting hadError
        if (setQ.hadError || empty(valSql)) return false
        setSqlClauses |> push("\"{sqlCol}\" = {valSql}")
    }
    setBindExprs |> reserve(length(setBindExprs) + length(setQ.bindExprs))
    for (b in setQ.bindExprs) {
        setBindExprs |> push(b)
    }
    return true
}

[macro_function]
def private build_returning_clause(rootT : TypeDeclPtr) : string {
    if (rootT == null || rootT.structType == null) return ""
    var st = rootT.structType
    return build_string() $(var w) {
        w |> write(" RETURNING ")
        var first = true
        for (field in st.fields) {
            if (!first) {
                w |> write(", ")
            }
            w |> write("\"")
            w |> write(field_to_column_name(rootT, string(field.name)))
            w |> write("\"")
            first = false
        }
    }
}

[macro_function]
def private emit_dml_call(var dbE : ExpressionPtr; var sqlExpr : ExpressionPtr;
                          var bindStmts : array<ExpressionPtr>;
                          isTry : bool; isReturning : bool;
                          rootT : TypeDeclPtr; at : LineInfo) : ExpressionPtr {
    var bindLambda = qmacro($(var _sql_stmt : sqlite3_stmt?) {
        $b(bindStmts)
    })
    var res : ExpressionPtr
    if (isReturning) {
        var rootClone = clone_type(rootT)
        var readerLambda = qmacro($(_sql_stmt : sqlite3_stmt?) => _::_sql_read_row(_sql_stmt, type<$t(rootClone)>))
        let fname = (isTry ? "try_run_dml_returning" : "run_dml_returning")
        res = qmacro($c(fname)($e(dbE), $e(sqlExpr), $e(bindLambda), $e(readerLambda)))
    } else {
        let fname = (isTry ? "try_run_dml" : "run_dml")
        res = qmacro($c(fname)($e(dbE), $e(sqlExpr), $e(bindLambda)))
    }
    res |> force_at(at)
    return res
}

[macro_function]
def private emit_update_body(prog : ProgramPtr; var call : ExprCallMacro?;
                             var rootT : TypeDeclPtr;
                             isTry : bool; isReturning : bool) : ExpressionPtr {
    if (rootT == null || rootT.structType == null) {
        macro_error(prog, call.at, "_sql_update: T must be a struct (got {describe(rootT)})")
        return null
    }
    var dbE = call.arguments[0]
    var whereE = call.arguments[2]
    var setE = call.arguments[3]
    let tableName = sql_table_name_from_type(rootT)
    var q = SqlQuery(
        rootType = rootT,
        tableName = tableName,
        argNames <- ["_"],
        argSourceIdx <- [0]
    )
    // Translate WHERE — raw AST. pred_to_sql emitted macro_error, OR returned "" without setting hadError.
    let whereSql = pred_to_sql(whereE, q, prog, call.at)
    if (q.hadError || empty(whereSql)) return null
    // Translate SET — raw named-tuple literal.
    var setSqlClauses : array<string>
    var setBindExprs : array<ExpressionPtr>
    if (!analyze_set_clause(setE, q, setSqlClauses, setBindExprs, prog, call.at)) return null
    // Build SQL.
    var sql = build_string() $(var w) {
        w |> write("UPDATE \"")
        w |> write(tableName)
        w |> write("\" SET ")
        for (i in range(length(setSqlClauses))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write(setSqlClauses[i])
        }
        w |> write(" WHERE ")
        w |> write(whereSql)
    }
    if (isReturning) {
        sql += build_returning_clause(rootT)
    }
    // Combine binds: SET first, then WHERE.
    var allBinds : array<ExpressionPtr>
    allBinds |> reserve(length(setBindExprs) + length(q.bindExprs))
    for (b in setBindExprs) {
        allBinds |> push(b)
    }
    for (b in q.bindExprs) {
        allBinds |> push(b)
    }
    var frags : array<SqlFrag>
    sql_to_frags(sql, allBinds, frags)
    var sqlExpr : ExpressionPtr
    var bindExprs : array<ExpressionPtr>
    fold_to_builder(frags, call.at, sqlExpr, bindExprs)
    var bindStmts : array<ExpressionPtr>
    bindStmts |> reserve(length(bindExprs))
    for (i in range(length(bindExprs))) {
        let bindIdx = i + 1
        var be = bindExprs[i]
        bindStmts |> push <| qmacro_expr(${
            sql_bind_to_stmt(_sql_stmt, $v(bindIdx), $e(be));
        })
    }
    return emit_dml_call(dbE, sqlExpr, bindStmts, isTry, isReturning, rootT, call.at)
}

[macro_function]
def private emit_delete_body(prog : ProgramPtr; var call : ExprCallMacro?;
                             var rootT : TypeDeclPtr;
                             isTry : bool; isReturning : bool) : ExpressionPtr {
    if (rootT == null || rootT.structType == null) {
        macro_error(prog, call.at, "_sql_delete: T must be a struct (got {describe(rootT)})")
        return null
    }
    var dbE = call.arguments[0]
    var whereE = call.arguments[2]
    let tableName = sql_table_name_from_type(rootT)
    var q = SqlQuery(
        rootType = rootT,
        tableName = tableName,
        argNames <- ["_"],
        argSourceIdx <- [0]
    )
    // pred_to_sql emitted macro_error, OR returned "" without setting hadError.
    let whereSql = pred_to_sql(whereE, q, prog, call.at)
    if (q.hadError || empty(whereSql)) return null
    var sql = "DELETE FROM \"{tableName}\" WHERE {whereSql}"
    if (isReturning) {
        sql += build_returning_clause(rootT)
    }
    var frags : array<SqlFrag>
    sql_to_frags(sql, q.bindExprs, frags)
    var sqlExpr : ExpressionPtr
    var bindExprs : array<ExpressionPtr>
    fold_to_builder(frags, call.at, sqlExpr, bindExprs)
    var bindStmts : array<ExpressionPtr>
    bindStmts |> reserve(length(bindExprs))
    for (i in range(length(bindExprs))) {
        let bindIdx = i + 1
        var be = bindExprs[i]
        bindStmts |> push <| qmacro_expr(${
            sql_bind_to_stmt(_sql_stmt, $v(bindIdx), $e(be));
        })
    }
    return emit_dml_call(dbE, sqlExpr, bindStmts, isTry, isReturning, rootT, call.at)
}

// ----------------------------------------------------------------------------
// Outer macros — args 1+ stay raw via canVisitArgument so `_` is unbound for pred_to_sql.
// ----------------------------------------------------------------------------

[macro_function]
def private validate_outer_args(prog : ProgramPtr; var call : ExprCallMacro?; var rootT : TypeDeclPtr&;
                                expected_arity : int; sig_text : string) : bool {
    if (length(call.arguments) != expected_arity) {
        macro_error(prog, call.at, "{call.name}: expected {sig_text} — got {length(call.arguments)} args")
        return false
    }
    var typeE = call.arguments[1]
    if (!qmatch(typeE, type<$t(rootT)>).matched) {
        macro_error(prog, call.at, "{call.name}: second argument must be a `type<T>` literal")
        return false
    }
    if (has_struct_annotation(rootT, "sql_view")) {
        macro_error(prog, call.at, "{call.name}: cannot mutate [sql_view] '{rootT.structType.name}' — views are read-only. Mutate the underlying table(s) and re-query the view.")
        return false
    }
    return true
}

class private OuterArgZeroMacro : AstCallMacro {
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
}

class private OuterArgTwoMacro : AstCallMacro {
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex < 2
    }
}

[call_macro(name="_sql_update")]
class private SqlUpdateMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 4, "(db, type<T>, where_expr, set_expr)")) return null
        return emit_update_body(prog, call, rootT, false, false)
    }
}

[call_macro(name="_sql_try_update")]
class private SqlTryUpdateMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 4, "(db, type<T>, where_expr, set_expr)")) return null
        return emit_update_body(prog, call, rootT, true, false)
    }
}

[call_macro(name="_sql_update_returning")]
class private SqlUpdateReturningMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 4, "(db, type<T>, where_expr, set_expr)")) return null
        return emit_update_body(prog, call, rootT, false, true)
    }
}

[call_macro(name="_sql_try_update_returning")]
class private SqlTryUpdateReturningMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 4, "(db, type<T>, where_expr, set_expr)")) return null
        return emit_update_body(prog, call, rootT, true, true)
    }
}

[call_macro(name="_sql_delete")]
class private SqlDeleteMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 3, "(db, type<T>, where_expr)")) return null
        return emit_delete_body(prog, call, rootT, false, false)
    }
}

[call_macro(name="_sql_try_delete")]
class private SqlTryDeleteMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 3, "(db, type<T>, where_expr)")) return null
        return emit_delete_body(prog, call, rootT, true, false)
    }
}

[call_macro(name="_sql_delete_returning")]
class private SqlDeleteReturningMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 3, "(db, type<T>, where_expr)")) return null
        return emit_delete_body(prog, call, rootT, false, true)
    }
}

[call_macro(name="_sql_try_delete_returning")]
class private SqlTryDeleteReturningMacro : OuterArgZeroMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_args(prog, call, rootT, 3, "(db, type<T>, where_expr)")) return null
        return emit_delete_body(prog, call, rootT, true, true)
    }
}

// ===== _sql_upsert family (INSERT...ON CONFLICT DO UPDATE) =====

[macro_function]
def private is_field_computed(field) : bool {
    let v = find_arg(field.annotation, "sql_computed")
    if (v is tString) return !empty(v as tString)
    return false
}

[macro_function]
def private validate_outer_upsert_args(prog : ProgramPtr; var call : ExprCallMacro?; var rootT : TypeDeclPtr&) : bool {
    if (length(call.arguments) != 4) {
        macro_error(prog, call.at, "{call.name}: expected (db, row, on_conflict, do_update) — got {length(call.arguments)} args")
        return false
    }
    var rowE = call.arguments[1]
    if (rowE._type == null || rowE._type.structType == null) {
        macro_error(prog, call.at, "{call.name}: second argument must be a [sql_table] struct value (got {describe(rowE._type)})")
        return false
    }
    if (rowE._type.isGoodArrayType) {
        macro_error(prog, call.at, "{call.name}: bulk array<T> upsert is deferred to a follow-up — pass a single row for now and call inside a transaction loop if you need batching")
        return false
    }
    if (has_struct_annotation(rowE._type, "sql_view")) {
        macro_error(prog, call.at, "{call.name}: cannot upsert into [sql_view] '{rowE._type.structType.name}' — views are read-only. Mutate the underlying table(s) and re-query the view.")
        return false
    }
    rootT = clone_type(rowE._type)
    return true
}

[macro_function]
def private analyze_upsert_conflict(var conflictExpr : ExprCallMacro?; rootT : TypeDeclPtr;
                                    var conflictCols : array<string>&;
                                    prog : ProgramPtr; at : LineInfo) : bool {
    var node : ExpressionPtr = conflictExpr
    return analyze_upsert_conflict_inner(node, rootT, conflictCols, prog, at)
}

[macro_function]
def private analyze_upsert_conflict_inner(var node : ExpressionPtr; rootT : TypeDeclPtr;
                                          var conflictCols : array<string>&;
                                          prog : ProgramPtr; at : LineInfo) : bool {
    if (node == null) {
        macro_error(prog, at, "_sql_upsert: on_conflict is null")
        return false
    }
    // Single column form: `_.Col`.
    {
        var col : string
        if (qmatch(node, _.$f(col)).matched) {
            conflictCols |> push(col)
            return validate_conflict_cols(rootT, conflictCols, prog, at)
        }
    }
    var n = node
    if (n is ExprRef2Value) {
        n = (n as ExprRef2Value).subexpr
    }
    if (n is ExprField) {
        // ExprField but receiver isn't `_`.
        macro_error(prog, at, "_sql_upsert: on_conflict column ref must be `_.Col`; got {describe(node)}")
        return false
    }
    // Tuple form: `tuple(_.A, _.B, ...)` parses as ExprMakeTuple.
    if (n is ExprMakeTuple) {
        var mkt = n as ExprMakeTuple
        if (empty(mkt.values)) {
            macro_error(prog, at, "_sql_upsert: on_conflict tuple is empty")
            return false
        }
        for (idx in range(length(mkt.values))) {
            var sm = mkt.values[idx]
            var col : string
            if (qmatch(sm, _.$f(col)).matched) {
                conflictCols |> push(col)
                continue
            }
            macro_error(prog, at, "_sql_upsert: on_conflict tuple elements must be `_.Col`; got {describe(mkt.values[idx])}")
            return false
        }
        return validate_conflict_cols(rootT, conflictCols, prog, at)
    }
    macro_error(prog, at, "_sql_upsert: on_conflict must be `_.Col` (single) or `tuple(_.C1, _.C2, ...)` (composite); got {describe(node)}")
    return false
}

[macro_function]
def private validate_conflict_cols(rootT : TypeDeclPtr; conflictCols : array<string>; prog : ProgramPtr; at : LineInfo) : bool {
    if (rootT == null || rootT.structType == null) {
        macro_error(prog, at, "_sql_upsert: row type has no struct (got {describe(rootT)})")
        return false
    }
    var st = rootT.structType
    for (col in conflictCols) {
        var found = false
        for (f in st.fields) {
            if (f.name == col) {
                found = true
            }
        }
        if (!found) {
            macro_error(prog, at, "_sql_upsert: on_conflict column \"{col}\" is not a field of {st.name}")
            return false
        }
    }
    return true
}

[macro_function]
def private analyze_upsert_set_clause(var setExpr : ExpressionPtr; var rootT : TypeDeclPtr; tableName : string;
                                      var setSqlClauses : array<string>&;
                                      var setBindExprs : array<ExpressionPtr>&;
                                      prog : ProgramPtr; at : LineInfo) : bool {
    if (setExpr == null) {
        macro_error(prog, at, "_sql_upsert: do_update is null")
        return false
    }
    var body = setExpr
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
    }
    if (!(body is ExprMakeTuple)) {
        macro_error(prog, at, "_sql_upsert: do_update must be a named-tuple literal `(Col=expr, ...)`; got: {describe(body)}")
        return false
    }
    var mkTup = body as ExprMakeTuple
    if (empty(mkTup.values)) {
        macro_error(prog, at, "_sql_upsert: do_update is empty — pass at least one (Col=expr) pair")
        return false
    }
    var tupT = mkTup._type
    var recordNames : array<string>
    if (tupT != null && tupT.baseType == Type.tTuple
            && length(tupT.argNames) == length(mkTup.values)) {
        recordNames <- [for (n in tupT.argNames); string(n)]
    } elif (length(mkTup.recordNames) == length(mkTup.values)) {
        recordNames <- [for (n in mkTup.recordNames); string(n)]
    } else {
        macro_error(prog, at, "_sql_upsert: do_update must be a NAMED-tuple literal `(Col=expr, ...)`; could not extract per-element column names")
        return false
    }
    for (i in range(length(recordNames))) {
        if (empty(recordNames[i])) {
            macro_error(prog, at, "_sql_upsert: do_update positional element at index {i} has no field name; use `(Col=expr, ...)` form")
            return false
        }
    }
    if (rootT != null && rootT.structType != null) {
        var st = rootT.structType
        var seenCols : table<string; bool>
        for (colName in recordNames) {
            if (key_exists(seenCols, colName)) {
                macro_error(prog, at, "_sql_upsert: duplicate do_update column \"{colName}\"")
                return false
            }
            seenCols[colName] = true
            var found = false
            var is_computed = false
            for (f in st.fields) {
                if (f.name == colName) {
                    found = true
                    if (is_field_computed(f)) {
                        is_computed = true
                    }
                    break
                }
            }
            if (!found) {
                macro_error(prog, at, "_sql_upsert: do_update column \"{colName}\" is not a field of {st.name}")
                return false
            }
            if (is_computed) {
                macro_error(prog, at, "_sql_upsert: do_update column \"{colName}\" is @sql_computed; SQLite rejects writes to generated columns. Update the source columns and the computed value follows.")
                return false
            }
        }
    }
    var setQ = SqlQuery(
        rootType = rootT,
        tableName = tableName,
        argNames <- ["_"],
        argSourceIdx <- [0],
        upsertMode = true
    )
    setSqlClauses |> reserve(length(setSqlClauses) + length(mkTup.values))
    for (i in range(length(mkTup.values))) {
        let colName = recordNames[i]
        let sqlCol = field_to_column_name(rootT, colName)
        var val = mkTup.values[i]
        let valSql = pred_to_sql(val, setQ, prog, at)
        // pred_to_sql emitted macro_error, OR returned "" without setting hadError
        if (setQ.hadError || empty(valSql)) return false
        setSqlClauses |> push("\"{sqlCol}\" = {valSql}")
    }
    setBindExprs |> reserve(length(setBindExprs) + length(setQ.bindExprs))
    for (b in setQ.bindExprs) {
        setBindExprs |> push(b)
    }
    return true
}

[macro_function]
def private emit_upsert_body(prog : ProgramPtr; var call : ExprCallMacro?;
                             var rootT : TypeDeclPtr;
                             isTry : bool; isReturning : bool) : ExpressionPtr {
    if (rootT == null || rootT.structType == null) {
        macro_error(prog, call.at, "_sql_upsert: row type must be a [sql_table] struct (got {describe(rootT)})")
        return null
    }
    var st = rootT.structType
    var dbE = call.arguments[0]
    var rowE = call.arguments[1]
    var conflictE = call.arguments[2]
    var doUpdateE = call.arguments[3]
    let tableName = sql_table_name_from_type(rootT)
    // 1) on_conflict columns
    var conflictCols : array<string>
    if (!analyze_upsert_conflict_inner(conflictE, rootT, conflictCols, prog, call.at)) return null
    // 2) do_update SET clauses (with upsertMode=true so `_excluded.Col` resolves)
    var setSqlClauses : array<string>
    var setBindExprs : array<ExpressionPtr>
    if (!analyze_upsert_set_clause(doUpdateE, rootT, tableName, setSqlClauses, setBindExprs, prog, call.at)) return null
    // 3) INSERT column list (non-computed only) — SQL-side names, positional bind indices.
    var nonComputedCols : array<string>
    var nRowBinds = 0
    for (f in st.fields) {
        if (is_field_computed(f)) continue
        nonComputedCols |> push(field_to_column_name(rootT, string(f.name)))
        nRowBinds++
    }
    // 4) Assemble SQL. conflictCols arrive in daslang names — translate via field_to_column_name at emission.
    var sql = build_string() $(var w) {
        w |> write("INSERT INTO \"{tableName}\" (")
        for (i in range(length(nonComputedCols))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write("\"{nonComputedCols[i]}\"")
        }
        w |> write(") VALUES (")
        for (i in range(length(nonComputedCols))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write("?")
        }
        w |> write(") ON CONFLICT(")
        for (i in range(length(conflictCols))) {
            if (i > 0) {
                w |> write(", ")
            }
            let sqlCfCol = field_to_column_name(rootT, conflictCols[i])
            w |> write("\"{sqlCfCol}\"")
        }
        w |> write(") DO UPDATE SET ")
        for (i in range(length(setSqlClauses))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write(setSqlClauses[i])
        }
    }
    if (isReturning) {
        sql += build_returning_clause(rootT)
    }
    // 5) Bind order: row binds (1..N_row) via `_sql_bind_row`, then SET binds (N_row+1..).
    var rowClone = clone_expression(rowE)
    var bindStmts : array<ExpressionPtr>
    bindStmts |> reserve(1 + length(setBindExprs))
    bindStmts |> push <| qmacro_expr(${
        _::_sql_bind_row(_sql_stmt, $e(rowClone), true);
    })
    for (i in range(length(setBindExprs))) {
        let bindIdx = nRowBinds + i + 1
        var be = setBindExprs[i]
        bindStmts |> push <| qmacro_expr(${
            sql_bind_to_stmt(_sql_stmt, $v(bindIdx), $e(be));
        })
    }
    // Row binds come from `_sql_bind_row` (not enumerated as `?` walking targets); pass empty binds to fold.
    var noBinds : array<ExpressionPtr>
    var frags : array<SqlFrag>
    sql_to_frags(sql, noBinds, frags)
    var sqlExpr : ExpressionPtr
    var bindExprs : array<ExpressionPtr>
    fold_to_builder(frags, call.at, sqlExpr, bindExprs)
    return emit_dml_call(dbE, sqlExpr, bindStmts, isTry, isReturning, rootT, call.at)
}

[call_macro(name="_sql_upsert")]
class private SqlUpsertMacro : OuterArgTwoMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_upsert_args(prog, call, rootT)) return null
        return emit_upsert_body(prog, call, rootT, false, false)
    }
}

[call_macro(name="_sql_try_upsert")]
class private SqlTryUpsertMacro : OuterArgTwoMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_upsert_args(prog, call, rootT)) return null
        return emit_upsert_body(prog, call, rootT, true, false)
    }
}

[call_macro(name="_sql_upsert_returning")]
class private SqlUpsertReturningMacro : OuterArgTwoMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_upsert_args(prog, call, rootT)) return null
        return emit_upsert_body(prog, call, rootT, false, true)
    }
}

[call_macro(name="_sql_try_upsert_returning")]
class private SqlTryUpsertReturningMacro : OuterArgTwoMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_upsert_args(prog, call, rootT)) return null
        return emit_upsert_body(prog, call, rootT, true, true)
    }
}
