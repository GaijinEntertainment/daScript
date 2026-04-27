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
// _join — inner equi-join. Emits `INNER JOIN ... ON ...`.
// Multi-source mode forces table-aliased column refs (`"t0"."Col"`).
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_inner_join_emits_inner_join_on(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _join(_db |> select_from(type<Order>),
                                   $(u : User, o : Order) => u.Id == o.UserId,
                                   $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\", \"t1\".\"Total\" FROM \"Users\" AS \"t0\" INNER JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\"",
        "_join emits SELECT aliased cols + INNER JOIN ON aliased keys")
}

[test]
def test_inner_join_with_outer_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Active)
                          |> _join(_db |> select_from(type<Order>),
                                   $(u : User, o : Order) => u.Id == o.UserId,
                                   $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\", \"t1\".\"Total\" FROM \"Users\" AS \"t0\" INNER JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\" WHERE \"t0\".\"Active\"",
        "_where on left side qualifies as t0.Col in multi-source mode")
}

[test]
def test_inner_join_with_inner_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _join(_db |> select_from(type<Order>) |> _where(_.Total > 75),
                                   $(u : User, o : Order) => u.Id == o.UserId,
                                   $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    t |> equal(sql,
        "SELECT \"t0\".\"Name\", \"t1\".\"Total\" FROM \"Users\" AS \"t0\" INNER JOIN \"Orders\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"UserId\" AND (\"t1\".\"Total\" > ?)",
        "_where inside the right-side chain emits in JOIN's ON clause (qualified t1.Col, NOT in outer WHERE) — required for LEFT JOIN, used uniformly for INNER too")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_inner_join_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _join(db |> select_from(type<Order>),
                                    $(u : User, o : Order) => u.Id == o.UserId,
                                    $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
        // Alice has 2 orders, Bob has 1, Carol 0. Inner join → 3 rows.
        t |> equal(length(rows), 3, "inner join yields 3 rows (Alice×2 + Bob×1, Carol absent)")
        // Total across all join rows: 100 + 250 + 50 = 400
        var sum_total = 0
        for (r in rows) {
            sum_total += r.Total
        }
        t |> equal(sum_total, 400, "Total across joined rows sums correctly")
    }
}
