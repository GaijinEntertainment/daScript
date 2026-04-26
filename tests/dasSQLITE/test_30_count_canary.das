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
    db |> insert(Car(Id = 1, Name = "BMW",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",    Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla",   Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",    Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo",   Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// _count canary — proves the aggregate framework path:
//   - ProjectionShape.Aggregate sets the SELECT-list to "COUNT(*)"
//   - Materializer.One reads back a single int64
//
// `_count` is reserved as a 2-arg call_macro in linq_boost (predicate form,
// chunk 4 territory). For the bare aggregate we use linq.das's `count(src)`
// directly (no underscore).
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_count_returns_int64_total(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> count())
        t |> equal(int(n), 5, "5 rows total")
    }
}

[test]
def test_count_on_empty_table_is_zero(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let n = _sql(db |> select_from(type<Car>) |> count())
        t |> equal(int(n), 0, "empty table counts 0")
    }
}

[test]
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

[test]
def test_count_after_select_returns_same_total(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> _select(_.Name) |> count())
        t |> equal(int(n), 5, "_select(_.Name) |> count() returns row total, not column count")
    }
}

[test]
def test_count_after_select_emits_count_star(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Name) |> count())
    t |> equal(sql, "SELECT COUNT(*) FROM \"Cars\"",
        "_select before count() is ignored; aggregate emits COUNT(*) regardless")
}
