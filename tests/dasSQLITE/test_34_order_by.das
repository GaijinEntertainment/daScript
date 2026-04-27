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
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 100))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// _order_by — single column ASC. Multi-column via tuple-key (_.k1, _.k2).
// _order_by_descending — single column DESC.
// Mixed ASC/DESC across columns is plan D2 (out of chunk-4 scope).
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_order_by_emits_asc(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by(_.Price))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" ASC",
        "_order_by emits ORDER BY col ASC")
}

[test]
def test_order_by_descending_emits_desc(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by_descending(_.Price))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" DESC",
        "_order_by_descending emits ORDER BY col DESC")
}

[test]
def test_order_by_tuple_key_emits_multi_col(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by((_.Price, _.Name)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" ORDER BY \"Price\" ASC, \"Name\" ASC",
        "tuple-key _order_by emits each field as its own ORDER BY column")
}

[test]
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

[test]
def test_order_by_runtime_sorts_ascending(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price))
        t |> equal(length(rows), 5, "5 rows")
        t |> equal(rows[0].Price, 50,  "first by Price ASC = Lada (50)")
        t |> equal(rows[4].Price, 999, "last by Price ASC = Lambo (999)")
    }
}

[test]
def test_order_by_descending_runtime_sorts(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by_descending(_.Price))
        t |> equal(rows[0].Price, 999, "first DESC = Lambo")
        t |> equal(rows[4].Price, 50,  "last DESC = Lada")
    }
}

[test]
def test_order_by_tuple_key_runtime_breaks_ties(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // BMW(100) and Tesla(100) tie on Price; tuple key (Price, Name)
        // breaks the tie alphabetically by Name ASC.
        let rows <- _sql(db |> select_from(type<Car>) |> _order_by((_.Price, _.Name)))
        t |> equal(rows[0].Name, "Lada",  "Price=50")
        t |> equal(rows[1].Name, "BMW",   "Price=100, Name=BMW first alphabetically")
        t |> equal(rows[2].Name, "Tesla", "Price=100, Name=Tesla second")
    }
}
