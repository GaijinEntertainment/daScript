options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/strings_boost

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "Ford"))
    db |> insert(Car(Id = 2, Name = "Honda"))
    db |> insert(Car(Id = 3, Name = "Toyota"))
    db |> insert(Car(Id = 4, Name = "Lada"))
}

[test]
def test_where_contains_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let needle = "o"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> contains(needle)))
        t |> equal(length(cars), 3, "3 cars contain 'o' (Ford, Honda, Toyota)")
    }
}

[test]
def test_where_contains_emits_like_wildcards(t : T?) {
    var _db = SqlRunner()
    let _needle = "x"
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where(_.Name |> contains(_needle)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\" FROM \"Cars\" WHERE \"Name\" LIKE ? ESCAPE '\\'",
        "contains -> LIKE ? ESCAPE '\\'")
}

[test]
def test_where_contains_escapes_literal_backslash(t : T?) {
    // Literal '\' in the needle must not collide with the ESCAPE char.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "C:\\Windows"))
        db |> insert(Car(Id = 2, Name = "Toyota"))
        let needle = "\\Windows"
        let cars <- _sql(db |> select_from(type<Car>)
                           |> _where(_.Name |> contains(needle)))
        t |> equal(length(cars), 1, "backslash needle matches the path row")
        t |> equal(cars[0].Id, 1, "matched the literal-backslash row")
    }
}
