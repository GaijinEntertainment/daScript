// Shapes the fuzzer reaches that must be rejected, one diagnostic each. The expect list is
// an exact multiset: an error that stops being reported fails this file just as loudly as a
// new one, which is what keeps a crash from being traded for a silent accept.
options gen2
expect 30107, 30110, 30320, 30805, 30821, 30826, 30832

// A void expression handed to debug().
def v {
}

// An inherited field redeclared as sealed, in a class deriving from a template.
class template T {
}

class e : T {
    f : r<t>
    sealed f : int
}

// typeinfo is_argument outside of a function, in the dim expression of a field.
var g_probe = 1

struct l {
    f : int[typeinfo is_argument(g_probe)]
}

[export]
def main {
    debug(v())
}
