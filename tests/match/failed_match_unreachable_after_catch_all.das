// an arm after one that matches everything is unreachable, guarded or not
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 1
    return match (x) {
        n => n == 1
        m && m > 0 => false
    }
}
