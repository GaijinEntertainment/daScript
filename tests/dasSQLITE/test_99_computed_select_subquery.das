options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Two features exercised here:
//   B1 — a computed scalar `_select(_.a + _.b)` lowers to `<frag>` via pred_to_sql + a computed projection
//        slot, so `sum/min/max/average/count` over it works (previously rejected: "_.Field only").
//   A  — take(n)/skip(n) BEFORE an aggregate wraps the pre-aggregate chain into an inner subquery
//        (LIMIT/OFFSET stay inner) and renders the aggregate on the outer SELECT (previously rejected).

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW", Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi", Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada", Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ===== B1 — computed scalar _select =====

[test]
def test_computed_select_emits_fragment(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price + _.Id) |> sum())
    t |> success(sql == "SELECT SUM((\"Price\") + (\"Id\")) FROM \"Cars\"",
        "computed _select renders the arithmetic into the aggregate argument")
}

[test]
def test_computed_select_sum_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // (100+1)+(200+2)+(300+3)+(50+4)+(999+5) = 1664
        let s = _sql(db |> select_from(type<Car>) |> _select(_.Price + _.Id) |> sum())
        t |> equal(s, 1664, "SUM(Price+Id)")
    }
}

[test]
def test_computed_select_min_max_avg_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // values: 101, 202, 303, 54, 1004
        t |> equal(_sql(db |> select_from(type<Car>) |> _select(_.Price + _.Id) |> min()), 54, "MIN(Price+Id)")
        t |> equal(_sql(db |> select_from(type<Car>) |> _select(_.Price + _.Id) |> max()), 1004, "MAX(Price+Id)")
        t |> numericEqual(_sql(db |> select_from(type<Car>) |> _select(_.Price + _.Id) |> average()), 332.8lf)
    }
}

[test]
def test_computed_predicate_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // A computed comparison in `_where` (pred_to_sql already renders arithmetic predicates).
        // count where Price*2 > 400 -> Price > 200 -> Tesla(300), Lambo(999) = 2
        let c = _sql(db |> select_from(type<Car>) |> _where(_.Price * 2 > 400) |> count())
        t |> equal(c, 2, "COUNT of (Price*2 > 400)")
    }
}

[test]
def test_bare_column_select_still_works(t : T?) {
    // regression: single-column `_select(_.Col)` must keep emitting `SUM("Col")`, not the computed path.
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> sum())
    t |> success(sql == "SELECT SUM(\"Price\") FROM \"Cars\"", "bare column aggregate unchanged")
}

// ===== A — take/skip before an aggregate =====

[test]
def test_take_before_count_emits_subquery(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price > 100) |> take(2) |> count())
    t |> success(sql == "SELECT COUNT(*) FROM (SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" > ? LIMIT ?) AS \"t0\"",
        "take before count wraps the pre-aggregate chain (LIMIT inner)")
}

[test]
def test_take_before_sum_emits_subquery(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> take(2) |> sum())
    t |> success(sql == "SELECT SUM(\"Price\") FROM (SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT ?) AS \"t0\"",
        "take before sum wraps; outer SUM references the inner column")
}

[test]
def test_take_count_filtered_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Price>100: Audi, Tesla, Lambo (3); take 2 of those -> 2
        let c = _sql(db |> select_from(type<Car>) |> _where(_.Price > 100) |> take(2) |> count())
        t |> equal(c, 2, "count of first 2 rows matching Price>100")
    }
}

[test]
def test_take_sum_aggregate_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // first 2 rows by insertion order: BMW(100) + Audi(200) = 300
        let s = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> take(2) |> sum())
        t |> equal(s, 300, "sum of first 2 prices")
    }
}

[test]
def test_skip_before_count_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // skip 3 of 5 -> 2 remain
        let c = _sql(db |> select_from(type<Car>) |> skip(3) |> count())
        t |> equal(c, 2, "count after skip(3)")
    }
}

[test]
def test_take_then_where_then_count_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // take(3) -> BMW(100), Audi(200), Tesla(300); then Price>100 -> Audi, Tesla = 2
        let c = _sql(db |> select_from(type<Car>) |> take(3) |> _where(_.Price > 100) |> count())
        t |> equal(c, 2, "take-then-filter-then-count")
    }
}

// ===== composition — computed _select + take + aggregate =====

[test]
def test_computed_select_take_sum_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // first 2 rows: (100+1)+(200+2) = 303
        let s = _sql(db |> select_from(type<Car>) |> _select(_.Price + _.Id) |> take(2) |> sum())
        t |> equal(s, 303, "computed projection summed over the first 2 rows")
    }
}
