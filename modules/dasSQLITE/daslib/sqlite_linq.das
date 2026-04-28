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

enum private JoinKind {
    Inner   //!< INNER JOIN — `_join` macro
    Left    //!< LEFT JOIN — `_left_join` macro; right-side row is `Option<TB>`
}

enum private SetOpKind {
    None       //!< no set operation in the chain
    // Compound queries are emitted WITHOUT surrounding parens — SQLite parses
    // a per-side `ORDER BY` / `LIMIT` / `OFFSET` as a syntax error in that
    // shape, which is why chunk 5 rejects those stages on either side. Per-
    // side parens (and the corresponding compound-trailing-clause support)
    // are deferred to a future chunk.
    Union      //!< `union(a, b)`     → `<a> UNION <b>`
    Intersect  //!< `intersect(a, b)` → `<a> INTERSECT <b>`
    Except     //!< `except(a, b)`    → `<a> EXCEPT <b>`
}

struct private JoinSpec {
    //! Single `_join` / `_left_join` recognized by chunk 5: the root source
    //! is alias `t0`, the joined source is alias `t1`. `process_join_call`
    //! rejects a second join in the same chain (3+ table joins are deferred
    //! to a future chunk). The `joins : array<JoinSpec>` field on `SqlQuery`
    //! holds the future shape — chunk 5 fills at most one entry.
    kind        : JoinKind
    rootType    : TypeDeclPtr     //!< right-side row type
    tableName   : string          //!< right-side table name (unquoted)
    alias       : string          //!< right-side alias (currently always `t1`)
    leftCol     : string          //!< left side of equi-join `on` (field name on the LEFT source)
    leftAlias   : string          //!< alias of the LEFT source (currently always `t0`)
    rightCol    : string          //!< right side of equi-join `on` (field name on the JOINED source)
    // Right-side `_where` filter — emitted into the JOIN's `ON` clause
    // (NOT the outer WHERE) as `... ON t0.k = t1.k AND (rightWhereSql)`.
    // Required for `LEFT JOIN`: a per-row predicate in the outer WHERE
    // would filter out NULL-extended unmatched rows and silently turn
    // LEFT JOIN into INNER JOIN. INNER JOIN uses the same ON-clause path
    // for consistency (semantically equivalent to a WHERE merge for INNER,
    // but more idiomatic SQL and stable across query planners).
    rightWhereSql     : string
    // Right-side bind exprs are stored per-JoinSpec rather than appended
    // to outer q.bindExprs, because ON binds must precede outer-WHERE
    // binds in the runtime placeholder order (`SELECT ... ON ?_on ...
    // WHERE ?_outer`). The runtime bind builder concatenates them in this
    // order: per-join ON binds (in join order) → outer WHERE → HAVING →
    // LIMIT/OFFSET.
    rightOnBindExprs  : array<ExpressionPtr>
}

struct private SqlQuery {
    rootType    : TypeDeclPtr
    tableName   : string
    rootAlias   : string                  //!< `""` in single-source mode; `"t0"` once a join is peeled (see seenJoin)
    selectCols  : array<string>           //!< SingleColumn / FullRow / NamedTuple: column names (quoted in SQL); empty entry when the column is a computed expression (selectColSqlFragments[i] holds the SQL).
    selectColAliases : array<string>      //!< multi-source: alias for each column's owning source (parallel to selectCols); empty for single-source columns and for computed expressions.
    selectColTypes : array<TypeDeclPtr>   //!< multi-source: the RESOLVED column type (already field-looked-up). Used by build_row_builder directly — no further lookup needed. Empty in single-source mode (build_row_builder falls back to q.rootType + lookup_struct_field_type).
    selectColSqlFragments : array<string> //!< multi-source: precomputed SQL fragment for non-trivial projection values (e.g. `"t1"."Id" IS NOT NULL` for `o |> is_some`). Empty entry → emit `"alias"."col"` from selectColAliases/selectCols.
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
    seenJoin    : bool                    //!< multi-source mode flag (any join peeled) — qualifies column refs as `"talias"."Col"`
    joins       : array<JoinSpec>         //!< joined sources in chain order
    setOpKind     : SetOpKind = SetOpKind.None  //!< set-op (UNION/INTERSECT/EXCEPT) wrapping the LHS query
    setOpRhsSql   : string                 //!< RHS query's SQL string (already built via build_sql_string on the sub-q)
    seenSetOp     : bool                  //!< guard: only one set-op per chain in chunk 5 (chained set-ops are deferred)
    // Lambda-arg → source-idx binding stack. Populated transiently around
    // `analyze_predicate` / `analyze_projection` calls to teach `pred_to_sql`
    // / projection analyzers which source a 2-arg `into` lambda's named
    // parameters refer to. Single-source mode keeps this empty (`_` in
    // `_where` predicates always means source 0; we don't need a mapping).
    argNames    : array<string>
    argSourceIdx : array<int>             //!< parallel to argNames
    dbExpr      : ExpressionPtr
}

// ============================================================================
// Multi-source column qualification helpers.
//
// Single-source mode emits unqualified `"Col"`. Multi-source mode (any join
// peeled) emits `"talias"."Col"` so column refs are unambiguous when
// SQLite's planner sees JOIN clauses.
//
// `column_sql_for_alias(alias, field)` is the single emission point that
// every column-ref site routes through. Callers compute the alias via
// `source_alias_for_idx(q, idx)` (idx 0 = root, 1+ = join idx 0+) and pass
// the alias here.
// ============================================================================

[macro_function]
def private source_alias_for_idx(q : SqlQuery; idx : int) : string {
    //! Alias of source `idx`: 0 = root (`q.rootAlias`), 1.. = `q.joins[idx-1].alias`.
    //! Returns `""` when no rootAlias is set (single-source mode); a non-empty
    //! rootAlias is the alias-emission signal, not `q.seenJoin` — the right-
    //! side subQ analyzed by `process_join_call` has `rootAlias = "t1"` but
    //! `seenJoin = false` (see process_join_call step 3).
    if (empty(q.rootAlias)) {
        return ""
    }
    if (idx == 0) {
        return q.rootAlias
    }
    return q.joins[idx - 1].alias
}

[macro_function]
def private source_rootType_for_idx(var q : SqlQuery&; idx : int) : TypeDeclPtr {
    //! Look up the rootType (struct TypeDecl) of source `idx`. Source 0 is
    //! `q.rootType`; sources 1.. live on `q.joins[idx-1].rootType`. Used by
    //! `build_row_builder` for column-type lookup in multi-source projections.
    //! `var q&` is required because daslang's const-q field access yields
    //! `TypeDecl const?` (pointer-to-const), incompatible with the
    //! `lookup_struct_field_type` signature.
    if (idx == 0) {
        return q.rootType
    }
    return q.joins[idx - 1].rootType
}

[macro_function]
def private column_sql_for_alias(alias : string; field : string) : string {
    //! Format a column SQL fragment. Empty alias → unqualified `"Col"` (single-
    //! source mode). Non-empty alias → `"talias"."Col"`.
    if (empty(alias)) {
        return "\"{field}\""
    }
    return "\"{alias}\".\"{field}\""
}

[macro_function]
def private lookup_arg_source(q : SqlQuery; argName : string) : int {
    //! Resolve a lambda-arg name to its source index. Returns -1 if not
    //! bound (caller falls back to source 0 in single-source mode, or
    //! emits a macro_error in multi-source mode).
    let n = length(q.argNames)
    for (i in range(n)) {
        if (q.argNames[i] == argName) {
            return q.argSourceIdx[i]
        }
    }
    return -1
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
def private extract_key_selector_field(var keyLambda : ExpressionPtr; expectedArgName : string;
                                       prog : ProgramPtr; at : LineInfo) : string {
    //! Pull the `<arg>.<field>` out of a 1-arg key-selector lambda body.
    //! `linq_boost`'s `_join`/`_left_join` macro renames the arg to a
    //! fresh `__la__` / `__lb__` symbol; we accept that exact name (passed
    //! in via `expectedArgName`) and reject anything else loud — the only
    //! supported equi-join shape is `(l, r) => l.X == r.Y`, and the macro
    //! has already validated it. This step extracts the column name.
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
    var argName : string
    var fieldName : string
    if (!extract_arg_field(ret.subexpr, argName, fieldName)) {
        macro_error(prog, at, "_sql: _join key selector body must be `<arg>.Field` (column ref); computed key expressions are not yet translatable to SQL — use raw SQL or restructure the join")
        return ""
    }
    if (argName != expectedArgName) {
        macro_error(prog, at, "_sql: _join key selector references arg `{argName}`, expected `{expectedArgName}` — the on-clause must read `(l, r) => l.X == r.Y` referring to the join's own params, not outer captures")
        return ""
    }
    return fieldName
}

[macro_function]
def private process_join_call(var jCall : ExprCall?; var q : SqlQuery&;
                              kind : JoinKind; prog : ProgramPtr; at : LineInfo) : bool {
    //! Shared body for `_join` (INNER) and `_left_join` (LEFT) peels.
    //!
    //! Post-expansion shape from `daslib/linq_boost`:
    //!   join(srca, srcb, $(__la__:TA) => __la__.X, $(__lb__:TB) => __lb__.Y, into_lambda)
    //!   left_join(...)  — same 5-arg shape
    //!
    //! Steps:
    //!  1. Mark multi-source mode (q.seenJoin, q.rootAlias = "t0").
    //!  2. Recurse into srca (populates q.rootType, q.tableName, processes
    //!     any pre-join `_where` with qualified column refs).
    //!  3. Build a sub-`SqlQuery` for srcb with its own pre-set rootAlias
    //!     ("t1"), recurse to analyze it. The right side accepts only
    //!     `select_from(type<T>)` plus optional `_where(...)`; aggregates,
    //!     `_group_by`, `_select`, terminals, and nested joins are rejected.
    //!  4. Construct a `JoinSpec` from the sub-q's rootType / tableName /
    //!     whereSql. Extract the equi-join field names from keya / keyb.
    //!     Push the JoinSpec and keep the right-side `_where` SQL on
    //!     `spec.rightWhereSql` (emitted in the JOIN's `ON ... AND (...)`
    //!     clause, NOT outer WHERE — required for LEFT JOIN, used uniformly
    //!     for INNER too) and its binds on `spec.rightOnBindExprs`. The
    //!     emit-time bind builder concatenates pools in SQL clause order:
    //!     per-join ON binds (in join order) → outer WHERE → HAVING →
    //!     LIMIT/OFFSET, so placeholder indices line up with
    //!     `... ON keyEq AND (rightWhereSql) ... WHERE (outer) ...`.
    //!  5. Process the `into` projection: bind its 2 lambda args to source
    //!     0 / source 1, then run analyze_projection on the body.
    if (q.seenJoin) {
        macro_error(prog, at, "_sql: only one join per chain (3+ table joins are not yet supported in chunk 5; chain multiple two-table joins or use raw SQL)")
        return false
    }
    if (q.seenSelect) {
        macro_error(prog, at, "_sql: _join must precede `_select` in source order — the join's `into` lambda is the projection. Drop the trailing `_select(...)` or restructure the chain.")
        return false
    }
    if (q.seenGroupBy) {
        macro_error(prog, at, "_sql: _join cannot follow `_group_by` (joining grouped rows is not yet supported); group AFTER the join instead")
        return false
    }
    if (length(jCall.arguments) != 5) {
        macro_error(prog, at, "_sql: _join expected 5 args (srca, srcb, keya, keyb, into) post-expansion; got {length(jCall.arguments)}")
        return false
    }
    q.seenJoin = true
    q.rootAlias = "t0"

    // 2. Recurse into srca — populates q.rootType, q.tableName, processes
    //    any inner `_where`. Inner `_where`'s pred_to_sql emits qualified
    //    column refs (`"t0"."Col"`) because q.seenJoin is now true.
    if (!analyze_chain(jCall.arguments[0], q, prog, at)) {
        return false
    }

    // 3. Build sub-q for srcb. Pre-set rootAlias = "t1" so its inner
    //    `_where` peel emits qualified columns from the start. We
    //    deliberately do NOT pre-set `subQ.seenJoin = true` here:
    //    `seenJoin` also gates projection parsing (multi-source arg-name
    //    lookup via `q.argNames`) and join-duplicate guards, so forcing
    //    it on the subQ would surface confusing errors for inner stages
    //    that we already reject post-hoc (e.g. `_select` on the right
    //    side). Column qualification is driven by `!empty(q.rootAlias)`
    //    in `pred_to_sql` — the alias-emission signal is independent of
    //    "this query contains a join".
    var subQ = SqlQuery(rootAlias = "t1")
    if (!analyze_chain(jCall.arguments[1], subQ, prog, at)) {
        return false
    }
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
    if (subQ.seenJoin && length(subQ.joins) > 0) {
        macro_error(prog, at, "_sql: _join right side cannot itself contain a join (3+ table joins not yet supported)")
        return false
    }

    // 4. Construct JoinSpec.
    var spec <- default<JoinSpec>
    spec.kind = kind
    spec.rootType = subQ.rootType
    spec.tableName = subQ.tableName
    spec.alias = "t1"
    spec.leftAlias = "t0"
    spec.leftCol = extract_key_selector_field(jCall.arguments[2], "__la__", prog, at)
    if (empty(spec.leftCol)) {
        return false
    }
    spec.rightCol = extract_key_selector_field(jCall.arguments[3], "__lb__", prog, at)
    if (empty(spec.rightCol)) {
        return false
    }
    spec.rightWhereSql = subQ.whereSql
    // Hold right-side WHERE binds on the JoinSpec (NOT the outer q.bindExprs)
    // — the runtime bind builder concatenates per-join ON binds first, then
    // outer WHERE / HAVING / LIMIT / OFFSET, so the placeholder index lines
    // up with `... ON keyEq AND (rightWhereSql) ... WHERE (outer)`.
    spec.rightOnBindExprs |> reserve(length(subQ.bindExprs))
    for (b in subQ.bindExprs) {
        spec.rightOnBindExprs |> push <| clone_expression(b)
    }
    q.joins |> emplace(spec)

    // 5. Process `into` projection — 2-arg lambda; bind arg0 → source 0,
    //    arg1 → source 1, then run analyze_projection on the body.
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
    let projOk = analyze_projection(iret.subexpr, q, prog, at)
    q.argNames |> clear
    q.argSourceIdx |> clear
    if (!projOk) {
        return false
    }
    q.seenSelect = true
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

    // Peel set operations: union(a, b) / intersect(a, b) / except(a, b).
    // The LHS chain populates the outer `q`; the RHS chain analyzes into a
    // sub-q whose SQL string + binds get stored on the outer q. Restrict to
    // one set-op per chain (chained set-ops would need parenthesization
    // bookkeeping not yet wired). LHS may already have joins / WHERE /
    // _select / etc. — set-op wraps the WHOLE LHS query.
    //
    // RHS rejections (`seenTerminal`/`distinctFlag`/`limitBindExpr`/etc.)
    // happen via the same checks used for subquery analyzers — set-op
    // RHS is essentially another subquery that must be a complete SELECT.
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
            if (!analyze_chain(soCall.arguments[0], q, prog, at)) {
                return false
            }
            // Set-op SQL is emitted as `<inner-LHS> UNION <inner-RHS>` with no
            // surrounding parens, so SQLite parses any per-side `ORDER BY` /
            // `LIMIT` / `OFFSET` as a syntax error ("ORDER BY clause should
            // come after UNION not before"). Reject those stages on either
            // side in chunk 5; compound-query semantics (per-side parens or a
            // single trailing ORDER BY/LIMIT) are deferred.
            if (q.seenOrderBy) {
                macro_error(prog, at, "_sql: set-op LHS cannot use `_order_by` (apply to the combined set-op result instead; chunk 5)")
                return false
            }
            if (q.limitBindExpr != null || q.offsetBindExpr != null) {
                macro_error(prog, at, "_sql: set-op LHS cannot use `take(n)` / `skip(n)` (chunk 5)")
                return false
            }
            // Recurse into RHS in a fresh sub-q; reject set-ops within set-ops.
            var subQ = SqlQuery()
            if (!analyze_chain(soCall.arguments[1], subQ, prog, at)) {
                return false
            }
            if (subQ.seenSetOp || subQ.setOpKind != SetOpKind.None) {
                macro_error(prog, at, "_sql: set-ops cannot themselves contain set-ops (chunk 5)")
                return false
            }
            if (subQ.seenTerminal || subQ.seenToArray) {
                macro_error(prog, at, "_sql: set-op RHS cannot have terminals (`_first`, `_to_array`, etc.)")
                return false
            }
            if (subQ.seenOrderBy) {
                macro_error(prog, at, "_sql: set-op RHS cannot use `_order_by` (apply to the combined set-op result instead; chunk 5)")
                return false
            }
            if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
                macro_error(prog, at, "_sql: set-op RHS cannot use `take(n)` / `skip(n)` (chunk 5)")
                return false
            }
            let rhsSql = build_sql_string(subQ)
            // Compound-query bind ordering must follow SQL clause order:
            // LHS(on → where → having) then RHS(on → where → having). The
            // outer-q finalizer (in emit_sql_macro_body) prepends q.joins'
            // ON binds and appends q.havingBindExprs to q.bindExprs at SQL
            // emit time — that pattern is correct for a single SELECT, but
            // for `<lhs> UNION <rhs>` we have to flatten *both* sides here
            // and clear the per-pool sources so the finalizer doesn't re-
            // prepend / re-append already-folded LHS binds and end up with
            // double-binds and mis-ordered placeholders.
            //
            // After this block, q.bindExprs holds the COMPLETE flat bind
            // list for the compound query, and q.joins[*].rightOnBindExprs
            // / q.havingBindExprs are empty.
            var folded : array<ExpressionPtr>
            var foldedCount = 0
            for (j in q.joins) {
                foldedCount += length(j.rightOnBindExprs)
            }
            for (j in subQ.joins) {
                foldedCount += length(j.rightOnBindExprs)
            }
            folded |> reserve(foldedCount + length(q.bindExprs) + length(q.havingBindExprs)
                            + length(subQ.bindExprs) + length(subQ.havingBindExprs))
            // LHS: on → where → having
            for (j in q.joins) {
                for (b in j.rightOnBindExprs) {
                    folded |> push(b)
                }
            }
            for (b in q.bindExprs) {
                folded |> push(b)
            }
            for (b in q.havingBindExprs) {
                folded |> push(b)
            }
            // Clear LHS per-pool sources — the finalizer must not re-prepend
            // these. q.joins itself stays (the FROM/SELECT emitter walks it
            // for JOIN clauses), only its bind pools are emptied.
            for (j in q.joins) {
                j.rightOnBindExprs |> clear
            }
            q.havingBindExprs |> clear
            // RHS: on → where → having (RHS bindings come AFTER LHS in the
            // SQL `<lhs> UNION <rhs>` placeholder order).
            for (j in subQ.joins) {
                for (b in j.rightOnBindExprs) {
                    folded |> push <| clone_expression(b)
                }
            }
            for (b in subQ.bindExprs) {
                folded |> push <| clone_expression(b)
            }
            for (b in subQ.havingBindExprs) {
                folded |> push <| clone_expression(b)
            }
            q.bindExprs <- folded
            q.setOpKind = soKind
            q.setOpRhsSql = rhsSql
            q.seenSetOp = true
            return true
        }
    }

    // Peel join(srca, srcb, keya, keyb, into) — post-expansion of `_join`
    // (linq_boost's macro splits the equi-join `on` into keya/keyb selectors).
    {
        var jCall = match_linq_call(node, "join")
        if (jCall != null) {
            return process_join_call(jCall, q, JoinKind.Inner, prog, at)
        }
    }

    // Peel left_join(...) — post-expansion of `_left_join`. Same 5-arg shape
    // as `join` but the `into` lambda's right param is `Option<TB>`; under
    // `_sql` the right-side columns become NULL on no-match rows.
    {
        var ljCall = match_linq_call(node, "left_join")
        if (ljCall != null) {
            return process_join_call(ljCall, q, JoinKind.Left, prog, at)
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
def private extract_arg_field(var node : ExpressionPtr; var argName : string&; var fieldName : string&) : bool {
    //! Extract `<argName>.<fieldName>` from an ExprField-rooted-at-ExprVar node.
    //! Peels ExprRef2Value wrappers. Returns true on match; populates the
    //! out-strings via reference. Used by analyze_projection / pred_to_sql to
    //! handle both single-source (`_.Col`) and multi-source (`u.Col`) shapes
    //! through one extractor.
    var n = peel_ref2val(node)
    if (n == null || !(n is ExprField)) {
        return false
    }
    var ef = n as ExprField
    var recv = peel_ref2val(ef.value)
    if (recv == null || !(recv is ExprVar)) {
        return false
    }
    argName = string((recv as ExprVar).name)
    fieldName = string(ef.name)
    return true
}

[macro_function]
def private try_left_join_null_probe(var node : ExpressionPtr; q : SqlQuery;
                                     var argName : string&; positive : bool) : string {
    //! If `node` is a bare `ExprVar` whose name maps to a joined source
    //! (idx > 0) AND that source is a LEFT JOIN, return the SQL fragment
    //! that probes the right-side join key column for NULL-ness:
    //!   positive = true  (`is_some`)  → `"alias"."rightCol" IS NOT NULL`
    //!   positive = false (`is_none`)  → `"alias"."rightCol" IS NULL`
    //! Returns "" otherwise — caller falls through to the default
    //! is_some/is_none translation (which expects a column ref).
    var n = peel_ref2val(node)
    if (n == null || !(n is ExprVar)) {
        return ""
    }
    if (!q.seenJoin) {
        return ""
    }
    argName = string((n as ExprVar).name)
    let srcIdx = lookup_arg_source(q, argName)
    if (srcIdx <= 0) {
        return ""
    }
    unsafe {
        let spec & = q.joins[srcIdx - 1]
        if (spec.kind != JoinKind.Left) {
            return ""
        }
        if (positive) {
            return "\"{spec.alias}\".\"{spec.rightCol}\" IS NOT NULL"
        }
        return "\"{spec.alias}\".\"{spec.rightCol}\" IS NULL"
    }
}

[macro_function]
def private resolve_proj_arg_to_source(q : SqlQuery; argName : string; prog : ProgramPtr; at : LineInfo) : int {
    //! Map a projection-lambda arg name to its source idx. Single-source mode
    //! accepts only `_` (idx 0). Multi-source mode looks up `q.argNames`
    //! (populated transiently by the join peel) and macro_errors on miss.
    if (!q.seenJoin) {
        if (argName == "_") {
            return 0
        }
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
    // After Mode-2 expansion the typer may wrap field reads in ExprRef2Value
    // (the value-from-ref adapter). Peel it before pattern-matching.
    if (body is ExprRef2Value) {
        body = (body as ExprRef2Value).subexpr
        if (body == null) {
            macro_error(prog, at, "_sql: _select: ExprRef2Value with null subexpr")
            return false
        }
    }

    // Single-column form: <argname>.FieldName  (`_` in single-source, `u`/`o` etc. after a join).
    {
        var argName : string
        var fieldName : string
        if (extract_arg_field(body, argName, fieldName)) {
            let srcIdx = resolve_proj_arg_to_source(q, argName, prog, at)
            if (srcIdx < 0) {
                return false
            }
            q.selectCols |> clear
            q.selectColAliases |> clear
            q.selectColTypes |> clear
            q.selectColSqlFragments |> clear
            q.selectCols |> push(fieldName)
            if (q.seenJoin) {
                q.selectColAliases |> push(source_alias_for_idx(q, srcIdx))
                q.selectColSqlFragments |> push("")
                let srcType = source_rootType_for_idx(q, srcIdx)
                q.selectColTypes |> push(lookup_struct_field_type(srcType, fieldName))
            }
            q.proj = ProjectionShape.SingleColumn
            return true
        }
    }

    // Named-tuple form: each value is either `<arg>.Field` (column ref) or
    // a multi-source `is_some(arg)` / `is_none(arg)` (LEFT JOIN bool probe).
    // `unwrap_or` and other Option<T> ops on whole-row Option<TB> are not
    // yet supported (they require fragmenting unwrap_or's `default.Field`
    // into per-column COALESCE — punted to follow-up work). recordNames
    // live on the inferred tuple type's argNames.
    if (body is ExprMakeTuple) {
        var mkTup = body as ExprMakeTuple
        var tupT = mkTup._type
        if (tupT != null && tupT.baseType == Type.tTuple
                && length(tupT.argNames) > 0
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
                var argName : string
                var fieldName : string
                if (extract_arg_field(val, argName, fieldName)) {
                    let srcIdx = resolve_proj_arg_to_source(q, argName, prog, at)
                    if (srcIdx < 0) {
                        return false
                    }
                    q.selectCols |> push(fieldName)
                    q.projRecordNames |> push(string(tupT.argNames[i]))
                    if (q.seenJoin) {
                        q.selectColAliases |> push(source_alias_for_idx(q, srcIdx))
                        q.selectColSqlFragments |> push("")
                        let srcType = source_rootType_for_idx(q, srcIdx)
                        q.selectColTypes |> push(lookup_struct_field_type(srcType, fieldName))
                    }
                    continue
                }
                // Multi-source computed projection: is_some(arg) / is_none(arg).
                // Recognized only when one of the lambda args is a LEFT JOIN's
                // Option<TB> param. The result type is inferred from the AST
                // (`val._type`) — bool for is_some/is_none.
                if (q.seenJoin && val is ExprCall) {
                    var fcall = val as ExprCall
                    // `fcall.func` may be unresolved when this projection
                    // expression is reached pre-inference; fall back to the
                    // textual call name (`fcall.name`) just like pred_to_sql /
                    // fn_call_to_sql do for predicate-side calls.
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
                        let frag = try_left_join_null_probe(fcall.arguments[0], q, probeArg, fname == "is_some")
                        if (!empty(frag)) {
                            q.selectCols |> push("")
                            q.selectColAliases |> push("")
                            q.selectColSqlFragments |> push(frag)
                            q.projRecordNames |> push(string(tupT.argNames[i]))
                            // Result type is bool for both.
                            q.selectColTypes |> push(new TypeDecl(at = at, baseType = Type.tBool))
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
    //
    // Special case: `_not_in` lowers to `!contains(subq, x)`; under daslang's
    // type-inference ordering the inner `contains` call may not be resolved
    // (call.func is null) at the time pred_to_sql recurses on it, even
    // though `_in` (no leading `!`) is. Peek at the inner ExprCall's
    // textual name (which is set on `ExprLooksLikeCall` even before typer
    // overload resolution) and route to the subquery NOT IN emitter
    // directly. Falls through to the generic `!expr` peel for other shapes.
    {
        var inner : ExpressionPtr
        if (qmatch(node, !$e(inner)).matched) {
            if (inner != null && inner is ExprCall) {
                var icall = inner as ExprCall
                let iname = string(icall.name)
                let iargc = user_arg_count(icall)
                if (iname == "contains" && iargc == 2) {
                    // Disambiguate string contains vs iterator contains by the
                    // inferred type of the first arg (the subquery).
                    var firstT = icall.arguments[0]._type
                    if (firstT != null && (firstT.isIterator || firstT.isGoodArrayType)) {
                        let xSql = pred_to_sql(icall.arguments[1], q, prog, at)
                        var binds : array<ExpressionPtr>
                        let subSql = analyze_subquery(icall.arguments[0], binds, prog, at, true)
                        if (empty(subSql)) {
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

    // Column reference: <argname>.ColName  (ExprField rooted at any lambda param).
    // Single-source: emit unqualified `"Col"` (only `_` is bound).
    // Multi-source: look up arg → source idx → emit `"talias"."Col"`. Unrecognized
    // arg names in multi-source mode are a `macro_error` — they're either the
    // group-key shape (`_._0`) handled below, or genuinely unbound (caller bug).
    {
        var n2 = peel_ref2val(node)
        if (n2 != null && n2 is ExprField) {
            var ef = n2 as ExprField
            var recv = peel_ref2val(ef.value)
            if (recv != null && recv is ExprVar) {
                let argName = string((recv as ExprVar).name)
                let fieldName = string(ef.name)
                // Alias-qualification is driven by `rootAlias` being non-
                // empty, NOT by `seenJoin`. The two are intentionally
                // decoupled: the right-side subQ analyzed by
                // `process_join_call` runs with `rootAlias = "t1"` but
                // `seenJoin = false` so `_where` in the inner chain qualifies
                // columns ("t1"."Col") without dragging in multi-source
                // projection-arg-lookup semantics.
                if (!empty(q.rootAlias)) {
                    var srcIdx = lookup_arg_source(q, argName)
                    // Pre-join `_where` always references the root (source 0)
                    // through `_`. The arg-binding stack is only populated
                    // for the `into` projection's named 2-arg lambda; outside
                    // that scope, treat `_` as source 0.
                    if (srcIdx < 0 && argName == "_") {
                        srcIdx = 0
                    }
                    if (srcIdx >= 0) {
                        return column_sql_for_alias(source_alias_for_idx(q, srcIdx), fieldName)
                    }
                    // Not bound to any source — fall through to other patterns
                    // (e.g. `_._0` group-key references which keep their own
                    // recognition path below).
                } else {
                    // Single-source: only `_` is bound. Match the legacy emission.
                    if (argName == "_") {
                        return "\"{fieldName}\""
                    }
                }
            }
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

    // Unexpanded ExprCallMacro for `_in` / `_not_in` / `_any` / `_none`.
    // Mode-2 expansion only recurses through call_macros and chain
    // operators — when one of these macros lands inside a binary operator
    // (`&&` / `||` / etc.) of a `_where` predicate, daslang doesn't deep-
    // walk into the operator's children, so the macro stays in
    // `ExprCallMacro` form. Recognize them here by the macro's name and
    // emit the SQL fragment without going through expansion.
    if (node is ExprCallMacro) {
        var mcall = node as ExprCallMacro
        let mname = string(mcall.name)
        if ((mname == "_in" || mname == "_not_in") && length(mcall.arguments) == 2) {
            // `x._in(subq)` / `x._not_in(subq)` — call_macro arg order is
            // (element, subquery).
            let xSql = pred_to_sql(mcall.arguments[0], q, prog, at)
            var binds : array<ExpressionPtr>
            let subSql = analyze_subquery(mcall.arguments[1], binds, prog, at, true)
            if (empty(subSql)) {
                return ""
            }
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            if (mname == "_not_in") {
                return "{xSql} NOT IN ({subSql})"
            }
            return "{xSql} IN ({subSql})"
        }
        if ((mname == "_any" || mname == "_none") && length(mcall.arguments) >= 1) {
            // `subq._any()` / `subq._none()` (1 arg) or
            // `subq._any(pred)` / `subq._none(pred)` (2 args, pred is a
            // bare expression — the call_macro hasn't lambda-wrapped it
            // yet because expansion didn't fire).
            var subSql : string
            var binds : array<ExpressionPtr>
            if (length(mcall.arguments) == 1) {
                subSql = analyze_subquery(mcall.arguments[0], binds, prog, at)
            } else {
                // Wrap the pred in a single-arg lambda manually, matching
                // what `AstCallMacro_LinqPred2` would emit, then re-use
                // analyze_subquery_with_pred. We don't actually have the
                // expanded `(_:T) => pred` shape yet — but the inner
                // analyze_predicate just walks the predicate AST, so we
                // can pass a synthetic `ExprMakeBlock` wrapping the pred.
                // Easier: build the subquery first, then call
                // analyze_predicate directly on the bare pred expression.
                var subQ = SqlQuery()
                if (!analyze_chain(mcall.arguments[0], subQ, prog, at)) {
                    return ""
                }
                if (subQ.seenJoin || length(subQ.joins) > 0) {
                    macro_error(prog, at, "_sql: subqueries cannot themselves contain joins (chunk 5)")
                    return ""
                }
                if (subQ.seenGroupBy || !empty(subQ.havingSql) || subQ.seenOrderBy
                        || subQ.distinctFlag || subQ.limitBindExpr != null
                        || subQ.offsetBindExpr != null || subQ.seenTerminal
                        || subQ.seenToArray) {
                    macro_error(prog, at, "_sql: _any/_none subquery must be a `select_from(...)` (chain stages and terminals are not yet allowed)")
                    return ""
                }
                if (!analyze_predicate(mcall.arguments[1], subQ, prog, at)) {
                    return ""
                }
                subSql = build_sql_string(subQ)
                binds |> reserve(length(binds) + length(subQ.bindExprs))
                for (b in subQ.bindExprs) {
                    binds |> push <| clone_expression(b)
                }
            }
            if (empty(subSql)) {
                return ""
            }
            for (b in binds) {
                if (q.inHaving) {
                    q.havingBindExprs |> push <| clone_expression(b)
                } else {
                    q.bindExprs |> push <| clone_expression(b)
                }
            }
            if (mname == "_none") {
                return "NOT EXISTS ({subSql})"
            }
            return "EXISTS ({subSql})"
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
def private analyze_subquery(var subqExpr : ExpressionPtr; var binds : array<ExpressionPtr>&;
                             prog : ProgramPtr; at : LineInfo; requireSingleColumn : bool = false) : string {
    //! Recursively analyze a subquery chain (bare `select_from(type<T>)` or
    //! `select_from(...) |> _where(...)` or `select_from(...) |> _select(_.Col)`)
    //! and return its SQL string. Subquery WHERE binds are pushed to `binds`;
    //! the caller appends them to outer `q.bindExprs` AT THE CALL SITE so the
    //! placeholder index matches the SQL emission order (outer predicate's
    //! own pred_to_sql may have already appended binds before us, and the
    //! subquery's binds must come after).
    //!
    //! Reject subqueries that are correlated, grouped, ordered, distinct, or
    //! have terminals — chunk-5 ships uncorrelated, single-source subqueries
    //! only. Correlated subqueries (referring to outer columns inside the
    //! subquery's WHERE) are deferred.
    //!
    //! `requireSingleColumn` is set by `_in` / `_not_in` callsites: SQLite
    //! requires the inner SELECT to project exactly one column for `x IN (...)`,
    //! and emits a runtime "sub-select returns N columns" error otherwise. We
    //! reject at compile time so users see a `_sql` diagnostic instead of a
    //! runtime panic. `_any` / `_none` use `EXISTS (...)` and don't care about
    //! projection arity, so they leave `requireSingleColumn` at false.
    var subQ = SqlQuery()
    if (!analyze_chain(subqExpr, subQ, prog, at)) {
        return ""
    }
    if (subQ.seenJoin || length(subQ.joins) > 0) {
        macro_error(prog, at, "_sql: subqueries cannot themselves contain joins (chunk 5)")
        return ""
    }
    if (subQ.seenGroupBy || !empty(subQ.havingSql)) {
        macro_error(prog, at, "_sql: subqueries cannot use `_group_by` / `_having` (chunk 5)")
        return ""
    }
    if (subQ.seenOrderBy || subQ.distinctFlag) {
        macro_error(prog, at, "_sql: subqueries cannot use `_order_by` / `distinct()` (chunk 5)")
        return ""
    }
    if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
        macro_error(prog, at, "_sql: subqueries cannot use `take(n)` / `skip(n)` (chunk 5)")
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
                                       prog : ProgramPtr; at : LineInfo) : string {
    //! Like `analyze_subquery`, but injects an additional WHERE clause from
    //! the predicate lambda body. Used for `any(subq, predLambda)` and
    //! `none(subq, predLambda)`. The lambda's arg is bound to the subquery's
    //! row (`_` always refers to source 0 of the subquery).
    var subQ = SqlQuery()
    if (!analyze_chain(subqExpr, subQ, prog, at)) {
        return ""
    }
    if (subQ.seenJoin || length(subQ.joins) > 0) {
        macro_error(prog, at, "_sql: subqueries cannot themselves contain joins (chunk 5)")
        return ""
    }
    if (subQ.seenGroupBy || !empty(subQ.havingSql)) {
        macro_error(prog, at, "_sql: subqueries cannot use `_group_by` / `_having` (chunk 5)")
        return ""
    }
    if (subQ.seenOrderBy || subQ.distinctFlag) {
        macro_error(prog, at, "_sql: subqueries cannot use `_order_by` / `distinct()` (chunk 5)")
        return ""
    }
    if (subQ.limitBindExpr != null || subQ.offsetBindExpr != null) {
        macro_error(prog, at, "_sql: subqueries cannot use `take(n)` / `skip(n)` (chunk 5)")
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
    if (!analyze_predicate(body, subQ, prog, at)) {
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
def private fn_call_to_sql(var call : ExprCall?; var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : string {
    //! Returns the SQL expression text for a recognized function call,
    //! or "" if the call name isn't in the dispatch table (caller falls
    //! through to literal/captured-var bind).
    //!
    //! Resolves the function name from `call.name` (the `ExprLooksLikeCall`
    //! textual name, always set even before typer overload resolution) and
    //! prefers the more-specific `call.func.fromGeneric.name` /
    //! `call.func.name` when those are available. Using `call.name` as the
    //! fallback lets us recognize calls whose overload hasn't been resolved
    //! yet by the inferer — happens for `contains(...)` nested inside `&&`
    //! / `!` predicates, where daslang fires the macro_function before the
    //! inner call's typer pass completes.
    if (call == null) {
        return ""
    }
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
        // Two `contains` overloads land here:
        //   - string `contains(haystack, needle)` → LIKE pattern (chunk 3)
        //   - array/iterator `contains(subquery, x)` → IN subquery
        //     (chunk 5, post-expansion of `x._in(subq)`)
        // Disambiguate by the first arg's type: iterator/array routes to
        // the subquery handler below; string falls through here.
        var firstT = call.arguments[0]._type
        if (firstT != null && (firstT.isIterator || firstT.isGoodArrayType)) {
            // Fall through to the subquery `contains` branch below.
        } else {
            let lhs = pred_to_sql(call.arguments[0], q, prog, at)
            let rhs = pred_to_sql(call.arguments[1], q, prog, at)
            return "{lhs} LIKE '%' || {rhs} || '%'"
        }
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
    //
    // Special case for `_left_join`: when the arg is a bare `ExprVar`
    // referring to the join's `Option<TB>` parameter (source idx > 0),
    // probe the join's right-key column for NULL-ness — the row missed
    // its match exactly when that column came back NULL. Without this
    // case, pred_to_sql would treat the bare arg as a captured var and
    // bind it as `?`, producing semantically wrong SQL.
    if (fname == "is_some" && argc == 1) {
        var argName : string
        let nullProbe = try_left_join_null_probe(call.arguments[0], q, argName, true)
        if (!empty(nullProbe)) {
            return nullProbe
        }
        let inner = pred_to_sql(call.arguments[0], q, prog, at)
        return "{inner} IS NOT NULL"
    }
    if (fname == "is_none" && argc == 1) {
        var argName : string
        let nullProbe = try_left_join_null_probe(call.arguments[0], q, argName, false)
        if (!empty(nullProbe)) {
            return nullProbe
        }
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

    // ---- Subqueries ----
    // `x._in(subquery)` post-expansion: `contains(subquery, x)`.
    // `x._not_in(subquery)` post-expansion: `!contains(subquery, x)` —
    // the leading `!` is handled by pred_to_sql's existing NOT peel
    // before this function fires, recursing into `contains(...)` here.
    // Subquery rules and rejections live in `analyze_subquery`.
    if (fname == "contains" && argc == 2) {
        let xSql = pred_to_sql(call.arguments[1], q, prog, at)
        var binds : array<ExpressionPtr>
        let subSql = analyze_subquery(call.arguments[0], binds, prog, at, true)
        if (empty(subSql)) {
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

    // `subquery._any()` post-expansion: `any(subquery)` — emits `EXISTS (...)`.
    // `subquery._any(pred)` post-expansion: `any(subquery, $(_:T) => pred)`
    //   — emits `EXISTS (... WHERE pred)`.
    if (fname == "any") {
        if (argc == 1) {
            var binds : array<ExpressionPtr>
            let subSql = analyze_subquery(call.arguments[0], binds, prog, at)
            if (empty(subSql)) {
                return ""
            }
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
            let subSql = analyze_subquery_with_pred(call.arguments[0], call.arguments[1], binds, prog, at)
            if (empty(subSql)) {
                return ""
            }
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
            let subSql = analyze_subquery(call.arguments[0], binds, prog, at)
            if (empty(subSql)) {
                return ""
            }
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
            let subSql = analyze_subquery_with_pred(call.arguments[0], call.arguments[1], binds, prog, at)
            if (empty(subSql)) {
                return ""
            }
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
    let inner = build_sql_select(q)
    if (q.setOpKind == SetOpKind.None) {
        return inner
    }
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
def private build_sql_select(q : SqlQuery) : string {
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
                // Multi-source: precomputed SQL fragments (e.g. `IS NOT
                // NULL` probes) live in selectColSqlFragments; alias-
                // qualified column refs use selectColAliases + selectCols.
                // Single-source: emit unqualified `"Col"`.
                if (q.seenJoin && i < length(q.selectColSqlFragments) && !empty(q.selectColSqlFragments[i])) {
                    w |> write(q.selectColSqlFragments[i])
                } elif (q.seenJoin && i < length(q.selectColAliases) && !empty(q.selectColAliases[i])) {
                    w |> write("\"")
                    w |> write(q.selectColAliases[i])
                    w |> write("\".\"")
                    w |> write(q.selectCols[i])
                    w |> write("\"")
                } else {
                    w |> write("\"")
                    w |> write(q.selectCols[i])
                    w |> write("\"")
                }
            }
        }
        // FROM clause: alias the root table only when joins are present.
        // Single-source mode keeps the existing unaliased `FROM "Tbl"` shape
        // so all chunk-2/3/4 tests still match.
        if (q.seenJoin) {
            w |> write(" FROM \"")
            w |> write(q.tableName)
            w |> write("\" AS \"")
            w |> write(q.rootAlias)
            w |> write("\"")
            for (j in q.joins) {
                if (j.kind == JoinKind.Inner) {
                    w |> write(" INNER JOIN \"")
                } else {
                    w |> write(" LEFT JOIN \"")
                }
                w |> write(j.tableName)
                w |> write("\" AS \"")
                w |> write(j.alias)
                w |> write("\" ON \"")
                w |> write(j.leftAlias)
                w |> write("\".\"")
                w |> write(j.leftCol)
                w |> write("\" = \"")
                w |> write(j.alias)
                w |> write("\".\"")
                w |> write(j.rightCol)
                w |> write("\"")
                // Right-side `_where` lives in the JOIN's ON clause (NOT the
                // outer WHERE). Required for LEFT JOIN — a row-level filter
                // in WHERE drops NULL-extended unmatched rows and silently
                // turns LEFT JOIN into INNER JOIN. INNER JOIN uses the same
                // placement for consistency / planner stability.
                if (!empty(j.rightWhereSql)) {
                    w |> write(" AND (")
                    w |> write(j.rightWhereSql)
                    w |> write(")")
                }
            }
        } else {
            w |> write(" FROM \"")
            w |> write(q.tableName)
            w |> write("\"")
        }
        // Outer WHERE only — right-side `_where` is now in the ON clause
        // above. The outer WHERE applies to the joined result and may
        // legally reference any source's columns (`is_some(o)`-style probes
        // for LEFT JOIN, qualified column refs, etc.).
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
def private build_row_builder(var q : SqlQuery&; prog : ProgramPtr; at : LineInfo) : ExpressionPtr {
    //! Build the per-row materialization expression based on q.proj.
    //! Returns null on error (macro_error already emitted).
    if (q.proj == ProjectionShape.Aggregate) {
        let colIdx = 0
        var aggT = clone_type(q.aggregateType)
        return qmacro(sqlite_read(_sql_stmt, $v(colIdx), type<$t(aggT)>))
    }
    if (q.proj == ProjectionShape.SingleColumn) {
        let colIdx = 0
        // Multi-source: column type is already resolved into `selectColTypes`
        // at projection-analysis time. Single-source: look up via rootType.
        var fieldType : TypeDeclPtr
        if (q.seenJoin && length(q.selectColTypes) > 0) {
            fieldType = q.selectColTypes[0]
        } else {
            fieldType = lookup_struct_field_type(q.rootType, q.selectCols[0])
        }
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
            // Multi-source: type already resolved at projection-analysis time
            // (covers both `<arg>.Col` field-lookup AND computed-column
            // results like `is_some(o)` → bool). Single-source: lookup via
            // q.rootType + selectCols[i].
            var fieldType : TypeDeclPtr
            if (q.seenJoin && i < length(q.selectColTypes)) {
                fieldType = q.selectColTypes[i]
            } else {
                fieldType = lookup_struct_field_type(q.rootType, q.selectCols[i])
            }
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
    //
    // Order is fixed by SQL clause order: per-join ON binds (in join order)
    // → outer WHERE → HAVING → LIMIT → OFFSET. Build the final list in
    // that order; `q.bindExprs` accumulates outer WHERE during analysis,
    // so we splice ON binds in front of it before appending the rest.
    var joinOnCount = 0
    for (j in q.joins) {
        joinOnCount += length(j.rightOnBindExprs)
    }
    var joinOnBinds : array<ExpressionPtr>
    joinOnBinds |> reserve(joinOnCount)
    for (j in q.joins) {
        for (b in j.rightOnBindExprs) {
            joinOnBinds |> push(b)
        }
    }
    if (length(joinOnBinds) > 0) {
        var ordered : array<ExpressionPtr>
        ordered |> reserve(length(joinOnBinds) + length(q.bindExprs))
        for (b in joinOnBinds) {
            ordered |> push(b)
        }
        for (b in q.bindExprs) {
            ordered |> push(b)
        }
        q.bindExprs <- ordered
    }
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

// ============================================================================
// `_sql_update` / `_sql_delete` — predicate-based mutations
//
// These macros run with `canVisitArgument = false` for everything except `db`
// so the WHERE / SET arguments arrive as raw AST (the user writes `_.Col < x`
// with `_` unbound). Analysis proceeds on the raw shape:
//
//   * `_.Col` references are recognized via the AST shape (ExprField over
//     ExprVar named "_") and translated to SQL column refs (`"Col"`). They
//     never become bind parameters.
//   * Captured locals and literals classify as binds — pred_to_sql emits a
//     `?` placeholder and the original ExprVar / ExprConst is pushed to the
//     bind list. At the macro's output, the bind expression is passed verbatim
//     to ``sqlite_bind(stmt, idx, expr)``; daslang's typer resolves the
//     captured-var's type and overload-dispatches `sqlite_bind`.
//
// Bind ordering for SET-then-WHERE SQL: SET binds come first, then WHERE
// binds (matches placeholder order in `UPDATE ... SET col1=?, ... WHERE ...`).
// For RETURNING, the RETURNING clause is appended without adding binds and
// the row materializer reuses the `[sql_table]`-generated `_sql_read_row`.
// ============================================================================

[macro_function]
def private analyze_set_clause(var setExpr : ExpressionPtr; var q : SqlQuery&;
                               var setSqlClauses : array<string>&;
                               var setBindExprs : array<ExpressionPtr>&;
                               prog : ProgramPtr; at : LineInfo) : bool {
    //! Walk a raw `(Col1 = expr1, Col2 = expr2, ...)` named-tuple literal and
    //! emit a list of `"Col" = <sql>` fragments + the bind expressions they
    //! introduce. The SET clause SQL fragments are pushed into
    //! ``setSqlClauses``; bind expressions accumulate into ``setBindExprs``.
    //! WHERE binds (from ``q.bindExprs``) stay separate and are appended
    //! after SET binds at SQL emission time.
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
    if (length(mkTup.values) == 0) {
        macro_error(prog, at, "_sql_update: set clause is empty — UPDATE with no columns is invalid SQL. Pass at least one (Col=expr) pair.")
        return false
    }
    // The named-tuple literal's record names live on the inferred tuple type
    // (`tupT.argNames`). When the macro runs with ``canVisitArgument=false``
    // the SET arg arrives raw and `mkTup._type` is null — fall back to the
    // recordNames carried directly on the MakeTuple node by the parser.
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
    // Validate each SET column: must be a real field on T, no duplicates.
    // The WHERE side validates `_.Field` via pred_to_sql; mirror that
    // discipline on the SET side so typos / collisions surface as a
    // macro_error at compile time instead of a SQLite runtime error.
    if (q.rootType != null && q.rootType.structType != null) {
        var st = q.rootType.structType
        var seenCols : table<string; bool>
        for (i in range(length(recordNames))) {
            let colName = recordNames[i]
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
    // Each value is translated via the same pred_to_sql machinery as a WHERE
    // predicate. Captured vars and `_.Col` references both work; binds are
    // pushed into a SET-local list so we can interleave them with WHERE
    // binds in correct placeholder order at SQL emission time.
    var setQ = SqlQuery(
        rootType = q.rootType,
        tableName = q.tableName,
        argNames <- ["_"],
        argSourceIdx <- [0]
    )
    setSqlClauses |> reserve(length(setSqlClauses) + length(mkTup.values))
    for (i in range(length(mkTup.values))) {
        let colName = recordNames[i]
        var val = mkTup.values[i]
        let valSql = pred_to_sql(val, setQ, prog, at)
        if (empty(valSql)) {
            return false  // pred_to_sql already issued macro_error
        }
        setSqlClauses |> push("\"{colName}\" = {valSql}")
    }
    setBindExprs |> reserve(length(setBindExprs) + length(setQ.bindExprs))
    for (b in setQ.bindExprs) {
        setBindExprs |> push(b)
    }
    return true
}

[macro_function]
def private build_returning_clause(rootT : TypeDeclPtr) : string {
    //! Append `RETURNING "c1", "c2", ...` listing every field of T in
    //! struct-declaration order. Mirrors `_sql_select_all_sql`'s output so
    //! the row materializer (`_sql_read_row`) reads columns in struct order.
    if (rootT == null || rootT.structType == null) {
        return ""
    }
    var st = rootT.structType
    return build_string() $(var w) {
        w |> write(" RETURNING ")
        var first = true
        for (field in st.fields) {
            if (!first) {
                w |> write(", ")
            }
            w |> write("\"")
            w |> write(field.name)
            w |> write("\"")
            first = false
        }
    }
}

[macro_function]
def private emit_dml_call(var dbE : ExpressionPtr; sql : string;
                          var bindStmts : array<ExpressionPtr>;
                          isTry : bool; isReturning : bool;
                          rootT : TypeDeclPtr; at : LineInfo) : ExpressionPtr {
    //! Build the runtime call for `_sql_update*` / `_sql_delete*`. Picks one of
    //! the four `_::run_dml*` variants from `(isTry, isReturning)` — the call
    //! name is the only intrinsic fork (qmacro can't parameterize `_::name`);
    //! the bind lambda and (for returning variants) the row reader are built
    //! once and spliced into each branch.
    var bindLambda = qmacro($(var _sql_stmt : sqlite3_stmt?) {
        $b(bindStmts)
    })
    var res : ExpressionPtr
    if (isReturning) {
        var rootClone = clone_type(rootT)
        var readerLambda = qmacro($(_sql_stmt : sqlite3_stmt?) => _::_sql_read_row(_sql_stmt, type<$t(rootClone)>))
        if (isTry) {
            res = qmacro(_::try_run_dml_returning($e(dbE), $v(sql), $e(bindLambda), $e(readerLambda)))
        } else {
            res = qmacro(_::run_dml_returning($e(dbE), $v(sql), $e(bindLambda), $e(readerLambda)))
        }
    } else {
        if (isTry) {
            res = qmacro(_::try_run_dml($e(dbE), $v(sql), $e(bindLambda)))
        } else {
            res = qmacro(_::run_dml($e(dbE), $v(sql), $e(bindLambda)))
        }
    }
    res |> force_at(at)
    return res
}

[macro_function]
def private emit_update_body(prog : ProgramPtr; var call : ExprCallMacro?;
                             var rootT : TypeDeclPtr;
                             isTry : bool; isReturning : bool) : ExpressionPtr {
    //! Shared body for the four `_sql_update*` macros. Operates on the raw
    //! WHERE / SET argument expressions (canVisitArgument=false skipped
    //! type inference for them); pred_to_sql analyzes the AST shape and
    //! pushes captured vars / literals as binds. Caller supplies the already-
    //! extracted ``rootT`` (the struct TypeDecl from `type<T>`).
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
    // Translate WHERE — raw AST.
    let whereSql = pred_to_sql(whereE, q, prog, call.at)
    if (empty(whereSql)) {
        return null
    }
    // Translate SET — raw named-tuple literal.
    var setSqlClauses : array<string>
    var setBindExprs : array<ExpressionPtr>
    if (!analyze_set_clause(setE, q, setSqlClauses, setBindExprs, prog, call.at)) {
        return null
    }
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
    var bindStmts : array<ExpressionPtr>
    bindStmts |> reserve(length(allBinds))
    for (i in range(length(allBinds))) {
        let bindIdx = i + 1
        var be = allBinds[i]
        bindStmts |> push <| qmacro_expr(${
            sqlite_bind(_sql_stmt, $v(bindIdx), $e(be));
        })
    }
    return emit_dml_call(dbE, sql, bindStmts, isTry, isReturning, rootT, call.at)
}

[macro_function]
def private emit_delete_body(prog : ProgramPtr; var call : ExprCallMacro?;
                             var rootT : TypeDeclPtr;
                             isTry : bool; isReturning : bool) : ExpressionPtr {
    //! Shared body for the four `_sql_delete*` macros.
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
    let whereSql = pred_to_sql(whereE, q, prog, call.at)
    if (empty(whereSql)) {
        return null
    }
    var sql = "DELETE FROM \"{tableName}\" WHERE {whereSql}"
    if (isReturning) {
        sql += build_returning_clause(rootT)
    }
    var bindStmts : array<ExpressionPtr>
    bindStmts |> reserve(length(q.bindExprs))
    for (i in range(length(q.bindExprs))) {
        let bindIdx = i + 1
        var be = q.bindExprs[i]
        bindStmts |> push <| qmacro_expr(${
            sqlite_bind(_sql_stmt, $v(bindIdx), $e(be));
        })
    }
    return emit_dml_call(dbE, sql, bindStmts, isTry, isReturning, rootT, call.at)
}

// ----------------------------------------------------------------------------
// Outer macros — `canVisitArgument = false` for everything except db so the
// WHERE / SET arguments stay raw (with `_` unbound). pred_to_sql operates on
// the AST shape directly; bind expressions (captured locals, literals) are
// passed through to ``sqlite_bind`` verbatim and resolve at the macro's
// output type-inference pass.
// ----------------------------------------------------------------------------

[macro_function]
def private validate_outer_update_args(prog : ProgramPtr; var call : ExprCallMacro?; var rootT : TypeDeclPtr&) : bool {
    if (length(call.arguments) != 4) {
        macro_error(prog, call.at, "{call.name}: expected (db, type<T>, where_expr, set_expr) — got {length(call.arguments)} args")
        return false
    }
    var typeE = call.arguments[1]
    if (!qmatch(typeE, type<$t(rootT)>).matched) {
        macro_error(prog, call.at, "{call.name}: second argument must be a `type<T>` literal")
        return false
    }
    return true
}

[macro_function]
def private validate_outer_delete_args(prog : ProgramPtr; var call : ExprCallMacro?; var rootT : TypeDeclPtr&) : bool {
    if (length(call.arguments) != 3) {
        macro_error(prog, call.at, "{call.name}: expected (db, type<T>, where_expr) — got {length(call.arguments)} args")
        return false
    }
    var typeE = call.arguments[1]
    if (!qmatch(typeE, type<$t(rootT)>).matched) {
        macro_error(prog, call.at, "{call.name}: second argument must be a `type<T>` literal")
        return false
    }
    return true
}

[call_macro(name="_sql_update")]
class private SqlUpdateMacro : AstCallMacro {
    //! `_sql_update(db, type<T>, where_expr, set_expr)` — predicate-based UPDATE.
    //! ``where_expr`` is an expression in the row variable `_` (e.g.
    //! ``_.LastSeen < cutoff``). ``set_expr`` is a named-tuple literal
    //! ``(Col1=expr1, Col2=expr2, ...)`` whose RHS may reference ``_.Col``
    //! and captured locals. Returns ``int`` rows-affected. Panics on SQL error.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_update_args(prog, call, rootT)) {
            return null
        }
        return emit_update_body(prog, call, rootT, false, false)
    }
}

[call_macro(name="_sql_try_update")]
class private SqlTryUpdateMacro : AstCallMacro {
    //! Non-panic sibling of `_sql_update`. Returns ``Result<int, string>``.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_update_args(prog, call, rootT)) {
            return null
        }
        return emit_update_body(prog, call, rootT, true, false)
    }
}

[call_macro(name="_sql_update_returning")]
class private SqlUpdateReturningMacro : AstCallMacro {
    //! `_sql_update(...)` with a trailing ``RETURNING *`` clause. Returns
    //! ``array<T>`` of post-update rows. Panics on SQL error.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_update_args(prog, call, rootT)) {
            return null
        }
        return emit_update_body(prog, call, rootT, false, true)
    }
}

[call_macro(name="_sql_try_update_returning")]
class private SqlTryUpdateReturningMacro : AstCallMacro {
    //! Non-panic sibling of `_sql_update_returning`. Returns
    //! ``Result<array<T>, string>``.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_update_args(prog, call, rootT)) {
            return null
        }
        return emit_update_body(prog, call, rootT, true, true)
    }
}

[call_macro(name="_sql_delete")]
class private SqlDeleteMacro : AstCallMacro {
    //! `_sql_delete(db, type<T>, where_expr)` — predicate-based DELETE.
    //! Returns ``int`` rows-affected. Panics on SQL error.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_delete_args(prog, call, rootT)) {
            return null
        }
        return emit_delete_body(prog, call, rootT, false, false)
    }
}

[call_macro(name="_sql_try_delete")]
class private SqlTryDeleteMacro : AstCallMacro {
    //! Non-panic sibling of `_sql_delete`. Returns ``Result<int, string>``.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_delete_args(prog, call, rootT)) {
            return null
        }
        return emit_delete_body(prog, call, rootT, true, false)
    }
}

[call_macro(name="_sql_delete_returning")]
class private SqlDeleteReturningMacro : AstCallMacro {
    //! `_sql_delete(...)` with a trailing ``RETURNING *`` clause. Returns
    //! ``array<T>`` of just-deleted rows. Panics on SQL error.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_delete_args(prog, call, rootT)) {
            return null
        }
        return emit_delete_body(prog, call, rootT, false, true)
    }
}

[call_macro(name="_sql_try_delete_returning")]
class private SqlTryDeleteReturningMacro : AstCallMacro {
    //! Non-panic sibling of `_sql_delete_returning`. Returns
    //! ``Result<array<T>, string>``.
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        var rootT : TypeDeclPtr
        if (!validate_outer_delete_args(prog, call, rootT)) {
            return null
        }
        return emit_delete_body(prog, call, rootT, true, true)
    }
}
