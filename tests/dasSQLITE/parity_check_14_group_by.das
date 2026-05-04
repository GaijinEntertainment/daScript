options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 14 — `_group_by` and `_having`.

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

def sort_by_city_age(var rows : array<auto(R)>) : array<R> {
    rows |> sort() $(a, b) {
        return a.City == b.City ? a.Age < b.Age : a.City < b.City
    }
    return <- rows
}

[test]
def parity_group_by_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by(_.City)
                         |> _order_by(_._0)
                         |> _select((City = _._0, N = _._1 |> length)))
        let m2 <- (db |> select_from(type<User>)
                     |> _group_by(_.City)
                     |> _order_by(_._0)
                     |> _select((City = _._0, N = _._1 |> length)))
        let m3 <- (arr |> _group_by(_.City)
                       |> _order_by(_._0)
                       |> _select((City = _._0, N = _._1 |> length)))

        t |> equal(length(m1), length(m2), "group+count: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "group+count: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test]
def parity_group_by_multi_aggregate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by(_.City)
                         |> _order_by(_._0)
                         |> _select((
                             City   = _._0,
                             N      = _._1 |> length,
                             MinAge = _._1 |> select($(u : User) => u.Age)    |> min,
                             MaxAge = _._1 |> select($(u : User) => u.Age)    |> max,
                             Total  = _._1 |> select($(u : User) => u.Salary) |> sum)))
        let m2 <- (db |> select_from(type<User>)
                     |> _group_by(_.City)
                     |> _order_by(_._0)
                     |> _select((
                         City   = _._0,
                         N      = _._1 |> length,
                         MinAge = _._1 |> select($(u : User) => u.Age)    |> min,
                         MaxAge = _._1 |> select($(u : User) => u.Age)    |> max,
                         Total  = _._1 |> select($(u : User) => u.Salary) |> sum)))
        let m3 <- (arr |> _group_by(_.City)
                       |> _order_by(_._0)
                       |> _select((
                           City   = _._0,
                           N      = _._1 |> length,
                           MinAge = _._1 |> select($(u : User) => u.Age)    |> min,
                           MaxAge = _._1 |> select($(u : User) => u.Age)    |> max,
                           Total  = _._1 |> select($(u : User) => u.Salary) |> sum)))

        t |> equal(length(m1), length(m2), "multi-agg length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "multi-agg length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City,   m2[i].City,   "row[{i}].City:   M1 ≡ M2")
            t |> equal(m1[i].N,      m2[i].N,      "row[{i}].N:      M1 ≡ M2")
            t |> equal(m1[i].MinAge, m2[i].MinAge, "row[{i}].MinAge: M1 ≡ M2")
            t |> equal(m1[i].MaxAge, m2[i].MaxAge, "row[{i}].MaxAge: M1 ≡ M2")
            t |> equal(m1[i].Total,  m2[i].Total,  "row[{i}].Total:  M1 ≡ M2")
            t |> equal(m2[i].City,   m3[i].City,   "row[{i}].City:   M2 ≡ M3")
            t |> equal(m2[i].N,      m3[i].N,      "row[{i}].N:      M2 ≡ M3")
            t |> equal(m2[i].MinAge, m3[i].MinAge, "row[{i}].MinAge: M2 ≡ M3")
            t |> equal(m2[i].MaxAge, m3[i].MaxAge, "row[{i}].MaxAge: M2 ≡ M3")
            t |> equal(m2[i].Total,  m3[i].Total,  "row[{i}].Total:  M2 ≡ M3")
        }
    }
}

[test]
def parity_where_group_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.Age >= 30)
                         |> _group_by(_.City)
                         |> _order_by(_._0)
                         |> _select((City = _._0, N = _._1 |> length)))
        let m2 <- (db |> select_from(type<User>)
                     |> _where(_.Age >= 30)
                     |> _group_by(_.City)
                     |> _order_by(_._0)
                     |> _select((City = _._0, N = _._1 |> length)))
        let m3 <- (arr |> _where(_.Age >= 30)
                       |> _group_by(_.City)
                       |> _order_by(_._0)
                       |> _select((City = _._0, N = _._1 |> length)))

        t |> equal(length(m1), length(m2), "where|group|count: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "where|group|count: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test]
def parity_group_having(t : T?) {
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

        t |> equal(length(m1), length(m2), "group|having: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "group|having: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test]
def parity_multi_key_group(t : T?) {
    // No inline `_order_by` here: the `_sql` analyzer's tuple-key
    // validator accepts only `_.Field` / string expressions, not nested
    // walks like `_._0._N`, so multi-component sorts after `_group_by`
    // can't be expressed as a single `_order_by` clause across all
    // three modes. Post-sort by (City, Age) instead.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _group_by((_.City, _.Age))
                         |> _having(_._1 |> length > 1)
                         |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
        var m2 <- (db |> select_from(type<User>)
                     |> _group_by((_.City, _.Age))
                     |> _having(_._1 |> length > 1)
                     |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
        var m3 <- (arr |> _group_by((_.City, _.Age))
                       |> _having(_._1 |> length > 1)
                       |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))

        m1 <- sort_by_city_age(m1)
        m2 <- sort_by_city_age(m2)
        m3 <- sort_by_city_age(m3)

        t |> equal(length(m1), length(m2), "multi-key length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "multi-key length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City, m2[i].City, "row[{i}].City: M1 ≡ M2")
            t |> equal(m1[i].Age,  m2[i].Age,  "row[{i}].Age:  M1 ≡ M2")
            t |> equal(m1[i].N,    m2[i].N,    "row[{i}].N:    M1 ≡ M2")
            t |> equal(m2[i].City, m3[i].City, "row[{i}].City: M2 ≡ M3")
            t |> equal(m2[i].Age,  m3[i].Age,  "row[{i}].Age:  M2 ≡ M3")
            t |> equal(m2[i].N,    m3[i].N,    "row[{i}].N:    M2 ≡ M3")
        }
    }
}

[test]
def parity_full_reporting_chain(t : T?) {
    // Full reporting: WHERE → GROUP BY → HAVING → ORDER BY → SELECT → take.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let arr <- fixture_array()
        let min_age = 25
        let min_count = 1

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.Age >= min_age)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length >= min_count)
                         |> _order_by(_._0)
                         |> _select((
                             City      = _._0,
                             N         = _._1 |> length,
                             TotSalary = _._1 |> select($(u : User) => u.Salary) |> sum,
                             OldestAge = _._1 |> select($(u : User) => u.Age)    |> max))
                         |> take(10))
        let m2 <- (db |> select_from(type<User>)
                     |> _where(_.Age >= min_age)
                     |> _group_by(_.City)
                     |> _having(_._1 |> length >= min_count)
                     |> _order_by(_._0)
                     |> _select((
                         City      = _._0,
                         N         = _._1 |> length,
                         TotSalary = _._1 |> select($(u : User) => u.Salary) |> sum,
                         OldestAge = _._1 |> select($(u : User) => u.Age)    |> max))
                     |> take(10))
        let m3 <- (arr |> _where(_.Age >= min_age)
                       |> _group_by(_.City)
                       |> _having(_._1 |> length >= min_count)
                       |> _order_by(_._0)
                       |> _select((
                           City      = _._0,
                           N         = _._1 |> length,
                           TotSalary = _._1 |> select($(u : User) => u.Salary) |> sum,
                           OldestAge = _._1 |> select($(u : User) => u.Age)    |> max))
                       |> take(10))

        t |> equal(length(m1), length(m2), "full chain length: M1 ≡ M2")
        t |> equal(length(m2), length(m3), "full chain length: M2 ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].City,      m2[i].City,      "row[{i}].City:      M1 ≡ M2")
            t |> equal(m1[i].N,         m2[i].N,         "row[{i}].N:         M1 ≡ M2")
            t |> equal(m1[i].TotSalary, m2[i].TotSalary, "row[{i}].TotSalary: M1 ≡ M2")
            t |> equal(m1[i].OldestAge, m2[i].OldestAge, "row[{i}].OldestAge: M1 ≡ M2")
            t |> equal(m2[i].City,      m3[i].City,      "row[{i}].City:      M2 ≡ M3")
            t |> equal(m2[i].N,         m3[i].N,         "row[{i}].N:         M2 ≡ M3")
            t |> equal(m2[i].TotSalary, m3[i].TotSalary, "row[{i}].TotSalary: M2 ≡ M3")
            t |> equal(m2[i].OldestAge, m3[i].OldestAge, "row[{i}].OldestAge: M2 ≡ M3")
        }
    }
}
