options gen2

require dastest/testing_boost public
require daslib/result
require sqlite/sqlite_boost
require daslib/sql
require daslib/option

[test(description = "query_scalar reads an integer result")]
def test_query_scalar_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 42", type<int>)
        t |> equal(v, 42, "int scalar")
    }
}

[test(description = "query_scalar reads a negative integer")]
def test_query_scalar_int_negative(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT -7", type<int>)
        t |> equal(v, -7, "int scalar negative")
    }
}

[test(description = "try_query_scalar returns Ok carrying the integer")]
def test_try_query_scalar_int_ok(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 100", type<int>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 100, "value")
    }
}

[test(description = "query_scalar with an int64 type reads the largest 64-bit value without loss")]
def test_query_scalar_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 9223372036854775807", type<int64>)
        t |> equal(v, 9223372036854775807l, "int64 max")
    }
}

[test(description = "query_scalar with an int64 type reads a COUNT result")]
def test_query_scalar_int64_count(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE t (x INT)")
        db |> exec("INSERT INTO t VALUES (1), (2), (3)")
        let v = db |> query_scalar("SELECT COUNT(*) FROM t", type<int64>)
        t |> equal(int(v), 3, "count(*) -> int64")
    }
}

[test(description = "query_scalar reads a double result")]
def test_query_scalar_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 3.14", type<double>)
        t |> equal(v, 3.14lf, "double scalar")
    }
}

[test(description = "query_scalar keeps double precision")]
def test_query_scalar_double_pi(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 3.141592653589793", type<double>)
        t |> success(v > 3.14lf && v < 3.15lf, "double precision")
    }
}

[test(description = "query_scalar reads 1 as true")]
def test_query_scalar_bool_true(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 1", type<bool>)
        t |> equal(v, true, "1 -> true")
    }
}

[test(description = "query_scalar reads 0 as false")]
def test_query_scalar_bool_false(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 0", type<bool>)
        t |> equal(v, false, "0 -> false")
    }
}

[test(description = "query_scalar reads the result of a SQL comparison as a bool")]
def test_query_scalar_bool_from_predicate(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT 5 > 3", type<bool>)
        t |> equal(v, true, "5 > 3 -> true")
    }
}

[test(description = "query_scalar_opt returns some of a string result")]
def test_query_scalar_opt_string_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 'hello'", type<string>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, "hello", "value")
    }
}

[test(description = "query_scalar_opt returns some of an integer result")]
def test_query_scalar_opt_int_some(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 42", type<int>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, 42, "value")
    }
}

[test(description = "query_scalar_opt returns none when the query yields no rows")]
def test_query_scalar_opt_none_on_empty(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE u (x INT)")
        let res = db |> query_scalar_opt("SELECT x FROM u WHERE x = 999", type<int>)
        t |> success(res |> is_none, "no rows -> none")
    }
}

[test(description = "query_scalar_opt returns some of the largest 64-bit value")]
def test_query_scalar_opt_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 9223372036854775807", type<int64>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, 9223372036854775807l, "int64 max")
    }
}

[test(description = "query_scalar_opt returns some of a double result")]
def test_query_scalar_opt_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 3.14", type<double>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, 3.14lf, "value")
    }
}

[test(description = "query_scalar_opt returns some of a bool result")]
def test_query_scalar_opt_bool(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> query_scalar_opt("SELECT 1", type<bool>)
        t |> success(res |> is_some, "some")
        t |> equal(res |> unwrap, true, "value")
    }
}

[test(description = "try_query_scalar with an int type returns Ok with the value")]
def test_try_query_scalar_int(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 7", type<int>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 7, "value")
    }
}

[test(description = "try_query_scalar with an int64 type returns Ok with the value")]
def test_try_query_scalar_int64(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 12345", type<int64>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 12345l, "value")
    }
}

[test(description = "try_query_scalar with a double type returns Ok with the value")]
def test_try_query_scalar_double(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 1.5", type<double>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, 1.5lf, "value")
    }
}

[test(description = "try_query_scalar with a bool type returns Ok with the value")]
def test_try_query_scalar_bool(t : T?) {
    with_sqlite(":memory:") $(db) {
        let res = db |> try_query_scalar("SELECT 1", type<bool>)
        t |> success(res |> is_ok, "ok")
        t |> equal(res |> unwrap, true, "value")
    }
}

[test(description = "try_query_scalar returns an error when the query yields no rows")]
def test_try_query_scalar_int_err_on_no_rows(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE u (x INT)")
        let res = db |> try_query_scalar("SELECT x FROM u WHERE x = 999", type<int>)
        t |> success(res |> is_err, "no rows -> err")
    }
}
