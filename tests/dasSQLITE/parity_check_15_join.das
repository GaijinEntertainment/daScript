options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Parity check for tutorial 15 — `_join`: inner equi-join.
//
// Shapes:
//   1. plain inner equi-join + named-tuple projection
//   2. pre-join `_where` (filters left source before join)
//   3. right-side `_where` inside the joined chain (filter via JOIN ON)
//
// Each test uses a structurally distinct projection tuple to avoid
// the generic-instantiation recordNames collision: any two
// `tuple<string;int>` literals flowing through the same linq generic
// in this compilation unit would share the cached instantiation and
// lose the second's recordNames. (See feedback memory.)

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

def fixture_db(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total =  50))
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

def sort_by_pair(var rows : array<auto(R)>) : array<R> {
    rows |> sort() <| $(a, b) {
        if (a.UserName != b.UserName) {
            return a.UserName < b.UserName
        }
        return a.OrderTotal < b.OrderTotal
    }
    return <- rows
}

[test]
def parity_inner_join_basic(t : T?) {
    // Tuple shape: (UserName : string, OrderTotal : int) → tuple<string;int>
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total)))
        var m2 <- (db |> select_from(type<User>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User, o : Order) => u.Id == o.UserId,
                              $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total)))
        var m3 <- (users |> _join(orders,
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total)))

        m1 <- sort_by_pair(m1)
        m2 <- sort_by_pair(m2)
        m3 <- sort_by_pair(m3)

        t |> equal(length(m1), 3, "inner join row count: 3 (Carol absent)")
        t |> equal(length(m1), length(m2), "join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
        }
    }
}

def sort_by_pair_with_active(var rows : array<auto(R)>) : array<R> {
    rows |> sort() <| $(a, b) {
        if (a.UserName != b.UserName) {
            return a.UserName < b.UserName
        }
        return a.OrderTotal < b.OrderTotal
    }
    return <- rows
}

[test]
def parity_left_filter_join(t : T?) {
    // Tuple shape: (UserName, OrderTotal, IsActive) → tuple<string;int;bool> —
    // structurally distinct from parity_inner_join_basic.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.Active)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total, IsActive = u.Active)))
        var m2 <- (db |> select_from(type<User>)
                     |> _where(_.Active)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User, o : Order) => u.Id == o.UserId,
                              $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total, IsActive = u.Active)))
        var m3 <- (users |> _where(_.Active)
                         |> _join(orders,
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total, IsActive = u.Active)))

        m1 <- sort_by_pair_with_active(m1)
        m2 <- sort_by_pair_with_active(m2)
        m3 <- sort_by_pair_with_active(m3)

        t |> equal(length(m1), length(m2), "where|join: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "where|join: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].IsActive,   m2[i].IsActive,   "row[{i}].IsActive:   M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].IsActive,   m3[i].IsActive,   "row[{i}].IsActive:   M2 ≡ M3")
        }
    }
}

def sort_by_pair_with_orderid(var rows : array<auto(R)>) : array<R> {
    rows |> sort() <| $(a, b) {
        if (a.UserName != b.UserName) {
            return a.UserName < b.UserName
        }
        return a.OrderId < b.OrderId
    }
    return <- rows
}

def sort_by_buyer_orderid(var rows : array<auto(R)>) : array<R> {
    rows |> sort() <| $(a, b) {
        if (a.Buyer != b.Buyer) {
            return a.Buyer < b.Buyer
        }
        return a.OrderId < b.OrderId
    }
    return <- rows
}

[test]
def parity_join_with_outer_where_then_first(t : T?) {
    // v2.1 regression: `_join(...) |> _where(_.alias) |> _order_by(_.alias) |> _first()`
    // previously failed at compile time with the misleading "_join LHS chain cannot
    // end in a terminal" error — the snapshot helper conflated the OUTER terminal
    // flag with an INNER terminal it was meant to reject. Hoisted check now lives in
    // process_join_call's srca recursion. Explicit `_order_by` on the projected alias
    // pins the chosen row deterministically across all three modes (M1=_sql,
    // M2=linq via select_from, M3=plain linq over array). Tuple shape:
    // (RefId:int, Label:string, Amount:int) → tuple<int;string;int> — leading int
    // makes the arg-type list structurally distinct from every other test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 = _sql(db |> select_from(type<User>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (RefId = o.Id, Label = u.Name, Amount = o.Total))
                        |> _where(_.Amount > 75)
                        |> _order_by(_.Amount)
                        |> _first())
        let m2 = (db |> select_from(type<User>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User, o : Order) => u.Id == o.UserId,
                             $(u : User, o : Order) => (RefId = o.Id, Label = u.Name, Amount = o.Total))
                    |> _where(_.Amount > 75)
                    |> _order_by(_.Amount)
                    |> _first())
        let m3 = (users |> _join(orders,
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (RefId = o.Id, Label = u.Name, Amount = o.Total))
                        |> _where(_.Amount > 75)
                        |> _order_by(_.Amount)
                        |> _first())

        // Alice's orders 100 / 250 both pass `> 75`; ORDER BY Amount ASC picks 100
        // (Order Id 10 = "Alice" / 100). Same row in all three modes.
        t |> equal(m1.Amount, 100,    "M1 _first picks the ASC-min passing row")
        t |> equal(m1.Label,  "Alice", "M1 row is Alice's")
        t |> equal(m1.Amount, m2.Amount, "M1 ≡ M2 Amount")
        t |> equal(m1.Label,  m2.Label,  "M1 ≡ M2 Label")
        t |> equal(m1.RefId,  m2.RefId,  "M1 ≡ M2 RefId")
        t |> equal(m2.Amount, m3.Amount, "M2 ≡ M3 Amount")
        t |> equal(m2.Label,  m3.Label,  "M2 ≡ M3 Label")
        t |> equal(m2.RefId,  m3.RefId,  "M2 ≡ M3 RefId")
    }
}

[test]
def parity_join_outer_where_first_inner_limit_leak(t : T?) {
    // v2.1 review-2 regression for the inner-LIMIT leak: pre-fix, snapshot's
    // `build_sql_string(q, true)` ran with q.matr=One pre-set by outer `_first` →
    // inner subquery emitted `LIMIT 1` BEFORE the outer WHERE filtered. With the
    // fixture's natural JOIN order (Alice 100, Alice 250, Bob 50), inner LIMIT 1
    // picked Alice 100; outer WHERE OrderAmount == 50 filtered it → 0 rows →
    // `_first()` (Materializer.One) panicked on empty result.
    //
    // After the fix, snapshot save→clears outer-terminal state pre-build_sql_string
    // and restores it post-reset; inner emits no LIMIT, outer is
    // `WHERE OrderAmount == 50 LIMIT 1` → picks Bob 50 correctly.
    //
    // M2/M3 (linq over `select_from`'s materialized array / plain in-memory array) don't go
    // through the SQL builder at all — they serve as oracles confirming the post-fix M1 picks
    // the same row the in-memory references do. The bug only ever existed in M1.
    //
    // Tuple shape (IsActive:bool, BuyerName:string, OrderAmount:int) → tuple<bool;string;int>
    // — structurally distinct from every other test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 = _sql(db |> select_from(type<User>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (IsActive = u.Active, BuyerName = u.Name, OrderAmount = o.Total))
                        |> _where(_.OrderAmount == 50)
                        |> _first())
        let m2 = (db |> select_from(type<User>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User, o : Order) => u.Id == o.UserId,
                             $(u : User, o : Order) => (IsActive = u.Active, BuyerName = u.Name, OrderAmount = o.Total))
                    |> _where(_.OrderAmount == 50)
                    |> _first())
        let m3 = (users |> _join(orders,
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (IsActive = u.Active, BuyerName = u.Name, OrderAmount = o.Total))
                        |> _where(_.OrderAmount == 50)
                        |> _first())

        // Without the fix: m1 would be garbage (0 rows from outer WHERE → _first panic
        // or zeroed). With the fix: deterministically Bob's order 12 (UserId 2, Total 50).
        t |> equal(m1.BuyerName,   "Bob", "WHERE OrderAmount==50 picks Bob (only matching row)")
        t |> equal(m1.OrderAmount, 50,    "Picked-row Total is 50")
        t |> equal(m1.IsActive,    true,  "Bob is active")
        // M2/M3 oracles: in-memory references confirm M1 picks the same row.
        t |> equal(m1.BuyerName,   m2.BuyerName,   "M1 ≡ M2 BuyerName")
        t |> equal(m1.OrderAmount, m2.OrderAmount, "M1 ≡ M2 OrderAmount")
        t |> equal(m1.IsActive,    m2.IsActive,    "M1 ≡ M2 IsActive")
        t |> equal(m2.BuyerName,   m3.BuyerName,   "M2 ≡ M3 BuyerName")
        t |> equal(m2.OrderAmount, m3.OrderAmount, "M2 ≡ M3 OrderAmount")
        t |> equal(m2.IsActive,    m3.IsActive,    "M2 ≡ M3 IsActive")
    }
}

[test]
def parity_join_with_outer_where(t : T?) {
    // v2.1: outer WHERE on a single _join's projected alias must filter PROJECTED rows.
    // The WHERE peel snapshots the analyzed q in place; SQL renders as
    // `... FROM (joinSql) AS "t0" WHERE "alias" = ?` with the alias resolved through
    // q.fromRowType. Tuple shape: (Buyer:string, Spend:int, OrderId:int, Win:bool) →
    // tuple<string;int;int;bool> — structurally distinct from the other tests in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _join(db |> select_from(type<Order>),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (Buyer = u.Name, Spend = o.Total, OrderId = o.Id, Win = u.Active))
                         |> _where(_.Spend > 75))
        var m2 <- (db |> select_from(type<User>)
                     |> _join(db |> select_from(type<Order>),
                              $(u : User, o : Order) => u.Id == o.UserId,
                              $(u : User, o : Order) => (Buyer = u.Name, Spend = o.Total, OrderId = o.Id, Win = u.Active))
                     |> _where(_.Spend > 75))
        var m3 <- (users |> _join(orders,
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (Buyer = u.Name, Spend = o.Total, OrderId = o.Id, Win = u.Active))
                         |> _where(_.Spend > 75))

        m1 <- sort_by_buyer_orderid(m1)
        m2 <- sort_by_buyer_orderid(m2)
        m3 <- sort_by_buyer_orderid(m3)

        // Alice: orders 100, 250 → both > 75. Bob: 50 → dropped. Expect 2 rows.
        t |> equal(length(m1), 2, "join + outer WHERE Spend>75: 2 rows (Alice ×2, Bob's 50 dropped)")
        t |> equal(length(m1), length(m2), "M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Buyer,   m2[i].Buyer,   "row[{i}].Buyer:   M1 ≡ M2")
            t |> equal(m1[i].Spend,   m2[i].Spend,   "row[{i}].Spend:   M1 ≡ M2")
            t |> equal(m1[i].OrderId, m2[i].OrderId, "row[{i}].OrderId: M1 ≡ M2")
            t |> equal(m2[i].Buyer,   m3[i].Buyer,   "row[{i}].Buyer:   M2 ≡ M3")
            t |> equal(m2[i].Spend,   m3[i].Spend,   "row[{i}].Spend:   M2 ≡ M3")
            t |> equal(m2[i].OrderId, m3[i].OrderId, "row[{i}].OrderId: M2 ≡ M3")
        }
    }
}

[test]
def parity_right_filter_join(t : T?) {
    // Tuple shape: (UserName, OrderTotal, OrderId) → tuple<string;int;int>.
    // Right-side `_where` filters the joined source before the equi-join.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        var m1 <- _sql(db |> select_from(type<User>)
                         |> _join(db |> select_from(type<Order>) |> _where(_.Total > 75),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total, OrderId = o.Id)))
        var m2 <- (db |> select_from(type<User>)
                     |> _join(db |> select_from(type<Order>) |> _where(_.Total > 75),
                              $(u : User, o : Order) => u.Id == o.UserId,
                              $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total, OrderId = o.Id)))
        var m3 <- (users |> _join(orders |> _where(_.Total > 75),
                                  $(u : User, o : Order) => u.Id == o.UserId,
                                  $(u : User, o : Order) => (UserName = u.Name, OrderTotal = o.Total, OrderId = o.Id)))

        m1 <- sort_by_pair_with_orderid(m1)
        m2 <- sort_by_pair_with_orderid(m2)
        m3 <- sort_by_pair_with_orderid(m3)

        t |> equal(length(m1), 2, "right-filter join row count: 2 (Bob's 50 dropped)")
        t |> equal(length(m1), length(m2), "right-filter: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "right-filter: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i].UserName,   m2[i].UserName,   "row[{i}].UserName:   M1 ≡ M2")
            t |> equal(m1[i].OrderTotal, m2[i].OrderTotal, "row[{i}].OrderTotal: M1 ≡ M2")
            t |> equal(m1[i].OrderId,    m2[i].OrderId,    "row[{i}].OrderId:    M1 ≡ M2")
            t |> equal(m2[i].UserName,   m3[i].UserName,   "row[{i}].UserName:   M2 ≡ M3")
            t |> equal(m2[i].OrderTotal, m3[i].OrderTotal, "row[{i}].OrderTotal: M2 ≡ M3")
            t |> equal(m2[i].OrderId,    m3[i].OrderId,    "row[{i}].OrderId:    M2 ≡ M3")
        }
    }
}

[test]
def parity_join_then_count(t : T?) {
    // v2.1 review-3 regression for E1 (no-snapshot path): bare `_join(...) |> count()`
    // (no _where, no snapshot). Pre-fix: process_join_call's analyze_projection
    // unconditionally wrote q.proj=NamedTuple and clobbered the aggregateExpr=COUNT(*)
    // set by the upstream count() peel; build emitted `SELECT t0."Name", t1."Total" FROM
    // Users JOIN Orders LIMIT 1` and returned a tuple instead of an int. Post-fix:
    // process_join_call saves/restores aggregate state across analyze_projection.
    //
    // Inner `into` tuple: (Pay:int, Holder:string, Premium:bool) → tuple<int;string;bool>
    // — structurally distinct from every other test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let n1 = _sql(db |> select_from(type<User>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (Pay = o.Total, Holder = u.Name, Premium = u.Active))
                        |> count())
        let n2 = (db |> select_from(type<User>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User, o : Order) => u.Id == o.UserId,
                             $(u : User, o : Order) => (Pay = o.Total, Holder = u.Name, Premium = u.Active))
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (Pay = o.Total, Holder = u.Name, Premium = u.Active))
                        |> count())

        // 3 INNER-join rows (Alice ×2, Bob ×1; Carol absent).
        t |> equal(n1, 3, "_join | count(): 3 INNER-join rows")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}

[test]
def parity_join_outer_where_then_count(t : T?) {
    // v2.1 review-3 regression for E2 (snapshot path): `_join(...) |> _where(_.alias) |>
    // count()` exercises the WHERE-peel snapshot. Pre-fix: snapshot reset dropped
    // aggregateExpr/aggregateType, OUTER rendered projection columns instead of COUNT(*)
    // and returned a tuple. Post-fix: snapshot saves/clears/restores aggregate state and
    // post-restore overrides q.proj=Aggregate.
    //
    // Inner `into` tuple: (Live:bool, Pay:int, Tag:string) → tuple<bool;int;string>
    // — structurally distinct from every other test in this file.
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let n1 = _sql(db |> select_from(type<User>)
                        |> _join(db |> select_from(type<Order>),
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (Live = u.Active, Pay = o.Total, Tag = u.Name))
                        |> _where(_.Pay > 75)
                        |> count())
        let n2 = (db |> select_from(type<User>)
                    |> _join(db |> select_from(type<Order>),
                             $(u : User, o : Order) => u.Id == o.UserId,
                             $(u : User, o : Order) => (Live = u.Active, Pay = o.Total, Tag = u.Name))
                    |> _where(_.Pay > 75)
                    |> count())
        let n3 = (users |> _join(orders,
                                 $(u : User, o : Order) => u.Id == o.UserId,
                                 $(u : User, o : Order) => (Live = u.Active, Pay = o.Total, Tag = u.Name))
                        |> _where(_.Pay > 75)
                        |> count())

        // Alice's 100 + 250 pass `Pay > 75`; Bob's 50 dropped → 2 rows.
        t |> equal(n1, 2, "_join | _where(Pay>75) | count(): 2 rows (Alice ×2)")
        t |> equal(n1, n2, "M1 ≡ M2 count")
        t |> equal(n2, n3, "M2 ≡ M3 count")
    }
}
