options gen2

// Versioned schema migrations for SQLite.
//
// A DB file holds *schema* (tables, columns, indexes) and *data* (rows). When
// v1 of an app ships, users start filling tables. When v2 ships with a new
// column, the user's existing `app.db` still has the v1 shape — v2 code that
// queries the new column panics on the first call. Migrations solve this:
// each schema change ships as a numbered, frozen step, and the runtime
// applies whatever is missing on every startup.
//
// Three rules make this work:
//
//   1. Migrations are append-only. Once shipped, a migration is FROZEN —
//      to change anything, write a new one with a higher version number.
//   2. Versions are unique across the whole program. Two devs picking
//      `version=N` on parallel branches is a compile-time error.
//   3. The runner uses one transaction per call. If any migration fails,
//      every effect rolls back — re-run replays the whole pending list.
//
// The struct describes the CURRENT schema. Old shapes live ONLY in the
// migration body's raw SQL — daslang has no model-snapshot system.
//
// See: API_MIGRATION.md (design notes), tut 39 (schema_from), tut 42
// (multi-version ETL with schema_from for the *read* side).

require sqlite/sql_migrate

// =====================================================================
// PART 1 — Define a schema and write a few migrations
// =====================================================================

// Current shape. Migrations 1-3 below describe the path from empty to here.

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Email : string                    // added in migration 2
    Score : int = 0                   // added in migration 3 with default
}

// Migration 1 — bootstrap.
//
// Annotation parameters:
//   version=N           required
//   description=...     optional; appears in __schema_version + logs
//   vacuum=true         optional; runs `VACUUM` after this call commits
//   analyze=true        optional; runs `ANALYZE` after this call commits
//
// Body uses raw SQL because at v=1 the struct's *current* shape (Id, Name,
// Email, Score) doesn't match v=1's schema (Id, Name). Migrations always
// reference prior schemas via raw SQL; only the current struct knows the
// current shape.

[sql_migration(version = 1, description = "create users")]
def migration_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

// Migration 2 — add a column.

[sql_migration(version = 2, description = "add email column")]
def migration_002(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

// Migration 3 — multiple statements. All run inside the call's single
// transaction; failure of any one rolls back the whole call. DDL and DML
// mix freely.

[sql_migration(version = 3, description = "add score; backfill", analyze = true)]
def migration_003(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Score INTEGER NOT NULL DEFAULT 0")
    db |> exec("UPDATE users SET Score = 100 WHERE Email != ''")
    // analyze=true on the annotation runs `ANALYZE` once after the call
    // commits — refreshes stats after the column add. VACUUM works the
    // same way (cheap-then-expensive ordering: ANALYZE, then VACUUM).
}

[export]
def main() : int {
    // =================================================================
    // PART 2 — App startup with with_latest_sqlite
    // =================================================================
    //
    // Preferred form: with_latest_sqlite combines with_sqlite +
    // apply_recommended_pragmas + migrate_to_latest. Less ceremony,
    // harder to ship a binary that opens the DB without bringing it up
    // to date. The explicit two-step (with_sqlite + migrate_to_latest)
    // remains available for tools/CI that may want to inspect first.

    with_latest_sqlite(":memory:") $(db) {
        // DB is now at the latest schema. App-level logic goes here.
        db |> exec("INSERT INTO users (Name, Email) VALUES ('alice', 'alice@x')")
        let count = db |> query_scalar("SELECT COUNT(*) FROM users", type<int>)
        print("inserted {count} user(s)\n")

        // ================================================================
        // PART 3 — Inspection (CLI / health endpoint / CI gate)
        // ================================================================
        //
        // current_schema_version / pending_migrations / migration_history
        // are pure reads. They never create the audit table or write a
        // row — safe to call from `mytool db status` against a brand-new
        // DB without side effects.

        print("current schema version: {db |> current_schema_version()}\n")

        let pending <- db |> pending_migrations()
        print("pending migrations: {pending |> length}\n")

        let history <- db |> migration_history()
        print("history: {history |> length} row(s)\n")
        for (h in history) {
            print("  v{h.version}: '{h.description}'\n")
        }
    }

    // =================================================================
    // PART 4 — Adoption: baseline an existing DB
    // =================================================================
    //
    // Real-world story: the app has been shipping without migrations.
    // Existing user DBs already have `users` from v1 hand-rolled DDL.
    // Naive swap to with_latest_sqlite would crash — migration_001's
    // CREATE TABLE fails because the table already exists.
    //
    // baseline(db, version=N) stamps v1..vN as already-applied without
    // running their bodies. Idempotent — safe to call repeatedly.
    //
    // Layer-1: when migrate_to_latest detects user objects in a
    //   freshly-empty audit table, it logs a WARNING with the baseline
    //   recipe.
    // Layer-2: if the first migration fails because of pre-existing
    //   tables, the panic message appends the same baseline hint.
    //
    // Recommended in tut 43: prefer first-change-as-first-migration —
    // your first new migration ALTERs the existing schema rather than
    // recreating it. Reach for baseline only when you specifically want
    // CREATE-TABLE-shaped retroactive history.

    with_sqlite(":memory:") $(db) {
        // Simulate a pre-existing v1-era DB.
        db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
        db |> exec("INSERT INTO users (Name) VALUES ('legacy bob')")

        // Stamp v1 as already-applied so migration_001 won't re-run.
        db |> baseline(1)
        print("baseline(1): stamped v1 from existing schema\n")

        // Now run the rest. Only v2 + v3 actually execute.
        let n = db |> migrate_to_latest()
        print("applied {n} migration(s) after baseline\n")

        // Pre-existing data is preserved across the ALTERs.
        let bob_score = db |> query_scalar(
            "SELECT Score FROM users WHERE Name = 'legacy bob'", type<int>)
        print("legacy bob's Score (default applied): {bob_score}\n")
    }
    return 0
}
