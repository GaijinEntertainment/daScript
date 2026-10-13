// a view inside an array pattern has no value to read yet
options gen2

require daslib/match

expect 50503

[export]
def test {
    var rows : array<array<int>>
    return match (rows) {
        [(length(_) => 0)] => true
        _ => false
    }
}
