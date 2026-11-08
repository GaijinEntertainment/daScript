options gen2
require _strict_aliasing_views

//! lookibed's loop under the typed model: a float store cannot change
//! what an int? points at, so the bound and the addend leave the loop
[never_inline, hint(noinline)]
def scale_typed(var fa : float?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
}

//! the same loop opted out: the reads stay in the loop
[never_inline, hint(noinline, no_strict_aliasing)]
def scale_untyped(var fa : float?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
}

//! a reinterpret that views the floats as ints excludes the function
//! on its own: the int read of the first float sees the float store
[never_inline, hint(noinline)]
def scale_punning(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *reinterpret<int?>(fa)
    }
}

//! hands its caller a punned view
[never_inline, hint(noinline)]
def as_ints(var fa : float?) : int? => unsafe(reinterpret<int?>(fa))

//! excluded because it calls a punned-pointer source
[never_inline, hint(noinline)]
def scale_through_view(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits = as_ints(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

//! the punned view arrives through an indirect call: the callee is
//! unknown, the pointer result is enough to exclude the caller
[never_inline, hint(noinline)]
def scale_through_lambda(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let view <- @(var p : float?) : int? {
        return unsafe(reinterpret<int?>(p))
    }
    let bits = invoke(view, fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

//! the punned view arrives from another module, another partition
//! under --jit-split-modules
[never_inline, hint(noinline)]
def scale_through_far_view(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits = as_ints_far(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

//! the punned view arrives as a reference
[never_inline, hint(noinline)]
def scale_through_ref(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits & = as_int_ref(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
    return bits
}

//! the punned view arrives from a function the JIT never compiled
[never_inline, hint(noinline)]
def scale_through_interp(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits = as_ints_interp(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

//! the punned view arrives inside a struct
[never_inline, hint(noinline)]
def scale_through_struct(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let view = as_view_struct(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *view.bits
    }
}

//! no reinterpret of its own; a punning caller hands it both views
[never_inline, hint(noinline)]
def scale_two_views(var fa : float?; bits : int?; n_ptr : int?; k_ptr : int?) : int {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

[never_inline, hint(noinline)]
def scale_pass_view(var fa : float?; n_ptr : int?; k_ptr : int?) : int =>
    scale_two_views(fa, unsafe(reinterpret<int?>(fa)), n_ptr, k_ptr)

//! the loop lives in a lambda the punning function hands both views
[never_inline, hint(noinline)]
def scale_lambda_sink(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let body <- @(var p : float?; bits : int?) : int {
        unsafe {
            var i = 0
            while (i < *n_ptr) {
                p[i] = p[i] * 0.5f + float(*k_ptr)
                i++
            }
            return *bits
        }
    }
    return invoke(body, fa, unsafe(reinterpret<int?>(fa)))
}

//! a reinterpret to the type the pointer already has is no view
[never_inline, hint(noinline)]
def scale_self_view(var fa : float?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var same = reinterpret<float?>(fa)   // nolint:LINT005 - the self-cast IS the fixture
        var i = 0
        while (i < *n_ptr) {
            same[i] = same[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
}

//! the punned view reaches a lambda made elsewhere through an invoke
[never_inline, hint(noinline)]
def make_scaler() : lambda<(var p : float?; bits : int?; n_ptr : int?; k_ptr : int?) : int> {
    return <- @(var p : float?; bits : int?; n_ptr : int?; k_ptr : int?) : int {
        unsafe {
            var i = 0
            while (i < *n_ptr) {
                p[i] = p[i] * 0.5f + float(*k_ptr)
                i++
            }
            return *bits
        }
    }
}

[never_inline, hint(noinline)]
def scale_through_callback(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    var scaler <- make_scaler()
    let r = invoke(scaler, fa, unsafe(reinterpret<int?>(fa)), n_ptr, k_ptr)
    unsafe {
        delete scaler
    }
    return r
}

//! the punned view comes back through an out argument
[never_inline, hint(noinline)]
def view_out(var fa : float?; var bits : int?&) {
    bits = unsafe(reinterpret<int?>(fa))
}

[never_inline, hint(noinline)]
def scale_through_out(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    var bits : int?
    view_out(fa, bits)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

//! the punned view travels through a global: its writer is excluded
//! and so is every function that touches that global
var g_bits : int?

[never_inline, hint(noinline)]
def publish_view(var fa : float?) {
    g_bits = unsafe(reinterpret<int?>(fa))
}

[never_inline, hint(noinline)]
def scale_via_global(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *g_bits
    }
}

//! `memset32` writes the floats by bit pattern: its stores are
//! untyped, so the float reads after it see them
[never_inline, hint(noinline)]
def fill_floats_by_pattern(var fa : float?; n : int) : float {
    unsafe {
        memset32(reinterpret<void?>(fa), reinterpret<uint>(1.5f), n)
        var sum = 0.0f
        for (i in range(n)) {
            sum += fa[i]
        }
        return sum
    }
}

//! a byte view is not punning: the function stays tagged, and the
//! byte read inside the loop stays inside it
[never_inline, hint(noinline)]
def scale_byte_view(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    var low = 0
    unsafe {
        let bytes = reinterpret<uint8?>(fa)
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            low += int(bytes[0])
            i++
        }
    }
    return low
}

//! the double class
[never_inline, hint(noinline)]
def scale_double(var da : double?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            da[i] = da[i] * 0.5lf + double(*k_ptr)
            i++
        }
    }
}

//! a vector store carries no tag, so the int reads stay in the loop
[never_inline, hint(noinline)]
def scale_vector(var va : float4?; n_ptr : int?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            va[i] = va[i] * 0.5f + float4(float(*k_ptr))
            i++
        }
    }
}

//! the punned view passes through a middle function: the exclusion
//! needs a second pass over the callers, and the caller sits before
//! its source in the jit set
[never_inline, hint(noinline)]
def scale_before_middle(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits = through_middle(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

[never_inline, hint(noinline)]
def through_middle(var fa : float?) : int? => as_ints(fa)

[never_inline, hint(noinline)]
def scale_through_middle(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits = through_middle(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

//! the loop sits in a block literal of an excluded function: the
//! block's function is untagged too
[never_inline, hint(noinline)]
def scale_in_block(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits = as_ints(fa)
    var total = 0
    invoke($ {
        unsafe {
            var i = 0
            while (i < *n_ptr) {
                fa[i] = fa[i] * 0.5f + float(*k_ptr)
                i++
            }
            total = *bits
        }
    })
    return total
}

//! a reference bound under another type is punning too (a value
//! reinterpret is a bitcast of what the typed load read, not a view)
[never_inline, hint(noinline)]
def scale_through_int_ref(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    unsafe {
        let bits & = reinterpret<int&>(fa[0])
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return bits
    }
}

//! an opted-in source answers for its own pun: its caller stays
//! tagged
[never_inline, hint(noinline, strict_aliasing)]
def as_ints_forced(var fa : float?) : int? => unsafe(reinterpret<int?>(fa))

[never_inline, hint(noinline)]
def scale_through_forced(var fa : float?; n_ptr : int?; k_ptr : int?) : int {
    let bits = as_ints_forced(fa)
    unsafe {
        var i = 0
        while (i < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
        return *bits
    }
}

//! `double` and `float` are two classes: a bound read as `double`
//! leaves a `float` loop
[never_inline, hint(noinline)]
def scale_double_bound(var fa : float?; n_ptr : double?; k_ptr : int?) {
    unsafe {
        var i = 0
        while (double(i) < *n_ptr) {
            fa[i] = fa[i] * 0.5f + float(*k_ptr)
            i++
        }
    }
}

//! two integer widths share one class: the widened `memset16` store
//! is seen by the `uint16` reads
[never_inline, hint(noinline)]
def fill16(start, count : int; v : uint16) : int {
    var a : uint16[10]
    unsafe {
        memset16(addr(a[start]), v, count)
    }
    var n = 0
    for (i in 0..10) {
        if (a[i] == v) {
            n++
        }
    }
    return n
}

[export]
def main {
    var bufs : array<array<float>>
    bufs |> resize(23)
    for (b in bufs) {
        b |> resize(64)
    }
    var da : array<double>
    var va : array<float4>
    da |> resize(64)
    va |> resize(64)
    var ctl : int[2]
    ctl[0] = 64
    ctl[1] = 3
    var bits_c = 0
    var bits_d = 0
    var bits_e = 0
    var bits_f = 0
    var bits_g = 0
    var bits_h = 0
    var bits_k = 0
    var bits_l = 0
    var bits_m = 0
    var bits_n = 0
    var bits_o = 0
    var bits_p = 0
    var bits_q = 0
    var bits_r = 0
    var bits_s = 0
    var bits_t = 0
    var bits_u = 0
    var bound = 64.0lf
    var low = 0
    for (_p in range(3)) {
        unsafe {
            scale_typed(addr(bufs[0][0]), addr(ctl[0]), addr(ctl[1]))
            scale_untyped(addr(bufs[1][0]), addr(ctl[0]), addr(ctl[1]))
            bits_c = scale_punning(addr(bufs[2][0]), addr(ctl[0]), addr(ctl[1]))
            bits_d = scale_through_view(addr(bufs[3][0]), addr(ctl[0]), addr(ctl[1]))
            bits_e = scale_through_lambda(addr(bufs[4][0]), addr(ctl[0]), addr(ctl[1]))
            bits_f = scale_through_far_view(addr(bufs[5][0]), addr(ctl[0]), addr(ctl[1]))
            bits_g = scale_through_ref(addr(bufs[6][0]), addr(ctl[0]), addr(ctl[1]))
            bits_h = scale_through_interp(addr(bufs[7][0]), addr(ctl[0]), addr(ctl[1]))
            low = scale_byte_view(addr(bufs[8][0]), addr(ctl[0]), addr(ctl[1]))
            scale_double(addr(da[0]), addr(ctl[0]), addr(ctl[1]))
            scale_vector(addr(va[0]), addr(ctl[0]), addr(ctl[1]))
            bits_k = scale_through_middle(addr(bufs[10][0]), addr(ctl[0]), addr(ctl[1]))
            bits_l = scale_in_block(addr(bufs[11][0]), addr(ctl[0]), addr(ctl[1]))
            bits_m = scale_through_struct(addr(bufs[12][0]), addr(ctl[0]), addr(ctl[1]))
            bits_n = scale_pass_view(addr(bufs[13][0]), addr(ctl[0]), addr(ctl[1]))
            bits_o = scale_lambda_sink(addr(bufs[14][0]), addr(ctl[0]), addr(ctl[1]))
            publish_view(addr(bufs[15][0]))
            bits_p = scale_via_global(addr(bufs[15][0]), addr(ctl[0]), addr(ctl[1]))
            bits_q = scale_before_middle(addr(bufs[16][0]), addr(ctl[0]), addr(ctl[1]))
            bits_r = scale_through_int_ref(addr(bufs[17][0]), addr(ctl[0]), addr(ctl[1]))
            bits_s = scale_through_forced(addr(bufs[18][0]), addr(ctl[0]), addr(ctl[1]))
            scale_double_bound(addr(bufs[19][0]), addr(bound), addr(ctl[1]))
            bits_t = scale_through_out(addr(bufs[20][0]), addr(ctl[0]), addr(ctl[1]))
            bits_u = scale_through_callback(addr(bufs[21][0]), addr(ctl[0]), addr(ctl[1]))
            scale_self_view(addr(bufs[22][0]), addr(ctl[0]), addr(ctl[1]))
        }
    }
    let bits_5_25 = unsafe(reinterpret<int>(5.25f))
    let filled = fill16(2, ctl[1], uint16(0x55))
    let via_calls = (bits_d == bits_5_25 && bits_e == bits_5_25 && bits_f == bits_5_25
        && bits_g == bits_5_25 && bits_h == bits_5_25 && bits_k == bits_5_25 && bits_l == bits_5_25
        && bits_m == bits_5_25 && bits_n == bits_5_25 && bits_o == bits_5_25 && bits_p == bits_5_25
        && bits_q == bits_5_25 && bits_r == bits_5_25 && bits_s == bits_5_25 && bits_t == bits_5_25
        && bits_u == bits_5_25)
    let pattern = fill_floats_by_pattern(unsafe(addr(bufs[9][0])), 64)
    print("TYPED {bufs[0][7]} {bufs[0][63]} UNTYPED {bufs[1][7]} {bufs[1][63]} ")
    print("PUNNING {bits_c == bits_5_25} VIEW {via_calls} FILL {filled} ")
    print("PATTERN {pattern} BYTES {low} DOUBLE {da[63]} VECTOR {va[63].w} BOUND {bufs[19][63]}")
    print(" SELF {bufs[22][63]}\n")
}
