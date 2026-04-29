options gen2

// `@sql_column` validation. Each struct below trips one of the rejection
// paths in `[sql_table]`'s field-annotation reader:
//   1. Empty value (`@sql_column = ""`).
//   2. Embedded `"` (SQL identifier hygiene).
//   3. Embedded `\` (SQL identifier hygiene).
//   4. Cross-field collision — two fields produce the same SQL column name.
//   5. Cross-field collision — a `@sql_column` rename collides with the
//      daslang name of a sibling field.
//
// All five fail with 30111 ("macro [sql_table] failed to apply").
expect 30111:5

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "T1")]
struct T1 {
    @sql_primary_key Id : int
    @sql_column = ""              //!< empty value rejected
    Name : string
}

[sql_table(name = "T2")]
struct T2 {
    @sql_primary_key Id : int
    @sql_column = "ev\"il"        //!< embedded `"` rejected
    Name : string
}

[sql_table(name = "T3")]
struct T3 {
    @sql_primary_key Id : int
    @sql_column = "ev\\il"        //!< embedded `\` rejected
    Name : string
}

[sql_table(name = "T4")]
struct T4 {
    @sql_primary_key Id : int
    @sql_column = "x"
    a : string
    @sql_column = "x"             //!< two fields rename to the same SQL column
    b : string
}

[sql_table(name = "T5")]
struct T5 {
    @sql_primary_key Id : int
    @sql_column = "kind"          //!< rename collides with sibling field's daslang name
    code : string
    kind : string
}
