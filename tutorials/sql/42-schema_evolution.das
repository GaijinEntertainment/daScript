options gen2

// One ETL script handling multiple historical DB versions.
//
// Imagine you've got `Logs` databases in the wild at v1 (no Severity column)
// and v2 (Severity added). You want a single program that reads from either,
// normalizes, and writes to a fresh DB. With `[sql_table(schema_from=...)]`
// the v1/v2 structs come from the actual schemas; with
// `typeinfo has_field<...>` + `static_if` one function body handles both
// at compile time. Drift in either source DB stops the build at the exact
// lines that need updating.
//
// See tutorial 39 for `schema_from` basics. This tutorial is the wip.das
// pattern — an ETL/migration use case schema_from enables for free.

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// One struct per known historical shape. Both name="Logs" — they describe the
// same logical table at different points in its history.
[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v1.db")]
struct OldLogV1 {}

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct OldLogV2 {}

// The current shape — hand-declared, lives in code, evolves with the app.
[sql_table(name = "Logs")]
struct LogEntry {
    @sql_primary_key Id : int64
    Severity : int64       // 0 = trace, 1 = info, 2 = warn, 3 = error
    Message  : string
}

// `static_if (typeinfo has_field<Severity>(type<TT>))` lets ONE generic
// migration body handle both v1 and v2 sources. Compile-time branching;
// no runtime cost. If a future DB version drops Severity, the v1 branch
// kicks in automatically — and if it removes a field LogEntry still expects
// (Id or Message), the typing fails to compile at the assignment line.
def migrate_one_row(src : auto(TT)) : LogEntry {
    var dst : LogEntry
    dst.Id = src.Id
    dst.Message = src.Message
    static_if (typeinfo has_field<Severity>(type<TT>)) {
        dst.Severity = src.Severity
    } else {
        dst.Severity = 1l         // backfill: missing-Severity rows treated as info
    }
    return dst
}

[export]
def main() {
    // New DB lives in memory for this demo. In production it'd be a real path.
    with_sqlite(":memory:") $(newDb) {
        newDb |> create_table(type<LogEntry>)

        // Stream from a v1-shape source, normalize, insert.
        with_sqlite("tests/dasSQLITE/test_data/schema_from_v1.db") $(oldDb) {
            // `_each_sql` is the streaming iterator — one row at a time, no
            // full-array materialization. Important for large old DBs.
            for (e in _each_sql(oldDb |> select_from(type<OldLogV1>))) {
                newDb |> insert(migrate_one_row(e))
            }
        }
        // ... and from a v2-shape source. Same `migrate_one_row`, different
        // compile-time branch.
        with_sqlite("tests/dasSQLITE/test_data/schema_from_v2.db") $(oldDb) {
            for (e in _each_sql(oldDb |> select_from(type<OldLogV2>))) {
                newDb |> insert(migrate_one_row(e))
            }
        }

        // ... and the resulting current-shape DB is ready to query.
        let rows <- _sql(newDb |> select_from(type<LogEntry>))
        print("migrated {length(rows)} row(s) into the unified shape\n")
    }
}

// --- When this is the right tool ------------------------------------------
//
// schema_from + typeinfo has_field is for ETL, archival readers, one-shot
// data movement scripts. The script ships with the historical .db schemas
// baked in. If a new historical version surfaces, you add a third struct +
// a third loop and the type system tells you exactly where to wire it up.
//
// For "my app's schema evolves at runtime; I run migrations at startup",
// see `daslib/sql_migrate` (coming soon) — that's a different problem with
// a different shape: versioned `[sql_migration(version=N)]` functions
// applied in order, tracked in `__schema_version`, transactional per
// migration. The two patterns coexist: schema_from for code-on-current-
// schema, sql_migrate for the schema-grows-over-time runner.
