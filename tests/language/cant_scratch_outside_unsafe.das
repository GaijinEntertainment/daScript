//! every scratch primitive - set_scratch, scratch_reserve,
//! scratch_resize, scratch_ensure_capacity - called outside unsafe
//! does not compile
options gen2
// unsafe_function_call - every scratch primitive is gated; one site
// per def below
expect 31013 : 6

var a : array<int>
var tb : table<int; int>

[export]
def cant_set_scratch_array {
    a |> set_scratch(true)
}

[export]
def cant_set_scratch_table {
    tb |> set_scratch(true)
}

[export]
def cant_scratch_reserve {
    a |> scratch_reserve(10)
}

[export]
def cant_scratch_ensure_capacity {
    a |> scratch_ensure_capacity(10)
}

[export]
def cant_scratch_resize {
    a |> scratch_resize(10)
}

[export]
def cant_raw_builtin {
    __builtin_array_scratch_reserve(a, 10, 4)
}
