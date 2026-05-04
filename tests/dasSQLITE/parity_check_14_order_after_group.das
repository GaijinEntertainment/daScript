// Coverage for `_having |> _order_by` after `_group_by` across all
// three modes — was F3 in API_CHECKED.md (resolved by PR #2563,
// 2026-05-03 synthesized tuple/variant clone-constness fix).

options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name   : string
    City   : string
    Age    : int
    Salary : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100))
    db |> insert(User(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80))
    db |> insert(User(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120))
    db |> insert(User(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90))
    db |> insert(User(Id = 5, Name = "Eve",   City = "Boston",  Age = 50, Salary = 200))
}

def fixture_array() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100))
    arr |> emplace(User(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80))
    arr |> emplace(User(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120))
    arr |> emplace(User(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90))
    arr |> emplace(User(Id = 5, Name = "Eve",   City = "Boston",  Age = 50, Salary = 200))
    return <- arr
}

[test]
def parity_having_then_order_by(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length > 1)
                         |> _order_by(_._0)
                         |> _select((City = _._0, N = _._1 |> length)))
        let m2 <- (db |> select_from(type<User>)
                     |> _group_by(_.City)
                     |> _having(_._1 |> length > 1)
                     |> _order_by(_._0)
                     |> _select((City = _._0, N = _._1 |> length)))
        let m3 <- (arr |> _group_by(_.City)
                       |> _having(_._1 |> length > 1)
                       |> _order_by(_._0)
                       |> _select((City = _._0, N = _._1 |> length)))

        // Fixture has NYC=2, Chicago=2, Boston=1; HAVING > 1 drops Boston.
        t |> equal(length(m1), 2,           "having|order_by: HAVING > 1 keeps 2 groups")
        t |> equal(length(m1), length(m2), "having|order_by: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "having|order_by: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}
