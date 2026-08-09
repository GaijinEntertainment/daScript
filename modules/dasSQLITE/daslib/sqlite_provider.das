options gen2
options indenting = 4
options no_unused_function_arguments = false
options strict_smart_pointers

// Intentionally opt in for user-facing docs: keep comments concise and actionable.
options _comment_hygiene = true

module sqlite_provider shared public

require sqlite
require strings
require daslib/sql public
require daslib/sql_provider public
require daslib/ast_boost
require daslib/templates
require daslib/templates_boost

//! SQLite's `SqlProviderInfo` registration shim — the lean bridge between the raw
//! `sqlite` C binding and the neutral registry in `daslib/sql_provider`. Deliberately
//! does NOT require `sqlite/sqlite_boost`: `daslib/sql_provider` pulls this file into
//! every consulting macro context via `require ?sqlite`, and requiring the full boost
//! here would cycle (sqlite_boost → daslib/sql_boost → daslib/sql_provider → here).
//! Runtime surface (SqlRunner, exec/query, runner helpers) lives in `sqlite/sqlite_boost`.

def sqlite3_column_text_(stmt : sqlite3_stmt?; col : int) : string {
    //! Returns the text value of a column in the current row of a result set of a SELECT statement.
    unsafe {
        return clone_string(sqlite3_column_text(stmt, col))
    }
}

def sqlite3_prepare_v2(db : sqlite3?; sql : string; var stmt : sqlite3_stmt?&) {
    //! Prepares a SQL statement for execution. Routes through a C++ shim that drops the
    //! unused pzTail parameter (sidesteps an AOT codegen bug on ``string const?``).
    return sqlite3_prepare_v2_no_tail(db, sql, unsafe(addr(stmt)))
}

def sqlite_sql_type(t : SqlType) : string {
    //! Render an abstract ``SqlType`` to its SQLite-dialect spelling.
    //! Other providers ship their own helper so ``ColumnInfo`` round-trips across backends.
    if (t == SqlType.Integer) return "INTEGER"
    if (t == SqlType.Real) return "REAL"
    if (t == SqlType.Text) return "TEXT"
    if (t == SqlType.Blob) return "BLOB"
    return "NULL"
}

def sqlite_affinity_to_sql_type(decl_type : string) : SqlType {
    //! SQLite affinity rules: INTEGER first, then TEXT, BLOB, REAL, NUMERIC last (https://www.sqlite.org/datatype3.html § 3.1).
    let upper = decl_type |> to_upper
    if (find(upper, "INT") >= 0) return SqlType.Integer
    if (find(upper, "CHAR") >= 0 || find(upper, "CLOB") >= 0 || find(upper, "TEXT") >= 0) return SqlType.Text
    if (find(upper, "BLOB") >= 0 || empty(decl_type)) return SqlType.Blob
    if (find(upper, "REAL") >= 0 || find(upper, "FLOA") >= 0 || find(upper, "DOUB") >= 0) return SqlType.Real
    // NUMERIC affinity (NUMERIC/DECIMAL/BOOLEAN/DATE/DATETIME) → double; user partial-body overrides for BOOL/DATE.
    return SqlType.Real
}

def sqlite_read_table_info(handle : sqlite3?; schema_name : string; tbl_name : string) : Result<array<SchemaFromCol>, string> {
    //! `PRAGMA table_xinfo` → normalized column metadata (`data_type` via affinity rules).
    //! `table_xinfo`, not `table_info` - the latter omits GENERATED columns entirely.
    //! `schema_name` qualifies the pragma for ATTACH/with_schema readers; empty for main.
    var rows : array<SchemaFromCol>
    let sql = empty(schema_name) ? "PRAGMA table_xinfo({sql_quote_id(tbl_name)})" : "PRAGMA {sql_quote_id(schema_name)}.table_xinfo({sql_quote_id(tbl_name)})"
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "PRAGMA table_xinfo({tbl_name}) prepare failed: {sqlite3_errmsg(handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<array<SchemaFromCol>>)
    }
    while (true) {
        let src = sqlite3_step(stmt)
        if (src == SQLITE_DONE) break
        if (src != SQLITE_ROW) {
            let msg = "PRAGMA table_xinfo({tbl_name}) step failed: {sqlite3_errmsg(handle)}"
            sqlite3_finalize(stmt)
            return err(msg, type<array<SchemaFromCol>>)
        }
        // Columns: 0=cid, 1=name, 2=type, 3=notnull, 4=dflt_value, 5=pk, 6=hidden
        // hidden: 0=ordinary, 1=virtual-table hidden column, 2=VIRTUAL generated, 3=STORED generated
        let hidden = sqlite3_column_int(stmt, 6)
        if (hidden == 1) continue
        let dt = sqlite3_column_text_(stmt, 2)
        rows |> emplace(SchemaFromCol(
            name = sqlite3_column_text_(stmt, 1),
            decl_type = dt,
            data_type = sqlite_affinity_to_sql_type(dt),
            not_null = sqlite3_column_int(stmt, 3) != 0,
            pk = sqlite3_column_int(stmt, 5) != 0,
            generated = hidden == 2 || hidden == 3
        ))
    }
    sqlite3_finalize(stmt)
    return <- move_ok(rows, type<string>)
}

def private _sqlite_read_table_schema(path : string; tbl_name : string) : Result<array<SchemaFromCol>, string> {
    // READONLY open via the no-vfs C++ shim — read-only so a bad path won't create a stray file.
    var handle : sqlite3? = null
    let rc = sqlite3_open_v2_no_vfs(path, unsafe(addr(handle)), int(SQLITE_OPEN_READONLY))
    if (rc != SQLITE_OK) {
        // OOM-on-open writes NULL into *ppDb; sqlite3_errmsg(NULL) is undefined, close_v2(NULL) is a no-op.
        let msg = handle == null ? "sqlite3_open_v2(\"{path}\", READONLY) failed: out of memory" : "sqlite3_open_v2(\"{path}\", READONLY) failed: {sqlite3_errmsg(handle)}"
        sqlite3_close_v2(handle)
        return err(msg, type<array<SchemaFromCol>>)
    }
    var r <- sqlite_read_table_info(handle, "", tbl_name)
    sqlite3_close_v2(handle)
    return <- r
}

def private _sqlite_make_stmt_type() : TypeDeclPtr => qmacro_type(type<sqlite3_stmt?>)

def private _sqlite_make_stmt_type_const() : TypeDeclPtr => qmacro_type(type<sqlite3_stmt? const>)

[unused_argument(ordinal)]
def private _sqlite_render_placeholder(ordinal : int) : string => "?"

def private _sqlite_render_json_extract(target : string; jsonPath : string) : string => "json_extract({target}, {jsonPath})"

def private _sqlite_quote_id(id : string) : string => sql_quote_id(id)

def register_sqlite_provider {
    //! Registers the sqlite `SqlProviderInfo` entry in the CURRENT macro context.
    //! Reached via `sql_register_present_providers` (daslib/sql_provider) from each
    //! consulting macro module's own `[_macro]`.
    var info = SqlProviderInfo(
        name = "sqlite",
        runnerModule = "sqlite_boost",
        runnerStruct = "SqlRunner",
        makeStmtType = @@_sqlite_make_stmt_type,
        makeStmtTypeConst = @@_sqlite_make_stmt_type_const,
        renderPlaceholder = @@_sqlite_render_placeholder,
        noLimitSpelling = "LIMIT -1",
        renderJsonExtract = @@_sqlite_render_json_extract,
        quoteId = @@_sqlite_quote_id,
        sqlTypeFor = @@sqlite_sql_type,
        identityDdl = "PRIMARY KEY",
        readTableSchema = @@_sqlite_read_table_schema,
        caps = SqlProviderCaps.fts5 | SqlProviderCaps.client_udfs | SqlProviderCaps.returning,
        pkReport = SqlPkReport.LastInsertRowid)
    register_sql_provider(info)
}
