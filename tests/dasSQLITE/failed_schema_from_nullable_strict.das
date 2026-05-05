options gen2

// User declares non-optional `string` against a nullable TEXT column → compile error.
expect 30111:1

require daslib/sql
require sqlite/sqlite_boost

// nullable.db has `Note TEXT` (nullable). User declares Note as plain string (NOT NULL).
[sql_table(name = "Items", schema_from = "tests/dasSQLITE/test_data/schema_from_nullable.db")]
struct ItemBadNullable {
    @sql_primary_key Id : int64
    Note : string
}
