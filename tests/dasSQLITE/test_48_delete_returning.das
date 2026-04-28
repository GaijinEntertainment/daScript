options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Active : bool
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Active = true))
    db |> insert(User(Id = 2, Name = "bob",   Active = false))
    db |> insert(User(Id = 3, Name = "carol", Active = false))
}

[test]
def test_sql_delete_returning_returns_just_deleted_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let removed <- db |> _sql_delete_returning(type<User>, !_.Active)
        t |> equal(length(removed), 2, "RETURNING surfaces 2 rows that were deleted")
        var ids = 0
        for (u in removed) {
            ids += u.Id
        }
        t |> equal(ids, 5, "removed Ids 2 + 3 (sum=5)")
        let after <- db |> select_from(type<User>)
        t |> equal(length(after), 1, "1 row remains in the table")
    }
}

[test]
def test_sql_delete_returning_empty_when_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let removed <- db |> _sql_delete_returning(type<User>, _.Id == 99)
        t |> equal(length(removed), 0, "RETURNING returns empty array on no-match")
        t |> equal(length(db |> select_from(type<User>)), 3, "table unchanged")
    }
}

[test]
def test_sql_try_delete_returning_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var r <- db |> _sql_try_delete_returning(type<User>, _.Id == 1)
        t |> equal(r |> is_ok(), true, "try_delete_returning Ok on success")
        let rows <- r._value
        t |> equal(length(rows), 1, "Ok payload contains 1 returned row")
        t |> equal(rows[0].Name, "alice", "returned row carries pre-delete column data")
    }
}
