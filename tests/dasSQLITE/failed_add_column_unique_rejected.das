options gen2

// AddColumnMacro must reject @sql_unique fields — SQLite cannot add a UNIQUE
// column to an existing table inline; the user should use create_unique_index instead.
expect 50503

require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    @sql_unique
    Email : string
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> add_column(type<User>, "Email", "")
}
