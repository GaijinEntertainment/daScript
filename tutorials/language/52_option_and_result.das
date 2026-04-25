// Tutorial 52: Option<T> and Result<T, E>
//
// This tutorial covers:
//   - Option<T> — "value or nothing"
//   - Result<T, E> — "value or error" for fallible computations
//   - Functional composition: map, and_then, filter, or_else
//   - Extraction: unwrap, unwrap_or, unwrap_or_else, unwrap_or_default
//   - Operators: `??` (unwrap-or-default) and `==`
//   - Chaining through `|>`
//   - Bridging Result → Option via to_option / err_to_option
//   - Non-copyable payloads (array<T>, table<K;V>, lambdas) +
//     move_some / move_ok / move_err
//
// Run: daslang.exe tutorials/language/52_option_and_result.das

options gen2

require daslib/option
require daslib/result

// === Why Option<T> and Result<T, E>? ===
//
// Nullable pointers (T?) only work for pointer types, and they bleed into AST
// lifetime concerns. Sentinel return values (-1, "") are brittle and type-specific.
// Panics / asserts are coarse and not composable.
//
// Option<T> models "a value may be absent". Result<T, E> adds a meaningful error
// to that absence. Both are plain template_structure tagged unions that you can
// pass by value, compare with ==, and compose via `|>`.

// === Option<T> — construction ===
//
// `some(v)` wraps a value. `none(type<T>)` declares an empty option of a given
// payload type (the type witness is required since there is no value to infer
// from).

def test_option_constructors {
    let a = some(42)                   // Option<int>
    let b = none(type<int>)            // Option<int>, empty
    let c = some("hello")              // Option<string>

    print("is_some(a) = {a |> is_some}\n")     // true
    print("is_none(b) = {b |> is_none}\n")     // true
    print("value in a = {a |> unwrap}\n")      // 42
    print("payload of c = {c |> unwrap}\n")    // hello
}

// === Option<T> — transforming values ===
//
// `map` applies a function to the payload (if any), producing an option of the
// new type. None propagates through unchanged.

def test_option_map {
    let doubled = some(21) |> map($(x : int) => x * 2)
    print("doubled = {doubled |> unwrap}\n")   // 42

    let stringified = some(7) |> map($(x : int) => "x={x}")
    print("stringified = {stringified |> unwrap}\n")  // x=7

    let empty = none(type<int>) |> map($(x : int) => x + 100)
    print("empty stayed empty = {empty |> is_none}\n")  // true
}

// === Option<T> — monadic bind via and_then ===
//
// `and_then` takes a function that itself returns an Option. Use it to chain
// fallible steps where each step may refuse to continue.

def safe_parse_positive(s : string) : Option<int> {
    // pretend-parse: "1".."9" accepted, everything else is none
    if (s == "1") { return some(1); }
    if (s == "2") { return some(2); }
    if (s == "3") { return some(3); }
    return none(type<int>)
}

def test_option_and_then {
    let r = some("2") |> and_then($(s : string) => safe_parse_positive(s))
    print("parsed '2' = {r |> unwrap_or(-1)}\n")      // 2

    let r2 = some("x") |> and_then($(s : string) => safe_parse_positive(s))
    print("parsed 'x' is_none = {r2 |> is_none}\n")   // true
}

// === Option<T> — filtering + fallbacks ===

def test_option_filter_fallback {
    let kept = some(10) |> filter($(x : int) => x > 5)
    print("kept is_some = {kept |> is_some}\n")       // true

    let dropped = some(3) |> filter($(x : int) => x > 5)
    print("dropped is_none = {dropped |> is_none}\n") // true

    // or_value supplies an eager fallback, or_else a lazy one
    let eager = none(type<int>) |> or_value(99)
    print("eager fallback = {eager |> unwrap}\n")     // 99

    let lazy = none(type<int>) |> or_else() { return some(expensive_fallback()); }
    print("lazy fallback = {lazy |> unwrap}\n")       // 123
}

def expensive_fallback : int {
    // imagine a costly computation here
    return 123
}

// === Option<T> — extraction ===

def test_option_unwrap {
    // unwrap panics on none — use only when none is unreachable
    print("unwrap = {some(5) |> unwrap}\n")

    // unwrap_or returns a default
    print("unwrap_or = {none(type<int>) |> unwrap_or(7)}\n")   // 7

    // unwrap_or_else computes the default (useful if it is expensive)
    let computed = none(type<int>) |> unwrap_or_else() { return expensive_fallback(); }
    print("unwrap_or_else = {computed}\n")                     // 123

    // unwrap_or_default returns default<T> (the zero value of T)
    print("unwrap_or_default = {none(type<string>) |> unwrap_or_default}\n")  // (empty)
}

// === Option<T> — operators ===

def test_option_operators {
    // `??` reads like "unwrap or this default"
    print("??  = {some(5) ?? 0}\n")                 // 5
    print("??  = {none(type<int>) ?? 42}\n")        // 42

    // `==` is structural: same tag AND equal payloads
    print("some==some equal   = {some(5) == some(5)}\n")      // true
    print("some==some diff    = {some(5) == some(6)}\n")      // false
    print("none==none         = {none(type<int>) == none(type<int>)}\n")  // true
    print("some==none         = {some(5) == none(type<int>)}\n")          // false
}

// === Option<T> — side-effect combinators ===

def test_option_side_effects {
    var counter = 0
    some(3) |> if_some() $(x : int) { counter = x; }
    none(type<int>) |> if_some() $(x : int) { counter = 99; }
    print("if_some counter = {counter}\n")                    // 3

    var fired = false
    none(type<int>) |> if_none() { fired = true; }
    print("if_none fired  = {fired}\n")                       // true
}

// === Option<T> — zip ===

def test_option_zip {
    // zip combines two options into one option of a tuple.
    // Only `some` when BOTH are `some`.
    let paired = zip(some(1), some(2.5f))
    print("paired = ({paired |> unwrap._0}, {paired |> unwrap._1})\n")  // (1, 2.5)

    let mismatched = zip(some(1), none(type<float>))
    print("mismatched is_none = {mismatched |> is_none}\n")   // true
}

// === Option<T> — chaining through the pipe ===
//
// The free-function API means you can pipe the option through successive steps.

def test_option_chaining {
    let on_some = (some(3)
        |> map($(x : int) => x * 2)
        |> filter($(x : int) => x > 0)
        |> unwrap_or(-1))
    print("chain on some  = {on_some}\n")                     // 6

    let on_none = (none(type<int>)
        |> map($(x : int) => x * 2)
        |> filter($(x : int) => x > 0)
        |> unwrap_or(-1))
    print("chain on none  = {on_none}\n")                     // -1
}

// === Result<T, E> — construction ===
//
// `ok(v, type<E>)` and `err(e, type<T>)` build the two sides. The second
// argument is a type witness for the "other" side.

def test_result_constructors {
    let r = ok(42, type<string>)
    let e = err("boom", type<int>)
    print("is_ok(r) = {r |> is_ok}\n")                        // true
    print("is_err(e) = {e |> is_err}\n")                      // true
    print("ok value = {r |> unwrap}\n")                       // 42
    print("err value = {e |> unwrap_err}\n")                  // boom
}

// === Result<T, E> — map / map_err ===
//
// `map` transforms the ok payload (err passes through); `map_err` transforms
// the err payload (ok passes through).

def test_result_map {
    let doubled = ok(3, type<string>) |> map($(x : int) => x * 2)
    print("ok doubled = {doubled |> unwrap}\n")               // 6

    let wrapped = err("boom", type<int>) |> map_err($(e : string) => "wrapped: {e}")
    print("err wrapped = {wrapped |> unwrap_err}\n")          // wrapped: boom
}

// === Result<T, E> — monadic chaining ===

def parse_digit(s : string) : Result<int; string> {
    if (s == "1") { return ok(1, type<string>); }
    if (s == "2") { return ok(2, type<string>); }
    return err("not a digit: {s}", type<int>)
}

def test_result_and_then {
    let success = ok("2", type<string>) |> and_then($(s : string) => parse_digit(s))
    print("success = {success |> unwrap_or(-1)}\n")           // 2

    let failure = ok("z", type<string>) |> and_then($(s : string) => parse_digit(s))
    print("failure err = {failure |> unwrap_err}\n")          // not a digit: z
}

// === Result<T, E> — or_else (recovery) ===

def test_result_or_else {
    let recovered = err("boom", type<int>) |> or_else($(e : string) => ok(99, type<string>))
    print("recovered = {recovered |> unwrap}\n")              // 99
}

// === Result<T, E> — bridges to Option<T> ===

def test_result_to_option {
    let some_v = ok(42, type<string>) |> to_option
    print("ok -> some = {some_v |> unwrap_or(-1)}\n")         // 42

    let none_v = err("boom", type<int>) |> to_option
    print("err -> none = {none_v |> is_none}\n")              // true

    let err_v = err("boom", type<int>) |> err_to_option
    print("err -> some(err) = {err_v |> unwrap_or("")}\n")    // boom
}

// === Result<T, E> — chaining ===

def test_result_chaining {
    let on_ok = (ok(3, type<string>)
        |> map($(x : int) => x * 2)
        |> and_then($(x : int) => ok(x + 1, type<string>))
        |> unwrap_or(-1))
    print("ok chain = {on_ok}\n")                             // 7

    let on_err = (err("boom", type<int>)
        |> map($(x : int) => x * 2)
        |> map_err($(s : string) => "<{s}>"))
    print("err chain err = {on_err |> unwrap_err}\n")         // <boom>
}

// === Non-copyable payloads ===
//
// Option<T> and Result<T, E> work for any payload type. For non-copyable
// types like array<T>, the constructors clone the payload by default. When
// you actually want to drain a mutable source, use the move_* variants:
// move_some / move_ok / move_err leave the source empty after the call.

def test_non_copyable {
    // some clones — the source is still intact afterwards. Because nothing
    // mutates `src1`, it can be `let`.
    let src1 <- [1, 2, 3]
    let cloned = some(src1)
    print("cloned len = {length(cloned |> unwrap)}, src1 len = {length(src1)}\n")
    // cloned len = 3, src1 len = 3

    // move_some moves — the source is empty afterwards. The source must be
    // `var` because move_some takes `var v` and drains it.
    var src2 <- [4, 5, 6]
    let moved = move_some(src2)
    print("moved len = {length(moved |> unwrap)}, src2 len = {length(src2)}\n")
    // moved len = 3, src2 len = 0

    // The same pair exists for Result on each side: ok/move_ok and err/move_err.
    var payload <- [7, 8, 9]
    let r = move_ok(payload, type<string>)
    print("moved into ok, payload len = {length(payload)}\n")
    // moved into ok, payload len = 0

    // Combinators work transparently — map runs the block and clones
    // (or moves) on the non-copyable branch as needed.
    let lengths = some([10, 20, 30]) |> map() $(v : array<int>) { return length(v); }
    print("mapped non-copyable -> {lengths |> unwrap}\n")
    // mapped non-copyable -> 3
}

[export]
def main {
    print("=== Option ===\n")
    test_option_constructors()
    test_option_map()
    test_option_and_then()
    test_option_filter_fallback()
    test_option_unwrap()
    test_option_operators()
    test_option_side_effects()
    test_option_zip()
    test_option_chaining()

    print("\n=== Result ===\n")
    test_result_constructors()
    test_result_map()
    test_result_and_then()
    test_result_or_else()
    test_result_to_option()
    test_result_chaining()

    print("\n=== Non-copyable payloads ===\n")
    test_non_copyable()
}
