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
    LastSeen : int64
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Active = true,  LastSeen = 100l))
    db |> insert(User(Id = 2, Name = "bob",   Active = true,  LastSeen = 200l))
    db |> insert(User(Id = 3, Name = "carol", Active = true,  LastSeen = 300l))
    db |> insert(User(Id = 4, Name = "dave",  Active = false, LastSeen = 400l))
}

[test]
def test_sql_update_with_predicate_and_set_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cutoff : int64 = 250l
        let n = db |> _sql_update(type<User>, _.LastSeen < cutoff, (Active = false))
        t |> equal(n, 2, "rows with LastSeen<250 (alice, bob) are matched")
        var alice_active = true
        var carol_active = true
        for (u in db |> select_from(type<User>)) {
            if (u.Id == 1) {
                alice_active = u.Active
            }
            if (u.Id == 3) {
                carol_active = u.Active
            }
        }
        t |> equal(alice_active, false, "alice flipped to Active=false")
        t |> equal(carol_active, true, "carol untouched (LastSeen >= cutoff)")
    }
}

[test]
def test_sql_update_set_uses_captured_var(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let new_seen : int64 = 999l
        let n = db |> _sql_update(type<User>, _.Id == 1, (LastSeen = new_seen))
        t |> equal(n, 1, "single-row update by PK predicate")
        for (u in db |> select_from(type<User>)) {
            if (u.Id == 1) {
                t |> equal(u.LastSeen, 999l, "captured-var bind value lands in row")
            }
        }
    }
}

[test]
def test_sql_try_update_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> _sql_try_update(type<User>, _.Id == 99, (Name = "missing"))
        t |> equal(r |> is_ok(), true, "_sql_try_update returns Ok even when no rows match")
        t |> equal(r |> unwrap(), 0, "Ok payload is 0 rows-affected for no-match predicate")
    }
}

[test]
def test_sql_update_two_set_columns(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let touch : int64 = 555l
        let n = db |> _sql_update(type<User>, _.Active, (Name = "marker", LastSeen = touch))
        t |> equal(n, 3, "three rows matched _.Active")
        var marker_count = 0
        for (u in db |> select_from(type<User>)) {
            if (u.Name == "marker") {
                marker_count++
                t |> equal(u.LastSeen, 555l, "second SET column also lands")
            }
        }
        t |> equal(marker_count, 3, "all matched rows updated both columns")
    }
}
