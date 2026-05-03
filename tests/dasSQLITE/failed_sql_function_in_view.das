options gen2

// [sql_function(directonly=true)] cannot be used in `_create_view` body —
// SQLite rejects SQLITE_DIRECTONLY functions in views/triggers/CHECK.
// The chain analyzer surfaces this at compile time via pred_fail. Three cases:
//   1. directly in the outer view body's WHERE
//   2. inside a nested subquery (`_in(...)`) within the view body — verifies
//      that `q.inView` propagates into `analyze_subquery`'s sub-`SqlQuery`.
//   3. as a SQL-side computation in `_select` projection — verifies that the
//      directonly+inView guard fires from `try_recognize_sql_function_proj` too.
// Each case is a separate top-level `def` so that `macro_error` from one
// doesn't cascade into the other and inflate the count.
// 30305: free `_` reference in the un-replaced AST after the `_create_view` call_macro
// returns null on the directonly check. The 40104 messages above are the actionable ones.
expect 40104:4, 30305:1

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
}

[sql_view(name="LoudItems")]
struct LoudItem {
    Id   : int
    Name : string
}

[sql_view(name="MarkedItems")]
struct MarkedItem {
    Id   : int
    Name : string
}

[sql_view(name="ShoutedItems")]
struct ShoutedItem {
    Id   : int
    Name : string
}

[sql_function(directonly=true)]
def shout(s : string) : string => s |> to_upper

def case1_outer_where(db : SqlRunner) {
    // shout in the outer view body's WHERE — chain analyzer rejects at compile time.
    db |> _create_view(type<LoudItem>,
        db |> select_from(type<Item>)
            |> _where(shout(_.Name) == "X")
            |> _select((Id = _.Id, Name = _.Name)))
}

def case2_subquery_where(db : SqlRunner) {
    // shout in a NESTED subquery's WHERE within the view body — `q.inView` must
    // propagate into analyze_subquery's sub-SqlQuery, otherwise this slips past
    // macro analysis and fails at SQLite runtime.
    db |> _create_view(type<MarkedItem>,
        db |> select_from(type<Item>)
            |> _where(_.Id._in(
                db |> select_from(type<Item>)
                    |> _where(shout(_.Name) == "X")
                    |> _select(_.Id)))
            |> _select((Id = _.Id, Name = _.Name)))
}

def case3_select_projection(db : SqlRunner) {
    // shout in `_select` projection slot inside the view body — the directonly+inView
    // guard must fire from `try_recognize_sql_function_proj`'s `fn_call_to_sql` call too.
    db |> _create_view(type<ShoutedItem>,
        db |> select_from(type<Item>)
            |> _select((Id = _.Id, Name = shout(_.Name))))
}

[export]
def main {
}
