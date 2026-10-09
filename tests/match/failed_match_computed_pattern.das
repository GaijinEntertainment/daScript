// a computed expression is not a pattern - compare against it with match_expr
options gen2

require daslib/match

expect 50503

struct Limits {
    top : int
}

[export]
def test {
    let lim = Limits(top = 3)
    let x = 3
    return match (x) {
        lim.top => true
        _ => false
    }
}
