options gen2
options solid_context
options indenting = 4

require dastest/testing_boost public

// Under `options solid_context` a function address simulates as an index into the context's
// function table rather than a mangled-name lookup, which is a different node with its own
// typed reads. The handle has to come out the same in every store position here too.

typedef SolidFn = function<(a, b : int) : int>

def sa_add(a, b : int) : int => a + b

def sa_handle_of(f : SolidFn) : uint64 => intptr(f)

[test(description = "under solid_context a function address gives the same handle through a parameter, a declaration and int64 or void? casts, and the handle casts back to a callable")]
def test_solid_context_function_address(t : T?) {
    let handle_via_param = sa_handle_of(@@sa_add)
    t |> success(handle_via_param != 0ul, "the handle through a parameter is not null")
    unsafe {
        let declared = reinterpret<uint64>(@@sa_add)
        let as_int64 = reinterpret<int64>(@@sa_add)
        let as_ptr = reinterpret<void?>(@@sa_add)
        t |> equal(declared, handle_via_param, "declaration with initializer")
        t |> equal(as_int64, int64(handle_via_param), "cast to int64")
        t |> equal(intptr(as_ptr), handle_via_param, "cast to void?")
        t |> equal(invoke(reinterpret<SolidFn>(declared), 20, 22), 42, "a handle casts back to a callable")
    }
}

var g_solid_scale = 3
var g_solid_bias = 0.5

[test(description = "under solid_context a global combined with a local in a binary operation reads the global's value, for int and float")]
def test_solid_context_global_op_local(t : T?) {
    var sum = 0
    var total = 0.0
    for (i in range(5)) {
        sum += g_solid_scale * i
        let step = float(i)
        total += g_solid_bias + step
    }
    t |> equal(sum, 30)
    t |> equal(total, 12.5)
    g_solid_scale = 4
    let seven = 7
    t |> equal(g_solid_scale - seven, -3)
    t |> success(g_solid_scale < seven)
    g_solid_scale = 3
}

var g_solid_label = "four"

[never_inline]
def solid_scaled(scale, value : int) : int => scale * value + 1

[never_inline]
def solid_biased(bias, value : float) : float => bias + value

[never_inline]
def solid_labeled(text : string; count : int) : int => length(text) + count

[test(description = "under solid_context a call that takes a global and then a local passes the global's value, for int, float and string")]
def test_solid_context_call_global_local(t : T?) {
    var sum = 0
    var total = 0.0
    var lengths = 0
    for (i in range(4)) {
        sum += solid_scaled(g_solid_scale, i)
        let step = float(i)
        total += solid_biased(g_solid_bias, step)
        lengths += solid_labeled(g_solid_label, i)
    }
    t |> equal(sum, 22)
    t |> equal(total, 8.0)
    t |> equal(lengths, 22)
}
