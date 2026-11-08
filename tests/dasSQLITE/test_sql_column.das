options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

// ============================================================================
// `@sql_column = "<sql_name>"` field annotation. Renames the field's on-disk
// column without changing the daslang-side identifier — required when the SQL
// column name is a daslang or SQL keyword (e.g. `type`, `for`, `if`), or when
// the daslang code style favors `snake_case` while the schema uses something
// else. Renames flow through DDL, _sql predicates, projections, _order_by,
// _group_by, column_info introspection, INSERT/UPDATE/DELETE, RETURNING.
// `_sql_read_row` and `_sql_bind_row` ride the positional column index, so
// the daslang-side row builder is unaffected by the rename.
// ============================================================================

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    @sql_column = "type"
    col_type : string  // 'type' is a SQL keyword in some contexts; keep the
                       // daslang field name legal and let SQL see "type".
    Price : int
}

[test(description = "create_table emits the sql_column name in the DDL and never the daslang field name")]
def test_create_table_uses_sql_column(t : T?) {
    // The DDL helper `_sql_create_table_sql(type<T>)` returns the literal
    // CREATE TABLE statement. Inspect it directly — `_sql_create_table_sql`
    // is the same source `db |> create_table` runs at first use, so this
    // is what SQLite sees.
    let ddl = _::_sql_create_table_sql(type<Car>)
    t |> success(ddl |> find("\"type\"") >= 0,
        "DDL contains renamed SQL column \"type\"")
    t |> success(ddl |> find("\"col_type\"") < 0,
        "DDL must NOT contain daslang field name \"col_type\"")
}

[test(description = "a row with a renamed column round-trips through insert and select under the daslang field name")]
def test_select_all_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "Camry", col_type = "sedan", Price = 25000))
        db |> insert(Car(Id = 2, Name = "F-150", col_type = "truck", Price = 40000))
        let rows <- db |> select_from(type<Car>)
        t |> equal(length(rows), 2, "two rows")
        t |> equal(rows[0].Name, "Camry", "first row Name")
        t |> equal(rows[0].col_type, "sedan", "first row col_type round-trips through 'type'")
        t |> equal(rows[1].col_type, "truck", "second row col_type round-trips")
    }
}

[test(description = "a where on a renamed field emits the SQL column name")]
def test_sql_text_predicate_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.col_type == "sedan"))
    // Predicate emits the SQL identifier (renamed column) — NOT the daslang field name.
    t |> success(sql |> find("\"type\" = ?") >= 0,
        "WHERE on renamed column emits SQL identifier 'type'")
    t |> success(sql |> find("\"col_type\"") < 0,
        "daslang field name 'col_type' must NOT appear in emitted SQL")
}

[test(description = "a select of a renamed field projects the SQL column name")]
def test_sql_text_projection_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.col_type))
    t |> success(sql |> find("SELECT \"type\" FROM \"Cars\"") >= 0,
        "_select on renamed column projects 'type', not 'col_type'")
}

[test(description = "order_by on a renamed field emits the SQL column name")]
def test_sql_text_order_by_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by(_.col_type))
    t |> success(sql |> find("ORDER BY \"type\" ASC") >= 0,
        "_order_by on renamed column emits 'type'")
}

[test(description = "group_by on a renamed field emits the SQL column name and aliases it to the tuple field")]
def test_sql_text_group_by_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    // _group_by requires a projection to be meaningful; use the IGrouping
    // shape with `_._1 |> length` to get COUNT(*) per renamed-column key.
    let sql = _sql_text(_db |> select_from(type<Car>)
                            |> _group_by(_.col_type)
                            |> _select((Kind = _._0, N = _._1 |> length)))
    t |> success(sql |> find("GROUP BY \"type\"") >= 0,
        "_group_by on renamed column emits 'type'")
    t |> success(sql |> find("\"type\" AS \"Kind\", COUNT(*) AS \"N\"") >= 0,
        "projection of group key emits SQL identifier 'type' aliased to the tuple field")
}

[test(description = "a where inside an aggregate emits the SQL column name while unrenamed columns keep theirs")]
def test_sql_text_aggregate_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                            |> _where(_.col_type == "sedan")
                            |> _select(_.Price) |> sum())
    t |> success(sql |> find("\"type\" = ?") >= 0,
        "WHERE inside aggregate emits SQL identifier 'type'")
    t |> success(sql |> find("SUM(\"Price\")") >= 0,
        "SUM uses unaliased Price (no rename)")
}

[test(description = "a where on a renamed field filters correctly at runtime")]
def test_predicate_filters_at_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "Camry", col_type = "sedan", Price = 25000))
        db |> insert(Car(Id = 2, Name = "F-150", col_type = "truck", Price = 40000))
        db |> insert(Car(Id = 3, Name = "Civic", col_type = "sedan", Price = 22000))
        let sedans <- _sql(db |> select_from(type<Car>) |> _where(_.col_type == "sedan"))
        t |> equal(length(sedans), 2, "two sedans")
    }
}

// `column_info(type<T>)` is the introspection API — the `name` field carries
// the SQL-side column name (post-rename) so the daslang-side view matches
// `PRAGMA table_info`.
[test(description = "column_info reports the SQL column name of a renamed field")]
def test_column_info_returns_sql_column_name(t : T?) {
    let infos <- column_info(type<Car>)
    t |> equal(length(infos), 4, "four ColumnInfo entries")
    t |> equal(infos[2].name, "type", "col_type field surfaces as 'type' in column_info")
}

// UPDATE + WHERE on a renamed column.
[test(description = "an update whose where names a renamed field changes the right row")]
def test_sql_update_with_renamed_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "Camry", col_type = "sedan", Price = 25000))
        db |> insert(Car(Id = 2, Name = "F-150", col_type = "truck", Price = 40000))
        let n = _sql_update(db, type<Car>, _.col_type == "truck",
                            (Price = _.Price + 1000))
        t |> equal(n, 1, "one truck price bumped")
        let rows <- db |> select_from(type<Car>)
        for (r in rows) {
            if (r.col_type == "truck") {
                t |> equal(r.Price, 41000, "truck price reflects update")
            } else {
                t |> equal(r.Price, 25000, "sedan price unchanged")
            }
        }
    }
}

// DELETE with WHERE on a renamed column.
[test(description = "a delete whose where names a renamed field removes the right row")]
def test_sql_delete_with_renamed_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "Camry", col_type = "sedan", Price = 25000))
        db |> insert(Car(Id = 2, Name = "F-150", col_type = "truck", Price = 40000))
        let n = _sql_delete(db, type<Car>, _.col_type == "truck")
        t |> equal(n, 1, "one truck deleted")
        let remain <- db |> select_from(type<Car>)
        t |> equal(length(remain), 1, "sedan remains")
        t |> equal(remain[0].col_type, "sedan", "remaining row is sedan")
    }
}

// `@sql_column` rename through a JOIN. Both sides rename a different daslang
// field to a SQLite-keyword column name; the JOIN ON clause must emit the
// SQL identifiers (post-rename) on both sides, alias-qualified.

[sql_table(name = "Vehicles")]
struct Vehicle {
    @sql_primary_key Id : int
    @sql_column = "type"
    col_type : string
    Year : int
}

[sql_table(name = "Models")]
struct Model {
    @sql_primary_key Id : int
    @sql_column = "for"
    col_for : string  // 'for' is a daslang keyword; daslang field is `col_for`
    Maker : string
}

[test(description = "a join on two renamed columns emits both SQL names qualified by their aliases")]
def test_join_alias_qualifies_both_renamed_columns(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Vehicle>)
        |> _join(_db |> select_from(type<Model>),
                 $(v : Vehicle, m : Model) => v.col_type == m.col_for,
                 $(v : Vehicle, m : Model) => (Type = v.col_type, Make = m.Maker)))
    t |> success(sql |> find("\"t0\".\"type\" = \"t1\".\"for\"") >= 0,
        "JOIN ON emits both renames as SQL identifiers, alias-qualified")
    t |> success(sql |> find("\"t0\".\"col_type\"") < 0,
        "daslang name 'col_type' must NOT appear")
    t |> success(sql |> find("\"t1\".\"col_for\"") < 0,
        "daslang name 'col_for' must NOT appear")
}

[test(description = "a join projection of a renamed column emits the SQL name qualified by its alias and aliased to the tuple field")]
def test_join_projection_emits_renamed_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Vehicle>)
        |> _join(_db |> select_from(type<Model>),
                 $(v : Vehicle, m : Model) => v.col_type == m.col_for,
                 $(v : Vehicle, m : Model) => (T = v.col_type, M = m.Maker)))
    t |> success(sql |> find("SELECT \"t0\".\"type\" AS \"T\", \"t1\".\"Maker\" AS \"M\"") >= 0,
        "projection of renamed column emits 'type' qualified by alias, aliased to the tuple field")
}

[test(description = "a join across renamed columns matches the right rows at runtime")]
def test_join_left_join_runtime_with_renames(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Vehicle>)
        db |> create_table(type<Model>)
        db |> insert(Vehicle(Id = 1, col_type = "sedan", Year = 2024))
        db |> insert(Vehicle(Id = 2, col_type = "truck", Year = 2023))
        db |> insert(Model(Id = 1, col_for = "sedan", Maker = "Toyota"))
        db |> insert(Model(Id = 2, col_for = "sedan", Maker = "Honda"))
        db |> insert(Model(Id = 3, col_for = "suv", Maker = "Subaru"))
        let pairs <- _sql(db |> select_from(type<Vehicle>)
            |> _join(db |> select_from(type<Model>),
                     $(v : Vehicle, m : Model) => v.col_type == m.col_for,
                     $(v : Vehicle, m : Model) => (Y = v.Year, K = v.col_type, Mk = m.Maker)))
        t |> equal(length(pairs), 2, "two sedans matched against models")
    }
}

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

[test(description = "sql_index on a renamed field emits the SQL column name in CREATE INDEX")]
def test_sql_index_translates_daslang_to_sql_column(t : T?) {
    let idx_sql = _::_sql_create_indexes_sql(type<Category>)
    t |> success(idx_sql |> find("(\"label\")") >= 0,
        "[sql_index] field='col_label' emits CREATE INDEX (\"label\") at SQL")
    t |> success(idx_sql |> find("col_label") < 0,
        "daslang field name 'col_label' must NOT appear in DDL")
}

[test(description = "an explicit sql_index name is kept when the field is renamed")]
def test_sql_index_explicit_name_preserved(t : T?) {
    let idx_sql = _::_sql_create_indexes_sql(type<Category>)
    t |> success(idx_sql |> find("\"idx_label\"") >= 0,
        "explicit index name 'idx_label' preserved")
}

[test(description = "sql_references points at the renamed primary key column of the parent")]
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
[test(description = "a foreign key to a renamed parent key column accepts a valid child row")]
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
