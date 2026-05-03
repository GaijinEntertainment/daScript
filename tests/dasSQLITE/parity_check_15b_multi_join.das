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
def parity_three_table_join_then_first(t : T?) {
    // V2 latent regression: `_join(_join(A,B,...), C, ...) |> _first()` previously
    // failed at compile time with "_join LHS chain cannot end in a terminal" — the
    // snapshot helper mistook the OUTER `_first` for an INNER terminal. Same fix as
    // the v2.1 single-join+terminal case (hoisted check in process_join_call).
    // Explicit `_order_by(_.OrderTotal)` pins the chosen row so all three modes
    // (M1=_sql, M2=linq via select_from, M3=plain linq over array) agree on the
    // _first() result. Reuses case 1's projection shape (UserName/OrderTotal/Sku/Qty);
    // the named tuple is already cached from `parity_three_table_join`.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        let m1 = _sql(db |> select_from(type<User>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(db |> select_from(type<Item>),
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                        |> _order_by(_.OrderTotal)
                        |> _first())
        let m2 = (db |> select_from(type<User>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User, o : Order) => u.Id == o.UserId,
                             $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                    |> _join(db |> select_from(type<Item>),
                             $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                             $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                    |> _order_by(_.OrderTotal)
                    |> _first())
        let m3 = (users |> _join(orders,
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(items,
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) => (UserName = uo.UserName, OrderTotal = uo.OrderTotal, Sku = it.Sku, Qty = it.Qty))
                        |> _order_by(_.OrderTotal)
                        |> _first())

        // 4-row joined result; ORDER BY OrderTotal ASC: Bob's Order 12 (Total 50, PAPER, Qty 10) wins.
        t |> equal(m1.UserName,   "Bob",   "M1 _first picks Bob's row (lowest OrderTotal)")
        t |> equal(m1.OrderTotal, 50,      "M1 OrderTotal is 50")
        t |> equal(m1.Sku,        "PAPER", "M1 Sku matches Bob's only item")
        t |> equal(m1.UserName,   m2.UserName,   "M1 ≡ M2 UserName")
        t |> equal(m1.OrderTotal, m2.OrderTotal, "M1 ≡ M2 OrderTotal")
        t |> equal(m1.Sku,        m2.Sku,        "M1 ≡ M2 Sku")
        t |> equal(m1.Qty,        m2.Qty,        "M1 ≡ M2 Qty")
        t |> equal(m2.UserName,   m3.UserName,   "M2 ≡ M3 UserName")
        t |> equal(m2.OrderTotal, m3.OrderTotal, "M2 ≡ M3 OrderTotal")
        t |> equal(m2.Sku,        m3.Sku,        "M2 ≡ M3 Sku")
        t |> equal(m2.Qty,        m3.Qty,        "M2 ≡ M3 Qty")
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

        // Fixture has 2 BOOK items: Item 100 (Order 10, Alice, total 100) and
        // Item 102 (Order 11, Alice, total 250). Item 101 is PEN, 103 is PAPER.
        // Bob has no BOOK item. Expect 2 rows.
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
def parity_three_table_join_then_count(t : T?) {
    // v2.1 review-3 regression for E1 + multi-join: `_join(_join(A,B), C) |> count()`
    // (no WHERE). Pre-fix: outer process_join_call's analyze_projection clobbered
    // q.proj=NamedTuple after the multi-join snapshot ran, dropping aggregateExpr=COUNT(*).
    // Post-fix: process_join_call's save/restore preserves aggregate state across BOTH
    // inner and outer analyze_projection calls.
    //
    // Outer projection: (PayPlus:int, Stock:int, Code:string, Holder:string)
    // → tuple<int;int;string;string> — structurally distinct from every other test here.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        let n1 = _sql(db |> select_from(type<User>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(db |> select_from(type<Item>),
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) => (PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku, Holder = uo.UserName))
                        |> count())
        let n2 = (db |> select_from(type<User>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User, o : Order) => u.Id == o.UserId,
                             $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                    |> _join(db |> select_from(type<Item>),
                             $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                             $(uo : UserOrder, it : Item) => (PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku, Holder = uo.UserName))
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(items,
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) => (PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku, Holder = uo.UserName))
                        |> count())

        // 4 items each join their order's user → 4 rows total.
        t |> equal(n1, 4, "3-table | count(): 4 INNER-join rows")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}

[test]
def parity_three_table_join_outer_where_then_count(t : T?) {
    // v2.1 review-3 regression for stacked snapshots: multi-join (v2 snapshot via
    // process_join_call) + WHERE-peel (v2.1 snapshot) + count() — exercises BOTH snapshot
    // paths preserving aggregate state across the OUTER COUNT(*) projection.
    //
    // Outer projection: (Holder:string, PayPlus:int, Stock:int, Code:string)
    // → tuple<string;int;int;string> — structurally distinct from every other test here.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()
        var items  <- fixture_items()

        let n1 = _sql(db |> select_from(type<User>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(db |> select_from(type<Item>),
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) => (Holder = uo.UserName, PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku))
                        |> _where(_.PayPlus > 75)
                        |> count())
        let n2 = (db |> select_from(type<User>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User, o : Order) => u.Id == o.UserId,
                             $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                    |> _join(db |> select_from(type<Item>),
                             $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                             $(uo : UserOrder, it : Item) => (Holder = uo.UserName, PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku))
                    |> _where(_.PayPlus > 75)
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id, OrderTotal = o.Total))
                        |> _join(items,
                                 $(uo : UserOrder, it : Item) => uo.OrderId == it.OrderId,
                                 $(uo : UserOrder, it : Item) => (Holder = uo.UserName, PayPlus = uo.OrderTotal, Stock = it.Qty, Code = it.Sku))
                        |> _where(_.PayPlus > 75)
                        |> count())

        // Items joined to Order.Total > 75: order 10 (100, BOOK), order 11 (250, PEN+BOOK).
        // 3 rows. Order 12 (Total=50, PAPER) excluded.
        t |> equal(n1, 3, "3-table | WHERE PayPlus>75 | count(): 3 rows")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}
