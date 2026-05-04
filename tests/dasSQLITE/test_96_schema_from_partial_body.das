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
// Witness routes through `sql_storage_enum_for` (Option-aware); a bare `sql_bind` lookup
// would hit the generic auto overload and trip its concept_assert here.
[sql_table(name = "Items", schema_from = "tests/dasSQLITE/test_data/schema_from_nullable.db")]
struct ItemPartialOption {
    @sql_primary_key Id : int64
    Note  : Option<string>
    Score : Option<double>
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
