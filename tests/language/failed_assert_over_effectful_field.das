//! an assert whose condition calls a function with side effects does
//! not compile, since the condition is dropped when asserts are off
expect 30151

options gen2
// coverage turns every assert into verify, which may carry side
// effects
options no_coverage

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
