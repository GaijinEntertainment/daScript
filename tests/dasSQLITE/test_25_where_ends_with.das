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
    db |> insert(Car(Id = 4, Name = "Lada"))
}

[test]
def test_where_ends_with_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let suffix = "da"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> ends_with(suffix)))
        t |> equal(length(cars), 2, "2 cars end in 'da' (Honda, Lada)")
        t |> equal(cars[0].Name, "Honda", "first match")
        t |> equal(cars[1].Name, "Lada",  "second match")
    }
}

[test]
def test_where_ends_with_emits_like_suffix(t : T?) {
    var _db = SqlRunner()
    let _suffix = "X"
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Name |> ends_with(_suffix)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE \"Name\" LIKE ? ESCAPE '\\'",
        "ends_with -> LIKE ? ESCAPE '\\'")
}

[test]
def test_where_ends_with_escapes_literal_underscore(t : T?) {
    // Literal '_' must not act as the LIKE wildcard.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "model_X"))
        db |> insert(Car(Id = 2, Name = "modelAX"))
        let suffix = "_X"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> ends_with(suffix)))
        t |> equal(length(cars), 1, "literal '_' in user suffix matches only 'model_X'")
        t |> equal(cars[0].Id, 1, "matched the literal-underscore row")
    }
}
