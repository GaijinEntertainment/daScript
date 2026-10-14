// a call naming no alternative of the variant is not a pattern
options gen2

require daslib/match

expect 50503

variant IntOrStr {
    i : int
    s : string
}

[export]
def test {
    let v = IntOrStr(i = 1)
    return match (v) {
        f(_) => true
        _ => false
    }
}
