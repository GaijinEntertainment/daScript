// a constant inside an earlier arm's range is unreachable
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 3
    return match (x) {
        (1..5) => 1
        3 => 2
        _ => 3
    }
}
