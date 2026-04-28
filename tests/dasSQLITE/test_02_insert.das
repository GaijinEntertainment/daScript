options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

[test]
def test_macro_emits_table_name(t : T?) {
    t |> equal(_sql_table_name(type<Car>), "Cars", "[sql_table(name=...)] sets the on-disk name")
}

[test]
def test_macro_emits_create_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Car>)
    t |> success(ddl |> starts_with("CREATE TABLE \"Cars\"("), "create DDL must open with CREATE TABLE \"Cars\"(")
    t |> success(ddl |> ends_with(")"), "create DDL must close with )")
}

[test]
def test_macro_emits_insert_shapes(t : T?) {
    let with_pk = _sql_insert_with_pk_sql(type<Car>)
    let no_pk = _sql_insert_no_pk_sql(type<Car>)
    t |> equal(with_pk, "INSERT INTO \"Cars\" (\"Id\", \"Name\", \"Price\") VALUES (?, ?, ?)", "with-PK insert form (chunk 7: column-list form is now mandatory so computed columns can be skipped)")
    t |> equal(no_pk, "INSERT INTO \"Cars\" (\"Name\", \"Price\") VALUES (?, ?)", "no-PK insert form")
}

[test]
def test_create_drop_roundtrip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let n_after_create = db |> query_scalar("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='Cars'", type<string>)
        t |> equal(n_after_create, "1", "Cars table must exist after create_table")
        db |> drop_table_if_exists(type<Car>)
        let n_after_drop = db |> query_scalar("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='Cars'", type<string>)
        t |> equal(n_after_drop, "0", "Cars table must be gone after drop_table_if_exists")
    }
}

[test]
def test_insert_single_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let id = db |> insert(Car(Id = 42, Name = "Audi", Price = 52642))
        t |> equal(id, int64(42), "insert returns the explicit PK as rowid")
        let cnt = db |> query_scalar("SELECT COUNT(*) FROM Cars", type<string>)
        t |> equal(cnt, "1", "row count after one insert")
        let nm = db |> query_scalar("SELECT Name FROM Cars WHERE Id = 42", type<string>)
        t |> equal(nm, "Audi", "round-trip Name field")
    }
}

[test]
def test_insert_array_batched(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        let last_id = db |> insert([
            Car(Id=1, Name="Audi", Price=52642),
            Car(Id=2, Name="Mercedes", Price=57127),
            Car(Id=3, Name="Skoda", Price=9000),
            Car(Id=4, Name="Volvo", Price=29000),
            Car(Id=5, Name="Bentley", Price=350000),
            Car(Id=6, Name="Citroen", Price=21000),
            Car(Id=7, Name="Hummer", Price=41400),
            Car(Id=8, Name="Volkswagen", Price=21600)
        ])
        t |> equal(last_id, int64(8), "last rowid of 8-row insert")
        let cnt = db |> query_scalar("SELECT COUNT(*) FROM Cars", type<string>)
        t |> equal(cnt, "8", "all 8 rows landed")
        let bentley = db |> query_scalar("SELECT Name FROM Cars WHERE Price = 350000", type<string>)
        t |> equal(bentley, "Bentley", "expensive car query")
    }
}

[test]
def test_try_insert_constraint_violation(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "Audi", Price = 52642))
        // Re-insert with same PK must fail with SQLITE_CONSTRAINT
        let r = db |> try_insert(Car(Id = 1, Name = "Other", Price = 0))
        t |> success(r |> is_err, "duplicate PK must Err")
    }
}

[test]
def test_try_insert_mixed_pk_array_rejected(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        // Mixed PK presence: row 0 has Id=0 (unset), row 1 has Id=42 (set).
        // Must Err without writing — array shape decides one INSERT statement;
        // mixing rows would silently mis-bind columns.
        let r = db |> try_insert([
            Car(Name = "Tesla"),
            Car(Id = 42, Name = "Ford")
        ])
        t |> success(r |> is_err, "mixed-PK array must Err")
        // Nothing was inserted (validation runs before BEGIN).
        let cnt = db |> query_scalar("SELECT COUNT(*) FROM Cars", type<string>)
        t |> equal(cnt, "0", "mixed-PK array must not write any row")
    }
}

[sql_table(name="Tags")]
struct Tag {
    @sql_primary_key Id : int
}

[test]
def test_pk_only_struct_insert_uses_default_values(t : T?) {
    // A struct with only an auto-assigned PK has no non-PK columns.
    // The no-PK INSERT statement must emit `DEFAULT VALUES` (not the invalid
    // `INSERT INTO T () VALUES ()`) so a row can be created with just the
    // SQLite-assigned rowid.
    let sql = _sql_insert_no_pk_sql(type<Tag>)
    t |> equal(sql, "INSERT INTO \"Tags\" DEFAULT VALUES", "PK-only struct must use DEFAULT VALUES form")
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Tag>)
        let id1 = db |> insert(Tag())
        t |> equal(id1, int64(1), "first DEFAULT-VALUES insert assigns rowid 1")
        let id2 = db |> insert(Tag())
        t |> equal(id2, int64(2), "second DEFAULT-VALUES insert assigns rowid 2")
        let cnt = db |> query_scalar("SELECT COUNT(*) FROM Tags", type<string>)
        t |> equal(cnt, "2", "two rows landed via DEFAULT VALUES")
    }
}
