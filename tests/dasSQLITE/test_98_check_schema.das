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

[test]
def test_check_schema_match_via_schema_from(t : T?) {
    with_sqlite("tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, false, "schema matches → none")
    }
}

[test]
def test_check_schema_match_hand_written(t : T?) {
    with_sqlite("tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogHand>)
        t |> equal(r |> is_some, false, "hand-written struct also matches")
    }
}

[test]
def test_check_schema_column_count_mismatch(t : T?) {
    with_sqlite("tests/dasSQLITE/test_data/schema_from_v1.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, true, "v2 struct vs v1 DB: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "column count mismatch") >= 0, true, "diagnostic mentions count")
    }
}

[test]
def test_check_schema_type_mismatch(t : T?) {
    with_sqlite("tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogTypeMismatch>)
        t |> equal(r |> is_some, true, "string vs INTEGER: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "SqlType mismatch") >= 0, true, "diagnostic mentions SqlType")
    }
}

[test]
def test_check_schema_nullability_mismatch(t : T?) {
    with_sqlite("tests/dasSQLITE/test_data/schema_from_nullable.db") $(db) {
        let r = db |> try_check_schema(type<ItemNonOpt>)
        t |> equal(r |> is_some, true, "non-optional vs nullable: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "nullability mismatch") >= 0, true, "diagnostic mentions nullability")
    }
}

[test]
def test_check_schema_table_not_found(t : T?) {
    // Open the v2 DB (which has Logs), look for a struct named after a non-existent table.
    with_sqlite("tests/dasSQLITE/test_data/schema_from_blob.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, true, "Logs not in blob.db: should err")
    }
}

// Strict variant — make sure `check_schema` panics when `try_check_schema` returns some.
[test]
def test_check_schema_panics_on_mismatch(t : T?) {
    var caught = false
    try {
        with_sqlite("tests/dasSQLITE/test_data/schema_from_v1.db") $(db) {
            db |> check_schema(type<LogV2>)
        }
    } recover {
        caught = true
    }
    t |> equal(caught, true, "panic propagated through try/recover")
}
