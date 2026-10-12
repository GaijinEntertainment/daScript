//fmt:ignore-file
options gen2
options auto_inline_functions = false

require strings

def clones(s : string; var strs : array<string>) : string {
    strs |> push(s)
    var copy = s
    copy = s
    return copy
}

def lengths(s : string; arr : array<int>) : uint64 {
    var t = long_length(s)
    t += long_length(arr)
    t += to_int64(s, false)
    t += to_int64(s, true)
    t += int64(length(s) /* chars */)
    return uint64(long_length(arr)) + uint64(t)
}

def narrowing(n : int64; u : uint64; var arr : array<int>) : int {
    arr |> resize(n)
    arr |> reserve(int64(u))
    var total = 0
    for (i in range64(n)) {
        total += arr[i]
    }
    for (i in range(0, int(n))) {
        total += i
    }
    return total
}

def products(s : string; a : int) : int64 {
    let x = long_length(s) * 4l
    let y = long_length(s) * int64(a) + 1l
    let z = 100l / (int64(a) * long_length(s))
    return x + y + z
}

def sum_body(s : string; a : int) : int64 => long_length(s) * int64(a) - 1l // the pad
