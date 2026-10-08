// complementary float comparisons do not cover a value - with NaN neither holds
options gen2

require daslib/match

expect 50503

[export]
def test {
    let a = 1.0
    let b = 2.0
    let s = match (a, b) {
        (x, y) && x > y => "gt"
        (x, y) && x <= y => "le"
    }
    return s == "le"
}
