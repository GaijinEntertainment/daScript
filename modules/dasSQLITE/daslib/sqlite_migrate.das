options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module sqlite_migrate shared public

// Intentionally opt in for user-facing docs: keep comments concise and actionable.
options _comment_hygiene = true

require sqlite_boost public

require strings public
require daslib/strings_boost
require daslib/option public
require daslib/rtti               // this_context().last_exception inside recover blocks
require daslib/ast_boost
require daslib/macro_boost
require daslib/templates
require daslib/templates_boost

// ===== Public types =====

struct PendingMigration {
    //! One row from `pending_migrations(db)` — a registered migration that has not yet been applied.
    //! `description` reflects the *current* annotation text (compile-time source); annotation edits
    //! propagate until the migration is applied, after which `migration_history` carries the frozen text.
    version : int
    description : string
}

struct MigrationRecord {
    //! One row from `migration_history(db)` — a migration that has been applied to this DB.
    //! `description` is the text that was on the annotation at the moment of apply, frozen in the
    //! audit row; subsequent annotation edits do NOT propagate. `applied_at` is unix epoch seconds.
    version : int
    description : string
    applied_at : int64
}

// ===== Private registry types =====

struct private MigrationEntry {
    version : int
    description : string
    vacuum : bool
    analyze : bool
    body : function<(db : SqlRunner) : void>
    func_name : string
    module_name : string
}

// ===== Audit table constants =====

let private AUDIT_CREATE_DDL = "CREATE TABLE IF NOT EXISTS __schema_version (version INTEGER PRIMARY KEY, description TEXT NOT NULL DEFAULT '', applied_at INTEGER NOT NULL)"

// ===== Process-global registry =====

// Populated by per-module [init] thunks emitted by the [sql_migration] macro.
// Loaded once at program startup, before main(). Never mutated thereafter except
// by additional module loads.
var private _registry : array<MigrationEntry>

def _add_migration_entry(version : int; description : string; vacuum : bool; analyze : bool;
                         body : function<(db : SqlRunner) : void>;
                         func_name : string; module_name : string) : void {
    //! Public entry point used by ``[sql_migration]``-generated init blocks to register a
    //! migration. End users never call this directly.
    var entry <- MigrationEntry(
        version = version,
        description = description,
        vacuum = vacuum,
        analyze = analyze,
        body = body,
        func_name = func_name,
        module_name = module_name)
    _registry |> emplace(entry)
}

def private _registry_max_version() : int {
    var m = 0
    for (e in _registry) {
        if (e.version > m) {
            m = e.version
        }
    }
    return m
}

def private _registry_sorted() : array<MigrationEntry> {
    var out := _registry
    sort(out, $(a, b) => a.version < b.version)
    return <- out
}

def private _registry_find_description(version : int) : string {
    for (e in _registry) {
        if (e.version == version) return e.description
    }
    return ""
}

def private _verify_no_duplicate_versions() : void {
    // Belt-and-suspenders runtime defense for dynamic plugin loading past compile-time visibility.
    var seen : table<int; string>
    for (e in _registry) {
        if (key_exists(seen, e.version)) {
            let prev = seen[e.version]
            panic("duplicate [sql_migration(version={e.version})] at runtime: '{prev}' and '{e.func_name}' (in '{e.module_name}')")
        }
        seen |> insert(e.version, "{e.func_name} (in '{e.module_name}')")
    }
}

// ===== Internal helpers — audit table I/O and diagnostics =====

let private ADOPTION_HINT = "this DB has existing tables/views/indexes but __schema_version is empty (no migration history yet). If your first migration creates tables that already exist, it will fail. To adopt migrations on an existing DB, call `db |> baseline(version = N)` before `migrate_to_latest()` to mark v1..vN as already applied. See tut 43 § \"Adopting migrations on an existing DB\"."

def private _scalar_int_or_zero(db : SqlRunner; sql : string) : int {
    // Treats query failure (e.g. "no such table") as 0 — natural default for COUNT/MAX probes.
    let r = db |> try_query_scalar(sql, type<int>)
    return (r |> is_ok) ? (r |> unwrap) : 0
}

def private _has_user_objects(db : SqlRunner) : bool {
    // sqlite_* names are reserved for internals (sqlite_sequence, sqlite_autoindex_*).
    return _scalar_int_or_zero(db,
        "SELECT COUNT(*) FROM sqlite_master WHERE name NOT LIKE 'sqlite_%' AND name != '__schema_version'") > 0
}

def private _audit_insert(db : SqlRunner; version : int; description : string) : SqlError {
    return db |> try_exec(
        "INSERT INTO __schema_version (version, description, applied_at) VALUES (?, ?, unixepoch())",
        version, description)
}

def private _format_versions_csv(pending : array<MigrationEntry>; applied_count : int) : string {
    // "v4, v5" — every applied migration (inside the txn) plus the failing one — all rolled back.
    var parts : array<string>
    let limit = applied_count + 1
    var i = 0
    for (m in pending) {
        if (i >= limit) break
        parts |> push("v{m.version}")
        i++
    }
    return parts |> join(", ")
}

def private _build_enriched_err(failed_version : int; failed_desc : string; underlying : string;
                                rolled_back : string; layer2_hint : bool) : string {
    var msg = "migration v{failed_version} '{failed_desc}' failed:\n  {underlying}\n\nnote: this migrate_to_latest call's transaction was rolled back — {rolled_back} are NOT applied. Fix the migration body and re-run; the corrected migration set will replay on the next call. The DB does not need to be reset."
    if (layer2_hint) {
        msg += "\n\nnote: This is the first migration on this DB and __schema_version was empty before this call. Looks like an adoption attempt on an existing schema. Call `db |> baseline(version = N)` once before `migrate_to_latest()` to mark v1..vN as already applied. See tut 43 § \"Adopting migrations on an existing DB\"."
    }
    return msg
}

def private _log_warn_on_some(err : SqlError) : void {
    if (err |> is_some) {
        to_log(LOG_WARNING, "{err |> unwrap}\n")
    }
}

// ===== Internal runner — _migrate_inner =====

def private _migrate_inner(db : SqlRunner) : Result<int, string> {
    // Eager audit table; idempotent across concurrent runners.
    let create_err = db |> try_exec(AUDIT_CREATE_DDL)
    if (create_err |> is_some) return err(create_err |> unwrap, type<int>)
    // Was the audit table empty at the start of this call? (Used by Layer-1 + Layer-2.)
    let audit_was_empty_before = _scalar_int_or_zero(db, "SELECT COUNT(*) FROM __schema_version") == 0
    let max_registered = _registry_max_version()
    // Layer-1 only fires when there's something to migrate — empty registry + pre-existing schema is benign.
    if (audit_was_empty_before && max_registered > 0 && _has_user_objects(db)) {
        to_log(LOG_WARNING, "{ADOPTION_HINT}\n")
    }
    // Belt-and-suspenders runtime dup check.
    _verify_no_duplicate_versions()
    // Read current; downgrade detection (Scenario 4).
    let current = _scalar_int_or_zero(db, "SELECT COALESCE(MAX(version), 0) FROM __schema_version")
    if (current > max_registered) {
        let bin_max = max_registered
        to_log(LOG_WARNING, "DB at schema v{current} but this binary only knows up to v{bin_max} — DB appears to be running ahead of the binary. No migrations applied.\n")
        return ok(0, type<string>)
    }
    var pending <- _registry_sorted()
    var pending_idx = 0
    while (pending_idx < pending |> length) {
        if (pending[pending_idx].version <= current) {
            pending |> erase(pending_idx)
        } else {
            pending_idx++
        }
    }
    if (pending |> empty) return ok(0, type<string>)
    // BEGIN IMMEDIATE — soft-error path (lock contention surfaces here as SQLITE_BUSY).
    let begin_err = db |> try_exec("BEGIN IMMEDIATE")
    if (begin_err |> is_some) return err(begin_err |> unwrap, type<int>)
    // Concurrent-runner guard (Scenario 8): re-read MAX(version) under the write lock and drop pending entries another runner already applied between our optimistic read and BEGIN IMMEDIATE.
    let current_locked = _scalar_int_or_zero(db, "SELECT COALESCE(MAX(version), 0) FROM __schema_version")
    if (current_locked != current) {
        pending_idx = 0
        while (pending_idx < pending |> length) {
            if (pending[pending_idx].version <= current_locked) {
                pending |> erase(pending_idx)
            } else {
                pending_idx++
            }
        }
        if (pending |> empty) {
            // Another runner did our work. COMMIT releases the lock cleanly (no DDL was issued).
            db |> try_exec("COMMIT") |> _log_warn_on_some
            return ok(0, type<string>)
        }
    }
    // Mutables shared between try and recover.
    var applied_count = 0
    var any_vacuum = false
    var any_analyze = false
    var failed_version = 0
    var failed_desc = ""
    var inner_err = ""
    var commit_err = ""
    try {
        for (m in pending) {
            to_log(LOG_INFO, "applying migration v{m.version}: {m.description}\n")
            failed_version = m.version
            failed_desc = m.description
            m.body(db)                          // user body — may panic
            let ins_err = _audit_insert(db, m.version, m.description)
            if (ins_err |> is_some) {
                panic(ins_err |> unwrap)        // promote to panic so recover-block ROLLBACK runs
            }
            applied_count++
            any_vacuum ||= m.vacuum
            any_analyze ||= m.analyze
        }
        let cerr = db |> try_exec("COMMIT")
        if (cerr |> is_some) {
            db |> try_exec("ROLLBACK") |> _log_warn_on_some
            commit_err = cerr |> unwrap
        }
    } recover {
        let underlying = string(this_context().last_exception) |> rtrim
        let rolled_back_str = _format_versions_csv(pending, applied_count)
        to_log(LOG_ERROR, "migration v{failed_version} '{failed_desc}' FAILED — rolled back call\n  underlying error: {underlying}\n  rolled back: {rolled_back_str}\n")
        db |> try_exec("ROLLBACK") |> _log_warn_on_some
        let layer2_hint = audit_was_empty_before && (failed_version == pending[0].version)
        inner_err = _build_enriched_err(failed_version, failed_desc, underlying, rolled_back_str, layer2_hint)
    }
    if (!empty(commit_err)) return err(commit_err, type<int>)
    if (!empty(inner_err)) return err(inner_err, type<int>)
    // Post-COMMIT VACUUM/ANALYZE — Scenario 3: failures = LOG_WARNING, never block success.
    if (any_analyze) {
        (db |> try_exec("ANALYZE")) |> _log_warn_on_some
    }
    if (any_vacuum) {
        (db |> try_exec("VACUUM")) |> _log_warn_on_some
    }
    to_log(LOG_INFO, "migrate_to_latest: applied {applied_count} migration(s)\n")
    return ok(applied_count, type<string>)
}

// ===== Inspection — pure reads, no side effects =====
// Never create the audit table or write a row; absent __schema_version → 0 / all-pending / [].

def private _try_audit_table_exists(db : SqlRunner) : Result<bool, string> {
    // sqlite_master itself never reports "no such table"; transient errors here propagate.
    let r <- db |> try_query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='__schema_version'", type<int>)
    if (r |> is_err) return err(r |> unwrap_err, type<bool>)
    return ok((r |> unwrap) > 0, type<string>)
}

def private _audit_table_exists(db : SqlRunner) : bool {
    // Strict-form callers swallow transient errors (treat as "absent" → return 0/empty).
    let r <- _try_audit_table_exists(db)
    return (r |> is_ok) && (r |> unwrap)
}

def current_schema_version(db : SqlRunner) : int {
    //! Highest applied migration version, or 0 if no migrations were applied, the audit table is absent, or a transient SQLite error (e.g. ``SQLITE_BUSY``) blocks the read.
    //! Use ``try_current_schema_version`` to distinguish "nothing applied" from "couldn't tell".
    if (!_audit_table_exists(db)) return 0
    return _scalar_int_or_zero(db, "SELECT COALESCE(MAX(version), 0) FROM __schema_version")
}

def try_current_schema_version(db : SqlRunner) : Result<int, string> {
    //! ``ok(0)`` when the audit table is absent or empty; ``err`` on transient SQLite errors
    //! (lock contention etc.) — including failures of the sqlite_master probe.
    let exists <- _try_audit_table_exists(db)
    if (exists |> is_err) return err(exists |> unwrap_err, type<int>)
    if (!(exists |> unwrap)) return ok(0, type<string>)
    return <- db |> try_query_scalar("SELECT COALESCE(MAX(version), 0) FROM __schema_version", type<int>)
}

def try_pending_migrations(db : SqlRunner) : Result<array<PendingMigration>, string> {
    //! Registered migrations whose ``version > current_schema_version(db)``, in ascending order.
    //! Description sourced from the annotation (current code), not the audit table.
    let cv <- db |> try_current_schema_version()
    if (cv |> is_err) return err(cv |> unwrap_err, type<array<PendingMigration>>)
    let current = cv |> unwrap
    var inscope out : array<PendingMigration>
    let sorted <- _registry_sorted()
    for (m in sorted) {
        if (m.version > current) {
            out |> push(PendingMigration(version = m.version, description = m.description))
        }
    }
    return move_ok(out, type<string>)
}

def pending_migrations(db : SqlRunner) : array<PendingMigration> {
    //! Strict variant. Panics on transient errors; ``empty`` when no pending migrations.
    var r <- db |> try_pending_migrations()
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

def try_migration_history(db : SqlRunner) : Result<array<MigrationRecord>, string> {
    //! Every row in ``__schema_version``, in ascending version order. Description here is the
    //! frozen-at-apply text from the audit row (NOT the current annotation). ``ok([])`` when the
    //! audit table is absent; ``err`` on transient SQLite errors (incl. the sqlite_master probe).
    let exists <- _try_audit_table_exists(db)
    if (exists |> is_err) return err(exists |> unwrap_err, type<array<MigrationRecord>>)
    if (!(exists |> unwrap)) return ok(default<array<MigrationRecord>>, type<string>)
    return <- try_run_select(db,
        "SELECT version, description, applied_at FROM __schema_version ORDER BY version",
        $(var stmt : sqlite3_stmt?) {},
        $(stmt : sqlite3_stmt?) {
            return MigrationRecord(
                version = sqlite3_column_int(stmt, 0),
                description = sqlite3_column_text_(stmt, 1),
                applied_at = sqlite3_column_int64(stmt, 2))
        })
}

def migration_history(db : SqlRunner) : array<MigrationRecord> {
    //! Strict variant of ``try_migration_history``.
    var r <- db |> try_migration_history()
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return <- r._value
}

// ===== Public runner surfaces =====

def migrate_to_latest(db : SqlRunner) : int {
    //! Applies every pending migration in ascending order inside one transaction; returns
    //! the count applied. Panics with an enriched message on soft failure or body panic.
    let r <- _migrate_inner(db)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
    return r |> unwrap
}

def try_migrate_to_latest(db : SqlRunner) : Result<int, string> {
    //! Non-panic variant of ``migrate_to_latest``. Returns ``ok(count)`` on success or
    //! ``err(enriched_message)`` for both soft failures and body panics. ROLLBACK is
    //! guaranteed before the ``Err`` is returned (recover-block).
    return <- _migrate_inner(db)
}

// ===== Adoption — baseline =====

def private _audit_has_version(db : SqlRunner; version : int) : bool {
    // Version is an int we control (registered-migration number, not user input) — string format is safe.
    return _scalar_int_or_zero(db, "SELECT COUNT(*) FROM __schema_version WHERE version = {version}") > 0
}

def try_baseline(db : SqlRunner; version : int) : Result<int, string> {
    //! Stamps v1..``version`` as already-applied without running their bodies. Idempotent
    //! (versions already in the audit are skipped). Returns ``ok(stamped_count)`` or ``err``.
    //! Rejects ``version > _registry_max_version()`` as almost-certainly a typo.
    if (version <= 0) return err("baseline(version={version}) must be a positive integer", type<int>)
    let max_reg = _registry_max_version()
    if (version > max_reg) return err("baseline(version={version}) is above the highest registered migration (v{max_reg}) — looks like a typo", type<int>)
    let create_err = db |> try_exec(AUDIT_CREATE_DDL)
    if (create_err |> is_some) return err(create_err |> unwrap, type<int>)
    let begin_err = db |> try_exec("BEGIN IMMEDIATE")
    if (begin_err |> is_some) return err(begin_err |> unwrap, type<int>)
    var stamped = 0
    var inner_err = ""
    var commit_err = ""
    try {
        for (v in range(1, version + 1)) {
            if (_audit_has_version(db, v)) continue
            let desc_from_reg = _registry_find_description(v)
            let desc = empty(desc_from_reg) ? "[baseline]" : desc_from_reg
            let ins_err = _audit_insert(db, v, desc)
            if (ins_err |> is_some) {
                panic(ins_err |> unwrap)        // recover handles ROLLBACK + Err propagation
            }
            stamped++
        }
        let cerr = db |> try_exec("COMMIT")
        if (cerr |> is_some) {
            db |> try_exec("ROLLBACK") |> _log_warn_on_some
            commit_err = cerr |> unwrap
        }
    } recover {
        let underlying = string(this_context().last_exception) |> rtrim
        db |> try_exec("ROLLBACK") |> _log_warn_on_some
        inner_err = "baseline(version={version}) failed: {underlying}"
    }
    if (!empty(commit_err)) return err(commit_err, type<int>)
    if (!empty(inner_err)) return err(inner_err, type<int>)
    return ok(stamped, type<string>)
}

def baseline(db : SqlRunner; version : int) : void {
    //! Strict variant of ``try_baseline``. Panics on error.
    let r <- db |> try_baseline(version)
    if (r |> is_err) {
        panic(r |> unwrap_err)
    }
}

// ===== with_latest_sqlite — convenience wrapper =====

def with_latest_sqlite(path : string; blk : block<(db : SqlRunner) : void>) : void {
    //! Combines ``with_sqlite(path)`` + ``apply_recommended_pragmas`` + ``migrate_to_latest``.
    //! Preferred form for app code; the explicit two-step ``with_sqlite + migrate_to_latest``
    //! sequence stays available for tools/CI that may want to inspect before migrating.
    with_sqlite(path) $(db) {
        db |> apply_recommended_pragmas()
        db |> migrate_to_latest()
        invoke(blk, db)
    }
}

// ===== Compile-time migration registry helpers (used by [sql_migration] dup detection) =====

struct private RegisteredMigration {
    version : int
    fn_name : string
    mod_name : string
}

def private _collect_registered_migrations(var out : array<RegisteredMigration>) : void {
    // Single pass over compiling_program() — collects (version, fn, mod) for every [sql_migration].
    let prog = compiling_program() |> get_ptr()
    prog |> for_each_module_no_order($(mod) {
        mod |> for_each_module_function($(var f : FunctionPtr) {
            for (ann in f.annotations) {
                if (ann == null || ann.annotation.name != "sql_migration") continue
                let vArg = ann.arguments |> find_arg("version")
                if (!(vArg is tInt)) continue
                var entry <- RegisteredMigration(
                    version = vArg as tInt,
                    fn_name = "{f.name}",
                    mod_name = "{mod.name}")
                out |> push_clone(entry)
            }
        })
    })
}

// ===== [sql_migration] annotation =====

[function_macro(name="sql_migration")]
class SqlMigrationMacro : AstFunctionAnnotation {
    //! Marks ``def fn(db : SqlRunner)`` as a versioned schema migration; runs once per DB.
    //! Args: ``version=<int>`` (required, positive, unique), optional ``description``/``vacuum``/``analyze``.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (func.fromGeneric != null) {
            errors := "[sql_migration] is not supported on generic functions"
            return false
        }
        // Args
        let versionArg = args |> find_arg("version")
        if (!(versionArg is tInt)) {
            errors := "[sql_migration]: version=<int> is required (positive integer)"
            return false
        }
        let version = versionArg as tInt
        if (version <= 0) {
            errors := "[sql_migration]: version={version} must be a positive integer"
            return false
        }
        var description = ""
        let descArg = args |> find_arg("description")
        if (descArg is tString) {
            description = descArg as tString
        } elif (!(descArg is nothing)) {
            errors := "[sql_migration]: description= must be a string literal"
            return false
        }
        var vacuum = false
        let vacArg = args |> find_arg("vacuum")
        if (vacArg is tBool) {
            vacuum = vacArg as tBool
        } elif (!(vacArg is nothing)) {
            errors := "[sql_migration]: vacuum= must be a bool literal (true/false)"
            return false
        }
        var analyze = false
        let anaArg = args |> find_arg("analyze")
        if (anaArg is tBool) {
            analyze = anaArg as tBool
        } elif (!(anaArg is nothing)) {
            errors := "[sql_migration]: analyze= must be a bool literal (true/false)"
            return false
        }
        // Signature: def fn(db : SqlRunner) : void
        let nArgs = func.arguments |> length
        if (nArgs != 1) {
            errors := "[sql_migration] '{func.name}' must take exactly one argument of type SqlRunner; got {nArgs}"
            return false
        }
        let arg0Type = func.arguments[0]._type
        if (arg0Type.structType == null || arg0Type.structType.name != "SqlRunner") {
            errors := "[sql_migration] '{func.name}': single argument must be of type SqlRunner; got '{describe(arg0Type)}'"
            return false
        }
        if (func.result.baseType != Type.tVoid && func.result.baseType != Type.autoinfer) {
            errors := "[sql_migration] '{func.name}': must return void; got '{describe(func.result)}'"
            return false
        }
        let funcDasName = "{func.name}"
        let modName = "{compiling_module().name}"
        // Cross-module dup-version detection. The compile-time scan catches the typical
        // "two devs picked version=N" merge case; runtime _verify_no_duplicate_versions()
        // catches anything that slips past (dynamic plugin loading, etc.).
        var registered : array<RegisteredMigration>
        _collect_registered_migrations(registered)
        for (e in registered) {
            // skip non-matching versions and self
            if (e.version != version || (e.fn_name == funcDasName && e.mod_name == modName)) continue
            errors := "duplicate [sql_migration(version={version})]:\n  registered: {e.fn_name} in module '{e.mod_name}'\n  duplicate:  {funcDasName} in module '{modName}'\nhint: pick a different version number for one of them, or merge the bodies"
            return false
        }
        // Build ExprAddr to the user's function with explicit funcType
        var fnType = new TypeDecl(at = func.at, baseType = Type.tFunction)
        for (a in func.arguments) {
            fnType.argTypes |> emplace_new <| clone_type(a._type)
        }
        fnType.firstType = clone_type(func.result)
        var fnAddr = new ExprAddr(at = func.at, target := "__::{funcDasName}", funcType <- fnType)
        // Append registration into per-module init function `register`sqlite`migrations`
        var reg <- setup_call_list("register`sqlite`migrations", func.at, true/*isInit*/, true/*isPrivate*/)
        reg.list |> emplace_new <| qmacro_block() {
            sqlite_migrate::_add_migration_entry($v(version), $v(description), $v(vacuum), $v(analyze),
                $e(fnAddr), $v(funcDasName), $v(modName))
        }
        return true
    }
}

// ===== struct_convert_field — overloaded conversion dispatch =====
// User extension: declare ``def struct_convert_field(var dst : MyEnum&; src : int) { ... }`` in your module.

def struct_convert_field(var dst : auto(SCF_T)&; src : SCF_T) : void {
    //! Identity — same source and target type.
    dst = src
}

def struct_convert_field(var dst : Option<auto(SCF_T)>&; src : auto(SCF_S)) : void {
    //! ``S → Option<T>`` auto-wrap; inner S→T dispatches recursively so any S→T overload fires.
    var inner : SCF_T
    _::struct_convert_field(inner, src)
    dst = some(inner)
}

def struct_convert_field(var dst : auto(SCF_T)&; src : Option<auto(SCF_S)>) : void {
    //! ``Option<S> → T``. NULL collapses to ``default<S>``; inner S→T dispatches recursively.
    //! Override in your module for different NULL-handling semantics.
    _::struct_convert_field(dst, src ?? default<SCF_S>)
}

def struct_convert_field(var dst : Option<auto(SCF_T)>&; src : Option<auto(SCF_S)>) : void {
    //! ``Option<S> → Option<T>`` cross-payload — unwraps src, dispatches S→T inner, re-wraps.
    if (src |> is_some) {
        var inner : SCF_T
        _::struct_convert_field(inner, src |> unwrap)
        dst = some(inner)
    } else {
        dst = none(type<SCF_T>)
    }
}

// Target-typed conversion via the type's constructor. ``int(string)``, ``int(double)``, etc. dispatch
// through daslang's existing primitive constructors. Identity above wins for same-source-target.
def struct_convert_field(var dst : int&; src : auto(SCF_S)) : void {
    dst = int(src)
}
def struct_convert_field(var dst : int64&; src : auto(SCF_S)) : void {
    dst = int64(src)
}
def struct_convert_field(var dst : float&; src : auto(SCF_S)) : void {
    dst = float(src)
}
def struct_convert_field(var dst : double&; src : auto(SCF_S)) : void {
    dst = double(src)
}
def struct_convert_field(var dst : string&; src : auto(SCF_S)) : void {
    dst = "{src}"
}

// Disambiguators — primitive target from Option source. Beats both auto(T)←Option<T> and primitive←auto(S)
// at the typer's specificity check, so ``Option<int> → int`` (etc.) routes here cleanly with NULL→default<T>.
def struct_convert_field(var dst : int&; src : Option<auto(SCF_T)>) : void {
    dst = int(src ?? default<SCF_T>)
}
def struct_convert_field(var dst : int64&; src : Option<auto(SCF_T)>) : void {
    dst = int64(src ?? default<SCF_T>)
}
def struct_convert_field(var dst : float&; src : Option<auto(SCF_T)>) : void {
    dst = float(src ?? default<SCF_T>)
}
def struct_convert_field(var dst : double&; src : Option<auto(SCF_T)>) : void {
    dst = double(src ?? default<SCF_T>)
}
def struct_convert_field(var dst : string&; src : Option<auto(SCF_T)>) : void {
    dst = "{src ?? default<SCF_T>}"
}


// ===== [struct_convert] annotation =====

class private OverrideCollector : AstVisitor {
    // Records `dst.X` ONLY when it appears on the LHS of `=`/`<-`/`:=`.
    // Read-only mentions don't count (would silently leave dst.X at default).
    target_var_name : string
    overrides : table<string; bool>
    def OverrideCollector(name : string) {
        target_var_name = name
    }
    def record_lhs(lhs : ExpressionPtr) : void {
        if (lhs == null || !(lhs is ExprField)) return
        let f = lhs as ExprField
        if (f.value == null || !(f.value is ExprVar)) return
        let v = f.value as ExprVar
        if (v.name == target_var_name) {
            overrides |> insert(string(f.name), true)
        }
    }
    def override preVisitExprCopy(expr : ExprCopy?) : void {
        record_lhs(expr.left)
    }
    def override preVisitExprMove(expr : ExprMove?) : void {
        record_lhs(expr.left)
    }
    def override preVisitExprClone(expr : ExprClone?) : void {
        record_lhs(expr.left)
    }
}

def private collect_overrides(var body : ExpressionPtr; target_var_name : string) : table<string; bool> {
    var v = new OverrideCollector(target_var_name)
    make_visitor(*v) $(adapter) {
        visit(body, adapter)
    }
    var result <- v.overrides
    unsafe {
        delete v
    }
    return <- result
}

def private make_struct_convert_helper(var src_st : StructurePtr; var tgt_st : StructurePtr;
                                       fn_name : string; var auto_exprs : array<ExpressionPtr>;
                                       at : LineInfo) : FunctionPtr {
    // ``src`` / ``tgt`` literals — ``new`` is a daslang keyword.
    var fn = qmacro_function(fn_name) $(src : $t(src_st); var tgt : $t(tgt_st)) : void {
        $b(auto_exprs)
    }
    fn.flags.generated = true
    fn.flags.privateFunction = true
    fn.body |> force_at(at)
    return <- fn
}

def private make_sct_dispatch(var src_st : StructurePtr; var tgt_st : StructurePtr;
                              user_fn_name : string; at : LineInfo) : FunctionPtr {
    var fn = qmacro_function("_sct") $(src : $t(src_st); var tgt : $t(tgt_st)) : void {
        $c(user_fn_name)(src, tgt)
    }
    fn.flags.generated = true
    fn.body |> force_at(at)
    return <- fn
}

[function_macro(name="struct_convert")]
class SqlStructConvertMacro : AstFunctionAnnotation {
    //! Marks ``def fn(old : S; var dst : T) : void`` for auto-derived field-by-field translation.
    //! Same-name-same-type / ``T → Option<T>`` / ``INT↔STRING↔FLOAT`` cast / ``@sql_renamed_from`` are
    //! filled in; an explicit ``dst.X = …`` (or ``<-``/``:=``) on the LHS suppresses the auto-fill for X.
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        if (func.fromGeneric != null) {
            errors := "[struct_convert]: not supported on generic functions"
            return false
        }
        // Signature: def fn(old : S; var new : T) : void
        let nArgs = func.arguments |> length
        if (nArgs != 2) {
            errors := "[struct_convert] '{func.name}': must take exactly two arguments — `old : S` and `var new : T`. Got {nArgs}."
            return false
        }
        let arg0_type = func.arguments[0]._type
        let arg1_type = func.arguments[1]._type
        if (arg0_type.structType == null) {
            errors := "[struct_convert] '{func.name}': first argument '{func.arguments[0].name}' must be a struct type. Got '{describe(arg0_type)}'."
            return false
        }
        if (arg1_type.structType == null) {
            errors := "[struct_convert] '{func.name}': second argument '{func.arguments[1].name}' must be a struct type. Got '{describe(arg1_type)}'."
            return false
        }
        if (arg1_type.flags.constant) {
            errors := "[struct_convert] '{func.name}': second argument '{func.arguments[1].name}' must be a `var` parameter so the body can assign to its fields. Write `var {func.arguments[1].name} : {describe(arg1_type)}`."
            return false
        }
        if (func.result.baseType != Type.tVoid && func.result.baseType != Type.autoinfer) {
            errors := "[struct_convert] '{func.name}': must return void. Got '{describe(func.result)}'."
            return false
        }
        if (func.body == null || !(func.body is ExprBlock)) {
            errors := "[struct_convert] '{func.name}': body must be a brace-block (the macro prepends auto-fill statements). Use `def {func.name}(...) \{ ... \}`, not the `=> expr` short form."
            return false
        }
        // Strip const off the field-accessed StructurePtrs so `$t($t(...))` reification accepts them.
        // The parsing pipeline gives us const Structure? from a TypeDeclPtr field walk; the gc_node
        // representation is the same pointer either way — reinterpret is safe.
        var src_st : StructurePtr = unsafe(reinterpret<StructurePtr>(arg0_type.structType))
        var tgt_st : StructurePtr = unsafe(reinterpret<StructurePtr>(arg1_type.structType))
        let new_param_name = string(func.arguments[1].name)

        // Collect the user-overrides set by walking the existing body.
        let overrides <- collect_overrides(func.body, new_param_name)

        // Build auto-derived assignments for fields in T order, skipping user-overrides.
        // Nested for-loops because handled-type FieldDeclaration doesn't support indexed-let access —
        // for (f in fields) is the supported iteration form.
        var auto_exprs : array<ExpressionPtr>
        auto_exprs |> reserve(length(tgt_st.fields))
        var failed = false
        for (t_field in tgt_st.fields) {
            if (failed) break
            let tname = string(t_field.name)
            if (key_exists(overrides, tname)) continue
            // Honor @sql_renamed_from on T's field — read from old's <OldName> instead of <tname>.
            let renamed_arg = find_arg(t_field.annotation, "sql_renamed_from")
            let renamed_from = renamed_arg is tString ? string(renamed_arg as tString) : ""
            let lookup_name = empty(renamed_from) ? tname : renamed_from
            let t_type = t_field._type
            let t_is_option = is_option_field_type(t_type)
            // Find matching field in S by lookup_name; nested loop because indexed access on handled FieldDeclaration arrays needs unsafe.
            var s_found = false
            for (s_field in src_st.fields) {
                if (s_field.name != lookup_name) continue
                s_found = true
                // Emit a dispatch call. ``_::`` resolves at the user's module so they can extend the
                // overload set with their own (S_type, T_type) pairs (custom enums, value-types, etc.).
                auto_exprs |> push(qmacro_expr(${
                    _::struct_convert_field(tgt.$f(tname), src.$f(lookup_name));
                }))
                break
            }
            if (failed) break
            if (!s_found) {
                // Field absent from S — use T's default initializer or none() for Option<T>.
                if (t_field.init != null) {
                    let init_expr = t_field.init
                    auto_exprs |> push(qmacro_expr(${
                        tgt.$f(tname) = $e(clone_expression(init_expr));
                    }))
                } elif (t_is_option) {
                    let t_payload = unwrap_option_payload_type(unsafe(reinterpret<TypeDeclPtr>(t_type)))
                    auto_exprs |> push(qmacro_expr(${
                        tgt.$f(tname) = none(type<$t(t_payload)>);
                    }))
                } else {
                    errors := "[struct_convert] '{func.name}': new field '{tname}' has no default initializer and is not Option<T>. Provide a default like `{tname} : {describe(t_type)} = …` on the struct, or write `{new_param_name}.{tname} = …` in the body."
                    failed = true
                }
            }
        }
        if (failed) return false

        // Emit the per-(S,T) helper carrying the auto-derived assignments.
        let helper_name = "_struct_convert_helper`{func.name}"
        var helper_fn = make_struct_convert_helper(src_st, tgt_st, helper_name, auto_exprs, func.at)
        compiling_module() |> add_function(helper_fn)

        // Prepend a call to the helper at the top of the user's body. Build a fresh ExprBlock
        // that contains [helper_call, ...user_body.list] and replace func.body. ExprVar lookups
        // by name resolve to the user's actual params at typecheck time.
        let call_args <- array<ExpressionPtr>(
            new ExprVar(at = func.at, name := func.arguments[0].name),
            new ExprVar(at = func.at, name := func.arguments[1].name)
        )
        var user_body_stmts : array<ExpressionPtr>
        let user_body_block = func.body as ExprBlock
        for (s in user_body_block.list) {
            user_body_stmts |> push(clone_expression(s))
        }
        var combined <- qmacro_block() {
            $c(helper_name)($a(call_args));
            $b(user_body_stmts);
        }
        func.body = combined
        func.body |> force_at(func.at)

        // Emit `_sct(S, T)` dispatch overload.
        var sct_fn = make_sct_dispatch(src_st, tgt_st, string(func.name), func.at)
        compiling_module() |> add_function(sct_fn)

        return true
    }
}

// =====================================================================
// `convert` and `convert_and_rename` — db-driving primitives
// =====================================================================

[unused_argument(t1, t2), template(t1, t2)]
def convert(db : SqlRunner; t1 : type<auto(SS)>; t2 : type<auto(TT)>) : void {
    //! Streams every row of S through the [struct_convert] converter into T's own table.
    //! For the same-name legacy/current convention, use ``convert_and_rename`` instead.
    for (old in db |> select_from(type<SS>)) {
        var new_row : TT = default<TT>
        _::_sct(old, new_row)
        db |> insert(new_row)
    }
}

[unused_argument(t1, t2), template(t1, t2)]
def convert(db : SqlRunner; t1 : type<auto(SS)>; t2 : type<auto(TT)>; name : string) : void {
    //! Like ``convert(type<S>, type<T>)`` but inserts rows into table identifier ``name`` instead
    //! of T's own ``[sql_table(name=…)]``. Used during ``convert_and_rename`` to land rows in the
    //! staging table before the swap.
    for (old in db |> select_from(type<SS>)) {
        var new_row : TT = default<TT>
        _::_sct(old, new_row)
        db |> insert(new_row, name)
    }
}

[unused_argument(t1, t2), template(t1, t2)]
def convert_and_rename(db : SqlRunner; t1 : type<auto(SS)>; t2 : type<auto(TT)>) : void {
    //! Full schema rebuild: CREATE staging, copy via [struct_convert], DROP original, RENAME.
    //! S and T must share the same ``[sql_table(name=…)]`` (legacy/current convention).
    let source = _::_sql_table_name(default<SS>)
    let target = _::_sql_table_name(default<TT>)
    if (source != target) {
        panic("convert_and_rename: source '{source}' and target '{target}' must share the same [sql_table(name=...)]. Use `db |> convert(type<S>, type<T>, name=...)` for cross-table conversions.")
    }
    let staging = "{target}_new"
    // Staging table is created WITHOUT indexes — explicit `[sql_index(name=...)]` would
    // collide with the original's still-existing index of the same name (SQLite index
    // names are schema-global). Indexes are recreated on the renamed target below.
    db |> exec(_::_sql_create_table_sql(default<TT>, staging))
    db |> convert(type<SS>, type<TT>, staging)
    db |> exec("DROP TABLE {sql_quote_id(target)}")
    db |> exec("ALTER TABLE {sql_quote_id(staging)} RENAME TO {sql_quote_id(target)}")
    let idx_sql = _::_sql_create_indexes_sql(default<TT>)
    if (!empty(idx_sql)) {
        db |> exec(idx_sql)
    }
}
