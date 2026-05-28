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

// ──────────────────────────────────────────────────────────────────────────
// Row-returning `_distinct_by(K)` chains (`|> to_array`, optionally with
// trailing `_order_by` / `take` / `skip`). The same bare-aggregate inner
// subquery as the aggregate path above, but the outer SELECT preserves all
// source columns instead of folding to a scalar. Fixed a latent silent-drop
// bug from chunk N where `_distinct_by(K) |> to_array` emitted the source
// table without GROUP BY.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_distinct_by_to_array_emits_subquery_form(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _distinct_by(_.Name))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\"",
        "_distinct_by(K) |> to_array wraps source in bare-aggregate subquery (closes silent-drop bug)")
}

[test]
def test_distinct_by_to_array_returns_first_row_per_K(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // Fixture: BMW×2 (Ids 1,3), Audi×2 (Ids 2,5), Tesla×1 (Id 4).
        // First-per-Name by PK: Id 1 (BMW, 100), Id 2 (Audi, 200), Id 4 (Tesla, 300).
        let rows <- _sql(db |> select_from(type<Car>) |> _distinct_by(_.Name))
        t |> equal(length(rows), 3, "3 distinct Names → 3 rows")
        // Build a set of returned Ids to assert membership independent of engine row order.
        var ids : table<int>
        for (r in rows) {
            ids |> insert(r.Id)
        }
        t |> equal(key_exists(ids, 1), true, "first BMW (Id=1) survives")
        t |> equal(key_exists(ids, 2), true, "first Audi (Id=2) survives")
        t |> equal(key_exists(ids, 4), true, "Tesla (Id=4) survives")
    }
}

[test]
def test_distinct_by_order_by_emits_outer_order(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _distinct_by(_.Name)
                          |> _order_by(_.Price))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\" ORDER BY \"Price\" ASC",
        "_distinct_by(K) |> _order_by(S) emits outer ORDER BY over bare-aggregate subquery")
}

[test]
def test_distinct_by_order_by_returns_sorted_first_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // First-per-Name prices: BMW=100, Audi=200, Tesla=300 (already in ASC order).
        let rows <- _sql(db |> select_from(type<Car>)
                            |> _distinct_by(_.Name)
                            |> _order_by(_.Price))
        t |> equal(length(rows), 3, "3 rows")
        t |> equal(rows[0].Price, 100, "first row: BMW@100")
        t |> equal(rows[1].Price, 200, "second row: Audi@200")
        t |> equal(rows[2].Price, 300, "third row: Tesla@300")
    }
}

[test]
def test_distinct_by_order_by_take_emits_outer_limit(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _distinct_by(_.Name)
                          |> _order_by(_.Price)
                          |> take(2))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\" ORDER BY \"Price\" ASC LIMIT ?",
        "_distinct_by(K) |> _order_by(S) |> take(N) emits outer LIMIT")
}

[test]
def test_distinct_by_order_by_take_returns_top_n(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        let rows <- _sql(db |> select_from(type<Car>)
                            |> _distinct_by(_.Name)
                            |> _order_by(_.Price)
                            |> take(2))
        t |> equal(length(rows), 2, "take(2) → 2 rows")
        t |> equal(rows[0].Name, "BMW", "cheapest first-per-Name: BMW@100")
        t |> equal(rows[1].Name, "Audi", "next: Audi@200")
    }
}

[test]
def test_distinct_by_skip_emits_outer_offset(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _distinct_by(_.Name)
                          |> _order_by(_.Price)
                          |> skip(1))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\" ORDER BY \"Price\" ASC LIMIT -1 OFFSET ?",
        "_distinct_by(K) |> _order_by(S) |> skip(N) emits outer OFFSET (LIMIT -1 OFFSET ? when no take)")
}

// ──────────────────────────────────────────────────────────────────────────
// `reverse() |> _distinct_by(K)` — MAX(pk) variant of the bare-aggregate
// wrap, picks the LAST row per K by source/PK order (mirrors linq's
// `each(arr).reverse()._distinct_by(K)` "last per group" splice).
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_reverse_distinct_by_emits_max_pk(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> reverse() |> _distinct_by(_.Name))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MAX(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\"",
        "reverse() |> _distinct_by(K) emits MAX(pk) in the inner subquery")
}

[test]
def test_reverse_distinct_by_returns_last_row_per_K(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        // Fixture: BMW×2 (Ids 1,3), Audi×2 (Ids 2,5), Tesla×1 (Id 4).
        // LAST-per-Name by PK: Id 3 (BMW), Id 5 (Audi, 200), Id 4 (Tesla).
        let rows <- _sql(db |> select_from(type<Car>) |> reverse() |> _distinct_by(_.Name))
        t |> equal(length(rows), 3, "3 distinct Names → 3 rows")
        var ids : table<int>
        for (r in rows) {
            ids |> insert(r.Id)
        }
        t |> equal(key_exists(ids, 3), true, "last BMW (Id=3) survives")
        t |> equal(key_exists(ids, 5), true, "last Audi (Id=5) survives")
        t |> equal(key_exists(ids, 4), true, "Tesla (Id=4) survives")
    }
}

[test]
def test_reverse_distinct_by_composes_with_order_by(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> reverse()
                          |> _distinct_by(_.Name)
                          |> _order_by(_.Price))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MAX(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\" ORDER BY \"Price\" ASC",
        "MAX(pk) composes with outer ORDER BY same as MIN(pk)")
}

// ──────────────────────────────────────────────────────────────────────────
// `_group_by(K) |> _select((K=_._0, F=_._1 |> first()))` — single-key
// group + tuple projection of (group key, first row per group). Lowers via
// the same bare-aggregate inner subquery as `_distinct_by(K)` passthrough,
// expanding the row entry to all source columns in the outer SELECT and
// reconstructing the struct via `ExprMakeStruct` in the row builder.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_groupby_first_emits_bare_aggregate_with_expanded_row(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _group_by(_.Name)
                          |> _select((Brand = _._0, FirstCar = _._1 |> first())))
    t |> equal(sql,
        "SELECT \"Name\", \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\"",
        "_group_by(K) |> _select((K=_._0, R=_._1|>first())) wraps source in bare-aggregate subquery and expands the row entry to all source columns")
}

[test]
def test_groupby_first_returns_key_plus_first_row_per_group(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        let groups <- _sql(db |> select_from(type<Car>)
                              |> _group_by(_.Name)
                              |> _select((Brand = _._0, FirstCar = _._1 |> first())))
        t |> equal(length(groups), 3, "3 distinct Names → 3 groups")
        var firstIdByBrand <- {for (g in groups); g.Brand => g.FirstCar.Id}
        t |> equal(firstIdByBrand["BMW"], 1, "first BMW has Id=1")
        t |> equal(firstIdByBrand["Audi"], 2, "first Audi has Id=2")
        t |> equal(firstIdByBrand["Tesla"], 4, "first Tesla has Id=4")
    }
}

[test]
def test_groupby_first_row_fields_populated(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        let groups <- _sql(db |> select_from(type<Car>)
                              |> _group_by(_.Name)
                              |> _select((Brand = _._0, FirstCar = _._1 |> first())))
        for (g in groups) {
            // Each FirstCar must have all 3 source fields populated correctly.
            t |> equal(g.Brand, g.FirstCar.Name, "Brand key matches FirstCar.Name (consistency)")
            t |> equal(g.FirstCar.Id > 0, true, "FirstCar.Id is non-zero — row builder populated it from column 1")
            t |> equal(g.FirstCar.Price > 0, true, "FirstCar.Price is non-zero — row builder populated it from column 3")
        }
    }
}
