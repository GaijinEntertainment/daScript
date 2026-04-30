options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 36 — ATTACH DATABASE: cross-DB queries on one connection.
//
// SQLite-only. ATTACH lets a single connection see multiple database
// files at once, each addressed by a schema qualifier. Real use cases:
//
//   - Per-tenant DBs (`users/<id>.db`) attached at request time.
//   - Hot table in `app.db`, cold history in `archive.db`.
//   - Import / export — attach a source, INSERT … SELECT, detach.
//
// API surface (all in `sqlite/sqlite_boost`):
//
//   attach     / try_attach     — runtime ATTACH DATABASE
//   detach     / try_detach     — runtime DETACH DATABASE
//   with_schema(db, name)       — runner copy that qualifies queries
//                                  with `"<name>"."<table>"`
//   with_attached(db, path, as_name) <| $(scoped) { … }
//                                — attach + invoke + detach (covers panic)
//
// `:memory:` is a valid attach target — each `:memory:` attach creates
// a fresh, separate in-memory DB. The tutorial uses that so it has no
// external file dependencies.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name                : string
    Active              : bool
}

def fixture_main(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Name = "Alice",   Active = true))
    db |> insert(User(Id = 2, Name = "Bob",     Active = true))
}

def fixture_archive(arch : SqlRunner) {
    arch |> create_table(type<User>)
    arch |> insert(User(Id = 100, Name = "Carol-archived", Active = false))
    arch |> insert(User(Id = 101, Name = "Dave-archived",  Active = false))
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture_main(db)

        // --- attach + with_schema (manual) --- nolint:STYLE014
        // `attach` is a runtime DDL: ATTACH DATABASE ? AS "<as_name>".
        // `with_schema(db, name) $(scoped) { ... }` invokes the body with a
        // schema-qualified runner that emits `"<name>"."<table>"`. Block-shaped
        // so the qualified runner can't outlive its lexical scope (it shares
        // db's libsqlite3 handle; only the original owns the lifetime).
        db |> attach(":memory:", "archive")
        db |> with_schema("archive") $(arch) {
            fixture_archive(arch)
            let archived <- _sql(arch |> select_from(type<User>) |> _where(!_.Active))
            to_log(LOG_INFO, "archive has {length(archived)} inactive rows")
            for (u in archived) {
                to_log(LOG_INFO, "  archived: {u.Name}")
            }
        }
        db |> detach("archive")

        // --- with_attached block — preferred shape ------------------------
        // Attaches, invokes the body with a schema-qualified runner, and
        // detaches in `finally`. Detach runs even if the body panics.
        db |> with_attached(":memory:", "tenant_42") $(tenant) {
            fixture_archive(tenant)
            for (u in _sql(tenant |> select_from(type<User>))) {
                to_log(LOG_INFO, "  tenant_42: {u.Name}")
            }
        }

        // --- main-schema queries still work after detach ------------------
        let live <- _sql(db |> select_from(type<User>) |> _where(_.Active))
        to_log(LOG_INFO, "main has {length(live)} active rows")

        // --- try_attach for fallible paths --------------------------------
        // Bad path / reserved name / already-attached → some(errmsg).
        let err = db |> try_attach(":memory:", "main")   // "main" is reserved
        to_log(LOG_INFO, "try_attach(\"main\") err: {err}")

        // --- cross-schema queries via raw SQL (v1 escape) --- nolint:STYLE014
        // v1 ships ONE schema per `_sql` chain. To merge across schemas
        // (UNION / cross-schema JOIN), drop to raw `query` with
        // explicitly-qualified table names — `archive.Users`, etc.
        // Typed cross-schema joins are deferred; raw SQL covers v1.
        //
        //     db |> query(
        //         "SELECT Id, Name, Active FROM main.Users
        //          UNION
        //          SELECT Id, Name, Active FROM archive.Users
        //          ORDER BY Id",
        //         type<User>)
        //
        // DDL on an attached schema works through `with_schema` — `_qualify_schema`
        // rewrites CREATE TABLE / CREATE VIRTUAL TABLE / CREATE [UNIQUE] INDEX
        // as well as SELECT / INSERT / UPDATE / DELETE / JOIN. The `arch |>
        // create_table(type<User>)` path used above flows through that rewriter.
    }
}
