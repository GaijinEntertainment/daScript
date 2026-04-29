options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module sqlite_boost shared public

require sqlite public

require strings public
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
    //! Binds a blob value to a prepared statement at ``index``. SQLite
    //! makes its own copy via ``SQLITE_TRANSIENT`` (the default in this
    //! wrapper), so the caller's array can release after the call.
    //! Empty blobs route through ``sqlite3_bind_zeroblob`` with ``n=0``
    //! to avoid taking ``addr(data[0])`` on a zero-length array.
    if (data |> length == 0) {
        return sqlite3_bind_zeroblob(stmt, index, 0)
    }
    unsafe {
        // Byte count = element_count * sizeof(element). The wrapper
        // accepts any array via [expect_any_array]; ``length`` alone is
        // only correct for ``array<uint8>``.
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
    //! Prepares a SQL statement for execution. Delegates to a C++ shim
    //! (``sqlite3_prepare_v2_no_tail``) that drops the unused pzTail parameter,
    //! sidestepping a daslang AOT codegen bug where ``string const?`` renders as
    //! ``char * const *`` instead of ``const char **``.
    return sqlite3_prepare_v2_no_tail(db, sql, unsafe(addr(stmt)))
}

// ===== High-level API: runner, RAII, exec, last_insert_rowid =====

struct SqlRunner {
    //! Owns a libsqlite3 connection. Closed by `finalize` on `var inscope` scope exit.
    sqlite_handle : sqlite3? = null
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
    //! Opens a connection to ``path`` (``:memory:`` for an in-memory DB).
    //! Returns ``ok(runner)`` on success, ``err(errmsg)`` on failure.
    //! On failure, the partial libsqlite3 handle is closed per the SQLite contract.
    var db = SqlRunner()
    let rc = sqlite3_open(path, db.sqlite_handle)
    if (rc != SQLITE_OK) {
        let msg = "sqlite3_open(\"{path}\") failed: {sqlite3_errmsg(db.sqlite_handle)}"
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
    //! RAII helper. Opens the DB, sets ``PRAGMA foreign_keys=ON`` (SQLite ships
    //! it OFF by default for back-compat with pre-3.6.19 databases — without
    //! the pragma, FK clauses are parsed and stored but not enforced), invokes
    //! the block, finalizes on exit (including on panic / early return). Users
    //! who need FKs disabled can ``db |> exec("PRAGMA foreign_keys=OFF")`` from
    //! inside the block.
    var inscope db = open_sqlite(path)
    db |> exec("PRAGMA foreign_keys = ON")
    invoke(blk, db)
}

def try_exec(db : SqlRunner; sql : string) : SqlError {
    //! Executes ``sql`` (one or more statements, no result rows).
    //! Returns ``none`` on success, ``some(errmsg)`` on failure.
    var errmsg : string
    let rc = sqlite3_exec(db.sqlite_handle, sql, unsafe(addr(errmsg)))
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
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
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
    //! Returns the number of rows affected by the most recent INSERT, UPDATE, or DELETE
    //! on this connection. Wraps ``sqlite3_changes``; see https://sqlite.org/c3ref/changes.html
    //! for the exact contract (DML statements only — DDL, schema changes, and triggers
    //! count separately).
    return sqlite3_changes(db.sqlite_handle)
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
    //! Identity passthrough for BLOB columns. ``array<uint8>`` is non-
    //! copyable, so the const-reference parameter forces a clone on
    //! return. SQLite's ``sqlite3_bind_blob`` with ``SQLITE_TRANSIENT``
    //! adds a second copy. For very large BLOBs (multi-MB asset
    //! payloads), prefer the streaming ``sqlite3_blob_open`` API to
    //! avoid extra copying.
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
    //! Reads a BLOB column as a fresh ``array<uint8>``. Allocates on
    //! daslang's heap and copies — SQLite's blob pointer is only valid
    //! until the next ``step`` / ``reset`` / ``finalize``, so the copy
    //! is mandatory for safe out-of-statement use.
    let n = sqlite3_column_bytes(stmt, col)
    var r : array<uint8>
    r |> resize(n)
    if (n > 0) {
        unsafe {
            // sqlite3_column_blob returns ``const void *``; AOT codegen's
            // das_memcpy wants a non-const source, so strip the const here.
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
    if (sqlite3_column_type(stmt, col) == SQLITE_NULL) {
        return none(type<TT -const -#>)
    }
    return some(read_via_adapter(stmt, col, type<TT -const -#>))
}

[unused_argument(t)]
def read_via_adapter(stmt : sqlite3_stmt?; col : int; t : type<auto(TT)>) : TT {
    //! Reads a single column as the storage primitive
    //! ``_::sql_bind(default<TT>)`` returns, then materializes ``TT`` via
    //! ``_::sql_extract``. The primitive type is resolved at compile time
    //! via ``typedecl`` — no runtime call to the user's adapter.
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
    //! Other providers ship their own helper (`postgres_sql_type`,
    //! `mysql_sql_type`, …) so the same `ColumnInfo` round-trips across
    //! backends without ambiguity.
    if (t == SqlType.Integer) {
        return "INTEGER"
    }
    if (t == SqlType.Real) {
        return "REAL"
    }
    if (t == SqlType.Text) {
        return "TEXT"
    }
    if (t == SqlType.Blob) {
        return "BLOB"
    }
    return "NULL"
}

[unused_argument(t)]
def try_query_scalar(db : SqlRunner; sql : string; t : type<auto(TT)>) : Result<TT -const -#, string> {
    //! Prepares ``sql``, steps once, returns column 0 as ``TT``. Routes
    //! through the ``_::sql_bind`` / ``_::sql_extract`` adapter rail (tut 26)
    //! via ``read_via_adapter``, so ``TT`` can be any primitive, enum, or
    //! user type that ships an adapter pair.
    //! Returns ``Err(errmsg)`` on prepare/step error or zero rows.
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
    //! ``Option<TT>`` variant of ``query_scalar`` for when "0 rows" is a
    //! normal outcome (e.g. lookup by primary key). Returns ``some(val)``
    //! if a row is found, ``none`` if zero rows. Panics only on
    //! prepare/step failure (genuine errors).
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
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
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
            if (src == SQLITE_DONE) {
                return err("{err_prefix}: expected one row, got 0 rows", type<TT -const -#>)
            }
            if (src != SQLITE_ROW) {
                return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<TT -const -#>)
            }
            // Copyable rows initialize via ``=``; non-copyable (e.g. user types
            // containing ``array<uint8>``) route through ``clone_to_move`` so
            // the rvalue is movable.
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
            if (src == SQLITE_DONE) {
                return ok(none(type<TT -const -#>), type<string>)
            }
            if (src != SQLITE_ROW) {
                return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<Option<TT -const -#>>)
            }
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
            if (src != SQLITE_DONE) {
                return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<int>)
            }
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
            if (rc != SQLITE_DONE) {
                return err("{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}", type<array<TT -const -#>>)
            }
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
    if (err |> is_some) {
        return err
    }
    let indexes_sql = _::_sql_create_indexes_sql(type<TT>)
    if (empty(indexes_sql)) {
        return none(type<string>)
    }
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
    //! Driven by the per-struct ``_sql_column_info`` helper that the macro
    //! emits alongside DDL — same compile-time field walk, viewed through
    //! the abstract `ColumnInfo` lens. Render the dialect-specific column
    //! type via `sqlite_sql_type(c.data_type)`.
    return <- _::_sql_column_info(default<TT>)
}

def try_insert(db : SqlRunner; row : auto(TT)) : Result<int64, string> {
    //! Inserts a single row. Returns ``Ok(rowid)`` on success, ``Err(errmsg)`` on failure.
    //! When the row's integer primary key is zero, the macro-emitted no-PK INSERT runs and SQLite assigns the id.
    let pk_unset = _::_sql_pk_is_unset(row)
    let sql = pk_unset ? _::_sql_insert_no_pk_sql(row) : _::_sql_insert_with_pk_sql(row)
    let r = db |> try_step_dml(sql, $(var stmt : sqlite3_stmt?) {
        _::_sql_bind_row(stmt, row, !pk_unset)
    }, "insert")
    if (r |> is_err) {
        return err(r |> unwrap_err, type<int64>)
    }
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

def try_insert(db : SqlRunner; rows : array<auto(TT)>) : Result<int64, string> {
    //! Batched INSERT inside a single BEGIN IMMEDIATE / COMMIT transaction.
    //! All rows must agree on PK presence — first row decides which INSERT statement is prepared,
    //! and the function validates uniformity up front. A mixed-PK array returns ``Err`` without
    //! opening a transaction; partition such inputs at the call site.
    //! Returns ``Ok(last_rowid)`` on success, ``Err(errmsg)`` on failure (after ROLLBACK if a
    //! transaction was opened).
    if (empty(rows)) {
        return ok(int64(0), type<string>)
    }
    let pk_unset = _::_sql_pk_is_unset(rows[0])
    for (i in 1..length(rows)) {
        if (_::_sql_pk_is_unset(rows[i]) != pk_unset) {
            return err("insert: rows mix PK-set and PK-unset (row 0 vs row {i}); partition the array before calling insert", type<int64>)
        }
    }
    // BEGIN/COMMIT vs SAVEPOINT/RELEASE depending on ambient state, so this composes inside with_transaction.
    let was_in_txn = db |> in_transaction()
    let begin_sql = was_in_txn ? "SAVEPOINT das_ins_sp" : "BEGIN IMMEDIATE"
    let begin_err = db |> try_exec(begin_sql)
    if (begin_err |> is_some) {
        return err(begin_err |> unwrap, type<int64>)
    }
    let sql = pk_unset ? _::_sql_insert_no_pk_sql(rows[0]) : _::_sql_insert_with_pk_sql(rows[0])
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
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
    if (empty(or_clause)) {
        return plain_sql
    }
    return replace(plain_sql, "INSERT ", "INSERT {or_clause} ")
}

def private try_insert_or(db : SqlRunner; row : auto(TT); or_clause : string) : Result<int, string> {
    let pk_unset = _::_sql_pk_is_unset(row)
    let plain_sql = pk_unset ? _::_sql_insert_no_pk_sql(row) : _::_sql_insert_with_pk_sql(row)
    let sql = with_or_clause(plain_sql, or_clause)
    let r = db |> try_step_dml(sql, $(var stmt : sqlite3_stmt?) {
        _::_sql_bind_row(stmt, row, !pk_unset)
    }, "insert_or_{or_clause}")
    if (r |> is_err) {
        return err(r |> unwrap_err, type<int>)
    }
    return ok(sqlite3_changes(db.sqlite_handle), type<string>)
}

def private try_insert_or(db : SqlRunner; rows : array<auto(TT)>; or_clause : string) : Result<int, string> {
    if (empty(rows)) {
        return ok(0, type<string>)
    }
    let pk_unset = _::_sql_pk_is_unset(rows[0])
    for (i in 1..length(rows)) {
        if (_::_sql_pk_is_unset(rows[i]) != pk_unset) {
            return err("insert_or_{or_clause}: rows mix PK-set and PK-unset (row 0 vs row {i}); partition the array before calling", type<int>)
        }
    }
    let was_in_txn = db |> in_transaction()
    let begin_sql = was_in_txn ? "SAVEPOINT das_inso_sp" : "BEGIN IMMEDIATE"
    let begin_err = db |> try_exec(begin_sql)
    if (begin_err |> is_some) {
        return err(begin_err |> unwrap, type<int>)
    }
    let plain_sql = pk_unset ? _::_sql_insert_no_pk_sql(rows[0]) : _::_sql_insert_with_pk_sql(rows[0])
    let sql = with_or_clause(plain_sql, or_clause)
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
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
    //! Deletes the row identified by ``row``'s @sql_primary_key field. Non-PK
    //! fields are ignored. Returns ``Ok(rows_affected)`` (1 if matched, 0 if not),
    //! ``Err(errmsg)`` on SQL failure.
    //! Trailing underscore in the name is intentional: ``delete`` is a daslang
    //! keyword, mirroring the convention used by ``daslib/linq``'s ``where_``.
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
    //! Deletes the row of type ``T`` whose primary key equals ``id``. Avoids
    //! constructing a dummy struct just to identify the row. Returns
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
    //! Returns true if the connection is currently inside a BEGIN/COMMIT pair
    //! (or a SAVEPOINT). Wraps the inverse of ``sqlite3_get_autocommit``.
    return !sqlite3_get_autocommit_(db.sqlite_handle)
}

def private txn_begin_sql(was_in_txn : bool; mode : SqliteTxnMode) : string {
    if (was_in_txn) {
        return "SAVEPOINT das_sp"
    }
    if (mode == SqliteTxnMode.Immediate) {
        return "BEGIN IMMEDIATE"
    }
    if (mode == SqliteTxnMode.Exclusive) {
        return "BEGIN EXCLUSIVE"
    }
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
    //! Wraps ``blk`` in a SQL transaction. Emits BEGIN [mode] / COMMIT, or
    //! SAVEPOINT das_sp / RELEASE das_sp when nested inside an existing
    //! transaction. ROLLBACK fires on panic / early return via daslang's
    //! ``finally`` (best effort — original failure propagates).
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
    //! Non-panic 2-arg overload. Returns ``none`` on commit success, or
    //! ``some(errmsg)`` if BEGIN or COMMIT fails. Block panics still ROLLBACK
    //! and re-propagate — wrap the block in your own try/recover if you want
    //! to convert a block panic into a SqlError.
    //!
    //! Returns ``SqlError = Option<string>`` rather than ``Result<void, string>``
    //! because daslang's ``Result<T, E>`` does not yet support a ``void`` payload.
    return db |> try_transaction(SqliteTxnMode.Deferred, blk)
}

def try_transaction(db : SqlRunner; mode : SqliteTxnMode; blk : block<() : void>) : SqlError {
    //! Non-panic 3-arg overload. Same semantics as ``try_transaction(db, blk)``
    //! with a SQLite-specific BEGIN modifier.
    let was_in_txn = db |> in_transaction()
    let begin_err = db |> try_exec(txn_begin_sql(was_in_txn, mode))
    if (begin_err |> is_some) {
        return begin_err
    }
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
    //! Wraps ``s`` in double quotes; SQLite-portable form for a quoted
    //! identifier (table / column / view name). Embedded double quotes
    //! are doubled. Used by ``fold_to_builder`` for ``inline_id`` fragments
    //! and by ``_sql_pragma`` / ``_sql_vacuum_into`` for runtime-built SQL.
    let escaped = s |> replace("\"", "\"\"")
    return "\"{escaped}\""
}

def sql_quote_lit(s : string) : string {
    //! Wraps ``s`` in single quotes (SQL string literal). Embedded single
    //! quotes are doubled. Used by ``fold_to_builder`` for ``inline_lit``
    //! fragments where the value must be inlined into the SQL text
    //! (CREATE VIEW, PRAGMA, VACUUM INTO — none accept ``?`` placeholders).
    let escaped = s |> replace("'", "''")
    return "'{escaped}'"
}

// ===== PRAGMA helpers =====

def try_set_pragma(db : SqlRunner; name : string; value : string) : SqlError {
    //! Sets ``PRAGMA "<name>" = '<value>'``. Returns ``none`` on success,
    //! ``some(errmsg)`` on failure. The value is single-quoted; embedded
    //! single quotes in ``value`` are doubled to keep SQLite happy.
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
    //! Applies a curated set of pragmas that's appropriate for typical
    //! application use (write-ahead logging, foreign keys, sane busy
    //! behavior). Stops at the first failure and returns its error.
    //!
    //!   journal_mode = WAL        // concurrent reads + one writer
    //!   busy_timeout = 5000       // wait 5 s on contention before SQLITE_BUSY
    //!   foreign_keys = ON         // enforce FK constraints (off by default)
    //!   synchronous = NORMAL      // good safety+speed balance under WAL
    //!
    //! If your workload demands different defaults (durability-first servers
    //! → ``synchronous = FULL``, embedded ROM-like data → ``journal_mode =
    //! OFF``) call ``set_pragma`` directly. ``journal_mode`` cannot be
    //! changed inside an active transaction — call this at connection setup,
    //! not mid-flow.
    let e1 = db |> try_set_pragma("journal_mode", "WAL")
    if (e1 |> is_some) {
        return e1
    }
    let e2 = db |> try_set_pragma("busy_timeout", 5000l)
    if (e2 |> is_some) {
        return e2
    }
    let e3 = db |> try_set_pragma("foreign_keys", true)
    if (e3 |> is_some) {
        return e3
    }
    let e4 = db |> try_set_pragma("synchronous", "NORMAL")
    if (e4 |> is_some) {
        return e4
    }
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
    if (db |> in_transaction()) {
        return some("VACUUM cannot run inside an active transaction; commit or rollback first")
    }
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
    //! VACUUM INTO '<path>' — write a snapshot of the live database to
    //! ``path`` without modifying the source. Useful for "save as" / hot
    //! backup scenarios where you don't want VACUUM's lock contention on
    //! the live file. The path is single-quoted with `''` doubling for
    //! safety; SQLite's ``VACUUM INTO`` does not accept `?` placeholders.
    if (db |> in_transaction()) {
        return some("VACUUM INTO cannot run inside an active transaction; commit or rollback first")
    }
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
    //! ``PRAGMA optimize`` — refresh the query planner's stats. Recommended
    //! at connection close on long-lived connections; cheap on short-lived
    //! ones (it short-circuits when nothing has changed).
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
    //! Runs ``PRAGMA integrity_check`` and returns the result rows.
    //! On a healthy database, returns ``["ok"]``. On corruption returns
    //! one row per detected issue (free-page corruption, missing index
    //! entry, etc.). Returns ``Err`` only on prepare/step failure.
    return <- db |> try_check_impl("PRAGMA integrity_check")
}

def integrity_check(db : SqlRunner) : array<string> {
    //! Strict variant of ``try_integrity_check``. Panics on
    //! prepare/step failure (NOT on detected corruption — corruption is
    //! reported via the returned rows).
    var r <- db |> try_integrity_check()
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def try_quick_check(db : SqlRunner) : Result<array<string>, string> {
    //! Cheaper sibling of ``try_integrity_check`` — same return shape,
    //! but skips the index-vs-table cross check. Useful as a fast
    //! pre-flight before deciding whether to run the full check.
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
    //! Copies all of ``db`` into the open connection ``dest`` using
    //! SQLite's online backup API. Pages are copied incrementally and
    //! concurrent writers on ``db`` are tolerated (transient
    //! SQLITE_BUSY/LOCKED responses are retried with a 50 ms backoff).
    //! Useful for snapshotting :memory: databases, hot backups, or
    //! cloning before destructive migrations. Returns ``some(errmsg)``
    //! if the backup did not complete; ``none`` on success.
    //!
    //! The C++ shim returns ``SQLITE_OK`` on a fully-completed backup
    //! (the result of ``sqlite3_backup_finish`` after a clean
    //! ``SQLITE_DONE`` from the step loop) and any other rc on failure.
    let rc = sqlite3_backup_run(db.sqlite_handle, dest.sqlite_handle)
    if (rc != SQLITE_OK) {
        return some("backup_to: sqlite3_backup_run failed (rc={rc}): {sqlite3_errmsg(dest.sqlite_handle)}")
    }
    return SqlError()
}

def backup_to(db : SqlRunner; dest : SqlRunner) : void {
    //! Strict variant of ``try_backup_to``. Panics with the libsqlite3
    //! errmsg on any non-completion.
    let err = db |> try_backup_to(dest)
    if (err |> is_some) {
        panic(err |> unwrap)
    }
}

def try_backup_to(db : SqlRunner; path : string) : SqlError {
    //! Opens ``path`` (creating the file if it does not exist), copies
    //! ``db`` into it, and closes ``path`` on return. Returns ``some(err)``
    //! on either an open failure or a backup failure; on backup failure
    //! the destination file is finalized regardless, leaving a partially-
    //! populated file on disk that the caller can ``fio::remove`` or
    //! keep for diagnostics.
    var dest_result <- try_open_sqlite(path)
    if (dest_result |> is_err) {
        return some(dest_result |> unwrap_err)
    }
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
    //! Prepares ``sql`` (which must include a ``RETURNING ...`` clause),
    //! invokes ``bind_blk``, then steps repeatedly through ``SQLITE_ROW``
    //! rows, calling ``row_blk`` to materialize each result row.
    //! Used by ``_sql_update_returning`` / ``_sql_delete_returning`` macros.
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
    //! Reads every row of the table backing ``T`` and materializes them as ``array<TT>``.
    //! Compat-mode counterpart to the ``_sql(db |> select_from(type<T>))`` macro path:
    //! when no chain operators or ``_sql`` wrapper is involved, this is what runs.
    //! Returns ``Err(errmsg)`` on prepare or step failure.
    //!
    //! NOTE: uses a regular generic (`type<auto(TT)>`) rather than `[template]` so
    //! the `type<T>` argument survives in the AST for `_sql` chain-pattern matching.
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
    //! `_each_sql` macro runtime: lazy-iterating sibling of ``run_select``.
    //! Prepares ``sql``, runs ``bind_blk`` once, then returns a
    //! ``generator<TT>`` that lazily ``sqlite3_step``s the cursor; each
    //! yielded value is produced by ``row_lam(stmt)``. Useful for
    //! streaming over large result sets where the array materialization
    //! would dominate memory or latency.
    //!
    //! ``row_lam`` is a lambda (not a block) because it must be captured
    //! into the generator body and outlive the surrounding stack frame.
    //! ``bind_blk`` runs once before the generator is constructed and can
    //! stay a block.
    //!
    //! Lifetime: the prepared statement is owned by the generator;
    //! ``sqlite3_finalize`` runs in the generator's ``finally`` block —
    //! statement is released on normal exhaustion, early break, or panic.
    //! One-shot semantics: a generator can only be iterated once; re-run
    //! the same query by re-evaluating the ``_each_sql(...)`` call site.
    //!
    //! Panics on prepare failure (with the libsqlite3 errmsg). Step
    //! failures inside the loop also panic; the generator's ``finally``
    //! finalizes the stmt regardless. The strict semantics match
    //! ``run_select``; for non-panic recovery catch the panic at a higher
    //! level (try/recover).
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
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
    //! `_sql` macro runtime. Prepares ``sql``, runs ``bind_blk`` once,
    //! then steps the cursor and materializes each row via ``row_blk``.
    //! Generic on row type so the call site determines the array element
    //! shape. Returns ``Err(errmsg)`` on prepare or step failure.
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
    //! `_sql` macro runtime: single-row materializer used by `_sql(...)`
    //! chains ending in `_first()`, `_count()`, or aggregates. Prepares
    //! ``sql``, runs ``bind_blk`` once, steps once, materializes the first
    //! row via ``row_blk``. Returns ``Err(errmsg)`` on prepare or step
    //! failure or zero rows. (Sibling ``run_select_one_opt`` returns
    //! ``Ok(none)`` for "0 rows is expected" call sites.)
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
        if (v is tString) {
            return v as tString
        }
    } else {
        if (v is tBool) {
            return v as tBool
        }
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
    if (action == "cascade") {
        return "CASCADE"
    } elif (action == "set_null") {
        return "SET NULL"
    } elif (action == "set_default") {
        return "SET DEFAULT"
    } elif (action == "restrict") {
        return "RESTRICT"
    }
    return "NO ACTION"
}

def private sql_quote_string_literal(s : string) : string {
    let escaped = replace(s, "'", "''")
    return "'{escaped}'"
}

def private literal_init_to_default_clause(init : ExpressionPtr) : string {
    if (init == null) {
        return ""
    }
    if (init is ExprConstBool) {
        return (init as ExprConstBool).getValue ? " DEFAULT 1" : " DEFAULT 0"
    }
    if (init is ExprConstInt) {
        return " DEFAULT {(init as ExprConstInt).getValue}"
    }
    if (init is ExprConstInt64) {
        return " DEFAULT {(init as ExprConstInt64).getValue}"
    }
    if (init is ExprConstFloat) {
        return " DEFAULT {(init as ExprConstFloat).getValue}"
    }
    if (init is ExprConstDouble) {
        return " DEFAULT {(init as ExprConstDouble).getValue}"
    }
    if (init is ExprConstString) {
        return " DEFAULT {sql_quote_string_literal(string((init as ExprConstString).value))}"
    }
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
    computed_sql : string
    default_clause : string
    references_clause : string
}

def private is_option_field_type(td : TypeDeclPtr) : bool {
    if (td == null) {
        return false
    }
    if (td.baseType == Type.typeMacro && length(td.dimExpr) > 0 && (td.dimExpr[0] is ExprConstString)) {
        return (td.dimExpr[0] as ExprConstString).value == "Option"
    }
    return (td.baseType == Type.tStructure
        && td.structType != null
        && td.structType.name == "Option"
        && td.structType._module != null
        && td.structType._module.name == "option")
}

def private build_create_indexes_sql(st : StructurePtr; table_name : string; field_names : array<string>; var errors : das_string; var ok : bool&) : string {
    ok = true
    var stmt_count = 0
    let sql = build_string() $(var w) {
        for (ann in st.annotations) {
            if (string(ann.annotation.name) != "sql_index") {
                continue
            }
            let idx_name_arg = find_annotation(ann.arguments, "name", "")
            let is_unique = find_annotation(ann.arguments, "unique", false)
            let cols = find_string_array_annotation(ann.arguments, "fields")
            if (length(cols) == 0) {
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
    if (fn == null || fn.body == null || !(fn.body is ExprBlock)) {
        return ""
    }
    let blk = fn.body as ExprBlock
    if (length(blk.list) == 0) {
        return ""
    }
    let stmt = blk.list[0]
    if (!(stmt is ExprReturn)) {
        return ""
    }
    let ret = stmt as ExprReturn
    let val = ret.subexpr
    if (val == null || !(val is ExprConstString)) {
        return ""
    }
    return string((val as ExprConstString).value)
}

def private find_struct_helper_fn(name : string; st : StructurePtr) : FunctionPtr {
    var found : FunctionPtr
    for_each_function(compiling_module(), name) $(var fn) {
        if (length(fn.arguments) > 0
                && fn.arguments[0]._type != null
                && fn.arguments[0]._type.structType == st) {
            found = fn
        }
    }
    return found
}

[structure_macro(name=sql_index)]
class SqlIndexMacro : AstStructureAnnotation {
    //! Stackable struct-level annotation:
    //!   ``[sql_index(fields="ColName", unique=true, name="explicit_name")]``
    //!   ``[sql_index(fields=("A", "B"))]``
    //! Reads the per-struct ``_sql_create_indexes_sql`` helper that
    //! `[sql_table]` previously emitted (must appear earlier in the same
    //! annotation list — typically ``[sql_table(...), sql_index(...), ...]``)
    //! and rewrites its body to append one ``CREATE [UNIQUE] INDEX``
    //! statement per annotation, joined by ``;``.
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let cols = find_string_array_annotation(args, "fields")
        if (length(cols) == 0) {
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
        let auto_name = build_string() $(var w) {
            w |> write("idx_")
            w |> write(table_name)
            for (col in sql_cols) {
                w |> write("_")
                w |> write(col)
            }
        }
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
            if (string(ps.name) == parent_name) {
                found_st = ps
            }
        }
    }
    if (found_st == null) {
        return ("", "", false)
    }
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
            return (tab, pk_col, true)
        }
    }
    return (tab, "", false)
}

[structure_macro(name=sql_table)]
class SqlTableMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let table_name = find_annotation(args, "name", string(st.name))

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
        if (!emit_json_blob_adapters_for_struct(st, fields, "[sql_table]", errors)) {
            return false
        }

        // [sql_index] siblings rewrite the placeholder body in finish() (st.annotations not populated yet here).
        var fn_table_name = make_table_name_fn(st, table_name)
        compiling_module() |> add_function(fn_table_name)
        var fn_drop = make_drop_sql_fn(st, table_name)
        compiling_module() |> add_function(fn_drop)
        var fn_create = make_create_table_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_create)
        // Empty `_sql_create_indexes_sql` placeholder; [sql_index] siblings accumulate into it.
        var fn_indexes = make_create_indexes_sql_fn(st, "")
        compiling_module() |> add_function(fn_indexes)
        var fn_ins_pk = make_insert_sql_fn(st, table_name, fields, true)
        compiling_module() |> add_function(fn_ins_pk)
        var fn_ins_nopk = make_insert_sql_fn(st, table_name, fields, false)
        compiling_module() |> add_function(fn_ins_nopk)
        var fn_pk_unset = make_pk_is_unset_fn(st, fields)
        compiling_module() |> add_function(fn_pk_unset)
        var fn_bind = make_bind_row_fn(st, fields)
        compiling_module() |> add_function(fn_bind)
        var fn_select_all = make_select_all_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_select_all)
        var fn_read = make_read_row_fn(st, fields)
        compiling_module() |> add_function(fn_read)
        var fn_upd_pk = make_update_by_pk_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_upd_pk)
        var fn_del_pk = make_delete_by_pk_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_del_pk)
        var fn_bind_upd = make_bind_row_for_update_fn(st, fields)
        compiling_module() |> add_function(fn_bind_upd)
        var fn_bind_pk = make_bind_row_pk_only_fn(st, fields)
        compiling_module() |> add_function(fn_bind_pk)
        var fn_col_info = make_column_info_fn(st, fields)
        compiling_module() |> add_function(fn_col_info)

        return true
    }


    def make_table_name_fn(var st : StructurePtr; table_name : string) : FunctionPtr {
        var fn = qmacro_function("_sql_table_name") $(typ : $t(st)) : string {
            return $v(table_name)
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_drop_sql_fn(var st : StructurePtr; table_name : string) : FunctionPtr {
        let sql = "DROP TABLE IF EXISTS \"{table_name}\""
        var fn = qmacro_function("_sql_drop_table_if_exists_sql") $(typ : $t(st)) : string {
            return $v(sql)
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_create_indexes_sql_fn(var st : StructurePtr; indexes_sql : string) : FunctionPtr {
        //! Per-struct helper returning the concatenated CREATE INDEX statements
        //! for every `[sql_index(...)]` attached to the struct, joined by ``;``.
        //! Returns ``""`` when the struct has no indexes; ``try_create_table``
        //! short-circuits sqlite3_exec on empty input.
        var fn = qmacro_function("_sql_create_indexes_sql") $(typ : $t(st)) : string {
            return $v(indexes_sql)
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_create_table_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        var write_exprs : array<ExpressionPtr>
        write_exprs |> reserve(3 * length(fields) + 2)

        let header = "CREATE TABLE \"{table_name}\"("
        write_exprs |> push(qmacro_expr(${ writer |> write($v(header)); }))

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

        var fn = qmacro_function("_sql_create_table_sql") $(typ : $t(st)) : string {
            return build_string() $(var writer) {
                $b(write_exprs)
            }
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_column_info_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
        //! Per-struct helper backing the user-facing ``column_info(type<T>)``
        //! generic. Walks the same `FieldInfo` array DDL emission uses; emits
        //! one `ColumnInfo` push per field in declaration order, including
        //! ``@sql_computed`` fields (they appear in the schema and in
        //! ``PRAGMA table_info``, so the daslang-side view surfaces them
        //! too). ``is_nullable`` follows the daslang-side meaning — it is
        //! ``info.is_optional`` (Option<T>-derived), not a NOT-NULL constraint
        //! check. See API_REWORK § 09.
        var push_exprs : array<ExpressionPtr>
        push_exprs |> reserve(length(fields))
        for (i in range(length(fields))) {
            let info = fields[i]
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
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_insert_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>; include_pk : bool) : FunctionPtr {
        // Explicit column list skips GENERATED columns; falls back to `DEFAULT VALUES` when no bindable cols remain.
        let helper_name = include_pk ? "_sql_insert_with_pk_sql" : "_sql_insert_no_pk_sql"
        let cols = build_string() $(var w) {
            var first = true
            for (info in fields) {
                if (info.is_computed || (!include_pk && info.is_pk)) {
                    continue
                }
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
                if (info.is_computed || (!include_pk && info.is_pk)) {
                    continue
                }
                if (!first) {
                    w |> write(", ")
                }
                w |> write("?")
                first = false
            }
        }
        let sql = (empty(cols)
            ? "INSERT INTO \"{table_name}\" DEFAULT VALUES"
            : "INSERT INTO \"{table_name}\" ({cols}) VALUES ({placeholders})")
        var fn = qmacro_function(helper_name) $(typ : $t(st)) : string {
            return $v(sql)
        }
        fn.flags |= FunctionFlags.generated
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
            fn.flags |= FunctionFlags.generated
            fn.body |> force_at(st.at)
            return <- fn
        } else {
            var fn = qmacro_function("_sql_pk_is_unset") $(row : $t(st)) : bool {
                return false
            }
            fn.flags |= FunctionFlags.generated
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
            if (info.is_computed) {
                continue
            }
            with_pk_idx++
            let wi = with_pk_idx
            let field_name = info.name
            with_pk_exprs |> push(qmacro_expr(${
                sql_bind_to_stmt(stmt, $v(wi), row.$f(field_name));
            }))
        }

        var no_pk_idx = 0
        for (info in fields) {
            if (info.is_pk || info.is_computed) {
                continue
            }
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
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_select_all_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
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
        let sql = "SELECT {col_list} FROM \"{table_name}\""
        var fn = qmacro_function("_sql_select_all_sql") $(typ : $t(st)) : string {
            return $v(sql)
        }
        fn.flags |= FunctionFlags.generated
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
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def find_pk_field(fields : array<FieldInfo>) : int {
        for (i in range(length(fields))) {
            if (fields[i].is_pk) {
                return i
            }
        }
        return -1
    }

    def make_update_by_pk_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        // PK-less structs get a runtime-panic stub (concept_assert would fire on every [sql_table] decl).
        let pk_idx = find_pk_field(fields)
        if (pk_idx < 0) {
            let st_name = string(st.name)
            let msg = "[sql_table] {st_name}: update(row) / delete_(row) / delete_by_id are unavailable — no @sql_primary_key field. Use the macro forms `_sql_update(type<{st_name}>, where, set)` / `_sql_delete(type<{st_name}>, where)` instead."
            var fn = qmacro_function("_sql_update_by_pk_sql") $(typ : $t(st)) : string {
                panic($v(msg))
                return ""
            }
            fn.flags |= FunctionFlags.generated
            fn.body |> force_at(st.at)
            return <- fn
        }
        let pk_col = fields[pk_idx].col_name
        let set_clause = build_string() $(var w) {
            var first = true
            for (info in fields) {
                if (info.is_pk || info.is_computed) {
                    continue
                }
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
            var fn = qmacro_function("_sql_update_by_pk_sql") $(typ : $t(st)) : string {
                panic($v(msg))
                return ""
            }
            fn.flags |= FunctionFlags.generated
            fn.body |> force_at(st.at)
            return <- fn
        }
        let sql = "UPDATE \"{table_name}\" SET {set_clause} WHERE \"{pk_col}\" = ?"
        var fn = qmacro_function("_sql_update_by_pk_sql") $(typ : $t(st)) : string {
            return $v(sql)
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_delete_by_pk_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        let pk_idx = find_pk_field(fields)
        if (pk_idx < 0) {
            let st_name = string(st.name)
            let msg = "[sql_table] {st_name}: delete_(row) / delete_by_id require a @sql_primary_key field. Use `_sql_delete(type<{st_name}>, where)` instead."
            var fn = qmacro_function("_sql_delete_by_pk_sql") $(typ : $t(st)) : string {
                panic($v(msg))
                return ""
            }
            fn.flags |= FunctionFlags.generated
            fn.body |> force_at(st.at)
            return <- fn
        }
        let pk_col = fields[pk_idx].col_name
        let sql = "DELETE FROM \"{table_name}\" WHERE \"{pk_col}\" = ?"
        var fn = qmacro_function("_sql_delete_by_pk_sql") $(typ : $t(st)) : string {
            return $v(sql)
        }
        fn.flags |= FunctionFlags.generated
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
            fn.flags |= FunctionFlags.generated
            fn.body |> force_at(st.at)
            return <- fn
        }

        var bind_exprs : array<ExpressionPtr>
        var bind_idx = 0
        // Non-PK, non-computed columns first.
        for (i in range(length(fields))) {
            if (fields[i].is_pk || fields[i].is_computed) {
                continue
            }
            bind_idx++
            let bi = bind_idx
            let field_name = fields[i].name
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
        fn.flags |= FunctionFlags.generated
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
            fn.flags |= FunctionFlags.generated
            fn.body |> force_at(st.at)
            return <- fn
        }
        let pk_field_name = fields[pk_idx].name
        var fn = qmacro_function("_sql_bind_row_pk_only") $(var stmt : sqlite3_stmt?; row : $t(st)) : void {
            sql_bind_to_stmt(stmt, 1, row.$f(pk_field_name))
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

}

// ===== @sql_json adapter codegen =====

def private make_json_sql_bind_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
    // Parser marks struct/tuple params const; clone + set constant so generic resolution picks this overload.
    var t1 <- clone_type(field_type)
    t1.flags |= TypeDeclFlags.constant
    var fn = qmacro_function("sql_bind") $(v : $t(t1)) : string {
        return _::write_json(_::JV(v))
    }
    fn.body |> force_at(at)
    return <- fn
}

def private make_json_sql_extract_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
    var t1 <- clone_type(field_type)
    t1.flags |= TypeDeclFlags.constant
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
    t1.flags |= TypeDeclFlags.constant
    var fn = qmacro_function("sql_bind") $(v : $t(t1)) : array<uint8> {
        var v_local <- clone_to_move(v)
        return <- _::mem_archive_save(v_local)
    }
    fn.body |> force_at(at)
    return <- fn
}

def private make_blob_sql_extract_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
    var t1 <- clone_type(field_type)
    t1.flags |= TypeDeclFlags.constant
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
    if (td == null) {
        return SqlAdapterKind.None
    }
    if (td.baseType == Type.tString) {
        return SqlAdapterKind.Json
    }
    if (td.baseType == Type.tArray && td.firstType != null && td.firstType.baseType == Type.tUInt8) {
        return SqlAdapterKind.Blob
    }
    return SqlAdapterKind.Other
}

def private find_sql_bind_kind_for_type(mod : Module?; td : TypeDeclPtr) : SqlAdapterKind {
    if (td == null) {
        return SqlAdapterKind.None
    }
    var kind = SqlAdapterKind.None
    for_each_function(mod, "sql_bind") $(var fn) {
        if (length(fn.arguments) > 0
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
        if (!info.is_json && !info.is_blob) {
            continue
        }
        let ft = st.fields[i]._type
        if (ft == null) {
            continue
        }
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
        if (!emit_json_blob_adapters_for_struct(st, fields, "[sql_view]", errors)) {
            return false
        }
        // ---- read-path helpers (same names as [sql_table]) ----
        var fn_table_name = make_view_table_name_fn(st, view_name)
        compiling_module() |> add_function(fn_table_name)
        var fn_drop = make_view_drop_sql_fn(st, view_name)
        compiling_module() |> add_function(fn_drop)
        var fn_select_all = make_view_select_all_sql_fn(st, view_name, fields)
        compiling_module() |> add_function(fn_select_all)
        var fn_read = make_view_read_row_fn(st, fields)
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
        var fn_pk_unset = make_view_pk_is_unset_stub_fn(st)
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

def private make_view_table_name_fn(var st : StructurePtr; view_name : string) : FunctionPtr {
    var fn = qmacro_function("_sql_table_name") $(typ : $t(st)) : string {
        return $v(view_name)
    }
    fn.flags |= FunctionFlags.generated
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_drop_sql_fn(var st : StructurePtr; view_name : string) : FunctionPtr {
    let sql = "DROP VIEW IF EXISTS \"{view_name}\""
    var fn = qmacro_function("_sql_drop_view_if_exists_sql") $(typ : $t(st)) : string {
        return $v(sql)
    }
    fn.flags |= FunctionFlags.generated
    fn.body |> force_at(st.at)
    return <- fn
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
    fn.flags |= FunctionFlags.generated
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_read_row_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
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
    fn.flags |= FunctionFlags.generated
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_column_info_fn(var st : StructurePtr; fields : array<FieldInfo>) : FunctionPtr {
    var push_exprs : array<ExpressionPtr>
    push_exprs |> reserve(length(fields))
    for (i in range(length(fields))) {
        let info = fields[i]
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
    fn.flags |= FunctionFlags.generated
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_string_panic_stub_fn(var st : StructurePtr; helper_name : string;
                                           st_name : string; verb : string) : FunctionPtr {
    let msg = "[sql_view] {st_name}: cannot {verb} into a view — views are read-only. Mutate the underlying table(s) and re-query the view."
    var fn = qmacro_function(helper_name) $(typ : $t(st)) : string {
        panic($v(msg))
        return ""
    }
    fn.flags |= FunctionFlags.generated
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_pk_is_unset_stub_fn(var st : StructurePtr) : FunctionPtr {
    var fn = qmacro_function("_sql_pk_is_unset") $(row : $t(st)) : bool {
        return false
    }
    fn.flags |= FunctionFlags.generated
    fn.body |> force_at(st.at)
    return <- fn
}

def private make_view_bind_row_panic_stub_fn(var st : StructurePtr; helper_name : string;
                                             st_name : string; verb : string;
                                             has_include_pk : bool) : FunctionPtr {
    let msg = "[sql_view] {st_name}: cannot {verb} into a view — views are read-only. Mutate the underlying table(s) and re-query the view."
    if (has_include_pk) {
        var fn = qmacro_function(helper_name) $(var stmt : sqlite3_stmt?; row : $t(st); include_pk : bool) : void {
            panic($v(msg))
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    } else {
        var fn = qmacro_function(helper_name) $(var stmt : sqlite3_stmt?; row : $t(st)) : void {
            panic($v(msg))
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }
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
    if (td == null) {
        return (uint8(0), false)
    }
    if (td.dim |> length != 0) {
        return (uint8(0), false)
    }
    if (td.baseType == Type.tInt) {
        return (uint8(SqlFnTag.Int), true)
    }
    if (td.baseType == Type.tInt64) {
        return (uint8(SqlFnTag.Int64), true)
    }
    if (td.baseType == Type.tFloat) {
        return (uint8(SqlFnTag.Float), true)
    }
    if (td.baseType == Type.tDouble) {
        return (uint8(SqlFnTag.Double), true)
    }
    if (td.baseType == Type.tBool) {
        return (uint8(SqlFnTag.Bool), true)
    }
    if (td.baseType == Type.tString) {
        return (uint8(SqlFnTag.String), true)
    }
    return (uint8(0), false)
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

def _register_function_check_rc(db : SqlRunner; name : string; fn : auto(FN);
                                nArgs : int;
                                tag0 : uint8; tag1 : uint8; tag2 : uint8; tag3 : uint8;
                                retTag : uint8; deterministic : bool; directonly : bool) : void {
    //! Runtime tail of the `register_function` macro. Calls the C++ trampoline-
    //! registration shim and panics with the libsqlite3 errmsg on failure.
    let rc = sqlite3_register_function(db.sqlite_handle, name, fn, nArgs,
        tag0, tag1, tag2, tag3,
        retTag, deterministic, directonly)
    if (rc != SQLITE_OK) {
        panic("register_function('{name}') failed: {sqlite3_errmsg(db.sqlite_handle)}")
    }
}
