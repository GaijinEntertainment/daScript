options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require daslib/option
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 16 — `_left_join` with `Option<TB>` projection.
//
// Every left row surfaces; unmatched ones get `none`. Multi-match on the
// right duplicates the left row (Alice's 2 orders → 2 result rows).
// The `into` lambda's second param is `Option<Order>` and the projection
// can probe it via `is_some` / `is_none` (lowers to right-key IS NOT NULL
// in SQL).

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice"))
    db |> insert(User(Id = 2, Name = "Bob"))
    db |> insert(User(Id = 3, Name = "Carol"))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
}

def fixture_users() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "Alice"))
    arr |> emplace(User(Id = 2, Name = "Bob"))
    arr |> emplace(User(Id = 3, Name = "Carol"))
    return <- arr
}

def fixture_orders() : array<Order> {
    var arr : array<Order>
    arr |> emplace(Order(Id = 10, UserId = 1, Total = 100))
    arr |> emplace(Order(Id = 11, UserId = 1, Total = 250))
    arr |> emplace(Order(Id = 12, UserId = 2, Total =  50))
    return <- arr
}

def sort_rows(var rows : array<auto(R)>) : array<R> {
    // Both Alice rows are content-identical ((Name=Alice, HasOrder=true)),
    // so ordering between them doesn't matter — sort by Name alone.
    rows |> sort() <| $(a, b) {
        return a.Name < b.Name
    }
    return <- rows
}

[test]
def parity_left_join_basic(t : T?) {
    // 4 rows expected: Alice/true × 2, Bob/true, Carol/false.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _left_join(db |> select_from(type<Order>),
                                       $(u : User, o : Order)         => u.Id == o.UserId,
                                       $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
        var m2 <- (db |> select_from(type<User>)
                     |> _left_join(db |> select_from(type<Order>),
                                   $(u : User, o : Order)         => u.Id == o.UserId,
                                   $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
        var m3 <- (users |> _left_join(orders,
                                       $(u : User, o : Order)         => u.Id == o.UserId,
                                       $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))

        m1 <- sort_rows(m1)
        m2 <- sort_rows(m2)
        m3 <- sort_rows(m3)

        t |> equal(length(m1), 4, "left join row count: 4 (Alice ×2, Bob, Carol)")
        t |> equal(length(m1), length(m2), "left join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "left join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Name,     m2[i].Name,     "row[{i}].Name:     M1 ≡ M2")
            t |> equal(m1[i].HasOrder, m2[i].HasOrder, "row[{i}].HasOrder: M1 ≡ M2")
            t |> equal(m2[i].Name,     m3[i].Name,     "row[{i}].Name:     M2 ≡ M3")
            t |> equal(m2[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M2 ≡ M3")
        }
    }
}
