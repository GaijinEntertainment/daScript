options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require daslib/sql_linq

// Multi-Q lowering parity check: outer-Q `_where` over inner-Q `skip`.
//
//   `... |> _order_by(_.Price) |> skip(2) |> _where(_.Price > 100)`
//
// Lowers under `_sql` to:
//   SELECT … FROM (SELECT … ORDER BY "Price" ASC LIMIT -1 OFFSET 2) AS "t0" WHERE "Price" > ?
//
// Linq positional semantics: order by price, skip first 2, then filter the rest.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> insert(Car(Id = 1, Name = "BMW",   Price = 100))
    db |> insert(Car(Id = 2, Name = "Audi",  Price = 200))
    db |> insert(Car(Id = 3, Name = "Tesla", Price = 300))
    db |> insert(Car(Id = 4, Name = "Lada",  Price = 50))
    db |> insert(Car(Id = 5, Name = "Lambo", Price = 999))
}

def fixture_array() : array<Car> {
    return <- [
        Car(Id = 1, Name = "BMW",   Price = 100),
        Car(Id = 2, Name = "Audi",  Price = 200),
        Car(Id = 3, Name = "Tesla", Price = 300),
        Car(Id = 4, Name = "Lada",  Price = 50),
        Car(Id = 5, Name = "Lambo", Price = 999)
    ]
}

def cars_equal(a : array<Car>; b : array<Car>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Price != eb.Price) {
            return false
        }
    }
    return true
}

[test]
def parity_skip_then_where(t : T?) {
    // Order by price (Lada=50, BMW=100, Audi=200, Tesla=300, Lambo=999), skip 2
    // (drops Lada, BMW), then filter Price > 100 (keeps all 3 remaining since
    // Audi=200, Tesla=300, Lambo=999 all > 100).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(2) |> _where(_.Price > 100))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(2) |> _where(_.Price > 100))
        let m3 <- (arr |> _order_by(_.Price) |> skip(2) |> _where(_.Price > 100))

        t |> success(cars_equal(m1, m2), "order|skip|where: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "order|skip|where: M2 ≡ M3")
        t |> equal(length(m1), 3, "3 rows after skip+filter")
    }
}

[test]
def parity_skip_then_where_then_order_desc(t : T?) {
    // Inner: order ASC + skip(1) → drops Lada. Outer: where(Price>50) keeps the rest,
    // then order_descending(Price). All in outer Q after the skip's divert.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(1)
                         |> _where(_.Price > 50)
                         |> _order_by_descending(_.Price))
        let m2 <- (db |> select_from(type<Car>) |> _order_by(_.Price) |> skip(1)
                     |> _where(_.Price > 50)
                     |> _order_by_descending(_.Price))
        let m3 <- (arr |> _order_by(_.Price) |> skip(1)
                       |> _where(_.Price > 50)
                       |> _order_by_descending(_.Price))

        t |> success(cars_equal(m1, m2), "order|skip|where|order_desc: M1 ≡ M2")
        t |> success(cars_equal(m2, m3), "order|skip|where|order_desc: M2 ≡ M3")
        t |> equal(m1[0].Name, "Lambo", "highest price first")
    }
}
