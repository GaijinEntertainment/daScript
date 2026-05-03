options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 12b — UNION / INTERSECT / EXCEPT.
//
// Both sides must project the same shape (`_select(_.Col)`). The
// `_sql` macro lowers `lhs |> union(rhs)` etc. to SQL set operations;
// `daslib/linq` already implements `union`/`intersect`/`except` over
// arrays. Set-op output order is unspecified — assertions compare
// sorted multisets.

[sql_table(name = "Customers")]
struct Customer {
    @sql_primary_key Id : int
    Tier : int
}

[sql_table(name = "Prospects")]
struct Prospect {
    @sql_primary_key Id : int
    Tier : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Customer>)
    db |> create_table(type<Prospect>)
    db |> insert(Customer(Id = 1, Tier = 10))
    db |> insert(Customer(Id = 2, Tier = 20))
    db |> insert(Customer(Id = 3, Tier = 30))
    db |> insert(Prospect(Id = 100, Tier = 20))
    db |> insert(Prospect(Id = 200, Tier = 30))
    db |> insert(Prospect(Id = 300, Tier = 40))
}

def fixture_customers() : array<Customer> {
    var arr : array<Customer>
    arr |> emplace(Customer(Id = 1, Tier = 10))
    arr |> emplace(Customer(Id = 2, Tier = 20))
    arr |> emplace(Customer(Id = 3, Tier = 30))
    return <- arr
}

def fixture_prospects() : array<Prospect> {
    var arr : array<Prospect>
    arr |> emplace(Prospect(Id = 100, Tier = 20))
    arr |> emplace(Prospect(Id = 200, Tier = 30))
    arr |> emplace(Prospect(Id = 300, Tier = 40))
    return <- arr
}

def int_arrays_eq(a : array<int>; b : array<int>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

[test]
def parity_union(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var customers <- fixture_customers()
        var prospects <- fixture_prospects()

        var m1 <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                       |> union(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m2 <- ((db |> select_from(type<Customer>) |> _select(_.Tier))
                   |> union(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m3 <- ((customers |> _select(_.Tier)) |> union(prospects |> _select(_.Tier)))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 4, "UNION distinct count: 4 (10, 20, 30, 40)")
        t |> success(int_arrays_eq(m1, m2), "union: M1 ≡ M2 (sorted multiset)")
        t |> success(int_arrays_eq(m2, m3), "union: M2 ≡ M3 (sorted multiset)")
    }
}

[test]
def parity_intersect(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var customers <- fixture_customers()
        var prospects <- fixture_prospects()

        var m1 <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                       |> intersect(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m2 <- ((db |> select_from(type<Customer>) |> _select(_.Tier))
                   |> intersect(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m3 <- ((customers |> _select(_.Tier)) |> intersect(prospects |> _select(_.Tier)))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 2, "INTERSECT count: 2 (20, 30)")
        t |> success(int_arrays_eq(m1, m2), "intersect: M1 ≡ M2 (sorted multiset)")
        t |> success(int_arrays_eq(m2, m3), "intersect: M2 ≡ M3 (sorted multiset)")
    }
}

[test]
def parity_except(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var customers <- fixture_customers()
        var prospects <- fixture_prospects()

        var m1 <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                       |> except(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m2 <- ((db |> select_from(type<Customer>) |> _select(_.Tier))
                   |> except(db |> select_from(type<Prospect>) |> _select(_.Tier)))
        var m3 <- ((customers |> _select(_.Tier)) |> except(prospects |> _select(_.Tier)))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 1, "EXCEPT count: 1 (just 10)")
        t |> success(int_arrays_eq(m1, m2), "except: M1 ≡ M2 (sorted multiset)")
        t |> success(int_arrays_eq(m2, m3), "except: M2 ≡ M3 (sorted multiset)")
    }
}
