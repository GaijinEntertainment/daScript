// a reification tag other than $v is not a pattern
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 3
    return match (x) {
        $e(x) => true
        _ => false
    }
}
