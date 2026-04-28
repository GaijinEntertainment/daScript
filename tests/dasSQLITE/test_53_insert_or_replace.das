options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Email : string
    Name : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Email = "a@x.com", Name = "alice"))
}

[test]
def test_insert_or_replace_overwrites_existing(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_replace(User(Id = 1, Email = "a2@x.com", Name = "alice2"))
        t |> equal(n, 1, "replace returns 1 row affected")
        let name = db |> query_scalar("SELECT Name FROM Users WHERE Id = 1", type<string>)
        t |> equal(name, "alice2", "row was overwritten")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 1, "no duplicate row created")
    }
}

[test]
def test_insert_or_replace_inserts_when_pk_free(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_replace(User(Id = 5, Email = "e@x.com", Name = "eve"))
        t |> equal(n, 1, "fresh PK gives 1 row affected")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 2, "two rows now")
    }
}

[test]
def test_insert_or_replace_bulk(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_replace([
            User(Id = 1, Email = "a3@x.com", Name = "alice3"),
            User(Id = 2, Email = "b@x.com", Name = "bob")
        ])
        t |> equal(n, 2, "both rows applied (one replace, one insert)")
        let alice_name = db |> query_scalar("SELECT Name FROM Users WHERE Id = 1", type<string>)
        t |> equal(alice_name, "alice3", "row 1 replaced")
    }
}

[test]
def test_try_insert_or_replace_returns_result(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_insert_or_replace(User(Id = 1, Email = "ar@x.com", Name = "alice_r"))
        t |> success(r |> is_ok, "ok path")
        t |> equal(r |> unwrap, 1, "1 row affected")
    }
}
