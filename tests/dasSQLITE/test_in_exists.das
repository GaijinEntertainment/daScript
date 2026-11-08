options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

[sql_table(name="Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Active : bool
}

[sql_table(name="Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total = 50))
    // Carol (id 3) has no orders.
}

// ──────────────────────────────────────────────────────────────────────────
// `_in` / `_not_in` — IN / NOT IN with a subquery. Subquery is
// uncorrelated; users project a single column with `_select(_.Col)`.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_in over another table emits IN with a subquery")]
def test_in_emits_in_subquery(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Id._in(_db |> select_from(type<Order>) |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" IN (SELECT \"UserId\" FROM \"Orders\")",
        "_in with subquery emits IN (SELECT ...)")
}

[test(description = "_not_in over another table emits NOT IN with a subquery")]
def test_not_in_emits_not_in_subquery(t : T?) {
    var _db = SqlRunner()
    // _not_in expands to !contains(subq, x); pred_to_sql's NOT peel wraps
    // it.
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Id._not_in(_db |> select_from(type<Order>) |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" NOT IN (SELECT \"UserId\" FROM \"Orders\")",
        "_not_in emits explicit NOT IN (special-cased in pred_to_sql to peek through the `!contains(...)` AST shape)")
}

[test(description = "_in combined with an outer where emits both conditions joined by AND")]
def test_in_with_outer_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Active && _.Id._in(_db |> select_from(type<Order>) |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE ((\"Active\" <> 0)) AND (\"Id\" IN (SELECT \"UserId\" FROM \"Orders\"))",
        "_in composes with other WHERE predicates via && — bind order preserved")
}

[test(description = "a where inside the _in subquery emits its bind in the subquery")]
def test_in_with_inner_where_binds(t : T?) {
    var _db = SqlRunner()
    let _threshold = 100
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Id._in(_db |> select_from(type<Order>)
                                                  |> _where(_.Total > _threshold)
                                                  |> _select(_.UserId))))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" IN (SELECT \"UserId\" FROM \"Orders\" WHERE \"Total\" > ?)",
        "_in with inner _where emits subquery WHERE — bind appended after outer-WHERE binds")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime — verify rows match the IN/NOT IN semantics.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_in over another table returns only the rows whose key appears there")]
def test_in_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Users whose Id appears in Orders.UserId — Alice and Bob.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
        t |> equal(length(rows), 2, "Alice + Bob match (Carol absent)")
        var saw_alice = false
        var saw_bob = false
        for (u in rows) {
            if (u.Name == "Alice") {
                saw_alice = true
            }
            if (u.Name == "Bob") {
                saw_bob = true
            }
        }
        t |> success(saw_alice)
        t |> success(saw_bob)
    }
}

[test(description = "_not_in over another table returns only the rows whose key is absent there")]
def test_not_in_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
        t |> equal(length(rows), 1, "only Carol (no orders) matches NOT IN")
        t |> equal(rows[0].Name, "Carol")
    }
}

def private fixture_41(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(Order(Id = 10, UserId = 1, Total = 100))
    db |> insert(Order(Id = 11, UserId = 1, Total = 250))
    db |> insert(Order(Id = 12, UserId = 2, Total = 50))
}

// ──────────────────────────────────────────────────────────────────────────
// `_any` / `_none` in `_where` predicates → SQL `EXISTS` / `NOT
// EXISTS`. Subquery is uncorrelated — the inner predicate references
// only the subquery's own row (`_`), not outer columns.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "any over another table emits EXISTS with a subquery")]
def test_any_emits_exists(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._any()))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\")",
        "_any with no predicate emits EXISTS (subquery)")
}

[test(description = "none over another table emits NOT EXISTS with a subquery")]
def test_none_emits_not_exists(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._none()))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE NOT EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\")",
        "_none with no predicate emits NOT EXISTS (subquery)")
}

[test(description = "any with a predicate emits EXISTS with the predicate in the subquery WHERE")]
def test_any_with_predicate(t : T?) {
    var _db = SqlRunner()
    let _threshold = 200
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._any(_.Total > _threshold)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\" WHERE \"Total\" > ?)",
        "_any(pred) emits EXISTS with WHERE pred")
}

[test(description = "none with a predicate emits NOT EXISTS with the predicate in the subquery WHERE")]
def test_none_with_predicate(t : T?) {
    var _db = SqlRunner()
    let _threshold = 1000
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where((_db |> select_from(type<Order>))._none(_.Total > _threshold)))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE NOT EXISTS (SELECT \"Id\", \"UserId\", \"Total\" FROM \"Orders\" WHERE \"Total\" > ?)",
        "_none(pred) emits NOT EXISTS with WHERE pred")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime
// ──────────────────────────────────────────────────────────────────────────

[test(description = "any over a non-empty uncorrelated table keeps every row")]
def test_any_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_41(db)
        // EXISTS (any orders) — Orders is non-empty, so all Users match.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where((db |> select_from(type<Order>))._any()))
        t |> equal(length(rows), 3, "all 3 users (any order exists, uncorrelated)")
    }
}

[test(description = "any with a predicate keeps every row when a match exists and none when no row matches")]
def test_any_with_predicate_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_41(db)
        // EXISTS Orders where Total > 200 — there's the 250 order, so all
        // match.
        let rows <- _sql(db |> select_from(type<User>)
                           |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
        t |> equal(length(rows), 3, "all 3 users (an order with Total > 200 exists)")
        let rows_no_huge_orders <- _sql(db |> select_from(type<User>)
                                          |> _where((db |> select_from(type<Order>))._any(_.Total > 1000)))
        t |> equal(length(rows_no_huge_orders), 0, "0 users (no order with Total > 1000)")
    }
}

[sql_table(name = "Users")]
struct User_44 {
    @sql_primary_key Id : int
    Name : string
    Active : bool
}

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Tag : string
    @safe_when_uninitialized OwnerId : Option<int>
}

def private fixture_44(db : SqlRunner) {
    db |> create_table(type<User_44>)
    db |> create_table(type<Item>)
    db |> insert(User_44(Id = 1, Name = "Alice", Active = true))
    db |> insert(User_44(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User_44(Id = 3, Name = "Carol", Active = false))
    db |> insert(User_44(Id = 4, Name = "Dave",  Active = true))
    db |> insert(Item(Id = 10, Tag = "alpha", OwnerId = some(1)))
    db |> insert(Item(Id = 11, Tag = "beta",  OwnerId = some(2)))
    db |> insert(Item(Id = 12, Tag = "gamma", OwnerId = none(type<int>)))
    db |> insert(Item(Id = 13, Tag = "delta", OwnerId = some(3)))
}

// ──────────────────────────────────────────────────────────────────────────
// `_in` / `_not_in` over a captured runtime collection — JSON-bridge
// form. Lowers to `WHERE col IN (SELECT value FROM json_each(?))`
// with a single JSON-encoded TEXT bind. Single bind, any size,
// plan-cache-friendly.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_in over a runtime collection emits IN over json_each of one bind")]
def test_in_collection_emits_json_each(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User_44>) |> _where(_.Id._in([1, 2])))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" IN (SELECT value FROM json_each(?))",
        "_in(arr) lowers to json_each(?) with one bind")
}

[test(description = "_not_in over a runtime collection on a non-nullable column emits plain NOT IN")]
def test_not_in_collection_non_nullable_emits_naive(t : T?) {
    var _db = SqlRunner()
    // User_44.Id is non-nullable (primary key) — no defensive `OR IS
    // NULL` needed.
    let sql = _sql_text(_db |> select_from(type<User_44>) |> _where(_.Id._not_in([1, 2])))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" NOT IN (SELECT value FROM json_each(?))",
        "_not_in on non-nullable column emits naive NOT IN")
}

[test(description = "_not_in over a runtime collection on a nullable column wraps the test so NULL rows survive")]
def test_not_in_collection_nullable_emits_defensive(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Item>) |> _where(_.OwnerId._not_in([1, 2])))
    t |> equal(sql,
        "SELECT \"Id\", \"Tag\", \"OwnerId\" FROM \"Items\" WHERE NOT IFNULL(\"OwnerId\" IN (SELECT value FROM json_each(?)), 0)",
        "_not_in on Option<T> column emits defensive form: rows with NULL column survive")
}

[test(description = "_in over a runtime collection combines with an outer where by AND")]
def test_in_collection_with_outer_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User_44>)
                          |> _where(_.Active && _.Id._in([1, 2, 4])))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE ((\"Active\" <> 0)) AND (\"Id\" IN (SELECT value FROM json_each(?)))",
        "_in(arr) composes with other WHERE predicates via &&")
}

[test(description = "_in over a runtime string collection emits IN over json_each")]
def test_in_collection_string_array(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Item>) |> _where(_.Tag._in(["alpha", "delta"])))
    t |> equal(sql,
        "SELECT \"Id\", \"Tag\", \"OwnerId\" FROM \"Items\" WHERE \"Tag\" IN (SELECT value FROM json_each(?))",
        "_in(arr<string>) emits the same json_each form — JSON encodes any scalar element type")
}

// ──────────────────────────────────────────────────────────────────────────
// Runtime — verify the rows match the IN / NOT IN semantics.
// ──────────────────────────────────────────────────────────────────────────

[test(description = "_in over a runtime collection returns the rows whose key is in it")]
def test_in_collection_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        let rows <- _sql(db |> select_from(type<User_44>) |> _where(_.Id._in([1, 4])))
        t |> equal(length(rows), 2, "Alice (1) + Dave (4) match")
        var saw_alice = false
        var saw_dave = false
        for (u in rows) {
            if (u.Name == "Alice") {
                saw_alice = true
            }
            if (u.Name == "Dave") {
                saw_dave = true
            }
        }
        t |> success(saw_alice)
        t |> success(saw_dave)
    }
}

[test(description = "_not_in over a runtime collection returns the rows whose key is not in it")]
def test_not_in_collection_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        let rows <- _sql(db |> select_from(type<User_44>) |> _where(_.Id._not_in([1, 4])))
        t |> equal(length(rows), 2, "Bob (2) + Carol (3) survive (Alice + Dave excluded)")
    }
}

[test(description = "_not_in over a runtime collection keeps rows whose nullable column is NULL")]
def test_not_in_collection_nullable_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        // Items 10 (owner 1) + 11 (owner 2) excluded. Item 13 (owner 3)
        // survives, AND item 12 (NULL owner) survives via the defensive `NOT
        // IFNULL(x IN …, 0)` form — naive NOT IN would drop NULL-owner rows.
        let rows <- _sql(db |> select_from(type<Item>) |> _where(_.OwnerId._not_in([1, 2])))
        t |> equal(length(rows), 2, "items 12 (NULL owner) + 13 (owner 3) survive")
        var saw_null = false
        var saw_three = false
        for (it in rows) {
            if (it.Id == 12) {
                saw_null = true
            }
            if (it.Id == 13) {
                saw_three = true
            }
        }
        t |> success(saw_null, "NULL-owner row survived NOT IN — defensive form working")
        t |> success(saw_three)
    }
}

[test(description = "_in over an empty collection returns no rows")]
def test_in_empty_collection_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_44(db)
        // Empty array → json_each over '[]' yields zero rows → IN is false.
        var empty : array<int>  // nolint:LINT003 — explicit-type empty-array declaration; `let` can't init typed-empty
        let rows <- _sql(db |> select_from(type<User_44>) |> _where(_.Id._in(empty)))
        t |> equal(length(rows), 0, "_in([]) returns no rows")
    }
}
