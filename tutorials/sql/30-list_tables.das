options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 30 — listing tables in a database.
//
// "What tables exist in this DB?" is genuinely provider-specific:
//   SQLite : SELECT name FROM sqlite_master WHERE type='table'
//   PG     : SELECT tablename FROM pg_tables WHERE schemaname='public'
//   MySQL  : SELECT table_name FROM information_schema.tables
//   MSSQL  : SELECT name FROM sys.tables
//
// The catalog spelling is different per backend — and the columns it
// surfaces (system tables, virtual tables, partitions, schemas) diverge
// in non-trivial ways. dasSQLITE does NOT ship a `db |> tables() :
// array<string>` abstract helper for this reason: a uniform-subset
// contract would either lie about the differences or grow per-provider
// escape hatches.
//
// Instead, listing tables uses the same raw-SQL escape hatch as tut 29
// Band 3: the typed `query(sql, type<T>, …)` family. No new API surface;
// the SQL is provider-specific, the daslang side is uniform.

// `name` is reserved enough across catalogs that we can use it directly;
// `type` is a daslang/SQL keyword so we rename via @sql_column. The rename
// is positional-only here (raw `query` materializes by column index, not
// name), but the same annotation drives DDL / `_sql` / `column_info` /
// INSERT-UPDATE-DELETE-upsert when the struct is used with the chained
// API — see tut 29 Band 3 for the broader rename semantics. The struct
// stays minimal — projecting only what we need keeps the SQL honest
// about what's portable. `[sql_table]` opts the row shape into the
// materializer rail without claiming a real underlying table.
[sql_table(name = "sqlite_master_rows")]
struct MasterTable {
    @sql_column = "type"
    row_type : string
    name : string
}

// A second shape for the full sqlite_master payload — `tbl_name` /
// `rootpage` / `sql` round out what the catalog stores. Useful for
// inspection tools that want the original DDL the table was created
// with.
[sql_table(name = "sqlite_master_full")]
struct MasterFull {
    @sql_column = "type"
    row_type : string
    name : string
    tbl_name : string
    rootpage : int
    sql : string
}

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "Owners")]
struct Owner {
    @sql_primary_key Id : int
    Name : string
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> create_table(type<Owner>)
        db |> exec("CREATE INDEX idx_cars_name ON Cars(Name)")
        db |> exec("CREATE VIEW recent_cars AS SELECT * FROM Cars LIMIT 10")

        // --- Just the user tables --------------------------------------
        // sqlite_master `type` distinguishes 'table' / 'index' / 'view' /
        // 'trigger'. Filter for tables to skip the index + view we
        // created above.
        for (t in db |> query(
                "SELECT type, name FROM sqlite_master WHERE type='table' ORDER BY name",
                type<MasterTable>)) {
            to_log(LOG_INFO, "{t.row_type}: {t.name}\n")
        }

        // --- All catalog rows, including indexes / views ---------------
        // Drop the WHERE filter to see the full picture; SQLite stores
        // every schema object here, indexed by `type`.
        to_log(LOG_INFO, "--- full catalog ---\n")
        for (m in db |> query(
                "SELECT type, name, tbl_name, rootpage, sql FROM sqlite_master ORDER BY type, name",
                type<MasterFull>)) {
            to_log(LOG_INFO, "{m.row_type} {m.name} (tbl={m.tbl_name})\n")
        }

        // --- Parameterized listing — name predicate via positional bind
        // The `query` family ships 0/1/2/3 positional-arg variants;
        // raw-SQL placeholders (`?`) get bound left-to-right.
        let prefix = "C"
        for (t in db |> query(
                "SELECT type, name FROM sqlite_master WHERE type='table' AND name LIKE ?",
                type<MasterTable>, "{prefix}%")) {
            to_log(LOG_INFO, "user table starting with '{prefix}': {t.name}\n")
        }
    }
}

// --- Why no abstract `list_tables` helper -----------------------------
//
// EF Core deliberately does not expose a runtime "what tables physically
// exist" call either: `context.Model.GetEntityTypes()` lists the
// [sql_table]-equivalents YOUR CODE declares, not what the DB actually
// stores. Genuine DB-driven listing drops to design-time scaffolding
// or raw ADO.NET. Same trade-off here.
//
// Rule of thumb: if your code knows the schema, drive off the daslang
// types — `column_info(type<T>)` (tut 29) and the [sql_table] structs
// you already declared. If the schema lives in the DB and you're
// admin-tooling against it, `query(provider_specific_sql, type<Row>)`
// is the honest spelling.

// --- DEFERRED ---------------------------------------------------------
//
//   1. Cross-provider catalog query helper (tables / indexes / views).
//      Could ship as `daslib/sql_admin` once a second provider lands
//      and the lowest-common-denominator subset is concrete. Premature
//      to design with one backend in hand.
//
//   2. Schema-diff helpers (compare DB catalog ↔ declared `[sql_table]`
//      types). Useful for migration tooling but pushes design choices
//      around what counts as a "diff" (column renames vs. drop+add,
//      index name normalization, view body equivalence). Defer until
//      a real migration story is on the table.
