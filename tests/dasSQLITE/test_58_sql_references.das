options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require daslib/option

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int

    @sql_references = "User"
    @sql_on_delete = "cascade"
    UserId : int

    Total : float
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "alice"))
    db |> insert(User(Id = 2, Name = "bob"))
    db |> insert(Order(Id = 100, UserId = 1, Total = 42.0f))
    db |> insert(Order(Id = 101, UserId = 1, Total = 99.0f))
    db |> insert(Order(Id = 102, UserId = 2, Total = 7.5f))
}

[test]
def test_references_emitted_in_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Order>)
    t |> success(find(ddl, "REFERENCES \"Users\"(\"Id\") ON DELETE CASCADE ON UPDATE NO ACTION") >= 0,
                 "@sql_references emits the FK clause with the parent's PK column resolved")
}

[test]
def test_pragma_foreign_keys_on(t : T?) {
    with_sqlite(":memory:") $(db) {
        let pragma = db |> query_scalar("PRAGMA foreign_keys", type<int>)
        t |> equal(pragma, 1, "with_sqlite turns FKs on")
    }
}

[test]
def test_cascade_delete_propagates(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> exec("DELETE FROM Users WHERE Id = 1")
        let alice_orders = db |> query_scalar("SELECT COUNT(*) FROM Orders WHERE UserId = 1", type<int>)
        t |> equal(alice_orders, 0, "alice's orders cascade-deleted")
        let bob_orders = db |> query_scalar("SELECT COUNT(*) FROM Orders WHERE UserId = 2", type<int>)
        t |> equal(bob_orders, 1, "bob's orders untouched")
    }
}

[test]
def test_orphan_insert_rejected(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_insert(Order(Id = 999, UserId = 9999, Total = 1.0f))
        t |> success(r |> is_err, "FK violation on missing parent row")
    }
}
