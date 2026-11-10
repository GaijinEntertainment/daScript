//! passing a const local or a const struct to a generic
//! `var auto(TT)&` parameter does not compile
options gen2
expect 30341:2

def write_through(var value : auto(TT)&) : bool {
    return true
}

struct Destination {
    value : int
}

[export]
def pass_const_to_generic_var_ref {
    let number = 1
    write_through(number)
    let destination = Destination()
    write_through(destination)
}
