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

[test]
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

[test]
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

[test]
def test_count_with_where_emits_count_with_where_sql(t : T?) {
    var _db = SqlRunner()
    let cutoff = 150
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Price > cutoff)
                          |> count())
    t |> equal(sql,
        "SELECT COUNT(*) FROM \"Cars\" WHERE \"Price\" > ?",
        "count + where emits COUNT(*) FROM ... WHERE ...")
}
