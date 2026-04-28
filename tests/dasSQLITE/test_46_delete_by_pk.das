options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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
def test_delete_underscore_by_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> delete_(User(Id = 2, Name = ""))
        t |> equal(n, 1, "delete_ by row removes 1 matching row")
        let after <- db |> select_from(type<User>)
        t |> equal(length(after), 2, "table now holds 2 rows")
        for (u in after) {
            t |> equal(u.Id != 2, true, "Id 2 was removed")
        }
    }
}

[test]
def test_delete_underscore_missing(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> delete_(User(Id = 999, Name = ""))
        t |> equal(n, 0, "delete_ on missing PK returns 0 (not an error)")
    }
}

[test]
def test_delete_by_id_basic(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> delete_by_id(type<User>, 3)
        t |> equal(n, 1, "delete_by_id removes 1 row when PK matches")
    }
}

[test]
def test_try_delete_by_id_missing(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_delete_by_id(type<User>, 99)
        t |> equal(r |> is_ok(), true, "try_delete_by_id returns Ok on no-match")
        t |> equal(r |> unwrap(), 0, "Ok payload reports 0 rows-affected for no-match")
    }
}
