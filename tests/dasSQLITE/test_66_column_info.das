options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// ──────────────────────────────────────────────────────────────────────────
// `column_info(type<T>)` — compile-time walk over a [sql_table] struct,
// surfaced at runtime as `array<ColumnInfo>`. Mirrors the field walk DDL
// emission uses, so the same FieldInfo array drives both — including
// `@sql_computed` columns. `is_nullable` is the daslang-side
// `info.is_optional` (Option<T>-derived), not a NOT-NULL constraint check.
//
// `sqlite_sql_type(SqlType)` renders the abstract enum to the SQLite
// dialect spelling — "INTEGER" / "REAL" / "TEXT" / "BLOB" / "NULL".
// ──────────────────────────────────────────────────────────────────────────

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : float = 9.99
    @safe_when_uninitialized Tag : Option<string>
    @sql_default_fn = "CURRENT_TIMESTAMP"
    CreatedAt : string
}

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Width : int = 100
    Height : int = 50
    @sql_computed = "Width * Height"
    Area : int
    @sql_default_fn = "CURRENT_TIMESTAMP"
    UpdatedAt : string
}

struct PrefShape {
    theme : string
    lang : string
}

[sql_table(name = "Mixed")]
struct Mixed {
    @sql_primary_key Id : int
    @sql_json Prefs : PrefShape
    @sql_blob Bytes : tuple<a : int; b : int>
}

// ──────────────────────────────────────────────────────────────────────────
// Cardinality + ordering
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_column_info_returns_one_entry_per_field(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(length(cols), 5, "Car has 5 columns: Id, Name, Price, Tag, CreatedAt")
}

[test]
def test_column_info_preserves_field_order(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(cols[0].name, "Id", "first column is Id")
    t |> equal(cols[1].name, "Name", "second column is Name")
    t |> equal(cols[2].name, "Price", "third column is Price")
    t |> equal(cols[3].name, "Tag", "fourth column is Tag")
    t |> equal(cols[4].name, "CreatedAt", "fifth column is CreatedAt")
}

// ──────────────────────────────────────────────────────────────────────────
// Per-column metadata — PK / nullability / data_type / default_expr
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_column_info_marks_primary_key(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(cols[0].is_pk, true, "Id is PK")
    t |> equal(cols[1].is_pk, false, "Name is not PK")
    t |> equal(cols[3].is_pk, false, "Tag is not PK")
}

[test]
def test_column_info_marks_optional_as_nullable(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(cols[0].is_nullable, false, "PK never nullable")
    t |> equal(cols[1].is_nullable, false, "plain string is NOT NULL")
    t |> equal(cols[3].is_nullable, true, "Option<string> is nullable")
}

[test]
def test_column_info_data_type_int(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(int(cols[0].data_type), int(SqlType.Integer), "int Id maps to SqlType.Integer")
}

[test]
def test_column_info_data_type_real(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(int(cols[2].data_type), int(SqlType.Real), "float Price maps to SqlType.Real")
}

[test]
def test_column_info_data_type_text(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(int(cols[1].data_type), int(SqlType.Text), "string Name maps to SqlType.Text")
}

[test]
def test_column_info_data_type_text_through_option(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(int(cols[3].data_type), int(SqlType.Text), "Option<string> Tag still maps to SqlType.Text")
}

[test]
def test_column_info_default_expr_literal(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(cols[2].default_expr, "9.99", "literal float init becomes default_expr")
}

[test]
def test_column_info_default_expr_sql_fn(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(cols[4].default_expr, "CURRENT_TIMESTAMP", "@sql_default_fn flows into default_expr")
}

[test]
def test_column_info_default_expr_empty_when_none(t : T?) {
    let cols <- column_info(type<Car>)
    t |> equal(cols[0].default_expr, "", "Id has no default")
    t |> equal(cols[1].default_expr, "", "Name has no default")
    t |> equal(cols[3].default_expr, "", "Tag has no default")
}

// ──────────────────────────────────────────────────────────────────────────
// Computed columns — present in the metadata, no default, regular SqlType
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_column_info_includes_computed_column(t : T?) {
    let cols <- column_info(type<Item>)
    t |> equal(length(cols), 5, "Item has 5 columns including computed Area")
    t |> equal(cols[3].name, "Area", "fourth column is computed Area")
    t |> equal(int(cols[3].data_type), int(SqlType.Integer), "computed int column resolves to SqlType.Integer")
    t |> equal(cols[3].default_expr, "", "computed column has no default_expr")
}

// ──────────────────────────────────────────────────────────────────────────
// @sql_json / @sql_blob short-circuits — Text and Blob respectively
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_column_info_sql_json_field_is_text(t : T?) {
    let cols <- column_info(type<Mixed>)
    t |> equal(cols[1].name, "Prefs", "Prefs is the @sql_json column")
    t |> equal(int(cols[1].data_type), int(SqlType.Text), "@sql_json column reports SqlType.Text")
}

[test]
def test_column_info_sql_blob_field_is_blob(t : T?) {
    let cols <- column_info(type<Mixed>)
    t |> equal(cols[2].name, "Bytes", "Bytes is the @sql_blob column")
    t |> equal(int(cols[2].data_type), int(SqlType.Blob), "@sql_blob column reports SqlType.Blob")
}

// ──────────────────────────────────────────────────────────────────────────
// sqlite_sql_type — abstract → dialect string
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_sqlite_sql_type_integer(t : T?) {
    t |> equal(sqlite_sql_type(SqlType.Integer), "INTEGER", "SqlType.Integer renders as INTEGER")
}

[test]
def test_sqlite_sql_type_real(t : T?) {
    t |> equal(sqlite_sql_type(SqlType.Real), "REAL", "SqlType.Real renders as REAL")
}

[test]
def test_sqlite_sql_type_text(t : T?) {
    t |> equal(sqlite_sql_type(SqlType.Text), "TEXT", "SqlType.Text renders as TEXT")
}

[test]
def test_sqlite_sql_type_blob(t : T?) {
    t |> equal(sqlite_sql_type(SqlType.Blob), "BLOB", "SqlType.Blob renders as BLOB")
}

[test]
def test_sqlite_sql_type_null(t : T?) {
    t |> equal(sqlite_sql_type(SqlType.Null), "NULL", "SqlType.Null renders as NULL")
}

// ──────────────────────────────────────────────────────────────────────────
// Composes with the rest of the language — column_info is just data
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_column_info_filterable(t : T?) {
    let pks = [for (c in column_info(type<Car>)); c; where c.is_pk]
    t |> equal(length(pks), 1, "Car has one PK column")
    t |> equal(pks[0].name, "Id", "PK column is Id")
}

[test]
def test_column_info_paired_with_dialect_render(t : T?) {
    let cols <- column_info(type<Car>)
    let lines = [for (c in cols); "{c.name} {sqlite_sql_type(c.data_type)}"]
    t |> equal(lines[0], "Id INTEGER", "Id INTEGER")
    t |> equal(lines[2], "Price REAL", "Price REAL")
    t |> equal(lines[4], "CreatedAt TEXT", "CreatedAt TEXT")
}
