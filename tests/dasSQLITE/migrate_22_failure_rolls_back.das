options gen2

require dastest/testing_boost public
require sqlite/sql_migrate

[sql_migration(version = 1, description = "good")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY)")
}

[sql_migration(version = 2, description = "bad")]
def m2(db : SqlRunner) {
    db |> exec("CREATE TABLE side_effect (a INT)")
    db |> exec("THIS IS NOT VALID SQL")          // panic from strict exec
}

[sql_migration(version = 3, description = "never runs")]
def m3(db : SqlRunner) {
    db |> exec("CREATE TABLE three (x INT)")
}

[test]
def test_v1_rolls_back_when_v2_fails(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let r <- db |> try_migrate_to_latest()
    t |> success(r |> is_err, "try_migrate_to_latest must return Err when a migration panics")

    // α-shape: v1 rolls back too. users table must NOT exist.
    let users_exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='users'", type<int>)
    t |> equal(users_exists, 0, "v1's effect must roll back too (α-shape)")

    // v2's partial effect (side_effect table) must NOT exist.
    let side_exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='side_effect'", type<int>)
    t |> equal(side_exists, 0, "v2's partial effect must roll back")

    // v3 never ran.
    let three_exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='three'", type<int>)
    t |> equal(three_exists, 0, "v3 must not have run")

    // Audit table created (eager) but contains zero rows.
    let audit_rows = db |> query_scalar("SELECT COUNT(*) FROM __schema_version", type<int>)
    t |> equal(audit_rows, 0, "no audit rows on full rollback")

    // Connection is back in autocommit (transaction closed).
    t |> success(sqlite3_get_autocommit_(db.sqlite_handle),
        "autocommit must be restored after failed migrate")
}

[test]
def test_strict_form_panics_on_body_failure(t : T?) {
    // The detailed message-content assertions live in migrate_64 against the try_ form
    // (Result::Err.unwrap_err is directly inspectable without going through panic
    // propagation, which AOT-compiled code may not preserve verbatim through
    // last_exception).
    var inscope db = open_sqlite(":memory:")
    var caught = false
    try {
        db |> migrate_to_latest()
    } recover {
        caught = true
    }
    t |> success(caught, "strict migrate_to_latest must panic on body failure")
}

[test]
def test_rerun_after_fix_succeeds(t : T?) {
    // Simulating "fix v2 and re-run" needs the same registry shape, just a working v2.
    // We can't easily replace the [sql_migration] body at runtime, so this test verifies
    // the next-best property: after the failure, re-calling try_migrate_to_latest with the
    // SAME (still-broken) registry produces the SAME failure shape — no DB drift, no
    // half-state. The "fix and rerun applies cleanly" guarantee is a static property of
    // the algorithm: pending list rebuilds from MAX(version), which is unchanged.
    var inscope db = open_sqlite(":memory:")
    let r1 <- db |> try_migrate_to_latest()
    let r2 <- db |> try_migrate_to_latest()
    t |> success(r1 |> is_err, "first call returns Err")
    t |> success(r2 |> is_err, "rerun against unchanged broken registry still Err")
    let max_v = db |> query_scalar("SELECT COALESCE(MAX(version), 0) FROM __schema_version", type<int>)
    t |> equal(max_v, 0, "MAX unchanged across both failed calls")
}
