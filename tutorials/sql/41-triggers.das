options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 41 — triggers: DB-level callbacks via raw SQL.
//
// Concept-only tutorial — no new API. SQLite triggers are SQL
// statements that fire automatically before/after INSERT, UPDATE,
// or DELETE on a table. dasSQLITE deliberately does NOT add a
// daslang-side trigger DSL. Reasons:
//
//   - Triggers run inside the SQL engine. There is no daslang
//     callback, no row marshalling, no daslang-side type checking.
//     A typed wrapper would be a thin facade over `exec("CREATE
//     TRIGGER …")` — extra surface, no extra safety.
//   - Trigger bodies are SQL fragments. Cross-provider semantics
//     diverge sharply (PostgreSQL trigger functions, MySQL
//     `BEFORE` vs `AFTER` reloading, MSSQL inserted/deleted
//     pseudo-tables). A portable abstraction would lose the parts
//     people actually use triggers for.
//   - Schema migrations are the right home for trigger DDL — see
//     tutorial 32 (`[sql_migration]` + `daslib/sqlite_migrate`). Each
//     migration can `exec` a `CREATE TRIGGER` so the trigger
//     ships with the schema version that needs it.

[sql_table(name = "Articles")]
struct Article {
    @sql_primary_key Id : int
    Title               : string
    UpdatedAt           : int64    // unix seconds
}

[sql_table(name = "AuditLog")]
struct AuditEntry {
    @sql_primary_key Id : int
    Op                  : string   // "INSERT" / "UPDATE" / "DELETE"
    ArticleId           : int
    AtUnix              : int64
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> apply_recommended_pragmas()
        db |> create_table(type<Article>)
        db |> create_table(type<AuditEntry>)

        // --- AFTER INSERT trigger: stamp UpdatedAt + write audit row -----
        // Raw `exec`. The trigger lives in SQLite from this point on —
        // every subsequent INSERT into "Articles" fires it.
        db |> exec(
            "CREATE TRIGGER articles_audit_insert
             AFTER INSERT ON \"Articles\"
             BEGIN
                UPDATE \"Articles\" SET \"UpdatedAt\" = strftime('%s', 'now')
                    WHERE \"Id\" = NEW.\"Id\";
                INSERT INTO \"AuditLog\" (\"Op\", \"ArticleId\", \"AtUnix\")
                    VALUES ('INSERT', NEW.\"Id\", strftime('%s', 'now'));
             END")

        // --- AFTER UPDATE trigger ----------------------------------------
        // `OLD` and `NEW` are SQLite's row-shape pseudonyms (no quoting
        // — they're SQL keywords). Re-stamp UpdatedAt on every change.
        db |> exec(
            "CREATE TRIGGER articles_audit_update
             AFTER UPDATE ON \"Articles\"
             BEGIN
                UPDATE \"Articles\" SET \"UpdatedAt\" = strftime('%s', 'now')
                    WHERE \"Id\" = NEW.\"Id\";
                INSERT INTO \"AuditLog\" (\"Op\", \"ArticleId\", \"AtUnix\")
                    VALUES ('UPDATE', NEW.\"Id\", strftime('%s', 'now'));
             END")

        // --- Exercise the triggers via the typed daslang API --- nolint:STYLE014
        // INSERT and UPDATE on Article go through `db |> insert(...)` /
        // `_sql_update(...)`. The triggers fire transparently — daslang
        // never sees them.
        db |> insert(Article(Id = 1, Title = "Hello", UpdatedAt = 0l))
        db |> insert(Article(Id = 2, Title = "World", UpdatedAt = 0l))
        db |> _sql_update(type<Article>, _.Id == 1, (Title = "Hello (revised)"))

        // The audit log got two INSERTs and one UPDATE — and Article rows
        // 1 and 2 both have UpdatedAt set, courtesy of the trigger.
        let log <- _sql(db |> select_from(type<AuditEntry>)
                          |> _order_by(_.Id))
        for (entry in log) {
            to_log(LOG_INFO, "audit #{entry.Id}: {entry.Op} article {entry.ArticleId} @ {entry.AtUnix}")
        }

        let arts <- _sql(db |> select_from(type<Article>) |> _order_by(_.Id))
        for (a in arts) {
            to_log(LOG_INFO, "article {a.Id}: '{a.Title}' updated_at={a.UpdatedAt}")
        }
    }
}

// --- DROP / REPLACE --- nolint:STYLE014
//
//     db |> exec("DROP TRIGGER IF EXISTS articles_audit_insert")
//
// Replace = drop + recreate. The lifecycle is migration-shaped, which is
// why migrations (tutorial 32) is the natural home for trigger DDL.
//
// --- THE TYPICAL ANTI-PATTERN ------------------------------------------
//
// "Trigger that calls back into application code." It can't. SQLite
// triggers run inside the engine; there's no `RAISE_EVENT` that the
// daslang side observes. If you need a side effect outside the DB,
// commit and queue the work in your application code on the path
// that wrote the row — don't try to do it from a trigger.
