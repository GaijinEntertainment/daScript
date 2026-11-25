options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

// The CUDA home's flash pass at head 512 alone, with no model: hashed q rows and one K/V head's f16 rows at gemma-4's global
// shape (--heads query heads over one K/V head), a whole causal pass over --rows rows on each stamp - the wide tile and the
// KHR arm - the wall a launch and the multiply-adds a second, and how far the two outputs sit apart.
//
//   DASLLAMA_GPU=1 DASLLAMA_CUDA=1 bin/daslang -jit modules/dasLLAMA/harness/cuda_fa_probe.das -- [--rows 16384] [--heads 8] [--reps 3] [--mask 0]

require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_gpu_kernels_discrete   // FaArgs, fa_stamp_wgs
require dasllama/dasllama_math   // reserve_resize
require daslib/clargs
require _cuda_time   // cuda_time_best
require math

[CommandLineArgs]
struct Args {
    @clarg_doc = "Query rows, and keys: a whole causal pass"
    rows : int = 16384
    @clarg_doc = "Query heads over the one K/V head"
    heads : int = 8
    @clarg_doc = "Launches a timed chain"
    reps : int = 3
    @clarg_doc = "The wide tile's debug word (FaArgs.sinkoff): read only by a build that knocks phases out"
    mask : int = 0
    @clarg_doc = "Skip the KHR arm (slow at long passes)"
    wide_only : bool
    @clarg_short = "h"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let private HS = 512
let private SLACK_ROWS = 64

def private hash32(i : uint) : uint {
    var x = i * 0x9E3779B9u + 0x7F4A7C15u
    x ^= x >> 16u
    x *= 0x85EBCA6Bu
    x ^= x >> 13u
    x *= 0xC2B2AE35u
    x ^= x >> 16u
    return x
}

def private hashed(i : uint; amp : float) : float => (float(hash32(i) & 0xFFFFu) / 32768.0 - 1.0) * amp

[export, cold_path]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<Args>), "cuda_fa_probe")
        return 0
    }
    if (!cud_init() || !ensure_fa_mma_h512_cls() || !ensure_fa_khr_h512_cls()) {
        print("no CUDA device, or a flash stamp refused\n")
        return 1
    }
    let rows = a.rows
    let qd = a.heads * HS
    var q : array<float>
    q |> reserve_resize(int64(rows) * int64(qd))
    for (i in range64(long_length(q))) {
        q[i] = hashed(uint(i), 0.5)
    }
    var kv : array<float16>
    kv |> reserve_resize(int64(rows + SLACK_ROWS) * int64(HS))
    for (i in range64(long_length(kv))) {
        kv[i] = float16(i < int64(rows) * int64(HS) ? hashed(uint(i) + 0x9000001u, 0.4) : 0.0)
    }
    var s = g_cud.stream
    let dq = buf_upload(q, s)
    let dkv = buf_upload(kv, s)
    let obytes = uint64(int64(rows) * int64(qd) * 4l)
    let o_wide = cuda_mem_alloc(obytes)
    let o_khr = cuda_mem_alloc(obytes)
    delete q
    delete kv
    var pc = FaArgs(rows = uint(rows), w0 = 0u, qd = uint(qd), kvd = uint(HS), kv_mul = uint(a.heads), kvbase = 0u, scale = 0.05,
        qstride = uint(qd), qhs = uint(HS), window = 0u, cap = 0.0, sinkoff = uint(a.mask), row0 = 0u, ring = 0u)
    let ma = double(rows) * double(rows + 1) * 0.5lf * double(a.heads) * double(2 * HS)
    let us_wide = cuda_time_best(a.reps) $(_i) {
        enc_fa_mma_h512_cls(s, fixed_array(dq, dkv, dkv, o_wide), pc, cud_fa_wide_wgs(pc))
    }
    print("FA rows {rows} heads {a.heads} mask {a.mask}: wide {us_wide / 1000.0lf:.3f} ms, {ma / us_wide / 1.0e6lf:.1f} T multiply-adds a second\n")
    if (!a.wide_only) {
        var pk = pc
        pk.sinkoff = 0u
        let us_khr = cuda_time_best(a.reps) $(_i) {
            enc_fa_khr_h512_cls(s, fixed_array(dq, dkv, dkv, o_khr), pk, fa_stamp_wgs(true, int64(a.heads), int64(rows)))
        }
        print("FA rows {rows} heads {a.heads}: KHR arm {us_khr / 1000.0lf:.3f} ms, {ma / us_khr / 1.0e6lf:.1f} T multiply-adds a second\n")
        if (a.mask == 0) {   // the last row of each plane: the row that attended every key
            var hw, hk : array<float>
            hw |> resize(qd)
            hk |> resize(qd)
            let last = uint64(int64(rows - 1) * int64(qd) * 4l)
            cuda_memcpy_dtoh(unsafe(addr<void?>(hw[0])), o_wide + last, uint64(qd * 4), s)
            cuda_memcpy_dtoh(unsafe(addr<void?>(hk[0])), o_khr + last, uint64(qd * 4), s)
            cud_sync()
            var worst = 0.0
            var peak = 0.0
            for (x, y in hw, hk) {
                worst = max(worst, abs(x - y))
                peak = max(peak, abs(y))
            }
            print("FA rows {rows}: the last row, wide against the KHR arm: widest difference {worst} of a peak {peak}\n")
            delete hw
            delete hk
        }
    }
    cuda_mem_free(dq)
    cuda_mem_free(dkv)
    cuda_mem_free(o_wide)
    cuda_mem_free(o_khr)
    return 0
}
