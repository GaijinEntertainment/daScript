options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    @sql_unique Email : string
    Name : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Email = "a@x.com", Name = "alice"))
}

[test]
def test_unique_emitted_in_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<User>)
    t |> success(ddl |> ends_with("\"Email\" TEXT NOT NULL UNIQUE, \"Name\" TEXT NOT NULL)"),
                 "@sql_unique appends UNIQUE to the column declaration")
}

[test]
def test_unique_constraint_rejects_duplicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_insert(User(Id = 2, Email = "a@x.com", Name = "anne"))
        t |> success(r |> is_err, "duplicate Email should violate UNIQUE")
    }
}

[test]
def test_unique_does_not_block_distinct_emails(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> insert(User(Id = 2, Email = "b@x.com", Name = "bob"))
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 2, "different Email values insert fine")
    }
}
