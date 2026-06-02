options gen2

require dastest/testing_boost public
require daslib/linq_das
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// linq_das over a SQL source: `from c : Car in db` → `from_in(db, type<Car>)` → `select_from`, which
// `_fold` routes to `_sql` (WHERE/SELECT pushdown). The C# range variable is spliced verbatim; `_sql`
// normalizes the single-source lambda param to `_` internally, and absorbs the trailing `to_array()`.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name  : string
    price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(id = 1, name = "BMW",   price = 100))
    db |> insert(Car(id = 2, name = "Audi",  price = 200))
    db |> insert(Car(id = 3, name = "Tesla", price = 300))
}

[test]
def test_sql_where_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- %linq! from c : Car in db where c.price > 150 select c.name %%
        // parity with the hand-written placeholder chain through _sql
        let ref <- _fold(db |> select_from(type<Car>) |> _where(_.price > 150) |> _select(_.name))
        t |> equal(length(names), 2, "sql where/select (Audi, Tesla)")
        t |> equal(length(names), length(ref), "linq_das ≡ hand-written _sql chain")
        t |> equal(names[0], "Audi")
        t |> equal(names[1], "Tesla")
    }
}

[test]
def test_sql_identity_select(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- %linq! from c : Car in db where c.price >= 200 select c %%
        t |> equal(length(rows), 2, "sql identity select returns rows")
        t |> equal(rows[0].name, "Audi")
    }
}

[test]
def test_sql_iterator_output(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let names <- [for (nm in %linq! from c : Car in db where c.price > 150 select c.name iterator %%); nm]
        t |> equal(length(names), 2, "sql iterator output")
    }
}

[test]
def test_sql_orderby_ascending(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // ORDER BY price ASC over the SQL source
        let names <- %linq! from c : Car in db orderby c.price select c.name %%
        let ref <- _fold(db |> select_from(type<Car>) |> _order_by(_.price) |> _select(_.name))
        t |> equal(length(names), 3)
        t |> equal(names[0], "BMW")     // 100
        t |> equal(names[2], "Tesla")   // 300
        t |> equal(length(names), length(ref), "linq_das orderby ≡ hand-written _sql chain")
    }
}

[test]
def test_sql_orderby_descending_with_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // WHERE price >= 200 then ORDER BY price DESC → Tesla(300), Audi(200)
        let names <- %linq! from c : Car in db where c.price >= 200 orderby c.price descending select c.name %%
        t |> equal(length(names), 2, "sql where + order by desc")
        t |> equal(names[0], "Tesla")
        t |> equal(names[1], "Audi")
    }
}
