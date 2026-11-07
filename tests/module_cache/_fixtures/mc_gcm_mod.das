options gen2
options indenting = 4

module mc_gcm_mod shared public

require mc_apply_macro

[mc_kind]
def twice(x : int) : int => x * 2

//! a generic whose body still holds the call macro `twice`
//! registered into this module
def outer(a : auto) : int => twice(a)
