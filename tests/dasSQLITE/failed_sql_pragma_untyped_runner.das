options gen2
expect 50503, 30838

// _sql_pragma reports a missing provider, not an exception, when its
// runner argument has no type

require daslib/sql_linq
require sqlite/sqlite_boost

[export]
def main() {
    let err = no_such_db |> _sql_pragma("journal_mode", "MEMORY")
    print("{err}\n")
}
