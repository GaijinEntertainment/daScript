// guards that call a function are not combined - the call may answer differently each time
options gen2

require daslib/match

expect 50503

def private half(x : int) : int {
    return x / 2
}

[export]
def test {
    let a = 4
    let s = match (a) {
        n && half(n) > 1 => "big"
        n && half(n) <= 1 => "small"
    }
    return s == "big"
}
