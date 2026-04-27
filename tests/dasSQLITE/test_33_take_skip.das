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
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

// ──────────────────────────────────────────────────────────────────────────
// take(n) / skip(n) — emit LIMIT / OFFSET. Bind ordering: LIMIT/OFFSET
// binds come AFTER WHERE binds in the placeholder index sequence.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_take_emits_limit(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> take(2))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT ?",
        "take(n) emits LIMIT ?")
}

[test]
def test_skip_emits_limit_neg1_offset(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> skip(2))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT -1 OFFSET ?",
        "skip(n) without take needs LIMIT -1 to satisfy SQLite's OFFSET-requires-LIMIT rule")
}

[test]
def test_take_then_skip_emits_limit_and_offset(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> take(3) |> skip(1))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT ? OFFSET ?",
        "take + skip combine into LIMIT … OFFSET …")
}

[test]
def test_where_then_take_orders_binds_correctly(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > 100)
                          |> take(2))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Price\" > ? LIMIT ?",
        "WHERE bind goes first, LIMIT bind goes last — placeholder order matches bind-array order")
}

[test]
def test_take_runtime_returns_n_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>) |> take(2))
        t |> equal(length(rows), 2, "take(2) returns 2 rows")
        t |> equal(rows[0].Name, "BMW",  "first row")
        t |> equal(rows[1].Name, "Audi", "second row")
    }
}

[test]
def test_skip_runtime_skips_n_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>) |> skip(3))
        t |> equal(length(rows), 2, "skip(3) over 5 rows leaves 2")
        t |> equal(rows[0].Name, "Lada",  "row[3] = first after skip")
        t |> equal(rows[1].Name, "Lambo", "row[4]")
    }
}

[test]
def test_take_and_skip_combined_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>) |> take(2) |> skip(1))
        t |> equal(length(rows), 2, "take(2) skip(1) returns 2 rows starting from row 1")
        t |> equal(rows[0].Name, "Audi",  "row[1]")
        t |> equal(rows[1].Name, "Tesla", "row[2]")
    }
}

[test]
def test_where_take_runtime_binds(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let threshold = 100
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Price > threshold)
                           |> take(2))
        t |> equal(length(rows), 2, "WHERE filters then LIMIT applies")
        // Cars >100 in insertion order: Audi(200), Tesla(300), Lambo(999) — take 2.
        t |> equal(rows[0].Name, "Audi",  "first match")
        t |> equal(rows[1].Name, "Tesla", "second match")
    }
}

[test]
def test_first_after_take_uses_limit_1(t : T?) {
    var _db = SqlRunner()
    // take(5) |> _first() — terminal forces LIMIT 1 (overrides take's 5).
    let sql = _sql_text(_db |> select_from(type<Car>) |> take(5) |> _first())
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" LIMIT 1",
        "single-row terminal overrides take's LIMIT")
}
