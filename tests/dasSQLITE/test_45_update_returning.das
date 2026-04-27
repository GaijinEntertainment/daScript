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
    db |> insert(User(Id = 1, Name = "alice", Active = false))
    db |> insert(User(Id = 2, Name = "bob",   Active = false))
    db |> insert(User(Id = 3, Name = "carol", Active = true))
}

[test]
def test_sql_update_returning_returns_post_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let touched <- db |> _sql_update_returning(type<User>, _.Id == 1, (Name = "ALICE", Active = true))
        t |> equal(length(touched), 1, "RETURNING surfaces 1 row")
        t |> equal(touched[0].Id, 1, "RETURNING preserves PK")
        t |> equal(touched[0].Name, "ALICE", "RETURNING reflects post-update Name")
        t |> equal(touched[0].Active, true, "RETURNING reflects post-update Active")
    }
}

[test]
def test_sql_update_returning_multi_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let touched <- db |> _sql_update_returning(type<User>, !_.Active, (Active = true))
        t |> equal(length(touched), 2, "RETURNING surfaces both flipped rows")
        var ids_seen = 0
        for (u in touched) {
            t |> equal(u.Active, true, "every returned row reflects post-update Active=true")
            ids_seen += u.Id
        }
        t |> equal(ids_seen, 3, "Ids 1 + 2 returned")
    }
}

[test]
def test_sql_try_update_returning_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var r <- db |> _sql_try_update_returning(type<User>, _.Id == 3, (Name = "CAROL", Active = false))
        t |> equal(r |> is_ok(), true, "try_update_returning Ok on success")
        let rows <- r._value
        t |> equal(length(rows), 1, "Ok payload contains 1 returned row")
        t |> equal(rows[0].Name, "CAROL", "returned row reflects post-update Name")
    }
}
