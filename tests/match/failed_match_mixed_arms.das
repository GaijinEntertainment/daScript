// one match block takes either `if (pattern)` arms or `pattern => body` arms, never both
options gen2

require daslib/match

expect 50503

[export]
def test {
    match (1) {
        if (0) {
            return false
        }
        _ => $ { return true }
    }
    return false
}
