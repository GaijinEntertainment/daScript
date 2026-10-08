// an arm after guards that together cover their pattern is unreachable
options gen2

require daslib/match

expect 50503

[export]
def test {
    let a = 4
    let b = 2
    return match (a, b) {
        (x, y) && x > y => true
        (x, y) && x <= y => false
        _ => false
    }
}
