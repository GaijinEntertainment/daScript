options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _jit_fast_math = true   // match gen_tune_probe, whose fixture and bar this race reads
options _dasllama_internal = true

require ../performance/profile_common.das   // tune_gate - the standing pre-measure stack
require _race_fixture                       // the round loop, the poison, the compare and the refusal tally
require _gen_probe_fixture                  // close_within - the bar's compare
require _rm_fixture                         // the tuner's row-major fixture, its streamed shape, its bar, the row runner
require dasllama/dasllama_math
require dasllama/dasllama_math_gen
require dasllama/dasllama_kqformat
require dasllama/dasllama_par
require daslib/build_const                  // build_arg_value - `--rows`

// The row-major GEMV's row loop with and without LLVM's unroll-and-jam, raced: one process, every x86 perm row of the
// row-major families the noalias hints change (q8q8, mlx8, mlx4, q4032, q5032, iq4nl32, mx4) stamped twice through
// the [llvm_code] generator - `row_jam = true` lets the optimizer jam rows (what the family's noalias hints make
// legal), `row_jam = false` keeps the row loop as written - each walking the tuner's fixture at the tuner's streamed
// shape (`rm_streamed_shape`) and, for mlx8 and mlx4, at `MLX_WIDE_N` as well (every row, one call, one lane), once
// a round in turn, `ROUNDS` rounds after `WARMUP` dropped. A pair's baseline arm is its `row_jam = false` stamp (the
// form x64 ships), held to the fixture's scalar oracle within the family's bar (`rm_tol`); the compare region is
// every output row of the shape; the jammed arm is held bit for bit to the baseline's image. A pair whose compare
// misses prints the miss and no time; a pair whose row declined to its reference body on this box (its witness stub
// reads false) prints that and no time. `--rows <list>` races only the rows whose name holds a listed word.
//
//   bin/daslang -jit modules/dasLLAMA/harness/rm_row_jam_race.das [-- --rows maddubs]

let ROUNDS = 15
let WARMUP = 2
let MLX_WIDE_N = 4096l   //! the mlx models' hidden width, the row the affine families also serve beside the tuner's shape

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "maddubs", width = 256, mr = 8)]
def q8q8_maddubs_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_maddubs_w256_a0_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_maddubs_w256_a0_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "maddubs", width = 256, mr = 8, accs = 1)]
def q8q8_maddubs_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_maddubs_w256_a1_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_maddubs_w256_a1_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "maddubs", width = 256, mr = 8, accs = 2)]
def q8q8_maddubs_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_maddubs_w256_a2_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_maddubs_w256_a2_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "vpdpbusd", width = 256, mr = 8)]
def q8q8_vpdpbusd_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w256_a0_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w256_a0_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 1)]
def q8q8_vpdpbusd_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w256_a1_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w256_a1_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 2)]
def q8q8_vpdpbusd_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w256_a2_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w256_a2_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "vpdpbusd", width = 512, mr = 16)]
def q8q8_vpdpbusd_w512_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w512_a0_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w512_a0_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 1)]
def q8q8_vpdpbusd_w512_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w512_a1_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w512_a1_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 2)]
def q8q8_vpdpbusd_w512_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = true),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w512_a2_jam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q8q8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = false),
 hint(unsafe_range_check, noalias = wp, noalias = sp, noalias = xqp, noalias = xsp)]
def q8q8_vpdpbusd_w512_a2_nojam(var yp : float?; wp : int8 const?; sp : uint16 const?; xqp : int8 const?; xsp : float const?; n, rb, re : int64) : void {
    q8q8_rm_gemv_gen(yp, wp, sp, xqp, xsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8)]
def mlx8_maddubs_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_maddubs_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_maddubs_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 1)]
def mlx8_maddubs_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_maddubs_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_maddubs_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 2)]
def mlx8_maddubs_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_maddubs_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_maddubs_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8)]
def mlx8_vpdpbusd_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 1)]
def mlx8_vpdpbusd_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 2)]
def mlx8_vpdpbusd_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16)]
def mlx8_vpdpbusd_w512_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w512_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w512_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 1)]
def mlx8_vpdpbusd_w512_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w512_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w512_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 2)]
def mlx8_vpdpbusd_w512_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w512_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx8_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx8_vpdpbusd_w512_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx8q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8)]
def mlx4_maddubs_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_maddubs_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_maddubs_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 1)]
def mlx4_maddubs_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_maddubs_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_maddubs_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 2)]
def mlx4_maddubs_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_maddubs_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_maddubs_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8)]
def mlx4_vpdpbusd_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 1)]
def mlx4_vpdpbusd_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 2)]
def mlx4_vpdpbusd_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16)]
def mlx4_vpdpbusd_w512_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w512_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w512_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 1)]
def mlx4_vpdpbusd_w512_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w512_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w512_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 2)]
def mlx4_vpdpbusd_w512_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w512_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mlx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mlx4_vpdpbusd_w512_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mlx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8)]
def q4032_maddubs_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_maddubs_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_maddubs_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 1)]
def q4032_maddubs_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_maddubs_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_maddubs_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 2)]
def q4032_maddubs_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_maddubs_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_maddubs_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8)]
def q4032_vpdpbusd_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 1)]
def q4032_vpdpbusd_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 2)]
def q4032_vpdpbusd_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16)]
def q4032_vpdpbusd_w512_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w512_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w512_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 1)]
def q4032_vpdpbusd_w512_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w512_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w512_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 2)]
def q4032_vpdpbusd_w512_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w512_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q4032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q4032_vpdpbusd_w512_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q4032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8)]
def iq4nl32_maddubs_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_maddubs_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_maddubs_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 1)]
def iq4nl32_maddubs_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_maddubs_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_maddubs_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 2)]
def iq4nl32_maddubs_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_maddubs_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_maddubs_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8)]
def iq4nl32_vpdpbusd_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 1)]
def iq4nl32_vpdpbusd_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 2)]
def iq4nl32_vpdpbusd_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16)]
def iq4nl32_vpdpbusd_w512_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w512_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w512_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 1)]
def iq4nl32_vpdpbusd_w512_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w512_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w512_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 2)]
def iq4nl32_vpdpbusd_w512_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w512_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::iq4nl32_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def iq4nl32_vpdpbusd_w512_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    iq4nl32q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8)]
def mx4_maddubs_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_maddubs_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_maddubs_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 1)]
def mx4_maddubs_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_maddubs_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_maddubs_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 2)]
def mx4_maddubs_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_maddubs_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_maddubs_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8)]
def mx4_vpdpbusd_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 1)]
def mx4_vpdpbusd_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 2)]
def mx4_vpdpbusd_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16)]
def mx4_vpdpbusd_w512_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w512_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w512_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 1)]
def mx4_vpdpbusd_w512_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w512_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w512_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 2)]
def mx4_vpdpbusd_w512_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w512_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::mx4_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def mx4_vpdpbusd_w512_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    mx4q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8)]
def q5032_maddubs_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_maddubs_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "maddubs", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_maddubs_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 1)]
def q5032_maddubs_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_maddubs_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_maddubs_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "maddubs", width = 256, mr = 8, accs = 2)]
def q5032_maddubs_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_maddubs_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "maddubs", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_maddubs_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8)]
def q5032_vpdpbusd_w256_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w256_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w256_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 1)]
def q5032_vpdpbusd_w256_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w256_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w256_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 256, mr = 8, accs = 2)]
def q5032_vpdpbusd_w256_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w256_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 256, mr = 8, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w256_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16)]
def q5032_vpdpbusd_w512_a0_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w512_a0_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w512_a0_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 1)]
def q5032_vpdpbusd_w512_a1_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w512_a1_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 1, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w512_a1_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::kq_witness", dot = "vpdpbusd", width = 512, mr = 16, accs = 2)]
def q5032_vpdpbusd_w512_a2_witness() : bool => false

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = true),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w512_a2_jam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

[llvm_code(name = "dasllama_gemm_gen::q5032_rm_gemv", dot = "vpdpbusd", width = 512, mr = 16, accs = 2, row_jam = false),
 unused_argument(xbsp), hint(unsafe_range_check, noalias = qp, noalias = sp, noalias = xqp, noalias = xsp, noalias = xbsp)]
def q5032_vpdpbusd_w512_a2_nojam(var yp : float?; qp : uint8 const?; sp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n, rb, re : int64) : void {
    q5032q8_rm_gemv_gen(yp, qp, sp, xqp, xsp, xbsp, n, rb, re)
}

//! one raced stamp: its format, perm row and row-loop pick, its stub (the q8 signature or the per-32 one) and the row's
//! witness stub
struct Arm {
    fmt : KqFmt
    row : string
    jam : bool
    q8 : MatmulQ8Q8RowsFnS16
    kq : MatmulKqRowsFn
    witness : function<() : bool>
}

//! every raced stamp, the jammed arm of a pair first
def private arms_table() : array<Arm> {   // nolint:STYLE038 - a table, one row a raced stamp
    return <- [Arm(fmt = KqFmt.q8, row = "maddubs_width256_mr8", jam = true, q8 = @@q8q8_maddubs_w256_a0_jam, witness = @@q8q8_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.q8, row = "maddubs_width256_mr8", jam = false, q8 = @@q8q8_maddubs_w256_a0_nojam, witness = @@q8q8_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.q8, row = "maddubs_width256_mr8_accs1", jam = true, q8 = @@q8q8_maddubs_w256_a1_jam, witness = @@q8q8_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.q8, row = "maddubs_width256_mr8_accs1", jam = false, q8 = @@q8q8_maddubs_w256_a1_nojam, witness = @@q8q8_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.q8, row = "maddubs_width256_mr8_accs2", jam = true, q8 = @@q8q8_maddubs_w256_a2_jam, witness = @@q8q8_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.q8, row = "maddubs_width256_mr8_accs2", jam = false, q8 = @@q8q8_maddubs_w256_a2_nojam, witness = @@q8q8_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width256_mr8", jam = true, q8 = @@q8q8_vpdpbusd_w256_a0_jam, witness = @@q8q8_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width256_mr8", jam = false, q8 = @@q8q8_vpdpbusd_w256_a0_nojam, witness = @@q8q8_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width256_mr8_accs1", jam = true, q8 = @@q8q8_vpdpbusd_w256_a1_jam, witness = @@q8q8_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width256_mr8_accs1", jam = false, q8 = @@q8q8_vpdpbusd_w256_a1_nojam, witness = @@q8q8_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width256_mr8_accs2", jam = true, q8 = @@q8q8_vpdpbusd_w256_a2_jam, witness = @@q8q8_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width256_mr8_accs2", jam = false, q8 = @@q8q8_vpdpbusd_w256_a2_nojam, witness = @@q8q8_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width512_mr16", jam = true, q8 = @@q8q8_vpdpbusd_w512_a0_jam, witness = @@q8q8_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width512_mr16", jam = false, q8 = @@q8q8_vpdpbusd_w512_a0_nojam, witness = @@q8q8_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width512_mr16_accs1", jam = true, q8 = @@q8q8_vpdpbusd_w512_a1_jam, witness = @@q8q8_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width512_mr16_accs1", jam = false, q8 = @@q8q8_vpdpbusd_w512_a1_nojam, witness = @@q8q8_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width512_mr16_accs2", jam = true, q8 = @@q8q8_vpdpbusd_w512_a2_jam, witness = @@q8q8_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.q8, row = "vpdpbusd_width512_mr16_accs2", jam = false, q8 = @@q8q8_vpdpbusd_w512_a2_nojam, witness = @@q8q8_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.mlx8, row = "maddubs_width256_mr8", jam = true, kq = @@mlx8_maddubs_w256_a0_jam, witness = @@mlx8_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.mlx8, row = "maddubs_width256_mr8", jam = false, kq = @@mlx8_maddubs_w256_a0_nojam, witness = @@mlx8_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.mlx8, row = "maddubs_width256_mr8_accs1", jam = true, kq = @@mlx8_maddubs_w256_a1_jam, witness = @@mlx8_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.mlx8, row = "maddubs_width256_mr8_accs1", jam = false, kq = @@mlx8_maddubs_w256_a1_nojam, witness = @@mlx8_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.mlx8, row = "maddubs_width256_mr8_accs2", jam = true, kq = @@mlx8_maddubs_w256_a2_jam, witness = @@mlx8_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.mlx8, row = "maddubs_width256_mr8_accs2", jam = false, kq = @@mlx8_maddubs_w256_a2_nojam, witness = @@mlx8_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width256_mr8", jam = true, kq = @@mlx8_vpdpbusd_w256_a0_jam, witness = @@mlx8_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width256_mr8", jam = false, kq = @@mlx8_vpdpbusd_w256_a0_nojam, witness = @@mlx8_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width256_mr8_accs1", jam = true, kq = @@mlx8_vpdpbusd_w256_a1_jam, witness = @@mlx8_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width256_mr8_accs1", jam = false, kq = @@mlx8_vpdpbusd_w256_a1_nojam, witness = @@mlx8_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width256_mr8_accs2", jam = true, kq = @@mlx8_vpdpbusd_w256_a2_jam, witness = @@mlx8_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width256_mr8_accs2", jam = false, kq = @@mlx8_vpdpbusd_w256_a2_nojam, witness = @@mlx8_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width512_mr16", jam = true, kq = @@mlx8_vpdpbusd_w512_a0_jam, witness = @@mlx8_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width512_mr16", jam = false, kq = @@mlx8_vpdpbusd_w512_a0_nojam, witness = @@mlx8_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width512_mr16_accs1", jam = true, kq = @@mlx8_vpdpbusd_w512_a1_jam, witness = @@mlx8_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width512_mr16_accs1", jam = false, kq = @@mlx8_vpdpbusd_w512_a1_nojam, witness = @@mlx8_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width512_mr16_accs2", jam = true, kq = @@mlx8_vpdpbusd_w512_a2_jam, witness = @@mlx8_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.mlx8, row = "vpdpbusd_width512_mr16_accs2", jam = false, kq = @@mlx8_vpdpbusd_w512_a2_nojam, witness = @@mlx8_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.mlx4, row = "maddubs_width256_mr8", jam = true, kq = @@mlx4_maddubs_w256_a0_jam, witness = @@mlx4_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.mlx4, row = "maddubs_width256_mr8", jam = false, kq = @@mlx4_maddubs_w256_a0_nojam, witness = @@mlx4_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.mlx4, row = "maddubs_width256_mr8_accs1", jam = true, kq = @@mlx4_maddubs_w256_a1_jam, witness = @@mlx4_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.mlx4, row = "maddubs_width256_mr8_accs1", jam = false, kq = @@mlx4_maddubs_w256_a1_nojam, witness = @@mlx4_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.mlx4, row = "maddubs_width256_mr8_accs2", jam = true, kq = @@mlx4_maddubs_w256_a2_jam, witness = @@mlx4_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.mlx4, row = "maddubs_width256_mr8_accs2", jam = false, kq = @@mlx4_maddubs_w256_a2_nojam, witness = @@mlx4_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width256_mr8", jam = true, kq = @@mlx4_vpdpbusd_w256_a0_jam, witness = @@mlx4_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width256_mr8", jam = false, kq = @@mlx4_vpdpbusd_w256_a0_nojam, witness = @@mlx4_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width256_mr8_accs1", jam = true, kq = @@mlx4_vpdpbusd_w256_a1_jam, witness = @@mlx4_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width256_mr8_accs1", jam = false, kq = @@mlx4_vpdpbusd_w256_a1_nojam, witness = @@mlx4_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width256_mr8_accs2", jam = true, kq = @@mlx4_vpdpbusd_w256_a2_jam, witness = @@mlx4_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width256_mr8_accs2", jam = false, kq = @@mlx4_vpdpbusd_w256_a2_nojam, witness = @@mlx4_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width512_mr16", jam = true, kq = @@mlx4_vpdpbusd_w512_a0_jam, witness = @@mlx4_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width512_mr16", jam = false, kq = @@mlx4_vpdpbusd_w512_a0_nojam, witness = @@mlx4_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width512_mr16_accs1", jam = true, kq = @@mlx4_vpdpbusd_w512_a1_jam, witness = @@mlx4_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width512_mr16_accs1", jam = false, kq = @@mlx4_vpdpbusd_w512_a1_nojam, witness = @@mlx4_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width512_mr16_accs2", jam = true, kq = @@mlx4_vpdpbusd_w512_a2_jam, witness = @@mlx4_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.mlx4, row = "vpdpbusd_width512_mr16_accs2", jam = false, kq = @@mlx4_vpdpbusd_w512_a2_nojam, witness = @@mlx4_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.q4032, row = "maddubs_width256_mr8", jam = true, kq = @@q4032_maddubs_w256_a0_jam, witness = @@q4032_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.q4032, row = "maddubs_width256_mr8", jam = false, kq = @@q4032_maddubs_w256_a0_nojam, witness = @@q4032_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.q4032, row = "maddubs_width256_mr8_accs1", jam = true, kq = @@q4032_maddubs_w256_a1_jam, witness = @@q4032_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.q4032, row = "maddubs_width256_mr8_accs1", jam = false, kq = @@q4032_maddubs_w256_a1_nojam, witness = @@q4032_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.q4032, row = "maddubs_width256_mr8_accs2", jam = true, kq = @@q4032_maddubs_w256_a2_jam, witness = @@q4032_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.q4032, row = "maddubs_width256_mr8_accs2", jam = false, kq = @@q4032_maddubs_w256_a2_nojam, witness = @@q4032_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width256_mr8", jam = true, kq = @@q4032_vpdpbusd_w256_a0_jam, witness = @@q4032_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width256_mr8", jam = false, kq = @@q4032_vpdpbusd_w256_a0_nojam, witness = @@q4032_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width256_mr8_accs1", jam = true, kq = @@q4032_vpdpbusd_w256_a1_jam, witness = @@q4032_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width256_mr8_accs1", jam = false, kq = @@q4032_vpdpbusd_w256_a1_nojam, witness = @@q4032_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width256_mr8_accs2", jam = true, kq = @@q4032_vpdpbusd_w256_a2_jam, witness = @@q4032_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width256_mr8_accs2", jam = false, kq = @@q4032_vpdpbusd_w256_a2_nojam, witness = @@q4032_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width512_mr16", jam = true, kq = @@q4032_vpdpbusd_w512_a0_jam, witness = @@q4032_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width512_mr16", jam = false, kq = @@q4032_vpdpbusd_w512_a0_nojam, witness = @@q4032_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width512_mr16_accs1", jam = true, kq = @@q4032_vpdpbusd_w512_a1_jam, witness = @@q4032_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width512_mr16_accs1", jam = false, kq = @@q4032_vpdpbusd_w512_a1_nojam, witness = @@q4032_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width512_mr16_accs2", jam = true, kq = @@q4032_vpdpbusd_w512_a2_jam, witness = @@q4032_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.q4032, row = "vpdpbusd_width512_mr16_accs2", jam = false, kq = @@q4032_vpdpbusd_w512_a2_nojam, witness = @@q4032_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "maddubs_width256_mr8", jam = true, kq = @@iq4nl32_maddubs_w256_a0_jam, witness = @@iq4nl32_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "maddubs_width256_mr8", jam = false, kq = @@iq4nl32_maddubs_w256_a0_nojam, witness = @@iq4nl32_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "maddubs_width256_mr8_accs1", jam = true, kq = @@iq4nl32_maddubs_w256_a1_jam, witness = @@iq4nl32_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "maddubs_width256_mr8_accs1", jam = false, kq = @@iq4nl32_maddubs_w256_a1_nojam, witness = @@iq4nl32_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "maddubs_width256_mr8_accs2", jam = true, kq = @@iq4nl32_maddubs_w256_a2_jam, witness = @@iq4nl32_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "maddubs_width256_mr8_accs2", jam = false, kq = @@iq4nl32_maddubs_w256_a2_nojam, witness = @@iq4nl32_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width256_mr8", jam = true, kq = @@iq4nl32_vpdpbusd_w256_a0_jam, witness = @@iq4nl32_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width256_mr8", jam = false, kq = @@iq4nl32_vpdpbusd_w256_a0_nojam, witness = @@iq4nl32_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width256_mr8_accs1", jam = true, kq = @@iq4nl32_vpdpbusd_w256_a1_jam, witness = @@iq4nl32_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width256_mr8_accs1", jam = false, kq = @@iq4nl32_vpdpbusd_w256_a1_nojam, witness = @@iq4nl32_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width256_mr8_accs2", jam = true, kq = @@iq4nl32_vpdpbusd_w256_a2_jam, witness = @@iq4nl32_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width256_mr8_accs2", jam = false, kq = @@iq4nl32_vpdpbusd_w256_a2_nojam, witness = @@iq4nl32_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width512_mr16", jam = true, kq = @@iq4nl32_vpdpbusd_w512_a0_jam, witness = @@iq4nl32_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width512_mr16", jam = false, kq = @@iq4nl32_vpdpbusd_w512_a0_nojam, witness = @@iq4nl32_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width512_mr16_accs1", jam = true, kq = @@iq4nl32_vpdpbusd_w512_a1_jam, witness = @@iq4nl32_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width512_mr16_accs1", jam = false, kq = @@iq4nl32_vpdpbusd_w512_a1_nojam, witness = @@iq4nl32_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width512_mr16_accs2", jam = true, kq = @@iq4nl32_vpdpbusd_w512_a2_jam, witness = @@iq4nl32_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.iq4nl32, row = "vpdpbusd_width512_mr16_accs2", jam = false, kq = @@iq4nl32_vpdpbusd_w512_a2_nojam, witness = @@iq4nl32_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.mx4, row = "maddubs_width256_mr8", jam = true, kq = @@mx4_maddubs_w256_a0_jam, witness = @@mx4_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.mx4, row = "maddubs_width256_mr8", jam = false, kq = @@mx4_maddubs_w256_a0_nojam, witness = @@mx4_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.mx4, row = "maddubs_width256_mr8_accs1", jam = true, kq = @@mx4_maddubs_w256_a1_jam, witness = @@mx4_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.mx4, row = "maddubs_width256_mr8_accs1", jam = false, kq = @@mx4_maddubs_w256_a1_nojam, witness = @@mx4_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.mx4, row = "maddubs_width256_mr8_accs2", jam = true, kq = @@mx4_maddubs_w256_a2_jam, witness = @@mx4_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.mx4, row = "maddubs_width256_mr8_accs2", jam = false, kq = @@mx4_maddubs_w256_a2_nojam, witness = @@mx4_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width256_mr8", jam = true, kq = @@mx4_vpdpbusd_w256_a0_jam, witness = @@mx4_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width256_mr8", jam = false, kq = @@mx4_vpdpbusd_w256_a0_nojam, witness = @@mx4_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width256_mr8_accs1", jam = true, kq = @@mx4_vpdpbusd_w256_a1_jam, witness = @@mx4_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width256_mr8_accs1", jam = false, kq = @@mx4_vpdpbusd_w256_a1_nojam, witness = @@mx4_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width256_mr8_accs2", jam = true, kq = @@mx4_vpdpbusd_w256_a2_jam, witness = @@mx4_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width256_mr8_accs2", jam = false, kq = @@mx4_vpdpbusd_w256_a2_nojam, witness = @@mx4_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width512_mr16", jam = true, kq = @@mx4_vpdpbusd_w512_a0_jam, witness = @@mx4_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width512_mr16", jam = false, kq = @@mx4_vpdpbusd_w512_a0_nojam, witness = @@mx4_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width512_mr16_accs1", jam = true, kq = @@mx4_vpdpbusd_w512_a1_jam, witness = @@mx4_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width512_mr16_accs1", jam = false, kq = @@mx4_vpdpbusd_w512_a1_nojam, witness = @@mx4_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width512_mr16_accs2", jam = true, kq = @@mx4_vpdpbusd_w512_a2_jam, witness = @@mx4_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.mx4, row = "vpdpbusd_width512_mr16_accs2", jam = false, kq = @@mx4_vpdpbusd_w512_a2_nojam, witness = @@mx4_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.q5032, row = "maddubs_width256_mr8", jam = true, kq = @@q5032_maddubs_w256_a0_jam, witness = @@q5032_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.q5032, row = "maddubs_width256_mr8", jam = false, kq = @@q5032_maddubs_w256_a0_nojam, witness = @@q5032_maddubs_w256_a0_witness),
        Arm(fmt = KqFmt.q5032, row = "maddubs_width256_mr8_accs1", jam = true, kq = @@q5032_maddubs_w256_a1_jam, witness = @@q5032_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.q5032, row = "maddubs_width256_mr8_accs1", jam = false, kq = @@q5032_maddubs_w256_a1_nojam, witness = @@q5032_maddubs_w256_a1_witness),
        Arm(fmt = KqFmt.q5032, row = "maddubs_width256_mr8_accs2", jam = true, kq = @@q5032_maddubs_w256_a2_jam, witness = @@q5032_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.q5032, row = "maddubs_width256_mr8_accs2", jam = false, kq = @@q5032_maddubs_w256_a2_nojam, witness = @@q5032_maddubs_w256_a2_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width256_mr8", jam = true, kq = @@q5032_vpdpbusd_w256_a0_jam, witness = @@q5032_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width256_mr8", jam = false, kq = @@q5032_vpdpbusd_w256_a0_nojam, witness = @@q5032_vpdpbusd_w256_a0_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width256_mr8_accs1", jam = true, kq = @@q5032_vpdpbusd_w256_a1_jam, witness = @@q5032_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width256_mr8_accs1", jam = false, kq = @@q5032_vpdpbusd_w256_a1_nojam, witness = @@q5032_vpdpbusd_w256_a1_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width256_mr8_accs2", jam = true, kq = @@q5032_vpdpbusd_w256_a2_jam, witness = @@q5032_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width256_mr8_accs2", jam = false, kq = @@q5032_vpdpbusd_w256_a2_nojam, witness = @@q5032_vpdpbusd_w256_a2_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width512_mr16", jam = true, kq = @@q5032_vpdpbusd_w512_a0_jam, witness = @@q5032_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width512_mr16", jam = false, kq = @@q5032_vpdpbusd_w512_a0_nojam, witness = @@q5032_vpdpbusd_w512_a0_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width512_mr16_accs1", jam = true, kq = @@q5032_vpdpbusd_w512_a1_jam, witness = @@q5032_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width512_mr16_accs1", jam = false, kq = @@q5032_vpdpbusd_w512_a1_nojam, witness = @@q5032_vpdpbusd_w512_a1_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width512_mr16_accs2", jam = true, kq = @@q5032_vpdpbusd_w512_a2_jam, witness = @@q5032_vpdpbusd_w512_a2_witness),
        Arm(fmt = KqFmt.q5032, row = "vpdpbusd_width512_mr16_accs2", jam = false, kq = @@q5032_vpdpbusd_w512_a2_nojam, witness = @@q5032_vpdpbusd_w512_a2_witness)]
}

//! the pair's compare: the baseline against the oracle within the bar, the jammed arm against the baseline's bits
def private pair_held(fx : RmFixture; base, jam : array<float>) : tuple<base : Held; jam : Held> {
    let tol = rm_tol(fx)
    var maxdiff = 0.0
    let ok = close_within(base, fx.yref, int(fx.d), maxdiff) $(_i : int) => tol
    let hb : Held = (diff = maxdiff, bound = tol, ok = ok)
    return (base = hb, jam = held(jam, base, 0.0))
}

//! one family's arms at one shape: the pairs' rows, or -1 when a pair's compare misses
def private race(arms : array<Arm>; f : KqFmt; n, d : int64; rowsList : string) : int {
    var fx <- build_rm_fixture(f, n, d)
    var wanted <- [for (i, a in count(), arms); i; where a.fmt == f && arm_wanted(rowsList, a.row, true)]
    for (i in wanted) {
        if (arms[i].jam && !invoke(arms[i].witness)) {
            print("{f} n={fx.n} rows={fx.d} arm={arms[i].row} | declined to its reference body on this box - no row\n")
        }
    }
    var picked <- [for (i in wanted); i; where invoke(arms[i].witness)]
    var y : array<float>
    y |> resize(fx.d)
    var images : array<array<float>>
    images |> reserve(length(picked))
    for (i in picked) {
        poison(y)
        rm_run(arms[i].q8, arms[i].kq, fx, y)
        var inscope img := y
        images |> emplace(img)
    }
    var times <- time_rounds(length(picked), ROUNDS, WARMUP) $(k : int) {
        rm_run(arms[picked[k]].q8, arms[picked[k]].kq, fx, y)
    }
    var good = true
    for (pi in range(length(picked) / 2)) {
        let k = pi * 2
        let h = pair_held(fx, images[k + 1], images[k])
        let a = arms[picked[k]]
        let region = "{fx.d} rows"
        let lines = fixed_array(
            (jam = false, k = k + 1, words = compare_words(h.base, "the scalar oracle", region)),
            (jam = true, k = k, words = compare_words(h.jam, "the unjammed (baseline) arm", region)))
        for (ln in lines) {
            let role = ln.jam ? "" : " (baseline)"
            if (h.base.ok && h.jam.ok) {
                let st = stats(times[ln.k])
                print("{f} n={fx.n} rows={fx.d} arm={a.row} row_jam={ln.jam}{role} best_ms={st.best} median_ms={st.median} cv%={st.cv} | {ln.words}\n")
            } else {
                print("{f} n={fx.n} rows={fx.d} arm={a.row} row_jam={ln.jam}{role} | {ln.words}\n")
            }
        }
        good &&= h.base.ok && h.jam.ok
    }
    let printed = length(picked)
    delete fx
    delete y
    delete images
    delete times
    delete picked
    delete wanted
    return good ? printed : -1
}

[export]
def main() : int {
    var t : RaceTally
    let rowsList = build_arg_value("--rows", "")
    if (!tune_gate()) {
        print("rm_row_jam_race: the tune gate refused - no row\n")
        t.bad++
    } else {
        print("rm_row_jam_race: {ROUNDS} rounds after {WARMUP} dropped, one lane\n")
        let arms <- arms_table()
        for (f in [KqFmt.q8, KqFmt.mlx8, KqFmt.mlx4, KqFmt.q4032, KqFmt.q5032, KqFmt.iq4nl32, KqFmt.mx4]) {
            let shape = rm_streamed_shape(f)
            t |> tally(race(arms, f, shape.n, shape.d, rowsList))
            if (f == KqFmt.mlx8 || f == KqFmt.mlx4) {
                t |> tally(race(arms, f, MLX_WIDE_N, shape.d, rowsList))
            }
        }
    }
    return race_exit(t, "rm_row_jam_race")
}
