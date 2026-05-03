options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 10 — `_order_by` and `_order_by_descending`.
//
// `_order_by(key)` adds an `ORDER BY` clause. The key shape decides
// what the macro emits:
//
//   - `_.Field`              → ORDER BY "Field" ASC
//   - `(_.k1, _.k2, …)`      → ORDER BY "k1" ASC, "k2" ASC, …
//   - `_order_by_descending` flips ASC to DESC for the whole chain step
//
// Mixed ASC/DESC across columns in a single chain step is **not
// supported** — use the raw-SQL escape hatch
// (`db |> query_one("...ORDER BY a, b DESC", ...)`) when you need it.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 100))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── ASC by a single column ───────────────────────────────────────
        let by_price <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price))
        to_log(LOG_INFO, "ASC by Price — cheapest first:\n")
        for (c in by_price) {
            to_log(LOG_INFO, "  {c.Name}: {c.Price}\n")
        }

        // ── DESC by a single column ──────────────────────────────────────
        let by_price_desc <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.Price))
        to_log(LOG_INFO, "DESC by Price — priciest first:\n")
        for (c in by_price_desc) {
            to_log(LOG_INFO, "  {c.Name}: {c.Price}\n")
        }

        // ── Multi-column tuple key — primary then tie-breaker ────────────
        // BMW(100) and Tesla(100) tie on Price. The tuple-key form
        // `(_.Price, _.Name)` adds a second `ORDER BY` column so the
        // tie breaks by Name ASC.
        let by_price_then_name <- _sql(db |> select_from(type<Car>)
                                         |> _order_by((_.Price, _.Name)))
        to_log(LOG_INFO, "Tuple-key (Price, Name) — alphabetical tie-break:\n")
        for (c in by_price_then_name) {
            to_log(LOG_INFO, "  {c.Name}: {c.Price}\n")
        }

        // ── Composes with `_where` and `take` ────────────────────────────
        let cheap_top3 <- _sql(db |> select_from(type<Car>)
                                 |> _where(_.Price > 50)
                                 |> _order_by(_.Price)
                                 |> take(3))
        to_log(LOG_INFO, "WHERE Price>50 ORDER BY Price ASC LIMIT 3:\n")
        for (c in cheap_top3) {
            to_log(LOG_INFO, "  {c.Name}: {c.Price}\n")
        }

        // ── Inspect emitted SQL ──────────────────────────────────────────
        let sql = _sql_text(db |> select_from(type<Car>)
                              |> _order_by((_.Price, _.Name)))
        to_log(LOG_INFO, "  emitted SQL: {sql}\n")
    }
}
