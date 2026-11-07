expect 30151

options gen2

struct P {
    x : int
}

var g_bumps = 0

[sideeffects]
def bump : P {
    g_bumps++
    return P(x = 1)
}

[export]
def main {
    assert(bump().x == 1)
}
