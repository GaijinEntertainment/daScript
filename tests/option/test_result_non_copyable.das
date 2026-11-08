options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/result

// Result<T, E> with a non-copyable T (array<int>).
//
// These tests exercise every public function in result.das with a
// non-copyable value-side payload. The error side (E = string) stays
// copyable, so the failures pinpoint the value-side copy/return paths
// that still need migration to the static_if (typeinfo can_copy(...)) +
// clone_to_move(...) pattern already applied in option.das.

def operator ==(a : array<int>; b : array<int>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in 0 .. length(a)) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

def make_arr(a : int; b : int) : array<int> => <- [a, b]

[test(description = "a Result can carry a non-copyable array, err works with a non-copyable witness and is_ok and is_err agree")]
def test_constructors(t : T?) {
    t |> run("ok carries a non-copyable payload") @(t : T?) {
        let r = ok(make_arr(1, 2), type<string>)
        t |> success(is_ok(r))
        t |> equal(length(unwrap(r)), 2)
        t |> equal(unwrap(r)[0], 1)
        t |> equal(unwrap(r)[1], 2)
    }

    t |> run("err with a non-copyable T witness") @(t : T?) {
        let r = err("boom", type<array<int>>)
        t |> success(is_err(r))
        t |> equal(unwrap_err(r), "boom")
    }

    t |> run("is_ok / is_err on Result<array<int>, string>") @(t : T?) {
        t |> success(ok(make_arr(1, 2), type<string>) |> is_ok)
        t |> success(err("e", type<array<int>>) |> is_err)
    }
}

[test(description = "move_ok moves a non-copyable payload out of its source and also works on a copyable one")]
def test_move_ok(t : T?) {
    t |> run("move_ok moves a non-copyable value payload out of the source") @(t : T?) {
        var src = make_arr(1, 2)
        let r = move_ok(src, type<string>)
        t |> success(is_ok(r))
        t |> equal(length(unwrap(r)), 2)
        t |> equal(unwrap(r)[0], 1)
        t |> equal(unwrap(r)[1], 2)
        t |> equal(length(src), 0)
    }

    t |> run("move_ok works on a copyable payload too") @(t : T?) {
        let n = 42
        let r = move_ok(n, type<string>)
        t |> success(is_ok(r))
        t |> equal(unwrap(r), 42)
    }
}

[test(description = "move_err moves a non-copyable error out of its source and also works on a copyable one")]
def test_move_err(t : T?) {
    t |> run("move_err moves a non-copyable error payload out of the source") @(t : T?) {
        var src = make_arr(7, 8)
        let r = move_err(src, type<int>)
        t |> success(is_err(r))
        t |> equal(length(unwrap_err(r)), 2)
        t |> equal(unwrap_err(r)[0], 7)
        t |> equal(unwrap_err(r)[1], 8)
        t |> equal(length(src), 0)
    }

    t |> run("move_err works on a copyable error too") @(t : T?) {
        let s = "boom"
        let r = move_err(s, type<int>)
        t |> success(is_err(r))
        t |> equal(unwrap_err(r), "boom")
    }
}

[test(description = "map on a Result of an array changes the ok type, keeps a non-copyable output and leaves err as err")]
def test_map(t : T?) {
    t |> run("map Result<array<int>, string> -> Result<int, string>") @(t : T?) {
        let m = ok(make_arr(1, 2), type<string>) |> map($(v : array<int>) => length(v))
        t |> success(is_ok(m))
        t |> equal(unwrap(m), 2)
    }

    t |> run("map keeps a non-copyable output type") @(t : T?) {
        let m = ok(make_arr(1, 2), type<string>) |> map($(v : array<int>) => make_arr(length(v), 0))
        t |> success(is_ok(m))
        t |> equal(unwrap(m)[0], 2)
    }

    t |> run("map leaves err as err") @(t : T?) {
        let m = err("boom", type<array<int>>) |> map($(v : array<int>) => length(v))
        t |> success(is_err(m))
        t |> equal(unwrap_err(m), "boom")
    }
}

[test(description = "map_err on a Result of an array leaves ok unchanged and transforms err")]
def test_map_err(t : T?) {
    t |> run("map_err leaves ok unchanged") @(t : T?) {
        let m = ok(make_arr(1, 2), type<string>) |> map_err($(e : string) => "<{e}>")
        t |> success(is_ok(m))
        t |> equal(length(unwrap(m)), 2)
        t |> equal(unwrap(m)[0], 1)
    }

    t |> run("map_err transforms err side") @(t : T?) {
        let m = err("boom", type<array<int>>) |> map_err($(e : string) => "<{e}>")
        t |> success(is_err(m))
        t |> equal(unwrap_err(m), "<boom>")
    }
}

[test(description = "and_then on a Result of an array chains ok, allows a non-copyable output and short-circuits on err")]
def test_and_then(t : T?) {
    t |> run("and_then chains ok -> ok") @(t : T?) {
        let r = ok(make_arr(1, 2), type<string>) |> and_then($(v : array<int>) => ok(length(v), type<string>))
        t |> equal(unwrap(r), 2)
    }

    t |> run("and_then with non-copyable output") @(t : T?) {
        let r = ok(make_arr(1, 2), type<string>) |> and_then($(v : array<int>) => ok(make_arr(length(v), 0), type<string>))
        t |> success(is_ok(r))
        t |> equal(unwrap(r)[0], 2)
    }

    t |> run("and_then on err short-circuits") @(t : T?) {
        let r = err("boom", type<array<int>>) |> and_then($(v : array<int>) => ok(length(v), type<string>))
        t |> success(is_err(r))
        t |> equal(unwrap_err(r), "boom")
    }
}

[test(description = "or_else on a Result of an array passes ok through and recovers from err")]
def test_or_else(t : T?) {
    t |> run("or_else passes ok through") @(t : T?) {
        let r = ok(make_arr(1, 2), type<string>) |> or_else($(e : string) => ok(make_arr(9, 9), type<string>))
        t |> success(is_ok(r))
        t |> equal(unwrap(r)[0], 1)
    }

    t |> run("or_else recovers from err") @(t : T?) {
        let r = err("boom", type<array<int>>) |> or_else($(e : string) => ok(make_arr(9, 10), type<string>))
        t |> success(is_ok(r))
        t |> equal(unwrap(r)[0], 9)
    }
}

[test(description = "unwrap and its fallback forms return the array, the fallback or the error as specified")]
def test_unwrap(t : T?) {
    t |> run("unwrap on ok returns the array") @(t : T?) {
        let arr = ok(make_arr(5, 6), type<string>) |> unwrap
        t |> equal(length(arr), 2)
        t |> equal(arr[0], 5)
    }

    t |> run("unwrap_or returns fallback on err") @(t : T?) {
        let arr = err("e", type<array<int>>) |> unwrap_or(make_arr(9, 10))
        t |> equal(length(arr), 2)
        t |> equal(arr[0], 9)
    }

    t |> run("unwrap_or_default returns empty on err") @(t : T?) {
        let arr = err("e", type<array<int>>) |> unwrap_or_default
        t |> equal(length(arr), 0)
    }

    t |> run("unwrap_or_else computes fallback from error") @(t : T?) {
        let arr = err("xx", type<array<int>>) |> unwrap_or_else($(e : string) => make_arr(length(e), 0))
        t |> equal(arr[0], 2)
    }

    t |> run("unwrap_err returns the error") @(t : T?) {
        let e = err("boom", type<array<int>>) |> unwrap_err
        t |> equal(e, "boom")
    }
}

[test(description = "move_unwrap moves the array out of an ok and flips the result to its err state")]
def test_move_unwrap(t : T?) {
    t |> run("move_unwrap moves the array out of ok, flips r to err state") @(t : T?) {
        var r = ok(make_arr(5, 6), type<string>)
        let arr <- r |> move_unwrap
        t |> equal(length(arr), 2)
        t |> equal(arr[0], 5)
        t |> equal(arr[1], 6)
        t |> success(is_err(r))
    }
}

[test(description = "expect_value returns the array of an ok and expect_err returns the error of an err")]
def test_expect(t : T?) {
    t |> run("expect_value on ok returns the array") @(t : T?) {
        let arr = ok(make_arr(7, 8), type<string>) |> expect_value("never fires")
        t |> equal(length(arr), 2)
        t |> equal(arr[0], 7)
    }

    t |> run("expect_err on err returns the error") @(t : T?) {
        let e = err("boom", type<array<int>>) |> expect_err("never fires")
        t |> equal(e, "boom")
    }
}

[test(description = "if_ok and if_err run their block with the array or the error only on the matching state")]
def test_if_ok_if_err(t : T?) {
    t |> run("if_ok fires with the array") @(t : T?) {
        var seen = 0
        ok(make_arr(3, 4), type<string>) |> if_ok() $(v : array<int>) { seen = length(v); }
        t |> equal(seen, 2)
    }

    t |> run("if_ok is a no-op on err") @(t : T?) {
        var seen = 0
        err("boom", type<array<int>>) |> if_ok() $(v : array<int>) { seen = length(v); }
        t |> equal(seen, 0)
    }

    t |> run("if_err fires with the error") @(t : T?) {
        var seen = ""
        err("boom", type<array<int>>) |> if_err() $(e : string) { seen = e; }
        t |> equal(seen, "boom")
    }

    t |> run("if_err is a no-op on ok") @(t : T?) {
        var seen = ""
        ok(make_arr(1, 2), type<string>) |> if_err() $(e : string) { seen = e; }
        t |> equal(seen, "")
    }
}

[test(description = "to_option and err_to_option carry the array or the error and give none on the other state")]
def test_bridges(t : T?) {
    t |> run("to_option of ok carries the array") @(t : T?) {
        let o = ok(make_arr(7, 8), type<string>) |> to_option
        t |> success(is_some(o))
        t |> equal(length(unwrap(o)), 2)
    }

    t |> run("to_option of err is none") @(t : T?) {
        let o = err("e", type<array<int>>) |> to_option
        t |> success(is_none(o))
    }

    t |> run("err_to_option of err carries the error") @(t : T?) {
        let o = err("boom", type<array<int>>) |> err_to_option
        t |> success(is_some(o))
        t |> equal(unwrap(o), "boom")
    }

    t |> run("err_to_option of ok is none") @(t : T?) {
        let o = ok(make_arr(1, 2), type<string>) |> err_to_option
        t |> success(is_none(o))
    }
}

[test(description = "the ?? operator unwraps an ok array and returns the fallback on err")]
def test_op_qq(t : T?) {
    t |> run("?? unwraps ok array") @(t : T?) {
        let arr = ok(make_arr(7, 8), type<string>) ?? make_arr(0, 0)
        t |> equal(length(arr), 2)
        t |> equal(arr[0], 7)
    }

    t |> run("?? returns fallback on err") @(t : T?) {
        let arr = err("e", type<array<int>>) ?? make_arr(9, 10)
        t |> equal(length(arr), 2)
        t |> equal(arr[0], 9)
    }
}

[test(description = "== on a Result of an array compares tag and payload")]
def test_equality(t : T?) {
    t |> run("== on Result<array<int>, string>") @(t : T?) {
        t |> success(ok(make_arr(1, 2), type<string>) == ok(make_arr(1, 2), type<string>))
        t |> success(!(ok(make_arr(1, 2), type<string>) == ok(make_arr(1, 3), type<string>)))
        t |> success(err("e", type<array<int>>) == err("e", type<array<int>>))
        t |> success(!(ok(make_arr(1, 2), type<string>) == err("e", type<array<int>>)))
    }
}

[test(description = "a Result of an array compares with == and != against a plain array")]
def test_op_eq_value(t : T?) {
    t |> run("Result<array<int>, string> == array<int>") @(t : T?) {
        t |> success(ok(make_arr(1, 2), type<string>) == make_arr(1, 2))
        t |> success(make_arr(1, 2) == ok(make_arr(1, 2), type<string>))
        t |> success(!(ok(make_arr(1, 2), type<string>) == make_arr(1, 3)))
        t |> success(!(err("e", type<array<int>>) == make_arr(1, 2)))
    }

    t |> run("Result<array<int>, string> != array<int>") @(t : T?) {
        t |> success(ok(make_arr(1, 2), type<string>) != make_arr(1, 3))
        t |> success(make_arr(1, 3) != ok(make_arr(1, 2), type<string>))
        t |> success(err("e", type<array<int>>) != make_arr(1, 2))
    }
}

[test(description = "an ok array flows through map, and_then and unwrap_or while an err short-circuits the whole chain")]
def test_chaining(t : T?) {
    t |> run("ok -> map -> and_then -> unwrap_or") @(t : T?) {
        let n = (ok(make_arr(1, 2), type<string>)
            |> map($(v : array<int>) => length(v))
            |> and_then($(x : int) => ok(x + 1, type<string>))
            |> unwrap_or(-1))
        t |> equal(n, 3)
    }

    t |> run("err short-circuits the whole chain") @(t : T?) {
        let n = (err("boom", type<array<int>>)
            |> map($(v : array<int>) => length(v))
            |> and_then($(x : int) => ok(x + 1, type<string>))
            |> unwrap_or(-1))
        t |> equal(n, -1)
    }
}

[test(description = "move_ok and move_err work when the other side of the Result is a non-copyable array")]
def test_move_with_non_copyable_other_side(t : T?) {
    var value <- make_arr(1, 2)
    var r <- move_ok(value, type<array<int>>)
    t |> success(is_ok(r))
    t |> equal(length(move_unwrap(r)), 2)
    var error <- make_arr(3, 4)
    var e <- move_err(error, type<array<int>>)
    t |> success(is_err(e))
    t |> equal(length(move_unwrap_err(e)), 2)
}
