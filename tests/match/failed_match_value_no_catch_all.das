// a match used as a value needs a last arm that always matches
options gen2

require daslib/match

expect 50503

[export]
def test {
    let s = match (1) {
        0 => "zero"
        1 => "one"
    }
    return s == "one"
}
