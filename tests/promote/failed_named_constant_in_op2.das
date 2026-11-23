//! A module-scope named int constant beside a float in a binary
//! operator must not compile (30406): a name is not a constant the
//! operator promotes.
options gen2

expect 30406

let N = 3

[export]
def test {
    let f = 1.5
    return f * N > 0.0
}
