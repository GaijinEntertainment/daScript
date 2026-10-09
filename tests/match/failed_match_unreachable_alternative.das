// an alternative an earlier arm already matches is unreachable, though the rest of its arm is not
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 1
    return match (x) {
        1 => true
        2 | 1 => false
        _ => false
    }
}
