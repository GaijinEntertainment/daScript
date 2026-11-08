options gen2
//! add_column on a struct without sql_table must not compile because
//! there is no table to alter

// AddColumnMacro must reject a struct without [sql_table] — there's
// no table identifier to ALTER, so the call is meaningless.
expect 50503

require sqlite/sqlite_migrate

struct Plain {
    Id : int64
    Name : string
}

[sql_migration(version = 1)]
def m_001(db : SqlRunner) {
    db |> add_column(type<Plain>, "Name")
}
