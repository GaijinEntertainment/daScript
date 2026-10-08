// a range that overlaps an earlier arm's range is partly unreachable: the overlap is reported
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 7
    return match (x) {
        (0..10) => 1
        (5..20) => 2
        _ => 3
    }
}
