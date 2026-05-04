options gen2

// `[sql_table(schema_from = "path.db")]` opens the .db at compile time, reads
// `pragma_table_info`, and populates the struct's fields from the actual schema.
// The struct mirrors the database — which means schema drift becomes a COMPILE
// error at the exact lines that need updating. No reflection, no migration
// metadata, no model snapshots; the type system carries the contract.
//
// SQLite affinity → daslang type:
//   INTEGER → int64        REAL → double         TEXT → string
//   BLOB    → array<uint8> NUMERIC → double      (NUMERIC: BOOLEAN/DATE land here too;
//                                                 override via partial body if you need
//                                                 different types)
// `NOT NULL` columns → `T`; otherwise → `Option<T>`.
// `INTEGER PRIMARY KEY` columns get `@sql_primary_key` synthesized.
//
// Path resolution:
//   - absolute path → as-is
//   - relative path → resolved under daslang's project root (see `get_das_root()`)
//
// AOT note: the macro's output is part of the struct's semantic hash. If the
// .db schema changes between AOT-gen and runtime, the AOT link fails and the
// runtime falls back to the interpreter — no silent miscompile.

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// --- Empty body: every field synthesized from the .db ---------------------
//
// schema_from_v2.db has:  Logs(Id INTEGER PRIMARY KEY, Severity INTEGER NOT NULL, Message TEXT NOT NULL)
// → struct LogEntry gets: { @sql_primary_key Id : int64; Severity : int64; Message : string }
[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct LogEntry {}

// --- Partial body: hand-declared fields are CONTRACTS --------------------
//
// `Id : int` against an INTEGER PK column compiles — same affinity, narrower
// daslang type. `@sql_json` against a TEXT column tells the macro to bind/extract
// via JSON encoding. `@sql_blob` does the same for binary BLOB columns.
//
// What you CANNOT do: declare a `string` against an INTEGER column, or a plain
// `T` against a nullable column, or attach `@sql_primary_key` to a non-PK column.
// Each of those fails at compile time with a message naming the field and column.

struct Note {
    title : string
    rank  : int
}

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct LogEntryRefined {
    // Id : int (narrowed from INTEGER → int instead of synthesized int64).
    // Severity and Message left to schema_from synthesis.
    @sql_primary_key Id : int
}

[export]
def main() {
    // Compile-time introspection: `column_info(type<T>)` walks the struct's
    // fields. For schema_from-derived structs, those fields came from the .db.
    let cols = column_info(type<LogEntry>)
    print("LogEntry columns (schema_from synthesized):\n")
    for (c in cols) {
        print("  {c.name} : {sqlite_sql_type(c.data_type)} (pk={c.is_pk}, nullable={c.is_nullable})\n")
    }

    // Runtime: open the actual DB and read.
    with_sqlite("tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        // `check_schema` is the runtime startup-defense pattern. With schema_from
        // it's a tautology against the source DB — but if the user passes a
        // DIFFERENT DB at runtime, this catches it.
        db |> check_schema(type<LogEntry>)

        let rows <- _sql(db |> select_from(type<LogEntry>))
        print("read {length(rows)} row(s) from Logs\n")
    }
}

// --- When NOT to use schema_from ------------------------------------------
//
// schema_from is for "code on the current schema": the DB's shape today is
// what the script reads/writes. ETL between two DBs, archival readers,
// admin tooling — all good fits.
//
// For "the DB grows over time, run versioned schema migrations at startup",
// `daslib/sql_migrate` (separate, optional module — coming soon) ships a
// `[sql_migration(version=N)]` annotation + `migrate_to_latest(db)` runner.
// The two are orthogonal — schema_from gives compile-time contract checks
// against a known schema; sql_migrate evolves the schema over time.
