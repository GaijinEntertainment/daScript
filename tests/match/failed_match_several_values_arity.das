// a match on two values takes two-element patterns
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 1
    let y = 2
    return match (x, y) {
        (1, 2, 3) => true
        _ => false
    }
}
