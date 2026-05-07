options gen2

// `[struct_convert]` rejects new T fields that are not Option<T> and have no default initializer
// when the user provides no override — there's no auto-derivation rule that can populate them.
expect 20800

require sqlite/sqlite_migrate

[sql_table(name = "users", legacy = true)]
struct UserV1 {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Email : string  // new field, not Option, no default — needs an override
}

[struct_convert]
def my_v1_to_v2(old : UserV1; var dst : User) {
    pass
}
