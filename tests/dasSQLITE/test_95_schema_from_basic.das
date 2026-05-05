options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// schema_from with empty body — every field synthesized from the .db.
// Covers all five SQLite affinity classes and the (effective) NOT NULL / nullable contract.

[sql_table(name = "M", schema_from = "tests/dasSQLITE/test_data/schema_from_all_affinities.db")]
struct AllAffinities {}

[sql_table(name = "Items", schema_from = "tests/dasSQLITE/test_data/schema_from_nullable.db")]
struct Item {}

[test]
def test_all_affinities_field_count(t : T?) {
    let cols <- column_info(type<AllAffinities>)
    t |> equal(length(cols), 5, "5 columns: I, R, T, B, N")
}

[test]
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

[test]
def test_all_affinities_not_null(t : T?) {
    let cols <- column_info(type<AllAffinities>)
    for (c in cols) {
        t |> equal(c.is_nullable, false, "{c.name} is NOT NULL")
    }
}

[test]
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

[test]
def test_round_trip_select(t : T?) {
    let cols <- column_info(type<AllAffinities>)
    t |> equal(length(cols), 5, "selectable shape")
    with_sqlite("tests/dasSQLITE/test_data/schema_from_all_affinities.db") $(db) {
        let rows <- _sql(db |> select_from(type<AllAffinities>))
        t |> equal(length(rows), 0, "fresh fixture has no rows")
    }
}
