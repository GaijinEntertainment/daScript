options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 12 — `distinct()`: DISTINCT row dedupe.
//
// `distinct()` flips a single bool flag on the analyzer's `SqlQuery`
// and emits `SELECT DISTINCT` instead of plain `SELECT`. The rest
// of the chain (column list, WHERE, ORDER BY, LIMIT) composes
// normally.
//
// Set operations (UNION / INTERSECT / EXCEPT) live in tutorial
// `12b-set_ops.das`.

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
    db |> insert(Car(Id = 3, Name = "BMW",   Price = 150))
    db |> insert(Car(Id = 4, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 5, Name = "Audi",  Price = 200))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── Full-row DISTINCT ────────────────────────────────────────────
        // Whole-row dedupe — every column must match. PKs differ here
        // so all 5 rows survive; the point is to show the SQL emission.
        let all_rows <- _sql(db |> select_from(type<Car>) |> distinct())
        to_log(LOG_INFO, "Full-row DISTINCT: {length(all_rows)} rows\n")

        // ── Single-column DISTINCT — distinct names ──────────────────────
        // `_select(_.Name) |> distinct()` projects one column and
        // dedupes it. The result element type matches the column type.
        let names <- _sql(db |> select_from(type<Car>)
                            |> _select(_.Name)
                            |> distinct())
        to_log(LOG_INFO, "DISTINCT names: {length(names)} unique\n")
        for (n in names) {
            to_log(LOG_INFO, "  {n}\n")
        }

        // ── DISTINCT composes with WHERE ─────────────────────────────────
        let names_over_100 <- _sql(db |> select_from(type<Car>)
                                     |> _where(_.Price > 100)
                                     |> _select(_.Name)
                                     |> distinct())
        to_log(LOG_INFO, "DISTINCT names where Price>100: {length(names_over_100)}\n")
        for (n in names_over_100) {
            to_log(LOG_INFO, "  {n}\n")
        }

        // ── Inspect emitted SQL ──────────────────────────────────────────
        let sql = _sql_text(db |> select_from(type<Car>)
                              |> _select(_.Name)
                              |> distinct())
        to_log(LOG_INFO, "  emitted SQL: {sql}\n")

        // ── Distinct + first-row aggregates ──────────────────────────────
        // `_distinct_by(_.K)` followed by an aggregate terminator
        // (`_count(P)` / `_long_count(P)`, or `_select(_.F) |> sum/min/max/average()`)
        // lowers to a SQLite bare-aggregate subquery picking the row with the
        // smallest @sql_primary_key per group — matches linq's "first row per K"
        // semantics when PKs are monotonic with insertion. For the predicate-count
        // form, use the `_count(_.field > X)` linq_boost shorthand inside `_sql`;
        // explicit lambdas don't type-bind through `_sql`'s macro boundary.

        // Count distinct names whose first-encountered Car has Price > 150.
        // Fixture: BMW first @100 (no), Audi first @200 (yes), Tesla first @300 (yes) → 2.
        let n_first = _sql(db |> select_from(type<Car>)
                              |> _distinct_by(_.Name)
                              |> _count(_.Price > 150))
        to_log(LOG_INFO, "DISTINCT names with first-row Price>150: {n_first}\n")

        // Sum of first-row prices per Name: 100 + 200 + 300 = 600.
        let total_first = _sql(db |> select_from(type<Car>)
                                  |> _distinct_by(_.Name)
                                  |> _select(_.Price)
                                  |> sum())
        to_log(LOG_INFO, "Sum of first-row prices per Name: {total_first}\n")

        // Inspect the emitted SQL — `SELECT *, MIN("Id") FROM "Cars" GROUP BY "Name"`
        // is the bare-aggregate inner subquery; outer SELECT applies the aggregate.
        let sql_first = _sql_text(db |> select_from(type<Car>)
                                     |> _distinct_by(_.Name)
                                     |> _count(_.Price > 150))
        to_log(LOG_INFO, "  emitted SQL: {sql_first}\n")

        // ── Distinct + composition (row passthrough) ─────────────────────
        // `_distinct_by(K) |> _order_by(S) [|> take(N)]` wraps the source in
        // the same bare-aggregate subquery; outer SELECT applies ORDER BY +
        // LIMIT. Closes the "Window-function lowerings" group of
        // sqlite_linq_gaps.md without needing window functions.
        let cheapest_first <- _sql(db |> select_from(type<Car>)
                                      |> _distinct_by(_.Name)
                                      |> _order_by(_.Price)
                                      |> take(2))
        to_log(LOG_INFO, "Cheapest 2 first-per-Name: {length(cheapest_first)} rows\n")
        for (r in cheapest_first) {
            to_log(LOG_INFO, "  Id={r.Id} Name={r.Name} Price={r.Price}\n")
        }

        // ── reverse() + _distinct_by — LAST per group via MAX(pk) ────────
        // The bare-aggregate flips MIN(pk) → MAX(pk). reverse() in `_sql`
        // is only legal when wrapped by _distinct_by (SQL has no inherent
        // row ordering to reverse).
        let lasts <- _sql(db |> select_from(type<Car>)
                             |> reverse()
                             |> _distinct_by(_.Name))
        to_log(LOG_INFO, "LAST per Name ({length(lasts)} rows):\n")
        for (r in lasts) {
            to_log(LOG_INFO, "  Id={r.Id} Name={r.Name} Price={r.Price}\n")
        }

        // ── _group_by + first row per group — tuple projection ───────────
        // `_._1 |> first()` projects the first row of the group (by source
        // PK). Output type is a named tuple of (key, full source row).
        let groups <- _sql(db |> select_from(type<Car>)
                              |> _group_by(_.Name)
                              |> _select((Brand = _._0,
                                          FirstCar = _._1 |> first())))
        to_log(LOG_INFO, "Brand + first car per Name ({length(groups)} groups):\n")
        for (g in groups) {
            to_log(LOG_INFO, "  Brand={g.Brand}  FirstCar.Id={g.FirstCar.Id} Price={g.FirstCar.Price}\n")
        }

        // ── Set operations (UNION / INTERSECT / EXCEPT) ──────────────────
        // See tutorial `12b-set_ops.das`.
    }
}
