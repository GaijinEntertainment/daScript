options gen2

require dastest/testing_boost public
require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require daslib/sql_linq
require daslib/option
require daslib/cmp
require daslib/fts5_query

// Parity check for tutorial 17 — subqueries.
//
// Shapes (each must produce identical row sets across the three
// modes): 1. `_in(subquery)`            — IN 2. `_not_in(subquery)`
// — NOT IN 3. `_any(predicate)`          — EXISTS WHERE 4.
// `_none(predicate)`         — NOT EXISTS WHERE 5.
// `_in(captured_array)`      — IN (SELECT value FROM json_each(?)) 6.
// `_not_in(captured_array)`  — NOT IN (SELECT value FROM
// json_each(?))
//
// The 1-arg form `subquery._any()` (test if non-empty, lifts to plain
// `EXISTS (...)` under `_sql`) is Mode-1-only. The `_any` linq_boost
// call_macro requires a predicate; the 1-arg "is non-empty" semantics
// in plain linq are spelled `length(...) > 0` or `any(...)` (function
// form), not `_any(...)`. No parity to check.
//
// All four covered shapes nest a `_`-binding macro inside an outer
// `_where`'s `_`-binding macro — they regression-test the
// `can_shadow` synthesis on linq_boost's predicate-lambda parameter.

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
    return <- [
        User(Id = 1, Name = "Alice", Active = true),
        User(Id = 2, Name = "Bob",   Active = true),
        User(Id = 3, Name = "Carol", Active = false)
    ]
}

def fixture_orders() : array<Order> {
    return <- [
        Order(Id = 10, UserId = 1, Total = 100),
        Order(Id = 11, UserId = 1, Total = 250),
        Order(Id = 12, UserId = 2, Total =  50)
    ]
}

def users_equal(a : array<User>; b : array<User>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea.Id != eb.Id || ea.Name != eb.Name || ea.Active != eb.Active) {
            return false
        }
    }
    return true
}

[test(description = "an in subquery gives identical rows in all three modes")]
def parity_in_subquery(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m2 <- (db |> select_from(type<User>)
                     |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m3 <- (users |> _where(_.Id._in(orders |> _select(_.UserId))))

        t |> equal(length(m1), 2, "_in: 2 users with orders (Carol absent)")
        t |> success(users_equal(m1, m2), "_in: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_in: M2 ≡ M3")
    }
}

[test(description = "a not_in subquery gives identical rows in all three modes")]
def parity_not_in_subquery(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m2 <- (db |> select_from(type<User>)
                     |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
        let m3 <- (users |> _where(_.Id._not_in(orders |> _select(_.UserId))))

        t |> equal(length(m1), 1, "_not_in: 1 orderless user (Carol)")
        t |> success(users_equal(m1, m2), "_not_in: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_not_in: M2 ≡ M3")
    }
}

[test(description = "an any predicate gives identical rows in all three modes")]
def parity_any_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        // Uncorrelated EXISTS: when ANY order has Total > 200, every
        // user surfaces; when none do, none surface. Predicate
        // references only the subquery's own row through `_`.
        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        let m2 <- (db |> select_from(type<User>)
                     |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        let m3 <- (users |> _where((orders)._any(_.Total > 200)))

        t |> equal(length(m1), 3, "_any(>200): all 3 users surface")
        t |> success(users_equal(m1, m2), "_any: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_any: M2 ≡ M3")
    }
}

[test(description = "a none predicate gives identical rows in all three modes")]
def parity_none_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users  <- fixture_users()
        var orders <- fixture_orders()

        // Uncorrelated NOT EXISTS: when no order has Total > 1000, all
        // users surface.
        let m1 <- _sql(db |> select_from(type<User>)
                         |> _where((db |> select_from(type<Order>))._none(_.Total > 1000)))
        let m2 <- (db |> select_from(type<User>)
                     |> _where((db |> select_from(type<Order>))._none(_.Total > 1000)))
        let m3 <- (users |> _where((orders)._none(_.Total > 1000)))

        t |> equal(length(m1), 3, "_none(>1000): all 3 users surface")
        t |> success(users_equal(m1, m2), "_none: M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_none: M2 ≡ M3")
    }
}

[test(description = "in over a runtime collection gives identical rows in all three modes")]
def parity_in_collection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users <- fixture_users()
        let want_ids <- [1, 2]

        let m1 <- _sql(db |> select_from(type<User>) |> _where(_.Id._in(want_ids)))
        let m2 <- (db |> select_from(type<User>) |> _where(_.Id._in(want_ids)))
        let m3 <- (users |> _where(_.Id._in(want_ids)))

        t |> equal(length(m1), 2, "_in(arr): Alice + Bob match")
        t |> success(users_equal(m1, m2), "_in(arr): M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_in(arr): M2 ≡ M3")
    }
}

[test(description = "not_in over a runtime collection gives identical rows in all three modes")]
def parity_not_in_collection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var users <- fixture_users()
        let want_ids <- [1, 2]

        // Id is non-nullable PK → naive NOT IN form, identical across all
        // three modes.
        let m1 <- _sql(db |> select_from(type<User>) |> _where(_.Id._not_in(want_ids)))
        let m2 <- (db |> select_from(type<User>) |> _where(_.Id._not_in(want_ids)))
        let m3 <- (users |> _where(_.Id._not_in(want_ids)))

        t |> equal(length(m1), 1, "_not_in(arr): only Carol survives")
        t |> success(users_equal(m1, m2), "_not_in(arr): M1 ≡ M2")
        t |> success(users_equal(m2, m3), "_not_in(arr): M2 ≡ M3")
    }
}

// Parity check for tutorial 18 — NULL handling via `Option<T>`.
//
// Shapes:
//   1. Round-trip: full Option<T> fields survive Mode-1/2/3 paths
//   2. `_where(_.Col |> is_some)`     → "Col" IS NOT NULL
//   3. `_where(_.Col |> is_none)`     → "Col" IS NULL
// 4. `_where(_.Col |> unwrap_or(d) >= n)`  → COALESCE("Col", ?) >= ?

[sql_table(name = "Users")]
struct User_18 {
    @sql_primary_key Id : int
    Name      : string
    @safe_when_uninitialized Age       : Option<int>
    @safe_when_uninitialized Nick      : Option<string>
    @safe_when_uninitialized DeletedAt : Option<int64>
}

def fixture_db_18(db : SqlRunner) {
    db |> create_table(type<User_18>)
    db |> insert(
                 User_18(Id = 1, Name = "alice", Age = some(30), Nick = none(type<string>),
                         DeletedAt = none(type<int64>)))
    db |> insert(
                 User_18(Id = 2, Name = "bob",   Age = none(type<int>), Nick = some("bobby"),
                         DeletedAt = none(type<int64>)))
    db |> insert(
                 User_18(Id = 3, Name = "carol", Age = some(40), Nick = some("c"),
                         DeletedAt = some(1000l)))
}

def fixture_array() : array<User_18> {
    return <- [
        User_18(Id = 1, Name = "alice", Age = some(30), Nick = none(type<string>),
                DeletedAt = none(type<int64>)),
        User_18(Id = 2, Name = "bob",   Age = none(type<int>), Nick = some("bobby"),
                DeletedAt = none(type<int64>)),
        User_18(Id = 3, Name = "carol", Age = some(40), Nick = some("c"), DeletedAt = some(1000l))
    ]
}

def opt_int_eq(a : Option<int>; b : Option<int>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return a |> unwrap == b |> unwrap
}

def opt_str_eq(a : Option<string>; b : Option<string>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return a |> unwrap == b |> unwrap
}

def opt_i64_eq(a : Option<int64>; b : Option<int64>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return a |> unwrap == b |> unwrap
}

def user_eq(a : User_18; b : User_18) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && opt_int_eq(a.Age, b.Age)
        && opt_str_eq(a.Nick, b.Nick)
        && opt_i64_eq(a.DeletedAt, b.DeletedAt))
}

def users_eq(a : array<User_18>; b : array<User_18>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!user_eq(ea, eb)) {
            return false
        }
    }
    return true
}

[test(description = "option fields round-trip with identical some and none values in all three modes")]
def parity_round_trip(t : T?) {
    // Full row materialization across modes — Option<T> fields survive
    // SQL prep/step + adapter-rail decode in Mode 1/2; trivially in Mode
    // 3.
    with_sqlite(":memory:") $(db) {
        fixture_db_18(db)
        let arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User_18>) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User_18>) |> _order_by(_.Id))
        let m3 <- (arr |> _order_by(_.Id))

        t |> success(users_eq(m1, m2), "round-trip: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "round-trip: M2 ≡ M3")
    }
}

[test(description = "an is_some predicate gives identical rows in all three modes")]
def parity_where_is_some(t : T?) {
    // _where(_.Age |> is_some) — keeps alice and carol; drops bob.
    with_sqlite(":memory:") $(db) {
        fixture_db_18(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User_18>)
            |> _where(_.Age |> is_some) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User_18>) |> _where(_.Age |> is_some) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.Age |> is_some) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "is_some: 2 rows (alice, carol)")
        t |> success(users_eq(m1, m2), "is_some: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "is_some: M2 ≡ M3")
    }
}

[test(description = "an is_none predicate gives identical rows in all three modes")]
def parity_where_is_none(t : T?) {
    // _where(_.DeletedAt |> is_none) — keeps alive users (alice, bob);
    // drops carol.
    with_sqlite(":memory:") $(db) {
        fixture_db_18(db)
        var arr <- fixture_array()

        let m1 <- _sql(db |> select_from(type<User_18>)
            |> _where(_.DeletedAt |> is_none) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User_18>)
            |> _where(_.DeletedAt |> is_none) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.DeletedAt |> is_none) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "is_none: 2 rows (alice, bob)")
        t |> success(users_eq(m1, m2), "is_none: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "is_none: M2 ≡ M3")
    }
}

[test(description = "an unwrap_or predicate gives identical rows in all three modes")]
def parity_where_unwrap_or(t : T?) {
    // _where(_.Age |> unwrap_or(0) >= 18) — alice (30) and carol (40)
    // pass; bob (none → 0) fails. Mode 1: SQL `COALESCE("Age", ?) >= ?`.
    with_sqlite(":memory:") $(db) {
        fixture_db_18(db)
        var arr <- fixture_array()
        let cutoff = 18

        let m1 <- _sql(db |> select_from(type<User_18>) |> _where(_.Age
            |> unwrap_or(0) >= cutoff) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<User_18>) |> _where(_.Age
            |> unwrap_or(0) >= cutoff) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.Age |> unwrap_or(0) >= cutoff) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "unwrap_or: 2 rows (alice, carol)")
        t |> success(users_eq(m1, m2), "unwrap_or: M1 ≡ M2")
        t |> success(users_eq(m2, m3), "unwrap_or: M2 ≡ M3")
    }
}

// Parity check for tutorial 23 — foreign keys.
//
// FK cascade / set_null are DDL/runtime SQLite behavior with no
// plain-linq analog (plain arrays have no FK concept). This file
// verifies only the CHAIN SHAPES introduced by the tutorial: -
// `_where(...) |> count()`     count after filter - `_to_array()`
// terminal that materializes the chain
//
// Both ride on chains already covered in earlier tutorials but with
// the specific shape tutorial 23 demonstrates. Fixture is
// intentionally FK-free — the parity claim is about the chain
// emission, not the DDL-driven cascade behavior.

[sql_table(name = "Orders")]
struct Order_23 {
    @sql_primary_key Id : int
    UserId : int
    Total  : float
}

[sql_table(name = "Comments")]
struct Comment {
    @sql_primary_key Id : int
    @safe_when_uninitialized AuthorId : Option<int>
    Body : string
}

def fixture_db_23(db : SqlRunner) {
    db |> create_table(type<Order_23>)
    db |> create_table(type<Comment>)
    db |> insert(Order_23(Id = 100, UserId = 1, Total = 42.0f))
    db |> insert(Order_23(Id = 101, UserId = 1, Total = 99.0f))
    db |> insert(Order_23(Id = 102, UserId = 2, Total =  7.5f))
    db |> insert(Comment(Id = 1000, AuthorId = none(type<int>), Body = "alice's"))
    db |> insert(Comment(Id = 1001, AuthorId = none(type<int>), Body = "bob's"))
}

def fixture_orders_23() : array<Order_23> {
    return <- [
        Order_23(Id = 100, UserId = 1, Total = 42.0f),
        Order_23(Id = 101, UserId = 1, Total = 99.0f),
        Order_23(Id = 102, UserId = 2, Total =  7.5f)
    ]
}

def fixture_comments() : array<Comment> {
    return <- [
        Comment(Id = 1000, AuthorId = none(type<int>), Body = "alice's"),
        Comment(Id = 1001, AuthorId = none(type<int>), Body = "bob's")
    ]
}

def comment_eq(a : Comment; b : Comment) : bool {
    if (a.Id != b.Id || a.Body != b.Body || (a.AuthorId |> is_some) != (b.AuthorId |> is_some)) {
        return false
    }
    if (a.AuthorId |> is_none) {
        return true
    }
    return a.AuthorId |> unwrap == b.AuthorId |> unwrap
}

def comments_eq(a : array<Comment>; b : array<Comment>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!comment_eq(ea, eb)) {
            return false
        }
    }
    return true
}

[test(description = "where then count over a foreign-key child table gives the same number in all three modes")]
def parity_where_count(t : T?) {
    // _where(_.UserId == n) |> count() — alice has 2 orders.
    with_sqlite(":memory:") $(db) {
        fixture_db_23(db)
        var arr <- fixture_orders_23()

        let m1 = _sql(db |> select_from(type<Order_23>) |> _where(_.UserId == 1) |> count())
        let m2 = db |> select_from(type<Order_23>) |> _where(_.UserId == 1) |> count()
        let m3 = arr |> _where(_.UserId == 1) |> count()

        t |> equal(m1, 2,  "where|count: 2 rows for UserId=1")
        t |> equal(m1, m2, "where|count: M1 ≡ M2")
        t |> equal(m2, m3, "where|count: M2 ≡ M3")
    }
}

[test(description = "to_array over a foreign-key child table gives identical rows in all three modes")]
def parity_to_array_terminal(t : T?) {
    // `_to_array()` is the explicit materialization terminal. Result
    // type is the same as the default-no-terminal form (array<T>).
    with_sqlite(":memory:") $(db) {
        fixture_db_23(db)
        var arr <- fixture_comments()

        let m1 <- _sql(db |> select_from(type<Comment>) |> _to_array())
        let m2 <- (db |> select_from(type<Comment>) |> _to_array())
        let m3 <- (arr |> _to_array())

        t |> equal(length(m1), 2, "to_array: 2 rows")
        t |> success(comments_eq(m1, m2), "to_array: M1 ≡ M2")
        t |> success(comments_eq(m2, m3), "to_array: M2 ≡ M3")
    }
}

// Parity check for tutorial 24 — indexes.
//
// Indexes are transparent to query semantics — the SQLite planner
// uses them automatically; no chain shape changes. The tutorial's
// first chain (`_where(_.Email == s) |> _first()`) is covered by tut
// 05. The new shape here is `_where + _order_by_descending +
// _to_array()` over an int64 column. The DDL side (`[sql_index]`
// annotation, `_sql_create_indexes_sql`) and unique-constraint
// write-path are out of scope.

[sql_table(name = "Users"), sql_index(fields = "Email", unique = true), sql_index(fields = ("City", "LastSeen")), sql_index(fields = "LastSeen", name = "ix_users_lastseen")]

struct User_24 {
    @sql_primary_key Id : int
    Email    : string
    Name     : string
    City     : string
    LastSeen : int64
}

def fixture_db_24(db : SqlRunner) {
    db |> create_table(type<User_24>)
    db |> insert(
                 User_24(Id = 1, Email = "alice@x.com", Name = "alice", City = "SF",
                         LastSeen = 100l))
    db |> insert(
                 User_24(Id = 2, Email = "bob@x.com",   Name = "bob",   City = "SF",
                         LastSeen = 200l))
    db |> insert(
                 User_24(Id = 3, Email = "carol@x.com", Name = "carol", City = "NYC",
                         LastSeen = 300l))
}

def fixture_array_24() : array<User_24> {
    return <- [
        User_24(Id = 1, Email = "alice@x.com", Name = "alice", City = "SF",  LastSeen = 100l),
        User_24(Id = 2, Email = "bob@x.com",   Name = "bob",   City = "SF",  LastSeen = 200l),
        User_24(Id = 3, Email = "carol@x.com", Name = "carol", City = "NYC", LastSeen = 300l)
    ]
}

def user_eq_24(a : User_24; b : User_24) : bool {
    return (a.Id == b.Id
        && a.Email == b.Email
        && a.Name == b.Name
        && a.City == b.City
        && a.LastSeen == b.LastSeen)
}

def users_eq_24(a : array<User_24>; b : array<User_24>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!user_eq_24(ea, eb)) {
            return false
        }
    }
    return true
}

[test(description = "where, order_by_descending and to_array over an indexed int64 column give identical rows in all three modes")]
def parity_where_order_desc_to_array(t : T?) {
    // _where(_.City == "SF") narrows to alice + bob;
    // _order_by_descending(_.LastSeen) sorts so bob (200) precedes alice
    // (100). int64 column.
    with_sqlite(":memory:") $(db) {
        fixture_db_24(db)
        var arr <- fixture_array_24()

        let m1 <- _sql(db |> select_from(type<User_24>)
                         |> _where(_.City == "SF")
                         |> _order_by_descending(_.LastSeen)
                         |> _to_array())
        let m2 <- (db |> select_from(type<User_24>)
                     |> _where(_.City == "SF")
                     |> _order_by_descending(_.LastSeen)
                     |> _to_array())
        let m3 <- (arr |> _where(_.City == "SF")
                       |> _order_by_descending(_.LastSeen)
                       |> _to_array())

        t |> equal(length(m1), 2, "where|order_desc|to_array: 2 SF users")
        t |> success(users_eq_24(m1, m2), "where|order_desc|to_array: M1 ≡ M2")
        t |> success(users_eq_24(m2, m3), "where|order_desc|to_array: M2 ≡ M3")
        // Sanity on order: bob (LastSeen=200) should precede alice (100).
        t |> equal(m1[0].Name, "bob",   "first row: bob (LastSeen=200)")
        t |> equal(m1[1].Name, "alice", "second row: alice (LastSeen=100)")
    }
}

// Parity check for tutorial 26 — custom types via sql_bind /
// sql_extract.
//
// Chain shapes:
//   - `_where(_.Status == OrderStatus.Paid)`   filter on enum field
//   - full-row read of struct with Option<custom> field
//
// The adapter rail (sql_bind / sql_extract) is invisible to Mode 3 —
// plain arrays don't go through SQL, so the custom types just live as
// structs. Parity should hold because the round-trip preserves data.

struct DateTime {
    unix_seconds : int64
}

def sql_bind(dt : DateTime) : int64 => dt.unix_seconds

[unused_argument(t)]
def sql_extract(v : int64; t : type<DateTime>) : DateTime => DateTime(unix_seconds = v)

struct Guid {
    @safe_when_uninitialized bytes : array<uint8>
}

def sql_bind(g : Guid) : array<uint8> {
    var copy : array<uint8>
    copy := g.bytes
    return <- copy
}

[unused_argument(t)]
def sql_extract(var v : array<uint8>; t : type<Guid>) : Guid => Guid(bytes <- v)

enum OrderStatus {
    Pending
    Paid
    Shipped
    Cancelled
}

[sql_table(name = "Orders")]
struct Order_26 {
    @sql_primary_key Id : int
    ExternalId : Guid
    PlacedAt   : DateTime
    Status     : OrderStatus
    Total      : float
}

[sql_table(name = "Events")]
struct Event {
    @sql_primary_key Id : int
    At : DateTime
    @safe_when_uninitialized StartsAt : Option<DateTime>
}

def sample_bytes() : array<uint8> => <- [for (_ in range(16)); uint8(0xAB)]

def fixture_orders_db(db : SqlRunner) {
    db |> create_table(type<Order_26>)
    var b1 <- sample_bytes()
    var b2 <- sample_bytes()
    var b3 <- sample_bytes()
    db |> insert(
                 Order_26(Id = 1, ExternalId = Guid(bytes <- b1),
                          PlacedAt = DateTime(unix_seconds = 100l), Status = OrderStatus.Pending,
        Total = 10.0))
    db |> insert(
                 Order_26(Id = 2, ExternalId = Guid(bytes <- b2),
                          PlacedAt = DateTime(unix_seconds = 200l), Status = OrderStatus.Paid,
        Total = 20.0))
    db |> insert(
                 Order_26(Id = 3, ExternalId = Guid(bytes <- b3),
                          PlacedAt = DateTime(unix_seconds = 300l), Status = OrderStatus.Paid,
        Total = 30.0))
}

def fixture_orders_array() : array<Order_26> {
    var arr : array<Order_26>
    var b1 <- sample_bytes()
    var b2 <- sample_bytes()
    var b3 <- sample_bytes()
    arr |> emplace(
                   Order_26(Id = 1, ExternalId = Guid(bytes <- b1),
                            PlacedAt = DateTime(unix_seconds = 100l), Status = OrderStatus.Pending,
        Total = 10.0))
    arr |> emplace(
                   Order_26(Id = 2, ExternalId = Guid(bytes <- b2),
                            PlacedAt = DateTime(unix_seconds = 200l), Status = OrderStatus.Paid,
        Total = 20.0))
    arr |> emplace(
                   Order_26(Id = 3, ExternalId = Guid(bytes <- b3),
                            PlacedAt = DateTime(unix_seconds = 300l), Status = OrderStatus.Paid,
        Total = 30.0))
    return <- arr
}

def fixture_events_db(db : SqlRunner) {
    db |> create_table(type<Event>)
    db |> insert(Event(Id = 1, At = DateTime(unix_seconds = 0l), StartsAt = none(type<DateTime>)))
    db |> insert(
                 Event(Id = 2, At = DateTime(unix_seconds = 0l),
                       StartsAt = some(DateTime(unix_seconds = 1713000000l))))
}

def fixture_events_array() : array<Event> {
    return <- [
        Event(Id = 1, At = DateTime(unix_seconds = 0l), StartsAt = none(type<DateTime>)),
        Event(Id = 2, At = DateTime(unix_seconds = 0l),
              StartsAt = some(DateTime(unix_seconds = 1713000000l)))
    ]
}

def bytes_eq(a : array<uint8>; b : array<uint8>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea != eb) {
            return false
        }
    }
    return true
}

def order_eq(a : Order_26; b : Order_26) : bool {
    return (a.Id == b.Id
        && bytes_eq(a.ExternalId.bytes, b.ExternalId.bytes)
        && a.PlacedAt.unix_seconds == b.PlacedAt.unix_seconds
        && a.Status == b.Status
        && a.Total == b.Total)
}

def orders_eq(a : array<Order_26>; b : array<Order_26>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!order_eq(ea, eb)) {
            return false
        }
    }
    return true
}

def opt_dt_eq(a : Option<DateTime>; b : Option<DateTime>) : bool {
    if ((a |> is_some) != (b |> is_some)) {
        return false
    }
    if (a |> is_none) {
        return true
    }
    return unwrap(a).unix_seconds == unwrap(b).unix_seconds
}

def event_eq(a : Event; b : Event) : bool {
    return (a.Id == b.Id
        && a.At.unix_seconds == b.At.unix_seconds
        && opt_dt_eq(a.StartsAt, b.StartsAt))
}

def events_eq(a : array<Event>; b : array<Event>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!event_eq(ea, eb)) {
            return false
        }
    }
    return true
}

[test(description = "a where on an enum field gives identical rows in all three modes")]
def parity_where_enum_filter(t : T?) {
    // Filter by enum field. SQL emits "Status" = ? with the enum lowered
    // to int64.
    with_sqlite(":memory:") $(db) {
        fixture_orders_db(db)
        var arr <- fixture_orders_array()

        let m1 <- _sql(db |> select_from(type<Order_26>)
            |> _where(_.Status == OrderStatus.Paid) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<Order_26>)
            |> _where(_.Status == OrderStatus.Paid) |> _order_by(_.Id))
        let m3 <- (arr |> _where(_.Status == OrderStatus.Paid) |> _order_by(_.Id))

        t |> equal(length(m1), 2, "enum filter: 2 paid orders")
        t |> success(orders_eq(m1, m2), "enum filter: M1 ≡ M2")
        t |> success(orders_eq(m2, m3), "enum filter: M2 ≡ M3")
    }
}

[test(description = "an option of a custom type reads identically in all three modes")]
def parity_event_option_custom_type(t : T?) {
    // Full-row read of Event whose StartsAt is Option<DateTime> (a custom
    // type).
    with_sqlite(":memory:") $(db) {
        fixture_events_db(db)
        var arr <- fixture_events_array()

        let m1 <- _sql(db |> select_from(type<Event>) |> _order_by(_.Id))
        let m2 <- (db |> select_from(type<Event>) |> _order_by(_.Id))
        let m3 <- (arr |> _order_by(_.Id))

        t |> equal(length(m1), 2, "events: 2 rows")
        t |> success(events_eq(m1, m2), "Option<DateTime>: M1 ≡ M2")
        t |> success(events_eq(m2, m3), "Option<DateTime>: M2 ≡ M3")
    }
}

// Parity check for tutorial 28 — `@sql_json` columns and `_sql`
// walker descent into JSON paths.
//
// Chain shapes (NEW vs earlier tutorials):
// - `_where(_.Prefs.theme == "...")`             field descent in
// WHERE - `_select(_.Prefs.theme)`                     field descent
// in SELECT - `_select((Name = ..., Theme = _.Prefs.theme))` mixed
// projection - `_where(_.Prefs.notify == true) |> count()`  JSON
// predicate + count - `_where(_.Profile.Addr.City == "...")`
// nested struct path
//
// In Mode 1 these lower to `json_extract("Col", '$.path')`. In Modes
// 2/3 the field access is plain daslang struct/tuple access on the
// materialized values. Parity hinges on the SQL JSON round-trip
// preserving the values exactly (and on the nested path producing the
// same field as direct access).
//
// `@sql_blob` is opaque to `_sql` (compile error to descend); no
// chain to verify. Out of scope.

[sql_table(name = "Users")]
struct User_28 {
    @sql_primary_key Id : int
    Name  : string
    @sql_json Prefs : tuple<theme : string; lang : string; notify : bool>
}

def fixture_users_db(db : SqlRunner) {
    db |> create_table(type<User_28>)
    db |> insert(
                 User_28(Id = 1, Name = "alice",
                         Prefs = (theme = "dark",  lang = "en", notify = true)))
    db |> insert(
                 User_28(Id = 2, Name = "bob",
                         Prefs = (theme = "light", lang = "ru", notify = false)))
    db |> insert(
                 User_28(Id = 3, Name = "carol",
                         Prefs = (theme = "dark",  lang = "fr", notify = false)))
}

def fixture_users_array() : array<User_28> {
    return <- [
        User_28(Id = 1, Name = "alice", Prefs = (theme = "dark",  lang = "en", notify = true)),
        User_28(Id = 2, Name = "bob",   Prefs = (theme = "light", lang = "ru", notify = false)),
        User_28(Id = 3, Name = "carol", Prefs = (theme = "dark",  lang = "fr", notify = false))
    ]
}

def user_eq_28(a : User_28; b : User_28) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && a.Prefs.theme == b.Prefs.theme
        && a.Prefs.lang == b.Prefs.lang
        && a.Prefs.notify == b.Prefs.notify)
}

def users_eq_28(a : array<User_28>; b : array<User_28>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!user_eq_28(ea, eb)) {
            return false
        }
    }
    return true
}

def sort_by_id(var users : array<User_28>) {
    users |> sort() <| cmp_by(_.Id)
}

[test(description = "a where on a sql_json field gives identical rows in all three modes")]
def parity_json_where_descend(t : T?) {
    // _where(_.Prefs.theme == "dark") — keeps alice + carol.
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        var m1 <- _sql(db |> select_from(type<User_28>) |> _where(_.Prefs.theme == "dark"))
        var m2 <- (db |> select_from(type<User_28>) |> _where(_.Prefs.theme == "dark"))
        var m3 <- (arr |> _where(_.Prefs.theme == "dark"))

        sort_by_id(m1)
        sort_by_id(m2)
        sort_by_id(m3)

        t |> equal(length(m1), 2, "json where: 2 dark users")
        t |> success(users_eq_28(m1, m2), "json where: M1 ≡ M2")
        t |> success(users_eq_28(m2, m3), "json where: M2 ≡ M3")
    }
}

[test(description = "selecting a sql_json field gives identical values in all three modes")]
def parity_json_select_scalar(t : T?) {
    // _select(_.Prefs.theme) — projects the json-path leaf as
    // array<string>.
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        var m1 <- _sql(db |> select_from(type<User_28>) |> _select(_.Prefs.theme))
        var m2 <- (db |> select_from(type<User_28>) |> _select(_.Prefs.theme))
        var m3 <- (arr |> _select(_.Prefs.theme))

        m1 |> sort()
        m2 |> sort()
        m3 |> sort()

        t |> equal(length(m1), 3, "json select scalar: 3 themes")
        t |> equal(length(m1), length(m2), "json select scalar: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "json select scalar: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i], m2[i], "theme[{i}]: M1 ≡ M2")
            t |> equal(m2[i], m3[i], "theme[{i}]: M2 ≡ M3")
        }
    }
}

[test(description = "a named-tuple select mixing plain and sql_json fields gives identical rows in all three modes")]
def parity_json_select_named_tuple(t : T?) {
    // _select((Name = _.Name, Theme = _.Prefs.theme, Notify =
    // _.Prefs.notify)) Tuple shape (string, string, bool) — distinct from
    // earlier tests.
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        var m1 <- _sql(db |> select_from(type<User_28>)
                         |> _select(
                                    (Name = _.Name, Theme = _.Prefs.theme,
                                     Notify = _.Prefs.notify)))
        var m2 <- (db |> select_from(type<User_28>)
                     |> _select((Name = _.Name, Theme = _.Prefs.theme, Notify = _.Prefs.notify)))
        var m3 <- (arr |> _select((Name = _.Name, Theme = _.Prefs.theme, Notify = _.Prefs.notify)))

        m1 |> sort() <| cmp_by(_.Name)
        m2 |> sort() <| cmp_by(_.Name)
        m3 |> sort() <| cmp_by(_.Name)

        t |> equal(length(m1), 3, "json named-tuple: 3 rows")
        for (i in range(length(m1))) {
            t |> equal(m1[i].Name,   m2[i].Name,   "row[{i}].Name:   M1 ≡ M2")
            t |> equal(m1[i].Theme,  m2[i].Theme,  "row[{i}].Theme:  M1 ≡ M2")
            t |> equal(m1[i].Notify, m2[i].Notify, "row[{i}].Notify: M1 ≡ M2")
            t |> equal(m2[i].Name,   m3[i].Name,   "row[{i}].Name:   M2 ≡ M3")
            t |> equal(m2[i].Theme,  m3[i].Theme,  "row[{i}].Theme:  M2 ≡ M3")
            t |> equal(m2[i].Notify, m3[i].Notify, "row[{i}].Notify: M2 ≡ M3")
        }
    }
}

[test(description = "a sql_json predicate with count gives the same number in all three modes")]
def parity_json_where_count(t : T?) {
    // _where(_.Prefs.notify == true) |> count()
    with_sqlite(":memory:") $(db) {
        fixture_users_db(db)
        var arr <- fixture_users_array()

        let m1 = _sql(db |> select_from(type<User_28>) |> _where(_.Prefs.notify == true) |> count())
        let m2 = db |> select_from(type<User_28>) |> _where(_.Prefs.notify) |> count()
        let m3 = arr |> _where(_.Prefs.notify) |> count()

        t |> equal(m1, 1,  "json where|count: 1 notifiable user (alice)")
        t |> equal(m1, m2, "json where|count: M1 ≡ M2")
        t |> equal(m2, m3, "json where|count: M2 ≡ M3")
    }
}

// --- Nested struct paths ---

struct Address {
    City : string
    Zip  : string
}

struct Profile {
    Addr : Address
    Bio  : string
}

[sql_table(name = "Accounts")]
struct Account {
    @sql_primary_key Id : int
    @sql_json Profile : Profile
}

def account_eq(a : Account; b : Account) : bool {
    return (a.Id == b.Id
        && a.Profile.Addr.City == b.Profile.Addr.City
        && a.Profile.Addr.Zip == b.Profile.Addr.Zip
        && a.Profile.Bio == b.Profile.Bio)
}

def accounts_eq(a : array<Account>; b : array<Account>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!account_eq(ea, eb)) {
            return false
        }
    }
    return true
}

def fixture_accounts_db(db : SqlRunner) {
    db |> create_table(type<Account>)
    db |> insert(
                 Account(Id = 1,
                         Profile = Profile(Addr = Address(City = "SF", Zip = "94000"),
                                           Bio = "engineer")))
    db |> insert(
                 Account(Id = 2,
                         Profile = Profile(Addr = Address(City = "NYC", Zip = "10000"),
                                           Bio = "writer")))
    db |> insert(
                 Account(Id = 3,
                         Profile = Profile(Addr = Address(City = "SF", Zip = "94100"),
                                           Bio = "designer")))
}

def fixture_accounts_array() : array<Account> {
    return <- [
        Account(Id = 1,
                Profile = Profile(Addr = Address(City = "SF", Zip = "94000"), Bio = "engineer")),
        Account(Id = 2,
                Profile = Profile(Addr = Address(City = "NYC", Zip = "10000"), Bio = "writer")),
        Account(Id = 3,
                Profile = Profile(Addr = Address(City = "SF", Zip = "94100"), Bio = "designer"))
    ]
}

def sort_accounts_by_id(var rows : array<Account>) {
    rows |> sort() <| cmp_by(_.Id)
}

[test(description = "a where on a nested sql_json path gives identical rows in all three modes")]
def parity_json_nested_path(t : T?) {
    // _where(_.Profile.Addr.City == "SF") — 2-level nested path through
    // a JSON-stored struct. SQL emits json_extract("Profile",
    // '$.Addr.City').
    with_sqlite(":memory:") $(db) {
        fixture_accounts_db(db)
        var arr <- fixture_accounts_array()

        var m1 <- _sql(db |> select_from(type<Account>) |> _where(_.Profile.Addr.City == "SF"))
        var m2 <- (db |> select_from(type<Account>) |> _where(_.Profile.Addr.City == "SF"))
        var m3 <- (arr |> _where(_.Profile.Addr.City == "SF"))

        sort_accounts_by_id(m1)
        sort_accounts_by_id(m2)
        sort_accounts_by_id(m3)

        t |> equal(length(m1), 2, "nested json path: 2 SF accounts")
        t |> success(accounts_eq(m1, m2), "nested json path: M1 ≡ M2")
        t |> success(accounts_eq(m2, m3), "nested json path: M2 ≡ M3")
    }
}

// Parity check for tutorial 32 — `[sql_function]` user-defined SQL
// scalars.
//
// Chain shape: `_where(score(_.Hp, _.Armor) >= 100.0lf)` — a daslang
// function tagged `[sql_function]` invoked inside the predicate. The
// annotation does two things at once:
//   1. Auto-registers the function as a SQLite scalar at open() time
//   2. Tells the `_sql` analyzer it's safe to lower the call to SQL
//      (instead of falling back to per-row binds)
//
// Mode 1 emits `WHERE score("Hp", "Armor") >= ?` — SQLite calls back
// into the registered trampoline.
// Mode 2/3 just call `score(...)` as a normal daslang function over
// each row. Same function body, different execution path.
//
// Parity hinges on `[sql_function]` being a TRANSPARENT annotation:
// the function works identically from daslang code regardless of the
// SQL-binding side.

[sql_function(deterministic=true)]
def score(hp : float; armor : int) : double => double(hp) * (1.0lf - double(armor) / 100.0lf)

[sql_table(name = "Enemies")]
struct Enemy {
    @sql_primary_key Id : int
    Name : string
    Hp   : float
    Armor : int
}

def fixture_db_32(db : SqlRunner) {
    db |> create_table(type<Enemy>)
    db |> insert(Enemy(Id = 1, Name = "goblin", Hp = 100.0f, Armor = 10))   // score = 90
    db |> insert(Enemy(Id = 2, Name = "ogre",   Hp = 800.0f, Armor = 30))   // score = 560
    db |> insert(Enemy(Id = 3, Name = "dragon", Hp = 5000.0f, Armor = 60))  // score = 2000
}

def fixture_array_32() : array<Enemy> {
    return <- [
        Enemy(Id = 1, Name = "goblin", Hp = 100.0f, Armor = 10),
        Enemy(Id = 2, Name = "ogre",   Hp = 800.0f, Armor = 30),
        Enemy(Id = 3, Name = "dragon", Hp = 5000.0f, Armor = 60)
    ]
}

def enemy_eq(a : Enemy; b : Enemy) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && a.Hp == b.Hp
        && a.Armor == b.Armor)
}

def enemies_eq(a : array<Enemy>; b : array<Enemy>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!enemy_eq(ea, eb)) {
            return false
        }
    }
    return true
}

def sort_by_id_32(var rows : array<Enemy>) {
    rows |> sort() <| cmp_by(_.Id)
}

[test(description = "a sql_function used in a where gives identical rows in all three modes")]
def parity_sql_function_in_where(t : T?) {
    // _where(score(_.Hp, _.Armor) >= 100.0lf) — keeps ogre + dragon
    // (scores 560 and 2000); drops goblin (score 90).
    with_sqlite(":memory:") $(db) {
        fixture_db_32(db)
        var arr <- fixture_array_32()

        var m1 <- _sql(db |> select_from(type<Enemy>) |> _where(score(_.Hp, _.Armor) >= 100.0lf))
        var m2 <- (db |> select_from(type<Enemy>) |> _where(score(_.Hp, _.Armor) >= 100.0lf))
        var m3 <- (arr |> _where(score(_.Hp, _.Armor) >= 100.0lf))

        sort_by_id_32(m1)
        sort_by_id_32(m2)
        sort_by_id_32(m3)

        t |> equal(length(m1), 2, "sql_function where: 2 strong enemies")
        t |> success(enemies_eq(m1, m2), "sql_function where: M1 ≡ M2")
        t |> success(enemies_eq(m2, m3), "sql_function where: M2 ≡ M3")
    }
}

// Parity check for tutorial 35 — `_each_sql` streaming iteration.
//
// Same chain shape as `_sql(...)`, but the terminal is `iterator<T>`
// instead of `array<T>`. The parity claim is that streaming and
// materialization produce IDENTICAL row sequences:
//
//   M1a (materialized) : var rows <- _sql(chain); iterate rows
//   M1b (streamed)     : for (r in _each_sql(chain)) { collect }
//   M2                 : for (r in (db |> chain))   { collect }
//   M3                 : for (r in (arr |> chain))  { collect }
//
// The fourth shape (M3) is plain-array iteration — no SQL, no
// streaming machinery. M1/M2/M3 row order matches the chain's
// `_order_by`. M1a vs M1b is the interesting comparison: does
// `_each_sql` match `_sql` row-for-row at the same chain.

[sql_table(name = "Logs")]
struct LogEntry {
    @sql_primary_key Id : int
    Severity : int
    Message  : string
}

def fixture_db_35(db : SqlRunner) {
    db |> create_table(type<LogEntry>)
    for (i in range(20)) {
        db |> insert(LogEntry(
            Id = i + 1,
            Severity = (i % 4),
            Message = "msg #{i}"))
    }
}

def fixture_array_35() : array<LogEntry> {
    var arr : array<LogEntry>
    arr |> reserve(20)
    for (i in range(20)) {
        arr |> emplace(LogEntry(
            Id = i + 1,
            Severity = (i % 4),
            Message = "msg #{i}"))
    }
    return <- arr
}

def log_eq(a : LogEntry; b : LogEntry) : bool {
    return (a.Id == b.Id
        && a.Severity == b.Severity
        && a.Message == b.Message)
}

def logs_eq(a : array<LogEntry>; b : array<LogEntry>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!log_eq(ea, eb)) {
            return false
        }
    }
    return true
}

[test(description = "each_sql streams the same rows as the materialized _sql result and the other modes")]
def parity_each_sql_vs_materialized(t : T?) {
    // Same `_where + _order_by` chain, four delivery shapes. Streaming
    // (M1b) must match the materialized variant (M1a) row-for-row.
    with_sqlite(":memory:") $(db) {
        fixture_db_35(db)
        var arr <- fixture_array_35()

        let m1a <- _sql(db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2) |> _order_by(_.Id))

        let m1b <- [for (e in _each_sql(db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2) |> _order_by(_.Id))); e]

        let m2 <- [for (e in (db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2) |> _order_by(_.Id))); e]

        let m3 <- [for (e in (arr |> _where(_.Severity >= 2) |> _order_by(_.Id))); e]

        t |> equal(length(m1a), 10, "stream: 10 severe logs")
        t |> success(logs_eq(m1a, m1b), "stream: M1a (materialized) ≡ M1b (streamed)")
        t |> success(logs_eq(m1b, m2),  "stream: M1b ≡ M2")
        t |> success(logs_eq(m2, m3),   "stream: M2 ≡ M3")
    }
}

[test(description = "breaking out of an each_sql loop finds the same first row as the other modes and frees the statement")]
def parity_each_sql_early_break(t : T?) {
    // Iterate until predicate, then break. The first matching row's Id
    // must be the same across modes.
    with_sqlite(":memory:") $(db) {
        fixture_db_35(db)
        let arr <- fixture_array_35()

        var first_id_m1 = 0
        for (e in _each_sql(db |> select_from(type<LogEntry>) |> _order_by(_.Id))) {
            if (e.Severity == 3) {
                first_id_m1 = e.Id
                break
            }
        }

        var first_id_m2 = 0
        for (e in (db |> select_from(type<LogEntry>) |> _order_by(_.Id))) {
            if (e.Severity == 3) {
                first_id_m2 = e.Id
                break
            }
        }

        var first_id_m3 = 0
        for (e in (arr |> _order_by(_.Id))) {
            if (e.Severity == 3) {
                first_id_m3 = e.Id
                break
            }
        }

        t |> equal(first_id_m1, 4, "early break: first severity=3 is Id=4")
        t |> equal(first_id_m1, first_id_m2, "early break: M1 ≡ M2")
        t |> equal(first_id_m2, first_id_m3, "early break: M2 ≡ M3")

        // Sanity that the iterator's finally ran — a subsequent INSERT must
        // succeed.
        db |> insert(LogEntry(Id = 100, Severity = 0, Message = "after break"))
        let n = db |> query_scalar("SELECT COUNT(*) FROM Logs", type<int>)
        t |> equal(n, 21, "after break: insert succeeded (finally ran)")
    }
}

[test(description = "each_sql over a scalar projection streams the same values as the other modes")]
def parity_each_sql_scalar_projection(t : T?) {
    // _select(_.Message) — streaming a single-column projection.
    with_sqlite(":memory:") $(db) {
        fixture_db_35(db)
        let arr <- fixture_array_35()

        let m1 <- [for (s in _each_sql(db |> select_from(type<LogEntry>) |> _order_by(_.Id) |> _select(_.Message))); s]

        let m2 <- [for (s in (db |> select_from(type<LogEntry>) |> _order_by(_.Id) |> _select(_.Message))); s]

        let m3 <- [for (s in (arr |> _order_by(_.Id) |> _select(_.Message))); s]

        t |> equal(length(m1), 20, "scalar stream: 20 messages")
        t |> equal(length(m1), length(m2), "scalar stream: M1 length ≡ M2")
        t |> equal(length(m2), length(m3), "scalar stream: M2 length ≡ M3")
        for (i in range(length(m1))) {
            t |> equal(m1[i], m2[i], "msg[{i}]: M1 ≡ M2")
            t |> equal(m2[i], m3[i], "msg[{i}]: M2 ≡ M3")
        }
    }
}

// Parity check for tutorial 36 — ATTACH DATABASE.
//
// Chain shapes (`select_from + _where`) are unchanged from earlier
// tutorials; the attach-specific concept is the schema-qualified
// runner produced by `with_attached(db, path, name) $(scoped) { ...
// }` (or the manual `attach + with_schema(name)` pair).
//
// Parity claim: a chain run through a schema-qualified runner emits
// `"<schema>"."<table>"` instead of `"<table>"`, but the result rows
// are the same data. M3 (plain array) doesn't go through the attach
// rail at all — it just iterates the source array. So:
//
//   M1 : `_sql(scoped |> chain)` over an attached :memory: DB
//   M2 : `scoped |> chain`       over the same scoped runner
//   M3 : `arr |> chain`          over the array used to populate it
//
// If `with_schema`'s rewriter mangles a chain in a way that changes
// row content, this test catches it.

def fixture_users_36(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(User(Id = 4, Name = "Dave",  Active = false))
}

def fixture_array_36() : array<User> {
    return <- [
        User(Id = 1, Name = "Alice", Active = true),
        User(Id = 2, Name = "Bob",   Active = true),
        User(Id = 3, Name = "Carol", Active = false),
        User(Id = 4, Name = "Dave",  Active = false)
    ]
}

def user_eq_36(a : User; b : User) : bool {
    return (a.Id == b.Id
        && a.Name == b.Name
        && a.Active == b.Active)
}

def users_eq_36(a : array<User>; b : array<User>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (!user_eq_36(ea, eb)) {
            return false
        }
    }
    return true
}

def sort_by_id_36(var rows : array<User>) {
    rows |> sort() <| cmp_by(_.Id)
}

[test(description = "a chain through a with_attached runner gives identical rows in all three modes")]
def parity_attach_with_attached(t : T?) {
    // _where(_.Active) over a schema-qualified runner. Should produce
    // alice + bob (Id=1, Id=2).
    with_sqlite(":memory:") $(db) {
        var arr <- fixture_array_36()

        db |> with_attached(":memory:", "tenant") $(scoped) {
            fixture_users_36(scoped)

            var m1 <- _sql(scoped |> select_from(type<User>) |> _where(_.Active))
            var m2 <- (scoped |> select_from(type<User>) |> _where(_.Active))
            var m3 <- (arr |> _where(_.Active))

            sort_by_id_36(m1)
            sort_by_id_36(m2)
            sort_by_id_36(m3)

            t |> equal(length(m1), 2, "attached _where: 2 active users")
            t |> success(users_eq_36(m1, m2), "attached _where: M1 ≡ M2")
            t |> success(users_eq_36(m2, m3), "attached _where: M2 ≡ M3")
        }
    }
}

[test(description = "a chain through a manually attached schema runner gives identical rows in all three modes")]
def parity_attach_manual(t : T?) {
    // Manual `attach + with_schema + detach` lifecycle. Same parity claim
    // as `with_attached`; just exercises the lower-level surface.
    with_sqlite(":memory:") $(db) {
        var arr <- fixture_array_36()

        db |> attach(":memory:", "archive")
        db |> with_schema("archive") $(scoped) {
            fixture_users_36(scoped)

            var m1 <- _sql(scoped |> select_from(type<User>) |> _where(!_.Active))
            var m2 <- (scoped |> select_from(type<User>) |> _where(!_.Active))
            var m3 <- (arr |> _where(!_.Active))

            sort_by_id_36(m1)
            sort_by_id_36(m2)
            sort_by_id_36(m3)

            t |> equal(length(m1), 2, "manual attach _where !Active: 2 inactive users")
            t |> success(users_eq_36(m1, m2), "manual attach: M1 ≡ M2")
            t |> success(users_eq_36(m2, m3), "manual attach: M2 ≡ M3")
        }
        db |> detach("archive")
    }
}

// Parity check for tutorial 40 — FTS5 full-text search.
//
// The interesting parity claim: `text_match(col, query)` produces the
// SAME hit set whether evaluated by SQLite's FTS5 engine (M1, lowered
// to `MATCH ?`) or by `daslib/fts5_query`'s in-memory parser (M2/M3,
// plain daslang call per-row).
//
// Chain shape: `_where(_.Body |> text_match(q))`. The Rank column is
// SQL-only (filled by SQLite from BM25) — it's left at default in
// M2/M3, so parity is on the set of matching Body strings, not on
// Rank values.
//
// Coverage:
//   - whitespace-AND  ("quick fox")
//   - prefix          ("quick*")
//   - phrase          ("\"quick brown\"")
//   - boolean OR      ("fox OR boot")
//   - NEAR            ("NEAR(quick fox, 3)")

[sql_fts5(name = "docs_idx")]
struct Doc {
    Body : string
    @sql_fts_rank Rank : float
}

def fixture_db_40(db : SqlRunner) {
    db |> create_table(type<Doc>)
    db |> insert(Doc(Body = "The quick brown fox jumps over the lazy dog"))
    db |> insert(Doc(Body = "A swift red fox escaped the hounds at dawn"))
    db |> insert(Doc(Body = "Quicksand swallowed the hiker's boot yesterday"))
    db |> insert(Doc(Body = "Code review is the most cost-effective bug filter"))
}

def fixture_array_40() : array<Doc> {
    return <- [
        Doc(Body = "The quick brown fox jumps over the lazy dog"),
        Doc(Body = "A swift red fox escaped the hounds at dawn"),
        Doc(Body = "Quicksand swallowed the hiker's boot yesterday"),
        Doc(Body = "Code review is the most cost-effective bug filter")
    ]
}

def bodies_of(rows : array<Doc>) : array<string> {
    var out <- [for (r in rows); r.Body]
    out |> sort()
    return <- out
}

def strings_eq(a : array<string>; b : array<string>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (ea, eb in a, b) {
        if (ea != eb) {
            return false
        }
    }
    return true
}

def assert_text_match_parity(t : T?; query : string; expected_count : int; tag : string) {
    with_sqlite(":memory:") $(db) {
        fixture_db_40(db)
        var arr <- fixture_array_40()

        let m1 <- _sql(db |> select_from(type<Doc>) |> _where(_.Body |> text_match(query)))
        let m2 <- (db |> select_from(type<Doc>) |> _where(_.Body |> text_match(query)))
        let m3 <- (arr |> _where(_.Body |> text_match(query)))

        let b1 <- bodies_of(m1)
        let b2 <- bodies_of(m2)
        let b3 <- bodies_of(m3)

        t |> equal(length(b1), expected_count, "{tag}: M1 hit count")
        t |> success(strings_eq(b1, b2), "{tag}: M1 ≡ M2 (bodies)")
        t |> success(strings_eq(b2, b3), "{tag}: M2 ≡ M3 (bodies)")
    }
}

[test(description = "a whitespace-and text_match finds the same hits in SQLite FTS5 and the in-memory matcher")]
def parity_text_match_and(t : T?) {
    // Whitespace = AND. "quick fox" needs both tokens. Hits doc 1
    // ("quick brown fox jumps") only — doc 3 has "quicksand" but no
    // "fox".
    assert_text_match_parity(t, "quick fox", 1, "AND quick+fox")
}

[test(description = "a prefix text_match finds the same hits in SQLite FTS5 and the in-memory matcher")]
def parity_text_match_prefix(t : T?) {
    // "quick*" matches "quick" + "quicksand". Docs 1 and 3.
    assert_text_match_parity(t, "quick*", 2, "prefix quick*")
}

[test(description = "a phrase text_match finds the same hits in SQLite FTS5 and the in-memory matcher")]
def parity_text_match_phrase(t : T?) {
    // Quoted phrase = adjacent tokens in order. Only doc 1.
    assert_text_match_parity(t, "\"quick brown\"", 1, "phrase quick brown")
}

[test(description = "an or text_match finds the same hits in SQLite FTS5 and the in-memory matcher")]
def parity_text_match_or(t : T?) {
    // "fox OR boot" — docs 1, 2 (fox), 3 (boot). Three hits.
    assert_text_match_parity(t, "fox OR boot", 3, "OR fox|boot")
}

[test(description = "a NEAR text_match finds the same hits in SQLite FTS5 and the in-memory matcher")]
def parity_text_match_near(t : T?) {
    // NEAR(quick fox, 3) — both tokens within 3 of each other. Doc 1
    // ("quick brown fox" — distance 2) qualifies. Doc 3 has only
    // "quicksand" (no "fox"), doesn't qualify.
    assert_text_match_parity(t, "NEAR(quick fox, 3)", 1, "NEAR quick fox 3")
}
