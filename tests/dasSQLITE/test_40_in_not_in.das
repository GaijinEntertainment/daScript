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
    // Carol (id 3) has no orders.
}

// ──────────────────────────────────────────────────────────────────────────
// `_in` / `_not_in` — IN / NOT IN with a subquery.
// Subquery is uncorrelated; users project a single column with `_select(_.Col)`.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_in_emits_in_subquery(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Id._in(_db |> select_from(type<Order>) |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" IN (SELECT \"UserId\" FROM \"Orders\")",
        "_in with subquery emits IN (SELECT ...)")
}

[test]
def test_not_in_emits_not_in_subquery(t : T?) {
    var _db = SqlRunner()
    // _not_in expands to !contains(subq, x); pred_to_sql's NOT peel wraps it.
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Id._not_in(_db |> select_from(type<Order>) |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" NOT IN (SELECT \"UserId\" FROM \"Orders\")",
        "_not_in emits explicit NOT IN (special-cased in pred_to_sql to peek through the `!contains(...)` AST shape)")
}

[test]
def test_in_with_outer_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Active && _.Id._in(_db |> select_from(type<Order>) |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE (\"Active\") AND (\"Id\" IN (SELECT \"UserId\" FROM \"Orders\"))",
        "_in composes with other WHERE predicates via && — bind order preserved")
}

[test]
def test_in_with_inner_where_binds(t : T?) {
    var _db = SqlRunner()
    let threshold = 100
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Id._in(_db |> select_from(type<Order>)
                                                  |> _where(_.Total > threshold)
                                                  |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" IN (SELECT \"UserId\" FROM \"Orders\" WHERE \"Total\" > ?)",
        "_in with inner _where emits subquery WHERE — bind appended after outer-WHERE binds")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime — verify rows match the IN/NOT IN semantics.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_in_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Users whose Id appears in Orders.UserId — Alice and Bob.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        t |> equal(length(rows), 2, "Alice + Bob match (Carol absent)")
        var saw_alice = false
        var saw_bob = false
        for (u in rows) {
            if (u.Name == "Alice") {
                saw_alice = true
            }
            if (u.Name == "Bob") {
                saw_bob = true
            }
        }
        t |> success(saw_alice)
        t |> success(saw_bob)
    }
}

[test]
def test_not_in_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
        t |> equal(length(rows), 1, "only Carol (no orders) matches NOT IN")
        t |> equal(rows[0].Name, "Carol")
    }
}
