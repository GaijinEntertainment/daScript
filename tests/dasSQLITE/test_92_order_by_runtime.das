options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW", Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi", Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 100))
    db |> insert(Car(Id = 4, Name = "Lada", Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime _order_by(string) — column name supplied as a `string` expression.
// The compile-time path (_.Field) still emits a literal `"col" ASC`. The
// runtime path emits `sql_quote_id(<expr>)` so the column name is quoted at
// execution time, with embedded `"` properly doubled.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_order_by_const_string_emits_quoted(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by("Price"))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" ASC",
        "_order_by(const string) folds to compile-time quoted ORDER BY")
}

[test]
def test_order_by_descending_const_string(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by_descending("Price"))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" DESC",
        "_order_by_descending(const string) folds DESC")
}

[test]
def test_order_by_runtime_string_variable(t : T?) {
    var _db = SqlRunner()
    let col = "Name"
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by(col))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Name\" ASC",
        "_order_by(<runtime var>) quotes the column name at runtime")
}

[test]
def test_order_by_runtime_string_with_embedded_quote(t : T?) {
    var _db = SqlRunner()
    // Identifier with embedded `"` must be doubled, per SQLite identifier rules.
    let col = "We\"ird"
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by(col))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"We\"\"ird\" ASC",
        "embedded quote in runtime column name is doubled")
}

[test]
def test_order_by_runtime_string_executes(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let col = "Price"
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by(col))
        t |> equal(length(rows), 5, "5 rows")
        t |> equal(rows[0].Price, 50, "Price ASC: Lada first")
        t |> equal(rows[4].Price, 999, "Price ASC: Lambo last")
    }
}

[test]
def test_order_by_descending_runtime_string_executes(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let col = "Price"
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by_descending(col))
        t |> equal(rows[0].Price, 999, "Price DESC: Lambo first")
        t |> equal(rows[4].Price, 50, "Price DESC: Lada last")
    }
}

[test]
def test_order_by_tuple_runtime_strings(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let primary = "Price"
        let secondary = "Name"
        // BMW(100) and Tesla(100) tie on Price; Name ASC breaks the tie.
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by((primary, secondary)))
        t |> equal(rows[0].Name, "Lada", "Price=50 first")
        t |> equal(rows[1].Name, "BMW", "Price=100, Name=BMW")
        t |> equal(rows[2].Name, "Tesla", "Price=100, Name=Tesla")
    }
}

[test]
def test_order_by_tuple_mixed_field_and_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let secondary = "Name"
        // Mixed: compile-time _.Price + runtime secondary.
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by((_.Price, secondary)))
        t |> equal(rows[0].Name, "Lada", "Price=50 first")
        t |> equal(rows[1].Name, "BMW", "tie broken by Name ASC")
        t |> equal(rows[2].Name, "Tesla", "tie second")
    }
}

[test]
def test_order_by_runtime_string_with_where_and_take(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let col = "Price"
        let rows <- _sql(db |> select_from(type<Car>)
            |> _where(_.Price > 100)
            |> _order_by(col)
            |> take(2))
        t |> equal(length(rows), 2, "two rows past the WHERE filter")
        t |> equal(rows[0].Price, 200, "Audi (200) is smallest >100")
        t |> equal(rows[1].Price, 999, "Lambo (999) is next")
    }
}

[test]
def test_order_by_runtime_string_each_sql(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let col = "Price"
        var prices : array<int>
        for (row in _each_sql(db |> select_from(type<Car>) |> _order_by_descending(col))) {
            prices |> push(row.Price)
        }
        t |> equal(length(prices), 5, "_each_sql streams 5 rows in order")
        t |> equal(prices[0], 999, "DESC first")
        t |> equal(prices[4], 50, "DESC last")
    }
}
