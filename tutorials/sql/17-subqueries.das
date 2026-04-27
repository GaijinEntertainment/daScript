options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 17 — Subqueries: `_in`, `_not_in`, `_any`, `_none`.
//
// All four are `daslib/linq_boost` macros that, under `_sql(...)`,
// emit SQL's IN / NOT IN / EXISTS / NOT EXISTS forms with a nested
// `SELECT ...` subquery.
//
//   x._in(subquery)         →  `<x> IN (<subquery-sql>)`
//   x._not_in(subquery)     →  `<x> NOT IN (<subquery-sql>)`
//   subquery._any()         →  `EXISTS (<subquery-sql>)`
//   subquery._any(predicate) → `EXISTS (... WHERE <predicate>)`
//   subquery._none()        →  `NOT EXISTS (<subquery-sql>)`
//   subquery._none(predicate) → `NOT EXISTS (... WHERE <predicate>)`
//
// Negated forms have **explicit names** (`_not_in`, `_none`) rather
// than `!_in(...)` / `!_any(...)` because the `_sql` AST walker
// pattern-matches the macro's textual name; a leading `!` would
// require deep AST walking that fights with daslang's expansion order.
//
// For chunk 5, subqueries are **uncorrelated** — the inner WHERE
// references only the subquery's own row (`_`), not outer columns.
// Correlated subqueries (referring to outer columns from inside the
// subquery's WHERE) land later. For IN-style subqueries, project a
// single column with `_select(_.Col)` so the IN list shape matches.

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
    // Carol has no orders.
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // IN subquery — Users whose Id appears in Orders.UserId.
        let with_orders <- _sql(db |> select_from(type<User>)
                                  |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        to_log(LOG_INFO, "IN subquery — users with orders: {length(with_orders)}")
        // 2 rows: Alice, Bob.

        // NOT IN subquery — Users whose Id is NOT in Orders.UserId.
        let no_orders <- _sql(db |> select_from(type<User>)
                                |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
        to_log(LOG_INFO, "NOT IN subquery — orderless users: {length(no_orders)}")
        // 1 row: Carol.

        // EXISTS subquery — uncorrelated test (`Orders` non-empty?).
        let any_orders <- _sql(db |> select_from(type<User>)
                                 |> _where((db |> select_from(type<Order>))._any()))
        to_log(LOG_INFO, "EXISTS — when Orders has rows, all 3 users surface: {length(any_orders)}")

        // EXISTS with a predicate — uncorrelated; references only the
        // subquery's own row through `_`.
        let any_big_order <- _sql(db |> select_from(type<User>)
                                    |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        to_log(LOG_INFO, "EXISTS (Total > 200) — guard fires when any qualifying order exists: {length(any_big_order)}")

        // NOT EXISTS with a predicate — `_none(pred)` lifts to `NOT EXISTS (... WHERE pred)`.
        let none_huge <- _sql(db |> select_from(type<User>)
                                |> _where((db |> select_from(type<Order>))._none(_.Total > 1000)))
        to_log(LOG_INFO, "NOT EXISTS (Total > 1000) — when none exist, all users surface: {length(none_huge)}")

        // Inspect the SQL.
        let sql_in = _sql_text(db |> select_from(type<User>)
                                 |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        to_log(LOG_INFO, "SQL (IN): {sql_in}")
        let sql_exists = _sql_text(db |> select_from(type<User>)
                                     |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        to_log(LOG_INFO, "SQL (EXISTS): {sql_exists}")
    }
}
