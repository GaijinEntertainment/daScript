options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 16 — `_left_join`: left outer join with `Option<TB>` on the right.
//
// Same 4-arg shape as `_join` (other, on, into) but the `into` lambda's
// second parameter is `Option<TB>` — `none` when the left row had no
// matching right row, `some(tb)` otherwise.
//
// Under `_sql(...)` the right-side columns become NULL on no-match
// rows; `Option<TB>` is the daslang lift of that NULL semantics. The
// `into` projection can probe the option:
//
//   - `o |> is_some`   →  `"t1"."<rightKey>" IS NOT NULL`
//   - `o |> is_none`   →  `"t1"."<rightKey>" IS NULL`
//
// (The probe column is the right-side ON-key; if THAT came back NULL
// the row had no match. Per-column nullability inside `Option<TB>` is
// not yet wired — for `o.Total` you'd need to drop to raw SQL.)
//
// `_left_join` ships INNER + LEFT only; `_right_join`, `_full_outer_join`,
// and `_cross_join` are not shipped. RIGHT JOIN is `LEFT JOIN` with
// swapped sources; CROSS JOIN reads as an accidental footgun in macro
// form and belongs in raw SQL.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
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
    db |> insert(User(Id = 1, Name = "Alice"))
    db |> insert(User(Id = 2, Name = "Bob"))
    db |> insert(User(Id = 3, Name = "Carol"))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
    // Carol (id 3) has no orders.
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // LEFT JOIN — every User shows up, even Carol who has no orders.
        // The `HasOrder` projection probes whether the right-side join
        // key landed non-NULL.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _left_join(db |> select_from(type<Order>),
                                         $(u : User, o : Order) => u.Id == o.UserId,
                                         $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
        to_log(LOG_INFO, "left join — every user surfaces:")
        for (r in rows) {
            to_log(LOG_INFO, "  {r.Name}: HasOrder = {r.HasOrder}")
        }
        // Expected — 4 rows: Alice/true × 2 (two orders), Bob/true (one
        // order), Carol/false (no match).

        // Inspect the SQL — `LEFT JOIN ... ON ...` plus the IS NOT NULL
        // probe on `t1.UserId` (the right-side join key).
        let sql = _sql_text(db |> select_from(type<User>)
                              |> _left_join(db |> select_from(type<Order>),
                                            $(u : User, o : Order) => u.Id == o.UserId,
                                            $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
        to_log(LOG_INFO, "SQL: {sql}")
    }
}
