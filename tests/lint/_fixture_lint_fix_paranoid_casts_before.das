//fmt:ignore-file
options gen2
options auto_inline_functions = false

require strings

def clones(s : string; var strs : array<string>) : string {
    strs |> push_clone(s)
    var copy := s
    copy := s
    return copy
}

def lengths(s : string; arr : array<int>) : uint64 {
    var t = int64(length(s))
    t += int64(arr |> length())
    t += int64(to_int(s))
    t += int64(to_int(s, true))
    t += int64(length(s) /* chars */)
    return uint64(length(arr)) + uint64(t)
}

def narrowing(n : int64; u : uint64; var arr : array<int>) : int {
    arr |> resize(int(n))
    arr |> reserve(int(u))
    var total = 0
    for (i in range(int(n))) {
        total += arr[i]
    }
    for (i in range(0, int(n))) {
        total += i
    }
    return total
}

def products(s : string; a : int) : int64 {
    let x = int64(length(s) * 4)
    let y = int64(length(s) * a + 1)
    let z = 100l / int64(a * length(s))
    return x + y + z
}

def sum_body(s : string; a : int) : int64 => int64(length(s) * a - 1) // the pad
