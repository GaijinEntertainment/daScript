options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 44 — `_in` / `_not_in` against a captured collection.
//
// `_.Col._in(arr)` lowers to `WHERE Col IN (SELECT value FROM json_each(?))`
// — the captured array is JSON-encoded and bound as a single TEXT parameter.
// Single bind, any size (SQLite accepts 1G TEXT), no plan-cache pollution
// (one parameterized SQL string regardless of collection size). Same shape
// EF Core 8 picked as the default for `Contains(coll, x)` translations.
//
// Element types covered: int, int64, string, double, bool, float — anything
// `daslib/json`'s `JV(...)` handles (which is "any scalar daslang type").
//
// Subquery form (`_.Col._in(other_chain)`) coexists in the same operator —
// see tutorial 17. This file covers the captured-runtime-collection form.
//
// `_not_in` on a nullable (`Option<T>`) column emits the defensive
// `… NOT IN (…) OR Col IS NULL` form so rows with NULL Col survive the
// predicate. SQL three-valued logic would otherwise drop them silently
// (`x != NULL` is unknown, never true).

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Active : bool
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    @safe_when_uninitialized OwnerId : Option<int>
    Total : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert(User(Id = 1, Name = "Alice", Active = true))
    db |> insert(User(Id = 2, Name = "Bob",   Active = true))
    db |> insert(User(Id = 3, Name = "Carol", Active = false))
    db |> insert(User(Id = 4, Name = "Dave",  Active = true))
    db |> insert(Order(Id = 10, OwnerId = some(1), Total = 100))
    db |> insert(Order(Id = 11, OwnerId = some(2), Total = 250))
    db |> insert(Order(Id = 12, OwnerId = none(type<int>), Total = 50))
    db |> insert(Order(Id = 13, OwnerId = some(3), Total = 80))
}

[export]
def main() {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // ── Positive: _in(arr) ───────────────────────────────────────────
        // SQL: WHERE "Id" IN (SELECT value FROM json_each(?))
        // Bind: '[1,4]'
        let picked <- _sql(db |> select_from(type<User>) |> _where(_.Id._in([1, 4])))
        to_log(LOG_INFO, "_in([1,4]) -> {length(picked)} user(s)\n")

        // ── Negative on non-nullable column: naive NOT IN ────────────────
        // SQL: WHERE "Id" NOT IN (SELECT value FROM json_each(?))
        // (User.Id is the primary key — non-nullable — so the simpler form is
        // safe.)
        let not_picked <- _sql(db |> select_from(type<User>) |> _where(_.Id._not_in([1, 4])))
        to_log(LOG_INFO, "_not_in([1,4]) -> {length(not_picked)} user(s)\n")

        // ── Negative on nullable column: defensive NOT IN ────────────────
        // SQL: WHERE ("OwnerId" NOT IN (SELECT value FROM json_each(?))
        //             OR "OwnerId" IS NULL)
        // Order.OwnerId is Option<int>. Order 12 has OwnerId=NULL — without
        // the OR-IS-NULL clause it would silently drop from the result.
        let other_orders <- _sql(db |> select_from(type<Order>) |> _where(_.OwnerId._not_in([1])))
        to_log(LOG_INFO, "_not_in([1]) on Option<int> -> {length(other_orders)} order(s) (NULL row survives)\n")

        // ── Composes with other WHERE predicates ────────────────────────
        // SQL: WHERE ("Active") AND ("Id" IN (SELECT value FROM json_each(?)))
        let active_picked <- _sql(db |> select_from(type<User>)
                                     |> _where(_.Active && _.Id._in([1, 4])))
        to_log(LOG_INFO, "Active AND _in([1,4]) -> {length(active_picked)} user(s)\n")

        // ── String collections work the same ────────────────────────────
        // JSON encodes any scalar; bind type is TEXT for both.
        let by_name <- _sql(db |> select_from(type<User>) |> _where(_.Name._in(["Alice", "Carol"])))
        to_log(LOG_INFO, "_in([\"Alice\",\"Carol\"]) -> {length(by_name)} user(s)\n")

        // ── Empty collection ─────────────────────────────────────────────
        // json_each over '[]' yields zero rows → `IN (SELECT value FROM ...)`
        // is false everywhere → no rows match.
        var empty : array<int>  // nolint:LINT003 — explicit-type empty-array declaration; `let` can't init typed-empty
        let none_match <- _sql(db |> select_from(type<User>) |> _where(_.Id._in(empty)))
        to_log(LOG_INFO, "_in([]) -> {length(none_match)} user(s) (always 0)\n")

        // ── Inspect emitted SQL while debugging ──────────────────────────
        to_log(LOG_INFO, "SQL: {_sql_text(db |> select_from(type<User>) |> _where(_.Id._in([1, 2])))}\n")
    }
}
