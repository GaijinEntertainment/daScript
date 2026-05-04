options gen2

// Hand-declared field that doesn't exist in the schema_from .db → compile error.
expect 30111:1

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct LogBogus {
    @sql_primary_key Id : int
    Bogus : int      // not in v2.db
}
