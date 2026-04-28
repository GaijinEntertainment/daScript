options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice"))
    db |> insert(User(Id = 2, Name = "bob"))
    db |> insert(User(Id = 3, Name = "carol"))
}

[test]
def test_changes_after_raw_update(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> exec("UPDATE Users SET Name='X' WHERE Id <= 2")
        t |> equal(db |> changes(), 2, "changes() reports rows-affected from raw exec UPDATE")
    }
}

[test]
def test_changes_after_raw_delete(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> exec("DELETE FROM Users WHERE Id = 1")
        t |> equal(db |> changes(), 1, "changes() reports rows-affected from raw exec DELETE")
    }
}

[test]
def test_changes_after_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> exec("UPDATE Users SET Name='Z' WHERE Id = 999")
        t |> equal(db |> changes(), 0, "changes() reports 0 for no-match predicate")
    }
}
