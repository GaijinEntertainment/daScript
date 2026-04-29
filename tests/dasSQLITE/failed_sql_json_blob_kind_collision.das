options gen2

// Each pair of [sql_table]s below uses the same payload type with two
// different storage kinds (one @sql_json, the other @sql_blob). Adapter
// dedup is per-(T, kind), so the second struct in each pair triggers
// error 30111 = "macro [sql_table] failed to apply to the structure".
// Without this rejection a single payload type would silently route
// through whichever adapter was emitted first — a BLOB column reading
// from `sql_bind(T) : string` or vice versa.
expect 30111:3

require daslib/sql
require sqlite/sqlite_boost

struct PayloadA {
    name : string
    count : int
}

// 1. JSON first, BLOB second — first table emits sql_bind(PayloadA) : string;
//    second's @sql_blob field needs sql_bind(PayloadA) : array<uint8>.
[sql_table(name = "A1")]
struct A1 {
    @sql_primary_key Id : int
    @sql_json State : PayloadA
}

[sql_table(name = "A2")]
struct A2 {
    @sql_primary_key Id : int
    @sql_blob State : PayloadA
}

struct PayloadB {
    tag : string
}

// 2. BLOB first, JSON second — symmetric to case 1.
[sql_table(name = "B1")]
struct B1 {
    @sql_primary_key Id : int
    @sql_blob State : PayloadB
}

[sql_table(name = "B2")]
struct B2 {
    @sql_primary_key Id : int
    @sql_json State : PayloadB
}

struct PayloadC {
    when : int64
}

// 3. Two different fields of the same type in one struct, with different
//    kinds — caught when the second field is processed (adapter for the
//    type was just emitted with the first field's kind).
[sql_table(name = "C1")]
struct C1 {
    @sql_primary_key Id : int
    @sql_json First : PayloadC
    @sql_blob Second : PayloadC
}
