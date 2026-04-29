options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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

[test]
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

[test]
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

[test]
def test_sql_text_predicate_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _where(_.col_type == "sedan"))
    // Predicate emits the SQL identifier (renamed column) — NOT the daslang field name.
    t |> success(sql |> find("\"type\" = ?") >= 0,
        "WHERE on renamed column emits SQL identifier 'type'")
    t |> success(sql |> find("\"col_type\"") < 0,
        "daslang field name 'col_type' must NOT appear in emitted SQL")
}

[test]
def test_sql_text_projection_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.col_type))
    t |> success(sql |> find("SELECT \"type\" FROM \"Cars\"") >= 0,
        "_select on renamed column projects 'type', not 'col_type'")
}

[test]
def test_sql_text_order_by_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _order_by(_.col_type))
    t |> success(sql |> find("ORDER BY \"type\" ASC") >= 0,
        "_order_by on renamed column emits 'type'")
}

[test]
def test_sql_text_group_by_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    // _group_by requires a projection to be meaningful; use the IGrouping
    // shape with `_._1 |> length` to get COUNT(*) per renamed-column key.
    let sql = _sql_text(_db |> select_from(type<Car>)
                            |> _group_by(_.col_type)
                            |> _select((Kind = _._0, N = _._1 |> length)))
    t |> success(sql |> find("GROUP BY \"type\"") >= 0,
        "_group_by on renamed column emits 'type'")
    t |> success(sql |> find("\"type\", COUNT(*)") >= 0,
        "projection of group key emits SQL identifier 'type'")
}

[test]
def test_sql_text_aggregate_uses_sql_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _select(_.Price)
                            |> _where(_.col_type == "sedan") |> sum())
    t |> success(sql |> find("\"type\" = ?") >= 0,
        "WHERE inside aggregate emits SQL identifier 'type'")
    t |> success(sql |> find("SUM(\"Price\")") >= 0,
        "SUM uses unaliased Price (no rename)")
}

[test]
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
[test]
def test_column_info_returns_sql_column_name(t : T?) {
    let infos <- column_info(type<Car>)
    t |> equal(length(infos), 4, "four ColumnInfo entries")
    t |> equal(infos[2].name, "type", "col_type field surfaces as 'type' in column_info")
}

// UPDATE + WHERE on a renamed column.
[test]
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
[test]
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
