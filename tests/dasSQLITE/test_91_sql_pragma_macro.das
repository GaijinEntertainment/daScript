options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[test]
def test_sql_pragma_const_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> _sql_pragma("journal_mode", "MEMORY")
        t |> equal(err |> is_some, false, "const args succeed")
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        t |> equal(mode, "memory", "round-trips through SQLite")
    }
}

[test]
def test_sql_pragma_runtime_value(t : T?) {
    with_sqlite(":memory:") $(db) {
        let dynamic_value = "MEMORY"
        let err = db |> _sql_pragma("journal_mode", dynamic_value)
        t |> equal(err |> is_some, false, "runtime value succeeds")
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        t |> equal(mode, "memory", "runtime path produces same SQL")
    }
}

[test]
def test_sql_pragma_runtime_name(t : T?) {
    with_sqlite(":memory:") $(db) {
        let dynamic_name = "journal_mode"
        let err = db |> _sql_pragma(dynamic_name, "MEMORY")
        t |> equal(err |> is_some, false, "runtime name succeeds")
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        t |> equal(mode, "memory", "runtime name path works")
    }
}

[test]
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

[test]
def test_sql_pragma_unknown_name_silently_succeeds(t : T?) {
    with_sqlite(":memory:") $(db) {
        let err = db |> _sql_pragma("xyzzy_not_a_pragma", "any")
        t |> equal(err |> is_some, false, "unknown pragma is a no-op (matches try_set_pragma)")
    }
}
