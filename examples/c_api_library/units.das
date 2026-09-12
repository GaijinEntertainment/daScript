options gen2
options indenting = 4

require math


def celsius_to_fahrenheit(c : float) : float {
    return c * 1.8 + 32.0
}

def clamp_int(v, lo, hi : int) : int {
    return clamp(v, lo, hi)
}

def private rounding_bias() : float {
    return 0.5
}
