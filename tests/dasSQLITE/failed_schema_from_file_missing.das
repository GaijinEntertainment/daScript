options gen2

// `schema_from=` points to a path that doesn't exist.
expect 30111:1

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Whatever", schema_from = "tests/dasSQLITE/test_data/does_not_exist.db")]
struct Whatever {}
