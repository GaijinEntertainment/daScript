options gen2

// `[sql_index]` on a `legacy=true` struct is rejected: legacy structs are read-only
// and don't define schema, so the index would have no CREATE TABLE to attach to and
// would collide with the current struct's index of the same name on rebuild.
expect 20800

require sqlite/sqlite_migrate

[sql_table(name = "users", legacy = true),
 sql_index(name = "ux_users_name", fields = "Name", unique)]
struct UserV1 {
    @sql_primary_key Id : int
    Name : string
}
