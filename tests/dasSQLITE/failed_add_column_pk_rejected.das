options gen2

// AddColumnMacro must reject @sql_primary_key fields — SQLite cannot add a PK column
// to an existing table; this requires a table rebuild (chunk 14c struct_convert).
expect 40104:1

require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    @sql_primary_key
    Id : int64
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> add_column(type<User>, "Id")
}
