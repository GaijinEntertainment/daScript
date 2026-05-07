options gen2

// User attaches @sql_primary_key to a column the schema does NOT report as PK.
expect 20800

require daslib/sql
require sqlite/sqlite_boost

// nullable.db's Note column is not PK. User claims it is.
[sql_table(name = "Items", schema_from = "tests/dasSQLITE/test_data/schema_from_nullable.db")]
struct ItemBadPk {
    @sql_primary_key Note : Option<string>
}
