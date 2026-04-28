options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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

[test]
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

[test]
def test_join_projection_emits_renamed_column(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Vehicle>)
        |> _join(_db |> select_from(type<Model>),
                 $(v : Vehicle, m : Model) => v.col_type == m.col_for,
                 $(v : Vehicle, m : Model) => (T = v.col_type, M = m.Maker)))
    t |> success(sql |> find("SELECT \"t0\".\"type\", \"t1\".\"Maker\"") >= 0,
        "projection of renamed column emits 'type' qualified by alias")
}

[test]
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
