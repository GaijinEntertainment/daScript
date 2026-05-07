options gen2

// Each struct below intentionally violates one [sql_table] schema-validation
// rule introduced in chunk 7 (defaults / computed / foreign keys). Each
// failure produces error 30111 = "macro [sql_table] failed to apply to the
// structure". Validation is fail-fast (first error aborts that struct's
// apply()) so each struct contributes exactly one error.
expect 20800:12

require daslib/sql
require sqlite/sqlite_boost

// 1. @sql_computed on a @sql_primary_key column — SQLite rejects PRIMARY KEY
//    on a generated column.
[sql_table(name = "C1")]
struct C1 {
    @sql_primary_key
    @sql_computed = "1"
    Id : int
}

// 2. @sql_computed combined with a daslang field initializer — the SQL
//    expression is the only source of value for a computed column.
[sql_table(name = "C2")]
struct C2 {
    @sql_primary_key Id : int
    @sql_computed = "Id * 2"
    DoubleId : int = 7
}

// 3. @sql_computed combined with @sql_default_fn — defaults don't apply to
//    generated columns.
[sql_table(name = "C3")]
struct C3 {
    @sql_primary_key Id : int
    @sql_computed = "Id * 2"
    @sql_default_fn = "CURRENT_TIMESTAMP"
    DoubleId : int
}

// 4. Native field initializer combined with @sql_default_fn — pick one.
[sql_table(name = "C4")]
struct C4 {
    @sql_primary_key Id : int
    @sql_default_fn = "CURRENT_TIMESTAMP"
    CreatedAt : string = "fixed"
}

// 5. @sql_default_fn outside the supported whitelist (CURRENT_TIMESTAMP /
//    CURRENT_DATE / CURRENT_TIME).
[sql_table(name = "C5")]
struct C5 {
    @sql_primary_key Id : int
    @sql_default_fn = "now()"
    Ts : string
}

// 6. @sql_on_delete without @sql_references — FK actions only apply to a
//    foreign-key relationship.
[sql_table(name = "C6")]
struct C6 {
    @sql_primary_key Id : int
    @sql_on_delete = "cascade"
    OrphanFk : int
}

// 7. @sql_on_update without @sql_references — same rule, update side.
[sql_table(name = "C7")]
struct C7 {
    @sql_primary_key Id : int
    @sql_on_update = "cascade"
    OrphanFk : int
}

// 8. @sql_on_delete action outside the whitelist.
//    Need a real referenced struct so we hit the action check, not the
//    "missing reference" branch.
[sql_table(name = "C8Parent")]
struct C8Parent {
    @sql_primary_key Id : int
}

[sql_table(name = "C8")]
struct C8 {
    @sql_primary_key Id : int
    @sql_references = "C8Parent"
    @sql_on_delete = "blow_up"
    ParentId : int
}

// 9. @sql_on_delete = "set_null" on a non-Option<T> column — SQLite cannot
//    set a NOT NULL column to NULL.
[sql_table(name = "C9Parent")]
struct C9Parent {
    @sql_primary_key Id : int
}

[sql_table(name = "C9")]
struct C9 {
    @sql_primary_key Id : int
    @sql_references = "C9Parent"
    @sql_on_delete = "set_null"
    ParentId : int
}

// 10. @sql_on_delete = "set_default" without a default — SET DEFAULT has
//     nothing to set.
[sql_table(name = "C10Parent")]
struct C10Parent {
    @sql_primary_key Id : int
}

[sql_table(name = "C10")]
struct C10 {
    @sql_primary_key Id : int
    @sql_references = "C10Parent"
    @sql_on_delete = "set_default"
    ParentId : int
}

// 11. @sql_references naming a struct that doesn't exist in this module.
[sql_table(name = "C11")]
struct C11 {
    @sql_primary_key Id : int
    @sql_references = "NoSuchStruct"
    OtherId : int
}

// 12. @sql_references naming a struct that exists but has no
//     @sql_primary_key — FK target column cannot be resolved.
[sql_table(name = "C12NoPk")]
struct C12NoPk {
    Marker : int
}

[sql_table(name = "C12")]
struct C12 {
    @sql_primary_key Id : int
    @sql_references = "C12NoPk"
    ParentId : int
}

[export]
def main() {}
