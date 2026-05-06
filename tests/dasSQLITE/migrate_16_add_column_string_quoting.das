options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Name : string
    Author : string
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
    db |> exec("INSERT INTO users (Id, Name) VALUES (1, 'Alice')")
}

[sql_migration(version = 2)]
def m_002(db : SqlRunner) {
    // Embedded single-quote must be escaped as '' in the emitted DDL.
    db |> add_column(type<User>, "Author", "O'Reilly")
}

[test]
def test_single_quote_escapes_in_default(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let alice_author = db |> query_scalar("SELECT Author FROM users WHERE Id=1", type<string>)
    t |> equal(alice_author, "O'Reilly", "Embedded single-quote must round-trip via escape")
}

[test]
def test_default_clause_in_pragma(t : T?) {
    var inscope db = open_sqlite(":memory:")
    db |> migrate_to_latest()
    let dflt = db |> query_scalar(
        "SELECT dflt_value FROM pragma_table_info('users') WHERE name='Author'", type<string>)
    // PRAGMA reports the literal as it appears in the DDL: a SQL-quoted string.
    t |> equal(dflt, "'O''Reilly'", "DEFAULT must show SQL-quoted form with doubled single-quote")
}
