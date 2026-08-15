options gen2
expect 31013 : 6    // unsafe_function_call - every scratch primitive is gated; one site per def below

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
