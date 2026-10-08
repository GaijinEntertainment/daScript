// the sides of an alternative must bind the same names
options gen2

require daslib/match

struct Point {
    x : int
    y : int
}

expect 50503

[export]
def test {
    let p = Point(x = 0, y = 5)
    return match (p) {
        Point(x = 0, y = d) | Point(y = 0) => d > 0
        _ => false
    }
}
