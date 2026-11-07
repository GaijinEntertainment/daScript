options gen2
require dastest/testing_boost

def to_byte(v : bool) : uint8 => unsafe(reinterpret<uint8>(v))

def to_bool(v : uint8) : bool => unsafe(reinterpret<bool>(v))

def int_to_bool(v : int) : bool => unsafe(reinterpret<bool>(v))

var g_byte_source = 0u8
var g_int_source = 0

def store(var x : auto(TT)&; v : TT) {
    unsafe {
        memset8(addr<uint8?>(x), reinterpret<uint8>(v), 1)
    }
}

[test]
def test_a_bool_reinterpreted_to_uint8_is_its_byte(t : T?) {
    t |> equal(1, int(to_byte(true)))
    t |> equal(0, int(to_byte(false)))
    t |> equal(true, to_bool(1u8))
    t |> equal(false, to_bool(0u8))
    var b = false
    store(b, true)
    t |> equal(true, b)
    var u = 0u8
    store(u, 7u8)
    t |> equal(7, int(u))
}

//! a bool's truth by branching on it, not by comparing its byte
def truth(b : bool) : int => b ? 1 : 0

//! a bool byte other than 0 or 1 is defined only under the JIT, which
//! compares the low byte with zero; the interpreter and AOT read it
//! as a C++ bool
[test]
def test_a_runtime_integer_reinterpreted_to_bool_reads_its_low_byte(t : T?) {
    g_int_source = 256
    t |> equal(0, truth(int_to_bool(g_int_source)))
    g_int_source = 1
    t |> equal(1, truth(int_to_bool(g_int_source)))
    if (jit_enabled()) {
        g_byte_source = 2u8
        t |> equal(1, truth(to_bool(g_byte_source)))
        g_int_source = 2
        t |> equal(1, truth(int_to_bool(g_int_source)))
    }
}
