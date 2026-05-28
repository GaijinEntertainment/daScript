options gen2

// `_join |> _group_by(_.<not-an-alias>)` rejects with a clear error message
// listing the valid projection aliases. The fix path lives in push_group_key's
// post-join branch (sqlite_linq.das): when `q.seenJoin` is true and the
// `_.<field>` name is not in `q.joinProjRecordNames`, emit a macro_error.
expect 50503

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
