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
    db |> insert(User(Id = 2, Name = "bob",   Active = true))
    db |> insert(User(Id = 3, Name = "carol", Active = false))
}

[test]
def test_update_by_pk_changes_columns(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> update(User(Id = 1, Name = "ALICE", Active = false))
        t |> equal(n, 1, "by-PK update on existing row returns rows-affected = 1")
        let after <- db |> select_from(type<User>)
        var found_alice = false
        for (u in after) {
            if (u.Id == 1) {
                found_alice = true
                t |> equal(u.Name, "ALICE", "Name updated on PK match")
                t |> equal(u.Active, false, "Active updated on PK match")
            }
        }
        t |> equal(found_alice, true, "row with PK 1 is still present after update")
    }
}

[test]
def test_update_by_pk_missing_returns_zero(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> update(User(Id = 999, Name = "ghost", Active = false))
        t |> equal(n, 0, "by-PK update on missing PK returns 0 (not an error)")
    }
}

[test]
def test_try_update_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_update(User(Id = 2, Name = "BOB", Active = false))
        t |> equal(r |> is_ok(), true, "try_update returns Ok on success")
        t |> equal(r |> unwrap(), 1, "try_update Ok payload is rows-affected")
    }
}

[test]
def test_changes_after_update(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> update(User(Id = 1, Name = "alice2", Active = true))
        t |> equal(db |> changes(), 1, "changes() reports last UPDATE's rows-affected")
    }
}
