//! passing a const pointer to a generic by-value `var` parameter
//! does not compile, whether the parameter is untyped, `auto?` or
//! `auto(TT)`
options gen2
expect 30341:3

struct Pointee {
    value : int
}

def read_untyped(var pointer) => pointer.value

def read_auto_pointer(var pointer : auto?) => pointer.value

def read_named_auto(var pointer : auto(TT)) => pointer.value

[export]
def pass_const_pointer_to_generic_var {
    var pointee = Pointee()
    let pointer : Pointee? = unsafe(addr(pointee))
    read_untyped(pointer)
    read_auto_pointer(pointer)
    read_named_auto(pointer)
}
