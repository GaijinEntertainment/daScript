options gen2
options indenting = 4

module mc_late_mod shared private

[_macro]
def private late_macro_context {
    pass
}

[export]
def public late_answer(var out : int?) {
    unsafe {
        *out = 42
    }
}

[export]
def public late_add(a, b : int; var out : int?) {
    unsafe {
        *out = a + b
    }
}

[export]
def public late_add3(a, b, c : int; var out : int?) {
    unsafe {
        *out = a + b + c
    }
}

[export]
def public late_pick(var out : int?) {
    unsafe {
        *out = 1
    }
}

[export]
def public late_pick(a : int; var out : int?) {
    unsafe {
        *out = a
    }
}
