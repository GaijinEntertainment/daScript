options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for v2 multi-Q lowering — multi-join (3+ tables in a single chain).
//
// Inner `_join(A, B, ...)` produces a NamedTuple. The outer `_join(<inner>, C, ...)`
// snapshots the inner Q to an `(<innerSql>) AS "t0"` subquery FROM and joins C against
// it. Outer ON-clause and `into` projection resolve `<lhsArg>.<alias>` against the
// inner's projected named tuple (via `q.fromRowType`).
//
// In plain linq, the inner `_join` returns `iterator/array<tuple>`, and the outer
// `_join` accepts that as `srca`. M1 / M2 / M3 must agree.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name   : string
    Active : bool
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    OrderId : int
    Sku     : string
    Qty     : int
}

// Outer-join lambdas need explicit types on BOTH args — daslang's lambda inference can't
// derive the LHS tuple type from join's signature when one arg is typed and the other auto.
// A typedef alias keeps the call sites readable; spelling the tuple inline also works.
typedef UserOrder = tuple<UserName : string; OrderId : int; OrderTotal : int>

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> create_table(type<Item>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
    db |> insert(Item(Id = 100, OrderId = 10, Sku = "BOOK",   Qty = 2))
    db |> insert(Item(Id = 101, OrderId = 11, Sku = "PEN",    Qty = 5))
    db |> insert(Item(Id = 102, OrderId = 11, Sku = "BOOK",   Qty = 1))
    db |> insert(Item(Id = 103, OrderId = 12, Sku = "PAPER",  Qty = 10))
}

def fixture_users() : array<User> {
    var arr : array<User>
    arr |> emplace(User(Id = 1, Name = "Alice", Active = true))
    arr |> emplace(User(Id = 2, Name = "Bob",   Active = true))
    arr |> emplace(User(Id = 3, Name = "Carol", Active = false))
    return <- arr
}

def fixture_orders() : array<Order> {
    var arr : array<Order>
    arr |> emplace(Order(Id = 10, UserId = 1, Total = 100))
    arr |> emplace(Order(Id = 11, UserId = 1, Total = 250))
    arr |> emplace(Order(Id = 12, UserId = 2, Total =  50))
    return <- arr
}

def fixture_items() : array<Item> {
    var arr : array<Item>
    arr |> emplace(Item(Id = 100, OrderId = 10, Sku = "BOOK",  Qty = 2))
    arr |> emplace(Item(Id = 101, OrderId = 11, Sku = "PEN",   Qty = 5))
    arr |> emplace(Item(Id = 102, OrderId = 11, Sku = "BOOK",  Qty = 1))
    arr |> emplace(Item(Id = 103, OrderId = 12, Sku = "PAPER", Qty = 10))
    return <- arr
}

def sort_by_user_sku(var rows : array<auto(R)>) : array<R> {
    rows |> sort() <| $(a, b) {
        if (a.UserName != b.UserName) {
            return a.UserName < b.UserName
        }
        if (a.OrderTotal != b.OrderTotal) {
            return a.OrderTotal < b.OrderTotal
        }
        return a.Sku < b.Sku
    }
    return <- rows
}

[test]
def parity_three_table_join(t : T?) {
    // Case 1: User INNER JOIN Order INNER JOIN Item.
    // Inner _join projects (UserName, OrderId, OrderTotal). Outer references those
    // aliases by name — `uo.OrderId`, `uo.UserName`, `uo.OrderTotal`.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(db |> select_from(type<Item>),
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty)))
        var m2 <- (db |> select_from(type<User>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User, o : Order) => u.Id == o.UserId,
                              $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                     |> _join(db |> select_from(type<Item>),
                              $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                              $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty)))
        var m3 <- (users |> _join(orders,
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(items,
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty)))

        m1 <- sort_by_user_sku(m1)
        m2 <- sort_by_user_sku(m2)
        m3 <- sort_by_user_sku(m3)

        t |> equal(length(m1), 4, "3-table inner: 4 rows (Alice's 2 orders × items, Bob's 1 order × 1 item)")
        t |> equal(length(m1), length(m2), "3-table: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "3-table: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].Sku,        m2[i].Sku,        "row[{i}].Sku:        M1 ≡ M2")
            t |> equal(m1[i].Qty,        m2[i].Qty,        "row[{i}].Qty:        M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].Sku,        m3[i].Sku,        "row[{i}].Sku:        M2 ≡ M3")
            t |> equal(m2[i].Qty,        m3[i].Qty,        "row[{i}].Qty:        M2 ≡ M3")
        }
    }
}

[test]
def parity_three_table_join_with_outer_where(t : T?) {
    // Case 2 (v2.1): outer WHERE on a multi-join's projected alias. The WHERE peel
    // snapshots the analyzed q in place (in-place to avoid the linq Mode-2/3 typer
    // cascade that re-analysis would tickle) and renders as
    // `... FROM (3-table joinSql) AS "t0" WHERE "alias" = ?`, with `alias` resolved
    // through q.fromRowType set by the snapshot.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(db |> select_from(type<Item>),
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Sku == "BOOK"))
        var m2 <- (db |> select_from(type<User>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User, o : Order) => u.Id == o.UserId,
                              $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                     |> _join(db |> select_from(type<Item>),
                              $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                              $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                     |> _where(_.Sku == "BOOK"))
        var m3 <- (users |> _join(orders,
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(items,
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Sku == "BOOK"))

        m1 <- sort_by_user_sku(m1)
        m2 <- sort_by_user_sku(m2)
        m3 <- sort_by_user_sku(m3)

        // Fixture has 3 BOOK items (Item 100, 102 — Item 101 is PEN, 103 is PAPER). Item 102 belongs to
        // Order 11 (Alice, total 250); Item 100 to Order 10 (Alice, 100). Bob has no BOOK item. Expect 2 rows.
        t |> equal(length(m1), 2, "3-table + WHERE Sku=BOOK: 2 rows")
        t |> equal(length(m1), length(m2), "M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].Sku,        m2[i].Sku,        "row[{i}].Sku:        M1 ≡ M2")
            t |> equal(m1[i].Qty,        m2[i].Qty,        "row[{i}].Qty:        M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].Sku,        m3[i].Sku,        "row[{i}].Sku:        M2 ≡ M3")
            t |> equal(m2[i].Qty,        m3[i].Qty,        "row[{i}].Qty:        M2 ≡ M3")
            t |> equal(m1[i].Sku,        "BOOK",           "row[{i}].Sku == BOOK (predicate)")
        }
    }
}

[test]
def parity_three_table_join_with_outer_where_int(t : T?) {
    // v2.1: outer WHERE on a numeric outer-projection alias (Qty), with multi-arg AND.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(db |> select_from(type<Item>),
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Qty >= 2 && _.UserName == "Alice"))
        var m2 <- (db |> select_from(type<User>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User, o : Order) => u.Id == o.UserId,
                              $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                     |> _join(db |> select_from(type<Item>),
                              $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                              $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                     |> _where(_.Qty >= 2 && _.UserName == "Alice"))
        var m3 <- (users |> _join(orders,
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                         |> _join(items,
                                  $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                  $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                         |> _where(_.Qty >= 2 && _.UserName == "Alice"))

        m1 <- sort_by_user_sku(m1)
        m2 <- sort_by_user_sku(m2)
        m3 <- sort_by_user_sku(m3)

        // Alice's items with Qty >= 2: Item 100 (BOOK, Qty 2), Item 101 (PEN, Qty 5). Two rows.
        t |> equal(length(m1), 2, "3-table + WHERE Qty>=2 AND UserName=Alice: 2 rows")
        t |> equal(length(m1), length(m2), "M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName, m2[i].UserName, "row[{i}].UserName: M1 ≡ M2")
            t |> equal(m1[i].Sku,      m2[i].Sku,      "row[{i}].Sku:      M1 ≡ M2")
            t |> equal(m1[i].Qty,      m2[i].Qty,      "row[{i}].Qty:      M1 ≡ M2")
        }
    }
}
