options gen2

def mark(a : auto; _text : string) : int => 0

def take(a : int; b : int = 5) : int => a + b

struct Fx {
    name : string
}

variant Vi {
    i : int
    f : float
}

def members(fx : Fx; p : Fx?; v : float3; vi : Vi) : int {
    var total = 0
    total += mark(fx.name, "fx.name")
    total += mark(p?.name, "p?.name")
    total += mark(v.xy, "v.xy")
    total += mark(vi as i, "vi as i")
    total += mark(Fx(), "Fx()")
    total += mark(take(length("{total}")), "take(length(\"\{total\}\"))")
    return total
}

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
