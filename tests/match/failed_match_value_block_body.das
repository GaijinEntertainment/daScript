// a match used as a value takes expression bodies, not $-blocks
options gen2

require daslib/match

expect 50503

[export]
def test {
    let s = match (1) {
        0 => $ { return "zero" }
        _ => "other"
    }
    return s == "other"
}
