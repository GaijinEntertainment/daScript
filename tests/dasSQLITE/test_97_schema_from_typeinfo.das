options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// The wip.das pattern: one ETL script handling multiple historical DB versions.
// `typeinfo has_field<>` + `static_if` lets one function body branch at compile time
// based on whether a column exists in the source DB.

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v1.db")]
struct OldLogV1 {}

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct OldLogV2 {}

[test]
def test_v1_lacks_severity_via_typeinfo(t : T?) {
    static_if (typeinfo has_field<Severity>(type<OldLogV1>)) {
        t |> equal(true, false, "v1 should NOT have Severity field")
    } else {
        t |> equal(true, true, "v1 correctly missing Severity")
    }
}

[test]
def test_v2_has_severity_via_typeinfo(t : T?) {
    static_if (typeinfo has_field<Severity>(type<OldLogV2>)) {
        t |> equal(true, true, "v2 correctly has Severity")
    } else {
        t |> equal(true, false, "v2 should have Severity field")
    }
}

[test]
def test_v1_field_count(t : T?) {
    let cols <- column_info(type<OldLogV1>)
    t |> equal(length(cols), 2, "v1 has Id, Message")
}

[test]
def test_v2_field_count(t : T?) {
    let cols <- column_info(type<OldLogV2>)
    t |> equal(length(cols), 3, "v2 has Id, Severity, Message")
}

[test]
def test_each_sql_iterates_synthesized_struct(t : T?) {
    // Insert two rows into v2-shape, then iterate via streaming `_each_sql`.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<OldLogV2>)
        let _r1 = db |> insert(OldLogV2(Id = 1l, Severity = 1l, Message = "info"))
        let _r2 = db |> insert(OldLogV2(Id = 2l, Severity = 3l, Message = "error"))
        var seen = 0
        for (row in _each_sql(db |> select_from(type<OldLogV2>))) {
            seen += 1
            t |> equal(row.Severity >= 1l, true, "Severity present")
        }
        t |> equal(seen, 2, "iterated both rows")
    }
}
