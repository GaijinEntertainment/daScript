options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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


[test]
def test_narrow_int_against_integer_pk(t : T?) {
    let cols <- column_info(type<LogPartialNarrow>)
    t |> equal(length(cols), 3, "3 columns: hand-declared Id + synthesized Severity + Message")
    t |> equal(cols[0].name, "Id", "Id at position 0 (hand-declared)")
    t |> equal(cols[0].is_pk, true, "PK preserved")
    t |> equal(cols[0].data_type, SqlType.Integer, "user int → INTEGER (within affinity)")
}

[test]
def test_pk_redundancy_accepted(t : T?) {
    let cols <- column_info(type<LogPartialPkRedundant>)
    t |> equal(length(cols), 2, "user-declared shape matches schema exactly")
    t |> equal(cols[0].name, "Id", "Id first")
    t |> equal(cols[0].is_pk, true, "@sql_primary_key applied")
    t |> equal(cols[1].name, "Message", "Message second")
}

[test]
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

[test]
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

[test]
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

[test]
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
