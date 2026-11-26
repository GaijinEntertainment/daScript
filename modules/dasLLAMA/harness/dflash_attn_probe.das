options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// ATTRIBUTION SWEEP: the block drafter's attention pair - the key-tile kernel and its merge - timed on the CUDA home over
// synthetic planes at a drafter's geometry (rings of halves), no model loaded and no output compared: a launch a layer over the layers' rings
// in turn, at each ring fill asked for - microseconds a launch, the best of five chains of `--reps` launches after one
// warm chain it drops, and a pass's layers in milliseconds.
//
//   DASLLAMA_GPU=1 DASLLAMA_CUDA=1 bin/daslang -jit modules/dasLLAMA/harness/dflash_attn_probe.das -- [--rows 8]
//       [--heads 32] [--kv 8] [--fills 0,512,2048]

require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes_dflash   // the stamps
require dasllama/dasllama_gpu_kernels_common   // GkDflashAttnArgs, gk_dflash_attn_tiles, gk_dflash_attn_tile_wgs, gk_dflash_attn_partials
require daslib/clargs
require daslib/f16_cvt   // f32_to_f16: the rings are halves
require _cuda_time   // cuda_time_best
require _race_fixture   // race_fill
require _prompt_files   // id_list
require math

[CommandLineArgs]
struct Args {
    @clarg_doc = "Query heads, a whole multiple of --kv"
    heads : int = 32
    @clarg_doc = "K/V heads"
    kv : int = 8
    @clarg_doc = "Head size, even and no wider than the tile kernel stages (256)"
    hs : int = 128
    @clarg_doc = "The drafter's window, positions a ring"
    window : int = 2048
    @clarg_doc = "Layers, a ring each"
    layers : int = 5
    @clarg_doc = "The block's rows"
    rows : int = 8
    @clarg_doc = "Ring fills (context positions in view), comma-separated"
    fills : string = "0,256,512,1024,2048"
    @clarg_doc = "Dispatches a timed chain"
    reps : int = 50
    @clarg_short = "h"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! why the tile kernel cannot run the asked geometry, or "" when it can
def private geometry_refusal(a : Args) : string {
    return "--hs {a.hs}: the tile kernel stages an even head size of 256 at most" if (a.hs < 2 || a.hs > 256 || a.hs % 2 != 0)
    return "--heads {a.heads} is not a whole multiple of --kv {a.kv}" if (a.kv <= 0 || a.heads <= 0 || a.heads % a.kv != 0)
    return "--rows, --window, --layers and --reps are each one at least" if (a.rows < 1 || a.window < 1 || a.layers < 1 || a.reps < 1)
    return ""
}

//! `n` halves off the race's fill
def private halves_fill(n, salt : int; amp : float) : array<uint16> {
    var f <- race_fill(n, salt, amp, 0.0)
    var h <- [for (v in f); uint16(f32_to_f16(v))]
    delete f
    return <- h
}

[export, cold_path]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<Args>), "dflash_attn_probe")
        return 0
    }
    let refusal = geometry_refusal(a)
    if (!empty(refusal)) {
        print("error: {refusal}\n")
        return 2
    }
    if (!cud_init() || !ensure_dflash_attn_tile_cls() || !ensure_dflash_attn_merge_cls()) {
        print("no CUDA device, or a stamp did not compile\n")
        return 1
    }
    let rows = int64(a.rows)
    let heads = int64(a.heads)
    let hs = int64(a.hs)
    let grp = heads / int64(a.kv)
    let kvd = int64(a.kv) * hs
    let qd = a.heads * a.hs
    let w = int64(a.window)
    let layers = int64(a.layers)
    var s = g_cud.stream
    var q_host <- race_fill(a.rows * qd, 7, 0.4, 0.0)
    var kb_host <- race_fill(int(rows * kvd), 11, 0.4, 0.0)
    var vb_host <- race_fill(int(rows * kvd), 13, 0.6, 0.0)
    var rk_host <- halves_fill(int(layers * w * kvd), 17, 0.4)
    var rv_host <- halves_fill(int(layers * w * kvd), 19, 0.6)
    let dq = buf_upload(q_host, s)
    let dkb = buf_upload(kb_host, s)
    let dvb = buf_upload(vb_host, s)
    let drk = buf_upload(rk_host, s)
    let drv = buf_upload(rv_host, s)
    delete q_host
    delete kb_host
    delete vb_host
    delete rk_host
    delete rv_host
    let dtp = buf_fill(int(gk_dflash_attn_partials(rows, heads, hs)), 0.0, s)
    let dout = buf_fill(a.rows * qd, 0.0, s)
    let ring_mb = double(w * kvd) * 2.0lf / 1048576.0lf
    print("{rows} rows of {heads} heads of {hs} over {a.kv} K/V heads, a window of {w}: {layers} layers' rings, {ring_mb:.1f} MB of K and of V a layer in halves\n")
    var measured = 0
    for (nctx in id_list(a.fills, 0l, w)) {
        let tiles = gk_dflash_attn_tiles(nctx + rows)
        var pa = GkDflashAttnArgs(rows = uint(rows), pos0 = uint(nctx), lo = 0u, nctx = uint(nctx), w = uint(w), hs = uint(hs),
            nheads = uint(heads), grp = uint(grp), scale = 1.0 / sqrt(float(hs)), rbase = 0u, koff = 0u, voff = 0u)
        let wgs = gk_dflash_attn_tile_wgs(rows, heads, grp, tiles)
        let us_tile = cuda_time_best(a.reps) $(i) {
            let lbase = uint(int64(i) % layers * w * kvd)
            pa.koff = lbase
            pa.voff = lbase
            enc_dflash_attn_tile_cls(s, fixed_array(dq, dkb, dvb, drk, drv, dtp), pa, wgs)
        }
        let us_merge = cuda_time_best(a.reps) $(_i) {
            enc_dflash_attn_merge_cls(s, fixed_array(dtp, dout), pa, heads * rows)
        }
        let pass_ms = (us_tile + us_merge) * double(layers) / 1000.0lf
        print("fill {nctx}: {tiles} tile(s), {wgs} tile workgroups: the tile {us_tile:.1f} us + the merge {us_merge:.1f} us a layer, {pass_ms:.3f} ms a pass of {layers} layers timing-only\n")
        measured++
    }
    for (b in [dq, dkb, dvb, drk, drv, dtp, dout]) {
        cuda_mem_free(b)
    }
    if (measured == 0) {
        print("no fill of --fills {a.fills} lies in 0 .. {w}: nothing measured\n")
        return 1
    }
    return 0
}
