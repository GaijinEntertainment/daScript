options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq

// Basic shapes: 0/1/2/3/4-arg, every supported type round-trips correctly.
def fn_zero() : int64 => 42l

def fn_one_int(a : int) : int => a * 2

def fn_one_int64(a : int64) : int64 => a + 1l

def fn_one_double(a : double) : double => a * 2.5lf

def fn_one_float(a : float) : float => a * 0.5f

def fn_one_string(a : string) : string => a

def fn_one_bool(a : bool) : bool => !a

def fn_two_mixed(a : int; b : double) : double => double(a) + b

def fn_three_strings(a : string; b : string; c : string) : string => a + b + c

def fn_four_int(a : int; b : int; c : int; d : int) : int => a + b + c + d

[test(description = "a registered zero-argument function is callable from SQL and returns its int64 result")]
def test_zero_arg(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_zero", @@fn_zero)
        let v = db |> query_scalar("SELECT fn_zero()", type<int64>)
        t |> equal(v, 42l, "zero-arg int64 result")
    }
}

[test(description = "a registered one-argument function receives an int and returns an int")]
def test_one_arg_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_int", @@fn_one_int)
        let v = db |> query_scalar("SELECT fn_one_int(7)", type<int>)
        t |> equal(v, 14, "int*2")
    }
}

[test(description = "a registered one-argument function receives and returns an int64")]
def test_one_arg_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_int64", @@fn_one_int64)
        let v = db |> query_scalar("SELECT fn_one_int64(100)", type<int64>)
        t |> equal(v, 101l, "int64+1")
    }
}

[test(description = "a registered one-argument function receives and returns a double")]
def test_one_arg_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_double", @@fn_one_double)
        let v = db |> query_scalar("SELECT fn_one_double(2.0)", type<double>)
        t |> equal(v, 5.0lf, "double*2.5")
    }
}

[test(description = "a registered one-argument function receives and returns a float")]
def test_one_arg_float(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_float", @@fn_one_float)
        let v = db |> query_scalar("SELECT fn_one_float(8.0)", type<float>)
        t |> equal(v, 4.0f, "float*0.5")
    }
}

[test(description = "a registered one-argument function receives and returns a string")]
def test_one_arg_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_string", @@fn_one_string)
        let v = db |> query_scalar("SELECT fn_one_string('hello')", type<string>)
        t |> equal(v, "hello", "string round-trip")
    }
}

[test(description = "a registered function receives and returns a bool as 0 or 1")]
def test_one_arg_bool(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_bool", @@fn_one_bool)
        let r = db |> query_scalar("SELECT fn_one_bool(0)", type<int>)
        t |> equal(r, 1, "bool: !false = true (1)")
        let r2 = db |> query_scalar("SELECT fn_one_bool(1)", type<int>)
        t |> equal(r2, 0, "bool: !true = false (0)")
    }
}

[test(description = "a registered two-argument function accepts mixed int and double arguments")]
def test_two_arg_mixed(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_two_mixed", @@fn_two_mixed)
        let v = db |> query_scalar("SELECT fn_two_mixed(3, 0.5)", type<double>)
        t |> equal(v, 3.5lf, "int+double mixed")
    }
}

[test(description = "a registered three-argument function accepts three strings")]
def test_three_arg_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_three_strings", @@fn_three_strings)
        let v = db |> query_scalar("SELECT fn_three_strings('a', 'b', 'c')", type<string>)
        t |> equal(v, "abc", "concat 3 strings")
    }
}

[test(description = "a registered four-argument function accepts four ints")]
def test_four_arg_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_four_int", @@fn_four_int)
        let v = db |> query_scalar("SELECT fn_four_int(1, 2, 3, 4)", type<int>)
        t |> equal(v, 10, "sum of 4 ints")
    }
}

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

def fn_panic2(x : int) : int => x * x

[test(description = "a NULL argument makes the SQL call yield NULL without invoking the daslang function")]
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

[test(description = "a panic in a registered function surfaces as a query error and the connection keeps working")]
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

[test(description = "a registered function can be called twice inside one expression")]
def test_panic_inside_aggregation(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_panic2", @@fn_panic2)
        // Square some values; verify normal aggregation works.
        let r = db |> query_scalar(
            "SELECT fn_panic2(2) + fn_panic2(3)", type<int>)
        t |> equal(r, 13, "4 + 9 = 13")
    }
}

// Registered functions should be usable in WHERE / ORDER BY / projection
// just like a built-in scalar. We verify that a deterministic UDF embeds
// into common chain shapes.

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Item>)
    db |> insert(Item(Id = 1, Name = "alpha", Score = 10))
    db |> insert(Item(Id = 2, Name = "beta",  Score = 30))
    db |> insert(Item(Id = 3, Name = "gamma", Score = 20))
    db |> insert(Item(Id = 4, Name = "delta", Score = 50))
}

def doubler(x : int) : int => x * 2

def add_prefix(s : string) : string => "X_" + s

[test(description = "a registered function can be used in a where of a _sql chain")]
def test_udf_in_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> register_function("doubler", @@doubler, true)
        // doubler(Score) > 30 means Score > 15 → Items 2,3,4 match.
        let n = db |> query_scalar("SELECT COUNT(*) FROM Items WHERE doubler(Score) > 30", type<int>)
        t |> equal(n, 3, "doubler(Score) > 30 — 3 rows")
    }
}

[test(description = "a registered function can be used as an order_by key")]
def test_udf_in_order_by(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> register_function("doubler", @@doubler, true)
        // ORDER BY doubler(Score) DESC ⇒ items by Score desc: 50, 30, 20, 10
        let top = db |> query_scalar(
            "SELECT Name FROM Items ORDER BY doubler(Score) DESC LIMIT 1",
            type<string>)
        t |> equal(top, "delta", "highest score is delta")
    }
}

[test(description = "a registered string function can be used in a projection")]
def test_udf_string_in_projection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> register_function("add_prefix", @@add_prefix, true)
        let v = db |> query_scalar("SELECT add_prefix(Name) FROM Items WHERE Id = 1", type<string>)
        t |> equal(v, "X_alpha", "prefix prepended")
    }
}

// Lifetime semantics:
//   1. Re-registering the same name replaces the prior binding cleanly.
//   2. A registered function lives only on its connection — it is not
//      visible from a different connection (per-DB scope).
//   3. Across connection close + open the binding does not survive.

def fn_v1(x : int) : int => x + 100

def fn_v2(x : int) : int => x + 200

[test(description = "registering a name again replaces the previous function")]
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

[test(description = "a registered function is visible only on the connection it was registered on")]
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

[test(description = "a registered function does not survive into a newly opened connection")]
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
