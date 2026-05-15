options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

// Intentionally opt in for user-facing docs: keep comments concise and actionable.
options _comment_hygiene = true

module sqlite_boost shared public

require sqlite public

require strings public
require daslib/fio                  // [sql_table(schema_from=...)] uses is_absolute / path_join at macro time
require daslib/contracts
require daslib/option public
require daslib/sql public
require daslib/ast_boost
require daslib/macro_boost
require daslib/templates
require daslib/templates_boost
require daslib/json public          // [sql_table] @sql_json adapter codegen calls JV / read_json / write_json
require daslib/json_boost public    // [sql_table] @sql_json adapter codegen calls from_JV
require daslib/archive public       // [sql_table] @sql_blob adapter codegen calls mem_archive_save / mem_archive_load

def sqlite3_column_text_(stmt : sqlite3_stmt?; col : int) : string {
    //! Returns the text value of a column in the current row of a result set of a SELECT statement.
    unsafe {
        return clone_string(sqlite3_column_text(stmt, col))
    }
}

def sqlite3_free(std : string) {
    //! Frees memory previously allocated by SQLite.
    sqlite3_free(unsafe(reinterpret<void?> std))
}

[expect_any_array(data)]
def sqlite3_bind_blob(stmt : sqlite3_stmt?; index : int; data) {
    //! Binds a blob to a prepared statement at ``index`` (``SQLITE_TRANSIENT`` copy).
    //! Empty blobs route through ``sqlite3_bind_zeroblob`` to avoid ``addr(data[0])`` on zero-length.
    if (empty(data)) return sqlite3_bind_zeroblob(stmt, index, 0)
    unsafe {
        return sqlite3_bind_blob(stmt, index, unsafe(addr(data[0])), (data |> length) * typeinfo sizeof(data[0]))
    }
}

def sqlite3_get_autocommit_(db : sqlite3?) : bool {
    //! Returns true if the database connection is in autocommit mode.
    return sqlite3_get_autocommit(db) != 0
}

def sqlite3_open(filename : string; var db : sqlite3?&) : int {
    //! Opens a new database connection to a SQLite database file.
    return sqlite3_open(filename, unsafe(addr(db)))
}

def sqlite3_prepare_v2(db : sqlite3?; sql : string; var stmt : sqlite3_stmt?&) {
    //! Prepares a SQL statement for execution. Routes through a C++ shim that drops the
    //! unused pzTail parameter (sidesteps an AOT codegen bug on ``string const?``).
    return sqlite3_prepare_v2_no_tail(db, sql, unsafe(addr(stmt)))
}

def private _qualify_schema(sql : string; schema_name : string) : string {
    // String shim, not a parser: works because emitted SQL always quotes table refs and binds values.
    if (empty(schema_name)) return sql
    let q = sql_quote_id(schema_name) + "."
    var out = sql |> replace("FROM \"", "FROM {q}\"")
    out = out |> replace("INTO \"", "INTO {q}\"")
    out = out |> replace("UPDATE \"", "UPDATE {q}\"")
    out = out |> replace("JOIN \"", "JOIN {q}\"")
    out = out |> replace("CREATE TABLE \"", "CREATE TABLE {q}\"")
    out = out |> replace("CREATE TABLE IF NOT EXISTS \"", "CREATE TABLE IF NOT EXISTS {q}\"")
    out = out |> replace("CREATE VIRTUAL TABLE IF NOT EXISTS \"", "CREATE VIRTUAL TABLE IF NOT EXISTS {q}\"")
    out = out |> replace("CREATE VIRTUAL TABLE \"", "CREATE VIRTUAL TABLE {q}\"")
    out = out |> replace("CREATE UNIQUE INDEX IF NOT EXISTS \"", "CREATE UNIQUE INDEX IF NOT EXISTS {q}\"")
    out = out |> replace("CREATE INDEX IF NOT EXISTS \"", "CREATE INDEX IF NOT EXISTS {q}\"")
    out = out |> replace("CREATE UNIQUE INDEX \"", "CREATE UNIQUE INDEX {q}\"")
    out = out |> replace("CREATE INDEX \"", "CREATE INDEX {q}\"")
    out = out |> replace("DROP TABLE IF EXISTS \"", "DROP TABLE IF EXISTS {q}\"")
    out = out |> replace("DROP TABLE \"", "DROP TABLE {q}\"")
    out = out |> replace("DROP VIEW IF EXISTS \"", "DROP VIEW IF EXISTS {q}\"")
    out = out |> replace("DROP VIEW \"", "DROP VIEW {q}\"")
    out = out |> replace("DROP INDEX IF EXISTS \"", "DROP INDEX IF EXISTS {q}\"")
    out = out |> replace("ALTER TABLE \"", "ALTER TABLE {q}\"")
    return out
}

def private _db_prepare(db : SqlRunner; sql : string; var stmt : sqlite3_stmt?&) : int {
    return sqlite3_prepare_v2(db.sqlite_handle, _qualify_schema(sql, db.schema_name), stmt)
}

// ===== High-level API: runner, RAII, exec, last_insert_rowid =====

struct SqlRunner {
    //! Owns a libsqlite3 connection. Closed by `finalize` on `var inscope` scope exit.
    //! `schema_name` (default empty) prefixes emitted `FROM`/`JOIN`/etc. references
    //! with `"<schema>".`; set via `with_schema` to address ATTACHed databases.
    sqlite_handle : sqlite3? = null
    schema_name : string
}

typedef SqlError = Option<string>

def finalize(var db : SqlRunner) {
    //! Closes the underlying SQLite connection. Triggered automatically by `var inscope db`.
    if (db.sqlite_handle != null) {
        sqlite3_close_v2(db.sqlite_handle)
        db.sqlite_handle = null
    }
}

def sqlite_version() : string {
    //! Returns the linked libsqlite3 library version (e.g. "3.45.0"). No DB connection needed.
    return string(sqlite3_libversion())
}

def try_open_sqlite(path : string) : Result<SqlRunner, string> {
    //! Opens a connection to ``path`` (``:memory:`` for an in-memory DB). Returns
    //! ``err(...)`` on failure (handle closed); on success, registers every in-scope
    //! ``[sql_function]`` against the connection.
    var db = SqlRunner()
    let rc = sqlite3_open(path, db.sqlite_handle)
    if (rc != SQLITE_OK) {
        // SQLite leaves *ppDb non-null on most errors, but writes NULL on OOM allocation
        // failure — sqlite3_errmsg(NULL) is undefined; sqlite3_close_v2(NULL) is a no-op.
        let msg = db.sqlite_handle == null ? "sqlite3_open(\"{path}\") failed: out of memory" : "sqlite3_open(\"{path}\") failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_close_v2(db.sqlite_handle)
        db.sqlite_handle = null
        return err(msg, type<SqlRunner>)
    }
    let install_err = _install_sql_function_registry(db)
    if (install_err |> is_some) {
        let msg = install_err |> unwrap
        sqlite3_close_v2(db.sqlite_handle)
        db.sqlite_handle = null
        return err(msg, type<SqlRunner>)
    }
    return move_ok(db, type<string>)
}

def open_sqlite(path : string) : SqlRunner {
    //! Strict variant of ``try_open_sqlite``. Panics with the libsqlite3 errmsg on failure.
    var r = try_open_sqlite(path)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def with_sqlite(path : string; blk : block<(db : SqlRunner) : void>) : void {
    //! RAII helper. Opens the DB, enables ``PRAGMA foreign_keys=ON`` (off by default in SQLite),
    //! invokes ``blk``, and finalizes on exit (including panic / early return).
    var inscope db = open_sqlite(path)
    db |> exec("PRAGMA foreign_keys = ON")
    invoke(blk, db)
}

def try_exec(db : SqlRunner; sql : string) : SqlError {
    //! Executes ``sql`` (one or more statements, no result rows).
    //! Returns ``none`` on success, ``some(errmsg)`` on failure.
    var errmsg : string
    let rc = sqlite3_exec(db.sqlite_handle, _qualify_schema(sql, db.schema_name), unsafe(addr(errmsg)))
    if (rc != SQLITE_OK) {
        let msg = "exec failed: {errmsg}"
        sqlite3_free(errmsg)
        return some(msg)
    }
    return none(type<string>)
}

def private try_exec_with_binds(db : SqlRunner; sql : string;
                                bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : SqlError {
    var stmt : sqlite3_stmt?
    let prc = _db_prepare(db, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "exec failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return some(msg)
    }
    invoke(bind_blk, stmt)
    let src = sqlite3_step(stmt)
    if (src != SQLITE_DONE && src != SQLITE_ROW) {
        let msg = "exec failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return some(msg)
    }
    sqlite3_finalize(stmt)
    return none(type<string>)
}

def try_exec(db : SqlRunner; sql : string; arg0 : auto(A0)) : SqlError {
    //! 1-arg positional bind. ``sql`` must contain exactly one ``?``.
    return db |> try_exec_with_binds(sql, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
    })
}

def try_exec(db : SqlRunner; sql : string; arg0 : auto(A0); arg1 : auto(A1)) : SqlError {
    //! 2-arg positional bind.
    return db |> try_exec_with_binds(sql, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
    })
}

def try_exec(db : SqlRunner; sql : string; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : SqlError {
    //! 3-arg positional bind.
    return db |> try_exec_with_binds(sql, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
        sql_bind_to_stmt(stmt, 3, arg2)
    })
}

def exec(db : SqlRunner; sql : string; arg0 : auto(A0)) : void {
    //! Strict variant of 1-arg ``try_exec``.
    let err = db |> try_exec(sql, arg0)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def exec(db : SqlRunner; sql : string; arg0 : auto(A0); arg1 : auto(A1)) : void {
    //! Strict variant of 2-arg ``try_exec``.
    let err = db |> try_exec(sql, arg0, arg1)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def exec(db : SqlRunner; sql : string; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : void {
    //! Strict variant of 3-arg ``try_exec``.
    let err = db |> try_exec(sql, arg0, arg1, arg2)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def exec(db : SqlRunner; sql : string) : void {
    //! Strict variant of ``try_exec``. Panics with the libsqlite3 errmsg on failure.
    let err = db |> try_exec(sql)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def last_insert_rowid(db : SqlRunner) : int64 {
    //! Returns the rowid of the most recent successful INSERT on this connection.
    return sqlite3_last_insert_rowid(db.sqlite_handle)
}

def changes(db : SqlRunner) : int {
    //! Returns rows affected by the most recent INSERT/UPDATE/DELETE (wraps ``sqlite3_changes``).
    return sqlite3_changes(db.sqlite_handle)
}

// ===== ATTACH DATABASE — tut 36: cross-DB queries on one connection =====

def try_attach(db : SqlRunner; path, as_name : string) : SqlError {
    //! Runs `ATTACH DATABASE ? AS <as_name>`. `as_name` becomes the schema qualifier
    //! consumed by `with_schema`; reserved names (`main`, `temp`) → `some(errmsg)`.
    return db |> try_exec("ATTACH DATABASE ? AS {sql_quote_id(as_name)}", path)
}

def try_detach(db : SqlRunner; as_name : string) : SqlError {
    //! Runs `DETACH DATABASE <as_name>`.
    return db |> try_exec("DETACH DATABASE {sql_quote_id(as_name)}")
}

def attach(db : SqlRunner; path, as_name : string) : void {
    //! Strict variant of `try_attach` — panics on failure.
    let err = db |> try_attach(path, as_name)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def detach(db : SqlRunner; as_name : string) : void {
    //! Strict variant of `try_detach` — panics on failure.
    let err = db |> try_detach(as_name)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def private _schema_runner(db : SqlRunner; schema_name : string) : SqlRunner {
    // Aliases the same libsqlite3 handle as `db`; non-owning. Caller MUST NOT inscope.
    return SqlRunner(sqlite_handle = unsafe(reinterpret<sqlite3?>(db.sqlite_handle)),
                     schema_name = schema_name)
}

def with_schema(db : SqlRunner; schema_name : string;
                body : block<(scoped : SqlRunner) : void>) : void {
    //! Invokes `body` with a runner that emits `"<schema>"."<table>"` on every query.
    //! Block-scoped to keep the schema-qualified runner non-inscope-able (it shares
    //! `db`'s libsqlite3 handle; only the original owns lifetime).
    invoke(body, _schema_runner(db, schema_name))
}

def with_attached(db : SqlRunner; path, as_name : string;
                  body : block<(scoped : SqlRunner) : void>) : void {
    //! Attaches `path`, invokes `body` with a schema-qualified runner,
    //! and detaches in `finally` (covers normal + panic paths).
    db |> attach(path, as_name)
    {
        invoke(body, _schema_runner(db, as_name))
    } finally {
        db |> detach(as_name)
    }
}

// ===== LIKE-pattern escape helpers (tut 40 — string predicates) =====

def like_escape_prefix(s : string) : string {
    //! Returns `s` with `\`, `%`, `_` doubled via `\`, then `%` appended.
    //! Pair with `col LIKE ? ESCAPE '\'`.
    return (s |> replace("\\", "\\\\") |> replace("%", "\\%") |> replace("_", "\\_")) + "%"
}

def like_escape_suffix(s : string) : string {
    //! Returns `s` with `\`, `%`, `_` doubled via `\`, prepended with `%`.
    //! Pair with `col LIKE ? ESCAPE '\'`.
    return "%" + (s |> replace("\\", "\\\\") |> replace("%", "\\%") |> replace("_", "\\_"))
}

def like_escape_substring(s : string) : string {
    //! Returns `s` with `\`, `%`, `_` doubled via `\`, wrapped in `%…%`.
    //! Pair with `col LIKE ? ESCAPE '\'`.
    return "%" + (s |> replace("\\", "\\\\") |> replace("%", "\\%") |> replace("_", "\\_")) + "%"
}

// ===== SQL adapter rail: sql_bind / sql_extract  (tut 26) =====

// --- four primitive identity overloads ---

def sql_bind(v : int64) : int64 {
    return v
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<int64>) : int64 {
    return v
}

def sql_bind(v : double) : double {
    return v
}

[unused_argument(t)]
def sql_extract(v : double; t : type<double>) : double {
    return v
}

def sql_bind(s : string) : string {
    return s
}

[unused_argument(t)]
def sql_extract(s : string; t : type<string>) : string {
    return s
}

def sql_bind(b : array<uint8>) : array<uint8> {
    //! Identity passthrough for BLOB columns. For multi-MB payloads prefer the streaming
    //! ``sqlite3_blob_open`` API to avoid the implicit clone + ``SQLITE_TRANSIENT`` copy.
    return <- clone_to_move(b)
}

[unused_argument(t)]
def sql_extract(b : array<uint8>; t : type<array<uint8>>) : array<uint8> {
    return <- clone_to_move(b)
}

// --- stdlib widenings (all integer kinds widen to int64; float widens to double) ---

def sql_bind(v : int) : int64 {
    return int64(v)
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<int>) : int {
    return int(v)
}

def sql_bind(v : int8) : int64 {
    return int64(v)
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<int8>) : int8 {
    return int8(v)
}

def sql_bind(v : int16) : int64 {
    return int64(v)
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<int16>) : int16 {
    return int16(v)
}

def sql_bind(v : uint) : int64 {
    return int64(v)
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<uint>) : uint {
    return uint(v)
}

def sql_bind(v : uint8) : int64 {
    return int64(v)
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<uint8>) : uint8 {
    return uint8(v)
}

def sql_bind(v : uint16) : int64 {
    return int64(v)
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<uint16>) : uint16 {
    return uint16(v)
}

def sql_bind(v : uint64) : int64 {
    //! Reinterpret cast — values above ``int64.max`` round-trip through
    //! the sign bit (SQLite has no UNSIGNED type). For workloads that
    //! actually exceed that range, store as ``string`` via a custom adapter.
    return int64(v)
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<uint64>) : uint64 {
    return uint64(v)
}

def sql_bind(v : float) : double {
    return double(v)
}

[unused_argument(t)]
def sql_extract(v : double; t : type<float>) : float {
    return float(v)
}

def sql_bind(v : bool) : int64 {
    return v ? 1l : 0l
}

[unused_argument(t)]
def sql_extract(v : int64; t : type<bool>) : bool {
    return v != 0l
}

// --- enum generic ---

def sql_bind(value : auto(TT)) : int64 {
    static_if (typeinfo is_enum(value)) {
        return int64(value)
    } else {
        concept_assert(false, "sql_bind: unsupported field type. Define `def sql_bind(v : YourType) : int64 | double | string | array<uint8>` (and a matching sql_extract) in YourType's module.")
        return 0l
    }
}

// Option<T> fallback — `_::sql_bind` so caller-module adapters (incl. @sql_json/@sql_blob's `sql_bind(T)`) resolve.
// Without it, `sql_bind(default<Option<int>>)` falls into the auto(TT) catch-all and trips its concept_assert.
[unused_argument(value)]
def sql_bind(value : $Option < auto(TT) >) {
    return _::sql_bind(default<TT>)
}

[unused_argument(t)]
def sql_extract(value : int64; t : type<auto(TT)>) : TT {
    static_if (typeinfo is_enum(default<TT>)) {
        // int64 → enum via reinterpret. Mirrors typemacro_boost's
        // ``int64_to_enum``; inlined here to avoid pulling that module
        // into every dasSQLITE consumer.
        static_if (typeinfo sizeof(type<TT>) == 1) {
            return unsafe(reinterpret<TT>(int8(value)))
        } static_elif (typeinfo sizeof(type<TT>) == 2) {
            return unsafe(reinterpret<TT>(int16(value)))
        } else {
            return unsafe(reinterpret<TT>(int(value)))
        }
    } else {
        concept_assert(false, "sql_extract: unsupported field type. Define `def sql_extract(v : int64 | double | string | array<uint8>; t : type<YourType>) : YourType` (and a matching sql_bind) in YourType's module.")
        return default<TT>
    }
}

// ===== Internal bridge: sql_bind/sql_extract ↔ libsqlite3 C API =====

def sqlite3_column_blob_(stmt : sqlite3_stmt?; col : int) : array<uint8> {
    //! Reads a BLOB column as a fresh ``array<uint8>`` — copy is mandatory because
    //! SQLite's blob pointer dies on the next ``step`` / ``reset`` / ``finalize``.
    let n = sqlite3_column_bytes(stmt, col)
    var r : array<uint8>
    r |> resize(n)
    if (n > 0) {
        unsafe {
            memcpy(addr(r[0]), reinterpret<void?>(sqlite3_column_blob(stmt, col)), n)
        }
    }
    return <- r
}

// --- bind direction ---

def sql_bind_to_stmt(var stmt : sqlite3_stmt?; idx : int; v : int64) : void {
    sqlite3_bind_int64(stmt, idx, v)
}

def sql_bind_to_stmt(var stmt : sqlite3_stmt?; idx : int; v : double) : void {
    sqlite3_bind_double(stmt, idx, v)
}

def sql_bind_to_stmt(var stmt : sqlite3_stmt?; idx : int; v : string) : void {
    sqlite3_bind_text(stmt, idx, v)
}

def sql_bind_to_stmt(var stmt : sqlite3_stmt?; idx : int; v : array<uint8>) : void {
    sqlite3_bind_blob(stmt, idx, v)
}

def sql_bind_to_stmt(var stmt : sqlite3_stmt?; idx : int; v : $Option < auto(TT) >) : void {
    //! ``some(x)`` recurses on ``x`` (which routes through the user's
    //! ``_::sql_bind`` adapter for non-primitive ``T``); ``none`` binds NULL.
    if (v._has_value) {
        sql_bind_to_stmt(stmt, idx, v._value)
    } else {
        sqlite3_bind_null(stmt, idx)
    }
}

def sql_bind_to_stmt(var stmt : sqlite3_stmt?; idx : int; v : auto(TT)) : void {
    //! Catch-all: route through the user's ``_::sql_bind`` adapter, then
    //! recurse on the primitive return value. ``_::``-dispatched so the
    //! caller's-module overloads (the user's adapter pair) are visible.
    sql_bind_to_stmt(stmt, idx, _::sql_bind(v))
}

// --- read direction ---

[unused_argument(witness)]
def sql_read_witness(stmt : sqlite3_stmt?; col : int; witness : int64) : int64 {
    return sqlite3_column_int64(stmt, col)
}

[unused_argument(witness)]
def sql_read_witness(stmt : sqlite3_stmt?; col : int; witness : double) : double {
    return sqlite3_column_double(stmt, col)
}

[unused_argument(witness)]
def sql_read_witness(stmt : sqlite3_stmt?; col : int; witness : string) : string {
    return sqlite3_column_text_(stmt, col)
}

[unused_argument(witness)]
def sql_read_witness(stmt : sqlite3_stmt?; col : int; witness : array<uint8>) : array<uint8> {
    return <- sqlite3_column_blob_(stmt, col)
}

[unused_argument(stmt, col, witness)]
def sql_read_witness(stmt : sqlite3_stmt?; col : int; witness : auto(SS)) : SS {
    concept_assert(false, "sql_bind must return one of int64 / double / string / array<uint8>")
    return default<SS>
}

[unused_argument(t)]
def read_via_adapter(stmt : sqlite3_stmt?; col : int; t : type<$Option<auto(TT)>>) : Option<TT -const -#> {
    //! ``Option<T>`` overload: SQLITE_NULL → ``none``, otherwise read the
    //! inner ``T`` via the generic ``read_via_adapter`` and wrap in ``some``.
    if (sqlite3_column_type(stmt, col) == SQLITE_NULL) return none(type<TT -const -#>)
    return some(read_via_adapter(stmt, col, type<TT -const -#>))
}

[unused_argument(t)]
def read_via_adapter(stmt : sqlite3_stmt?; col : int; t : type<auto(TT)>) : TT {
    //! Reads column as ``_::sql_bind(default<TT>)``'s storage primitive, then materializes ``TT``
    //! via ``_::sql_extract``. Primitive type is resolved at compile time — no runtime adapter call.
    return _::sql_extract(
        sql_read_witness(stmt, col, default<typedecl(_::sql_bind(default<TT>))>),
        type<TT>
    )
}

// --- DDL: column-type string from sql_bind's return type ---

[unused_argument(witness)]
def sql_storage_type_witness(witness : int64) : string {
    return "INTEGER"
}

[unused_argument(witness)]
def sql_storage_type_witness(witness : double) : string {
    return "REAL"
}

[unused_argument(witness)]
def sql_storage_type_witness(witness : string) : string {
    return "TEXT"
}

[unused_argument(witness)]
def sql_storage_type_witness(witness : array<uint8>) : string {
    return "BLOB"
}

[unused_argument(witness)]
def sql_storage_type_witness(witness : auto(SS)) : string {
    concept_assert(false, "sql_bind must return one of int64 / double / string / array<uint8>")
    return ""
}

[unused_argument(t)]
def sql_storage_type_for(t : type<auto(TT)>) : string {
    return sql_storage_type_witness(default<typedecl(_::sql_bind(default<TT>))>)
}

[unused_argument(t)]
def sql_storage_type_for(t : type<$Option<auto(TT)>>) : string {
    //! Option<T> nullability is decided by the [sql_table] DDL emitter; the
    //! column type comes from the inner ``T``'s storage primitive.
    return sql_storage_type_for(type<TT>)
}

// --- column_info: same compile-time walk, abstract-typed view ---

[unused_argument(witness)]
def sql_storage_enum_witness(witness : int64) : SqlType {
    return SqlType.Integer
}

[unused_argument(witness)]
def sql_storage_enum_witness(witness : double) : SqlType {
    return SqlType.Real
}

[unused_argument(witness)]
def sql_storage_enum_witness(witness : string) : SqlType {
    return SqlType.Text
}

[unused_argument(witness)]
def sql_storage_enum_witness(witness : array<uint8>) : SqlType {
    return SqlType.Blob
}

[unused_argument(witness)]
def sql_storage_enum_witness(witness : auto(SS)) : SqlType {
    concept_assert(false, "sql_bind must return one of int64 / double / string / array<uint8>")
    return SqlType.Null
}

[unused_argument(t)]
def sql_storage_enum_for(t : type<auto(TT)>) : SqlType {
    return sql_storage_enum_witness(default<typedecl(_::sql_bind(default<TT>))>)
}

[unused_argument(t)]
def sql_storage_enum_for(t : type<$Option<auto(TT)>>) : SqlType {
    return sql_storage_enum_for(type<TT>)
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

[unused_argument(t)]
def try_query_scalar(db : SqlRunner; sql : string; t : type<auto(TT)>) : Result<TT -const -#, string> {
    //! Prepares ``sql``, steps once, returns column 0 as ``TT`` via the adapter rail (tut 26).
    //! ``Err(errmsg)`` on prepare/step error or zero rows.
    return <- try_one_row(db, sql, $(var stmt : sqlite3_stmt?) {}, $(stmt : sqlite3_stmt?) {
        static_if (typeinfo can_copy(default<TT -const -#>)) {
            return read_via_adapter(stmt, 0, type<TT -const -#>)
        } else {
            return <- clone_to_move(read_via_adapter(stmt, 0, type<TT -const -#>))
        }
    }, "query_scalar")
}

[unused_argument(t)]
def query_scalar(db : SqlRunner; sql : string; t : type<auto(TT)>) : TT -const -# {
    //! Strict variant of ``try_query_scalar``. Panics on prepare/step error or zero rows.
    let r = db |> try_query_scalar(sql, type<TT -const -#>)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

[unused_argument(t)]
def query_scalar_opt(db : SqlRunner; sql : string; t : type<auto(TT)>) : Option<TT -const -#> {
    //! ``Option`` variant of ``query_scalar`` — ``some(val)`` / ``none`` on zero rows.
    //! Panics only on prepare/step failure.
    var r <- try_one_row_opt(db, sql, $(var stmt : sqlite3_stmt?) {}, $(stmt : sqlite3_stmt?) {
        static_if (typeinfo can_copy(default<TT -const -#>)) {
            return read_via_adapter(stmt, 0, type<TT -const -#>)
        } else {
            return <- clone_to_move(read_via_adapter(stmt, 0, type<TT -const -#>))
        }
    }, "query_scalar_opt")
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

// ===== query_one — full-row read with positional bind args =====

def private try_with_stmt(db : SqlRunner; sql : string; err_prefix : string;
                          bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                          body_blk : block<(stmt : sqlite3_stmt?) : $Result<auto(R); string> >) : Result<R, string> {
    var stmt : sqlite3_stmt?
    let prc = _db_prepare(db, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "{err_prefix}: prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<R>)
    }
    invoke(bind_blk, stmt)
    var r <- invoke(body_blk, stmt)
    sqlite3_finalize(stmt)
    return <- r
}

def private try_one_row(db : SqlRunner; sql : string;
                        bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                        row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>;
                        err_prefix : string) : Result<TT -const -#, string> {
    return <- try_with_stmt(db, sql, err_prefix, bind_blk,
        $(stmt : sqlite3_stmt?) : Result<TT -const -#, string> {
            let src = sqlite3_step(stmt)
            if (src == SQLITE_DONE) return err("{err_prefix}: expected one row, got 0 rows", type<TT -const -#>)
            if (src != SQLITE_ROW) return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<TT -const -#>)
            static_if (typeinfo can_copy(default<TT -const -#>)) {
                var row : TT -const -# = invoke(row_blk, stmt) // nolint:LINT003
                return move_ok(row, type<string>)
            } else {
                var row : TT -const -# <- clone_to_move(invoke(row_blk, stmt))
                return move_ok(row, type<string>)
            }
        })
}

def private try_one_row_opt(db : SqlRunner; sql : string;
                            bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                            row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>;
                            err_prefix : string) : Result<Option<TT -const -#>, string> {
    return <- try_with_stmt(db, sql, err_prefix, bind_blk,
        $(stmt : sqlite3_stmt?) : Result<Option<TT -const -#>, string> {
            let src = sqlite3_step(stmt)
            if (src == SQLITE_DONE) return ok(none(type<TT -const -#>), type<string>)
            if (src != SQLITE_ROW) return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<Option<TT -const -#>>)
            static_if (typeinfo can_copy(default<TT -const -#>)) {
                var row : TT -const -# = invoke(row_blk, stmt) // nolint:LINT003
                return move_ok(move_some(row), type<string>)
            } else {
                var row : TT -const -# <- clone_to_move(invoke(row_blk, stmt))
                return move_ok(move_some(row), type<string>)
            }
        })
}

def private try_step_dml(db : SqlRunner; sql : string;
                         bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                         err_prefix : string) : Result<int, string> {
    return <- try_with_stmt(db, sql, err_prefix, bind_blk,
        $(stmt : sqlite3_stmt?) : Result<int, string> {
            let src = sqlite3_step(stmt)
            if (src != SQLITE_DONE) return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<int>)
            return ok(sqlite3_changes(db.sqlite_handle), type<string>)
        })
}

def private try_collect_rows(db : SqlRunner; sql : string;
                             bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                             row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>;
                             err_prefix : string) : Result<array<TT -const -#>, string> {
    return <- try_with_stmt(db, sql, err_prefix, bind_blk,
        $(stmt : sqlite3_stmt?) : Result<array<TT -const -#>, string> {
            var inscope result : array<TT -const -#>
            var rc = sqlite3_step(stmt)
            while (rc == SQLITE_ROW) {
                // Copyable rows ``push`` directly; non-copyable rows (e.g.
                // structs containing ``array<uint8>``) ``emplace`` from the
                // invoke rvalue so we don't deep-clone on every row.
                static_if (typeinfo can_copy(default<TT -const -#>)) {
                    result |> push(invoke(row_blk, stmt))
                } else {
                    result |> emplace(invoke(row_blk, stmt))
                }
                rc = sqlite3_step(stmt)
            }
            if (rc != SQLITE_DONE) return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<array<TT -const -#>>)
            return move_ok(result, type<string>)
        })
}

[unused_argument(t)]
def private try_query_one_impl(db : SqlRunner; sql : string; t : type<auto(TT)>;
                               bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : Result<TT -const -#, string> {
    return <- try_one_row(db, sql, bind_blk,
                          $(stmt : sqlite3_stmt?) {
                              static_if (typeinfo can_copy(default<TT -const -#>)) {
                                  return _::_sql_read_row(stmt, type<TT -const -#>)
                              } else {
                                  return <- clone_to_move(_::_sql_read_row(stmt, type<TT -const -#>))
                              }
                          },
                          "query_one")
}

def private query_one_impl(db : SqlRunner; sql : string; t : type<auto(TT)>;
                           bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : TT -const -# {
    var r <- try_query_one_impl(db, sql, type<TT -const -#>, bind_blk)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def private query_one_opt_impl(db : SqlRunner; sql : string; t : type<auto(TT)>;
                               bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : Option<TT -const -#> {
    var r <- try_one_row_opt(db, sql, bind_blk,
        $(stmt : sqlite3_stmt?) {
            static_if (typeinfo can_copy(default<TT -const -#>)) {
                return _::_sql_read_row(stmt, type<TT -const -#>)
            } else {
                return <- clone_to_move(_::_sql_read_row(stmt, type<TT -const -#>))
            }
        }, "query_one_opt")
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

// ---- try_query_one — Result<TT, string>; 0..3 positional args ----

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>) : Result<TT -const -#, string> {
    //! Zero-arg form. Errors on prepare/step failure or zero rows.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {})
}

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0)) : Result<TT -const -#, string> {
    //! 1-arg positional bind.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
    })
}

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : Result<TT -const -#, string> {
    //! 2-arg positional bind.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
    })
}

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : Result<TT -const -#, string> {
    //! 3-arg positional bind.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
        sql_bind_to_stmt(stmt, 3, arg2)
    })
}

// ---- query_one (strict) — TT; 0..3 positional args; panics on err ----

[unused_argument(t)]
def query_one(db : SqlRunner; sql : string; t : type<auto(TT)>) : TT -const -# {
    return <- query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {})
}

[unused_argument(t)]
def query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0)) : TT -const -# {
    return <- query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
    })
}

[unused_argument(t)]
def query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : TT -const -# {
    return <- query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
    })
}

[unused_argument(t)]
def query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : TT -const -# {
    return <- query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
        sql_bind_to_stmt(stmt, 3, arg2)
    })
}

// ---- query_one_opt — Option<TT>; 0..3 positional args; panics only on prepare/step error ----

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {})
}

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0)) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
    })
}

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
    })
}

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
        sql_bind_to_stmt(stmt, 3, arg2)
    })
}

// ===== query / try_query — typed multi-row raw-SQL read =====

[unused_argument(t)]
def private try_query_impl(db : SqlRunner; sql : string; t : type<auto(TT)>;
                           bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : Result<array<TT -const -#>, string> {
    return <- try_collect_rows(db, sql, bind_blk,
                               $(stmt : sqlite3_stmt?) {
                                   static_if (typeinfo can_copy(default<TT -const -#>)) {
                                       return _::_sql_read_row(stmt, type<TT -const -#>)
                                   } else {
                                       return <- clone_to_move(_::_sql_read_row(stmt, type<TT -const -#>))
                                   }
                               },
                               "query")
}

def private query_impl(db : SqlRunner; sql : string; t : type<auto(TT)>;
                       bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : array<TT -const -#> {
    var r <- try_query_impl(db, sql, type<TT -const -#>, bind_blk)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

// ---- try_query — Result<array<TT>, string>; 0..3 positional args ----

def try_query(db : SqlRunner; sql : string; t : type<auto(TT)>) : Result<array<TT -const -#>, string> {
    //! Zero-arg form. Errors only on prepare/step failure; an empty result set
    //! is `Ok([])`.
    return <- try_query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {})
}

def try_query(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0)) : Result<array<TT -const -#>, string> {
    //! 1-arg positional bind.
    return <- try_query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
    })
}

def try_query(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : Result<array<TT -const -#>, string> {
    //! 2-arg positional bind.
    return <- try_query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
    })
}

def try_query(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : Result<array<TT -const -#>, string> {
    //! 3-arg positional bind.
    return <- try_query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
        sql_bind_to_stmt(stmt, 3, arg2)
    })
}

// ---- query (strict) — array<TT>; 0..3 positional args; panics on err ----

[unused_argument(t)]
def query(db : SqlRunner; sql : string; t : type<auto(TT)>) : array<TT -const -#> {
    return <- query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {})
}

[unused_argument(t)]
def query(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0)) : array<TT -const -#> {
    return <- query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
    })
}

[unused_argument(t)]
def query(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : array<TT -const -#> {
    return <- query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
    })
}

[unused_argument(t)]
def query(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : array<TT -const -#> {
    return <- query_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, arg0)
        sql_bind_to_stmt(stmt, 2, arg1)
        sql_bind_to_stmt(stmt, 3, arg2)
    })
}

// ===== User-facing CRUD over [sql_table] structs =====

[template(t)]
def try_create_table(db : SqlRunner; t : auto(TT)) : SqlError {
    //! Emits CREATE TABLE (and any ``[sql_index(...)]`` CREATE INDEX statements
    //! attached to the struct) for the type ``T`` based on its `[sql_table]`
    //! definition. Returns ``none`` on success, ``some(errmsg)`` on failure.
    let table_sql = _::_sql_create_table_sql(type<TT>)
    let err = db |> try_exec(table_sql)
    if (err |> is_some) return err
    let indexes_sql = _::_sql_create_indexes_sql(type<TT>)
    if (empty(indexes_sql)) return none(type<string>)
    return db |> try_exec(indexes_sql)
}

[template(t)]
def create_table(db : SqlRunner; t : auto(TT)) : void {
    //! Strict variant of ``try_create_table``. Panics with the libsqlite3 errmsg on failure.
    let err = db |> try_create_table(type<TT>)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

[unused_argument(t), template(t)]
def try_create_table(db : SqlRunner; t : type<auto(TT)>; name : string) : SqlError {
    //! Like ``try_create_table(type<T>)`` but emits the DDL with table identifier ``name`` instead of T's
    //! own ``[sql_table(name=...)]``. Use during a struct_convert rebuild to land the new schema in
    //! ``users_new`` before the swap. Indexes auto-derive against the runtime name.
    let err = db |> try_exec(_::_sql_create_table_sql(default<TT>, name))
    if (err |> is_some) return err
    let idx = _::_sql_create_indexes_sql(default<TT>, name)
    if (empty(idx)) return none(type<string>)
    return db |> try_exec(idx)
}

[unused_argument(t), template(t)]
def create_table(db : SqlRunner; t : type<auto(TT)>; name : string) : void {
    //! Strict variant of ``try_create_table(type<T>, name)``. Panics with the libsqlite3 errmsg on failure.
    let err = db |> try_create_table(type<TT>, name)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

[template(t)]
def try_drop_table_if_exists(db : SqlRunner; t : auto(TT)) : SqlError {
    //! Emits DROP TABLE IF EXISTS for the type ``T``.
    //! Returns ``none`` on success, ``some(errmsg)`` on failure.
    return db |> try_exec(_::_sql_drop_table_if_exists_sql(type<TT>))
}

[template(t)]
def drop_table_if_exists(db : SqlRunner; t : auto(TT)) : void {
    //! Strict variant of ``try_drop_table_if_exists``. Panics with the libsqlite3 errmsg on failure.
    let err = db |> try_drop_table_if_exists(type<TT>)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

[template(t)]
def try_drop_view_if_exists(db : SqlRunner; t : auto(TT)) : SqlError {
    //! Emits DROP VIEW IF EXISTS for a `[sql_view]`-tagged type ``T``.
    //! Returns ``none`` on success, ``some(errmsg)`` on failure.
    return db |> try_exec(_::_sql_drop_view_if_exists_sql(type<TT>))
}

[template(t)]
def drop_view_if_exists(db : SqlRunner; t : auto(TT)) : void {
    //! Strict variant of ``try_drop_view_if_exists``. Panics with the libsqlite3 errmsg on failure.
    let err = db |> try_drop_view_if_exists(type<TT>)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

[unused_argument(t), template(t)]
def column_info(t : type<auto(TT)>) : array<ColumnInfo> {
    //! Returns the column metadata for a `[sql_table]`-tagged struct ``T``.
    //! Render dialect-specific column types via `sqlite_sql_type(c.data_type)`.
    return <- _::_sql_column_info(default<TT>)
}

[unused_argument(t), template(t)]
def try_check_schema(db : SqlRunner; t : type<auto(TT)>) : SqlError {
    //! Verifies the open ``db`` matches struct ``T`` on (name, SqlType, NOT NULL, PRIMARY KEY).
    //! Returns ``some(errmsg)`` on first mismatch — startup-defense pattern for non-self-created DBs.
    let want = _::_sql_column_info(default<TT>)
    let table_name = _::_sql_table_name(default<TT>)
    var got_result <- read_pragma_table_info(db, table_name)
    if (got_result |> is_err) return some(got_result |> unwrap_err)
    let got <- got_result._value
    if (empty(got)) return some("check_schema: table \"{table_name}\" not found in DB.")
    if (length(want) != length(got)) return some("check_schema: \"{table_name}\" column count mismatch — struct has {length(want)} field(s), DB has {length(got)} column(s).")
    // Name-keyed: [sql_table] emits SELECTs with explicit columns, so struct/DB order can differ.
    // With equal counts, missing-from-struct is implied by no name match in the want-loop.
    var by_name : table<string; int>
    for (j in range(length(got))) {
        by_name |> insert(got[j].name, j)
    }
    for (i in range(length(want))) {
        let w = want[i]
        let g_idx = by_name?[w.name] ?? -1
        if (g_idx < 0) return some("check_schema: \"{table_name}\" struct field \"{w.name}\" not found in DB columns.")
        let g = got[g_idx]
        let g_type = affinity_to_sql_type(g.decl_type)
        if (w.data_type != g_type) return some("check_schema: \"{table_name}\".\"{w.name}\" SqlType mismatch — struct expects {sqlite_sql_type(w.data_type)}, DB has {sqlite_sql_type(g_type)} (declared \"{g.decl_type}\").")
        // SQLite quirk: INTEGER PRIMARY KEY reports notnull=0 in pragma_table_info — treat PK as NOT NULL.
        let g_nullable = !g.not_null && !g.pk
        if (w.is_nullable != g_nullable) {
            if (w.is_nullable) {
                return some("check_schema: \"{table_name}\".\"{w.name}\" nullability mismatch — struct allows NULL (Option<T>), DB column is NOT NULL.")
            } else {
                return some("check_schema: \"{table_name}\".\"{w.name}\" nullability mismatch — struct is non-optional, DB column allows NULL.")
            }
        }
        if (w.is_pk != g.pk) {
            if (w.is_pk) {
                return some("check_schema: \"{table_name}\".\"{w.name}\" PK mismatch — struct has @sql_primary_key, DB column is not PRIMARY KEY.")
            } else {
                return some("check_schema: \"{table_name}\".\"{w.name}\" PK mismatch — DB column is PRIMARY KEY but struct field is missing @sql_primary_key.")
            }
        }
    }
    return none(type<string>)
}

[unused_argument(t), template(t)]
def check_schema(db : SqlRunner; t : type<auto(TT)>) {
    //! Strict variant of ``try_check_schema``. Panics with the diagnostic message on mismatch.
    //! Use at app startup to fail loud if the DB you opened doesn't match what your code expects.
    let r = db |> try_check_schema(type<TT>)
    if (r |> is_some) {
        panic(r |> unwrap)
    }
}

def try_insert(db : SqlRunner; row : auto(TT)) : Result<int64, string> {
    //! Inserts a single row. Returns ``Ok(rowid)`` on success, ``Err(errmsg)`` on failure.
    //! When the row's integer primary key is zero, the macro-emitted no-PK INSERT runs and SQLite assigns the id.
    let pk_unset = _::_sql_pk_is_unset(row)
    let sql = pk_unset ? _::_sql_insert_no_pk_sql(row) : _::_sql_insert_with_pk_sql(row)
    let r = db |> try_step_dml(sql, $(var stmt : sqlite3_stmt?) {
        _::_sql_bind_row(stmt, row, !pk_unset)
    }, "insert")
    if (r |> is_err) return err(r |> unwrap_err, type<int64>)
    return ok(sqlite3_last_insert_rowid(db.sqlite_handle), type<string>)
}

def insert(db : SqlRunner; row : auto(TT)) : int64 {
    //! Strict variant of ``try_insert(row)``. Panics with the libsqlite3 errmsg on failure.
    let r = db |> try_insert(row)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_insert(db : SqlRunner; row : auto(TT); name : string) : Result<int64, string> {
    //! Inserts a single row into table identifier ``name`` instead of T's own ``[sql_table(name=...)]``.
    //! Used during a struct_convert rebuild to write into the staging table before the rename swap.
    //! Same PK semantics as ``try_insert(row)`` — zero PK triggers the no-PK INSERT for autoincrement.
    let pk_unset = _::_sql_pk_is_unset(row)
    let sql = (pk_unset
        ? _::_sql_insert_no_pk_sql(default<TT>, name)
        : _::_sql_insert_with_pk_sql(default<TT>, name))
    let r = db |> try_step_dml(sql, $(var stmt : sqlite3_stmt?) {
        _::_sql_bind_row(stmt, row, !pk_unset)
    }, "insert")
    if (r |> is_err) return err(r |> unwrap_err, type<int64>)
    return ok(sqlite3_last_insert_rowid(db.sqlite_handle), type<string>)
}

def insert(db : SqlRunner; row : auto(TT); name : string) : int64 {
    //! Strict variant of ``try_insert(row, name)``. Panics with the libsqlite3 errmsg on failure.
    let r = db |> try_insert(row, name)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_insert(db : SqlRunner; rows : array<auto(TT)>) : Result<int64, string> {
    //! Batched INSERT inside a single BEGIN IMMEDIATE / COMMIT transaction.
    //! All rows must agree on PK presence — mixed-PK arrays return ``Err`` without opening a txn.
    //! ``Ok(last_rowid)`` on success, ``Err(errmsg)`` on failure (after ROLLBACK if applicable).
    if (empty(rows)) return ok(int64(0), type<string>)
    let pk_unset = _::_sql_pk_is_unset(rows[0])
    for (i in 1..length(rows)) {
        if (_::_sql_pk_is_unset(rows[i]) != pk_unset) return err("insert: rows mix PK-set and PK-unset (row 0 vs row {i}); partition the array before calling insert", type<int64>)
    }
    // BEGIN/COMMIT vs SAVEPOINT/RELEASE depending on ambient state, so this composes inside with_transaction.
    let was_in_txn = db |> in_transaction()
    let begin_sql = was_in_txn ? "SAVEPOINT das_ins_sp" : "BEGIN IMMEDIATE"
    let begin_err = db |> try_exec(begin_sql)
    if (begin_err |> is_some) return err(begin_err |> unwrap, type<int64>)
    let sql = pk_unset ? _::_sql_insert_no_pk_sql(rows[0]) : _::_sql_insert_with_pk_sql(rows[0])
    var stmt : sqlite3_stmt?
    let prc = _db_prepare(db, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "insert prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        if (was_in_txn) {
            db |> try_exec("ROLLBACK TO das_ins_sp") |> ignore_sql_error
            db |> try_exec("RELEASE das_ins_sp") |> ignore_sql_error
        } else {
            db |> try_exec("ROLLBACK") |> ignore_sql_error
        }
        return err(msg, type<int64>)
    }
    for (row in rows) {
        sqlite3_reset(stmt)
        sqlite3_clear_bindings(stmt)
        _::_sql_bind_row(stmt, row, !pk_unset)
        let src = sqlite3_step(stmt)
        if (src != SQLITE_DONE) {
            let msg = "insert step failed: {sqlite3_errmsg(db.sqlite_handle)}"
            sqlite3_finalize(stmt)
            if (was_in_txn) {
                db |> try_exec("ROLLBACK TO das_ins_sp") |> ignore_sql_error
                db |> try_exec("RELEASE das_ins_sp") |> ignore_sql_error
            } else {
                db |> try_exec("ROLLBACK") |> ignore_sql_error
            }
            return err(msg, type<int64>)
        }
    }
    sqlite3_finalize(stmt)
    let commit_sql = was_in_txn ? "RELEASE das_ins_sp" : "COMMIT"
    let commit_err = db |> try_exec(commit_sql)
    if (commit_err |> is_some) {
        // COMMIT/RELEASE can fail (SQLITE_BUSY/IO); roll back so the connection isn't left mid-transaction.
        if (was_in_txn) {
            db |> try_exec("ROLLBACK TO das_ins_sp") |> ignore_sql_error
            db |> try_exec("RELEASE das_ins_sp") |> ignore_sql_error
        } else {
            db |> try_exec("ROLLBACK") |> ignore_sql_error
        }
        return err(commit_err |> unwrap, type<int64>)
    }
    return ok(sqlite3_last_insert_rowid(db.sqlite_handle), type<string>)
}

def insert(db : SqlRunner; rows : array<auto(TT)>) : int64 {
    //! Strict variant of ``try_insert(array<T>)``. Panics with the libsqlite3 errmsg on failure (after ROLLBACK).
    let r = db |> try_insert(rows)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

[unused_argument(e)]
def private ignore_sql_error(e : SqlError) : void {
}

// ===== INSERT OR IGNORE / INSERT OR REPLACE =====

def private with_or_clause(plain_sql : string; or_clause : string) : string {
    if (empty(or_clause)) return plain_sql
    return replace(plain_sql, "INSERT ", "INSERT {or_clause} ")
}

def private try_insert_or(db : SqlRunner; row : auto(TT); or_clause : string) : Result<int, string> {
    let pk_unset = _::_sql_pk_is_unset(row)
    let plain_sql = pk_unset ? _::_sql_insert_no_pk_sql(row) : _::_sql_insert_with_pk_sql(row)
    let sql = with_or_clause(plain_sql, or_clause)
    let r = db |> try_step_dml(sql, $(var stmt : sqlite3_stmt?) {
        _::_sql_bind_row(stmt, row, !pk_unset)
    }, "insert_or_{or_clause}")
    if (r |> is_err) return err(r |> unwrap_err, type<int>)
    return ok(sqlite3_changes(db.sqlite_handle), type<string>)
}

def private try_insert_or(db : SqlRunner; rows : array<auto(TT)>; or_clause : string) : Result<int, string> {
    if (empty(rows)) return ok(0, type<string>)
    let pk_unset = _::_sql_pk_is_unset(rows[0])
    for (i in 1..length(rows)) {
        if (_::_sql_pk_is_unset(rows[i]) != pk_unset) return err("insert_or_{or_clause}: rows mix PK-set and PK-unset (row 0 vs row {i}); partition the array before calling", type<int>)
    }
    let was_in_txn = db |> in_transaction()
    let begin_sql = was_in_txn ? "SAVEPOINT das_inso_sp" : "BEGIN IMMEDIATE"
    let begin_err = db |> try_exec(begin_sql)
    if (begin_err |> is_some) return err(begin_err |> unwrap, type<int>)
    let plain_sql = pk_unset ? _::_sql_insert_no_pk_sql(rows[0]) : _::_sql_insert_with_pk_sql(rows[0])
    let sql = with_or_clause(plain_sql, or_clause)
    var stmt : sqlite3_stmt?
    let prc = _db_prepare(db, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "insert_or_{or_clause} prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        if (was_in_txn) {
            db |> try_exec("ROLLBACK TO das_inso_sp") |> ignore_sql_error
            db |> try_exec("RELEASE das_inso_sp") |> ignore_sql_error
        } else {
            db |> try_exec("ROLLBACK") |> ignore_sql_error
        }
        return err(msg, type<int>)
    }
    var total_changes = 0
    for (row in rows) {
        sqlite3_reset(stmt)
        sqlite3_clear_bindings(stmt)
        _::_sql_bind_row(stmt, row, !pk_unset)
        let src = sqlite3_step(stmt)
        if (src != SQLITE_DONE) {
            let msg = "insert_or_{or_clause} step failed: {sqlite3_errmsg(db.sqlite_handle)}"
            sqlite3_finalize(stmt)
            if (was_in_txn) {
                db |> try_exec("ROLLBACK TO das_inso_sp") |> ignore_sql_error
                db |> try_exec("RELEASE das_inso_sp") |> ignore_sql_error
            } else {
                db |> try_exec("ROLLBACK") |> ignore_sql_error
            }
            return err(msg, type<int>)
        }
        total_changes += sqlite3_changes(db.sqlite_handle)
    }
    sqlite3_finalize(stmt)
    let commit_sql = was_in_txn ? "RELEASE das_inso_sp" : "COMMIT"
    let commit_err = db |> try_exec(commit_sql)
    if (commit_err |> is_some) {
        if (was_in_txn) {
            db |> try_exec("ROLLBACK TO das_inso_sp") |> ignore_sql_error
            db |> try_exec("RELEASE das_inso_sp") |> ignore_sql_error
        } else {
            db |> try_exec("ROLLBACK") |> ignore_sql_error
        }
        return err(commit_err |> unwrap, type<int>)
    }
    return ok(total_changes, type<string>)
}

def try_insert_or_ignore(db : SqlRunner; row : auto(TT)) : Result<int, string> {
    //! Single-row INSERT OR IGNORE. Returns ``Ok(1)`` when the row was
    //! inserted, ``Ok(0)`` when a PK / UNIQUE violation was silently ignored.
    return <- db |> try_insert_or(row, "OR IGNORE")
}

def insert_or_ignore(db : SqlRunner; row : auto(TT)) : int {
    //! Strict variant of ``try_insert_or_ignore(row)``. Panics on SQL error.
    let r = db |> try_insert_or_ignore(row)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_insert_or_ignore(db : SqlRunner; rows : array<auto(TT)>) : Result<int, string> {
    //! Bulk INSERT OR IGNORE inside a single transaction. Returns the sum of
    //! per-row changes — total rows actually inserted (ignored rows count 0).
    return <- db |> try_insert_or(rows, "OR IGNORE")
}

def insert_or_ignore(db : SqlRunner; rows : array<auto(TT)>) : int {
    //! Strict variant of ``try_insert_or_ignore(array<T>)``. Panics on SQL error (after ROLLBACK).
    let r = db |> try_insert_or_ignore(rows)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_insert_or_replace(db : SqlRunner; row : auto(TT)) : Result<int, string> {
    //! Single-row INSERT OR REPLACE. Returns ``Ok(rows_affected)`` — typically
    //! 1; can be more when CASCADE deletes ripple from a PK collision.
    return <- db |> try_insert_or(row, "OR REPLACE")
}

def insert_or_replace(db : SqlRunner; row : auto(TT)) : int {
    //! Strict variant of ``try_insert_or_replace(row)``. Panics on SQL error.
    let r = db |> try_insert_or_replace(row)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_insert_or_replace(db : SqlRunner; rows : array<auto(TT)>) : Result<int, string> {
    //! Bulk INSERT OR REPLACE inside a single transaction. Returns the sum of
    //! per-row changes.
    return <- db |> try_insert_or(rows, "OR REPLACE")
}

def insert_or_replace(db : SqlRunner; rows : array<auto(TT)>) : int {
    //! Strict variant of ``try_insert_or_replace(array<T>)``. Panics on SQL error (after ROLLBACK).
    let r = db |> try_insert_or_replace(rows)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

// ===== by-PK UPDATE / DELETE =====

def try_update(db : SqlRunner; row : auto(TT)) : Result<int, string> {
    //! Updates a single row identified by its @sql_primary_key field. Returns
    //! ``Ok(rows_affected)`` (1 if the row matched, 0 if not), ``Err(errmsg)``
    //! on SQL failure.
    return <- db |> try_step_dml(_::_sql_update_by_pk_sql(row), $(var stmt : sqlite3_stmt?) {
        _::_sql_bind_row_for_update(stmt, row)
    }, "update")
}

def update(db : SqlRunner; row : auto(TT)) : int {
    //! Strict variant of ``try_update(row)``. Panics with the libsqlite3 errmsg on failure.
    let r = db |> try_update(row)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_delete_(db : SqlRunner; row : auto(TT)) : Result<int, string> {
    //! Deletes the row identified by ``row``'s @sql_primary_key field (non-PK fields ignored).
    //! ``Ok(rows_affected)`` (1 if matched, 0 if not), ``Err(errmsg)`` on SQL failure.
    //! Trailing underscore: ``delete`` is a daslang keyword (cf. ``daslib/linq``'s ``where_``).
    return <- db |> try_step_dml(_::_sql_delete_by_pk_sql(row), $(var stmt : sqlite3_stmt?) {
        _::_sql_bind_row_pk_only(stmt, row)
    }, "delete")
}

def delete_(db : SqlRunner; row : auto(TT)) : int {
    //! Strict variant of ``try_delete_(row)``. Panics with the libsqlite3 errmsg on failure.
    let r = db |> try_delete_(row)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

[unused_argument(t)]
def try_delete_by_id(db : SqlRunner; t : type<auto(TT)>; id : auto(K)) : Result<int, string> {
    //! Deletes the row of type ``T`` whose primary key equals ``id`` — no dummy struct needed.
    //! ``Ok(rows_affected)`` (1 or 0), ``Err(errmsg)`` on SQL failure.
    return <- db |> try_step_dml(_::_sql_delete_by_pk_sql(default<TT>), $(var stmt : sqlite3_stmt?) {
        sql_bind_to_stmt(stmt, 1, id)
    }, "delete_by_id")
}

[unused_argument(t)]
def delete_by_id(db : SqlRunner; t : type<auto(TT)>; id : auto(K)) : int {
    //! Strict variant of ``try_delete_by_id``. Panics with the libsqlite3 errmsg on failure.
    let r = db |> try_delete_by_id(type<TT>, id)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

// ===== Transactions =====

enum SqliteTxnMode {
    //! BEGIN modifiers for SQLite. Maps to ``BEGIN [DEFERRED|IMMEDIATE|EXCLUSIVE]``.
    Deferred    //!< Default; lock acquisition deferred to the first read/write.
    Immediate   //!< RESERVED lock at BEGIN; concurrent writers block immediately.
    Exclusive   //!< EXCLUSIVE lock; blocks readers and writers (legacy mode; not WAL-safe).
}

def in_transaction(db : SqlRunner) : bool {
    //! Returns true inside a BEGIN/COMMIT pair (or SAVEPOINT) — inverse of ``sqlite3_get_autocommit``.
    return !sqlite3_get_autocommit_(db.sqlite_handle)
}

def private txn_begin_sql(was_in_txn : bool; mode : SqliteTxnMode) : string {
    if (was_in_txn) return "SAVEPOINT das_sp"
    if (mode == SqliteTxnMode.Immediate) return "BEGIN IMMEDIATE"
    if (mode == SqliteTxnMode.Exclusive) return "BEGIN EXCLUSIVE"
    return "BEGIN"
}

def private txn_commit_sql(was_in_txn : bool) : string {
    return was_in_txn ? "RELEASE das_sp" : "COMMIT"
}

def private txn_best_effort_rollback(db : SqlRunner; was_in_txn : bool) : void {
    // Swallow rollback errors so the original failure reaches the caller intact.
    if (was_in_txn) {
        db |> try_exec("ROLLBACK TO das_sp") |> ignore_sql_error
        db |> try_exec("RELEASE das_sp") |> ignore_sql_error
    } else {
        db |> try_exec("ROLLBACK") |> ignore_sql_error
    }
}

def with_transaction(db : SqlRunner; blk : block<() : void>) : void {
    //! 2-arg overload — abstract layer entry point. Defaults to
    //! ``SqliteTxnMode.Deferred`` (matches SQLite's default ``BEGIN``).
    db |> with_transaction(SqliteTxnMode.Deferred, blk)
}

def with_transaction(db : SqlRunner; mode : SqliteTxnMode; blk : block<() : void>) : void {
    //! Wraps ``blk`` in a SQL transaction (BEGIN [mode] / COMMIT; nests as SAVEPOINT/RELEASE).
    //! ROLLBACK fires on panic / early return via ``finally`` — best effort.
    let was_in_txn = db |> in_transaction()
    db |> exec(txn_begin_sql(was_in_txn, mode))
    var success = false
    {
        invoke(blk)
        success = true
    } finally {
        if (success) {
            // COMMIT/RELEASE can fail (SQLITE_BUSY/IO); roll back, then panic with the commit message.
            let commit_err = db |> try_exec(txn_commit_sql(was_in_txn))
            if (commit_err |> is_some) {
                txn_best_effort_rollback(db, was_in_txn)
                panic(commit_err |> unwrap)
            }
        } else {
            txn_best_effort_rollback(db, was_in_txn)
        }
    }
}

def try_transaction(db : SqlRunner; blk : block<() : void>) : SqlError {
    //! Non-panic 2-arg overload. ``none`` on commit success, ``some(errmsg)`` on BEGIN/COMMIT fail.
    //! Block panics still ROLLBACK and re-propagate; wrap in your own try/recover to convert.
    return db |> try_transaction(SqliteTxnMode.Deferred, blk)
}

def try_transaction(db : SqlRunner; mode : SqliteTxnMode; blk : block<() : void>) : SqlError {
    //! Non-panic 3-arg overload. Same semantics as ``try_transaction(db, blk)``
    //! with a SQLite-specific BEGIN modifier.
    let was_in_txn = db |> in_transaction()
    let begin_err = db |> try_exec(txn_begin_sql(was_in_txn, mode))
    if (begin_err |> is_some) return begin_err
    var success = false
    {
        invoke(blk)
        success = true
    } finally {
        if (!success) {
            txn_best_effort_rollback(db, was_in_txn)
        }
    }
    // COMMIT/RELEASE can still fail (SQLITE_BUSY/IO); roll back so the connection isn't left mid-transaction.
    let commit_err = db |> try_exec(txn_commit_sql(was_in_txn))
    if (commit_err |> is_some) {
        txn_best_effort_rollback(db, was_in_txn)
    }
    return commit_err
}

// ===== SQL quoting helpers =====

def sql_quote_id(s : string) : string {
    //! Wraps ``s`` in double quotes (SQLite-portable identifier quoting); embedded ``"`` doubled.
    let escaped = s |> replace("\"", "\"\"")
    return "\"{escaped}\""
}

def sql_quote_lit(s : string) : string {
    //! Wraps ``s`` in single quotes (SQL string literal); embedded ``'`` doubled.
    //! Used for ``inline_lit`` fragments where the value can't go through ``?`` (CREATE VIEW, PRAGMA, …).
    let escaped = s |> replace("'", "''")
    return "'{escaped}'"
}

// ===== PRAGMA helpers =====

def try_set_pragma(db : SqlRunner; name : string; value : string) : SqlError {
    //! Sets ``PRAGMA "<name>" = '<value>'`` (value single-quoted, embedded ``'`` doubled).
    return db |> try_exec("PRAGMA {sql_quote_id(name)} = {sql_quote_lit(value)}")
}

def try_set_pragma(db : SqlRunner; name : string; value : int64) : SqlError {
    //! Sets ``PRAGMA "<name>" = <int>``. Used for numeric pragmas like
    //! ``busy_timeout`` (ms), ``cache_size`` (pages or KB), ``page_size``.
    return db |> try_exec("PRAGMA {sql_quote_id(name)} = {value}")
}

def try_set_pragma(db : SqlRunner; name : string; value : bool) : SqlError {
    //! Sets ``PRAGMA "<name>" = ON|OFF``. Used for boolean pragmas like
    //! ``foreign_keys``, ``recursive_triggers``, ``trusted_schema``.
    let onoff = value ? "ON" : "OFF"
    return db |> try_exec("PRAGMA {sql_quote_id(name)} = {onoff}")
}

def set_pragma(db : SqlRunner; name : string; value : string) : void {
    //! Strict variant of ``try_set_pragma`` (string overload). Panics on error.
    let err = db |> try_set_pragma(name, value)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def set_pragma(db : SqlRunner; name : string; value : int64) : void {
    //! Strict variant of ``try_set_pragma`` (int64 overload). Panics on error.
    let err = db |> try_set_pragma(name, value)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def set_pragma(db : SqlRunner; name : string; value : bool) : void {
    //! Strict variant of ``try_set_pragma`` (bool overload). Panics on error.
    let err = db |> try_set_pragma(name, value)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def try_apply_recommended_pragmas(db : SqlRunner) : SqlError {
    //! Applies WAL + busy_timeout=5000 + foreign_keys=ON + synchronous=NORMAL.
    //! Stops at the first failure and returns its error. Call at connection setup, not mid-txn
    //! (``journal_mode`` cannot change inside an active transaction).
    let e1 = db |> try_set_pragma("journal_mode", "WAL")
    if (e1 |> is_some) return e1
    let e2 = db |> try_set_pragma("busy_timeout", 5000l)
    if (e2 |> is_some) return e2
    let e3 = db |> try_set_pragma("foreign_keys", true)
    if (e3 |> is_some) return e3
    let e4 = db |> try_set_pragma("synchronous", "NORMAL")
    if (e4 |> is_some) return e4
    return none(type<string>)
}

def apply_recommended_pragmas(db : SqlRunner) : void {
    //! Strict variant of ``try_apply_recommended_pragmas``. Panics on the
    //! first failing pragma.
    let err = db |> try_apply_recommended_pragmas()
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

// ===== VACUUM / OPTIMIZE / integrity check / quick check =====

def try_vacuum(db : SqlRunner) : SqlError {
    //! VACUUM rewrites the database file. Cannot run inside an active
    //! transaction; if one is in progress this returns an error rather
    //! than letting SQLite emit a confusing "cannot VACUUM" message.
    if (db |> in_transaction()) return some("VACUUM cannot run inside an active transaction; commit or rollback first")
    return db |> try_exec("VACUUM")
}

def vacuum(db : SqlRunner) : void {
    //! Strict variant of ``try_vacuum``. Panics on failure.
    let err = db |> try_vacuum()
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def try_vacuum_into(db : SqlRunner; path : string) : SqlError {
    //! VACUUM INTO '<path>' — snapshot the live database without modifying the source.
    //! Path is single-quoted (``''`` doubled); ``VACUUM INTO`` doesn't accept ``?`` placeholders.
    if (db |> in_transaction()) return some("VACUUM INTO cannot run inside an active transaction; commit or rollback first")
    return db |> try_exec("VACUUM INTO {sql_quote_lit(path)}")
}

def vacuum_into(db : SqlRunner; path : string) : void {
    //! Strict variant of ``try_vacuum_into``. Panics on failure.
    let err = db |> try_vacuum_into(path)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def try_optimize(db : SqlRunner) : SqlError {
    //! ``PRAGMA optimize`` — refresh the planner stats. Run at connection close on long-lived
    //! connections; cheap otherwise (short-circuits when nothing changed).
    return db |> try_exec("PRAGMA optimize")
}

def optimize(db : SqlRunner) : void {
    //! Strict variant of ``try_optimize``. Panics on failure.
    let err = db |> try_optimize()
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def private try_check_impl(db : SqlRunner; sql : string) : Result<array<string>, string> {
    return <- db |> try_run_select(sql,
        $(var stmt : sqlite3_stmt?) {},
        $(stmt : sqlite3_stmt?) {
            return read_via_adapter(stmt, 0, type<string>)
        })
}

def try_integrity_check(db : SqlRunner) : Result<array<string>, string> {
    //! Runs ``PRAGMA integrity_check``; returns ``["ok"]`` on a healthy DB or one row per
    //! detected issue. ``Err`` only on prepare/step failure.
    return <- db |> try_check_impl("PRAGMA integrity_check")
}

def integrity_check(db : SqlRunner) : array<string> {
    //! Strict variant of ``try_integrity_check``. Panics only on prepare/step failure
    //! (corruption surfaces via the returned rows, not a panic).
    var r <- db |> try_integrity_check()
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def try_quick_check(db : SqlRunner) : Result<array<string>, string> {
    //! Cheaper sibling of ``try_integrity_check`` — same return shape, skips the
    //! index-vs-table cross check. Useful as a pre-flight before the full check.
    return <- db |> try_check_impl("PRAGMA quick_check")
}

def quick_check(db : SqlRunner) : array<string> {
    //! Strict variant of ``try_quick_check``. Panics on prepare/step failure.
    var r <- db |> try_quick_check()
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def try_backup_to(db : SqlRunner; dest : SqlRunner) : SqlError {
    //! Online-backup copy of ``db`` into ``dest`` (incremental; tolerates concurrent writers
    //! with 50 ms BUSY/LOCKED retry). Useful for ``:memory:`` snapshots and hot backups.
    let rc = sqlite3_backup_run(db.sqlite_handle, dest.sqlite_handle)
    if (rc != SQLITE_OK) return some("backup_to: sqlite3_backup_run failed (rc={rc}): {sqlite3_errmsg(dest.sqlite_handle)}")
    return SqlError()
}

def backup_to(db : SqlRunner; dest : SqlRunner) : void {
    //! Strict variant of ``try_backup_to``. Panics with the libsqlite3 errmsg on non-completion.
    let err = db |> try_backup_to(dest)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def try_backup_to(db : SqlRunner; path : string) : SqlError {
    //! Opens ``path`` (creating if absent), copies ``db`` into it, closes on return.
    //! On backup failure the partial file is left on disk for ``fio::remove`` or diagnostics.
    var dest_result <- try_open_sqlite(path)
    if (dest_result |> is_err) return some(dest_result |> unwrap_err)
    var inscope dest = dest_result |> move_unwrap
    return <- db |> try_backup_to(dest)
}

def backup_to(db : SqlRunner; path : string) : void {
    //! Strict variant of the path-form ``try_backup_to``. Panics on
    //! open or backup failure.
    let err = db |> try_backup_to(path)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

// ===== Generic DML runners (used by `_sql_update` / `_sql_delete` macros) =====

def try_run_dml(db : SqlRunner; sql : string;
                bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : Result<int, string> {
    //! Prepares ``sql``, invokes ``bind_blk`` to bind parameters, steps once,
    //! and returns rows-affected. Used by the `_sql_update` / `_sql_delete`
    //! macros for predicate-based mutations.
    return <- db |> try_step_dml(sql, bind_blk, "DML")
}

def run_dml(db : SqlRunner; sql : string;
            bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : int {
    //! Strict variant of ``try_run_dml``. Panics with the libsqlite3 errmsg on failure.
    let r = db |> try_run_dml(sql, bind_blk)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_run_dml_returning(db : SqlRunner; sql : string;
                          bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                          row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : Result<array<TT -const -#>, string> {
    //! Prepares ``sql`` (must include a ``RETURNING ...`` clause), binds, and steps through every
    //! ``SQLITE_ROW`` calling ``row_blk``. Backs the ``_sql_update/_delete_returning`` macros.
    return <- db |> try_collect_rows(sql, bind_blk, row_blk, "DML RETURNING")
}

def run_dml_returning(db : SqlRunner; sql : string;
                      bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                      row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : array<TT -const -#> {
    //! Strict variant of ``try_run_dml_returning``. Panics on failure.
    var r = db |> try_run_dml_returning(sql, bind_blk, row_blk)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

[unused_argument(t)]
def try_select_from(db : SqlRunner; t : type<auto(TT)>) : Result<array<TT -const -#>, string> {
    //! Reads every row of ``T``'s table as ``array<TT>``. Compat-mode counterpart to the
    //! ``_sql(db |> select_from(type<T>))`` chain. ``Err(errmsg)`` on prepare or step failure.
    return <- db |> try_collect_rows(_::_sql_select_all_sql(type<TT -const -#>),
        $(var stmt : sqlite3_stmt?) {},
        $(stmt : sqlite3_stmt?) {
            static_if (typeinfo can_copy(default<TT -const -#>)) {
                return _::_sql_read_row(stmt, type<TT -const -#>)
            } else {
                return <- clone_to_move(_::_sql_read_row(stmt, type<TT -const -#>))
            }
        }, "select_from")
}

[unused_argument(t)]
def select_from(db : SqlRunner; t : type<auto(TT)>) : array<TT -const -#> {
    //! Strict variant of ``try_select_from``. Panics with the libsqlite3 errmsg on failure.
    var r <- db |> try_select_from(type<TT -const -#>)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

[unsafe_outside_of_for, nodiscard]
def run_each_select(db : SqlRunner; sql : string;
                    bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                    var row_lam : lambda<(stmt : sqlite3_stmt?) : auto(TT)>) {
    //! `_each_sql` macro runtime — lazy ``generator<TT>`` sibling of ``run_select``.
    //! Generator owns the prepared statement; ``sqlite3_finalize`` runs in its ``finally``.
    //! One-shot; panics on prepare/step failure (catch at a higher level for recovery).
    var stmt : sqlite3_stmt?
    let prc = _db_prepare(db, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "_each_sql: prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        panic(msg)
    }
    invoke(bind_blk, stmt)
    return <- generator<TT -const -#> capture(<- row_lam, <- stmt) {
        // `yield` must be the LAST statement of the for-body; step first, then yield the row.
        for (_ in range(0x7FFFFFFF)) {
            let rc = sqlite3_step(stmt)
            if (rc != SQLITE_ROW) {
                if (rc != SQLITE_DONE) {
                    panic("_each_sql: step failed: rc={rc}: {sqlite3_errmsg(sqlite3_db_handle(stmt))}")
                }
                break
            }
            yield invoke(row_lam, stmt)
        }
        return false
    } finally {
        sqlite3_finalize(stmt)
    }
}

def try_run_select(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : Result<array<TT -const -#>, string> {
    //! `_sql` macro runtime. Prepares + binds, then steps and materializes each row via ``row_blk``.
    //! Returns ``Err(errmsg)`` on prepare or step failure.
    return <- db |> try_collect_rows(sql, bind_blk, row_blk, "_sql")
}

def run_select(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : array<TT -const -#> {
    //! Strict variant of ``try_run_select``. Panics on prepare/step failure.
    var r <- try_run_select(db, sql, bind_blk, row_blk)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def try_run_select_one(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : Result<TT -const -#, string> {
    //! `_sql` runtime: single-row materializer (``_first()`` / ``_count()`` / aggregate chains).
    //! ``Err(errmsg)`` on prepare/step failure or zero rows; ``run_select_one_opt`` for the optional case.
    return <- try_one_row(db, sql, bind_blk, row_blk, "_sql")
}

def run_select_one(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : TT -const -# {
    //! Strict variant of ``try_run_select_one``. Panics on prepare/step failure or zero rows.
    var r <- try_run_select_one(db, sql, bind_blk, row_blk)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def try_run_select_one_opt(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : Result<Option<TT -const -#>, string> {
    //! Prepares ``sql``, runs ``bind_blk`` once, steps once. Returns
    //! ``Ok(some(row))`` if a row is found, ``Ok(none)`` if zero rows, or
    //! ``Err(errmsg)`` on prepare/step failure.
    return <- try_one_row_opt(db, sql, bind_blk, row_blk, "_sql")
}

def run_select_one_opt(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : Option<TT -const -#> {
    //! Strict variant of ``try_run_select_one_opt``. Panics on prepare/step failure;
    //! returns ``none`` on zero rows (which is the expected empty case here).
    var r <- try_run_select_one_opt(db, sql, bind_blk, row_blk)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

// ===== [sql_table] structure macro =====

def private find_annotation(args : AnnotationArgumentList; argn : string; default_value : auto(TT)) : TT {
    let v = find_arg(args, argn)
    static_if (typeinfo is_string(default_value)) {
        if (v is tString) return v as tString
    } else {
        if (v is tBool) return v as tBool
    }
    return default_value
}

def private find_string_array_annotation(args : AnnotationArgumentList; argn : string) : array<string> {
    return <- [for (a in args); string(a.sValue); where a.name == argn && a.basicType == Type.tString]
}

// Whitelisted to surface typos at macro-expansion instead of as a runtime SQLite parse error.
let private SQL_DEFAULT_FN_WHITELIST = ["CURRENT_TIMESTAMP", "CURRENT_DATE", "CURRENT_TIME"]

let private SQL_FK_ACTION_WHITELIST = ["cascade", "set_null", "set_default", "restrict", "no_action"]

def private fk_action_to_sql(action : string) : string {
    return "CASCADE" if (action == "cascade")
    return "SET NULL" if (action == "set_null")
    return "SET DEFAULT" if (action == "set_default")
    return "RESTRICT" if (action == "restrict")
    return "NO ACTION"
}

def private sql_quote_string_literal(s : string) : string {
    let escaped = replace(s, "'", "''")
    return "'{escaped}'"
}

def private literal_init_to_default_clause(init : ExpressionPtr) : string {
    if (init == null) return ""
    if (init is ExprConstBool) return (init as ExprConstBool).getValue ? " DEFAULT 1" : " DEFAULT 0"
    if (init is ExprConstInt) return " DEFAULT {(init as ExprConstInt).getValue}"
    if (init is ExprConstInt64) return " DEFAULT {(init as ExprConstInt64).getValue}"
    if (init is ExprConstFloat) return " DEFAULT {(init as ExprConstFloat).getValue}"
    if (init is ExprConstDouble) return " DEFAULT {(init as ExprConstDouble).getValue}"
    if (init is ExprConstString) return " DEFAULT {sql_quote_string_literal(string((init as ExprConstString).value))}"
    return ""
}

struct private FieldInfo {
    name : string
    col_name : string
    is_pk : bool
    is_int_pk : bool
    is_optional : bool
    is_unique : bool
    is_computed : bool
    is_stored : bool
    is_json : bool
    is_blob : bool
    is_fts_rank : bool   // [sql_fts5] only — read-only, emits `rank`, skipped on INSERT
    computed_sql : string
    default_clause : string
    references_clause : string
}

// ===== schema_from helpers — compile-time DB introspection + runtime check_schema =====

struct private SchemaFromCol {
    name : string
    decl_type : string
    not_null : bool
    pk : bool
}

def private try_open_sqlite_readonly(path : string) : Result<SqlRunner, string> {
    // READONLY open via C++ shim — sqlite3_open_v2 needs NULL vfs (daslang `string` can't carry it).
    var db = SqlRunner()
    let rc = sqlite3_open_v2_no_vfs(path, unsafe(addr(db.sqlite_handle)), int(SQLITE_OPEN_READONLY))
    if (rc != SQLITE_OK) {
        // OOM-on-open writes NULL into *ppDb; sqlite3_errmsg(NULL) is undefined, close_v2(NULL) is a no-op.
        let msg = db.sqlite_handle == null ? "sqlite3_open_v2(\"{path}\", READONLY) failed: out of memory" : "sqlite3_open_v2(\"{path}\", READONLY) failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_close_v2(db.sqlite_handle)
        db.sqlite_handle = null
        return err(msg, type<SqlRunner>)
    }
    return move_ok(db, type<string>)
}

def private read_pragma_table_info(db : SqlRunner; tbl_name : string) : Result<array<SchemaFromCol>, string> {
    // Honors db.schema_name so check_schema queries the right schema on ATTACH/with_schema runners.
    var rows : array<SchemaFromCol>
    let sql = empty(db.schema_name) ? "PRAGMA table_info({sql_quote_id(tbl_name)})" : "PRAGMA {sql_quote_id(db.schema_name)}.table_info({sql_quote_id(tbl_name)})"
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "PRAGMA table_info({tbl_name}) prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<array<SchemaFromCol>>)
    }
    while (true) {
        let src = sqlite3_step(stmt)
        if (src == SQLITE_DONE) break
        if (src != SQLITE_ROW) {
            let msg = "PRAGMA table_info({tbl_name}) step failed: {sqlite3_errmsg(db.sqlite_handle)}"
            sqlite3_finalize(stmt)
            return err(msg, type<array<SchemaFromCol>>)
        }
        // Columns: 0=cid, 1=name, 2=type, 3=notnull, 4=dflt_value, 5=pk
        rows |> emplace(SchemaFromCol(
            name = sqlite3_column_text_(stmt, 1),
            decl_type = sqlite3_column_text_(stmt, 2),
            not_null = sqlite3_column_int(stmt, 3) != 0,
            pk = sqlite3_column_int(stmt, 5) != 0
        ))
    }
    sqlite3_finalize(stmt)
    return <- move_ok(rows, type<string>)
}

def private affinity_to_sql_type(decl_type : string) : SqlType {
    // SQLite affinity rules: INTEGER first, then TEXT, BLOB, REAL, NUMERIC last (https://www.sqlite.org/datatype3.html § 3.1).
    let upper = decl_type |> to_upper
    if (find(upper, "INT") >= 0) return SqlType.Integer
    if (find(upper, "CHAR") >= 0 || find(upper, "CLOB") >= 0 || find(upper, "TEXT") >= 0) return SqlType.Text
    if (find(upper, "BLOB") >= 0 || empty(decl_type)) return SqlType.Blob
    if (find(upper, "REAL") >= 0 || find(upper, "FLOA") >= 0 || find(upper, "DOUB") >= 0) return SqlType.Real
    // NUMERIC affinity (NUMERIC/DECIMAL/BOOLEAN/DATE/DATETIME) → double; user partial-body overrides for BOOL/DATE.
    return SqlType.Real
}

def private affinity_to_typedecl(sql_type : SqlType; not_null : bool) : TypeDeclPtr {
    // Build the daslang TypeDecl matching a SQLite affinity class; wrap in Option<T> if nullable.
    var inner : TypeDeclPtr
    if (sql_type == SqlType.Integer) {
        inner = qmacro_type(type<int64>)
    } elif (sql_type == SqlType.Real) {
        inner = qmacro_type(type<double>)
    } elif (sql_type == SqlType.Text) {
        inner = qmacro_type(type<string>)
    } elif (sql_type == SqlType.Blob) {
        inner = qmacro_type(type<array<uint8>>)
    } else {
        return null
    }
    if (not_null) return inner
    return qmacro_type(type<$Option<$t(inner)>>)
}

def private resolve_schema_from_path(p : string) : string {
    // Absolute → as-is; relative → joined under get_das_root() via daslib/fio.
    if (is_absolute(p)) return p
    return path_join(get_das_root(), p)
}

def is_option_field_type(td : TypeDeclPtr) : bool {
    if (td == null) return false
    // Pre-instantiation form: typeMacro `$Option<T>` — what [sql_table] sees during structure_macro apply().
    if (td.baseType == Type.typeMacro && !empty(td.dimExpr) && (td.dimExpr[0] is ExprConstString)) return (td.dimExpr[0] as ExprConstString).value == "Option"
    // Post-instantiation form: structural named tuple ``tuple<_has_value:bool; _value:T>``.
    // Option is built by daslib/option's typemacro_function as a structural tuple — no
    // module home, no Structure clone. Match the canonical shape.
    return (td.baseType == Type.tTuple
            && length(td.argTypes) == 2
            && length(td.argNames) == 2
            && td.argNames[0] == "_has_value"
            && td.argNames[1] == "_value"
            && td.argTypes[0].baseType == Type.tBool)
}

def unwrap_option_payload_type(var td : TypeDeclPtr) : TypeDeclPtr {
    // Unwrap Option<T> → T (or td unchanged); typeMacro carries T in dimExpr[1], the
    // structural tuple form carries T in argTypes[1] (the `_value` slot).
    if (td == null) return td
    if (td.baseType == Type.typeMacro) {
        if (length(td.dimExpr) < 2 || td.dimExpr[0] == null || !(td.dimExpr[0] is ExprConstString)
            || (td.dimExpr[0] as ExprConstString).value != "Option"
            || td.dimExpr[1] == null || !(td.dimExpr[1] is ExprTypeDecl)) return td
        return clone_type((td.dimExpr[1] as ExprTypeDecl).typeexpr)
    }
    // Post-instantiation form: structural named tuple ``tuple<_has_value:bool; _value:T>``.
    if (td.baseType == Type.tTuple
            && length(td.argTypes) == 2
            && length(td.argNames) == 2
            && td.argNames[0] == "_has_value"
            && td.argNames[1] == "_value"
            && td.argTypes[0].baseType == Type.tBool) return clone_type(td.argTypes[1])
    return td
}

def private build_create_indexes_sql(st : StructurePtr; table_name : string; field_names : array<string>; var errors : das_string; var ok : bool&) : string {
    ok = true
    var stmt_count = 0
    let sql = build_string() $(var w) {
        for (ann in st.annotations) {
            if (ann.annotation.name != "sql_index") continue
            let idx_name_arg = find_annotation(ann.arguments, "name", "")
            let is_unique = find_annotation(ann.arguments, "unique", false)
            let cols = find_string_array_annotation(ann.arguments, "fields")
            if (empty(cols)) {
                errors := "[sql_index]: missing or empty `fields=` argument. Use `fields=\"ColName\"` for a single-column index or `fields=(\"A\", \"B\")` for a composite."
                ok = false
                return
            }
            for (col in cols) {
                var found = false
                for (fn in field_names) {
                    if (fn == col) {
                        found = true
                    }
                }
                if (!found) {
                    errors := "[sql_index]: field '{col}' is not a column of struct '{st.name}'. Available: {field_names}"
                    ok = false
                    return
                }
            }
            let auto_name = build_string() $(var w2) {
                w2 |> write("idx_")
                w2 |> write(table_name)
                for (col in cols) {
                    w2 |> write("_")
                    w2 |> write(col)
                }
            }
            let idx_name = empty(idx_name_arg) ? auto_name : idx_name_arg
            let kind = is_unique ? "UNIQUE INDEX" : "INDEX"
            if (stmt_count > 0) {
                w |> write("; ")
            }
            stmt_count++
            w |> write("CREATE {kind} \"{idx_name}\" ON \"{table_name}\" (")
            for (j in range(length(cols))) {
                if (j > 0) {
                    w |> write(", ")
                }
                w |> write("\"")
                w |> write(cols[j])
                w |> write("\"")
            }
            w |> write(")")
        }
    }
    return sql
}

def private extract_string_return(var fn : FunctionPtr) : string {
    if (fn == null || fn.body == null || !(fn.body is ExprBlock)) return ""
    let blk = fn.body as ExprBlock
    // Walk past pre-return concept_asserts emitted by schema_from contract checks.
    for (stmt in blk.list) {
        if (!(stmt is ExprReturn)) continue
        let ret = stmt as ExprReturn
        let val = ret.subexpr
        if (val == null || !(val is ExprConstString)) return ""
        return string((val as ExprConstString).value)
    }
    return ""
}

def private find_struct_helper_fn(name : string; st : StructurePtr) : FunctionPtr {
    var found : FunctionPtr
    for_each_function(compiling_module(), name) $(var fn) {
        if (!empty(fn.arguments)
                && fn.arguments[0]._type != null
                && fn.arguments[0]._type.structType == st) {
            found = fn
        }
    }
    return found
}

def private derive_index_name(table_name : string; sql_cols : array<string>) : string {
    // Shared between [sql_index] structure annotation and create_index call macro: idx_<table>_<col1>_<col2>… on the post-@sql_column SQL identifiers.
    return build_string() $(var w) {
        w |> write("idx_")
        w |> write(table_name)
        for (col in sql_cols) {
            w |> write("_")
            w |> write(col)
        }
    }
}

[structure_macro(name=sql_index)]
class SqlIndexMacro : AstStructureAnnotation {
    //! Stackable struct-level annotation: ``[sql_index(fields="A"|("A","B"), unique=?, name=?)]``.
    //! Must appear after ``[sql_table]`` in the annotation list (it rewrites that helper's body).
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        // Reject on legacy=true structs — the [sql_table] emits no CREATE TABLE, so the index would
        // have no statement to attach to and would collide with the current struct's index on rebuild.
        if (find_struct_helper_fn("_sql_legacy_table_marker", st) != null) {
            errors := "[sql_index] on '{st.name}': not supported on [sql_table(legacy=true)] structs (they don't define schema). Drop the [sql_index] annotation; the current (non-legacy) struct's indexes are reapplied automatically by ``convert_and_rename``."
            return false
        }
        let cols = find_string_array_annotation(args, "fields")
        if (empty(cols)) {
            errors := "[sql_index]: missing or empty `fields=` argument. Use `fields=\"ColName\"` for a single-column index or `fields=(\"A\", \"B\")` for a composite."
            return false
        }
        let is_unique = find_annotation(args, "unique", false)
        let idx_name_arg = find_annotation(args, "name", "")

        // User writes daslang field names; emit @sql_column-renamed SQL identifiers in DDL.
        var sql_cols : array<string>
        sql_cols |> reserve(length(cols))
        for (col in cols) {
            var has_field = false
            var resolved = col
            for (field in st.fields) {
                if (field.name == col) {
                    has_field = true
                    let ca = find_arg(field.annotation, "sql_column")
                    if (ca is tString) {
                        resolved = string(ca as tString)
                    }
                }
            }
            if (!has_field) {
                errors := "[sql_index]: field '{col}' is not a column of struct '{st.name}'."
                return false
            }
            sql_cols |> push(resolved)
        }

        // [sql_table] must precede [sql_index] in the annotation list (source-order rule).
        var idx_fn = find_struct_helper_fn("_sql_create_indexes_sql", st)
        if (idx_fn == null) {
            errors := "[sql_index] on struct '{st.name}': no [sql_table] declared earlier in the annotation list. Use `[sql_table(...), sql_index(...), ...]` order so the index helper is in scope."
            return false
        }
        var name_fn = find_struct_helper_fn("_sql_table_name", st)
        if (name_fn == null) {
            errors := "[sql_index] on struct '{st.name}': cannot resolve table name (no [sql_table] visible). Same source-order rule as above."
            return false
        }
        let table_name = extract_string_return(name_fn)
        if (empty(table_name)) {
            errors := "[sql_index] on struct '{st.name}': internal — _sql_table_name body did not match expected shape."
            return false
        }

        // Auto-name uses SQL (post-rename) column names so distinct schemas don't collide.
        let auto_name = derive_index_name(table_name, sql_cols)
        let idx_name = empty(idx_name_arg) ? auto_name : idx_name_arg
        let kind = is_unique ? "UNIQUE INDEX" : "INDEX"
        let this_stmt = build_string() $(var w) {
            w |> write("CREATE {kind} \"{idx_name}\" ON \"{table_name}\" (")
            for (j in range(length(sql_cols))) {
                if (j > 0) {
                    w |> write(", ")
                }
                w |> write("\"")
                w |> write(sql_cols[j])
                w |> write("\"")
            }
            w |> write(")")
        }

        // Append this statement to whatever earlier [sql_index] siblings already accumulated.
        let prev_sql = extract_string_return(idx_fn)
        let new_sql = empty(prev_sql) ? this_stmt : "{prev_sql}; {this_stmt}"
        idx_fn.body = qmacro_block() {
            return $v(new_sql)
        }
        idx_fn.body |> force_at(st.at)
        return true
    }
}

def private find_parent_table_and_pk(var st : StructurePtr; parent_name : string) : tuple<tab_name : string; pk_col : string; found : bool> {
    var found_st : StructurePtr
    if (st.name == parent_name) {
        found_st = st
    } else {
        for_each_structure(compiling_module()) $(var ps) {
            if (ps.name == parent_name) {
                found_st = ps
            }
        }
    }
    if (found_st == null) return (tab_name = "", pk_col = "", found = false)
    var tab = string(found_st.name)
    for (ann in found_st.annotations) {
        if (ann.annotation.name == "sql_table") {
            tab = find_annotation(ann.arguments, "name", tab)
        }
    }
    for (pf in found_st.fields) {
        if (find_annotation(pf.annotation, "sql_primary_key", false)) {
            // FK references the SQL identifier (post @sql_column rename), not the daslang field name.
            var pk_col = string(pf.name)
            let pk_anno = find_arg(pf.annotation, "sql_column")
            if (pk_anno is tString) {
                pk_col = string(pk_anno as tString)
            }
            return (tab_name = tab, pk_col = pk_col, found = true)
        }
    }
    return (tab_name = tab, pk_col = "", found = false)
}

[structure_macro(name=sql_table)]
class SqlTableMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let table_name = find_annotation(args, "name", string(st.name))
        // legacy=true: emit read-side helpers only. Writes fail as unresolved-overload.
        let is_legacy = find_annotation(args, "legacy", false)

        var schema_assert_exprs : array<ExpressionPtr>
        let schema_from_arg = find_arg(args, "schema_from")
        if (schema_from_arg is tString) {
            let raw_path = string(schema_from_arg as tString)
            if (empty(raw_path)) {
                errors := "[sql_table]: schema_from=\"\" is invalid — provide a non-empty path to a SQLite database file."
                return false
            }
            if (!apply_schema_from(st, raw_path, table_name, schema_assert_exprs, errors)) return false
        }

        var fields : array<FieldInfo>
        fields |> reserve(length(st.fields))
        for (field in st.fields) {
            let fname = string(field.name)
            let is_pk = find_annotation(field.annotation, "sql_primary_key", false)
            let is_unique = find_annotation(field.annotation, "sql_unique", false)
            let is_optional = is_option_field_type(field._type)

            // @sql_computed expression
            let computed_sql = find_annotation(field.annotation, "sql_computed", "")
            let is_computed = !empty(computed_sql)
            let is_stored = find_annotation(field.annotation, "sql_stored", false)

            // @sql_json / @sql_blob — non-scalar column storage with an emitted sql_bind/sql_extract pair.
            let is_json = find_annotation(field.annotation, "sql_json", false)
            let is_blob = find_annotation(field.annotation, "sql_blob", false)

            // Defaults — native daslang field init OR @sql_default_fn (mutually exclusive)
            let sql_default_fn = find_annotation(field.annotation, "sql_default_fn", "")

            // Foreign key — @sql_references + optional @sql_on_delete / @sql_on_update
            let references = find_annotation(field.annotation, "sql_references", "")
            let on_delete = find_annotation(field.annotation, "sql_on_delete", "no_action")
            let on_update = find_annotation(field.annotation, "sql_on_update", "no_action")
            let has_on_delete_attr = !empty(find_annotation(field.annotation, "sql_on_delete", ""))
            let has_on_update_attr = !empty(find_annotation(field.annotation, "sql_on_update", ""))

            // @sql_column rename. find_arg distinguishes absent from `= ""` (find_annotation can't).
            var col_name = fname
            let col_anno = find_arg(field.annotation, "sql_column")
            if (col_anno is tString) {
                let cv = string(col_anno as tString)
                if (empty(cv)) {
                    errors := "[sql_table]: field '{fname}' @sql_column = \"\" is invalid — provide a non-empty SQL column name."
                    return false
                }
                if (find(cv, "\"") >= 0 || find(cv, "\\") >= 0) {
                    errors := "[sql_table]: field '{fname}' @sql_column=\"{cv}\" — embedded `\"` / `\\` are rejected (SQL identifier hygiene). Use a plain identifier."
                    return false
                }
                col_name = cv
            }

            // ---- validation ----
            if (is_pk && is_optional) {
                errors := "[sql_table]: @sql_primary_key on field '{fname}' must not be Option<T> — primary keys cannot be NULL in SQL. Use a non-optional type."
                return false
            }
            if (is_computed && is_pk) {
                errors := "[sql_table]: field '{fname}' cannot be both @sql_primary_key and @sql_computed — SQLite rejects PRIMARY KEY on a generated column."
                return false
            }
            if (is_computed && field.init != null) {
                errors := "[sql_table]: field '{fname}' is @sql_computed; remove the daslang field initializer (the SQL expression provides the value)."
                return false
            }
            if (is_computed && !empty(sql_default_fn)) {
                errors := "[sql_table]: field '{fname}' is @sql_computed; @sql_default_fn does not apply (computed columns derive their value from the expression)."
                return false
            }
            if (is_json && is_blob) {
                errors := "[sql_table]: field '{fname}' has both @sql_json and @sql_blob — pick one. Use @sql_json for queryable, schema-tolerant TEXT storage; use @sql_blob for opaque, faster binary storage."
                return false
            }
            let json_or_blob_kind = is_json ? "json" : "blob"
            if ((is_json || is_blob) && is_pk) {
                errors := "[sql_table]: field '{fname}' is @sql_{json_or_blob_kind} — non-scalar columns cannot be a PRIMARY KEY."
                return false
            }
            if ((is_json || is_blob) && is_computed) {
                errors := "[sql_table]: field '{fname}' is @sql_computed; @sql_{json_or_blob_kind} does not apply (computed columns derive their value from the SQL expression, not from a daslang adapter)."
                return false
            }
            if (field.init != null && !empty(sql_default_fn)) {
                errors := "[sql_table]: field '{fname}' has both a daslang field initializer and @sql_default_fn — pick one. Use the field init for literal defaults, @sql_default_fn for SQL built-ins like CURRENT_TIMESTAMP."
                return false
            }
            if (!empty(sql_default_fn)) {
                var fn_ok = false
                for (allowed in SQL_DEFAULT_FN_WHITELIST) {
                    if (allowed == sql_default_fn) {
                        fn_ok = true
                    }
                }
                if (!fn_ok) {
                    errors := "[sql_table]: field '{fname}' @sql_default_fn=\"{sql_default_fn}\" is not in the supported set (CURRENT_TIMESTAMP / CURRENT_DATE / CURRENT_TIME). Use raw SQL via `db |> exec(...)` for other expressions."
                    return false
                }
            }
            if ((has_on_delete_attr || has_on_update_attr) && empty(references)) {
                errors := "[sql_table]: field '{fname}' has @sql_on_delete / @sql_on_update without @sql_references — these annotations only apply to a foreign-key relationship."
                return false
            }
            if (!empty(references)) {
                var od_ok = false
                var ou_ok = false
                for (allowed in SQL_FK_ACTION_WHITELIST) {
                    if (allowed == on_delete) {
                        od_ok = true
                    }
                    if (allowed == on_update) {
                        ou_ok = true
                    }
                }
                if (!od_ok) {
                    errors := "[sql_table]: field '{fname}' @sql_on_delete=\"{on_delete}\" must be one of cascade / set_null / set_default / restrict / no_action."
                    return false
                }
                if (!ou_ok) {
                    errors := "[sql_table]: field '{fname}' @sql_on_update=\"{on_update}\" must be one of cascade / set_null / set_default / restrict / no_action."
                    return false
                }
                if (on_delete == "set_null" && !is_optional) {
                    errors := "[sql_table]: field '{fname}' @sql_on_delete=\"set_null\" requires column type Option<T> so the column can hold NULL. Either change the column type or pick a different action (cascade / restrict / no_action)."
                    return false
                }
                if (on_delete == "set_default" && field.init == null && empty(sql_default_fn)) {
                    errors := "[sql_table]: field '{fname}' @sql_on_delete=\"set_default\" requires a daslang field initializer (`{fname} : T = <literal>`) or @sql_default_fn. Otherwise SET DEFAULT has nothing to set."
                    return false
                }
            }

            // ---- DDL fragment construction ----
            var default_clause = ""
            if (!empty(sql_default_fn)) {
                default_clause = " DEFAULT {sql_default_fn}"
            } elif (field.init != null) {
                default_clause = literal_init_to_default_clause(field.init)
            }

            var references_clause = ""
            if (!empty(references)) {
                let pk_lookup = find_parent_table_and_pk(st, references)
                if (!pk_lookup.found) {
                    if (empty(pk_lookup.tab_name)) {
                        errors := "[sql_table]: field '{fname}' @sql_references=\"{references}\" — no struct named '{references}' is visible in this module."
                    } else {
                        errors := "[sql_table]: field '{fname}' @sql_references=\"{references}\" — parent struct '{references}' has no @sql_primary_key field; FK target column cannot be resolved."
                    }
                    return false
                }
                references_clause = " REFERENCES \"{pk_lookup.tab_name}\"(\"{pk_lookup.pk_col}\") ON DELETE {fk_action_to_sql(on_delete)} ON UPDATE {fk_action_to_sql(on_update)}"
            }

            fields |> push(FieldInfo(
                name = fname,
                col_name = col_name,
                is_pk = is_pk,
                is_int_pk = is_pk && (field._type.baseType == Type.tInt || field._type.baseType == Type.tInt64),
                is_optional = is_optional,
                is_unique = is_unique,
                is_computed = is_computed,
                is_stored = is_stored,
                is_json = is_json,
                is_blob = is_blob,
                computed_sql = computed_sql,
                default_clause = default_clause,
                references_clause = references_clause
            ))
        }

        // ---- @sql_column collision detection ----
        {
            var seen_cols : table<string; string>  // sql_col -> first daslang field it came from
            for (info in fields) {
                if (key_exists(seen_cols, info.col_name)) {
                    let first_owner = seen_cols[info.col_name]
                    errors := "[sql_table]: SQL column name '{info.col_name}' is claimed by both field '{first_owner}' and field '{info.name}'. @sql_column renames must produce a unique column name across the struct."
                    return false
                }
                seen_cols |> insert(info.col_name, info.name)
            }
        }

        // ---- @sql_json / @sql_blob adapter codegen (shared with [sql_view]) ----
        if (!emit_json_blob_adapters_for_struct(st, fields, "[sql_table]", errors)) return false

        // Read-side helpers are always emitted; write-side helpers are skipped for legacy=true structs
        // so the natural overload-resolution error fires when a caller tries to write to one.
        // schema_assert_exprs ride along inside _sql_table_name's body as typecheck-time concept_asserts.
        if (is_legacy) {
            // Marker so [sql_index] can reject legacy structs with a precise error.
            var fn_legacy_marker = qmacro_function("_sql_legacy_table_marker") $(typ : $t(st)) : bool {
                return true
            }
            fn_legacy_marker.flags.generated = true
            fn_legacy_marker.body |> force_at(st.at)
            compiling_module() |> add_function(fn_legacy_marker)
        }
        var fn_table_name = make_table_name_fn(st, table_name, schema_assert_exprs)
        compiling_module() |> add_function(fn_table_name)
        // DROP is read-side enough — devs may legitimately drop a legacy table during migration cleanup.
        // 2-arg form must be added BEFORE the 1-arg form so find_struct_helper_fn returns the 1-arg form.
        var fn_drop_named = make_drop_sql_named_fn(st)
        compiling_module() |> add_function(fn_drop_named)
        var fn_drop = make_drop_sql_fn(st, table_name)
        compiling_module() |> add_function(fn_drop)
        // SELECT and read-row stay so select_from(type<S>) works on legacy structs.
        var fn_select_all_named = make_select_all_sql_named_fn(st, fields)
        compiling_module() |> add_function(fn_select_all_named)
        var fn_select_all = make_select_all_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_select_all)
        var fn_read = make_read_row_fn(st, fields)
        compiling_module() |> add_function(fn_read)
        var fn_col_info = make_column_info_fn(st, fields)
        compiling_module() |> add_function(fn_col_info)

        // ---- Write-side helpers (skipped for [sql_table(legacy=true)]) ----
        if (!is_legacy) {
            var fn_create_named = make_create_table_sql_named_fn(st, fields)
            compiling_module() |> add_function(fn_create_named)
            var fn_create = make_create_table_sql_fn(st, table_name, fields)
            compiling_module() |> add_function(fn_create)
            // 2-arg form must be added BEFORE the 1-arg form so find_struct_helper_fn (used by [sql_index]
            // finish-pass) returns the 1-arg form whose body owns the burned-in SQL.
            var fn_indexes_named = make_create_indexes_sql_named_fn(st, table_name)
            compiling_module() |> add_function(fn_indexes_named)
            // Empty `_sql_create_indexes_sql` placeholder; [sql_index] siblings accumulate into it.
            var fn_indexes = make_create_indexes_sql_fn(st, "")
            compiling_module() |> add_function(fn_indexes)
            var fn_ins_pk_named = make_insert_sql_named_fn(st, fields, true)
            compiling_module() |> add_function(fn_ins_pk_named)
            var fn_ins_pk = make_insert_sql_fn(st, table_name, fields, true)
            compiling_module() |> add_function(fn_ins_pk)
            var fn_ins_nopk_named = make_insert_sql_named_fn(st, fields, false)
            compiling_module() |> add_function(fn_ins_nopk_named)
            var fn_ins_nopk = make_insert_sql_fn(st, table_name, fields, false)
            compiling_module() |> add_function(fn_ins_nopk)
            var fn_pk_unset = make_pk_is_unset_fn(st, fields)
            compiling_module() |> add_function(fn_pk_unset)
            var fn_bind = make_bind_row_fn(st, fields)
            compiling_module() |> add_function(fn_bind)
            var fn_upd_pk_named = make_update_by_pk_sql_named_fn(st, fields)
            compiling_module() |> add_function(fn_upd_pk_named)
            var fn_upd_pk = make_update_by_pk_sql_fn(st, table_name, fields)
            compiling_module() |> add_function(fn_upd_pk)
            var fn_del_pk_named = make_delete_by_pk_sql_named_fn(st, fields)
            compiling_module() |> add_function(fn_del_pk_named)
            var fn_del_pk = make_delete_by_pk_sql_fn(st, table_name, fields)
            compiling_module() |> add_function(fn_del_pk)
            var fn_bind_upd = make_bind_row_for_update_fn(st, fields)
            compiling_module() |> add_function(fn_bind_upd)
            var fn_bind_pk = make_bind_row_pk_only_fn(st, fields)
            compiling_module() |> add_function(fn_bind_pk)
        }

        return true
    }

    def apply_schema_from(var st : StructurePtr;
                          raw_path : string;
                          tbl_name : string;
                          var assert_exprs : array<ExpressionPtr>&;
                          var errors : das_string) : bool {
        // Resolve + open the .db at compile time (read-only — won't create a stray empty file).
        // Open-failure surfaces as the macro error.
        let resolved = resolve_schema_from_path(raw_path)
        var db_result <- try_open_sqlite_readonly(resolved)
        if (db_result |> is_err) {
            errors := "[sql_table]: schema_from=\"{raw_path}\" (resolved \"{resolved}\") — open failed: {db_result |> unwrap_err}"
            return false
        }
        var inscope db <- db_result._value
        // PRAGMA table_info → column metadata.
        var cols_result <- read_pragma_table_info(db, tbl_name)
        if (cols_result |> is_err) {
            errors := "[sql_table]: schema_from=\"{raw_path}\" — {cols_result |> unwrap_err}"
            return false
        }
        let cols <- cols_result._value
        if (empty(cols)) {
            errors := "[sql_table]: schema_from=\"{raw_path}\" — table \"{tbl_name}\" has no columns or doesn't exist in this database."
            return false
        }
        // Track which schema cols are covered by hand-declared fields.
        var covered : array<bool>
        covered |> resize(length(cols))
        assert_exprs |> reserve(length(st.fields))
        // Validate hand-declared fields: name, PK, nullability, and emit SqlType concept_asserts.
        for (ufi in range(length(st.fields))) {
            let ufname = string(st.fields[ufi].name)
            var matched_col_idx = -1
            for (ci in range(length(cols))) {
                if (cols[ci].name == ufname) {
                    matched_col_idx = ci
                    break
                }
            }
            if (matched_col_idx < 0) {
                errors := "[sql_table]: schema_from=\"{raw_path}\" — hand-declared field '{ufname}' not found in table \"{tbl_name}\". Either remove the field, rename it, or update the .db schema."
                return false
            }
            covered[matched_col_idx] = true
            let col_name_s = cols[matched_col_idx].name
            let col_decl_type = cols[matched_col_idx].decl_type
            let col_not_null = cols[matched_col_idx].not_null
            let col_pk = cols[matched_col_idx].pk
            // PK contract.
            let user_is_pk = find_annotation(st.fields[ufi].annotation, "sql_primary_key", false)
            if (user_is_pk != col_pk) {
                if (user_is_pk) {
                    errors := "[sql_table]: schema_from=\"{raw_path}\" — field '{ufname}' is @sql_primary_key but column \"{col_name_s}\" in \"{tbl_name}\" is not PRIMARY KEY."
                } else {
                    errors := "[sql_table]: schema_from=\"{raw_path}\" — column \"{col_name_s}\" in \"{tbl_name}\" is PRIMARY KEY but field '{ufname}' is missing @sql_primary_key."
                }
                return false
            }
            // Nullability contract — Option<T> ↔ NULL allowed.
            // SQLite quirk: INTEGER PRIMARY KEY reports notnull=0 in pragma — treat PK as NOT NULL anyway.
            let user_is_nullable = is_option_field_type(st.fields[ufi]._type)
            let col_is_nullable = !col_not_null && !col_pk
            if (user_is_nullable != col_is_nullable) {
                if (user_is_nullable) {
                    errors := "[sql_table]: schema_from=\"{raw_path}\" — field '{ufname}' is Option<T> but column \"{col_name_s}\" in \"{tbl_name}\" is NOT NULL. Drop the Option<> wrapper."
                } else {
                    errors := "[sql_table]: schema_from=\"{raw_path}\" — column \"{col_name_s}\" in \"{tbl_name}\" allows NULL but field '{ufname}' is non-optional. Use Option<T>."
                }
                return false
            }
            // SqlType contract: static_if peeks at `_::sql_bind`'s return via `default<typedecl(...)>`.
            // Generic `sql_bind(Option<T>)` fallback recurses to inner T's adapter via `_::`.
            let expected_sql_type = affinity_to_sql_type(col_decl_type)
            let storage_label = sqlite_sql_type(expected_sql_type)
            var expected_native = ""
            if (expected_sql_type == SqlType.Integer) {
                expected_native = "int64"
            } elif (expected_sql_type == SqlType.Real) {
                expected_native = "double"
            } elif (expected_sql_type == SqlType.Text) {
                expected_native = "string"
            } else {
                expected_native = "array<uint8>"
            }
            let msg = "[sql_table] schema_from: field '{ufname}' SqlType does not match column \"{col_name_s}\" in \"{tbl_name}\" (expected {storage_label})."
            assert_exprs |> push(qmacro_expr(${
                static_if (typeinfo stripped_typename(default<typedecl(_::sql_bind(default<$t(st.fields[ufi]._type)>))>) != $v(expected_native)) {
                    concept_assert(false, $v(msg));
                }
            }))
        }
        // Synthesize fields for uncovered schema columns.
        for (ci in range(length(cols))) {
            if (covered[ci]) continue
            let col = cols[ci]
            let sql_type = affinity_to_sql_type(col.decl_type)
            // SQLite quirk: INTEGER PRIMARY KEY columns report notnull=0 in pragma_table_info because
            // `INTEGER PRIMARY KEY` aliases rowid (which has its own NULL handling). Treat PK as NOT NULL.
            let effective_not_null = col.not_null || col.pk
            var td = affinity_to_typedecl(sql_type, effective_not_null)
            if (td == null) {
                errors := "[sql_table]: schema_from=\"{raw_path}\" — column \"{col.name}\" declared type \"{col.decl_type}\" couldn't be mapped to a daslang type."
                return false
            }
            let fi = st |> add_structure_field(col.name, td, default<ExpressionPtr>)
            if (col.pk) {
                st.fields[fi].annotation |> add_annotation_argument("sql_primary_key", true)
            }
        }
        return true
    }

    def make_table_name_fn(var st : StructurePtr; table_name : string; var assert_exprs : array<ExpressionPtr>) : FunctionPtr {
        var fn = qmacro_function("_sql_table_name") $(typ : $t(st)) : string {
            $b(assert_exprs)
            return $v(table_name)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_drop_sql_fn(var st : StructurePtr; table_name : string) : FunctionPtr {
        // 1-arg wrapper forwards via `default<T>` so `typ` stays unread (avoids "type expression result is used").
        var fn = qmacro_function("_sql_drop_table_if_exists_sql") $(typ : $t(st)) : string {
            return _sql_drop_table_if_exists_sql(default<$t(st)>, $v(table_name))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_drop_sql_named_fn(var st : StructurePtr) : FunctionPtr {
        // 2-arg form: runtime ``name`` lets a caller drop a staging/aliased table identifier
        // (used by struct_convert rebuild). [unused_argument(typ)] because daslang dispatches on T,
        // but the body needs only ``name``.
        var fn = qmacro_function("_sql_drop_table_if_exists_sql") $(typ : $t(st); name : string) : string {
            return "DROP TABLE IF EXISTS " + sql_quote_id(name)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_create_indexes_sql_fn(var st : StructurePtr; indexes_sql : string) : FunctionPtr {
        //! Per-struct helper returning ``;``-joined CREATE INDEX statements for every
        //! ``[sql_index(...)]`` (or ``""`` — ``try_create_table`` short-circuits on empty).
        var fn = qmacro_function("_sql_create_indexes_sql") $(typ : $t(st)) : string {
            return $v(indexes_sql)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_create_indexes_sql_named_fn(var st : StructurePtr; table_name : string) : FunctionPtr {
        //! 2-arg form: rewrites the 1-arg SQL onto ``name``. Auto-named ``idx_<table>_<col>`` follows; user-explicit
        //! ``[sql_index(name=...)]`` is preserved.
        let orig_quoted = "\"{table_name}\""
        let orig_idx_prefix = "\"idx_{table_name}_"
        var fn = qmacro_function("_sql_create_indexes_sql") $(typ : $t(st); name : string) : string {
            let escaped_name = name |> replace("\"", "\"\"")
            return (_sql_create_indexes_sql(default<$t(st)>)
                |> replace($v(orig_quoted), "\"" + escaped_name + "\"")
                |> replace($v(orig_idx_prefix), "\"idx_" + escaped_name + "_"))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_create_table_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        // 1-arg form: delegates to the 2-arg form with the struct's own [sql_table(name=...)] identifier.
        // See make_drop_sql_fn for why we construct default<$t(st)> rather than forwarding typ.
        var fn = qmacro_function("_sql_create_table_sql") $(typ : $t(st)) : string {
            return _sql_create_table_sql(default<$t(st)>, $v(table_name))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_create_table_sql_named_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        // 2-arg form: composes CREATE TABLE DDL against runtime ``name``. Per-field SQL is baked at macro time
        // (column name, type, constraints), but the table identifier comes from the runtime parameter so a
        // struct_convert rebuild can land the new schema in a staging table.
        var write_exprs : array<ExpressionPtr>
        write_exprs |> reserve(3 * length(fields) + 4)

        // CREATE TABLE "<runtime name>" ( — name routed through sql_quote_id for `"` escaping.
        write_exprs |> push(qmacro_expr(${ writer |> write("CREATE TABLE "); }))
        write_exprs |> push(qmacro_expr(${ writer |> write(sql_quote_id(name)); }))
        write_exprs |> push(qmacro_expr(${ writer |> write("("); }))

        for (i in range(length(fields))) {
            let info = fields[i]
            let prefix = i == 0 ? "\"{info.col_name}\" " : ", \"{info.col_name}\" "
            // computed columns reject NOT NULL/DEFAULT/UNIQUE/FK; non-computed get them in DDL order.
            var suffix = ""
            if (info.is_computed) {
                let storage = info.is_stored ? "STORED" : "VIRTUAL"
                suffix = " GENERATED ALWAYS AS ({info.computed_sql}) {storage}"
            } else {
                if (info.is_pk) {
                    suffix = " PRIMARY KEY"
                } elif (!info.is_optional) {
                    suffix = " NOT NULL"
                }
                if (info.is_unique && !info.is_pk) {
                    suffix += " UNIQUE"
                }
                suffix += info.default_clause
                suffix += info.references_clause
            }
            write_exprs |> push(qmacro_expr(${ writer |> write($v(prefix)); }))
            // @sql_json/@sql_blob pin storage to TEXT/BLOB; sql_storage_type_for would fail on struct/tuple T.
            if (info.is_json) {
                write_exprs |> push(qmacro_expr(${ writer |> write("TEXT"); }))
            } elif (info.is_blob) {
                write_exprs |> push(qmacro_expr(${ writer |> write("BLOB"); }))
            } else {
                write_exprs |> push(qmacro_expr(${ writer |> write(sql_storage_type_for(type<$t(st.fields[i]._type)>)); }))
            }
            write_exprs |> push(qmacro_expr(${ writer |> write($v(suffix)); }))
        }
        write_exprs |> push(qmacro_expr(${ writer |> write(")"); }))

        var fn = qmacro_function("_sql_create_table_sql") $(typ : $t(st); name : string) : string {
            return build_string() $(var writer) {
                $b(write_exprs)
            }
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_column_info_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        //! Per-struct helper backing the user-facing ``column_info(type<T>)`` generic.
        //! ``is_nullable`` follows the daslang-side meaning (``Option<T>``-derived, not NOT-NULL).
        var push_exprs : array<ExpressionPtr>
        push_exprs |> reserve(length(fields))
        for (info, i in fields, count()) {
            let name = info.col_name
            let is_pk = info.is_pk
            let is_nullable = info.is_optional
            // Strip leading " DEFAULT " so default_expr matches PRAGMA table_info's dflt_value column.
            var default_expr = ""
            if (length(info.default_clause) > length(" DEFAULT ")) {
                default_expr = info.default_clause |> slice(length(" DEFAULT "))
            }
            if (info.is_json) {
                push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
                    name = $v(name),
                    data_type = SqlType.Text,
                    is_pk = $v(is_pk),
                    is_nullable = $v(is_nullable),
                    default_expr = $v(default_expr))); }))
            } elif (info.is_blob) {
                push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
                    name = $v(name),
                    data_type = SqlType.Blob,
                    is_pk = $v(is_pk),
                    is_nullable = $v(is_nullable),
                    default_expr = $v(default_expr))); }))
            } else {
                push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
                    name = $v(name),
                    data_type = sql_storage_enum_for(type<$t(st.fields[i]._type)>),
                    is_pk = $v(is_pk),
                    is_nullable = $v(is_nullable),
                    default_expr = $v(default_expr))); }))
            }
        }
        var fn = qmacro_function("_sql_column_info") $(typ : $t(st)) : array<ColumnInfo> {
            var result : array<ColumnInfo>
            $b(push_exprs)
            return <- result
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_insert_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>; include_pk : bool) : FunctionPtr {
        // 1-arg form: delegates to the same-named 2-arg form. The 1-arg call site historically takes
        // either an instance OR `type<T>`; the wrapper must not READ typ or the type-witness path errors.
        let helper_name = include_pk ? "_sql_insert_with_pk_sql" : "_sql_insert_no_pk_sql"
        var fn = qmacro_function(helper_name) $(typ : $t(st)) : string {
            return $c(helper_name)(default<$t(st)>, $v(table_name))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_insert_sql_named_fn(var st : StructurePtr; fields : array<FieldInfo>; include_pk : bool) : FunctionPtr {
        // 2-arg form: composes the INSERT with runtime ``name``. The cols-list / placeholders chunk is
        // burned at macro time; the table identifier is the runtime parameter so a struct_convert rebuild
        // can target a staging table.
        let helper_name = include_pk ? "_sql_insert_with_pk_sql" : "_sql_insert_no_pk_sql"
        let cols = build_string() $(var w) {
            var first = true
            for (info in fields) {
                if (info.is_computed || (!include_pk && info.is_pk)) continue
                if (!first) {
                    w |> write(", ")
                }
                w |> write("\"")
                w |> write(info.col_name)
                w |> write("\"")
                first = false
            }
        }
        let placeholders = build_string() $(var w) {
            var first = true
            for (info in fields) {
                if (info.is_computed || (!include_pk && info.is_pk)) continue
                if (!first) {
                    w |> write(", ")
                }
                w |> write("?")
                first = false
            }
        }
        // Two-tail SQL: with explicit column list, or DEFAULT VALUES if every column is computed/excluded.
        // name routed through sql_quote_id for `"` escaping; suffix carries the post-identifier tail only.
        let suffix = (empty(cols)
            ? " DEFAULT VALUES"
            : " ({cols}) VALUES ({placeholders})")
        var fn = qmacro_function(helper_name) $(typ : $t(st); name : string) : string {
            return "INSERT INTO " + sql_quote_id(name) + $v(suffix)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_pk_is_unset_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        var pk_field_index = -1
        for (i in range(length(fields))) {
            if (fields[i].is_int_pk) {
                pk_field_index = i
                break
            }
        }
        if (pk_field_index >= 0) {
            let pk_name = fields[pk_field_index].name
            var fn = qmacro_function("_sql_pk_is_unset") $(row : $t(st)) : bool {
                return row.$f(pk_name) == default<$t(st.fields[pk_field_index]._type)>
            }
            fn.flags.generated = true
            fn.body |> force_at(st.at)
            return <- fn
        } else {
            var fn = qmacro_function("_sql_pk_is_unset") $(row : $t(st)) : bool {
                return false
            }
            fn.flags.generated = true
            fn.body |> force_at(st.at)
            return <- fn
        }
    }

    def make_bind_row_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        // Bind index counts bindable (non-computed) columns, NOT field position.
        var with_pk_exprs : array<ExpressionPtr>
        var no_pk_exprs : array<ExpressionPtr>
        with_pk_exprs |> reserve(length(fields))
        no_pk_exprs |> reserve(length(fields))

        var with_pk_idx = 0
        for (info in fields) {
            if (info.is_computed) continue
            with_pk_idx++
            let wi = with_pk_idx
            let field_name = info.name
            with_pk_exprs |> push(qmacro_expr(${
                sql_bind_to_stmt(stmt, $v(wi), row.$f(field_name));
            }))
        }

        var no_pk_idx = 0
        for (info in fields) {
            if (info.is_pk || info.is_computed) continue
            no_pk_idx++
            let field_name = info.name
            no_pk_exprs |> push(qmacro_expr(${
                sql_bind_to_stmt(stmt, $v(no_pk_idx), row.$f(field_name));
            }))
        }

        var fn = qmacro_function("_sql_bind_row") $(var stmt : sqlite3_stmt?; row : $t(st); include_pk : bool) : void {
            if (include_pk) {
                $b(with_pk_exprs)
            } else {
                $b(no_pk_exprs)
            }
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_select_all_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        // 1-arg form: delegates to the 2-arg form. See make_drop_sql_fn for the default<$t(st)> rationale.
        var fn = qmacro_function("_sql_select_all_sql") $(typ : $t(st)) : string {
            return _sql_select_all_sql(default<$t(st)>, $v(table_name))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_select_all_sql_named_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        // 2-arg form: column list is baked at macro time; table identifier is the runtime parameter.
        let col_list = build_string() $(var w) {
            for (i in range(length(fields))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write("\"")
                w |> write(fields[i].col_name)
                w |> write("\"")
            }
        }
        let prefix = "SELECT {col_list} FROM "
        var fn = qmacro_function("_sql_select_all_sql") $(typ : $t(st); name : string) : string {
            return $v(prefix) + sql_quote_id(name)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_read_row_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        var assign_exprs : array<ExpressionPtr>
        assign_exprs |> reserve(length(fields))

        for (i in range(length(fields))) {
            let info = fields[i]
            let col_idx = i
            let field_name = info.name
            assign_exprs |> push(qmacro_expr(${
                static_if (typeinfo can_copy(default<$t(st.fields[i]._type)>)) {
                    row.$f(field_name) = read_via_adapter(stmt, $v(col_idx), type<$t(st.fields[i]._type)>);
                } else {
                    var field_tmp <- clone_to_move(read_via_adapter(stmt, $v(col_idx), type<$t(st.fields[i]._type)>));
                    row.$f(field_name) <- field_tmp;
                }
            }))
        }

        // Explicit default<>: Option<T> fields don't satisfy strict_smart_pointers' uninit check otherwise.
        var fn = qmacro_function("_sql_read_row") $(stmt : sqlite3_stmt?; typ : $t(st)) : $t(st) {
            var row = default<$t(st)>
            $b(assign_exprs)
            return <- row
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def find_pk_field(fields : array<FieldInfo>) : int {
        for (i in range(length(fields))) {
            if (fields[i].is_pk) return i
        }
        return -1
    }

    def make_update_by_pk_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        // 1-arg form: delegates to the 2-arg form. See make_drop_sql_fn for the default<$t(st)> rationale.
        // The PK-less / no-SET-columns runtime-panic stubs ride inside the 2-arg form.
        var fn = qmacro_function("_sql_update_by_pk_sql") $(typ : $t(st)) : string {
            return _sql_update_by_pk_sql(default<$t(st)>, $v(table_name))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_update_by_pk_sql_named_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        // 2-arg form: composes the UPDATE with runtime ``name``. SET-clause is baked at macro time.
        // PK-less structs get a runtime-panic stub (concept_assert would fire on every [sql_table] decl).
        let pk_idx = find_pk_field(fields)
        if (pk_idx < 0) {
            let st_name = string(st.name)
            let msg = "[sql_table] {st_name}: update(row) / delete_(row) / delete_by_id are unavailable — no @sql_primary_key field. Use the macro forms `_sql_update(type<{st_name}>, where, set)` / `_sql_delete(type<{st_name}>, where)` instead."
            var fn = qmacro_function("_sql_update_by_pk_sql") $(typ : $t(st); name : string) : string {
                panic($v(msg))
                return ""
            }
            fn.flags.generated = true
            fn.body |> force_at(st.at)
            return <- fn
        }
        let pk_col = fields[pk_idx].col_name
        let set_clause = build_string() $(var w) {
            var first = true
            for (info in fields) {
                if (info.is_pk || info.is_computed) continue
                if (!first) {
                    w |> write(", ")
                }
                w |> write("\"")
                w |> write(info.col_name)
                w |> write("\" = ?")
                first = false
            }
        }
        // No SET columns -> `UPDATE T SET WHERE pk=?` is invalid SQL; emit a runtime-panic stub.
        if (empty(set_clause)) {
            let st_name = string(st.name)
            let msg = "[sql_table] {st_name}: update(row) has nothing to SET — every non-PK column is missing or @sql_computed. Add at least one writable non-PK column, or use raw `exec`."
            var fn = qmacro_function("_sql_update_by_pk_sql") $(typ : $t(st); name : string) : string {
                panic($v(msg))
                return ""
            }
            fn.flags.generated = true
            fn.body |> force_at(st.at)
            return <- fn
        }
        let prefix = "UPDATE "
        let suffix = " SET {set_clause} WHERE \"{pk_col}\" = ?"
        var fn = qmacro_function("_sql_update_by_pk_sql") $(typ : $t(st); name : string) : string {
            return $v(prefix) + sql_quote_id(name) + $v(suffix)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_delete_by_pk_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        // 1-arg form: delegates to the 2-arg form. See make_drop_sql_fn for the default<$t(st)> rationale.
        var fn = qmacro_function("_sql_delete_by_pk_sql") $(typ : $t(st)) : string {
            return _sql_delete_by_pk_sql(default<$t(st)>, $v(table_name))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_delete_by_pk_sql_named_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        // 2-arg form: composes the DELETE with runtime ``name``. WHERE-clause + PK column baked at macro time.
        let pk_idx = find_pk_field(fields)
        if (pk_idx < 0) {
            let st_name = string(st.name)
            let msg = "[sql_table] {st_name}: delete_(row) / delete_by_id require a @sql_primary_key field. Use `_sql_delete(type<{st_name}>, where)` instead."
            var fn = qmacro_function("_sql_delete_by_pk_sql") $(typ : $t(st); name : string) : string {
                panic($v(msg))
                return ""
            }
            fn.flags.generated = true
            fn.body |> force_at(st.at)
            return <- fn
        }
        let pk_col = fields[pk_idx].col_name
        let prefix = "DELETE FROM "
        let suffix = " WHERE \"{pk_col}\" = ?"
        var fn = qmacro_function("_sql_delete_by_pk_sql") $(typ : $t(st); name : string) : string {
            return $v(prefix) + sql_quote_id(name) + $v(suffix)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_bind_row_for_update_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        // SET-then-WHERE: non-PK cols at indices 1..N, PK at N+1 (mirrors _sql_update_by_pk_sql).
        let pk_idx = find_pk_field(fields)
        if (pk_idx < 0) {
            var fn = qmacro_function("_sql_bind_row_for_update") $(var stmt : sqlite3_stmt?; row : $t(st)) : void {
                pass
            }
            fn.flags.generated = true
            fn.body |> force_at(st.at)
            return <- fn
        }

        var bind_exprs : array<ExpressionPtr>
        var bind_idx = 0
        // Non-PK, non-computed columns first.
        for (field in fields) {
            if (field.is_pk || field.is_computed) continue
            bind_idx++
            let bi = bind_idx
            let field_name = field.name
            bind_exprs |> push(qmacro_expr(${
                sql_bind_to_stmt(stmt, $v(bi), row.$f(field_name));
            }))
        }
        // PK column last (WHERE clause).
        bind_idx++
        let pk_bind_idx = bind_idx
        let pk_field_name = fields[pk_idx].name
        bind_exprs |> push(qmacro_expr(${
            sql_bind_to_stmt(stmt, $v(pk_bind_idx), row.$f(pk_field_name));
        }))

        var fn = qmacro_function("_sql_bind_row_for_update") $(var stmt : sqlite3_stmt?; row : $t(st)) : void {
            $b(bind_exprs)
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_bind_row_pk_only_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        // Binds the PK at index 1 for `DELETE FROM T WHERE pk=?`; no-op stub for PK-less.
        let pk_idx = find_pk_field(fields)
        if (pk_idx < 0) {
            var fn = qmacro_function("_sql_bind_row_pk_only") $(var stmt : sqlite3_stmt?; row : $t(st)) : void {
                pass
            }
            fn.flags.generated = true
            fn.body |> force_at(st.at)
            return <- fn
        }
        let pk_field_name = fields[pk_idx].name
        var fn = qmacro_function("_sql_bind_row_pk_only") $(var stmt : sqlite3_stmt?; row : $t(st)) : void {
            sql_bind_to_stmt(stmt, 1, row.$f(pk_field_name))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }

}

// ===== @sql_json adapter codegen =====

def private make_json_sql_bind_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
    // Parser marks struct/tuple params const; clone + set constant so generic resolution picks this overload.
    var t1 <- clone_type(field_type)
    t1.flags.constant = true
    var fn = qmacro_function("sql_bind") $(v : $t(t1)) : string {
        return _::write_json(_::JV(v))
    }
    fn.body |> force_at(at)
    return <- fn
}

def private make_json_sql_extract_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
    var t1 <- clone_type(field_type)
    t1.flags.constant = true
    var t2 <- clone_type(field_type)
    var t3 <- clone_type(field_type)
    // Panic on malformed JSON (mirrors @sql_blob); else schema drift surfaces as silent zero rows.
    var fn = qmacro_function("sql_extract") $(v : string; t : type<$t(t1)>) : $t(t2) {
        var err = ""
        let jv = _::read_json(v, err)
        if (err != "") {
            panic("sql_extract: malformed JSON in column data: {err}")
        }
        return _::from_JV(jv, default<$t(t3)>)
    }
    fn.body |> force_at(at)
    return <- fn
}

// ===== @sql_blob adapter codegen =====

def private make_blob_sql_bind_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
    var t1 <- clone_type(field_type)
    t1.flags.constant = true
    var fn = qmacro_function("sql_bind") $(v : $t(t1)) : array<uint8> {
        var v_local <- clone_to_move(v)
        return <- _::mem_archive_save(v_local)
    }
    fn.body |> force_at(at)
    return <- fn
}

def private make_blob_sql_extract_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
    var t1 <- clone_type(field_type)
    t1.flags.constant = true
    var t2 <- clone_type(field_type)
    var t3 <- clone_type(field_type)
    var fn = qmacro_function("sql_extract") $(v : array<uint8>; t : type<$t(t1)>) : $t(t2) {
        var r = default<$t(t3)>
        var v_local <- clone_to_move(v)
        _::mem_archive_load(v_local, r)
        return <- r
    }
    fn.body |> force_at(at)
    return <- fn
}

// Classifies an existing `sql_bind(T)` overload's return-type shape for @sql_json / @sql_blob dedup.
enum private SqlAdapterKind {
    None
    Json
    Blob
    Other
}

def private classify_sql_bind_return(td : TypeDeclPtr) : SqlAdapterKind {
    if (td == null) return SqlAdapterKind.None
    if (td.baseType == Type.tString) return SqlAdapterKind.Json
    if (td.baseType == Type.tArray && td.firstType != null && td.firstType.baseType == Type.tUInt8) return SqlAdapterKind.Blob
    return SqlAdapterKind.Other
}

def private find_sql_bind_kind_for_type(mod : Module?; td : TypeDeclPtr) : SqlAdapterKind {
    if (td == null) return SqlAdapterKind.None
    var kind = SqlAdapterKind.None
    for_each_function(mod, "sql_bind") $(var fn) {
        if (!empty(fn.arguments)
                && fn.arguments[0]._type != null
                && is_same_type(td, fn.arguments[0]._type, RefMatters.no, ConstMatters.no, TemporaryMatters.no)) {
            kind = classify_sql_bind_return(fn.result)
        }
    }
    return kind
}

def private emit_json_blob_adapters_for_struct(var st : StructurePtr; fields : array<FieldInfo>; tag : string; var errors : das_string) : bool {
    var seen_kinds : table<string; SqlAdapterKind>
    for (i in range(length(fields))) {
        let info = fields[i]
        if (!info.is_json && !info.is_blob) continue
        // Unwrap: runtime sql_bind_to_stmt(Option<T>) strips Option before calling sql_bind, so adapter must bind bare T.
        let ft = unwrap_option_payload_type(st.fields[i]._type)
        if (ft == null) continue
        let key = describe(ft)
        let expected_kind = info.is_json ? SqlAdapterKind.Json : SqlAdapterKind.Blob
        let kind_label = info.is_json ? "@sql_json" : "@sql_blob"
        let prev_kind = seen_kinds?[key] ?? SqlAdapterKind.None
        if (prev_kind != SqlAdapterKind.None) {
            if (prev_kind != expected_kind) {
                let prev_label = prev_kind == SqlAdapterKind.Json ? "@sql_json" : "@sql_blob"
                errors := "{tag}: field '{info.name}' is {kind_label}, but an earlier field of struct '{st.name}' already used type '{key}' with {prev_label}. Adapter dedup is per-(T, kind); the same T must be used with one storage kind across this struct. Pick one kind, or wrap '{key}' in a typedef so the two storage views are distinct types."
                return false
            }
            continue
        }
        seen_kinds |> insert(key, expected_kind)
        let existing_kind = find_sql_bind_kind_for_type(compiling_module(), ft)
        if (existing_kind != SqlAdapterKind.None) {
            if (existing_kind != expected_kind) {
                let existing_label = existing_kind == SqlAdapterKind.Json ? "@sql_json (sql_bind returns string)" : (existing_kind == SqlAdapterKind.Blob ? "@sql_blob (sql_bind returns array<uint8>)" : "an unrelated `sql_bind` overload")
                errors := "{tag}: field '{info.name}' is {kind_label}, but type '{key}' is already bound by {existing_label} (from a sibling annotation or a hand-written adapter). Adapter dedup is per-(T, kind); the same T must be used with one storage kind. Pick one kind, or wrap '{key}' in a typedef so the two storage views are distinct types."
                return false
            }
            continue
        }
        if (info.is_json) {
            var fn_b = make_json_sql_bind_fn(ft, st.at)
            compiling_module() |> add_function(fn_b)
            var fn_e = make_json_sql_extract_fn(ft, st.at)
            compiling_module() |> add_function(fn_e)
        } else {
            var fn_b = make_blob_sql_bind_fn(ft, st.at)
            compiling_module() |> add_function(fn_b)
            var fn_e = make_blob_sql_extract_fn(ft, st.at)
            compiling_module() |> add_function(fn_e)
        }
    }
    return true
}

// ===== [sql_view] — read-only struct view backed by a SQLite VIEW =====

[structure_macro(name=sql_view)]
class private SqlViewMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let view_name = find_annotation(args, "name", string(st.name))
        // Reject sibling [sql_index] up front so the error points at the macro stack, not a per-field follow-on.
        for (annDecl in st.annotations) {
            if (annDecl.annotation.name == "sql_index") {
                errors := "[sql_view] on struct '{st.name}': [sql_index] cannot apply to a view — views are read-only and inherit indexing from the underlying table. Move the index to the underlying table's struct."
                return false
            }
        }
        // ---- per-field validation: views accept only @sql_column / @sql_json / @sql_blob ----
        var fields : array<FieldInfo>
        fields |> reserve(length(st.fields))
        for (field in st.fields) {
            let fname = string(field.name)
            if (find_annotation(field.annotation, "sql_primary_key", false)) {
                errors := "[sql_view]: field '{fname}' has @sql_primary_key — views are read-only and cannot declare keys; primary keys belong on the underlying table."
                return false
            }
            if (find_annotation(field.annotation, "sql_unique", false)) {
                errors := "[sql_view]: field '{fname}' has @sql_unique — uniqueness constraints belong on the underlying table, not the view."
                return false
            }
            let computed_sql = find_annotation(field.annotation, "sql_computed", "")
            if (!empty(computed_sql)) {
                errors := "[sql_view]: field '{fname}' has @sql_computed — view bodies are arbitrary SELECT expressions (write the expression directly inside `_create_view`'s chain), not GENERATED ALWAYS columns."
                return false
            }
            if (find_annotation(field.annotation, "sql_stored", false)) {
                errors := "[sql_view]: field '{fname}' has @sql_stored — only @sql_computed columns can be STORED, and views don't have @sql_computed. Remove the annotation."
                return false
            }
            if (!empty(find_annotation(field.annotation, "sql_default_fn", ""))) {
                errors := "[sql_view]: field '{fname}' has @sql_default_fn — defaults belong on the underlying table, not the view."
                return false
            }
            if (field.init != null) {
                errors := "[sql_view]: field '{fname}' has a daslang field initializer — view fields are read-only projections of the SELECT body. Remove the initializer."
                return false
            }
            if (!empty(find_annotation(field.annotation, "sql_references", ""))) {
                errors := "[sql_view]: field '{fname}' has @sql_references — foreign keys belong on the underlying table, not the view."
                return false
            }
            if (!empty(find_annotation(field.annotation, "sql_on_delete", ""))) {
                errors := "[sql_view]: field '{fname}' has @sql_on_delete — FK actions belong on the underlying table, not the view."
                return false
            }
            if (!empty(find_annotation(field.annotation, "sql_on_update", ""))) {
                errors := "[sql_view]: field '{fname}' has @sql_on_update — FK actions belong on the underlying table, not the view."
                return false
            }
            let is_optional = is_option_field_type(field._type)
            let is_json = find_annotation(field.annotation, "sql_json", false)
            let is_blob = find_annotation(field.annotation, "sql_blob", false)
            if (is_json && is_blob) {
                errors := "[sql_view]: field '{fname}' has both @sql_json and @sql_blob — pick one. Use @sql_json for queryable, schema-tolerant TEXT storage; use @sql_blob for opaque binary storage."
                return false
            }
            // @sql_column rename, same rules as [sql_table].
            var col_name = fname
            let col_anno = find_arg(field.annotation, "sql_column")
            if (col_anno is tString) {
                let cv = string(col_anno as tString)
                if (empty(cv)) {
                    errors := "[sql_view]: field '{fname}' @sql_column = \"\" is invalid — provide a non-empty SQL column name."
                    return false
                }
                if (find(cv, "\"") >= 0 || find(cv, "\\") >= 0) {
                    errors := "[sql_view]: field '{fname}' @sql_column=\"{cv}\" — embedded `\"` / `\\` are rejected (SQL identifier hygiene). Use a plain identifier."
                    return false
                }
                col_name = cv
            }
            fields |> push(FieldInfo(
                name = fname,
                col_name = col_name,
                is_pk = false,
                is_int_pk = false,
                is_optional = is_optional,
                is_unique = false,
                is_computed = false,
                is_stored = false,
                is_json = is_json,
                is_blob = is_blob,
                computed_sql = "",
                default_clause = "",
                references_clause = ""))
        }
        // ---- @sql_column collision detection ----
        {
            var seen_cols : table<string; string>
            for (info in fields) {
                if (key_exists(seen_cols, info.col_name)) {
                    let first_owner = seen_cols[info.col_name]
                    errors := "[sql_view]: SQL column name '{info.col_name}' is claimed by both field '{first_owner}' and field '{info.name}'. @sql_column renames must produce a unique column name across the struct."
                    return false
                }
                seen_cols |> insert(info.col_name, info.name)
            }
        }
        // ---- @sql_json / @sql_blob adapter codegen ----
        if (!emit_json_blob_adapters_for_struct(st, fields, "[sql_view]", errors)) return false
        // ---- read-path helpers (same names as [sql_table]) ----
        var fn_table_name = make_view_table_name_fn(st, view_name)
        compiling_module() |> add_function(fn_table_name)
        var fn_drop = make_view_drop_sql_fn(st, view_name)
        compiling_module() |> add_function(fn_drop)
        var fn_select_all = make_view_select_all_sql_fn(st, view_name, fields)
        compiling_module() |> add_function(fn_select_all)
        var fn_read = make_read_row_helper_fn(st, fields)
        compiling_module() |> add_function(fn_read)
        var fn_col_info = make_view_column_info_fn(st, fields)
        compiling_module() |> add_function(fn_col_info)
        // ---- mutation stubs (panic at runtime with a clear message) ----
        let st_name = string(st.name)
        var fn_ins_pk = make_view_string_panic_stub_fn(st, "_sql_insert_with_pk_sql", st_name, "insert")
        compiling_module() |> add_function(fn_ins_pk)
        var fn_ins_nopk = make_view_string_panic_stub_fn(st, "_sql_insert_no_pk_sql", st_name, "insert")
        compiling_module() |> add_function(fn_ins_nopk)
        var fn_upd_pk = make_view_string_panic_stub_fn(st, "_sql_update_by_pk_sql", st_name, "update")
        compiling_module() |> add_function(fn_upd_pk)
        var fn_del_pk = make_view_string_panic_stub_fn(st, "_sql_delete_by_pk_sql", st_name, "delete")
        compiling_module() |> add_function(fn_del_pk)
        var fn_pk_unset = make_pk_is_unset_returns_false_fn(st)
        compiling_module() |> add_function(fn_pk_unset)
        var fn_bind = make_view_bind_row_panic_stub_fn(st, "_sql_bind_row", st_name, "insert", true)
        compiling_module() |> add_function(fn_bind)
        var fn_bind_upd = make_view_bind_row_panic_stub_fn(st, "_sql_bind_row_for_update", st_name, "update", false)
        compiling_module() |> add_function(fn_bind_upd)
        var fn_bind_pk = make_view_bind_row_panic_stub_fn(st, "_sql_bind_row_pk_only", st_name, "delete", false)
        compiling_module() |> add_function(fn_bind_pk)
        return true
    }
}

// ===== Shared helper-function builders (used by [sql_view] and [sql_fts5]) =====
// Factored out because [sql_view]/[sql_fts5] generate identical qmacro_function shells.

def private make_const_string_helper_fn(var st : StructurePtr; helper_name, val : string) : FunctionPtr {
    var fn = qmacro_function(helper_name) $(typ : $t(st)) : string {
        return $v(val)
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_pk_is_unset_returns_false_fn(var st : StructurePtr) : FunctionPtr {
    var fn = qmacro_function("_sql_pk_is_unset") $(row : $t(st)) : bool {
        return false
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_string_panic_stub_helper_fn(var st : StructurePtr; helper_name, msg : string) : FunctionPtr {
    var fn = qmacro_function(helper_name) $(typ : $t(st)) : string {
        panic($v(msg))
        return ""
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_void_panic_stub_helper_fn(var st : StructurePtr; helper_name, msg : string;
                                           has_include_pk : bool) : FunctionPtr {
    if (has_include_pk) {
        var fn = qmacro_function(helper_name) $(var stmt : sqlite3_stmt?; row : $t(st); include_pk : bool) : void {
            panic($v(msg))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    } else {
        var fn = qmacro_function(helper_name) $(var stmt : sqlite3_stmt?; row : $t(st)) : void {
            panic($v(msg))
        }
        fn.flags.generated = true
        fn.body |> force_at(st.at)
        return <- fn
    }
}

def private make_view_table_name_fn(var st : StructurePtr; view_name : string) : FunctionPtr {
    return <- make_const_string_helper_fn(st, "_sql_table_name", view_name)
}

def private make_view_drop_sql_fn(var st : StructurePtr; view_name : string) : FunctionPtr {
    return <- make_const_string_helper_fn(st, "_sql_drop_view_if_exists_sql",
                                          "DROP VIEW IF EXISTS \"{view_name}\"")
}

def private make_view_select_all_sql_fn(var st : StructurePtr; view_name : string; fields : array<FieldInfo>) : FunctionPtr {
    let col_list = build_string() $(var w) {
        for (i in range(length(fields))) {
            if (i > 0) {
                w |> write(", ")
            }
            w |> write("\"")
            w |> write(fields[i].col_name)
            w |> write("\"")
        }
    }
    let sql = "SELECT {col_list} FROM \"{view_name}\""
    var fn = qmacro_function("_sql_select_all_sql") $(typ : $t(st)) : string {
        return $v(sql)
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_read_row_helper_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
    // Per-struct _sql_read_row: column index = field index in declaration order.
    var assign_exprs : array<ExpressionPtr>
    assign_exprs |> reserve(length(fields))
    for (i in range(length(fields))) {
        let info = fields[i]
        let col_idx = i
        let field_name = info.name
        assign_exprs |> push(qmacro_expr(${
            static_if (typeinfo can_copy(default<$t(st.fields[i]._type)>)) {
                row.$f(field_name) = read_via_adapter(stmt, $v(col_idx), type<$t(st.fields[i]._type)>);
            } else {
                var field_tmp <- clone_to_move(read_via_adapter(stmt, $v(col_idx), type<$t(st.fields[i]._type)>));
                row.$f(field_name) <- field_tmp;
            }
        }))
    }
    var fn = qmacro_function("_sql_read_row") $(stmt : sqlite3_stmt?; typ : $t(st)) : $t(st) {
        var row = default<$t(st)>
        $b(assign_exprs)
        return <- row
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_column_info_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
    var push_exprs : array<ExpressionPtr>
    push_exprs |> reserve(length(fields))
    for (info, i in fields, count()) {
        let name = info.col_name
        let is_nullable = info.is_optional
        if (info.is_json) {
            push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
                name = $v(name),
                data_type = SqlType.Text,
                is_pk = false,
                is_nullable = $v(is_nullable),
                default_expr = "")); }))
        } elif (info.is_blob) {
            push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
                name = $v(name),
                data_type = SqlType.Blob,
                is_pk = false,
                is_nullable = $v(is_nullable),
                default_expr = "")); }))
        } else {
            push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
                name = $v(name),
                data_type = sql_storage_enum_for(type<$t(st.fields[i]._type)>),
                is_pk = false,
                is_nullable = $v(is_nullable),
                default_expr = "")); }))
        }
    }
    var fn = qmacro_function("_sql_column_info") $(typ : $t(st)) : array<ColumnInfo> {
        var result : array<ColumnInfo>
        $b(push_exprs)
        return <- result
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_string_panic_stub_fn(var st : StructurePtr; helper_name : string;
                                           st_name : string; verb : string) : FunctionPtr {
    let msg = "[sql_view] {st_name}: cannot {verb} into a view — views are read-only. Mutate the underlying table(s) and re-query the view."
    return <- make_string_panic_stub_helper_fn(st, helper_name, msg)
}

def private make_view_bind_row_panic_stub_fn(var st : StructurePtr; helper_name : string;
                                             st_name : string; verb : string;
                                             has_include_pk : bool) : FunctionPtr {
    let msg = "[sql_view] {st_name}: cannot {verb} into a view — views are read-only. Mutate the underlying table(s) and re-query the view."
    return <- make_void_panic_stub_helper_fn(st, helper_name, msg, has_include_pk)
}

// ===== [sql_fts5] — FTS5 virtual-table struct (tut 40) =====

struct private FtsRejectedAnnotation {
    name    : string
    is_bool : bool
    why     : string
}

let private FTS5_REJECTED_ANNOTATIONS = fixed_array(
    FtsRejectedAnnotation(name = "sql_primary_key", is_bool = true,
        why = "FTS5 virtual tables don't take user PRIMARY KEYs (rowid is implicit)."),
    FtsRejectedAnnotation(name = "sql_unique", is_bool = true,
        why = "uniqueness constraints are not supported on FTS5 virtual tables."),
    FtsRejectedAnnotation(name = "sql_computed", is_bool = false,
        why = "generated columns are not supported on FTS5 virtual tables."),
    FtsRejectedAnnotation(name = "sql_stored", is_bool = true,
        why = "generated-column storage does not apply to FTS5 virtual tables."),
    FtsRejectedAnnotation(name = "sql_default_fn", is_bool = false,
        why = "DEFAULT clauses are not supported on FTS5 virtual tables."),
    FtsRejectedAnnotation(name = "sql_references", is_bool = false,
        why = "foreign keys are not supported on FTS5 virtual tables."),
    FtsRejectedAnnotation(name = "sql_on_delete", is_bool = false,
        why = "FK actions are not supported on FTS5 virtual tables."),
    FtsRejectedAnnotation(name = "sql_on_update", is_bool = false,
        why = "FK actions are not supported on FTS5 virtual tables."),
    FtsRejectedAnnotation(name = "sql_json", is_bool = true,
        why = "FTS5 columns are tokenized text, not JSON. Drop the annotation or use a regular [sql_table]."),
    FtsRejectedAnnotation(name = "sql_blob", is_bool = true,
        why = "FTS5 columns are tokenized text, not BLOB. Drop the annotation or use a regular [sql_table]."))
// Self-contained mode only: content fields must be string/Option<string>;
// @sql_fts_rank is read-only and maps to FTS5's hidden `rank` on SELECT.

[structure_macro(name=sql_fts5)]
class private SqlFts5Macro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let table_name = find_annotation(args, "name", string(st.name))
        // ---- per-field validation: FTS5 columns are simple text ----
        var fields : array<FieldInfo>
        var rank_count = 0
        var content_count = 0
        fields |> reserve(length(st.fields))
        for (field in st.fields) {
            let fname = string(field.name)
            let is_optional = is_option_field_type(field._type)
            let is_fts_rank = find_annotation(field.annotation, "sql_fts_rank", false)
            // Reject every annotation that doesn't apply to a virtual table.
            for (rej in FTS5_REJECTED_ANNOTATIONS) {
                var present = false
                if (rej.is_bool) {
                    present = find_annotation(field.annotation, rej.name, false)
                } else {
                    present = !empty(find_annotation(field.annotation, rej.name, ""))
                }
                if (present) {
                    errors := "[sql_fts5]: field '{fname}' has @{rej.name} — {rej.why}"
                    return false
                }
            }
            // @sql_column rename — same shape as [sql_table]; rejected on @sql_fts_rank
            // (rank is hardcoded to FTS5's hidden `rank` column).
            var col_name = fname
            let col_anno = find_arg(field.annotation, "sql_column")
            if (col_anno is tString) {
                if (is_fts_rank) {
                    errors := "[sql_fts5]: field '{fname}' has both @sql_fts_rank and @sql_column — rank is hardcoded to FTS5's hidden `rank` column. Drop the @sql_column rename."
                    return false
                }
                let cv = string(col_anno as tString)
                if (empty(cv)) {
                    errors := "[sql_fts5]: field '{fname}' @sql_column = \"\" is invalid — provide a non-empty SQL column name."
                    return false
                }
                if (find(cv, "\"") >= 0 || find(cv, "\\") >= 0) {
                    errors := "[sql_fts5]: field '{fname}' @sql_column=\"{cv}\" — embedded `\"` / `\\` are rejected (SQL identifier hygiene). Use a plain identifier."
                    return false
                }
                col_name = cv
            }
            // @sql_fts_rank: read-only float column, reads FTS5's hidden `rank`. No daslang init allowed.
            if (is_fts_rank) {
                col_name = "rank"   // FTS5 hidden column — not a declared content column
                if (field.init != null) {
                    errors := "[sql_fts5]: field '{fname}' is @sql_fts_rank — read-only column populated from FTS5's hidden `rank` on SELECT. Remove the daslang field initializer."
                    return false
                }
                if (is_optional) {
                    errors := "[sql_fts5]: field '{fname}' is @sql_fts_rank — must not be Option<T>. FTS5 always materializes a relevance score on a MATCH-using SELECT."
                    return false
                }
                if (field._type.baseType != Type.tFloat && field._type.baseType != Type.tDouble) {
                    errors := "[sql_fts5]: field '{fname}' is @sql_fts_rank — must be a float / double (FTS5 BM25 score). Got '{describe(field._type)}'."
                    return false
                }
                rank_count++
            } else {
                // Content column: string or Option<string>.
                let bt = field._type.baseType
                let is_text = ((bt == Type.tString)
                            || (is_optional && field._type.firstType != null && field._type.firstType.baseType == Type.tString))
                if (!is_text) {
                    errors := "[sql_fts5]: field '{fname}' has unsupported type '{describe(field._type)}' — FTS5 content columns must be `string` or `Option<string>`. Tag the field with `@sql_fts_rank` if it's the relevance score column."
                    return false
                }
                content_count++
            }
            fields |> push(FieldInfo(
                name = fname,
                col_name = col_name,
                is_pk = false,
                is_int_pk = false,
                is_optional = is_optional,
                is_unique = false,
                is_computed = false,
                is_stored = false,
                is_json = false,
                is_blob = false,
                is_fts_rank = is_fts_rank,
                computed_sql = "",
                default_clause = "",
                references_clause = ""))
        }
        if (content_count == 0) {
            errors := "[sql_fts5] on struct '{st.name}': must declare at least one content column (`string` or `Option<string>`). A struct with only @sql_fts_rank fields has nothing to index."
            return false
        }
        if (rank_count > 1) {
            errors := "[sql_fts5] on struct '{st.name}': only one @sql_fts_rank field is allowed (FTS5 emits a single `rank` column per SELECT)."
            return false
        }
        // ---- @sql_column collision detection (content columns only — rank uses FTS5's hidden id) ----
        {
            var seen_cols : table<string; string>
            for (info in fields) {
                if (info.is_fts_rank) continue
                if (key_exists(seen_cols, info.col_name)) {
                    let first_owner = seen_cols[info.col_name]
                    errors := "[sql_fts5]: SQL column name '{info.col_name}' is claimed by both field '{first_owner}' and field '{info.name}'. @sql_column renames must produce a unique column name across the struct."
                    return false
                }
                seen_cols |> insert(info.col_name, info.name)
            }
        }
        // [sql_index] siblings would be meaningless on a virtual table — reject up front.
        for (annDecl in st.annotations) {
            if (annDecl.annotation.name == "sql_index") {
                errors := "[sql_fts5] on struct '{st.name}': sibling [sql_index] is not supported — FTS5 builds its own inverted index. Drop the [sql_index]."
                return false
            }
        }
        // ---- generated helper functions ----
        let create_sql = build_fts5_create_table_sql(table_name, fields)
        let select_all_sql = build_fts5_select_all_sql(table_name, fields)
        let insert_sql = build_fts5_insert_sql(table_name, fields)
        let st_name = string(st.name)
        let panic_msg_pk = "[sql_fts5] {st_name}: cannot {0} on an FTS5 virtual table — UPDATE/DELETE-by-PK is not supported in v1. Drop and re-INSERT, or use raw `db |> exec(\"DELETE FROM ... WHERE rowid = ?\", id)`."
        let panic_msg_bind = "[sql_fts5] {st_name}: cannot {0} on an FTS5 virtual table — not supported in v1. Use raw `db |> exec(...)` for DELETE/UPDATE."
        var fn_table_name = make_const_string_helper_fn(st, "_sql_table_name", table_name)
        compiling_module() |> add_function(fn_table_name)
        var fn_drop = make_const_string_helper_fn(st, "_sql_drop_table_if_exists_sql",
                                                  "DROP TABLE IF EXISTS \"{table_name}\"")
        compiling_module() |> add_function(fn_drop)
        var fn_create = make_const_string_helper_fn(st, "_sql_create_table_sql", create_sql)
        compiling_module() |> add_function(fn_create)
        var fn_indexes = make_const_string_helper_fn(st, "_sql_create_indexes_sql", "")
        compiling_module() |> add_function(fn_indexes)
        var fn_select_all = make_const_string_helper_fn(st, "_sql_select_all_sql", select_all_sql)
        compiling_module() |> add_function(fn_select_all)
        var fn_read = make_read_row_helper_fn(st, fields)
        compiling_module() |> add_function(fn_read)
        // FTS5 has no daslang-side PK; both insert helper variants emit the same SQL.
        var fn_ins_pk = make_const_string_helper_fn(st, "_sql_insert_with_pk_sql", insert_sql)
        compiling_module() |> add_function(fn_ins_pk)
        var fn_ins_nopk = make_const_string_helper_fn(st, "_sql_insert_no_pk_sql", insert_sql)
        compiling_module() |> add_function(fn_ins_nopk)
        var fn_pk_unset = make_pk_is_unset_returns_false_fn(st)
        compiling_module() |> add_function(fn_pk_unset)
        var fn_bind = make_fts5_bind_row_fn(st, fields)
        compiling_module() |> add_function(fn_bind)
        var fn_col_info = make_fts5_column_info_fn(st, fields)
        compiling_module() |> add_function(fn_col_info)
        // UPDATE/DELETE not supported in v1 — runtime panic stubs (compile error would be nicer but the macro expander generates the calls unconditionally for `[sql_table]`-shape APIs).
        var fn_upd_pk = make_string_panic_stub_helper_fn(st, "_sql_update_by_pk_sql",
            panic_msg_pk |> replace("{0}", "update by primary key"))
        compiling_module() |> add_function(fn_upd_pk)
        var fn_del_pk = make_string_panic_stub_helper_fn(st, "_sql_delete_by_pk_sql",
            panic_msg_pk |> replace("{0}", "delete by primary key"))
        compiling_module() |> add_function(fn_del_pk)
        var fn_bind_upd = make_void_panic_stub_helper_fn(st, "_sql_bind_row_for_update",
            panic_msg_bind |> replace("{0}", "update"), false)
        compiling_module() |> add_function(fn_bind_upd)
        var fn_bind_pk = make_void_panic_stub_helper_fn(st, "_sql_bind_row_pk_only",
            panic_msg_bind |> replace("{0}", "delete by primary key"), false)
        compiling_module() |> add_function(fn_bind_pk)
        return true
    }
}

def private build_fts5_create_table_sql(table_name : string; fields : array<FieldInfo>) : string {
    let col_list = build_string() $(var w) {
        var first = true
        for (info in fields) {
            continue if (info.is_fts_rank)   // rank is implicit in FTS5; not declared as a content column
            if (!first) {
                w |> write(", ")
            }
            w |> write("\"")
            w |> write(info.col_name)
            w |> write("\"")
            first = false
        }
    }
    return "CREATE VIRTUAL TABLE IF NOT EXISTS \"{table_name}\" USING fts5({col_list}, tokenize='unicode61')"
}

def private build_fts5_select_all_sql(table_name : string; fields : array<FieldInfo>) : string {
    let col_list = build_string() $(var w) {
        for (i in range(length(fields))) {
            if (i > 0) {
                w |> write(", ")
            }
            if (fields[i].is_fts_rank) {
                w |> write("rank")  // FTS5 hidden column; emitted unquoted
            } else {
                w |> write("\"")
                w |> write(fields[i].col_name)
                w |> write("\"")
            }
        }
    }
    return "SELECT {col_list} FROM \"{table_name}\""
}

def private build_fts5_insert_sql(table_name : string; fields : array<FieldInfo>) : string {
    let cols = build_string() $(var w) {
        var first = true
        for (info in fields) {
            if (info.is_fts_rank) continue
            if (!first) {
                w |> write(", ")
            }
            w |> write("\"")
            w |> write(info.col_name)
            w |> write("\"")
            first = false
        }
    }
    let placeholders = build_string() $(var w) {
        var first = true
        for (info in fields) {
            if (info.is_fts_rank) continue
            if (!first) {
                w |> write(", ")
            }
            w |> write("?")
            first = false
        }
    }
    return "INSERT INTO \"{table_name}\" ({cols}) VALUES ({placeholders})"
}

def private make_fts5_bind_row_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
    var no_pk_exprs : array<ExpressionPtr>
    no_pk_exprs |> reserve(length(fields))
    var idx = 0
    for (info in fields) {
        if (info.is_fts_rank) continue
        idx++
        let bind_idx = idx
        let field_name = info.name
        no_pk_exprs |> push(qmacro_expr(${
            sql_bind_to_stmt(stmt, $v(bind_idx), row.$f(field_name));
        }))
    }
    var fn = qmacro_function("_sql_bind_row") $(var stmt : sqlite3_stmt?; row : $t(st); include_pk : bool) : void {
        $b(no_pk_exprs)
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_fts5_column_info_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
    var push_exprs : array<ExpressionPtr>
    push_exprs |> reserve(length(fields))
    for (info, i in fields, count()) {
        let name = info.col_name
        let is_nullable = info.is_optional
        push_exprs |> push(qmacro_expr(${ result |> push(ColumnInfo(
            name = $v(name),
            data_type = sql_storage_enum_for(type<$t(st.fields[i]._type)>),
            is_pk = false,
            is_nullable = $v(is_nullable),
            default_expr = "")); }))
    }
    var fn = qmacro_function("_sql_column_info") $(typ : $t(st)) : array<ColumnInfo> {
        var result : array<ColumnInfo>
        $b(push_exprs)
        return <- result
    }
    fn.flags.generated = true
    fn.body |> force_at(st.at)
    return <- fn
}

// ===== register_function — bind a daslang function as a SQL scalar UDF =====

enum SqlFnTag {
    //! Type tag matching the C++-side SqlFnTag enum in dasSQLITE.userfn.cpp.
    //! Order is wire-stable; do NOT renumber without also updating the C++.
    Int    = 0
    Int64  = 1
    Float  = 2
    Double = 3
    Bool   = 4
    String = 5
}

[macro_function]
def private sql_fn_tag_for_type(td : TypeDeclPtr) : tuple<tag : uint8; ok : bool> {
    if (td == null || !empty(td.dim)) return (tag = uint8(0), ok = false)
    if (td.baseType == Type.tInt) return (tag = uint8(SqlFnTag.Int), ok = true)
    if (td.baseType == Type.tInt64) return (tag = uint8(SqlFnTag.Int64), ok = true)
    if (td.baseType == Type.tFloat) return (tag = uint8(SqlFnTag.Float), ok = true)
    if (td.baseType == Type.tDouble) return (tag = uint8(SqlFnTag.Double), ok = true)
    if (td.baseType == Type.tBool) return (tag = uint8(SqlFnTag.Bool), ok = true)
    if (td.baseType == Type.tString) return (tag = uint8(SqlFnTag.String), ok = true)
    return (tag = uint8(0), ok = false)
}

[call_macro(name="register_function")]
class private RegisterFunctionMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        let nCallArgs = call.arguments |> length
        macro_verify(nCallArgs >= 3 && nCallArgs <= 5, prog, call.at,
            "register_function expects (db, name, fn[, deterministic[, directonly]]) — got {nCallArgs} args")
        // Need the inferred function-pointer type; otherwise the inner @@fn hasn't resolved yet.
        var fnExpr = call.arguments[2]
        macro_verify(fnExpr._type != null, prog, call.at,
            "register_function: function-pointer argument is not inferred yet — make sure the symbol exists at this point")
        macro_verify(!fnExpr._type.isAutoOrAlias, prog, call.at,
            "register_function: function-pointer type is auto/alias after inference")
        macro_verify(fnExpr._type.baseType == Type.tFunction, prog, call.at,
            "register_function: third argument must be a function pointer (e.g. `@@my_fn`); got {describe(fnExpr._type)}")
        let nArgs = fnExpr._type.argTypes |> length
        macro_verify(nArgs <= 4, prog, call.at,
            "register_function: SQLite scalar functions support up to 4 arguments; got {nArgs}")
        // Derive per-arg + return tags; reject unsupported types per position.
        var tags : array<uint8>
        tags |> reserve(4)
        for (i in range(nArgs)) {
            let r = sql_fn_tag_for_type(fnExpr._type.argTypes[i])
            if (!r.ok) {
                macro_error(prog, call.at,
                    "register_function: argument #{i + 1} of fn has unsupported type '{describe(fnExpr._type.argTypes[i])}' — supported: int, int64, float, double, bool, string")
                return null
            }
            tags |> push(r.tag)
        }
        // Pad tag slots to 4: C++ side reads nArgs but the binding takes 4 scalar uint8s.
        while (tags |> length < 4) {
            tags |> push(uint8(0))
        }
        let retR = sql_fn_tag_for_type(fnExpr._type.firstType)
        if (!retR.ok) {
            macro_error(prog, call.at,
                "register_function: return type '{describe(fnExpr._type.firstType)}' is unsupported — supported: int, int64, float, double, bool, string")
            return null
        }
        var dbExpr   = clone_expression(call.arguments[0])
        var nameExpr = clone_expression(call.arguments[1])
        var fnClone  = clone_expression(fnExpr)
        // Optional positional flags. Default both to false.
        var detExpr : ExpressionPtr = nCallArgs >= 4 ? clone_expression(call.arguments[3]) : qmacro(false)
        var donExpr : ExpressionPtr = nCallArgs >= 5 ? clone_expression(call.arguments[4]) : qmacro(false)
        let nArgsLit = int(nArgs)
        let tag0 = tags[0]
        let tag1 = tags[1]
        let tag2 = tags[2]
        let tag3 = tags[3]
        let retTag = retR.tag
        // Emit: helper checks rc and panics on failure with the libsqlite3 errmsg.
        var res = qmacro(_::_register_function_check_rc(
            $e(dbExpr), $e(nameExpr), $e(fnClone), $v(nArgsLit),
            $v(tag0), $v(tag1), $v(tag2), $v(tag3),
            $v(retTag), $e(detExpr), $e(donExpr)))
        res.force_at(call.at)
        return res
    }
}

def _try_register_function_check_rc(db : SqlRunner; name : string; fn : auto(FN);
                                    nArgs : int;
                                    tag0 : uint8; tag1 : uint8; tag2 : uint8; tag3 : uint8;
                                    retTag : uint8; deterministic : bool; directonly : bool) : SqlError {
    //! Soft variant: returns ``some(errmsg)`` instead of panicking. Used by the
    //! ``[sql_function]`` install loop so ``try_open_sqlite`` can preserve its
    //! ``Result`` contract.
    let rc = sqlite3_register_function(db.sqlite_handle, name, fn, nArgs,
        tag0, tag1, tag2, tag3,
        retTag, deterministic, directonly)
    if (rc != SQLITE_OK) return some("register_function('{name}') failed: {sqlite3_errmsg(db.sqlite_handle)}")
    return none(type<string>)
}

def _register_function_check_rc(db : SqlRunner; name : string; fn : auto(FN);
                                nArgs : int;
                                tag0 : uint8; tag1 : uint8; tag2 : uint8; tag3 : uint8;
                                retTag : uint8; deterministic : bool; directonly : bool) : void {
    //! Runtime tail of the `register_function` macro. Calls the C++ trampoline-
    //! registration shim and panics with the libsqlite3 errmsg on failure.
    let err = _try_register_function_check_rc(db, name, fn, nArgs,
        tag0, tag1, tag2, tag3,
        retTag, deterministic, directonly)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def private _is_valid_sql_function_name(s : string) : bool {
    // SQLite unquoted identifier shape: [A-Za-z_][A-Za-z0-9_]* — anything else would need quoting in emitted SQL.
    if (empty(s)) return false
    var ok = true
    peek_data(s) $(arr) {
        let c0 = int(arr[0])
        if (!(is_alpha(c0) || c0 == '_')) {
            ok = false
            return
        }
        for (i in range(1, length(arr))) {
            let c = int(arr[i])
            if (!(is_alnum(c) || c == '_')) {
                ok = false
                return
            }
        }
    }
    return ok
}

// ===== [sql_function] — auto-registered SQLite scalar UDF, visible in `_sql` chains =====

struct private SqlFunctionThunk {
    name    : string
    nArgs   : int
    install : lambda<(db : SqlRunner) : SqlError>
}

// Populated by [init] thunks; replayed on every try_open_sqlite. (name, nArgs) pair detects cross-module collisions (SQLite would silently replace).
var private sql_function_registry : array<SqlFunctionThunk>

def _add_sql_function_thunk(name : string; nArgs : int;
                            var install : lambda<(db : SqlRunner) : SqlError>) : void {
    //! Public entry point used by ``[sql_function]``-generated ``[init]`` blocks
    //! to append a registration thunk to ``sql_function_registry``.
    var entry <- SqlFunctionThunk(name = name, nArgs = nArgs, install <- install)
    sql_function_registry |> emplace(entry)
}

def private _install_sql_function_registry(db : SqlRunner) : SqlError {
    var seen : table<string; bool>
    for (thunk in sql_function_registry) {
        let key = "{thunk.name}/{thunk.nArgs}"
        if (key_exists(seen, key)) return some("[sql_function] duplicate registration: '{thunk.name}' with {thunk.nArgs} args is tagged in more than one place; SQLite would silently replace the earlier binding")
        seen |> insert(key, true)
        let err = invoke(thunk.install, db)
        if (err |> is_some) return err
    }
    return none(type<string>)
}

[function_macro(name="sql_function")]
class SqlFunctionMacro : AstFunctionAnnotation {
    //! Marks a daslang function as a SQLite scalar UDF: registered against every opened
    //! connection, and emitted as ``<name>(args)`` inside ``_sql(...)`` chains rather than
    //! bound at compile time. Optional args: ``name``, ``deterministic``, ``directonly``.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (func.fromGeneric != null) {
            errors := "[sql_function] is not supported on generic functions; instantiate with concrete types and tag the concrete instance"
            return false
        }
        let nArgs = func.arguments |> length
        if (nArgs > 4) {
            errors := "[sql_function]: SQLite scalar functions support up to 4 arguments; got {nArgs}"
            return false
        }
        var tags : array<uint8>
        tags |> reserve(4)
        for (i in range(nArgs)) {
            let r = sql_fn_tag_for_type(func.arguments[i]._type)
            if (!r.ok) {
                errors := "[sql_function]: argument #{i + 1} of '{func.name}' has unsupported type '{describe(func.arguments[i]._type)}' — supported: int, int64, float, double, bool, string"
                return false
            }
            tags |> push(r.tag)
        }
        while (tags |> length < 4) {
            tags |> push(uint8(0))
        }
        let retR = sql_fn_tag_for_type(func.result)
        if (!retR.ok) {
            errors := "[sql_function]: return type '{describe(func.result)}' of '{func.name}' is unsupported — supported: int, int64, float, double, bool, string"
            return false
        }
        let funcDasName = "{func.name}"
        var sqlName = funcDasName
        let nameArg = args |> find_arg("name")
        if (nameArg is tString) {
            sqlName = nameArg as tString
            if (empty(sqlName)) {
                errors := "[sql_function]: name= cannot be empty"
                return false
            }
            if (!_is_valid_sql_function_name(sqlName)) {
                errors := "[sql_function]: name='{sqlName}' is not a valid SQL identifier (must match [A-Za-z_][A-Za-z0-9_]*)"
                return false
            }
        } elif (!(nameArg is nothing)) {
            errors := "[sql_function]: name= must be a string literal"
            return false
        }
        var deterministic = false
        let detArg = args |> find_arg("deterministic")
        if (detArg is tBool) {
            deterministic = detArg as tBool
        } elif (!(detArg is nothing)) {
            errors := "[sql_function]: deterministic= must be a bool literal (true/false)"
            return false
        }
        var directonly = false
        let donArg = args |> find_arg("directonly")
        if (donArg is tBool) {
            directonly = donArg as tBool
        } elif (!(donArg is nothing)) {
            errors := "[sql_function]: directonly= must be a bool literal (true/false)"
            return false
        }
        var reg <- setup_call_list("register`sqlite`functions", func.at, true/*isInit*/, true/*isPrivate*/)
        let nArgsLit = int(nArgs)
        let tag0 = tags[0]
        let tag1 = tags[1]
        let tag2 = tags[2]
        let tag3 = tags[3]
        let retTag = retR.tag
        // ExprAddr with explicit funcType + `__::` target — funcType alone ties on same-signature imports.
        var fnType = new TypeDecl(at = func.at, baseType = Type.tFunction)
        for (a in func.arguments) {
            fnType.argTypes |> emplace_new <| clone_type(a._type)
        }
        fnType.firstType = clone_type(func.result)
        // `__::<name>` scopes ExprAddr resolution to *this* module (where `func` lives), so
        // a same-signature import (e.g. fio_core::normalize) doesn't tie with user's `normalize`.
        // funcType-only disambiguation isn't enough when the imported symbol shares the signature.
        var fnAddr = new ExprAddr(at = func.at, target := "__::{funcDasName}", funcType <- fnType)
        reg.list |> emplace_new <| qmacro_block() {
            let thunk <- @(db : SqlRunner) : SqlError {
                return _::_try_register_function_check_rc(db, $v(sqlName), $e(fnAddr),
                    $v(nArgsLit), $v(tag0), $v(tag1), $v(tag2), $v(tag3),
                    $v(retTag), $v(deterministic), $v(directonly))
            }
            sqlite_boost::_add_sql_function_thunk($v(sqlName), $v(nArgsLit), thunk)
        }
        return true
    }
}

// ===== Typed ALTER macros — migration-context ergonomics =====

[macro_function]
def private resolve_sql_table(prog : ProgramPtr; at : LineInfo; var typeArg : ExpressionPtr;
                              macroName : string; var st : StructurePtr&; var table_name : string&) : bool {
    // T must carry [sql_table]'s _sql_table_name helper.
    if (typeArg._type == null) {
        macro_error(prog, at, "{macroName}: `type<T>` argument has no inferred type yet")
        return false
    }
    if (typeArg._type.baseType != Type.tStructure || typeArg._type.structType == null) {
        macro_error(prog, at, "{macroName}: second argument must be `type<SomeStruct>` — got '{describe(typeArg._type)}'")
        return false
    }
    st = typeArg._type.structType
    var tname_fn = find_struct_helper_fn("_sql_table_name", st)
    if (tname_fn == null) {
        macro_error(prog, at, "{macroName}: type '{st.name}' has no [sql_table] declaration; only [sql_table]-decorated structs can drive typed ALTER")
        return false
    }
    table_name = extract_string_return(tname_fn)
    if (empty(table_name)) {
        macro_error(prog, at, "{macroName}: internal — _sql_table_name body did not match expected shape on '{st.name}'")
        return false
    }
    return true
}

[macro_function]
def private resolve_field_index(var st : StructurePtr; field_name : string) : int {
    // -1 if missing. Caller emits the macro_error with full context (macro name + struct name).
    for (i in range(length(st.fields))) {
        if (st.fields[i].name == field_name) return i
    }
    return -1
}

[macro_function]
def private field_sql_name_at(var st : StructurePtr; idx : int) : string {
    // @sql_column("X") rename → X; otherwise daslang field name. idx must be in range.
    let ca = find_arg(st.fields[idx].annotation, "sql_column")
    if (ca is tString) return string(ca as tString)
    return string(st.fields[idx].name)
}

[macro_function]
def private decode_string_const(e : ExpressionPtr) : tuple<ok : bool; value : string> {
    // Strict literal-only: peel ExprRef2Value, accept ExprConstString.
    var inner = e
    if (inner != null && inner is ExprRef2Value) {
        inner = (inner as ExprRef2Value).subexpr
    }
    if (inner == null || !(inner is ExprConstString)) return (ok = false, value = "")
    return (ok = true, value = string((inner as ExprConstString).value))
}

[call_macro(name="add_column")]
class private AddColumnMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        // call.arguments = [db, type<T>, "FieldName"] (3) or [..., defaultLit] (4) after the `db |>` desugar.
        let nArgs = call.arguments |> length
        macro_verify(nArgs == 3 || nArgs == 4, prog, call.at,
            "add_column expects (db, type<T>, \"Field\" [, defaultLit]) — got {nArgs} args")
        var st : StructurePtr
        var table_name : string
        if (!resolve_sql_table(prog, call.at, call.arguments[1], "add_column", st, table_name)) return null
        // Field-name string literal.
        let nameRes = decode_string_const(call.arguments[2])
        if (!nameRes.ok) {
            macro_error(prog, call.at, "add_column: third argument must be a string literal naming the field — got '{describe(call.arguments[2])}'")
            return null
        }
        let field_name = nameRes.value
        let idx = resolve_field_index(st, field_name)
        if (idx < 0) {
            macro_error(prog, call.at, "add_column: field '{field_name}' is not a column of struct '{st.name}'")
            return null
        }
        // Reject shapes that need a table rebuild.
        let pk_arg = find_arg(st.fields[idx].annotation, "sql_primary_key")
        if (pk_arg is tBool && (pk_arg as tBool)) {
            macro_error(prog, call.at,
                "add_column cannot add a PRIMARY KEY column to an existing table; recreate the table — see chunk 14c struct_convert")
            return null
        }
        let uniq_arg = find_arg(st.fields[idx].annotation, "sql_unique")
        if (uniq_arg is tBool && (uniq_arg as tBool)) {
            macro_error(prog, call.at,
                "add_column cannot add a UNIQUE column to an existing table; create_unique_index(type<T>, \"{field_name}\") instead")
            return null
        }
        let computed_arg = find_arg(st.fields[idx].annotation, "sql_computed")
        if (computed_arg is tString || (computed_arg is tBool && (computed_arg as tBool))) {
            macro_error(prog, call.at,
                "add_column cannot add a generated/computed column to an existing table; recreate the table — see chunk 14c struct_convert")
            return null
        }
        let default_fn_arg = find_arg(st.fields[idx].annotation, "sql_default_fn")
        if (default_fn_arg is tString) {
            macro_error(prog, call.at,
                "add_column does not support @sql_default_fn fields yet; use a literal default or `db |> exec(…)`")
            return null
        }
        // Read storage hints + nullability.
        let json_arg = find_arg(st.fields[idx].annotation, "sql_json")
        let is_json = json_arg is tBool && (json_arg as tBool)
        let blob_arg = find_arg(st.fields[idx].annotation, "sql_blob")
        let is_blob = blob_arg is tBool && (blob_arg as tBool)
        let is_optional = is_option_field_type(st.fields[idx]._type)
        let col_name = field_sql_name_at(st, idx)
        // DEFAULT clause from optional 4th arg (compile-time literal only).
        var default_clause = ""
        if (nArgs == 4) {
            if (is_json || is_blob) {
                macro_error(prog, call.at,
                    "add_column: explicit default not supported for @sql_json / @sql_blob columns; the column accepts NULL by default — wrap the field in Option<T> if nullability is desired")
                return null
            }
            var defArg = call.arguments[3]
            if (defArg != null && defArg is ExprRef2Value) {
                defArg = (defArg as ExprRef2Value).subexpr
            }
            default_clause = literal_init_to_default_clause(defArg)
            if (empty(default_clause)) {
                macro_error(prog, call.at,
                    "add_column: DEFAULT must be a compile-time literal (int / int64 / float / double / bool / string); got '{describe(call.arguments[3])}'. Inline the value, or use `db |> exec(…)` for runtime expressions.")
                return null
            }
        } elif (!is_optional && !is_json && !is_blob) {
            // SQLite ADD COLUMN against existing rows: NOT NULL without DEFAULT panics for any pre-existing row.
            macro_error(prog, call.at,
                "add_column: field '{field_name}' on '{st.name}' is non-nullable; provide a literal default (e.g. `add_column(type<T>, \"{field_name}\", 0)`) or wrap the field in Option<T>")
            return null
        }
        // Build prefix + suffix at macro time; SQL type comes from the runtime adapter rail
        // (sql_storage_type_for) unless the field is explicitly @sql_json / @sql_blob.
        let null_clause = is_optional ? "" : " NOT NULL"
        let prefix = "ALTER TABLE \"{table_name}\" ADD COLUMN \"{col_name}\" "
        let suffix = "{null_clause}{default_clause}"
        var dbExpr = clone_expression(call.arguments[0])
        var res : ExpressionPtr
        if (is_json) {
            let full = "{prefix}TEXT{suffix}"
            res = qmacro(_::exec($e(dbExpr), $v(full)))
        } elif (is_blob) {
            let full = "{prefix}BLOB{suffix}"
            res = qmacro(_::exec($e(dbExpr), $v(full)))
        } else {
            var fieldType <- clone_type(st.fields[idx]._type)
            res = qmacro(_::exec($e(dbExpr), build_string() $(var w) {
                w |> write($v(prefix))
                w |> write(_::sql_storage_type_for(type<$t(fieldType)>))
                w |> write($v(suffix))
            }))
        }
        res.force_at(call.at)
        return res
    }
}

[macro_function]
def private resolve_field_columns(prog : ProgramPtr; at : LineInfo; var st : StructurePtr;
                                  fieldsArg : ExpressionPtr; macroName : string;
                                  var sql_cols : array<string>&) : bool {
    // Single string literal or a tuple of string literals. Array literals are AST-hairy and rejected.
    if (fieldsArg == null) {
        macro_error(prog, at, "{macroName}: missing fields argument")
        return false
    }
    var inner = fieldsArg
    if (inner is ExprRef2Value) {
        inner = (inner as ExprRef2Value).subexpr
    }
    var col_names : array<string>
    if (inner is ExprConstString) {
        col_names |> push(string((inner as ExprConstString).value))
    } elif (inner is ExprMakeTuple) {
        let mkt = inner as ExprMakeTuple
        for (v in mkt.values) {
            var item = v
            if (item is ExprRef2Value) {
                item = (item as ExprRef2Value).subexpr
            }
            if (item == null || !(item is ExprConstString)) {
                macro_error(prog, at, "{macroName}: tuple fields entries must be string literals — got '{describe(v)}'")
                return false
            }
            col_names |> push(string((item as ExprConstString).value))
        }
    } else {
        macro_error(prog, at, "{macroName}: fields argument must be a string literal (\"Field\") or a tuple of string literals (\"A\", \"B\") — got '{describe(fieldsArg)}'")
        return false
    }
    if (empty(col_names)) {
        macro_error(prog, at, "{macroName}: empty fields list. Provide \"Field\" for a single-column index or (\"A\", \"B\") for a composite.")
        return false
    }
    sql_cols |> reserve(length(col_names))
    for (cn in col_names) {
        let idx = resolve_field_index(st, cn)
        if (idx < 0) {
            macro_error(prog, at, "{macroName}: field '{cn}' is not a column of struct '{st.name}'")
            return false
        }
        sql_cols |> push(field_sql_name_at(st, idx))
    }
    return true
}

[macro_function]
def private emit_create_index(prog : ProgramPtr; var call : ExprCallMacro?; is_unique : bool) : ExpressionPtr {
    let macroName = is_unique ? "create_unique_index" : "create_index"
    let nArgs = call.arguments |> length
    if (nArgs < 3 || nArgs > 4) {
        macro_error(prog, call.at,
            "{macroName} expects (db, type<T>, fields [, \"name\"]) — got {nArgs} args")
        return null
    }
    var st : StructurePtr
    var table_name : string
    if (!resolve_sql_table(prog, call.at, call.arguments[1], macroName, st, table_name)) return null
    var sql_cols : array<string>
    if (!resolve_field_columns(prog, call.at, st, call.arguments[2], macroName, sql_cols)) return null
    var idx_name = derive_index_name(table_name, sql_cols)
    if (nArgs == 4) {
        let nameRes = decode_string_const(call.arguments[3])
        if (!nameRes.ok) {
            macro_error(prog, call.at,
                "{macroName}: optional `name` argument must be a string literal — got '{describe(call.arguments[3])}'")
            return null
        }
        if (empty(nameRes.value)) {
            macro_error(prog, call.at, "{macroName}: index name string is empty")
            return null
        }
        idx_name = nameRes.value
    }
    let kind = is_unique ? "UNIQUE INDEX" : "INDEX"
    let ddl = build_string() $(var w) {
        w |> write("CREATE {kind} \"{idx_name}\" ON \"{table_name}\" (")
        for (j in range(length(sql_cols))) {
            if (j > 0) {
                w |> write(", ")
            }
            w |> write("\"")
            w |> write(sql_cols[j])
            w |> write("\"")
        }
        w |> write(")")
    }
    var dbExpr = clone_expression(call.arguments[0])
    var res = qmacro(_::exec($e(dbExpr), $v(ddl)))
    res.force_at(call.at)
    return res
}

[call_macro(name="create_index")]
class private CreateIndexMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        return emit_create_index(prog, call, false)
    }
}

[call_macro(name="create_unique_index")]
class private CreateUniqueIndexMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        return emit_create_index(prog, call, true)
    }
}

def drop_index_if_exists(db : SqlRunner; name : string) {
    //! Idempotent DROP INDEX. SQLite supports IF EXISTS natively; safe to call on a
    //! missing index. Pair with ``create_unique_index`` for migration patterns that
    //! rebuild an index under a new name.
    db |> exec("DROP INDEX IF EXISTS \"{name}\"")
}
