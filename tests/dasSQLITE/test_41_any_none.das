options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Active : bool
}

[sql_table(name="Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total = 50))
}

// ──────────────────────────────────────────────────────────────────────────
// `_any` / `_none` in `_where` predicates → SQL `EXISTS` / `NOT EXISTS`.
// Subquery is uncorrelated — the inner predicate references only the
// subquery's own row (`_`), not outer columns.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_any_emits_exists(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._any()))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\")",
        "_any with no predicate emits EXISTS (subquery)")
}

[test]
def test_none_emits_not_exists(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._none()))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE NOT EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\")",
        "_none with no predicate emits NOT EXISTS (subquery)")
}

[test]
def test_any_with_predicate(t : T?) {
    var _db = SqlRunner()
    let _threshold = 200
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._any(_.Total > _threshold)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\" WHERE \"Total\" > ?)",
        "_any(pred) emits EXISTS with WHERE pred")
}

[test]
def test_none_with_predicate(t : T?) {
    var _db = SqlRunner()
    let _threshold = 1000
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._none(_.Total > _threshold)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE NOT EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\" WHERE \"Total\" > ?)",
        "_none(pred) emits NOT EXISTS with WHERE pred")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_any_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // EXISTS (any orders) — Orders is non-empty, so all Users match.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where((db |> select_from(type<Order>))._any()))
        t |> equal(length(rows), 3, "all 3 users (any order exists, uncorrelated)")
    }
}

[test]
def test_any_with_predicate_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // EXISTS Orders where Total > 200 — there's the 250 order, so all match.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        t |> equal(length(rows), 3, "all 3 users (an order with Total > 200 exists)")
        let rows_no_huge_orders <- _sql(db |> select_from(type<User>)
                                          |> _where((db |> select_from(type<Order>))._any(_.Total > 1000)))
        t |> equal(length(rows_no_huge_orders), 0, "0 users (no order with Total > 1000)")
    }
}
