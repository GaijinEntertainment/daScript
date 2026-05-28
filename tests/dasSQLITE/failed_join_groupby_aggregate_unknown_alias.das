options gen2

// `_join |> _group_by(_.K) |> _select((K=_._0, X=_._1|>_select(_.<bad>)|>sum()))`
// rejects when the aggregate's inner _select references a column that's not in
// the join's `into` projection. The fix path is in try_translate_group_aggregate's
// post-join branch — when the col name isn't found in joinProjRecordNames,
// emit a macro_error listing the available aliases.
//
// macro_error fires but typer cascade adds typer-level errors for the unknown
// tuple field in the inner _select lambda; probe expects all 3 codes.
expect 50503
expect 30831
expect 30341

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
        |> _group_by(_.Brand)
        |> _select((Brand = _._0, Total = _._1 |> _select(_.NotAField) |> sum())))
}
