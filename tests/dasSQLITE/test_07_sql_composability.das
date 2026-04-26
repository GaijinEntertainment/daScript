options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

require test_07_macros

// ──────────────────────────────────────────────────────────────────────────
// Composability tests for `_sql`.
//
// Mode 2's promise: user-defined call_macros that expand to `_where` /
// `_select` compose transparently inside `_sql(...)`. The macro pass
// expands inner call_macros bottom-up before `_sql.visit` fires, so by the
// time the analyzer walks the chain, every wrapper has already cascaded
// into the canonical `where_(...,$(_:T) => ...)` / `select(...)` shape.
//
// `test_07_macros.das` defines two wrappers:
//   * when_price_lt(it, val)         → _where(it, _.Price < val)
//   * when_lt(it, fieldExpr, val)    → _where(it, fieldExpr < val)
// ──────────────────────────────────────────────────────────────────────────

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
// Baked-field wrapper — when_price_lt
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_when_price_lt_sql_text(t : T?) {
    var _db = SqlRunner()
    let with_wrapper = _sql_text(_db |> select_from(type<Car>) |> when_price_lt(200))
    let direct       = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price < 200))
    t |> equal(with_wrapper, direct, "when_price_lt and direct _where emit identical SQL")
    t |> equal(with_wrapper, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" < ?",
        "wrapper round-trips through _where → where_ → analyzer with the expected SQL")
}

[test]
def test_when_price_lt_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(200))
        t |> equal(length(cars), 2, "two rows priced < 200")
        t |> equal(cars[0].Name, "BMW",  "row 0")
        t |> equal(cars[1].Name, "Lada", "row 1")
    }
}

[test]
def test_when_price_lt_with_captured_value(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let threshold = 300
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(threshold))
        t |> equal(length(cars), 3, "three rows priced < 300 (BMW, Lada, Audi)")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Generic wrapper — when_lt(it, _.Field, val)
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_when_lt_sql_text(t : T?) {
    var _db = SqlRunner()
    let with_wrapper = _sql_text(_db |> select_from(type<Car>) |> when_lt(_.Price, 200))
    let direct       = _sql_text(_db |> select_from(type<Car>) |> _where(_.Price < 200))
    t |> equal(with_wrapper, direct, "when_lt and direct _where emit identical SQL")
}

[test]
def test_when_lt_different_field(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> when_lt(_.Id, 3))
    t |> equal(sql, "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Id\" < ?",
        "field-ref argument routes through to the right column")
}

[test]
def test_when_lt_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> when_lt(_.Id, 3))
        t |> equal(length(cars), 2, "two rows with Id < 3 (BMW=1, Audi=2)")
        t |> equal(cars[0].Name, "BMW",  "row 0")
        t |> equal(cars[1].Name, "Audi", "row 1")
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Composition: wrapper + canonical _where / _select in the same chain.
// Verifies the analyzer treats wrapper-emitted `where_` calls identically
// to user-written ones.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_wrapper_then_where_compose_as_and(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(500) |> _where(_.Id != 2))
        t |> equal(length(cars), 3, "Price<500 AND Id!=2 → BMW, Tesla, Lada")
    }
}

[test]
def test_wrapper_then_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- _sql(db |> select_from(type<Car>) |> when_price_lt(300) |> _select(_.Name))
        t |> equal(length(names), 3, "three names priced < 300 (BMW, Lada, Audi)")
    }
}

[test]
def test_wrapper_then_select_sql_text(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> when_price_lt(300) |> _select(_.Name))
    t |> equal(sql, "SELECT \"Name\" FROM \"Cars\" WHERE \"Price\" < ?",
        "wrapper-emitted WHERE composes with explicit _select projection")
}

[test]
def test_two_wrappers_compose_as_and(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Two wrapper calls — proves the analyzer's where AND-composition
        // treats wrapper output identically to source-level _where output.
        let cars <- _sql(db |> select_from(type<Car>) |> when_price_lt(500) |> when_lt(_.Id, 4))
        t |> equal(length(cars), 3, "Price<500 AND Id<4 → BMW(1), Audi(2), Tesla(3)")
    }
}
