// a _ arm after every variant alternative is matched is unreachable
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
        IntOrStr(i = _) => true
        IntOrStr(s = _) => false
        _ => false
    }
}
