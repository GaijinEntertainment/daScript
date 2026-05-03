options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 09 — `_select` projections.
//
// Default (full row) and single-column `_select(_.Field)` were covered in
// tut 04. New here:
//   - named-tuple projection `_select((Name = _.Name, Price = _.Price))`
//   - renaming via named tuple
//   - named-tuple + terminal: `_select((...)) |> _first()`
//
// `_sql_text` is out of scope per API_CHECKED.md.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
}

def fixture_array() : array<Car> {
    var arr : array<Car>
    arr |> emplace(Car(Id = 1, Name = "BMW",   Price = 100))
    arr |> emplace(Car(Id = 2, Name = "Audi",  Price = 200))
    arr |> emplace(Car(Id = 3, Name = "Tesla", Price = 300))
    return <- arr
}

[test]
def parity_select_named_tuple(t : T?) {
    // Named-tuple projection: result element is tuple<Name:string;Price:int>
    // in all three modes; recordNames preserved so .Name / .Price access works.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _select((Name = _.Name, Price = _.Price)))
        let m2 <- (db |> select_from(type<Car>) |> _select((Name = _.Name, Price = _.Price)))
        let m3 <- (arr |> _select((Name = _.Name, Price = _.Price)))

        t |> equal(length(m1), length(m2), "named-tuple length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "named-tuple length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Name,  m2[i].Name,  "row[{i}].Name: M1 ≡ M2")
            t |> equal(m1[i].Price, m2[i].Price, "row[{i}].Price: M1 ≡ M2")
            t |> equal(m2[i].Name,  m3[i].Name,  "row[{i}].Name: M2 ≡ M3")
            t |> equal(m2[i].Price, m3[i].Price, "row[{i}].Price: M2 ≡ M3")
        }
    }
}

[test]
def parity_select_renamed(t : T?) {
    // Renaming: result fields differ from source columns. The tuple shape
    // (Identifier:int; Label:string) must agree across all three modes.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _select((Identifier = _.Id, Label = _.Name)))
        let m2 <- (db |> select_from(type<Car>) |> _select((Identifier = _.Id, Label = _.Name)))
        let m3 <- (arr |> _select((Identifier = _.Id, Label = _.Name)))

        t |> equal(length(m1), length(m2), "renamed length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "renamed length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Identifier, m2[i].Identifier, "row[{i}].Identifier: M1 ≡ M2")
            t |> equal(m1[i].Label,      m2[i].Label,      "row[{i}].Label: M1 ≡ M2")
            t |> equal(m2[i].Identifier, m3[i].Identifier, "row[{i}].Identifier: M2 ≡ M3")
            t |> equal(m2[i].Label,      m3[i].Label,      "row[{i}].Label: M2 ≡ M3")
        }
    }
}

[test]
def parity_select_named_tuple_first(t : T?) {
    // Named-tuple projection + `_first()` terminal: result is the tuple itself.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        let m1 = _sql(db |> select_from(type<Car>)
                        |> _select((Name = _.Name, Price = _.Price))
                        |> _first())
        let m2 = (db |> select_from(type<Car>)
                    |> _select((Name = _.Name, Price = _.Price))
                    |> _first())
        let m3 = arr |> _select((Name = _.Name, Price = _.Price)) |> _first()

        t |> equal(m1.Name,  m2.Name,  "head.Name: M1 ≡ M2")
        t |> equal(m1.Price, m2.Price, "head.Price: M1 ≡ M2")
        t |> equal(m2.Name,  m3.Name,  "head.Name: M2 ≡ M3")
        t |> equal(m2.Price, m3.Price, "head.Price: M2 ≡ M3")
    }
}
