options gen2

// _sql_pragma / _sql_vacuum_into argument-shape errors. Each line triggers
// exactly one macro_error (40104).
expect 40104:5

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[export]
def main {
    with_sqlite(":memory:") $(db) {
        // 1. _sql_pragma with int64 value — must use try_set_pragma instead.
        db |> _sql_pragma("busy_timeout", 1500l)
        // 2. _sql_pragma with bool value — must use try_set_pragma instead.
        db |> _sql_pragma("foreign_keys", true)
        // 3. _sql_pragma with non-string name.
        db |> _sql_pragma(42, "value")
        // 4. _sql_pragma with wrong arg count.
        db |> _sql_pragma("only_two_args")
        // 5. _sql_vacuum_into with non-string path.
        db |> _sql_vacuum_into(123l)
    }
}
