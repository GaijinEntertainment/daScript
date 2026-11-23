//! With promote_folded_int_constants off, an int constant folded from
//! a call must not promote in a compound assignment (30406): only a
//! literal adapts.
options gen2
options promote_folded_int_constants = false

expect 30406

[export]
def test {
    var f = 1.5
    f *= int(2)
    return true
}
