options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

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

[test]
def test_native_init_emits_default_in_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Item>)
    t |> success(find(ddl, "\"Active\" INTEGER NOT NULL DEFAULT 1") >= 0,
                 "bool field-init = true → DEFAULT 1")
    t |> success(find(ddl, "\"Quantity\" INTEGER NOT NULL DEFAULT 100") >= 0,
                 "int field-init = 100 → DEFAULT 100")
    t |> success(find(ddl, "\"Tag\" TEXT NOT NULL DEFAULT 'unknown'") >= 0,
                 "string field-init = \"unknown\" → DEFAULT 'unknown'")
}

[test]
def test_default_fn_emits_in_ddl(t : T?) {
    let ddl = _sql_create_table_sql(type<Item>)
    t |> success(find(ddl, "\"CreatedAt\" TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP") >= 0,
                 "@sql_default_fn=CURRENT_TIMESTAMP → DEFAULT CURRENT_TIMESTAMP (no quotes)")
}

[test]
def test_computed_emits_generated_clause(t : T?) {
    let ddl = _sql_create_table_sql(type<Item>)
    t |> success(find(ddl, "\"DoubleQty\" INTEGER GENERATED ALWAYS AS (Quantity * 2) VIRTUAL") >= 0,
                 "@sql_computed without @sql_stored → VIRTUAL")
    t |> success(find(ddl, "\"QtyPlusOne\" INTEGER GENERATED ALWAYS AS (Quantity + 1) STORED") >= 0,
                 "@sql_computed + @sql_stored=true → STORED")
}

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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
