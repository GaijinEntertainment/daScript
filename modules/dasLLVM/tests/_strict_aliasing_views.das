options gen2
module _strict_aliasing_views shared public

//! a punned view handed across a module boundary - under
//! --jit-split-modules a separate partition
[never_inline, hint(noinline)]
def as_ints_far(var fa : float?) : int? => unsafe(reinterpret<int?>(fa))

//! the same view, inside a struct
struct IntView {
    bits : int?
}

[never_inline, hint(noinline)]
def as_view_struct(var fa : float?) : IntView => IntView(bits = unsafe(reinterpret<int?>(fa)))

//! the same view, as a reference
[never_inline, hint(noinline)]
def as_int_ref(var fa : float?) : int& {
    unsafe {
        return *reinterpret<int?>(fa)
    }
}

//! the same view from a function the JIT never compiles: its body is
//! never scanned, so its pointer result counts as punned
[no_jit]
def as_ints_interp(var fa : float?) : int? => unsafe(reinterpret<int?>(fa))
