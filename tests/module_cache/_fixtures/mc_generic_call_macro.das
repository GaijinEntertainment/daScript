options gen2
options indenting = 4

require mc_apply_macro

[mc_kind]
def twice(x : int) : int => x * 2

//! a generic: its body stays uninferred, the call macro `twice`
//! registered into this module still in it
def outer(a : auto) : int => twice(a)

[export]
def main {
    print("outer={outer(5)}\n")
}
