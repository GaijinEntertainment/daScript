options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/strings_boost

[test(description = "sqlite_version answers a non-empty string without an open database")]
def test_sqlite_version_no_db(t : T?) {
    let v = sqlite_version()
    t |> success(!empty(v), "sqlite_version() must return non-empty string")
}

[test(description = "with_sqlite opens an in-memory database, runs its block and lets query_scalar read from it")]
def test_with_sqlite_memory(t : T?) {
    var entered = false
    with_sqlite(":memory:") $(db) {
        entered = true
        let v = db |> query_scalar("SELECT SQLITE_VERSION()", type<string>)
        t |> success(!empty(v), "query_scalar must return non-empty version")
    }
    t |> success(entered, "with_sqlite block must run")
}

[test(description = "open_sqlite populates the handle and scope exit finalizes and nulls it")]
def test_open_close(t : T?) {
    var inscope db = open_sqlite(":memory:")
    t |> success(db.sqlite_handle != null, "open_sqlite must populate sqlite_handle")
    // finalize fires at scope exit and nulls the handle
}

[test(description = "try_open_sqlite on an uncreatable path returns an error with the message")]
def test_try_open_failure_returns_err(t : T?) {
    // /no/such/dir/x.db cannot be created on Windows or Unix
    let r = try_open_sqlite("/no/such/dir/x.db")
    t |> success(r |> is_err, "try_open_sqlite must return err(errmsg) on bad path")
}

[test(description = "query_scalar round-trips a string value")]
def test_query_scalar_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 'hello'", type<string>)
        t |> equal(v, "hello", "scalar string round-trip")
    }
}

[test(description = "try_query_scalar on a query with no rows returns an error")]
def test_try_query_scalar_zero_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        // SELECT WHERE FALSE returns no rows — try_ form must surface Err.
        let r = db |> try_query_scalar("SELECT 'x' WHERE 1=0", type<string>)
        t |> success(r |> is_err, "zero-rows scalar must Err")
    }
}

[test(description = "try_exec on invalid SQL returns the error message instead of panicking")]
def test_try_exec_failure(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> try_exec("THIS IS NOT VALID SQL")
        t |> success(err |> is_some, "try_exec must return some(errmsg) on bad SQL")
    }
}

[test(description = "last_insert_rowid is 0 before any insert")]
def test_last_insert_rowid_zero_initially(t : T?) {
    with_sqlite(":memory:") $(db) {
        let id = db |> last_insert_rowid()
        t |> equal(id, int64(0), "last_insert_rowid is 0 before any insert")
    }
}

[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : int
}

[test(description = "sql_table name sets the on-disk table name the macro reports")]
def test_macro_emits_table_name(t : T?) {
    t |> equal(_sql_table_name(type<Car>), "Cars", "[sql_table(name=...)] sets the on-disk name")
}

[test(description = "the generated create DDL opens with CREATE TABLE and the quoted table name and closes with a parenthesis")]
def test_macro_emits_create_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Car>)
    t |> success(ddl |> starts_with("CREATE TABLE \"Cars\"("), "create DDL must open with CREATE TABLE \"Cars\"(")
    t |> success(ddl |> ends_with(")"), "create DDL must close with )")
}

[test(description = "the generated INSERT lists the columns, with the primary key when it is set and without it when it is not")]
def test_macro_emits_insert_shapes(t : T?) {
    let with_pk = _sql_insert_with_pk_sql(type<Car>)
    let no_pk = _sql_insert_no_pk_sql(type<Car>)
    t |> equal(with_pk, "INSERT INTO \"Cars\" (\"Id\", \"Name\", \"Price\") VALUES (?, ?, ?)", "with-PK insert form (chunk 7: column-list form is now mandatory so computed columns can be skipped)")
    t |> equal(no_pk, "INSERT INTO \"Cars\" (\"Name\", \"Price\") VALUES (?, ?)", "no-PK insert form")
}

[test(description = "create_table makes the table exist and drop_table_if_exists removes it")]
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

[test(description = "insert of one row returns the explicit primary key and the row reads back intact")]
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

[test(description = "insert of an array lands every row and returns the last rowid")]
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

[test(description = "try_insert of a duplicate primary key returns an error")]
def test_try_insert_constraint_violation(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)
        db |> insert(Car(Id = 1, Name = "Audi", Price = 52642))
        // Re-insert with same PK must fail with SQLITE_CONSTRAINT
        let r = db |> try_insert(Car(Id = 1, Name = "Other", Price = 0))
        t |> success(r |> is_err, "duplicate PK must Err")
    }
}

[test(description = "try_insert of an array that mixes set and unset primary keys is rejected before anything is written")]
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

[test(description = "a struct holding only an auto primary key inserts with DEFAULT VALUES and receives sequential rowids")]
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

[sql_table(name="Friends")]
struct Friend {
    @sql_primary_key Id : int
    Name : string
}

[test(description = "insert without a primary key value assigns the next rowid and stores the row under it")]
def test_no_pk_insert_assigns_rowid(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let new_id = db |> insert(Friend(Name = "Tom"))
        t |> equal(new_id, int64(1), "first no-PK insert assigns rowid 1")
        let nm = db |> query_scalar("SELECT Name FROM Friends WHERE Id = 1", type<string>)
        t |> equal(nm, "Tom", "row stored under assigned rowid")
    }
}

[test(description = "insert with an explicit primary key keeps that id")]
def test_explicit_pk_insert_keeps_id(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let new_id = db |> insert(Friend(Id = 42, Name = "Roger"))
        t |> equal(new_id, int64(42), "explicit PK is preserved")
        let nm = db |> query_scalar("SELECT Name FROM Friends WHERE Id = 42", type<string>)
        t |> equal(nm, "Roger", "row landed at explicit Id")
    }
}

[test(description = "array insert returns the rowid of the last row")]
def test_array_insert_returns_last_rowid(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let last_id = db |> insert([
            Friend(Name="Tom"),
            Friend(Name="Rebecca"),
            Friend(Name="Jim"),
            Friend(Name="Roger"),
            Friend(Name="Robert")
        ])
        t |> equal(last_id, int64(5), "array insert returns last rowid")
    }
}

[test(description = "the value insert returns equals last_insert_rowid queried afterwards")]
def test_last_insert_rowid_matches_insert_return(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        let returned = db |> insert(Friend(Name = "One"))
        let queried = db |> last_insert_rowid()
        t |> equal(returned, queried, "insert return value equals last_insert_rowid()")
    }
}

[test(description = "the primary-key-unset predicate is true for a zero id and false for a set id")]
def test_pk_is_unset_predicate(t : T?) {
    let unset = Friend(Name = "X")
    let set_ = Friend(Id = 42, Name = "X")
    t |> success(_sql_pk_is_unset(unset), "Id=0 is unset")
    t |> success(!_sql_pk_is_unset(set_), "Id=42 is set")
}

[test(description = "after drop and create the rowid sequence restarts at 1")]
def test_drop_then_recreate(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        db |> insert(Friend(Name = "Tom"))
        db |> drop_table_if_exists(type<Friend>)
        // Drop must succeed even though it existed.
        db |> create_table(type<Friend>)
        // After recreate, rowid sequence restarts at 1 for INTEGER PRIMARY KEY.
        let new_id = db |> insert(Friend(Name = "Reset"))
        t |> equal(new_id, int64(1), "rowid restarts at 1 after drop+create")
    }
}

// User-defined column type. Stored as TEXT (sql_bind returns string), formatted
// "r,g,b" on bind, parsed on read-back. Exercises the tut-26 sql_bind /
// sql_extract adapter rail.
struct Color {
    r : int
    g : int
    b : int
}

def sql_bind(value : Color) : string => "{value.r},{value.g},{value.b}"

[unused_argument(t)]
def sql_extract(s : string; t : type<Color>) : Color {
    let parts <- split(s, ",")
    return Color(r = to_int(parts[0]), g = to_int(parts[1]), b = to_int(parts[2]))
}

[sql_table(name="Pixels")]
struct Pixel {
    @sql_primary_key Id : int
    Tint : Color
}

[test(description = "a user sql_bind return type drives the column type in the create DDL")]
def test_user_type_in_create_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Pixel>)
    t |> success(ddl |> find("\"Tint\" TEXT NOT NULL") >= 0, "user sql_bind return type drives DDL column type")
}

[test(description = "a user type is formatted by its sql_bind on insert")]
def test_user_type_round_trip(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Pixel>)
        db |> insert(Pixel(Id = 1, Tint = Color(r = 255, g = 64, b = 0)))
        let s = db |> query_scalar("SELECT Tint FROM Pixels WHERE Id = 1", type<string>)
        t |> equal(s, "255,64,0", "user sql_bind formatted the value on insert")
    }
}

[test(description = "select_from reads user-typed columns back through the user sql_extract")]
def test_user_type_select_from(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Pixel>)
        db |> insert(Pixel(Id = 1, Tint = Color(r = 255, g = 64, b = 0)))
        db |> insert(Pixel(Id = 2, Tint = Color(r = 0, g = 128, b = 255)))
        let rows <- db |> select_from(type<Pixel>)
        t |> equal(length(rows), 2, "select_from returns all rows")
        t |> equal(rows[0].Id, 1, "row 0 Id")
        t |> equal(rows[0].Tint.r, 255, "row 0 Tint.r — round-tripped through user sql_extract")
        t |> equal(rows[0].Tint.g, 64, "row 0 Tint.g")
        t |> equal(rows[0].Tint.b, 0, "row 0 Tint.b")
        t |> equal(rows[1].Tint.r, 0, "row 1 Tint.r")
        t |> equal(rows[1].Tint.g, 128, "row 1 Tint.g")
        t |> equal(rows[1].Tint.b, 255, "row 1 Tint.b")
    }
}
