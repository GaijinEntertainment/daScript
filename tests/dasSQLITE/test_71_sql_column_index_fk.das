options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// `@sql_column` rename interacting with sibling annotations:
//   - `[sql_index(fields="<daslang>")]` accepts the daslang field name and
//     emits the SQL identifier in CREATE INDEX(...).
//   - `@sql_references = "Parent"` looks up the parent's @sql_primary_key
//     field, including its own `@sql_column` rename, and emits the renamed
//     SQL identifier in REFERENCES "Parent"("<sql_pk_col>").

[sql_table(name = "Categories"),
 sql_index(fields = "col_label", name = "idx_label")]
struct Category {
    @sql_column = "type"          // PK column renamed to SQL keyword 'type'
    @sql_primary_key Id : int
    @sql_column = "label"
    col_label : string
}

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
    // FK to Category.Id — the parent's PK was renamed to 'type', so the
    // emitted REFERENCES clause must point at the SQL column 'type', not
    // the daslang field name 'Id'.
    @sql_references = "Category"
    CatId : int
}

[test]
def test_sql_index_translates_daslang_to_sql_column(t : T?) {
    let idx_sql = _::_sql_create_indexes_sql(type<Category>)
    t |> success(idx_sql |> find("(\"label\")") >= 0,
        "[sql_index] field='col_label' emits CREATE INDEX (\"label\") at SQL")
    t |> success(idx_sql |> find("col_label") < 0,
        "daslang field name 'col_label' must NOT appear in DDL")
}

[test]
def test_sql_index_explicit_name_preserved(t : T?) {
    let idx_sql = _::_sql_create_indexes_sql(type<Category>)
    t |> success(idx_sql |> find("\"idx_label\"") >= 0,
        "explicit index name 'idx_label' preserved")
}

[test]
def test_sql_references_translates_parent_pk_rename(t : T?) {
    let ddl = _::_sql_create_table_sql(type<Item>)
    // Parent's @sql_primary_key was renamed via @sql_column = "type".
    // The FK reference must point at the SQL identifier on the parent.
    t |> success(ddl |> find("REFERENCES \"Categories\"(\"type\")") >= 0,
        "FK references parent's renamed PK column 'type'")
    t |> success(ddl |> find("REFERENCES \"Categories\"(\"Id\")") < 0,
        "FK must NOT reference daslang field name 'Id'")
}

// End-to-end: create both tables, insert parent + child, verify the FK works
// in SQLite (would fail at runtime with `foreign key mismatch` if the
// REFERENCES clause pointed at a non-existent column).
[test]
def test_sql_references_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("PRAGMA foreign_keys = ON")
        db |> create_table(type<Category>)
        db |> create_table(type<Item>)
        db |> insert(Category(Id = 100, col_label = "tools"))
        db |> insert(Item(Id = 1, Name = "hammer", CatId = 100))
        let items <- db |> select_from(type<Item>)
        t |> equal(length(items), 1, "child row inserted with valid FK")
        t |> equal(items[0].CatId, 100, "FK value preserved")
    }
}
