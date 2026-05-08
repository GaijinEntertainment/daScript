options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Users")]
struct User {
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

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Item>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(User(Id = 4, Name = "Dave",  Active = true))
    db |> insert(Item(Id = 10, Tag = "alpha", OwnerId = some(1)))
    db |> insert(Item(Id = 11, Tag = "beta",  OwnerId = some(2)))
    db |> insert(Item(Id = 12, Tag = "gamma", OwnerId = none(type<int>)))
    db |> insert(Item(Id = 13, Tag = "delta", OwnerId = some(3)))
}

// ──────────────────────────────────────────────────────────────────────────
// `_in` / `_not_in` over a captured runtime collection — JSON-bridge form.
// Lowers to `WHERE col IN (SELECT value FROM json_each(?))` with a single
// JSON-encoded TEXT bind. Single bind, any size, plan-cache-friendly.
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_in_collection_emits_json_each(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>) |> _where(_.Id._in([1, 2])))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" IN (SELECT value FROM json_each(?))",
        "_in(arr) lowers to json_each(?) with one bind")
}

[test]
def test_not_in_collection_non_nullable_emits_naive(t : T?) {
    var _db = SqlRunner()
    // User.Id is non-nullable (primary key) — no defensive `OR IS NULL` needed.
    let sql = _sql_text(_db |> select_from(type<User>) |> _where(_.Id._not_in([1, 2])))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE \"Id\" NOT IN (SELECT value FROM json_each(?))",
        "_not_in on non-nullable column emits naive NOT IN")
}

[test]
def test_not_in_collection_nullable_emits_defensive(t : T?) {
    var _db = SqlRunner()
    // Item.OwnerId is Option<int> — defensive `OR IS NULL` so NULL rows survive NOT IN.
    let sql = _sql_text(_db |> select_from(type<Item>) |> _where(_.OwnerId._not_in([1, 2])))
    t |> equal(sql,
        "SELECT \"Id\", \"Tag\", \"OwnerId\" FROM \"Items\" WHERE (\"OwnerId\" NOT IN (SELECT value FROM json_each(?)) OR \"OwnerId\" IS NULL)",
        "_not_in on Option<T> column emits defensive form: rows with NULL column survive")
}

[test]
def test_in_collection_with_outer_where(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<User>)
                          |> _where(_.Active && _.Id._in([1, 2, 4])))
    t |> equal(sql,
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\" WHERE (\"Active\") AND (\"Id\" IN (SELECT value FROM json_each(?)))",
        "_in(arr) composes with other WHERE predicates via &&")
}

[test]
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

[test]
def test_in_collection_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>) |> _where(_.Id._in([1, 4])))
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

[test]
def test_not_in_collection_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<User>) |> _where(_.Id._not_in([1, 4])))
        t |> equal(length(rows), 2, "Bob (2) + Carol (3) survive (Alice + Dave excluded)")
    }
}

[test]
def test_not_in_collection_nullable_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Items 10 (owner 1) + 11 (owner 2) excluded. Item 13 (owner 3) survives, AND
        // item 12 (NULL owner) survives via the defensive `OR IS NULL` clause —
        // naive NOT IN would drop NULL-owner rows.
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

[test]
def test_in_empty_collection_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Empty array → json_each over '[]' yields zero rows → IN is false.
        var empty : array<int>  // nolint:LINT003 — explicit-type empty-array declaration; `let` can't init typed-empty
        let rows <- _sql(db |> select_from(type<User>) |> _where(_.Id._in(empty)))
        t |> equal(length(rows), 0, "_in([]) returns no rows")
    }
}
