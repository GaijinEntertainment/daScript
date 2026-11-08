options gen2
//! a global aggregate in a position SQL cannot express must not
//! compile

expect 50503:9

require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

[sql_table(name = "GlobalAggregateRows")]
struct GlobalAggregateRow {
    @sql_primary_key Id : int
    Amount : int
}

[export]
def main() {
    var db = SqlRunner()

    // Result must be a named tuple.
    let bad1 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> _aggregate($(_rows) => _rows |> count))
    let bad2 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> _aggregate($(_rows) => (
            _rows |> count,
            _rows |> _select(_.Amount) |> sum)))

    // Every tuple slot must be an aggregate over the block's rowset.
    let bad3 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> _aggregate($(_rows) => (N = 1)))

    var captured = GlobalAggregateRow(Id = 0, Amount = 10)
    let bad4 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> _aggregate($(_rows) => (
            Total = _rows
                |> _select($(row : GlobalAggregateRow) => captured.Amount)
                |> sum)))

    // V1 aggregates the filtered source directly; pre-projection and
    // DISTINCT need a separate inner-query composition rail.
    let bad5 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> _select(_.Amount)
        |> _aggregate($(_rows) => (N = _rows |> count)))
    let bad6 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> distinct()
        |> _aggregate($(_rows) => (N = _rows |> count)))
    let bad7 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> _group_by(_.Amount)
        |> _aggregate($(_rows) => (N = _rows |> count)))
    let bad8 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> _order_by(_.Amount)
        |> _aggregate($(_rows) => (N = _rows |> count)))
    let bad9 = _sql(db |> select_from(type<GlobalAggregateRow>)
        |> take(10)
        |> _aggregate($(_rows) => (N = _rows |> count)))
}
