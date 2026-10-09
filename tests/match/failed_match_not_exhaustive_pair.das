// a match on several values must cover every combination of finite values
options gen2

require daslib/match

expect 50503

[export]
def test {
    let a = true
    let b = false
    let s = match (a, b) {
        (true, _) => "t"
        (false, true) => "ft"
    }
    return s == "t"
}
