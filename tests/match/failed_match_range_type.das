// a range pattern matches only values of its element type - an int range does not match a uint
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 5u
    return match (x) {
        (0..10) => true
        _ => false
    }
}
