options gen2

// `_join |> _order_by(_.<not-an-alias>)` rejects with a clear error listing the
// valid projection aliases. collect_order_keys has a post-join branch that consults
// the projection registry and emits a macro_error when the field name isn't found.
//
// macro_error fires but typer cascade adds typer-level errors for the unknown
// tuple field; probe expects all 3 codes.
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
        |> _order_by(_.NotAnAlias))
}
