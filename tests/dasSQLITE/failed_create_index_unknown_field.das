options gen2

// CreateIndexMacro must reject field names that don't exist on the struct,
// with the same wording the [sql_index] structure annotation uses.
expect 50503

require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
    Email : string
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> create_index(type<User>, "NotAField")
}
