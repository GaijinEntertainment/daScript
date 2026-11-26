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

var g_solid_table : int[8]
var g_solid_bytes = fixed_array(3u8, 2u8, 1u8, 0u8)
var g_solid_cursor : int?
var g_solid_colormap : uint8?

[test(description = "under solid_context a global fixed array and a global pointer read and written by index give the element, and small int elements widen")]
def test_solid_context_global_indexing(t : T?) {
    for (i in range(8)) {
        g_solid_table[i] = i * 7
    }
    var through_bytes = 0
    for (i in range(4)) {
        through_bytes += g_solid_table[int(g_solid_bytes[i])] * (i + 1)
    }
    t |> equal(through_bytes, 70)
    g_solid_cursor = unsafe(addr(g_solid_table[0]))
    var through_pointer = 0
    for (i in range(8)) {
        through_pointer += unsafe(g_solid_cursor[i]) + unsafe(g_solid_cursor[7 - i])
    }
    t |> equal(through_pointer, 392)
    unsafe(g_solid_cursor[2]) = through_pointer + 1
    t |> equal(g_solid_table[2], 393)
    g_solid_colormap = unsafe(addr(g_solid_bytes[0]))
    var mapped = 0
    for (i in range(4)) {
        mapped += int(unsafe(g_solid_colormap[int(unsafe(g_solid_colormap[i]))])) * (i + 1)
    }
    t |> equal(mapped, 20)
    g_solid_cursor = null
    g_solid_colormap = null
}
