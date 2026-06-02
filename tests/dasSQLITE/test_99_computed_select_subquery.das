options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Features exercised here — all "computed expression pushdown" via pred_to_sql:
//   B1 — a computed scalar `_select(_.a + _.b)` lowers to `<frag>` via pred_to_sql + a computed projection
//        slot, so `sum/min/max/average/count` over it works (previously rejected: "_.Field only").
//   B2 — computed COLUMNS in a named-tuple `_select((N=_.Name, Bonus=_.Price/10))` lower to `<frag> AS N`
//        via the same pred_to_sql + push_computed_proj_slot path (previously rejected: "_.Field only").
//   B3 — string `+` (daslang concatenation) lowers to SQL `||`, not `+` (SQLite `+` is numeric: 'a'+'b'==0).
//   C  — computed `_order_by(_.Price/10)` keys (single + tuple entry) render with constants inlined, like
//        _group_by computed keys (previously rejected: "_.Field or string only").
//   D  — bitwise `& | << >>` and unary `- ~` lower to the matching SQLite operators.
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

// ===== B2 — computed columns in a named-tuple _select (projection) =====

[test]
def test_computed_named_tuple_emits_fragment(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
        |> _select((Name = _.Name, Bonus = _.Price / 10))
        |> to_array())
    // plain column kept as a column ref; computed column rendered via pred_to_sql alongside it
    // (result tuple fields map by position via projRecordNames, so no cosmetic SQL alias).
    t |> success(sql == "SELECT \"Name\", (\"Price\") / (?) FROM \"Cars\"",
        "computed column pushed down next to the plain column; SQL: {sql}")
}

[test]
def test_computed_named_tuple_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Price/10: BMW 10, Audi 20, Tesla 30, Lada 5, Lambo 99
        let rows <- _sql(db |> select_from(type<Car>)
            |> _select((Name = _.Name, Bonus = _.Price / 10))
            |> to_array())
        t |> equal(length(rows), 5, "all rows projected")
        t |> equal(rows[0].Name, "BMW")
        t |> equal(rows[0].Bonus, 10, "BMW Price/10")
        t |> equal(rows[2].Bonus, 30, "Tesla Price/10")
        t |> equal(rows[4].Bonus, 99, "Lambo Price/10")
    }
}

[test]
def test_computed_named_tuple_with_computed_where_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // computed WHERE + computed SELECT together — exercises bind ordering (proj fragment + where binds).
        // where Price/10 > 20 -> Tesla(30), Lambo(99)
        let rows <- _sql(db |> select_from(type<Car>)
            |> _where(_.Price / 10 > 20)
            |> _select((Name = _.Name, Bonus = _.Price / 10))
            |> to_array())
        t |> equal(length(rows), 2, "filtered by computed predicate")
        t |> equal(rows[0].Name, "Tesla")
        t |> equal(rows[0].Bonus, 30)
        t |> equal(rows[1].Name, "Lambo")
        t |> equal(rows[1].Bonus, 99)
    }
}

[test]
def test_computed_mixed_with_plain_columns_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // a plain column and a computed column in the same projection
        let rows <- _sql(db |> select_from(type<Car>)
            |> _select((Id = _.Id, Doubled = _.Price * 2))
            |> to_array())
        t |> equal(rows[0].Id, 1)
        t |> equal(rows[0].Doubled, 200, "BMW Price*2")
        t |> equal(rows[4].Doubled, 1998, "Lambo Price*2")
    }
}

[test]
def test_bare_named_tuple_select_unchanged(t : T?) {
    // regression: a pure-column named-tuple must keep emitting plain column refs, not the computed path.
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>)
            |> _select((N = _.Name, P = _.Price))
            |> to_array())
        t |> equal(length(rows), 5)
        t |> equal(rows[1].N, "Audi")
        t |> equal(rows[1].P, 200)
    }
}

// ===== B3 — string concatenation (`+` → SQL `||`) =====

[test]
def test_string_concat_column_emits_pipe(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select((G = _.Name + "!")) |> to_array())
    t |> success(sql == "SELECT (\"Name\") || (?) FROM \"Cars\"", "string `+` lowers to SQL `||`; SQL: {sql}")
}

[test]
def test_string_concat_column_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // SQLite `+` would coerce both strings to 0; `||` concatenates.
        let rows <- _sql(db |> select_from(type<Car>) |> _select((G = _.Name + "!")) |> to_array())
        t |> equal(rows[0].G, "BMW!", "string concat returns text, not 0")
        t |> equal(rows[1].G, "Audi!")
    }
}

[test]
def test_string_concat_in_where_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let c = _sql(db |> select_from(type<Car>) |> _where(_.Name + "!" == "BMW!") |> count())
        t |> equal(c, 1, "string concat in WHERE matches via ||")
    }
}

// ===== C — computed _order_by keys (single + tuple entry), constants inlined =====

[test]
def test_computed_order_by_emits_fragment(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
        |> _order_by(_.Price / 10)
        |> _select(_.Name)
        |> to_array())
    t |> success(sql |> find("ORDER BY") >= 0 && sql |> find("/") >= 0 && sql |> find("?") < 0,
        "computed order key inlined (no binds); SQL: {sql}")
}

[test]
def test_computed_order_by_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // order by Price/10 asc: Lada(5), BMW(10), Audi(20), Tesla(30), Lambo(99)
        let rows <- _sql(db |> select_from(type<Car>)
            |> _order_by(_.Price / 10)
            |> _select(_.Name)
            |> to_array())
        t |> equal(length(rows), 5)
        t |> equal(rows[0], "Lada")
        t |> equal(rows[1], "BMW")
        t |> equal(rows[4], "Lambo")
    }
}

[test]
def test_computed_order_by_descending_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // order by Price/10 desc: Lambo(99), Tesla(30), Audi(20), BMW(10), Lada(5)
        let rows <- _sql(db |> select_from(type<Car>)
            |> _order_by_descending(_.Price / 10)
            |> _select(_.Name)
            |> to_array())
        t |> equal(rows[0], "Lambo")
        t |> equal(rows[4], "Lada")
    }
}

[test]
def test_computed_order_by_tuple_entry_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // order by (Price/100, Name): Price/100 -> BMW 1, Audi 2, Tesla 3, Lada 0, Lambo 9
        // asc: Lada(0), BMW(1), Audi(2), Tesla(3), Lambo(9)
        let rows <- _sql(db |> select_from(type<Car>)
            |> _order_by((_.Price / 100, _.Name))
            |> _select(_.Name)
            |> to_array())
        t |> equal(rows[0], "Lada")
        t |> equal(rows[1], "BMW")
        t |> equal(rows[4], "Lambo")
    }
}

// ===== D — bitwise (& | << >>) and unary (- ~) operators =====

[test]
def test_bitwise_and_emits(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select((V = _.Price & 96)) |> to_array())
    t |> success(sql == "SELECT (\"Price\") & (?) FROM \"Cars\"", "bitwise & lowers to SQL &; SQL: {sql}")
}

[test]
def test_bitwise_and_shift_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // BMW Price=100: 100&96=96, 100>>2=25, 100<<1=200, 100|3=103
        let a <- _sql(db |> select_from(type<Car>) |> _select((V = _.Price & 96)) |> to_array())
        t |> equal(a[0].V, 96, "Price & 96")
        let r <- _sql(db |> select_from(type<Car>) |> _select((V = _.Price >> 2)) |> to_array())
        t |> equal(r[0].V, 25, "Price >> 2")
        let l <- _sql(db |> select_from(type<Car>) |> _select((V = _.Price << 1)) |> to_array())
        t |> equal(l[0].V, 200, "Price << 1")
        let o <- _sql(db |> select_from(type<Car>) |> _select((V = _.Price | 3)) |> to_array())
        t |> equal(o[0].V, 103, "Price | 3")
    }
}

[test]
def test_unary_minus_and_not_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let neg <- _sql(db |> select_from(type<Car>) |> _select((V = -_.Price)) |> to_array())
        t |> equal(neg[0].V, -100, "-Price")
        let bn <- _sql(db |> select_from(type<Car>) |> _select((V = ~_.Id)) |> to_array())
        t |> equal(bn[0].V, -2, "~Id (two's complement)")
    }
}

[test]
def test_bitwise_in_where_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Price & 4 == 4 -> BMW(100), Tesla(300), Lambo(999)
        let c = _sql(db |> select_from(type<Car>) |> _where((_.Price & 4) == 4) |> count())
        t |> equal(c, 3, "count where Price & 4 == 4")
    }
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
