options gen2

// CreateIndexMacro must reject an empty-string field name — equivalent to "fields=()".
// Mirrors [sql_index]'s "missing or empty `fields=`" error.
expect 40104:1

require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    Id : int64
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> create_index(type<User>, "")
}
