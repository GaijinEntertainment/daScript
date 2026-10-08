// a set has no values - its table pattern lists keys only
options gen2

require daslib/match

expect 50503

[export]
def test {
    var s <- { "a", "b" }
    return match (s) {
        ({ "a" => 1 }) => true
        _ => false
    }
}
