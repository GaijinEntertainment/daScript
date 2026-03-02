options gen2
require dastest/testing_boost public
require daslib/strings_boost

[test]
def test_string_operations(t : T?) {
    t |> run("string building") @(t : T?) {
        let s = "hello"
        let w = "world"
        t |> equal("{s} {w}", "hello world")
        t |> equal("{1 + 2}", "3")
    }
    t |> run("string functions") @(t : T?) {
        t |> success("hello world" |> starts_with("hello"))
        t |> success("hello world" |> ends_with("world"))
        t |> equal(length("test"), 4)
    }
    t |> run("string comparison") @(t : T?) {
        t |> success("abc" == "abc")
        t |> success("abc" != "def")
        t |> success("abc" < "abd")
    }
}

def greet(name : string) : string {
    return "Hello, {name}!"
}

[test]
def test_string_functions(t : T?) {
    t |> run("custom string function") @(t : T?) {
        t |> equal(greet("daslang"), "Hello, daslang!")
    }
    t |> run("to_lower and to_upper") @(t : T?) {
        t |> equal(to_lower("HELLO"), "hello")
        t |> equal(to_upper("hello"), "HELLO")
    }
    t |> run("join") @(t : T?) {
        var arr : array<string>
        arr |> push("a")
        arr |> push("b")
        arr |> push("c")
        t |> equal(join(arr, ","), "a,b,c")
    }
}
