options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "v1")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[sql_migration(version = 2, description = "v2")]
def m2(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

[sql_migration(version = 3, description = "v3")]
def m3(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Score INTEGER NOT NULL DEFAULT 0")
}

[test]
def test_baseline_stamps_v1_through_n(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let r <- db |> try_baseline(3)
    t |> success(r |> is_ok, "try_baseline(3) must succeed")
    t |> equal(r |> unwrap, 3, "stamped 3 rows (v1 + v2 + v3)")

    let h <- db |> migration_history()
    t |> equal(h |> length, 3, "history has 3 rows")
    t |> equal(h[0].version, 1, "first row is v1")
    t |> equal(h[2].version, 3, "third row is v3")
}

[test]
def test_baseline_uses_annotation_description_when_available(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> baseline(2)
    let desc = db |> query_scalar(
        "SELECT description FROM __schema_version WHERE version=1", type<string>)
    t |> equal(desc, "v1", "baseline picks up the annotation description for registered versions")
}
