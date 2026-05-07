options gen2

// Hand-declared field's type doesn't match the schema column's affinity.
// This fires as a `concept_assert` failure inside the generated `_sql_table_name`
// function — same compile-time chain, different error site.
expect 31400

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Schema says `Severity INTEGER NOT NULL`. User declares it as `string` — TEXT affinity.
// SqlType mismatch is caught at typecheck via the static_if + concept_assert(false) emission.
[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct LogTypeMismatch {
    @sql_primary_key Id : int64
    Severity : string
}

// Force `_sql_table_name` to typecheck — any operation on the struct triggers it.
[export]
def main {
    let _ = column_info(type<LogTypeMismatch>)
    let _name = _::_sql_table_name(default<LogTypeMismatch>)
}
