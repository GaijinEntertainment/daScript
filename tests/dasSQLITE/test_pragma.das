options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

[test(description = "set_pragma with a string value sets journal_mode and it reads back")]
def test_set_pragma_string_journal_mode(t : T?) {
    with_sqlite(":memory:") $(db) {
        // :memory: databases ignore journal_mode at the disk level but
        // accept the pragma syntax — fine for round-tripping the value.
        db |> set_pragma("journal_mode", "MEMORY")
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        // SQLite normalizes pragma values to lowercase.
        t |> equal(mode, "memory", "journal_mode round-trips")
    }
}

[test(description = "set_pragma with an int value sets busy_timeout and it reads back")]
def test_set_pragma_int_busy_timeout(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> set_pragma("busy_timeout", 1500l)
        let ms = db |> query_scalar("PRAGMA busy_timeout", type<int>)
        t |> equal(ms, 1500, "busy_timeout round-trips")
    }
}

[test(description = "set_pragma with a bool value switches foreign_keys off and on")]
def test_set_pragma_bool_foreign_keys(t : T?) {
    with_sqlite(":memory:") $(db) {
        // with_sqlite turns FKs on by default; toggle off + on to exercise both branches.
        db |> set_pragma("foreign_keys", false)
        let off = db |> query_scalar("PRAGMA foreign_keys", type<int>)
        t |> equal(off, 0, "foreign_keys = OFF")

        db |> set_pragma("foreign_keys", true)
        let on = db |> query_scalar("PRAGMA foreign_keys", type<int>)
        t |> equal(on, 1, "foreign_keys = ON")
    }
}

[test(description = "try_set_pragma with an unknown pragma name returns no error")]
def test_try_set_pragma_unknown_succeeds_silently(t : T?) {
    with_sqlite(":memory:") $(db) {
        // SQLite is famously permissive with unknown pragma names — they
        // succeed silently rather than erroring. Document the behavior.
        let err = db |> try_set_pragma("not_a_real_pragma_xyzzy", "any")
        t |> equal(err |> is_some, false, "unknown pragma is a no-op")
    }
}

[test(description = "apply_recommended_pragmas succeeds and sets busy_timeout and foreign_keys")]
def test_apply_recommended_pragmas(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> try_apply_recommended_pragmas()
        t |> equal(err |> is_some, false, "recommended pragmas applied without error")
        // Verify the boolean / int values landed (journal_mode/synchronous are
        // accepted but :memory: normalizes journal_mode to memory).
        let busy = db |> query_scalar("PRAGMA busy_timeout", type<int>)
        t |> equal(busy, 5000, "busy_timeout = 5000")
        let fks = db |> query_scalar("PRAGMA foreign_keys", type<int>)
        t |> equal(fks, 1, "foreign_keys ON")
    }
}

[test(description = "a pragma string value containing quotes does not break the statement")]
def test_pragma_string_value_with_quote_escapes(t : T?) {
    with_sqlite(":memory:") $(db) {
        // The string overload doubles embedded single quotes so the SQL
        // stays well-formed. Use a synthetic pragma name (which SQLite
        // ignores) so the test is portable across versions.
        let err = db |> try_set_pragma("xyzzy_test_pragma", "a''b")
        t |> equal(err |> is_some, false, "embedded quotes don't break the SQL")
    }
}

[test(description = "the _sql_pragma macro with constant arguments sets the pragma")]
def test_sql_pragma_const_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> _sql_pragma("journal_mode", "MEMORY")
        t |> equal(err |> is_some, false, "const args succeed")
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        t |> equal(mode, "memory", "round-trips through SQLite")
    }
}

[test(description = "the _sql_pragma macro with a runtime value sets the pragma")]
def test_sql_pragma_runtime_value(t : T?) {
    with_sqlite(":memory:") $(db) {
        let dynamic_value = "MEMORY"
        let err = db |> _sql_pragma("journal_mode", dynamic_value)
        t |> equal(err |> is_some, false, "runtime value succeeds")
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        t |> equal(mode, "memory", "runtime path produces same SQL")
    }
}

[test(description = "the _sql_pragma macro with a runtime pragma name sets the pragma")]
def test_sql_pragma_runtime_name(t : T?) {
    with_sqlite(":memory:") $(db) {
        let dynamic_name = "journal_mode"
        let err = db |> _sql_pragma(dynamic_name, "MEMORY")
        t |> equal(err |> is_some, false, "runtime name succeeds")
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        t |> equal(mode, "memory", "runtime name path works")
    }
}

[test(description = "the _sql_pragma macro escapes a single quote in the value")]
def test_sql_pragma_value_quote_safety(t : T?) {
    with_sqlite(":memory:") $(db) {
        // Embedded single quote in the value — must be doubled to keep SQLite
        // happy. SQLite ignores unknown pragma names silently (returns no
        // error) so we can assert against the no-crash path.
        let tricky = "it's tricky"
        let err = db |> _sql_pragma("not_real_pragma", tricky)
        t |> equal(err |> is_some, false, "single-quote in value parses cleanly")
    }
}

[test(description = "the _sql_pragma macro with an unknown name is a no-op like try_set_pragma")]
def test_sql_pragma_unknown_name_silently_succeeds(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> _sql_pragma("xyzzy_not_a_pragma", "any")
        t |> equal(err |> is_some, false, "unknown pragma is a no-op (matches try_set_pragma)")
    }
}
