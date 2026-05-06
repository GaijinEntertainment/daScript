options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// `convert_and_rename` requires source and target structs to share the same
// ``[sql_table(name=...)]`` (legacy/current convention). Mismatched names would
// silently SELECT from the source table but DROP/RENAME the target — a footgun.
// The runtime guard panics with a clear message pointing at the proper escape
// hatch (``convert(type<S>, type<T>, name=...)``) for cross-table conversions.

[sql_table(name = "items_old", legacy = true)]
struct ItemV1 {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
}

[struct_convert]
def m_v1_to_v2(old : ItemV1; var dst : Item) {}

[test]
def test_convert_and_rename_panics_on_mismatched_table_names(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> exec("CREATE TABLE items_old (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
    var panicked = false
    try {
        db |> convert_and_rename(type<ItemV1>, type<Item>)
    } recover {
        panicked = true
    }
    t |> success(panicked,
        "convert_and_rename must panic when source/target [sql_table(name=...)] disagree")
}
