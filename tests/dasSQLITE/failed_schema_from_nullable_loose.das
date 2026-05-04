options gen2

// User declares Option<int64> against a NOT NULL INTEGER PRIMARY KEY column → compile error.
expect 30111:1

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct LogBadOptionalPk {
    @sql_primary_key Id : Option<int64>
    Severity : int64
    Message : string
}
