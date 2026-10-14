// a _ arm after a call per alternative is unreachable
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
        i(_) => true
        s(_) => false
        _ => false
    }
}
