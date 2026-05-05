options gen2

// `name=` references a table that doesn't exist in the .db.
expect 30111:1

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Nonexistent", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct Bogus {}
