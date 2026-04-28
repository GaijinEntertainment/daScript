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
def test_insert_or_ignore_inserts_new_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_ignore(User(Id = 2, Email = "b@x.com", Name = "bob"))
        t |> equal(n, 1, "fresh PK insert returns 1 row affected")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 2, "row count after fresh insert")
    }
}

[test]
def test_insert_or_ignore_silently_ignores_pk_conflict(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_ignore(User(Id = 1, Email = "z@x.com", Name = "zoe"))
        t |> equal(n, 0, "PK conflict returns 0 rows affected")
        let name = db |> query_scalar("SELECT Name FROM Users WHERE Id = 1", type<string>)
        t |> equal(name, "alice", "original row left untouched by IGNORE")
    }
}

[test]
def test_insert_or_ignore_bulk_partitions_by_conflict(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> insert_or_ignore([
            User(Id = 1, Email = "ignored@x.com", Name = "dup"),
            User(Id = 2, Email = "b@x.com", Name = "bob"),
            User(Id = 3, Email = "c@x.com", Name = "cat")
        ])
        t |> equal(n, 2, "two of three rows actually inserted")
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 3, "row count: original alice + bob + cat")
    }
}

[test]
def test_try_insert_or_ignore_ok_path(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_insert_or_ignore(User(Id = 1, Email = "z@x.com", Name = "zoe"))
        t |> success(r |> is_ok, "try_insert_or_ignore on conflict still returns Ok")
        t |> equal(r |> unwrap, 0, "rows-affected = 0 because IGNORE silenced the conflict")
    }
}
