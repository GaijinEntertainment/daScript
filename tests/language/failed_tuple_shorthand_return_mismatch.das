//! returning a shorthand tuple whose variable names do not match the
//! declared named-tuple result does not compile
options gen2
// incompatible return type: shorthand var names don't match declared
// named-tuple result
expect 30343

require dastest/testing_boost public

def fail_return_name_mismatch() : tuple<eid : int; distSq : float> {
    let foo = 1
    let bar = 1.1
    return (foo, bar)
}
