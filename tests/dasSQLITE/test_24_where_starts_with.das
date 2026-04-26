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
    db |> insert(Car(Id = 1, Name = "Ford",     Price = 100))
    db |> insert(Car(Id = 2, Name = "Ferrari",  Price = 9999))
    db |> insert(Car(Id = 3, Name = "Audi",     Price = 200))
    db |> insert(Car(Id = 4, Name = "Lada",     Price = 50))
}

[test]
def test_where_starts_with_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let prefix = "F"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> starts_with(prefix)))
        t |> equal(length(cars), 2, "2 cars start with 'F'")
        t |> equal(cars[0].Name, "Ford",    "first match")
        t |> equal(cars[1].Name, "Ferrari", "second match")
    }
}

[test]
def test_where_starts_with_emits_like_pattern(t : T?) {
    var _db = SqlRunner()
    let _prefix = "F"
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Name |> starts_with(_prefix)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Price\" FROM \"Cars\" WHERE \"Name\" LIKE ? || '%'",
        "starts_with -> LIKE ? || '%'")
}
