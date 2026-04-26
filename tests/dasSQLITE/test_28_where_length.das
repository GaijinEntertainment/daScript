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
    db |> insert(Car(Id = 1, Name = "Audi"))
    db |> insert(Car(Id = 2, Name = "Ferrari"))
    db |> insert(Car(Id = 3, Name = "Lamborghini"))
}

[test]
def test_where_length_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let cutoff = 5
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(length(_.Name) > cutoff))
        t |> equal(length(cars), 2, "Ferrari + Lamborghini have length > 5")
    }
}

[test]
def test_where_length_emits_length_call(t : T?) {
    var _db = SqlRunner()
    let cutoff = 5
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(length(_.Name) > cutoff))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE LENGTH(\"Name\") > ?",
        "length -> LENGTH(...)")
}
