options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[test]
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

[test]
def test_set_pragma_int_busy_timeout(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> set_pragma("busy_timeout", 1500l)
        let ms = db |> query_scalar("PRAGMA busy_timeout", type<int>)
        t |> equal(ms, 1500, "busy_timeout round-trips")
    }
}

[test]
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

[test]
def test_try_set_pragma_unknown_succeeds_silently(t : T?) {
    with_sqlite(":memory:") $(db) {
        // SQLite is famously permissive with unknown pragma names — they
        // succeed silently rather than erroring. Document the behavior.
        let err = db |> try_set_pragma("not_a_real_pragma_xyzzy", "any")
        t |> equal(err |> is_some, false, "unknown pragma is a no-op")
    }
}

[test]
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

[test]
def test_pragma_string_value_with_quote_escapes(t : T?) {
    with_sqlite(":memory:") $(db) {
        // The string overload doubles embedded single quotes so the SQL
        // stays well-formed. Use a synthetic pragma name (which SQLite
        // ignores) so the test is portable across versions.
        let err = db |> try_set_pragma("xyzzy_test_pragma", "a''b")
        t |> equal(err |> is_some, false, "embedded quotes don't break the SQL")
    }
}
