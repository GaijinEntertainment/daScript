options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for v2 multi-Q lowering — aggregate-then-filter:
//   `_group_by |> _select |> _where(_.alias)` and friends.
//
// In `_sql` mode, `_select` (eval phase 5) appearing inside `_where` /
// `take` / etc. (lower phases) triggers a multi-Q lowering: the `_select`
// (with its `_group_by`) becomes a nested SELECT exposed via `AS "alias"`
// columns; the outer chain references those aliases through `_.alias`.
//
// Plain linq runs the same chain positionally — `_select` projects to a
// named tuple, then `_where` filters that tuple by `_.alias`. M1 / M2 / M3
// must agree.
//
// Tuple structural-uniqueness note: parity_check_14_group_by.das already
// installs `tuple<string;int>` with names (City, N) and other shapes;
// case 2 below uses `tuple<string;double>` (City, AvgSalary) to avoid
// arg-type-list collisions. See feedback_daslang_tuple_recordnames_collide.md.

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
    db |> insert(User(Id = 5, Name = "Eve",   City = "Chicago", Age = 50, Salary = 200))
    db |> insert(User(Id = 6, Name = "Frank", City = "Boston",  Age = 28, Salary = 70))
    db |> insert(User(Id = 7, Name = "Adam",  City = "NYC",     Age = 30, Salary = 110))
}

def fixture_array() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "Alice", City = "NYC",     Age = 30, Salary = 100))
    arr |> emplace(User(Id = 2, Name = "Bob",   City = "NYC",     Age = 25, Salary = 80))
    arr |> emplace(User(Id = 3, Name = "Carol", City = "Chicago", Age = 40, Salary = 120))
    arr |> emplace(User(Id = 4, Name = "Dave",  City = "Chicago", Age = 35, Salary = 90))
    arr |> emplace(User(Id = 5, Name = "Eve",   City = "Chicago", Age = 50, Salary = 200))
    arr |> emplace(User(Id = 6, Name = "Frank", City = "Boston",  Age = 28, Salary = 70))
    arr |> emplace(User(Id = 7, Name = "Adam",  City = "NYC",     Age = 30, Salary = 110))
    return <- arr
}

def sort_by_city(var rows : array<auto(R)>) : array<R> {
    rows |> sort() <| $(a, b) {
        return a.City < b.City
    }
    return <- rows
}

[test]
def parity_group_select_where(t : T?) {
    // Case 1: `_group_by |> _select((City=_._0, N=_._1 |> length)) |> _where(_.N > 1)`.
    // Outer WHERE references the aggregate alias `N`. Expect cities with >1 user (NYC=3, Chicago=3).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by(_.City)
                         |> _select((City = _._0, N = _._1 |> length))
                         |> _where(_.N > 1))
        var m2 <- (db |> select_from(type<User>)
                     |> _group_by(_.City)
                     |> _select((City = _._0, N = _._1 |> length))
                     |> _where(_.N > 1))
        var m3 <- (arr |> _group_by(_.City)
                       |> _select((City = _._0, N = _._1 |> length))
                       |> _where(_.N > 1))

        m1 <- sort_by_city(m1)
        m2 <- sort_by_city(m2)
        m3 <- sort_by_city(m3)

        t |> equal(length(m1), length(m2), "select|where: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "select|where: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test]
def parity_group_select_avg_where(t : T?) {
    // Case 2: aggregate sub-select + outer WHERE on the float aggregate alias.
    // Tuple shape `tuple<string;double>` with (City, AvgSalary) — structurally distinct from
    // tuples installed by parity_check_14_group_by to avoid recordNames collision.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by(_.City)
                         |> _select((City = _._0,
                                     AvgSalary = _._1 |> select($(u : User) => u.Salary) |> average))
                         |> _where(_.AvgSalary > 95.0lf))
        var m2 <- (db |> select_from(type<User>)
                     |> _group_by(_.City)
                     |> _select((City = _._0,
                                 AvgSalary = _._1 |> select($(u : User) => u.Salary) |> average))
                     |> _where(_.AvgSalary > 95.0lf))
        var m3 <- (arr |> _group_by(_.City)
                       |> _select((City = _._0,
                                   AvgSalary = _._1 |> select($(u : User) => u.Salary) |> average))
                       |> _where(_.AvgSalary > 95.0lf))

        m1 <- sort_by_city(m1)
        m2 <- sort_by_city(m2)
        m3 <- sort_by_city(m3)

        t |> equal(length(m1), length(m2), "select|where(avg): M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "select|where(avg): M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City,      m2[i].City,      "row[{i}].City:      M1 ≡ M2")
            t |> equal(m1[i].AvgSalary, m2[i].AvgSalary, "row[{i}].AvgSalary: M1 ≡ M2")
            t |> equal(m2[i].City,      m3[i].City,      "row[{i}].City:      M2 ≡ M3")
            t |> equal(m2[i].AvgSalary, m3[i].AvgSalary, "row[{i}].AvgSalary: M2 ≡ M3")
        }
    }
}

[test]
def parity_group_having_select_where(t : T?) {
    // Case 3: HAVING (inner-Q filter on group) plus outer WHERE on the projected alias.
    // Inner Q has GROUP BY + HAVING. Outer Q is WHERE over the projected (City, N).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length > 1)
                         |> _select((City = _._0, N = _._1 |> length))
                         |> _where(_.N > 2))
        var m2 <- (db |> select_from(type<User>)
                     |> _group_by(_.City)
                     |> _having(_._1 |> length > 1)
                     |> _select((City = _._0, N = _._1 |> length))
                     |> _where(_.N > 2))
        var m3 <- (arr |> _group_by(_.City)
                       |> _having(_._1 |> length > 1)
                       |> _select((City = _._0, N = _._1 |> length))
                       |> _where(_.N > 2))

        m1 <- sort_by_city(m1)
        m2 <- sort_by_city(m2)
        m3 <- sort_by_city(m3)

        t |> equal(length(m1), length(m2), "having|select|where: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "having|select|where: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test]
def parity_group_select_where_order_take(t : T?) {
    // Case 4: full chain — outer WHERE + ORDER + TAKE all reference inner projection.
    // Stresses bind ordering: inner GROUP BY binds (none here) → outer WHERE binds (none) →
    // outer LIMIT bind (3).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by(_.City)
                         |> _select((City = _._0, N = _._1 |> length))
                         |> _where(_.N > 1)
                         |> _order_by(_.N)
                         |> take(3))
        var m2 <- (db |> select_from(type<User>)
                     |> _group_by(_.City)
                     |> _select((City = _._0, N = _._1 |> length))
                     |> _where(_.N > 1)
                     |> _order_by(_.N)
                     |> take(3))
        var m3 <- (arr |> _group_by(_.City)
                       |> _select((City = _._0, N = _._1 |> length))
                       |> _where(_.N > 1)
                       |> _order_by(_.N)
                       |> take(3))

        // Order is by N (ASC), then ties — keep order from query, do NOT post-sort.
        t |> equal(length(m1), length(m2), "select|where|order|take: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "select|where|order|take: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].N, m2[i].N, "row[{i}].N: M1 ≡ M2 (order pin)")
            t |> equal(m2[i].N, m3[i].N, "row[{i}].N: M2 ≡ M3 (order pin)")
        }
    }
}
