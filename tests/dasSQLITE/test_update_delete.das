options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

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

[test(description = "update by primary key rewrites the row and reports one row affected")]
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

[test(description = "update by primary key on a missing id returns 0 rather than an error")]
def test_update_by_pk_missing_returns_zero(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> update(User(Id = 999, Name = "ghost", Active = false))
        t |> equal(n, 0, "by-PK update on missing PK returns 0 (not an error)")
    }
}

[test(description = "try_update returns Ok carrying the rows-affected count")]
def test_try_update_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_update(User(Id = 2, Name = "BOB", Active = false))
        t |> equal(r |> is_ok(), true, "try_update returns Ok on success")
        t |> equal(r |> unwrap(), 1, "try_update Ok payload is rows-affected")
    }
}

[test(description = "changes reports the rows affected by the last update")]
def test_changes_after_update(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> update(User(Id = 1, Name = "alice2", Active = true))
        t |> equal(db |> changes(), 1, "changes() reports last UPDATE's rows-affected")
    }
}

[sql_table(name = "Users")]
struct User_44 {
    @sql_primary_key Id : int
    Name : string
    Active : bool
    LastSeen : int64
}

def private fixture_44(db : SqlRunner) {
    db |> create_table(type<User_44>)
    db |> insert(User_44(Id = 1, Name = "alice", Active = true,  LastSeen = 100l))
    db |> insert(User_44(Id = 2, Name = "bob",   Active = true,  LastSeen = 200l))
    db |> insert(User_44(Id = 3, Name = "carol", Active = true,  LastSeen = 300l))
    db |> insert(User_44(Id = 4, Name = "dave",  Active = false, LastSeen = 400l))
}

[test(description = "a predicate update with a literal SET changes exactly the matching rows")]
def test_sql_update_with_predicate_and_set_literal(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        let cutoff : int64 = 250l
        let n = db |> _sql_update(type<User_44>, _.LastSeen < cutoff, (Active = false))
        t |> equal(n, 2, "rows with LastSeen<250 (alice, bob) are matched")
        var alice_active = true
        var carol_active = true
        for (u in db |> select_from(type<User_44>)) {
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

[test(description = "a SET value taken from a captured variable is bound into the row")]
def test_sql_update_set_uses_captured_var(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        let new_seen : int64 = 999l
        let n = db |> _sql_update(type<User_44>, _.Id == 1, (LastSeen = new_seen))
        t |> equal(n, 1, "single-row update by PK predicate")
        for (u in db |> select_from(type<User_44>)) {
            if (u.Id == 1) {
                t |> equal(u.LastSeen, 999l, "captured-var bind value lands in row")
            }
        }
    }
}

[test(description = "a predicate try_update with no match returns Ok with 0 rows affected")]
def test_sql_try_update_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        let r = db |> _sql_try_update(type<User_44>, _.Id == 99, (Name = "missing"))
        t |> equal(r |> is_ok(), true, "_sql_try_update returns Ok even when no rows match")
        t |> equal(r |> unwrap(), 0, "Ok payload is 0 rows-affected for no-match predicate")
    }
}

[test(description = "an update setting two columns changes both on every matched row")]
def test_sql_update_two_set_columns(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        let touch : int64 = 555l
        let n = db |> _sql_update(type<User_44>, _.Active, (Name = "marker", LastSeen = touch))
        t |> equal(n, 3, "three rows matched _.Active")
        var marker_count = 0
        for (u in db |> select_from(type<User_44>)) {
            if (u.Name == "marker") {
                marker_count++
                t |> equal(u.LastSeen, 555l, "second SET column also lands")
            }
        }
        t |> equal(marker_count, 3, "all matched rows updated both columns")
    }
}

def private fixture_45(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Active = false))
    db |> insert(User(Id = 2, Name = "bob",   Active = false))
    db |> insert(User(Id = 3, Name = "carol", Active = true))
}

[test(description = "update with RETURNING gives back the post-update row")]
def test_sql_update_returning_returns_post_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_45(db)
        let touched <- db |> _sql_update_returning(type<User>, _.Id == 1, (Name = "ALICE", Active = true))
        t |> equal(length(touched), 1, "RETURNING surfaces 1 row")
        t |> equal(touched[0].Id, 1, "RETURNING preserves PK")
        t |> equal(touched[0].Name, "ALICE", "RETURNING reflects post-update Name")
        t |> equal(touched[0].Active, true, "RETURNING reflects post-update Active")
    }
}

[test(description = "update with RETURNING gives back every updated row in its post-update state")]
def test_sql_update_returning_multi_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_45(db)
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

[test(description = "try_update_returning returns Ok with the updated rows")]
def test_sql_try_update_returning_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_45(db)
        var r <- db |> _sql_try_update_returning(type<User>, _.Id == 3, (Name = "CAROL", Active = false))
        t |> equal(r |> is_ok(), true, "try_update_returning Ok on success")
        let rows <- move_unwrap(r)
        t |> equal(length(rows), 1, "Ok payload contains 1 returned row")
        t |> equal(rows[0].Name, "CAROL", "returned row reflects post-update Name")
    }
}

[sql_table(name = "Users")]
struct User_46 {
    @sql_primary_key Id : int
    Name : string
}

def private fixture_46(db : SqlRunner) {
    db |> create_table(type<User_46>)
    db |> insert(User_46(Id = 1, Name = "alice"))
    db |> insert(User_46(Id = 2, Name = "bob"))
    db |> insert(User_46(Id = 3, Name = "carol"))
}

[test(description = "delete_ by row removes exactly the row with that primary key")]
def test_delete_underscore_by_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_46(db)
        let n = db |> delete_(User_46(Id = 2, Name = ""))
        t |> equal(n, 1, "delete_ by row removes 1 matching row")
        let after <- db |> select_from(type<User_46>)
        t |> equal(length(after), 2, "table now holds 2 rows")
        for (u in after) {
            t |> equal(u.Id != 2, true, "Id 2 was removed")
        }
    }
}

[test(description = "delete_ of a missing primary key returns 0 rather than an error")]
def test_delete_underscore_missing(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_46(db)
        let n = db |> delete_(User_46(Id = 999, Name = ""))
        t |> equal(n, 0, "delete_ on missing PK returns 0 (not an error)")
    }
}

[test(description = "delete_by_id removes the row when the primary key matches")]
def test_delete_by_id_basic(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_46(db)
        let n = db |> delete_by_id(type<User_46>, 3)
        t |> equal(n, 1, "delete_by_id removes 1 row when PK matches")
    }
}

[test(description = "try_delete_by_id on a missing id returns Ok with 0 rows affected")]
def test_try_delete_by_id_missing(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_46(db)
        let r = db |> try_delete_by_id(type<User_46>, 99)
        t |> equal(r |> is_ok(), true, "try_delete_by_id returns Ok on no-match")
        t |> equal(r |> unwrap(), 0, "Ok payload reports 0 rows-affected for no-match")
    }
}

def private fixture_47(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Active = true))
    db |> insert(User(Id = 2, Name = "bob",   Active = false))
    db |> insert(User(Id = 3, Name = "carol", Active = false))
    db |> insert(User(Id = 4, Name = "dave",  Active = true))
}

[test(description = "a predicate delete removes exactly the matching rows")]
def test_sql_delete_with_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_47(db)
        let n = db |> _sql_delete(type<User>, !_.Active)
        t |> equal(n, 2, "removed both inactive rows")
        let after <- db |> select_from(type<User>)
        t |> equal(length(after), 2, "table has 2 rows left")
        for (u in after) {
            t |> equal(u.Active, true, "all surviving rows are active")
        }
    }
}

[test(description = "a predicate delete compares against a captured variable")]
def test_sql_delete_with_captured_var(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_47(db)
        let cutoff_id = 2
        let n = db |> _sql_delete(type<User>, _.Id <= cutoff_id)
        t |> equal(n, 2, "removed rows with Id<=2")
    }
}

[test(description = "a predicate try_delete with no match returns Ok with 0 rows affected")]
def test_sql_try_delete_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_47(db)
        let r = db |> _sql_try_delete(type<User>, _.Id == 999)
        t |> equal(r |> is_ok(), true, "try_delete returns Ok on no-match")
        t |> equal(r |> unwrap(), 0, "Ok payload is 0 for no-match predicate")
    }
}

[test(description = "changes reports the rows affected by the last delete")]
def test_changes_after_delete(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_47(db)
        let n = db |> _sql_delete(type<User>, _.Id < 3)
        t |> equal(n, 2, "deleted 2 rows")
        t |> equal(db |> changes(), 2, "changes() reports last DELETE's rows-affected")
    }
}

def private fixture_48(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "alice", Active = true))
    db |> insert(User(Id = 2, Name = "bob",   Active = false))
    db |> insert(User(Id = 3, Name = "carol", Active = false))
}

[test(description = "delete with RETURNING gives back exactly the deleted rows")]
def test_sql_delete_returning_returns_just_deleted_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_48(db)
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

[test(description = "delete with RETURNING on no match returns an empty array and leaves the table unchanged")]
def test_sql_delete_returning_empty_when_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_48(db)
        let removed <- db |> _sql_delete_returning(type<User>, _.Id == 99)
        t |> equal(length(removed), 0, "RETURNING returns empty array on no-match")
        t |> equal(length(db |> select_from(type<User>)), 3, "table unchanged")
    }
}

[test(description = "try_delete_returning returns Ok with the rows as they were before deletion")]
def test_sql_try_delete_returning_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_48(db)
        var r <- db |> _sql_try_delete_returning(type<User>, _.Id == 1)
        t |> equal(r |> is_ok(), true, "try_delete_returning Ok on success")
        let rows <- move_unwrap(r)
        t |> equal(length(rows), 1, "Ok payload contains 1 returned row")
        t |> equal(rows[0].Name, "alice", "returned row carries pre-delete column data")
    }
}

[sql_table(name = "Users")]
struct User_49 {
    @sql_primary_key Id : int
    Name : string
}

def private fixture_49(db : SqlRunner) {
    db |> create_table(type<User_49>)
    db |> insert(User_49(Id = 1, Name = "alice"))
    db |> insert(User_49(Id = 2, Name = "bob"))
    db |> insert(User_49(Id = 3, Name = "carol"))
}

[test(description = "changes reports the rows affected by a raw exec UPDATE")]
def test_changes_after_raw_update(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_49(db)
        db |> exec("UPDATE Users SET Name='X' WHERE Id <= 2")
        t |> equal(db |> changes(), 2, "changes() reports rows-affected from raw exec UPDATE")
    }
}

[test(description = "changes reports the rows affected by a raw exec DELETE")]
def test_changes_after_raw_delete(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_49(db)
        db |> exec("DELETE FROM Users WHERE Id = 1")
        t |> equal(db |> changes(), 1, "changes() reports rows-affected from raw exec DELETE")
    }
}

[test(description = "changes reports 0 after a statement that matched nothing")]
def test_changes_after_no_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_49(db)
        db |> exec("UPDATE Users SET Name='Z' WHERE Id = 999")
        t |> equal(db |> changes(), 0, "changes() reports 0 for no-match predicate")
    }
}
