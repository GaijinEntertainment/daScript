options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require daslib/linq_fold
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// `_fold` SQL pass-through (daslib/linq_fold + sqlite/linq_fold_sql): a `[sql_table]`
// `select_from` source makes `_fold(chain)` re-dispatch the whole chain to `_sql(chain)`.
// These tests assert the pass-through compiles, expands, runs, and is result-identical to
// writing `_sql(...)` directly across the representative chain shapes. Distinct projection
// tuple shapes per test avoid the generic-instantiation recordNames collision.

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name      : string
    price     : int
    brand     : int
    dealer_id : int
}

[sql_table(name = "Dealers")]
struct Dealer {
    @sql_primary_key id : int
    name : string
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> create_table(type<Dealer>)
    db |> insert(Dealer(id = 1, name = "North"))
    db |> insert(Dealer(id = 2, name = "South"))
    db |> insert(Car(id = 1, name = "BMW",   price = 100, brand = 0, dealer_id = 1))
    db |> insert(Car(id = 2, name = "Audi",  price = 200, brand = 1, dealer_id = 2))
    db |> insert(Car(id = 3, name = "Tesla", price = 300, brand = 0, dealer_id = 1))
    db |> insert(Car(id = 4, name = "Lada",  price =  50, brand = 1, dealer_id = 2))
    db |> insert(Car(id = 5, name = "Lambo", price = 999, brand = 2, dealer_id = 1))
}

def cars_equal(a, b : array<Car>) : bool {
    if (length(a) != length(b)) return false
    for (i in range(length(a))) {
        if (a[i].id != b[i].id || a[i].name != b[i].name || a[i].price != b[i].price) return false
    }
    return true
}

[test]
def fold_select_all(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let m_fold <- _fold(db |> select_from(type<Car>))
        let m_sql  <- _sql(db |> select_from(type<Car>))
        t |> equal(length(m_fold), 5, "select_all: 5 rows")
        t |> success(cars_equal(m_fold, m_sql), "select_all: _fold ≡ _sql")
    }
}

[test]
def fold_where_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let c_fold = _fold(db |> select_from(type<Car>) |> _where(_.price > 150) |> count())
        let c_sql  = _sql(db |> select_from(type<Car>) |> _where(_.price > 150) |> count())
        t |> equal(c_fold, 3, "where(price>150) | count(): 3 (Audi, Tesla, Lambo)")
        t |> equal(c_fold, c_sql, "where|count: _fold ≡ _sql")
    }
}

[test]
def fold_group_by(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        // projection tuple<int;int> — distinct from the join test's shape
        var g_fold <- _fold(db |> select_from(type<Car>)
                              |> _group_by(_.brand)
                              |> _select((Brand = _._0, N = _._1 |> length)))
        var g_sql  <- _sql(db |> select_from(type<Car>)
                             |> _group_by(_.brand)
                             |> _select((Brand = _._0, N = _._1 |> length)))
        g_fold |> sort() <| $(a, b) => a.Brand < b.Brand
        g_sql  |> sort() <| $(a, b) => a.Brand < b.Brand
        t |> equal(length(g_fold), length(g_sql), "group_by: _fold ≡ _sql group count")
        for (i in range(length(g_fold))) {
            t |> equal(g_fold[i].Brand, g_sql[i].Brand, "group[{i}].Brand: _fold ≡ _sql")
            t |> equal(g_fold[i].N,     g_sql[i].N,     "group[{i}].N: _fold ≡ _sql")
        }
    }
}

[test]
def fold_join_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        // projection tuple<string;string> — distinct from the group_by test's shape
        let n_fold = _fold(db |> select_from(type<Car>)
                             |> _join(db |> select_from(type<Dealer>),
                                      $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                      $(c : Car, d : Dealer) => (CarName = c.name, DealerName = d.name))
                             |> count())
        let n_sql  = _sql(db |> select_from(type<Car>)
                            |> _join(db |> select_from(type<Dealer>),
                                     $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                     $(c : Car, d : Dealer) => (CarName = c.name, DealerName = d.name))
                            |> count())
        t |> equal(n_fold, 5, "join | count(): 5 (every car has a dealer)")
        t |> equal(n_fold, n_sql, "join|count: _fold ≡ _sql")
    }
}
