options gen2
expect 20800:1
require sqlite/sqlite_boost

[sql_table(name="composite_parent")]
struct CompositeParent {
    @sql_primary_key
    category : string
    @sql_primary_key
    ordinal : int64
}

[sql_table(name="child_row")]
struct ChildRow {
    @sql_primary_key
    id : int64
    @sql_references = "CompositeParent"
    category : string
}
