// i(n) covers only alternative i, so the s case is missing
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
        i(n) => n
    }
}
