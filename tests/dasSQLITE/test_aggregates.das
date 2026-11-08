options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",    Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo",   Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// _count canary — proves the aggregate framework path:
//   - ProjectionShape.Aggregate sets the SELECT-list to "COUNT(*)"
//   - Materializer.One reads back a single int (matches daslang's
//     `length(arr)`/`count(arr)` return type; SQLite COUNT(*) is int64
//     internally but `read_via_adapter(stmt, 0, type<int>)` narrows it)
//
// `_count` is reserved as a 2-arg call_macro in linq_boost (predicate form,
// chunk 4 territory). For the bare aggregate we use linq.das's `count(src)`
// directly (no underscore).
// ──────────────────────────────────────────────────────────────────────────

[test(description = "count over a whole table returns the row total")]
def test_count_returns_total(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> count())
        t |> equal(n, 5, "5 rows total")
    }
}

[test(description = "count over an empty table returns 0")]
def test_count_on_empty_table_is_zero(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let n = _sql(db |> select_from(type<Car>) |> count())
        t |> equal(n, 0, "empty table counts 0")
    }
}

[test(description = "count emits a COUNT star projection")]
def test_count_emits_count_star(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> count())
    t |> equal(sql, "SELECT COUNT(*) FROM \"Cars\"", "_count() emits COUNT(*) projection")
}

// ──────────────────────────────────────────────────────────────────────────
// _select |> count(): _select before an aggregate terminal is a soft
// pass-through — the aggregate's COUNT(*) wins. Verifies the analyzer
// accepts the chain and emits the same SQL as plain count().
// ──────────────────────────────────────────────────────────────────────────

[test(description = "count after a column select returns the row total, not a column count")]
def test_count_after_select_returns_same_total(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> _select(_.Name) |> count())
        t |> equal(n, 5, "_select(_.Name) |> count() returns row total, not column count")
    }
}

[test(description = "a select before count is ignored and the aggregate still emits COUNT star")]
def test_count_after_select_emits_count_star(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Name) |> count())
    t |> equal(sql, "SELECT COUNT(*) FROM \"Cars\"",
        "_select before count() is ignored; aggregate emits COUNT(*) regardless")
}

[test(description = "count after a where counts only the matching rows")]
def test_count_with_where_filter(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cutoff = 150
        let n = _sql(db |> select_from(type<Car>)
                       |> _where(_.Price > cutoff)
                       |> count())
        t |> equal(n, 3, "3 rows with Price > 150")
    }
}

[test(description = "count after a where that matches nothing returns 0")]
def test_count_with_no_matches_is_zero(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let target = 999999
        let n = _sql(db |> select_from(type<Car>)
                       |> _where(_.Id == target)
                       |> count())
        t |> equal(n, 0, "no matches -> 0")
    }
}

[test(description = "count after a where emits COUNT star with the WHERE clause")]
def test_count_with_where_emits_count_with_where_sql(t : T?) {
    var _db = SqlRunner()
    let _cutoff = 150
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > _cutoff)
                          |> count())
    t |> equal(sql,
        "SELECT COUNT(*) FROM \"Cars\" WHERE \"Price\" > ?",
        "count + where emits COUNT(*) FROM ... WHERE ...")
}

[sql_table(name = "Cars")]
struct Car_31 {
    @sql_primary_key Id : int
    Brand : string
    Price : int
}

def private fixture_31(db : SqlRunner) {
    db |> create_table(type<Car_31>)
    db |> insert(Car_31(Id = 1, Brand = "BMW",   Price = 100))
    db |> insert(Car_31(Id = 2, Brand = "Audi",  Price = 200))
    db |> insert(Car_31(Id = 3, Brand = "BMW",   Price = 300))
    db |> insert(Car_31(Id = 4, Brand = "Tesla", Price = 50))
    db |> insert(Car_31(Id = 5, Brand = "BMW",   Price = 999))
}

[test(description = "long_count returns the row total as an int64")]
def test_long_count_returns_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_31(db)
        let n = _sql(db |> select_from(type<Car_31>) |> long_count())
        t |> equal(n, 5l, "5 rows total (int64)")
    }
}

[test(description = "long_count emits a COUNT star projection")]
def test_long_count_emits_count_star(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car_31>) |> long_count())
    t |> equal(sql, "SELECT COUNT(*) FROM \"Cars\"", "long_count() emits COUNT(*)")
}

[test(description = "long_count after a where counts only the matching rows")]
def test_long_count_with_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_31(db)
        let n = _sql(db |> select_from(type<Car_31>) |> _where(_.Price > 99) |> long_count())
        t |> equal(n, 4l, "4 rows where Price > 99")
    }
}

[test(description = "count after distinct_by emits COUNT DISTINCT of the key column")]
def test_distinct_by_count_emits_count_distinct(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car_31>) |> _distinct_by(_.Brand) |> count())
    t |> equal(sql, "SELECT COUNT(DISTINCT \"Brand\") FROM \"Cars\"",
        "_distinct_by(_.Brand) |> count() emits COUNT(DISTINCT \"Brand\")")
}

[test(description = "count after distinct_by returns the number of distinct keys")]
def test_distinct_by_count_returns_distinct_brand_total(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_31(db)
        let n = _sql(db |> select_from(type<Car_31>) |> _distinct_by(_.Brand) |> count())
        t |> equal(n, 3, "3 distinct brands")
    }
}

[test(description = "long_count after distinct_by returns the distinct-key count as an int64")]
def test_distinct_by_long_count_returns_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_31(db)
        let n = _sql(db |> select_from(type<Car_31>) |> _distinct_by(_.Brand) |> long_count())
        t |> equal(n, 3l, "3 distinct brands (int64)")
    }
}

[test(description = "long_count after distinct_by emits the same COUNT DISTINCT as count")]
def test_distinct_by_long_count_emits_count_distinct(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car_31>) |> _distinct_by(_.Brand) |> long_count())
    t |> equal(sql, "SELECT COUNT(DISTINCT \"Brand\") FROM \"Cars\"",
        "long_count after _distinct_by emits the same COUNT(DISTINCT col) as count")
}

[test(description = "long_count inside a group_by accumulates group sizes as int64")]
def test_group_by_long_count_widens_to_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_31(db)
        let groups <- _sql(db |> select_from(type<Car_31>)
                              |> _group_by(_.Brand)
                              |> _select((Brand = _._0, N = _._1 |> long_count())))
        t |> equal(length(groups), 3, "3 distinct brands grouped")
        var total = 0l
        for (g in groups) {
            total += g.N
        }
        t |> equal(total, 5l, "group totals sum to row count (int64 accumulator)")
    }
}

def private fixture_35(db : SqlRunner) {
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

[test(description = "sum emits SUM of the selected column")]
def test_sum_emits_sum_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> sum())
    t |> equal(sql,
        "SELECT SUM(\"Price\") FROM \"Cars\"",
        "sum() emits SELECT SUM(col)")
}

[test(description = "average emits AVG of the selected column")]
def test_average_emits_avg_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> average())
    t |> equal(sql,
        "SELECT AVG(\"Price\") FROM \"Cars\"",
        "average() emits SELECT AVG(col)")
}

[test(description = "min emits MIN of the selected column")]
def test_min_emits_min_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> min())
    t |> equal(sql,
        "SELECT MIN(\"Price\") FROM \"Cars\"",
        "min() emits SELECT MIN(col)")
}

[test(description = "max emits MAX of the selected column")]
def test_max_emits_max_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price) |> max())
    t |> equal(sql,
        "SELECT MAX(\"Price\") FROM \"Cars\"",
        "max() emits SELECT MAX(col)")
}

[test(description = "sum returns the total of the column over all rows")]
def test_sum_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_35(db)
        let total = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> sum())
        t |> equal(total, 100 + 200 + 300 + 50 + 999, "SUM of all prices")
    }
}

[test(description = "min returns the smallest value of the column")]
def test_min_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_35(db)
        let cheapest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> min())
        t |> equal(cheapest, 50, "min Price = Lada")
    }
}

[test(description = "max returns the largest value of the column")]
def test_max_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_35(db)
        let priciest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> max())
        t |> equal(priciest, 999, "max Price = Lambo")
    }
}

[test(description = "average returns a double even for an integer column")]
def test_average_runtime_promotes_to_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_35(db)
        let avg = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> average())
        // AVG returns double; allow tiny FP slop.
        let expected = double(100 + 200 + 300 + 50 + 999) / 5.0lf
        t |> equal(avg, expected, "AVG returns double")
    }
}

[test(description = "sum after a where emits SUM with the WHERE clause")]
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

[test(description = "sum after a where totals only the matching rows")]
def test_sum_with_where_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_35(db)
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
