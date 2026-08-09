options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

// check_schema vs @sql_computed (GENERATED) columns. SQLite's `PRAGMA table_info`
// omits generated columns, so the schema reader must use `table_xinfo` or every
// computed-column table fails its own startup defense.

[sql_table(name = "Boxes")]
struct Box {
    @sql_primary_key Id : int64
    W : int64
    H : int64
    @sql_computed = "W * H"
    Area : int64
}

[sql_table(name = "SBoxes")]
struct SBox {
    @sql_primary_key Id : int64
    W : int64
    H : int64
    @sql_computed = "W * H"
    @sql_stored
    Area : int64
}

// Struct that omits the generated column - legal: a generated column never needs
// a struct field (this is the shape schema_from produces).
[sql_table(name = "Boxes")]
struct BoxNoArea {
    @sql_primary_key Id : int64
    W : int64
    H : int64
}

[test]
def test_check_schema_virtual_computed_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Box>, "Boxes")
        db |> insert(Box(Id = 1l, W = 3l, H = 4l))
        let r = db |> try_check_schema(type<Box>)
        t |> equal(r |> is_some, false, "check_schema must accept the table create_table just made from the same struct")
    }
}

[test]
def test_check_schema_stored_computed_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<SBox>, "SBoxes")
        let r = db |> try_check_schema(type<SBox>)
        t |> equal(r |> is_some, false, "STORED generated column round-trips too")
    }
}

[test]
def test_check_schema_struct_may_omit_generated(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Box>, "Boxes")
        let r = db |> try_check_schema(type<BoxNoArea>)
        t |> equal(r |> is_some, false, "a struct may omit a generated DB column (the schema_from shape)")
    }
}

// All-ordinary table, but the struct claims Area is computed.
[sql_table(name = "PlainBoxes")]
struct PlainBox {
    @sql_primary_key Id : int64
    W : int64
    H : int64
    Area : int64
}

[sql_table(name = "PlainBoxes")]
struct PlainBoxClaimsComputed {
    @sql_primary_key Id : int64
    W : int64
    H : int64
    @sql_computed = "W * H"
    Area : int64
}

// Same shape as Box but Area is a plain field - over the Boxes DB its column is GENERATED.
[sql_table(name = "Boxes")]
struct BoxPlainArea {
    @sql_primary_key Id : int64
    W : int64
    H : int64
    Area : int64
}

// Covers the generated Area by name but misses ordinary H - the coverage sweep case.
[sql_table(name = "Boxes")]
struct BoxMissesOrdinary {
    @sql_primary_key Id : int64
    W : int64
    @sql_computed = "W * H"
    Area : int64
}

[test]
def test_check_schema_computed_claim_on_ordinary_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<PlainBox>, "PlainBoxes")
        let r = db |> try_check_schema(type<PlainBoxClaimsComputed>)
        t |> equal(r |> is_some, true, "@sql_computed field over an ordinary DB column must be reported")
        let msg = r |> unwrap
        t |> equal(find(msg, "computed mismatch") >= 0, true, "diagnostic mentions computed")
        t |> equal(find(msg, "\"Area\"") >= 0, true, "diagnostic names the field")
    }
}

[test]
def test_check_schema_plain_field_on_generated_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Box>, "Boxes")
        let r = db |> try_check_schema(type<BoxPlainArea>)
        t |> equal(r |> is_some, true, "plain field over a GENERATED DB column must be reported - INSERT would fail")
        let msg = r |> unwrap
        t |> equal(find(msg, "computed mismatch") >= 0, true, "diagnostic mentions computed")
    }
}

[test]
def test_check_schema_uncovered_ordinary_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Box>, "Boxes")
        let r = db |> try_check_schema(type<BoxMissesOrdinary>)
        t |> equal(r |> is_some, true, "an ordinary DB column with no struct field must be reported")
        let msg = r |> unwrap
        t |> equal(find(msg, "\"H\"") >= 0, true, "diagnostic names the uncovered column")
    }
}
