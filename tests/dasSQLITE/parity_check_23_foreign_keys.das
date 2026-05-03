options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/option
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 23 — foreign keys.
//
// FK cascade / set_null are DDL/runtime SQLite behavior with no plain-linq
// analog (plain arrays have no FK concept). This file verifies only the
// CHAIN SHAPES introduced by the tutorial:
//   - `_where(...) |> count()`     count after filter
//   - `_to_array()`                terminal that materializes the chain
//
// Both ride on chains already covered in earlier tutorials but with the
// specific shape tutorial 23 demonstrates. Fixture is intentionally
// FK-free — the parity claim is about the chain emission, not the
// DDL-driven cascade behavior.

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : float
}

[sql_table(name = "Comments")]
struct Comment {
    @sql_primary_key Id : int
    @safe_when_uninitialized AuthorId : Option<int>
    Body : string
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Order>)
    db |> create_table(type<Comment>)
    db |> insert(Order(Id = 100, UserId = 1, Total = 42.0f))
    db |> insert(Order(Id = 101, UserId = 1, Total = 99.0f))
    db |> insert(Order(Id = 102, UserId = 2, Total =  7.5f))
    db |> insert(Comment(Id = 1000, AuthorId = none(type<int>), Body = "alice's"))
    db |> insert(Comment(Id = 1001, AuthorId = none(type<int>), Body = "bob's"))
}

def fixture_orders() : array<Order> {
    var arr : array<Order>
    arr |> emplace(Order(Id = 100, UserId = 1, Total = 42.0f))
    arr |> emplace(Order(Id = 101, UserId = 1, Total = 99.0f))
    arr |> emplace(Order(Id = 102, UserId = 2, Total =  7.5f))
    return <- arr
}

def fixture_comments() : array<Comment> {
    var arr : array<Comment>
    arr |> emplace(Comment(Id = 1000, AuthorId = none(type<int>), Body = "alice's"))
    arr |> emplace(Comment(Id = 1001, AuthorId = none(type<int>), Body = "bob's"))
    return <- arr
}

def comment_eq(a : Comment; b : Comment) : bool {
    if (a.Id != b.Id || a.Body != b.Body) {
        return false
    }
    if ((a.AuthorId |> is_some) != (b.AuthorId |> is_some)) {
        return false
    }
    if (a.AuthorId |> is_none) {
        return true
    }
    return a.AuthorId |> unwrap == b.AuthorId |> unwrap
}

def comments_eq(a : array<Comment>; b : array<Comment>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (!comment_eq(a[i], b[i])) {
            return false
        }
    }
    return true
}

[test]
def parity_where_count(t : T?) {
    // _where(_.UserId == n) |> count() — alice has 2 orders.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_orders()

        let m1 = _sql(db |> select_from(type<Order>) |> _where(_.UserId == 1) |> count())
        let m2 = db |> select_from(type<Order>) |> _where(_.UserId == 1) |> count()
        let m3 = arr |> _where(_.UserId == 1) |> count()

        t |> equal(m1, 2,  "where|count: 2 rows for UserId=1")
        t |> equal(m1, m2, "where|count: M1 ≡ M2")
        t |> equal(m2, m3, "where|count: M2 ≡ M3")
    }
}

[test]
def parity_to_array_terminal(t : T?) {
    // `_to_array()` is the explicit materialization terminal. Result
    // type is the same as the default-no-terminal form (array<T>).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_comments()

        let m1 <- _sql(db |> select_from(type<Comment>) |> _to_array())
        let m2 <- (db |> select_from(type<Comment>) |> _to_array())
        let m3 <- (arr |> _to_array())

        t |> equal(length(m1), 2, "to_array: 2 rows")
        t |> success(comments_eq(m1, m2), "to_array: M1 ≡ M2")
        t |> success(comments_eq(m2, m3), "to_array: M2 ≡ M3")
    }
}
