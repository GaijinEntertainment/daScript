options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Lifetime semantics:
//   1. Re-registering the same name replaces the prior binding cleanly.
//   2. A registered function lives only on its connection — it is not
//      visible from a different connection (per-DB scope).
//   3. Across connection close + open the binding does not survive.

def fn_v1(x : int) : int {
    return x + 100
}

def fn_v2(x : int) : int {
    return x + 200
}

[test]
def test_replace_by_re_registering(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn", @@fn_v1)
        let r1 = db |> query_scalar("SELECT fn(0)", type<int>)
        t |> equal(r1, 100, "v1 active")
        // Re-register with a different daslang fn under the same name.
        db |> register_function("fn", @@fn_v2)
        let r2 = db |> query_scalar("SELECT fn(0)", type<int>)
        t |> equal(r2, 200, "v2 replaces v1")
    }
}

[test]
def test_per_connection_scope(t : T?) {
    var inscope db_a = open_sqlite(":memory:")
    var inscope db_b = open_sqlite(":memory:")
    db_a |> register_function("fn", @@fn_v1)
    // db_a sees it.
    let ra = db_a |> query_scalar("SELECT fn(1)", type<int>)
    t |> equal(ra, 101, "fn registered on db_a")
    // db_b does NOT see it — query should error with "no such function".
    let rb <- db_b |> try_query_scalar("SELECT fn(1)", type<int>)
    t |> equal(rb |> is_err, true, "fn not visible on db_b")
    let msg = rb |> unwrap_err
    t |> equal(find(msg, "no such function") >= 0, true,
        "error mentions 'no such function' — got: '{msg}'")
}

[test]
def test_does_not_persist_across_open(t : T?) {
    {
        var inscope db = open_sqlite(":memory:")
        db |> register_function("fn", @@fn_v1)
        let v = db |> query_scalar("SELECT fn(2)", type<int>)
        t |> equal(v, 102, "registered on first connection")
    }
    // Fresh `:memory:` connection has its own (empty) function table.
    var inscope db2 = open_sqlite(":memory:")
    let r <- db2 |> try_query_scalar("SELECT fn(0)", type<int>)
    t |> equal(r |> is_err, true, "fn does not survive new connection")
}
