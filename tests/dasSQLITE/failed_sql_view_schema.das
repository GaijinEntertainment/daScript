options gen2

// Each struct intentionally violates one [sql_view] schema-validation rule.
// Each failure produces error 30111 = "macro [sql_view] failed to apply to
// the structure". Validation is fail-fast (first error aborts that struct's
// apply()) so each struct contributes exactly one error.
expect 20800:9

require daslib/sql
require sqlite/sqlite_boost

// 1. @sql_primary_key — views are read-only and don't declare keys.
[sql_view(name = "V1")]
struct V1 {
    @sql_primary_key Id : int
    Name : string
}

// 2. @sql_unique — uniqueness belongs to the underlying table.
[sql_view(name = "V2")]
struct V2 {
    Id : int
    @sql_unique Name : string
}

// 3. @sql_computed — view body expressions live inside _create_view, not
//    GENERATED ALWAYS columns.
[sql_view(name = "V3")]
struct V3 {
    Id : int
    @sql_computed = "Id * 2" Doubled : int
}

// 4. @sql_default_fn — defaults belong to the underlying table.
[sql_view(name = "V4")]
struct V4 {
    Id : int
    @sql_default_fn = "CURRENT_TIMESTAMP" CreatedAt : string
}

// 5. Daslang field initializer — view fields are read-only projections.
[sql_view(name = "V5")]
struct V5 {
    Id : int
    Name : string = "default"
}

// 6. @sql_references — FKs belong to the underlying table.
[sql_table(name = "Parent")]
struct Parent {
    @sql_primary_key Id : int
}
[sql_view(name = "V6")]
struct V6 {
    Id : int
    @sql_references = "Parent" ParentId : int
}

// 7. @sql_on_delete — FK action belongs to the underlying table.
[sql_view(name = "V7")]
struct V7 {
    Id : int
    @sql_on_delete = "cascade" ParentId : int
}

// 8. Both @sql_json and @sql_blob on the same field.
[sql_view(name = "V8")]
struct V8 {
    Id : int
    @sql_json @sql_blob Payload : tuple<a : int; b : string>
}

// 9. @sql_column collision — two fields renaming to the same SQL column.
[sql_view(name = "V9")]
struct V9 {
    @sql_column = "name" A : string
    @sql_column = "name" B : string
}
