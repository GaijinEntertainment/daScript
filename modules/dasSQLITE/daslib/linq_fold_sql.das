options gen2
options indenting = 4
options _comment_hygiene = true

module linq_fold_sql shared public

//! linq_fold SQL source pass-through: ``extract_sql_source``. Lets ``_fold`` over a
//! ``db |> select_from(type<Row>)`` chain (where ``Row`` is a ``[sql_table]``/``[sql_view]``/
//! ``[sql_fts5]`` struct) re-dispatch the whole chain to the ``_sql`` macro instead of folding the
//! materialized full-table ``array<Row>`` in memory. There is no emitting adapter — ``_sql`` is a
//! complete parallel lowering; this module only recognizes the source. Optional companion to
//! ``daslib/linq_fold`` — pulled in via ``require ?sqlite`` and gated by
//! ``static_if (typeinfo builtin_module_exists(sqlite))``. See daslib/linq_fold.md.

require daslib/linq_fold_common public   // get_call_short_name + AST types
require daslib/ast_boost

def private is_sql_table_select(c : ExprCall?) : bool {
    // Gate on a SQL-table annotation so an unrelated `select_from`-named function is never mistaken for SQL.
    if (c._type == null || !c._type.isGoodArrayType || c._type.firstType == null) return false
    let st = c._type.firstType.structType
    if (st == null) return false
    for (annDecl in st.annotations) {
        if (annDecl.annotation.name == "sql_table" || annDecl.annotation.name == "sql_view"
                || annDecl.annotation.name == "sql_fts5") return true
    }
    return false
}

// Walk the chain's `arguments[0]` (piped-source) spine to its bottom, looking for a `[sql_table]`
// `select_from`. Not linq_fold's flattened `top`: the SQL DSL ops (`_where`/`_select`/`_join`/...) aren't
// in `linqCalls`, so flatten stops at the first one and never reaches the source. Returns it, or null.
[macro_function]
def extract_sql_source(var expr : Expression?) : ExprCall? {
    var e = expr
    while (e != null && e is ExprCall) {
        var c = e as ExprCall
        if (get_call_short_name(c) == "select_from") {
            return is_sql_table_select(c) ? c : null
        }
        if (empty(c.arguments)) break
        e = c.arguments[0]
    }
    return null
}
