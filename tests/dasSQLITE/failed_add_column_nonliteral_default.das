options gen2

// AddColumnMacro requires the DEFAULT to be a compile-time literal: int / float /
// double / bool / string. Runtime expressions (function calls, variables, arithmetic)
// are rejected with a fixit pointing to db |> exec(…).
expect 40104:1

require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Score : int
}

def compute_default() : int {
    return 42
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> add_column(type<User>, "Score", compute_default())
}
