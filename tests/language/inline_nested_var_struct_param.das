options gen2
require dastest/testing_boost

struct Rt {
    pc : int = 0
    code : array<int>
    stack : array<int>
}

[inline]
def next_code(var rt : Rt) : int {
    let v = rt.code[rt.pc]
    rt.pc++
    return v
}

[inline]
def at(rt : Rt; i : int) : int => rt.stack[i]

[inline]
def read(var rt : Rt) : int {
    let i = next_code(rt)
    return at(rt, i)
}

[inline]
def read_one_expression(var rt : Rt) : int {
    return at(rt, next_code(rt))
}

def op(var rt : Rt) : int {
    return read(rt)
}

def op_one_expression(var rt : Rt) : int {
    return read_one_expression(rt)
}

def make_rt() : Rt => <- Rt(code <- [1], stack <- [7, 9])

[test]
def test_a_nested_inline_call_with_the_callers_var_struct_parameter(t : T?) {
    var rt <- make_rt()
    t |> equal(op(rt), 9)
    var rt2 <- make_rt()
    t |> equal(op_one_expression(rt2), 9)
}
