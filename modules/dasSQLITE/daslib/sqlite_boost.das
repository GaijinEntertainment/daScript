options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

// Intentionally opt in for user-facing docs: keep comments concise and actionable.
options _comment_hygiene = true

module sqlite_boost shared public

require sqlite public
require sqlite/sqlite_provider public       // registration shim: sqlite_sql_type, stmt factories, schema readers
require daslib/sql_boost public             // provider-neutral macro layer: [sql_table]/[sql_view]/[sql_index], adapter rail

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

def sqlite3_free(std : string) {
    //! Frees memory previously allocated by SQLite.
    sqlite3_free(unsafe(reinterpret<void?>(std)))
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

// ===== SQL provider registration (see modules/dasSQLITE/PROVIDER_CONTRACT.md) =====

// This module's macros ([sql_fts5]) may consult the provider registry, and only a
// module's own [_macro] functions run at its macro build (linq_fold.das:123 pattern)
// — so it registers the present providers from its own [_macro].
[_macro]
def private _boost_register_sql_providers {
    if (!is_compiling_macros_in_module("sqlite_boost")) return
    sql_register_present_providers()
}

def finalize(var db : SqlRunner) {
    //! Closes the underlying SQLite connection. Triggered automatically by `var inscope db`.
    if (db.sqlite_handle != null) {
        sqlite3_close_v2(db.sqlite_handle)
        db.sqlite_handle = null
    }
}

def sqlite_version() : string {
    //! Returns the linked libsqlite3 library version (e.g. "3.45.0"). No DB connection needed.
    return sqlite3_libversion()
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
                var row : TT -const -# = invoke(row_blk, stmt) // nolint:LINT003,LINT019 - move_ok consumes row, so it must stay var; LINT003 fires only in downstream instantiations of this generic
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
                var row : TT -const -# = invoke(row_blk, stmt) // nolint:LINT003,LINT019 - move_some/move_ok consume row, so it must stay var; LINT003 fires only in downstream instantiations of this generic
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
def try_check_schema(db : SqlRunner; t : type<auto(TT)>) : SqlError {
    //! Verifies the open ``db`` matches struct ``T`` on (name, SqlType, NOT NULL, PRIMARY KEY).
    //! Returns ``some(errmsg)`` on first mismatch — startup-defense pattern for non-self-created DBs.
    let want = _::_sql_column_info(default<TT>)
    let table_name = _::_sql_table_name(default<TT>)
    var got_result <- sqlite_read_table_info(db.sqlite_handle, db.schema_name, table_name)
    if (got_result |> is_err) return some(got_result |> unwrap_err)
    let got <- got_result._value
    if (empty(got)) return some("check_schema: table \"{table_name}\" not found in DB.")
    if (length(want) != length(got)) return some("check_schema: \"{table_name}\" column count mismatch - struct has {length(want)} field(s), DB has {length(got)} column(s).")
    // Name-keyed: [sql_table] emits SELECTs with explicit columns, so struct/DB order can differ.
    // With equal counts, missing-from-struct is implied by no name match in the want-loop.
    var by_name : table<string; int>
    for (j in range(length(got))) {
        by_name |> insert(got[j].name, j)
    }
    for (w in want) {
        let g_idx = by_name?[w.name] ?? -1
        if (g_idx < 0) return some("check_schema: \"{table_name}\" struct field \"{w.name}\" not found in DB columns.")
        let g = got[g_idx]
        let g_type = g.data_type
        if (w.data_type != g_type) return some("check_schema: \"{table_name}\".\"{w.name}\" SqlType mismatch - struct expects {sqlite_sql_type(w.data_type)}, DB has {sqlite_sql_type(g_type)} (declared \"{g.decl_type}\").")
        // SQLite quirk: INTEGER PRIMARY KEY reports notnull=0 in pragma_table_info — treat PK as NOT NULL.
        let g_nullable = !g.not_null && !g.pk
        if (w.is_nullable != g_nullable) {
            if (w.is_nullable) {
                return some("check_schema: \"{table_name}\".\"{w.name}\" nullability mismatch - struct allows NULL (Option<T>), DB column is NOT NULL.")
            } else {
                return some("check_schema: \"{table_name}\".\"{w.name}\" nullability mismatch - struct is non-optional, DB column allows NULL.")
            }
        }
        if (w.is_pk != g.pk) {
            if (w.is_pk) {
                return some("check_schema: \"{table_name}\".\"{w.name}\" PK mismatch - struct has @sql_primary_key, DB column is not PRIMARY KEY.")
            } else {
                return some("check_schema: \"{table_name}\".\"{w.name}\" PK mismatch - DB column is PRIMARY KEY but struct field is missing @sql_primary_key.")
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
    //! ROLLBACK fires on early return (block ``return`` unwinds through ``finally``); a
    //! panic skips ``finally`` by design — the open txn discards when the connection closes.
    let was_in_txn = db |> in_transaction()
    db |> exec(txn_begin_sql(was_in_txn, mode))
    var success : bool
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
    var success : bool
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
        $(var _stmt : sqlite3_stmt?) {},
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
// Self-contained mode only: indexed content fields must be string/Option<string>;
// @sql_fts_unindexed stores typed metadata without tokenizing it; @sql_fts_rank is
// read-only and maps to FTS5's hidden `rank` on SELECT.

[structure_macro(name=sql_fts5)]
class private SqlFts5Macro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {  // nolint:STYLE037,STYLE038 - one arm per rejected annotation shape, then a flat one-call-per-helper emission list
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
            let is_fts_unindexed = find_annotation(field.annotation, "sql_fts_unindexed", false)
            // Reject every annotation that doesn't apply to a virtual table.
            for (rej in FTS5_REJECTED_ANNOTATIONS) {
                var present = false
                if (rej.is_bool) {
                    present = find_annotation(field.annotation, rej.name, false)
                } else {
                    present = !empty(find_annotation(field.annotation, rej.name, ""))
                }
                if (present) {
                    errors := "[sql_fts5]: field '{fname}' has @{rej.name} - {rej.why}"
                    return false
                }
            }
            // @sql_column rename, as in [sql_table]; rejected on @sql_fts_rank (hardcoded to FTS5's hidden `rank`).
            var col_name = fname
            let col_anno = find_arg(field.annotation, "sql_column")
            if (col_anno is tString) {
                if (is_fts_rank) {
                    errors := "[sql_fts5]: field '{fname}' has both @sql_fts_rank and @sql_column - rank is hardcoded to FTS5's hidden `rank` column. Drop the @sql_column rename."
                    return false
                }
                let cv = col_anno as tString
                if (empty(cv)) {
                    errors := "[sql_fts5]: field '{fname}' @sql_column = \"\" is invalid - provide a non-empty SQL column name."
                    return false
                }
                if (find(cv, "\"") >= 0 || find(cv, "\\") >= 0) {
                    errors := "[sql_fts5]: field '{fname}' @sql_column=\"{cv}\" - embedded `\"` / `\\` are rejected (SQL identifier hygiene). Use a plain identifier."
                    return false
                }
                col_name = cv
            }
            // @sql_fts_rank: read-only float column, reads FTS5's hidden `rank`. No daslang init allowed.
            if (is_fts_rank) {
                if (is_fts_unindexed) {
                    errors := "[sql_fts5]: field '{fname}' cannot be both @sql_fts_rank and @sql_fts_unindexed - rank is FTS5's hidden relevance column, not stored metadata."
                    return false
                }
                col_name = "rank"   // FTS5 hidden column — not a declared content column
                if (field.init != null) {
                    errors := "[sql_fts5]: field '{fname}' is @sql_fts_rank - read-only column populated from FTS5's hidden `rank` on SELECT. Remove the daslang field initializer."
                    return false
                }
                if (is_optional) {
                    errors := "[sql_fts5]: field '{fname}' is @sql_fts_rank - must not be Option<T>. FTS5 always materializes a relevance score on a MATCH-using SELECT."
                    return false
                }
                if (field._type.baseType != Type.tFloat && field._type.baseType != Type.tDouble) {
                    errors := "[sql_fts5]: field '{fname}' is @sql_fts_rank - must be a float / double (FTS5 BM25 score). Got '{describe(field._type)}'."
                    return false
                }
                rank_count++
            } elif (is_fts_unindexed) {
                // Stored metadata: typed by the ordinary sql_bind/sql_extract rail; FTS5's UNINDEXED excludes it from MATCH.
            } else {
                // Content column: string or Option<string>.
                let bt = field._type.baseType
                let is_text = ((bt == Type.tString)
                            || (is_optional && field._type.firstType != null && field._type.firstType.baseType == Type.tString))
                if (!is_text) {
                    errors := "[sql_fts5]: field '{fname}' has unsupported type '{describe(field._type)}' - FTS5 content columns must be `string` or `Option<string>`. Tag the field with `@sql_fts_rank` if it's the relevance score column."
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
                is_fts_unindexed = is_fts_unindexed,
                computed_sql = "",
                default_clause = "",
                references_clause = ""))
        }
        if (content_count == 0) {
            errors := "[sql_fts5] on struct '{st.name}': must declare at least one indexed content column (`string` or `Option<string>`). A struct with only @sql_fts_unindexed metadata / @sql_fts_rank fields has nothing to index."
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
                errors := "[sql_fts5] on struct '{st.name}': sibling [sql_index] is not supported - FTS5 builds its own inverted index. Drop the [sql_index]."
                return false
            }
        }
        // ---- generated helper functions ----
        let create_sql = build_fts5_create_table_sql(table_name, fields)
        let select_all_sql = build_fts5_select_all_sql(table_name, fields)
        let insert_sql = build_fts5_insert_sql(table_name, fields)
        let st_name = string(st.name)
        let panic_msg_pk = "[sql_fts5] {st_name}: cannot {0} on an FTS5 virtual table - row-shaped UPDATE/DELETE-by-PK is not supported. Use `_sql_delete(type<{st_name}>, predicate)` for typed predicate deletes; UPDATE still requires delete + re-INSERT or raw SQL."
        let panic_msg_bind = "[sql_fts5] {st_name}: cannot {0} on an FTS5 virtual table - row-shaped mutation helpers are not supported. Use `_sql_delete(type<{st_name}>, predicate)` for typed predicate deletes; UPDATE still requires delete + re-INSERT or raw SQL."
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
        compiling_module() |> add_generic(fn_read)
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
        // Row-shaped UPDATE/DELETE-by-PK are unsupported (panic stubs); predicate `_sql_delete(type<T>, pred)` still works.
        var fn_upd_pk = make_string_panic_stub_helper_fn(st, "_sql_update_by_pk_sql",
            panic_msg_pk |> replace("{0}", "update by primary key"))
        compiling_module() |> add_function(fn_upd_pk)
        var fn_del_pk = make_string_panic_stub_helper_fn(st, "_sql_delete_by_pk_sql",
            panic_msg_pk |> replace("{0}", "delete by primary key"))
        compiling_module() |> add_function(fn_del_pk)
        var fn_bind_upd = make_void_panic_stub_helper_fn(st, "_sql_bind_row_for_update",
            panic_msg_bind |> replace("{0}", "update"), false)
        compiling_module() |> add_generic(fn_bind_upd)
        var fn_bind_pk = make_void_panic_stub_helper_fn(st, "_sql_bind_row_pk_only",
            panic_msg_bind |> replace("{0}", "delete by primary key"), false)
        compiling_module() |> add_generic(fn_bind_pk)
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
            if (info.is_fts_unindexed) {
                w |> write(" UNINDEXED")
            }
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
// Direct, connection-scoped rail. SqlFnTag + sql_fn_tag_for_type and the
// program-scoped [sql_function] annotation live in daslib/sql_boost.

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
                    "register_function: argument #{i + 1} of fn has unsupported type '{describe(fnExpr._type.argTypes[i])}' - supported: int, int64, float, double, bool, string")
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
                "register_function: return type '{describe(fnExpr._type.firstType)}' is unsupported - supported: int, int64, float, double, bool, string")
            return null
        }
        var dbExpr   = call.arguments[0]
        var nameExpr = call.arguments[1]
        // Optional positional flags. Default both to false.
        var detExpr : ExpressionPtr = nCallArgs >= 4 ? clone_expression(call.arguments[3]) : qmacro(false)
        var donExpr : ExpressionPtr = nCallArgs >= 5 ? clone_expression(call.arguments[4]) : qmacro(false)
        let nArgsLit = nArgs
        let tag0 = tags[0]
        let tag1 = tags[1]
        let tag2 = tags[2]
        let tag3 = tags[3]
        let retTag = retR.tag
        // Emit: helper checks rc and panics on failure with the libsqlite3 errmsg.
        var res = qmacro(_::_register_function_check_rc(
            $e(dbExpr), $e(nameExpr), $e(fnExpr), $v(nArgsLit),
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

// ===== [sql_function] install loop — replay the neutral daslib/sql_boost queue =====
// Registers every queued entry against a freshly opened connection; the erased
// carrier passes straight to the das::Func extern parameter.

def private _install_sql_function_registry(db : SqlRunner) : SqlError {
    let dup = sql_function_duplicate_error()
    if (dup |> is_some) {
        return dup
    }
    var install_err : SqlError = none(type<string>)
    for_each_sql_function() $(e) {
        if (install_err |> is_some) {
            return
        }
        let rc = sqlite3_register_function(db.sqlite_handle, e.name, e.fn, e.nArgs,
            e.tag0, e.tag1, e.tag2, e.tag3,
            e.retTag, e.deterministic, e.directonly)
        if (rc != SQLITE_OK) {
            install_err = some("[sql_function]('{e.name}') registration failed: {sqlite3_errmsg(db.sqlite_handle)}")
        }
    }
    return install_err
}

def drop_index_if_exists(db : SqlRunner; name : string) {
    //! Idempotent DROP INDEX. SQLite supports IF EXISTS natively; safe to call on a
    //! missing index. Pair with ``create_unique_index`` for migration patterns that
    //! rebuild an index under a new name.
    db |> exec("DROP INDEX IF EXISTS \"{name}\"")
}
