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
// Set operations (UNION / INTERSECT / EXCEPT) are **deferred to a
// later chunk**: they need a multi-source / multi-stage analyzer
// extension that lives alongside joins and subqueries. Until then,
// the raw-SQL escape hatch (`db |> exec(...)` /
// `db |> query_one(...)`) handles those cases.

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

        // ── Set operations (UNION / INTERSECT / EXCEPT) — deferred ───────
        // The chain analyzer is single-source today: every SELECT comes
        // from one `select_from(type<T>)`. Set ops bring in a second
        // SqlQuery and need a multi-stage emitter — same engine work
        // joins and subqueries need. Until that lands, drive `sqlite3_*`
        // directly or `exec` a `CREATE TEMP VIEW` that wraps the union
        // and `select_from` the view.
    }
}
