options gen2
options _dasllama_internal = true

require dasllama/dasllama_math_gen

// The program test_kquant.das's row-loop metadata cell compiles - its claim is this program's own compile: the row-major
// q8 GEMV stamped through its generator four ways, a NEON row and an x64 maddubs row, each at the generator's default and
// with `row_jam` flipped, so a compile-only `--jit-dump` shows which forms carry `llvm.loop.unroll_and_jam.disable`. A
// row the target cannot run declines to the family's reference body. main walks one row of each so every stub is used.

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", mr = 4), hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def neon_row_default(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", mr = 4, row_jam = false), hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def neon_row_unjammed(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8), hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def x64_row_default(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true), hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def x64_row_jammed(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[export]
def main {
    var y : array<float>
    var w : array<int8>
    var s : array<uint16>
    var xq : array<int8>
    var xs : array<float>
    y |> resize(1)
    w |> resize(32)
    s |> resize(1)
    xq |> resize(32)
    xs |> resize(1)
    unsafe {
        neon_row_default(addr(y[0]), addr(w[0]), addr(s[0]), addr(xq[0]), addr(xs[0]), 32l, 0l, 1l)
        neon_row_unjammed(addr(y[0]), addr(w[0]), addr(s[0]), addr(xq[0]), addr(xs[0]), 32l, 0l, 1l)
        x64_row_default(addr(y[0]), addr(w[0]), addr(s[0]), addr(xq[0]), addr(xs[0]), 32l, 0l, 1l)
        x64_row_jammed(addr(y[0]), addr(w[0]), addr(s[0]), addr(xq[0]), addr(xs[0]), 32l, 0l, 1l)
    }
    print("ROWS {y[0]}\n")
}
