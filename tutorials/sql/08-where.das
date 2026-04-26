options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require math
require daslib/strings_boost

// Tutorial 08 — `_where` predicates: the full surface.
//
// Inside `_sql(...)`, `_where(predicate)` accumulates a WHERE clause via
// a recursive AST walk. Multiple `_where` calls compose with AND.
//
// Recognized predicate shapes:
//   - Column refs:        `_.FieldName`
//   - Captured vars/lits: `someVar`, `42`, `"alice"`  → bind params (`?`)
//   - Comparison ops:     `==`, `!=`, `<`, `<=`, `>`, `>=`  → SQL `=`, `<>`, `<`, etc.
//   - Logic ops:          `&&`, `||`, `!`               → SQL AND, OR, NOT
//   - String tests:       `|> starts_with(s)`, `|> ends_with(s)`,
//                         `|> contains(s)`              → SQL LIKE patterns
//   - Case folding:       `|> to_lower()`, `|> to_upper()` → LOWER / UPPER
//   - String length:      `length(s)`                   → LENGTH(...)
//   - Numeric scalar:     `|> abs()`                    → ABS(...)
//
// Untranslatable shapes raise a compile-time macro_error pointing at the
// offending node. Reach for the raw-SQL escape hatch (`db |> exec(...)`,
// `query_one`, etc.) for cases the macro doesn't translate.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "Ford",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Ferrari",  Price = 9999))
    db |> insert(Car(Id = 3, Name = "Honda",    Price = 200))
    db |> insert(Car(Id = 4, Name = "Lada",     Price = 50))
    db |> insert(Car(Id = 5, Name = "Lamborghini", Price = 88888))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── Captured-var binding: simple equality ────────────────────────
        let target = 3
        let one <- _sql(db |> select_from(type<Car>) |> _where(_.Id == target))
        to_log(LOG_INFO, "Id == 3 -> {length(one)} row(s)\n")

        // ── Multiple _where calls compose with AND ───────────────────────
        let cheap_F <- _sql(db |> select_from(type<Car>)
                              |> _where(_.Price < 1000)
                              |> _where(_.Name |> starts_with("F")))
        to_log(LOG_INFO, "Price<1000 AND starts_with('F') -> {length(cheap_F)} row(s)\n")

        // ── Logic ops: && / || / ! ───────────────────────────────────────
        let cheap_or_F <- _sql(db |> select_from(type<Car>)
                                 |> _where(_.Price < 100 || (_.Name |> starts_with("F"))))
        to_log(LOG_INFO, "Price<100 OR starts_with('F') -> {length(cheap_or_F)} row(s)\n")

        // ── String predicates ────────────────────────────────────────────
        let ends_da <- _sql(db |> select_from(type<Car>)
                              |> _where(_.Name |> ends_with("da")))
        to_log(LOG_INFO, "ends_with('da') -> {length(ends_da)} row(s)\n")

        let contains_o <- _sql(db |> select_from(type<Car>)
                                 |> _where(_.Name |> contains("o")))
        to_log(LOG_INFO, "contains('o') -> {length(contains_o)} row(s)\n")

        // ── Case folding ─────────────────────────────────────────────────
        let case_match <- _sql(db |> select_from(type<Car>)
                                 |> _where(_.Name |> to_lower() == "ford"))
        to_log(LOG_INFO, "lower(name) == 'ford' -> {length(case_match)} row(s)\n")

        // ── String length ────────────────────────────────────────────────
        let long_names <- _sql(db |> select_from(type<Car>)
                                 |> _where(length(_.Name) > 5))
        to_log(LOG_INFO, "length(name) > 5 -> {length(long_names)} row(s)\n")

        // ── Numeric scalar (abs) ─────────────────────────────────────────
        let dear <- _sql(db |> select_from(type<Car>)
                           |> _where((_.Price |> abs()) > 5000))
        to_log(LOG_INFO, "|price| > 5000 -> {length(dear)} row(s)\n")

        // ── Inspect the emitted SQL via _sql_text (debugging aid) ────────
        let sql = _sql_text(db |> select_from(type<Car>)
                              |> _where((_.Price |> abs()) > 5000))
        to_log(LOG_INFO, "  emitted SQL: {sql}\n")
    }
}
