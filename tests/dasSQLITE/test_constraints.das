options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/option
require daslib/sql_linq

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    @sql_unique Email : string
    Name : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert(User(Id = 1, Email = "a@x.com", Name = "alice"))
}

[test(description = "sql_unique is emitted as a separate CREATE UNIQUE INDEX, not as an inline column constraint")]
def test_unique_emitted_as_portable_index(t : T?) {
    let table_ddl = _sql_create_table_sql(type<User>)
    t |> success(table_ddl |> find("\"Email\" TEXT NOT NULL UNIQUE") < 0,
        "@sql_unique is emitted through the provider-neutral index helper")
    t |> equal(_sql_create_indexes_sql(type<User>),
        "CREATE UNIQUE INDEX \"uq_Users_Email\" ON \"Users\" (\"Email\")")
    t |> equal(_::_sql_create_indexes_sql(default<User>, "UsersNext"),
        "CREATE UNIQUE INDEX \"uq_UsersNext_Email\" ON \"UsersNext\" (\"Email\")",
        "named-table helper rewrites both the table and generated index name")
}

[test(description = "a sql_unique column rejects a duplicate value on insert")]
def test_unique_index_rejects_duplicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r = db |> try_insert(User(Id = 2, Email = "a@x.com", Name = "anne"))
        t |> success(r |> is_err, "duplicate Email should violate UNIQUE")
    }
}

[test(description = "a sql_unique column accepts distinct values")]
def test_unique_does_not_block_distinct_emails(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> insert(User(Id = 2, Email = "b@x.com", Name = "bob"))
        let total = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
        t |> equal(total, 2, "different Email values insert fine")
    }
}

[sql_table(name = "Users")]
struct User_58 {
    @sql_primary_key Id : int
    Name : string
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int

    @sql_references = "User_58"
    @sql_on_delete = "cascade"
    UserId : int

    Total : float
}

def private fixture_58(db : SqlRunner) {
    db |> create_table(type<User_58>)
    db |> create_table(type<Order>)
    db |> insert(User_58(Id = 1, Name = "alice"))
    db |> insert(User_58(Id = 2, Name = "bob"))
    db |> insert(Order(Id = 100, UserId = 1, Total = 42.0f))
    db |> insert(Order(Id = 101, UserId = 1, Total = 99.0f))
    db |> insert(Order(Id = 102, UserId = 2, Total = 7.5f))
}

[test(description = "sql_references emits a FOREIGN KEY clause naming the parent primary key column with its cascade actions")]
def test_references_emitted_in_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Order>)
    t |> success(find(ddl, "REFERENCES \"Users\"(\"Id\") ON DELETE CASCADE ON UPDATE NO ACTION") >= 0,
                 "@sql_references emits the FK clause with the parent's PK column resolved")
}

[test(description = "with_sqlite turns the foreign_keys pragma on")]
def test_pragma_foreign_keys_on(t : T?) {
    with_sqlite(":memory:") $(db) {
        let pragma = db |> query_scalar("PRAGMA foreign_keys", type<int>)
        t |> equal(pragma, 1, "with_sqlite turns FKs on")
    }
}

[test(description = "deleting a parent row cascades to its child rows and leaves other children alone")]
def test_cascade_delete_propagates(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_58(db)
        db |> exec("DELETE FROM Users WHERE Id = 1")
        let alice_orders = db |> query_scalar("SELECT COUNT(*) FROM Orders WHERE UserId = 1", type<int>)
        t |> equal(alice_orders, 0, "alice's orders cascade-deleted")
        let bob_orders = db |> query_scalar("SELECT COUNT(*) FROM Orders WHERE UserId = 2", type<int>)
        t |> equal(bob_orders, 1, "bob's orders untouched")
    }
}

[test(description = "inserting a child row whose parent is missing is rejected by the foreign key")]
def test_orphan_insert_rejected(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_58(db)
        let r = db |> try_insert(Order(Id = 999, UserId = 9999, Total = 1.0f))
        t |> success(r |> is_err, "FK violation on missing parent row")
    }
}

[sql_table(name = "Posts"),
 sql_index(fields = "AuthorId"),
 sql_index(fields = ("AuthorId", "Published"), name = "idx_posts_author_published"),
 sql_index(fields = "Slug", unique = true)]

struct Post {
    @sql_primary_key Id : int
    AuthorId : int
    Slug : string
    Published : int64
    Title : string
}

[test(description = "the generated index SQL contains the auto-named, explicitly named composite and unique indexes")]
def test_indexes_helper_concatenates(t : T?) {
    let sql = _sql_create_indexes_sql(type<Post>)
    t |> success(find(sql, "CREATE INDEX \"idx_Posts_AuthorId\"") >= 0, "auto-named single-col index")
    t |> success(find(sql, "CREATE INDEX \"idx_posts_author_published\"") >= 0, "explicit-named composite index")
    t |> success(find(sql, "CREATE UNIQUE INDEX \"idx_Posts_Slug\"") >= 0, "auto-named unique index")
}

[test(description = "create_table registers every declared index with SQLite")]
def test_indexes_actually_created_in_sqlite(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Post>)
        let n = db |> query_scalar("SELECT COUNT(*) FROM sqlite_master WHERE type='index' AND tbl_name='Posts' AND name LIKE 'idx_%'", type<int>)
        t |> equal(n, 3, "all three indexes registered with SQLite")
    }
}

[test(description = "a unique sql_index rejects a duplicate value")]
def test_unique_index_rejects_duplicate_59(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Post>)
        db |> insert(Post(Id = 1, AuthorId = 1, Slug = "first", Published = 0l, Title = "First"))
        let r = db |> try_insert(Post(Id = 2, AuthorId = 1, Slug = "first", Published = 0l, Title = "Dup"))
        t |> success(r |> is_err, "duplicate Slug rejected by UNIQUE INDEX")
    }
}

// Native daslang field-init becomes the SQL DEFAULT clause: literals only
// (int/int64/bool/float/double/string). Non-literal initializers are silently
// dropped from the DDL but still run in daslang construction. @sql_default_fn
// covers the SQLite built-ins (CURRENT_TIMESTAMP / CURRENT_DATE / CURRENT_TIME).
// @sql_computed emits ` GENERATED ALWAYS AS (...) VIRTUAL|STORED` and skips the
// column from INSERT/UPDATE bind paths.

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int

    Name : string

    // Native field init becomes ` DEFAULT 1`.
    Active : bool = true

    // Native field init becomes ` DEFAULT 100`.
    Quantity : int = 100

    // Native field init becomes ` DEFAULT 'unknown'` (single quotes doubled).
    Tag : string = "unknown"

    // SQL-side built-in default.
    @sql_default_fn = "CURRENT_TIMESTAMP"
    CreatedAt : string

    // VIRTUAL generated column (the default storage class).
    @sql_computed = "Quantity * 2"
    DoubleQty : int

    // STORED generated column.
    @sql_computed = "Quantity + 1"
    @sql_stored = true
    QtyPlusOne : int
}

[test(description = "field initializers of bool, int and string become DEFAULT clauses in the table DDL")]
def test_native_init_emits_default_in_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Item>)
    t |> success(find(ddl, "\"Active\" INTEGER NOT NULL DEFAULT 1") >= 0,
                 "bool field-init = true → DEFAULT 1")
    t |> success(find(ddl, "\"Quantity\" INTEGER NOT NULL DEFAULT 100") >= 0,
                 "int field-init = 100 → DEFAULT 100")
    t |> success(find(ddl, "\"Tag\" TEXT NOT NULL DEFAULT 'unknown'") >= 0,
                 "string field-init = \"unknown\" → DEFAULT 'unknown'")
}

[test(description = "sql_default_fn emits an unquoted SQL function call as the column default")]
def test_default_fn_emits_in_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Item>)
    t |> success(find(ddl, "\"CreatedAt\" TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP") >= 0,
                 "@sql_default_fn=CURRENT_TIMESTAMP → DEFAULT CURRENT_TIMESTAMP (no quotes)")
}

[test(description = "sql_computed emits a generated column, VIRTUAL by default and STORED when asked")]
def test_computed_emits_generated_clause(t : T?) {
    let ddl = _sql_create_table_sql(type<Item>)
    t |> success(find(ddl, "\"DoubleQty\" INTEGER GENERATED ALWAYS AS (Quantity * 2) VIRTUAL") >= 0,
                 "@sql_computed without @sql_stored → VIRTUAL")
    t |> success(find(ddl, "\"QtyPlusOne\" INTEGER GENERATED ALWAYS AS (Quantity + 1) STORED") >= 0,
                 "@sql_computed + @sql_stored=true → STORED")
}

[test(description = "the generated INSERT never names a computed column")]
def test_insert_sql_skips_computed_columns(t : T?) {
    let sql = _sql_insert_with_pk_sql(type<Item>)
    // Computed columns must be absent from the INSERT column list.
    t |> success(find(sql, "DoubleQty") == -1, "INSERT does not name computed column DoubleQty")
    t |> success(find(sql, "QtyPlusOne") == -1, "INSERT does not name computed column QtyPlusOne")
    // Non-computed columns must all be present.
    t |> success(find(sql, "\"Id\"") >= 0, "INSERT names Id")
    t |> success(find(sql, "\"Quantity\"") >= 0, "INSERT names Quantity")
    t |> success(find(sql, "\"CreatedAt\"") >= 0, "INSERT names CreatedAt")
}

[test(description = "an unset field takes its DEFAULT value in the database")]
def test_native_default_applied_when_field_left_zero(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Item>)
        // Insert raw row — let SQLite fill in DEFAULTs by going through the SQL
        // path. We use `exec` so SQLite handles the DEFAULT on omitted columns;
        // the daslang `insert` macro always binds every non-computed column.
        db |> exec("INSERT INTO Items (Id, Name) VALUES (1, 'x')")
        let n = db |> query_scalar("SELECT Quantity FROM Items WHERE Id=1", type<int>)
        t |> equal(n, 100, "Quantity got DEFAULT 100")
        let s = db |> query_scalar("SELECT Tag FROM Items WHERE Id=1", type<string>)
        t |> equal(s, "unknown", "Tag got DEFAULT 'unknown'")
        let active = db |> query_scalar("SELECT Active FROM Items WHERE Id=1", type<int>)
        t |> equal(active, 1, "Active got DEFAULT 1 (true)")
    }
}

[test(description = "a sql_default_fn column is filled by the database on insert")]
def test_default_fn_populates_created_at(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Item>)
        db |> exec("INSERT INTO Items (Id, Name) VALUES (1, 'x')")
        let s = db |> query_scalar("SELECT CreatedAt FROM Items WHERE Id=1", type<string>)
        // Just check it's non-empty and looks like a SQLite timestamp
        // (YYYY-MM-DD HH:MM:SS — at least 19 chars, has the dashes/colons).
        t |> success(length(s) >= 19, "CreatedAt populated by CURRENT_TIMESTAMP (length {length(s)})")
    }
}

[test(description = "computed columns are derived by SQLite from the inserted values")]
def test_computed_columns_round_trip_via_insert(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Item>)
        // The daslang struct Quantity = 7; the computed columns get their
        // values from SQLite. Construction-side values for DoubleQty /
        // QtyPlusOne are ignored on INSERT (SQL bind path skips them).
        db |> insert(Item(Id = 1, Name = "thing", Active = true, Quantity = 7,
                          Tag = "t", CreatedAt = "",
                          DoubleQty = 9999, QtyPlusOne = 9999))
        let dq = db |> query_scalar("SELECT DoubleQty FROM Items WHERE Id=1", type<int>)
        t |> equal(dq, 14, "VIRTUAL DoubleQty = Quantity*2 = 14")
        let qp1 = db |> query_scalar("SELECT QtyPlusOne FROM Items WHERE Id=1", type<int>)
        t |> equal(qp1, 8, "STORED QtyPlusOne = Quantity+1 = 8")
    }
}

[test(description = "a select reads computed columns back into their fields")]
def test_first_reads_computed_columns(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Item>)
        db |> insert(Item(Id = 1, Name = "thing", Active = true, Quantity = 5,
                          Tag = "t", CreatedAt = "",
                          DoubleQty = 0, QtyPlusOne = 0))
        let r = _sql(db |> select_from(type<Item>) |> _first())
        t |> equal(r.Quantity, 5, "Quantity round-trips")
        t |> equal(r.DoubleQty, 10, "VIRTUAL DoubleQty round-trips through SELECT")
        t |> equal(r.QtyPlusOne, 6, "STORED QtyPlusOne round-trips through SELECT")
    }
}

[test(description = "an update leaves computed columns to be re-derived from the new values")]
def test_update_skips_computed_columns(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Item>)
        db |> insert(Item(Id = 1, Name = "thing", Active = true, Quantity = 3,
                          Tag = "t", CreatedAt = "",
                          DoubleQty = 0, QtyPlusOne = 0))
        // Mutate Quantity via the macro update — computed columns must NOT
        // appear in the SET list, otherwise SQLite rejects the write.
        db |> _sql_update(type<Item>, _.Id == 1, (Quantity = 11))
        let dq = db |> query_scalar("SELECT DoubleQty FROM Items WHERE Id=1", type<int>)
        t |> equal(dq, 22, "DoubleQty re-derived from new Quantity (11*2=22)")
        let qp1 = db |> query_scalar("SELECT QtyPlusOne FROM Items WHERE Id=1", type<int>)
        t |> equal(qp1, 12, "QtyPlusOne re-derived from new Quantity (11+1=12)")
    }
}
