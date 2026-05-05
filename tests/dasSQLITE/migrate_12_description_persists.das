options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

[sql_migration(version = 1, description = "add email column")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[test]
def test_description_lands_in_audit_row(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let desc = db |> query_scalar(
        "SELECT description FROM __schema_version WHERE version=1", type<string>)
    t |> equal(desc, "add email column", "audit row description must match the annotation")
}
