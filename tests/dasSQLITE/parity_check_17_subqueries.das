options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 17 — subqueries.
//
// Shapes (each must produce identical row sets across the three modes):
//   1. `_in(subquery)`            — IN
//   2. `_not_in(subquery)`        — NOT IN
//   3. `_any(predicate)`          — EXISTS WHERE
//   4. `_none(predicate)`         — NOT EXISTS WHERE
//   5. `_in(captured_array)`      — IN (SELECT value FROM json_each(?))
//   6. `_not_in(captured_array)`  — NOT IN (SELECT value FROM json_each(?))
//
// The 1-arg form `subquery._any()` (test if non-empty, lifts to plain
// `EXISTS (...)` under `_sql`) is Mode-1-only. The `_any` linq_boost
// call_macro requires a predicate; the 1-arg "is non-empty" semantics
// in plain linq are spelled `length(...) > 0` or `any(...)` (function
// form), not `_any(...)`. No parity to check.
//
// All four covered shapes nest a `_`-binding macro inside an outer
// `_where`'s `_`-binding macro — they regression-test the
// `can_shadow` synthesis on linq_boost's predicate-lambda parameter.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name   : string
    Active : bool
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
}

def fixture_users() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "Alice", Active = true))
    arr |> emplace(User(Id = 2, Name = "Bob",   Active = true))
    arr |> emplace(User(Id = 3, Name = "Carol", Active = false))
    return <- arr
}

def fixture_orders() : array<Order> {
    var arr : array<Order>
    arr |> emplace(Order(Id = 10, UserId = 1, Total = 100))
    arr |> emplace(Order(Id = 11, UserId = 1, Total = 250))
    arr |> emplace(Order(Id = 12, UserId = 2, Total =  50))
    return <- arr
}

def users_equal(a : array<User>; b : array<User>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i].Id != b[i].Id || a[i].Name != b[i].Name || a[i].Active != b[i].Active) {
            return false
        }
    }
    return true
}

[test]
def parity_in_subquery(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m2 <- (db |> select_from(type<User>)
                     |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m3 <- (users |> _where(_.Id._in(orders |> _select(_.UserId))))

        t |> equal(length(m1), 2, "_in: 2 users with orders (Carol absent)")
        t |> success(users_equal(m1, m2), "_in: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_in: M2 ≡ M3")
    }
}

[test]
def parity_not_in_subquery(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m2 <- (db |> select_from(type<User>)
                     |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m3 <- (users |> _where(_.Id._not_in(orders |> _select(_.UserId))))

        t |> equal(length(m1), 1, "_not_in: 1 orderless user (Carol)")
        t |> success(users_equal(m1, m2), "_not_in: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_not_in: M2 ≡ M3")
    }
}

[test]
def parity_any_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        // Uncorrelated EXISTS: when ANY order has Total > 200, every
        // user surfaces; when none do, none surface. Predicate
        // references only the subquery's own row through `_`.
        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        let m2 <- (db |> select_from(type<User>)
                     |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        let m3 <- (users |> _where((orders)._any(_.Total > 200)))

        t |> equal(length(m1), 3, "_any(>200): all 3 users surface")
        t |> success(users_equal(m1, m2), "_any: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_any: M2 ≡ M3")
    }
}

[test]
def parity_none_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        // Uncorrelated NOT EXISTS: when no order has Total > 1000, all
        // users surface.
        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where((db |> select_from(type<Order>))._none(_.Total > 1000)))
        let m2 <- (db |> select_from(type<User>)
                     |> _where((db |> select_from(type<Order>))._none(_.Total > 1000)))
        let m3 <- (users |> _where((orders)._none(_.Total > 1000)))

        t |> equal(length(m1), 3, "_none(>1000): all 3 users surface")
        t |> success(users_equal(m1, m2), "_none: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_none: M2 ≡ M3")
    }
}

[test]
def parity_in_collection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users <- fixture_users()
        let want_ids <- [1, 2]

        let m1 <- _sql(db |> select_from(type<User>) |> _where(_.Id._in(want_ids)))
        let m2 <- (db |> select_from(type<User>) |> _where(_.Id._in(want_ids)))
        let m3 <- (users |> _where(_.Id._in(want_ids)))

        t |> equal(length(m1), 2, "_in(arr): Alice + Bob match")
        t |> success(users_equal(m1, m2), "_in(arr): M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_in(arr): M2 ≡ M3")
    }
}

[test]
def parity_not_in_collection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users <- fixture_users()
        let want_ids <- [1, 2]

        // Id is non-nullable PK → naive NOT IN form, identical across all three modes.
        let m1 <- _sql(db |> select_from(type<User>) |> _where(_.Id._not_in(want_ids)))
        let m2 <- (db |> select_from(type<User>) |> _where(_.Id._not_in(want_ids)))
        let m3 <- (users |> _where(_.Id._not_in(want_ids)))

        t |> equal(length(m1), 1, "_not_in(arr): only Carol survives")
        t |> success(users_equal(m1, m2), "_not_in(arr): M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_not_in(arr): M2 ≡ M3")
    }
}
