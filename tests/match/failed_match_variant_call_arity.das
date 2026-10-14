// a case without a value takes no argument, a case with one takes one
options gen2

require daslib/match

expect 50503:2

variant Answer {
    nothing
    amount : int
}

[export]
def test {
    let a = Answer(nothing)
    let x = match (a) {
        nothing(1) => 1
        _ => 0
    }
    let y = match (a) {
        amount() => 1
        _ => 0
    }
    return x + y
}
