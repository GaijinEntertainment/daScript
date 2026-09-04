// Shapes the fuzzer reaches that must be rejected, one diagnostic each. The expect list is
// an exact multiset: an error that stops being reported fails this file just as loudly as a
// new one, which is what keeps a crash from being traded for a silent accept.
options gen2
expect 30320, 30805, 30821, 30826, 30832

// An inherited field redeclared as sealed, in a class deriving from a template.
class template T {
}

class e : T {
    f : r<t>
    sealed f : int
}

[export]
def main {
}
