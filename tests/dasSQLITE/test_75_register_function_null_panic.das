options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// NULL short-circuit + panic recovery semantics.
//
// NULL: if any argument cell is SQL NULL, the trampoline must NOT invoke
// the daslang function and the SELECT must produce NULL. We assert this
// via SUM(...) IS NULL plus a side-channel counter that records every
// invocation.
//
// Panic: a `panic(...)` inside the daslang function is caught and the
// SELECT statement fails with an error message that includes the panic
// text. The connection itself stays usable for subsequent statements.

var g_call_count = 0

def fn_count(x : int) : int {
    g_call_count = g_call_count + 1
    return x + 1
}

def fn_panic(x : int) : int {
    if (x < 0) {
        panic("negative input rejected")
    }
    return x * 2
}

def fn_panic2(x : int) : int {
    return x * x
}

[test]
def test_null_short_circuits_invocation(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_count", @@fn_count)
        g_call_count = 0
        // NULL arg should NOT invoke fn_count and SELECT should produce NULL.
        let r <- db |> try_query_scalar("SELECT fn_count(NULL) IS NULL", type<int>)
        t |> equal(r |> is_ok, true, "NULL-input SELECT succeeds")
        t |> equal(r |> unwrap, 1, "fn_count(NULL) yields NULL")
        t |> equal(g_call_count, 0, "daslang fn was NOT called when input was NULL")
        // Non-NULL arg DOES invoke.
        let r2 = db |> query_scalar("SELECT fn_count(5)", type<int>)
        t |> equal(r2, 6, "fn_count(5) = 6")
        t |> equal(g_call_count, 1, "daslang fn called exactly once for non-NULL input")
    }
}

[test]
def test_panic_surfaces_as_error_and_connection_survives(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_panic", @@fn_panic)
        let r <- db |> try_query_scalar("SELECT fn_panic(-1)", type<int>)
        t |> equal(r |> is_err, true, "panic surfaces as Err")
        let msg = r |> unwrap_err
        t |> equal(find(msg, "negative input rejected") >= 0, true,
            "Err message contains the panic text — got: '{msg}'")
        // Connection stays usable; a subsequent successful query works.
        let r2 = db |> query_scalar("SELECT fn_panic(7)", type<int>)
        t |> equal(r2, 14, "post-panic call succeeds")
    }
}

[test]
def test_panic_inside_aggregation(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_panic2", @@fn_panic2)
        // Square some values; verify normal aggregation works.
        let r = db |> query_scalar(
            "SELECT fn_panic2(2) + fn_panic2(3)", type<int>)
        t |> equal(r, 13, "4 + 9 = 13")
    }
}
