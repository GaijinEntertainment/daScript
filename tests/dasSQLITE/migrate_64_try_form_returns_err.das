options gen2

require dastest/testing_boost public
require sqlite/sql_migrate
require daslib/strings_boost

[sql_migration(version = 1, description = "good")]
def m1(db : SqlRunner) {
    db |> exec("CREATE TABLE x (a INT)")
}

[sql_migration(version = 2, description = "broken — references missing function")]
def m2(db : SqlRunner) {
    db |> exec("SELECT not_a_real_function(1)")
}

[test]
def test_try_form_returns_err_with_enriched_message(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let r <- db |> try_migrate_to_latest()
    t |> success(r |> is_err, "try_migrate_to_latest must return Err")

    let msg = r |> unwrap_err
    t |> success(msg |> contains("v2"), "Err must name the failing version")
    t |> success(msg |> contains("broken"), "Err must include the description")
    t |> success(msg |> contains("DB does not need to be reset"),
        "Err must contain the no-reset reassurance")
    t |> success(msg |> contains("rolled back"),
        "Err must mention rollback")
}
