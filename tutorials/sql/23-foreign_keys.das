options gen2

require daslib/sql
require daslib/option
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 23 — foreign keys: the DDL-side of relationships.
//
// dasSQLITE ships only the DDL half of the FK story: a child column
// names its parent struct via `@sql_references`, optional `@sql_on_delete`
// and `@sql_on_update` pick the integrity action. There are no
// navigation properties (no `_include`, no `u.Orders` shortcut) — to
// query across a relationship use `_join` (tutorial 15) or `_any` /
// `_in` correlated subqueries (tutorial 17).
//
// `with_sqlite` enables `PRAGMA foreign_keys=ON` on every connection.
// SQLite ships the pragma OFF for back-compat with pre-3.6.19 databases;
// without it FK clauses are parsed and stored but never enforced — orphans
// silently appear and CASCADE silently no-ops. dasSQLITE turns it on
// always.

// --- annotation shape (per-field decorators) --------------------------
//
//   @sql_references = "ParentTypeName"
//   @sql_on_delete  = "cascade" | "set_null" | "set_default"
//                       | "restrict" | "no_action"
//   @sql_on_update  = (same value set as on_delete)
//
// Each decorator is a separate `@name = value` line — daslang field
// annotations are scalar key/value pairs, not function-call shapes.
//
// `@sql_references` resolves at `[sql_table]` expansion time: it finds
// the named struct in the same module, reads its `[sql_table(name=...)]`
// to get the table name, and finds the `@sql_primary_key` field. Defaults
// when omitted: `on_delete = "no_action"`, `on_update = "no_action"`.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int

    // CASCADE: deleting a User auto-deletes their Orders.
    @sql_references = "User"
    @sql_on_delete = "cascade"
    UserId : int

    Total : float
}

// `set_null` requires the column be `Option<T>` so SQLite can write NULL
// when the parent goes away. The `[sql_table]` macro rejects the
// combination at compile time if the column is plain `T`.
[sql_table(name = "Comments")]
struct Comment {
    @sql_primary_key Id : int

    @sql_references = "User"
    @sql_on_delete = "set_null"
    @safe_when_uninitialized AuthorId : Option<int>

    Body : string
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        // Drop in child-first order; create parent-first. The FK clause in
        // Orders names Users, so Users must exist when Orders is created.
        db |> drop_table_if_exists(type<Comment>)
        db |> drop_table_if_exists(type<Order>)
        db |> drop_table_if_exists(type<User>)
        db |> create_table(type<User>)
        db |> create_table(type<Order>)
        db |> create_table(type<Comment>)

        // --- seed data --------------------------------------------------
        db |> insert([
            User(Id = 1, Name = "alice"),
            User(Id = 2, Name = "bob")
        ])
        db |> insert([
            Order(Id = 100, UserId = 1, Total = 42.0f),
            Order(Id = 101, UserId = 1, Total = 99.0f),
            Order(Id = 102, UserId = 2, Total = 7.5f)
        ])
        db |> insert([
            Comment(Id = 1000, AuthorId = some(1), Body = "alice's"),
            Comment(Id = 1001, AuthorId = some(2), Body = "bob's")
        ])

        // (a) ON DELETE CASCADE — child rows follow the parent ----------
        // SQL: DELETE FROM "Users" WHERE "Id" = ?
        //      (SQLite cascades internally to DELETE FROM "Orders"
        //       WHERE "UserId" = 1)
        db |> _sql_delete(type<User>, _.Id == 1)
        let alice_orders = _sql(db |> select_from(type<Order>) |> _where(_.UserId == 1) |> count())
        to_log(LOG_INFO, "alice's orders after cascade: {alice_orders} (expect 0)\n")

        // (b) ON DELETE SET NULL — child column nulled out ---------------
        // The first DELETE above (alice) already nulled out comment 1000
        // because Comments.AuthorId points at Users with `set_null`.
        // Deleting bob nulls out his comment too. Both AuthorId columns
        // now hold NULL — surfaced as `Option<int>::None` in daslang.
        // SQL: DELETE FROM "Users" WHERE "Id" = ?
        //      (SQLite cascades internally to UPDATE "Comments"
        //       SET "AuthorId" = NULL WHERE "AuthorId" = 2)
        db |> _sql_delete(type<User>, _.Id == 2)
        let comments <- _sql(db |> select_from(type<Comment>) |> _to_array())
        for (c in comments) {
            let author_str = c.AuthorId |> is_some ? "{c.AuthorId |> unwrap}" : "<null>"
            to_log(LOG_INFO, "comment {c.Id}: author = {author_str}\n")
        }

        // (c) FK violations surface through try_insert -------------------
        // The orphan UserId 9999 has no matching parent row; SQLite
        // rejects the INSERT with "FOREIGN KEY constraint failed".
        let res = db |> try_insert(Order(Id = 999, UserId = 9999, Total = 1.0f))
        if (res |> is_err) {
            to_log(LOG_ERROR, "orphan rejected: {res |> unwrap_err}\n")
        }
    }
}

// --- on_delete / on_update reference table ----------------------------
//
//   "cascade"      → CASCADE        propagate the change to children
//   "set_null"     → SET NULL       (requires Option<T> on the FK column)
//   "set_default"  → SET DEFAULT    (requires a daslang field initializer
//                                    or @sql_default_fn — see tut 25)
//   "restrict"     → RESTRICT       reject the change immediately
//   "no_action"    → NO ACTION      reject at constraint-check time (the
//                                    SQL standard default)
//
// Querying across the relationship is always explicit — no nav properties.
// See tutorial 15 for `_join` (inner) and tutorial 16 for `_left_join`,
// or tutorial 17 for `_any` / `_in` correlated subqueries when only the
// existence of a match matters.
