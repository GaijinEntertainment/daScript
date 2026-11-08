options gen2
//! schema_from naming a table that is not in the database must not
//! compile

// `name=` references a table that doesn't exist in the .db.
expect 20800

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Nonexistent", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct Bogus {}
