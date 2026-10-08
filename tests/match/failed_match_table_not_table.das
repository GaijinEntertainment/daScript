// a table pattern matches only a table
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 3
    return match (x) {
        ({ "a" => 1 }) => true
        _ => false
    }
}
