options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: provider-neutral migrations (daslib/sql_migrate + the provider's glue
// module) — apply order, idempotence, pending/history inspection, baseline adoption,
// failure rollback, and the [struct_convert] rebuild rail. Migrations registered here
// live in this provider's STREAM (keyed by the runner type of the body's parameter).

require dastest/testing_boost public

require _conformance_provider

// ===== registered migrations (program-global registry, fresh DB per test) =====

// Current shape of conf_m1 — used only to READ BACK what the migrations did
// (the bodies themselves use raw DDL, as real early migrations must).
[sql_table(name = "conf_m1")]
struct private ConfM1 {
    @sql_primary_key Id : int
    X : int
}

[sql_migration(version = 1, description = "create conf_m1")]
def private conf_migration_v1(db : SqlRunner) {
    // quoted identifiers: unquoted names case-fold on PostgreSQL, and the
    // [sql_table] read-back below emits quoted "Id"/"X"
    db |> exec("CREATE TABLE conf_m1 (\"Id\" INTEGER PRIMARY KEY, \"X\" INTEGER)")
}

[sql_migration(version = 2, description = "create conf_m2")]
def private conf_migration_v2(db : SqlRunner) {
    db |> exec("CREATE TABLE conf_m2 (\"Y\" INTEGER)")
}

[sql_migration(version = 3, description = "seed conf_m1")]
def private conf_migration_v3(db : SqlRunner) {
    db |> exec("INSERT INTO conf_m1 (\"Id\", \"X\") VALUES (?, ?)", 1, 42)
}

// v4 is benign by default; the failure-rollback test flips the flag for one run.
var private g_fail_v4 = false

[sql_migration(version = 4, description = "flag-gated failure probe"), unused_argument(db)]
def private conf_migration_v4(db : SqlRunner) {
    if (g_fail_v4) {
        panic("intentional v4 failure (conformance rollback probe)")
    }
}

[test(description = "migrate_to_latest applies pending migrations in order, is idempotent and reports pending and history correctly")]
def test_conf_migrate_apply(t : T?) {
    t |> run("applies all pending in order; idempotent on re-run") @(t : T?) {
        with_conf_db() $(db) {
            t |> equal(db |> migrate_to_latest(), 4)
            t |> equal(db |> current_schema_version(), 4)
            t |> equal(_sql(db |> select_from(type<ConfM1>) |> _first()).X, 42)
            // second run: nothing pending, applies 0
            t |> equal(db |> migrate_to_latest(), 0)
            t |> equal(db |> current_schema_version(), 4)
        }
    }
    t |> run("pending before, empty after; descriptions from annotations") @(t : T?) {
        with_conf_db() $(db) {
            let before <- db |> pending_migrations()
            t |> equal(before |> length, 4)
            t |> equal(before[0].version, 1)
            t |> equal(before[0].description, "create conf_m1")
            t |> equal(before[3].version, 4)
            db |> migrate_to_latest()
            let after <- db |> pending_migrations()
            t |> equal(after |> length, 0)
        }
    }
    t |> run("history: frozen descriptions, ascending, epoch stamped") @(t : T?) {
        with_conf_db() $(db) {
            // absent audit table -> empty history, version 0
            t |> equal(db |> current_schema_version(), 0)
            let none_yet <- db |> migration_history()
            t |> equal(none_yet |> length, 0)
            db |> migrate_to_latest()
            let hist <- db |> migration_history()
            t |> equal(hist |> length, 4)
            t |> equal(hist[0].version, 1)
            t |> equal(hist[0].description, "create conf_m1")
            t |> success(hist[0].applied_at > int64(0))
            t |> equal(hist[3].version, 4)
        }
    }
}

[test(description = "baseline stamps versions without running bodies and rejects a version above the registered maximum")]
def test_conf_migrate_baseline(t : T?) {
    t |> run("baseline stamps without running bodies; migrate applies the rest") @(t : T?) {
        with_conf_db() $(db) {
            db |> baseline(2)
            t |> equal(db |> current_schema_version(), 2)
            // v1/v2 bodies did NOT run — conf_m1 does not exist yet
            let pend <- db |> pending_migrations()
            t |> equal(pend |> length, 2)
            t |> equal(pend[0].version, 3)
            // v3 inserts into conf_m1, which baseline skipped creating — expected failure
            let r <- db |> try_migrate_to_latest()
            t |> success(r |> is_err)
            // baselined versions keep their registry descriptions in the audit
            let hist <- db |> migration_history()
            t |> equal(hist |> length, 2)
            t |> equal(hist[0].description, "create conf_m1")
        }
    }
    t |> run("baseline above max registered is rejected") @(t : T?) {
        with_conf_db() $(db) {
            let r <- db |> try_baseline(99)
            t |> success(r |> is_err)
        }
    }
}

[test(description = "a failing migration body rolls back the whole call and a retry succeeds")]
def test_conf_migrate_rollback(t : T?) {
    t |> run("body failure rolls back the whole call; retry succeeds") @(t : T?) {
        with_conf_db() $(db) {
            g_fail_v4 = true
            let r <- db |> try_migrate_to_latest()
            t |> success(r |> is_err)
            // v1..v3 applied inside the same txn as failing v4 — everything rolled back
            t |> equal(db |> current_schema_version(), 0)
            g_fail_v4 = false
            t |> equal(db |> migrate_to_latest(), 4)
            t |> equal(db |> current_schema_version(), 4)
        }
    }
}

// ===== [struct_convert] + convert_and_rename (portable rebuild rail) =====

[sql_table(name = "ConfShape", legacy = true)]
struct private ConfShapeOld {
    @sql_primary_key Id : int
    Label : string
}

[sql_table(name = "ConfShape")]
struct private ConfShape {
    @sql_primary_key Id : int
    Label : string
    Area : double = 1.0lf
}

[struct_convert]
def private conf_shape_old_to_new(old : ConfShapeOld; var fresh : ConfShape) {
    fresh.Area = 2.0lf
}

[test(description = "convert_and_rename rebuilds a table with translated rows")]
def test_conf_struct_convert_rebuild(t : T?) {
    t |> run("convert_and_rename rebuilds the table with translated rows") @(t : T?) {
        with_conf_db() $(db) {
            db |> exec("CREATE TABLE \"ConfShape\" (\"Id\" INTEGER PRIMARY KEY, \"Label\" TEXT NOT NULL)")
            db |> exec("INSERT INTO \"ConfShape\" (\"Id\", \"Label\") VALUES (1, 'one'), (2, 'two')")
            db |> convert_and_rename(type<ConfShapeOld>, type<ConfShape>)
            let rows <- _sql(db |> select_from(type<ConfShape>) |> _order_by(_.Label))
            t |> equal(rows |> length, 2)
            t |> equal(rows[0].Label, "one")
            t |> equal(rows[0].Area, 2.0lf)
        }
    }
}
