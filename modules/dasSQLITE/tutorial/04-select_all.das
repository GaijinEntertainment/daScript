options gen2

// Tutorial 04 — Reading rows with `_sql`.
//
// Defines a [sql_table] struct, inserts a few rows, and reads them back through
// the `_sql(...)` call macro. `_sql` takes a daslib/linq-shaped chain
// (`db |> select_from(type<T>) |> _select(_.X) |> _where(predicate)` etc.)
// and translates it at compile time into SQL + bound parameters. The runtime
// just runs the prepared statement and materializes the result.

require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Cars")]
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
        db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
        db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))

        // ── Read every row ────────────────────────────────────────────
        // _sql(db |> select_from(type<Car>)) compiles to:
        //   SELECT "Id", "Name", "Price" FROM "Cars"
        // and materializes each row as a Car.
        let cars <- _sql(db |> select_from(type<Car>))
        to_log(LOG_INFO, "Read {length(cars)} cars:\n")
        for (car in cars) {
            to_log(LOG_INFO, "  Id={car.Id} Name={car.Name} Price={car.Price}\n")
        }

        // ── Filter with _where (bind capture) ─────────────────────────
        // Free variables in the predicate become `?` bind parameters.
        // Macro emits:  ... WHERE "Price" > ?     binds: [threshold]
        let threshold = 200
        let pricey <- _sql(db |> select_from(type<Car>) |> _where(_.Price > threshold))
        to_log(LOG_INFO, "Cars priced over {threshold}:\n")
        for (car in pricey) {
            to_log(LOG_INFO, "  {car.Name}: {car.Price}\n")
        }

        // ── Project a single column with _select ──────────────────────
        // Macro emits:  SELECT "Name" FROM "Cars"
        // Result type is array<string>.
        let names <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
        to_log(LOG_INFO, "All car names: {names}\n")

        // ── Inspect the generated SQL ─────────────────────────────────
        // _sql_text emits the SQL string the macro would have run, with `?`
        // placeholders for binds. Useful for tests and debugging.
        let sql_text = _sql_text(db |> select_from(type<Car>) |> _where(_.Id == 5))
        to_log(LOG_INFO, "_sql_text: {sql_text}\n")
    }
}
