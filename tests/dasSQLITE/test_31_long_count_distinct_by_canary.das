options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Brand : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Brand = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Brand = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Brand = "BMW",   Price = 300))
    db |> insert(Car(Id = 4, Brand = "Tesla", Price = 50))
    db |> insert(Car(Id = 5, Brand = "BMW",   Price = 999))
}

[test]
def test_long_count_returns_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> long_count())
        t |> equal(n, 5l, "5 rows total (int64)")
    }
}

[test]
def test_long_count_emits_count_star(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> long_count())
    t |> equal(sql, "SELECT COUNT(*) FROM \"Cars\"", "long_count() emits COUNT(*)")
}

[test]
def test_long_count_with_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> _where(_.Price > 99) |> long_count())
        t |> equal(n, 4l, "4 rows where Price > 99")
    }
}

[test]
def test_distinct_by_count_emits_count_distinct(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _distinct_by(_.Brand) |> count())
    t |> equal(sql, "SELECT COUNT(DISTINCT \"Brand\") FROM \"Cars\"",
        "_distinct_by(_.Brand) |> count() emits COUNT(DISTINCT \"Brand\")")
}

[test]
def test_distinct_by_count_returns_distinct_brand_total(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> _distinct_by(_.Brand) |> count())
        t |> equal(n, 3, "3 distinct brands")
    }
}

[test]
def test_distinct_by_long_count_returns_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = _sql(db |> select_from(type<Car>) |> _distinct_by(_.Brand) |> long_count())
        t |> equal(n, 3l, "3 distinct brands (int64)")
    }
}

[test]
def test_distinct_by_long_count_emits_count_distinct(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _distinct_by(_.Brand) |> long_count())
    t |> equal(sql, "SELECT COUNT(DISTINCT \"Brand\") FROM \"Cars\"",
        "long_count after _distinct_by emits the same COUNT(DISTINCT col) as count")
}

[test]
def test_group_by_long_count_widens_to_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let groups <- _sql(db |> select_from(type<Car>)
                              |> _group_by(_.Brand)
                              |> _select((Brand = _._0, N = _._1 |> long_count())))
        t |> equal(length(groups), 3, "3 distinct brands grouped")
        var total = 0l
        for (g in groups) {
            total += g.N
        }
        t |> equal(total, 5l, "group totals sum to row count (int64 accumulator)")
    }
}

