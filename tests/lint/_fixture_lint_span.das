options gen2

def mark(a : auto; _text : string) : int => 0

def take(a : int; b : int = 5) : int => a + b

def spans(s : string; var arr : array<int>; n : int64) : int {
    var total = 0
    total += mark(int64(length(s)), "int64(length(s))")
    total += mark((n + 1l) * 2l, "(n + 1l) * 2l")
    total += mark(n * (2l + n), "n * (2l + n)")
    total += mark(take(1), "take(1)")
    total += mark(take(
        1 + 2), "take(\n        1 + 2)")
    total += mark("a{n}b", "\"a\{n\}b\"")
    total += mark(arr |> length(), "arr |> length()")
    total += mark(arr[length(arr) - 1], "arr[length(arr) - 1]")
    total += mark(length(s) > 0 ? s : "x", "length(s) > 0 ? s : \"x\"")
    total += mark(int(n), "int(n)")
    return total
}
