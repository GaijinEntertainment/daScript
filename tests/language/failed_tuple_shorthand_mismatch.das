//! a shorthand tuple whose variable names do not match the target
//! tuple's field names does not convert to it
options gen2
// function_not_found: shorthand var names don't match target tuple
// field names
expect 30341

require dastest/testing_boost public

def fail_name_mismatch() {
    var arr : array<tuple<eid : int; distSq : float>>
    let foo = 1
    let bar = 1.1
    arr |> push((foo, bar))
}
