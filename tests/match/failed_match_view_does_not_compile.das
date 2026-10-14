// a view whose call does not compile for the value is an error
options gen2

require daslib/match

expect 50503

[export]
def test {
    let x = 3
    return match (x) {
        (length(_) => 0) => true
        _ => false
    }
}
