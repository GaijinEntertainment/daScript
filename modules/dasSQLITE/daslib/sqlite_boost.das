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
    //! Binds a blob value to a prepared statement.
    unsafe {
        return sqlite3_bind_blob(stmt, 1, unsafe(addr(data[0])), data |> length)
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
    //! RAII helper. Opens the DB, invokes the block, finalizes on exit (including on panic / early return).
    var inscope db = open_sqlite(path)
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
// Convention dispatch: sqlite_sql_type / sqlite_bind
//
// `[sql_table]`-generated DDL and INSERT helpers emit calls to these two
// functions for every column. Built-in primitive types are handled by the
// overloads below. User-defined types extend the same surface with their own
// overloads — no registration step:
//
//     def sqlite_sql_type(witness : MyType) : string { return "TEXT" }
//     def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : MyType) : void { ... }
//
// The generic catch-alls fire `concept_assert(false, ...)` for any type
// that doesn't have a specific overload. `concept_assert` reports the error
// at the *caller's* line (the user's `[sql_table]` struct), not deep inside
// this module.
// ============================================================================

[unused_argument(witness)]
def sqlite_sql_type(witness : int) : string {
    return "INTEGER"
}

[unused_argument(witness)]
def sqlite_sql_type(witness : int64) : string {
    return "INTEGER"
}

[unused_argument(witness)]
def sqlite_sql_type(witness : float) : string {
    return "REAL"
}

[unused_argument(witness)]
def sqlite_sql_type(witness : double) : string {
    return "REAL"
}

[unused_argument(witness)]
def sqlite_sql_type(witness : string) : string {
    return "TEXT"
}

[unused_argument(witness)]
def sqlite_sql_type(witness : bool) : string {
    return "INTEGER"
}

[unused_argument(witness)]
def sqlite_sql_type(witness : $Option < auto(TT) >) : string {
    //! Option<T> nullability is type-driven — the column's SQL type comes from the inner ``T``.
    //! Whether a NOT NULL constraint is emitted is decided at macro-expansion time by the
    //! [sql_table] DDL emitter, not here.
    return sqlite_sql_type(default<TT>)
}

[unused_argument(witness)]
def sqlite_sql_type(witness : auto(TT)) : string {
    concept_assert(false, "sqlite_sql_type: unsupported field type. Define `def sqlite_sql_type(witness : YourType) : string` to extend.")
    return ""
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : int) : void {
    sqlite3_bind_int64(stmt, idx, int64(value))
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : int64) : void {
    sqlite3_bind_int64(stmt, idx, value)
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : float) : void {
    sqlite3_bind_double(stmt, idx, double(value))
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : double) : void {
    sqlite3_bind_double(stmt, idx, value)
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : string) : void {
    sqlite3_bind_text(stmt, idx, value)
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : bool) : void {
    sqlite3_bind_int64(stmt, idx, value ? 1l : 0l)
}

def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : $Option < auto(TT) >) : void {
    //! Option<T>: bind the underlying value when ``some``, SQL NULL when ``none``.
    if (value._has_value) {
        sqlite_bind(stmt, idx, value._value)
    } else {
        sqlite3_bind_null(stmt, idx)
    }
}

[unused_argument(stmt, idx, value)]
def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : auto(TT)) : void {
    concept_assert(false, "sqlite_bind: unsupported field type. Define `def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : YourType) : void` to extend.")
}

// ============================================================================
// Read-side dispatch: sqlite_read
//
// Mirrors `sqlite_bind` for the read direction. `[sql_table]`-generated
// `_sql_read_row` and the `_sql` macro's row materializer emit calls to
// `sqlite_read(stmt, col, type<T>) : T` for every column. Built-in primitive
// types are handled by the overloads below; user-defined types extend the
// surface with their own overload — no registration step:
//
//     def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<MyType>) : MyType { ... }
//
// The generic catch-all fires `concept_assert(false, ...)` for any type that
// doesn't have a specific overload. Reports at the caller's line.
// ============================================================================

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<int>) : int {
    return sqlite3_column_int(stmt, col)
}

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<int64>) : int64 {
    return sqlite3_column_int64(stmt, col)
}

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<float>) : float {
    return float(sqlite3_column_double(stmt, col))
}

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<double>) : double {
    return sqlite3_column_double(stmt, col)
}

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<string>) : string {
    return sqlite3_column_text_(stmt, col)
}

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<bool>) : bool {
    return sqlite3_column_int(stmt, col) != 0
}

[unused_argument(t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<$Option<auto(TT)>>) : Option<TT -const -#> {
    //! Option<T>: SQLITE_NULL → ``none``; otherwise read the inner ``T`` and wrap in ``some``.
    if (sqlite3_column_type(stmt, col) == SQLITE_NULL) {
        return none(type<TT -const -#>)
    }
    return some(sqlite_read(stmt, col, type<TT -const -#>))
}

[unused_argument(stmt, col, t)]
def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<auto(TT)>) : TT {
    concept_assert(false, "sqlite_read: unsupported field type. Define `def sqlite_read(stmt : sqlite3_stmt?; col : int; t : type<YourType>) : YourType` to extend.")
    return default<TT>
}

// ============================================================================
// query_scalar — read column 0 of the first row as ``TT``
//
// Generic over any type with a ``sqlite_read(stmt, col, type<TT>) : TT``
// overload. Built-ins: string, int, int64, float, double, bool. User types
// extend by adding their own ``sqlite_read`` overload — no registration step.
//
// `try_*` returns ``Result<TT, string>`` (prepare/step error or zero rows).
// `query_scalar` is the strict variant — panics on err.
// ============================================================================

[unused_argument(t)]
def try_query_scalar(db : SqlRunner; sql : string; t : type<auto(TT)>) : Result<TT -const -#, string> {
    //! Prepares ``sql``, steps once, returns column 0 as ``TT``.
    //! Returns ``Err(errmsg)`` on prepare/step error or zero rows.
    return <- try_one_row(db, sql, $(var stmt : sqlite3_stmt?) {}, $(stmt : sqlite3_stmt?) {
        return sqlite_read(stmt, 0, type<TT -const -#>)
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
        return sqlite_read(stmt, 0, type<TT -const -#>)
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

def private try_one_row(db : SqlRunner; sql : string;
                        bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                        row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>;
                        err_prefix : string) : Result<TT -const -#, string> {
    // Error format is normalized to `{err_prefix}: <what>` across all
    // three failure paths. The pre-refactor inlined messages were
    // inconsistent (`query_one: step failed` had a colon,
    // `_sql step failed` did not — likewise for `prepare failed`); the
    // unified shape replaces both. No consumers parse these strings.
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "{err_prefix}: prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<TT -const -#>)
    }
    invoke(bind_blk, stmt)
    let src = sqlite3_step(stmt)
    if (src == SQLITE_DONE) {
        sqlite3_finalize(stmt)
        return err("{err_prefix}: expected one row, got 0 rows", type<TT -const -#>)
    }
    if (src != SQLITE_ROW) {
        let msg = "{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<TT -const -#>)
    }
    var row : TT -const -# = invoke(row_blk, stmt) // nolint:LINT003
    sqlite3_finalize(stmt)
    return move_ok(row, type<string>)
}

def private try_one_row_opt(db : SqlRunner; sql : string;
                            bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                            row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>;
                            err_prefix : string) : Result<Option<TT -const -#>, string> {
    //! Like `try_one_row`, but returns `Ok(none)` on zero rows instead of
    //! `Err`. Use when "no row matched" is a normal outcome and only
    //! prepare/step failures are real errors.
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "{err_prefix}: prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<Option<TT -const -#>>)
    }
    invoke(bind_blk, stmt)
    let src = sqlite3_step(stmt)
    if (src == SQLITE_DONE) {
        sqlite3_finalize(stmt)
        return ok(none(type<TT -const -#>), type<string>)
    }
    if (src != SQLITE_ROW) {
        let msg = "{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<Option<TT -const -#>>)
    }
    var row : TT -const -# = invoke(row_blk, stmt) // nolint:LINT003
    sqlite3_finalize(stmt)
    return move_ok(move_some(row), type<string>)
}

def private try_step_dml(db : SqlRunner; sql : string;
                         bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                         err_prefix : string) : Result<int, string> {
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "{err_prefix}: prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<int>)
    }
    invoke(bind_blk, stmt)
    let src = sqlite3_step(stmt)
    if (src != SQLITE_DONE) {
        let msg = "{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<int>)
    }
    let n = sqlite3_changes(db.sqlite_handle)
    sqlite3_finalize(stmt)
    return ok(n, type<string>)
}

def private try_collect_rows(db : SqlRunner; sql : string;
                             bind_blk : block<(var stmt : sqlite3_stmt?) : void>;
                             row_blk : block<(stmt : sqlite3_stmt?) : auto(TT)>;
                             err_prefix : string) : Result<array<TT -const -#>, string> {
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "{err_prefix}: prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<array<TT -const -#>>)
    }
    invoke(bind_blk, stmt)
    var inscope result : array<TT -const -#>
    var rc = sqlite3_step(stmt)
    while (rc == SQLITE_ROW) {
        result |> push(invoke(row_blk, stmt))
        rc = sqlite3_step(stmt)
    }
    if (rc != SQLITE_DONE) {
        let msg = "{err_prefix}: step failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<array<TT -const -#>>)
    }
    sqlite3_finalize(stmt)
    return move_ok(result, type<string>)
}

[unused_argument(t)]
def private try_query_one_impl(db : SqlRunner; sql : string; t : type<auto(TT)>;
                               bind_blk : block<(var stmt : sqlite3_stmt?) : void>) : Result<TT -const -#, string> {
    return <- try_one_row(db, sql, bind_blk,
                          $(stmt : sqlite3_stmt?) {
                              return _::_sql_read_row(stmt, type<TT -const -#>)
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
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        sqlite3_finalize(stmt)
        panic("query_one_opt prepare failed: {sqlite3_errmsg(db.sqlite_handle)}")
    }
    invoke(bind_blk, stmt)
    let src = sqlite3_step(stmt)
    if (src == SQLITE_DONE) {
        sqlite3_finalize(stmt)
        return none(type<TT -const -#>)
    }
    if (src != SQLITE_ROW) {
        sqlite3_finalize(stmt)
        panic("query_one_opt step failed: {sqlite3_errmsg(db.sqlite_handle)}")
    }
    var row : TT -const -# = _::_sql_read_row(stmt, type<TT -const -#>) // nolint:LINT003
    sqlite3_finalize(stmt)
    return move_some(row)
}

// ---- try_query_one — Result<TT, string>; 0..3 positional args ----

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>) : Result<TT -const -#, string> {
    //! Zero-arg form. Errors on prepare/step failure or zero rows.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {})
}

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0)) : Result<TT -const -#, string> {
    //! 1-arg positional bind.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
    })
}

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : Result<TT -const -#, string> {
    //! 2-arg positional bind.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
        sqlite_bind(stmt, 2, arg1)
    })
}

def try_query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : Result<TT -const -#, string> {
    //! 3-arg positional bind.
    return <- try_query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
        sqlite_bind(stmt, 2, arg1)
        sqlite_bind(stmt, 3, arg2)
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
        sqlite_bind(stmt, 1, arg0)
    })
}

[unused_argument(t)]
def query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : TT -const -# {
    return <- query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
        sqlite_bind(stmt, 2, arg1)
    })
}

[unused_argument(t)]
def query_one(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : TT -const -# {
    return <- query_one_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
        sqlite_bind(stmt, 2, arg1)
        sqlite_bind(stmt, 3, arg2)
    })
}

// ---- query_one_opt — Option<TT>; 0..3 positional args; panics only on prepare/step error ----

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {})
}

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0)) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
    })
}

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1)) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
        sqlite_bind(stmt, 2, arg1)
    })
}

def query_one_opt(db : SqlRunner; sql : string; t : type<auto(TT)>; arg0 : auto(A0); arg1 : auto(A1); arg2 : auto(A2)) : Option<TT -const -#> {
    return <- query_one_opt_impl(db, sql, type<TT -const -#>, $(var stmt : sqlite3_stmt?) {
        sqlite_bind(stmt, 1, arg0)
        sqlite_bind(stmt, 2, arg1)
        sqlite_bind(stmt, 3, arg2)
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
    //! Emits CREATE TABLE for the type ``T`` based on its `[sql_table]` definition.
    //! Returns ``none`` on success, ``some(errmsg)`` on failure.
    return db |> try_exec(_::_sql_create_table_sql(type<TT>))
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
        sqlite_bind(stmt, 1, id)
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
// `sqlite_bind(stmt, idx, expr)` calls) and execute it. Return rows-affected
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
            return _::_sql_read_row(stmt, type<TT -const -#>)
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
// All generated function bodies emit `sqlite_sql_type(type<T>)` and
// `sqlite_bind(stmt, idx, row.field)` calls — so primitive and user-defined
// field types travel the same code path. The witness on `sqlite_sql_type` is
// a type tag for overload dispatch only; bodies must not read it. Type
// resolution defers to each generated function's own typecheck pass; the
// macro itself does not depend on field types being fully resolved at
// apply() time.
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

def private find_bool_annotation(args : AnnotationArgumentList; argn : string; default_value : bool) : bool {
    let v = find_arg(args, argn)
    if (v is tBool) {
        return v as tBool
    }
    return default_value
}

def private find_string_annotation(args : AnnotationArgumentList; argn : string; default_value : string) : string {
    let v = find_arg(args, argn)
    if (v is tString) {
        return v as tString
    }
    return default_value
}

struct private FieldInfo {
    name : string
    col_name : string
    is_pk : bool
    is_int_pk : bool
    is_optional : bool
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

[structure_macro(name=sql_table)]
class SqlTableMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let table_name = find_string_annotation(args, "name", string(st.name))

        var fields : array<FieldInfo>
        for (field in st.fields) {
            let is_pk = find_bool_annotation(field.annotation, "sql_primary_key", false)
            let is_optional = is_option_field_type(field._type)
            if (is_pk && is_optional) {
                errors := "[sql_table]: @sql_primary_key on field '{field.name}' must not be Option<T> — primary keys cannot be NULL in SQL. Use a non-optional type."
                return false
            }
            fields |> push(FieldInfo(
                name = string(field.name),
                col_name = string(field.name),
                is_pk = is_pk,
                is_int_pk = is_pk && (field._type.baseType == Type.tInt || field._type.baseType == Type.tInt64),
                is_optional = is_optional
            ))
        }

        var fn_table_name = make_table_name_fn(st, table_name)
        compiling_module() |> add_function(fn_table_name)
        var fn_drop = make_drop_sql_fn(st, table_name)
        compiling_module() |> add_function(fn_drop)
        var fn_create = make_create_table_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_create)
        var fn_ins_pk = make_insert_with_pk_sql_fn(st, table_name, fields)
        compiling_module() |> add_function(fn_ins_pk)
        var fn_ins_nopk = make_insert_no_pk_sql_fn(st, table_name, fields)
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

    def make_create_table_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        var write_exprs : array<ExpressionPtr>
        write_exprs |> reserve(3 * length(fields) + 2)

        let header = "CREATE TABLE \"{table_name}\"("
        write_exprs |> push(qmacro_expr(${ writer |> write($v(header)); }))

        for (i in range(length(fields))) {
            let info = fields[i]
            let prefix = i == 0 ? "\"{info.col_name}\" " : ", \"{info.col_name}\" "
            // Nullability is type-driven: Option<T> fields drop NOT NULL.
            // PK columns are implicitly NOT NULL in SQLite for INTEGER PRIMARY KEY,
            // and rejected as Option<T> earlier in apply().
            let suffix = info.is_pk ? " PRIMARY KEY" : (info.is_optional ? "" : " NOT NULL")
            write_exprs |> push(qmacro_expr(${ writer |> write($v(prefix)); }))
            write_exprs |> push(qmacro_expr(${ writer |> write(sqlite_sql_type(type<$t(st.fields[i]._type)>)); }))
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

    def make_insert_with_pk_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        let placeholders = build_string() $(var w) {
            for (i in range(length(fields))) {
                if (i > 0) {
                    w |> write(", ")
                }
                w |> write("?")
            }
        }
        let sql = "INSERT INTO \"{table_name}\" VALUES ({placeholders})"
        var fn = qmacro_function("_sql_insert_with_pk_sql") $(typ : $t(st)) : string {
            return $v(sql)
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }

    def make_insert_no_pk_sql_fn(var st : StructurePtr; table_name : string; fields : array<FieldInfo>) : FunctionPtr {
        let cols = build_string() $(var w) {
            var first = true
            for (info in fields) {
                if (info.is_pk) {
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
                if (info.is_pk) {
                    continue
                }
                if (!first) {
                    w |> write(", ")
                }
                w |> write("?")
                first = false
            }
        }
        // PK-only structs have no non-PK columns. SQLite rejects `INSERT INTO T () VALUES ()`,
        // so fall back to `DEFAULT VALUES` — the canonical way to insert a row with all defaults
        // (here, just the auto-assigned rowid).
        let sql = (empty(cols)
            ? "INSERT INTO \"{table_name}\" DEFAULT VALUES"
            : "INSERT INTO \"{table_name}\" ({cols}) VALUES ({placeholders})")
        var fn = qmacro_function("_sql_insert_no_pk_sql") $(typ : $t(st)) : string {
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
        var with_pk_exprs : array<ExpressionPtr>
        var no_pk_exprs : array<ExpressionPtr>
        with_pk_exprs |> reserve(length(fields))
        no_pk_exprs |> reserve(length(fields))

        for (i in range(length(fields))) {
            let info = fields[i]
            let with_idx = i + 1
            let field_name = info.name
            with_pk_exprs |> push(qmacro_expr(${
                sqlite_bind(stmt, $v(with_idx), row.$f(field_name));
            }))
        }

        var no_pk_idx = 0
        for (info in fields) {
            if (info.is_pk) {
                continue
            }
            no_pk_idx++
            let field_name = info.name
            no_pk_exprs |> push(qmacro_expr(${
                sqlite_bind(stmt, $v(no_pk_idx), row.$f(field_name));
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
                row.$f(field_name) = sqlite_read(stmt, $v(col_idx), type<$t(st.fields[i]._type)>);
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
                if (info.is_pk) {
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
        // PK-only struct (no SET columns): `UPDATE T SET WHERE pk=?` is invalid
        // SQL. Stub panics at call time with a clear message.
        if (empty(set_clause)) {
            let st_name = string(st.name)
            let msg = "[sql_table] {st_name}: update(row) has nothing to SET — every non-PK field is missing. Add at least one non-PK column, or use raw `exec`."
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
        // Non-PK columns first.
        for (i in range(length(fields))) {
            if (fields[i].is_pk) {
                continue
            }
            bind_idx++
            let bi = bind_idx
            let field_name = fields[i].name
            bind_exprs |> push(qmacro_expr(${
                sqlite_bind(stmt, $v(bi), row.$f(field_name));
            }))
        }
        // PK column last (WHERE clause).
        bind_idx++
        let pk_bind_idx = bind_idx
        let pk_field_name = fields[pk_idx].name
        bind_exprs |> push(qmacro_expr(${
            sqlite_bind(stmt, $v(pk_bind_idx), row.$f(pk_field_name));
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
            sqlite_bind(stmt, 1, row.$f(pk_field_name))
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        return <- fn
    }
}
