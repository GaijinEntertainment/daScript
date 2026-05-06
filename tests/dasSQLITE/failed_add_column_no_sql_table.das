options gen2

// AddColumnMacro must reject a struct without [sql_table] — there's no table
// identifier to ALTER, so the call is meaningless.
expect 40104:1

require sqlite/sqlite_migrate

struct Plain {
    Id : int64
    Name : string
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> add_column(type<Plain>, "Name")
}
