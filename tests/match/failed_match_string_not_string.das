// a string pattern matches only a string
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 3
    return match (x) {
        "a" + _ => true
        _ => false
    }
}
