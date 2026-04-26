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
}

// ──────────────────────────────────────────────────────────────────────────
// _select with named-tuple projection: (Name=_.Name, Price=_.Price)
// Returns array<tuple<Name:string;Price:int>> with recordNames preserved.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_select_named_tuple_two_fields(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _select((Name = _.Name, Price = _.Price)))
        t |> equal(length(rows), 3, "3 rows")
        t |> equal(rows[0].Name,  "BMW",  "row 0 Name (by tuple field)")
        t |> equal(rows[0].Price, 100,    "row 0 Price (by tuple field)")
        t |> equal(rows[2].Name,  "Tesla", "row 2 Name")
        t |> equal(rows[2].Price, 300,    "row 2 Price")
    }
}

[test]
def test_select_named_tuple_with_first(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let row = _sql(db |> select_from(type<Car>)
                         |> _select((Name = _.Name, Price = _.Price))
                         |> _first())
        t |> equal(row.Name,  "BMW", "first Name (named-tuple + _first)")
        t |> equal(row.Price, 100,   "first Price")
    }
}

[test]
def test_select_named_tuple_emits_select_two_cols(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _select((Name = _.Name, Price = _.Price)))
    t |> equal(sql,
        "SELECT \"Name\", \"Price\" FROM \"Cars\"",
        "named-tuple projection emits SELECT col1, col2")
}

[test]
def test_select_named_tuple_three_fields(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _select((Identifier = _.Id, Label = _.Name, Cost = _.Price)))
        t |> equal(length(rows), 3, "3 rows")
        t |> equal(rows[1].Identifier, 2,      "renamed Id field")
        t |> equal(rows[1].Label,      "Audi", "renamed Name field")
        t |> equal(rows[1].Cost,       200,    "renamed Price field")
    }
}
