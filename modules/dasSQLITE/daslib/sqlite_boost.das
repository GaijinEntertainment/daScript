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

// ============================================================================
// High-level API — runner, RAII, scalar query, exec, last_insert_rowid
//
// Error-handling shape: each fallible operation has a `try_*` form that does
// the work and returns the failure-or-result via daslib/option +
// daslib/result. The strict form (no `try_`) is a thin wrapper that unwraps
// and panics on failure — so the panic message is always the libsqlite3
// errmsg, never a generic "unwrap called on err". For void-returning
// operations the try_ form returns `SqlError = Option<string>` where
// `none = success` and `some(errmsg) = failure`.
// ============================================================================

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
    //! Single-statement ``exec`` with positional binds. Used by the 1/2/3-arg
    //! ``try_exec`` / ``exec`` overloads below.  Routes through prepare /
    //! bind / step / finalize because ``sqlite3_exec`` doesn't accept bind
    //! parameters directly.
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

// ============================================================================
// SQL adapter rail: sql_bind / sql_extract  (tut 26)
//
// Convention-by-name dispatch. Every SQL-addressable type T has a pair:
//
//     def sql_bind    (v : T) : P                 // T → primitive
//     def sql_extract (v : P; t : type<T>) : T    // primitive → T
//
// where P is one of {int64, double, string, array<uint8>} — the four
// SQLite storage classes. dasSQLITE ships pairs for all standard types
// plus a generic enum overload; user code adds pairs for domain types in
// their own modules. The [sql_table] macro emits ``_::sql_bind`` /
// ``_::sql_extract`` calls (tut 26 lock 4) so the caller's-module
// overloads are always visible.
//
// The return type of ``sql_bind`` IS the storage type — the macro reads
// it via ``typedecl`` and emits the matching ``sqlite3_bind_*`` /
// ``sqlite3_column_*`` call. Single source of truth, no separate
// ``sql_column_type(type<T>)`` registration function.
// ============================================================================

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

// --- enum generic (lock 6) ---
//
// Any enum T round-trips through INTEGER via ``int64(value)`` /
// ``T(int64)``. The catch-all generic uses ``static_if (typeinfo
// is_enum)`` to gate; non-enum non-primitive types trip the
// ``concept_assert`` and surface a clean error at the [sql_table]
// expansion site (or any other _::sql_bind caller).
//
// Tradeoff: enum-value reordering silently corrupts data (value 2 means
// something different in the new schema version). Text-backed enums
// (``@sql_enum_as_string``) are deferred until a user opts in.

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

// ============================================================================
// Internal bridge: sql_bind/sql_extract ↔ libsqlite3 C API
//
// The [sql_table] macro emits ``_::sql_bind(row.field)`` (resolves the user's
// adapter at the caller's module) and routes the resulting primitive into
// libsqlite3 via the four ``sql_bind_to_stmt`` overloads below. For reads,
// ``read_via_adapter`` uses ``typedecl`` to resolve the storage primitive at
// compile time, calls the matching ``sqlite3_column_*``, and feeds the
// primitive into ``_::sql_extract`` to materialize the user type.
//
// ``sql_storage_type_for`` does the same compile-time resolution for the DDL
// emitter — column type strings are picked from the four-primitive table.
// ============================================================================

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
//
// Dispatch table: four primitive overloads route directly to libsqlite3;
// the Option<T> overload handles NULL semantics; the catch-all generic
// routes any other type through the user's ``_::sql_bind`` adapter and
// recurses on the resulting primitive (or the inner value of an Option).

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
//
// Dispatch table: four primitive ``sql_read_witness`` overloads read the
// matching ``sqlite3_column_*``; ``read_via_adapter`` resolves the storage
// primitive at compile time via ``typedecl(_::sql_bind(default<T>))`` and
// then dispatches to the matching witness overload, feeding the primitive
// into ``_::sql_extract``. The ``Option<T>`` overload of
// ``read_via_adapter`` handles NULL detection.

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
//
// Mirror of the DDL string-witness machinery, returning the abstract
// `SqlType` enum from `daslib/sql` instead of the SQLite spelling. The
// per-table `_sql_column_info` helper synthesized by `[sql_table]` calls
// `sql_storage_enum_for(type<F>)` per non-`@sql_json`/`@sql_blob` field;
// `@sql_json` and `@sql_blob` fields short-circuit to `Text` and `Blob`
// respectively (mirrors the DDL bypass at `make_create_table_sql_fn`).

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

// ============================================================================
// query_scalar — read column 0 of the first row as ``TT``
//
// Routes through the tut-26 ``_::sql_bind`` / ``_::sql_extract`` adapter
// rail via ``read_via_adapter``. Built-ins: every ``int*`` / ``uint*``,
// ``float`` / ``double``, ``bool``, ``string``, ``array<uint8>``, every
// enum, and any user type that ships an adapter pair.
//
// `try_*` returns ``Result<TT, string>`` (prepare/step error or zero rows).
// `query_scalar` is the strict variant — panics on err.
// ============================================================================

[unused_argument(t)]
def try_query_scalar(db : SqlRunner; sql : string; t : type<auto(TT)>) : Result<TT -const -#, string> {
    //! Prepares ``sql``, steps once, returns column 0 as ``TT``.
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

// ============================================================================
// query_scalar_opt — Option<TT> variant for "0 rows is expected"
//
// Returns ``some(val)`` if a row is found, ``none`` if zero rows. Panics
// only on prepare/step failure (genuine errors). Use when 0 rows is a
// normal outcome (lookups by primary key, etc.); use ``query_scalar`` when
// 0 rows is a programmer error.
// ============================================================================

[unused_argument(t)]
def query_scalar_opt(db : SqlRunner; sql : string; t : type<auto(TT)>) : Option<TT -const -#> {
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

// ============================================================================
// query_one — full-row read with positional bind args
//
// Each [sql_table] struct gets a generated ``_sql_read_row`` helper at macro
// time; ``query_one`` family uses that to materialize the row. Bind args go
// to positional placeholders in declaration order. Currently only 0..3
// positional args are supported (one explicit overload per arity). 4+ args
// and named-tuple bind (``query_one(sql, type<T>, (a=1, b=2, ...))`` for
// ``:name`` placeholders) are deferred to chunk 4 — the named-tuple form
// needs a ``[call_macro]`` to walk the trailing tuple's argNames.
//
// `try_*` returns ``Result<T, string>`` (bind/prepare/step errors AND zero
// rows); strict panics on err; ``*_opt`` returns ``Option<T>`` (``none`` on
// zero rows; panic only on bind/prepare/step error).
// ============================================================================

// All 12 public variants below delegate to one of three private impls
// that take a `bind_blk : block<(var stmt : sqlite3_stmt?) : void>`,
// invoked once after prepare. Each public wrapper supplies the right
// bind block for its arity.

// Shared prepare/bind/step/finalize bodies for the runtime layer. Four
// helpers cover every shape the public API needs:
//   - `try_one_row`     — single row, ``Err`` on zero rows; consumed by
//                         `try_query_one_impl`, `try_run_select_one`, and
//                         `try_query_scalar`.
//   - `try_one_row_opt` — single row, ``Ok(none)`` on zero rows; consumed
//                         by `query_scalar_opt` and `try_run_select_one_opt`.
//   - `try_step_dml`    — single step, returns rows-affected; consumed by
//                         every non-RETURNING DML (`try_run_dml`,
//                         `try_insert`, `try_update`, `try_delete_`,
//                         `try_delete_by_id`).
//   - `try_collect_rows` — multi-row materializer; consumed by
//                          `try_run_select`, `try_run_dml_returning`, and
//                          `try_select_from`.
// Each one parameterizes only on (a) the row/bind blocks and (b) the
// user-visible error prefix. Adding new wrappers should reuse one of
// these instead of inlining a fifth prepare/bind loop.
//
// All four executors share an identical 6-line prologue (prepare +
// error-on-failure + bind) and a finalize-on-every-exit epilogue.
// `try_with_stmt` owns that boilerplate; each executor only has to
// describe its step-result strategy in `body_blk`.
//
// Error format is normalized to `{err_prefix}: <what>` across all
// failure paths. No consumers parse these strings.

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
    //! Like `try_one_row`, but returns `Ok(none)` on zero rows instead of
    //! `Err`. Use when "no row matched" is a normal outcome and only
    //! prepare/step failures are real errors.
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

// ============================================================================
// query / try_query — typed multi-row raw-SQL read.
//
// The fourth member of the `query_*` family. Unlike `select_from(type<T>)`
// (which builds a `SELECT cols-of(T) FROM <table>` and runs it), `query` takes
// the user's own SQL — useful for catalog reads (`PRAGMA table_info`,
// `sqlite_master`), `SELECT … JOIN` shapes outside the `_sql` macro, and any
// raw-SQL projection the LINQ chain can't (or shouldn't) translate.
//
// Row-shape constraint: T must be a `[sql_table]` struct so the macro-emitted
// `_sql_read_row(stmt, type<T>) : T` is in scope. The struct does not have to
// correspond to a real table — `[sql_table]` on a read-only row shape is the
// idiomatic way to opt in to the typed materializer without committing to a
// CREATE TABLE.
//
// Column order is positional: SELECT projection order must match the struct's
// field declaration order. For name-driven mapping, drop to `try_run_select` /
// `run_select` with an explicit row block.
// ============================================================================

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

// ============================================================================
// User-facing CRUD over [sql_table] structs
//
// Each generic forwards to the per-struct helpers emitted by the macro.
// Type witness is passed through to the helper as `t` (`type<Car>`-style call
// site) or via a row instance (`row` argument also matches the helper's
// `(typ : Car)` parameter — daslang accepts type-witness AND value-witness in
// the same param slot).
// ============================================================================

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
    // Choose BEGIN/COMMIT vs SAVEPOINT/RELEASE based on ambient state so
    // bulk-insert composes inside an outer ``with_transaction`` block.
    // Without this branch SQLite rejects ``BEGIN IMMEDIATE`` with "cannot
    // start a transaction within a transaction" when nested.
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
        // COMMIT/RELEASE can fail (SQLITE_BUSY, IO error). Roll back so the
        // connection isn't left mid-transaction. Bulk-insert uses its own
        // savepoint name (das_ins_sp) so the rollback path is local.
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
    //! Discards a SqlError. Used by try_insert(array) to swallow ROLLBACK failures while preserving the original error.
}

// ============================================================================
// INSERT OR IGNORE / INSERT OR REPLACE
//
// Variants of ``insert`` that change SQLite's conflict resolution. Both reuse
// the per-struct ``_sql_insert_with_pk_sql`` / ``_sql_insert_no_pk_sql`` SQL
// emitters and ``_sql_bind_row`` bind code; the only difference is a textual
// ``OR IGNORE`` / ``OR REPLACE`` modifier inserted after ``INSERT`` and a
// rows-affected return (via ``sqlite3_changes``) rather than ``last_insert_rowid``.
//
// ``OR IGNORE``: silent no-op on PK / UNIQUE violation. Returns 0 for the
//   conflicting row, 1 for the inserted row.
// ``OR REPLACE``: deletes the conflicting row, then inserts the new one. Be
//   careful — any column not in the new row gets its DEFAULT (rarely what you
//   want for partial updates; tut 21's UPSERT ON CONFLICT DO UPDATE is the
//   proper merge form).
// ============================================================================

def private with_or_clause(plain_sql : string; or_clause : string) : string {
    //! Insert `OR IGNORE` / `OR REPLACE` between `INSERT` and `INTO` /
    //! `DEFAULT VALUES`. The plain SQL always begins with ``INSERT `` so the
    //! substitution target is unambiguous.
    if (empty(or_clause)) {
        return plain_sql
    }
    return replace(plain_sql, "INSERT ", "INSERT {or_clause} ")
}

def private try_insert_or(db : SqlRunner; row : auto(TT); or_clause : string) : Result<int, string> {
    //! Shared body for `try_insert_or_ignore` / `try_insert_or_replace` (single row).
    //! Returns ``Ok(rows_affected)`` — 0 if the row was IGNORE-d, 1 otherwise.
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
    //! Shared body for `try_insert_or_ignore` / `try_insert_or_replace` (bulk).
    //! Wraps the per-row prepared-statement loop in a transaction (BEGIN/COMMIT
    //! at the outer level, SAVEPOINT/RELEASE when nested). Returns the SUM of
    //! per-statement ``sqlite3_changes`` over the batch.
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

// ============================================================================
// by-PK UPDATE / DELETE
//
// Single-row mutators driven by [sql_table]'s generated `_sql_update_by_pk_sql`,
// `_sql_delete_by_pk_sql`, and `_sql_bind_row_for_update` helpers. The struct
// must declare exactly one @sql_primary_key field; otherwise the generated
// helper panics with a clear message at call time (see make_update_by_pk_sql_fn
// at the bottom of this file).
//
// All four functions return ``int`` (rows-affected — 1 or 0). 0 is NOT an
// error: the row simply wasn't there. Reserve ``Err`` for genuine SQL
// failures (constraint violation, IO, BUSY).
// ============================================================================

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
    // The PK is bound at index 1 (the only placeholder in `DELETE FROM T WHERE pk=?`).
    // We can't reuse `_sql_bind_row_for_update` here because it binds non-PK
    // columns first; instead, look up the PK field directly via the generated
    // _sql_pk_value helper that matches each [sql_table] struct.
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

// ============================================================================
// Transactions
//
// `with_transaction` blocks emit BEGIN/COMMIT/ROLLBACK or
// SAVEPOINT/RELEASE/ROLLBACK TO depending on whether the connection is
// already inside a transaction (detected via sqlite3_get_autocommit). All
// nested calls use the same SAVEPOINT name `das_sp` — SQLite resolves
// RELEASE/ROLLBACK TO against the most-recent matching savepoint, so proper
// LIFO nesting composes correctly without the runner tracking depth itself.
//
// Two overloads, not one optional-parameter function: piping puts the block
// last, so an optional middle `mode` parameter wouldn't resolve from a
// no-mode call site. See API_REWORK.md § "12 + 13 + 14" for the full
// rationale.
// ============================================================================

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
    // Best-effort: swallow rollback errors so the original failure (panic
    // message or BEGIN/blk SQL error) reaches the caller intact.
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
            // COMMIT/RELEASE can fail (SQLITE_BUSY, IO error). If it does, roll
            // back so the connection isn't left mid-transaction, then panic
            // with the commit message.
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
    // COMMIT/RELEASE can fail (SQLITE_BUSY, IO error). If it does, roll back
    // so the connection isn't left mid-transaction before returning the error.
    let commit_err = db |> try_exec(txn_commit_sql(was_in_txn))
    if (commit_err |> is_some) {
        txn_best_effort_rollback(db, was_in_txn)
    }
    return commit_err
}

// ============================================================================
// Generic DML runners — used by the `_sql_update` / `_sql_delete` macros.
//
// These take a SQL string + bind block (the macro fills in the
// `sql_bind_to_stmt(stmt, idx, expr)` calls) and execute it. Return rows-affected
// for non-RETURNING; array<T> for RETURNING.
// ============================================================================

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

// ============================================================================
// _sql macro runtime: try_run_select / run_select
//
// The `_sql` macro emits a call to one of these. `bind_blk` runs once after
// prepare to push bound values; `row_blk` is invoked per row to materialize
// the typed result. Generic on the row type (`auto(TT)`) so the call site
// determines whether the array is `array<Car>`, `array<string>`, or any
// other projected shape.
// ============================================================================

def try_run_select(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : Result<array<TT -const -#>, string> {
    //! Prepares ``sql``, runs ``bind_blk`` once, then steps the cursor and
    //! materializes each row via ``row_blk``. Returns ``Err(errmsg)`` on
    //! prepare or step failure.
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

// ============================================================================
// _sql macro runtime: single-row materializers (One / OneOpt)
//
// Used by `_sql(...)` chains that end in `_first()`, `_first_opt()`, or
// `_count()` (and chunk-4 aggregates). One panics-on-empty (for `_first`,
// `_count`, future `_sum`-on-empty); OneOpt returns Option<T>.
// ============================================================================

def try_run_select_one(db : SqlRunner; sql : string; bind_blk : block<(var stmt : sqlite3_stmt?) : void>; row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>) : Result<TT -const -#, string> {
    //! Prepares ``sql``, runs ``bind_blk`` once, steps once, materializes
    //! the first row via ``row_blk``. Returns ``Err(errmsg)`` on prepare or
    //! step failure or zero rows.
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

// ============================================================================
// [sql_table] structure macro
//
// Reads struct-level args (`name="..."` overrides the on-disk table name)
// and per-field annotations (`@sql_primary_key` for now). Generates, attached
// to the user's struct type, a fixed family of helper functions consumed by
// `create_table` / `drop_table_if_exists` / `insert` below.
//
// All generated function bodies emit `sql_storage_type_for(type<T>)` and
// `sql_bind_to_stmt(stmt, idx, row.field)` calls — so primitive and user-
// defined field types travel the same code path. ``sql_bind_to_stmt``'s
// catch-all routes the value through the user's ``_::sql_bind`` adapter
// (resolved at the caller's-module overload set). Type resolution defers
// to each generated function's own typecheck pass; the macro itself does
// not depend on field types being fully resolved at apply() time.
//
// TODO: 5 of the 7 emitted helpers (_sql_table_name, _sql_drop_table_if_exists_sql,
// _sql_create_table_sql, _sql_insert_with_pk_sql, _sql_insert_no_pk_sql) take
// `typ : T` and never read it — and so does the no-PK fallback variant of
// _sql_pk_is_unset. Currently silenced by the module-level
// `options no_unused_function_arguments = false`. Switch to per-fn
// `[unused_argument(typ)]` annotations attached at macro-expansion time so
// stricter callers/linters get clean per-function info. Settle after
// chunk-1 review.
// ============================================================================

def private find_annotation(args : AnnotationArgumentList; argn : string; default_value : auto(TT)) : TT {
    //! Generic annotation reader. ``TT`` must be ``bool`` or ``string``;
    //! daslang's ``static_if (typeinfo is_string …)`` picks the matching
    //! ``tString`` / ``tBool`` accessor on the AnnotationArgument variant.
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
    //! Reads a parens-list field annotation `@argn=("A","B",...)` or the bare single
    //! `@argn="A"` form. The parser flattens parens-lists into multiple flat entries
    //! all sharing the parent name, so a single by-name walk handles both shapes.
    return <- [for (a in args); string(a.sValue); where a.name == argn && a.basicType == Type.tString]
}

// Whitelisted SQL-side default functions (`@sql_default_fn=...`). These are
// deliberately enumerated rather than passed-through verbatim — typo-detection
// at macro-expansion time gives a clearer error than a runtime SQLite parse error.
let private SQL_DEFAULT_FN_WHITELIST = ["CURRENT_TIMESTAMP", "CURRENT_DATE", "CURRENT_TIME"]

let private SQL_FK_ACTION_WHITELIST = ["cascade", "set_null", "set_default", "restrict", "no_action"]

def private fk_action_to_sql(action : string) : string {
    //! Map `@sql_on_delete` / `@sql_on_update` value to its DDL form.
    //! Caller has already validated against `SQL_FK_ACTION_WHITELIST`.
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
    //! Wrap `s` in single quotes, doubling any embedded single quote. Used for
    //! string-typed `field : string = "default"` initializers in the DDL emitter.
    let escaped = replace(s, "'", "''")
    return "'{escaped}'"
}

def private literal_init_to_default_clause(init : ExpressionPtr) : string {
    //! Translate a daslang field initializer expression to `" DEFAULT <literal>"`.
    //! Returns `""` when the expression is null or not a recognized literal — non-
    //! literal initializers (e.g. `some(5)` for `Option<int>`) are silently
    //! ignored so the daslang in-memory default still compiles, just without an
    //! SQL-side counterpart.
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
    is_stored : bool                //!< false → VIRTUAL (SQLite default), true → STORED
    is_json : bool                  //!< @sql_json — TEXT-backed via daslib/json round-trip; `_sql` walker descends as JSON path
    is_blob : bool                  //!< @sql_blob — BLOB-backed via daslib/archive; opaque to `_sql`
    computed_sql : string           //!< body of `GENERATED ALWAYS AS (...)`; non-empty iff is_computed
    default_clause : string         //!< pre-built ` DEFAULT <expr>` or `""` when no default applies
    references_clause : string      //!< pre-built ` REFERENCES "T"("Pk") ON DELETE X ON UPDATE Y` or `""`
}

def private is_option_field_type(td : TypeDeclPtr) : bool {
    //! Recognize ``Option<T>`` for the [sql_table] DDL emitter (drop NOT NULL,
    //! reject @sql_primary_key on the field). Structure macros run before the
    //! ``[template_structure]`` typemacro has been resolved, so a field
    //! declared as ``Option<int>`` arrives as ``Type.typeMacro`` with the
    //! macro name in ``dimExpr[0]``. We also accept the post-resolution form
    //! (``Type.tStructure`` with structType.name == "Option", module ==
    //! "option") so the helper works even if a later pass reaches it.
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
    //! Walk every `[sql_index(...)]` on ``st`` and concatenate the CREATE INDEX
    //! statements. Each `fields=` entry is cross-checked against ``field_names``.
    //! On validation failure, sets ``ok = false``, populates ``errors``, and
    //! returns ``""``. Returns ``""`` when no indexes are present (also with
    //! ``ok = true``) so the caller can skip a sqlite3_exec call.
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
    //! Read back a `return "literal"` body emitted by `qmacro_function ... {
    //! return $v(sql) }`. Returns the string value or `""` when the body
    //! doesn't match the expected single-return-of-string-literal shape.
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
    //! Locate a `[sql_table]`-generated helper function by name whose first
    //! parameter type resolves to ``st``. Returns ``null`` when no overload
    //! matches the struct (e.g. ``[sql_index]`` was placed before its
    //! ``[sql_table]`` and the helper isn't generated yet).
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

        // Cross-check every named column against the struct's fields, and
        // build the daslang→SQL column-name mapping in the same pass. The
        // user writes `fields="col_type"` (the DASLANG field name); the
        // emitted DDL uses the @sql_column-renamed SQL identifier. Annotations
        // stay in daslang-space; renames are transparent at emission time.
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

        // Locate the per-struct `_sql_create_indexes_sql` placeholder emitted
        // by [sql_table]. Source-order rule: [sql_table] must precede
        // [sql_index] in the annotation list.
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

        // Build the CREATE INDEX statement for THIS annotation. Auto-name uses
        // SQL column names so two structs with the same daslang field name
        // mapped to different SQL columns get distinct index names.
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

        // Read the current accumulated SQL (empty after [sql_table], or the
        // result of earlier [sql_index] applies on the same struct), append
        // this statement, and replace fn.body.
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
    //! Resolve `@sql_references="ParentName"` to the parent struct's
    //! `[sql_table(name=...)]` table name and `@sql_primary_key` column. Walks
    //! `compiling_module()`; self-reference (`parent_name == st.name`) short-
    //! circuits to the current struct since `[sql_table]` is being applied to
    //! it right now.
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
            // The PK column may have been renamed via @sql_column on the
            // parent struct's PK field — read that here so the FK references
            // the SQL identifier, not the daslang field name.
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

            // @sql_json / @sql_blob — opt in to non-scalar column storage. `[sql_table]`
            // generates a `sql_bind` / `sql_extract` adapter pair for the field's
            // declared type; `_sql` walker descends into @sql_json columns as JSON
            // paths and rejects descent into @sql_blob columns.
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

            // @sql_column = "<sql_name>" — rename the field's SQL column so a
            // daslang struct field can shadow a SQL keyword (`type`, `for`, …)
            // or use a different on-disk column name. We DON'T use
            // find_annotation here because that helper can't distinguish
            // "absent" from `= ""`; both return the default. find_arg + is tString
            // surfaces the missing-vs-empty case so we can reject the empty
            // form with a clear error.
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
        // After all per-field annotation reads complete, sweep `fields` once
        // for duplicate `col_name` values. A collision can come from two
        // fields renaming to the same SQL column, OR from one renamed field
        // shadowing another field's daslang name (e.g. `@sql_column = "B"` on
        // field `A` when there's already a sibling field literally named `B`).
        // Either case would emit malformed DDL (CREATE TABLE with two
        // `"X"` columns) — fail loud at macro time with a precise pointer.
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

        // ---- @sql_json / @sql_blob adapter codegen ----
        // Emit FIRST (before the per-table helpers below) so the adapter
        // overloads are visible to overload resolution when the helpers
        // (`_sql_read_row` / `_sql_bind_row`) get type-checked, both of which
        // call into the chunk-8 `_::sql_bind` / `_::sql_extract` rail.
        //
        // Dedup is **per-(T, kind)**: an existing `sql_bind(T) : string`
        // satisfies a fresh @sql_json field, `sql_bind(T) : array<uint8>`
        // satisfies @sql_blob — but using the same T with a different kind
        // (across structs in this module, or across fields of one struct)
        // would silently miscompile, so it's rejected as a compile error.
        // Within-struct: `seen_kinds` maps describe(T) -> expected return
        // (`"string"` for JSON, `"array<uint8>"` for BLOB). Cross-struct:
        // `find_user_adapter_return_type` looks up any existing `sql_bind(T)`
        // overload in the module and reports its return primitive.
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
                    errors := "[sql_table]: field '{info.name}' is {kind_label}, but an earlier field of struct '{st.name}' already used type '{key}' with {prev_label}. Adapter dedup is per-(T, kind); the same T must be used with one storage kind across this struct. Pick one kind, or wrap '{key}' in a typedef so the two storage views are distinct types."
                    return false
                }
                continue
            }
            seen_kinds |> insert(key, expected_kind)
            let existing_kind = find_sql_bind_kind_for_type(compiling_module(), ft)
            if (existing_kind != SqlAdapterKind.None) {
                if (existing_kind != expected_kind) {
                    let existing_label = existing_kind == SqlAdapterKind.Json ? "@sql_json (sql_bind returns string)" : (existing_kind == SqlAdapterKind.Blob ? "@sql_blob (sql_bind returns array<uint8>)" : "an unrelated `sql_bind` overload")
                    errors := "[sql_table]: field '{info.name}' is {kind_label}, but type '{key}' is already bound by {existing_label} (from a sibling annotation or a hand-written adapter). Adapter dedup is per-(T, kind); the same T must be used with one storage kind. Pick one kind, or wrap '{key}' in a typedef so the two storage views are distinct types."
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

        // [sql_index] sibling-annotation processing happens in finish() —
        // st.annotations is not yet populated during apply(). The helper
        // function `_sql_create_indexes_sql` is added to the module there.
        var fn_table_name = make_table_name_fn(st, table_name)
        compiling_module() |> add_function(fn_table_name)
        var fn_drop = make_drop_sql_fn(st, table_name)
        compiling_module() |> add_function(fn_drop)
        var fn_create = make_create_table_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_create)
        // Empty `_sql_create_indexes_sql` placeholder. Sibling [sql_index]
        // annotations rewrite this body to accumulate CREATE INDEX statements.
        // User-facing source order requirement: [sql_table, sql_index, ...].
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
            // Compose the column suffix:
            //   - computed: ` GENERATED ALWAYS AS (...) VIRTUAL|STORED` and nothing
            //     else; SQLite rejects NOT NULL / DEFAULT / UNIQUE / FK on a
            //     generated column.
            //   - PK: ` PRIMARY KEY`. UNIQUE is implied; DEFAULT may still fire if
            //     the user gave a literal init.
            //   - Otherwise: NOT NULL (unless Option<T>), UNIQUE, DEFAULT,
            //     REFERENCES — in DDL order.
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
            // @sql_json / @sql_blob short-circuit `sql_storage_type_for` (which would
            // fail for struct/tuple T anyway). The annotation pins storage class:
            // TEXT for json, BLOB for blob — adapter codegen below produces the
            // matching `sql_bind` / `sql_extract` overloads that bind the column
            // through that storage class.
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
            // Strip the leading " DEFAULT " prefix that `default_clause` carries
            // for direct DDL concatenation. ``default_expr`` is the raw SQL
            // expression (`"1"` / `"'x'"` / `"CURRENT_TIMESTAMP"`) — readable
            // and aligned with PRAGMA table_info's ``dflt_value`` column.
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
        // Always emit explicit column list so computed (GENERATED) columns can be
        // skipped — SQLite rejects writes to generated columns. ``include_pk``
        // toggles whether the primary key column participates: with-PK is the
        // user-specified-keys path, without-PK is the AUTOINCREMENT path.
        // Both fall back to `DEFAULT VALUES` when no bindable columns remain
        // (PK-only struct without PK, or all-computed struct).
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
        // Bind indices follow the column-list emitted by `_sql_insert_with_pk_sql`
        // / `_sql_insert_no_pk_sql` — both skip computed columns. The index is
        // a running counter over bindable (non-computed) columns; it is NOT
        // the field's position in `fields`.
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

        // `var row = default<$t(st)>` — explicit default so structs with
        // Option<T> fields (which lack a builtin per-field default and rely
        // on `@safe_when_uninitialized` only in the value position) still
        // satisfy strict_smart_pointers' uninitialized-var check inside the
        // generated `_sql_read_row` body.
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
        // PK-less / SET-less structs get a stub that panics at runtime — using
        // ``concept_assert`` here would fire at compile-time on every
        // ``[sql_table]`` declaration (the function body is type-checked
        // unconditionally). The stub stays unreachable in practice because
        // ``update(row)`` panics from the same function before binding runs.
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
        // PK-only / all-computed struct (no SET columns): `UPDATE T SET WHERE pk=?`
        // is invalid SQL. Stub panics at call time with a clear message.
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
        // Emits SET-then-WHERE binds: every non-PK column at indices 1..N,
        // PK column at index N+1. Mirrors the SQL emitted by
        // `_sql_update_by_pk_sql`. For PK-less / SET-less structs the stub
        // never runs (the SQL helper panics first), but we still emit a
        // valid no-op body so the surface compiles.
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
        // Binds just the PK column at index 1. Used by `try_delete_(row)` to
        // execute `DELETE FROM T WHERE pk=?` without needing every field set.
        // PK-less structs get a no-op stub (the SQL helper panics first).
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

    // ---- @sql_json adapter codegen ----
    //
    //   def sql_bind    (v : T) : string          { return write_json(JV(v)) }
    //   def sql_extract (v : string; t : type<T>) : T {
    //       var err = ""
    //       let jv = read_json(v, err)
    //       if (err != "") {
    //           panic("sql_extract: malformed JSON in column data: {err}")
    //       }
    //       return from_JV(jv, default<T>)
    //   }
    //
    // The pair plugs into the chunk-8 `_::sql_bind` / `_::sql_extract` rail.
    // Storage class is TEXT (driven by the `string` return of `sql_bind`).
    // Round-trip fidelity matches `daslib/json_boost`'s `JV` / `from_JV`
    // surface (struct, tuple, array, table, variant, enum, primitives).
    //
    // Note on `clone_type`: a field's `_type` arrives with the field-context
    // modifier flags (ref/const/etc.) that come from being read inside a struct.
    // Substituting the raw decl into a function parameter yields a slightly
    // different type than a hand-written `(v : T)` — overload resolution then
    // fails to pick the emitted adapter and falls through to the chunk-8
    // catch-all. Clone the decl and use the fresh copy so the substituted type
    // matches a hand-written reference exactly.
    def make_json_sql_bind_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
        // The hand-written form `def sql_bind(v : tuple<...>) : string` is
        // parsed with `TypeDeclFlags.constant = true` on the parameter
        // (struct/tuple params are pass-by-ref and the parser implicitly
        // marks them as ``const`` to keep the body immutable). When
        // `[sql_table]` calls `_::sql_bind(v)` through the chunk-8 catch-
        // all, the argument arrives with `const explicit` flags from the
        // generic instantiation — and a non-const parameter rejects a
        // const argument, dropping our overload from the candidate set.
        // Match the parsed form: clone the field type and turn on
        // ``constant`` so resolution accepts the const argument.
        var t1 <- clone_type(field_type)
        t1.flags |= TypeDeclFlags.constant
        var fn = qmacro_function("sql_bind") $(v : $t(t1)) : string {
            return _::write_json(_::JV(v))
        }
        fn.body |> force_at(at)
        return <- fn
    }

    def make_json_sql_extract_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
        var t1 <- clone_type(field_type)
        t1.flags |= TypeDeclFlags.constant
        var t2 <- clone_type(field_type)
        var t3 <- clone_type(field_type)
        // Panic on malformed JSON (matching the @sql_blob symmetry where
        // `mem_archive_load` panics on corrupt bytes). Without the check,
        // `read_json` returning null silently feeds into `from_JV` which
        // returns `default<T>` — schema drift / disk corruption then
        // surfaces as zero-initialized rows, masking real data loss.
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

    // ---- @sql_blob adapter codegen ----
    //
    //   def sql_bind    (v : T) : array<uint8>            { var v_local : T; v_local := v; return <- mem_archive_save(v_local) }
    //   def sql_extract (v : array<uint8>; t : type<T>) : T {
    //       var r = default<T>
    //       _::mem_archive_load(v, r)   // panics on failure (canfail=false)
    //       return r
    //   }
    //
    // Storage class is BLOB. The opaqueness is enforced by `_sql`'s walker:
    // any `_.<blob_col>.<path>` chain in a predicate produces a compile error
    // pointing at the field, since `daslib/archive` data is positional binary
    // and SQL has no way to introspect it.
    //
    // `mem_archive_save(var t : auto&)` requires a mutable reference, but our
    // adapter is reached through the chunk-8 generic `sql_bind_to_stmt(... v :
    // auto(TT))` which holds `v` as a non-var binding. Clone into a local var
    // to bridge the const/var gap without taking `var v` ourselves (which would
    // force the catch-all to also take var, rippling up to the row binder).
    def make_blob_sql_bind_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
        var t1 <- clone_type(field_type)
        t1.flags |= TypeDeclFlags.constant
        var fn = qmacro_function("sql_bind") $(v : $t(t1)) : array<uint8> {
            var v_local <- clone_to_move(v)
            return <- _::mem_archive_save(v_local)
        }
        fn.body |> force_at(at)
        return <- fn
    }

    def make_blob_sql_extract_fn(field_type : TypeDeclPtr; at : LineInfo) : FunctionPtr {
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
}

// SqlAdapterKind — discriminates an existing `sql_bind(T)` overload by its
// return-type shape, used for [sql_table] @sql_json / @sql_blob dedup.
//   None — no overload of `sql_bind(T)` exists for T in the module.
//   Json — overload returns `string` (matches @sql_json codegen).
//   Blob — overload returns `array<uint8>` (matches @sql_blob codegen).
//   Other — overload exists but returns something else (user-supplied or
//           a stale generated form); always treated as a kind conflict.
enum private SqlAdapterKind {
    None
    Json
    Blob
    Other
}

def private classify_sql_bind_return(td : TypeDeclPtr) : SqlAdapterKind {
    //! Map a `sql_bind(T)` return type to its adapter kind. JSON returns are
    //! `string`; BLOB returns are `array<uint8>`. Anything else is "Other"
    //! and treated as a conflict.
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
    //! Walk every overload of ``sql_bind`` in ``mod`` and report the kind of
    //! the one whose first parameter is type-equivalent to ``td`` (ignoring
    //! const/ref/temp). Returns ``None`` when no matching overload exists.
    //! Used by [sql_table] @sql_json / @sql_blob adapter codegen for
    //! kind-aware dedup: a JSON kind matches @sql_json reuse, Blob kind
    //! matches @sql_blob reuse; ``Other`` (or kind mismatch) is a compile
    //! error.
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
