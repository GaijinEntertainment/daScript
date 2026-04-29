options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Basic shapes: 0/1/2/3/4-arg, every supported type round-trips correctly.

def fn_zero() : int64 {
    return 42l
}

def fn_one_int(a : int) : int {
    return a * 2
}

def fn_one_int64(a : int64) : int64 {
    return a + 1l
}

def fn_one_double(a : double) : double {
    return a * 2.5lf
}

def fn_one_float(a : float) : float {
    return a * 0.5f
}

def fn_one_string(a : string) : string {
    return a
}

def fn_one_bool(a : bool) : bool {
    return !a
}

def fn_two_mixed(a : int; b : double) : double {
    return double(a) + b
}

def fn_three_strings(a : string; b : string; c : string) : string {
    return a + b + c
}

def fn_four_int(a : int; b : int; c : int; d : int) : int {
    return a + b + c + d
}

[test]
def test_zero_arg(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_zero", @@fn_zero)
        let v = db |> query_scalar("SELECT fn_zero()", type<int64>)
        t |> equal(v, 42l, "zero-arg int64 result")
    }
}

[test]
def test_one_arg_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_int", @@fn_one_int)
        let v = db |> query_scalar("SELECT fn_one_int(7)", type<int>)
        t |> equal(v, 14, "int*2")
    }
}

[test]
def test_one_arg_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_int64", @@fn_one_int64)
        let v = db |> query_scalar("SELECT fn_one_int64(100)", type<int64>)
        t |> equal(v, 101l, "int64+1")
    }
}

[test]
def test_one_arg_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_double", @@fn_one_double)
        let v = db |> query_scalar("SELECT fn_one_double(2.0)", type<double>)
        t |> equal(v, 5.0lf, "double*2.5")
    }
}

[test]
def test_one_arg_float(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_float", @@fn_one_float)
        let v = db |> query_scalar("SELECT fn_one_float(8.0)", type<float>)
        t |> equal(v, 4.0f, "float*0.5")
    }
}

[test]
def test_one_arg_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_string", @@fn_one_string)
        let v = db |> query_scalar("SELECT fn_one_string('hello')", type<string>)
        t |> equal(v, "hello", "string round-trip")
    }
}

[test]
def test_one_arg_bool(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_one_bool", @@fn_one_bool)
        let r = db |> query_scalar("SELECT fn_one_bool(0)", type<int>)
        t |> equal(r, 1, "bool: !false = true (1)")
        let r2 = db |> query_scalar("SELECT fn_one_bool(1)", type<int>)
        t |> equal(r2, 0, "bool: !true = false (0)")
    }
}

[test]
def test_two_arg_mixed(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_two_mixed", @@fn_two_mixed)
        let v = db |> query_scalar("SELECT fn_two_mixed(3, 0.5)", type<double>)
        t |> equal(v, 3.5lf, "int+double mixed")
    }
}

[test]
def test_three_arg_string(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_three_strings", @@fn_three_strings)
        let v = db |> query_scalar("SELECT fn_three_strings('a', 'b', 'c')", type<string>)
        t |> equal(v, "abc", "concat 3 strings")
    }
}

[test]
def test_four_arg_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> register_function("fn_four_int", @@fn_four_int)
        let v = db |> query_scalar("SELECT fn_four_int(1, 2, 3, 4)", type<int>)
        t |> equal(v, 10, "sum of 4 ints")
    }
}
