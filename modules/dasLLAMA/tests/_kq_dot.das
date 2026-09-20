options gen2
options _dasllama_internal = true

require dasllama/dasllama_math_default
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
