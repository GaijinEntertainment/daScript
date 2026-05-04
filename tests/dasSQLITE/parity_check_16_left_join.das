options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require daslib/option
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 16 — every join shape (INNER, LEFT, RIGHT, FULL OUTER, CROSS).
//
// Each test runs the same chain in three modes and compares row-by-row:
//   M1 — `_sql(...)` lowering through SQLite
//   M2 — same chain WITHOUT `_sql` (linq runtime over SQL source)
//   M3 — same chain on plain in-memory arrays
//
// Fixture asymmetries: Carol (id=3) has no orders → LEFT-only, FULL-only.
// Order#13 (UserId=99) has no user → RIGHT-only, FULL-only.
// Each test's projection is structurally distinct (different named-tuple
// field names) to avoid the recordNames-collision regression (#2557).

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
    db |> insert(Order(Id = 10, UserId = 1,  Total = 100))
    db |> insert(Order(Id = 11, UserId = 1,  Total = 250))
    db |> insert(Order(Id = 12, UserId = 2,  Total =  50))
    db |> insert(Order(Id = 13, UserId = 99, Total = 999))   // orphan — no matching user
}

def fixture_users() : array<User> {
    return <- [
        User(Id = 1, Name = "Alice"),
        User(Id = 2, Name = "Bob"),
        User(Id = 3, Name = "Carol")
    ]
}

def fixture_orders() : array<Order> {
    return <- [
        Order(Id = 10, UserId = 1,  Total = 100),
        Order(Id = 11, UserId = 1,  Total = 250),
        Order(Id = 12, UserId = 2,  Total =  50),
        Order(Id = 13, UserId = 99, Total = 999)
    ]
}

def sort_left(var rows : array<auto(R)>) : array<R> {
    // Both Alice rows are content-identical ((Name=Alice, HasOrder=true)),
    // so ordering between them doesn't matter — sort by Name alone.
    rows |> sort() $(a, b) {
        return a.Name < b.Name
    }
    return <- rows
}

def sort_right(var rows : array<auto(R)>) : array<R> {
    rows |> sort() $(a, b) {
        return a.OrderId < b.OrderId
    }
    return <- rows
}

def sort_full(var rows : array<auto(R)>) : array<R> {
    // Multi-key — both probes drive deterministic order across (T,T)/(T,F)/(F,T) groups.
    rows |> sort() $(a, b) {
        if (a.HasUser != b.HasUser) {
            return !a.HasUser && b.HasUser
        }
        return !a.HasOrder && b.HasOrder
    }
    return <- rows
}

def sort_cross(var rows : array<auto(R)>) : array<R> {
    rows |> sort() $(a, b) {
        if (a.UserName != b.UserName) {
            return a.UserName < b.UserName
        }
        return a.OrderId < b.OrderId
    }
    return <- rows
}

[test]
def parity_left_join_basic(t : T?) {
    // 4 rows expected: Alice/true × 2, Bob/true, Carol/false. Orphan Order#13 doesn't surface (LEFT).
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let users  <- fixture_users()
        let orders <- fixture_orders()

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

        m1 <- sort_left(m1)
        m2 <- sort_left(m2)
        m3 <- sort_left(m3)

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

[test]
def parity_right_join_basic(t : T?) {
    // 4 rows expected: orders #10/#11/#12 with HasUser=true, orphan #13 with HasUser=false.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let users  <- fixture_users()
        let orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _right_join(db |> select_from(type<Order>),
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) => (HasUser = u |> is_some, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>)
                     |> _right_join(db |> select_from(type<Order>),
                                    $(u : User, o : Order)         => u.Id == o.UserId,
                                    $(u : Option<User>, o : Order) => (HasUser = u |> is_some, OrderId = o.Id)))
        var m3 <- (users |> _right_join(orders,
                                        $(u : User, o : Order)         => u.Id == o.UserId,
                                        $(u : Option<User>, o : Order) => (HasUser = u |> is_some, OrderId = o.Id)))

        m1 <- sort_right(m1)
        m2 <- sort_right(m2)
        m3 <- sort_right(m3)

        t |> equal(length(m1), 4, "right join row count: 4 (orders 10/11/12 with HasUser=true, orphan 13 with HasUser=false)")
        t |> equal(length(m1), length(m2), "right join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "right join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].HasUser, m2[i].HasUser, "row[{i}].HasUser: M1 ≡ M2")
            t |> equal(m1[i].OrderId, m2[i].OrderId, "row[{i}].OrderId: M1 ≡ M2")
            t |> equal(m2[i].HasUser, m3[i].HasUser, "row[{i}].HasUser: M2 ≡ M3")
            t |> equal(m2[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M2 ≡ M3")
        }
    }
}

[test]
def parity_full_outer_join_basic(t : T?) {
    // 5 rows expected: 3 matched (Alice ×2, Bob), Carol with HasOrder=false, orphan with HasUser=false.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let users  <- fixture_users()
        let orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _full_outer_join(db |> select_from(type<Order>),
                                             $(u : User, o : Order)                 => u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) => (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m2 <- (db |> select_from(type<User>)
                     |> _full_outer_join(db |> select_from(type<Order>),
                                         $(u : User, o : Order)                 => u.Id == o.UserId,
                                         $(u : Option<User>, o : Option<Order>) => (HasUser = u |> is_some, HasOrder = o |> is_some)))
        var m3 <- (users |> _full_outer_join(orders,
                                             $(u : User, o : Order)                 => u.Id == o.UserId,
                                             $(u : Option<User>, o : Option<Order>) => (HasUser = u |> is_some, HasOrder = o |> is_some)))

        m1 <- sort_full(m1)
        m2 <- sort_full(m2)
        m3 <- sort_full(m3)

        t |> equal(length(m1), 5, "full outer row count: 5 (3 matched + 1 LEFT-only + 1 RIGHT-only)")
        t |> equal(length(m1), length(m2), "full outer: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "full outer: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].HasUser,  m2[i].HasUser,  "row[{i}].HasUser:  M1 ≡ M2")
            t |> equal(m1[i].HasOrder, m2[i].HasOrder, "row[{i}].HasOrder: M1 ≡ M2")
            t |> equal(m2[i].HasUser,  m3[i].HasUser,  "row[{i}].HasUser:  M2 ≡ M3")
            t |> equal(m2[i].HasOrder, m3[i].HasOrder, "row[{i}].HasOrder: M2 ≡ M3")
        }
    }
}

[test]
def parity_cross_join_basic(t : T?) {
    // 12 rows expected: 3 users × 4 orders, every pair.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        let users  <- fixture_users()
        let orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _cross_join(db |> select_from(type<Order>),
                                        $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>)
                     |> _cross_join(db |> select_from(type<Order>),
                                    $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))
        var m3 <- (users |> _cross_join(orders,
                                        $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))

        m1 <- sort_cross(m1)
        m2 <- sort_cross(m2)
        m3 <- sort_cross(m3)

        t |> equal(length(m1), 12, "cross row count: 12 (3 users × 4 orders)")
        t |> equal(length(m1), length(m2), "cross: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "cross: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName, m2[i].UserName, "row[{i}].UserName: M1 ≡ M2")
            t |> equal(m1[i].OrderId,  m2[i].OrderId,  "row[{i}].OrderId:  M1 ≡ M2")
            t |> equal(m2[i].UserName, m3[i].UserName, "row[{i}].UserName: M2 ≡ M3")
            t |> equal(m2[i].OrderId,  m3[i].OrderId,  "row[{i}].OrderId:  M2 ≡ M3")
        }
    }
}
