options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// [struct_convert] dispatches to overloaded ``struct_convert_field``. This test exercises the
// numeric/string conversion overloads — int↔int64, float↔double, string→numeric, numeric→string.

[sql_table(name = "items", legacy = true)]
struct ItemV1 {
    @sql_primary_key Id : int
    Count : int          // → int64 (widening)
    Score : int64        // → int   (narrowing — silent, user opted in)
    Ratio : float        // → double (widening)
    Avg : double         // → float (narrowing)
    LabelInt : int       // → string (numeric → string)
    PriceStr : string    // → double (string → double parse)
}

[sql_table(name = "items")]
struct Item {
    @sql_primary_key Id : int
    Count : int64
    Score : int
    Ratio : double
    Avg : float
    LabelInt : string
    PriceStr : double
}

[struct_convert]
def convert_item_v1_to_v2(old : ItemV1; var dst : Item) {}

[test]
def test_primitive_casts_through_dispatch(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE items (Id INTEGER PRIMARY KEY, Count INTEGER NOT NULL, Score INTEGER NOT NULL, Ratio REAL NOT NULL, Avg REAL NOT NULL, LabelInt INTEGER NOT NULL, PriceStr TEXT NOT NULL)")
    db |> exec("INSERT INTO items (Id, Count, Score, Ratio, Avg, LabelInt, PriceStr) VALUES (1, 42, 9999, 1.5, 2.75, 7, '3.14')")
    db |> convert_and_rename(type<ItemV1>, type<Item>)
    let count_int64 = db |> query_scalar("SELECT Count FROM items WHERE Id=1", type<int64>)
    t |> equal(count_int64, 42l, "int → int64 widening preserved")
    let score_int = db |> query_scalar("SELECT Score FROM items WHERE Id=1", type<int>)
    t |> equal(score_int, 9999, "int64 → int narrowing preserved (within range)")
    let ratio_double = db |> query_scalar("SELECT Ratio FROM items WHERE Id=1", type<double>)
    t |> equal(ratio_double, 1.5lf, "float → double widening preserved")
    let avg_float = db |> query_scalar("SELECT Avg FROM items WHERE Id=1", type<float>)
    t |> equal(avg_float, 2.75f, "double → float narrowing preserved")
    let label_str = db |> query_scalar("SELECT LabelInt FROM items WHERE Id=1", type<string>)
    t |> equal(label_str, "7", "int → string interp")
    let price_double = db |> query_scalar("SELECT PriceStr FROM items WHERE Id=1", type<double>)
    t |> equal(price_double, 3.14lf, "string → double parse")
}
