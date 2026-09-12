options gen2


require daslib/export_c

struct Wide {
    small : int8
    wide : int64
    fraction : float
}

[export_c]
def widen(n : int) : Wide {
    return Wide(small = int8(n), wide = int64(n), fraction = float(n))
}
