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

require sqlite/sqlite_migrate

// =====================================================================
// PART 1 — Define a schema and write a few migrations
// =====================================================================

// Current shape. Migrations 1-3 below describe the path from empty to here;
// migration 6 (PART 5) shows the rebuild path that adds LastLoginAt.

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Email : string                    // added in migration 2
    Score : int = 0                   // added in migration 3 with default
    LastLoginAt : Option<int64>       // added in migration 6 via convert_and_rename
}

// Pre-rebuild shape, kept around as a [sql_table(legacy=true)] historical
// reference. Read-only — usable with select_from inside the rebuild migration,
// but the compiler refuses create_table / insert / update / delete on it.
[sql_table(name = "users", legacy = true)]
struct UserV5 {
    @sql_primary_key Id : int
    Name : string
    Email : string
    Score : int = 0
}

// [struct_convert] walks T's fields and emits `_::struct_convert_field(tgt.X, src.X)`
// per field. Identity / Option-wrap / numeric / string conversions ship out of the box;
// drop your own overload in your module to extend (e.g. `int → MyEnum`).
// Body is empty because every field auto-derives — no per-field overrides needed.
[struct_convert]
def m_v5_to_v6(old : UserV5; var dst : User) {
    pass
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

// Migration 2 — add a column, typed form.
//
// `add_column(type<T>, "FieldName" [, defaultLit])` reads the field on the
// current `[sql_table]` struct: SQL type comes from the daslang type via the
// `_::sql_bind` adapter rail, NOT NULL is derived from the absence of
// `Option<>` wrapping, DEFAULT from the literal you pass. The macro honors
// `@sql_column` rename, `@sql_json`, `@sql_blob`. PK / UNIQUE / generated
// columns can't be ADDed post-hoc — those need a table rebuild
// (chunk 14c, struct_convert).
//
// Raw equivalent (typed form errors at compile time if the field name or
// daslang shape changes; raw form silently drifts):
//
//     db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")

[sql_migration(version = 2, description = "add email column")]
def migration_002(db : SqlRunner) {
    db |> add_column(type<User>, "Email", "")
}

// Migration 3 — typed ADD COLUMN + raw DML. All run inside the call's
// single transaction; failure of any one rolls back the whole call.
// `analyze=true` on the annotation runs `ANALYZE` once after the call
// commits — refreshes stats after the column add. VACUUM works the same
// way (cheap-then-expensive ordering: ANALYZE, then VACUUM).

[sql_migration(version = 3, description = "add score; backfill", analyze = true)]
def migration_003(db : SqlRunner) {
    db |> add_column(type<User>, "Score", 0)
    db |> exec("UPDATE users SET Score = 100 WHERE Email != ''")
}

// Migration 4 — typed CREATE INDEX. Auto-name follows [sql_index]:
// `idx_<table>_<col1>_<col2>`, on the post-`@sql_column` SQL identifiers.
// Pass a custom name as the third positional arg if you want one. For
// composite indexes, pass a tuple: ("Email", "Name").

[sql_migration(version = 4, description = "index Email")]
def migration_004(db : SqlRunner) {
    db |> create_index(type<User>, "Email")
}

// Migration 5 — UNIQUE composite + idempotent rebuild via
// `drop_index_if_exists` (SQLite supports IF EXISTS natively, so a missing
// name is a clean no-op). Pattern: drop → create — safe if a previous
// version already had the index, safe if it didn't.

[sql_migration(version = 5, description = "unique (Email, Name)")]
def migration_005(db : SqlRunner) {
    db |> drop_index_if_exists("ux_email_name")
    db |> create_unique_index(type<User>, ("Email", "Name"), "ux_email_name")
}

// Migration 6 — schema REBUILD via convert_and_rename.
//
// Adds LastLoginAt to User. Strictly speaking, `add_column(type<User>,
// "LastLoginAt")` would also work for this case (SQLite handles ADD COLUMN
// on nullable types fine). Migration 6 uses convert_and_rename anyway to
// demonstrate the rebuild shape, which IS the only path for the cases ALTER
// can't reach: PK changes, type narrowing, FK alterations, CHECK changes.
//
// The dance:
//   1. CREATE TABLE users_new with the new shape.
//   2. SELECT every row of `users` (read via UserV5 — the legacy struct),
//      run it through m_v5_to_v6 (auto-derived field copy + LastLoginAt
//      defaults to none()), INSERT into users_new.
//   3. DROP TABLE users.
//   4. ALTER TABLE users_new RENAME TO users.
// Runs inside migrate_to_latest's α-shape transaction; failure rolls back.

[sql_migration(version = 6, description = "rebuild: add LastLoginAt")]
def migration_006(db : SqlRunner) {
    db |> convert_and_rename(type<UserV5>, type<User>)
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

        // ================================================================
        // PART 3.5 — Typed ALTER: when daslang has enough info to validate
        // ================================================================
        //
        // Migrations 2-5 above use the typed surface in `daslib/sqlite_boost`:
        //
        //   db |> add_column(type<T>, "Field" [, defaultLit])
        //   db |> create_index(type<T>, "Field" | ("A","B") [, "name"])
        //   db |> create_unique_index(type<T>, … same shape …)
        //   db |> drop_index_if_exists("name")
        //
        // What the typed forms buy you:
        //
        //  1. Compile-time field-name checks. `add_column(type<T>, "Emial")`
        //     fails at compile time, not on the user's DB at startup.
        //  2. Type-derived NOT NULL: forgetting NOT NULL+DEFAULT used to
        //     mean existing rows panic on the next read. The macro derives
        //     nullability from `Option<>` and refuses to ADD a non-nullable
        //     column without a literal default.
        //  3. `@sql_column` rename + `@sql_json` / `@sql_blob` storage are
        //     honored automatically — same conventions as `[sql_table]` /
        //     `[sql_index]`, no double-bookkeeping.
        //
        // What stays on `db |> exec(…)` (raw SQL):
        //
        //   - DROP COLUMN / RENAME COLUMN — old names aren't in the current
        //     struct, so daslang has nothing to validate against.
        //   - PK / UNIQUE inline / generated columns added post-hoc — SQLite
        //     can't ALTER these in place; needs a table rebuild — see
        //     PART 5 below for the struct_convert + convert_and_rename path.
        //   - CHECK constraints, FK ADD/DROP, column type changes.
        //   - Anything ad-hoc that doesn't map to a struct field.

        // ================================================================
        // PART 5 — Schema rebuild: verify migration 6 landed
        // ================================================================
        //
        // Migration 6 ran inside migrate_to_latest. Two checks:
        //   - The new column is on the table.
        //   - alice survived the rebuild (Email preserved).

        let last_login_count = db |> query_scalar(
            "SELECT COUNT(*) FROM pragma_table_info('users') WHERE name='LastLoginAt'", type<int>)
        print("post-rebuild LastLoginAt column present: {last_login_count == 1}\n")
        let alice_post_rebuild = db |> query_scalar(
            "SELECT Email FROM users WHERE Name='alice'", type<string>)
        print("alice survived rebuild: Email={alice_post_rebuild}\n")

        // GOTCHA: indexes created at runtime (migrations 4 and 5 used
        // `create_index` / `create_unique_index`) are NOT reflected in
        // [sql_index] annotations on the User struct, so convert_and_rename's
        // `create_table(type<User>, "users_new")` doesn't recreate them.
        // After the rebuild, the indexes are gone:
        let n_idx = db |> query_scalar(
            "SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND tbl_name='users' AND name NOT LIKE 'sqlite_%'",
            type<int>)
        print("user indexes after rebuild: {n_idx} (gone — see note above)\n")
        // Two ways to avoid this:
        //   - Declare indexes via [sql_index(...)] siblings on the struct so
        //     create_table emits them during convert_and_rename.
        //   - Recreate indexes inside migration_006 after convert_and_rename
        //     via create_index / create_unique_index calls.
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
