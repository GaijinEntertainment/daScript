options gen2

// [sql_index] validation rules introduced in chunk 7. Each struct triggers
// exactly one error; macro [sql_index] failures emit error 30111.
expect 20800:4

require daslib/sql
require sqlite/sqlite_boost

// 1. [sql_index] without `fields=` — at least one column is required.
//    Pass only `name=` to omit fields=.
[sql_table(name = "I1"),
 sql_index(name = "ix_nofields")]
struct I1 {
    @sql_primary_key Id : int
    Name : string
}

// 2. [sql_index] referencing a column that is not in the struct.
[sql_table(name = "I2"),
 sql_index(fields = ("Bogus"))]
struct I2 {
    @sql_primary_key Id : int
    Name : string
}

// 3. [sql_index] composite with one valid + one bogus column — same error.
[sql_table(name = "I3"),
 sql_index(fields = ("Name", "Bogus"))]
struct I3 {
    @sql_primary_key Id : int
    Name : string
}

// 4. [sql_index] before [sql_table] in source order — the helper function
//    isn't in scope yet, so the body-rewrite cannot proceed.
[sql_index(fields = ("Name")),
 sql_table(name = "I4")]
struct I4 {
    @sql_primary_key Id : int
    Name : string
}

[export]
def main() {}
