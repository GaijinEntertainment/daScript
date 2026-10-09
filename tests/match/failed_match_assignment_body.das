// an assignment body binds looser than `=>` and needs a $-block
options gen2

require daslib/match

expect 50503

[export]
def test {
    var total = 0
    match (1) {
        n => total += n
    }
    return total == 1
}
