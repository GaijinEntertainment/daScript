options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

// schema_from with empty body — every field synthesized from the .db.
// Covers all five SQLite affinity classes and the (effective) NOT NULL / nullable contract.

[sql_table(name = "M", schema_from = "tests/dasSQLITE/test_data/schema_from_all_affinities.db")]
struct AllAffinities {}

[sql_table(name = "Items", schema_from = "tests/dasSQLITE/test_data/schema_from_nullable.db")]
struct Item {}

[test(description = "a struct synthesized from a database table has one field per column")]
def test_all_affinities_field_count(t : T?) {
    let cols <- column_info(type<AllAffinities>)
    t |> equal(length(cols), 5, "5 columns: I, R, T, B, N")
}

[test(description = "schema_from maps each SQLite column affinity to the matching SqlType")]
def test_all_affinities_types(t : T?) {
    let cols <- column_info(type<AllAffinities>)
    t |> equal(cols[0].name, "I", "I name")
    t |> equal(cols[0].data_type, SqlType.Integer, "I → INTEGER")
    t |> equal(cols[1].name, "R", "R name")
    t |> equal(cols[1].data_type, SqlType.Real, "R → REAL")
    t |> equal(cols[2].name, "T", "T name")
    t |> equal(cols[2].data_type, SqlType.Text, "T → TEXT")
    t |> equal(cols[3].name, "B", "B name")
    t |> equal(cols[3].data_type, SqlType.Blob, "B → BLOB")
    t |> equal(cols[4].name, "N", "N name")
    t |> equal(cols[4].data_type, SqlType.Real, "N (NUMERIC affinity) → REAL")
}

[test(description = "NOT NULL columns synthesize non-nullable fields")]
def test_all_affinities_not_null(t : T?) {
    let cols <- column_info(type<AllAffinities>)
    for (c in cols) {
        t |> equal(c.is_nullable, false, "{c.name} is NOT NULL")
    }
}

[test(description = "nullable columns synthesize option fields and the primary key is never nullable")]
def test_nullable_schema_wraps_in_option(t : T?) {
    let cols <- column_info(type<Item>)
    t |> equal(length(cols), 3, "Items has Id, Note, Score")
    t |> equal(cols[0].is_pk, true, "Id is PK")
    t |> equal(cols[0].is_nullable, false, "PK is never nullable (SQLite quirk handled)")
    t |> equal(cols[1].name, "Note", "Note column")
    t |> equal(cols[1].is_nullable, true, "Note is nullable TEXT → Option<string>")
    t |> equal(cols[2].name, "Score", "Score column")
    t |> equal(cols[2].is_nullable, true, "Score is nullable REAL → Option<double>")
}

[test(description = "a schema_from struct can be selected from its table")]
def test_round_trip_select(t : T?) {
    let cols <- column_info(type<AllAffinities>)
    t |> equal(length(cols), 5, "selectable shape")
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_all_affinities.db") $(db) {
        let rows <- _sql(db |> select_from(type<AllAffinities>))
        t |> equal(length(rows), 0, "fresh fixture has no rows")
    }
}

// Partial body — hand-declared fields are CONTRACTS. Same-affinity narrowing is allowed
// (int instead of synthesized int64). Annotation overrides (@sql_json on TEXT, @sql_blob on BLOB)
// route through the existing custom-types adapter rail.

// 1) Type narrowing: declare Id as `int` against INTEGER PK column.
[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct LogPartialNarrow {
    @sql_primary_key Id : int
}

// 2) PK redundancy: explicitly mark @sql_primary_key on the existing PK column.
[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v1.db")]
struct LogPartialPkRedundant {
    @sql_primary_key Id : int
    Message : string
}

// 3) Option<T> in partial body against nullable columns — must compile.
// The generic `sql_bind($Option<auto(TT)>)` fallback in sqlite_boost routes the
// witness through inner T's sql_bind; without it the contract check would hit
// the auto-overload's concept_assert.
[sql_table(name = "Items", schema_from = "tests/dasSQLITE/test_data/schema_from_nullable.db")]
struct ItemPartialOption {
    @sql_primary_key Id : int64
    Note  : Option<string>
    Score : Option<double>
}

// 4) @sql_json + Option<UserStruct> on nullable TEXT column.
// Three sub-bugs were fixed at once:
//   - macro-side: emit_json_blob_adapters_for_struct now unwraps Option<T> before
//     codegen so the generated `sql_bind(NoteData)` is reachable from the runtime
//     `sql_bind_to_stmt(Option<T>)` Option-unwrap path.
//   - typeclass-side: the new generic `sql_bind($Option<auto(TT)>)` delegates via
//     `_::sql_bind(default<TT>)` so caller-module adapters resolve.
//   - contract-side: schema_from witness is `_::sql_bind(default<field_type>)`
//     directly; the generic Option fallback handles the unwrap.
struct NoteData {
    title : string
    color : int
}

[sql_table(name = "Items", schema_from = "tests/dasSQLITE/test_data/schema_from_nullable.db")]
struct ItemPartialJson {
    @sql_primary_key Id : int64
    @sql_json Note  : Option<NoteData>
    Score : Option<double>
}

// 5) @sql_blob + Option<UserStruct> on nullable BLOB column. Same three rails as #4.
struct HashPayload {
    bytes : array<uint8>
}

[sql_table(name = "Files", schema_from = "tests/dasSQLITE/test_data/schema_from_blob.db")]
struct FilePartialBlobOpt {
    @sql_primary_key Id : int64
    Hash : array<uint8>
    @sql_blob Body : Option<HashPayload>
}

[test(description = "a hand-declared int primary key is accepted against an INTEGER column and the other fields are synthesized")]
def test_narrow_int_against_integer_pk(t : T?) {
    let cols <- column_info(type<LogPartialNarrow>)
    t |> equal(length(cols), 3, "3 columns: hand-declared Id + synthesized Severity + Message")
    t |> equal(cols[0].name, "Id", "Id at position 0 (hand-declared)")
    t |> equal(cols[0].is_pk, true, "PK preserved")
    t |> equal(cols[0].data_type, SqlType.Integer, "user int → INTEGER (within affinity)")
}

[test(description = "declaring the primary key by hand in a schema_from body is accepted when it matches")]
def test_pk_redundancy_accepted(t : T?) {
    let cols <- column_info(type<LogPartialPkRedundant>)
    t |> equal(length(cols), 2, "user-declared shape matches schema exactly")
    t |> equal(cols[0].name, "Id", "Id first")
    t |> equal(cols[0].is_pk, true, "@sql_primary_key applied")
    t |> equal(cols[1].name, "Message", "Message second")
}

[test(description = "a hand-declared option field maps to a nullable column and keeps its affinity")]
def test_partial_body_option_against_nullable(t : T?) {
    let cols <- column_info(type<ItemPartialOption>)
    t |> equal(length(cols), 3, "3 columns")
    t |> equal(cols[1].name, "Note", "Note column")
    t |> equal(cols[1].is_nullable, true, "Note Option<string> mapped to nullable")
    t |> equal(cols[1].data_type, SqlType.Text, "TEXT affinity preserved through Option<>")
    t |> equal(cols[2].name, "Score", "Score column")
    t |> equal(cols[2].is_nullable, true, "Score Option<double> mapped to nullable")
    t |> equal(cols[2].data_type, SqlType.Real, "REAL affinity preserved through Option<>")
}

[test(description = "a hand-declared optional sql_json field round-trips against a nullable TEXT column")]
def test_partial_body_json_option(t : T?) {
    // Compile-pass + runtime coverage for @sql_json + Option<UserStruct>.
    let cols <- column_info(type<ItemPartialJson>)
    t |> equal(length(cols), 3, "Id + Note (json) + Score")
    t |> equal(cols[1].name, "Note", "Note column")
    t |> equal(cols[1].is_nullable, true, "Option<NoteData> ↔ nullable TEXT")
    t |> equal(cols[1].data_type, SqlType.Text, "@sql_json binds to TEXT")
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<ItemPartialJson>)
        let _r1 = db |> insert(ItemPartialJson(Id = 1l,
            Note = some(NoteData(title = "first", color = 7)),
            Score = some(1.5lf)))
        let _r2 = db |> insert(ItemPartialJson(Id = 2l,
            Note = none(type<NoteData>),
            Score = none(type<double>)))
        let rows <- _sql(db |> select_from(type<ItemPartialJson>) |> _order_by(_.Id))
        t |> equal(length(rows), 2, "two rows")
        t |> equal(rows[0].Note |> is_some, true, "row 0 Note carries payload")
        t |> equal((rows[0].Note |> unwrap).title, "first", "json title round-trips")
        t |> equal((rows[0].Note |> unwrap).color, 7, "json color round-trips")
        t |> equal(rows[1].Note |> is_none, true, "row 1 Note is none (NULL TEXT)")
    }
}

[test(description = "a hand-declared optional sql_blob field round-trips against a nullable BLOB column")]
def test_partial_body_blob_option(t : T?) {
    let cols <- column_info(type<FilePartialBlobOpt>)
    t |> equal(length(cols), 3, "Id + Hash + Body (blob, optional)")
    t |> equal(cols[2].name, "Body", "Body column")
    t |> equal(cols[2].is_nullable, true, "Body Option<HashPayload> ↔ nullable BLOB")
    t |> equal(cols[2].data_type, SqlType.Blob, "@sql_blob binds to BLOB")
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FilePartialBlobOpt>)
        let _r1 = db |> insert(FilePartialBlobOpt(Id = 1l,
            Hash <- [0xCAu8, 0xFEu8],
            Body = some(HashPayload(bytes <- [0xDEu8, 0xADu8]))))
        let _r2 = db |> insert(FilePartialBlobOpt(Id = 2l,
            Hash <- [0u8, 1u8],
            Body = none(type<HashPayload>)))
        let rows <- _sql(db |> select_from(type<FilePartialBlobOpt>) |> _order_by(_.Id))
        t |> equal(length(rows), 2, "two rows")
        t |> equal(rows[0].Body |> is_some, true, "row 0 Body carries payload")
        t |> equal(length((rows[0].Body |> unwrap).bytes), 2, "blob payload length preserved")
        t |> equal((rows[0].Body |> unwrap).bytes[0], 0xDEu8, "first byte preserved")
        t |> equal(rows[1].Body |> is_none, true, "row 1 Body is none (NULL BLOB)")
    }
}

[test(description = "a partially declared schema_from struct reads rows with synthesized and hand-declared fields")]
def test_partial_body_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<LogPartialNarrow>)
        // PK 0 means no-pk insert path (unset). Use literal 1 to drive insert-with-pk.
        let row = LogPartialNarrow(Id = 1, Severity = 2l, Message = "boom")
        let _rowid = db |> insert(row)
        let rows <- _sql(db |> select_from(type<LogPartialNarrow>))
        t |> equal(length(rows), 1, "one row")
        t |> equal(rows[0].Id, 1, "Id round-trip")
        t |> equal(rows[0].Severity, 2l, "synthesized Severity round-trip")
        t |> equal(rows[0].Message, "boom", "synthesized Message round-trip")
    }
}

// The wip.das pattern: one ETL script handling multiple historical DB versions.
// `typeinfo has_field<>` + `static_if` lets one function body branch at compile time
// based on whether a column exists in the source DB.

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v1.db")]
struct OldLogV1 {}

[sql_table(name = "Logs", schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
struct OldLogV2 {}

[test(description = "the v1 synthesized struct has no Severity field at compile time")]
def test_v1_lacks_severity_via_typeinfo(t : T?) {
    static_if (typeinfo has_field<Severity>(type<OldLogV1>)) {
        t |> equal(true, false, "v1 should NOT have Severity field")
    } else {
        t |> equal(true, true, "v1 correctly missing Severity")
    }
}

[test(description = "the v2 synthesized struct has a Severity field at compile time")]
def test_v2_has_severity_via_typeinfo(t : T?) {
    static_if (typeinfo has_field<Severity>(type<OldLogV2>)) {
        t |> equal(true, true, "v2 correctly has Severity")
    } else {
        t |> equal(true, false, "v2 should have Severity field")
    }
}

[test(description = "the v1 synthesized struct has the two columns of the v1 table")]
def test_v1_field_count(t : T?) {
    let cols <- column_info(type<OldLogV1>)
    t |> equal(length(cols), 2, "v1 has Id, Message")
}

[test(description = "the v2 synthesized struct has the three columns of the v2 table")]
def test_v2_field_count(t : T?) {
    let cols <- column_info(type<OldLogV2>)
    t |> equal(length(cols), 3, "v2 has Id, Severity, Message")
}

[test(description = "each_sql streams rows into a synthesized struct")]
def test_each_sql_iterates_synthesized_struct(t : T?) {
    // Insert two rows into v2-shape, then iterate via streaming `_each_sql`.
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<OldLogV2>)
        let _r1 = db |> insert(OldLogV2(Id = 1l, Severity = 1l, Message = "info"))
        let _r2 = db |> insert(OldLogV2(Id = 2l, Severity = 3l, Message = "error"))
        var seen = 0
        for (row in _each_sql(db |> select_from(type<OldLogV2>))) {
            seen++
            t |> equal(row.Severity >= 1l, true, "Severity present")
        }
        t |> equal(seen, 2, "iterated both rows")
    }
}

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

[test(description = "check_schema returns none for a struct synthesized from the same table")]
def test_check_schema_match_via_schema_from(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, false, "schema matches → none")
    }
}

[test(description = "check_schema returns none for a hand-written struct matching the table")]
def test_check_schema_match_hand_written(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogHand>)
        t |> equal(r |> is_some, false, "hand-written struct also matches")
    }
}

[test(description = "check_schema reports a column count mismatch")]
def test_check_schema_column_count_mismatch(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v1.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, true, "v2 struct vs v1 DB: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "column count mismatch") >= 0, true, "diagnostic mentions count")
    }
}

[test(description = "check_schema reports a SqlType mismatch")]
def test_check_schema_type_mismatch(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogTypeMismatch>)
        t |> equal(r |> is_some, true, "string vs INTEGER: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "SqlType mismatch") >= 0, true, "diagnostic mentions SqlType")
    }
}

[test(description = "check_schema reports a nullability mismatch")]
def test_check_schema_nullability_mismatch(t : T?) {
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_nullable.db") $(db) {
        let r = db |> try_check_schema(type<ItemNonOpt>)
        t |> equal(r |> is_some, true, "non-optional vs nullable: should err")
        let msg = r |> unwrap
        t |> equal(find(msg, "nullability mismatch") >= 0, true, "diagnostic mentions nullability")
    }
}

[test(description = "check_schema reports a table missing from the database")]
def test_check_schema_table_not_found(t : T?) {
    // Open the v2 DB (which has Logs), look for a struct named after a non-existent table.
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_blob.db") $(db) {
        let r = db |> try_check_schema(type<LogV2>)
        t |> equal(r |> is_some, true, "Logs not in blob.db: should err")
    }
}

[test(description = "check_schema accepts fields in a different order")]
def test_check_schema_field_order_independent(t : T?) {
    // Struct field order is independent of DB column order — [sql_table] emits SELECTs with
    // explicit column names, so reordered structs round-trip correctly. check_schema agrees.
    with_sqlite("{get_das_root()}/tests/dasSQLITE/test_data/schema_from_v2.db") $(db) {
        let r = db |> try_check_schema(type<LogReordered>)
        t |> equal(r |> is_some, false, "reordered struct must match — same names, types, nullability, pk")
    }
}

[test(description = "check_schema reports a struct field the database lacks as a count mismatch")]
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

[test(description = "check_schema names a struct field that is not found in the database")]
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

[test(description = "check_schema finds a table in an attached schema")]
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
[test(description = "the panicking check_schema form panics on a mismatch")]
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

[test(description = "check_schema accepts a table created from a struct with a virtual computed column")]
def test_check_schema_virtual_computed_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Box>, "Boxes")
        db |> insert(Box(Id = 1l, W = 3l, H = 4l))
        let r = db |> try_check_schema(type<Box>)
        t |> equal(r |> is_some, false, "check_schema must accept the table create_table just made from the same struct")
    }
}

[test(description = "check_schema accepts a table created from a struct with a stored computed column")]
def test_check_schema_stored_computed_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<SBox>, "SBoxes")
        let r = db |> try_check_schema(type<SBox>)
        t |> equal(r |> is_some, false, "STORED generated column round-trips too")
    }
}

[test(description = "a struct may omit a generated database column and still pass check_schema")]
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

[test(description = "check_schema reports a computed field declared over an ordinary column")]
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

[test(description = "check_schema reports a plain field declared over a generated column")]
def test_check_schema_plain_field_on_generated_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Box>, "Boxes")
        let r = db |> try_check_schema(type<BoxPlainArea>)
        t |> equal(r |> is_some, true, "plain field over a GENERATED DB column must be reported - INSERT would fail")
        let msg = r |> unwrap
        t |> equal(find(msg, "computed mismatch") >= 0, true, "diagnostic mentions computed")
    }
}

[test(description = "check_schema reports an ordinary column no struct field covers")]
def test_check_schema_uncovered_ordinary_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Box>, "Boxes")
        let r = db |> try_check_schema(type<BoxMissesOrdinary>)
        t |> equal(r |> is_some, true, "an ordinary DB column with no struct field must be reported")
        let msg = r |> unwrap
        t |> equal(find(msg, "\"H\"") >= 0, true, "diagnostic names the uncovered column")
    }
}
