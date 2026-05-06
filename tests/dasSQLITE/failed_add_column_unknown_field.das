options gen2

// AddColumnMacro must reject a field name that doesn't exist on the struct,
// with the same wording the [sql_index] structure annotation uses.
expect 40104:1

require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> add_column(type<User>, "NotARealField")
}
