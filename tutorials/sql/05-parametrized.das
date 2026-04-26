options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 05 — Parameter binding.
//
// Two stories share this tutorial:
//
//  1. Under `_sql(...)`, captured variables in the chain become SQL bind
//     parameters AUTOMATICALLY. The reader never types `?` or `:name` —
//     the macro's expression walker classifies `_.Field` as a column
//     identifier and any free variable as a bind value, then emits
//     `SELECT ... WHERE Id = ?` with the value bound. Tutorial 5's whole
//     conceptual surface (parameter binding) becomes invisible — that's
//     the design win.
//
//  2. For raw SQL (migrations, ad-hoc queries, statements `_sql` can't
//     translate), the `query_one(sql, type<T>, args...)` family takes
//     positional `?` arguments via variadic trailing args (chunk 3 ships
//     0..3 args; named-tuple bind for `:name` placeholders ships in a
//     follow-up).

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
        db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
        db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))

        // ── LINQ form (primary) ──────────────────────────────────────────
        // `target` is a captured local; `_.Id` is a column reference.
        // `_sql` emits  SELECT "Id", "Name", "Price" FROM "Cars" WHERE "Id" = ? LIMIT 1
        // and binds [target] before stepping. `_first()` panics on empty.
        let target = 3
        let car = _sql(db |> select_from(type<Car>)
                         |> _where(_.Id == target)
                         |> _first())
        to_log(LOG_INFO, "Found ID = {car.Id}, name = {car.Name}\n")

        // ── Non-panic LINQ variant: _first_opt() returns Option<T> ───────
        let maybe = _sql(db |> select_from(type<Car>)
                           |> _where(_.Id == 999)
                           |> _first_opt())
        to_log(LOG_INFO, "ID 999 found? {maybe |> is_some}\n")

        // ── Raw-SQL escape hatch (positional `?`) ────────────────────────
        // Variadic trailing args bind to placeholders in declaration order.
        // Mixed types (here: int) are fine — no homogeneity constraint.
        let raw = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
            type<Car>, target)
        to_log(LOG_INFO, "Raw lookup ID = {raw.Id}, name = {raw.Name}\n")

        // ── Multi-arg positional bind ────────────────────────────────────
        // Up to 3 positional args this chunk; named-tuple `(id=…, name=…)`
        // form for `:name` placeholders is a chunk-4 follow-up.
        let multi = db |> query_one(
            "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ? AND Name = ?",
            type<Car>, 2, "Audi")
        to_log(LOG_INFO, "Multi-arg lookup: {multi.Name}\n")
    }
}
