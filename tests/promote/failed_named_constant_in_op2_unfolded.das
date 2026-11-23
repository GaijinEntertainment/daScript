//! With infer-time folding off, a module-scope named int constant
//! beside a float in a binary operator must not compile (30406), the
//! same as with folding on.
options gen2
options infer_time_folding = false

expect 30406

let N = 3

[export]
def test {
    let f = 1.5
    return f * N > 0.0
}
