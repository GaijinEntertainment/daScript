options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

[sql_migration(version = 1, description = "v1")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE x (a INT)")
}

[sql_migration(version = 2, description = "v2")]
def m2(db : SqlRunner) {
    db |> exec("CREATE TABLE y (b INT)")
}

[test(description = "a second baseline call stamps nothing and leaves the audit unchanged")]
def test_baseline_twice_is_idempotent(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let r1 <- db |> try_baseline(2)
    let r2 <- db |> try_baseline(2)
    t |> equal(r1 |> unwrap, 2, "first call stamps 2 rows")
    t |> equal(r2 |> unwrap, 0, "second call stamps 0 rows (idempotent)")

    let n = db |> query_scalar("SELECT COUNT(*) FROM __schema_version", type<int>)
    t |> equal(n, 2, "audit row count unchanged after idempotent rerun")
}

[test(description = "try_baseline above the highest registered version returns an error and creates no audit table")]
def test_try_baseline_rejects_above_max(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let r <- db |> try_baseline(5)               // registered max is 2
    t |> success(r |> is_err, "try_baseline(5) must return Err — above registered max")

    let audit_exists = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE name='__schema_version'", type<int>)
    t |> equal(audit_exists, 0, "rejected baseline must not create the audit table")
}

[test(description = "baseline above the highest registered version panics")]
def test_strict_baseline_panics_above_max(t : T?) {
    var inscope db = open_sqlite(":memory:")
    var caught = false
    try {
        db |> baseline(5)
    } recover {
        caught = true
    }
    t |> success(caught, "strict baseline must panic when above max")
}

[test(description = "baseline rejects version 0 and negative versions")]
def test_baseline_zero_or_negative_rejected(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let r0 <- db |> try_baseline(0)
    let rn <- db |> try_baseline(-1)
    t |> success(r0 |> is_err, "version=0 must be rejected")
    t |> success(rn |> is_err, "version=-1 must be rejected")
}
