options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 21 — UPSERT: INSERT-or-update-on-conflict.
//
// Three SQL shapes for "INSERT but, if it collides with a UNIQUE / PK
// constraint, do something instead":
//
//   (a) INSERT OR IGNORE          silent no-op on conflict
//   (b) INSERT OR REPLACE         wipe-and-reinsert (loses the old row)
//   (c) ON CONFLICT … DO UPDATE   proper merge (keeps the old row;
//                                  updates only the columns you choose)
//   (d) UPSERT RETURNING          capture the post-merge row
//   (e) try_ non-panic variants   Result<int, string> instead of panic
//
// (a) and (b) are plain functions — no on_conflict / do_update blocks.
// (c) is the macro `_sql_upsert(row, on_conflict, do_update)`. The
// `_` sigil in `do_update` means "the existing row"; `_excluded` is
// "the row we tried to INSERT". The composite-conflict form passes
// `tuple(_.A, _.B)` and the underlying UNIQUE constraint must be a
// composite index (see tutorial 24).

[sql_table(name = "WordHits")]
struct WordHit {
    @sql_primary_key Id : int
    Word : string
    Hits : int
    Last : int64
}

// Composite-conflict demo — tutorial 24 covers `[sql_index]` itself.
[sql_table(name = "UserAccts"),
 sql_index(fields = ("Email", "Tenant"), unique = true)]
struct UserAcct {
    @sql_primary_key Id : int
    Email : string
    Name : string
    Tenant : string
}

def fixture(db : SqlRunner) {
    db |> create_table(type<WordHit>)
    db |> create_table(type<UserAcct>)
    db |> insert(WordHit(Id = 1, Word = "hello", Hits = 1, Last = 100l))
    db |> insert(UserAcct(Id = 1, Email = "x@y.com", Name = "alice", Tenant = "acme"))
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // (a) INSERT OR IGNORE -------------------------------------------
        // Silent no-op on PK / UNIQUE conflict. Returns rows-affected:
        // 1 if inserted, 0 if ignored.
        // SQL: INSERT OR IGNORE INTO "WordHits" (...) VALUES (?,?,?,?)
        let n_first = db |> insert_or_ignore(
            WordHit(Id = 2, Word = "world", Hits = 1, Last = 200l))
        let n_again = db |> insert_or_ignore(
            WordHit(Id = 1, Word = "hello", Hits = 99, Last = 0l))
        to_log(LOG_INFO, "ignore: inserted {n_first}, ignored {1 - n_again}\n")

        // (b) INSERT OR REPLACE ------------------------------------------
        // On conflict, the existing row is DELETED and a fresh one is
        // inserted from the new values. Columns not in the new row reset
        // to defaults — rarely what you want for partial updates. Use
        // (c) for proper merging.
        // SQL: INSERT OR REPLACE INTO "WordHits" (...) VALUES (?,?,?,?)
        db |> insert_or_replace(
            WordHit(Id = 1, Word = "hello", Hits = 42, Last = 999l))

        // (c) ON CONFLICT … DO UPDATE — the proper merge -----------------
        // `_sql_upsert(row, on_conflict, do_update)`.
        //   row          the values to INSERT
        //   on_conflict  `_.Col` (single) or `tuple(_.A, _.B)` (composite)
        //   do_update    named-tuple `(Col = expr, ...)` — `_` is the
        //                existing row, `_excluded` is the row we tried to
        //                INSERT
        // SQL: INSERT INTO "WordHits" (...) VALUES (?,?,?,?)
        //      ON CONFLICT("Id") DO UPDATE SET
        //         "Hits" = "WordHits"."Hits" + 1,
        //         "Last" = "excluded"."Last"
        let bumped = db |> _sql_upsert(
            WordHit(Id = 1, Word = "hello", Hits = 1, Last = 1234l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        to_log(LOG_INFO, "single-key upsert touched {bumped} row(s)\n")

        // Composite conflict target — `tuple(_.Email, _.Tenant)` resolves
        // against the UNIQUE index on `(Email, Tenant)` declared on the
        // struct. SQLite throws "ON CONFLICT clause does not match any
        // PRIMARY KEY or UNIQUE constraint" at runtime if no constraint
        // covers the column list.
        // SQL: INSERT INTO "UserAccts" (...) VALUES (?,?,?,?)
        //      ON CONFLICT("Email","Tenant") DO UPDATE SET
        //         "Name" = "excluded"."Name"
        db |> _sql_upsert(
            UserAcct(Id = 999, Email = "x@y.com", Name = "alice2", Tenant = "acme"),
            tuple(_.Email, _.Tenant),
            (Name = _excluded.Name))

        // (d) UPSERT RETURNING — capture the post-merge row --------------
        // `_sql_upsert_returning` returns `array<T>` (always one row in
        // practice; the array shape mirrors `_sql_update_returning`).
        // SQL: INSERT INTO "WordHits" (...) VALUES (?,?,?,?)
        //      ON CONFLICT("Id") DO UPDATE SET "Hits" = "WordHits"."Hits" + 1
        //      RETURNING "Id","Word","Hits","Last"
        let after <- db |> _sql_upsert_returning(
            WordHit(Id = 1, Word = "hello", Hits = 1, Last = 0l),
            _.Id,
            (Hits = _.Hits + 1))
        for (w in after) {
            to_log(LOG_INFO, "after upsert: {w.Word} hits = {w.Hits}\n")
        }

        // (e) try_ non-panic variants ------------------------------------
        // Each upsert variant has a `try_` sibling returning
        // `Result<int, string>` (or `Result<array<T>, string>` for
        // `_returning`). Useful when callers want to handle constraint
        // violations without a panic.
        let attempt = db |> _sql_try_upsert(
            WordHit(Id = 1, Word = "hello", Hits = 1, Last = 0l),
            _.Id,
            (Hits = _.Hits + 1))
        if (attempt |> is_err) {
            to_log(LOG_ERROR, "upsert failed: {attempt |> unwrap_err}\n")
        } else {
            to_log(LOG_INFO, "try_upsert touched {attempt |> unwrap} row(s)\n")
        }

        let try_ig = db |> try_insert_or_ignore(
            WordHit(Id = 5, Word = "five", Hits = 1, Last = 0l))
        to_log(LOG_INFO, "try_insert_or_ignore: {try_ig |> unwrap}\n")
    }
}
