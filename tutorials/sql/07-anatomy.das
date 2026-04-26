options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 07 — Anatomy of `_sql(...)`.
//
// Everything in `_sql(chain)` is decided at compile time. The macro
// walks the daslib/linq-shaped chain, classifies each operator, and
// emits a SQL string + a list of bind expressions. There is no
// runtime LINQ→SQL inspection — by the time the program runs, only
// the SQL and the binds remain.
//
// The translation has three moving parts:
//   1. Column references — `_.Field` becomes a quoted identifier.
//   2. Captured values  — any free variable or literal becomes a `?`
//                          bind parameter.
//   3. Recognized ops   — comparisons / boolean logic / a fixed set
//                          of function calls (`starts_with`, `length`,
//                          `is_some`, `unwrap_or`, …) lower to SQL
//                          equivalents.
//
// Anything outside that surface is rejected at compile time with a
// `macro_error` pointing at the offending node. The escape hatch is
// the raw-SQL form: `db |> exec(...)`, `db |> query_one(...)`,
// `db |> query_scalar(...)`. They take a SQL string and bind
// parameters directly — no chain to translate.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
        db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
        db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))

        // ── (1) `_sql_text` — the inspector ──────────────────────────────
        // Same analyzer as `_sql`, but returns the SQL string instead
        // of running it. The `?` placeholders show where each bind goes.
        let sql1 = _sql_text(db |> select_from(type<Car>) |> _where(_.Price > 100))
        to_log(LOG_INFO, "(1) SELECT all rows where Price > 100:\n  {sql1}\n")
        // → SELECT "Id", "Name", "Price" FROM "Cars" WHERE "Price" > ?

        // ── (2) Captured-vs-literal binding ──────────────────────────────
        // A captured local variable becomes a bind. A literal *also*
        // becomes a bind (the analyzer doesn't bother distinguishing —
        // the SQL stays parameterized either way, which is the safe
        // default for SQL injection).
        let cutoff = 150
        let sql2 = _sql_text(db |> select_from(type<Car>) |> _where(_.Price > cutoff))
        to_log(LOG_INFO, "(2) Captured-var WHERE:\n  {sql2}\n")

        // ── (3) Column ref vs bind: which side is which ──────────────────
        // `_.Price` is a column reference (becomes "Price"). `cutoff`
        // is a captured value (becomes ?). Swap the sides — the macro
        // figures out which is the column on each side independently.
        let sql3a = _sql_text(db |> select_from(type<Car>) |> _where(_.Price > cutoff))
        let sql3b = _sql_text(db |> select_from(type<Car>) |> _where(cutoff < _.Price))
        to_log(LOG_INFO, "(3) symmetric:\n  a: {sql3a}\n  b: {sql3b}\n")

        // ── (4) Multiple `_where` calls compose with AND ─────────────────
        // Every `_where` adds another conjunct. Chain freely.
        let sql4 = _sql_text(db |> select_from(type<Car>)
                               |> _where(_.Price > 100)
                               |> _where(_.Name |> starts_with("T")))
        to_log(LOG_INFO, "(4) two _where calls → AND:\n  {sql4}\n")

        // ── (5) Running the same chain — the analyzer is pure ────────────
        // `_sql_text` and `_sql` share the analyzer; the only difference
        // is what the runtime helper does with the SQL string + binds.
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Price > 100)
                           |> _where(_.Name |> starts_with("T")))
        to_log(LOG_INFO, "(5) {length(cars)} cars match the chain in (4) above\n")

        // ── (6) Raw-SQL escape hatch ─────────────────────────────────────
        // For SQL the macro doesn't translate (DDL, vendor-specific
        // statements, dynamic SQL), drop down to `db |> exec(...)` /
        // `db |> query_one(...)`. Bind parameters are positional `?`.
        // Tutorial 5 covers the parametrized-query API in detail.
        let total = db |> query_scalar("SELECT SUM(Price) FROM Cars", type<int>)
        to_log(LOG_INFO, "(6) raw-SQL total Price = {total}\n")

        // ── (7) What `_sql` refuses ──────────────────────────────────────
        // The translation table is intentionally finite. If you write a
        // shape the analyzer doesn't recognize — an arbitrary user-
        // defined function call inside `_where`, an unsupported math
        // op, a regex — compile fails with `macro_error` pointing at
        // the offending node. Two possible responses:
        //   - Add the rule to the analyzer (fork-and-extend) if the
        //     pattern is reusable.
        //   - Use the raw-SQL escape hatch (6) if it's a one-off.
        //
        // The translation surface for `_where` predicates lives in
        // tutorial 8; for `_select` projections in tutorial 9. The
        // remaining read-side operators (order_by, take/skip, distinct,
        // aggregates, group_by, NULL handling) live in 10-14 and 18.
    }
}
