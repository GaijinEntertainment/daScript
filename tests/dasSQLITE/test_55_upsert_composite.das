options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Composite UNIQUE on (Email, Tenant) via [sql_index(unique=true, fields=...)].
[sql_table(name = "UserAccts"),
 sql_index(fields = ("Email", "Tenant"), unique = true, name = "ux_useraccts_email_tenant")]
struct UserAcct {
    @sql_primary_key Id : int
    Email : string
    Name : string
    Tenant : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<UserAcct>)
    db |> insert(UserAcct(Id = 1, Email = "x@y.com", Name = "alice", Tenant = "acme"))
}

[test]
def test_upsert_composite_conflict_fires(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Same (Email, Tenant) → conflict → DO UPDATE
        let n = db |> _sql_upsert(
            UserAcct(Id = 999, Email = "x@y.com", Name = "alice2", Tenant = "acme"),
            tuple(_.Email, _.Tenant),
            (Name = _excluded.Name))
        t |> equal(n, 1, "merge counted 1")
        let name = db |> query_scalar("SELECT Name FROM UserAccts WHERE Email='x@y.com' AND Tenant='acme'", type<string>)
        t |> equal(name, "alice2", "Name updated via _excluded.Name")
        let total = db |> query_scalar("SELECT COUNT(*) FROM UserAccts", type<int>)
        t |> equal(total, 1, "still one row — composite conflict merged into existing")
    }
}

[test]
def test_upsert_composite_no_conflict_inserts(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Different Tenant → no conflict → fresh insert
        db |> _sql_upsert(
            UserAcct(Id = 2, Email = "x@y.com", Name = "alice_other", Tenant = "other"),
            tuple(_.Email, _.Tenant),
            (Name = _excluded.Name))
        let total = db |> query_scalar("SELECT COUNT(*) FROM UserAccts", type<int>)
        t |> equal(total, 2, "two rows — different Tenant doesn't conflict")
    }
}
