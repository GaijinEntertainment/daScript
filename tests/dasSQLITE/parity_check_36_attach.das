options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 36 — ATTACH DATABASE.
//
// Chain shapes (`select_from + _where`) are unchanged from earlier
// tutorials; the attach-specific concept is the schema-qualified runner
// produced by `with_attached(db, path, name) $(scoped) { ... }` (or
// the manual `attach + with_schema(name)` pair).
//
// Parity claim: a chain run through a schema-qualified runner emits
// `"<schema>"."<table>"` instead of `"<table>"`, but the result rows are
// the same data. M3 (plain array) doesn't go through the attach rail at
// all — it just iterates the source array. So:
//
//   M1 : `_sql(scoped |> chain)` over an attached :memory: DB
//   M2 : `scoped |> chain`       over the same scoped runner
//   M3 : `arr |> chain`          over the array used to populate it
//
// If `with_schema`'s rewriter mangles a chain in a way that changes
// row content, this test catches it.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name   : string
    Active : bool
}

def fixture_users(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(User(Id = 4, Name = "Dave",  Active = false))
}

def fixture_array() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "Alice", Active = true))
    arr |> emplace(User(Id = 2, Name = "Bob",   Active = true))
    arr |> emplace(User(Id = 3, Name = "Carol", Active = false))
    arr |> emplace(User(Id = 4, Name = "Dave",  Active = false))
    return <- arr
}

def user_eq(a : User; b : User) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && a.Active == b.Active)
}

def users_eq(a : array<User>; b : array<User>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!user_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

def sort_by_id(var rows : array<User>) : array<User> {
    rows |> sort() <| $(a, b) {
        return a.Id < b.Id
    }
    return <- rows
}

[test]
def parity_attach_with_attached(t : T?) {
    // _where(_.Active) over a schema-qualified runner. Should produce
    // alice + bob (Id=1, Id=2).
    with_sqlite(":memory:") $(db) {
        var arr <- fixture_array()

        db |> with_attached(":memory:", "tenant") $(scoped) {
            fixture_users(scoped)

            var m1 <- _sql(scoped |> select_from(type<User>) |> _where(_.Active))
            var m2 <- (scoped |> select_from(type<User>) |> _where(_.Active))
            var m3 <- (arr |> _where(_.Active))

            m1 <- sort_by_id(m1)
            m2 <- sort_by_id(m2)
            m3 <- sort_by_id(m3)

            t |> equal(length(m1), 2, "attached _where: 2 active users")
            t |> success(users_eq(m1, m2), "attached _where: M1 ≡ M2")
            t |> success(users_eq(m2, m3), "attached _where: M2 ≡ M3")
        }
    }
}

[test]
def parity_attach_manual(t : T?) {
    // Manual `attach + with_schema + detach` lifecycle. Same parity claim
    // as `with_attached`; just exercises the lower-level surface.
    with_sqlite(":memory:") $(db) {
        var arr <- fixture_array()

        db |> attach(":memory:", "archive")
        db |> with_schema("archive") $(scoped) {
            fixture_users(scoped)

            var m1 <- _sql(scoped |> select_from(type<User>) |> _where(!_.Active))
            var m2 <- (scoped |> select_from(type<User>) |> _where(!_.Active))
            var m3 <- (arr |> _where(!_.Active))

            m1 <- sort_by_id(m1)
            m2 <- sort_by_id(m2)
            m3 <- sort_by_id(m3)

            t |> equal(length(m1), 2, "manual attach _where !Active: 2 inactive users")
            t |> success(users_eq(m1, m2), "manual attach: M1 ≡ M2")
            t |> success(users_eq(m2, m3), "manual attach: M2 ≡ M3")
        }
        db |> detach("archive")
    }
}
