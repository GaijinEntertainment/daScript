options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// Column aggregates: sum / average / min / max. Each peels via the shared
// peel_column_aggregate helper — recurses into the inner chain first so
// `_select(_.Col)` populates selectCols, then wraps selectCols[0] in the
// SQL aggregate. AVG always promotes to double; SUM/MIN/MAX inherit the
// column's type.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sum_emits_sum_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> sum())
    t |> equal(sql,
        "SELECT SUM(\"Price\") FROM \"Cars\"",
        "sum() emits SELECT SUM(col)")
}

[test]
def test_average_emits_avg_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> average())
    t |> equal(sql,
        "SELECT AVG(\"Price\") FROM \"Cars\"",
        "average() emits SELECT AVG(col)")
}

[test]
def test_min_emits_min_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> min())
    t |> equal(sql,
        "SELECT MIN(\"Price\") FROM \"Cars\"",
        "min() emits SELECT MIN(col)")
}

[test]
def test_max_emits_max_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> max())
    t |> equal(sql,
        "SELECT MAX(\"Price\") FROM \"Cars\"",
        "max() emits SELECT MAX(col)")
}

[test]
def test_sum_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let total = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> sum())
        t |> equal(total, 100 + 200 + 300 + 50 + 999, "SUM of all prices")
    }
}

[test]
def test_min_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cheapest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> min())
        t |> equal(cheapest, 50, "min Price = Lada")
    }
}

[test]
def test_max_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let priciest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> max())
        t |> equal(priciest, 999, "max Price = Lambo")
    }
}

[test]
def test_average_runtime_promotes_to_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let avg = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> average())
        // AVG returns double; allow tiny FP slop.
        let expected = double(100 + 200 + 300 + 50 + 999) / 5.0lf
        t |> equal(avg, expected, "AVG returns double")
    }
}

[test]
def test_sum_with_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > 100)
                          |> _select(_.Price)
                          |> sum())
    t |> equal(sql,
        "SELECT SUM(\"Price\") FROM \"Cars\" WHERE \"Price\" > ?",
        "sum() with WHERE composes")
}

[test]
def test_sum_with_where_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let total = _sql(db |> select_from(type<Car>)
                           |> _where(_.Price > 100)
                           |> _select(_.Price)
                           |> sum())
        t |> equal(total, 200 + 300 + 999, "SUM where Price > 100")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// `take(n)` / `skip(n)` upstream of an aggregate is rejected at compile time:
// `SELECT SUM(p) FROM Cars LIMIT n` is degenerate (aggregate already returns
// one row, so LIMIT is a no-op). The user almost certainly wants the subquery
// form `SELECT SUM(c) FROM (SELECT p AS c FROM Cars LIMIT n)`, which lands
// with joins/subqueries in chunk 5+. Compile-time rejection lives in the
// failed_sql_macro suite (bad12) — see also tests/dasSQLITE/failed_sql_macro.das.
//
// Without that rejection the analyzer used to produce `SELECT SUM(p) FROM
// Cars LIMIT ?` with NO bind for the placeholder, which SQLite read as NULL
// (= no upper bound) — `take(n)` was silently ignored. Caught by Copilot's
// review of PR #2492.
// ──────────────────────────────────────────────────────────────────────────
