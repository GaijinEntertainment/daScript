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
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// take(n) / skip(n) — emit LIMIT / OFFSET. Bind ordering:
// LIMIT/OFFSET binds come AFTER WHERE binds in the placeholder index
// sequence.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "take emits LIMIT with a bind")]
def test_take_emits_limit(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> take(2))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT ?",
        "take(n) emits LIMIT ?")
}

[test(description = "skip emits an unbounded LIMIT with an OFFSET bind")]
def test_skip_emits_limit_neg1_offset(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> skip(2))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT -1 OFFSET ?",
        "skip(n) without take needs LIMIT -1 to satisfy SQLite's OFFSET-requires-LIMIT rule")
}

[test(description = "take followed by skip wraps the limited query in a subquery before applying the offset")]
def test_take_then_skip_emits_subquery(t : T?) {
    var _db = SqlRunner()
    // Multi-Q lowering: `take(3) |> skip(1)` is divergent from canonical
    // SQL phase order (linq positional says "take 3, drop 1"; flat `LIMIT
    // 3 OFFSET 1` would skip-then-take = 3 rows). Lower to nested SELECT
    // so all three modes agree (`max(0, n-m)` rows).
    let sql = _sql_text(_db |> select_from(type<Car>) |> take(3) |> skip(1))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM (SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT ?) AS \"t0\" LIMIT -1 OFFSET ?",
        "take + skip lowers to nested SELECT (take in inner, skip in outer)")
}

[test(description = "skip followed by take flattens into one LIMIT with OFFSET")]
def test_skip_then_take_emits_flat_limit_offset(t : T?) {
    var _db = SqlRunner()
    // Canonical pagination: `skip(m) |> take(n)` matches SQL `LIMIT n
    // OFFSET m` semantics (skip-then-take). Stays a flat SELECT.
    let sql = _sql_text(_db |> select_from(type<Car>) |> skip(1) |> take(3))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT ? OFFSET ?",
        "skip + take stays flat — canonical pagination shape")
}

[test(description = "a where before take emits the WHERE bind ahead of the LIMIT bind")]
def test_where_then_take_orders_binds_correctly(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > 100)
                          |> take(2))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" > ? LIMIT ?",
        "WHERE bind goes first, LIMIT bind goes last — placeholder order matches bind-array order")
}

[test(description = "take returns the first n rows")]
def test_take_runtime_returns_n_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>) |> take(2))
        t |> equal(length(rows), 2, "take(2) returns 2 rows")
        t |> equal(rows[0].Name, "BMW",  "first row")
        t |> equal(rows[1].Name, "Audi", "second row")
    }
}

[test(description = "skip drops the first n rows and returns the rest")]
def test_skip_runtime_skips_n_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>) |> skip(3))
        t |> equal(length(rows), 2, "skip(3) over 5 rows leaves 2")
        t |> equal(rows[0].Name, "Lada",  "row[3] = first after skip")
        t |> equal(rows[1].Name, "Lambo", "row[4]")
    }
}

[test(description = "take followed by skip drops rows from the already limited set")]
def test_take_then_skip_combined_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Multi-Q lowering: `take(2) |> skip(1)` = "take 2 rows, then drop
        // 1". n - m = 1 row remains (BMW taken, skipped, leaving Audi).
        let rows <- _sql(db |> select_from(type<Car>) |> take(2) |> skip(1))
        t |> equal(length(rows), 1, "take(2) |> skip(1) returns max(0, n-m) = 1 row")
        t |> equal(rows[0].Name, "Audi",
                   "the second row of the inner take, after dropping the first")
    }
}

[test(description = "skip followed by take returns a window starting after the skipped rows")]
def test_skip_then_take_combined_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Canonical pagination: `skip(1) |> take(2)` = "skip 1, take next 2".
        let rows <- _sql(db |> select_from(type<Car>) |> skip(1) |> take(2))
        t |> equal(length(rows), 2, "skip(1) |> take(2) returns 2 rows starting from row 1")
        t |> equal(rows[0].Name, "Audi",  "row[1]")
        t |> equal(rows[1].Name, "Tesla", "row[2]")
    }
}

[test(description = "an aggregate after take sees only the taken rows")]
def test_take_then_select_then_aggregate_runtime(t : T?) {
    // bad12 promoted to a positive case: multi-Q lowers `take(n) |>
    // _select(...) |> sum` to `SELECT SUM(col) FROM (SELECT col FROM
    // table LIMIT n)`. Sum of the first n rows' projected column.
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let total = _sql(db |> select_from(type<Car>) |> take(2) |> _select(_.Price) |> sum())
        t |> equal(total, 100 + 200, "SUM over first 2 rows' Price (BMW+Audi)")
    }
}

[test(description = "take after a where limits the filtered rows")]
def test_where_take_runtime_binds(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let threshold = 100
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Price > threshold)
                           |> take(2))
        t |> equal(length(rows), 2, "WHERE filters then LIMIT applies")
        // Cars >100 in insertion order: Audi(200), Tesla(300), Lambo(999) —
        // take 2.
        t |> equal(rows[0].Name, "Audi",  "first match")
        t |> equal(rows[1].Name, "Tesla", "second match")
    }
}

[test(description = "first after take emits a LIMIT clamped to at most one row")]
def test_first_after_take_clamps_limit(t : T?) {
    var _db = SqlRunner()
    // take(n) |> _first() — clamps to at most one row, keeping take(0)'s
    // empty result.
    let sql = _sql_text(_db |> select_from(type<Car>) |> take(5) |> _first())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT CASE WHEN ? < 1 THEN 0 ELSE 1 END",
        "single-row terminal clamps take's LIMIT, keeping the bind")
}

def private fixture_34(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 100))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// _order_by — single column ASC. Multi-column via tuple-key (_.k1,
// _.k2). _order_by_descending — single column DESC. Mixed ASC/DESC
// across columns is plan D2 (out of chunk-4 scope).
// ──────────────────────────────────────────────────────────────────────────

[test(description = "order_by emits ORDER BY ascending")]
def test_order_by_emits_asc(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by(_.Price))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" ASC",
        "_order_by emits ORDER BY col ASC")
}

[test(description = "order_by_descending emits ORDER BY descending")]
def test_order_by_descending_emits_desc(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by_descending(_.Price))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" DESC",
        "_order_by_descending emits ORDER BY col DESC")
}

[test(description = "order_by on a tuple key emits every column in order")]
def test_order_by_tuple_key_emits_multi_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by((_.Price, _.Name)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" ASC, \"Name\" ASC",
        "tuple-key _order_by emits each field as its own ORDER BY column")
}

[test(description = "order_by composes between a where and a take in clause order")]
def test_order_by_with_where_and_take(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > 50)
                          |> _order_by(_.Price)
                          |> take(3))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" > ? ORDER BY \"Price\" ASC LIMIT ?",
        "WHERE then ORDER BY then LIMIT — SQL clause order")
}

[test(description = "order_by returns the rows sorted ascending")]
def test_order_by_runtime_sorts_ascending(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_34(db)
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price))
        t |> equal(length(rows), 5, "5 rows")
        t |> equal(rows[0].Price, 50,  "first by Price ASC = Lada (50)")
        t |> equal(rows[4].Price, 999, "last by Price ASC = Lambo (999)")
    }
}

[test(description = "order_by_descending returns the rows sorted descending")]
def test_order_by_descending_runtime_sorts(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_34(db)
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.Price))
        t |> equal(rows[0].Price, 999, "first DESC = Lambo")
        t |> equal(rows[4].Price, 50,  "last DESC = Lada")
    }
}

[test(description = "a tuple order key breaks ties on the second column")]
def test_order_by_tuple_key_runtime_breaks_ties(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_34(db)
        // BMW(100) and Tesla(100) tie on Price; tuple key (Price, Name)
        // breaks the tie alphabetically by Name ASC.
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by((_.Price, _.Name)))
        t |> equal(rows[0].Name, "Lada",  "Price=50")
        t |> equal(rows[1].Name, "BMW",   "Price=100, Name=BMW first alphabetically")
        t |> equal(rows[2].Name, "Tesla", "Price=100, Name=Tesla second")
    }
}

[sql_table(name = "Cars")]
struct Car_92 {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

def private fixture_92(db : SqlRunner) {
    db |> create_table(type<Car_92>)
    db |> insert(Car_92(Id = 1, Name = "BMW", Price = 100))
    db |> insert(Car_92(Id = 2, Name = "Audi", Price = 200))
    db |> insert(Car_92(Id = 3, Name = "Tesla", Price = 100))
    db |> insert(Car_92(Id = 4, Name = "Lada", Price = 50))
    db |> insert(Car_92(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime _order_by(string) — column name supplied as a `string`
// expression. The compile-time path (_.Field) still emits a literal
// `"col" ASC`. The runtime path emits `sql_quote_id(<expr>)` so the
// column name is quoted at execution time, with embedded `"` properly
// doubled.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "order_by with a constant column name string emits the quoted column")]
def test_order_by_const_string_emits_quoted(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car_92>) |> _order_by("Price"))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" ASC",
        "_order_by(const string) folds to compile-time quoted ORDER BY")
}

[test(description = "order_by_descending with a constant column name string emits DESC on the quoted column")]
def test_order_by_descending_const_string(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car_92>) |> _order_by_descending("Price"))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" DESC",
        "_order_by_descending(const string) folds DESC")
}

[test(description = "order_by with a column name held in a variable emits that column quoted")]
def test_order_by_runtime_string_variable(t : T?) {
    var _db = SqlRunner()
    let col = "Name"
    let sql = _sql_text(_db |> select_from(type<Car_92>) |> _order_by(col))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Name\" ASC",
        "_order_by(<runtime var>) quotes the column name at runtime")
}

[test(description = "a runtime column name containing a double quote is escaped in the emitted ORDER BY")]
def test_order_by_runtime_string_with_embedded_quote(t : T?) {
    var _db = SqlRunner()
    // Identifier with embedded `"` must be doubled, per SQLite identifier
    // rules.
    let col = "We\"ird"
    let sql = _sql_text(_db |> select_from(type<Car_92>) |> _order_by(col))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"We\"\"ird\" ASC",
        "embedded quote in runtime column name is doubled")
}

[test(description = "order_by with a runtime column name sorts the rows ascending")]
def test_order_by_runtime_string_executes(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_92(db)
        let col = "Price"
        let rows <- _sql(db |> select_from(type<Car_92>) |> _order_by(col))
        t |> equal(length(rows), 5, "5 rows")
        t |> equal(rows[0].Price, 50, "Price ASC: Lada first")
        t |> equal(rows[4].Price, 999, "Price ASC: Lambo last")
    }
}

[test(description = "order_by_descending with a runtime column name sorts the rows descending")]
def test_order_by_descending_runtime_string_executes(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_92(db)
        let col = "Price"
        let rows <- _sql(db |> select_from(type<Car_92>) |> _order_by_descending(col))
        t |> equal(rows[0].Price, 999, "Price DESC: Lambo first")
        t |> equal(rows[4].Price, 50, "Price DESC: Lada last")
    }
}

[test(description = "an order key tuple of runtime column names sorts by both")]
def test_order_by_tuple_runtime_strings(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_92(db)
        let primary = "Price"
        let secondary = "Name"
        // BMW(100) and Tesla(100) tie on Price; Name ASC breaks the tie.
        let rows <- _sql(db |> select_from(type<Car_92>) |> _order_by((primary, secondary)))
        t |> equal(rows[0].Name, "Lada", "Price=50 first")
        t |> equal(rows[1].Name, "BMW", "Price=100, Name=BMW")
        t |> equal(rows[2].Name, "Tesla", "Price=100, Name=Tesla")
    }
}

[test(description = "an order key tuple can mix a field reference and a runtime column name")]
def test_order_by_tuple_mixed_field_and_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_92(db)
        let secondary = "Name"
        // Mixed: compile-time _.Price + runtime secondary.
        let rows <- _sql(db |> select_from(type<Car_92>) |> _order_by((_.Price, secondary)))
        t |> equal(rows[0].Name, "Lada", "Price=50 first")
        t |> equal(rows[1].Name, "BMW", "tie broken by Name ASC")
        t |> equal(rows[2].Name, "Tesla", "tie second")
    }
}

[test(description = "a runtime column order_by composes with where and take")]
def test_order_by_runtime_string_with_where_and_take(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_92(db)
        let col = "Price"
        let rows <- _sql(db |> select_from(type<Car_92>)
            |> _where(_.Price > 100)
            |> _order_by(col)
            |> take(2))
        t |> equal(length(rows), 2, "two rows past the WHERE filter")
        t |> equal(rows[0].Price, 200, "Audi (200) is smallest >100")
        t |> equal(rows[1].Price, 999, "Lambo (999) is next")
    }
}

[test(description = "a runtime column order_by streams in order through each_sql")]
def test_order_by_runtime_string_each_sql(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_92(db)
        let col = "Price"
        let prices <- [for (row in _each_sql(db |> select_from(type<Car_92>)
            |> _order_by_descending(col))); row.Price]
        t |> equal(length(prices), 5, "_each_sql streams 5 rows in order")
        t |> equal(prices[0], 999, "DESC first")
        t |> equal(prices[4], 50, "DESC last")
    }
}
