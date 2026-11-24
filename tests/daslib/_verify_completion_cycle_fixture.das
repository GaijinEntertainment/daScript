options gen2
options indenting = 4

require daslib/validate_code

//! Companion to the fan-out fixture: the cycle here closes through
//! `shared_tail`, which eight different branches reach. A memo that
//! swallowed an already-visited function on the current path would
//! let the cycle through, so this fixture must still fail to compile.
//! The branches are declared in reverse name order, and the reported
//! cycle names the first one by name.

[never_inline]
def shared_tail() : int {
    return branch_top()
}

[never_inline]
def branch_h() : int {
    return shared_tail()
}

[never_inline]
def branch_g() : int {
    return shared_tail()
}

[never_inline]
def branch_f() : int {
    return shared_tail()
}

[never_inline]
def branch_e() : int {
    return shared_tail()
}

[never_inline]
def branch_d() : int {
    return shared_tail()
}

[never_inline]
def branch_c() : int {
    return shared_tail()
}

[never_inline]
def branch_b() : int {
    return shared_tail()
}

[never_inline]
def branch_a() : int {
    return shared_tail()
}

[never_inline]
def branch_top() : int {
    return (branch_h() + branch_g() + branch_f() + branch_e()
        + branch_d() + branch_c() + branch_b() + branch_a())
}

[verify_completion]
def cycle_root() : int {
    return branch_top()
}

[export]
def main {
    feint("{cycle_root()}")
}
