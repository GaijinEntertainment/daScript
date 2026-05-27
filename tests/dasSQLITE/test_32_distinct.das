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

def private fixture_with_dupes(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "BMW",   Price = 150))
    db |> insert(Car(Id = 4, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 5, Name = "Audi",  Price = 200))
}

// ──────────────────────────────────────────────────────────────────────────
// distinct() — emits SELECT DISTINCT. The framework slot is a single bool
// on SqlQuery; the SELECT-list still comes from the rest of the chain
// (FullRow / SingleColumn / NamedTuple). Tests here cover:
//   - bare full-row DISTINCT (deduplicates whole rows)
//   - emitted SQL via _sql_text
//   - duplicate distinct() in chain → macro_error (covered in failed_*)
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_distinct_full_row_emits_select_distinct(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> distinct())
    t |> equal(sql,
        "SELECT DISTINCT \"Id\", \"Name\", \"Price\" FROM \"Cars\"",
        "distinct() emits SELECT DISTINCT before the full column list")
}

[test]
def test_distinct_with_where_emits_distinct_and_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > 100)
                          |> distinct())
    t |> equal(sql,
        "SELECT DISTINCT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" > ?",
        "distinct() composes with _where")
}

[test]
def test_distinct_full_row_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // All 5 inserted rows have unique PKs (Id), so full-row DISTINCT
        // returns all 5. That confirms the SELECT DISTINCT emission roundtrips
        // through the runtime without losing rows.
        let rows <- _sql(db |> select_from(type<Car>) |> distinct())
        t |> equal(length(rows), 5, "all 5 PK-distinct rows survive full-row DISTINCT")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// `_distinct_by(_.K) |> _count(P) / _long_count(P)` and
// `_distinct_by(_.K) |> _select(_.F) |> sum() / min() / max() / average()`
// — the SQLite bare-aggregate wrap added in chunk N. Inner subquery
// `SELECT *, MIN("Id") FROM "Cars" GROUP BY "K"` picks the row with the
// smallest PK per group (first-row-per-K semantics matching linq
// _distinct_by); outer SELECT applies the aggregate over those rows.
//
// Fixture has 5 rows across 3 distinct names (BMW×2, Audi×2, Tesla×1).
// First-encountered (by Id) is BMW=100, Audi=200, Tesla=300.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_distinct_by_count_pred_emits_subquery_form(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                            |> _distinct_by(_.Name)
                            |> _count(_.Price > 150))
    t |> equal(sql,
        "SELECT COUNT(*) FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\" WHERE \"Price\" > ?",
        "_distinct_by + _count(P) emits COUNT(*) FROM (bare-aggregate subquery) WHERE")
}

[test]
def test_distinct_by_count_pred_returns_first_row_match_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // First-encountered per Name: BMW=100, Audi=200, Tesla=300.
        // Count first-rows where Price > 150 → Audi(200) + Tesla(300) = 2.
        let n = _sql(db |> select_from(type<Car>)
                        |> _distinct_by(_.Name)
                        |> _count(_.Price > 150))
        t |> equal(n, 2, "first-row semantics: BMW(100) excluded, Audi(200) + Tesla(300) match")
    }
}

[test]
def test_distinct_by_long_count_pred_returns_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        let n64 = _sql(db |> select_from(type<Car>)
                          |> _distinct_by(_.Name)
                          |> _long_count(_.Price > 150))
        t |> equal(n64, 2l, "long_count(P) returns int64 with first-row semantics")
    }
}

[test]
def test_distinct_by_select_sum_emits_subquery_form(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                            |> _distinct_by(_.Name)
                            |> _select(_.Price)
                            |> sum())
    t |> equal(sql,
        "SELECT SUM(\"Price\") FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\"",
        "_distinct_by + _select(F) + sum() emits SUM(F) FROM (bare-aggregate subquery)")
}

[test]
def test_distinct_by_select_sum_returns_first_row_sum(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // First-row prices per Name: 100 + 200 + 300 = 600.
        let s = _sql(db |> select_from(type<Car>)
                        |> _distinct_by(_.Name)
                        |> _select(_.Price)
                        |> sum())
        t |> equal(s, 600, "sum of first-row prices per Name = 100+200+300 = 600")
    }
}

[test]
def test_distinct_by_select_min_returns_smallest_first_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // First-row prices per Name: BMW=100, Audi=200, Tesla=300; min = 100.
        let m = _sql(db |> select_from(type<Car>)
                        |> _distinct_by(_.Name)
                        |> _select(_.Price)
                        |> min())
        t |> equal(m, 100, "min of first-row prices = BMW(100)")
    }
}

[test]
def test_distinct_by_select_max_returns_largest_first_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        let m = _sql(db |> select_from(type<Car>)
                        |> _distinct_by(_.Name)
                        |> _select(_.Price)
                        |> max())
        t |> equal(m, 300, "max of first-row prices = Tesla(300)")
    }
}

[test]
def test_distinct_by_select_average_returns_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // Average of {100, 200, 300} = 200.0
        let a = _sql(db |> select_from(type<Car>)
                        |> _distinct_by(_.Name)
                        |> _select(_.Price)
                        |> average())
        t |> equal(a, 200.0lf, "average of first-row prices = 200.0 (DOUBLE)")
    }
}
