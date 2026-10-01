options gen2
options _dasllama_internal = true

require dasllama/dasllama_math_default
require dasllama/dasllama_math_gen
require dasllama/dasllama_kqformat

//! One row's disk-order kq dot for a format decided at run time - the reference the kq gates
//! hold the kernels to. It lives here and not beside `dot_kq` (HOW_TO_ADD_A_FORMAT.md sec.4).
def dot_kq(kqrow : uint8 const?; ksrow : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; fmt : KqFmt) : float {
    var r = 0.0
    kq_fmt_stamp(fmt) $(F) {
        r = dot_kq(kqrow, ksrow, xqp, xsp, xbsp, n, F)
    }
    return r
}

//! Rows [rb, re) of the format-`kf` GEMV off its grp planes, through the engine's `kq_gemv_gen` family - a kernel under test.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#jit-partition-inlining")]
def kq_gemv_rt(kf : KqFmt; var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) {
    kq_fmt_stamp(kf) $(F) {
        kq_gemv_gen(yp, kqp, ksp, xqp, xsp, xbsp, n, rb, re, F)
    }
}

//! The format-`kf` 4-token tile of mr-group `g` at token `tk`, through the engine's `kq_tile_gen` family - a kernel under test.
//! `kqg` is the group's quant plane where the format's tile reads the packed planes, else its unpacked byte panel.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#jit-partition-inlining")]
def kq_tile_rt(kf : KqFmt; var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, d, g, tk : int64) {
    kq_fmt_stamp(kf) $(F) {
        kq_tile_gen(yp, kqg, ksg, xqp, xsp, xbsp, n, d, g, tk, F)
    }
}

//! Row `r` of an mr-group's format-`kf` grp planes dotted with a Q8_K activation row - the engine's grp row dot, a kernel
//! under test in a cell and never a gate's expected value; the harness's kq fixture reads it as its reference body.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#jit-partition-inlining")]
def kq_grp_row_dot_rt(kf : KqFmt; kqg : uint8 const?; ksg : uint8 const?; r, mr : int64; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64) : float {
    var v = 0.0
    kq_fmt_stamp(kf) $(F) {
        v = grp_row_dot(kqg, ksg, r, mr, xqp, xsp, xbsp, n, F)
    }
    return v
}

//! The formats with no superblock grp planes: the stamp binds every `KqFmt` member, and a call on one of these panics.
def private grp_row_dot(_kqg : uint8 const?; _ksg : uint8 const?; _r, _mr : int64; _xqp : int8 const?; _xsp : float const?; _xbsp : int const?; _n : int64; _f : KqTag_q8 | KqTag_q51 | KqTag_iq4nl32 | KqTag_mx4) : float {
    panic("{typeinfo stripped_typename(_f)} has no superblock grp row dot")
    return 0.0
}
