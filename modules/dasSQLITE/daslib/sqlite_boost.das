options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module sqlite_boost shared public

require sqlite public

require strings public
require daslib/contracts
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

[unused_argument(stmt, idx, value)]
def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : auto(TT)) : void {
    concept_assert(false, "sqlite_bind: unsupported field type. Define `def sqlite_bind(var stmt : sqlite3_stmt?; idx : int; value : YourType) : void` to extend.")
}

[unused_argument(t)]
def try_query_scalar(db : SqlRunner; sql : string; t : type<string>) : Result<string, string> {
    //! Prepares ``sql``, steps once, returns column 0 as ``string``.
    //! Returns ``Err(errmsg)`` on prepare/step error or zero rows.
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "query_scalar prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<string>)
    }
    let src = sqlite3_step(stmt)
    if (src != SQLITE_ROW) {
        let msg = "query_scalar: expected one row, got rc={src}: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<string>)
    }
    var result : string
    unsafe {
        result := clone_string(sqlite3_column_text(stmt, 0))
    }
    sqlite3_finalize(stmt)
    return ok(result, type<string>)
}

[unused_argument(t)]
def query_scalar(db : SqlRunner; sql : string; t : type<string>) : string {
    //! Strict variant of ``try_query_scalar``. Panics on prepare/step error or zero rows.
    let r = db |> try_query_scalar(sql, type<string>)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
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
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "insert prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<int64>)
    }
    _::_sql_bind_row(stmt, row, !pk_unset)
    let src = sqlite3_step(stmt)
    if (src != SQLITE_DONE) {
        let msg = "insert step failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        return err(msg, type<int64>)
    }
    sqlite3_finalize(stmt)
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
    let begin_err = db |> try_exec("BEGIN IMMEDIATE")
    if (begin_err |> is_some) {
        return err(begin_err |> unwrap, type<int64>)
    }
    let sql = pk_unset ? _::_sql_insert_no_pk_sql(rows[0]) : _::_sql_insert_with_pk_sql(rows[0])
    var stmt : sqlite3_stmt?
    let prc = sqlite3_prepare_v2(db.sqlite_handle, sql, stmt)
    if (prc != SQLITE_OK) {
        let msg = "insert prepare failed: {sqlite3_errmsg(db.sqlite_handle)}"
        sqlite3_finalize(stmt)
        db |> try_exec("ROLLBACK") |> ignore_sql_error
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
            db |> try_exec("ROLLBACK") |> ignore_sql_error
            return err(msg, type<int64>)
        }
    }
    sqlite3_finalize(stmt)
    let commit_err = db |> try_exec("COMMIT")
    if (commit_err |> is_some) {
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
}

[structure_macro(name=sql_table)]
class SqlTableMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let table_name = find_string_annotation(args, "name", string(st.name))

        var fields : array<FieldInfo>
        for (field in st.fields) {
            let is_pk = find_bool_annotation(field.annotation, "sql_primary_key", false)
            fields |> push(FieldInfo(
                name = string(field.name),
                col_name = string(field.name),
                is_pk = is_pk,
                is_int_pk = is_pk && (field._type.baseType == Type.tInt || field._type.baseType == Type.tInt64)
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
            let suffix = info.is_pk ? " PRIMARY KEY" : " NOT NULL"
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
}
