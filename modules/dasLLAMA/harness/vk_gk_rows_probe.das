options gen2
options stack = 524288
options _dasllama_internal = true

//! The row-kernel ruler for the Vulkan stamps of the templates both homes stamp (`dasllama_gpu_kernels_common.das`): each
//! class timed alone at a qwen35moe / qwen4exp decode shape (and the deltanet out-norm at a 512-row window) on the device
//! clock - DISPATCHES back-to-back dispatches a command, SUBMITS submits a round after a warm-up, ROUNDS rounds; a row is the
//! median per-dispatch time with the rounds' cv, and a row past 3% cv is VOID. Its Metal twin is
//! `../benchmarks/ew/bench_metal_gk_rows.das`; the numerics are the kernel cells' (`../tests/test_vulkan_kernels.das`).
//!
//!   bin/daslang -jit modules/dasLLAMA/harness/vk_gk_rows_probe.das

require math
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_kqformat   // KqFmt: the routed down's format
require _vk_probe_fixture   // pfq_arm, pfq_avg_us: the device clock

let private DISPATCHES = 64
let private SUBMITS = 8
let private ROUNDS = 9
let private BUF_BYTES = 64l << 20l

//! the median per-dispatch device time over the rounds of `rec`'s DISPATCHES dispatches, and the rounds' cv
def private time_cls(name : string; rec : block<(raw : VkCommandBuffer; var h : VkHaz) : void>) {
    var raw = alloc_cmd()
    record_cmd(raw, false) $(r; var h) {
        pfq_arm(r)
        pfq_ts(r)
        for (_i in range(DISPATCHES)) {
            invoke(rec, r, h)
            pfq_ts(r)
        }
        pfq_end(r)
    }
    var times <- [for (_r in range(ROUNDS)); pfq_avg_us(raw, SUBMITS, DISPATCHES)]
    var sorted := times
    sorted |> sort()
    var mean = 0.0lf
    for (v in times) {
        mean += v
    }
    mean /= double(ROUNDS)
    var var2 = 0.0lf
    for (v in times) {
        var2 += (v - mean) * (v - mean)
    }
    let cv = sqrt(var2 / double(ROUNDS)) / mean
    print("| {name} | {sorted[ROUNDS / 2]:8.3f} | {cv * 100.0lf:5.2f}%{cv > 0.03lf ? " VOID" : ""} |\n")
}

[export]
def main : int {   // nolint:STYLE038 — one-shot probe: the classes, the fixtures and the timing rounds share every buffer
    let classes_ok = (vk_moe_init() && ensure_hc_norm_cls() && ensure_hc_mix_cls() && ensure_hc_comb_cls() && ensure_ple_gate_cls()
        && ensure_ple_conv_cls() && ensure_qk_rms_cls() && ensure_topk_n_cls() && ensure_topk_n2_cls() && ensure_combine_cls()
        && ensure_dn_scan_p3_cls() && ensure_qkn_rope_f16_cls() && ensure_cls_epi() && ensure_cls_ar_comb_g4()
        && ensure_router_gemv_cls() && ensure_router_gemv_bf16_cls() && ensure_cls_ar() && ensure_cls_ar_rq_b()
        && ensure_ple_act_f32_cls() && ensure_ple_finish_cls())
    if (!classes_ok) {
        print("vk_gk_rows_probe: no Vulkan device, or a class declined\n")
        return 1
    }
    let dim = 2048
    let hc = 4
    let hcd = hc * dim
    var b : array<uint64>
    b |> reserve(6)
    for (_i in range(6)) {
        let d = make_device_buf(BUF_BYTES)
        upload_zeros(d, 0l, BUF_BYTES)
        b |> push(d)
    }
    var tokb = make_host_buf(TOK_META_BYTES * 8l, true)
    unsafe {
        var tm = reinterpret<TokMeta?>(tokb.mapped)
        tm[0].pos = 17u
        tm[0].cnt = 18u
    }
    let bb = BUF_BYTES
    print("vk_gk_rows_probe: {DISPATCHES} dispatches x {SUBMITS} submits x {ROUNDS} rounds, {g_gpu.sm_count} SMs\n")
    print("| kernel | us/dispatch | cv |\n")
    print("| --- | ---: | ---: |\n")
    var s_norm = set_hc_norm_cls(fixed_array(b[0], b[1], b[2]), fixed_array(bb, bb, bb), fixed_array(1u, 2u, 0u))
    time_cls("hc_norm dim {dim} hc {hc} 1 row") $(r; var h) {
        enc_hc_norm_cls(r, h, s_norm, GkHcNormArgs(dim = uint(dim), hc = uint(hc), woff = 0u, row0 = 0u, eps = 1e-6), int64(hc))
    }
    var s_mix = set_hc_mix_cls(fixed_array(b[0], b[1], b[2]), fixed_array(bb, bb, bb), fixed_array(1u, 2u, 4u))
    time_cls("hc_mix dim {dim} hc {hc} 1 row") $(r; var h) {
        enc_hc_mix_cls(r, h, s_mix, GkHcMixArgs(dim = uint(dim), hc = uint(hc), rows = 1u), int64((dim + 255) / 256))
    }
    var s_comb = set_hc_comb_cls(fixed_array(b[0], b[1], b[2], b[3], b[4]), fixed_array(bb, bb, bb, bb, bb), fixed_array(1u, 2u, 4u, 8u, 16u))
    time_cls("hc_comb dim {dim} hc {hc} 1 row, MoE") $(r; var h) {
        enc_hc_comb_cls(r, h, s_comb, GkHcCombArgs(dim = uint(dim), hc = uint(hc), moe_on = 1u, sh_on = 1u, gated = 1u, rows = 1u, lstride = 257u),
            int64((hcd + 255) / 256))
    }
    var s_pg = set_ple_gate_cls(fixed_array(b[0], b[1], b[2], b[3], b[4], b[5], tokb.buf, b[4]), fixed_array(bb, bb, bb, bb, bb, bb, TOK_META_BYTES * 8l, bb),
        fixed_array(1u, 2u, 4u, 8u, 16u, 0u, 0u, 16u))
    time_cls("ple_gate dim {dim} hc {hc} the token row") $(r; var h) {
        enc_ple_gate_cls(r, h, s_pg, GkPleGateArgs(dim = uint(dim), hc = uint(hc), nrows = 10u, eps = 1e-6, inv_sq = 1.0 / sqrt(float(dim))), int64(hc))
    }
    var s_pc = set_ple_conv_cls(fixed_array(b[0], b[1], b[2], b[3], tokb.buf, b[2]), fixed_array(bb, bb, bb, bb, TOK_META_BYTES * 8l, bb),
        fixed_array(1u, 2u, 4u, 0u, 0u, 4u))
    time_cls("ple_conv dim {dim} hc {hc} the token row") $(r; var h) {
        enc_ple_conv_cls(r, h, s_pc, GkPleConvArgs(dim = uint(dim), hc = uint(hc), kern = 4u, dil = 3u), int64((hcd + 255) / 256))
    }
    var s_qk = set_qk_rms_cls(fixed_array(b[0], b[1], b[2]), fixed_array(bb, bb, bb), fixed_array(1u, 2u, 0u))
    time_cls("qk_rms hs 256 q 16 k 2 1 row") $(r; var h) {
        enc_qk_rms_cls(r, h, s_qk, GkQkRmsArgs(hs = 256u, nh = 16u, nkvh = 2u, qstride = 4096u, kstride = 512u, qwoff = 0u, kwoff = 256u, eps = 1e-6),
            18l)
    }
    var s_tk = set_topk_n_cls(fixed_array(b[0], b[1], b[2], b[3], b[4], b[5], b[5], b[1]), fixed_array(bb, bb, bb, bb, bb, bb, bb, bb),
        fixed_array(1u, 2u, 4u, 8u, 16u, 0u, 0u, 32u))
    time_cls("topk_n ne 256 k 8 1 row") $(r; var h) {
        enc_topk_n_cls(r, h, s_tk, GkTopKArgs(ne = 256u, k = 8u, norm = 1u, lstride = 256u, dsoff = GK_NO_DSCALE), 1l)
    }
    time_cls("topk_n ne 128 k 8 1 row, the down scales (gemma-4)") $(r; var h) {
        enc_topk_n_cls(r, h, s_tk, GkTopKArgs(ne = 128u, k = 8u, norm = 1u, lstride = 128u, dsoff = 0u), 1l)
    }
    time_cls("topk_n2 ne 512 k 10 1 row") $(r; var h) {   //! the family's one set
        enc_topk_n2_cls(r, h, s_tk, GkTopKArgs(ne = 512u, k = 10u, norm = 1u, lstride = 512u, dsoff = GK_NO_DSCALE), 1l)
    }
    var s_cb = set_combine_cls(fixed_array(b[0], b[1], b[2], b[3]), fixed_array(bb, bb, bb, bb), fixed_array(1u, 2u, 4u, 8u))
    time_cls("combine dim {dim} k 8 1 row") $(r; var h) {
        enc_combine_cls(r, h, s_cb, GkMoeCombArgs(n = uint(dim), k = 8u, r0 = 0u, r1 = 0xFFFFFFFFu), 1l)
    }
    var s_dn = set_dn_scan_p3_cls(fixed_array(b[0], b[1], b[2]), fixed_array(bb, bb, bb), fixed_array(1u, 2u, 0u))
    time_cls("dn_scan_p3 nvh 32 ds 128 1 row") $(r; var h) {
        enc_dn_scan_p3_cls(r, h, s_dn, GkDnGateArgs(nvh = 32u, di = 4096u, ds = 128u, eps = 1e-6), 1l)
    }
    time_cls("dn_scan_p3 nvh 32 ds 128 512 rows") $(r; var h) {
        enc_dn_scan_p3_cls(r, h, s_dn, GkDnGateArgs(nvh = 32u, di = 4096u, ds = 128u, eps = 1e-6), 512l)
    }
    var s_qkn = set_qkn_rope_f16_cls(fixed_array(b[0], b[1], b[2], b[3], b[4], b[5], tokb.buf, b[5], b[1]),
        fixed_array(bb, bb, bb, bb, bb, bb, TOK_META_BYTES * 8l, bb, bb), fixed_array(1u, 0u, 2u, 4u, 0u, 0u, 0u, 0u, 0u))
    let qkn = GkQknRopeArgs(qd = 4096u, kvd = 1024u, hs = 128u, half = 64u, neox = 1u, nh = 32u, kwoff = 128u, eps = 1e-6,
        kvrow = 6144u, qrow = 6144u, cosrow = 128u)
    time_cls("qkn_rope f16 hs 128 q 32 k 8 1 row") $(r; var h) {
        var q1 = qkn
        enc_qkn_rope_f16_cls(r, h, s_qkn, q1, 40l)
    }
    time_cls("qkn_rope f16 hs 128 q 32 k 8 4 rows") $(r; var h) {
        var q4 = qkn
        q4.rowwg = 40u
        enc_qkn_rope_f16_cls(r, h, s_qkn, q4, 160l)
    }
    let g4dim = 2816u
    var s_g4 = set_cls_ar_comb_g4(fixed_array(b[0], b[1], b[2], b[3], b[4], b[5], b[0]), fixed_array(bb, bb, bb, bb, bb, bb, bb),
        fixed_array(1u, 0u, 2u, 4u, 8u, 0u, 16u))
    time_cls("gemma-4 MoE step dim {g4dim} k 8 1 row") $(r; var h) {
        enc_cls_ar_comb_g4(r, h, s_g4, GkMoeCombArgs(n = g4dim, k = 8u, eps = 1e-6, ascale = 0.75, rpost_off = 0u, dpost_off = g4dim,
            spost_off = 2u * g4dim, nwoff = 3u * g4dim), 1l)
    }
    let vocab = 262144l
    var s_epi = set_cls_epi(fixed_array(b[0], b[1]), fixed_array(bb, bb), fixed_array(1u, 0u))
    for (nr in fixed_array(1l, 4l)) {
        time_cls("cls epilogue vocab {vocab} cap 30 two pins {nr} rows") $(r; var h) {
            enc_cls_epi(r, h, s_epi, GkClsEpiArgs(vocab = uint(vocab), cap = 30.0, nsupp = 2u, rows = uint(nr)), (vocab * nr + 255l) / 256l)
        }
    }
    //! the residual step at the gemma-3-12b and E4B widths: the plain add + norm, the sandwich seam (the branch under its post norm,
    //! the add, the next norm) and the seam quantizing its norm, at a decode row and four rows
    var s_ar = set_cls_ar(fixed_array(b[0], b[1], b[2], b[3]), fixed_array(bb, bb, bb, bb), fixed_array(1u, 2u, 0u, 4u))
    var s_arq = set_cls_ar_rq_b(fixed_array(b[0], b[1], b[2], b[3], b[4]), fixed_array(bb, bb, bb, bb, bb), fixed_array(1u, 2u, 0u, 4u, 8u))
    for (adim in fixed_array(2560u, 3840u)) {
        let plain = GkAddRmsArgs(dim = adim, add_on = 1u, woff = 0u, eps = 1e-6, ascale = 1.0)
        let sand = GkAddRmsArgs(dim = adim, add_on = 1u, woff = 0u, eps = 1e-6, ascale = 0.75, pre_on = 1u, pwoff = adim)
        for (nr in fixed_array(1l, 4l)) {
            time_cls("ar dim {int(adim)} {nr} rows: add + norm") $(r; var h) {
                var pc = plain
                enc_cls_ar(r, h, s_ar, pc, nr)
            }
            time_cls("ar dim {int(adim)} {nr} rows: sandwich seam") $(r; var h) {
                var pc = sand
                enc_cls_ar(r, h, s_ar, pc, nr)
            }
            time_cls("ar dim {int(adim)} {nr} rows: sandwich seam, Q8_0 out") $(r; var h) {
                var pc = sand
                enc_cls_ar_rq_b(r, h, s_arq, pc, nr)
            }
        }
    }
    //! the gemma-4 26B's routed down at its decode shape (704 -> 2816, k 8, Q5_1, every slot its own expert): the summed GEMV both
    //! commands fold under the routing weights, at one, two and four token rows, against the per-slot leaf the combine sums
    let dn = 704
    let dd = 2816
    let dk = 8
    let dnb = dn / 32
    let q51 = int(KqFmt.q51)
    if (gemv_shell_ensure(q51, GEMV_SHELL_SUM) && gemv_cls_ensure(q51)) {
        var regs <- [for (i in range(8 * dk)); uint(i % 2 == 0 ? (i / 2) * dd * dnb : (i / 2) * dnb)]
        upload_region_at(b[2], 0l, unsafe(addr<uint8 const?>(regs[0])), long_length(regs) * 4l)
        var s_sum = set_q51_gemv_sum_cls(fixed_array(b[0], b[1], b[2], b[3], b[4], b[5], b[1]), fixed_array(bb, bb, bb, bb, bb, bb, bb),
            fixed_array(0u, 0u, 0u, 0u, 0u, 1u, 0u))
        var s_leaf = gemv_cls_set(q51, fixed_array(b[0], b[1], b[2], b[3], b[4], b[5]), fixed_array(bb, bb, bb, bb, bb, bb), fixed_array(0u, 0u, 0u, 0u, 0u, 1u))
        for (nr in fixed_array(1, 2, 4)) {
            time_cls("down q51 {dn} -> {dd} k {dk} summed, {nr} rows") $(r; var h) {
                var pc = GemvArgs(n = uint(dn), d = uint(dd), nreg = uint(dk * nr), ncols = uint(nr))
                gemv_shell_enc(q51, GEMV_SHELL_SUM, r, h, s_sum, pc, gemv_wgs(int64(dd * nr), 0u))
            }
            time_cls("down q51 {dn} -> {dd} k {dk} a slot at a time, {nr} rows") $(r; var h) {
                var pc = GemvArgs(n = uint(dn), d = uint(dd), nreg = uint(dk * nr))
                gemv_enc(q51, r, h, s_leaf, pc, int64(dk * dd * nr))
            }
        }
        delete regs
    }
    //! the E-series side rows' projection at the E2B shape (35 layers x 256 rows of 1536): the f32 rows against the bf16 word form
    let pne = 35 * 256
    let pdim = 1536
    var s_rf = set_router_gemv_cls(fixed_array(b[0], b[1], b[2], b[3]), fixed_array(bb, bb, bb, bb), fixed_array(0u, 1u, 2u, 0u))
    var s_rb = set_router_gemv_bf16_cls(fixed_array(b[0], b[1], b[2], b[3]), fixed_array(bb, bb, bb, bb), fixed_array(0u, 1u, 2u, 0u))
    for (np in fixed_array(1u, 4u)) {
        let pr = GkRouterArgs(ne = uint(pne), dim = uint(pdim), ncols = np, yscale = 1.0 / sqrt(float(pdim)))
        time_cls("ple proj f32 rows ne {pne} dim {pdim} {np} cols") $(r; var h) {
            var p1 = pr
            router_enc_cols(false, r, h, s_rf, p1, int64(pne))
        }
        time_cls("ple proj bf16 rows ne {pne} dim {pdim} {np} cols") $(r; var h) {
            var p1 = pr
            router_enc_cols(true, r, h, s_rb, p1, int64(pne))
        }
    }
    //! the E-series side rows at the E2B shape (35 layers x 256): the act over a layer's side slice and the pre-step's finish
    let ple = 256
    let pnl = 35
    var s_pa = set_ple_act_f32_cls(fixed_array(b[0], b[1], b[2]), fixed_array(bb, bb, bb), fixed_array(1u, 2u, 4u))
    var s_pf = set_ple_finish_cls(fixed_array(b[1], b[2], b[0]), fixed_array(bb, bb, bb), fixed_array(2u, 0u, 1u))
    for (np in fixed_array(1, 512)) {
        time_cls("ple act ple {ple} nl {pnl} {np} rows") $(r; var h) {
            var pc = GkPleActArgs(nelem = uint(np * ple), sbase = uint(7 * ple), sstride = uint(pnl * ple), slen = uint(ple))
            enc_ple_act_f32_cls(r, h, s_pa, pc, int64((np * ple + 255) / 256))
        }
        time_cls("ple finish ple {ple} nl {pnl} {np} rows") $(r; var h) {
            var pc = GkPleFinArgs(ple = uint(ple), eps = 1e-6)
            enc_ple_finish_cls(r, h, s_pf, pc, int64(pnl * np))
        }
    }
    return 0
}
