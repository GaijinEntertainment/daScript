options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

// `check_schema` is the runtime startup-defense pattern. Works for any [sql_table]
// struct, not just schema_from-derived ones — it's a contract check between the
// compiled struct and whatever the DB looks like at runtime.

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct LogV2 {}

// Hand-written struct (no schema_from) — proves check_schema is independent.
[sql_table(name = "Logs")]
struct LogHand {
    @sql_primary_key Id : int64
    Severity : int64
    Message  : string
}

// Mismatched against v2 — has Severity but as TEXT instead of INTEGER.
[sql_table(name = "Logs")]
struct LogTypeMismatch {
    @sql_primary_key Id : int64
    Severity : string
    Message  : string
}

// Mismatched against v2 — fewer columns.
[sql_table(name = "Logs")]
struct LogTooShort {
    @sql_primary_key Id : int64
    Severity : int64
}

// Mismatched against nullable.db (Items): non-optional Note where DB has nullable.
[sql_table(name = "Items")]
struct ItemNonOpt {
    @sql_primary_key Id : int64
    Note  : string
    Score : Option<double>
}

// Same fields as LogHand but Message/Severity swapped — name-keyed comparison
// must accept this since [sql_table] emits explicit-column SELECTs.
[sql_table(name = "Logs")]
struct LogReordered {
    @sql_primary_key Id : int64
    Message  : string
    Severity : int64
}

// Hand-written struct with a column the DB doesn't have — exercises "not found in DB columns".
[sql_table(name = "Logs")]
struct LogStrayField {
    @sql_primary_key Id : int64
    Severity : int64
    Message  : string
    Stray    : string
}

// Same arity as v2 but Message renamed to Body — exercises the by-name miss with matching count.
[sql_table(name = "Logs")]
struct LogRenamedField {
    @sql_primary_key Id : int64
    Severity : int64
    Body     : string
}

[test]
def test_check_schema_match_via_schema_from(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, false, "schema matches → none")
    }
}

[test]
def test_check_schema_match_hand_written(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogHand>)
        t |> equal(r |> is_some, false, "hand-written struct also matches")
    }
}

[test]
def test_check_schema_column_count_mismatch(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v1.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, true, "v2 struct vs v1 DB: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "column count mismatch") >= 0, true, "diagnostic mentions count")
    }
}

[test]
def test_check_schema_type_mismatch(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogTypeMismatch>)
        t |> equal(r |> is_some, true, "string vs INTEGER: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "SqlType mismatch") >= 0, true, "diagnostic mentions SqlType")
    }
}

[test]
def test_check_schema_nullability_mismatch(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_nullable.db") $(db) {
        let r = db |> try_check_schema(type<ItemNonOpt>)
        t |> equal(r |> is_some, true, "non-optional vs nullable: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "nullability mismatch") >= 0, true, "diagnostic mentions nullability")
    }
}

[test]
def test_check_schema_table_not_found(t : T?) {
    // Open the v2 DB (which has Logs), look for a struct named after a non-existent table.
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_blob.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, true, "Logs not in blob.db: should err")
    }
}

[test]
def test_check_schema_field_order_independent(t : T?) {
    // Struct field order is independent of DB column order — [sql_table] emits SELECTs with
    // explicit column names, so reordered structs round-trip correctly. check_schema agrees.
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogReordered>)
        t |> equal(r |> is_some, false, "reordered struct must match — same names, types, nullability, pk")
    }
}

[test]
def test_check_schema_struct_field_missing_in_db(t : T?) {
    // A struct field with no matching DB column — diagnostic names the offender.
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogStrayField>)
        t |> equal(r |> is_some, true, "struct has Stray, DB doesn't")
        let msg = r |> unwrap
        // Triggers count-mismatch first (4 vs 3); the by-name path is exercised when counts agree.
        t |> equal(find(msg, "column count mismatch") >= 0, true, "diagnostic mentions count")
    }
}

[test]
def test_check_schema_renamed_field(t : T?) {
    // Same column count but a struct field that's not in the DB — by-name lookup misses.
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogRenamedField>)
        t |> equal(r |> is_some, true, "Body is not in DB")
        let msg = r |> unwrap
        t |> equal(find(msg, "\"Body\"") >= 0, true, "diagnostic names the offending struct field")
        t |> equal(find(msg, "not found in DB") >= 0, true, "diagnostic mentions not found")
    }
}

[test]
def test_check_schema_attached_schema(t : T?) {
    // Regression: try_check_schema must honor `db.schema_name` so it queries the
    // attached schema's table_info, not main's. Without the schema-qualified PRAGMA,
    // this would report "table not found" because the in-memory main schema is empty.
    with_sqlite(":memory:") $(db) {
        db |> with_attached("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db", "archive") $(arch) {
            let r = arch |> try_check_schema(type<LogV2>)
            t |> equal(r |> is_some, false, "schema-qualified PRAGMA must find Logs in attached schema")
        }
    }
}

// Strict variant — make sure `check_schema` panics when `try_check_schema` returns some.
[test]
def test_check_schema_panics_on_mismatch(t : T?) {
    var caught = false
    try {
        with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v1.db") $(db) {
            db |> check_schema(type<LogV2>)
        }
    } recover {
        caught = true
    }
    t |> equal(caught, true, "panic propagated through try/recover")
}
