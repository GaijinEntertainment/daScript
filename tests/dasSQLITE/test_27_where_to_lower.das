options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "Ford"))
    db |> insert(Car(Id = 2, Name = "Honda"))
    db |> insert(Car(Id = 3, Name = "Audi"))
}

[test]
def test_where_to_lower_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let target = "ford"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> to_lower() == target))
        t |> equal(length(cars), 1, "case-insensitive match")
        t |> equal(cars[0].Name, "Ford", "matched")
    }
}

[test]
def test_where_to_lower_emits_lower_call(t : T?) {
    var _db = SqlRunner()
    let target = "x"
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Name |> to_lower() == target))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE LOWER(\"Name\") = ?",
        "to_lower -> LOWER(...)")
}

[test]
def test_where_to_upper_emits_upper_call(t : T?) {
    var _db = SqlRunner()
    let target = "X"
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Name |> to_upper() == target))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE UPPER(\"Name\") = ?",
        "to_upper -> UPPER(...)")
}
