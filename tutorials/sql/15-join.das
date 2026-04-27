options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 15 — `_join`: inner equi-join across two tables.
//
// `_join(other, on, into)` is sugar over the existing
// `daslib/linq.das::join` — under `_sql(...)` it emits SQL's
// `INNER JOIN ... ON ...`.
//
// The `on` predicate is locked to the equi-join shape:
//
//     $(l : TA, r : TB) => l.X == r.Y
//
// — a 2-arg lambda whose body is one `==` comparison between a
// left-side field and a right-side field. Theta joins (`<`, `>`,
// `&&`-chained multi-key equality, function calls inside) emit a
// compile-time `macro_error` pointing at raw SQL as the escape hatch.
// The single-key restriction matches the backing `join` fn's
// hash-based O(n+m) implementation; multi-key tuple joins land later.
//
// The `into` lambda is the projection. Its two args bind to the two
// sources — naming is up to you (`u`, `o` here). Refer to columns
// from each source through the arg name (`u.Name`, `o.Total`); the
// `_sql` translator rewrites them to alias-qualified column refs
// (`"t0"."Name"`, `"t1"."Total"`).
//
// In multi-source mode the FROM clause aliases the root table as
// `t0`, the joined table as `t1`, and every column ref in the WHERE,
// projection, and ON clauses is qualified with the matching alias.
// Single-source chains keep the unqualified shape — only multi-source
// queries pay the alias-noise tax.

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

def fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
    // Carol (id 3) has no orders. Inner join drops her.
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // Plain inner equi-join + named-tuple projection from both sources.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _join(db |> select_from(type<Order>),
                                    $(u : User, o : Order) => u.Id == o.UserId,
                                    $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total)))
        to_log(LOG_INFO, "inner join — User × Order:")
        for (r in rows) {
            to_log(LOG_INFO, "  {r.UserName} placed an order of {r.OrderTotal}")
        }
        // Expected output (3 rows, Carol absent):
        //   Alice placed an order of 100
        //   Alice placed an order of 250
        //   Bob   placed an order of  50

        // Pre-join `_where` filters the LEFT source before the join.
        // Column refs qualify with the left alias `t0` automatically.
        let active_only <- _sql(db |> select_from(type<User>)
                                  |> _where(_.Active)
                                  |> _join(db |> select_from(type<Order>),
                                           $(u : User, o : Order) => u.Id == o.UserId,
                                           $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total)))
        to_log(LOG_INFO, "active-only join: {length(active_only)} rows")
        // 3 rows still — both Alice and Bob are active. Carol was already
        // dropped by the inner join above.

        // _where can also live INSIDE the right-side chain. Filters there
        // qualify with the right alias `t1` and emit into the JOIN's ON
        // clause: `INNER JOIN "Orders" AS "t1" ON ... AND ("t1"."Total" > ?)`.
        // (Required for LEFT JOIN to preserve unmatched rows; INNER uses the
        // same placement for consistency with LEFT and planner stability.)
        let big_orders <- _sql(db |> select_from(type<User>)
                                 |> _join(db |> select_from(type<Order>) |> _where(_.Total > 75),
                                          $(u : User, o : Order) => u.Id == o.UserId,
                                          $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total)))
        to_log(LOG_INFO, "big-order join (Total > 75): {length(big_orders)} rows")
        // 2 rows — Alice's 100, Alice's 250. Bob's 50 dropped.

        // Inspect the generated SQL with `_sql_text(...)`. Aliased FROM,
        // INNER JOIN, qualified WHERE.
        let sql = _sql_text(db |> select_from(type<User>)
                              |> _where(_.Active)
                              |> _join(db |> select_from(type<Order>),
                                       $(u : User, o : Order) => u.Id == o.UserId,
                                       $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total)))
        to_log(LOG_INFO, "SQL: {sql}")
    }
}
