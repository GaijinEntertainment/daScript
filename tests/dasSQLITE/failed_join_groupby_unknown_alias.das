options gen2

// `_join |> _group_by(_.<not-an-alias>)` rejects with a clear error message
// listing the valid projection aliases. The fix path lives in push_group_key's
// post-join branch (sqlite_linq.das): when `q.seenJoin` is true and the
// `_.<field>` name is not in `q.joinProjRecordNames`, emit a macro_error.
//
// The macro_error fires but the typer continues expanding the chain because
// `_sql_text` returns null when its macro errors out — `_._0`/`_._1` in the
// trailing `_select` then cascade to additional typer errors (linq generic
// instantiation for group_by_lazy with an invalid tuple). Probe expects the
// full cascade so the test is stable across linq.das internal changes.
expect 50503
expect 50503
expect 30831
expect 30341
expect 30838
expect 30838
expect 30808
expect 30808

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Price : int
    DealerId : int
}

[sql_table(name = "Dealers")]
struct Dealer {
    @sql_primary_key Id : int
    Brand : string
}

[export]
def main() {
    var _db = SqlRunner()
    let _ = _sql_text(_db |> select_from(type<Car>)
        |> _join(_db |> select_from(type<Dealer>),
                 $(c : Car, d : Dealer) => c.DealerId == d.Id,
                 $(c : Car, d : Dealer) => (Brand = d.Brand, Price = c.Price))
        |> _group_by(_.NotAnAlias)
        |> _select((K = _._0, N = _._1 |> count())))
}
