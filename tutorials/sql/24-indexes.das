options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 24 — indexes: declaring `CREATE INDEX` from struct annotations.
//
// SHAPE: `[sql_index(fields = ..., unique = ..., name = ...)]` is a
// sibling annotation alongside `[sql_table]`. Both must live in the
// **same** bracket pair, comma-separated, and `[sql_table]` must come
// first — `[sql_index]` rewrites a helper that `[sql_table]` emits.
//
//   [sql_table(name = "Users"),
//    sql_index(fields = "Email", unique = true),
//    sql_index(fields = ("City", "LastSeen")),
//    sql_index(fields = "LastSeen")]
//   struct User { ... }
//
// `fields=` accepts a single string or a tuple of strings (composite
// index). `unique` defaults to `false`. `name` defaults to
// `idx_<table>_<col1>[_<col2>...]`.
//
// `[sql_table]` validates every field name against the struct fields at
// macro-expansion time. Misspellings produce a compile error with the
// list of valid columns.
//
// Indexes themselves are transparent to query code — the SQLite planner
// uses them automatically. The only user-visible effect outside DDL is
// that a `UNIQUE INDEX` lets the upsert composite-conflict path (tut 21)
// resolve `tuple(_.A, _.B)` against a real constraint.

[sql_table(name = "Users"),
 sql_index(fields = "Email", unique = true),
 sql_index(fields = ("City", "LastSeen")),
 sql_index(fields = "LastSeen", name = "ix_users_lastseen")]
struct User {
    @sql_primary_key Id : int
    Email : string
    Name : string
    City : string
    LastSeen : int64
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        // `create_table` runs the CREATE TABLE statement followed by the
        // accumulated CREATE INDEX statements in source order, all inside
        // a single transaction (so a failing index rolls the whole table
        // creation back).
        // SQL emitted:
        //   CREATE TABLE "Users" (...);
        //   CREATE UNIQUE INDEX "idx_Users_Email" ON "Users" ("Email");
        //   CREATE INDEX "idx_Users_City_LastSeen" ON "Users" ("City", "LastSeen");
        //   CREATE INDEX "ix_users_lastseen" ON "Users" ("LastSeen");
        db |> create_table(type<User>)

        // The DDL helper that the macro generates is callable directly —
        // useful for migration scripts and test asserts.
        let idx_sql = _sql_create_indexes_sql(type<User>)
        to_log(LOG_INFO, "index DDL: {idx_sql}\n")

        // Querying ignores indexes entirely — the planner uses them
        // automatically. Same chain regardless of what's indexed.
        // SQL: SELECT ... FROM "Users" WHERE "Email" = ? LIMIT 1
        //      (uses idx_Users_Email)
        db |> insert([
            User(Id = 1, Email = "alice@x.com", Name = "alice", City = "SF",  LastSeen = 100l),
            User(Id = 2, Email = "bob@x.com",   Name = "bob",   City = "SF",  LastSeen = 200l),
            User(Id = 3, Email = "carol@x.com", Name = "carol", City = "NYC", LastSeen = 300l)
        ])
        let alice = _sql(db |> select_from(type<User>)
                            |> _where(_.Email == "alice@x.com") |> _first())
        to_log(LOG_INFO, "lookup-by-email hit {alice.Name}\n")

        // The composite index supports queries that filter on `City`
        // (or `City + LastSeen`) and order by `LastSeen` — SQLite walks
        // the index in order without a separate sort step.
        // SQL: SELECT ... FROM "Users" WHERE "City" = ?
        //                                ORDER BY "LastSeen" DESC
        //      (uses idx_Users_City_LastSeen for both filter and sort)
        let sf_recent <- _sql(db |> select_from(type<User>)
                                  |> _where(_.City == "SF")
                                  |> _order_by_descending(_.LastSeen)
                                  |> _to_array())
        for (u in sf_recent) {
            to_log(LOG_INFO, "SF user (recent first): {u.Name} @ {u.LastSeen}\n")
        }

        // UNIQUE-INDEX violations surface through `try_insert`.
        // SQL: INSERT INTO "Users" (...) VALUES (?,...,?)
        //      → SQLite rejects: "UNIQUE constraint failed: Users.Email"
        let dup = User(Id = 4, Email = "alice@x.com", Name = "alice2",
                       City = "LA", LastSeen = 0l)
        let res = db |> try_insert(dup)
        if (res |> is_err) {
            to_log(LOG_ERROR, "duplicate email rejected: {res |> unwrap_err}\n")
        }
    }
}

// --- composite UNIQUE for upsert composite-conflict --------------------
//
// `_sql_upsert(row, tuple(_.A, _.B), do_update)` (tutorial 21) requires
// a UNIQUE constraint covering the same column set. `[sql_index(unique
// = true, fields = ("A", "B"))]` is how you declare one — there is no
// separate `[sql_unique]` table-level annotation.
//
// Single-column uniqueness can use either `@sql_unique` on the field
// (emits `UNIQUE` directly in the column DDL) or `[sql_index(unique =
// true, fields = "Col")]` (separate `CREATE UNIQUE INDEX`). Both work
// for the upsert path; the index form is preferable when you also need
// to control the index name.
