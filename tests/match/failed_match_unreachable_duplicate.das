// an arm whose values an earlier arm already matches is unreachable
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 1
    return match (x) {
        1 => true
        1 => false
        _ => false
    }
}
