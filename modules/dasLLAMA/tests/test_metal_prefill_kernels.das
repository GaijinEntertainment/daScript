options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/math_bits           // float<->uint bits — the moe_wscale sel-buffer pun
require dasllama/dasllama_math
require dasllama/dasllama_convert  // used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require dasllama/dasllama_gemm_schema  // kq_qsb — the ONE per-format quant-plane stride source
require dasllama/dasllama_kqformat  // KqFmt: the expert GEMV and per-32 mul_mm cells' format key
require ?das_metal dasllama/dasllama_metal_prefill  // the Phase-6 prefill kernel set (Apple builds)
require ?das_metal dasllama/dasllama_metal_kernels  // the Wave C MoE routing kernel set (Apple builds)
require ?das_metal dasllama/dasllama_metal_common   // the pipeline globals the generated builders read
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // the MetalCell record, check_buf, the device shell, fixtures
require daslib/f16_cvt  // f32_to_f16: the q51 gates hand-pack f16 d/m scale halfwords
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_gpu_math   // nolint:STYLE030,LINT019 — tts_tanh, tts_hash_normal, sigmoid_f32: the StyleTTS2 gates' CPU forms, inside the Apple half alone
require dasllama/dasllama_tower
require dasllama/dasllama_gemma4a   // gemma4a_attn_scales - the family's attention scales the RPE cells feed
require dasllama/dasllama_moe
require dasllama/dasllama_common
require dasllama/dasllama_rope   // build_rope_tabs - the Pk rope gates' tables
require _attn_oracle
require _compares   // Bar, Shape: the cells' bars; mismatch_rel, span_maxdiff
require _kernel_oracles   // padded_bases: the MoE route's padded-CSR prefix the GEMM cells read too; rope_tab_ref
require _tts_kernel_oracles   // the StyleTTS2 cells' fixtures and double forms the Vulkan TTS cells share
require daslib/jobque_boost
require math

// The Phase-6 GPU prefill kernels vs their dasLLAMA CPU twins on synthetic data. rmsnorm /
// rope / swiglu / attention compare within the shared fp tolerance (reduction order, hardware
// exp vs the CPU's exp/exp4 — all inside 1e-4 relative); the residual add is bit-exact (one
// fadd both sides); the blob GEMV gets exact-arithmetic data (pow2 scales). Without das_metal in
// the build, or with no Metal device, every check skips; a Metal-4 twin leg also skips without
// the Metal-4 tensor toolchain.

// attention shape: GQA with 2 queries per kv head, small enough to hand-oracle; NPOS is
// deliberately NOT a 32-multiple (np32 = 64) so the padded-slab rails are exercised
let NPOS = 48
let NP32 = 64                           // ceil32(NPOS) — the padded slab / buffer row count
let HEAD_SIZE = 32
let N_HEADS = 4
let N_KV_HEADS = 2
let KV_MUL = N_HEADS / N_KV_HEADS
let QD = N_HEADS * HEAD_SIZE            // 128
let KV_DIM = N_KV_HEADS * HEAD_SIZE     // 64

// rmsnorm / swiglu / add shapes
let NROWS = 6
let DIM = 128
let HIDDEN = 64
let EPS = 1.0e-5

let private BAR_FP = Bar(rel = 1e-4, poison_scaled = true)   //! the shared fp bar (fapprox); the poisoned last element moves 0.5 of its magnitude floored at 1

//! GENERIC (untyped dev/queue) so it instantiates only inside the Apple static_if.
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-twin-dual-store")]
def private qk_rope_gate(t : T?; var dev, queue; hx, donorm, neox, partial : bool) {
    let tag = "qk_rope{hx ? " hx" : ""}{donorm ? " norm" : ""}{neox ? " neox" : ""}{partial ? " part" : ""}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kr = kernel_of(@@enc_qk_rope)
        let krh = kernel_of(@@enc_qk_rope_hx)
        var pso = hx ? (mc |> keep_stamp(krh, err)) : (mc |> keep_stamp(kr, err))
        return if (!pso_ok(t, tag, pso, err))
        let nreal = 6
        let npad = hx ? 2 : 0
        let nrows = nreal + npad
        let hs = HEAD_SIZE
        let nh = N_HEADS
        let rstride = nh * hs
        let rot = partial ? hs / 2 : hs
        let halfr = rot / 2
        var inscope xv, wv, tc, ts, want : array<float>
        xv |> resize(nrows * rstride)
        want |> resize(nrows * rstride)
        wv |> resize(hs)
        tc |> resize(nreal * halfr)
        ts |> resize(nreal * halfr)
        for (k in range(nrows * rstride)) {
            xv[k] = 0.19 * float(k % 29) - 1.7
        }
        for (k in range(hs)) {
            wv[k] = 0.8 + 0.05 * float(k % 7)
        }
        for (k in range(nreal * halfr)) {
            tc[k] = cos(0.03 * float(k % 83))
            ts[k] = sin(0.03 * float(k % 83))
        }
        for (k in range(nrows * rstride)) {
            want[k] = xv[k]
        }
        unsafe {
            for (p in range(nreal)) {
                if (donorm) {
                    for (h in range(nh)) {
                        let rb = p * rstride + h * hs
                        rmsnorm(addr(want[rb]), addr<float const?>(want[rb]), addr<float const?>(wv[0]), int64(hs), EPS)
                    }
                }
                rope_tab_ref(want, p * rstride, hs, rot, rstride, neox, tc, ts, p * halfr)
            }
        }
        var moved = 0
        for (k in range(nreal * rstride)) {
            moved += fapprox(want[k], xv[k]) ? 0 : 1
        }
        t |> success(moved > 0, "{tag}: oracle is not the identity ({moved} moved)")
        let bx = mc |> io_(xv)
        let bw = mc |> in_(wv)
        let btc = mc |> in_(tc)
        let bts = mc |> in_(ts)
        let bxh = hx ? (mc |> out_words(nrows * rstride, float16(-77.0))) : -1
        var ka = QkRopeArgs(rstride = uint(rstride), hs = uint(hs), eps = EPS,
            donorm = donorm ? 1u : 0u, neox = neox || partial ? 1u : 0u, rot = uint(rot), nreal = uint(nreal), hscale = 1.0)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (hx) {
                invoke(krh.enc, enc, pso, mc.buf[bx], 0ul, mc.buf[bw], mc.buf[btc], mc.buf[bts], ka, mc.buf[bxh], int64(nh), int64(nrows), int64(hs / 2))
            } else {
                invoke(kr.enc, enc, pso, mc.buf[bx], 0ul, mc.buf[bw], mc.buf[btc], mc.buf[bts], ka, int64(nh), int64(nrows), int64(hs / 2))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_FP, Shape(row_width = rstride, head_width = hs))
        if (hx) {
            check_hx_narrow(t, "{tag} f16 twin", mc.buf[bxh], mc.buf[bx], nrows * rstride, nrows * rstride)
        }
    }
}

//! the key range each trio query row attends and its score transforms: `uend` 0 = causal; `uend` > 0 = the queries at
//! [ulo, uend) attend up to uend (a uniform span of media rows, the fused eval_embd_span_ contract) while the rest stay
//! causal - the legacy whole-eval span is ulo 0, uend qoff + npos; `window` > 0 = a sliding layer's floor, query-based
//! even for span rows; `qoff` = the continuation's existing context rows
struct private AttnKeys {
    qoff : int
    window : int
    ulo : int
    uend : int
    softcap : float
    hass : int
}

//! The sgmat attention trio (QK^T -> rowstat -> P.V) vs `attn_row_oracle` over the driver's padded rows: queries to 32, keys (context + chunk) to 64.
def private attn_trio_gate(t : T?; var dev, queue; head_size, n_heads, n_kv_heads, npos : int; mm : bool; keys : AttnKeys = AttnKeys()) {
    let kv_mul = n_heads / n_kv_heads
    let qd = n_heads * head_size
    let kv_dim = n_kv_heads * head_size
    let nk = keys.qoff + npos
    let nkp = (nk + 63) / 64 * 64           // 64-padded key extent (the driver's nk64)
    let qp = (npos + 31) / 32 * 32          // padded query rows (continuation: != nkp)
    let tag = "attention trio (hs={head_size}, mm={mm}, qoff={keys.qoff}, w={keys.window}, span=[{keys.ulo},{keys.uend}), cap={keys.softcap}, sink={keys.hass})"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let ksc = mm ? kernel_of(@@enc_qk_mm_c) : kernel_of(@@enc_qk)
        let ksm = kernel_of(@@enc_rowstat)
        let kav = mm ? kernel_of(@@enc_av_mm_c) : kernel_of(@@enc_av)
        var pso_sc = mc |> keep_stamp(ksc, err)
        let sc_ok = pso_ok(t, "{tag} qk", pso_sc, err)
        var pso_sm = mc |> keep_stamp(ksm, err)
        let sm_ok = pso_ok(t, "{tag} rowstat", pso_sm, err)
        var pso_av = mc |> keep_stamp(kav, err)
        return if (!pso_ok(t, "{tag} av", pso_av, err) || !sc_ok || !sm_ok)
        let scale = 1.0 / sqrt(float(head_size))
        var inscope aq, ak, av, axb_want : array<float>
        aq |> resize(qp * qd)
        ak |> resize(nkp * kv_dim)
        av |> resize(nkp * kv_dim)
        axb_want |> resize(npos * qd)
        for (k in range(npos * qd)) {
            aq[k] = 0.13 * float(k % 31) - 1.9
        }
        for (k in range(nk * kv_dim)) {
            ak[k] = 0.17 * float(k % 23) - 1.8
            av[k] = 0.09 * float(k % 37) - 1.6
        }
        var inscope sinks : array<float>
        sinks |> resize(n_heads)
        for (h in range(n_heads)) {
            sinks[h] = 0.4 * float(h) - 0.2   // near the score range so the sink genuinely moves sums
        }
        for (h in range(n_heads)) {
            for (qi in range(npos)) {
                let ap = keys.qoff + qi
                let jlo = keys.window > 0 && ap + 1 > keys.window ? ap + 1 - keys.window : 0
                let jhi = keys.uend > 0 && ap >= keys.ulo && ap < keys.uend ? keys.uend : ap + 1
                attn_row_oracle(aq, qi * qd + h * head_size, ak, 0, av, 0, kv_dim, h, head_size, kv_mul, jhi, scale,
                    axb_want, qi * qd + h * head_size, jlo, keys.softcap, keys.hass != 0, sinks[h])
            }
        }
        let baq = mc |> in_(aq)
        let bak = mc |> in_(ak)
        let bav = mc |> in_(av)
        // +1000 poison: production leaves QK-skipped slab positions as pool garbage — a mask hole
        // in rowstat/pf_p_weight EXPLODES through exp() and reds the compare (0 or -1000 would hide)
        let batt = mc |> out_words(n_heads * qp * nkp, float16(1000.0))
        let bstat = mc |> out_(n_heads * qp * 2)
        let bxb = mc |> out_(qp * qd)
        var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kv_dim), head_size = uint(head_size),
            kv_mul = uint(kv_mul), npos = uint(nk), np32 = uint(nkp), scale = scale,
            qoff = uint(keys.qoff), qrows = uint(qp), window = uint(keys.window), softcap = keys.softcap, hass = uint(keys.hass),
            uend = uint(keys.uend), ulo = uint(keys.ulo))
        let bcap0 = keys.hass != 0 ? (mc |> in_(sinks)) : (mc |> in_plain([0.0]))   // the sink plane (hass = 0: never read)
        let aran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(ksc.enc, enc, pso_sc, mc.buf[baq], mc.buf[bak], mc.buf[batt], ka, int64(qp), int64(nkp), int64(n_heads))
            invoke(ksm.enc, enc, pso_sm, mc.buf[batt], ka, mc.buf[bstat], mc.buf[bcap0], int64(npos), int64(n_heads))   // sink identity (hass = 0)
            invoke(kav.enc, enc, pso_av, mc.buf[batt], mc.buf[bav], mc.buf[bxb], ka, mc.buf[bstat], int64(qp), int64(n_heads), int64(head_size))
        }
        return if (!aran)
        // the f16 score slab + half staging — the envelope is the half-precision score shift
        // through softmax, ~1% of the convex-combination output (both variants)
        check_buf(t, "{tag} f16 score slab", mc.buf[bxb], axb_want, Bar(rel = 2e-2, poison_scaled = true), Shape(row_width = qd, head_width = head_size))
    }
}

//! the f16-exact q / k / v rows of `n` elements the tower and conformer attention cells share
def private f16_qkv_rows(n : int) : tuple<q : array<float>; k : array<float>; v : array<float>> {
    var q, k, v : array<float>
    q |> resize(n)
    k |> resize(n)
    v |> resize(n)
    for (i in range(n)) {
        q[i] = float(float16(0.11 * float(i % 37) - 1.9))
        k[i] = float(float16(0.07 * float((i * 13) % 41) - 1.3))
        v[i] = float(float16(0.05 * float((i * 7) % 53) - 1.1))
    }
    return <- (q = q, k = k, v = v)
}

// ===== the tower flash (the dk72 lifted flash + its per-head-contiguous f16 K/V restride) =====
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
def private tower_flash_kvhc_gate(t : T?; var dev, queue) {
    let tag = "tower_flash"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kf = kernel_of(@@enc_tower_flash)
        var pso = mc |> keep_stamp(kf, err)
        return if (!pso_ok(t, tag, pso, err))
        let kkv = kernel_of(@@enc_tower_kv_hc)
        var pso_hc = mc |> keep_stamp(kkv, err)
        return if (!pso_ok(t, "{tag} kv_hc", pso_hc, err))
        let np = 128
        let heads = 2
        let hs = 72
        let qd = heads * hs
        var inscope(qv, kv, vv) <- f16_qkv_rows(np * qd)
        // the SECOND 64-key block carries every row's running max (x4, a power of two, so the keys
        // stay f16-exact): with a uniform K the max settles in block 0 and the flash's cross-block
        // rescale multiplies by 1, which no compare can witness
        for (i in range(64 * qd, np * qd)) {
            kv[i] = kv[i] * 4.0
        }
        var inscope khc, vhc : array<float16>
        khc |> resize(np * qd)
        vhc |> resize(np * qd + 256)   //! + the flash's V tail slack: its dead PV tiles read past the last V row
        for (p in range(np)) {
            for (c in range(qd)) {
                khc[((c / hs) * np + p) * hs + c % hs] = float16(kv[p * qd + c])
                vhc[((c / hs) * np + p) * hs + c % hs] = float16(vv[p * qd + c])
            }
        }
        var inscope want : array<float>
        want |> resize(np * qd)
        let scale = 1.0 / sqrt(float(hs))
        var inscope hp, att : array<float>
        with_dasllama_jobque_() {
            attention_bidir(want, qv, kv, vv, hp, att, int64(np), int64(qd), int64(heads), scale)
        }
        let bq = mc |> in_(qv)
        let bkc = mc |> in_(kv)
        let bvc = mc |> in_(vv)
        let bkh = mc |> out_words(np * qd, float16(-77.0))
        let bvh = mc |> out_words(np * qd + 256, float16(-77.0))
        let bo = mc |> out_words(np * qd, -1.0e30)
        let bu_hs = mc |> in_plain([uint(hs)])
        let bu_d = mc |> in_plain([uint(qd)])
        let bu_np = mc |> in_plain([uint(np)])
        let bu_tot = mc |> in_plain([uint(np * qd)])
        var ka = AttnArgs(qd = uint(qd), kv_dim = uint(qd), head_size = uint(hs), kv_mul = 1u,
            npos = uint(np), np32 = uint(np), scale = scale,
            qoff = 0u, qrows = uint(np), window = 0u, softcap = 0.0, hass = 0u, uend = uint(np))
        var inscope vslice <- copy_span(vhc, 0, np * qd)
        for (leg in range(2)) {
            if (leg == 1) {
                vv[(np / 2) * qd + hs / 2] += 3.0
                mc |> plane_upload(bvc, vv)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(kkv.enc, enc, pso_hc, mc.buf[bkc], mc.buf[bkh], mc.buf[bu_hs], mc.buf[bu_d], mc.buf[bu_np], mc.buf[bu_tot], int64(np * qd))
                invoke(kkv.enc, enc, pso_hc, mc.buf[bvc], mc.buf[bvh], mc.buf[bu_hs], mc.buf[bu_d], mc.buf[bu_np], mc.buf[bu_tot], int64(np * qd))
                invoke(kf.enc, enc, pso, mc.buf[bq], mc.buf[bkh], mc.buf[bvh], mc.buf[bo], ka, int64(np), int64(heads))
            }
            return if (!ran)
            if (leg == 0) {
                check_buf_h(t, "{tag} kv_hc K panel", mc.buf[bkh], [for (h in khc); float(h)], BAR_EXACT, FLAT_SHAPE)
                check_buf_h(t, "{tag} kv_hc V panel", mc.buf[bvh], [for (h in vslice); float(h)], BAR_EXACT, FLAT_SHAPE)
                check_buf(t, tag, mc.buf[bo], want, Bar(rel = 1e-3, abs_floor = 1e-4, poison_scaled = true), Shape(row_width = qd, head_width = hs))
            } else {
                var inscope got <- mc |> plane_run(bo)
                t |> success(mismatch_rel("{tag} poison (MUST mismatch)", got, want, 1e-3, 1e-4) > 0,
                    "{tag}: a poisoned V element reds the bars through kv_hc + flash")
            }
        }
    }
}

// ===== the qwen2.5 ViT per-window attention (block-diagonal, ragged last window) =====
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
def private tower_win_gate(t : T?; var dev, queue) {
    let tag = "tower_win"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kw = kernel_of(@@enc_tower_win_attn)
        var pso = mc |> keep_stamp(kw, err)
        return if (!pso_ok(t, tag, pso, err))
        let np = 160
        let heads = 2
        let hs = 80
        let qd = heads * hs
        let wlo <- [ 0u, 64u, 128u, 160u ]   //! two full windows + a ragged 32-row tail
        let nwin = 3
        var inscope(qv, kv, vv) <- f16_qkv_rows(np * qd)
        var inscope want : array<float>
        want |> resize(np * qd)
        let scale = 1.0 / sqrt(float(hs))
        let win_lo <- [for (w in wlo); int64(w)]
        var inscope hp, att : array<float>
        with_dasllama_jobque_() {
            attention_bidir_windows(want, qv, kv, vv, hp, att, win_lo, int64(qd), int64(heads), scale)
        }
        let bq = mc |> in_(qv)
        let bk = mc |> in_(kv)
        let bw = mc |> in_plain(wlo)
        let bo = mc |> out_words(np * qd, -1.0e30)
        let bv = mc |> in_(vv)
        var ka = GkWinAttnArgs(nheads = uint(heads), hs = uint(hs), scale = scale)
        for (leg in range(2)) {
            if (leg == 1) {
                vv[80 * qd + hs / 2] += 3.0
                mc |> plane_upload(bv, vv)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(kw.enc, enc, pso, mc.buf[bq], mc.buf[bk], mc.buf[bv], mc.buf[bo], mc.buf[bw], ka, int64(nwin * heads))
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[bo], want, Bar(rel = 1e-3, abs_floor = 1e-4, poison_scaled = true), Shape(row_width = qd, head_width = hs))
            } else {
                var inscope got <- mc |> plane_run(bo)
                t |> success(mismatch_rel("{tag} poison (MUST mismatch)", got, want, 1e-3, 1e-4) > 0,
                    "{tag}: a poisoned V element reds the bars")
            }
        }
    }
}

struct private FcFixture {
    np : int = 24
    heads : int = 2
    hs : int = 128
    d : int = 256
    mp : int
    nkp : int
    ww : int
    wwpad : int
}

//! the fixture's pads come from the driver's own derivation, so a lattice change moves both sides
def private fc_fixture(dev; np : int = 24) : FcFixture {
    var f = FcFixture(np = np)
    let ka = fc_attn_args(int64(f.d), int64(f.hs), int64(f.np))
    f.mp = int(ka.mp)
    f.nkp = int(ka.nkp)
    f.ww = int(ka.ww)
    f.wwpad = int(ka.wwpad)
    return f
}

def private fc_args(dev; f : FcFixture) => fc_attn_args(int64(f.d), int64(f.hs), int64(f.np))

//! the mismatches of the first length(want) floats of `buf` - halves widened to float under `half` - against `want` at `bar_ok`'s rel and floor - a poisoned input's rerun must have some
def private buf_mismatches(buf; want : array<float>; rel, abs_floor : float; tag : string; half : bool = false) : int {
    if (half) {
        var inscope halves <- buf_download(buf, length(want), type<float16>)
        var inscope widened <- [for (h in halves); float(h)]
        return mismatch_rel(tag, widened, want, rel, abs_floor)
    }
    var inscope got <- buf_download(buf, length(want), type<float>)
    return mismatch_rel(tag, got, want, rel, abs_floor)
}

//! `buf_mismatches` over plane `p` of `mc`
def private rerun_mismatches(mc; p : int; want : array<float>; rel, abs_floor : float; tag : string; half : bool = false) : int => buf_mismatches(mc.buf[p], want, rel, abs_floor, tag, half)

let private BAR_ZERO = Bar(rel = 0.0)   //! a copy's bar: no difference at all, a NaN off

//! The operand stamps against the host's five panels at no difference at all: the per-head f32 pack (head 1, V transposed)
//! a copy, and under `half` the all-heads half operands the RNE narrow of the same f32 sum or product - one add or one
//! multiply a value, so the device rounds as the host does. One poisoned input per rerun, each panel its own control.
def private fc_operands_gate(t : T?; var dev, queue; half : bool) {
    let tag = half ? "fc_twin" : "fc_pack"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kpack = kernel_of(@@enc_fc_pack)
        let ktwin = kernel_of(@@enc_fc_twin)
        var pso = half ? (mc |> keep_stamp(ktwin, err)) : (mc |> keep_stamp(kpack, err))
        return if (!pso_ok(t, tag, pso, err))
        let f = fc_fixture(dev)
        let h = 1
        let w = half ? f.d : f.hs
        var inscope(qv, kv, vv) <- f16_qkv_rows(f.np * f.d)
        var inscope pl <- [for (i in range(f.ww * f.d)); float(float16(0.03 * float((i * 11) % 29) - 0.4))]
        var inscope bu <- [for (i in range(f.d)); float(float16(0.02 * float(i % 19) - 0.15))]
        var inscope bvv <- [for (i in range(f.d)); float(float16(0.015 * float((i * 3) % 23) - 0.11))]
        var ka = fc_args(dev, f)
        //! the oracle's five panels: the resize's zeros are the expected pad rows
        var inscope want : array<array<float>>
        for (rows in [f.mp, f.mp, f.nkp, f.nkp, f.wwpad]) {
            var inscope panel : array<float>
            panel |> resize(rows * w)
            want |> emplace(panel)
        }
        for (r in range(f.np)) {
            for (j in range(w)) {
                let c = half ? j : h * f.hs + j
                let s = r * f.d + c
                let i = r * w + j
                want[0][i] = half ? float(float16(qv[s] + bu[c])) : qv[s] + bu[c]
                want[1][i] = half ? float(float16(qv[s] + bvv[c])) : qv[s] + bvv[c]
                want[2][i] = half ? float(float16(kv[s] * ka.scale)) : kv[s]
                want[3][half ? i : j * f.nkp + r] = half ? float(float16(vv[s])) : vv[s]
            }
        }
        for (r in range(f.ww)) {
            for (j in range(w)) {
                let s = r * f.d + (half ? j : h * f.hs + j)
                want[4][r * w + j] = half ? float(float16(pl[s] * ka.scale)) : pl[s]
            }
        }
        let names = half ? fixed_array("Q+u", "Q+v", "K", "V", "pos") : fixed_array("Q+u", "Q+v", "K_h", "V_hT", "pos_h")
        let poisons = fixed_array("bias_u", "bias_v", "K", "V", "pos")
        let inputs = fixed_array("content bias", "rel bias", "K element", "V element", "rel-table element")
        let bq = mc |> in_(qv)
        let bk = mc |> in_(kv)
        let bv = mc |> in_(vv)
        let bpl = mc |> in_(pl)
        let bbu = mc |> in_(bu)
        let bbv = mc |> in_(bvv)
        var inscope outs <- [for (p in range(5)); half ? (mc |> out_words(length(want[p]), float16(-77.0))) : (mc |> out_words(length(want[p]), -1.0e30))]
        let bu_h = half ? -1 : (mc |> in_plain([uint(h)]))
        let total = f.wwpad * w
        // one poisoned input per rerun, the previous one's clean plane back first: each compare has its own control
        for (leg in range(6)) {
            if (leg == 1) {
                mc |> plane_upload(bbu, poisoned_copy(bu, h * f.hs + 5, 2.0))
            } elif (leg == 2) {
                mc |> plane_upload(bbu, bu)
                mc |> plane_upload(bbv, poisoned_copy(bvv, h * f.hs + 9, 2.0))
            } elif (leg == 3) {
                mc |> plane_upload(bbv, bvv)
                mc |> plane_upload(bk, poisoned_copy(kv, 3 * f.d + h * f.hs + 1, 2.0))
            } elif (leg == 4) {
                mc |> plane_upload(bk, kv)
                mc |> plane_upload(bv, poisoned_copy(vv, 5 * f.d + h * f.hs + 2, 2.0))
            } elif (leg == 5) {
                mc |> plane_upload(bv, vv)
                mc |> plane_upload(bpl, poisoned_copy(pl, 11 * f.d + h * f.hs + 3, 2.0))
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                if (half) {
                    invoke(ktwin.enc, enc, pso, mc.buf[bq], mc.buf[bk], mc.buf[bv], mc.buf[bpl], mc.buf[bbu], 0ul, mc.buf[bbv], 0ul,
                        mc.buf[outs[0]], mc.buf[outs[1]], mc.buf[outs[2]], mc.buf[outs[3]], mc.buf[outs[4]], ka, int64(total))
                } else {
                    invoke(kpack.enc, enc, pso, mc.buf[bq], mc.buf[bk], mc.buf[bv], mc.buf[bpl], mc.buf[bbu], 0ul, mc.buf[bbv], 0ul,
                        mc.buf[outs[0]], mc.buf[outs[1]], mc.buf[outs[2]], mc.buf[outs[3]], mc.buf[outs[4]], ka, mc.buf[bu_h], int64(total))
                }
            }
            if (leg == 0) {
                return if (!ran)
                for (p in range(5)) {
                    if (half) {
                        check_buf_h(t, "{tag} {names[p]}", mc.buf[outs[p]], want[p], BAR_ZERO, Shape(row_width = f.d, head_width = f.hs))
                    } else {
                        check_buf(t, "{tag} {names[p]}", mc.buf[outs[p]], want[p], BAR_ZERO, Shape(row_width = p == 3 ? f.nkp : f.hs))
                    }
                }
            } elif (ran) {
                let p = leg - 1
                t |> success(mc |> rerun_mismatches(outs[p], want[p], 0.0, 0.0, "{tag} {poisons[p]} poison (MUST mismatch)", half) > 0,
                    "{tag}: a poisoned {inputs[p]} reds the {names[p]} panel")
            }
        }
    }
}

//! np = 24 keeps every live key in simdgroup 0; np = 300 fills every simdgroup and strides each lane twice.
//! The per-head softmax's bar: the kernel sums f32 in a lane-then-simdgroup tree (error under 16 ulp of the sum) with the
//! fast-math exp (2 ulp per term), the oracle sums in double - 1e-5 relative is ~10x that budget,
//! 1e-6 absolute covers the probabilities under 0.1 where the relative bar tightens past it.
//! The `pexp` stamp's bars, over two heads: a weight is the half of a fast-math exp (2 ulp of f32), which can land on the
//! neighbouring half - one half step is 1e-3 relative, 1e-7 absolute covers the subnormal weights; a stat is one over an f32
//! lane-then-simdgroup sum against the oracle's double sum, 1e-5 relative as the per-head softmax's.
def private fc_row_gate(t : T?; var dev, queue; np : int; pexp : bool) {
    let tag = pexp ? "fc_pexp np{np}" : "fc_softmax np{np}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let ksoft = kernel_of(@@enc_fc_softmax)
        let kpexp = kernel_of(@@enc_fc_pexp)
        var pso = pexp ? (mc |> keep_stamp(kpexp, err)) : (mc |> keep_stamp(ksoft, err))
        return if (!pso_ok(t, tag, pso, err))
        let f = fc_fixture(dev, np)
        let heads = pexp ? f.heads : 1
        var inscope sc <- [for (i in range(heads * f.mp * f.nkp));
            float(float16(pexp ? 0.25 * float((i * 7) % 31) - 3.5 : 0.9 * float((i * 7) % 31) - 12.0))]
        var inscope sr <- [for (i in range(heads * f.mp * f.wwpad));
            float(float16(pexp ? 0.2 * float((i * 5) % 37) - 3.3 : 0.7 * float((i * 5) % 37) - 11.0))]
        var ka = fc_args(dev, f)
        let scale = pexp ? 1.0 : ka.scale
        //! the per-head stamp's pad rows and pad columns are the resize's zeros; the `pexp` stamp's keep the scores they held,
        //! and a pad row's stat is 0 (the resize's zeros)
        var inscope want : array<float>
        if (pexp) {
            want := sc
        } else {
            want |> resize(f.mp * f.nkp)
        }
        var inscope wstat : array<float>
        wstat |> resize(heads * f.mp)
        for (h in range(heads)) {
            for (r in range(f.np)) {
                let crow = (h * f.mp + r) * f.nkp
                let rrow = (h * f.mp + r) * f.wwpad + f.np - 1 - r
                var mx = -3.0e38
                for (k in range(f.np)) {
                    mx = max(mx, (sc[crow + k] + sr[rrow + k]) * scale)
                }
                var tot = 0.0lf
                for (k in range(f.np)) {
                    let e = exp((sc[crow + k] + sr[rrow + k]) * scale - mx)
                    want[crow + k] = pexp ? float(float16(e)) : e
                    tot += double(e)
                }
                if (pexp) {
                    wstat[h * f.mp + r] = float(1.0lf / tot)
                } else {
                    for (k in range(f.np)) {
                        want[crow + k] = float(double(want[crow + k]) / tot)
                    }
                }
            }
        }
        var inscope sc16 <- [for (v in sc); float16(v)]
        var inscope sr16 <- [for (v in sr); float16(v)]
        let bsc = pexp ? (mc |> in_plain(sc16)) : (mc |> in_(sc))
        let bsr = pexp ? (mc |> in_plain(sr16)) : (mc |> in_(sr))
        let bout = mc |> out_words(pexp ? heads * f.mp : f.mp * f.nkp, -1.0e30)
        for (leg in range(2)) {
            if (leg == 1) {
                let at = ((heads - 1) * f.mp + 3) * f.wwpad + (f.np - 1 - 3) + 7
                if (pexp) {
                    mc |> plane_upload(bsc, sc16)
                    sr16[at] = float16(float(sr16[at]) + 6.0)
                    mc |> plane_upload(bsr, sr16)
                } else {
                    sr[at] += 6.0
                    mc |> plane_upload(bsr, sr)
                }
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                if (pexp) {
                    invoke(kpexp.enc, enc, pso, mc.buf[bsc], mc.buf[bsr], mc.buf[bout], ka, int64(f.mp), int64(heads))
                } else {
                    invoke(ksoft.enc, enc, pso, mc.buf[bsc], mc.buf[bsr], mc.buf[bout], ka, int64(f.mp))
                }
            }
            return if (!ran)
            if (leg == 0 && pexp) {
                check_buf_h(t, "{tag} weights", mc.buf[bsc], want, Bar(rel = 1.2e-3, abs_floor = 1e-7, poison_scaled = true), Shape(row_width = f.nkp))
                check_buf(t, "{tag} stat", mc.buf[bout], wstat, Bar(rel = 1e-5, abs_floor = 1e-7, poison_scaled = true), Shape(row_width = f.mp))
            } elif (leg == 0) {
                check_buf(t, tag, mc.buf[bout], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = f.nkp))
            } elif (pexp) {
                t |> success(mc |> rerun_mismatches(bsc, want, 1.2e-3, 1e-7, "{tag} rel-score poison (MUST mismatch)", true) > 0,
                    "{tag}: a poisoned rel score reds the weights")
                t |> success(mc |> rerun_mismatches(bout, wstat, 1e-5, 1e-7, "{tag} rel-score poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned rel score reds the stat")
            } else {
                t |> success(mc |> rerun_mismatches(bout, want, 1e-5, 1e-6, "{tag} rel-score poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned rel score reds the bars")
            }
        }
    }
}

def private fc_unpack_gate(t : T?; var dev, queue) {
    let tag = "fc_unpack"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let ku = kernel_of(@@enc_fc_unpack)
        var pso = mc |> keep_stamp(ku, err)
        return if (!pso_ok(t, tag, pso, err))
        let f = fc_fixture(dev)
        var inscope oh : array<float>
        oh |> resize(f.heads * f.mp * f.hs)
        for (i in range(f.heads * f.mp * f.hs)) {
            oh[i] = float(float16(0.05 * float((i * 3) % 41) - 1.0))
        }
        var inscope want : array<float>
        want |> resize(f.mp * f.d)   //! the resize's zeros are the expected pad rows
        for (r in range(f.np)) {
            for (c in range(f.d)) {
                want[r * f.d + c] = oh[((c / f.hs) * f.mp + r) * f.hs + c % f.hs]
            }
        }
        var ka = fc_args(dev, f)
        let bao = mc |> out_words(f.mp * f.d, -1.0e30)
        let total = f.mp * f.d
        let boh = mc |> in_(oh)
        for (leg in range(2)) {
            if (leg == 1) {
                oh[(1 * f.mp + 9) * f.hs + 17] += 2.0
                mc |> plane_upload(boh, oh)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(ku.enc, enc, pso, mc.buf[boh], mc.buf[bao], ka, int64(total))
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[bao], want, BAR_ZERO, Shape(row_width = f.d, head_width = f.hs))
            } else {
                t |> success(mc |> rerun_mismatches(bao, want, 0.0, 0.0, "{tag} poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned AV element reds the compare")
            }
        }
    }
}

//! Bar: nine f16-exact products summed in f32 (under 9 ulp), the folded BN affine and the
//! fast-math exp of the SiLU (2 ulp) against the oracle's f32 chain in the same order - 1e-5
//! relative is ~50x that budget, 1e-6 absolute covers the outputs near zero.
def private fc_dw_gate(t : T?; var dev, queue; tap_major : bool) {
    let tag = tap_major ? "pk_dw" : "cn_dw"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = tap_major ? kernel_of(@@enc_pk_dw) : kernel_of(@@enc_cn_dw)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let np = 13
        let d = 32
        let nk = 9
        let kpad = (nk - 1) / 2
        var inscope xv : array<float>
        xv |> resize(np * d)
        for (i in range(np * d)) {
            xv[i] = float(float16(0.09 * float((i * 5) % 43) - 1.7))
        }
        var inscope wv : array<float>
        wv |> resize(d * nk)
        for (i in range(d * nk)) {
            wv[i] = float(float16(0.06 * float((i * 7) % 31) - 0.8))
        }
        var inscope bnw, bnb : array<float>
        bnw |> resize(d)
        bnb |> resize(d)
        for (i in range(d)) {
            bnw[i] = float(float16(0.8 + 0.03 * float(i % 11)))
            bnb[i] = float(float16(0.01 * float((i * 3) % 17) - 0.07))
        }
        var inscope want : array<float>
        want |> resize(np * d)
        for (tt in range(np)) {
            for (c in range(d)) {
                var acc = 0.0
                for (kk in range(nk)) {
                    let src = tt + kk - kpad
                    if (src >= 0 && src < np) {
                        acc += wv[tap_major ? kk * d + c : c * nk + kk] * xv[src * d + c]
                    }
                }
                let bvv2 = acc * bnw[c] + bnb[c]
                want[tt * d + c] = bvv2 / (1.0 + exp(-bvv2))
            }
        }
        let bx = mc |> in_(xv)
        var inscope wn <- clone_to_move(wv)   // the taps, the folded scale and the folded shift as one weight plane at element offsets
        wn |> push_from(bnw)
        wn |> push_from(bnb)
        let bwn = mc |> in_(wn)
        let by = mc |> out_words(np * d, -1.0e30)
        let total = np * d
        var ka = GkDwConvRowsArgs(c = uint(d), k = uint(nk), t_in = uint(np), woff = 0u, bnwoff = uint(d * nk), bnboff = uint(d * nk + d))
        for (leg in range(2)) {
            if (leg == 1) {
                wn[5 * nk + 3] += 2.0
                mc |> plane_upload(bwn, wn)
                mc |> plane_refill(by)   // the second run's output starts from the sentinel, not the first run's rows
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(kd.enc, enc, pso, mc.buf[bx], mc.buf[by], mc.buf[bwn], ka, int64(total))
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[by], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = d))
            } else {
                t |> success(mc |> rerun_mismatches(by, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned tap reds the bars")
            }
        }
    }
}

//! gemma4a's causal depthwise conv (the shared template's FORM 1 at five taps) against the CPU loop over 13 rows of 32
//! channels: a row reads the four rows before it and nothing before the start. Bar: five f16-exact products summed in
//! f32 against the same order; a poisoned tap must red it.
def private g4a_dw_gate(t : T?; var dev, queue) {
    let tag = "g4a_dw"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_g4a_dw)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let np = 13
        let d = 32
        let nk = 5
        var inscope xv <- f16_fill(np * d, 5, 0.09, -1.7)
        var inscope wv <- f16_fill(d * nk, 7, 0.06, -0.8)
        var inscope want : array<float>
        want |> resize(np * d)
        for (tt in range(np)) {
            for (c in range(d)) {
                var acc = 0.0
                for (kk in range(nk)) {
                    let src = tt + kk - (nk - 1)
                    if (src >= 0) {
                        acc += wv[c * nk + kk] * xv[src * d + c]
                    }
                }
                want[tt * d + c] = acc
            }
        }
        let bx = mc |> in_(xv)
        let bwn = mc |> in_(wv)
        let by = mc |> out_words(np * d, -1.0e30)
        let total = np * d
        var ka = GkDwConvRowsArgs(c = uint(d), t_in = uint(np), woff = 0u)
        for (leg in range(2)) {
            if (leg == 1) {
                wv[5 * nk + 3] += 2.0
                mc |> plane_upload(bwn, wv)
                mc |> plane_refill(by)   // the second run's output starts from the sentinel, not the first run's rows
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(kd.enc, enc, pso, mc.buf[bx], mc.buf[by], mc.buf[bwn], ka, int64(total))
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[by], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = d))
            } else {
                t |> success(mc |> rerun_mismatches(by, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned tap reds the bars")
            }
        }
    }
}

//! The conv module's GLU on the shared gated-rows template, gemma4a's and the FastConformers' stamp: the first half of a
//! 2 d row gated by the sigmoid of its second, against the CPU form at BAR_APPROX (the kernel's f32 exp against the CPU's: a
//! few ulp of the sigmoid, far under the bar), the halves swapped as the control that must miss.
def private g4a_glu_gate(t : T?; var dev, queue) {
    let tag = "g4a_glu"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_g4a_glu)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let rows = 7
        let d = 70
        let total = rows * d
        var inscope h <- [for (i in range(2 * total)); float((i * 37) % 101 - 50) * 0.043]
        var inscope want, swapped : array<float>
        for (i in range(total)) {
            let a = double(h[(i / d) * 2 * d + i % d])
            let g = double(h[(i / d) * 2 * d + d + i % d])
            want |> push(float(a / (1.0lf + exp(-g))))
            swapped |> push(float(g / (1.0lf + exp(-a))))
        }
        var apart = 0
        for (a, b in want, swapped) {
            apart += abs(a - b) > 1e-3 ? 1 : 0
        }
        t |> success(apart > total / 2, "{tag}: the fixture tells the halves apart ({apart} of {total})")
        var ka = GkGatedRowsArgs(d = uint(d), nelem = uint(total))
        let bh = mc |> in_(h)
        let by = mc |> out_words(total, -1.0e30)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[bh], mc.buf[by], ka, int64(total))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_APPROX, Shape(row_width = d))
    }
}

//! The subsample convs' LayerNorm + ReLU on the shared template, gemma4a's stamp: the padded conv rows in at ldin, compact
//! rows out, against a double-precision form at BAR_APPROX (the f32 mean and variance over cout terms round at cout ulp,
//! under 1e-5 of the row's scale), the weight row scaled as the control that must miss.
def private g4a_lnrelu_gate(t : T?; var dev, queue) {
    let tag = "g4a_lnrelu"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_g4a_lnrelu)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let rows = 9
        let cout = 70
        let ldin = 128
        let eps = 1e-5
        var inscope x <- [for (i in range(rows * ldin)); float((i * 37) % 101 - 50) * 0.043]
        var inscope w <- [for (i in range(cout)); 0.5 + float((i * 5) % 11) * 0.17]
        var inscope want : array<float>
        want |> resize(rows * cout)
        for (r in range(rows)) {
            var mean = 0.0lf
            for (c in range(cout)) {
                mean += double(x[r * ldin + c])
            }
            mean /= double(cout)
            var vv = 0.0lf
            for (c in range(cout)) {
                let dv = double(x[r * ldin + c]) - mean
                vv += dv * dv
            }
            let inv = 1.0lf / sqrt(vv / double(cout) + double(eps))
            for (c in range(cout)) {
                want[r * cout + c] = float(max((double(x[r * ldin + c]) - mean) * inv * double(w[c]), 0.0lf))
            }
        }
        var ka = GkLnReluRowsArgs(cout = uint(cout), ldin = uint(ldin), ldout = uint(cout), rows = uint(rows), woff = 0u, eps = eps)
        let bx = mc |> in_(x)
        let bw = mc |> in_(w)
        let by = mc |> out_words(rows * cout, -1.0e30)
        for (leg in range(2)) {
            if (leg == 1) {
                for (c in range(cout)) {
                    w[c] *= 1.5
                }
                mc |> plane_upload(bw, w)
                mc |> plane_refill(by)   // the second run's output starts from the sentinel, not the first run's rows
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(kd.enc, enc, pso, mc.buf[bx], mc.buf[by], mc.buf[bw], ka, int64(rows))
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[by], want, BAR_APPROX, Shape(row_width = cout))
            } else {
                t |> success(mc |> rerun_mismatches(by, want, 1e-5, 1e-6, "{tag} scaled weight (MUST mismatch)") > 0, "{tag}: a scaled weight row reds the bars")
            }
        }
    }
}

//! The qwen3a feature shuffle on the shared template's f32 stamp: feat[ci x 13 + ow][h + 16 c] off a row-padded, channel-padded
//! conv3 slab against the in-test map, bit for bit; a chunk stride off the compact one is the control the compact form must miss.
def private q3a_shuffle_gate(t : T?; var dev, queue) {
    let tag = "q3a_shuffle"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_q3a_shuffle)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let nch = 6
        let ncp = 8
        let n_chunks = 2
        let mp3 = 224      // a chunk's rows padded past 16 x 13
        let npos = n_chunks * 13
        let featw = 16 * nch
        var inscope y3 <- [for (i in range(n_chunks * mp3 * ncp)); float(i % 97) * 0.125 - 6.0]
        var inscope want : array<float>
        want |> resize(npos * featw)
        var inscope compact : array<float>
        compact |> resize(npos * featw)
        for (gid in range(npos * featw)) {
            let row = gid / featw
            let j = gid % featw
            let h = j % 16
            let c = j / 16
            let ci = row / 13
            let ow = row % 13
            want[gid] = y3[ci * mp3 * ncp + (h * 13 + ow) * ncp + c]
            compact[gid] = y3[ci * 16 * 13 * ncp + (h * 13 + ow) * ncp + c]
        }
        var apart = 0
        for (a, b in want, compact) {
            apart += a != b ? 1 : 0
        }
        t |> success(apart > npos * featw / 4, "{tag}: the padded chunk stride tells the forms apart ({apart} of {npos * featw})")
        var ka = GkQ3aShuffleArgs(nch = uint(nch), ncp = uint(ncp), chunk_stride = uint(mp3 * ncp), npos = uint(npos))
        let by3 = mc |> in_(y3)
        let bfeat = mc |> out_words(npos * featw, -1.0e30)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[by3], mc.buf[bfeat], ka, int64(npos * featw))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bfeat], want, BAR_ZERO, Shape(row_width = featw))
    }
}

//! The FastConformers' rel-position table on the device, on the shared template: the 2 tt - 1 rows against the CPU
//! `rel_pos_table` at an fp32-exact bar, the rows past them zero under a sentinel fill; the table of one frame more as the
//! control that must miss.
def private tw_rel_sincos_gate(t : T?; var dev, queue) {
    let tag = "tw_rel_sincos"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_tw_rel_sincos)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let d = 64
        let tt = 23
        let ww = 2 * tt - 1
        let rows_pad = 48
        var inscope want : array<float>
        rel_pos_table(want, int64(d), int64(tt), int64(ww))
        want |> resize(rows_pad * d)   // the resize's zeros are the expected pad rows
        var inscope other : array<float>
        rel_pos_table(other, int64(d), int64(tt + 1), int64(ww + 2))
        other |> resize(rows_pad * d)
        var ka = GkRelSinCosArgs(d = uint(d), tt = uint(tt), rows_pad = uint(rows_pad))
        let btab = mc |> out_words(rows_pad * d, -1.0e30)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[btab], ka, int64(rows_pad * d))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[btab], want, Bar(rel = 1e-5, abs_floor = 1e-5, poison_scaled = true), Shape(row_width = d))
        t |> success(mc |> rerun_mismatches(btab, other, 1e-5, 1e-5, "{tag} one frame more (MUST mismatch)") > 0, "{tag}: the table of one frame more misses")
    }
}


//! The fronts' first conv off the one-channel mel as the chain the drivers run - the shared im2col at one channel, the f16-staged
//! GEMM over the [ch][32] tap panel, the bias-and-ReLU rows - against the CPU loop on an f16-exact fixture (the fixture's
//! outputs take both signs).
def private pk_conv0_gate(t : T?; var dev, queue) {
    let tag = "pk_conv0"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sim = stamp_of(@@enc_im2col2d)
        let smm = stamp_of(@@enc_f32_mm)
        let sb = stamp_of(@@enc_bias_act)
        var pim = mc |> keep_stamp(sim, err)
        return if (!pso_ok(t, "{tag} im2col", pim, err))
        var pmm = mc |> keep_stamp(smm, err)
        return if (!pso_ok(t, "{tag} gemm", pmm, err))
        var pb = mc |> keep_stamp(sb, err)
        return if (!pso_ok(t, "{tag} bias", pb, err))
        let ih = 11
        let iw = 10
        let ch = 64
        let kdp = 32
        let ka = pk_front_args(int64(iw), int64(ih), int64(ch))
        let oh = int(ka.oh)
        let ow = int(ka.ow)
        t |> equal(oh * 10 + ow, 65, "{tag}: the image halves to 6 x 5, the odd axis rounding up")
        let rows = oh * ow
        let mp = (rows + 31) / 32 * 32
        let lut = fixed_array(0.25, 0.5, 1.0, 2.0, -0.25, -0.5, -1.0, -2.0)   // every product a multiple of 1/16 under 4: nine of them sum exactly in half
        var inscope xv <- [for (i in range(ih * iw)); lut[(i * 5 + 1) % 8]]
        var inscope wv <- [for (i in range(9 * ch)); lut[(i * 7 + 3) % 8]]
        var inscope bv <- [for (i in range(ch)); 0.25 * float(i % 5 - 2)]
        var inscope want <- dw_s2_front_ref(true, true, iw, ch, oh, ow, ih, xv, wv, bv)
        var inscope want_no_relu <- dw_s2_front_ref(true, false, iw, ch, oh, ow, ih, xv, wv, bv)   //! the same sums with no ReLU: how many fall below zero
        var below = 0
        for (v in want_no_relu) {
            below += v < 0.0 ? 1 : 0
        }
        t |> success(below > 8 && below < rows * ch - 8, "{tag}: the fixture's sums take both signs ({below} below zero), so the ReLU shows")
        var inscope panel <- [for (i in range(ch * kdp)); i % kdp < 9 ? wv[(i % kdp) * ch + i / kdp] : 0.0]   // [ch][kdp] off the tap-major taps
        let bx = mc |> in_(xv)
        let bw = mc |> in_(panel)
        let bb = mc |> in_(bv)
        let bcol = mc |> out_words(mp * kdp, -222.0)
        let by = mc |> out_words(mp * ch, -1.0e30)
        let bk = mc |> in_plain([uint(kdp)])
        let bn = mc |> in_plain([uint(ch)])
        var kim = GkIm2col2dArgs(iw = uint(iw), ih = uint(ih), cin = 1u, sstride = 1u, kdp = uint(kdp), rows = uint(rows), soff = 0u)
        var kb = GkBiasActArgs(d = uint(ch), nelem = uint(rows * ch), boff = 0u, act = BIAS_ACT_RELU)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(sim, pim) {
                invoke(sim.enc, enc, mc.buf[bx], mc.buf[bcol], kim, int64(rows * kdp))
            }
            stamp_run(smm, pmm) {
                invoke(smm.enc, enc, mc.buf[bw], 0ul, mc.buf[bcol], mc.buf[by], 0ul, mc.buf[bk], mc.buf[bn], int64(mp), int64(ch))
            }
            stamp_run(sb, pb) {
                invoke(sb.enc, enc, mc.buf[by], mc.buf[bb], kb, int64((rows * ch + 3) / 4))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = ch))
    }
}

//! The subsample depthwise conv on the shared template, both Metal stamps - parakeet's over the tap-major [9][ch] taps and
//! canary's over the file's [ch][9] ones - against the CPU form, then the length mask: the rows past `ih_valid` add nothing.
def private pk_dw2d_gate(t : T?; var dev, queue) {
    let tag = "pk_dw2d"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_pk_dw2d)
        let kcn = kernel_of(@@enc_cn_dw2d)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        var pso_cn = mc |> keep_stamp(kcn, err)
        return if (!pso_ok(t, "cn_dw2d", pso_cn, err))
        let ih = 11
        let iw = 10
        let ch = 8
        let valid = 7
        let oh = int(conv_s2_out(int64(ih)))
        let ow = int(conv_s2_out(int64(iw)))
        t |> equal(oh * 10 + ow, 65, "{tag}: the image halves to 6 x 5, the odd axis rounding up")
        let total = oh * ow * ch
        var inscope xv <- [for (i in range(ih * iw * ch)); float(float16(0.11 * float((i * 5) % 37) - 2.0))]
        var inscope wv <- [for (i in range(9 * ch)); float(float16(0.07 * float((i * 7) % 29) - 1.0))]
        var inscope bv <- [for (i in range(ch)); float(float16(0.3 * float(i % 5) - 0.6))]
        var inscope want <- dw_s2_front_ref(false, false, iw, ch, oh, ow, ih, xv, wv, bv)
        var inscope masked <- dw_s2_front_ref(false, false, iw, ch, oh, ow, valid, xv, wv, bv)
        var inscope wn <- clone_to_move(wv)
        wn |> push_from(bv)
        var inscope wcn : array<float>
        wcn |> resize(9 * ch)
        for (c in range(ch)) {
            for (k9 in range(9)) {
                wcn[c * 9 + k9] = wv[k9 * ch + c]
            }
        }
        wcn |> push_from(bv)
        let bx = mc |> in_(xv)
        let bwn = mc |> in_(wn)
        let bwcn = mc |> in_(wcn)
        let by = mc |> out_words(total, -1.0e30)
        let bycn = mc |> out_words(total, -1.0e30)
        var da = GkDwConv2dArgs(iw = uint(iw), ih = uint(ih), ih_valid = uint(ih), ch = uint(ch), woff = 0u, boff = uint(9 * ch))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[bx], mc.buf[by], mc.buf[bwn], da, int64(total))
            invoke(kcn.enc, enc, pso_cn, mc.buf[bx], mc.buf[bycn], mc.buf[bwcn], da, int64(total))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = ch))
        check_buf(t, "cn_dw2d", mc.buf[bycn], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = ch))
        var dm = GkDwConv2dArgs(iw = uint(iw), ih = uint(ih), ih_valid = uint(valid), ch = uint(ch), woff = 0u, boff = uint(9 * ch))
        mc |> plane_refill(by)   // the mask run's output starts from the sentinel, not the unmasked rows
        let ran_m = mc |> cell_record(t, "pk_dw2d_mask") $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[bx], mc.buf[by], mc.buf[bwn], dm, int64(total))
        }
        return if (!ran_m)
        check_buf(t, "pk_dw2d_mask", mc.buf[by], masked, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = ch))
        t |> success(mc |> rerun_mismatches(by, want, 1e-5, 1e-6, "pk_dw2d_mask vs unmasked (MUST mismatch)") > 0, "pk_dw2d: the mask moves the rows past it")
    }
}

//! The front's feature rows: the last stage's [oh x ow x ch] rows permuted to [fr + ow * c], the rows past oh zero
//! under a sentinel fill - a copy, bit for bit.
def private pk_xf_gate(t : T?; var dev, queue) {
    let tag = "pk_xf"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_pk_xf)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let oh = 5
        let ow = 3
        let ch = 8
        let mp = 8
        let feat = ow * ch
        var inscope yv <- [for (i in range(oh * ow * ch)); float(i) * 0.25 - 7.0]
        var inscope want : array<float>
        want |> resize(mp * feat)   //! the resize's zeros are the expected pad rows
        for (tt in range(oh)) {
            for (fr in range(ow)) {
                for (c in range(ch)) {
                    want[tt * feat + fr + ow * c] = yv[(tt * ow + fr) * ch + c]
                }
            }
        }
        var ka = GkFeatShuffleArgs(w = uint(ow), ch = uint(ch), t = uint(oh), rows_pad = uint(mp))
        let by = mc |> in_(yv)
        let bxf = mc |> out_words(mp * feat, -1.0e30)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[by], mc.buf[bxf], ka, int64(mp * feat))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bxf], want, BAR_ZERO, Shape(row_width = feat))
    }
}

//! Canary's log and per-feature normalization, in place over the mel sums: log(sum + floor), the mean and the Bessel
//! deviation over the valid frames in double, every valid frame normalized and the frames past them zero; rows past
//! the mel keep their sentinel.
def private cn_melnorm_gate(t : T?; var dev, queue) {
    let tag = "cn_melnorm"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_cn_melnorm)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let nmel = 70      // off the 64-thread group: the last group's guard runs
        let nlen = 11
        let valid = 7
        let rows = 13      // two rows past the mel: untouched
        let mfloor = 5.960464477539063e-08
        var inscope x <- [for (i in range(rows * nmel)); 0.001 + float((i * 37) % 101) * 0.37]
        var inscope want := x
        for (j in range(nmel)) {
            var sum = 0.0lf
            for (i in range(valid)) {
                sum += log(double(x[i * nmel + j]) + double(mfloor))
            }
            let mean = sum / double(valid)
            var sq = 0.0lf
            for (i in range(valid)) {
                let dv = log(double(x[i * nmel + j]) + double(mfloor)) - mean
                sq += dv * dv
            }
            let denom = sqrt(sq / double(valid - 1)) + 1e-5lf
            for (i in range(nlen)) {
                want[i * nmel + j] = i < valid ? float((log(double(x[i * nmel + j]) + double(mfloor)) - mean) / denom) : 0.0
            }
        }
        var ka = GkMelNormArgs(n_mel = uint(nmel), n_frames = uint(nlen), valid = uint(valid), mfloor = mfloor)
        let bx = mc |> io_(x)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[bx], ka, int64(nmel))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_APPROX, Shape(row_width = nmel))
    }
}

//! The whisper-class projector tails' two row kernels at a width off the 64-thread group: the pair pool against the
//! mean of each row pair, the row-split gate against silu(second half) * first half - the fixture's halves differ, so
//! the gate with its halves swapped misses on most elements (asserted on the host).
def private tw_tail_rows_gate(t : T?; var dev, queue) {
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kp = kernel_of(@@enc_tw_pool2)
        let kg = kernel_of(@@enc_tw_swiglu_rows)
        var pso_p = mc |> keep_stamp(kp, err)
        return if (!pso_ok(t, "tw_pool2", pso_p, err))
        var pso_g = mc |> keep_stamp(kg, err)
        return if (!pso_ok(t, "tw_swiglu_rows", pso_g, err))
        let width = 70
        let rows = 5
        let total = rows * width
        var inscope x <- [for (i in range(2 * total)); float((i * 37) % 101 - 50) * 0.043]
        var inscope pooled <- [for (i in range(total)); 0.5 * (x[(i / width) * 2 * width + i % width] + x[(i / width) * 2 * width + width + i % width])]
        var inscope gated, swapped : array<float>
        for (i in range(total)) {
            let a = double(x[(i / width) * 2 * width + i % width])
            let g = double(x[(i / width) * 2 * width + width + i % width])
            gated |> push(float(g / (1.0lf + exp(-g)) * a))
            swapped |> push(float(a / (1.0lf + exp(-a)) * g))
        }
        var apart = 0
        for (a, b in gated, swapped) {
            apart += abs(a - b) > 1e-3 ? 1 : 0
        }
        t |> success(apart > total / 2, "tw_swiglu_rows: the fixture tells the halves apart ({apart} of {total})")
        var ka = GkPoolRowsArgs(width = uint(width), total = uint(total))
        var kg_args = GkGatedRowsArgs(d = uint(width), nelem = uint(total))
        let bx = mc |> in_(x)
        let bp = mc |> out_(total)
        let bg = mc |> out_(total)
        let ran = mc |> cell_record(t, "tw_tail_rows") $(enc : MetalComputeEncoder?) {
            invoke(kp.enc, enc, pso_p, mc.buf[bx], mc.buf[bp], ka, int64(total))
            invoke(kg.enc, enc, pso_g, mc.buf[bx], mc.buf[bg], kg_args, int64(total))
        }
        return if (!ran)
        check_buf(t, "tw_pool2", mc.buf[bp], pooled, BAR_APPROX, Shape(row_width = width))
        check_buf(t, "tw_swiglu_rows", mc.buf[bg], gated, BAR_APPROX, Shape(row_width = width))
    }
}

//! A vision tail's row standardize, in place, at a width off the 64-thread group: (x * scale - b[c]) * m[c] against the
//! host form, two rows past the run left as they were; the form with no scale misses on most elements (asserted on the host).
def private tw_affine_gate(t : T?; var dev, queue) {
    let tag = "tw_affine_rows"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_tw_affine_rows)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let width = 70
        let rows = 5
        let total = rows * width
        let scale = 27.712812   // sqrt(768)
        var inscope x <- [for (i in range((rows + 2) * width)); float((i * 37) % 101 - 50) * 0.043]
        var inscope b <- [for (i in range(width)); float((i * 7) % 13 - 6) * 0.31]
        var inscope m <- [for (i in range(width)); 0.5 + float((i * 5) % 11) * 0.17]
        var inscope want := x
        var apart = 0
        for (i in range(total)) {
            want[i] = (x[i] * scale - b[i % width]) * m[i % width]
            apart += abs(want[i] - (x[i] - b[i % width]) * m[i % width]) > 1e-3 ? 1 : 0
        }
        t |> success(apart > total / 2, "{tag}: the fixture tells the scale apart ({apart} of {total})")
        var ka = GkAffineRowsArgs(width = uint(width), total = uint(total), scale = scale, boff = 0u, moff = 0u, std = 1u)
        let bx = mc |> io_(x)
        let bb = mc |> in_(b)
        let bm = mc |> in_(m)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[bx], mc.buf[bb], mc.buf[bm], ka, int64(total))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, BAR_APPROX, Shape(row_width = width))
        t |> success(moved_off(want, x, 0, BAR_APPROX), "{tag}: the rows moved off x")
        var ka0 = ka
        ka0.std = 0u
        var inscope want0 <- [for (i in range(total)); x[i] * scale]
        let bx0 = mc |> io_(x)
        let ran0 = mc |> cell_record(t, "{tag} scale-only") $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[bx0], mc.buf[bb], mc.buf[bm], ka0, int64(total))
        }
        return if (!ran0)
        check_buf(t, "{tag} scale-only", mc.buf[bx0], want0, lead_poisoned(BAR_APPROX, want0), Shape(row_width = width))
        t |> success(moved_off(want0, x, 0, BAR_APPROX), "{tag} scale-only: the rows moved off x")
    }
}

enum RopeTabStamp {
    tw   //! the tower's NEOX rope over a fused row's k slot (`MetalTwRopeTab`), the tables read at the token's row
    pk   //! the pocket prompt's interleaved rope from a position base (`MetalPkRopeTab`), the tables bound whole
}

//! the position-based table rope both homes stamp (`GkRopeTab`) over the k span of fused [q | k | v] rows in place from
//! position `pos0`: the rows against `rope_tab_ref`, the q and v spans as they were, the k span moved off the input, a poisoned
//! k element reds the compare
def private rope_tab_gate(t : T?; var dev, queue; stamp : RopeTabStamp; rows, d, dh, pos0 : int) {
    let tag = "rope_tab {stamp} rows={rows} d={d} pos0={pos0}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp == RopeTabStamp.tw ? stamp_of(@@enc_tw_rope_tab) : stamp_of(@@enc_pk_rope_tab)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let qs = 3 * d
        let half = dh / 2
        var inscope x <- f16_fill(rows * qs, 41, 0.05, -2.1)
        var inscope tcos, tsin : array<float>
        let none : array<float>
        build_rope_tabs(tcos, tsin, 10000.0, 1.0, 1.0, none, false, 0l, int64(pos0 + rows), int64(dh))
        var inscope want := x
        for (r in range(rows)) {
            rope_tab_ref(want, r * qs + d, dh, 0, d, stamp == RopeTabStamp.tw, tcos, tsin, (pos0 + r) * half)
        }
        let ka = GkRopeTabArgs(rows = uint(rows), qs = uint(qs), col = uint(d), d = uint(d), dh = uint(dh), coff = uint(pos0 * half), soff = uint(pos0 * half))
        let pc = mc |> in_(tcos)
        let ps = mc |> in_(tsin)
        var bc = mc.buf[pc]
        var bs = mc.buf[ps]
        let run <- @(bxx : MetalBuffer?) : bool {
            var kl = ka
            return queue_record(t, queue, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, bxx, bc, bs, kl, int64(rows * d / 2))
                }
            }
        }
        let bar = Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true)
        let px = mc |> io_(x)
        if (invoke(run, mc.buf[px])) {
            check_buf(t, tag, mc.buf[px], want, lead_poisoned(bar, want), Shape(row_width = qs))
            let kz = (pos0 > 0 ? 0 : qs) + d + 2   //! a row at a nonzero position: position 0 turns nothing
            t |> success(moved_off(want, x, kz, bar), "{tag}: a k pair at a nonzero angle turned off its input")
        }
        x[d + 5] += 1.5
        let px2 = mc |> io_(x)
        if (invoke(run, mc.buf[px2])) {
            t |> success(mc |> rerun_mismatches(px2, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned k element reds the compare")
        }
    }
}

//! the gated hidden stamp in place over g against silu(g + bg) . (u + bu) in double, the bias rows at offsets in one plane; a
//! poisoned expectation reds the compare, the rows moved off g
def private tw_bias_gate_gate(t : T?; var dev, queue; ff, rows : int) {
    let tag = "tw_bias_gate ff={ff} rows={rows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_tw_bias_gate)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let total = rows * ff
        var inscope g <- [for (i in range(total)); float((i * 37) % 101 - 50) * 0.043]
        var inscope u <- [for (i in range(total)); float((i * 53) % 89 - 44) * 0.031]
        var inscope wn <- [for (i in range(4 * ff)); float((i * 7) % 13 - 6) * 0.21]   //! the gate bias row at ff, the up bias row at 3 ff
        var inscope want : array<float>
        for (i in range(total)) {
            let gb = double(g[i] + wn[ff + i % ff])
            want |> push(float(gb / (1.0lf + exp(-gb)) * double(u[i] + wn[3 * ff + i % ff])))
        }
        var ka = GkBiasGateArgs(d = uint(ff), nelem = uint(total), goff = uint(ff), uoff = uint(3 * ff))
        let bg = mc |> io_(g)
        let bu = mc |> in_(u)
        let bw = mc |> in_(wn)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bg], mc.buf[bu], mc.buf[bw], ka, int64((total + 3) / 4))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bg], want, lead_poisoned(BAR_APPROX, want), Shape(row_width = ff))
        t |> success(moved_off(want, g, 0, BAR_APPROX), "{tag}: the rows moved off g")
    }
}

enum TwSeamStamp {
    add_ln          //! the biased add + layernorm
    add_rms         //! the biased add + rms
    add_post_rms    //! the gemma post-normed add + rms
    pk_add_ln       //! the pocket seam: the unbiased add + layernorm, the normed rows at a 16-float offset of their buffer
}

//! the residual seam stamps vs the CPU forms past the 256-thread stride; the ln_on-off leg leaves the norm rows at their
//! sentinel, a poisoned branch element reds both compares
def private tw_seam_gate(t : T?; var dev, queue; kind : TwSeamStamp; d, rows : int) {
    let tag = "tw_seam {kind} d={d} rows={rows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = kind == TwSeamStamp.add_ln ? stamp_of(@@enc_tw_add_ln) : (kind == TwSeamStamp.add_rms ? stamp_of(@@enc_tw_add_rms) : (kind == TwSeamStamp.add_post_rms ? stamp_of(@@enc_tw_add_post_rms) : stamp_of(@@enc_pk_add_ln)))
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let eps = 1e-6
        let total = rows * d
        let ln = kind == TwSeamStamp.add_ln || kind == TwSeamStamp.pk_add_ln
        let biased = kind != TwSeamStamp.pk_add_ln
        let xboff = kind == TwSeamStamp.pk_add_ln ? 16 : 0   //! the pocket stamp lands the normed rows past a lead the sentinel must keep
        var inscope x <- [for (i in range(total)); float((i * 37) % 101 - 50) * 0.043]
        var inscope y <- [for (i in range(total)); float((i * 53) % 89 - 44) * 0.031]
        var inscope wn <- [for (i in range(4 * d)); 0.5 + float((i * 5) % 11) * 0.17 - (i < d ? 0.9 : 0.0)]   //! the bias row, the norm weight, the norm bias, the post-norm weight
        var inscope want_x : array<float>
        want_x |> resize(total)
        for (r in range(rows)) {
            if (kind == TwSeamStamp.add_post_rms) {
                var inscope yn : array<float>
                yn |> resize(d)
                unsafe {
                    rmsnorm(addr(yn[0]), addr<float const?>(y[r * d]), addr<float const?>(wn[3 * d]), int64(d), eps)
                }
                for (i in range(d)) {
                    want_x[r * d + i] = x[r * d + i] + yn[i]
                }
            } else {
                for (i in range(d)) {
                    want_x[r * d + i] = x[r * d + i] + (biased ? y[r * d + i] + wn[i] : y[r * d + i])
                }
            }
        }
        var inscope want_xb <- [for (_i in range(xboff)); -7.0]
        want_xb |> resize(xboff + total)
        for (r in range(rows)) {
            unsafe {
                if (ln) {
                    layernorm(addr(want_xb[xboff + r * d]), addr<float const?>(want_x[r * d]), addr<float const?>(wn[d]), addr<float const?>(wn[2 * d]), int64(d), eps)
                } else {
                    rmsnorm(addr(want_xb[xboff + r * d]), addr<float const?>(want_x[r * d]), addr<float const?>(wn[d]), int64(d), eps)
                }
            }
        }
        var ka = (kind == TwSeamStamp.add_post_rms ? gk_add_post_rms_args(uint(d), uint(rows), eps, uint(3 * d), true, uint(d))
                            : gk_add_ln_args(uint(d), uint(rows), eps, 0u, true, uint(d), uint(2 * d)))
        ka.xboff = uint(xboff)
        let pocket = kind == TwSeamStamp.pk_add_ln   //! the pocket's rows hold the tight bars its own gate held
        let bar_x = pocket ? Bar(rel = 1e-6, abs_floor = 1e-6, poison_scaled = true) : BAR_APPROX
        let bar_xb = pocket ? Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true) : BAR_APPROX
        let pwn = mc |> in_(wn)
        var bwn = mc.buf[pwn]
        let run <- @(bxx, byy, bxbb : MetalBuffer?; pa : GkPostAddLnArgs) : bool {
            var pav = pa
            return queue_record(t, queue, "{tag} dispatch") $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, bxx, byy, bxbb, bwn, pav, int64(rows))
                }
            }
        }
        let px = mc |> io_(x)
        let py = mc |> in_(y)
        let pxb = mc |> out_words(xboff + total, -7.0)
        if (invoke(run, mc.buf[px], mc.buf[py], mc.buf[pxb], ka)) {
            check_buf(t, "{tag} x", mc.buf[px], want_x, bar_x, Shape(row_width = d))
            check_buf(t, "{tag} xb", mc.buf[pxb], want_xb, bar_xb, FLAT_SHAPE)
            t |> success(moved_off(want_x, x, 0, bar_x), "{tag}: the joined rows moved off x")
        }
        var ka0 = ka
        ka0.ln_on = 0u
        let px0 = mc |> io_(x)
        let py0 = mc |> in_(y)
        let pxb0 = mc |> out_words(xboff + total, -7.0)
        if (invoke(run, mc.buf[px0], mc.buf[py0], mc.buf[pxb0], ka0)) {
            check_buf(t, "{tag} seam alone x", mc.buf[px0], want_x, bar_x, Shape(row_width = d))
            let untouched <- [for (_i in range(xboff + total)); -7.0]
            check_buf(t, "{tag} seam alone xb", mc.buf[pxb0], untouched, BAR_EXACT, FLAT_SHAPE)
        }
        y[(rows / 2) * d + d / 2] += 0.5
        let px2 = mc |> io_(x)
        let py2 = mc |> in_(y)
        let pxb2 = mc |> out_words(xboff + total, -7.0)
        if (invoke(run, mc.buf[px2], mc.buf[py2], mc.buf[pxb2], ka)) {
            t |> success(mc |> rerun_mismatches(px2, want_x, bar_x.rel, 1e-6, "{tag} x poison (MUST mismatch)") > 0, "{tag}: a poisoned branch element reds the joined rows")
            t |> success(mc |> rerun_mismatches(pxb2, want_xb, bar_xb.rel, 1e-6, "{tag} xb poison (MUST mismatch)") > 0, "{tag}: a poisoned branch element reds the norm rows")
        }
    }
}

//! A vision tail's grid mean pool at a width off the 64-thread group: a 6 x 4 row grid pooled 2 x 2 into 3 x 2 rows
//! against the host mean of each block; the pool along the other axis alone misses on most elements (asserted on the host).
def private tw_pool2d_gate(t : T?; var dev, queue) {
    let tag = "tw_pool2d"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kd = kernel_of(@@enc_tw_pool2d)
        var pso = mc |> keep_stamp(kd, err)
        return if (!pso_ok(t, tag, pso, err))
        let width = 70
        let gw = 6
        let gh = 4
        let k = 2
        let ow = gw / k
        let total = ow * (gh / k) * width
        var inscope x <- [for (i in range(gw * gh * width)); float((i * 37) % 101 - 50) * 0.043]
        var inscope want, row_only : array<float>
        for (i in range(total)) {
            let o = i / width
            let c = i % width
            var sum = 0.0lf
            var row = 0.0lf
            for (dy in range(k)) {
                for (dx in range(k)) {
                    let v = double(x[(((o / ow) * k + dy) * gw + (o % ow) * k + dx) * width + c])
                    sum += v
                    row += dy == 0 ? v : 0.0lf
                }
            }
            want |> push(float(sum / double(k * k)))
            row_only |> push(float(row / double(k)))
        }
        var apart = 0
        for (a, b in want, row_only) {
            apart += abs(a - b) > 1e-3 ? 1 : 0
        }
        t |> success(apart > total / 2, "{tag}: the fixture tells a one-axis pool apart ({apart} of {total})")
        var ka = GkPool2dArgs(width = uint(width), gw = uint(gw), k = uint(k), total = uint(total))
        let bx = mc |> in_(x)
        let by = mc |> out_(total)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            invoke(kd.enc, enc, pso, mc.buf[bx], mc.buf[by], ka, int64(total))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_APPROX, Shape(row_width = width))
    }
}

//! A 1x1 conv's bias and ReLU over its rows, in place: max(x + b, 0) against the same f32 form, the fixture on
//! both sides of zero.
def private bias_relu_rows_gate(t : T?; var dev, queue) {
    let tag = "bias_relu_rows"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_bias_act)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nrows = 7
        let n = 24
        let total = nrows * n
        var inscope x <- [for (i in range(total)); float((i * 5) % 9 - 4) * 0.5]
        var inscope b <- [for (i in range(n)); float((i * 7 + 1) % 9 - 4) * 0.25]
        var inscope want <- [for (i in range(total)); max(x[i] + b[i % n], 0.0)]
        var clamped = 0
        for (i in range(total)) {
            clamped += x[i] + b[i % n] < 0.0 ? 1 : 0
        }
        t |> success(clamped > 8 && clamped < total - 8, "{tag}: the sums take both signs ({clamped} of {total} clamp)")
        let bx = mc |> io_(x)
        let bb = mc |> in_(b)
        var ka = GkBiasActArgs(d = uint(n), nelem = uint(total), boff = 0u, act = BIAS_ACT_RELU)
        mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[bb], ka, int64((total + 3) / 4))
            }
        }
        t |> success(mismatch_exact(want, x) > 0, "{tag}: the reference moves off the input")
        check_buf(t, tag, mc.buf[bx], want, BAR_EXACT, Shape(row_width = n))
    }
}

// ===== the gemma4a sliding-window RPE attention (causal window 12, softcap, per-dim scale) =====
def private g4a_attn_gate(t : T?; var dev, queue) {
    let tag = "g4a_attn"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let kg = kernel_of(@@enc_g4a_attn)
        var pso = mc |> keep_stamp(kg, err)
        return if (!pso_ok(t, tag, pso, err))
        let np = 40
        let heads = 2
        let hs = 128
        let d = heads * hs
        let past = 12
        let scales = gemma4a_attn_scales(int64(hs))
        let qscale = scales.qscale
        let kscale = scales.kscale
        let cap = 50.0
        var inscope(qv, kv, vv) <- f16_qkv_rows(np * d)
        var inscope rpe : array<float>
        rpe |> resize((past + 1) * d)
        for (i in range((past + 1) * d)) {
            rpe[i] = float(float16(0.03 * float((i * 11) % 29) - 0.4))
        }
        var inscope pds : array<float>
        pds |> resize(hs)
        for (i in range(hs)) {
            pds[i] = float(float16(0.5 + 0.02 * float(i % 17)))
        }
        var inscope want : array<float>
        want |> resize(np * d)
        for (h in range(heads)) {
            for (qi in range(np)) {
                let j0 = max(0, qi - (past - 1))
                var mx = -3.0e38
                var sc : float[16]
                for (j in range(j0, qi + 1)) {
                    var cs = 0.0lf
                    var rs = 0.0lf
                    let rr = (past - (qi - j)) * d
                    for (dd in range(hs)) {
                        let qd0 = double(qv[qi * d + h * hs + dd]) * double(qscale) * double(pds[dd])
                        cs += qd0 * (double(kv[j * d + h * hs + dd]) * double(kscale))
                        rs += qd0 * double(rpe[rr + h * hs + dd])
                    }
                    let raw = float(cs + rs)
                    let s = cap * float(tanh(double(raw) / double(cap)))
                    sc[j - j0] = s
                    mx = max(mx, s)
                }
                var tot = 0.0lf
                var oa : float[128]
                for (dd in range(hs)) {
                    oa[dd] = 0.0
                }
                for (j in range(j0, qi + 1)) {
                    let e = exp(sc[j - j0] - mx)
                    tot += double(e)
                    for (dd in range(hs)) {
                        oa[dd] += e * vv[j * d + h * hs + dd]
                    }
                }
                for (dd in range(hs)) {
                    want[qi * d + h * hs + dd] = oa[dd] / float(tot)
                }
            }
        }
        let bq = mc |> in_(qv)
        let bk = mc |> in_(kv)
        let bv = mc |> in_(vv)
        let brp = mc |> in_(rpe)
        let bpd = mc |> in_(pds)
        let bo = mc |> out_words(np * d, -1.0e30)
        var ka = GkG4aAttnArgs(d = uint(d), npos = uint(np), hs = uint(hs), pdsoff = 0u, cap = cap, qscale = qscale, kscale = kscale)
        for (leg in range(2)) {
            if (leg == 1) {
                vv[18 * d + hs / 2] += 3.0
                mc |> plane_upload(bv, vv)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                invoke(kg.enc, enc, pso, mc.buf[bq], mc.buf[bk], mc.buf[bv], mc.buf[bo], mc.buf[brp], mc.buf[bpd], ka, int64(np * heads))
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[bo], want, Bar(rel = 1e-3, abs_floor = 1e-4, poison_scaled = true), Shape(row_width = d, head_width = hs))
            } else {
                t |> success(mc |> rerun_mismatches(bo, want, 1e-3, 1e-4, "{tag} poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned V element reds the bars")
            }
        }
    }
}

// ===== the StyleTTS2 chain's kernels (the tower driver's TTS seats) =====

def private st2_conv_fixture(dev; transposed : bool) : St2ConvArgs {
    //! forward: 40 channels (an 8-run never straddles a tap; the k-run tail lands past the taps -
    //! kdim 280 pads to 288), dilation 3 with pad 9 reaches off both ends of the 45 rows, which pad
    //! to 64 (the pad-rows regime); transposed: stride 6 skips the source rows the taps miss
    if (transposed) {
        return St2ConvArgs(cin = 40u, k = 12u, stride = 6u, pad_l = 3u, dil = 1u, t_in = 8u, r0 = 0u, rows = 48u, xs = 40u)
    }
    return St2ConvArgs(cin = 40u, k = 7u, stride = 1u, pad_l = 9u, dil = 3u, t_in = 45u, r0 = 0u, rows = 45u, xs = 40u)
}

//! the CPU twin of the kernel's gather: the input row output row r's tap reaches, or -1
def private st2_conv_src(dev; ka; transposed : bool; r, tap : int) : int {
    if (transposed) {
        let q = int(ka.r0) + r + int(ka.pad_l) - tap * int(ka.dil)
        return (q >= 0 && q % int(ka.stride) == 0 && q / int(ka.stride) < int(ka.t_in)) ? q / int(ka.stride) : -1
    }
    let st = (int(ka.r0) + r) * int(ka.stride) + tap * int(ka.dil) - int(ka.pad_l)
    return (st >= 0 && st < int(ka.t_in)) ? st : -1
}

//! the conv GEMM on either stamp: the f16-staged one on f16-exact operands (`f16_at`: every product exact,
//! the bar the sum order's), the f32-exact one on operands off the f16 lattice at a
//! bar the f16 stamp must exceed on the same fixture (the stamp's control)
def private st2_conv_gate(t : T?; var dev, queue; transposed : bool; exact : bool) {
    let tag = (exact ? "st2_conv_exact_mm " : "st2_conv_mm ") + (transposed ? "transposed" : "forward")
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sh = transposed ? stamp_of(@@enc_st2_conv_tr_mm) : stamp_of(@@enc_st2_conv_mm)
        let se = transposed ? stamp_of(@@enc_st2_conv_tr_exact_mm) : stamp_of(@@enc_st2_conv_exact_mm)
        let sm = exact ? se : sh
        var pso = mc |> keep_stamp(sm, err)
        return if (!pso_ok(t, tag, pso, err))
        var pso_half : MetalComputePipeline?
        if (exact) {
            pso_half = mc |> keep_stamp(sh, err)
            return if (!pso_ok(t, "{tag} f16-staged control", pso_half, err))
        }
        let off16 = exact ? 1.0 / 3.0 : 1.0   //! a factor that takes the operands off the f16 lattice
        let ka = st2_conv_fixture(dev, transposed)
        let cin = int(ka.cin)
        let k = int(ka.k)
        let kdim = (k * cin + 31) / 32 * 32
        let cout = 64
        let t_in = int(ka.t_in)
        let rows = int(ka.rows)
        let mp = (rows + 31) / 32 * 32
        var inscope x : array<float>
        x |> resize(t_in * cin)
        for (i in range(t_in * cin)) {
            x[i] = f16_at((i * 7) % 41, 0.05, -1.0) * off16
        }
        var inscope w : array<float>
        w |> resize(cout * kdim)   //! the pad columns past k * cin stay zero, as the slab's do
        for (co in range(cout)) {
            for (j in range(k * cin)) {
                w[co * kdim + j] = f16_at(((co + 1) * (j + 3)) % 37, 0.03, -0.5) * off16
            }
        }
        let rel = exact ? 2e-6 : 1e-3
        let abs_floor = exact ? 2e-6 : 1e-3
        var inscope want : array<float>
        want |> resize(mp * cout)   // the resize's zeros are the expected pad rows
        for (r in range(rows)) {
            for (co in range(cout)) {
                var acc = 0.0lf
                for (tap in range(k)) {
                    let src = st2_conv_src(dev, ka, transposed, r, tap)
                    continue if (src < 0)
                    for (ci in range(cin)) {
                        acc += double(x[src * cin + ci]) * double(w[co * kdim + tap * cin + ci])
                    }
                }
                want[r * cout + co] = float(acc)
            }
        }
        let bw = mc |> in_(w)
        let by = mc |> out_words(mp * cout, -1.0e30)
        let bx = mc |> in_(x)
        let bn = mc |> in_plain([uint(cout)])
        var kac = ka
        // leg 0: the stamp; leg 1 (exact only): the f16-staged stamp on the same operands; leg 2: a poisoned input
        for (leg in range(3)) {
            continue if (leg == 1 && !exact)
            if (leg == 2) {
                x[(t_in / 2) * cin + 5] += 2.0   // a mid-row element every output row near it reads
                mc |> plane_upload(bx, x)
            }
            let half = leg == 1
            let sr = half ? sh : sm
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(sr, half ? pso_half : pso) {
                    invoke(sr.enc, enc, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by], 0ul, mc.buf[bn], kac, int64(mp), int64(cout))
                }
            }
            if (leg == 0) {
                return if (!ran)
                check_buf(t, tag, mc.buf[by], want, Bar(rel = rel, abs_floor = abs_floor, poison_scaled = true), Shape(row_width = cout))
            } elif (!ran) {
                continue
            } elif (leg == 1) {
                t |> success(mc |> rerun_mismatches(by, want, rel, abs_floor, "{tag} f16-staged stamp on the same operands (MUST mismatch)") > 0,
                    "{tag}: the f16-staged stamp misses the exact bar on operands off the f16 lattice (the stamp's control)")
            } else {
                t |> success(mc |> rerun_mismatches(by, want, rel, abs_floor, "{tag} x poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned input element reds the bars")
            }
        }
    }
}

//! the im2col route (a channel count off the 8-run, the noise convs' 22): whole and offset chunks, exact
def private st2_im2col_gate(t : T?; var dev, queue; r0, rows : int; transposed : bool) {
    let tag = (transposed ? "st2_im2col transposed r0 " : "st2_im2col r0 ") + "{r0} rows {rows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_im2col)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let oracle_args = St2ConvArgs(cin = 22u, k = 12u, stride = 6u, pad_l = 3u, dil = 1u, t_in = 30u, r0 = uint(r0), rows = uint(rows), xs = 22u)   //! the oracle's geometry
        let cin = 22
        let kdim = 288
        var kim = GkIm2col1dArgs(cin = 22u, tin = 30u, tout = uint(rows), k = 12u, stride = 6u, pad_l = 3u, dilation = 1u, transposed = transposed ? 1u : 0u,
            kdim = uint(kdim), xoff = 0u, coloff = 0u, row_major = 1u, r0 = uint(r0), mp = 32u)
        var inscope x : array<float>
        x |> resize(30 * cin)
        for (i in range(30 * cin)) {
            x[i] = f16_at((i * 5) % 43, 0.07, -1.5)
        }
        var inscope want : array<float>
        want |> resize(32 * kdim)   //! zeros past the taps, the pad columns and the pad rows
        for (r in range(rows)) {
            for (tap in range(12)) {
                let src = st2_conv_src(dev, oracle_args, transposed, r, tap)
                continue if (src < 0)
                for (ci in range(cin)) {
                    want[r * kdim + tap * cin + ci] = x[src * cin + ci]
                }
            }
        }
        let bcols = mc |> out_words(32 * kdim, -1.0e30)
        let bx = mc |> in_(x)
        for (leg in range(2)) {
            if (leg == 1) {
                var hit = -1
                for (tap in range(12)) {   //! an input row the first chunk row's taps reach, on either geometry
                    hit = st2_conv_src(dev, oracle_args, transposed, 0, tap)
                    break if (hit >= 0)
                }
                t |> success(hit >= 0, "{tag}: the first row reads some input row")
                x[hit * cin + 3] += 2.0
                mc |> plane_upload(bx, x)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[bx], mc.buf[bcols], kim, int64(32 * kdim))
                }
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[bcols], want, BAR_ZERO, Shape(row_width = kdim))
            } else {
                t |> success(mc |> rerun_mismatches(bcols, want, 0.0, 0.0, "{tag} x poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned input element reds the compare")
            }
        }
    }
}

//! column stats + fused AdaIN against the CPU formula at 24 channels: one stride with a tail, two strides plus a tail
//! on f16-exact fixtures (`f16_at`: products exact), so the bar covers only the reduction and the Snake's sin()
def private st2_adain_gate(t : T?; var dev, queue; nrows : int; snake : bool) {
    let tag = snake ? "st2_adain_snake t{nrows}" : "st2_adain_leaky t{nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sst = stamp_of(@@enc_st2_colstats)
        let ssn = stamp_of(@@enc_st2_adain_snake)
        let slk = stamp_of(@@enc_st2_adain_leaky)
        var pso_st = mc |> keep_stamp(sst, err)
        let st_ok = pso_ok(t, "{tag} colstats", pso_st, err)
        var pso = snake ? (mc |> keep_stamp(ssn, err)) : (mc |> keep_stamp(slk, err))
        return if (!pso_ok(t, tag, pso, err) || !st_ok)
        let c = 24
        let n = nrows * c
        var inscope x : array<float>
        x |> resize(n)
        for (i in range(n)) {
            x[i] = f16_at((i * 11) % 53, 0.04, -1.1)
        }
        var inscope h, gw, gb, alpha : array<float>
        h |> resize(2 * c)
        gw |> resize(c)
        gb |> resize(c)
        alpha |> resize(c)
        for (i in range(c)) {
            h[i] = f16_at((i * 3) % 17, 0.02, -0.1)
            h[c + i] = f16_at((i * 5) % 19, 0.03, -0.2)
            gw[i] = f16_at((i * 7) % 13, 0.05, 0.7)
            gb[i] = f16_at((i * 2) % 11, 0.01, -0.05)
            alpha[i] = f16_at((i * 3) % 7, 0.1, 0.3)
        }
        var ka = GkAdainArgs(c = uint(c), t = uint(nrows), hoff = 0u, gwoff = 0u, gboff = uint(c), aoff = uint(2 * c))
        var inscope want_st, want : array<float>
        want_st |> resize(2 * c)
        want |> resize(n)
        for (ci in range(c)) {
            var mean = 0.0lf
            for (r in range(nrows)) {
                mean += double(x[r * c + ci])
            }
            mean /= double(nrows)
            var ssq = 0.0lf
            for (r in range(nrows)) {
                let d = double(x[r * c + ci]) - mean
                ssq += d * d
            }
            want_st[ci] = float(mean)
            want_st[c + ci] = float(ssq)
            let rs = 1.0 / sqrt(float(ssq) / float(nrows) + 1e-5)
            let g1 = 1.0 + h[ci]
            let scale = rs * gw[ci] * g1
            let shift = (gb[ci] - float(mean) * rs * gw[ci]) * g1 + h[c + ci]
            for (r in range(nrows)) {
                let v = x[r * c + ci] * scale + shift
                let sn = sin(alpha[ci] * v)
                want[r * c + ci] = snake ? v + (1.0 / alpha[ci]) * (sn * sn) : (v > 0.0 ? v : v * 0.2)
            }
        }
        let bst = mc |> out_words(2 * c, -1.0e30)
        let by = mc |> out_words(n, -1.0e30)
        let bh = mc |> in_(h)
        var inscope wn <- clone_to_move(gw)
        wn |> push_from(gb)
        wn |> push_from(alpha)
        let bwn = mc |> in_(wn)
        let bx = mc |> in_(x)
        let bt = mc |> in_plain([uint(nrows)])
        let bc = mc |> in_plain([uint(c)])
        let bar = Bar(rel = 1e-4, abs_floor = 1e-5, poison_scaled = true)
        for (leg in range(2)) {
            if (leg == 1) {
                x[3 * c + 5] += 2.0
                mc |> plane_upload(bx, x)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                let (px, pst) = (mc.buf[bx], mc.buf[bst])
                stamp_run(sst, pso_st) {
                    invoke(sst.enc, enc, px, pst, mc.buf[bt], mc.buf[bc], int64(c))
                }
                if (snake) {
                    stamp_run(ssn, pso) {
                        invoke(ssn.enc, enc, px, mc.buf[by], pst, mc.buf[bh], mc.buf[bwn], ka, int64(n))
                    }
                } else {
                    stamp_run(slk, pso) {
                        invoke(slk.enc, enc, px, mc.buf[by], pst, mc.buf[bh], mc.buf[bwn], ka, int64(n))
                    }
                }
            }
            return if (!ran)
            if (leg == 0) {
                check_buf(t, "{tag} stats", mc.buf[bst], want_st, bar, Shape(row_width = c))
                check_buf(t, tag, mc.buf[by], want, bar, Shape(row_width = c))
            } else {
                t |> success(mc |> rerun_mismatches(bst, want_st, 1e-4, 1e-5, "{tag} stats x poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned input element reds the stats")
                t |> success(mc |> rerun_mismatches(by, want, 1e-4, 1e-5, "{tag} x poison (MUST mismatch)") > 0,
                    "{tag}: a poisoned input element reds the rows")
            }
        }
    }
}

//! leaky (in place), axpy on both zero regimes, add, reflect - every total off the 64-lane grid, so the tail threadgroup runs
def private st2_rows_gate(t : T?; var dev, queue) {
    let tag = "st2 rows"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let slk = stamp_of(@@enc_st2_leaky)
        let sax = stamp_of(@@enc_st2_axpy)
        let sad = stamp_of(@@enc_st2_add)
        let srf = stamp_of(@@enc_st2_reflect1)
        var pso_lk = mc |> keep_stamp(slk, err)
        let lk_ok = pso_ok(t, "{tag} leaky", pso_lk, err)
        var pso_ax = mc |> keep_stamp(sax, err)
        let ax_ok = pso_ok(t, "{tag} axpy", pso_ax, err)
        var pso_add = mc |> keep_stamp(sad, err)
        let add_ok = pso_ok(t, "{tag} add", pso_add, err)
        var pso_rf = mc |> keep_stamp(srf, err)
        return if (!pso_ok(t, "{tag} reflect1", pso_rf, err) || !lk_ok || !ax_ok || !add_ok)
        let c = 24
        let nrows = 9
        let n = nrows * c   //! 216: three threadgroups and a 24-lane tail
        var inscope a, b : array<float>
        a |> resize(n)
        b |> resize(n)
        for (i in range(n)) {
            a[i] = f16_at((i * 7) % 31, 0.1, -1.5)
            b[i] = f16_at((i * 5) % 29, 0.1, -1.4)
        }
        var inscope want_lk, want_ax0, want_ax1, want_add, want_rf : array<float>
        want_lk |> resize(n)
        want_ax0 |> resize(n)
        want_ax1 |> resize(n)
        want_add |> resize(n)
        want_rf |> resize(n + c)
        var neg_at = -1
        for (i in range(n)) {
            want_lk[i] = a[i] > 0.0 ? a[i] : a[i] * 0.1
            if (neg_at < 0 && a[i] < 0.0) {
                neg_at = i
            }
            want_ax0[i] = a[i] + 0.25 * b[i]
            want_ax1[i] = 0.25 * b[i]
            want_add[i] = a[i] + b[i]
        }
        for (r in range(nrows + 1)) {
            for (ci in range(c)) {
                want_rf[r * c + ci] = a[(r == 0 ? 1 : r - 1) * c + ci]
            }
        }
        let po = mc |> out_words(n + c, -1.0e30)
        var bo = mc.buf[po]
        var (bn, bslope, bc, bnrf) = (mc.buf[mc |> in_plain([uint(n)])], mc.buf[mc |> in_plain([0.1])],
            mc.buf[mc |> in_plain([uint(c)])], mc.buf[mc |> in_plain([uint(n + c)])])
        let run_leaky <- @(bxx : MetalBuffer?) : bool {
            return queue_record(t, queue, "{tag} leaky") $(enc : MetalComputeEncoder?) {
                stamp_run(slk, pso_lk) {
                    invoke(slk.enc, enc, bxx, 0ul, bn, bslope, int64(n))
                }
            }
        }
        let run_axpy <- @(bax, bbx : MetalBuffer?; zero : uint) : bool {
            return queue_record(t, queue, "{tag} axpy") $(enc : MetalComputeEncoder?) {
                stamp_run(sax, pso_ax) {
                    invoke(sax.enc, enc, bax, bbx, GkAxpyArgs(nelem = uint(n), scale = 0.25, zero = zero), int64(n))
                }
            }
        }
        let run_add <- @(bax, bbx : MetalBuffer?; joined : bool) : bool {
            return queue_record(t, queue, "{tag} add") $(enc : MetalComputeEncoder?) {
                stamp_run(sad, pso_add) {
                    invoke(sad.enc, enc, bax, bbx, bo, GkAddScaleArgs(nelem = uint(n), scale = joined ? 0.70710678 : 1.0), int64(n))
                }
            }
        }
        let run_reflect <- @(bax : MetalBuffer?) : bool {
            return queue_record(t, queue, "{tag} reflect1") $(enc : MetalComputeEncoder?) {
                stamp_run(srf, pso_rf) {
                    invoke(srf.enc, enc, bax, bo, bc, bnrf, int64(n + c))
                }
            }
        }
        let px = mc |> io_(a)
        if (invoke(run_leaky, mc.buf[px])) {
            check_buf(t, "{tag} leaky", mc.buf[px], want_lk, BAR_ZERO, Shape(row_width = c))
            t |> success(neg_at >= 0 && want_lk[neg_at] != a[neg_at], "{tag} leaky: the output differs from the input at [{neg_at}]")
            a[neg_at] += 2.0
            let px2 = mc |> io_(a)
            if (invoke(run_leaky, mc.buf[px2])) {
                t |> success(mc |> rerun_mismatches(px2, want_lk, 0.0, 0.0, "{tag} leaky poison (MUST mismatch)") > 0, "{tag} leaky: a poisoned element reds the compare")
            }
            a[neg_at] -= 2.0
        }
        let pa = mc |> io_(a)
        let pb = mc |> in_(b)
        let axpy_bar = Bar(rel = 1e-6, abs_floor = 1e-6, poison_scaled = true)
        if (invoke(run_axpy, mc.buf[pa], mc.buf[pb], 0u)) {
            check_buf(t, "{tag} axpy accumulate", mc.buf[pa], want_ax0, axpy_bar, Shape(row_width = c))
        }
        mc |> plane_upload(pa, a)
        if (invoke(run_axpy, mc.buf[pa], mc.buf[pb], 1u)) {
            check_buf(t, "{tag} axpy zero", mc.buf[pa], want_ax1, axpy_bar, Shape(row_width = c))
        }
        b[7] += 2.0
        let pb2 = mc |> in_(b)
        mc |> plane_upload(pa, a)
        if (invoke(run_axpy, mc.buf[pa], mc.buf[pb2], 1u)) {
            t |> success(mc |> rerun_mismatches(pa, want_ax1, 1e-6, 1e-6, "{tag} axpy poison (MUST mismatch)") > 0, "{tag} axpy: a poisoned y element reds the compare")
        }
        mc |> plane_upload(pa, a)
        if (invoke(run_axpy, mc.buf[pa], mc.buf[pb2], 0u)) {
            t |> success(mc |> rerun_mismatches(pa, want_ax0, 1e-6, 1e-6, "{tag} axpy accumulate poison (MUST mismatch)") > 0, "{tag} axpy accumulate: a poisoned y element reds the compare")
        }
        b[7] -= 2.0
        mc |> plane_upload(pa, a)
        if (invoke(run_add, mc.buf[pa], mc.buf[pb], false)) {
            check_buf(t, "{tag} add", bo, want_add, BAR_ZERO, Shape(row_width = c))
        }
        if (invoke(run_add, mc.buf[pa], mc.buf[pb2], false)) {
            t |> success(mc |> rerun_mismatches(po, want_add, 0.0, 0.0, "{tag} add poison (MUST mismatch)") > 0, "{tag} add: a poisoned b element reds the compare")
        }
        var inscope want_join : array<float>
        want_join |> resize(n)
        for (i in range(n)) {
            want_join[i] = (a[i] + b[i]) * 0.70710678
        }
        if (invoke(run_add, mc.buf[pa], mc.buf[pb], true)) {
            check_buf(t, "{tag} add scaled (the residual join)", bo, want_join, Bar(rel = 1e-6, abs_floor = 1e-7, poison_scaled = true), Shape(row_width = c))
        }
        if (invoke(run_add, mc.buf[pa], mc.buf[pb2], true)) {
            t |> success(mc |> rerun_mismatches(po, want_join, 1e-6, 1e-7, "{tag} add scaled poison (MUST mismatch)") > 0, "{tag} add scaled: a poisoned b element reds the compare")
        }
        mc |> plane_refill(po)
        if (invoke(run_reflect, mc.buf[pa])) {
            check_buf(t, "{tag} reflect1", bo, want_rf, BAR_ZERO, Shape(row_width = c))
        }
        a[c + 2] += 2.0   //! row 1: the reflected row 0 and row 2 both read it
        let pa2 = mc |> in_(a)
        if (invoke(run_reflect, mc.buf[pa2])) {
            t |> success(mc |> rerun_mismatches(po, want_rf, 0.0, 0.0, "{tag} reflect1 poison (MUST mismatch)") > 0, "{tag} reflect1: a poisoned source row reds the compare")
        }
    }
}

//! the row gather with its add flags off and on, exact, a poisoned source row as the control
def private st2_gather_gate(t : T?; var dev, queue) {
    let tag = "st2_gather"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_row_gather)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let c = 24
        let nrows = 9
        var inscope a <- f16_fill(16 * c, 7, 0.05, -1.0)
        var inscope b <- f16_fill(nrows * c, 5, 0.03, 0.2)
        var inscope cv <- f16_fill(c, 3, 0.02, -0.1)
        var inscope idx : array<uint>
        idx |> resize(nrows)
        for (r in range(nrows)) {
            idx[r] = uint((r * 5 + 3) % 16)
        }
        let pa = mc |> in_(a)
        let pb = mc |> in_(b)
        let pcv = mc |> in_(cv)
        let pidx = mc |> in_plain(idx)
        let po = mc |> out_words(nrows * c, -1.0e30)
        var bb = mc.buf[pb]
        var bcv = mc.buf[pcv]
        var bidx = mc.buf[pidx]
        var bo = mc.buf[po]
        let run <- @(bax : MetalBuffer?; add_b, add_c : uint) : bool {
            var ka = GkRowGatherArgs(c = uint(c), rows = uint(nrows), aoff = 0u, boff = 0u, coff = 0u, out_off = 0u, add_b = add_b, add_c = add_c, scale = 1.0)
            return queue_record(t, queue, "{tag} add_b {add_b} add_c {add_c}") $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, bax, bidx, bb, bcv, bo, ka, int64(nrows * c))
                }
            }
        }
        var inscope want : array<float>
        want |> resize(nrows * c)
        for (r in range(nrows)) {
            for (j in range(c)) {
                want[r * c + j] = a[int(idx[r]) * c + j]
            }
        }
        if (invoke(run, mc.buf[pa], 0u, 0u)) {
            check_buf(t, "{tag} plain", bo, want, BAR_ZERO, Shape(row_width = c))
        }
        for (r in range(nrows)) {
            for (j in range(c)) {
                want[r * c + j] += b[r * c + j] + cv[j]
            }
        }
        if (invoke(run, mc.buf[pa], 1u, 1u)) {
            check_buf(t, "{tag} with the row and column adds", bo, want, BAR_ZERO, Shape(row_width = c))
        }
        for (arm in range(2)) {   //! each add alone
            var inscope want_one <- [for (i in range(nrows * c)); a[int(idx[i / c]) * c + i % c] + (arm == 0 ? b[i] : cv[i % c])]
            if (invoke(run, mc.buf[pa], arm == 0 ? 1u : 0u, arm == 0 ? 0u : 1u)) {
                check_buf(t, "{tag} {arm == 0 ? "add_b" : "add_c"} alone", bo, want_one, BAR_ZERO, Shape(row_width = c))
            }
        }
        a[int(idx[4]) * c + 2] += 2.0
        mc |> plane_upload(pa, a)
        if (invoke(run, mc.buf[pa], 1u, 1u)) {
            t |> success(mc |> rerun_mismatches(po, want, 0.0, 0.0, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned source row reds the compare")
        }
    }
}

//! the concat rows with and without the residual columns, strided f0 / n columns, exact
def private st2_concat_gate(t : T?; var dev, queue; with_res : bool) {
    let tag = with_res ? "st2_concat with res" : "st2_concat no res"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_concat)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let c = 24
        let rc = 16
        let rs = 64
        let nrows = 7
        let f0s = 4
        let width = with_res ? 48 : 32
        var inscope x <- f16_fill(nrows * c, 7, 0.05, -1.0)
        var inscope res <- f16_fill(nrows * rs, 5, 0.03, 0.2)
        var inscope f0 <- f16_fill(nrows * f0s, 3, 1.5, 80.0)
        var inscope nd <- f16_fill(nrows * f0s, 11, 0.1, -3.0)
        var inscope want <- concat_ref(x, res, f0, nd, nrows, c, rc, rs, f0s, f0s, width, with_res)
        let px = mc |> in_(x)
        let pf0 = mc |> in_(f0)
        let pnd = mc |> in_(nd)
        let po = mc |> out_words(nrows * width, -1.0e30)
        let pres = mc |> in_(res)
        for (leg in range(2)) {
            if (leg == 1) {
                if (with_res) {
                    res[3 * rs + 5] += 2.0
                    mc |> plane_upload(pres, res)
                } else {
                    want[3 * width + c] += 2.0   //! the expectation moves off the true f0 column: the compare must red
                }
            }
            var ka = GkConcatArgs(c = uint(c), rc = uint(rc), rs = uint(rs), f0s = uint(f0s), ns = uint(f0s), with_res = with_res ? 1u : 0u,
                width = uint(width), rows = uint(nrows))
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[px], mc.buf[pres], mc.buf[pf0], mc.buf[pnd], mc.buf[po], ka, int64(nrows * width))
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[po], want, BAR_ZERO, Shape(row_width = width))
            } else {
                t |> success(mc |> rerun_mismatches(po, want, 0.0, 0.0, "{tag} poison (MUST mismatch)") > 0, "{tag}: the poison reds the compare")
            }
        }
    }
}

//! the depthwise transposed pool at stride 2 against the CPU gather, taps off both ends
def private st2_pool_gate(t : T?; var dev, queue) {
    let tag = "st2_pool_dw"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_pool_dw)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let c = 24
        let k = 3
        let stride = 2
        let pad_l = 1
        let t_in = 9
        let t_out = (t_in - 1) * stride - 2 * pad_l + (k - 1) + 1
        var inscope x <- f16_fill(t_in * c, 7, 0.05, -1.0)
        var inscope w <- f16_fill(c * k, 5, 0.1, -0.15)
        var inscope b <- f16_fill(c, 3, 0.02, 0.05)
        var inscope want : array<float>
        want |> resize(t_out * c)
        for (p in range(t_out)) {
            for (j in range(c)) {
                var acc = double(b[j])
                for (kk in range(k)) {
                    let num = p + pad_l - kk
                    continue if (num < 0 || num % stride != 0 || num / stride >= t_in)
                    acc += double(w[j * k + kk]) * double(x[(num / stride) * c + j])
                }
                want[p * c + j] = float(acc)
            }
        }
        var inscope wn <- clone_to_move(w)
        wn |> push_from(b)
        let pwn = mc |> in_(wn)
        let py = mc |> out_words(t_out * c, -1.0e30)
        let px = mc |> in_(x)
        for (leg in range(2)) {
            if (leg == 1) {
                x[4 * c + 6] += 2.0
                mc |> plane_upload(px, x)
            }
            var ka = GkDwConvRowsArgs(c = uint(c), k = uint(k), stride = uint(stride), pad_l = uint(pad_l), dil = 1u, t_in = uint(t_in), t_out = uint(t_out),
                woff = 0u, boff = uint(length(w)))
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[px], mc.buf[py], mc.buf[pwn], ka, int64(t_out * c))
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[py], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = c))
            } else {
                t |> success(mc |> rerun_mismatches(py, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned input row reds the compare")
            }
        }
    }
}


//! both LSTM directions on one input-gate set against a CPU recurrence: 8 units (32 lanes), 13 steps, a padded gate stride
def private st2_lstm_gate(t : T?; var dev, queue) {
    let tag = "st2_lstm"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_lstm)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let hd = 8
        let steps = 13
        let xgs = 64
        var inscope xg <- f16_fill(steps * xgs, 7, 0.05, -1.15)   // centred, so no gate saturates and the poison stays visible
        var inscope whh <- f16_fill(4 * hd * hd, 5, 0.012, -0.28)
        var inscope want : array<float>
        want |> resize(steps * 2 * hd)
        for (dir in range(2)) {
            var inscope h, cs : array<float>
            h |> resize(hd)
            cs |> resize(hd)
            for (step in range(steps)) {
                let ti = dir == 1 ? steps - 1 - step : step
                var inscope hn : array<float>
                hn |> resize(hd)
                for (j in range(hd)) {
                    var g : float[4]
                    for (gi in range(4)) {
                        var acc = double(xg[ti * xgs + gi * hd + j])
                        for (i in range(hd)) {
                            acc += double(whh[(gi * hd + j) * hd + i]) * double(h[i])
                        }
                        g[gi] = float(acc)
                    }
                    cs[j] = sigmoid_f32(g[1]) * cs[j] + sigmoid_f32(g[0]) * tts_tanh(g[2])
                    hn[j] = sigmoid_f32(g[3]) * tts_tanh(cs[j])
                    want[ti * 2 * hd + dir * hd + j] = hn[j]
                }
                h := hn
            }
        }
        var peak = 0.0
        for (v in want) {
            peak = max(peak, abs(v))
        }
        t |> success(peak < 0.98, "{tag}: the fixture's outputs stay off tanh's saturation (peak {peak}), so a poison can move them")
        let pw = mc |> in_(whh)
        let py = mc |> out_words(steps * 2 * hd, -1.0e30)
        let pxg = mc |> in_(xg)
        for (leg in range(2)) {
            if (leg == 1) {
                xg[6 * xgs + 2 * hd + 3] += 2.0
                mc |> plane_upload(pxg, xg)
            }
            var ran = true
            for (dir in range(2)) {
                continue if (!ran)
                var ka = St2LstmArgs(t = uint(steps), hidden = uint(hd), xgs = uint(xgs), ystride = uint(2 * hd), yoff = uint(dir * hd), reverse = uint(dir))
                ran = mc |> cell_record(t, "{tag} dir {dir}") $(enc : MetalComputeEncoder?) {
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, mc.buf[pxg], mc.buf[py], mc.buf[pw], 0ul, ka, int64(4 * hd))
                    }
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[py], want, Bar(rel = 1e-4, abs_floor = 1e-5, poison_scaled = true), Shape(row_width = 2 * hd, head_width = hd))
            } else {
                t |> success(mc |> rerun_mismatches(py, want, 1e-4, 1e-5, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned input gate reds the compare")
            }
        }
    }
}

//! the ALBERT attention rows against the CPU `attention_rows`: two heads of 64, rows off (37) and past (100) the 32-lane split
def private st2_attn_gate(t : T?; var dev, queue; nrows : int) {
    let tag = "st2_attn t{nrows}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_attn)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let heads = 2
        let hs = 64
        let d = heads * hs
        var inscope q <- f16_fill(nrows * d, 7, 0.02, -0.4)
        var inscope k <- f16_fill(nrows * d, 5, 0.03, -0.6)
        var inscope vv <- f16_fill(nrows * d, 3, 0.05, -1.1)
        var inscope want : array<float>
        with_dasllama_jobque_() {
            attention_rows(q, k, vv, int64(nrows), int64(d), int64(heads), want)
        }
        let pq = mc |> in_(q)
        let pv = mc |> in_(vv)
        let po = mc |> out_words(nrows * d, -1.0e30)
        let pk = mc |> in_(k)
        for (leg in range(2)) {
            if (leg == 1) {
                k[(nrows / 2) * d + 70] += 3.0   //! a key of the second head: every query row's softmax moves
                mc |> plane_upload(pk, k)
            }
            var ka = St2AttnArgs(t = uint(nrows), d = uint(d), hs = uint(hs), heads = uint(heads))
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[pq], mc.buf[pk], mc.buf[pv], mc.buf[po], ka, int64(heads * nrows))
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[po], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = d, head_width = hs))
            } else {
                t |> success(mc |> rerun_mismatches(po, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned key reds the compare")
            }
        }
    }
}


//! 10 rows of 40 from column 8, rows 2.., of a [.][48] source into a [.][64] target at row 3, column 5; the target's
//! other cells keep their sentinel - the copy touches its rows and nothing else
def private pk_rows_gate(t : T?; var dev, queue; elu : bool) {
    let tag = elu ? "pk_rows_elu" : "pk_rows"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = elu ? stamp_of(@@enc_pk_rows_elu) : stamp_of(@@enc_pk_rows)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let rows = 10
        let width = 40
        let ka = GkPkRowsArgs(rows = uint(rows), width = uint(width), ss = 48u, sc = 8u, sr0 = 2u, ds = 64u, dc = 5u, dr0 = 3u)
        var inscope x <- f16_fill((2 + rows) * 48, 3, 0.5, -1.0)
        var inscope want : array<float>
        want |> resize((3 + rows) * 64)
        for (w in want) {
            w = -7.0
        }
        for (r in range(rows)) {
            for (j in range(width)) {
                let v = x[(2 + r) * 48 + 8 + j]
                want[(3 + r) * 64 + 5 + j] = elu ? (v > 0.0 ? v : exp(v) - 1.0) : v
            }
        }
        let run <- @(bxx : MetalBuffer?; byy : MetalBuffer?) : bool {
            var kl = ka
            return queue_record(t, queue, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, bxx, byy, kl, int64(rows * width))
                }
            }
        }
        let px = mc |> in_(x)
        let py = mc |> out_words(length(want), -7.0)
        if (invoke(run, mc.buf[px], mc.buf[py])) {
            check_buf(t, tag, mc.buf[py], want, Bar(rel = 1e-6, abs_floor = 1e-7, poison_scaled = true), Shape(row_width = 64))
            t |> success(want[(3 + 1) * 64 + 5 + 4] != -7.0, "{tag}: the copied cell left the sentinel")
        }
        x[(2 + 1) * 48 + 8 + 4] += 1.5
        let px2 = mc |> in_(x)
        let py2 = mc |> out_words(length(want), -7.0)
        if (invoke(run, mc.buf[px2], mc.buf[py2])) {
            t |> success(mc |> rerun_mismatches(py2, want, 1e-6, 1e-7, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned source cell reds the compare")
        }
    }
}

//! x [9][48] *= s[48], the scale read 16 floats into its buffer (the slab offset)
def private pk_row_scale_gate(t : T?; var dev, queue) {
    let tag = "pk_row_scale"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_pk_row_scale)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let tt = 9
        let c = 48
        var inscope x <- f16_fill(tt * c, 5, 0.3, -0.7)
        var inscope s <- f16_fill(16 + c, 11, 0.1, 0.5)
        var inscope want : array<float>
        want |> resize(tt * c)
        for (i in range(tt * c)) {
            want[i] = x[i] * s[16 + i % c]
        }
        let ps = mc |> in_(s)
        var bs = mc.buf[ps]
        var ka = GkBiasActArgs(d = uint(c), nelem = uint(tt * c), boff = 16u, act = BIAS_ACT_NONE)   // nolint:LINT003 - the dispatch's args seat wants a non-ref value
        let run <- @(bxx : MetalBuffer?) : bool {
            return queue_record(t, queue, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, bxx, bs, ka, int64((tt * c + 3) / 4))
                }
            }
        }
        t |> success(mismatch_exact(want, x) > 0, "{tag}: the reference moves off the input")
        let px = mc |> io_(x)
        if (invoke(run, mc.buf[px])) {
            check_buf(t, tag, mc.buf[px], want, Bar(rel = 1e-6, abs_floor = 1e-7, poison_scaled = true), Shape(row_width = c))
        }
        x[c + 7] += 1.5
        let px2 = mc |> io_(x)
        if (invoke(run, mc.buf[px2])) {
            t |> success(mc |> rerun_mismatches(px2, want, 1e-6, 1e-7, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned row reds the compare")
        }
    }
}

//! rows [7][96] rotated over 64 columns from column 16 (two heads of 32) at positions 5..: the CPU's `rope_rows` on the same columns
def private rope_stride_gate(t : T?; var dev, queue) {
    let tag = "rope stride"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_pso(pso_stamp(dev, metal_rope_msl, err))
        return if (!pso_ok(t, tag, pso, err))
        let tt = 7
        let stride = 96
        let col = 16
        let d = 64
        let dh = 32
        let pos0 = 5
        let period = 10000.0
        var inscope x <- f16_fill(tt * stride, 13, 0.4, -0.9)
        var inscope y : array<float>
        y |> resize(tt * d)
        for (r in range(tt)) {
            for (j in range(d)) {
                y[r * d + j] = x[r * stride + col + j]
            }
        }
        rope_rows(y, int64(tt), int64(d), int64(dh), int64(pos0), period)
        var inscope want := x
        for (r in range(tt)) {
            for (j in range(d)) {
                want[r * stride + col + j] = y[r * d + j]
            }
        }
        var inscope none, tcos, tsin : array<float>
        build_rope_tabs(tcos, tsin, period, 1.0, 1.0, none, false, 0l, int64(pos0 + tt), int64(dh))
        let pcos = mc |> in_(tcos)
        let psin = mc |> in_(tsin)
        let pn = mc |> in_plain([uint(d)])
        let phs = mc |> in_plain([uint(dh)])
        let pnp = mc |> in_plain([uint(tt * d / 2)])
        let pneox = mc |> in_plain([0u])
        let pstride = mc |> in_plain([uint(stride)])
        var bcos = mc.buf[pcos]
        var bsin = mc.buf[psin]
        var u_n = mc.buf[pn]
        var u_hs = mc.buf[phs]
        var u_np = mc.buf[pnp]
        var u_neox = mc.buf[pneox]
        var u_stride = mc.buf[pstride]
        let toff = uint64(pos0 * (dh / 2) * 4)
        let run <- @(bxx : MetalBuffer?) : bool {
            return dispatch_64_lanes(t, queue, tag, pso, tt * d / 2) $(enc) {
                // binds by number: the tables bind at the pos0 row, an offset MetalRope's tcos / tsin carry no @off for
                metal_set_buffer(enc, bxx, uint64(col * 4), 0)
                metal_set_buffer(enc, bcos, toff, 1)
                metal_set_buffer(enc, bsin, toff, 2)
                metal_set_buffer(enc, u_n, 0ul, 3)
                metal_set_buffer(enc, u_hs, 0ul, 4)
                metal_set_buffer(enc, u_np, 0ul, 5)
                metal_set_buffer(enc, u_neox, 0ul, 6)
                metal_set_buffer(enc, u_hs, 0ul, 7)
                metal_set_buffer(enc, u_stride, 0ul, 8)
            }
        }
        let px = mc |> io_(x)
        if (invoke(run, mc.buf[px])) {
            check_buf(t, tag, mc.buf[px], want, Bar(rel = 1e-6, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = stride))
            t |> success(want[2 * stride + col + 3] != x[2 * stride + col + 3], "{tag}: a rotated column moved")
            t |> success(want[2 * stride + 3] == x[2 * stride + 3], "{tag}: a column outside the span stayed")
        }
        x[3 * stride + col + 10] += 0.5
        let px2 = mc |> io_(x)
        if (invoke(run, mc.buf[px2])) {
            t |> success(mc |> rerun_mismatches(px2, want, 1e-6, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned element reds the compare")
        }
    }
}

//! 12 new query rows at positions 10.. over a 22-key cache, two heads of 64, the last `ctx` keys
//! each (0 = every key): `attention_causal_rows` over a `TtsKvCache` of the same keys is the oracle
def private pk_attn_gate(t : T?; var dev, queue; ctx, n_old : int) {
    let tag = "pk_attn ctx{ctx} old{n_old}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_pk_attn)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let heads = 2
        let hs = PK_ATTN_HS
        let d = heads * hs
        let tt = 12
        let n = n_old + tt
        let qs = 160
        let qcol = 16
        var inscope q <- f16_fill(tt * qs, 7, 0.05, -0.6)
        var inscope k <- f16_fill(n * d, 5, 0.06, -0.7)
        var inscope vv <- f16_fill(n * d, 3, 0.08, -1.1)
        var inscope qn : array<float>
        qn |> resize(tt * d)
        for (r in range(tt)) {
            for (j in range(d)) {
                qn[r * d + j] = q[r * qs + qcol + j]
            }
        }
        var inscope kc = TtsKvCache()
        kv_cache_init(kc, int64(heads), int64(hs), int64(n))
        kv_cache_append(kc, k, vv, int64(n), int64(d))
        var inscope want : array<float>
        with_dasllama_jobque_() {
            attention_causal_rows(qn, int64(tt), int64(d), int64(ctx), kc, want)
        }
        let ka = GkPkAttnArgs(t = uint(tt), qs = uint(qs), qcol = uint(qcol), d = uint(d), pos0 = uint(n_old), ctx = uint(ctx))
        let pq = mc |> in_(q)
        let pv = mc |> in_(vv)
        var bq = mc.buf[pq]
        var bv = mc.buf[pv]
        let run <- @(bkk : MetalBuffer?; bo : MetalBuffer?) : bool {
            var kl = ka
            return queue_record(t, queue, "{tag} dispatch") $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, bq, bkk, bv, bo, kl, int64(heads * tt))
                }
            }
        }
        let bar = Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true)
        let pk = mc |> in_(k)
        let po = mc |> out_words(tt * d, -1.0e30)
        if (invoke(run, mc.buf[pk], mc.buf[po])) {
            check_buf(t, tag, mc.buf[po], want, bar, Shape(row_width = d, head_width = hs))
        }
        if (ctx > 0 && ctx < n_old) {
            k[0 * d + 3] += 3.0   //! the first key: outside every query's window, so nothing moves
            let pk1 = mc |> in_(k)
            let po1 = mc |> out_words(tt * d, -1.0e30)
            if (invoke(run, mc.buf[pk1], mc.buf[po1])) {
                check_buf(t, "{tag} unseen key", mc.buf[po1], want, bar, Shape(row_width = d, head_width = hs))
            }
        }
        k[(n - 1) * d + 70] += 3.0   //! the newest key, in the last query's window: its row moves
        let pk2 = mc |> in_(k)
        let po2 = mc |> out_words(tt * d, -1.0e30)
        if (invoke(run, mc.buf[pk2], mc.buf[po2])) {
            t |> success(mc |> rerun_mismatches(po2, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned key in the window reds the compare")
        }
    }
}

//! one row of q, k, v at position 5 over a 64-wide model of two heads: q and k rotated as
//! `rope_rows` rotates them, the rotated k and the v landing on the caches' row 5
//! MetalRopeStoreKvT's f32 stamp at the frame loop's binds: neox 0, no bias, the whole head, the tables and the caches at the position's row
def private rope_store_pocket_gate(t : T?; var dev, queue) {
    let tag = "rope_store f32 pocket"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_rpst32_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let d = 64
        let dh = 32
        let pos = 5
        let cap = 8
        let period = 10000.0
        var inscope qkv <- f16_fill(3 * d, 13, 0.4, -0.9)
        var inscope q, k : array<float>
        q |> resize(d)
        k |> resize(d)
        for (j in range(d)) {
            q[j] = qkv[j]
            k[j] = qkv[d + j]
        }
        rope_rows(q, 1l, int64(d), int64(dh), int64(pos), period)
        rope_rows(k, 1l, int64(d), int64(dh), int64(pos), period)
        var inscope want_qkv := qkv
        var inscope want_k, want_v : array<float>
        want_k |> resize(cap * d)
        want_v |> resize(cap * d)
        for (i in range(cap * d)) {
            want_k[i] = -7.0
            want_v[i] = -7.0
        }
        for (j in range(d)) {
            want_qkv[j] = q[j]
            want_k[pos * d + j] = k[j]
            want_v[pos * d + j] = qkv[2 * d + j]
        }
        var inscope none, tcos, tsin : array<float>
        build_rope_tabs(tcos, tsin, period, 1.0, 1.0, none, false, 0l, int64(cap), int64(dh))
        let ka = RopeStoreArgs(qd = uint(d), head_size = uint(dh), npairs_q = uint(d / 2), npairs = uint(d), neox = 0u, has_bias = 0u, rot = uint(dh))
        let pcos = mc |> in_(tcos)
        let psin = mc |> in_(tsin)
        let pbias = mc |> in_(plane_of(3 * d, 0.0))
        var bcos = mc.buf[pcos]
        var bsin = mc.buf[psin]
        var bbias = mc.buf[pbias]
        let toff = uint64(pos * (dh / 2) * 4)
        let roff = uint64(pos * d * 4)
        let run <- @(bxx : MetalBuffer?; bkk : MetalBuffer?; bvv : MetalBuffer?) : bool {
            var kl = ka
            return queue_record(t, queue, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {   // v from the fused row's third, the caches and the tables at the position's row
                    invoke(st.enc, enc, bxx, bxx, uint64(2 * d * 4), bkk, roff, bvv, roff, bcos, toff, bsin, bbias, kl, int64(d))
                }
            }
        }
        let bar = Bar(rel = 1e-6, abs_floor = 1e-6, poison_scaled = true)
        let px = mc |> io_(qkv)
        let pk = mc |> out_words(cap * d, -7.0)
        let pv = mc |> out_words(cap * d, -7.0)
        if (invoke(run, mc.buf[px], mc.buf[pk], mc.buf[pv])) {
            check_buf(t, "{tag} q", mc.buf[px], want_qkv, bar, Shape(row_width = d, head_width = dh))
            check_buf(t, "{tag} k", mc.buf[pk], want_k, bar, Shape(row_width = d, head_width = dh))
            check_buf(t, "{tag} v", mc.buf[pv], want_v, bar, Shape(row_width = d, head_width = dh))
            t |> success(want_k[pos * d + 3] != qkv[d + 3], "{tag}: the stored k is the rotated one")
            t |> success(want_k[(pos - 1) * d + 3] == -7.0, "{tag}: the row before stayed a sentinel")
        }
        qkv[d + 10] += 0.5
        let px2 = mc |> io_(qkv)
        let pk2 = mc |> out_words(cap * d, -7.0)
        let pv2 = mc |> out_words(cap * d, -7.0)
        if (invoke(run, mc.buf[px2], mc.buf[pk2], mc.buf[pv2])) {
            t |> success(mc |> rerun_mismatches(pk2, want_k, 1e-6, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned k element reds the stored row")
        }
        qkv[d + 10] -= 0.5
        qkv[10] += 0.5
        qkv[2 * d + 5] += 0.5
        let px3 = mc |> io_(qkv)
        let pk3 = mc |> out_words(cap * d, -7.0)
        let pv3 = mc |> out_words(cap * d, -7.0)
        if (invoke(run, mc.buf[px3], mc.buf[pk3], mc.buf[pv3])) {
            t |> success(mc |> rerun_mismatches(px3, want_qkv, 1e-6, 1e-6, "{tag} q poison (MUST mismatch)") > 0, "{tag}: a poisoned q element reds the rotated q")
            t |> success(mc |> rerun_mismatches(pv3, want_v, 1e-6, 1e-6, "{tag} v poison (MUST mismatch)") > 0, "{tag}: a poisoned v element reds the stored v")
        }
    }
}

//! the row GEMV stamp's kernel of a weight form and epilogue kind: 0 the bare dot, 1 the normed SiLU, 2 the gated
//! residual, 3 the SiLU over the slab vector, 4 the frame tail
def private pk_gemv_kernel(q8; kind) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (q8) {
        return (kind == 1 ? kernel_of(@@enc_pk_gemv_q8_ln_silu).make : (kind == 2 ? kernel_of(@@enc_pk_gemv_q8_gate).make
            : (kind == 3 ? kernel_of(@@enc_pk_gemv_q8_add_silu).make : kernel_of(@@enc_pk_gemv_q8_tail).make)))
    }
    return (kind == 0 ? kernel_of(@@enc_pk_gemv_f32).make : (kind == 1 ? kernel_of(@@enc_pk_gemv_f32_ln_silu).make
        : (kind == 2 ? kernel_of(@@enc_pk_gemv_f32_gate).make : (kind == 3 ? kernel_of(@@enc_pk_gemv_f32_add_silu).make : kernel_of(@@enc_pk_gemv_f32_tail).make))))
}

//! the row GEMV over 9 rows of 96 (three q8 blocks; the f32 rows at stride 128) under one stamp:
//! the bare dot, the normed and modulated x with the SiLU out, the gated residual, the SiLU over
//! the slab vector, and the frame tail - x, y and the latent row bound at offsets, an fp64 oracle
def private pk_gemv_gate(t : T?; var dev, queue; q8 : bool; kind, nin : int) {
    let tag = "pk_gemv {q8 ? "q8" : "f32"} kind{kind} nin{nin}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_pso(invoke(pk_gemv_kernel(q8, kind), dev, err))
        return if (!pso_ok(t, tag, pso, err))
        let nout = 9
        let stride = (nin + 127) / 128 * 128
        let eps = 1e-6
        let xoff = 4
        let yoff = 3
        let loff = 8
        let bar = 2e-7 * float(nin)   //! nin products under 0.25 each summed at f32 (6e-8 an ulp) against the fp64 row: the sum's rounding under nin x 1.5e-8, the bar ten times that
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(nout, max(nin / 32, 1), blob, wdeq)
        if (!q8) {
            delete wdeq
            wdeq <- f16_fill(nout * nin, 29, 0.5, -0.2)   // the f32 rows take any width; the q8 blob needs whole blocks
        }
        var inscope wf : array<float>
        wf |> resize(nout * stride)
        for (r in range(nout)) {
            for (j in range(nin)) {
                wf[r * stride + j] = wdeq[r * nin + j]
            }
        }
        var inscope b <- f16_fill(nout, 5, 0.2, -0.1)
        let sc = sqrt(96.0 / float(nin))   //! the dot's spread held at the 96-wide case's, so the SiLU epilogues stay off their flat tail and the poison shows
        var inscope x <- f16_fill(xoff + nin, 7, 0.5 * sc, -0.3 * sc)
        var inscope lw <- f16_fill(nin, 3, 0.3 * sc, 0.9 * sc)
        var inscope lb <- f16_fill(nin, 11, 0.2, -0.1)
        var inscope md <- f16_fill(3 * nin, 13, 0.4, 0.1)
        var inscope aux <- f16_fill(2 * nout, 17, 0.3, 0.2)
        var inscope x0 <- f16_fill(nout, 19, 0.5, -0.4)
        var inscope y0 <- f16_fill(yoff + nout, 23, 0.6, 0.3)
        var inscope xs : array<float>
        xs |> resize(nin)
        for (j in range(nin)) {
            xs[j] = x[xoff + j]
        }
        if (kind == 1 || kind == 4) {
            var inscope ln : array<float>
            ln |> resize(nin)
            unsafe {
                layernorm(addr(ln[0]), addr<float const?>(xs[0]), addr<float const?>(lw[0]), addr<float const?>(lb[0]), int64(nin), eps)
            }
            for (j in range(nin)) {
                xs[j] = ln[j] * (1.0 + md[nin + j]) + md[j]
            }
        }
        var inscope want_y := y0
        var inscope want_lat : array<float>
        want_lat |> resize(loff + nout)
        for (i in range(loff + nout)) {
            want_lat[i] = -7.0
        }
        for (r in range(nout)) {
            var acc = 0.0lf
            for (j in range(nin)) {
                acc += double(wdeq[r * nin + j]) * double(xs[j])
            }
            let dd = acc + double(b[r])
            if (kind == 0) {
                want_y[yoff + r] = float(dd)
            } elif (kind == 1) {
                want_y[yoff + r] = float(silu_d(dd))
            } elif (kind == 2) {
                want_y[yoff + r] = float(double(y0[yoff + r]) + double(md[2 * nin + r]) * dd)
            } elif (kind == 3) {
                want_y[yoff + r] = float(silu_d(dd + double(aux[r])))
            } else {
                let v = dd + double(x0[r])
                want_y[yoff + r] = float(v)
                want_lat[loff + r] = float(v * double(aux[r]) + double(aux[nout + r]))
            }
        }
        let ka = PkGemvArgs(nin = uint(nin), nout = uint(nout), stride = uint(stride), eps = eps)
        let pw = mc |> in_(wf)
        let pb = mc |> in_(b)
        let pq = mc |> in_plain(blob)
        let pmod = mc |> in_(md)
        let plw = mc |> in_(lw)
        let plb = mc |> in_(lb)
        let paux = mc |> in_(aux)
        let px0 = mc |> in_(x0)
        var bw = mc.buf[pw]
        var bb = mc.buf[pb]
        var bq = mc.buf[pq]
        var bmod = mc.buf[pmod]
        var blw = mc.buf[plw]
        var blb = mc.buf[plb]
        var baux = mc.buf[paux]
        var bx0 = mc.buf[px0]
        let run <- @(bxx : MetalBuffer?; byy : MetalBuffer?; blat : MetalBuffer?) : bool {
            var kl = ka
            let (xo, yo, lo, rows) = (uint64(xoff * 4), uint64(yoff * 4), uint64(loff * 4), int64(nout))
            return queue_record(t, queue, "{tag} dispatch") $(enc : MetalComputeEncoder?) {
                if (!q8 && kind == 0) {
                    let st = stamp_of(@@enc_pk_gemv_f32)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bw, 0ul, bb, 0ul, bxx, xo, byy, yo, kl, rows)
                    }
                } elif (!q8 && kind == 1) {
                    let st = stamp_of(@@enc_pk_gemv_f32_ln_silu)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bw, 0ul, bb, 0ul, bxx, xo, byy, yo, bmod, 0ul, blw, 0ul, blb, 0ul, kl, rows)
                    }
                } elif (!q8 && kind == 2) {
                    let st = stamp_of(@@enc_pk_gemv_f32_gate)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bw, 0ul, bb, 0ul, bxx, xo, byy, yo, bmod, 0ul, kl, rows)
                    }
                } elif (!q8 && kind == 3) {
                    let st = stamp_of(@@enc_pk_gemv_f32_add_silu)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bw, 0ul, bb, 0ul, bxx, xo, byy, yo, baux, 0ul, kl, rows)
                    }
                } elif (!q8) {
                    let st = stamp_of(@@enc_pk_gemv_f32_tail)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bw, 0ul, bb, 0ul, bxx, xo, byy, yo, bmod, 0ul, blw, 0ul, blb, 0ul, baux, 0ul, bx0, 0ul, blat, lo, kl, rows)
                    }
                } elif (kind == 1) {   // the q8 forms: one blob, its half-scale and byte views
                    let st = stamp_of(@@enc_pk_gemv_q8_ln_silu)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bb, 0ul, bq, 0ul, bq, bxx, xo, byy, yo, bmod, 0ul, blw, 0ul, blb, 0ul, kl, rows)
                    }
                } elif (kind == 2) {
                    let st = stamp_of(@@enc_pk_gemv_q8_gate)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bb, 0ul, bq, 0ul, bq, bxx, xo, byy, yo, bmod, 0ul, kl, rows)
                    }
                } elif (kind == 3) {
                    let st = stamp_of(@@enc_pk_gemv_q8_add_silu)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bb, 0ul, bq, 0ul, bq, bxx, xo, byy, yo, baux, 0ul, kl, rows)
                    }
                } else {
                    let st = stamp_of(@@enc_pk_gemv_q8_tail)
                    stamp_run(st, pso) {
                        invoke(st.enc, enc, bb, 0ul, bq, 0ul, bq, bxx, xo, byy, yo, bmod, 0ul, blw, 0ul, blb, 0ul, baux, 0ul, bx0, 0ul, blat, lo, kl, rows)
                    }
                }
            }
        }
        let gbar = Bar(rel = bar, abs_floor = 1e-6, poison_scaled = true)
        let px = mc |> in_(x)
        let py = mc |> io_(y0)
        let pl = mc |> out_words(loff + nout, -7.0)
        if (invoke(run, mc.buf[px], mc.buf[py], mc.buf[pl])) {
            check_buf(t, "{tag} y", mc.buf[py], want_y, gbar, FLAT_SHAPE)
            check_buf(t, "{tag} lat", mc.buf[pl], want_lat, gbar, FLAT_SHAPE)
            t |> success(want_y[yoff - 1] == y0[yoff - 1], "{tag}: the row under the offset stayed")
        }
        for (j in range(nin)) {   //! every element moved 4 along the first row's weight signs: the row's dot rises by four times the weights' magnitude sum, more than any input under 1 can pull it down, so it lands on the SiLU's live side whatever the fixture's dot was
            x[xoff + j] += wdeq[j] > 0.0 ? 4.0 : -4.0
        }
        let px2 = mc |> in_(x)
        let py2 = mc |> io_(y0)
        let pl2 = mc |> out_words(loff + nout, -7.0)
        if (invoke(run, mc.buf[px2], mc.buf[py2], mc.buf[pl2])) {
            t |> success(mc |> rerun_mismatches(py2, want_y, bar, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned x element reds the compare")
            if (kind == 4) {
                t |> success(mc |> rerun_mismatches(pl2, want_lat, bar, 1e-6, "{tag} lat poison (MUST mismatch)") > 0, "{tag}: a poisoned x element reds the latent row")
            }
        }
    }
}

//! the sigmoid sums over 50 of 64 columns at 1/1.3 speed
def private st2_sigsum_gate(t : T?; var dev, queue) {
    let tag = "st2_sigsum"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_sigsum)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nrows = 37
        let nb = 50
        let ls = 64
        var inscope lg <- f16_fill(nrows * ls, 7, 0.3, -5.0)
        var inscope want : array<float>
        want |> resize(nrows)
        for (r in range(nrows)) {
            var acc = 0.0lf
            for (j in range(nb)) {
                acc += sigmoid_d(double(lg[r * ls + j]))
            }
            want[r] = float(acc) * (1.0 / 1.3)
        }
        let praw = mc |> out_words(nrows, -1.0e30)
        let plg = mc |> in_(lg)
        var ka = GkSigSumArgs(t = uint(nrows), nb = uint(nb), ls = uint(ls), speed = 1.3)
        for (leg in range(2)) {
            if (leg == 1) {
                lg[5 * ls + 7] += 6.0
                mc |> plane_upload(plg, lg)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[plg], mc.buf[praw], ka, int64(nrows))
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[praw], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), FLAT_SHAPE)
            } else {
                t |> success(mc |> rerun_mismatches(praw, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned logit reds the compare")
            }
        }
    }
}

//! the tanh-form GELU in place, a value past the clamp among the rows
def private st2_gelu_gate(t : T?; var dev, queue) {
    let tag = "st2_gelu_tanh"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_gelu_tanh)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let n = 216
        var inscope x <- f16_fill(n, 7, 0.25, -3.0)
        x[17] = 40.0
        x[18] = -40.0
        var inscope want := x
        gelu(want, int64(n))
        var bn = mc.buf[mc |> in_plain([uint(n)])]
        let run <- @(bxx : MetalBuffer?) : bool {
            return queue_record(t, queue, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, bxx, 0ul, bn, int64(n))
                }
            }
        }
        let px = mc |> io_(x)
        if (invoke(run, mc.buf[px])) {
            check_buf(t, tag, mc.buf[px], want, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), FLAT_SHAPE)
            t |> success(want[3] != x[3], "{tag}: the output differs from the input at [3]")
        }
        x[9] += 2.0
        let px2 = mc |> io_(x)
        if (invoke(run, mc.buf[px2])) {
            t |> success(mc |> rerun_mismatches(px2, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned element reds the compare")
        }
    }
}

def private st2_source_gate(t : T?; var dev, queue; torch_math : bool) {
    let tag = torch_math ? "st2_source torch law" : "st2_source onnx law"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let slow = torch_math ? stamp_of(@@enc_st2_src_low_torch) : stamp_of(@@enc_st2_src_low_onnx)
        let scum = torch_math ? stamp_of(@@enc_st2_src_cumsum_torch) : stamp_of(@@enc_st2_src_cumsum_onnx)
        let ssin = torch_math ? stamp_of(@@enc_st2_src_sines_torch) : stamp_of(@@enc_st2_src_sines_onnx)
        let snz = stamp_of(@@enc_st2_src_noise)
        var pso_low = mc |> keep_stamp(slow, err)
        let low_ok = pso_ok(t, "{tag} low", pso_low, err)
        var pso_cum = mc |> keep_stamp(scum, err)
        let cum_ok = pso_ok(t, "{tag} cumsum", pso_cum, err)
        var pso_sin = mc |> keep_stamp(ssin, err)
        let sin_ok = pso_ok(t, "{tag} sines", pso_sin, err)
        var pso_noise = mc |> keep_stamp(snz, err)
        return if (!pso_ok(t, "{tag} noise", pso_noise, err) || !low_ok || !cum_ok || !sin_ok)
        var inscope src <- tts_src_fixture(torch_math, 300, 700)   // 210000 samples: the phases past a hundred thousand radians, where a fast sin loses them
        let up = src.up
        let nh = TTS_SRC_NH
        let n = src.n
        let nlow = src.nlow
        var inscope f0 := src.f0
        let ka = GkSourceArgs(n = uint(n), nlow = uint(nlow), nh = uint(nh), up = uint(up), sr = TTS_SRC_SR, sine_amp = 0.1, noise_std = 0.003,
            voiced_thr = 10.0, seed = 77u, woff = 0u, boff = uint(length(src.lin_w)))
        t |> equal(src.rows, int64(n), "{tag}: the CPU source mixes every sample")
        let pnu = mc |> in_(src.nu)
        let pnoise = mc |> in_(src.noise)
        var inscope wn <- clone_to_move(src.lin_w)
        wn |> push_from(src.lin_b)
        let plw = mc |> in_(wn)
        let plow = mc |> out_words(nh * nlow, -1.0e30)
        let plowc = mc |> out_words(nh * nlow, -1.0e30)
        let phar = mc |> out_words(n, -1.0e30)
        var bnu = mc.buf[pnu]
        var bnoise = mc.buf[pnoise]
        var blw = mc.buf[plw]
        var blow = mc.buf[plow]
        var blowc = mc.buf[plowc]
        var bhar = mc.buf[phar]
        let run <- @(bf0 : MetalBuffer?; kax : GkSourceArgs; own : bool; bnz : MetalBuffer?; bhar : MetalBuffer?) : bool {
            var kk = kax
            return queue_record(t, queue, "{tag} dispatch") $(enc : MetalComputeEncoder?) {
                if (own) {   // the driver's own draw: the noise rows hashed on the device before the sines read them
                    stamp_run(snz, pso_noise) {
                        invoke(snz.enc, enc, bnz, kk, int64(n * nh))
                    }
                }
                stamp_run(slow, pso_low) {
                    invoke(slow.enc, enc, bf0, bnu, blow, kk, int64(nh * nlow))
                }
                stamp_run(scum, pso_cum) {
                    invoke(scum.enc, enc, blow, blowc, kk, int64(nh))
                }
                stamp_run(ssin, pso_sin) {
                    invoke(ssin.enc, enc, bf0, blowc, bnz, blw, bhar, kk, int64(n))
                }
            }
        }
        let pf0 = mc |> in_(f0)
        if (invoke(run, mc.buf[pf0], ka, false, bnoise, bhar)) {
            check_buf(t, "{tag} lowc (the phase frames, within one ulp of the CPU carry)", blowc, src.want_lowc, Bar(rel = 1.2e-7, poison_scaled = true), Shape(row_width = nlow))
            check_buf(t, "{tag} har", bhar, src.want_har, Bar(rel = 2e-6, abs_floor = 2e-7, poison_scaled = true), FLAT_SHAPE)
        }
        var inscope har_captured <- mc |> plane_run(phar)
        {   // the long cumsum: 40000 low-rate frames of one harmonic, where a plain f32 sum and a double one part by cycles
            let nlong = 40000
            var inscope lc <- tts_long_cumsum(torch_math, up, nlong)
            t |> success(lc.f32_drift_cycles > 0.01, "{tag} long cumsum: the two laws part by {lc.f32_drift_cycles} cycles over {nlong} frames (the leg's control)")
            let plowl = mc |> in_(lc.low)
            let plowcl = mc |> out_words(nlong, -1.0e30)
            var kl = ka
            kl.nlow = uint(nlong)
            kl.nh = 1u
            let ranl = mc |> cell_record(t, "{tag} long cumsum") $(enc : MetalComputeEncoder?) {
                stamp_run(scum, pso_cum) {
                    invoke(scum.enc, enc, mc.buf[plowl], mc.buf[plowcl], kl, 1l)
                }
            }
            if (ranl) {
                //! the control: two radians on the last frame - the bars are a few ulps of a phase in the thousands
                check_buf(t, "{tag} long cumsum", mc.buf[plowcl], lc.want, Bar(rel = 2e-7, abs_floor = 1e-3, poison = 2.0), FLAT_SHAPE)
            }
        }
        f0[4] += 40.0   //! a voiced frame: every later phase moves
        let pf02 = mc |> in_(f0)
        if (invoke(run, mc.buf[pf02], ka, false, bnoise, bhar)) {
            t |> success(buf_mismatches(blowc, src.want_lowc, 1.2e-7, 0.0, "{tag} lowc poison (MUST mismatch)") > 0, "{tag}: a poisoned f0 frame reds the lowc compare")
            t |> success(buf_mismatches(bhar, src.want_har, 2e-6, 2e-7, "{tag} har poison (MUST mismatch)") > 0, "{tag}: a poisoned f0 frame reds the har compare")
        }
        var ka_own = ka
        let pnz1 = mc |> out_words(n * nh, -1.0e30)
        let ph1 = mc |> out_words(n, -1.0e30)
        if (invoke(run, mc.buf[pf0], ka_own, true, mc.buf[pnz1], mc.buf[ph1])) {
            var inscope want_noise : array<float>
            want_noise |> resize(n * nh)
            for (i in range(n * nh)) {
                want_noise[i] = tts_hash_normal(ka_own.seed, uint(i))
            }
            check_buf(t, "{tag} own draw vs the CPU hash", mc.buf[pnz1], want_noise, Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true), Shape(row_width = n))   //! the kernel's log and cos under the default fast math against the CPU's: within 1e-5 of a value under 5
            var inscope own1 <- mc |> plane_run(ph1)
            var in_tanh_range = true
            for (v in own1) {
                in_tanh_range = in_tanh_range && abs(v) <= 1.0
            }
            t |> success(in_tanh_range, "{tag}: the driver's own noise draw stays within tanh's range")
            t |> success(mc |> rerun_mismatches(ph1, har_captured, 2e-6, 2e-7, "{tag} own draw vs captured (MUST mismatch)") > 0, "{tag}: the own draw differs from the captured rows")
            let pnz2 = mc |> out_words(n * nh, -1.0e30)
            let ph2 = mc |> out_words(n, -1.0e30)
            if (invoke(run, mc.buf[pf0], ka_own, true, mc.buf[pnz2], mc.buf[ph2])) {
                check_buf(t, "{tag} own draw repeat", mc.buf[ph2], own1, BAR_ZERO, FLAT_SHAPE)
            }
            for (other in [78u, 81u]) {   //! 81 = 77 xor 4: a raw-seed xor into the counter would alias it to a shuffled 77
                ka_own.seed = other
                let pnz3 = mc |> out_words(n * nh, -1.0e30)
                let ph3 = mc |> out_words(n, -1.0e30)
                if (invoke(run, mc.buf[pf0], ka_own, true, mc.buf[pnz3], mc.buf[ph3])) {
                    t |> success(mc |> rerun_mismatches(ph3, own1, 2e-6, 2e-7, "{tag} own draw seed {other} (MUST mismatch)") > 0, "{tag}: seed {other} draws other noise than seed 77")
                }
            }
        }
    }
}

//! the STFT as two convs over the padded signal on either pad law, magnitudes and phases
def private st2_stft_gate(t : T?; var dev, queue; reflect : bool) {
    let tag = reflect ? "st2_stft reflect" : "st2_stft edge"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = reflect ? stamp_of(@@enc_st2_stft_reflect) : stamp_of(@@enc_st2_stft_edge)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let n = 40
        let k = 8
        let hop = 4
        let pad = 2
        let bins = 3
        let frames = (n + 2 * pad - k) / hop + 1
        var inscope har <- f16_fill(n, 7, 0.05, -0.9)
        var inscope wre <- f16_fill(bins * k, 5, 0.2, -0.7)
        var inscope wim <- f16_fill(bins * k, 3, 0.15, -0.5)
        wim[2 * k + 5] = 0.0
        var inscope want <- stft_ref(har, wre, wim, n, k, hop, pad, bins, reflect)
        var inscope wn <- clone_to_move(wre)
        wn |> push_from(wim)
        let pwn = mc |> in_(wn)
        let pspec = mc |> out_words(frames * 2 * bins, -1.0e30)
        let ph = mc |> in_(har)
        for (leg in range(2)) {
            if (leg == 1) {
                har[1] += 2.0   //! the first frame's reflected tap reads it
                mc |> plane_upload(ph, har)
            }
            var ka = GkStftArgs(n = uint(n), frames = uint(frames), bins = uint(bins), k = uint(k), hop = uint(hop), pad = uint(pad), eps = 1e-9,
                reoff = 0u, imoff = uint(bins * k))
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[ph], mc.buf[pwn], mc.buf[pspec], ka, int64(frames * bins))
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[pspec], want, Bar(rel = 1e-4, abs_floor = 1e-5, poison_scaled = true), Shape(row_width = 2 * bins))
            } else {
                t |> success(mc |> rerun_mismatches(pspec, want, 1e-4, 1e-5, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned sample reds the compare")
            }
        }
    }
}

//! the inverse STFT from log-magnitude / phase rows against a CPU overlap-add twin, with and without the envelope
def private st2_istft_gate(t : T?; var dev, queue; envelope : bool) {
    let tag = envelope ? "st2_istft envelope" : "st2_istft plain"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_st2_istft)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let tt = 9
        let stride = 8
        let bins = 3
        let k = 8
        let hop = 4
        let pad = 2
        let n = (tt - 1) * hop + k - 2 * pad
        var inscope y <- f16_fill(tt * stride, 7, 0.15, -1.2)
        var inscope wre <- f16_fill(k * bins, 5, 0.2, -0.7)   // [k][bins], the transposed conv's operand layout
        var inscope wim <- f16_fill(k * bins, 3, 0.15, -0.5)
        var inscope window <- f16_fill(k, 11, 0.1, 0.3)
        var inscope ref <- istft_ref(y, wre, wim, window, tt, stride, bins, k, hop, pad, envelope)
        var inscope wn <- clone_to_move(wre)
        wn |> push_from(wim)
        wn |> push_from(window)
        let pwn = mc |> in_(wn)
        let pw = mc |> out_words(n, -1.0e30)
        let py = mc |> in_(y)
        for (leg in range(2)) {
            if (leg == 1) {
                y[4 * stride + 1] += 2.0   //! a log-magnitude: every sample its frame overlaps moves
                mc |> plane_upload(py, y)
            }
            var ka = GkIstftArgs(tt = uint(tt), stride = uint(stride), bins = uint(bins), k = uint(k), hop = uint(hop), pad = uint(pad),
                envelope = envelope ? 1u : 0u, n = uint(n), reoff = 0u, imoff = uint(k * bins), wnoff = uint(2 * k * bins))
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {
                    invoke(st.enc, enc, mc.buf[py], mc.buf[pwn], mc.buf[pw], ka, int64(n))
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[pw], ref.want, Bar(rel = 1e-4, abs_floor = 1e-5, poison_scaled = true), FLAT_SHAPE)
            } else {
                t |> success(mc |> rerun_mismatches(pw, ref.want, 1e-4, 1e-5, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned magnitude reds the compare")
            }
        }
    }
}

// ===== the tower row gather (the qwen25v stem's window-sort reorder) =====
//! The stem's window-sort reorder: MetalRowGather with both adds off over a full permutation
def private tower_gather_gate(t : T?; var dev, queue) {
    let tag = "tower_gather"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_row_gather)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let nr = 37
        let d = 64
        var inscope xv : array<float>
        xv |> resize(nr * d)
        for (i in range(nr * d)) {
            xv[i] = 0.03 * float((i * 17) % 61) - 0.9
        }
        var inscope src : array<uint>
        src |> resize(nr)
        for (r in range(nr)) {
            src[r] = uint((r * 11) % nr)   //! 11 coprime to 37: a full permutation
        }
        var inscope want : array<float>
        want |> resize(nr * d)
        for (r in range(nr)) {
            for (c in range(d)) {
                want[r * d + c] = xv[int(src[r]) * d + c]
            }
        }
        let px = mc |> in_(xv)
        let ps = mc |> in_plain(src)
        let py = mc |> out_words(nr * d, -1.0e30)
        var ka = GkRowGatherArgs(c = uint(d), rows = uint(nr), aoff = 0u, boff = 0u, coff = 0u, out_off = 0u, add_b = 0u, add_c = 0u, scale = 1.0)
        for (leg in range(2)) {
            if (leg == 1) {
                src[nr / 2] = src[nr / 2] == 0u ? 1u : 0u
                mc |> plane_upload(ps, src)
            }
            let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
                stamp_run(st, pso) {   // both adds off: x stands in for the unread b and c
                    invoke(st.enc, enc, mc.buf[px], mc.buf[ps], mc.buf[px], mc.buf[px], mc.buf[py], ka, int64(nr * d))
                }
            }
            continue if (!ran)
            if (leg == 0) {
                check_buf(t, tag, mc.buf[py], want, BAR_EXACT, Shape(row_width = d))
            } else {
                var inscope got <- mc |> plane_run(py)
                t |> success(mismatch_exact(got, want) > 0,
                    "{tag}: a swapped gather index reds the exact compare")
            }
        }
    }
}

//! one arm of the table-driven rope at the q width: the input pattern, the table rows' step and period, the pairing
//! and the rotated width (rot < HEAD_SIZE = partial NEOX)
struct private RopeArm {
    tag : string
    neox : bool
    rot : int
    xscale : float
    xmod : int
    xbias : float
    tstep : float
    tmod : int
}

//! MetalRope over NPOS rows of QD against `rope_scaled_tab` / `_neox_tab` / `_neox_tab_part` - the arm's pairing and width
def private rope_arm_gate(t : T?; var dev, queue; pso; arm : RopeArm) {
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        let half = arm.rot / 2
        var inscope rv, rcos, rsin : array<float>
        rv |> resize(NPOS * QD)
        rcos |> resize(NPOS * half)
        rsin |> resize(NPOS * half)
        for (k in range(NPOS * QD)) {
            rv[k] = arm.xscale * float(k % arm.xmod) + arm.xbias
        }
        for (k in range(NPOS * half)) {
            rcos[k] = cos(arm.tstep * float(k % arm.tmod))
            rsin[k] = sin(arm.tstep * float(k % arm.tmod))
        }
        var inscope rvwant := rv
        for (p in range(NPOS)) {
            rope_tab_ref(rvwant, p * QD, HEAD_SIZE, arm.rot, QD, arm.neox, rcos, rsin, p * half)
        }
        let brv = mc |> io_(rv)
        let brc = mc |> in_(rcos)
        let brs = mc |> in_(rsin)
        let brn = mc |> in_plain([uint(QD)])
        let brhs = mc |> in_plain([uint(HEAD_SIZE)])
        let brnp = mc |> in_plain([uint(NPOS * QD / 2)])
        let brnx = mc |> in_plain([arm.neox ? 1u : 0u])
        let brot = mc |> in_plain([uint(arm.rot)])
        mc |> cell_record(t, arm.tag) $(enc : MetalComputeEncoder?) {   // brn again: the row stride (contiguous rows)
            enc_rope_pso(enc, pso, mc.buf[brv], 0ul, mc.buf[brc], mc.buf[brs], mc.buf[brn], mc.buf[brhs], mc.buf[brnp], mc.buf[brnx], mc.buf[brot], mc.buf[brn], int64(NPOS * QD / 2))
        }
        check_buf(t, arm.tag, mc.buf[brv], rvwant, BAR_FP, Shape(row_width = QD, head_width = HEAD_SIZE))
    }
}

//! One `_metal_ew2_msl` stamp `st`, y's op into x in place, against its CPU op: `approx` bars, or bit-exact where the op is
//! one rounding on both sides. `bind(enc, x, y, total plane, ascale plane, total)` calls the stamp's builder - the ascale
//! plane holds 1.0, the identity, for a builder that takes it.
def private ew2_gate(t : T?; var dev, queue; tag : string; st; x, y, want : array<float>; exact : bool; bind) : array<float> {
    var got : array<float>
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let total = length(x)
        let bx = mc |> io_(x)
        let by = mc |> in_(y)
        let btot = mc |> in_plain([uint(total)])
        let bone = mc |> in_plain([1.0])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(bind, enc, mc.buf[bx], mc.buf[by], mc.buf[btot], mc.buf[bone], int64(total))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[bx], want, exact ? BAR_EXACT : BAR_FP, FLAT_SHAPE)
        got <- mc |> plane_run(bx)
    }
    return <- got
}

[test]
def test_metal_prefill_kernels(t : T?) {
    t |> run("phase-6 prefill kernels == dasLLAMA CPU twins") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_gate(t) $(var dev : MetalDevice?; var queue : MetalCommandQueue?) {
                // ===== rmsnorm =====
                with_metal_cell(dev, queue) $(var mc : MetalCell) {
                    var err : string
                    let srms = stamp_of(@@pf_enc_rms)
                    var pso_rms = mc |> keep_stamp(srms, err)
                    return if (!pso_ok(t, "rmsnorm", pso_rms, err))
                    var inscope rx : array<float>
                    var inscope rw : array<float>
                    var inscope rwant : array<float>
                    rx |> resize(NROWS * DIM)
                    rw |> resize(DIM)
                    rwant |> resize(NROWS * DIM)
                    for (k in range(NROWS * DIM)) {
                        rx[k] = 0.25 * float(k % 29) - 3.4
                    }
                    for (k in range(DIM)) {
                        rw[k] = 0.5 + 0.05 * float(k % 13)
                    }
                    unsafe {
                        for (r in range(NROWS)) {
                            rmsnorm(addr(rwant[r * DIM]), addr<float const?>(rx[r * DIM]),
                                addr<float const?>(rw[0]), int64(DIM), EPS)
                        }
                    }
                    let brx = mc |> in_(rx)
                    let brw = mc |> in_(rw)
                    let bry = mc |> out_(NROWS * DIM)
                    let brdim = mc |> in_plain([uint(DIM)])
                    let breps = mc |> in_plain([EPS])
                    let rran = mc |> cell_record(t, "rmsnorm") $(enc : MetalComputeEncoder?) {
                        stamp_run(srms, pso_rms) {
                            invoke(srms.enc, enc, mc.buf[brx], 0ul, mc.buf[brw], mc.buf[bry], mc.buf[brdim], mc.buf[breps], int64(NROWS))
                        }
                    }
                    return if (!rran)
                    check_buf(t, "rmsnorm", mc.buf[bry], rwant, BAR_FP, Shape(row_width = DIM))
                }

                with_metal_cell(dev, queue) $(var mc : MetalCell) {
                    var err : string
                    var pso_rope = mc |> keep_stamp(kernel_of(@@enc_rope), err)
                    return if (!pso_ok(t, "rope", pso_rope, err))
                    let rope_arms <- [
                        RopeArm(tag = "rope", neox = false, rot = HEAD_SIZE, xscale = 0.11, xmod = 41, xbias = -2.2, tstep = 0.01, tmod = 97),
                        RopeArm(tag = "rope NEOX", neox = true, rot = HEAD_SIZE, xscale = 0.13, xmod = 37, xbias = -1.9, tstep = 0.01, tmod = 97),
                        RopeArm(tag = "rope partial-NEOX", neox = true, rot = HEAD_SIZE / 2, xscale = 0.17, xmod = 33, xbias = -2.4, tstep = 0.02, tmod = 89)
                    ]
                    for (arm in rope_arms) {
                        rope_arm_gate(t, dev, queue, pso_rope, arm)
                    }
                }

                // ===== the fused qk_norm+rope kernel: every (hx, donorm, pairing) production arm =====
                qk_rope_gate(t, dev, queue, false, true, true, false)    //! K rows, qwen3 class
                qk_rope_gate(t, dev, queue, true, true, true, false)     //! q panel + twin, qwen3 class
                qk_rope_gate(t, dev, queue, false, true, false, false)   //! classic pairing with norm
                qk_rope_gate(t, dev, queue, true, false, false, false)   //! rope-only + twin, llama class
                qk_rope_gate(t, dev, queue, false, false, true, true)    //! partial rot, Wave D class

                tower_flash_kvhc_gate(t, dev, queue)
                tower_win_gate(t, dev, queue)
                g4a_attn_gate(t, dev, queue)
                fc_operands_gate(t, dev, queue, false)
                fc_row_gate(t, dev, queue, 24, false)
                fc_row_gate(t, dev, queue, 300, false)
                fc_unpack_gate(t, dev, queue)
                fc_operands_gate(t, dev, queue, true)
                fc_row_gate(t, dev, queue, 24, true)
                fc_row_gate(t, dev, queue, 300, true)
                fc_dw_gate(t, dev, queue, false)
                fc_dw_gate(t, dev, queue, true)
                g4a_dw_gate(t, dev, queue)
                g4a_glu_gate(t, dev, queue)
                g4a_lnrelu_gate(t, dev, queue)
                tw_rel_sincos_gate(t, dev, queue)
                q3a_shuffle_gate(t, dev, queue)
                pk_conv0_gate(t, dev, queue)
                pk_dw2d_gate(t, dev, queue)
                pk_xf_gate(t, dev, queue)
                cn_melnorm_gate(t, dev, queue)
                tw_tail_rows_gate(t, dev, queue)
                tw_pool2d_gate(t, dev, queue)
                tw_affine_gate(t, dev, queue)
                rope_tab_gate(t, dev, queue, RopeTabStamp.tw, 5, 96, 32, 0)
                tw_bias_gate_gate(t, dev, queue, 70, 3)
                tw_bias_gate_gate(t, dev, queue, 209, 1)   //! a row count off the 4-lane stride: the last lane's guard
                for (kind in [TwSeamStamp.add_ln, TwSeamStamp.add_rms, TwSeamStamp.add_post_rms, TwSeamStamp.pk_add_ln]) {
                    tw_seam_gate(t, dev, queue, kind, 300, 3)   //! the row past the 256-thread stride
                    tw_seam_gate(t, dev, queue, kind, 512, 2)   //! two whole trips of the stride, no tail
                }
                tw_seam_gate(t, dev, queue, TwSeamStamp.pk_add_ln, 100, 1)   //! under the stride, the pocket's one row
                bias_relu_rows_gate(t, dev, queue)
                st2_conv_gate(t, dev, queue, false, false)
                st2_conv_gate(t, dev, queue, true, false)
                st2_conv_gate(t, dev, queue, false, true)
                st2_conv_gate(t, dev, queue, true, true)
                st2_im2col_gate(t, dev, queue, 0, 5, false)
                st2_im2col_gate(t, dev, queue, 3, 2, false)
                st2_im2col_gate(t, dev, queue, 0, 5, true)
                st2_im2col_gate(t, dev, queue, 2, 3, true)
                st2_adain_gate(t, dev, queue, 37, true)
                st2_adain_gate(t, dev, queue, 520, true)
                st2_adain_gate(t, dev, queue, 37, false)
                st2_rows_gate(t, dev, queue)
                st2_gather_gate(t, dev, queue)
                st2_concat_gate(t, dev, queue, true)
                st2_concat_gate(t, dev, queue, false)
                st2_pool_gate(t, dev, queue)
                st2_lstm_gate(t, dev, queue)
                st2_attn_gate(t, dev, queue, 20)
                st2_attn_gate(t, dev, queue, 37)
                st2_attn_gate(t, dev, queue, 100)
                st2_sigsum_gate(t, dev, queue)
                st2_gelu_gate(t, dev, queue)
                st2_source_gate(t, dev, queue, true)
                st2_source_gate(t, dev, queue, false)
                st2_stft_gate(t, dev, queue, true)
                st2_stft_gate(t, dev, queue, false)
                st2_istft_gate(t, dev, queue, true)
                st2_istft_gate(t, dev, queue, false)
                pk_rows_gate(t, dev, queue, false)
                pk_rows_gate(t, dev, queue, true)
                pk_row_scale_gate(t, dev, queue)
                rope_stride_gate(t, dev, queue)
                rope_tab_gate(t, dev, queue, RopeTabStamp.pk, 23, 128, 64, 37)
                pk_attn_gate(t, dev, queue, 0, 10)
                pk_attn_gate(t, dev, queue, 8, 10)
                pk_attn_gate(t, dev, queue, 0, 300)   //! past one 256-key stride of the staged scores
                pk_attn_gate(t, dev, queue, 8, 300)
                rope_store_pocket_gate(t, dev, queue)
                for (q8 in [false, true]) {
                    for (kind in range(q8 ? 1 : 0, 5)) {   //! the bare q8 dot rides the decode GEMV, gated in test_metal_gemv_kernels
                        pk_gemv_gate(t, dev, queue, q8, kind, 96)
                        pk_gemv_gate(t, dev, queue, q8, kind, 256)   //! a whole stage stride
                        pk_gemv_gate(t, dev, queue, q8, kind, 1024)   //! several stage strides, several row strides
                    }
                    pk_gemv_gate(t, dev, queue, false, 0, 100)   //! the f32 row loop's partial tail
                }
                tower_gather_gate(t, dev, queue)

                // ===== attention: the tiled sgmat trio vs the direct causal-GQA oracle, at the
                // classic small shape (hs 32 — one QK k-slab) AND the llama-3B head shape
                // (hs 128 — the QK kernel's two-k-slab path; 2 heads keep the oracle cheap)
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, false)
                attn_trio_gate(t, dev, queue, 128, 2, 1, NPOS, false)
                // the tiled QK/AV pair (hs % 64 shapes: llama-1B/3B classes + GQA)
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true)
                attn_trio_gate(t, dev, queue, 128, 6, 2, 40, true)
                // softcap + sink arms: rowstat and pf_p_weight carry MIRRORED window/softcap
                // math — these are the only cells where a drift between the two can red
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, false, AttnKeys(softcap = 30.0))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true, AttnKeys(softcap = 30.0))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, false, AttnKeys(hass = 1))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true, AttnKeys(hass = 1))

                // ===== the uniform span (media rows): fresh, continuation, continuation+window =====
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, false, AttnKeys(uend = NPOS))
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, false, AttnKeys(qoff = 32, uend = 32 + NPOS))
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, false, AttnKeys(qoff = 32, window = 40, uend = 32 + NPOS))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true, AttnKeys(uend = NPOS))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true, AttnKeys(qoff = 32, uend = 32 + NPOS))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true, AttnKeys(qoff = 32, window = 40, uend = 32 + NPOS))
                // the fused MIXED shape (AttnArgs.ulo): causal head + span rows [5, NPOS - 11) + causal tail
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, false, AttnKeys(ulo = 5, uend = NPOS - 11))
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, false, AttnKeys(qoff = 32, window = 40, ulo = 32 + 5, uend = 32 + NPOS - 11))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true, AttnKeys(ulo = 5, uend = NPOS - 11))
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true, AttnKeys(qoff = 32, ulo = 32 + 5, uend = 32 + NPOS - 11))

                // ===== swiglu + residual add =====
                {
                    let total = NROWS * HIDDEN
                    var inscope sg, su : array<float>
                    sg |> resize(total)
                    su |> resize(total)
                    for (k in range(total)) {
                        sg[k] = 0.21 * float(k % 43) - 4.3
                        su[k] = 0.31 * float(k % 17) - 2.5
                    }
                    var inscope sg_want := sg
                    swiglu(sg_want, su, int64(total))
                    var inscope sg_gpu <- ew2_gate(t, dev, queue, "swiglu", stamp_of(@@enc_swiglu), sg, su, sg_want, false) $(enc : MetalComputeEncoder?; bx, by, btot, bone : MetalBuffer?; n : int64) {
                        enc_swiglu(enc, bx, 0ul, by, 0ul, btot, n)
                    }
                    // add: x += y, bit-exact (one fadd on both sides) — seeded from the GPU gate
                    // output, not the CPU one: the exp-envelope difference upstream would otherwise
                    // masquerade as an add mismatch
                    var inscope add_want : array<float>
                    add_want |> resize(total)
                    for (k in range(total)) {
                        add_want[k] = sg_gpu[k] + su[k]
                    }
                    var inscope added <- ew2_gate(t, dev, queue, "add", stamp_of(@@enc_add_c), sg_gpu, su, add_want, true) $(enc : MetalComputeEncoder?; bx, by, btot, bone : MetalBuffer?; n : int64) {
                        enc_add_c(enc, bx, 0ul, by, 0ul, btot, bone, n)
                    }
                }

                // ===== the production mul_mm — the resident path's default GEMM =====
                mulmm_gate(t, dev, queue, 96, 128, 64)      // 3 k-blocks, 2 output tiles, 2 token tiles
                mulmm_gate(t, dev, queue, 64, 192, 32)      // 2 k-blocks, 3 output tiles, 1 token tile

                // ===== the classifier GEMV (logits-on-GPU) =====
                // exact-arithmetic data (quants in [-8, 7], pow2 scales, int x): every per-byte4
                // dot, scale product, and simd_sum partial is exactly representable — BIT-exact
                // vs the CPU reference. d = 21 exercises the row < ddim tail guard (grid pads to 22).
                gemv_gate(t, dev, queue, 96, 21)            // 3 blocks per row, guarded tail
                gemv_gate(t, dev, queue, 160, 64)           // 5 blocks per row, exact grid
                gemv_gate(t, dev, queue, 1120, 8)           // 35 blocks: exercises the 4-chunk main loop, not only the tail

                // ===== MoE routing kernels (Wave C) =====
                // select: exact-tie + odd-ne shapes vs moe_select_core; router /
                // expert-indexed q8 GEMV / combine: exact-arithmetic data, BIT-exact. The kq
                // expert twins share the q8 twin's slot/index math verbatim (dequant bodies are
                // the proven site GEMVs') — their numeric coverage rides the fam-qwen3moe row.
                moe_select_gate(t, dev, queue, 128, 8, 1, true)    // the qwen3moe shape + exact ties
                moe_select_gate(t, dev, queue, 60, 4, 1, false)    // ne not a lane multiple — pad lanes
                moe_select_gate(t, dev, queue, 256, 16, 1, false)  // the 8-deep stamp's cap
                moe_select_gate(t, dev, queue, 512, 10, 1, false)  // the 16-deep stamp: the Coder-Next shape and the serve gate's cap
                moe_select_gate(t, dev, queue, 300, 8, 2, true)    // past 256 with pad lanes and exact ties, two stream rows
                moe_select_gate(t, dev, queue, 128, 8, 3, false)   // batch: one stream row per tg
                moe_select_gate(t, dev, queue, 32, 4, 1, false, 1)  // gpt-oss softmax_weight mode
                moe_router_gate(t, dev, queue, 64, 128, 1, false)
                moe_gemv_gate(t, dev, queue, KqFmt.q8, 96, 24, 5, 3, 1, false)   // shared x (the gate/up sites)
                moe_gemv_gate(t, dev, queue, KqFmt.q8, 64, 32, 7, 4, 1, true)    // per-slot x rows (the down site)
                moe_gemv_gate(t, dev, queue, KqFmt.q8, 64, 24, 5, 3, 3, false)   // batch streams, shared-x form
                moe_gemv_gate(t, dev, queue, KqFmt.q8, 64, 24, 5, 3, 3, true)    // batch streams, down form
                moe_combine_gate(t, dev, queue, 200, 4)

                // ===== q51 expert kernels (the 26B-A4B Q4_K_M down stacks) =====
                // exact-arithmetic q51 planes (pow2 d, quarter m, int x) — BIT-exact both kernels;
                // n = 96 covers 3 blocks/row, the mulmm shapes cover padded CSR tails (ce % 32)
                moe_gemv_gate(t, dev, queue, KqFmt.q51, 96, 24, 5, 3, 1, false)   // gate/up site (shared x)
                moe_gemv_gate(t, dev, queue, KqFmt.q51, 64, 32, 7, 4, 1, true)    // down site (per-slot x)
                moe_gemv_gate(t, dev, queue, KqFmt.q51, 64, 24, 5, 3, 3, true)    // batch streams, down form
                moe_mulmm_b32_gate(t, dev, queue, KqFmt.q51, 64, 64, [32, 40, 64])    // padded tail on expert 1
                moe_mulmm_b32_gate(t, dev, queue, KqFmt.q51, 96, 128, [8, 96])        // 2 col tiles, tiny run + multi-tile
                moe_mulmm_b32_gate(t, dev, queue, KqFmt.q51, 64, 64, [136, 32])       // counts crossing 128 — both tall-pair stamps live

                // ===== iq4nl32 expert kernels (the Qwen3.8-Flash-Next 640-wide down stacks) =====
                // exact-arithmetic planes (pow2 d, the int8 codebook, int x) — BIT-exact both kernels, the same shapes as q51
                moe_gemv_gate(t, dev, queue, KqFmt.iq4nl32, 96, 24, 5, 3, 1, false)   // gate/up site (shared x)
                moe_gemv_gate(t, dev, queue, KqFmt.iq4nl32, 64, 32, 7, 4, 1, true)    // down site (per-slot x)
                moe_gemv_gate(t, dev, queue, KqFmt.iq4nl32, 64, 24, 5, 3, 3, true)    // batch streams, down form
                moe_mulmm_b32_gate(t, dev, queue, KqFmt.iq4nl32, 64, 64, [32, 40, 64])
                moe_mulmm_b32_gate(t, dev, queue, KqFmt.iq4nl32, 96, 128, [8, 96])
                moe_mulmm_b32_gate(t, dev, queue, KqFmt.iq4nl32, 64, 64, [136, 32])

                // ===== mx4 expert GEMV (reworked to the q51 shape — gpt-oss's decode kernel) =====
                moe_gemv_gate(t, dev, queue, KqFmt.mx4, 96, 24, 5, 3, 1, false)          // gate/up (shared x)
                moe_gemv_gate(t, dev, queue, KqFmt.mx4, 64, 32, 7, 4, 1, true, true)     // down site + bias rows
                moe_gemv_gate(t, dev, queue, KqFmt.mx4, 64, 24, 5, 3, 3, true, true)     // batch streams, biased down

                // ===== deltanet kernels (Wave D) =====
                // the chain gates compare each stage at its seam vs a sequential CPU mirror;
                // shapes: decode (npos 1), a batch window, npos < taps (history straddle),
                // and nkh == nvh (the 0.8B class; the rest exercise the h % nkh fan-out)
                dn_chain_gate(t, dev, queue, 2, 4, 4, 1)
                dn_chain_gate(t, dev, queue, 2, 4, 4, 5)
                dn_chain_gate(t, dev, queue, 2, 4, 4, 2)
                dn_chain_gate(t, dev, queue, 4, 4, 4, 3)
                dn_scan_half_gate(t, dev, queue, 2, 4, 5)
                dn_scan_half_gate(t, dev, queue, 4, 4, 1)
                dn_scan_half_gate(t, dev, queue, 2, 2, 37)   // npos x nvh off the prepare pass's 64-wide groups
                dn_deint_sigmul_gate(t, dev, queue, 3, 64, 2)
                axpy_sig_gate(t, dev, queue, 1, 96)    // shexp combine (stage 3): decode shape
                axpy_sig_gate(t, dev, queue, 5, 64)    // ... and the per-position prefill shape

                // ===== MoE plumbing (router batch GEMV, CSR pair, wscale, reduce, swiglu-oai) =====
                swiglu_oai_gate(t, dev, queue, 256)
                swiglu_oai_pf_gate(t, dev, queue, 16, [32, 40, 8], 32)   // 32 ghost rows stay untouched
                g4_router_norm_gate(t, dev, queue, 5, 96)
                moe_wscale_gate(t, dev, queue, 13, 2, 7)
                moe_router_gate(t, dev, queue, 64, 7, 13, false)         // ne=7: row guard; np=13: stream clamp
                moe_router_gate(t, dev, queue, 64, 7, 13, true)
                moe_csr_gate(t, dev, queue, 13, 5, 2)
                moe_csr_gate(t, dev, queue, 40, 3, 4)                    // an expert crosses one 32-pad tile
                moe_gather_x_gate(t, dev, queue, 7, 3, 2, 64)            // stale tail words: in-range copies, >= total zeroes
                moe_reduce_gate(t, dev, queue, 96, 9, 2)

                // ===== prefill misc (PLE pre-step, bias epilogue, MTP cat, deltanet ba) =====
                ple_gather_gate(t, dev, queue, 0, 11, 3, 32, 5, 0)
                // the kq table twins: 300 superblocks of table past a non-zero base, one row per
                // token = 3 superblocks, so every arm walks the row-stride and sub-block math
                ple_gather_gate(t, dev, queue, 4, 100, 3, 256, 5, 7)
                ple_gather_gate(t, dev, queue, 5, 100, 3, 256, 5, 7)
                ple_gather_gate(t, dev, queue, 40, 100, 3, 256, 5, 7)
                ple_finish_gate(t, dev, queue, 3, 64, 5)
                add_bias_rows_gate(t, dev, queue, 7, 96)
                pf_cat2_gate(t, dev, queue, 5, 48)
                dn_ba_gate(t, dev, queue, 768, 11, 3)   // fast stripe + strided tail + row guard
            }
        } else {
            t |> skip("das_metal is not built on this platform; nothing to compare")
        }
    }
}

// ===== MoE plumbing gates (Wave C: swiglu-oai, g4 router norm, wscale, router, CSR, reduce) =====

// the decode form (MetalSwigluOai): in-place clamped swiglu over a flat span, the enc_ew2 shape
def private swiglu_oai_gate(t : T?; var dev, queue; total : int) {
    let tag = "swiglu_oai total={total}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_swiglu_oai)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope g, u : array<float>
        g |> resize(total)
        u |> resize(total)
        for (i in range(total)) {
            g[i] = float((i * 7) % 23 - 11)   // past the +-7 clamps on both sides
            u[i] = float((i * 5 + 3) % 23 - 11)
        }
        var inscope want := g
        swiglu_oai(want, u, int64(total))
        let bg = mc |> io_(g)
        let bu = mc |> in_(u)
        let btot = mc |> in_plain([uint(total)])
        mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bg], 0ul, mc.buf[bu], 0ul, mc.buf[btot], int64(total))
            }
        }
        check_buf(t, tag, mc.buf[bg], want, BAR_FP, FLAT_SHAPE)
    }
}

// the prefill twin (MetalSwigluOaiPf): same math over the padded bucket panel, plus the
// liveness bound — ghost rows past the last bucket's padded end must stay UNTOUCHED
def private swiglu_oai_pf_gate(t : T?; var dev, queue; nfe : int; counts : array<int>; extra_rows : int) {
    let tag = "swiglu_oai_pf nfe={nfe}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(kernel_of(@@pf_enc_swiglu_oai), err)
        return if (!pso_ok(t, tag, pso, err))
        let ne = length(counts)
        var inscope(cnt, basep, pad) <- padded_bases(counts)
        let live = pad * nfe
        let total = (pad + extra_rows) * nfe
        var inscope g, u : array<float>
        g |> resize(total)
        u |> resize(total)
        for (i in range(total)) {
            g[i] = float((i * 7) % 23 - 11)
            u[i] = float((i * 5 + 3) % 23 - 11)
        }
        var inscope want := g
        swiglu_oai(want, u, int64(live))
        let bg = mc |> io_(g)
        let bu = mc |> in_(u)
        let btot = mc |> in_plain([uint(total)])
        let bcnt = mc |> in_plain(cnt)
        let bbase = mc |> in_plain(basep)
        let bne = mc |> in_plain([uint(ne)])
        let bnfe = mc |> in_plain([uint(nfe)])
        mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {   //! four elements a thread
            pf_enc_swiglu_oai_pso(enc, pso, mc.buf[bg], 0ul, mc.buf[bu], 0ul, mc.buf[btot], mc.buf[bcnt], mc.buf[bbase], mc.buf[bne], mc.buf[bnfe], int64(total))
        }
        check_buf(t, tag, mc.buf[bg], want, BAR_FP, Shape(row_width = nfe))
    }
}

// gemma4 router-input norm (MetalG4RouterNorm): y = x * (1/sqrt(mean(x^2)+eps)) * invd * rs
def private g4_router_norm_gate(t : T?; var dev, queue; rows, dim : int) {
    let tag = "g4_router_norm rows={rows} dim={dim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_g4_router_norm)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let eps = 1.0e-5
        let invd = 1.0 / sqrt(float(dim))
        var inscope x, rs : array<float>
        x |> resize(rows * dim)
        rs |> resize(dim)
        for (i in range(rows * dim)) {
            x[i] = 0.13 * float(i % 29) - 1.7
        }
        for (i in range(dim)) {
            rs[i] = 0.07 * float(i % 17) - 0.5
        }
        var inscope want : array<float>
        want |> resize(rows * dim)
        for (r in range(rows)) {
            unsafe {
                rmsnorm(addr(want[r * dim]), addr<float const?>(x[r * dim]), addr<float const?>(rs[0]), int64(dim), eps)
            }
            for (c in range(dim)) {
                want[r * dim + c] *= invd
            }
        }
        let bx = mc |> in_(x)
        let brs = mc |> in_(rs)
        let by = mc |> out_words(rows * dim, -1000.0)
        let bdim = mc |> in_plain([uint(dim)])
        let beps = mc |> in_plain([eps])
        let binvd = mc |> in_plain([invd])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[brs], mc.buf[by], mc.buf[bdim], mc.buf[beps], mc.buf[binvd], int64(rows))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_FP, Shape(row_width = dim))
    }
}

// gemma4 per-expert down-scale fold (MetalMoeWScale): selw[r][j] *= ds[seli[r][j]] in place on
// the mixed sel buffer (uint idx half + float w half) — compared as raw uint bits
def private moe_wscale_gate(t : T?; var dev, queue; rows, nk, ne : int) {
    let tag = "moe_wscale rows={rows} nk={nk}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_moe_wscale)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope ds : array<float>
        ds |> resize(ne)
        for (e in range(ne)) {
            ds[e] = float((e * 3) % 7 - 3) * 0.5
        }
        var inscope sel, want : array<uint>
        sel |> resize(rows * 2 * nk)
        want |> resize(rows * 2 * nk)
        for (r in range(rows)) {
            for (j in range(nk)) {
                let idx = uint((r * 3 + j * 5) % ne)
                let w = float((r + j * 7) % 9 - 4) * 0.25
                sel[r * 2 * nk + j] = idx
                sel[r * 2 * nk + nk + j] = float_bits_to_uint(w)
                want[r * 2 * nk + j] = idx
                want[r * 2 * nk + nk + j] = float_bits_to_uint(w * ds[int(idx)])
            }
        }
        let bsel = mc |> in_plain(sel)
        let bds = mc |> in_(ds)
        let bnk = mc |> in_plain([uint(nk)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // the sel buffer twice: its uint idx view and its float w view
                invoke(st.enc, enc, mc.buf[bsel], mc.buf[bsel], mc.buf[bds], mc.buf[bnk], int64(rows))
            }
        }
        return if (!ran)
        check_buf_exact(t, tag, mc.buf[bsel], want)
    }
}

//! The router GEMV BIT-exact on dyadic data: npos 1 = MetalMoeRouter (BATCHED gates slot 8 out); past one MetalMoeRouterB, 8 streams a column, pad stores gated.
def private moe_router_gate(t : T?; var dev, queue; dim, ne, npos : int; hasb : bool) {
    let tag = "moe_router{npos > 1 ? "_b" : ""} dim={dim} ne={ne} np={npos}{hasb ? " bias" : ""}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sr = stamp_of(@@enc_moe_router)
        let srb = stamp_of(@@enc_moe_router_b)
        var pso = npos > 1 ? mc |> keep_stamp(srb, err) : mc |> keep_stamp(sr, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope w, x, rb : array<float>
        w |> resize(ne * dim)
        x |> resize(npos * dim)
        rb |> resize(ne)
        for (i in range(ne * dim)) {
            w[i] = float((i * 5) % 9 - 4) * 0.5
        }
        for (i in range(npos * dim)) {
            x[i] = float((i * 7 + 2) % 9 - 4) * 0.25
        }
        for (e in range(ne)) {
            rb[e] = float((e * 11) % 9 - 4) * 0.125
        }
        var inscope want : array<float>
        want |> resize(npos * ne)
        for (st in range(npos)) {
            for (e in range(ne)) {
                var acc = 0.0lf
                for (c in range(dim)) {
                    acc += double(w[e * dim + c]) * double(x[st * dim + c])
                }
                if (hasb) {
                    acc += double(rb[e])
                }
                want[st * ne + e] = float(acc)
            }
        }
        let bw = mc |> in_(w)
        let bx = mc |> in_(x)
        let by = mc |> out_words(npos * ne, -1000.0)
        let bdim = mc |> in_plain([uint(dim)])
        let bne = mc |> in_plain([uint(ne)])
        let brb = mc |> in_(rb)
        let bhasb = mc |> in_plain([hasb ? 1u : 0u])
        let bnp = mc |> in_plain([uint(npos)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            if (npos > 1) {   // bdim twice: the width and the per-stream x stride
                stamp_run(srb, pso) {
                    invoke(srb.enc, enc, mc.buf[bw], mc.buf[bx], mc.buf[by], mc.buf[bdim], mc.buf[bne], mc.buf[bdim], mc.buf[brb], mc.buf[bhasb], mc.buf[bnp], int64(ne), int64(npos))
                }
            } else {
                stamp_run(sr, pso) {
                    invoke(sr.enc, enc, mc.buf[bw], mc.buf[bx], mc.buf[by], mc.buf[bdim], mc.buf[bne], mc.buf[bdim], mc.buf[brb], mc.buf[bhasb], int64(ne), int64(npos))
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = ne))
        var inscope got <- mc |> plane_run(by)
        let (mx, at) = span_maxdiff(got, want, 0, 0, npos * ne)
        to_log(LOG_INFO, "{tag}: max |gpu - cpu| {mx} at stream {at / ne} expert {at % ne}\n")
    }
}

//! The CPU twin of the count -> bucket CSR pair: counts, padded bases (the total past the last expert), bucket rows (pad tails at 0xFFFFFFFF), each slot's row.
def private csr_ref(sel : array<uint>; npos, ne, nk : int) : tuple<cnt : array<uint>; base : array<uint>; bkt : array<uint>; inv : array<uint>; pad : int> {
    let total = npos * nk
    var counts : array<int>
    counts |> resize(ne)
    for (tt in range(total)) {
        counts[int(sel[(tt / nk) * 2 * nk + tt % nk])]++
    }
    var (cnt, base, pad) <- padded_bases(counts)
    base |> push(uint(pad))
    var bkt, inv : array<uint>
    bkt |> resize(pad)
    inv |> resize(total)
    for (i in range(pad)) {
        bkt[i] = 0xFFFFFFFF
    }
    for (e in range(ne)) {
        var b = int(base[e])
        for (tt in range(total)) {
            if (int(sel[(tt / nk) * 2 * nk + tt % nk]) == e) {
                bkt[b] = uint(tt)
                inv[tt] = uint(b)
                b++
            }
        }
    }
    delete counts
    return <- (cnt = cnt, base = base, bkt = bkt, inv = inv, pad = pad)
}

// the atomics-free CSR pair (MetalMoeCount -> MetalMoeBucket) chained as production dispatches
// them: cnt, then padded-prefix basep + serial-order bkt/inv — all four outputs checked exactly
// (the kernel must STAMP pad bkt slots to 0xFFFFFFFF — the gather-x sentinel — so the
// gate seeds bkt with a different poison)
def private moe_csr_gate(t : T?; var dev, queue; npos, ne, nk : int) {
    let tag = "moe_csr np={npos} ne={ne} nk={nk}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var psoC = mc |> keep_stamp(kernel_of(@@pf_enc_moe_count), err)
        return if (!pso_ok(t, "{tag} count", psoC, err))
        var psoB = mc |> keep_stamp(kernel_of(@@pf_enc_moe_bucket), err)
        return if (!pso_ok(t, "{tag} bucket", psoB, err))
        let total = npos * nk
        var inscope sel : array<uint>
        sel |> resize(npos * 2 * nk)
        for (r in range(npos)) {
            for (j in range(nk)) {
                sel[r * 2 * nk + j] = uint((r * 3 + j * 5 + 1) % ne)
                sel[r * 2 * nk + nk + j] = float_bits_to_uint(1.0)   // weight half — unread here
            }
        }
        var inscope(cntw, basew, bktw, invw, pad) <- csr_ref(sel, npos, ne, nk)    // basew[ne] = the padded total the extra threadgroup publishes
        let bsel = mc |> in_plain(sel)
        let bcnt = mc |> out_words(ne, 0xFFFFFFFF)
        let bbase = mc |> out_words(ne + 1, 0xFFFFFFFF)
        let bbkt = mc |> out_words(pad, 0xEEEEEEEE)
        let binv = mc |> out_words(total, 0xFFFFFFFF)
        let btot = mc |> in_plain([uint(total)])
        let bnk = mc |> in_plain([uint(nk)])
        let bne = mc |> in_plain([uint(ne)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            pf_enc_moe_count_pso(enc, psoC, mc.buf[bsel], mc.buf[bcnt], mc.buf[btot], mc.buf[bnk], int64(ne))
            pf_enc_moe_bucket_pso(enc, psoB, mc.buf[bsel], mc.buf[bbase], mc.buf[bbkt], mc.buf[binv], mc.buf[btot], mc.buf[bnk], mc.buf[bcnt], mc.buf[bne], int64(ne + 1))   // + the total's threadgroup
        }
        return if (!ran)
        check_buf_exact(t, "{tag} counts", mc.buf[bcnt], cntw)
        check_buf_exact(t, "{tag} bases", mc.buf[bbase], basew)
        check_buf_exact(t, "{tag} buckets", mc.buf[bbkt], bktw)
        check_buf_exact(t, "{tag} slot rows", mc.buf[binv], invw)
    }
}

// the gather-X panel (MetalMoeGatherX): real bkt rows copy their token's xf row into the f16
// panel; the sentinel, an expert's stamped pad tail, and STALE words past the last expert's
// tail all resolve through the ONE range test — a stale word >= total zeroes (the old
// sentinel-equality form sent its token index out past the activation rows), a stale word
// below total copies a live row into a slot no mm tile reads. All compares f16-exact.
def private moe_gather_x_gate(t : T?; var dev, queue; npos, ne, nk, dim : int) {
    let tag = "moe_gather_x np={npos} ne={ne} nk={nk} dim={dim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(kernel_of(@@enc_moe_gather_x), err)
        return if (!pso_ok(t, tag, pso, err))
        let total = npos * nk
        var inscope sel : array<uint>
        sel |> resize(npos * 2 * nk)
        for (r in range(npos)) {
            for (j in range(nk)) {
                sel[r * 2 * nk + j] = uint((r * 3 + j * 5 + 1) % ne)
            }
        }
        var inscope(cntw, basew, bktw, invw, pad) <- csr_ref(sel, npos, ne, nk)
        let mpad = pad + 32   // a trailing gap past the last expert's stamped tail
        bktw |> resize(mpad)
        // the gap: recycled-pool flavors — total (>= total boundary: MUST zero; the old equality
        // test read the one-past-the-end activation row here), a live selection (harmless copy),
        // and arbitrary garbage (>= total: zero)
        for (i in range(16)) {
            bktw[pad + i] = uint(total)
        }
        for (i in range(16, 24)) {
            bktw[pad + i] = 5u
        }
        for (i in range(24, 32)) {
            bktw[pad + i] = 0xEEEEEEEE
        }
        var inscope xfw : array<float>
        xfw |> resize((npos + 1) * dim)
        for (r in range(npos)) {
            for (i in range(dim)) {
                xfw[r * dim + i] = float(r * 10 + 1)
            }
        }
        for (i in range(dim)) {
            xfw[npos * dim + i] = 777.0   // one past the real rows: reachable ONLY by a broken validity test
        }
        var inscope want : array<float>
        want |> resize(mpad * dim)
        for (r in range(mpad)) {
            let v = bktw[r]
            let live = v < uint(total)
            for (i in range(dim)) {
                want[r * dim + i] = float(live ? float16(xfw[int(v / uint(nk)) * dim + i]) : float16(0.0))
            }
        }
        let bbkt = mc |> in_plain(bktw)
        let bxf = mc |> in_(xfw)
        let bxg = mc |> out_words(mpad * dim, float16(555.0))
        var gka = MoeGxArgs(dim = uint(dim), nk = uint(nk), ne = uint(ne), total = uint(total))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            enc_moe_gather_x_pso(enc, pso, mc.buf[bbkt], mc.buf[bxf], mc.buf[bxg], gka, int64(mpad))
        }
        return if (!ran)
        check_buf_h(t, tag, mc.buf[bxg], want, BAR_EXACT, Shape(row_width = dim))
    }
}

// the ordered weighted reduce (MetalMoeReduce): y[r] = sum_j selw[r][j] * dn[inv[r*k+j]] in
// slot order — BIT-exact on the pow2 fixture
def private moe_reduce_gate(t : T?; var dev, queue; dim, npos, nk : int) {
    let tag = "moe_reduce dim={dim} np={npos} nk={nk}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(kernel_of(@@pf_enc_moe_reduce), err)
        return if (!pso_ok(t, tag, pso, err))
        let nrows_dn = npos * nk + 7
        var inscope dn : array<float>
        dn |> resize(nrows_dn * dim)
        for (i in range(nrows_dn * dim)) {
            dn[i] = float((i * 5) % 9 - 4) * 0.5
        }
        var inscope sel : array<uint>
        sel |> resize(npos * 2 * nk)
        var inscope inv : array<uint>
        inv |> resize(npos * nk)
        for (r in range(npos)) {
            for (j in range(nk)) {
                sel[r * 2 * nk + j] = 0u
                sel[r * 2 * nk + nk + j] = float_bits_to_uint(float((r + j * 3) % 9 - 4) * 0.25)
                inv[r * nk + j] = uint(((r * nk + j) * 3 + 1) % nrows_dn)
            }
        }
        var inscope want : array<float>
        want |> resize(npos * dim)
        for (r in range(npos)) {
            for (g in range(dim)) {
                var acc = 0.0
                for (j in range(nk)) {
                    acc += uint_bits_to_float(sel[r * 2 * nk + nk + j]) * dn[int(inv[r * nk + j]) * dim + g]
                }
                want[r * dim + g] = acc
            }
        }
        let bdn = mc |> in_(dn)
        let bsel = mc |> in_plain(sel)
        let binv = mc |> in_plain(inv)
        let by = mc |> out_words(npos * dim, -1000.0)
        let bdim = mc |> in_plain([uint(dim)])
        let bnk = mc |> in_plain([uint(nk)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            pf_enc_moe_reduce_pso(enc, pso, mc.buf[bdn], mc.buf[bsel], mc.buf[binv], mc.buf[by], mc.buf[bdim], mc.buf[bnk], int64(dim), int64(npos))   // bsel: the float w view
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = dim))
    }
}

// ===== prefill misc gates (PLE pre-step, bias epilogue, MTP cat, deltanet ba GEMV) =====

//! the PLE gather kernel of a token table's format
def private ple_gather_make(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.q8) return kernel_of(@@pf_enc_ple_gather).make
    if (f == KqFmt.k4) return kernel_of(@@pf_enc_ple_gather_k4).make
    if (f == KqFmt.k5) return kernel_of(@@pf_enc_ple_gather_k5).make
    if (f != KqFmt.q40) {
        panic("ple_gather_make: {f} has no PLE gather kernel")
    }
    return kernel_of(@@pf_enc_ple_gather_q40).make
}

//! the PLE gather builder of `f` through `pso`: the scale plane `ks` at `soff` (q8: the blob, bound twice - its half-scale
//! and byte views), the quant plane `kq` at `qoff` where the format has one
def private ple_gather_dispatch(f; enc; pso; ks; soff : uint64; tok, y; var ka : auto&; kq; qoff : uint64; nlayers, npos, ple : int64) {
    if (f == KqFmt.q8) {
        pf_enc_ple_gather_pso(enc, pso, ks, soff, ks, tok, y, ka, nlayers, npos, ple)
    } elif (f == KqFmt.k4) {
        pf_enc_ple_gather_k4_pso(enc, pso, ks, soff, ks, tok, y, ka, kq, qoff, nlayers, npos, ple)
    } elif (f == KqFmt.k5) {
        pf_enc_ple_gather_k5_pso(enc, pso, ks, soff, ks, tok, y, ka, kq, qoff, nlayers, npos, ple)
    } else {
        pf_enc_ple_gather_q40_pso(enc, pso, ks, soff, tok, y, ka, kq, qoff, nlayers, npos, ple)
    }
}

//! PLE stage 1: fmt 0 the q8 table bound twice vs its exact dequant; a kq id its native planes bound at sb0, as production binds them, vs kq_row_ref; exact in fp32.
def private ple_gather_gate(t : T?; var dev, queue; fmt, vocab, nlayers, ple, npos, sb0 : int) {
    let tag = "ple_gather f={fmt} v={vocab} nl={nlayers} ple={ple} np={npos} sb0={sb0}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let f = fmt == 0 ? KqFmt.q8 : kq_fmt_of_id(fmt, "ple_gather_gate")
        var pso = mc |> keep_pso(invoke(ple_gather_make(f), dev, err))
        return if (!pso_ok(t, tag, pso, err))
        let rowlen = nlayers * ple
        let rowsb = rowlen / 256          // superblocks per token row - the quants gate's `all % 256` clause
        let nsb = sb0 + vocab * rowsb
        var inscope quants : array<uint8>             // fmt 0: the q8 blob
        var inscope ks : array<uint8>
        var inscope wdeq : array<float>
        if (fmt == 0) {
            q8_blob_fill(1, vocab * rowlen / 32, quants, wdeq)
        } else {
            kq_fill_planes(fmt, nsb, quants, ks)
        }
        var inscope tok : array<uint>
        tok |> resize(npos)
        for (p in range(npos)) {
            tok[p] = uint((p * 23 + 5) % vocab)
        }
        let sc0 = sqrt(float(ple))
        var inscope wrow, want : array<float>
        wrow |> resize(rowlen)
        want |> resize(nlayers * npos * ple)
        for (p in range(npos)) {
            if (fmt == 0) {
                for (i in range(rowlen)) {
                    wrow[i] = wdeq[int(tok[p]) * rowlen + i]
                }
            } else {
                kq_row_ref(fmt, sb0 + int(tok[p]) * rowsb, rowsb, nsb, quants, ks, wrow)
            }
            for (l in range(nlayers)) {
                for (i in range(ple)) {
                    want[(l * npos + p) * ple + i] = wrow[l * ple + i] * sc0
                }
            }
        }
        var ka = PleArgs(npos = uint(npos), ple = uint(ple), nlayers = uint(nlayers), sc0 = sc0,
            inv_sd = 1.0, eps = 1.0e-6, inv_s2 = 1.0)
        let bkq = mc |> in_plain(quants)
        let bks = fmt == 0 ? bkq : (mc |> in_plain(ks))
        let btok = mc |> in_plain(tok)
        let by = mc |> out_words(nlayers * npos * ple, -1000.0)
        let soff = fmt == 0 ? 0ul : uint64(sb0 * 16)
        let qoff = fmt == 0 ? 0ul : uint64(int64(sb0) * kq_qsb(fmt))
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            ple_gather_dispatch(f, enc, pso, mc.buf[bks], soff, mc.buf[btok], mc.buf[by], ka, mc.buf[bkq], qoff, int64(nlayers), int64(npos), int64(ple))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, lead_poisoned(Bar(rel = 1e-4), want), Shape(row_width = ple))
    }
}

// PLE stage 3 (MetalPleFinish): per (layer, position) slice, in place on the panel —
// side = (side + pnorm * (v * rsqrt(mean(v^2) + eps))) / sqrt(2), v = proj/sqrt(dim)
def private ple_finish_gate(t : T?; var dev, queue; nlayers, ple, npos : int) {
    let tag = "ple_finish nl={nlayers} ple={ple} np={npos}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(kernel_of(@@pf_enc_ple_finish), err)
        return if (!pso_ok(t, tag, pso, err))
        let rowlen = nlayers * ple
        let eps = 1.0e-5
        let inv_sd = 0.125   // stands in for 1/sqrt(dim)
        let inv_s2 = 1.0 / sqrt(2.0)
        var inscope proj, pnorm, side : array<float>
        proj |> resize(npos * rowlen)
        pnorm |> resize(ple)
        side |> resize(nlayers * npos * ple)
        for (i in range(npos * rowlen)) {
            proj[i] = 0.11 * float(i % 31) - 1.6
        }
        for (i in range(ple)) {
            pnorm[i] = 0.09 * float(i % 13) - 0.5
        }
        for (i in range(nlayers * npos * ple)) {
            side[i] = 0.07 * float(i % 19) - 0.6
        }
        var inscope want, v : array<float>
        want |> resize(nlayers * npos * ple)
        v |> resize(ple)
        for (l in range(nlayers)) {
            for (p in range(npos)) {
                for (i in range(ple)) {
                    v[i] = proj[p * rowlen + l * ple + i] * inv_sd
                }
                rmsnorm(v, v, pnorm, int64(ple), eps)
                for (i in range(ple)) {
                    let yi = (l * npos + p) * ple + i
                    want[yi] = (side[yi] + v[i]) * inv_s2
                }
            }
        }
        var ka = PleArgs(npos = uint(npos), ple = uint(ple), nlayers = uint(nlayers), sc0 = 1.0,
            inv_sd = inv_sd, eps = eps, inv_s2 = inv_s2)
        let bproj = mc |> in_(proj)
        let bpn = mc |> in_(pnorm)
        let by = mc |> io_(side)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            pf_enc_ple_finish_pso(enc, pso, mc.buf[bproj], mc.buf[bpn], mc.buf[by], ka, int64(nlayers), int64(npos), int64(ple))
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_FP, Shape(row_width = ple))
    }
}

//! the QKV bias epilogue (the shared bias-rows stamp at act NONE): x[r*n + i] += b[i] in place, BIT-exact
def private add_bias_rows_gate(t : T?; var dev, queue; nrows, n : int) {
    let tag = "add_bias_rows rows={nrows} n={n}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_bias_act)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        let total = nrows * n
        var inscope x, b : array<float>
        x |> resize(total)
        b |> resize(n)
        for (i in range(total)) {
            x[i] = float((i * 5) % 9 - 4) * 0.5
        }
        for (i in range(n)) {
            b[i] = float((i * 7 + 1) % 9 - 4) * 0.25
        }
        var inscope want : array<float>
        want |> resize(total)
        for (i in range(total)) {
            want[i] = x[i] + b[i % n]
        }
        let bx = mc |> io_(x)
        let bb = mc |> in_(b)
        var ka = GkBiasActArgs(d = uint(n), nelem = uint(total), boff = 0u, act = BIAS_ACT_NONE)
        mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bx], mc.buf[bb], ka, int64((total + 3) / 4))
            }
        }
        t |> success(mismatch_exact(want, x) > 0, "{tag}: the reference moves off the input")
        check_buf(t, tag, mc.buf[bx], want, BAR_EXACT, Shape(row_width = n))
    }
}

// the MTP eh_proj interleave (MetalPfCat2): y[p] = [a[p] ; b[p]], pure copy — BIT-exact
def private pf_cat2_gate(t : T?; var dev, queue; npos, d : int) {
    let tag = "pf_cat2 np={npos} d={d}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        var pso = mc |> keep_stamp(kernel_of(@@pf_enc_cat2), err)
        return if (!pso_ok(t, tag, pso, err))
        let total = npos * d
        var inscope a, b : array<float>
        a |> resize(total)
        b |> resize(total)
        for (i in range(total)) {
            a[i] = float(i * 3 % 17) * 0.5
            b[i] = float(i * 5 % 19) * 0.25 - 2.0
        }
        var inscope want : array<float>
        want |> resize(npos * 2 * d)
        for (p in range(npos)) {
            for (i in range(d)) {
                want[p * 2 * d + i] = a[p * d + i]
                want[p * 2 * d + d + i] = b[p * d + i]
            }
        }
        let ba = mc |> in_(a)
        let bb = mc |> in_(b)
        let by = mc |> out_words(npos * 2 * d, -1000.0)
        let bd = mc |> in_plain([uint(d)])
        let btot = mc |> in_plain([uint(total)])
        mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            pf_enc_cat2_pso(enc, pso, mc.buf[by], mc.buf[ba], mc.buf[bb], mc.buf[bd], mc.buf[btot], int64(total))
        }
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = 2 * d))
    }
}

// the deltanet beta/alpha projection GEMV (MetalDnBa): narrow q8 blob rows per position —
// ndim large enough to walk both the 4-chunk fast stripe AND the strided tail, BIT-exact
def private dn_ba_gate(t : T?; var dev, queue; ndim, ddim, npos : int) {
    let tag = "dn_ba n={ndim} d={ddim} np={npos}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@pf_enc_dn_ba)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, tag, pso, err))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(1, ddim * ndim / 32, blob, wdeq)
        var inscope x : array<float>
        x |> resize(npos * ndim)
        for (i in range(npos * ndim)) {
            x[i] = float((i * 5 + i / 17) % 9 - 4) * 0.5
        }
        var inscope want : array<float>
        want |> resize(npos * ddim)
        for (p in range(npos)) {
            for (r in range(ddim)) {
                var acc = 0.0lf
                for (c in range(ndim)) {
                    acc += double(wdeq[r * ndim + c]) * double(x[p * ndim + c])
                }
                want[p * ddim + r] = float(acc)
            }
        }
        let bw = mc |> in_plain(blob)
        let bx = mc |> in_(x)
        let by = mc |> out_words(npos * ddim, -1000.0)
        let bn = mc |> in_plain([uint(ndim)])
        let bd = mc |> in_plain([uint(ddim)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // the blob twice: its half-scale and byte views
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], 0ul, mc.buf[bn], mc.buf[bd], int64(ddim), int64(npos))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = ddim))
    }
}

// The classifier GEMV kernel on exact-arithmetic data: [d x n] Q8 rows (the 34B blob form —
// half scale + 32 int8, mulmm_gate's layout) dot one f32 vector vs a direct CPU dot,
// BIT-exact (see the call site for why exactness holds; pow2 scales are exact in f16).
def private gemv_gate(t : T?; var dev, queue; n, d : int) {
    let tag = "gemv n={n} d={d}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_gemv_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, "gemv", pso, err))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(d, n / 32, blob, wdeq)
        var inscope xv, want : array<float>
        xv |> resize(n)
        want |> resize(d)
        for (i in range(n)) {
            xv[i] = float((i * 3) % 9 - 4)
        }
        for (r in range(d)) {
            var acc = 0.0
            for (k in range(n)) {
                acc += wdeq[r * n + k] * xv[k]
            }
            want[r] = acc
        }
        let bw = mc |> in_plain(blob)   // bound TWICE — half + byte views
        // one sentinel float past the output: the odd-d tail guard's only witness - an unguarded
        // pair row writes element d, which a d-wide compare never reads
        want |> push(-1000.0)
        let bx = mc |> in_(xv)
        let by = mc |> out_words(d + 1, -1000.0)
        let bn = mc |> in_plain([uint(n)])
        let bd = mc |> in_plain([uint(d)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], 0ul, mc.buf[by], 0ul, mc.buf[bn], mc.buf[bd], int64(d), 1l)
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, FLAT_SHAPE)
    }
}

// The production mul_mm kernel on exact-arithmetic data: 34B q8-block W blobs (GGUF q8_0's
// own layout: f16 scale + 32 int8) x raw f32 X rows vs a CPU reference, BIT-exact (quants in
// [-8, 7], pow2 scales: every staged half, every product, and every f32 partial sum is exactly
// representable, so any deviation is a real bug). The blob is built here independently of the
// driver — it doubles as the layout oracle for upload_blob_region.
def private mulmm_gate(t : T?; var dev, queue; k, d, m : int) {
    let tag = "mul_mm {k}x{d}x{m}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_gemm_mm_c)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, "mul_mm", pso, err))
        var inscope blob : array<uint8>
        var inscope wdeq : array<float>
        q8_blob_fill(d, k / 32, blob, wdeq)
        let bw = mc |> in_plain(blob)
        // X: raw f32 rows, integer-times-pow2 values (exact in half)
        var inscope xf : array<float>
        q8_x_fill(m * k, xf)
        let bx = mc |> in_(xf)
        let by = mc |> out_(m * d)
        let bk = mc |> in_plain([uint(k)])
        let bd = mc |> in_plain([uint(d)])
        var inscope want : array<float>
        want |> resize(m * d)
        for (r in range(m)) {
            for (j in range(d)) {
                var acc = 0.0lf
                for (c in range(k)) {
                    acc += double(wdeq[j * k + c]) * double(xf[r * k + c])
                }
                want[r * d + j] = float(acc)
            }
        }
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], 0ul, mc.buf[bk], mc.buf[bd], int64(m), int64(d))
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = d))
    }
}

// ===== MoE routing kernel gates (Wave C) =====

//! the select buffer against one expectation pair - winner ids exact, weights within `approx`;
//! the gate runs it twice, once on the reference and once on the poisoned control expectation
def private moe_select_bad(bsel; widx : array<uint>; ww : array<float>; nst, k : int) : tuple<idx : int; w : int> {
    var bidx = 0
    var bw = 0
    unsafe {
        var pi = reinterpret<uint?>(metal_buffer_contents(bsel))
        var pw = reinterpret<float?>(metal_buffer_contents(bsel))
        for (s in range(nst)) {
            for (j in range(k)) {
                bidx += pi[s * 2 * k + j] != widx[s * k + j] ? 1 : 0
                bw += fapprox(pw[s * 2 * k + k + j], ww[s * k + j]) ? 0 : 1
            }
        }
    }
    return (idx = bidx, w = bw)
}

// The select kernel vs the host select `moe_select_core` (softmax + top-k + norm_topk renorm),
// one stream row per threadgroup ([k idx u32][k w f32] per row). idx must match EXACTLY — ties
// are exact-equal logits, so both sides agree by the lowest-index rule; w compares within the
// shared fp tolerance (hardware exp vs the CPU's exp).
def private moe_select_gate(t : T?; var dev, queue; ne, k, nst : int; ties : bool; gmode : int = 0) {
    let tag = "moe select ne={ne} k={k} nst={nst}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = ne > 256 ? stamp_of(@@enc_moe_select_512) : stamp_of(@@enc_moe_select)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, "moe select", pso, err))
        var inscope lg : array<float>
        lg |> resize(nst * ne)
        for (i in range(nst * ne)) {
            lg[i] = 0.37 * float((i * 13 + 5) % ne) - 1.9
        }
        if (ties) {   // three exact-equal maxima — the lowest indices must win, in order
            lg[7] = 9.5
            lg[19] = 9.5
            lg[33] = 9.5
        }
        // gmode 0: softmax over all ne, top-k, the norm_topk renorm; gmode 1 (gpt-oss softmax_weight): top-k on the raw logits, softmax over the k, no renorm
        var inscope lgc := lg
        var inscope sidx : array<int64>
        var inscope ww : array<float>
        sidx |> resize(nst * k)
        ww |> resize(nst * k)
        for (s in range(nst)) {
            unsafe {
                moe_select_core(gmode == 0 ? MoeGate.softmax : MoeGate.softmax_weight, int64(ne), int64(k), gmode == 0, 1.0,
                    addr(lgc[s * ne]), addr(sidx[0]), addr(ww[0]), int64(s * k))
            }
        }
        var inscope widx <- [for (i in sidx); uint(i)]
        let blg = mc |> in_(lg)
        let bsel = mc |> out_words(nst * 2 * k, NAN_SENTINEL_BITS)
        let bne = mc |> in_plain([uint(ne)])
        let bnk = mc |> in_plain([uint(k)])
        let bgm = mc |> in_plain([uint(gmode)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // sel twice: the idx view and the float w view - row layout is in-kernel
                invoke(st.enc, enc, mc.buf[blg], mc.buf[bsel], mc.buf[bsel], mc.buf[bne], mc.buf[bnk], mc.buf[bgm], int64(nst))
            }
        }
        return if (!ran)
        let bad = moe_select_bad(mc.buf[bsel], widx, ww, nst, k)
        t |> equal(bad.idx, 0)
        t |> equal(bad.w, 0)
        // the control: the same compares must RED on a poisoned expectation - the first winner moved
        // to its neighbour id (no id can satisfy both) and its weight off by an added 1.0 + a tenth of
        // itself, past any band `approx` admits on a renormalized weight
        var inscope pidx := widx
        var inscope pww := ww
        pidx[0] = (pidx[0] + 1u) % uint(ne)
        pww[0] += abs(ww[0]) * 0.1 + 1.0
        let badp = moe_select_bad(mc.buf[bsel], pidx, pww, nst, k)
        t |> success(badp.idx + badp.w > 0, "moe select ne={ne} k={k}: poison control reds")
    }
}

//! the mx4 nibble values: the signed doubled-e2m1 table
let private MX4_VT = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0,
                                 0.0, -1.0, -2.0, -3.0, -4.0, -6.0, -8.0, -12.0)

//! An expert stack's planes in `fmt` (q8: plane 1 empty, one blob serves both views) and their exact dequant, BIT-exact under any GPU reduction order.
def private moe_gemv_planes(fmt : KqFmt; rows, n) : tuple<p0 : array<uint8>; p1 : array<uint8>; wref : array<float>> {   //! `rows`, `n` untyped: q8_blob_fill sits behind the optional das_metal require
    var p0, p1 : array<uint8>
    var wref : array<float>
    if (fmt == KqFmt.q8) {
        q8_blob_fill(rows, n / 32, p0, wref)
    } elif (fmt == KqFmt.q51) {
        q51_test_planes(rows, n, p0, p1, wref)
    } elif (fmt == KqFmt.iq4nl32) {
        iq4nl32_test_planes(rows, n, p0, p1, wref)
    } else {
        let nb = n / 32
        p0 |> resize(rows * nb * 16)
        p1 |> resize(rows * nb)
        wref |> resize(rows * n)
        for (b in range(rows * nb)) {
            let sc = 120 + (b * 3) % 12   // pow2 E8M0 scales 2^-8 .. 2^3 — every product dyadic
            let scale = unsafe(reinterpret<float>(uint(sc - 1) << 23u))
            p1[b] = uint8(sc)
            for (i in range(16)) {
                let nl = (b * 7 + i * 5 + 1) % 16
                let nh = (b * 11 + i * 3 + 2) % 16
                p0[b * 16 + i] = uint8(nl | (nh << 4))
                wref[b * 32 + i] = MX4_VT[nl] * scale
                wref[b * 32 + 16 + i] = MX4_VT[nh] * scale
            }
        }
    }
    return <- (p0 = p0, p1 = p1, wref = wref)
}

//! The expert-indexed GEMV BIT-exact over a net-expert stack in `fmt`, sel picking k planes a stream; `perslot` the down site's x stride, nst > 1 the batch strides.
def private moe_gemv_gate(t : T?; var dev, queue; fmt : KqFmt; n, d, net, k, nst : int; perslot : bool; bias : bool = false) {
    let tag = "moe gemv {fmt} n={n} d={d} net={net} k={k} nst={nst}{bias ? " bias" : ""}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sq8 = stamp_of(@@enc_moe_gemv_q8_c)
        let sq51 = stamp_of(@@enc_moe_gemv_q51_c)
        let siq = stamp_of(@@enc_moe_gemv_iq4nl32_c)
        let smx = stamp_of(@@enc_moe_gemv_mx4_c)
        var pso = (fmt == KqFmt.q8 ? mc |> keep_stamp(sq8, err) : (fmt == KqFmt.q51 ? mc |> keep_stamp(sq51, err)
            : (fmt == KqFmt.iq4nl32 ? mc |> keep_stamp(siq, err) : mc |> keep_stamp(smx, err))))
        return if (!pso_ok(t, tag, pso, err))
        let nb = n / 32
        var inscope(p0, p1, wref) <- moe_gemv_planes(fmt, net * d, n)
        // selection rows: [k idx u32][k w f32] per stream — only the idx section is read here
        var inscope sel : array<uint>
        sel |> resize(nst * 2 * k)
        for (s in range(nst)) {
            for (j in range(k)) {
                sel[s * 2 * k + j] = uint((s + j * 3 + net - 1) % net)
            }
        }
        let xrows = perslot ? nst * k : nst
        var inscope xv : array<float>
        xv |> resize(xrows * n)
        for (i in range(xrows * n)) {
            xv[i] = float((i * 3) % 9 - 4)
        }
        var inscope wb : array<float>
        wb |> resize(net * d)
        for (i in range(net * d)) {
            wb[i] = bias ? float(i % 7 - 3) * 0.25 : 0.0
        }
        var inscope want : array<float>
        want |> resize(nst * k * d)
        for (s in range(nst)) {
            for (j in range(k)) {
                let e = int(sel[s * 2 * k + j])
                let x0 = (perslot ? s * k + j : s) * n
                for (r in range(d)) {
                    var acc = 0.0
                    for (c in range(n)) {
                        acc += wref[(e * d + r) * n + c] * xv[x0 + c]
                    }
                    want[(s * k + j) * d + r] = acc + wb[e * d + r]
                }
            }
        }
        let bw = mc |> in_plain(p0)
        let bs = fmt == KqFmt.q8 ? bw : (mc |> in_plain(p1))
        let bx = mc |> in_(xv)
        let by = mc |> out_words(nst * k * d, -1000.0)
        let bsel = mc |> in_plain(sel)
        let bwb = mc |> in_(wb)
        let bvt = mc |> in_([for (v in MX4_VT); v])
        var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
            xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
            bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = bias ? 1u : 0u)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            let (dr, kk, ns) = (int64(d), int64(k), int64(nst))
            if (fmt == KqFmt.q8) {   // one blob, its half-scale and byte views
                stamp_run(sq8, pso) {
                    invoke(sq8.enc, enc, mc.buf[bw], 0ul, mc.buf[bw], mc.buf[bx], mc.buf[by], 0ul, ka, mc.buf[bsel], dr, kk, ns)
                }
            } elif (fmt == KqFmt.q51) {
                stamp_run(sq51, pso) {
                    invoke(sq51.enc, enc, mc.buf[bw], 0ul, mc.buf[bs], 0ul, mc.buf[bx], mc.buf[by], 0ul, ka, mc.buf[bsel], dr, kk, ns)
                }
            } elif (fmt == KqFmt.iq4nl32) {
                stamp_run(siq, pso) {
                    invoke(siq.enc, enc, mc.buf[bw], 0ul, mc.buf[bs], 0ul, mc.buf[bx], mc.buf[by], 0ul, ka, mc.buf[bsel], dr, kk, ns)
                }
            } else {
                stamp_run(smx, pso) {
                    invoke(smx.enc, enc, mc.buf[bw], 0ul, mc.buf[bs], 0ul, mc.buf[bx], mc.buf[by], 0ul, ka, mc.buf[bsel], mc.buf[bwb], mc.buf[bvt], dr, kk, ns)
                }
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = d))
    }
}

// Build q51 plane pairs for `rows` rows of n elements: patterned 5-bit quants (position-
// dependent), pow2 f16 d + quarter-value f16 m — every product/sum dyadic, so the CPU float
// reference is BIT-exact under any GPU reduction order.
def private q51_test_planes(rows, n : int; var qp : array<uint8>; var sp : array<uint8>; var wref : array<float>) {
    let nb = n / 32
    let dlut = fixed_array(0.25, 0.5, 1.0, 2.0)
    let mlut = fixed_array(-1.0, 0.5, 1.25, -0.25)
    qp |> resize(rows * nb * 20)
    sp |> resize(rows * nb * 4)
    wref |> resize(rows * n)
    for (b in range(rows * nb)) {
        let d = dlut[(b / 3) % 4]
        let m = mlut[(b * 5 + 1) % 4]
        let dbits = f32_to_f16(d)
        let mbits = f32_to_f16(m)
        sp[b * 4] = uint8(dbits & 0xFF)
        sp[b * 4 + 1] = uint8(dbits >> 8u)
        sp[b * 4 + 2] = uint8(mbits & 0xFF)
        sp[b * 4 + 3] = uint8(mbits >> 8u)
        var qh = 0u
        for (i in range(32)) {
            let q = (b * 11 + i * 7 + i / 3) % 32
            wref[b * 32 + i] = d * float(q) + m
            if ((q & 16) != 0) {
                qh |= 1u << uint(i)
            }
            if (i < 16) {
                qp[b * 20 + i] = uint8(q & 15)
            } else {
                qp[b * 20 + i - 16] = uint8(uint(qp[b * 20 + i - 16]) | (uint(q & 15) << 4u))
            }
        }
        for (i in range(4)) {
            qp[b * 20 + 16 + i] = uint8(qh >> uint(i * 8))
        }
    }
}

// Build iq4nl32 plane pairs for `rows` rows of n elements: patterned nibbles (position-dependent,
// every codebook slot reached), pow2 f16 d - d*LUT[q] is an f16-exact value, so the CPU float
// reference is BIT-exact under any GPU reduction order.
def private iq4nl32_test_planes(rows, n : int; var qp : array<uint8>; var sp : array<uint8>; var wref : array<float>) {
    let nb = n / 32
    let dlut = fixed_array(0.25, 0.5, 1.0, 2.0)
    let lut = iq4nl_lut()
    qp |> resize(rows * nb * 16)
    sp |> resize(rows * nb * 2)
    wref |> resize(rows * n)
    for (b in range(rows * nb)) {
        let d = dlut[(b / 3) % 4]
        let dbits = f32_to_f16(d)
        sp[b * 2] = uint8(dbits & 0xFF)
        sp[b * 2 + 1] = uint8(dbits >> 8u)
        for (i in range(32)) {
            let q = (b * 11 + i * 7 + i / 3) % 16
            wref[b * 32 + i] = d * float(int(lut[q]))
            if (i < 16) {
                qp[b * 16 + i] = uint8(q)
            } else {
                qp[b * 16 + i - 16] = uint8(uint(qp[b * 16 + i - 16]) | (uint(q) << 4u))
            }
        }
    }
}

//! one per-32 format's expert mul_mm kernels: the simdgroup base and the four tensor twins
def private b32_mm_kernels(f) {   //! generic, as the gates are: the Apple half alone instantiates it
    if (f == KqFmt.q51) {
        return (base = kernel_of(@@pf_enc_moe_mm_q51_c), t = kernel_of(@@pf_enc_moe_mm_q51_t_c), th = kernel_of(@@pf_enc_moe_mm_q51_th_c),
            th128 = kernel_of(@@pf_enc_moe_mm_q51_th128_c), thr = kernel_of(@@pf_enc_moe_mm_q51_thr_c))
    }
    if (f != KqFmt.iq4nl32) {
        panic("b32_mm_kernels: {f} is not a per-32 expert mul_mm format")
    }
    return (base = kernel_of(@@pf_enc_moe_mm_iq4nl32_c), t = kernel_of(@@pf_enc_moe_mm_iq4nl32_t_c), th = kernel_of(@@pf_enc_moe_mm_iq4nl32_th_c),
        th128 = kernel_of(@@pf_enc_moe_mm_iq4nl32_th128_c), thr = kernel_of(@@pf_enc_moe_mm_iq4nl32_thr_c))
}

// The gathered per-32 mul_mm (q51, iq4nl32): ne experts with padded-CSR row runs (gather = 0
// bucket-rows mode); exact-arithmetic planes (the A tile's f16 values are dyadic and f16-exact:
// d*q + m <= 63.25 on q51, d*LUT[q] <= 254 on iq4nl32), int x rows — every simdgroup accumulate
// is exact, so the whole padded y compares BIT-exact.
//! device-free: the dev-W knee arithmetic (the pick the board's deep-dense cells ride).
//! Expectations hold at the DEVW_SMALL_PANEL default (32 MiB) - no sidecar loads here
[test]
def test_devw_tile_pick(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let cap = 16ul * 1024ul * 1024ul * 1024ul
        // small panel (16 MiB, narrow N) serves whole
        t |> equal(devw_tile_pick(256l, 4096l, 2048l, false, cap), 1l)
        // deep-dense up/gate (320 MiB, d/64 = 512): first divisor tile under the small knee is
        // 16 - unreachable under a 2..8 cap
        t |> equal(devw_tile_pick(2048l, 32768l, 5120l, false, cap), 16l)
        // deep-dense down (long-K, d/64 = 80): tc = 10, and rows below the long-K floor decline
        t |> equal(devw_tile_pick(2048l, 5120l, 32768l, false, cap), 10l)
        t |> equal(devw_tile_pick(512l, 5120l, 32768l, false, cap), 0l)
        // over-knee under the tiled-rows floor: no form serves - unless the slow-fallback class
        // lowers the floor to 512
        t |> equal(devw_tile_pick(512l, 32768l, 5120l, false, cap), 0l)
        t |> equal(devw_tile_pick(512l, 32768l, 5120l, true, cap), 16l)
        // mid panel (40 MiB) needs the big-rows floor
        t |> equal(devw_tile_pick(2048l, 4096l, 5120l, false, cap), 1l)
        t |> equal(devw_tile_pick(1024l, 4096l, 5120l, false, cap), 0l)
    } else {
        t |> skip("das_metal is not built on this platform; the knee predicate is not compiled")
    }
}

//! device-free: the dense tall-kq pick - the byte knee, the 128-row floor, and the
//! set_metal_prefill_tall A/B seat (the seat clause is what the override contract pins)
[test]
def test_tallkq_pick(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> success(tallkq_pick(512l, 16384l, 4096l, true), "over-knee tall panel takes")
        t |> success(!tallkq_pick(512l, 16384l, 4096l, false), "set_metal_prefill_tall(false) pins the tall form off")
        t |> success(!tallkq_pick(96l, 16384l, 4096l, true), "under the 128-row floor declines")
        t |> success(!tallkq_pick(512l, 4096l, 4096l, true), "under the byte knee declines")
    } else {
        t |> skip("das_metal is not built on this platform; the knee predicate is not compiled")
    }
}

//! the resident panel registry's two classes on the driver's device: an uncharged panel (a routed layer's router rows)
//! is served outside the dev-W budget and charges nothing, a charged one answers to the budget
[test]
def test_resident_panel_charge(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_metal_decode_gate(t) $(var queue : MetalCommandQueue?) {
            t |> success(queue != null, "the driver's device is up")
            metal_prefill_shutdown()
            let before = metal_prefill_resident_ask(0x51ul, 4096ul, false)
            t |> success(before.got && before.fresh, "an uncharged ask makes its panel")
            t |> equal(before.charged_bytes, 0ul, "and charges nothing")
            let again = metal_prefill_resident_ask(0x51ul, 4096ul, false)
            t |> success(again.got && !again.fresh, "the same key answers with the panel it made")
            let foreign = metal_prefill_resident_ask(0x51ul, 8192ul, false)
            t |> success(!foreign.got, "a hit of another size is refused")
            let charged = metal_prefill_resident_ask(0x52ul, 4096ul, true)
            let want = charged.got ? 4096ul : 0ul
            t |> equal(charged.charged_bytes, want, "a charged ask counts its bytes exactly when the budget serves it")
            t |> success(charged.got || !charged.fresh, "a refused ask makes nothing")
            metal_prefill_shutdown()
            let after = metal_prefill_resident_ask(0x52ul, 4096ul, false)
            t |> success(after.got && after.fresh, "the shutdown dropped the registry: the key is made again")
            t |> equal(after.charged_bytes, 0ul, "with the charge back at zero")
            metal_prefill_shutdown()
        }
        t |> equal(metal_log_leaks_if_any(), 0l)
    } else {
        t |> skip("das_metal is not built on this platform; the registry is not compiled")
    }
}

def private moe_mulmm_b32_gate(t : T?; var dev, queue; fmt : KqFmt; kdim, ndim : int; counts : array<int>) {
    let tag = "moe {fmt} mulmm"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let ks = b32_mm_kernels(fmt)
        var pso = mc |> keep_stamp(ks.base, err)
        return if (!pso_ok(t, tag, pso, err))
        let ne = length(counts)
        let nb = kdim / 32
        var inscope planes <- moe_gemv_planes(fmt, ne * ndim, kdim)
        var inscope csr <- padded_bases(counts)
        let pad = csr.pad
        var inscope xv : array<float>
        xv |> resize(pad * kdim)
        for (i in range(pad * kdim)) {
            xv[i] = float((i * 5) % 9 - 4)
        }
        // padded tail rows are computed from their (defined) x rows too — want covers the WHOLE buffer
        var inscope want : array<float>
        want |> resize(pad * ndim)
        for (e in range(ne)) {
            let rows = (counts[e] + 31) / 32 * 32
            for (r in range(rows)) {
                let row = int(csr.base[e]) + r
                for (col in range(ndim)) {
                    var acc = 0.0
                    for (c in range(kdim)) {
                        acc += planes.wref[(e * ndim + col) * kdim + c] * xv[row * kdim + c]
                    }
                    want[row * ndim + col] = acc
                }
            }
        }
        let bw = mc |> in_plain(planes.p0)
        let bs = mc |> in_plain(planes.p1)
        let bx = mc |> in_(xv)
        let by = mc |> out_words(pad * ndim, -1000.0)
        let bcnt = mc |> in_plain(csr.cnt)
        let bbase = mc |> in_plain(csr.base)
        let bbkt = mc |> in_plain([0u])   // gather = 0: bkt unread — a 4B stand-in satisfies the binding
        var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nb),
            nk = 1u, gather = 0u, hasb = 0u)
        let ran = mc |> cell_record(t, "{tag} kdim={kdim} ndim={ndim} ne={ne} pad={pad}") $(enc : MetalComputeEncoder?) {
            invoke(ks.base.enc, enc, pso, mc.buf[bs], 0ul, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by], ka, mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], int64(ndim), int64(pad), int64(ne))
        }
        if (ran) {
            check_buf(t, tag, mc.buf[by], want, BAR_EXACT, Shape(row_width = ndim))
        }
        // the contiguous tensor twin (TH stamp over an f16 X): same exact-arithmetic oracle —
        // pow2 d / quarter m / int x all stage to f16 losslessly, so the twin is BIT-exact too.
        // The leg keys on the ENGINE's own pick predicate: weakening moe_q51_twin_fits dispatches
        // the twin on the kdim=96 fixture, whose 64-deep K walk reads off the plane and mismatches.
        return if (!moe_q51_twin_fits(uint(kdim)))
        var terr : string
        var tw_pso = mc |> keep_stamp(ks.th, terr)
        return if (!tensor_pso_or_skip(t, "{tag} twin", tw_pso, dev, terr))
        let bxh = mc |> in_plain([for (v in xv); float16(v)])
        let by_t = mc |> out_words(pad * ndim, -500.0)
        let rant = mc |> cell_record(t, "{tag} twin") $(enc : MetalComputeEncoder?) {
            invoke(ks.th.enc, enc, tw_pso, mc.buf[bs], 0ul, mc.buf[bw], 0ul, mc.buf[bxh], mc.buf[by_t], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(pad), int64(ne))
        }
        if (rant) {
            check_buf(t, "{tag} twin", mc.buf[by_t], want, BAR_EXACT, Shape(row_width = ndim))
        }
        // the float-X T stamp (the bxh == null pick arm): stages raw f32 rows to the same
        // f16 tiles, so the int-valued fixtures stay bit-exact through it too
        var tf_pso = mc |> keep_stamp(ks.t, terr)
        if (pso_ok(t, "{tag} T stamp", tf_pso, terr)) {
            let by_f = mc |> out_words(pad * ndim, -600.0)
            let ranf = mc |> cell_record(t, "{tag} T stamp") $(enc : MetalComputeEncoder?) {
                invoke(ks.t.enc, enc, tf_pso, mc.buf[bs], 0ul, mc.buf[bw], 0ul, mc.buf[bx], mc.buf[by_f], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(pad), int64(ne))
            }
            if (ranf) {
                check_buf(t, "{tag} T stamp", mc.buf[by_f], want, BAR_EXACT, Shape(row_width = ndim))
            }
        }
        // the tall pair: 128-floor stamp + remainder stamp over the same fixtures — the
        // toolchain is proven (tw_pso compiled), so a pair PSO that fails to compile is a RED
        var tl_pso = mc |> keep_stamp(ks.th128, terr)
        var rm_pso = mc |> keep_stamp(ks.thr, terr)
        t |> success(tl_pso != null && rm_pso != null, "{tag} tall pair: pipeline: {terr}")
        return if (tl_pso == null || rm_pso == null)
        var maxce = 0
        for (c in csr.cnt) {
            maxce = max(maxce, int(c))
        }
        let by_p = mc |> out_words(pad * ndim, -700.0)
        let ranp = mc |> cell_record(t, "{tag} tall pair") $(enc : MetalComputeEncoder?) {
            invoke(ks.th128.enc, enc, tl_pso, mc.buf[bs], 0ul, mc.buf[bw], 0ul, mc.buf[bxh], mc.buf[by_p], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(maxce), int64(ne))
            invoke(ks.thr.enc, enc, rm_pso, mc.buf[bs], 0ul, mc.buf[bw], 0ul, mc.buf[bxh], mc.buf[by_p], ka, mc.buf[bcnt], mc.buf[bbase], int64(ndim), int64(ne))
        }
        if (ranp) {
            check_buf(t, "{tag} tall pair", mc.buf[by_p], want, BAR_EXACT, Shape(row_width = ndim))
        }
    }
}

// The weighted top-k reduce on exact data (integer dn, pow2 weights), BIT-exact.
def private moe_combine_gate(t : T?; var dev, queue; dim, k : int) {
    let tag = "moe combine dim={dim} k={k}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_moe_combine)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, "moe combine", pso, err))
        let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
        var inscope dn : array<float>
        dn |> resize(k * dim)
        for (i in range(k * dim)) {
            dn[i] = float((i * 11 + 2) % 17 - 8)
        }
        var inscope selw : array<float>
        selw |> reserve(k)
        for (j in range(k)) {
            selw |> push(lut[j % 4])
        }
        var inscope want : array<float>
        want |> resize(dim)
        for (i in range(dim)) {
            var acc = 0.0
            for (j in range(k)) {
                acc += selw[j] * dn[j * dim + i]
            }
            want[i] = acc
        }
        // the sel buffer's layout: [k idx u32][k w f32] — the combine binds the float half at byte k*4
        var inscope sel : array<float>
        sel |> resize(2 * k)
        for (j in range(k)) {
            sel[k + j] = selw[j]
        }
        let bsel = mc |> in_(sel)
        let bdn = mc |> in_(dn)
        let by = mc |> out_words(dim, -1000.0)
        let bdim = mc |> in_plain([uint(dim)])
        let bnk = mc |> in_plain([uint(k)])
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {   // sel as the float w view - stream row layout in-kernel
                invoke(st.enc, enc, mc.buf[bdn], mc.buf[bsel], mc.buf[by], mc.buf[bdim], mc.buf[bnk], int64(dim), 1l)
            }
        }
        return if (!ran)
        check_buf(t, tag, mc.buf[by], want, BAR_EXACT, FLAT_SHAPE)
    }
}

// ===== deltanet kernel gates (Wave D) =====

//! the sequential delta rule per v-head over the conv rows `cv` from state `st0` (k-heads fan via h % nkh): the beta and decay gates, the output rows, the last state and the state after row npos - 2 (the dual store's)
def private dn_rule_oracle(cv, braw, graw, aa, dtb, st0 : array<float>; nkh, nvh, npos, ds : int) : tuple<beta : array<float>; decay : array<float>; o : array<float>; st : array<float>; stmid : array<float>> {
    let kd = nkh * ds
    let di = nvh * ds
    let cd = 2 * kd + di
    var beta <- [for (v in braw); 1.0 / (1.0 + exp(-v))]
    var decay : array<float>
    decay |> resize(npos * nvh)
    for (p in range(npos)) {
        for (h in range(nvh)) {
            let gr = graw[p * nvh + h] + dtb[h]
            decay[p * nvh + h] = exp(aa[h] * (gr > 20.0 ? gr : log(1.0 + exp(gr))))
        }
    }
    var o, st, stmid : array<float>
    var inscope skd : array<float>
    o |> resize(npos * di)
    st := st0
    stmid |> resize(nvh * ds * ds)
    skd |> resize(ds)
    for (h in range(nvh)) {
        let kh = h % nkh
        let sb = h * ds * ds
        for (p in range(npos)) {
            let dec = decay[p * nvh + h]
            let qb = p * cd + kh * ds
            let kb = p * cd + kd + kh * ds
            let vb = p * cd + 2 * kd + h * ds
            for (j in range(ds)) {
                skd[j] = 0.0
            }
            for (i in range(ds)) {
                let ki = cv[kb + i]
                for (j in range(ds)) {
                    let sv = st[sb + i * ds + j] * dec
                    st[sb + i * ds + j] = sv
                    skd[j] += ki * sv
                }
            }
            for (j in range(ds)) {
                skd[j] = (cv[vb + j] - skd[j]) * beta[p * nvh + h]
                o[p * di + h * ds + j] = 0.0
            }
            for (i in range(ds)) {
                let ki = cv[kb + i]
                let qi = cv[qb + i]
                for (j in range(ds)) {
                    let sv = st[sb + i * ds + j] + ki * skd[j]
                    st[sb + i * ds + j] = sv
                    o[p * di + h * ds + j] += qi * sv
                }
            }
            if (p + 2 == npos) {
                for (k in range(ds * ds)) {
                    stmid[sb + k] = st[sb + k]
                }
            }
        }
    }
    return <- (beta = beta, decay = decay, o = o, st = st, stmid = stmid)
}

//! MetalDnPrep + MetalDnScanH vs the sequential rule on f16-lattice rows (so the f32 bar); gates compared alone; a rerun on the left state must miss
def private dn_scan_half_gate(t : T?; var dev, queue; nkh, nvh, npos : int) {
    let ds = 128
    let kd = nkh * ds
    let di = nvh * ds
    let cd = 2 * kd + di
    let tag = "dn prep+scan half npos={npos} nkh={nkh} nvh={nvh}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sprep = stamp_of(@@enc_dn_prep)
        let sscan = stamp_of(@@enc_dn_scan_h)
        var pso_prep = mc |> keep_stamp(sprep, err)
        return if (!pso_ok(t, "dn prep", pso_prep, err))
        var pso_scan = mc |> keep_stamp(sscan, err)
        return if (!pso_ok(t, "dn scan half", pso_scan, err))
        var inscope cv <- [for (k in range(npos * cd)); float((k * 7 + 3) % 29 - 14) / 128.0]   // f16-exact
        var inscope braw <- [for (k in range(npos * nvh)); 0.3 * float(k % 11) - 1.5]
        var inscope graw <- [for (k in range(npos * nvh)); 0.2 * float(k % 13) - 1.0]
        var inscope aa <- [for (k in range(nvh)); -(0.4 + 0.15 * float(k))]
        var inscope dtb <- [for (k in range(nvh)); 0.1 * float(k) - 0.2]
        var inscope st0 <- [for (k in range(nvh * ds * ds)); 0.01 * float(k % 37) - 0.18]
        var inscope(bwant, gwant, owant, stw, stmid) <- dn_rule_oracle(cv, braw, graw, aa, dtb, st0, nkh, nvh, npos, ds)
        var inscope cvh <- [for (v in cv); float16(v)]
        var ka = DnArgs(cd = uint(cd), dconv = 4u, npos = uint(npos), kd = uint(kd), ds = uint(ds),
            nvh = uint(nvh), nkh = uint(nkh), di = uint(di), eps = EPS, qscale = 1.0)
        let bcv = mc |> in_plain(cvh)
        let bst = mc |> io_(st0)
        let bbr = mc |> io_(braw)
        let bgr = mc |> io_(graw)
        let baa = mc |> in_(aa)
        let bdt = mc |> in_(dtb)
        let bo = mc |> out_(npos * di)
        let ran = mc |> cell_record(t, tag) $(enc : MetalComputeEncoder?) {
            stamp_run(sprep, pso_prep) {
                invoke(sprep.enc, enc, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], ka, int64((npos * nvh + 63) / 64 * 64))
            }
            stamp_run(sscan, pso_scan) {
                invoke(sscan.enc, enc, mc.buf[bcv], mc.buf[bst], 0ul, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], mc.buf[bo], mc.buf[bst], 0ul, ka, int64(ds), int64(nvh))
            }
        }
        return if (!ran)
        check_buf(t, "{tag} beta", mc.buf[bbr], bwant, BAR_FP, Shape(row_width = nvh))
        check_buf(t, "{tag} decay", mc.buf[bgr], gwant, BAR_FP, Shape(row_width = nvh))
        check_buf(t, "{tag} out", mc.buf[bo], owant, BAR_FP, Shape(row_width = di, head_width = ds))
        check_buf(t, "{tag} state", mc.buf[bst], stw, BAR_FP, Shape(row_width = ds))
        //! the template's other forms on this stamp: the state a row back kept in a second plane, the last state landing in
        //! that plane alone, and the rows read from a row offset
        let prep_total = int64((npos * nvh + 63) / 64 * 64)
        let bstm = mc |> out_(nvh * ds * ds)
        if (npos >= 2) {
            mc |> plane_upload(bst, st0)
            mc |> plane_upload(bbr, braw)
            mc |> plane_upload(bgr, graw)
            mc |> plane_refill(bo)
            var kmid = ka
            kmid.mid = 1u
            mc |> cell_record(t, "{tag} mid") $(enc : MetalComputeEncoder?) {
                stamp_run(sprep, pso_prep) {
                    invoke(sprep.enc, enc, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], kmid, prep_total)
                }
                stamp_run(sscan, pso_scan) {
                    invoke(sscan.enc, enc, mc.buf[bcv], mc.buf[bst], 0ul, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], mc.buf[bo], mc.buf[bstm], 0ul, kmid, int64(ds), int64(nvh))
                }
            }
            check_buf(t, "{tag} mid out", mc.buf[bo], owant, BAR_FP, Shape(row_width = di, head_width = ds))
            check_buf(t, "{tag} mid: the last state", mc.buf[bst], stw, BAR_FP, Shape(row_width = ds))
            check_buf(t, "{tag} mid: the state a row back", mc.buf[bstm], stmid, BAR_FP, Shape(row_width = ds))
        }
        mc |> plane_upload(bst, st0)
        mc |> plane_upload(bbr, braw)
        mc |> plane_upload(bgr, graw)
        mc |> plane_refill(bo)
        mc |> plane_refill(bstm)
        var ksplit = ka
        ksplit.split = 1u
        mc |> cell_record(t, "{tag} split") $(enc : MetalComputeEncoder?) {
            stamp_run(sprep, pso_prep) {
                invoke(sprep.enc, enc, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], ksplit, prep_total)
            }
            stamp_run(sscan, pso_scan) {
                invoke(sscan.enc, enc, mc.buf[bcv], mc.buf[bst], 0ul, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], mc.buf[bo], mc.buf[bstm], 0ul, ksplit, int64(ds), int64(nvh))
            }
        }
        check_buf(t, "{tag} split out", mc.buf[bo], owant, BAR_FP, Shape(row_width = di, head_width = ds))
        check_buf(t, "{tag} split: the last state lands in the second plane", mc.buf[bstm], stw, BAR_FP, Shape(row_width = ds))
        check_buf_exact(t, "{tag} split: the first plane is left as it was", mc.buf[bst], st0)
        var inscope cvh1 <- [for (k in range((npos + 1) * cd)); k < cd ? float16(100.0) : cvh[k - cd]]
        var inscope braw1 <- [for (k in range((npos + 1) * nvh)); k < nvh ? 50.0 : braw[k - nvh]]
        var inscope graw1 <- [for (k in range((npos + 1) * nvh)); k < nvh ? 50.0 : graw[k - nvh]]
        var inscope bwant1 <- [for (k in range((npos + 1) * nvh)); k < nvh ? 50.0 : bwant[k - nvh]]
        var inscope gwant1 <- [for (k in range((npos + 1) * nvh)); k < nvh ? 50.0 : gwant[k - nvh]]
        let bcv1 = mc |> in_plain(cvh1)
        let bbr1 = mc |> io_(braw1)
        let bgr1 = mc |> io_(graw1)
        let bo1 = mc |> out_(npos * di, di)
        var krow = ka
        krow.row = 1u
        mc |> cell_record(t, "{tag} row") $(enc : MetalComputeEncoder?) {
            stamp_run(sprep, pso_prep) {
                invoke(sprep.enc, enc, mc.buf[bbr1], mc.buf[bgr1], mc.buf[baa], mc.buf[bdt], krow, prep_total)
            }
            stamp_run(sscan, pso_scan) {
                invoke(sscan.enc, enc, mc.buf[bcv1], mc.buf[bst], 0ul, mc.buf[bbr1], mc.buf[bgr1], mc.buf[baa], mc.buf[bdt], mc.buf[bo1], mc.buf[bst], 0ul, krow, int64(ds), int64(nvh))
            }
        }
        check_buf(t, "{tag} row: beta, the row before the offset as it was", mc.buf[bbr1], bwant1, BAR_FP, Shape(row_width = nvh))
        check_buf(t, "{tag} row: decay, the row before the offset as it was", mc.buf[bgr1], gwant1, BAR_FP, Shape(row_width = nvh))
        var inscope orow <- mc |> plane_run(bo1)
        check(t, "{tag} row out", orow, owant, BAR_FP, Shape(row_width = di, head_width = ds))
        var inscope ohead <- mc |> plane_as(bo1, 0.0, di)
        var unwritten = 0
        for (v in ohead) {
            unwritten += v != v ? 1 : 0
        }
        t |> equal(unwritten, di, "{tag} row: the output row before the offset keeps its sentinel")
        check_buf(t, "{tag} row state", mc.buf[bst], stw, BAR_FP, Shape(row_width = ds))
        //! the control: the walk again over the gates it just consumed, the state it left - both moved
        mc |> cell_record(t, "{tag} control") $(enc : MetalComputeEncoder?) {
            stamp_run(sprep, pso_prep) {
                invoke(sprep.enc, enc, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], ka, int64((npos * nvh + 63) / 64 * 64))
            }
            stamp_run(sscan, pso_scan) {
                invoke(sscan.enc, enc, mc.buf[bcv], mc.buf[bst], 0ul, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], mc.buf[bo], mc.buf[bst], 0ul, ka, int64(ds), int64(nvh))
            }
        }
        t |> success(mc |> rerun_mismatches(bo, owant, 1e-4, 0.0, "{tag} CONTROL(expected-red)") > 0, "{tag} control: gates prepared twice over a moved state must miss")
    }
}

// The whole recurrent chain vs a sequential CPU mirror — conv+SiLU (history straddle), q/k L2
// norm + q pre-scale, the fused sequential delta rule, the z-gated out-norm — compared at each
// stage's seam so a red localizes to (stage). ds pinned 128 (the serve gate's shape).
def private dn_chain_gate(t : T?; var dev, queue; nkh, nvh, dconv, npos : int) {
    let ds = 128
    let kd = nkh * ds
    let di = nvh * ds
    let cd = 2 * kd + di
    let taps = dconv - 1
    let qscale = 1.0 / sqrt(float(ds))
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sconv = stamp_of(@@enc_dn_conv)
        let shist = stamp_of(@@enc_dn_conv_hist)
        let sl2 = stamp_of(@@enc_dn_l2norm)
        let sscan = stamp_of(@@enc_dn_scan)
        let sgate = stamp_of(@@enc_dn_gate)
        let sgsig = stamp_of(@@enc_dn_gate_sig)
        var pso_conv = mc |> keep_stamp(sconv, err)
        let conv_ok = pso_ok(t, "dn conv", pso_conv, err)
        var pso_hist = mc |> keep_stamp(shist, err)
        let hist_ok = pso_ok(t, "dn hist", pso_hist, err)
        var pso_l2 = mc |> keep_stamp(sl2, err)
        let l2_ok = pso_ok(t, "dn l2norm", pso_l2, err)
        var pso_scan = mc |> keep_stamp(sscan, err)
        let scan_ok = pso_ok(t, "dn scan", pso_scan, err)
        var pso_gate = mc |> keep_stamp(sgate, err)
        let gate_ok = pso_ok(t, "dn gate", pso_gate, err)
        var pso_gate_sig = mc |> keep_stamp(sgsig, err)
        return if (!pso_ok(t, "dn gate sig", pso_gate_sig, err) || !conv_ok || !hist_ok || !l2_ok || !scan_ok || !gate_ok)
        // inputs — hash patterns; ssm_a strictly negative (= -exp(A_log)), nonzero history + state
        var inscope xq, hist, cw, braw, graw, aa, dtb, st0, zz, wn : array<float>
        xq |> resize(npos * cd)
        hist |> resize(cd * taps)
        cw |> resize(cd * dconv)
        braw |> resize(npos * nvh)
        graw |> resize(npos * nvh)
        aa |> resize(nvh)
        dtb |> resize(nvh)
        st0 |> resize(nvh * ds * ds)
        zz |> resize(npos * di)
        wn |> resize(ds)
        for (k in range(npos * cd)) {
            xq[k] = 0.03 * float(k % 53) - 0.8
        }
        for (k in range(cd * taps)) {
            hist[k] = 0.05 * float(k % 31) - 0.7
        }
        for (k in range(cd * dconv)) {
            cw[k] = 0.04 * float(k % 23) - 0.4
        }
        for (k in range(npos * nvh)) {
            braw[k] = 0.3 * float(k % 11) - 1.5
            graw[k] = 0.2 * float(k % 13) - 1.0
        }
        for (k in range(nvh)) {
            aa[k] = -(0.4 + 0.15 * float(k))
            dtb[k] = 0.1 * float(k) - 0.2
        }
        for (k in range(nvh * ds * ds)) {
            st0[k] = 0.01 * float(k % 37) - 0.18
        }
        for (k in range(npos * di)) {
            zz[k] = 0.07 * float(k % 27) - 0.9
        }
        for (k in range(ds)) {
            wn[k] = 0.6 + 0.02 * float(k % 17)
        }
        // stage 1 mirror: conv + SiLU (window rows or history straddle), then the history advance
        var inscope cv, hwant : array<float>
        cv |> resize(npos * cd)
        hwant |> resize(cd * taps)
        for (p in range(npos)) {
            for (ch in range(cd)) {
                var acc = 0.0
                for (k in range(dconv)) {
                    let sp = p + k - taps
                    let xv = sp >= 0 ? xq[sp * cd + ch] : hist[ch * taps + p + k]
                    acc += xv * cw[ch * dconv + k]
                }
                cv[p * cd + ch] = acc / (1.0 + exp(-acc))
            }
        }
        for (ch in range(cd)) {
            for (k in range(taps)) {
                let src = npos - taps + k
                hwant[ch * taps + k] = src >= 0 ? xq[src * cd + ch] : hist[ch * taps + src + taps]
            }
        }
        // dual-store mirror (npos >= 2): the npos-1 window's history — the MTP verify's shadow store
        var inscope hmid_want : array<float>
        if (npos >= 2) {
            hmid_want |> resize(cd * taps)
            for (ch in range(cd)) {
                for (k in range(taps)) {
                    let src = npos - 1 - taps + k
                    hmid_want[ch * taps + k] = src >= 0 ? xq[src * cd + ch] : hist[ch * taps + src + taps]
                }
            }
        }
        let bx = mc |> in_(xq)
        let bh = mc |> io_(hist)
        let bw = mc |> in_(cw)
        let by = mc |> out_(npos * cd)
        // ONE DnArgs serves all five stages (each ignores the fields it does not read);
        // mid = npos >= 2 arms the dual-store shadow writes in hist and scan
        var ka = DnArgs(cd = uint(cd), dconv = uint(dconv), npos = uint(npos), kd = uint(kd),
            ds = uint(ds), nvh = uint(nvh), nkh = uint(nkh), di = uint(di), eps = EPS,
            qscale = qscale, mid = npos >= 2 ? 1u : 0u)
        mc |> cell_record(t, "dn conv npos={npos}") $(enc : MetalComputeEncoder?) {
            stamp_run(sconv, pso_conv) {
                invoke(sconv.enc, enc, mc.buf[bx], mc.buf[bh], 0ul, mc.buf[bw], mc.buf[by], ka, int64(cd), int64(npos))
            }
        }
        check_buf(t, "dn conv npos={npos}", mc.buf[by], cv, BAR_FP, Shape(row_width = cd))
        let bhm = npos >= 2 ? (mc |> out_words(cd * taps, -1000.0)) : bh
        mc |> cell_record(t, "dn hist npos={npos}") $(enc : MetalComputeEncoder?) {
            stamp_run(shist, pso_hist) {
                invoke(shist.enc, enc, mc.buf[bx], mc.buf[bh], 0ul, mc.buf[bhm], 0ul, ka, int64(cd))
            }
        }
        check_buf(t, "dn hist npos={npos}", mc.buf[bh], hwant, BAR_FP, Shape(row_width = taps))
        if (npos >= 2) {
            check_buf(t, "dn hist mid npos={npos}", mc.buf[bhm], hmid_want, BAR_FP, Shape(row_width = taps))
        }
        // stage 2 mirror: per-head L2 over the q/k heads (contiguous at each position's row start), then the q pre-scale
        for (p in range(npos)) {
            unsafe {
                l2_norm_rows(addr(cv[p * cd]), int64(2 * nkh), int64(ds), EPS)
            }
            for (j in range(kd)) {
                cv[p * cd + j] *= qscale
            }
        }
        mc |> cell_record(t, "dn l2norm npos={npos}") $(enc : MetalComputeEncoder?) {
            stamp_run(sl2, pso_l2) {
                invoke(sl2.enc, enc, mc.buf[by], ka, int64(2 * nkh), int64(npos))
            }
        }
        check_buf(t, "dn l2norm npos={npos}", mc.buf[by], cv, BAR_FP, Shape(row_width = cd, head_width = ds))
        // stage 3 mirror: the sequential delta rule per v-head (k-heads fan via h % nkh)
        var inscope(_, _, owant, stw, stmid) <- dn_rule_oracle(cv, braw, graw, aa, dtb, st0, nkh, nvh, npos, ds)
        let bst = mc |> io_(st0)
        let bbr = mc |> in_(braw)
        let bgr = mc |> in_(graw)
        let baa = mc |> in_(aa)
        let bdt = mc |> in_(dtb)
        let bo = mc |> out_(npos * di)
        let bstm = npos >= 2 ? (mc |> out_words(nvh * ds * ds, -1000.0)) : bst
        mc |> cell_record(t, "dn scan npos={npos} nkh={nkh} nvh={nvh}") $(enc : MetalComputeEncoder?) {
            stamp_run(sscan, pso_scan) {
                invoke(sscan.enc, enc, mc.buf[by], mc.buf[bst], 0ul, mc.buf[bbr], mc.buf[bgr], mc.buf[baa], mc.buf[bdt], mc.buf[bo], mc.buf[bstm], 0ul, ka, int64(ds), int64(nvh))
            }
        }
        check_buf(t, "dn scan npos={npos} out", mc.buf[bo], owant, BAR_FP, Shape(row_width = di, head_width = ds))
        check_buf(t, "dn scan npos={npos} state", mc.buf[bst], stw, BAR_FP, Shape(row_width = ds))
        if (npos >= 2) {
            check_buf(t, "dn scan npos={npos} mid state", mc.buf[bstm], stmid, BAR_FP, Shape(row_width = ds))
        }
        // stage 4 mirror: per-head RMS with the shared weight, ⊙ SiLU(z)
        var inscope gwant : array<float>
        gwant |> resize(npos * di)
        for (p in range(npos)) {
            for (h in range(nvh)) {
                let hb = p * di + h * ds
                unsafe {
                    rmsnorm(addr(gwant[hb]), addr<float const?>(owant[hb]), addr<float const?>(wn[0]), int64(ds), EPS)
                }
                for (j in range(ds)) {
                    let zv = zz[hb + j]
                    gwant[hb + j] *= zv / (1.0 + exp(-zv))
                }
            }
        }
        let bz = mc |> in_(zz)
        let bwn = mc |> in_(wn)
        mc |> cell_record(t, "dn gate npos={npos}") $(enc : MetalComputeEncoder?) {
            stamp_run(sgate, pso_gate) {
                invoke(sgate.enc, enc, mc.buf[bo], mc.buf[bz], mc.buf[bwn], ka, int64(nvh), int64(npos))
            }
        }
        check_buf(t, "dn gate npos={npos}", mc.buf[bo], gwant, BAR_FP, Shape(row_width = di, head_width = ds))
        // the σ(z) stamp (qwen4exp) on the scan oracle's rows, so its leg stands without the scan: the
        // same norm, the gate a sigmoid; the silu form's oracle is the control it must miss
        var inscope gsig : array<float>
        gsig |> resize(npos * di)
        for (p in range(npos)) {
            for (h in range(nvh)) {
                let hb = p * di + h * ds
                unsafe {
                    rmsnorm(addr(gsig[hb]), addr<float const?>(owant[hb]), addr<float const?>(wn[0]), int64(ds), EPS)
                }
                for (j in range(ds)) {
                    gsig[hb + j] *= 1.0 / (1.0 + exp(-zz[hb + j]))
                }
            }
        }
        let bos = mc |> io_(owant)
        mc |> cell_record(t, "dn gate sig npos={npos}") $(enc : MetalComputeEncoder?) {
            stamp_run(sgsig, pso_gate_sig) {
                invoke(sgsig.enc, enc, mc.buf[bos], mc.buf[bz], mc.buf[bwn], ka, int64(nvh), int64(npos))
            }
        }
        check_buf(t, "dn gate sig npos={npos}", mc.buf[bos], gsig, BAR_FP, Shape(row_width = di, head_width = ds))
        t |> success(mc |> rerun_mismatches(bos, gwant, 1e-4, 0.0, "dn gate sig npos={npos} CONTROL(expected-red)") > 0, "dn gate sig control: the silu form's rows must miss")
    }
}

// The 2x-wide [q|gate] deinterleave + the σ(g) out-gate — trivial mirrors, bit-exact.
def private dn_deint_sigmul_gate(t : T?; var dev, queue; nh, hs, npos : int) {
    let qd = nh * hs
    let tag = "dn_deint_sigmul nh={nh} hs={hs} npos={npos}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let sde = stamp_of(@@enc_dn_deint)
        var pso_de = mc |> keep_stamp(sde, err)
        return if (!pso_ok(t, "dn deint", pso_de, err))
        let qs = qd + 64          // a wider destination row stride (the qkv panel form)
        var inscope qg, qwant, gwant : array<float>
        qg |> resize(npos * 2 * qd)
        qwant |> resize(npos * qd)
        gwant |> resize(npos * qd)
        for (k in range(npos * 2 * qd)) {
            qg[k] = 0.09 * float(k % 43) - 1.7
        }
        for (p in range(npos)) {
            for (i in range(qd)) {
                let h = i / hs
                let ii = i % hs
                qwant[p * qd + i] = qg[p * 2 * qd + h * 2 * hs + ii]
                gwant[p * qd + i] = qg[p * 2 * qd + h * 2 * hs + hs + ii]
            }
        }
        let bqg = mc |> in_(qg)
        let bq = mc |> out_(npos * qs)
        let bgt = mc |> out_(npos * qd)
        var ka = DnDeintArgs(hs = uint(hs), qd = uint(qd), qs = uint(qs))
        let dran = mc |> cell_record(t, "dn deint") $(enc : MetalComputeEncoder?) {
            stamp_run(sde, pso_de) {
                invoke(sde.enc, enc, mc.buf[bqg], mc.buf[bq], mc.buf[bgt], ka, int64(qd), int64(npos))
            }
        }
        return if (!dran)
        // the pad tail of each destination row is unwritten — compare only the written columns
        var inscope qall <- mc |> plane_run(bq)
        var inscope qgot <- [for (k in range(npos * qd)); qall[(k / qd) * qs + k % qd]]
        check(t, "{tag} q", qgot, qwant, BAR_EXACT, Shape(row_width = qd, head_width = hs))
        check_buf(t, "{tag} gate", mc.buf[bgt], gwant, BAR_EXACT, Shape(row_width = qd, head_width = hs))
        // sigmul: x ⊙ σ(g) on the deinterleaved gate rows
        var inscope xv, swant : array<float>
        xv |> resize(npos * qd)
        swant |> resize(npos * qd)
        for (k in range(npos * qd)) {
            xv[k] = 0.06 * float(k % 39) - 1.1
            swant[k] = xv[k] / (1.0 + exp(-gwant[k]))
        }
        var inscope sgot <- ew2_gate(t, dev, queue, "sigmul", stamp_of(@@enc_sigmul), xv, gwant, swant, false) $(enc : MetalComputeEncoder?; bx, by, btot, bone : MetalBuffer?; n : int64) {   // the gate rows the deint compare held exact
            enc_sigmul(enc, bx, 0ul, by, 0ul, btot, n)
        }
    }
}

def private axpy_sig_gate(t : T?; var dev, queue; npos, dim : int) {
    let tag = "axpy_sig npos={npos} dim={dim}"
    with_metal_cell(dev, queue) $(var mc : MetalCell) {
        var err : string
        let st = stamp_of(@@enc_axpy_sig)
        var pso = mc |> keep_stamp(st, err)
        return if (!pso_ok(t, "axpy_sig", pso, err))
        let total = npos * dim
        var inscope yv, xv, gv, want : array<float>
        yv |> resize(total)
        xv |> resize(total)
        gv |> resize(npos)
        want |> resize(total)
        for (k in range(total)) {
            yv[k] = 0.05 * float(k % 37) - 0.9
            xv[k] = 0.07 * float(k % 41) - 1.3
        }
        for (p in range(npos)) {
            gv[p] = 0.6 * float(p) - 1.2
        }
        for (k in range(total)) {
            want[k] = yv[k] + xv[k] / (1.0 + exp(-gv[k / dim]))
        }
        let by = mc |> io_(yv)
        let bx = mc |> in_(xv)
        let bg = mc |> in_(gv)
        let btot = mc |> in_plain([uint(total)])
        let bdim = mc |> in_plain([uint(dim)])
        let ran = mc |> cell_record(t, "axpy_sig dispatch") $(enc : MetalComputeEncoder?) {
            stamp_run(st, pso) {
                invoke(st.enc, enc, mc.buf[by], mc.buf[bx], mc.buf[bg], mc.buf[btot], mc.buf[bdim], int64(total))
            }
        }
        check_buf(t, tag, mc.buf[by], want, BAR_FP, Shape(row_width = dim))
    }
}
