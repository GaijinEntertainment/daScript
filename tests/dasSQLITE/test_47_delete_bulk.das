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
    db |> insert(User(Id = 4, Name = "dave",  Active = true))
}

[test]
def test_sql_delete_with_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> _sql_delete(type<User>, !_.Active)
        t |> equal(n, 2, "removed both inactive rows")
        let after <- db |> select_from(type<User>)
        t |> equal(length(after), 2, "table has 2 rows left")
        for (u in after) {
            t |> equal(u.Active, true, "all surviving rows are active")
        }
    }
}

[test]
def test_sql_delete_with_captured_var(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cutoff_id = 2
        let n = db |> _sql_delete(type<User>, _.Id <= cutoff_id)
        t |> equal(n, 2, "removed rows with Id<=2")
    }
}

[test]
def test_sql_try_delete_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> _sql_try_delete(type<User>, _.Id == 999)
        t |> equal(r |> is_ok(), true, "try_delete returns Ok on no-match")
        t |> equal(r |> unwrap(), 0, "Ok payload is 0 for no-match predicate")
    }
}

[test]
def test_changes_after_delete(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> _sql_delete(type<User>, _.Id < 3)
        t |> equal(n, 2, "deleted 2 rows")
        t |> equal(db |> changes(), 2, "changes() reports last DELETE's rows-affected")
    }
}
