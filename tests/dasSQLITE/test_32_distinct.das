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
