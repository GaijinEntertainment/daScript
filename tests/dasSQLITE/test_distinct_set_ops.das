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

[test(description = "distinct on a full-row chain emits SELECT DISTINCT")]
def test_distinct_full_row_emits_select_distinct(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> distinct())
    t |> equal(sql,
        "SELECT DISTINCT \"Id\", \"Name\", \"Price\" FROM \"Cars\"",
        "distinct() emits SELECT DISTINCT before the full column list")
}

[test(description = "distinct after a where emits SELECT DISTINCT with the WHERE clause")]
def test_distinct_with_where_emits_distinct_and_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > 100)
                          |> distinct())
    t |> equal(sql,
        "SELECT DISTINCT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" > ?",
        "distinct() composes with _where")
}

[test(description = "full-row distinct keeps every row whose whole row is unique")]
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

[test(description = "distinct_by with a counting predicate emits a first-row-per-key subquery under the count")]
def test_distinct_by_count_pred_emits_subquery_form(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                            |> _distinct_by(_.Name)
                            |> _count(_.Price > 150))
    t |> equal(sql,
        "SELECT COUNT(*) FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\" WHERE \"Price\" > ?",
        "_distinct_by + _count(P) emits COUNT(*) FROM (bare-aggregate subquery) WHERE")
}

[test(description = "a counting predicate after distinct_by is evaluated on the first row of each key")]
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

[test(description = "long_count with a predicate after distinct_by returns an int64 with first-row semantics")]
def test_distinct_by_long_count_pred_returns_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_with_dupes(db)
        let n64 = _sql(db |> select_from(type<Car>)
                          |> _distinct_by(_.Name)
                          |> _long_count(_.Price > 150))
        t |> equal(n64, 2l, "long_count(P) returns int64 with first-row semantics")
    }
}

[test(description = "sum after distinct_by emits SUM over the first-row-per-key subquery")]
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

[test(description = "sum after distinct_by adds up only the first row of each key")]
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

[test(description = "min after distinct_by ranges over the first row of each key")]
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

[test(description = "max after distinct_by ranges over the first row of each key")]
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

[test(description = "average after distinct_by returns a double over the first row of each key")]
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

[test(description = "distinct_by to_array emits a SELECT over the first-row-per-key subquery")]
def test_distinct_by_to_array_emits_subquery_form(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _distinct_by(_.Name))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\"",
        "_distinct_by(K) |> to_array wraps source in bare-aggregate subquery (closes silent-drop bug)")
}

[test(description = "distinct_by returns exactly the first row of each key")]
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

[test(description = "order_by after distinct_by orders the outer query over the subquery")]
def test_distinct_by_order_by_emits_outer_order(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _distinct_by(_.Name)
                          |> _order_by(_.Price))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\" ORDER BY \"Price\" ASC",
        "_distinct_by(K) |> _order_by(S) emits outer ORDER BY over bare-aggregate subquery")
}

[test(description = "order_by after distinct_by returns the first rows in sorted order")]
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

[test(description = "take after distinct_by and order_by emits an outer LIMIT")]
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

[test(description = "take after distinct_by and order_by returns the top rows of the sorted first rows")]
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

[test(description = "skip after distinct_by emits an outer OFFSET with an unbounded LIMIT")]
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

[test(description = "reverse before distinct_by picks the last row per key by selecting MAX of the primary key")]
def test_reverse_distinct_by_emits_max_pk(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> reverse() |> _distinct_by(_.Name))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT *, MAX(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\"",
        "reverse() |> _distinct_by(K) emits MAX(pk) in the inner subquery")
}

[test(description = "reverse distinct_by returns exactly the last row of each key")]
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

[test(description = "reverse distinct_by composes with order_by over the max-key subquery")]
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

[test(description = "a group_by that takes first emits a bare aggregate and the expanded first row")]
def test_groupby_first_emits_bare_aggregate_with_expanded_row(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _group_by(_.Name)
                          |> _select((Brand = _._0, FirstCar = _._1 |> first())))
    t |> equal(sql,
        "SELECT \"Name\" AS \"Brand\", \"Id\", \"Name\", \"Price\" FROM (SELECT *, MIN(\"Id\") FROM \"Cars\" GROUP BY \"Name\") AS \"t0\"",
        "_group_by(K) |> _select((K=_._0, R=_._1|>first())) wraps source in bare-aggregate subquery and expands the row entry to all source columns")
}

[test(description = "a group_by that takes first returns the key with the first row of each group")]
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

[test(description = "the first row of a group_by group has every field populated from its columns")]
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

[sql_table(name="A")]
struct TabA {
    @sql_primary_key Id : int
    Tag : int
}

[sql_table(name="B")]
struct TabB {
    @sql_primary_key Id : int
    Tag : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<TabA>)
    db |> create_table(type<TabB>)
    db |> insert(TabA(Id = 1, Tag = 10))
    db |> insert(TabA(Id = 2, Tag = 20))
    db |> insert(TabA(Id = 3, Tag = 30))
    db |> insert(TabB(Id = 100, Tag = 20))
    db |> insert(TabB(Id = 200, Tag = 30))
    db |> insert(TabB(Id = 300, Tag = 40))
}

// ──────────────────────────────────────────────────────────────────────────
// SQL emission — UNION / INTERSECT / EXCEPT wrap the LHS query and append
// the RHS query separated by the set-op keyword.
// Both sides project the same column shape (here `_.Tag`) so the resulting
// schemas align.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "union of two queries emits the UNION keyword between both SELECTs")]
def test_union_emits_union(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text((_db |> select_from(type<TabA>) |> _select(_.Tag))
                        |> union(_db |> select_from(type<TabB>) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" UNION SELECT \"Tag\" FROM \"B\"",
        "union(a, b) emits `<a-sql> UNION <b-sql>`")
}

[test(description = "intersect of two queries emits the INTERSECT keyword between both SELECTs")]
def test_intersect_emits_intersect(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text((_db |> select_from(type<TabA>) |> _select(_.Tag))
                        |> intersect(_db |> select_from(type<TabB>) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" INTERSECT SELECT \"Tag\" FROM \"B\"",
        "intersect(a, b) emits `<a-sql> INTERSECT <b-sql>`")
}

[test(description = "except of two queries emits the EXCEPT keyword between both SELECTs")]
def test_except_emits_except(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text((_db |> select_from(type<TabA>) |> _select(_.Tag))
                        |> except(_db |> select_from(type<TabB>) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" EXCEPT SELECT \"Tag\" FROM \"B\"",
        "except(a, b) emits `<a-sql> EXCEPT <b-sql>`")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime — verify the actual rows match the set-op semantics.
// A.Tag values: {10, 20, 30}; B.Tag values: {20, 30, 40}
// ──────────────────────────────────────────────────────────────────────────

[test(description = "union returns the distinct values of both tables")]
def test_union_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var rows <- _sql((db |> select_from(type<TabA>) |> _select(_.Tag))
                         |> union(db |> select_from(type<TabB>) |> _select(_.Tag)))
        // UNION dedupes: {10, 20, 30, 40}
        sort(rows)
        t |> equal(length(rows), 4, "UNION = 4 distinct tags")
        t |> equal(rows[0], 10)
        t |> equal(rows[1], 20)
        t |> equal(rows[2], 30)
        t |> equal(rows[3], 40)
    }
}

[test(description = "intersect returns the values present in both tables")]
def test_intersect_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var rows <- _sql((db |> select_from(type<TabA>) |> _select(_.Tag))
                         |> intersect(db |> select_from(type<TabB>) |> _select(_.Tag)))
        // {20, 30}
        sort(rows)
        t |> equal(length(rows), 2)
        t |> equal(rows[0], 20)
        t |> equal(rows[1], 30)
    }
}

[test(description = "except returns the values of the left table missing from the right")]
def test_except_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var rows <- _sql((db |> select_from(type<TabA>) |> _select(_.Tag))
                         |> except(db |> select_from(type<TabB>) |> _select(_.Tag)))
        // A's tags minus B's = {10}
        sort(rows)
        t |> equal(length(rows), 1)
        t |> equal(rows[0], 10)
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Compound-query bind ordering — set-op SQL is `<lhs> {OP} <rhs>` with no
// per-side parens, so placeholders run left-to-right across both sides.
// The runtime bind list must follow SQL clause order:
//   LHS(on → where → having)  then  RHS(on → where → having)
// These tests pin that ordering: a misordered build would produce wrong
// runtime values OR get caught by the SQL emission check (placeholder count
// vs. declared bind count) before runtime.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "a set operation with a where on each side emits both WHEREs in order")]
def test_set_op_basic_lhs_rhs_where_binds(t : T?) {
    var _db = SqlRunner()
    let _lo = 15
    let _hi = 35
    let sql = _sql_text(
        (_db |> select_from(type<TabA>) |> _where(_.Tag >= _lo) |> _select(_.Tag))
        |> union(_db |> select_from(type<TabB>) |> _where(_.Tag <= _hi) |> _select(_.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" WHERE \"Tag\" >= ? UNION SELECT \"Tag\" FROM \"B\" WHERE \"Tag\" <= ?",
        "compound query with WHERE on both sides emits two placeholders, LHS then RHS")
}

[test(description = "a set operation binds the left where parameters before the right where parameters")]
def test_set_op_basic_lhs_rhs_where_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let lo = 15
        let hi = 35
        var rows <- _sql(
            (db |> select_from(type<TabA>) |> _where(_.Tag >= lo) |> _select(_.Tag))
            |> union(db |> select_from(type<TabB>) |> _where(_.Tag <= hi) |> _select(_.Tag)))
        // A.Tag where >= 15 → {20, 30}; B.Tag where <= 35 → {20, 30}; UNION → {20, 30}
        sort(rows)
        t |> equal(length(rows), 2, "compound runtime: bind order must match LHS-WHERE first, RHS-WHERE second")
        t |> equal(rows[0], 20)
        t |> equal(rows[1], 30)
    }
}

[test(description = "a left HAVING and a right WHERE bind their parameters in placeholder order")]
def test_set_op_lhs_having_then_rhs_where_runtime(t : T?) {
    // The discriminating test for the round-3 bind-ordering fix: LHS has
    // both _where (placeholder #1) AND _having (placeholder #2), then UNION,
    // then RHS has _where (placeholder #3). Pre-fix, LHS HAVING binds were
    // pushed to q.havingBindExprs which the finalizer appended LAST — so
    // placeholders would have ended up in order [LHS-WHERE, RHS-WHERE,
    // LHS-HAVING], silently swapping LHS-HAVING and RHS-WHERE values.
    //
    // Both sides project as named-tuple `(Tg : int)` so the UNION column
    // shape matches (a `_select` after `_group_by` must be a named tuple).
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let lhs_min_tag = 15      // LHS WHERE bind value
        let lhs_min_count = 1     // LHS HAVING bind value (each Tag occurs once in A)
        let rhs_max_tag = 25      // RHS WHERE bind value
        var rows <- _sql(
            (db |> select_from(type<TabA>)
                |> _where(_.Tag >= lhs_min_tag)
                |> _group_by(_.Tag)
                |> _having(_._1 |> length >= lhs_min_count)
                |> _select((Tg = _._0)))
            |> union(db |> select_from(type<TabB>) |> _where(_.Tag <= rhs_max_tag) |> _select((Tg = _.Tag))))
        // LHS: A.Tag where >= 15, GROUP BY Tag, HAVING count >= 1 → {20, 30}
        // RHS: B.Tag where <= 25 → {20}
        // UNION: distinct {20, 30}
        // If the misordering were present, swapping placeholders #2 and #3
        // would assign rhs_max_tag=25 to the LHS HAVING (count >= 25 → 0
        // rows) and lhs_min_count=1 to the RHS WHERE (Tag <= 1 → 0 rows),
        // producing an empty result.
        var tags <- [for (r in rows); r.Tg]
        sort(tags)
        t |> equal(length(tags), 2, "LHS HAVING + RHS WHERE binds must occupy correct placeholder positions")
        t |> equal(tags[0], 20)
        t |> equal(tags[1], 30)
    }
}

[test(description = "a set operation whose left side groups with HAVING emits the clauses in order")]
def test_set_op_lhs_having_emits_correct_sql(t : T?) {
    var _db = SqlRunner()
    let _lo = 15
    let _mc = 1
    let _hi = 25
    let sql = _sql_text(
        (_db |> select_from(type<TabA>)
            |> _where(_.Tag >= _lo)
            |> _group_by(_.Tag)
            |> _having(_._1 |> length >= _mc)
            |> _select((Tg = _._0)))
        |> union(_db |> select_from(type<TabB>) |> _where(_.Tag <= _hi) |> _select((Tg = _.Tag))))
    t |> equal(sql,
        "SELECT \"Tag\" AS \"Tg\" FROM \"A\" WHERE \"Tag\" >= ? GROUP BY \"Tag\" HAVING COUNT(*) >= ? UNION SELECT \"Tag\" AS \"Tg\" FROM \"B\" WHERE \"Tag\" <= ?",
        "compound SQL preserves clause order: LHS WHERE → GROUP BY → HAVING → UNION → RHS WHERE")
}

[test(description = "a set operation whose right side joins with a bound ON condition emits the join and the bind")]
def test_set_op_rhs_join_on_bind_emits_correct_sql(t : T?) {
    // RHS has a join whose right-side _where carries a bind — the bind
    // belongs to the JOIN's ON clause (per chunk-5 ON-emission). The fix
    // also walks subQ.joins to fold their rightOnBindExprs into the
    // compound-query placeholder list.
    var _db = SqlRunner()
    let _bnd = 5
    let sql = _sql_text(
        (_db |> select_from(type<TabA>) |> _select(_.Tag))
        |> union(_db |> select_from(type<TabA>)
                    |> _join(_db |> select_from(type<TabB>) |> _where(_.Tag > _bnd),
                             $(a : TabA, b : TabB) => a.Id == b.Id,
                             $(a : TabA, b : TabB) => a.Tag)))
    t |> equal(sql,
        "SELECT \"Tag\" FROM \"A\" UNION SELECT \"t0\".\"Tag\" FROM \"A\" AS \"t0\" INNER JOIN \"B\" AS \"t1\" ON \"t0\".\"Id\" = \"t1\".\"Id\" AND (\"t1\".\"Tag\" > ?)",
        "RHS join right-side _where placeholder appears inside compound query's RHS ON clause")
}
