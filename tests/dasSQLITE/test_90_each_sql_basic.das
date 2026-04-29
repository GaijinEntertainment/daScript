options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 800))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

[test]
def test_each_sql_iterate_full_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var seen : array<string>
        for (c in _each_sql(db |> select_from(type<Car>))) {
            seen |> push(c.Name)
        }
        t |> equal(length(seen), 5, "5 rows yielded")
        t |> equal(seen[0], "BMW",   "row 0")
        t |> equal(seen[4], "Lambo", "row 4")
    }
}

[test]
def test_each_sql_break_finalizes_stmt(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Break after 2 rows. If the stmt isn't finalized, a follow-up
        // INSERT on the same DB would block (SQLite locks held by the
        // open read cursor). The fact that the INSERT below succeeds
        // proves the generator's finally ran.
        var count = 0
        for (_ in _each_sql(db |> select_from(type<Car>))) {
            count++
            if (count >= 2) {
                break
            }
        }
        t |> equal(count, 2, "broke after 2 rows")
        // Mutating insert must succeed — would error/block if stmt leaked.
        db |> insert(Car(Id = 99, Name = "Test", Price = 1))
        let n = db |> query_scalar("SELECT COUNT(*) FROM Cars", type<int>)
        t |> equal(n, 6, "insert succeeded after early break")
    }
}

[test]
def test_each_sql_with_where_filter(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let threshold = 200
        var matched = 0
        for (c in _each_sql(db |> select_from(type<Car>) |> _where(_.Price >= threshold))) {
            matched++
            t |> equal(c.Price >= 200, true, "row matches predicate")
        }
        t |> equal(matched, 3, "Audi + Tesla + Lambo (>= 200)")
    }
}

[test]
def test_each_sql_single_column_projection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var names : array<string>
        for (n in _each_sql(db |> select_from(type<Car>) |> _select(_.Name))) {
            names |> push(n)
        }
        t |> equal(length(names), 5, "5 names")
        t |> equal(names[2], "Tesla", "row 2")
    }
}

[test]
def test_each_sql_named_tuple_projection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        var pairs : array<int>
        for (p in _each_sql(db |> select_from(type<Car>) |> _select((Id = _.Id, Price = _.Price)))) {
            pairs |> push(p.Id * 1000 + p.Price)
        }
        t |> equal(length(pairs), 5, "5 named-tuple rows")
        t |> equal(pairs[0], 1100, "row 0: 1*1000+100")
        t |> equal(pairs[4], 5999, "row 4: 5*1000+999")
    }
}
