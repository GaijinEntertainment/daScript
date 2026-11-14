options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The recurrent rows' kernels timed on the CUDA home over synthetic planes at a hybrid's geometry: the fused step a row a
// dispatch, and the window's conv, scan (per token and chunked), gated out-norm and ring tail over the same row counts -
// microseconds, the best of five chains.
//
//   DASLLAMA_GPU=1 DASLLAMA_CUDA=1 bin/daslang -jit modules/dasLLAMA/harness/cuda_dn_probe.das -- [--nvh 48] [--rows 1,2,4,8,16]

require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_common
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_gpu_kernels_common   // GkDnGateArgs
require dasllama/dasllama_gpu_kernels_discrete   // DnStepArgs, DnScanArgs, DnCommitArgs, TokMeta
require dasllama/dasllama_math   // reserve_resize
require dasllama/dasllama_gpu_math   // dn_scan_wgs, dn_conv_wgs
require daslib/clargs
require _cuda_time   // cuda_time_best
require _prompt_files   // id_list
require math

[CommandLineArgs]
struct Args {
    @clarg_doc = "Value heads"
    nvh : int = 48
    @clarg_doc = "Key heads"
    nkh : int = 16
    @clarg_doc = "Row counts, comma-separated, 1 to 16"
    rows : string = "1,2,4,8,16"
    @clarg_doc = "Dispatches a timed chain"
    reps : int = 48
    @clarg_short = "h"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private fill(n : int64; scale, off : float; seed : uint) : array<float> {
    var a : array<float>
    a |> reserve_resize(n)
    var x = seed
    for (i in range64(n)) {
        x = x * 1664525u + 1013904223u
        a[i] = (float(x >> 8u) / 16777216.0 - 0.5) * scale + off
    }
    return <- a
}

[export, cold_path]
def main : int {   // nolint:STYLE038 — one linear measurement sweep
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help) {
        print_help(get_command_info(type<Args>), "cuda_dn_probe")
        return 0
    }
    if (!cud_init() || !ensure_dn_scan_cls() || !ensure_dn_scan_p3_cls() || !ensure_dn_tail_cls() || !ensure_dn_step_cls()) {
        print("no CUDA device, or a stamp did not compile\n")
        return 1
    }
    let ds = 128
    let nvh = a.nvh
    let nkh = a.nkh
    let kd = nkh * ds
    let cd = 2 * kd + nvh * ds
    let di = nvh * ds
    let dconv = 4
    let taps = dconv - 1
    let maxr = 16
    let beta_off = 16
    let g_off = beta_off + maxr * nvh
    let a_off = g_off + maxr * nvh
    let dt_off = a_off + nvh
    let taps_off = dt_off + nvh
    let wnorm_off = taps_off + cd * dconv
    let hist_off = wnorm_off + ds
    let sm_n = hist_off + 2 * cd * taps
    var s = g_cud.stream
    var projh <- fill(int64(maxr) * int64(cd + di), 0.3, 0.0, 7u)
    var sth <- fill(int64(nvh) * int64(ds * ds), 0.04, 0.0, 11u)
    var smh <- fill(int64(sm_n), 0.4, 0.0, 13u)
    for (i in range(nvh)) {
        smh[a_off + i] = -0.4
    }
    var tok <- [for (_i in range(maxr)); TokMeta(parity = 1u, dnslot = 0u)]
    var nocap <- [0u, 0u]
    let dproj = buf_upload(projh, s)
    let dst = buf_upload(sth, s)
    let dsm = buf_upload(smh, s)
    let dos = buf_fill(maxr * di / 32, 0.0, s)
    let doq = buf_fill(maxr * di / 4, 0u, s)
    let dtok = buf_upload(tok, s)
    let dof = buf_fill(maxr * di, 0.0, s)
    let dcap = buf_upload(nocap, s)
    let dcs = buf_fill(16, 0.0, s)
    let dconvp = buf_fill(maxr * cd, 0.0, s)
    let chunk_ok = ensure_dn_scan_chunk_cls() && ensure_dn_conv_cls()
    print("{nvh} value heads of {ds} over {nkh} key heads: a state of {double(nvh * ds * ds) * 4.0lf / 1048576.0lf:.1f} MB\n")
    for (nr64 in id_list(a.rows, 1l, int64(maxr))) {
        let nr = int(nr64)
        var pc = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh), ds = uint(ds), dconv = uint(dconv),
            beta_off = uint(beta_off), g_off = uint(g_off), taps_off = uint(taps_off), wnorm_off = uint(wnorm_off),
            a_off = uint(a_off), dt_off = uint(dt_off), hist_off = uint(hist_off), eps = 0.00001, qscale = 0.088,
            out_f32 = 0u, beta_row_stride = uint(nvh), row0 = 0u)
        let us_step = cuda_time_best(a.reps) $(_i) {   // the fused step a row a dispatch: what the rows cost stepped in turn
            for (i in range(nr)) {
                pc.row0 = uint(i)
                enc_dn_step_cls(s, fixed_array(dproj, dst, dsm, dos, doq, dtok, dof), pc, int64(nvh))
            }
        }
        var pcs = DnScanArgs(npos = uint(nr), cd = uint(cd), kd = uint(kd), nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
            beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off), dt_off = uint(dt_off), o_off = 0u, di = uint(di))
        var pc3 = GkDnGateArgs(nvh = uint(nvh), di = uint(di), ds = uint(ds), o_off = 0u, wnorm_off = uint(wnorm_off), eps = 0.00001)
        let us_scan = cuda_time_best(a.reps) $(_i) {
            enc_dn_scan_cls(s, fixed_array(dproj, dst, dsm, dof, dcap, dcs), pcs, dn_scan_wgs(int64(nvh), int64(ds)))
        }
        var pcv = GkDnConvArgs(npos = uint(nr), cd = uint(cd), kd = uint(kd), ds = uint(ds), dconv = uint(dconv), hist_mode = 1u,
            taps_off = uint(taps_off), hist_off = uint(hist_off), eps = 0.00001, qscale = 0.088)
        let us_conv = cuda_time_best(a.reps) $(_i) {
            enc_dn_conv_cls(s, fixed_array(dproj, dconvp, dsm), pcv, dn_conv_wgs(int64(cd), int64(nr)))
        }
        var us_chunk = -1.0lf
        if (chunk_ok) {
            us_chunk = cuda_time_best(a.reps) $(_i) {
                enc_dn_scan_chunk_cls(s, fixed_array(dproj, dst, dsm, dof, dcap, dcs), pcs, dn_scan_chunk_wgs(int64(nvh), int64(ds), DNC_V))
            }
        }
        let us_p3 = cuda_time_best(a.reps) $(_i) {
            enc_dn_scan_p3_cls(s, fixed_array(dproj, dof, dsm), pc3, int64(nr))
        }
        var pct = DnTailArgs(cd = uint(cd), taps = uint(taps), row0 = uint(max(nr - taps, 0)), hist_off = uint(hist_off),
            old_rows = uint(max(taps - nr, 0)), old_off = uint(hist_off + cd * taps))
        let us_tail = cuda_time_best(a.reps) $(_i) {
            enc_dn_tail_cls(s, fixed_array(dproj, dsm), pct, int64((cd * taps + 255) / 256))
        }
        print("rows {nr}: the fused step a row a dispatch {us_step:.1f} us; the window's conv {us_conv:.1f} us + scan {us_scan:.1f} us (chunked {us_chunk:.1f}) + out-norm {us_p3:.1f} us, its ring tail {us_tail:.1f} us\n")
    }
    for (b in [dproj, dst, dsm, dos, doq, dtok, dof, dcap, dcs, dconvp]) {
        cuda_mem_free(b)
    }
    delete projh
    delete sth
    delete smh
    delete tok
    delete nocap
    return 0
}
