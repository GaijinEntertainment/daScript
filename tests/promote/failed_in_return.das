//! An out-of-range literal in a return statement must report the
//! range error (30515) and not an invalid return type.
options gen2

expect 30515

def f() : uint8 {
    return 256
}

[export]
def test {
    return true
}
