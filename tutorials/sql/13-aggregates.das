options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 13 — Column aggregates: `sum`, `average`, `min`, `max`,
// plus `count` (already shown in tutorial 6 and used as a canary in
// chunk 3).
//
// All five are *terminals* — they end the chain and produce a
// scalar, not an array. The translation pattern is the same for the
// four column-driven ones:
//
//   db |> select_from(type<Car>) |> _select(_.Price) |> sum()
//                                                       ^^^^^
//   - `_select(_.Price)` picks the column.
//   - The terminal (`sum` / `average` / `min` / `max`) wraps it.
//
// `count()` doesn't need a `_select` — it counts rows.
//
// Result types:
//   - `sum`, `min`, `max` inherit the column's daslang type
//     (`int` for `Price`, `int64` for big-number columns, etc.).
//   - `average` always promotes to `double`.
//   - `count` returns `int` (matching daslang's `length(arr)`).
//
// Composing with `_where` works as expected — the predicate filters
// the rows the aggregate sees.

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
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── count — over the whole source, no `_select` needed ──────────
        let n = _sql(db |> select_from(type<Car>) |> count())
        to_log(LOG_INFO, "count = {n} rows\n")

        // ── sum — column total ──────────────────────────────────────────
        let total = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> sum())
        to_log(LOG_INFO, "SUM(Price) = {total}\n")

        // ── average — promotes to double ─────────────────────────────────
        let mean = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> average())
        to_log(LOG_INFO, "AVG(Price) = {mean}\n")

        // ── min / max — inherit the column's type ────────────────────────
        let cheapest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> min())
        let priciest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> max())
        to_log(LOG_INFO, "MIN(Price) = {cheapest}\n")
        to_log(LOG_INFO, "MAX(Price) = {priciest}\n")

        // ── Aggregates compose with `_where` ─────────────────────────────
        let cutoff = 100
        let total_over_100 = _sql(db |> select_from(type<Car>)
                                    |> _where(_.Price > cutoff)
                                    |> _select(_.Price)
                                    |> sum())
        to_log(LOG_INFO, "SUM(Price) WHERE Price>100 = {total_over_100}\n")

        // ── Inspect emitted SQL ──────────────────────────────────────────
        let sql_sum = _sql_text(db |> select_from(type<Car>)
                                  |> _select(_.Price)
                                  |> sum())
        let sql_avg = _sql_text(db |> select_from(type<Car>)
                                  |> _select(_.Price)
                                  |> average())
        to_log(LOG_INFO, "  SUM SQL: {sql_sum}\n")
        to_log(LOG_INFO, "  AVG SQL: {sql_avg}\n")
    }
}
