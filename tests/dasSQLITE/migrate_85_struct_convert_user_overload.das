options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// User-side overload extension: declare ``struct_convert_field`` for ``int → string`` in
// your module and the [struct_convert] macro picks it up automatically — your overload
// shadows the shipped ``string ← auto(S)`` catch-all because it's more specific.

[sql_table(name = "items", legacy = true)]
struct ItemV1 {
    @sql_primary_key Id : int
    Score : int
}

[sql_table(name = "items")]
struct Item {
    @sql_primary_key Id : int
    Score : string
}

// User-side dispatch overload — formats the int with a domain-specific prefix instead of
// the default ``"{src}"`` interp. The macro's ``_::`` resolves at user's module, so this
// overload wins for the int→string field of [struct_convert].
def struct_convert_field(var dst : string&; src : int) : void {
    dst = "score={src}"
}

[struct_convert]
def m_v1_to_v2(old : ItemV1; var dst : Item) {}

[test]
def test_user_overload_extends_dispatch(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE items (Id INTEGER PRIMARY KEY, Score INTEGER NOT NULL)")
    db |> exec("INSERT INTO items (Id, Score) VALUES (1, 42)")
    db |> convert_and_rename(type<ItemV1>, type<Item>)
    let landed = db |> query_scalar("SELECT Score FROM items WHERE Id=1", type<string>)
    t |> equal(landed, "score=42", "user-defined overload wins over shipped string<-auto(S) catch-all")
}
