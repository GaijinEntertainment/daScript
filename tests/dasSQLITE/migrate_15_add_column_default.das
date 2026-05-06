options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Name : string
    Score : int
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
    db |> exec("INSERT INTO users (Id, Name) VALUES (1, 'Alice')")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    db |> add_column(type<User>, "Score", 0)
}

[test]
def test_int_default_emits_not_null_default_zero(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let notnull = db |> query_scalar(
        "SELECT \"notnull\" FROM pragma_table_info('users') WHERE name='Score'", type<int>)
    t |> equal(notnull, 1, "int field with default must be NOT NULL")
    let dflt = db |> query_scalar(
        "SELECT dflt_value FROM pragma_table_info('users') WHERE name='Score'", type<string>)
    t |> equal(dflt, "0", "DEFAULT 0 must be in the column DDL")
    let stype = db |> query_scalar(
        "SELECT type FROM pragma_table_info('users') WHERE name='Score'", type<string>)
    t |> equal(stype, "INTEGER", "int maps to INTEGER storage")
}

[test]
def test_existing_rows_get_default_value(t : T?) {
    // SQLite ALTER TABLE … ADD COLUMN with DEFAULT backfills pre-existing rows.
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let alice_score = db |> query_scalar("SELECT Score FROM users WHERE Id=1", type<int>)
    t |> equal(alice_score, 0, "Alice (inserted at v1) must read Score=0 after v2 ALTER")
}
