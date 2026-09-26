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
require ?das_metal dasllama/dasllama_metal_prefill  // the Phase-6 prefill kernel set (Apple builds)
require ?das_metal dasllama/dasllama_metal_kernels  // the Wave C MoE routing kernel set (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers + fixtures; only the tag-less mismatch compares stay local
require ?das_metal _metal4_probe
require daslib/f16_cvt  // f32_to_f16: the q51 gates hand-pack f16 d/m scale halfwords
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tower
require dasllama/dasllama_moe
require dasllama/dasllama_common
require dasllama/dasllama_rope   // build_rope_tabs - the Pk rope gates' tables
require _attn_oracle
require daslib/jobque_boost
require math

// The Phase-6 GPU prefill kernels vs their dasLLAMA CPU twins on synthetic data. rmsnorm /
// rope / swiglu / attention compare within the shared fp tolerance (reduction order, hardware
// exp vs the CPU's exp/exp4 — all inside 1e-4 relative); the residual add is bit-exact (one
// fadd both sides); the blob GEMV gets exact-arithmetic data (pow2 scales). Off-Apple (or with
// no Metal device) everything feints cleanly.

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

//! GENERIC (untyped dev/queue) so it instantiates only inside the Apple static_if.
[arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-twin-dual-store")]
def private qk_rope_gate(t : T?; dev, queue; hx, donorm, neox, partial : bool) {
    let tag = "qk_rope{hx ? " hx" : ""}{donorm ? " norm" : ""}{neox ? " neox" : ""}{partial ? " part" : ""}"
    var err : string
    var pso = (hx
        ? pipeline_from_source(dev, MetalQkRopeHX_metal_qk_rope_msl, MetalQkRopeHX_metal_qk_rope_msl_entry, MetalQkRopeHX_metal_qk_rope_msl_fastmath, err)
        : pipeline_from_source(dev, MetalQkRope_metal_qk_rope_msl, MetalQkRope_metal_qk_rope_msl_entry, MetalQkRope_metal_qk_rope_msl_fastmath, err))
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let nreal = 6
    let npad = hx ? 2 : 0
    let nrows = nreal + npad
    let hs = HEAD_SIZE
    let nh = N_HEADS
    let rstride = nh * hs
    let rot = partial ? hs / 2 : hs
    let halfr = rot / 2
    var xv : array<float>
    var wv : array<float>
    var tc : array<float>
    var ts : array<float>
    var want : array<float>
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
            if (partial) {
                rope_scaled_neox_tab_part(addr(want[p * rstride]), int64(hs), int64(rot),
                    int64(rstride), addr < float const? >(tc[p * halfr]), addr < float const? >(ts[p * halfr]))
            } elif (neox) {
                rope_scaled_neox_tab(addr(want[p * rstride]), int64(hs),
                    int64(rstride), addr < float const? >(tc[p * halfr]), addr < float const? >(ts[p * halfr]))
            } else {
                rope_scaled_tab(addr(want[p * rstride]), int64(hs),
                    int64(rstride), addr < float const? >(tc[p * halfr]), addr < float const? >(ts[p * halfr]))
            }
        }
    }
    var moved = 0
    for (k in range(nreal * rstride)) {
        moved += fapprox(want[k], xv[k]) ? 0 : 1
    }
    t |> success(moved > 0, "{tag}: oracle is not the identity ({moved} moved)")
    var bx = buf_upload(dev, xv)
    var bw = buf_upload(dev, wv)
    var btc = buf_upload(dev, tc)
    var bts = buf_upload(dev, ts)
    var bxh = hx ? buf_fill(dev, nrows * rstride, float16(-77.0)) : null
    var ka = QkRopeArgs(rstride = uint(rstride), hs = uint(hs), eps = EPS,
        donorm = donorm ? 1u : 0u, neox = neox || partial ? 1u : 0u, rot = uint(rot), nreal = uint(nreal))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, hx ? MetalQkRopeHX_metal_qk_rope_msl_tgmem : MetalQkRope_metal_qk_rope_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, btc, 0ul, 2)
        metal_set_buffer(enc, bts, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr < void? >(ka)), uint64(typeinfo sizeof(ka)), 4)
        if (hx) {
            metal_set_buffer(enc, bxh, 0ul, 5)
        }
        metal_dispatch_threadgroups(enc, uint3(uint(nh), uint(nrows), 1u), uint3(uint(hs / 2), 1u, 1u))
    }
    t |> success(ran, "{tag} dispatch: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(bx, want, tag), 0)
        if (hx) {
            var badh = 0
            unsafe {
                let pf = reinterpret<float const?>(metal_buffer_contents(bx))
                let ph = reinterpret<uint16 const?>(metal_buffer_contents(bxh))
                for (k in range(nrows * rstride)) {
                    let wantbits = uint(reinterpret<uint16>(float16(pf[k])))
                    if (uint(ph[k]) != wantbits) {
                        badh++
                    }
                }
            }
            t |> equal(badh, 0, "{tag}: f16 twin bit-equals float16(panel)")
        }
    }
    metal_release(bx)
    metal_release(bw)
    metal_release(btc)
    metal_release(bts)
    if (bxh != null) {
        metal_release(bxh)
    }
    metal_release(pso)
    delete xv
    delete wv
    delete tc
    delete ts
    delete want
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
def private attn_trio_gate(t : T?; dev, queue; head_size, n_heads, n_kv_heads, npos : int; mm : bool; keys : AttnKeys = AttnKeys()) {
    let kv_mul = n_heads / n_kv_heads
    let qd = n_heads * head_size
    let kv_dim = n_kv_heads * head_size
    let nk = keys.qoff + npos
    let nkp = (nk + 63) / 64 * 64           // 64-padded key extent (the driver's nk64)
    let qp = (npos + 31) / 32 * 32          // padded query rows (continuation: != nkp)
    let tag = "attention trio (hs={head_size}, mm={mm}, qoff={keys.qoff}, w={keys.window}, span=[{keys.ulo},{keys.uend}), cap={keys.softcap}, sink={keys.hass})"
    var err : string
    var pso_sc = (mm
        ? pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, err))
    t |> success(pso_sc != null, "{tag} qk pipeline: {err}")
    var pso_sm = pipeline_from_source(dev, metal_attn_rowstat_msl, metal_attn_rowstat_msl_entry, metal_attn_rowstat_msl_fastmath, err)
    t |> success(pso_sm != null, "{tag} rowstat pipeline: {err}")
    var pso_av = (mm
        ? pipeline_from_source(dev, MetalAttnAVMm_metal_attn_av_mm_msl, MetalAttnAVMm_metal_attn_av_mm_msl_entry, MetalAttnAVMm_metal_attn_av_mm_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, err))
    t |> success(pso_av != null, "{tag} av pipeline: {err}")
    let scale = 1.0 / sqrt(float(head_size))
    var aq : array<float>
    var ak : array<float>
    var av : array<float>
    var axb_want : array<float>
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
    var sinks : array<float>
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
    var baq = buf_upload(dev, aq)
    var bak = buf_upload(dev, ak)
    var bav = buf_upload(dev, av)
    // +1000 poison: production leaves QK-skipped slab positions as pool garbage — a mask hole
    // in rowstat/pf_p_weight EXPLODES through exp() and reds the compare (0 or -1000 would hide)
    var batt = buf_fill(dev, n_heads * qp * nkp, float16(1000.0))
    var bstat = buf_fill(dev, n_heads * qp * 2, 0.0)
    var bxb = buf_fill(dev, qp * qd, 0.0)
    var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kv_dim), head_size = uint(head_size),
        kv_mul = uint(kv_mul), npos = uint(nk), np32 = uint(nkp), scale = scale,
        qoff = uint(keys.qoff), qrows = uint(qp), window = uint(keys.window), softcap = keys.softcap, hass = uint(keys.hass),
        uend = uint(keys.uend), ulo = uint(keys.ulo))
    var bcap0 = keys.hass != 0 ? buf_upload(dev, sinks) : buf_f32(dev, 0.0)   // the sink plane (hass = 0: never read)
    let aran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_sc)
        metal_set_threadgroup_memory_length(enc, mm ? metal_attn_qk_mm_msl_tgmem : metal_attn_qk_msl_tgmem, 0)
        metal_set_buffer(enc, baq, 0ul, 0)
        metal_set_buffer(enc, bak, 0ul, 1)
        metal_set_buffer(enc, batt, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(qp / 32), uint(nkp / (mm ? 64 : 32)), uint(n_heads)), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_sm)
        metal_set_threadgroup_memory_length(enc, metal_attn_rowstat_msl_tgmem, 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 1)
        metal_set_buffer(enc, bstat, 0ul, 2)
        metal_set_buffer(enc, bcap0, 0ul, 20)   // sink identity (hass = 0)
        metal_dispatch_threadgroups(enc, uint3(uint(npos), uint(n_heads), 1u), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_av)
        metal_set_threadgroup_memory_length(enc, mm ? MetalAttnAVMm_metal_attn_av_mm_msl_tgmem : metal_attn_av_msl_tgmem, 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_buffer(enc, bav, 0ul, 1)
        metal_set_buffer(enc, bxb, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_set_buffer(enc, bstat, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint(qp / 32), uint(head_size / (mm ? 64 : 32)), uint(n_heads)), uint3(128u, 1u, 1u))
    }
    t |> success(aran, "{tag} dispatches: {err}")
    // the f16 score slab + half staging — the envelope is the half-precision score shift
    // through softmax, ~1% of the convex-combination output (both variants)
    t |> equal(buf_mismatch_tol(bxb, axb_want, 2e-2), 0)
    metal_release(baq)
    metal_release(bak)
    metal_release(bav)
    metal_release(batt)
    metal_release(bstat)
    metal_release(bxb)
    metal_release(bcap0)
    metal_release(pso_sc)
    metal_release(pso_sm)
    metal_release(pso_av)
    delete aq
    delete ak
    delete av
    delete axb_want
    delete sinks
}

//! the f16-exact q / k / v rows of `n` elements the tower and conformer attention cells share
def private f16_qkv_rows(n : int) : tuple<q : array<float>; k : array<float>; v : array<float>> {
    var q : array<float>
    var k : array<float>
    var v : array<float>
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
def private tower_flash_kvhc_gate(t : T?; dev, queue) {
    let tag = "tower_flash"
    var err : string
    var pso = pipeline_from_source(dev, metal_tower_flash_msl, metal_tower_flash_msl_entry, metal_tower_flash_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    var pso_hc = pipeline_from_source(dev, metal_tower_kv_hc_msl, metal_tower_kv_hc_msl_entry, metal_tower_kv_hc_msl_fastmath, err)
    t |> success(pso_hc != null, "{tag} kv_hc pipeline: {err}")
    return if (pso_hc == null)
    let np = 128
    let heads = 2
    let hs = 72
    let qd = heads * hs
    var (qv, kv, vv) <- f16_qkv_rows(np * qd)
    // the SECOND 64-key block carries every row's running max (x4, a power of two, so the keys
    // stay f16-exact): with a uniform K the max settles in block 0 and the flash's cross-block
    // rescale multiplies by 1, which no compare can witness
    for (i in range(64 * qd, np * qd)) {
        kv[i] = kv[i] * 4.0
    }
    var khc : array<float16>
    var vhc : array<float16>
    khc |> resize(np * qd)
    vhc |> resize(np * qd + 256)   //! + the flash's V tail slack: its dead PV tiles read past the last V row
    for (p in range(np)) {
        for (c in range(qd)) {
            khc[((c / hs) * np + p) * hs + c % hs] = float16(kv[p * qd + c])
            vhc[((c / hs) * np + p) * hs + c % hs] = float16(vv[p * qd + c])
        }
    }
    var want : array<float>
    want |> resize(np * qd)
    let scale = 1.0 / sqrt(float(hs))
    var hp : array<float>
    var att : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        attention_bidir(want, qv, kv, vv, hp, att, int64(np), int64(qd), int64(heads), scale)
    }
    delete hp
    delete att
    var bq = buf_upload(dev, qv)
    var bkc = buf_upload(dev, kv)
    var bvc = buf_upload(dev, vv)
    var bkh = buf_fill(dev, np * qd, float16(-77.0))
    var bvh = buf_fill(dev, np * qd + 256, float16(-77.0))
    var bo = buf_fill(dev, np * qd, -1.0e30)
    let ka = AttnArgs(qd = uint(qd), kv_dim = uint(qd), head_size = uint(hs), kv_mul = 1u,
        npos = uint(np), np32 = uint(np), scale = scale,
        qoff = 0u, qrows = uint(np), window = 0u, softcap = 0.0, hass = 0u, uend = uint(np))
    let run <- @(bvx : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            var u_hs = uint(hs)
            var u_d = uint(qd)
            var u_np = uint(np)
            var u_tot = uint(np * qd)
            metal_set_pipeline(enc, pso_hc)
            metal_set_buffer(enc, bkc, 0ul, 0)
            metal_set_buffer(enc, bkh, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(u_hs)), 4ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(u_d)), 4ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(u_np)), 4ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(u_tot)), 4ul, 5)
            metal_dispatch_threadgroups(enc, uint3(uint((np * qd + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
            metal_set_buffer(enc, bvx, 0ul, 0)
            metal_set_buffer(enc, bvh, 0ul, 1)
            metal_dispatch_threadgroups(enc, uint3(uint((np * qd + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
            metal_set_pipeline(enc, pso)
            metal_set_threadgroup_memory_length(enc, metal_tower_flash_msl_tgmem, 0)
            metal_set_buffer(enc, bq, 0ul, 0)
            metal_set_buffer(enc, bkh, 0ul, 1)
            metal_set_buffer(enc, bvh, 0ul, 2)
            metal_set_buffer(enc, bo, 0ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
            metal_dispatch_threadgroups(enc, uint3(uint(np / 8), uint(heads), 1u), uint3(128u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    return if (!invoke(run, bvc))
    t |> equal(buf_mismatch_exact(bkh, khc, "{tag} kv_hc K panel"), 0)
    var vslice : array<float16>
    vslice |> resize(np * qd)
    for (i in range(np * qd)) {
        vslice[i] = vhc[i]
    }
    t |> equal(buf_mismatch_exact(bvh, vslice, "{tag} kv_hc V panel"), 0)
    t |> equal(buf_mismatch_bars(bo, want, 1e-3, 1e-4, tag), 0)
    vv[(np / 2) * qd + hs / 2] += 3.0
    var bvc2 = buf_upload(dev, vv)
    if (invoke(run, bvc2)) {
        t |> success(buf_mismatch_bars(bo, want, 1e-3, 1e-4, "{tag} poison (MUST mismatch)") > 0,
            "{tag}: a poisoned V element reds the bars through kv_hc + flash")
    }
    metal_release(bq)
    metal_release(bkc)
    metal_release(bvc)
    metal_release(bvc2)
    metal_release(bkh)
    metal_release(bvh)
    metal_release(bo)
    metal_release(pso)
    metal_release(pso_hc)
    delete qv
    delete kv
    delete vv
    delete khc
    delete vhc
    delete vslice
    delete want
}

// ===== the qwen2.5 ViT per-window attention (block-diagonal, ragged last window) =====
[arch(at="../ARCHITECTURE_GPU_TOWER.md#tower-attn-routes")]
def private tower_win_gate(t : T?; dev, queue) {
    let tag = "tower_win"
    var err : string
    var pso = pipeline_from_source(dev, metal_tower_win_msl, metal_tower_win_msl_entry, metal_tower_win_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let np = 160
    let heads = 2
    let hs = 80
    let qd = heads * hs
    let wlo <- [ 0u, 64u, 128u, 160u ]   //! two full windows + a ragged 32-row tail
    let nwin = 3
    var (qv, kv, vv) <- f16_qkv_rows(np * qd)
    var want : array<float>
    want |> resize(np * qd)
    let scale = 1.0 / sqrt(float(hs))
    let win_lo <- [for (w in wlo); int64(w)]
    var hp : array<float>
    var att : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        attention_bidir_windows(want, qv, kv, vv, hp, att, win_lo, int64(qd), int64(heads), scale)
    }
    delete hp
    delete att
    var bq = buf_upload(dev, qv)
    var bk = buf_upload(dev, kv)
    var bw = buf_upload(dev, wlo)
    var bo = buf_fill(dev, np * qd, -1.0e30)
    let ka = AttnArgs(qd = uint(qd), kv_dim = uint(qd), head_size = uint(hs), kv_mul = 1u,
        npos = uint(np), np32 = uint(np), scale = scale,
        qoff = 0u, qrows = uint(np), window = 0u, softcap = 0.0, hass = 0u, uend = uint(np))
    let run <- @(bvx : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bq, 0ul, 0)
            metal_set_buffer(enc, bk, 0ul, 1)
            metal_set_buffer(enc, bvx, 0ul, 2)
            metal_set_buffer(enc, bo, 0ul, 3)
            metal_set_buffer(enc, bw, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
            metal_dispatch_threadgroups(enc, uint3(uint(nwin), uint(heads), 1u), uint3(64u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bv = buf_upload(dev, vv)
    return if (!invoke(run, bv))
    t |> equal(buf_mismatch_bars(bo, want, 1e-3, 1e-4, tag), 0)
    vv[80 * qd + hs / 2] += 3.0
    var bv2 = buf_upload(dev, vv)
    if (invoke(run, bv2)) {
        t |> success(buf_mismatch_bars(bo, want, 1e-3, 1e-4, "{tag} poison (MUST mismatch)") > 0,
            "{tag}: a poisoned V element reds the bars")
    }
    metal_release(bq)
    metal_release(bk)
    metal_release(bv)
    metal_release(bv2)
    metal_release(bw)
    metal_release(bo)
    metal_release(pso)
    delete qv
    delete kv
    delete vv
    delete want
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

def private fc_pack_gate(t : T?; dev, queue) {
    let tag = "fc_pack"
    var err : string
    var pso = pipeline_from_source(dev, metal_fc_pack_msl, metal_fc_pack_msl_entry, metal_fc_pack_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let f = fc_fixture(dev)
    let h = 1
    var (qv, kv, vv) <- f16_qkv_rows(f.np * f.d)
    var pl : array<float>
    pl |> resize(f.ww * f.d)
    for (i in range(f.ww * f.d)) {
        pl[i] = float(float16(0.03 * float((i * 11) % 29) - 0.4))
    }
    var bu : array<float>
    var bvv : array<float>
    bu |> resize(f.d)
    bvv |> resize(f.d)
    for (i in range(f.d)) {
        bu[i] = float(float16(0.02 * float(i % 19) - 0.15))
        bvv[i] = float(float16(0.015 * float((i * 3) % 23) - 0.11))
    }
    //! the oracle's five panels: the resize's zeros are the expected pad rows
    var wqu : array<float>
    var wqv : array<float>
    var wkh : array<float>
    var wvt : array<float>
    var wph : array<float>
    wqu |> resize(f.mp * f.hs)
    wqv |> resize(f.mp * f.hs)
    wkh |> resize(f.nkp * f.hs)
    wvt |> resize(f.hs * f.nkp)
    wph |> resize(f.wwpad * f.hs)
    for (r in range(f.np)) {
        for (j in range(f.hs)) {
            let c = h * f.hs + j
            wqu[r * f.hs + j] = qv[r * f.d + c] + bu[c]
            wqv[r * f.hs + j] = qv[r * f.d + c] + bvv[c]
            wkh[r * f.hs + j] = kv[r * f.d + c]
            wvt[j * f.nkp + r] = vv[r * f.d + c]
        }
    }
    for (r in range(f.ww)) {
        for (j in range(f.hs)) {
            wph[r * f.hs + j] = pl[r * f.d + h * f.hs + j]
        }
    }
    var bq = buf_upload(dev, qv)
    var bk = buf_upload(dev, kv)
    var bv = buf_upload(dev, vv)
    var bpl = buf_upload(dev, pl)
    var bbu = buf_upload(dev, bu)
    var bbv = buf_upload(dev, bvv)
    var bqu = buf_fill(dev, f.mp * f.hs, -1.0e30)
    var bqv = buf_fill(dev, f.mp * f.hs, -1.0e30)
    var bkh = buf_fill(dev, f.nkp * f.hs, -1.0e30)
    var bvt = buf_fill(dev, f.hs * f.nkp, -1.0e30)
    var bph = buf_fill(dev, f.wwpad * f.hs, -1.0e30)
    let ka = fc_args(dev, f)
    let total = f.wwpad * f.hs
    let run_once <- @(bqx, bkx, bvx, bplx, bbux, bbvx : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, tag, pso, total) $(enc) {
            metal_set_buffer(enc, bqx, 0ul, 0)
            metal_set_buffer(enc, bkx, 0ul, 1)
            metal_set_buffer(enc, bvx, 0ul, 2)
            metal_set_buffer(enc, bplx, 0ul, 3)
            metal_set_buffer(enc, bbux, 0ul, 4)
            metal_set_buffer(enc, bbvx, 0ul, 5)
            metal_set_buffer(enc, bqu, 0ul, 6)
            metal_set_buffer(enc, bqv, 0ul, 7)
            metal_set_buffer(enc, bkh, 0ul, 8)
            metal_set_buffer(enc, bvt, 0ul, 9)
            metal_set_buffer(enc, bph, 0ul, 10)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 11)
            var u_h = uint(h)
            metal_set_bytes(enc, unsafe(addr<void?>(u_h)), 4ul, 12)
        }
    }
    return if (!invoke(run_once, bq, bk, bv, bpl, bbu, bbv))
    t |> equal(buf_mismatch_bars(bqu, wqu, 0.0, 0.0, "{tag} Q+u"), 0)
    t |> equal(buf_mismatch_bars(bqv, wqv, 0.0, 0.0, "{tag} Q+v"), 0)
    t |> equal(buf_mismatch_bars(bkh, wkh, 0.0, 0.0, "{tag} K_h"), 0)
    t |> equal(buf_mismatch_bars(bvt, wvt, 0.0, 0.0, "{tag} V_hT"), 0)
    t |> equal(buf_mismatch_bars(bph, wph, 0.0, 0.0, "{tag} pos_h"), 0)
    // one poisoned input per panel: each compare above has its own control
    bu[h * f.hs + 5] += 2.0
    var bbu2 = buf_upload(dev, bu)
    if (invoke(run_once, bq, bk, bv, bpl, bbu2, bbv)) {
        t |> success(buf_mismatch_bars(bqu, wqu, 0.0, 0.0, "{tag} bias_u poison (MUST mismatch)") > 0,
            "{tag}: a poisoned content bias reds the Q+u panel")
    }
    bvv[h * f.hs + 9] += 2.0
    var bbv2 = buf_upload(dev, bvv)
    if (invoke(run_once, bq, bk, bv, bpl, bbu, bbv2)) {
        t |> success(buf_mismatch_bars(bqv, wqv, 0.0, 0.0, "{tag} bias_v poison (MUST mismatch)") > 0,
            "{tag}: a poisoned rel bias reds the Q+v panel")
    }
    kv[3 * f.d + h * f.hs + 1] += 2.0
    var bk2 = buf_upload(dev, kv)
    if (invoke(run_once, bq, bk2, bv, bpl, bbu, bbv)) {
        t |> success(buf_mismatch_bars(bkh, wkh, 0.0, 0.0, "{tag} K poison (MUST mismatch)") > 0,
            "{tag}: a poisoned K element reds the K_h panel")
    }
    vv[5 * f.d + h * f.hs + 2] += 2.0
    var bv2 = buf_upload(dev, vv)
    if (invoke(run_once, bq, bk, bv2, bpl, bbu, bbv)) {
        t |> success(buf_mismatch_bars(bvt, wvt, 0.0, 0.0, "{tag} V poison (MUST mismatch)") > 0,
            "{tag}: a poisoned V element reds the V_hT panel")
    }
    pl[11 * f.d + h * f.hs + 3] += 2.0
    var bpl2 = buf_upload(dev, pl)
    if (invoke(run_once, bq, bk, bv, bpl2, bbu, bbv)) {
        t |> success(buf_mismatch_bars(bph, wph, 0.0, 0.0, "{tag} pos poison (MUST mismatch)") > 0,
            "{tag}: a poisoned rel-table element reds the pos_h panel")
    }
    metal_release(bq)
    metal_release(bk)
    metal_release(bv)
    metal_release(bpl)
    metal_release(bbu)
    metal_release(bbu2)
    metal_release(bbv)
    metal_release(bbv2)
    metal_release(bk2)
    metal_release(bv2)
    metal_release(bpl2)
    metal_release(bqu)
    metal_release(bqv)
    metal_release(bkh)
    metal_release(bvt)
    metal_release(bph)
    metal_release(pso)
}

//! np = 24 keeps every live key in simdgroup 0; np = 300 fills every simdgroup and strides each lane twice.
//! Bar: the kernel sums f32 in a lane-then-simdgroup tree (error under 16 ulp of the sum) with the
//! fast-math exp (2 ulp per term), the oracle sums in double - 1e-5 relative is ~10x that budget,
//! 1e-6 absolute covers the probabilities under 0.1 where the relative bar tightens past it.
def private fc_softmax_gate(t : T?; dev, queue; np : int) {
    let tag = "fc_softmax np{np}"
    var err : string
    var pso = pipeline_from_source(dev, metal_fc_softmax_msl, metal_fc_softmax_msl_entry, metal_fc_softmax_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let f = fc_fixture(dev, np)
    var sc : array<float>
    var sr : array<float>
    sc |> resize(f.mp * f.nkp)
    sr |> resize(f.mp * f.wwpad)
    for (i in range(f.mp * f.nkp)) {
        sc[i] = float(float16(0.9 * float((i * 7) % 31) - 12.0))
    }
    for (i in range(f.mp * f.wwpad)) {
        sr[i] = float(float16(0.7 * float((i * 5) % 37) - 11.0))
    }
    let ka = fc_args(dev, f)
    var want : array<float>
    want |> resize(f.mp * f.nkp)   //! the resize's zeros are the expected pad rows and pad columns
    for (r in range(f.np)) {
        let base = f.np - 1 - r
        var mx = -3.0e38
        for (k in range(f.np)) {
            mx = max(mx, (sc[r * f.nkp + k] + sr[r * f.wwpad + base + k]) * ka.scale)
        }
        var tot = 0.0lf
        for (k in range(f.np)) {
            let e = exp((sc[r * f.nkp + k] + sr[r * f.wwpad + base + k]) * ka.scale - mx)
            want[r * f.nkp + k] = e
            tot += double(e)
        }
        for (k in range(f.np)) {
            want[r * f.nkp + k] = float(double(want[r * f.nkp + k]) / tot)
        }
    }
    var bsc = buf_upload(dev, sc)
    var bp = buf_fill(dev, f.mp * f.nkp, -1.0e30)
    let run_once <- @(bsrx : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bsc, 0ul, 0)
            metal_set_buffer(enc, bsrx, 0ul, 1)
            metal_set_buffer(enc, bp, 0ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
            metal_set_threadgroup_memory_length(enc, metal_fc_softmax_msl_tgmem, 0)
            metal_dispatch_threadgroups(enc, uint3(uint(f.mp), 1u, 1u), uint3(256u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bsr = buf_upload(dev, sr)
    return if (!invoke(run_once, bsr))
    t |> equal(buf_mismatch_bars(bp, want, 1e-5, 1e-6, tag), 0)
    sr[3 * f.wwpad + (f.np - 1 - 3) + 7] += 6.0
    var bsr2 = buf_upload(dev, sr)
    if (invoke(run_once, bsr2)) {
        t |> success(buf_mismatch_bars(bp, want, 1e-5, 1e-6, "{tag} rel-score poison (MUST mismatch)") > 0,
            "{tag}: a poisoned rel score reds the bars")
    }
    metal_release(bsc)
    metal_release(bsr)
    metal_release(bsr2)
    metal_release(bp)
    metal_release(pso)
}

def private fc_unpack_gate(t : T?; dev, queue) {
    let tag = "fc_unpack"
    var err : string
    var pso = pipeline_from_source(dev, metal_fc_unpack_msl, metal_fc_unpack_msl_entry, metal_fc_unpack_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let f = fc_fixture(dev)
    var oh : array<float>
    oh |> resize(f.heads * f.mp * f.hs)
    for (i in range(f.heads * f.mp * f.hs)) {
        oh[i] = float(float16(0.05 * float((i * 3) % 41) - 1.0))
    }
    var want : array<float>
    want |> resize(f.mp * f.d)   //! the resize's zeros are the expected pad rows
    for (r in range(f.np)) {
        for (c in range(f.d)) {
            want[r * f.d + c] = oh[((c / f.hs) * f.mp + r) * f.hs + c % f.hs]
        }
    }
    let ka = fc_args(dev, f)
    var bao = buf_fill(dev, f.mp * f.d, -1.0e30)
    let total = f.mp * f.d
    let run_once <- @(bohx : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, tag, pso, total) $(enc) {
            metal_set_buffer(enc, bohx, 0ul, 0)
            metal_set_buffer(enc, bao, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 2)
        }
    }
    var boh = buf_upload(dev, oh)
    return if (!invoke(run_once, boh))
    t |> equal(buf_mismatch_bars(bao, want, 0.0, 0.0, tag), 0)
    oh[(1 * f.mp + 9) * f.hs + 17] += 2.0
    var boh2 = buf_upload(dev, oh)
    if (invoke(run_once, boh2)) {
        t |> success(buf_mismatch_bars(bao, want, 0.0, 0.0, "{tag} poison (MUST mismatch)") > 0,
            "{tag}: a poisoned AV element reds the compare")
    }
    metal_release(boh)
    metal_release(boh2)
    metal_release(bao)
    metal_release(pso)
}

//! Bar: nine f16-exact products summed in f32 (under 9 ulp), the folded BN affine and the
//! fast-math exp of the SiLU (2 ulp) against the oracle's f32 chain in the same order - 1e-5
//! relative is ~50x that budget, 1e-6 absolute covers the outputs near zero.
def private fc_dw_gate(t : T?; dev, queue; tap_major : bool) {
    let tag = tap_major ? "pk_dw" : "cn_dw"
    var err : string
    var pso : MetalComputePipeline?
    if (tap_major) {
        pso = pipeline_from_source(dev, metal_pk_dw_msl, metal_pk_dw_msl_entry, metal_pk_dw_msl_fastmath, err)
    } else {
        pso = pipeline_from_source(dev, metal_cn_dw_msl, metal_cn_dw_msl_entry, metal_cn_dw_msl_fastmath, err)
    }
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let np = 13
    let d = 32
    let nk = 9
    let kpad = (nk - 1) / 2
    var xv : array<float>
    xv |> resize(np * d)
    for (i in range(np * d)) {
        xv[i] = float(float16(0.09 * float((i * 5) % 43) - 1.7))
    }
    var wv : array<float>
    wv |> resize(d * nk)
    for (i in range(d * nk)) {
        wv[i] = float(float16(0.06 * float((i * 7) % 31) - 0.8))
    }
    var bnw : array<float>
    var bnb : array<float>
    bnw |> resize(d)
    bnb |> resize(d)
    for (i in range(d)) {
        bnw[i] = float(float16(0.8 + 0.03 * float(i % 11)))
        bnb[i] = float(float16(0.01 * float((i * 3) % 17) - 0.07))
    }
    var want : array<float>
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
    var bx = buf_upload(dev, xv)
    var bw = buf_upload(dev, wv)
    var bbnw = buf_upload(dev, bnw)
    var bbnb = buf_upload(dev, bnb)
    var by = buf_fill(dev, np * d, -1.0e30)
    let total = np * d
    let run_once <- @(bwx : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, tag, pso, total) $(enc) {
            metal_set_buffer(enc, bx, 0ul, 0)
            metal_set_buffer(enc, by, 0ul, 1)
            metal_set_buffer(enc, bwx, 0ul, 2)
            metal_set_buffer(enc, bbnw, 0ul, 3)
            metal_set_buffer(enc, bbnb, 0ul, 4)
            var u_d = uint(d)
            var u_nk = uint(nk)
            var u_np = uint(np)
            var u_tot = uint(total)
            metal_set_bytes(enc, unsafe(addr<void?>(u_d)), 4ul, 5)
            metal_set_bytes(enc, unsafe(addr<void?>(u_nk)), 4ul, 6)
            metal_set_bytes(enc, unsafe(addr<void?>(u_np)), 4ul, 7)
            metal_set_bytes(enc, unsafe(addr<void?>(u_tot)), 4ul, 8)
        }
    }
    return if (!invoke(run_once, bw))
    t |> equal(buf_mismatch_bars(by, want, 1e-5, 1e-6, tag), 0)
    wv[5 * nk + 3] += 2.0
    var bw2 = buf_upload(dev, wv)
    if (invoke(run_once, bw2)) {
        t |> success(buf_mismatch_bars(by, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0,
            "{tag}: a poisoned tap reds the bars")
    }
    metal_release(bx)
    metal_release(bw)
    metal_release(bw2)
    metal_release(bbnw)
    metal_release(bbnb)
    metal_release(by)
    metal_release(pso)
}

// ===== the gemma4a sliding-window RPE attention (causal window 12, softcap, per-dim scale) =====
def private g4a_attn_gate(t : T?; dev, queue) {
    let tag = "g4a_attn"
    var err : string
    var pso = pipeline_from_source(dev, metal_g4a_attn_msl, metal_g4a_attn_msl_entry, metal_g4a_attn_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let np = 40
    let heads = 2
    let hs = 128
    let d = heads * hs
    let past = 12
    let qscale = (1.0 / sqrt(float(hs))) / log(2.0)
    let kscale = log(1.0 + exp(1.0)) / log(2.0)
    let cap = 50.0
    var (qv, kv, vv) <- f16_qkv_rows(np * d)
    var rpe : array<float>
    rpe |> resize((past + 1) * d)
    for (i in range((past + 1) * d)) {
        rpe[i] = float(float16(0.03 * float((i * 11) % 29) - 0.4))
    }
    var pds : array<float>
    pds |> resize(hs)
    for (i in range(hs)) {
        pds[i] = float(float16(0.5 + 0.02 * float(i % 17)))
    }
    var want : array<float>
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
    var bq = buf_upload(dev, qv)
    var bk = buf_upload(dev, kv)
    var bv = buf_upload(dev, vv)
    var brp = buf_upload(dev, rpe)
    var bpd = buf_upload(dev, pds)
    var bo = buf_fill(dev, np * d, -1.0e30)
    let ka = G4aAttnArgs(d = uint(d), hs = uint(hs), npos = uint(np), past = uint(past),
        qscale = qscale, kscale = kscale, cap = cap)
    let np64 = (np + 63) / 64
    let run <- @(bvx : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bq, 0ul, 0)
            metal_set_buffer(enc, bk, 0ul, 1)
            metal_set_buffer(enc, bvx, 0ul, 2)
            metal_set_buffer(enc, bo, 0ul, 3)
            metal_set_buffer(enc, brp, 0ul, 4)
            metal_set_buffer(enc, bpd, 0ul, 5)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 6)
            metal_dispatch_threadgroups(enc, uint3(uint(np64), uint(heads), 1u), uint3(64u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    return if (!invoke(run, bv))
    t |> equal(buf_mismatch_bars(bo, want, 1e-3, 1e-4, tag), 0)
    vv[18 * d + hs / 2] += 3.0
    var bv2 = buf_upload(dev, vv)
    if (invoke(run, bv2)) {
        t |> success(buf_mismatch_bars(bo, want, 1e-3, 1e-4, "{tag} poison (MUST mismatch)") > 0,
            "{tag}: a poisoned V element reds the bars")
    }
    metal_release(bq)
    metal_release(bk)
    metal_release(bv)
    metal_release(bv2)
    metal_release(brp)
    metal_release(bpd)
    metal_release(bo)
    metal_release(pso)
    delete qv
    delete kv
    delete vv
    delete rpe
    delete pds
    delete want
}

// ===== the StyleTTS2 chain's kernels (the tower driver's TTS seats) =====
//! f16-exact fixture values: every product is exact, so the bars below cover only the f32
//! accumulation order (conv_mm) or the reduction order and sin() rounding (adain_snake)
def private st2_f16(seed : int; scale, bias : float) : float => float(float16(scale * float(seed) + bias))

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

//! the conv GEMM on either stamp: the f16-staged one on f16-exact operands (every product exact,
//! the bar the accumulation order's), the f32-exact one on operands off the f16 lattice at a
//! bar the f16 stamp must exceed on the same fixture (the stamp's control)
def private st2_conv_gate(t : T?; dev, queue; transposed : bool; exact : bool) {
    let tag = (exact ? "st2_conv_exact_mm " : "st2_conv_mm ") + (transposed ? "transposed" : "forward")
    var err : string
    var pso : MetalComputePipeline?
    if (transposed) {
        pso = (exact ? pipeline_from_source(dev, metal_st2_conv_tr_exact_mm_msl, metal_st2_conv_tr_exact_mm_msl_entry, metal_st2_conv_tr_exact_mm_msl_fastmath, err)
            : pipeline_from_source(dev, metal_st2_conv_tr_mm_msl, metal_st2_conv_tr_mm_msl_entry, metal_st2_conv_tr_mm_msl_fastmath, err))
    } else {
        pso = (exact ? pipeline_from_source(dev, metal_st2_conv_exact_mm_msl, metal_st2_conv_exact_mm_msl_entry, metal_st2_conv_exact_mm_msl_fastmath, err)
            : pipeline_from_source(dev, metal_st2_conv_mm_msl, metal_st2_conv_mm_msl_entry, metal_st2_conv_mm_msl_fastmath, err))
    }
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    var pso_half : MetalComputePipeline?
    if (exact) {
        pso_half = (transposed ? pipeline_from_source(dev, metal_st2_conv_tr_mm_msl, metal_st2_conv_tr_mm_msl_entry, metal_st2_conv_tr_mm_msl_fastmath, err)
            : pipeline_from_source(dev, metal_st2_conv_mm_msl, metal_st2_conv_mm_msl_entry, metal_st2_conv_mm_msl_fastmath, err))
        t |> success(pso_half != null, "{tag} f16-staged control pipeline: {err}")
        return if (pso_half == null)
    }
    let tgm_half = transposed ? metal_st2_conv_tr_mm_msl_tgmem : metal_st2_conv_mm_msl_tgmem
    let tgm_exact = transposed ? metal_st2_conv_tr_exact_mm_msl_tgmem : metal_st2_conv_exact_mm_msl_tgmem
    let off16 = exact ? 1.0 / 3.0 : 1.0   //! a factor that takes the operands off the f16 lattice
    let ka = st2_conv_fixture(dev, transposed)
    let cin = int(ka.cin)
    let k = int(ka.k)
    let kdim = (k * cin + 31) / 32 * 32
    let cout = 64
    let t_in = int(ka.t_in)
    let rows = int(ka.rows)
    let mp = (rows + 31) / 32 * 32
    var x : array<float>
    x |> resize(t_in * cin)
    for (i in range(t_in * cin)) {
        x[i] = st2_f16((i * 7) % 41, 0.05, -1.0) * off16
    }
    var w : array<float>
    w |> resize(cout * kdim)   //! the pad columns past k * cin stay zero, as the slab's do
    for (co in range(cout)) {
        for (j in range(k * cin)) {
            w[co * kdim + j] = st2_f16(((co + 1) * (j + 3)) % 37, 0.03, -0.5) * off16
        }
    }
    let rel = exact ? 2e-6 : 1e-3
    let absf = exact ? 2e-6 : 1e-3
    var want : array<float>
    want |> resize(mp * cout)   //! the resize's zeros are the expected pad rows
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
    var bw = buf_upload(dev, w)
    var by = buf_fill(dev, mp * cout, -1.0e30)
    let run_once <- @(bxx : MetalBuffer?; p : MetalComputePipeline?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, p)
            metal_set_buffer(enc, bw, 0ul, 0)
            metal_set_buffer(enc, bxx, 0ul, 1)
            metal_set_buffer(enc, by, 0ul, 2)
            var u_kdim = uint(kdim)
            var u_ndim = uint(cout)
            metal_set_bytes(enc, unsafe(addr<void?>(u_kdim)), 4ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(u_ndim)), 4ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
            metal_set_threadgroup_memory_length(enc, p == pso_half ? tgm_half : (exact ? tgm_exact : tgm_half), 0)
            metal_dispatch_threadgroups(enc, uint3(uint(mp / 32), uint(cout / 64), 1u), uint3(128u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bx = buf_upload(dev, x)
    return if (!invoke(run_once, bx, pso))
    t |> equal(buf_mismatch_bars(by, want, rel, absf, tag), 0)
    if (exact && invoke(run_once, bx, pso_half)) {
        t |> success(buf_mismatch_bars(by, want, rel, absf, "{tag} f16-staged stamp on the same operands (MUST mismatch)") > 0,
            "{tag}: the f16-staged stamp misses the exact bar on operands off the f16 lattice (the stamp's control)")
    }
    x[(t_in / 2) * cin + 5] += 2.0   //! a mid-row element every output row near it reads
    var bx2 = buf_upload(dev, x)
    if (invoke(run_once, bx2, pso)) {
        t |> success(buf_mismatch_bars(by, want, rel, absf, "{tag} x poison (MUST mismatch)") > 0,
            "{tag}: a poisoned input element reds the bars")
    }
    metal_release(bw)
    metal_release(bx)
    metal_release(bx2)
    metal_release(by)
    metal_release(pso)
    if (pso_half != null) {
        metal_release(pso_half)
    }
}

//! the im2col route (a channel count off the 8-run, the noise convs' 22): whole and offset chunks, exact
def private st2_im2col_gate(t : T?; dev, queue; r0, rows : int; transposed : bool) {
    let tag = (transposed ? "st2_im2col transposed r0 " : "st2_im2col r0 ") + "{r0} rows {rows}"
    var err : string
    var pso = (transposed ? pipeline_from_source(dev, metal_st2_im2col_tr_msl, metal_st2_im2col_tr_msl_entry, metal_st2_im2col_tr_msl_fastmath, err)
        : pipeline_from_source(dev, metal_st2_im2col_msl, metal_st2_im2col_msl_entry, metal_st2_im2col_msl_fastmath, err))
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let ka = St2ConvArgs(cin = 22u, k = 12u, stride = 6u, pad_l = 3u, dil = 1u, t_in = 30u, r0 = uint(r0), rows = uint(rows), xs = 22u)
    let cin = 22
    let kdim = 288
    var x : array<float>
    x |> resize(30 * cin)
    for (i in range(30 * cin)) {
        x[i] = st2_f16((i * 5) % 43, 0.07, -1.5)
    }
    var want : array<float>
    want |> resize(32 * kdim)   //! zeros past the taps, the pad columns and the pad rows
    for (r in range(rows)) {
        for (tap in range(12)) {
            let src = st2_conv_src(dev, ka, transposed, r, tap)
            continue if (src < 0)
            for (ci in range(cin)) {
                want[r * kdim + tap * cin + ci] = x[src * cin + ci]
            }
        }
    }
    var bcols = buf_fill(dev, 32 * kdim, -1.0e30)
    let run_once <- @(bxx : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, tag, pso, 32 * kdim) $(enc) {
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, bcols, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 2)
        }
    }
    var bx = buf_upload(dev, x)
    return if (!invoke(run_once, bx))
    t |> equal(buf_mismatch_bars(bcols, want, 0.0, 0.0, tag), 0)
    var hit = -1
    for (tap in range(12)) {   //! an input row the first chunk row's taps reach, on either geometry
        hit = st2_conv_src(dev, ka, transposed, 0, tap)
        break if (hit >= 0)
    }
    t |> success(hit >= 0, "{tag}: the first row reads some input row")
    x[hit * cin + 3] += 2.0
    var bx2 = buf_upload(dev, x)
    if (invoke(run_once, bx2)) {
        t |> success(buf_mismatch_bars(bcols, want, 0.0, 0.0, "{tag} x poison (MUST mismatch)") > 0,
            "{tag}: a poisoned input element reds the compare")
    }
    metal_release(bx)
    metal_release(bx2)
    metal_release(bcols)
    metal_release(pso)
}

//! column stats + fused AdaIN against the CPU formula at 24 channels: one stride with a tail, two strides plus a tail
def private st2_adain_gate(t : T?; dev, queue; nrows : int; snake : bool) {
    let tag = snake ? "st2_adain_snake t{nrows}" : "st2_adain_leaky t{nrows}"
    var err : string
    var pso_st = pipeline_from_source(dev, metal_st2_colstats_msl, metal_st2_colstats_msl_entry, metal_st2_colstats_msl_fastmath, err)
    t |> success(pso_st != null, "{tag} colstats pipeline: {err}")
    var pso : MetalComputePipeline?
    if (snake) {
        pso = pipeline_from_source(dev, metal_st2_adain_snake_msl, metal_st2_adain_snake_msl_entry, metal_st2_adain_snake_msl_fastmath, err)
    } else {
        pso = pipeline_from_source(dev, metal_st2_adain_leaky_msl, metal_st2_adain_leaky_msl_entry, metal_st2_adain_leaky_msl_fastmath, err)
    }
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null || pso_st == null)
    let c = 24
    let n = nrows * c
    var x : array<float>
    x |> resize(n)
    for (i in range(n)) {
        x[i] = st2_f16((i * 11) % 53, 0.04, -1.1)
    }
    var h : array<float>
    var gw : array<float>
    var gb : array<float>
    var alpha : array<float>
    h |> resize(2 * c)
    gw |> resize(c)
    gb |> resize(c)
    alpha |> resize(c)
    for (i in range(c)) {
        h[i] = st2_f16((i * 3) % 17, 0.02, -0.1)
        h[c + i] = st2_f16((i * 5) % 19, 0.03, -0.2)
        gw[i] = st2_f16((i * 7) % 13, 0.05, 0.7)
        gb[i] = st2_f16((i * 2) % 11, 0.01, -0.05)
        alpha[i] = st2_f16((i * 3) % 7, 0.1, 0.3)
    }
    let ka = St2AdainArgs(c = uint(c), t = uint(nrows), eps = 1e-5)
    var want_st : array<float>
    var want : array<float>
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
    var bst = buf_fill(dev, 2 * c, -1.0e30)
    var by = buf_fill(dev, n, -1.0e30)
    var bh = buf_upload(dev, h)
    var bgw = buf_upload(dev, gw)
    var bgb = buf_upload(dev, gb)
    var balpha = buf_upload(dev, alpha)
    let run_once <- @(bxx : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso_st)
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, bst, 0ul, 1)
            var u_t = uint(nrows)
            var u_c = uint(c)
            metal_set_bytes(enc, unsafe(addr<void?>(u_t)), 4ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(u_c)), 4ul, 3)
            metal_set_threadgroup_memory_length(enc, metal_st2_colstats_msl_tgmem, 0)
            metal_dispatch_threadgroups(enc, uint3(uint(c), 1u, 1u), uint3(256u, 1u, 1u))
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, by, 0ul, 1)
            metal_set_buffer(enc, bst, 0ul, 2)
            metal_set_buffer(enc, bh, 0ul, 3)
            metal_set_buffer(enc, bgw, 0ul, 4)
            metal_set_buffer(enc, bgb, 0ul, 5)
            metal_set_buffer(enc, balpha, 0ul, 6)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 7)
            metal_dispatch_threadgroups(enc, uint3(uint((n + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bx = buf_upload(dev, x)
    return if (!invoke(run_once, bx))
    t |> equal(buf_mismatch_bars(bst, want_st, 1e-4, 1e-5, "{tag} stats"), 0)
    t |> equal(buf_mismatch_bars(by, want, 1e-4, 1e-5, tag), 0)
    x[3 * c + 5] += 2.0
    var bx2 = buf_upload(dev, x)
    if (invoke(run_once, bx2)) {
        t |> success(buf_mismatch_bars(bst, want_st, 1e-4, 1e-5, "{tag} stats x poison (MUST mismatch)") > 0,
            "{tag}: a poisoned input element reds the stats")
        t |> success(buf_mismatch_bars(by, want, 1e-4, 1e-5, "{tag} x poison (MUST mismatch)") > 0,
            "{tag}: a poisoned input element reds the rows")
    }
    metal_release(bx)
    metal_release(bx2)
    metal_release(bst)
    metal_release(by)
    metal_release(bh)
    metal_release(bgw)
    metal_release(bgb)
    metal_release(balpha)
    metal_release(pso)
    metal_release(pso_st)
}

//! leaky (in place), axpy on both zero regimes, add, reflect - every total off the 64-lane grid, so the tail threadgroup runs
def private st2_rows_gate(t : T?; dev, queue) {
    let tag = "st2 rows"
    var err : string
    var pso_lk = pipeline_from_source(dev, metal_st2_leaky_msl, metal_st2_leaky_msl_entry, metal_st2_leaky_msl_fastmath, err)
    t |> success(pso_lk != null, "{tag} leaky pipeline: {err}")
    var pso_ax = pipeline_from_source(dev, metal_st2_axpy_msl, metal_st2_axpy_msl_entry, metal_st2_axpy_msl_fastmath, err)
    t |> success(pso_ax != null, "{tag} axpy pipeline: {err}")
    var pso_add = pipeline_from_source(dev, MetalSt2Add_metal_ew2_msl, MetalSt2Add_metal_ew2_msl_entry, MetalSt2Add_metal_ew2_msl_fastmath, err)
    t |> success(pso_add != null, "{tag} add pipeline: {err}")
    var pso_rf = pipeline_from_source(dev, metal_st2_reflect1_msl, metal_st2_reflect1_msl_entry, metal_st2_reflect1_msl_fastmath, err)
    t |> success(pso_rf != null, "{tag} reflect1 pipeline: {err}")
    return if (pso_lk == null || pso_ax == null || pso_add == null || pso_rf == null)
    let c = 24
    let nrows = 9
    let n = nrows * c   //! 216: three threadgroups and a 24-lane tail
    var a : array<float>
    var b : array<float>
    a |> resize(n)
    b |> resize(n)
    for (i in range(n)) {
        a[i] = st2_f16((i * 7) % 31, 0.1, -1.5)
        b[i] = st2_f16((i * 5) % 29, 0.1, -1.4)
    }
    var want_lk : array<float>
    var want_ax0 : array<float>
    var want_ax1 : array<float>
    var want_add : array<float>
    var want_rf : array<float>
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
    var bo = buf_fill(dev, n + c, -1.0e30)
    let u_slope = 0.1
    let u_scale = 0.25
    let u_n = uint(n)
    let u_c = uint(c)
    let u_nrf = uint(n + c)
    let run_leaky <- @(bxx : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, "{tag} leaky", pso_lk, n) $(enc) {
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_bytes(enc, unsafe(addr<void?>(u_n)), 4ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(u_slope)), 4ul, 2)
        }
    }
    let run_axpy <- @(bax, bbx : MetalBuffer?; zero : uint) : bool {
        var u_zero = zero
        return dispatch_64_lanes(t, queue, "{tag} axpy", pso_ax, n) $(enc) {
            metal_set_buffer(enc, bax, 0ul, 0)
            metal_set_buffer(enc, bbx, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(u_scale)), 4ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(u_zero)), 4ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(u_n)), 4ul, 4)
        }
    }
    let run_add <- @(bax, bbx : MetalBuffer?; scale : float) : bool {
        var u_sc = scale
        return dispatch_64_lanes(t, queue, "{tag} add", pso_add, n) $(enc) {
            metal_set_buffer(enc, bax, 0ul, 0)
            metal_set_buffer(enc, bbx, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(u_n)), 4ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(u_sc)), 4ul, 3)
            metal_set_buffer(enc, bo, 0ul, 7)
        }
    }
    let run_reflect <- @(bax : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, "{tag} reflect1", pso_rf, n + c) $(enc) {
            metal_set_buffer(enc, bax, 0ul, 0)
            metal_set_buffer(enc, bo, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(u_c)), 4ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(u_nrf)), 4ul, 3)
        }
    }
    var bx = buf_upload(dev, a)
    if (invoke(run_leaky, bx)) {
        t |> equal(buf_mismatch_bars(bx, want_lk, 0.0, 0.0, "{tag} leaky"), 0)
        t |> success(neg_at >= 0 && want_lk[neg_at] != a[neg_at], "{tag} leaky: the output differs from the input at [{neg_at}]")
        a[neg_at] += 2.0
        var bx2 = buf_upload(dev, a)
        if (invoke(run_leaky, bx2)) {
            t |> success(buf_mismatch_bars(bx2, want_lk, 0.0, 0.0, "{tag} leaky poison (MUST mismatch)") > 0, "{tag} leaky: a poisoned element reds the compare")
        }
        a[neg_at] -= 2.0
        metal_release(bx2)
    }
    metal_release(bx)
    var ba = buf_upload(dev, a)
    var bb = buf_upload(dev, b)
    if (invoke(run_axpy, ba, bb, 0u)) {
        t |> equal(buf_mismatch_bars(ba, want_ax0, 1e-6, 1e-6, "{tag} axpy accumulate"), 0)
    }
    buf_store(ba, a)
    if (invoke(run_axpy, ba, bb, 1u)) {
        t |> equal(buf_mismatch_bars(ba, want_ax1, 1e-6, 1e-6, "{tag} axpy zero"), 0)
    }
    b[7] += 2.0
    var bb2 = buf_upload(dev, b)
    buf_store(ba, a)
    if (invoke(run_axpy, ba, bb2, 1u)) {
        t |> success(buf_mismatch_bars(ba, want_ax1, 1e-6, 1e-6, "{tag} axpy poison (MUST mismatch)") > 0, "{tag} axpy: a poisoned y element reds the compare")
    }
    buf_store(ba, a)
    if (invoke(run_axpy, ba, bb2, 0u)) {
        t |> success(buf_mismatch_bars(ba, want_ax0, 1e-6, 1e-6, "{tag} axpy accumulate poison (MUST mismatch)") > 0, "{tag} axpy accumulate: a poisoned y element reds the compare")
    }
    b[7] -= 2.0
    buf_store(ba, a)
    if (invoke(run_add, ba, bb, 1.0)) {
        t |> equal(buf_mismatch_bars(bo, want_add, 0.0, 0.0, "{tag} add"), 0)
    }
    if (invoke(run_add, ba, bb2, 1.0)) {
        t |> success(buf_mismatch_bars(bo, want_add, 0.0, 0.0, "{tag} add poison (MUST mismatch)") > 0, "{tag} add: a poisoned b element reds the compare")
    }
    var want_join : array<float>
    want_join |> resize(n)
    for (i in range(n)) {
        want_join[i] = (a[i] + b[i]) * 0.70710678
    }
    if (invoke(run_add, ba, bb, 0.70710678)) {
        t |> equal(buf_mismatch_bars(bo, want_join, 1e-6, 1e-7, "{tag} add scaled (the residual join)"), 0)
    }
    if (invoke(run_add, ba, bb2, 0.70710678)) {
        t |> success(buf_mismatch_bars(bo, want_join, 1e-6, 1e-7, "{tag} add scaled poison (MUST mismatch)") > 0, "{tag} add scaled: a poisoned b element reds the compare")
    }
    var sentinel : array<float>
    sentinel |> resize(n + c)
    for (i in range(n + c)) {
        sentinel[i] = -1.0e30
    }
    buf_store(bo, sentinel)
    if (invoke(run_reflect, ba)) {
        t |> equal(buf_mismatch_bars(bo, want_rf, 0.0, 0.0, "{tag} reflect1"), 0)
    }
    a[c + 2] += 2.0   //! row 1: the reflected row 0 and row 2 both read it
    var ba2 = buf_upload(dev, a)
    if (invoke(run_reflect, ba2)) {
        t |> success(buf_mismatch_bars(bo, want_rf, 0.0, 0.0, "{tag} reflect1 poison (MUST mismatch)") > 0, "{tag} reflect1: a poisoned source row reds the compare")
    }
    metal_release(ba)
    metal_release(ba2)
    metal_release(bb)
    metal_release(bb2)
    metal_release(bo)
    metal_release(pso_lk)
    metal_release(pso_ax)
    metal_release(pso_add)
    metal_release(pso_rf)
}

//! `n` f16-exact patterned values: seed (i * seed_mul) % 47 through `st2_f16`
def private st2_fill(n : int; seed_mul : int; scale, bias : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = st2_f16((i * seed_mul) % 47, scale, bias)
    }
    return <- a
}

//! the row gather with its add flags off and on, exact, a poisoned source row as the control
def private st2_gather_gate(t : T?; dev, queue) {
    let tag = "st2_gather"
    var err : string
    var pso = pipeline_from_source(dev, metal_row_gather_msl, metal_row_gather_msl_entry, metal_row_gather_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let c = 24
    let nrows = 9
    var a <- st2_fill(16 * c, 7, 0.05, -1.0)
    let b <- st2_fill(nrows * c, 5, 0.03, 0.2)
    let cv <- st2_fill(c, 3, 0.02, -0.1)
    var idx : array<uint>
    idx |> resize(nrows)
    for (r in range(nrows)) {
        idx[r] = uint((r * 5 + 3) % 16)
    }
    var ba = buf_upload(dev, a)
    var bb = buf_upload(dev, b)
    var bcv = buf_upload(dev, cv)
    var bidx = buf_upload(dev, idx)
    var bo = buf_fill(dev, nrows * c, -1.0e30)
    let run <- @(bax : MetalBuffer?; add_b, add_c : uint) : bool {
        var ka = RowGatherArgs(c = uint(c), add_b = add_b, add_c = add_c, total = uint(nrows * c))
        return dispatch_64_lanes(t, queue, "{tag} add_b {add_b} add_c {add_c}", pso, nrows * c) $(enc) {
            metal_set_buffer(enc, bax, 0ul, 0)
            metal_set_buffer(enc, bidx, 0ul, 1)
            metal_set_buffer(enc, bb, 0ul, 2)
            metal_set_buffer(enc, bcv, 0ul, 3)
            metal_set_buffer(enc, bo, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        }
    }
    var want : array<float>
    want |> resize(nrows * c)
    for (r in range(nrows)) {
        for (j in range(c)) {
            want[r * c + j] = a[int(idx[r]) * c + j]
        }
    }
    if (invoke(run, ba, 0u, 0u)) {
        t |> equal(buf_mismatch_bars(bo, want, 0.0, 0.0, "{tag} plain"), 0)
    }
    for (r in range(nrows)) {
        for (j in range(c)) {
            want[r * c + j] += b[r * c + j] + cv[j]
        }
    }
    if (invoke(run, ba, 1u, 1u)) {
        t |> equal(buf_mismatch_bars(bo, want, 0.0, 0.0, "{tag} with the row and column adds"), 0)
    }
    a[int(idx[4]) * c + 2] += 2.0
    var ba2 = buf_upload(dev, a)
    if (invoke(run, ba2, 1u, 1u)) {
        t |> success(buf_mismatch_bars(bo, want, 0.0, 0.0, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned source row reds the compare")
    }
    metal_release(ba)
    metal_release(ba2)
    metal_release(bb)
    metal_release(bcv)
    metal_release(bidx)
    metal_release(bo)
    metal_release(pso)
}

//! the concat rows with and without the residual columns, strided f0 / n columns, exact
def private st2_concat_gate(t : T?; dev, queue; with_res : bool) {
    let tag = with_res ? "st2_concat with res" : "st2_concat no res"
    var err : string
    var pso = pipeline_from_source(dev, metal_st2_concat_msl, metal_st2_concat_msl_entry, metal_st2_concat_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let c = 24
    let rc = 16
    let rs = 64
    let nrows = 7
    let f0s = 4
    let width = with_res ? 48 : 32
    let x <- st2_fill(nrows * c, 7, 0.05, -1.0)
    var res <- st2_fill(nrows * rs, 5, 0.03, 0.2)
    let f0 <- st2_fill(nrows * f0s, 3, 1.5, 80.0)
    let nd <- st2_fill(nrows * f0s, 11, 0.1, -3.0)
    var want : array<float>
    want |> resize(nrows * width)
    let rcw = with_res ? rc : 0
    for (r in range(nrows)) {
        for (j in range(c)) {
            want[r * width + j] = x[r * c + j]
        }
        for (j in range(rcw)) {
            want[r * width + c + j] = res[r * rs + j]
        }
        want[r * width + c + rcw] = f0[r * f0s]
        want[r * width + c + rcw + 1] = nd[r * f0s]
    }
    var bx = buf_upload(dev, x)
    var bf0 = buf_upload(dev, f0)
    var bnd = buf_upload(dev, nd)
    var bo = buf_fill(dev, nrows * width, -1.0e30)
    let run <- @(bres : MetalBuffer?) : bool {
        var ka = St2ConcatArgs(c = uint(c), rc = uint(rc), rs = uint(rs), f0s = uint(f0s), ns = uint(f0s), with_res = with_res ? 1u : 0u,
            width = uint(width), total = uint(nrows * width))
        return dispatch_64_lanes(t, queue, tag, pso, nrows * width) $(enc) {
            metal_set_buffer(enc, bx, 0ul, 0)
            metal_set_buffer(enc, bres, 0ul, 1)
            metal_set_buffer(enc, bf0, 0ul, 2)
            metal_set_buffer(enc, bnd, 0ul, 3)
            metal_set_buffer(enc, bo, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        }
    }
    var bres = buf_upload(dev, res)
    if (invoke(run, bres)) {
        t |> equal(buf_mismatch_bars(bo, want, 0.0, 0.0, tag), 0)
    }
    if (with_res) {
        res[3 * rs + 5] += 2.0
    } else {
        want[3 * width + c] += 2.0   //! the expectation moves off the true f0 column: the compare must red
    }
    var bres2 = buf_upload(dev, res)
    if (invoke(run, bres2)) {
        t |> success(buf_mismatch_bars(bo, want, 0.0, 0.0, "{tag} poison (MUST mismatch)") > 0, "{tag}: the poison reds the compare")
    }
    metal_release(bx)
    metal_release(bres)
    metal_release(bres2)
    metal_release(bf0)
    metal_release(bnd)
    metal_release(bo)
    metal_release(pso)
}

//! the depthwise transposed pool at stride 2 against the CPU gather, taps off both ends
def private st2_pool_gate(t : T?; dev, queue) {
    let tag = "st2_pool_dw"
    var err : string
    var pso = pipeline_from_source(dev, metal_st2_pool_dw_msl, metal_st2_pool_dw_msl_entry, metal_st2_pool_dw_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let c = 24
    let k = 3
    let stride = 2
    let pad_l = 1
    let t_in = 9
    let t_out = (t_in - 1) * stride - 2 * pad_l + (k - 1) + 1
    var x <- st2_fill(t_in * c, 7, 0.05, -1.0)
    let w <- st2_fill(c * k, 5, 0.1, -0.15)
    let b <- st2_fill(c, 3, 0.02, 0.05)
    var want : array<float>
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
    var bw = buf_upload(dev, w)
    var bb = buf_upload(dev, b)
    var by = buf_fill(dev, t_out * c, -1.0e30)
    let run <- @(bxx : MetalBuffer?) : bool {
        var ka = St2PoolArgs(c = uint(c), k = uint(k), stride = uint(stride), pad_l = uint(pad_l), dil = 1u, t_in = uint(t_in), total = uint(t_out * c))
        return dispatch_64_lanes(t, queue, tag, pso, t_out * c) $(enc) {
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, by, 0ul, 1)
            metal_set_buffer(enc, bw, 0ul, 2)
            metal_set_buffer(enc, bb, 0ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        }
    }
    var bx = buf_upload(dev, x)
    if (invoke(run, bx)) {
        t |> equal(buf_mismatch_bars(by, want, 1e-5, 1e-6, tag), 0)
    }
    x[4 * c + 6] += 2.0
    var bx2 = buf_upload(dev, x)
    if (invoke(run, bx2)) {
        t |> success(buf_mismatch_bars(by, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned input row reds the compare")
    }
    metal_release(bx)
    metal_release(bx2)
    metal_release(bw)
    metal_release(bb)
    metal_release(by)
    metal_release(pso)
}

def private st2_sigmoid(v : float) : float => 1.0 / (1.0 + exp(-v))


//! both LSTM directions on one input-gate set against a CPU recurrence: 8 units (32 lanes), 13 steps, a padded gate stride
def private st2_lstm_gate(t : T?; dev, queue) {
    let tag = "st2_lstm"
    var err : string
    var pso = pipeline_from_source(dev, metal_st2_lstm_msl, metal_st2_lstm_msl_entry, metal_st2_lstm_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let hd = 8
    let steps = 13
    let xgs = 64
    var xg <- st2_fill(steps * xgs, 7, 0.05, -1.15)   //! centred, so no gate saturates and the poison stays visible
    let whh <- st2_fill(4 * hd * hd, 5, 0.012, -0.28)
    var want : array<float>
    want |> resize(steps * 2 * hd)
    for (dir in range(2)) {
        var h : array<float>
        var cs : array<float>
        h |> resize(hd)
        cs |> resize(hd)
        for (step in range(steps)) {
            let ti = dir == 1 ? steps - 1 - step : step
            var hn : array<float>
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
                cs[j] = st2_sigmoid(g[1]) * cs[j] + st2_sigmoid(g[0]) * st2_tanh(g[2])
                hn[j] = st2_sigmoid(g[3]) * st2_tanh(cs[j])
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
    var bw = buf_upload(dev, whh)
    var by = buf_fill(dev, steps * 2 * hd, -1.0e30)
    let run <- @(bxgx : MetalBuffer?; dir : int) : bool {
        var err2 : string
        var ka = St2LstmArgs(t = uint(steps), hidden = uint(hd), xgs = uint(xgs), ystride = uint(2 * hd), yoff = uint(dir * hd), reverse = uint(dir))
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bxgx, 0ul, 0)
            metal_set_buffer(enc, by, 0ul, 1)
            metal_set_buffer(enc, bw, 0ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
            metal_set_threadgroup_memory_length(enc, metal_st2_lstm_msl_tgmem, 0)
            metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(uint(4 * hd), 1u, 1u))
        }
        t |> success(ran, "{tag} dir {dir} dispatch: {err2}")
        return ran
    }
    var bxg = buf_upload(dev, xg)
    if (invoke(run, bxg, 0) && invoke(run, bxg, 1)) {
        t |> equal(buf_mismatch_bars(by, want, 1e-4, 1e-5, tag), 0)
    }
    xg[6 * xgs + 2 * hd + 3] += 2.0
    var bxg2 = buf_upload(dev, xg)
    if (invoke(run, bxg2, 0) && invoke(run, bxg2, 1)) {
        t |> success(buf_mismatch_bars(by, want, 1e-4, 1e-5, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned input gate reds the compare")
    }
    metal_release(bxg)
    metal_release(bxg2)
    metal_release(bw)
    metal_release(by)
    metal_release(pso)
}

//! the ALBERT attention rows against the CPU `attention_rows`: two heads of 64, rows off (37) and past (100) the 32-lane split
def private st2_attn_gate(t : T?; dev, queue; nrows : int) {
    let tag = "st2_attn t{nrows}"
    var err : string
    var pso = pipeline_from_source(dev, metal_st2_attn_msl, metal_st2_attn_msl_entry, metal_st2_attn_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let heads = 2
    let hs = 64
    let d = heads * hs
    var q <- st2_fill(nrows * d, 7, 0.02, -0.4)
    var k <- st2_fill(nrows * d, 5, 0.03, -0.6)
    var vv <- st2_fill(nrows * d, 3, 0.05, -1.1)
    var want : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        attention_rows(q, k, vv, int64(nrows), int64(d), int64(heads), want)
    }
    var bq = buf_upload(dev, q)
    var bv = buf_upload(dev, vv)
    var bo = buf_fill(dev, nrows * d, -1.0e30)
    let run <- @(bkk : MetalBuffer?) : bool {
        var err2 : string
        var ka = St2AttnArgs(t = uint(nrows), d = uint(d), hs = uint(hs), heads = uint(heads))
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bq, 0ul, 0)
            metal_set_buffer(enc, bkk, 0ul, 1)
            metal_set_buffer(enc, bv, 0ul, 2)
            metal_set_buffer(enc, bo, 0ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
            metal_dispatch_threadgroups(enc, uint3(uint((heads * nrows + 1) / 2), 1u, 1u), uint3(64u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bk = buf_upload(dev, k)
    if (invoke(run, bk)) {
        t |> equal(buf_mismatch_bars(bo, want, 1e-5, 1e-6, tag), 0)
    }
    k[(nrows / 2) * d + 70] += 3.0   //! a key of the second head: every query row's softmax moves
    var bk2 = buf_upload(dev, k)
    if (invoke(run, bk2)) {
        t |> success(buf_mismatch_bars(bo, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned key reds the compare")
    }
    metal_release(bq)
    metal_release(bk)
    metal_release(bk2)
    metal_release(bv)
    metal_release(bo)
    metal_release(pso)
}


//! 10 rows of 40 from column 8, rows 2.., of a [.][48] source into a [.][64] target at row 3, column 5; the target's
//! other cells keep their sentinel - the copy touches its rows and nothing else
def private pk_rows_gate(t : T?; dev, queue; elu : bool) {
    let tag = elu ? "pk_rows_elu" : "pk_rows"
    var err : string
    var pso = (elu ? pipeline_from_source(dev, metal_pk_rows_elu_msl, metal_pk_rows_elu_msl_entry, metal_pk_rows_elu_msl_fastmath, err)
        : pipeline_from_source(dev, metal_pk_rows_msl, metal_pk_rows_msl_entry, metal_pk_rows_msl_fastmath, err))
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let rows = 10
    let width = 40
    let ka = PkRowsArgs(rows = uint(rows), width = uint(width), ss = 48u, sc = 8u, sr0 = 2u, ds = 64u, dc = 5u, dr0 = 3u)
    var x <- st2_fill((2 + rows) * 48, 3, 0.5, -1.0)
    var want : array<float>
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
        return dispatch_64_lanes(t, queue, tag, pso, rows * width) $(enc) {
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, byy, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 2)
        }
    }
    var bx = buf_upload(dev, x)
    var by = buf_fill(dev, length(want), -7.0)
    if (invoke(run, bx, by)) {
        t |> equal(buf_mismatch_bars(by, want, 1e-6, 1e-7, tag), 0)
        t |> success(want[(3 + 1) * 64 + 5 + 4] != -7.0, "{tag}: the copied cell left the sentinel")
    }
    x[(2 + 1) * 48 + 8 + 4] += 1.5
    var bx2 = buf_upload(dev, x)
    var by2 = buf_fill(dev, length(want), -7.0)
    if (invoke(run, bx2, by2)) {
        t |> success(buf_mismatch_bars(by2, want, 1e-6, 1e-7, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned source cell reds the compare")
    }
    metal_release(bx)
    metal_release(by)
    metal_release(bx2)
    metal_release(by2)
    metal_release(pso)
}

//! x [9][48] *= s[48], the scale read 16 floats into its buffer (the slab offset)
def private pk_row_scale_gate(t : T?; dev, queue) {
    let tag = "pk_row_scale"
    var err : string
    var pso = pipeline_from_source(dev, metal_pk_row_scale_msl, metal_pk_row_scale_msl_entry, metal_pk_row_scale_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let tt = 9
    let c = 48
    var x <- st2_fill(tt * c, 5, 0.3, -0.7)
    let s <- st2_fill(16 + c, 11, 0.1, 0.5)
    var want : array<float>
    want |> resize(tt * c)
    for (i in range(tt * c)) {
        want[i] = x[i] * s[16 + i % c]
    }
    var bs = buf_upload(dev, s)
    let run <- @(bxx : MetalBuffer?) : bool {
        var u_c = uint(c)
        var u_n = uint(tt * c)
        return dispatch_64_lanes(t, queue, tag, pso, tt * c) $(enc) {
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, bs, 64ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(u_c)), 4ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(u_n)), 4ul, 3)
        }
    }
    var bx = buf_upload(dev, x)
    if (invoke(run, bx)) {
        t |> equal(buf_mismatch_bars(bx, want, 1e-6, 1e-7, tag), 0)
    }
    x[c + 7] += 1.5
    var bx2 = buf_upload(dev, x)
    if (invoke(run, bx2)) {
        t |> success(buf_mismatch_bars(bx2, want, 1e-6, 1e-7, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned row reds the compare")
    }
    metal_release(bs)
    metal_release(bx)
    metal_release(bx2)
    metal_release(pso)
}

//! rows [7][96] rotated over 64 columns from column 16 (two heads of 32) at positions 5..: the CPU's `rope_rows` on the same columns
//! The codec's rope: MetalRope over rows at a stride from a column, the tables bound at a position's row
def private rope_stride_gate(t : T?; dev, queue) {
    let tag = "rope stride"
    var err : string
    var pso = pipeline_from_source(dev, metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let tt = 7
    let stride = 96
    let col = 16
    let d = 64
    let dh = 32
    let pos0 = 5
    let period = 10000.0
    var x <- st2_fill(tt * stride, 13, 0.4, -0.9)
    var y : array<float>
    y |> resize(tt * d)
    for (r in range(tt)) {
        for (j in range(d)) {
            y[r * d + j] = x[r * stride + col + j]
        }
    }
    rope_rows(y, int64(tt), int64(d), int64(dh), int64(pos0), period)
    var want := x
    for (r in range(tt)) {
        for (j in range(d)) {
            want[r * stride + col + j] = y[r * d + j]
        }
    }
    var none : array<float>
    var tcos : array<float>
    var tsin : array<float>
    build_rope_tabs(tcos, tsin, period, 1.0, 1.0, none, false, 0l, int64(pos0 + tt), int64(dh))
    var bcos = buf_upload(dev, tcos)
    var bsin = buf_upload(dev, tsin)
    var u_n = buf_u32(dev, uint(d))
    var u_hs = buf_u32(dev, uint(dh))
    var u_np = buf_u32(dev, uint(tt * d / 2))
    var u_neox = buf_u32(dev, 0u)
    var u_stride = buf_u32(dev, uint(stride))
    let toff = uint64(pos0 * (dh / 2) * 4)
    let run <- @(bxx : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, tag, pso, tt * d / 2) $(enc) {
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
    var bx = buf_upload(dev, x)
    if (invoke(run, bx)) {
        t |> equal(buf_mismatch_bars(bx, want, 1e-6, 1e-6, tag), 0)
        t |> success(want[2 * stride + col + 3] != x[2 * stride + col + 3], "{tag}: a rotated column moved")
        t |> success(want[2 * stride + 3] == x[2 * stride + 3], "{tag}: a column outside the span stayed")
    }
    x[3 * stride + col + 10] += 0.5
    var bx2 = buf_upload(dev, x)
    if (invoke(run, bx2)) {
        t |> success(buf_mismatch_bars(bx2, want, 1e-6, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned element reds the compare")
    }
    metal_release(bcos)
    metal_release(bsin)
    metal_release(u_n)
    metal_release(u_hs)
    metal_release(u_np)
    metal_release(u_neox)
    metal_release(u_stride)
    metal_release(bx)
    metal_release(bx2)
    metal_release(pso)
}

//! 12 new query rows at positions 10.. over a 22-key cache, two heads of 64, the last `ctx` keys
//! each (0 = every key): `attention_causal_rows` over a `TtsKvCache` of the same keys is the oracle
def private pk_attn_gate(t : T?; dev, queue; ctx, n_old : int) {
    let tag = "pk_attn ctx{ctx} old{n_old}"
    var err : string
    var pso = pipeline_from_source(dev, metal_pk_attn_msl, metal_pk_attn_msl_entry, metal_pk_attn_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let heads = 2
    let hs = PK_ATTN_HS
    let d = heads * hs
    let tt = 12
    let n = n_old + tt
    let qs = 160
    let qcol = 16
    let q <- st2_fill(tt * qs, 7, 0.05, -0.6)
    var k <- st2_fill(n * d, 5, 0.06, -0.7)
    var vv <- st2_fill(n * d, 3, 0.08, -1.1)
    var qn : array<float>
    qn |> resize(tt * d)
    for (r in range(tt)) {
        for (j in range(d)) {
            qn[r * d + j] = q[r * qs + qcol + j]
        }
    }
    var kc = TtsKvCache()
    kv_cache_init(kc, int64(heads), int64(hs), int64(n))
    kv_cache_append(kc, k, vv, int64(n), int64(d))
    var want : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        attention_causal_rows(qn, int64(tt), int64(d), int64(ctx), kc, want)
    }
    let ka = PkAttnArgs(t = uint(tt), qs = uint(qs), qcol = uint(qcol), d = uint(d), pos0 = uint(n_old), ctx = uint(ctx))
    var bq = buf_upload(dev, q)
    var bv = buf_upload(dev, vv)
    let run <- @(bkk : MetalBuffer?; bo : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bq, 0ul, 0)
            metal_set_buffer(enc, bkk, 0ul, 1)
            metal_set_buffer(enc, bv, 0ul, 2)
            metal_set_buffer(enc, bo, 0ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
            metal_set_threadgroup_memory_length(enc, metal_pk_attn_msl_tgmem, 0)
            metal_dispatch_threadgroups(enc, uint3(uint(heads * tt), 1u, 1u), uint3(256u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bk = buf_upload(dev, k)
    var bo = buf_fill(dev, tt * d, -1.0e30)
    if (invoke(run, bk, bo)) {
        t |> equal(buf_mismatch_bars(bo, want, 1e-5, 1e-6, tag), 0)
    }
    if (ctx > 0 && ctx < n_old) {
        k[0 * d + 3] += 3.0   //! the first key: outside every query's window, so nothing moves
        var bk1 = buf_upload(dev, k)
        var bo1 = buf_fill(dev, tt * d, -1.0e30)
        if (invoke(run, bk1, bo1)) {
            t |> equal(buf_mismatch_bars(bo1, want, 1e-5, 1e-6, "{tag} unseen key"), 0)
        }
        metal_release(bk1)
        metal_release(bo1)
    }
    k[(n - 1) * d + 70] += 3.0   //! the newest key, in the last query's window: its row moves
    var bk2 = buf_upload(dev, k)
    var bo2 = buf_fill(dev, tt * d, -1.0e30)
    if (invoke(run, bk2, bo2)) {
        t |> success(buf_mismatch_bars(bo2, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned key in the window reds the compare")
    }
    metal_release(bo2)
    delete kc
    metal_release(bq)
    metal_release(bk)
    metal_release(bk2)
    metal_release(bv)
    metal_release(bo)
    metal_release(pso)
}

//! one row of q, k, v at position 5 over a 64-wide model of two heads: q and k rotated as
//! `rope_rows` rotates them, the rotated k and the v landing on the caches' row 5
//! MetalRopeStoreKvT's f32 stamp at the frame loop's binds: neox 0, no bias, the whole head, the tables and the caches at the position's row
def private rope_store_pocket_gate(t : T?; dev, queue) {
    let tag = "rope_store f32 pocket"
    var err : string
    var pso = pipeline_from_source(dev, MetalRopeStoreF32_metal_rope_store_msl, MetalRopeStoreF32_metal_rope_store_msl_entry, MetalRopeStoreF32_metal_rope_store_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let d = 64
    let dh = 32
    let pos = 5
    let cap = 8
    let period = 10000.0
    var qkv <- st2_fill(3 * d, 13, 0.4, -0.9)
    var q : array<float>
    var k : array<float>
    q |> resize(d)
    k |> resize(d)
    for (j in range(d)) {
        q[j] = qkv[j]
        k[j] = qkv[d + j]
    }
    rope_rows(q, 1l, int64(d), int64(dh), int64(pos), period)
    rope_rows(k, 1l, int64(d), int64(dh), int64(pos), period)
    var want_qkv := qkv
    var want_k : array<float>
    var want_v : array<float>
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
    var none : array<float>
    var tcos : array<float>
    var tsin : array<float>
    build_rope_tabs(tcos, tsin, period, 1.0, 1.0, none, false, 0l, int64(cap), int64(dh))
    let ka = RopeStoreArgs(qd = uint(d), head_size = uint(dh), npairs_q = uint(d / 2), npairs = uint(d), neox = 0u, has_bias = 0u, rot = uint(dh))
    var bcos = buf_upload(dev, tcos)
    var bsin = buf_upload(dev, tsin)
    var bbias = buf_fill(dev, 3 * d, 0.0)
    let toff = uint64(pos * (dh / 2) * 4)
    let roff = uint64(pos * d * 4)
    let run <- @(bxx : MetalBuffer?; bkk : MetalBuffer?; bvv : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, tag, pso, d) $(enc) {
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, bxx, uint64(2 * d * 4), 1)
            metal_set_buffer(enc, bkk, roff, 2)
            metal_set_buffer(enc, bvv, roff, 3)
            metal_set_buffer(enc, bcos, toff, 4)
            metal_set_buffer(enc, bsin, toff, 5)
            metal_set_buffer(enc, bbias, 0ul, 6)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 7)
        }
    }
    var bx = buf_upload(dev, qkv)
    var bk = buf_fill(dev, cap * d, -7.0)
    var bv = buf_fill(dev, cap * d, -7.0)
    if (invoke(run, bx, bk, bv)) {
        t |> equal(buf_mismatch_bars(bx, want_qkv, 1e-6, 1e-6, "{tag} q"), 0)
        t |> equal(buf_mismatch_bars(bk, want_k, 1e-6, 1e-6, "{tag} k"), 0)
        t |> equal(buf_mismatch_bars(bv, want_v, 1e-6, 1e-6, "{tag} v"), 0)
        t |> success(want_k[pos * d + 3] != qkv[d + 3], "{tag}: the stored k is the rotated one")
        t |> success(want_k[(pos - 1) * d + 3] == -7.0, "{tag}: the row before stayed a sentinel")
    }
    qkv[d + 10] += 0.5
    var bx2 = buf_upload(dev, qkv)
    var bk2 = buf_fill(dev, cap * d, -7.0)
    var bv2 = buf_fill(dev, cap * d, -7.0)
    if (invoke(run, bx2, bk2, bv2)) {
        t |> success(buf_mismatch_bars(bk2, want_k, 1e-6, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned k element reds the stored row")
    }
    qkv[d + 10] -= 0.5
    qkv[10] += 0.5
    qkv[2 * d + 5] += 0.5
    var bx3 = buf_upload(dev, qkv)
    var bk3 = buf_fill(dev, cap * d, -7.0)
    var bv3 = buf_fill(dev, cap * d, -7.0)
    if (invoke(run, bx3, bk3, bv3)) {
        t |> success(buf_mismatch_bars(bx3, want_qkv, 1e-6, 1e-6, "{tag} q poison (MUST mismatch)") > 0, "{tag}: a poisoned q element reds the rotated q")
        t |> success(buf_mismatch_bars(bv3, want_v, 1e-6, 1e-6, "{tag} v poison (MUST mismatch)") > 0, "{tag}: a poisoned v element reds the stored v")
    }
    metal_release(bx3)
    metal_release(bk3)
    metal_release(bv3)
    metal_release(bcos)
    metal_release(bsin)
    metal_release(bbias)
    metal_release(bx)
    metal_release(bk)
    metal_release(bv)
    metal_release(bx2)
    metal_release(bk2)
    metal_release(bv2)
    metal_release(pso)
}

//! x += a in place and the LayerNorm of the sum at a 16-float offset of y, at a width under the
//! threadgroup and one past it
def private pk_add_ln_gate(t : T?; dev, queue; dim : int) {
    let tag = "pk_add_ln dim{dim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_pk_add_ln_msl, metal_pk_add_ln_msl_entry, metal_pk_add_ln_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let eps = 1e-5
    let yoff = 16
    let x <- st2_fill(dim, 7, 0.5, -0.3)
    var a <- st2_fill(dim, 11, 0.4, 0.2)
    let w <- st2_fill(dim, 5, 0.3, 0.9)
    let bb <- st2_fill(dim, 3, 0.2, -0.1)
    var want_x : array<float>
    want_x |> resize(dim)
    for (i in range(dim)) {
        want_x[i] = x[i] + a[i]
    }
    var yn : array<float>
    yn |> resize(dim)
    unsafe {
        layernorm(addr(yn[0]), addr<float const?>(want_x[0]), addr<float const?>(w[0]), addr<float const?>(bb[0]), int64(dim), eps)
    }
    var want_y : array<float>
    want_y |> resize(yoff + dim)
    for (i in range(yoff + dim)) {
        want_y[i] = i < yoff ? -7.0 : yn[i - yoff]
    }
    let ka = PkAddLnArgs(dim = uint(dim), eps = eps)
    var bw = buf_upload(dev, w)
    var bbb = buf_upload(dev, bb)
    let run <- @(bxx : MetalBuffer?; baa : MetalBuffer?; byy : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_buffer(enc, baa, 0ul, 1)
            metal_set_buffer(enc, bw, 0ul, 2)
            metal_set_buffer(enc, bbb, 0ul, 3)
            metal_set_buffer(enc, byy, uint64(yoff * 4), 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
            metal_set_threadgroup_memory_length(enc, metal_pk_add_ln_msl_tgmem, 0)
            metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(256u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bx = buf_upload(dev, x)
    var ba = buf_upload(dev, a)
    var by = buf_fill(dev, yoff + dim, -7.0)
    if (invoke(run, bx, ba, by)) {
        t |> equal(buf_mismatch_bars(bx, want_x, 1e-6, 1e-6, "{tag} x"), 0)
        t |> equal(buf_mismatch_bars(by, want_y, 1e-5, 1e-6, "{tag} y"), 0)   //! f32 mean and variance over values under 1 against the CPU's double mean and variance: an error of a few f32 ulps (6e-8 each), the bar a hundred of them
    }
    a[dim / 2] += 0.5
    var bx2 = buf_upload(dev, x)
    var ba2 = buf_upload(dev, a)
    var by2 = buf_fill(dev, yoff + dim, -7.0)
    if (invoke(run, bx2, ba2, by2)) {
        t |> success(buf_mismatch_bars(bx2, want_x, 1e-6, 1e-6, "{tag} x poison (MUST mismatch)") > 0, "{tag}: a poisoned addend reds the joined x")
        t |> success(buf_mismatch_bars(by2, want_y, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned addend reds the norm")
    }
    metal_release(bw)
    metal_release(bbb)
    metal_release(bx)
    metal_release(ba)
    metal_release(by)
    metal_release(bx2)
    metal_release(ba2)
    metal_release(by2)
    metal_release(pso)
}

def private pk_gemv_pso(dev; q8 : bool; kind : int; var err : string&) {
    if (q8) {
        if (kind == 1) {
            return pipeline_from_source(dev, metal_pk_gemv_q8_ln_silu_msl, metal_pk_gemv_q8_ln_silu_msl_entry, metal_pk_gemv_q8_ln_silu_msl_fastmath, err)
        } elif (kind == 2) {
            return pipeline_from_source(dev, metal_pk_gemv_q8_gate_msl, metal_pk_gemv_q8_gate_msl_entry, metal_pk_gemv_q8_gate_msl_fastmath, err)
        } elif (kind == 3) {
            return pipeline_from_source(dev, metal_pk_gemv_q8_add_silu_msl, metal_pk_gemv_q8_add_silu_msl_entry, metal_pk_gemv_q8_add_silu_msl_fastmath, err)
        }
        return pipeline_from_source(dev, metal_pk_gemv_q8_tail_msl, metal_pk_gemv_q8_tail_msl_entry, metal_pk_gemv_q8_tail_msl_fastmath, err)
    }
    if (kind == 0) {
        return pipeline_from_source(dev, metal_pk_gemv_f32_msl, metal_pk_gemv_f32_msl_entry, metal_pk_gemv_f32_msl_fastmath, err)
    } elif (kind == 1) {
        return pipeline_from_source(dev, metal_pk_gemv_f32_ln_silu_msl, metal_pk_gemv_f32_ln_silu_msl_entry, metal_pk_gemv_f32_ln_silu_msl_fastmath, err)
    } elif (kind == 2) {
        return pipeline_from_source(dev, metal_pk_gemv_f32_gate_msl, metal_pk_gemv_f32_gate_msl_entry, metal_pk_gemv_f32_gate_msl_fastmath, err)
    } elif (kind == 3) {
        return pipeline_from_source(dev, metal_pk_gemv_f32_add_silu_msl, metal_pk_gemv_f32_add_silu_msl_entry, metal_pk_gemv_f32_add_silu_msl_fastmath, err)
    }
    return pipeline_from_source(dev, metal_pk_gemv_f32_tail_msl, metal_pk_gemv_f32_tail_msl_entry, metal_pk_gemv_f32_tail_msl_fastmath, err)
}

def private pk_gemv_tgmem(dev; q8 : bool; kind : int) : uint64 {   //! `dev` untyped: the module behind `?das_metal` may be absent
    if (q8) {
        return (kind == 1 ? metal_pk_gemv_q8_ln_silu_msl_tgmem : (kind == 2 ? metal_pk_gemv_q8_gate_msl_tgmem
            : (kind == 3 ? metal_pk_gemv_q8_add_silu_msl_tgmem : metal_pk_gemv_q8_tail_msl_tgmem)))
    }
    return kind == 0 ? metal_pk_gemv_f32_msl_tgmem : (kind == 1 ? metal_pk_gemv_f32_ln_silu_msl_tgmem : (kind == 2 ? metal_pk_gemv_f32_gate_msl_tgmem
        : (kind == 3 ? metal_pk_gemv_f32_add_silu_msl_tgmem : metal_pk_gemv_f32_tail_msl_tgmem)))
}
//! the row GEMV over 9 rows of 96 (three q8 blocks; the f32 rows at stride 128) under one stamp:
//! the bare dot, the normed and modulated x with the SiLU out, the gated residual, the SiLU over
//! the slab vector, and the frame tail - x, y and the latent row bound at offsets, an fp64 oracle
def private pk_gemv_gate(t : T?; dev, queue; q8 : bool; kind, nin : int) {
    let tag = "pk_gemv {q8 ? "q8" : "f32"} kind{kind} nin{nin}"
    var err : string
    var pso = pk_gemv_pso(dev, q8, kind, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let nout = 9
    let stride = (nin + 127) / 128 * 128
    let eps = 1e-6
    let xoff = 4
    let yoff = 3
    let loff = 8
    let bar = 2e-7 * float(nin)   //! nin products under 0.25 each summed at f32 (6e-8 an ulp) against the fp64 row: the sum's rounding under nin x 1.5e-8, the bar ten times that
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(nout, max(nin / 32, 1), blob, wdeq)
    if (!q8) {
        delete wdeq
        wdeq <- st2_fill(nout * nin, 29, 0.5, -0.2)   //! the f32 rows take any width; the q8 blob needs whole blocks
    }
    var wf : array<float>
    wf |> resize(nout * stride)
    for (r in range(nout)) {
        for (j in range(nin)) {
            wf[r * stride + j] = wdeq[r * nin + j]
        }
    }
    let b <- st2_fill(nout, 5, 0.2, -0.1)
    let sc = sqrt(96.0 / float(nin))   //! the dot's spread held at the 96-wide case's, so the SiLU epilogues stay off their flat tail and the poison shows
    var x <- st2_fill(xoff + nin, 7, 0.5 * sc, -0.3 * sc)
    let lw <- st2_fill(nin, 3, 0.3 * sc, 0.9 * sc)
    let lb <- st2_fill(nin, 11, 0.2, -0.1)
    let md <- st2_fill(3 * nin, 13, 0.4, 0.1)
    let aux <- st2_fill(2 * nout, 17, 0.3, 0.2)
    let x0 <- st2_fill(nout, 19, 0.5, -0.4)
    let y0 <- st2_fill(yoff + nout, 23, 0.6, 0.3)
    var xs : array<float>
    xs |> resize(nin)
    for (j in range(nin)) {
        xs[j] = x[xoff + j]
    }
    if (kind == 1 || kind == 4) {
        var ln : array<float>
        ln |> resize(nin)
        unsafe {
            layernorm(addr(ln[0]), addr<float const?>(xs[0]), addr<float const?>(lw[0]), addr<float const?>(lb[0]), int64(nin), eps)
        }
        for (j in range(nin)) {
            xs[j] = ln[j] * (1.0 + md[nin + j]) + md[j]
        }
    }
    var want_y := y0
    var want_lat : array<float>
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
    var bw = buf_upload(dev, wf)
    var bb = buf_upload(dev, b)
    var bq = buf_upload(dev, blob)
    var bmod = buf_upload(dev, md)
    var blw = buf_upload(dev, lw)
    var blb = buf_upload(dev, lb)
    var baux = buf_upload(dev, aux)
    var bx0 = buf_upload(dev, x0)
    let run <- @(bxx : MetalBuffer?; byy : MetalBuffer?; blat : MetalBuffer?) : bool {
        var err2 : string
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso)
            metal_set_buffer(enc, bw, 0ul, 0)
            metal_set_buffer(enc, bb, 0ul, 1)
            metal_set_buffer(enc, bq, 0ul, 2)
            metal_set_buffer(enc, bq, 0ul, 3)
            metal_set_buffer(enc, bxx, uint64(xoff * 4), 4)
            metal_set_buffer(enc, byy, uint64(yoff * 4), 5)
            metal_set_buffer(enc, bmod, 0ul, 6)
            metal_set_buffer(enc, blw, 0ul, 7)
            metal_set_buffer(enc, blb, 0ul, 8)
            metal_set_buffer(enc, baux, 0ul, 9)
            metal_set_buffer(enc, bx0, 0ul, 10)
            metal_set_buffer(enc, blat, uint64(loff * 4), 11)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 12)
            metal_set_threadgroup_memory_length(enc, pk_gemv_tgmem(dev, q8, kind), 0)
            metal_dispatch_threadgroups(enc, uint3(uint((nout + 3) / 4), 1u, 1u), uint3(128u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bx = buf_upload(dev, x)
    var by = buf_upload(dev, y0)
    var bl = buf_fill(dev, loff + nout, -7.0)
    if (invoke(run, bx, by, bl)) {
        t |> equal(buf_mismatch_bars(by, want_y, bar, 1e-6, "{tag} y"), 0)
        t |> equal(buf_mismatch_bars(bl, want_lat, bar, 1e-6, "{tag} lat"), 0)
        t |> success(want_y[yoff - 1] == y0[yoff - 1], "{tag}: the row under the offset stayed")
    }
    for (j in range(nin)) {   //! every element moved 4 along the first row's weight signs: the row's dot rises by four times the weights' magnitude sum, more than any input under 1 can pull it down, so it lands on the SiLU's live side whatever the fixture's dot was
        x[xoff + j] += wdeq[j] > 0.0 ? 4.0 : -4.0
    }
    var bx2 = buf_upload(dev, x)
    var by2 = buf_upload(dev, y0)
    var bl2 = buf_fill(dev, loff + nout, -7.0)
    if (invoke(run, bx2, by2, bl2)) {
        t |> success(buf_mismatch_bars(by2, want_y, bar, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned x element reds the compare")
        if (kind == 4) {
            t |> success(buf_mismatch_bars(bl2, want_lat, bar, 1e-6, "{tag} lat poison (MUST mismatch)") > 0, "{tag}: a poisoned x element reds the latent row")
        }
    }
    metal_release(bw)
    metal_release(bb)
    metal_release(bq)
    metal_release(bmod)
    metal_release(blw)
    metal_release(blb)
    metal_release(baux)
    metal_release(bx0)
    metal_release(bx)
    metal_release(by)
    metal_release(bl)
    metal_release(bx2)
    metal_release(by2)
    metal_release(bl2)
    metal_release(pso)
}

//! the sigmoid sums over 50 of 64 columns at 1/1.3 speed
def private st2_sigsum_gate(t : T?; dev, queue) {
    let tag = "st2_sigsum"
    var err : string
    var pso = pipeline_from_source(dev, metal_st2_sigsum_msl, metal_st2_sigsum_msl_entry, metal_st2_sigsum_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let nrows = 37
    let nb = 50
    let ls = 64
    var lg <- st2_fill(nrows * ls, 7, 0.3, -5.0)
    var want : array<float>
    want |> resize(nrows)
    for (r in range(nrows)) {
        var acc = 0.0lf
        for (j in range(nb)) {
            acc += sigmoid_d(double(lg[r * ls + j]))
        }
        want[r] = float(acc) * (1.0 / 1.3)
    }
    var braw = buf_fill(dev, nrows, -1.0e30)
    let run <- @(blg : MetalBuffer?) : bool {
        var u_nb = uint(nb)
        var u_ls = uint(ls)
        var u_speed = 1.3
        var u_t = uint(nrows)
        return dispatch_64_lanes(t, queue, tag, pso, nrows) $(enc) {
            metal_set_buffer(enc, blg, 0ul, 0)
            metal_set_buffer(enc, braw, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(u_nb)), 4ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(u_ls)), 4ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(u_speed)), 4ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(u_t)), 4ul, 5)
        }
    }
    var blg = buf_upload(dev, lg)
    if (invoke(run, blg)) {
        t |> equal(buf_mismatch_bars(braw, want, 1e-5, 1e-6, tag), 0)
    }
    lg[5 * ls + 7] += 6.0
    var blg2 = buf_upload(dev, lg)
    if (invoke(run, blg2)) {
        t |> success(buf_mismatch_bars(braw, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned logit reds the compare")
    }
    metal_release(blg)
    metal_release(blg2)
    metal_release(braw)
    metal_release(pso)
}

//! the tanh-form GELU in place, a value past the clamp among the rows
def private st2_gelu_gate(t : T?; dev, queue) {
    let tag = "st2_gelu_tanh"
    var err : string
    var pso = pipeline_from_source(dev, metal_st2_gelu_tanh_msl, metal_st2_gelu_tanh_msl_entry, metal_st2_gelu_tanh_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let n = 216
    var x <- st2_fill(n, 7, 0.25, -3.0)
    x[17] = 40.0
    x[18] = -40.0
    var want := x
    gelu(want, int64(n))
    let run <- @(bxx : MetalBuffer?) : bool {
        var u_n = uint(n)
        return dispatch_64_lanes(t, queue, tag, pso, n) $(enc) {
            metal_set_buffer(enc, bxx, 0ul, 0)
            metal_set_bytes(enc, unsafe(addr<void?>(u_n)), 4ul, 1)
        }
    }
    var bx = buf_upload(dev, x)
    if (invoke(run, bx)) {
        t |> equal(buf_mismatch_bars(bx, want, 1e-5, 1e-6, tag), 0)
        t |> success(want[3] != x[3], "{tag}: the output differs from the input at [3]")
    }
    x[9] += 2.0
    var bx2 = buf_upload(dev, x)
    if (invoke(run, bx2)) {
        t |> success(buf_mismatch_bars(bx2, want, 1e-5, 1e-6, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned element reds the compare")
    }
    metal_release(bx)
    metal_release(bx2)
    metal_release(pso)
}

def private st2_source_gate(t : T?; dev, queue; torch_math : bool) {
    let tag = torch_math ? "st2_source torch law" : "st2_source onnx law"
    var err : string
    var pso_low = (torch_math ? pipeline_from_source(dev, metal_st2_src_low_torch_msl, metal_st2_src_low_torch_msl_entry, metal_st2_src_low_torch_msl_fastmath, err)
        : pipeline_from_source(dev, metal_st2_src_low_onnx_msl, metal_st2_src_low_onnx_msl_entry, metal_st2_src_low_onnx_msl_fastmath, err))
    t |> success(pso_low != null, "{tag} low pipeline: {err}")
    var pso_cum = (torch_math ? pipeline_from_source(dev, metal_st2_src_cumsum_torch_msl, metal_st2_src_cumsum_torch_msl_entry, metal_st2_src_cumsum_torch_msl_fastmath, err)
        : pipeline_from_source(dev, metal_st2_src_cumsum_onnx_msl, metal_st2_src_cumsum_onnx_msl_entry, metal_st2_src_cumsum_onnx_msl_fastmath, err))
    t |> success(pso_cum != null, "{tag} cumsum pipeline: {err}")
    var pso_sin = (torch_math ? pipeline_from_source(dev, metal_st2_src_sines_torch_msl, metal_st2_src_sines_torch_msl_entry, metal_st2_src_sines_torch_msl_fastmath, err)
        : pipeline_from_source(dev, metal_st2_src_sines_onnx_msl, metal_st2_src_sines_onnx_msl_entry, metal_st2_src_sines_onnx_msl_fastmath, err))
    t |> success(pso_sin != null, "{tag} sines pipeline: {err}")
    var pso_noise = pipeline_from_source(dev, metal_st2_src_noise_msl, metal_st2_src_noise_msl_entry, metal_st2_src_noise_msl_fastmath, err)
    t |> success(pso_noise != null, "{tag} noise pipeline: {err}")
    return if (pso_low == null || pso_cum == null || pso_sin == null || pso_noise == null)
    let up = 300
    let nh = 3
    let frames = 700   //! 210000 samples: the phases past a hundred thousand radians, where a fast sin loses them
    let n = frames * up
    let nlow = int(resize_len(int64(n), 1.0 / float(up)))
    let sr = 24000.0
    var f0 : array<float>
    f0 |> resize(frames)
    for (i in range(frames)) {
        f0[i] = i % 3 == 2 ? 0.0 : float(float16(180.0 + 0.3 * float(i % 97)))
    }
    let nu <- st2_fill(nh, 3, 0.1, 0.2)
    let noise <- st2_fill(n * nh, 7, 0.4, -0.9)
    var lw <- st2_fill(nh, 5, 0.3, 0.2)
    var lb : array<float>
    lb |> push(0.05)
    let ka = St2SourceArgs(n = uint(n), nlow = uint(nlow), nh = uint(nh), up = uint(up), sr = sr, sine_amp = 0.1, noise_std = 0.003,
        voiced_thr = 10.0, seed = 77u)
    let cfg = SineSourceCfg(upsample = int64(up), sample_rate = sr, n_harm = int64(nh), sine_amp = 0.1, noise_std = 0.003, voiced_thr = 10.0, torch_math = torch_math)
    var cr = SineSourceCarry()
    sine_carry_reset(cr, int64(nh), int64(nlow))
    var sines : array<float>
    let rows = sine_source(cfg, f0, int64(frames), nu, 0l, int64(nlow), cr, noise, 0l, sines)
    t |> equal(rows, int64(n), "{tag}: the CPU source mixes every sample")
    var want_lowc : array<float>
    want_lowc |> resize(nh * nlow)
    for (h in range(nh)) {
        for (k in range(nlow)) {
            want_lowc[h * nlow + k] = cr.lowc[int64(h) * cr.stride + int64(k)]
        }
    }
    var lin = TtsLinear(nin = int64(nh), nout = 1l)
    lin.w := lw
    lin.b := lb
    var want_har : array<float>
    linear_rows(lin, sines, int64(n), want_har)
    tanh_inplace(want_har, int64(n))
    var bnu = buf_upload(dev, nu)
    var bnoise = buf_upload(dev, noise)
    var blw = buf_upload(dev, lw)
    var blb = buf_upload(dev, lb)
    var blow = buf_fill(dev, nh * nlow, -1.0e30)
    var blowc = buf_fill(dev, nh * nlow, -1.0e30)
    var bhar = buf_fill(dev, n, -1.0e30)
    let run <- @(bf0 : MetalBuffer?; kax : St2SourceArgs; own : bool; bnz : MetalBuffer?; bhar : MetalBuffer?) : bool {
        var err2 : string
        var kk = kax
        let ran = with_compute_encoder(queue, err2) $(enc : MetalComputeEncoder?) {
            if (own) {   //! the driver's own draw: the noise rows hashed on the device before the sines read them
                metal_set_pipeline(enc, pso_noise)
                metal_set_buffer(enc, bnz, 0ul, 0)
                metal_set_bytes(enc, unsafe(addr<void?>(kk)), uint64(typeinfo sizeof(kk)), 1)
                metal_dispatch_threadgroups(enc, uint3(uint((n * nh + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
            }
            metal_set_pipeline(enc, pso_low)
            metal_set_buffer(enc, bf0, 0ul, 0)
            metal_set_buffer(enc, bnu, 0ul, 1)
            metal_set_buffer(enc, blow, 0ul, 2)
            metal_set_bytes(enc, unsafe(addr<void?>(kk)), uint64(typeinfo sizeof(kk)), 3)
            metal_dispatch_threadgroups(enc, uint3(uint((nh * nlow + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
            metal_set_pipeline(enc, pso_cum)
            metal_set_buffer(enc, blow, 0ul, 0)
            metal_set_buffer(enc, blowc, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(kk)), uint64(typeinfo sizeof(kk)), 2)
            metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(uint(nh), 1u, 1u))
            metal_set_pipeline(enc, pso_sin)
            metal_set_buffer(enc, bf0, 0ul, 0)
            metal_set_buffer(enc, blowc, 0ul, 1)
            metal_set_buffer(enc, bnz, 0ul, 2)
            metal_set_buffer(enc, blw, 0ul, 3)
            metal_set_buffer(enc, blb, 0ul, 4)
            metal_set_buffer(enc, bhar, 0ul, 5)
            metal_set_bytes(enc, unsafe(addr<void?>(kk)), uint64(typeinfo sizeof(kk)), 6)
            metal_dispatch_threadgroups(enc, uint3(uint((n + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
        }
        t |> success(ran, "{tag} dispatch: {err2}")
        return ran
    }
    var bf0 = buf_upload(dev, f0)
    if (invoke(run, bf0, ka, false, bnoise, bhar)) {
        t |> equal(buf_mismatch_bars(blowc, want_lowc, 1.2e-7, 0.0, "{tag} lowc (the phase frames, within one ulp of the CPU carry)"), 0)
        t |> equal(buf_mismatch_bars(bhar, want_har, 2e-6, 2e-7, "{tag} har"), 0)
    }
    let har_captured <- buf_download(bhar, n, type<float>)
    {   // the long cumsum: 40000 low-rate frames of one harmonic, where a plain f32 sum and a double one part by cycles
        let nlong = 40000
        var lowl : array<float>
        var wantl : array<float>
        lowl |> resize(nlong)
        wantl |> resize(nlong)
        var acc64 = 0.0lf
        var accf = 0.0
        var accp = 0.0
        for (k in range(nlong)) {
            lowl[k] = float(float16(0.0071 + 0.0003 * float(k % 13)))
            acc64 += double(lowl[k])
            accf = torch_math ? float(acc64) : accf + lowl[k]
            accp += lowl[k]
            wantl[k] = ((accf * 2.0) * PI) * float(up)
        }
        t |> success(abs(float(acc64) - accp) > 0.01, "{tag} long cumsum: the two laws part by {abs(float(acc64) - accp)} cycles over {nlong} frames (the leg's control)")
        var blowl = buf_upload(dev, lowl)
        var blowcl = buf_fill(dev, nlong, -1.0e30)
        var kl = ka
        kl.nlow = uint(nlong)
        kl.nh = 1u
        var err3 : string
        let ranl = with_compute_encoder(queue, err3) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, pso_cum)
            metal_set_buffer(enc, blowl, 0ul, 0)
            metal_set_buffer(enc, blowcl, 0ul, 1)
            metal_set_bytes(enc, unsafe(addr<void?>(kl)), uint64(typeinfo sizeof(kl)), 2)
            metal_dispatch_threadgroups(enc, uint3(1u, 1u, 1u), uint3(1u, 1u, 1u))
        }
        t |> success(ranl, "{tag} long cumsum dispatch: {err3}")
        if (ranl) {
            t |> equal(buf_mismatch_bars(blowcl, wantl, 2e-7, 1e-3, "{tag} long cumsum"), 0)
            var wantp := wantl
            wantp[nlong - 1] += 2.0   //! two radians on the last frame: the bars are a few ulps of a phase in the thousands
            t |> success(buf_mismatch_bars(blowcl, wantp, 2e-7, 1e-3, "{tag} long cumsum poison (MUST mismatch)") > 0, "{tag} long cumsum: a moved last frame reds the compare (the compare's control)")
        }
        metal_release(blowl)
        metal_release(blowcl)
    }
    f0[4] += 40.0   //! a voiced frame: every later phase moves
    var bf02 = buf_upload(dev, f0)
    if (invoke(run, bf02, ka, false, bnoise, bhar)) {
        t |> success(buf_mismatch_bars(blowc, want_lowc, 1.2e-7, 0.0, "{tag} lowc poison (MUST mismatch)") > 0, "{tag}: a poisoned f0 frame reds the lowc compare")
        t |> success(buf_mismatch_bars(bhar, want_har, 2e-6, 2e-7, "{tag} har poison (MUST mismatch)") > 0, "{tag}: a poisoned f0 frame reds the har compare")
    }
    var ka_own = ka
    var bnz1 = buf_fill(dev, n * nh, -1.0e30)
    var bh1 = buf_fill(dev, n, -1.0e30)
    if (invoke(run, bf0, ka_own, true, bnz1, bh1)) {
        var want_noise : array<float>
        want_noise |> resize(n * nh)
        for (i in range(n * nh)) {
            want_noise[i] = st2_hash_normal(ka_own.seed, uint(i))
        }
        t |> equal(buf_mismatch_bars(bnz1, want_noise, 1e-5, 1e-6, "{tag} own draw vs the CPU hash"), 0)   //! the kernel's log and cos under the default fast math against the CPU's: within 1e-5 of a value under 5
        var own1 <- buf_download(bh1, n, type<float>)
        var in_tanh_range = true
        for (v in own1) {
            in_tanh_range = in_tanh_range && abs(v) <= 1.0
        }
        t |> success(in_tanh_range, "{tag}: the driver's own noise draw stays within tanh's range")
        t |> success(buf_mismatch_bars(bh1, har_captured, 2e-6, 2e-7, "{tag} own draw vs captured (MUST mismatch)") > 0, "{tag}: the own draw differs from the captured rows")
        var bnz2 = buf_fill(dev, n * nh, -1.0e30)
        var bh2 = buf_fill(dev, n, -1.0e30)
        if (invoke(run, bf0, ka_own, true, bnz2, bh2)) {
            t |> equal(buf_mismatch_bars(bh2, own1, 0.0, 0.0, "{tag} own draw repeat"), 0)
        }
        metal_release(bnz2)
        metal_release(bh2)
        for (other in [78u, 81u]) {   //! 81 = 77 xor 4: a raw-seed xor into the counter would alias it to a shuffled 77
            ka_own.seed = other
            var bnz3 = buf_fill(dev, n * nh, -1.0e30)
            var bh3 = buf_fill(dev, n, -1.0e30)
            if (invoke(run, bf0, ka_own, true, bnz3, bh3)) {
                t |> success(buf_mismatch_bars(bh3, own1, 2e-6, 2e-7, "{tag} own draw seed {other} (MUST mismatch)") > 0, "{tag}: seed {other} draws other noise than seed 77")
            }
            metal_release(bnz3)
            metal_release(bh3)
        }
    }
    metal_release(bnz1)
    metal_release(bh1)
    metal_release(bf0)
    metal_release(bf02)
    metal_release(bnu)
    metal_release(bnoise)
    metal_release(blw)
    metal_release(blb)
    metal_release(blow)
    metal_release(blowc)
    metal_release(bhar)
    metal_release(pso_low)
    metal_release(pso_cum)
    metal_release(pso_sin)
    metal_release(pso_noise)
}

//! the STFT as two convs over the padded signal on either pad law, magnitudes and phases
def private st2_stft_gate(t : T?; dev, queue; reflect : bool) {
    let tag = reflect ? "st2_stft reflect" : "st2_stft edge"
    var err : string
    var pso = (reflect ? pipeline_from_source(dev, metal_st2_stft_reflect_msl, metal_st2_stft_reflect_msl_entry, metal_st2_stft_reflect_msl_fastmath, err)
        : pipeline_from_source(dev, metal_st2_stft_edge_msl, metal_st2_stft_edge_msl_entry, metal_st2_stft_edge_msl_fastmath, err))
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let n = 40
    let k = 8
    let hop = 4
    let pad = 2
    let bins = 3
    let frames = (n + 2 * pad - k) / hop + 1
    var har <- st2_fill(n, 7, 0.05, -0.9)
    let wre <- st2_fill(bins * k, 5, 0.2, -0.7)
    var wim <- st2_fill(bins * k, 3, 0.15, -0.5)
    wim[2 * k + 5] = 0.0
    var want : array<float>
    want |> resize(frames * 2 * bins)
    for (f in range(frames)) {
        for (b in range(bins)) {
            var re = 0.0lf
            var im = 0.0lf
            for (tap in range(k)) {
                var src = f * hop + tap - pad
                if (src < 0) {
                    src = reflect ? -src : 0
                } elif (src >= n) {
                    src = reflect ? 2 * (n - 1) - src : n - 1
                }
                re += double(wre[b * k + tap]) * double(har[src])
                im += double(wim[b * k + tap]) * double(har[src])
            }
            want[f * 2 * bins + b] = float(sqrt(re * re + im * im + 1e-9lf))
            let imf = float(im)
            let ref = float(re)
            want[f * 2 * bins + bins + b] = imf == 0.0 ? (ref < 0.0 ? PI : 0.0) : atan2(imf, ref)
        }
    }
    var bre = buf_upload(dev, wre)
    var bim = buf_upload(dev, wim)
    var bspec = buf_fill(dev, frames * 2 * bins, -1.0e30)
    let run <- @(bh : MetalBuffer?) : bool {
        var ka = St2StftArgs(n = uint(n), frames = uint(frames), bins = uint(bins), k = uint(k), hop = uint(hop), pad = uint(pad), eps = 1e-9)
        return dispatch_64_lanes(t, queue, tag, pso, frames * bins) $(enc) {
            metal_set_buffer(enc, bh, 0ul, 0)
            metal_set_buffer(enc, bre, 0ul, 1)
            metal_set_buffer(enc, bim, 0ul, 2)
            metal_set_buffer(enc, bspec, 0ul, 3)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        }
    }
    var bh = buf_upload(dev, har)
    if (invoke(run, bh)) {
        t |> equal(buf_mismatch_bars(bspec, want, 1e-4, 1e-5, tag), 0)
    }
    har[1] += 2.0   //! the first frame's reflected tap reads it
    var bh2 = buf_upload(dev, har)
    if (invoke(run, bh2)) {
        t |> success(buf_mismatch_bars(bspec, want, 1e-4, 1e-5, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned sample reds the compare")
    }
    metal_release(bh)
    metal_release(bh2)
    metal_release(bre)
    metal_release(bim)
    metal_release(bspec)
    metal_release(pso)
}

//! the inverse STFT from log-magnitude / phase rows against a CPU overlap-add twin, with and without the envelope
def private st2_istft_gate(t : T?; dev, queue; envelope : bool) {
    let tag = envelope ? "st2_istft envelope" : "st2_istft plain"
    var err : string
    var pso = pipeline_from_source(dev, metal_st2_istft_msl, metal_st2_istft_msl_entry, metal_st2_istft_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let tt = 9
    let stride = 8
    let bins = 3
    let k = 8
    let hop = 4
    let pad = 2
    let n = (tt - 1) * hop + k - 2 * pad
    var y <- st2_fill(tt * stride, 7, 0.15, -1.2)
    let wre <- st2_fill(k * bins, 5, 0.2, -0.7)   //! [k][bins], the transposed conv's operand layout
    let wim <- st2_fill(k * bins, 3, 0.15, -0.5)
    let window <- st2_fill(k, 11, 0.1, 0.3)
    var want : array<float>
    want |> resize(n)
    for (i in range(n)) {
        let m = i + pad
        var acc = 0.0lf
        var env = 0.0lf
        for (tap in range(k)) {
            continue if (m < tap || (m - tap) % hop != 0)
            let q = (m - tap) / hop
            continue if (q >= tt)
            for (b in range(bins)) {
                let mag = exp(y[q * stride + b])
                let ph = sin(y[q * stride + bins + b])
                acc += double(mag * cos(ph) * wre[tap * bins + b]) - double(mag * sin(ph) * wim[tap * bins + b])
            }
            env += double(window[tap] * window[tap])
        }
        want[i] = float((envelope && env > 1e-11lf) ? acc / env : acc)
    }
    var bre = buf_upload(dev, wre)
    var bim = buf_upload(dev, wim)
    var bwn = buf_upload(dev, window)
    var bw = buf_fill(dev, n, -1.0e30)
    let run <- @(byy : MetalBuffer?) : bool {
        var ka = St2IstftArgs(tt = uint(tt), stride = uint(stride), bins = uint(bins), k = uint(k), hop = uint(hop), pad = uint(pad),
            envelope = envelope ? 1u : 0u, n = uint(n))
        return dispatch_64_lanes(t, queue, tag, pso, n) $(enc) {
            metal_set_buffer(enc, byy, 0ul, 0)
            metal_set_buffer(enc, bre, 0ul, 1)
            metal_set_buffer(enc, bim, 0ul, 2)
            metal_set_buffer(enc, bwn, 0ul, 3)
            metal_set_buffer(enc, bw, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        }
    }
    var by = buf_upload(dev, y)
    if (invoke(run, by)) {
        t |> equal(buf_mismatch_bars(bw, want, 1e-4, 1e-5, tag), 0)
    }
    y[4 * stride + 1] += 2.0   //! a log-magnitude: every sample its frame overlaps moves
    var by2 = buf_upload(dev, y)
    if (invoke(run, by2)) {
        t |> success(buf_mismatch_bars(bw, want, 1e-4, 1e-5, "{tag} poison (MUST mismatch)") > 0, "{tag}: a poisoned magnitude reds the compare")
    }
    metal_release(by)
    metal_release(by2)
    metal_release(bre)
    metal_release(bim)
    metal_release(bwn)
    metal_release(bw)
    metal_release(pso)
}

// ===== the tower row gather (the qwen25v stem's window-sort reorder) =====
//! The stem's window-sort reorder: MetalRowGather with both adds off over a full permutation
def private tower_gather_gate(t : T?; dev, queue) {
    let tag = "tower_gather"
    var err : string
    var pso = pipeline_from_source(dev, metal_row_gather_msl, metal_row_gather_msl_entry, metal_row_gather_msl_fastmath, err)
    t |> success(pso != null, "{tag} pipeline: {err}")
    return if (pso == null)
    let nr = 37
    let d = 64
    var xv : array<float>
    xv |> resize(nr * d)
    for (i in range(nr * d)) {
        xv[i] = 0.03 * float((i * 17) % 61) - 0.9
    }
    var src : array<uint>
    src |> resize(nr)
    for (r in range(nr)) {
        src[r] = uint((r * 11) % nr)   //! 11 coprime to 37: a full permutation
    }
    var want : array<float>
    want |> resize(nr * d)
    for (r in range(nr)) {
        for (c in range(d)) {
            want[r * d + c] = xv[int(src[r]) * d + c]
        }
    }
    var bx = buf_upload(dev, xv)
    var bs = buf_upload(dev, src)
    var by = buf_fill(dev, nr * d, -1.0e30)
    let ka = RowGatherArgs(c = uint(d), add_b = 0u, add_c = 0u, total = uint(nr * d))
    let run <- @(bidx : MetalBuffer?) : bool {
        return dispatch_64_lanes(t, queue, tag, pso, nr * d) $(enc) {
            metal_set_buffer(enc, bx, 0ul, 0)
            metal_set_buffer(enc, bidx, 0ul, 1)
            metal_set_buffer(enc, bx, 0ul, 2)
            metal_set_buffer(enc, bx, 0ul, 3)
            metal_set_buffer(enc, by, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        }
    }
    if (invoke(run, bs)) {
        t |> equal(buf_mismatch_exact(by, want, tag), 0)
    }
    src[nr / 2] = src[nr / 2] == 0u ? 1u : 0u
    var bs2 = buf_upload(dev, src)
    if (invoke(run, bs2)) {
        t |> success(buf_mismatch_exact(by, want, "{tag} poison (MUST mismatch)") > 0,
            "{tag}: a swapped gather index reds the exact compare")
    }
    metal_release(bx)
    metal_release(bs)
    metal_release(bs2)
    metal_release(by)
    metal_release(pso)
    delete xv
    delete src
    delete want
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
def private rope_arm_gate(t : T?; dev, queue; pso; arm : RopeArm) {
    let half = arm.rot / 2
    var rv : array<float>
    var rcos : array<float>
    var rsin : array<float>
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
    var rvwant := rv
    unsafe {
        for (p in range(NPOS)) {
            if (arm.rot < HEAD_SIZE) {
                rope_scaled_neox_tab_part(addr(rvwant[p * QD]), int64(HEAD_SIZE), int64(arm.rot),
                    int64(QD), addr<float const?>(rcos[p * half]), addr<float const?>(rsin[p * half]))
            } elif (arm.neox) {
                rope_scaled_neox_tab(addr(rvwant[p * QD]), int64(HEAD_SIZE), int64(QD),
                    addr<float const?>(rcos[p * half]), addr<float const?>(rsin[p * half]))
            } else {
                rope_scaled_tab(addr(rvwant[p * QD]), int64(HEAD_SIZE), int64(QD),
                    addr<float const?>(rcos[p * half]), addr<float const?>(rsin[p * half]))
            }
        }
    }
    var brv = buf_upload(dev, rv)
    var brc = buf_upload(dev, rcos)
    var brs = buf_upload(dev, rsin)
    var brn = buf_u32(dev, uint(QD))
    var brhs = buf_u32(dev, uint(HEAD_SIZE))
    var brnp = buf_u32(dev, uint(NPOS * QD / 2))
    var brnx = buf_u32(dev, arm.neox ? 1u : 0u)
    var brot = buf_u32(dev, uint(arm.rot))
    var bufs <- [brv, brc, brs, brn, brhs, brnp, brnx, brot, brn]   // brn again: the row stride (contiguous rows)
    var err : string
    let ran = run_compute_1d(queue, pso, bufs, uint(NPOS * QD / 2), 64u, err)
    t |> success(ran, "{arm.tag} dispatch: {err}")
    t |> equal(buf_mismatch_approx(brv, rvwant), 0)
    bufs |> clear()
    unsafe {
        delete bufs
    }
    metal_release(brv)
    metal_release(brc)
    metal_release(brs)
    metal_release(brn)
    metal_release(brhs)
    metal_release(brnp)
    metal_release(brnx)
    metal_release(brot)
    delete rv
    delete rcos
    delete rsin
    delete rvwant
}

//! One `_metal_ew2_msl` stamp, y's op into x in place, against its CPU op: `approx` bars, or bit-exact where the op is one rounding on both sides.
def private ew2_gate(t : T?; dev, queue; tag : string; pso; x, y, want : array<float>; exact : bool) : array<float> {
    let total = length(x)
    var bx = buf_upload(dev, x)
    var by = buf_upload(dev, y)
    var btot = buf_u32(dev, uint(total))
    var bone = buf_f32(dev, 1.0)   // the ascale slot at its identity (exact: *1.0); a stamp without the slot erases it
    var bufs <- [bx, by, btot, bone]
    var err : string
    let ran = run_compute_1d(queue, pso, bufs, uint(total), 64u, err)
    t |> success(ran, "{tag} dispatch: {err}")
    t |> equal(exact ? buf_mismatch_exact(bx, want) : buf_mismatch_approx(bx, want), 0, tag)
    var got <- buf_download(bx, total, type<float>)
    bufs |> clear()
    unsafe {
        delete bufs
    }
    metal_release(bx)
    metal_release(by)
    metal_release(btot)
    metal_release(bone)
    return <- got
}

[test]
def test_metal_prefill_kernels(t : T?) {
    t |> run("phase-6 prefill kernels == dasLLAMA CPU twins") @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                var err : string

                // ===== rmsnorm =====
                var pso_rms = pipeline_from_source(dev, MetalRmsNorm_metal_rmsnorm_msl, MetalRmsNorm_metal_rmsnorm_msl_entry, MetalRmsNorm_metal_rmsnorm_msl_fastmath, err)
                t |> success(pso_rms != null, "rmsnorm pipeline: {err}")
                var rx : array<float>
                var rw : array<float>
                var rwant : array<float>
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
                var brx = buf_upload(dev, rx)
                var brw = buf_upload(dev, rw)
                var bry = buf_fill(dev, NROWS * DIM, 0.0)
                var brdim = buf_u32(dev, uint(DIM))
                var breps = buf_f32(dev, EPS)
                let rran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
                    metal_set_pipeline(enc, pso_rms)
                    metal_set_threadgroup_memory_length(enc, MetalRmsNorm_metal_rmsnorm_msl_tgmem, 0)
                    metal_set_buffer(enc, brx, 0ul, 0)
                    metal_set_buffer(enc, brw, 0ul, 1)
                    metal_set_buffer(enc, bry, 0ul, 2)
                    metal_set_buffer(enc, brdim, 0ul, 3)
                    metal_set_buffer(enc, breps, 0ul, 4)
                    metal_dispatch_threadgroups(enc, uint3(uint(NROWS), 1u, 1u), uint3(256u, 1u, 1u))
                }
                t |> success(rran, "rmsnorm dispatch: {err}")
                t |> equal(buf_mismatch_approx(bry, rwant), 0)
                metal_release(brx)
                metal_release(brw)
                metal_release(bry)
                metal_release(brdim)
                metal_release(breps)
                metal_release(pso_rms)

                var pso_rope = pipeline_from_source(dev, metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath, err)
                t |> success(pso_rope != null, "rope pipeline: {err}")
                let rope_arms <- [
                    RopeArm(tag = "rope", neox = false, rot = HEAD_SIZE, xscale = 0.11, xmod = 41, xbias = -2.2, tstep = 0.01, tmod = 97),
                    RopeArm(tag = "rope NEOX", neox = true, rot = HEAD_SIZE, xscale = 0.13, xmod = 37, xbias = -1.9, tstep = 0.01, tmod = 97),
                    RopeArm(tag = "rope partial-NEOX", neox = true, rot = HEAD_SIZE / 2, xscale = 0.17, xmod = 33, xbias = -2.4, tstep = 0.02, tmod = 89)
                ]
                for (arm in rope_arms) {
                    rope_arm_gate(t, dev, queue, pso_rope, arm)
                }
                metal_release(pso_rope)

                // ===== the fused qk_norm+rope kernel: every (hx, donorm, pairing) production arm =====
                qk_rope_gate(t, dev, queue, false, true, true, false)    //! K rows, qwen3 class
                qk_rope_gate(t, dev, queue, true, true, true, false)     //! q panel + twin, qwen3 class
                qk_rope_gate(t, dev, queue, false, true, false, false)   //! classic pairing with norm
                qk_rope_gate(t, dev, queue, true, false, false, false)   //! rope-only + twin, llama class
                qk_rope_gate(t, dev, queue, false, false, true, true)    //! partial rot, Wave D class

                tower_flash_kvhc_gate(t, dev, queue)
                tower_win_gate(t, dev, queue)
                g4a_attn_gate(t, dev, queue)
                fc_pack_gate(t, dev, queue)
                fc_softmax_gate(t, dev, queue, 24)
                fc_softmax_gate(t, dev, queue, 300)
                fc_unpack_gate(t, dev, queue)
                fc_dw_gate(t, dev, queue, false)
                fc_dw_gate(t, dev, queue, true)
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
                pk_attn_gate(t, dev, queue, 0, 10)
                pk_attn_gate(t, dev, queue, 8, 10)
                pk_attn_gate(t, dev, queue, 0, 300)   //! past one 256-key stride of the staged scores
                pk_attn_gate(t, dev, queue, 8, 300)
                rope_store_pocket_gate(t, dev, queue)
                pk_add_ln_gate(t, dev, queue, 100)
                pk_add_ln_gate(t, dev, queue, 300)
                pk_add_ln_gate(t, dev, queue, 512)   //! two whole 256-lane strides
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
                var pso_sw = pipeline_from_source(dev, MetalSwiglu_metal_ew2_msl, MetalSwiglu_metal_ew2_msl_entry, MetalSwiglu_metal_ew2_msl_fastmath, err)
                t |> success(pso_sw != null, "swiglu pipeline: {err}")
                var pso_add = pipeline_from_source(dev, MetalAdd_metal_ew2_msl, MetalAdd_metal_ew2_msl_entry, MetalAdd_metal_ew2_msl_fastmath, err)
                t |> success(pso_add != null, "add pipeline: {err}")
                let total = NROWS * HIDDEN
                var sg : array<float>
                var su : array<float>
                sg |> resize(total)
                su |> resize(total)
                for (k in range(total)) {
                    sg[k] = 0.21 * float(k % 43) - 4.3
                    su[k] = 0.31 * float(k % 17) - 2.5
                }
                var sg_want := sg
                swiglu(sg_want, su, int64(total))
                let sg_gpu <- ew2_gate(t, dev, queue, "swiglu", pso_sw, sg, su, sg_want, false)
                // add: x += y, bit-exact (one fadd on both sides) — seeded from the GPU gate
                // output, not the CPU one: the exp-envelope difference upstream would otherwise
                // masquerade as an add mismatch
                var add_want : array<float>
                add_want |> resize(total)
                for (k in range(total)) {
                    add_want[k] = sg_gpu[k] + su[k]
                }
                var added <- ew2_gate(t, dev, queue, "add", pso_add, sg_gpu, su, add_want, true)
                delete added
                metal_release(pso_sw)
                metal_release(pso_add)

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
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.q8, 96, 24, 5, 3, 1, false)   // shared x (the gate/up sites)
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.q8, 64, 32, 7, 4, 1, true)    // per-slot x rows (the down site)
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.q8, 64, 24, 5, 3, 3, false)   // batch streams, shared-x form
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.q8, 64, 24, 5, 3, 3, true)    // batch streams, down form
                moe_combine_gate(t, dev, queue, 200, 4)

                // ===== q51 expert kernels (the 26B-A4B Q4_K_M down stacks) =====
                // exact-arithmetic q51 planes (pow2 d, quarter m, int x) — BIT-exact both kernels;
                // n = 96 covers 3 blocks/row, the mulmm shapes cover padded CSR tails (ce % 32)
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.q51, 96, 24, 5, 3, 1, false)   // gate/up site (shared x)
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.q51, 64, 32, 7, 4, 1, true)    // down site (per-slot x)
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.q51, 64, 24, 5, 3, 3, true)    // batch streams, down form
                moe_mulmm_q51_gate(t, dev, queue, 64, 64, [32, 40, 64])    // padded tail on expert 1
                moe_mulmm_q51_gate(t, dev, queue, 96, 128, [8, 96])        // 2 col tiles, tiny run + multi-tile
                moe_mulmm_q51_gate(t, dev, queue, 64, 64, [136, 32])       // counts crossing 128 — both tall-pair stamps live

                // ===== mx4 expert GEMV (reworked to the q51 shape — gpt-oss's decode kernel) =====
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.mx4, 96, 24, 5, 3, 1, false)          // gate/up (shared x)
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.mx4, 64, 32, 7, 4, 1, true, true)     // down site + bias rows
                moe_gemv_gate(t, dev, queue, MoeGemvFmt.mx4, 64, 24, 5, 3, 3, true, true)     // batch streams, biased down

                // ===== deltanet kernels (Wave D) =====
                // the chain gates compare each stage at its seam vs a sequential CPU mirror;
                // shapes: decode (npos 1), a batch window, npos < taps (history straddle),
                // and nkh == nvh (the 0.8B class; the rest exercise the h % nkh fan-out)
                dn_chain_gate(t, dev, queue, 2, 4, 4, 1)
                dn_chain_gate(t, dev, queue, 2, 4, 4, 5)
                dn_chain_gate(t, dev, queue, 2, 4, 4, 2)
                dn_chain_gate(t, dev, queue, 4, 4, 4, 3)
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

                metal_release(queue)
            }
            t |> equal(metal_log_leaks_if_any(), 0l)
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}

// ===== MoE plumbing gates (Wave C: swiglu-oai, g4 router norm, wscale, router, CSR, reduce) =====

// the decode form (MetalSwigluOai): in-place clamped swiglu over a flat span, the enc_ew2 shape
def private swiglu_oai_gate(t : T?; dev, queue; total : int) {
    let tag = "swiglu_oai total={total}"
    var err : string
    var pso = pipeline_from_source(dev, MetalSwigluOai_metal_swiglu_oai_msl, MetalSwigluOai_metal_swiglu_oai_msl_entry, MetalSwigluOai_metal_swiglu_oai_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var g : array<float>
    var u : array<float>
    g |> resize(total)
    u |> resize(total)
    for (i in range(total)) {
        g[i] = float((i * 7) % 23 - 11)   // past the +-7 clamps on both sides
        u[i] = float((i * 5 + 3) % 23 - 11)
    }
    var want := g
    swiglu_oai(want, u, int64(total))
    var bg = buf_upload(dev, g)
    var bu = buf_upload(dev, u)
    var btot = buf_u32(dev, uint(total))
    dispatch_64_lanes(t, queue, tag, pso, total) $(enc) {
        metal_set_buffer(enc, bg, 0ul, 0)
        metal_set_buffer(enc, bu, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
    }
    t |> equal(buf_mismatch_approx(bg, want), 0)
    metal_release(bg)
    metal_release(bu)
    metal_release(btot)
    metal_release(pso)
    delete g
    delete u
    delete want
}

// the prefill twin (MetalSwigluOaiPf): same math over the padded bucket panel, plus the
// liveness bound — ghost rows past the last bucket's padded end must stay UNTOUCHED
def private swiglu_oai_pf_gate(t : T?; dev, queue; nfe : int; counts : array<int>; extra_rows : int) {
    let tag = "swiglu_oai_pf nfe={nfe}"
    var err : string
    var pso = pipeline_from_source(dev, MetalSwigluOaiPf_metal_swiglu_oai_msl, MetalSwigluOaiPf_metal_swiglu_oai_msl_entry, MetalSwigluOaiPf_metal_swiglu_oai_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let ne = length(counts)
    var (cnt, basep, pad) <- padded_bases(counts)
    let live = pad * nfe
    let total = (pad + extra_rows) * nfe
    var g : array<float>
    var u : array<float>
    g |> resize(total)
    u |> resize(total)
    for (i in range(total)) {
        g[i] = float((i * 7) % 23 - 11)
        u[i] = float((i * 5 + 3) % 23 - 11)
    }
    var want := g
    swiglu_oai(want, u, int64(live))
    var bg = buf_upload(dev, g)
    var bu = buf_upload(dev, u)
    var btot = buf_u32(dev, uint(total))
    var bcnt = buf_upload(dev, cnt)
    var bbase = buf_upload(dev, basep)
    var bne = buf_u32(dev, uint(ne))
    var bnfe = buf_u32(dev, uint(nfe))
    dispatch_64_lanes(t, queue, tag, pso, total / 4) $(enc) {   //! four elements a thread
        metal_set_buffer(enc, bg, 0ul, 0)
        metal_set_buffer(enc, bu, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
        metal_set_buffer(enc, bcnt, 0ul, 3)
        metal_set_buffer(enc, bbase, 0ul, 4)
        metal_set_buffer(enc, bne, 0ul, 5)
        metal_set_buffer(enc, bnfe, 0ul, 6)
    }
    t |> equal(buf_mismatch_approx(bg, want), 0)
    metal_release(bg)
    metal_release(bu)
    metal_release(btot)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bne)
    metal_release(bnfe)
    metal_release(pso)
    delete cnt
    delete basep
    delete g
    delete u
    delete want
}

// gemma4 router-input norm (MetalG4RouterNorm): y = x * (1/sqrt(mean(x^2)+eps)) * invd * rs
def private g4_router_norm_gate(t : T?; dev, queue; rows, dim : int) {
    let tag = "g4_router_norm rows={rows} dim={dim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_g4_router_norm_msl, metal_g4_router_norm_msl_entry, metal_g4_router_norm_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let eps = 1.0e-5
    let invd = 1.0 / sqrt(float(dim))
    var x : array<float>
    var rs : array<float>
    x |> resize(rows * dim)
    rs |> resize(dim)
    for (i in range(rows * dim)) {
        x[i] = 0.13 * float(i % 29) - 1.7
    }
    for (i in range(dim)) {
        rs[i] = 0.07 * float(i % 17) - 0.5
    }
    var want : array<float>
    want |> resize(rows * dim)
    for (r in range(rows)) {
        unsafe {
            rmsnorm(addr(want[r * dim]), addr<float const?>(x[r * dim]), addr<float const?>(rs[0]), int64(dim), eps)
        }
        for (c in range(dim)) {
            want[r * dim + c] *= invd
        }
    }
    var bx = buf_upload(dev, x)
    var brs = buf_upload(dev, rs)
    var by = buf_fill(dev, rows * dim, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var beps = buf_f32(dev, eps)
    var binvd = buf_f32(dev, invd)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_g4_router_norm_msl_tgmem, 0)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, brs, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bdim, 0ul, 3)
        metal_set_buffer(enc, beps, 0ul, 4)
        metal_set_buffer(enc, binvd, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint(rows), 1u, 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    t |> equal(buf_mismatch_approx(by, want), 0)
    metal_release(bx)
    metal_release(brs)
    metal_release(by)
    metal_release(bdim)
    metal_release(beps)
    metal_release(binvd)
    metal_release(pso)
    delete x
    delete rs
    delete want
}

// gemma4 per-expert down-scale fold (MetalMoeWScale): selw[r][j] *= ds[seli[r][j]] in place on
// the mixed sel buffer (uint idx half + float w half) — compared as raw uint bits
def private moe_wscale_gate(t : T?; dev, queue; rows, nk, ne : int) {
    let tag = "moe_wscale rows={rows} nk={nk}"
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_wscale_msl, metal_moe_wscale_msl_entry, metal_moe_wscale_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var ds : array<float>
    ds |> resize(ne)
    for (e in range(ne)) {
        ds[e] = float((e * 3) % 7 - 3) * 0.5
    }
    var sel : array<uint>
    var want : array<uint>
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
    var bsel = buf_upload(dev, sel)
    var bds = buf_upload(dev, ds)
    var bnk = buf_u32(dev, uint(nk))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bsel, 0ul, 0)   // uint idx view
        metal_set_buffer(enc, bsel, 0ul, 1)   // float w view of the same buffer
        metal_set_buffer(enc, bds, 0ul, 2)
        metal_set_buffer(enc, bnk, 0ul, 3)
        metal_dispatch_threadgroups(enc, uint3(uint(rows), 1u, 1u), uint3(32u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    t |> equal(buf_mismatch_exact(bsel, want), 0)
    metal_release(bsel)
    metal_release(bds)
    metal_release(bnk)
    metal_release(pso)
    delete ds
    delete sel
    delete want
}

//! The router GEMV BIT-exact on dyadic data: npos 1 = MetalMoeRouter (BATCHED gates slot 8 out); past one MetalMoeRouterB, 8 streams a column, pad stores gated.
def private moe_router_gate(t : T?; dev, queue; dim, ne, npos : int; hasb : bool) {
    let tag = "moe_router{npos > 1 ? "_b" : ""} dim={dim} ne={ne} np={npos}{hasb ? " bias" : ""}"
    var err : string
    var pso = (npos > 1
        ? pipeline_from_source(dev, MetalMoeRouterB_metal_moe_router_msl, MetalMoeRouterB_metal_moe_router_msl_entry, MetalMoeRouterB_metal_moe_router_msl_fastmath, err)
        : pipeline_from_source(dev, MetalMoeRouter_metal_moe_router_msl, MetalMoeRouter_metal_moe_router_msl_entry, MetalMoeRouter_metal_moe_router_msl_fastmath, err))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var w : array<float>
    var x : array<float>
    var rb : array<float>
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
    var want : array<float>
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
    var bw = buf_upload(dev, w)
    var bx = buf_upload(dev, x)
    var by = buf_fill(dev, npos * ne, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var bne = buf_u32(dev, uint(ne))
    var brb = buf_upload(dev, rb)
    var bhasb = buf_u32(dev, hasb ? 1u : 0u)
    var bnp = buf_u32(dev, uint(npos))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bdim, 0ul, 3)
        metal_set_buffer(enc, bne, 0ul, 4)
        metal_set_buffer(enc, bdim, 0ul, 5)   // per-stream x stride = dim
        metal_set_buffer(enc, brb, 0ul, 6)
        metal_set_buffer(enc, bhasb, 0ul, 7)
        if (npos > 1) {
            metal_set_buffer(enc, bnp, 0ul, 8)
        }
        metal_dispatch_threadgroups(enc, uint3(uint((ne + 3) / 4), uint((npos + 7) / 8), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bdim)
    metal_release(bne)
    metal_release(brb)
    metal_release(bhasb)
    metal_release(bnp)
    metal_release(pso)
    delete w
    delete x
    delete rb
    delete want
}

//! The padded-CSR prefix: each expert's rows from a 32-aligned base (the route prefix contract); `pad` = the padded total.
def private padded_bases(counts : array<int>) : tuple<cnt : array<uint>; base : array<uint>; pad : int> {
    var cnt : array<uint>
    var base : array<uint>
    cnt |> reserve(length(counts))
    base |> reserve(length(counts))
    var pad = 0
    for (c in counts) {
        cnt |> push(uint(c))
        base |> push(uint(pad))
        pad += (c + 31) / 32 * 32
    }
    return <- (cnt = cnt, base = base, pad = pad)
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
    var bkt : array<uint>
    var inv : array<uint>
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
def private moe_csr_gate(t : T?; dev, queue; npos, ne, nk : int) {
    let tag = "moe_csr np={npos} ne={ne} nk={nk}"
    var err : string
    var psoC = pipeline_from_source(dev, metal_moe_count_msl, metal_moe_count_msl_entry, metal_moe_count_msl_fastmath, err)
    t |> success(psoC != null, "{tag}: count pipeline: {err}")
    return if (psoC == null)
    var psoB = pipeline_from_source(dev, metal_moe_bucket_msl, metal_moe_bucket_msl_entry, metal_moe_bucket_msl_fastmath, err)
    t |> success(psoB != null, "{tag}: bucket pipeline: {err}")
    if (psoB == null) {
        metal_release(psoC)
        return
    }
    let total = npos * nk
    var sel : array<uint>
    sel |> resize(npos * 2 * nk)
    for (r in range(npos)) {
        for (j in range(nk)) {
            sel[r * 2 * nk + j] = uint((r * 3 + j * 5 + 1) % ne)
            sel[r * 2 * nk + nk + j] = float_bits_to_uint(1.0)   // weight half — unread here
        }
    }
    var (cntw, basew, bktw, invw, pad) <- csr_ref(sel, npos, ne, nk)   // basew[ne] = the padded total the extra threadgroup publishes
    var bsel = buf_upload(dev, sel)
    var bcnt = buf_fill(dev, ne, 0xFFFFFFFF)
    var bbase = buf_fill(dev, ne + 1, 0xFFFFFFFF)
    var bbkt = buf_fill(dev, pad, 0xEEEEEEEE)
    var binv = buf_fill(dev, total, 0xFFFFFFFF)
    var btot = buf_u32(dev, uint(total))
    var bnk = buf_u32(dev, uint(nk))
    var bne = buf_u32(dev, uint(ne))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, psoC)
        metal_set_buffer(enc, bsel, 0ul, 0)
        metal_set_buffer(enc, bcnt, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
        metal_set_buffer(enc, bnk, 0ul, 3)
        metal_dispatch_threadgroups(enc, uint3(uint(ne), 1u, 1u), uint3(32u, 1u, 1u))
        metal_set_pipeline(enc, psoB)
        metal_set_buffer(enc, bsel, 0ul, 0)
        metal_set_buffer(enc, bbase, 0ul, 1)
        metal_set_buffer(enc, bbkt, 0ul, 2)
        metal_set_buffer(enc, binv, 0ul, 3)
        metal_set_buffer(enc, btot, 0ul, 4)
        metal_set_buffer(enc, bnk, 0ul, 5)
        metal_set_buffer(enc, bcnt, 0ul, 6)
        metal_set_buffer(enc, bne, 0ul, 7)
        metal_dispatch_threadgroups(enc, uint3(uint(ne + 1), 1u, 1u), uint3(32u, 1u, 1u))   // + the total's threadgroup
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bcnt, cntw), 0)
        t |> equal(buf_mismatch_exact(bbase, basew), 0)
        t |> equal(buf_mismatch_exact(bbkt, bktw), 0)
        t |> equal(buf_mismatch_exact(binv, invw), 0)
    }
    metal_release(bsel)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(binv)
    metal_release(btot)
    metal_release(bnk)
    metal_release(bne)
    metal_release(psoC)
    metal_release(psoB)
    delete sel
    delete cntw
    delete basew
    delete bktw
    delete invw
}

// the gather-X panel (MetalMoeGatherX): real bkt rows copy their token's xf row into the f16
// panel; the sentinel, an expert's stamped pad tail, and STALE words past the last expert's
// tail all resolve through the ONE range test — a stale word >= total zeroes (the old
// sentinel-equality form sent its token index out past the activation rows), a stale word
// below total copies a live row into a slot no mm tile reads. All compares f16-exact.
def private moe_gather_x_gate(t : T?; dev, queue; npos, ne, nk, dim : int) {
    let tag = "moe_gather_x np={npos} ne={ne} nk={nk} dim={dim}"
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_gather_x_msl, metal_moe_gather_x_msl_entry, metal_moe_gather_x_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = npos * nk
    var sel : array<uint>
    sel |> resize(npos * 2 * nk)
    for (r in range(npos)) {
        for (j in range(nk)) {
            sel[r * 2 * nk + j] = uint((r * 3 + j * 5 + 1) % ne)
        }
    }
    var (cntw, basew, bktw, invw, pad) <- csr_ref(sel, npos, ne, nk)
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
    var xfw : array<float>
    xfw |> resize((npos + 1) * dim)
    for (r in range(npos)) {
        for (i in range(dim)) {
            xfw[r * dim + i] = float(r * 10 + 1)
        }
    }
    for (i in range(dim)) {
        xfw[npos * dim + i] = 777.0   // one past the real rows: reachable ONLY by a broken validity test
    }
    var want : array<float16>
    want |> resize(mpad * dim)
    for (r in range(mpad)) {
        let v = bktw[r]
        let live = v < uint(total)
        for (i in range(dim)) {
            want[r * dim + i] = live ? float16(xfw[int(v / uint(nk)) * dim + i]) : float16(0.0)
        }
    }
    var bbkt = buf_upload(dev, bktw)
    var bxf = buf_upload(dev, xfw)
    var bxg = buf_fill(dev, mpad * dim, float16(555.0))
    var gka = MoeGxArgs(dim = uint(dim), nk = uint(nk), ne = uint(ne), total = uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bbkt, 0ul, 0)
        metal_set_buffer(enc, bxf, 0ul, 1)
        metal_set_buffer(enc, bxg, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(gka)), uint64(typeinfo sizeof(gka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(mpad), 1u, 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_exact(bxg, want), 0)
    }
    metal_release(bbkt)
    metal_release(bxf)
    metal_release(bxg)
    metal_release(pso)
    delete sel
    delete cntw
    delete basew
    delete bktw
    delete invw
    delete xfw
    delete want
}

// the ordered weighted reduce (MetalMoeReduce): y[r] = sum_j selw[r][j] * dn[inv[r*k+j]] in
// slot order — BIT-exact on the pow2 fixture
def private moe_reduce_gate(t : T?; dev, queue; dim, npos, nk : int) {
    let tag = "moe_reduce dim={dim} np={npos} nk={nk}"
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_reduce_msl, metal_moe_reduce_msl_entry, metal_moe_reduce_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nrows_dn = npos * nk + 7
    var dn : array<float>
    dn |> resize(nrows_dn * dim)
    for (i in range(nrows_dn * dim)) {
        dn[i] = float((i * 5) % 9 - 4) * 0.5
    }
    var sel : array<uint>
    sel |> resize(npos * 2 * nk)
    var inv : array<uint>
    inv |> resize(npos * nk)
    for (r in range(npos)) {
        for (j in range(nk)) {
            sel[r * 2 * nk + j] = 0u
            sel[r * 2 * nk + nk + j] = float_bits_to_uint(float((r + j * 3) % 9 - 4) * 0.25)
            inv[r * nk + j] = uint(((r * nk + j) * 3 + 1) % nrows_dn)
        }
    }
    var want : array<float>
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
    var bdn = buf_upload(dev, dn)
    var bsel = buf_upload(dev, sel)
    var binv = buf_upload(dev, inv)
    var by = buf_fill(dev, npos * dim, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var bnk = buf_u32(dev, uint(nk))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bdn, 0ul, 0)
        metal_set_buffer(enc, bsel, 0ul, 1)   // float w view
        metal_set_buffer(enc, binv, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bdim, 0ul, 4)
        metal_set_buffer(enc, bnk, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint((dim + 255) / 256), uint(npos), 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bdn)
    metal_release(bsel)
    metal_release(binv)
    metal_release(by)
    metal_release(bdim)
    metal_release(bnk)
    metal_release(pso)
    delete dn
    delete sel
    delete inv
    delete want
}

// ===== prefill misc gates (PLE pre-step, bias epilogue, MTP cat, deltanet ba GEMV) =====

//! PLE stage 1: fmt 0 the q8 table bound twice vs its exact dequant; a kq id its native planes bound at sb0, as production binds them, vs kq_row_ref; exact in fp32.
def private ple_gather_gate(t : T?; dev, queue; fmt, vocab, nlayers, ple, npos, sb0 : int) {
    let tag = "ple_gather f={fmt} v={vocab} nl={nlayers} ple={ple} np={npos} sb0={sb0}"
    var err : string
    var pso = (fmt == 0
        ? pipeline_from_source(dev, metal_ple_gather_q8_msl, metal_ple_gather_q8_msl_entry, metal_ple_gather_q8_msl_fastmath, err)
        : (fmt == 4
            ? pipeline_from_source(dev, MetalPleGatherK4_metal_ple_gather_kq_msl, MetalPleGatherK4_metal_ple_gather_kq_msl_entry, MetalPleGatherK4_metal_ple_gather_kq_msl_fastmath, err)
            : (fmt == 5
                ? pipeline_from_source(dev, MetalPleGatherK5_metal_ple_gather_kq_msl, MetalPleGatherK5_metal_ple_gather_kq_msl_entry, MetalPleGatherK5_metal_ple_gather_kq_msl_fastmath, err)
                : pipeline_from_source(dev, MetalPleGatherQ40_metal_ple_gather_kq_msl, MetalPleGatherQ40_metal_ple_gather_kq_msl_entry, MetalPleGatherQ40_metal_ple_gather_kq_msl_fastmath, err))))
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let rowlen = nlayers * ple
    let rowsb = rowlen / 256          // superblocks per token row - the quants gate's `all % 256` clause
    let nsb = sb0 + vocab * rowsb
    var quants : array<uint8>             // fmt 0: the q8 blob
    var ks : array<uint8>
    var wdeq : array<float>
    if (fmt == 0) {
        q8_blob_fill(1, vocab * rowlen / 32, quants, wdeq)
    } else {
        kq_fill_planes(fmt, nsb, quants, ks)
    }
    var tok : array<uint>
    tok |> resize(npos)
    for (p in range(npos)) {
        tok[p] = uint((p * 23 + 5) % vocab)
    }
    let sc0 = sqrt(float(ple))
    var wrow : array<float>
    var want : array<float>
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
    var bkq = buf_upload(dev, quants)
    var bks = fmt == 0 ? bkq : buf_upload(dev, ks)
    var btok = buf_upload(dev, tok)
    var by = buf_fill(dev, nlayers * npos * ple, -1000.0)
    let soff = fmt == 0 ? 0ul : uint64(sb0 * 16)
    let qoff = fmt == 0 ? 0ul : uint64(int64(sb0) * kq_qsb(fmt))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bks, soff, 0)
        if (fmt != 40) {
            metal_set_buffer(enc, bks, soff, 1)   // the byte (q8) / uint4 (quants) view of the same blocks - `@template_gate = "!Q40"` drops it on q40
        }
        metal_set_buffer(enc, btok, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        if (fmt != 0) {
            metal_set_buffer(enc, bkq, qoff, 6)
        }
        metal_dispatch_threadgroups(enc, uint3(uint(nlayers), uint(npos), 1u), uint3(uint(ple), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    if (ran) {
        t |> equal(buf_mismatch_approx(by, want), 0, "{tag}: gather vs the table's row dequant")
        // the control: the same compare must RED on a poisoned expectation - an added
        // abs(want[0])*0.1 + 1.0, past buf_mismatch_approx's 1e-4*max(1, |v|) band at any magnitude
        var wantp := want
        wantp[0] += abs(want[0]) * 0.1 + 1.0
        t |> success(buf_mismatch_approx(by, wantp) != 0, "{tag}: poison control reds")
        delete wantp
    }
    metal_release(bkq)
    if (fmt != 0) {
        metal_release(bks)
    }
    metal_release(btok)
    metal_release(by)
    metal_release(pso)
    delete quants
    delete ks
    delete wdeq
    delete tok
    delete wrow
    delete want
}

// PLE stage 3 (MetalPleFinish): per (layer, position) slice, in place on the panel —
// side = (side + pnorm * (v * rsqrt(mean(v^2) + eps))) / sqrt(2), v = proj/sqrt(dim)
def private ple_finish_gate(t : T?; dev, queue; nlayers, ple, npos : int) {
    let tag = "ple_finish nl={nlayers} ple={ple} np={npos}"
    var err : string
    var pso = pipeline_from_source(dev, metal_ple_finish_msl, metal_ple_finish_msl_entry, metal_ple_finish_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let rowlen = nlayers * ple
    let eps = 1.0e-5
    let inv_sd = 0.125   // stands in for 1/sqrt(dim)
    let inv_s2 = 1.0 / sqrt(2.0)
    var proj : array<float>
    var pnorm : array<float>
    var side : array<float>
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
    var want : array<float>
    var v : array<float>
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
    delete v
    var ka = PleArgs(npos = uint(npos), ple = uint(ple), nlayers = uint(nlayers), sc0 = 1.0,
        inv_sd = inv_sd, eps = eps, inv_s2 = inv_s2)
    var bproj = buf_upload(dev, proj)
    var bpn = buf_upload(dev, pnorm)
    var by = buf_upload(dev, side)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_ple_finish_msl_tgmem, 0)
        metal_set_buffer(enc, bproj, 0ul, 0)
        metal_set_buffer(enc, bpn, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_dispatch_threadgroups(enc, uint3(uint(nlayers), uint(npos), 1u), uint3(uint(ple), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    t |> equal(buf_mismatch_approx(by, want), 0)
    metal_release(bproj)
    metal_release(bpn)
    metal_release(by)
    metal_release(pso)
    delete proj
    delete pnorm
    delete side
    delete want
}

// the QKV bias epilogue (MetalAddBiasRows): x[r*n + i] += b[i] in place, BIT-exact
def private add_bias_rows_gate(t : T?; dev, queue; nrows, n : int) {
    let tag = "add_bias_rows rows={nrows} n={n}"
    var err : string
    var pso = pipeline_from_source(dev, metal_add_bias_rows_msl, metal_add_bias_rows_msl_entry, metal_add_bias_rows_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = nrows * n
    var x : array<float>
    var b : array<float>
    x |> resize(total)
    b |> resize(n)
    for (i in range(total)) {
        x[i] = float((i * 5) % 9 - 4) * 0.5
    }
    for (i in range(n)) {
        b[i] = float((i * 7 + 1) % 9 - 4) * 0.25
    }
    var want : array<float>
    want |> resize(total)
    for (i in range(total)) {
        want[i] = x[i] + b[i % n]
    }
    var bx = buf_upload(dev, x)
    var bb = buf_upload(dev, b)
    var bn = buf_u32(dev, uint(n))
    var btot = buf_u32(dev, uint(total))
    dispatch_64_lanes(t, queue, tag, pso, total) $(enc) {
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bb, 0ul, 1)
        metal_set_buffer(enc, bn, 0ul, 2)
        metal_set_buffer(enc, btot, 0ul, 3)
    }
    t |> equal(buf_mismatch_exact(bx, want), 0)
    metal_release(bx)
    metal_release(bb)
    metal_release(bn)
    metal_release(btot)
    metal_release(pso)
    delete x
    delete b
    delete want
}

// the MTP eh_proj interleave (MetalPfCat2): y[p] = [a[p] ; b[p]], pure copy — BIT-exact
def private pf_cat2_gate(t : T?; dev, queue; npos, d : int) {
    let tag = "pf_cat2 np={npos} d={d}"
    var err : string
    var pso = pipeline_from_source(dev, metal_pf_cat2_msl, metal_pf_cat2_msl_entry, metal_pf_cat2_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let total = npos * d
    var a : array<float>
    var b : array<float>
    a |> resize(total)
    b |> resize(total)
    for (i in range(total)) {
        a[i] = float(i * 3 % 17) * 0.5
        b[i] = float(i * 5 % 19) * 0.25 - 2.0
    }
    var want : array<float>
    want |> resize(npos * 2 * d)
    for (p in range(npos)) {
        for (i in range(d)) {
            want[p * 2 * d + i] = a[p * d + i]
            want[p * 2 * d + d + i] = b[p * d + i]
        }
    }
    var ba = buf_upload(dev, a)
    var bb = buf_upload(dev, b)
    var by = buf_fill(dev, npos * 2 * d, -1000.0)
    var bd = buf_u32(dev, uint(d))
    var btot = buf_u32(dev, uint(total))
    dispatch_64_lanes(t, queue, tag, pso, total) $(enc) {
        metal_set_buffer(enc, by, 0ul, 0)
        metal_set_buffer(enc, ba, 0ul, 1)
        metal_set_buffer(enc, bb, 0ul, 2)
        metal_set_buffer(enc, bd, 0ul, 3)
        metal_set_buffer(enc, btot, 0ul, 4)
    }
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(ba)
    metal_release(bb)
    metal_release(by)
    metal_release(bd)
    metal_release(btot)
    metal_release(pso)
    delete a
    delete b
    delete want
}

// the deltanet beta/alpha projection GEMV (MetalDnBa): narrow q8 blob rows per position —
// ndim large enough to walk both the 4-chunk fast stripe AND the strided tail, BIT-exact
def private dn_ba_gate(t : T?; dev, queue; ndim, ddim, npos : int) {
    let tag = "dn_ba n={ndim} d={ddim} np={npos}"
    var err : string
    var pso = pipeline_from_source(dev, MetalDnBa_metal_q8_gemv_msl, MetalDnBa_metal_q8_gemv_msl_entry, MetalDnBa_metal_q8_gemv_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(1, ddim * ndim / 32, blob, wdeq)
    var x : array<float>
    x |> resize(npos * ndim)
    for (i in range(npos * ndim)) {
        x[i] = float((i * 5 + i / 17) % 9 - 4) * 0.5
    }
    var want : array<float>
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
    var bw = buf_upload(dev, blob)
    var bx = buf_upload(dev, x)
    var by = buf_fill(dev, npos * ddim, -1000.0)
    var bn = buf_u32(dev, uint(ndim))
    var bd = buf_u32(dev, uint(ddim))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   // half-scale view
        metal_set_buffer(enc, bw, 0ul, 1)   // byte view
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_set_buffer(enc, bd, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint((ddim + 7) / 8), uint(npos), 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bd)
    metal_release(pso)
    delete blob
    delete wdeq
    delete x
    delete want
}

// The classifier GEMV kernel on exact-arithmetic data: [d x n] Q8 rows (the 34B blob form —
// half scale + 32 int8, mulmm_gate's layout) dot one f32 vector vs a direct CPU dot,
// BIT-exact (see the call site for why exactness holds; pow2 scales are exact in f16).
def private gemv_gate(t : T?; dev, queue; n, d : int) {
    var err : string
    var pso = pipeline_from_source(dev, MetalQ8Gemv_metal_q8_gemv_msl, MetalQ8Gemv_metal_q8_gemv_msl_entry, MetalQ8Gemv_metal_q8_gemv_msl_fastmath, err)
    t |> success(pso != null, "gemv pipeline: {err}")
    if (pso == null) {
        return
    }
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(d, n / 32, blob, wdeq)
    var xv : array<float>
    var want : array<float>
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
    var bw = buf_upload(dev, blob)   // bound TWICE — half + byte views
    // one sentinel float past the output: the odd-d tail guard's only witness - an unguarded
    // pair row writes element d, which a d-wide compare never reads
    want |> push(-1000.0)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, d + 1, -1000.0)
    var bn = buf_u32(dev, uint(n))
    var bd = buf_u32(dev, uint(d))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   // half-scale view
        metal_set_buffer(enc, bw, 0ul, 1)   // byte view
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bn, 0ul, 4)
        metal_set_buffer(enc, bd, 0ul, 5)
        metal_set_threadgroup_memory_length(enc, MetalQ8Gemv_metal_q8_gemv_msl_tgmem, 0)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 1) / 2), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "gemv dispatch n={n} d={d}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bd)
    metal_release(pso)
    delete blob
    delete wdeq
    delete xv
    delete want
}

// The production mul_mm kernel on exact-arithmetic data: 34B q8-block W blobs (GGUF q8_0's
// own layout: f16 scale + 32 int8) x raw f32 X rows vs a CPU reference, BIT-exact (quants in
// [-8, 7], pow2 scales: every staged half, every product, and every f32 partial sum is exactly
// representable, so any deviation is a real bug). The blob is built here independently of the
// driver — it doubles as the layout oracle for upload_blob_region.
def private mulmm_gate(t : T?; dev, queue; k, d, m : int) {
    var err : string
    var pso = pipeline_from_source(dev, metal_q8_mulmm_msl, metal_q8_mulmm_msl_entry, metal_q8_mulmm_msl_fastmath, err)
    t |> success(pso != null, "mul_mm pipeline: {err}")
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(d, k / 32, blob, wdeq)
    var bw = buf_upload(dev, blob)
    // X: raw f32 rows, integer-times-pow2 values (exact in half)
    var xf : array<float>
    q8_x_fill(m * k, xf)
    var bx = buf_upload(dev, xf)
    var by = buf_fill(dev, m * d, 0.0)
    var bk = buf_u32(dev, uint(k))
    var bd = buf_u32(dev, uint(d))
    var want : array<float>
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
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_q8_mulmm_msl_tgmem, 0)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bk, 0ul, 4)
        metal_set_buffer(enc, bd, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(d / 64), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "mul_mm dispatch {k}x{d}x{m}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bk)
    metal_release(bd)
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    if (pso != null) {
        metal_release(pso)
    }
    delete blob
    delete wdeq
    delete xf
    delete want
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
def private moe_select_gate(t : T?; dev, queue; ne, k, nst : int; ties : bool; gmode : int = 0) {
    var err : string
    var pso : MetalComputePipeline?
    if (ne > 256) {
        pso = pipeline_from_source(dev, MetalMoeSelect512_metal_moe_select_msl, MetalMoeSelect512_metal_moe_select_msl_entry, MetalMoeSelect512_metal_moe_select_msl_fastmath, err)
    } else {
        pso = pipeline_from_source(dev, MetalMoeSelect_metal_moe_select_msl, MetalMoeSelect_metal_moe_select_msl_entry, MetalMoeSelect_metal_moe_select_msl_fastmath, err)
    }
    t |> success(pso != null, "moe select pipeline: {err}")
    if (pso == null) {
        return
    }
    var lg : array<float>
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
    var lgc := lg
    var sidx : array<int64>
    var ww : array<float>
    sidx |> resize(nst * k)
    ww |> resize(nst * k)
    for (s in range(nst)) {
        unsafe {
            moe_select_core(gmode == 0 ? MoeGate.softmax : MoeGate.softmax_weight, int64(ne), int64(k), gmode == 0, 1.0,
                addr(lgc[s * ne]), addr(sidx[0]), addr(ww[0]), int64(s * k))
        }
    }
    var widx <- [for (i in sidx); uint(i)]
    delete lgc
    delete sidx
    var blg = buf_upload(dev, lg)
    var bsel = metal_new_buffer(dev, uint64(nst * 2 * k * 4))
    var bne = buf_u32(dev, uint(ne))
    var bnk = buf_u32(dev, uint(k))
    var bgm = buf_u32(dev, uint(gmode))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, blg, 0ul, 0)
        metal_set_buffer(enc, bsel, 0ul, 1)
        metal_set_buffer(enc, bsel, 0ul, 2)   // the float w view — row layout is in-kernel
        metal_set_buffer(enc, bne, 0ul, 3)
        metal_set_buffer(enc, bnk, 0ul, 4)
        metal_set_buffer(enc, bgm, 0ul, 5)
        metal_dispatch_threadgroups(enc, uint3(uint(nst), 1u, 1u), uint3(32u, 1u, 1u))
    }
    t |> success(ran, "moe select dispatch ne={ne} k={k} nst={nst}: {err}")
    let bad = moe_select_bad(bsel, widx, ww, nst, k)
    t |> equal(bad.idx, 0)
    t |> equal(bad.w, 0)
    // the control: the same compares must RED on a poisoned expectation - the first winner moved
    // to its neighbour id (no id can satisfy both) and its weight off by an added 1.0 + a tenth of
    // itself, past any band `approx` admits on a renormalized weight
    var pidx := widx
    var pww := ww
    pidx[0] = (pidx[0] + 1u) % uint(ne)
    pww[0] += abs(ww[0]) * 0.1 + 1.0
    let badp = moe_select_bad(bsel, pidx, pww, nst, k)
    t |> success(badp.idx + badp.w > 0, "moe select ne={ne} k={k}: poison control reds")
    delete pidx
    delete pww
    metal_release(blg)
    metal_release(bsel)
    metal_release(bne)
    metal_release(bnk)
    metal_release(bgm)
    metal_release(pso)
    delete lg
    delete widx
    delete ww
}

enum private MoeGemvFmt {
    q8
    q51
    mx4
}

//! the mx4 nibble values: the signed doubled-e2m1 table
let private MX4_VT = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0,
                                 0.0, -1.0, -2.0, -3.0, -4.0, -6.0, -8.0, -12.0)

//! An expert stack's planes in `fmt` (q8: plane 1 empty, one blob serves both views) and their exact dequant, BIT-exact under any GPU reduction order.
def private moe_gemv_planes(fmt : MoeGemvFmt; rows, n : int) : tuple<p0 : array<uint8>; p1 : array<uint8>; wref : array<float>> {
    var p0 : array<uint8>
    var p1 : array<uint8>
    var wref : array<float>
    if (fmt == MoeGemvFmt.q8) {
        q8_blob_fill(rows, n / 32, p0, wref)
    } elif (fmt == MoeGemvFmt.q51) {
        q51_test_planes(rows, n, p0, p1, wref)
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
def private moe_gemv_gate(t : T?; dev, queue; fmt : MoeGemvFmt; n, d, net, k, nst : int; perslot : bool; bias : bool = false) {
    let tag = "moe gemv {fmt} n={n} d={d} net={net} k={k} nst={nst}{bias ? " bias" : ""}"
    var err : string
    var pso : MetalComputePipeline?
    if (fmt == MoeGemvFmt.q8) {
        pso = pipeline_from_source(dev, metal_moe_gemv_q8_msl, metal_moe_gemv_q8_msl_entry, metal_moe_gemv_q8_msl_fastmath, err)
    } elif (fmt == MoeGemvFmt.q51) {
        pso = pipeline_from_source(dev, metal_moe_gemv_q51_msl, metal_moe_gemv_q51_msl_entry, metal_moe_gemv_q51_msl_fastmath, err)
    } else {
        pso = pipeline_from_source(dev, metal_moe_gemv_mx4_msl, metal_moe_gemv_mx4_msl_entry, metal_moe_gemv_mx4_msl_fastmath, err)
    }
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let nb = n / 32
    var (p0, p1, wref) <- moe_gemv_planes(fmt, net * d, n)
    // selection rows: [k idx u32][k w f32] per stream — only the idx section is read here
    var sel : array<uint>
    sel |> resize(nst * 2 * k)
    for (s in range(nst)) {
        for (j in range(k)) {
            sel[s * 2 * k + j] = uint((s + j * 3 + net - 1) % net)
        }
    }
    let xrows = perslot ? nst * k : nst
    var xv : array<float>
    xv |> resize(xrows * n)
    for (i in range(xrows * n)) {
        xv[i] = float((i * 3) % 9 - 4)
    }
    var wb : array<float>
    wb |> resize(net * d)
    for (i in range(net * d)) {
        wb[i] = bias ? float(i % 7 - 3) * 0.25 : 0.0
    }
    var want : array<float>
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
    var bw = buf_upload(dev, p0)
    var bs = fmt == MoeGemvFmt.q8 ? bw : buf_upload(dev, p1)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nst * k * d, -1000.0)
    var bsel = buf_upload(dev, sel)
    var bwb = buf_upload(dev, wb)
    var vtf <- [for (v in MX4_VT); v]
    var bvt = buf_upload(dev, vtf)
    var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
        xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
        bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = bias ? 1u : 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        if (fmt == MoeGemvFmt.mx4) {
            metal_set_threadgroup_memory_length(enc, metal_moe_gemv_mx4_msl_tgmem, 0)
        }
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bs, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_set_buffer(enc, bsel, 0ul, 6)
        if (fmt == MoeGemvFmt.mx4) {
            metal_set_buffer(enc, bwb, 0ul, 12)
            metal_set_buffer(enc, bvt, 0ul, 15)
        }
        let tg = fmt == MoeGemvFmt.q8 ? 128u : 64u
        metal_dispatch_threadgroups(enc, uint3(uint((d + 3) / 4), uint(k), uint(nst)), uint3(tg, 1u, 1u))
    }
    t |> success(ran, "{tag} dispatch: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0, tag)
    metal_release(bw)
    if (fmt != MoeGemvFmt.q8) {
        metal_release(bs)
    }
    metal_release(bx)
    metal_release(by)
    metal_release(bsel)
    metal_release(bwb)
    metal_release(bvt)
    metal_release(pso)
    delete p0
    delete p1
    delete wref
    delete sel
    delete xv
    delete wb
    delete want
    delete vtf
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

// The gathered q51 mul_mm: ne experts with padded-CSR row runs (gather = 0 bucket-rows mode);
// exact-arithmetic planes (the A tile's f16(d*q + m) values are dyadic <= 63.25 — f16-exact),
// int x rows — every simdgroup accumulate is exact, so the whole padded y compares BIT-exact.
//! device-free: the dev-W knee arithmetic (the pick the board's deep-dense cells ride).
//! Expectations hold at the DEVW_SMALL_PANEL default (32 MiB) - no sidecar loads here
[test, unused_argument(t)]   // off-Apple the body feints without touching t
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
        feint("das_metal is not built on this platform; the knee predicate is not compiled\n")
    }
}

//! device-free: the dense tall-kq pick - the byte knee, the 128-row floor, and the
//! set_metal_prefill_tall A/B seat (the seat clause is what the override contract pins)
[test, unused_argument(t)]   // off-Apple the body feints without touching t
def test_tallkq_pick(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> success(tallkq_pick(512l, 16384l, 4096l, true), "over-knee tall panel takes")
        t |> success(!tallkq_pick(512l, 16384l, 4096l, false), "set_metal_prefill_tall(false) pins the tall form off")
        t |> success(!tallkq_pick(96l, 16384l, 4096l, true), "under the 128-row floor declines")
        t |> success(!tallkq_pick(512l, 4096l, 4096l, true), "under the byte knee declines")
    } else {
        feint("das_metal is not built on this platform; the knee predicate is not compiled\n")
    }
}

def private moe_mulmm_q51_gate(t : T?; dev, queue; kdim, ndim : int; counts : array<int>) {
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_mulmm_q51_msl, metal_moe_mulmm_q51_msl_entry, metal_moe_mulmm_q51_msl_fastmath, err)
    t |> success(pso != null, "moe q51 mulmm pipeline: {err}")
    if (pso == null) {
        return
    }
    let ne = length(counts)
    let nb = kdim / 32
    var qp : array<uint8>
    var sp : array<uint8>
    var wref : array<float>
    q51_test_planes(ne * ndim, kdim, qp, sp, wref)
    var (cnt, basep, pad) <- padded_bases(counts)
    var xv : array<float>
    xv |> resize(pad * kdim)
    for (i in range(pad * kdim)) {
        xv[i] = float((i * 5) % 9 - 4)
    }
    // padded tail rows are computed from their (defined) x rows too — want covers the WHOLE buffer
    var want : array<float>
    want |> resize(pad * ndim)
    for (e in range(ne)) {
        let rows = (counts[e] + 31) / 32 * 32
        for (r in range(rows)) {
            let row = int(basep[e]) + r
            for (col in range(ndim)) {
                var acc = 0.0
                for (c in range(kdim)) {
                    acc += wref[(e * ndim + col) * kdim + c] * xv[row * kdim + c]
                }
                want[row * ndim + col] = acc
            }
        }
    }
    var bw = buf_upload(dev, qp)
    var bs = buf_upload(dev, sp)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, pad * ndim, -1000.0)
    var bcnt = buf_upload(dev, cnt)
    var bbase = buf_upload(dev, basep)
    var bbkt = buf_u32(dev, 0u)   // gather = 0: bkt unread — a 4B stand-in satisfies the binding
    var ka = MoeMmArgs(kdim = uint(kdim), ndim = uint(ndim), eplane = uint(ndim * nb),
        nk = 1u, gather = 0u, hasb = 0u)
    let tiles = (pad + 31) / 32
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_moe_mulmm_q51_msl_tgmem, 0)
        metal_set_buffer(enc, bs, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        // the MetalMoeMulMmBase family's shared numbering — 2 is the k-quant third weight view
        metal_set_buffer(enc, bx, 0ul, 3)
        metal_set_buffer(enc, by, 0ul, 4)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
        metal_set_buffer(enc, bcnt, 0ul, 7)
        metal_set_buffer(enc, bbase, 0ul, 8)
        metal_set_buffer(enc, bbkt, 0ul, 9)
        metal_dispatch_threadgroups(enc, uint3(uint(tiles), uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "moe q51 mulmm dispatch kdim={kdim} ndim={ndim} ne={ne} pad={pad}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    // the contiguous tensor twin (TH stamp over an f16 X): same exact-arithmetic oracle —
    // pow2 d / quarter m / int x all stage to f16 losslessly, so the twin is BIT-exact too.
    // The leg keys on the ENGINE's own pick predicate: weakening moe_q51_twin_fits dispatches
    // the twin on the kdim=96 fixture, whose 64-deep K walk reads off the plane and mismatches.
    if (!moe_q51_twin_fits(uint(kdim))) {
        metal_release(bw)
        metal_release(bs)
        metal_release(bx)
        metal_release(by)
        metal_release(bcnt)
        metal_release(bbase)
        metal_release(bbkt)
        metal_release(pso)
        delete qp
        delete sp
        delete wref
        delete cnt
        delete basep
        delete xv
        delete want
        return
    }
    var terr : string
    var tw_pso = pipeline_from_source(dev, MetalMoeMulMmQ51TH_metal_moe_mulmm_q51_t_msl, MetalMoeMulMmQ51TH_metal_moe_mulmm_q51_t_msl_entry, MetalMoeMulMmQ51TH_metal_moe_mulmm_q51_t_msl_fastmath, terr)
    if (tw_pso == null && !metal4_tensor_available(dev)) {
        to_log(LOG_INFO, "moe q51 mulmm twin: no Metal-4 tensor toolchain - twin leg skipped\n")
    } else {
        t |> success(tw_pso != null, "moe q51 mulmm twin: pipeline: {terr}")
    }
    if (tw_pso != null) {
        var xh <- [for (v in xv); float16(v)]
        var bxh = buf_upload(dev, xh)
        var by_t = buf_fill(dev, pad * ndim, -500.0)
        let rant = with_compute_encoder(queue, terr) $(enc : MetalComputeEncoder?) {
            metal_set_pipeline(enc, tw_pso)
            metal_set_threadgroup_memory_length(enc, MetalMoeMulMmQ51TH_metal_moe_mulmm_q51_t_msl_tgmem, 0)
            metal_set_buffer(enc, bs, 0ul, 0)
            metal_set_buffer(enc, bw, 0ul, 1)
            metal_set_buffer(enc, bxh, 0ul, 3)
            metal_set_buffer(enc, by_t, 0ul, 4)
            metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
            metal_set_buffer(enc, bcnt, 0ul, 6)
            metal_set_buffer(enc, bbase, 0ul, 7)
            metal_dispatch_threadgroups(enc, uint3(uint(tiles), uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
        }
        t |> success(rant, "moe q51 mulmm twin: encode: {terr}")
        if (rant) {
            t |> equal(buf_mismatch_exact(by_t, want), 0)
        }
        // the float-X T stamp (the bxh == null pick arm): stages raw f32 rows to the same
        // f16 tiles, so the int-valued fixtures stay bit-exact through it too
        var tf_pso = pipeline_from_source(dev, MetalMoeMulMmQ51T_metal_moe_mulmm_q51_t_msl, MetalMoeMulMmQ51T_metal_moe_mulmm_q51_t_msl_entry, MetalMoeMulMmQ51T_metal_moe_mulmm_q51_t_msl_fastmath, terr)
        t |> success(tf_pso != null, "moe q51 mulmm T stamp: pipeline: {terr}")
        if (tf_pso != null) {
            var by_f = buf_fill(dev, pad * ndim, -600.0)
            let ranf = with_compute_encoder(queue, terr) $(enc : MetalComputeEncoder?) {
                metal_set_pipeline(enc, tf_pso)
                metal_set_threadgroup_memory_length(enc, MetalMoeMulMmQ51T_metal_moe_mulmm_q51_t_msl_tgmem, 0)
                metal_set_buffer(enc, bs, 0ul, 0)
                metal_set_buffer(enc, bw, 0ul, 1)
                metal_set_buffer(enc, bx, 0ul, 3)
                metal_set_buffer(enc, by_f, 0ul, 4)
                metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
                metal_set_buffer(enc, bcnt, 0ul, 6)
                metal_set_buffer(enc, bbase, 0ul, 7)
                metal_dispatch_threadgroups(enc, uint3(uint(tiles), uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
            }
            t |> success(ranf, "moe q51 mulmm T stamp: encode: {terr}")
            if (ranf) {
                t |> equal(buf_mismatch_exact(by_f, want), 0)
            }
            metal_release(by_f)
            metal_release(tf_pso)
        }
        // the tall pair: 128-floor stamp + remainder stamp over the same fixtures — the
        // toolchain is proven (tw_pso compiled), so a pair PSO that fails to compile is a RED
        var tl_pso = pipeline_from_source(dev, MetalMoeMulMmQ51TH128_metal_moe_mulmm_q51_t_msl, MetalMoeMulMmQ51TH128_metal_moe_mulmm_q51_t_msl_entry, MetalMoeMulMmQ51TH128_metal_moe_mulmm_q51_t_msl_fastmath, terr)
        var rm_pso = pipeline_from_source(dev, MetalMoeMulMmQ51THR_metal_moe_mulmm_q51_t_msl, MetalMoeMulMmQ51THR_metal_moe_mulmm_q51_t_msl_entry, MetalMoeMulMmQ51THR_metal_moe_mulmm_q51_t_msl_fastmath, terr)
        t |> success(tl_pso != null && rm_pso != null, "moe q51 mulmm tall pair: pipeline: {terr}")
        if (tl_pso != null && rm_pso != null) {
            var maxce = 0
            for (c in cnt) {
                maxce = max(maxce, int(c))
            }
            var by_p = buf_fill(dev, pad * ndim, -700.0)
            let ranp = with_compute_encoder(queue, terr) $(enc : MetalComputeEncoder?) {
                metal_set_pipeline(enc, tl_pso)
                metal_set_threadgroup_memory_length(enc, MetalMoeMulMmQ51TH128_metal_moe_mulmm_q51_t_msl_tgmem, 0)
                metal_set_buffer(enc, bs, 0ul, 0)
                metal_set_buffer(enc, bw, 0ul, 1)
                metal_set_buffer(enc, bxh, 0ul, 3)
                metal_set_buffer(enc, by_p, 0ul, 4)
                metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
                metal_set_buffer(enc, bcnt, 0ul, 6)
                metal_set_buffer(enc, bbase, 0ul, 7)
                metal_dispatch_threadgroups(enc, uint3(uint((maxce + 127) / 128), uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
                metal_set_pipeline(enc, rm_pso)
                metal_set_threadgroup_memory_length(enc, MetalMoeMulMmQ51THR_metal_moe_mulmm_q51_t_msl_tgmem, 0)
                metal_set_buffer(enc, bs, 0ul, 0)
                metal_set_buffer(enc, bw, 0ul, 1)
                metal_set_buffer(enc, bxh, 0ul, 3)
                metal_set_buffer(enc, by_p, 0ul, 4)
                metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 5)
                metal_set_buffer(enc, bcnt, 0ul, 6)
                metal_set_buffer(enc, bbase, 0ul, 7)
                metal_dispatch_threadgroups(enc, uint3(4u, uint(ndim / 64), uint(ne)), uint3(128u, 1u, 1u))
            }
            t |> success(ranp, "moe q51 mulmm tall pair: encode: {terr}")
            if (ranp) {
                t |> equal(buf_mismatch_exact(by_p, want), 0)
            }
            metal_release(by_p)
        }
        if (tl_pso != null) {
            metal_release(tl_pso)
        }
        if (rm_pso != null) {
            metal_release(rm_pso)
        }
        metal_release(bxh)
        metal_release(by_t)
        metal_release(tw_pso)
        delete xh
    }
    metal_release(bw)
    metal_release(bs)
    metal_release(bx)
    metal_release(by)
    metal_release(bcnt)
    metal_release(bbase)
    metal_release(bbkt)
    metal_release(pso)
    delete qp
    delete sp
    delete wref
    delete cnt
    delete basep
    delete xv
    delete want
}

// The weighted top-k reduce on exact data (integer dn, pow2 weights), BIT-exact.
def private moe_combine_gate(t : T?; dev, queue; dim, k : int) {
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_combine_msl, metal_moe_combine_msl_entry, metal_moe_combine_msl_fastmath, err)
    t |> success(pso != null, "moe combine pipeline: {err}")
    if (pso == null) {
        return
    }
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    var dn : array<float>
    dn |> resize(k * dim)
    for (i in range(k * dim)) {
        dn[i] = float((i * 11 + 2) % 17 - 8)
    }
    var selw : array<float>
    selw |> reserve(k)
    for (j in range(k)) {
        selw |> push(lut[j % 4])
    }
    var want : array<float>
    want |> resize(dim)
    for (i in range(dim)) {
        var acc = 0.0
        for (j in range(k)) {
            acc += selw[j] * dn[j * dim + i]
        }
        want[i] = acc
    }
    // the sel buffer's layout: [k idx u32][k w f32] — the combine binds the float half at byte k*4
    var bsel = metal_new_buffer(dev, uint64(2 * k * 4))
    unsafe {
        var pw = reinterpret<float?>(metal_buffer_contents(bsel))
        for (j in range(k)) {
            pw[k + j] = selw[j]
        }
    }
    var bdn = buf_upload(dev, dn)
    var by = buf_fill(dev, dim, -1000.0)
    var bdim = buf_u32(dev, uint(dim))
    var bnk = buf_u32(dev, uint(k))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bdn, 0ul, 0)
        metal_set_buffer(enc, bsel, 0ul, 1)   // the float w view — stream row layout in-kernel
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bdim, 0ul, 3)
        metal_set_buffer(enc, bnk, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint((dim + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "moe combine dispatch dim={dim} k={k}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bdn)
    metal_release(bsel)
    metal_release(by)
    metal_release(bdim)
    metal_release(bnk)
    metal_release(pso)
    delete dn
    delete selw
    delete want
}

// ===== deltanet kernel gates (Wave D) =====

// The whole recurrent chain vs a sequential CPU mirror — conv+SiLU (history straddle), q/k L2
// norm + q pre-scale, the fused sequential delta rule, the z-gated out-norm — compared at each
// stage's seam so a red localizes to (stage). ds pinned 128 (the serve gate's shape).
def private dn_chain_gate(t : T?; dev, queue; nkh, nvh, dconv, npos : int) {
    let ds = 128
    let kd = nkh * ds
    let di = nvh * ds
    let cd = 2 * kd + di
    let taps = dconv - 1
    let qscale = 1.0 / sqrt(float(ds))
    var err : string
    var pso_conv = pipeline_from_source(dev, metal_dn_conv_msl, metal_dn_conv_msl_entry, metal_dn_conv_msl_fastmath, err)
    t |> success(pso_conv != null, "dn conv pipeline: {err}")
    var pso_hist = pipeline_from_source(dev, metal_dn_conv_hist_msl, metal_dn_conv_hist_msl_entry, metal_dn_conv_hist_msl_fastmath, err)
    t |> success(pso_hist != null, "dn hist pipeline: {err}")
    var pso_l2 = pipeline_from_source(dev, metal_dn_l2norm_msl, metal_dn_l2norm_msl_entry, metal_dn_l2norm_msl_fastmath, err)
    t |> success(pso_l2 != null, "dn l2norm pipeline: {err}")
    var pso_scan = pipeline_from_source(dev, metal_dn_scan_msl, metal_dn_scan_msl_entry, metal_dn_scan_msl_fastmath, err)
    t |> success(pso_scan != null, "dn scan pipeline: {err}")
    var pso_gate = pipeline_from_source(dev, metal_dn_gate_msl, metal_dn_gate_msl_entry, metal_dn_gate_msl_fastmath, err)
    t |> success(pso_gate != null, "dn gate pipeline: {err}")
    if (pso_conv == null || pso_hist == null || pso_l2 == null || pso_scan == null || pso_gate == null) {
        return
    }
    // inputs — hash patterns; ssm_a strictly negative (= -exp(A_log)), nonzero history + state
    var xq : array<float>
    var hist : array<float>
    var cw : array<float>
    var braw : array<float>
    var graw : array<float>
    var aa : array<float>
    var dtb : array<float>
    var st0 : array<float>
    var zz : array<float>
    var wn : array<float>
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
    var cv : array<float>
    var hwant : array<float>
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
    var hmid_want : array<float>
    if (npos >= 2) {
        hmid_want |> resize(cd * taps)
        for (ch in range(cd)) {
            for (k in range(taps)) {
                let src = npos - 1 - taps + k
                hmid_want[ch * taps + k] = src >= 0 ? xq[src * cd + ch] : hist[ch * taps + src + taps]
            }
        }
    }
    var bx = buf_upload(dev, xq)
    var bh = buf_upload(dev, hist)
    var bw = buf_upload(dev, cw)
    var by = buf_fill(dev, npos * cd, 0.0)
    // ONE DnArgs serves all five stages (each ignores the fields it does not read);
    // mid = npos >= 2 arms the dual-store shadow writes in hist and scan
    var ka = DnArgs(cd = uint(cd), dconv = uint(dconv), npos = uint(npos), kd = uint(kd),
        ds = uint(ds), nvh = uint(nvh), nkh = uint(nkh), di = uint(di), eps = EPS,
        qscale = qscale, mid = npos >= 2 ? 1u : 0u)
    let cran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_conv)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bh, 0ul, 1)
        metal_set_buffer(enc, bw, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_dispatch_threadgroups(enc, uint3(uint((cd + 255) / 256), uint(npos), 1u), uint3(256u, 1u, 1u))
    }
    t |> success(cran, "dn conv dispatch npos={npos}: {err}")
    t |> equal(buf_mismatch_approx(by, cv), 0)
    var bhm = npos >= 2 ? buf_fill(dev, cd * taps, -1000.0) : null
    let hran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_hist)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bh, 0ul, 1)
        metal_set_buffer(enc, npos >= 2 ? bhm : bh, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint((cd + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
    }
    t |> success(hran, "dn hist dispatch npos={npos}: {err}")
    t |> equal(buf_mismatch_approx(bh, hwant), 0)
    if (npos >= 2) {
        t |> equal(buf_mismatch_approx(bhm, hmid_want), 0)
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
    let lran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_l2)
        metal_set_buffer(enc, by, 0ul, 0)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 1)
        metal_dispatch_threadgroups(enc, uint3(uint((2 * nkh + 7) / 8), uint(npos), 1u), uint3(256u, 1u, 1u))
    }
    t |> success(lran, "dn l2norm dispatch npos={npos}: {err}")
    t |> equal(buf_mismatch_approx(by, cv), 0)
    // stage 3 mirror: the sequential delta rule per v-head (k-heads fan via h % nkh)
    var owant : array<float>
    var stw : array<float>
    var stmid : array<float>
    var skd : array<float>
    owant |> resize(npos * di)
    stw := st0
    if (npos >= 2) {
        stmid |> resize(nvh * ds * ds)
    }
    skd |> resize(ds)
    for (h in range(nvh)) {
        let kh = h % nkh
        let sb = h * ds * ds
        for (p in range(npos)) {
            let gr = graw[p * nvh + h] + dtb[h]
            let dec = exp(aa[h] * (gr > 20.0 ? gr : log(1.0 + exp(gr))))
            let beta = 1.0 / (1.0 + exp(-braw[p * nvh + h]))
            let qb = p * cd + kh * ds
            let kb = p * cd + kd + kh * ds
            let vb = p * cd + 2 * kd + h * ds
            for (j in range(ds)) {
                skd[j] = 0.0
            }
            for (i in range(ds)) {
                let ki = cv[kb + i]
                for (j in range(ds)) {
                    let sv = stw[sb + i * ds + j] * dec
                    stw[sb + i * ds + j] = sv
                    skd[j] += ki * sv
                }
            }
            for (j in range(ds)) {
                skd[j] = (cv[vb + j] - skd[j]) * beta
                owant[p * di + h * ds + j] = 0.0
            }
            for (i in range(ds)) {
                let ki = cv[kb + i]
                let qi = cv[qb + i]
                for (j in range(ds)) {
                    let sv = stw[sb + i * ds + j] + ki * skd[j]
                    stw[sb + i * ds + j] = sv
                    owant[p * di + h * ds + j] += qi * sv
                }
            }
            if (npos >= 2 && p == npos - 2) {   // the dual-store point: state after token npos-2
                for (k in range(ds * ds)) {
                    stmid[sb + k] = stw[sb + k]
                }
            }
        }
    }
    var bst = buf_upload(dev, st0)
    var bbr = buf_upload(dev, braw)
    var bgr = buf_upload(dev, graw)
    var baa = buf_upload(dev, aa)
    var bdt = buf_upload(dev, dtb)
    var bo = buf_fill(dev, npos * di, 0.0)
    var bstm = npos >= 2 ? buf_fill(dev, nvh * ds * ds, -1000.0) : null
    let sran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_scan)
        metal_set_buffer(enc, by, 0ul, 0)
        metal_set_buffer(enc, bst, 0ul, 1)
        metal_set_buffer(enc, bbr, 0ul, 2)
        metal_set_buffer(enc, bgr, 0ul, 3)
        metal_set_buffer(enc, baa, 0ul, 4)
        metal_set_buffer(enc, bdt, 0ul, 5)
        metal_set_buffer(enc, bo, 0ul, 6)
        metal_set_buffer(enc, npos >= 2 ? bstm : bst, 0ul, 7)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 8)
        metal_dispatch_threadgroups(enc, uint3(uint(ds / 4), uint(nvh), 1u), uint3(128u, 1u, 1u))
    }
    t |> success(sran, "dn scan dispatch npos={npos} nkh={nkh} nvh={nvh}: {err}")
    t |> equal(buf_mismatch_approx(bo, owant), 0)
    t |> equal(buf_mismatch_approx(bst, stw), 0)
    if (npos >= 2) {
        t |> equal(buf_mismatch_approx(bstm, stmid), 0)
    }
    // stage 4 mirror: per-head RMS with the shared weight, ⊙ SiLU(z)
    var gwant : array<float>
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
    var bz = buf_upload(dev, zz)
    var bwn = buf_upload(dev, wn)
    let gran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_gate)
        metal_set_buffer(enc, bo, 0ul, 0)
        metal_set_buffer(enc, bz, 0ul, 1)
        metal_set_buffer(enc, bwn, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint((nvh + 7) / 8), uint(npos), 1u), uint3(256u, 1u, 1u))
    }
    t |> success(gran, "dn gate dispatch npos={npos}: {err}")
    t |> equal(buf_mismatch_approx(bo, gwant), 0)
    metal_release(bx)
    metal_release(bh)
    metal_release(bw)
    metal_release(by)
    metal_release(bst)
    metal_release(bbr)
    metal_release(bgr)
    metal_release(baa)
    metal_release(bdt)
    metal_release(bo)
    metal_release(bz)
    metal_release(bwn)
    if (bhm != null) {
        metal_release(bhm)
    }
    if (bstm != null) {
        metal_release(bstm)
    }
    metal_release(pso_conv)
    metal_release(pso_hist)
    metal_release(pso_l2)
    metal_release(pso_scan)
    metal_release(pso_gate)
    delete xq
    delete hist
    delete cw
    delete braw
    delete graw
    delete aa
    delete dtb
    delete st0
    delete zz
    delete wn
    delete cv
    delete hwant
    delete hmid_want
    delete owant
    delete stw
    delete stmid
    delete skd
    delete gwant
}

// The 2x-wide [q|gate] deinterleave + the σ(g) out-gate — trivial mirrors, bit-exact.
def private dn_deint_sigmul_gate(t : T?; dev, queue; nh, hs, npos : int) {
    let qd = nh * hs
    var err : string
    var pso_de = pipeline_from_source(dev, metal_dn_deint_msl, metal_dn_deint_msl_entry, metal_dn_deint_msl_fastmath, err)
    t |> success(pso_de != null, "dn deint pipeline: {err}")
    var pso_sg = pipeline_from_source(dev, MetalSigmul_metal_ew2_msl, MetalSigmul_metal_ew2_msl_entry, MetalSigmul_metal_ew2_msl_fastmath, err)
    t |> success(pso_sg != null, "sigmul pipeline: {err}")
    if (pso_de == null || pso_sg == null) {
        return
    }
    let qs = qd + 64          // a wider destination row stride (the qkv panel form)
    var qg : array<float>
    var qwant : array<float>
    var gwant : array<float>
    qg |> resize(npos * 2 * qd)
    qwant |> resize(npos * qs)
    gwant |> resize(npos * qd)
    for (k in range(npos * 2 * qd)) {
        qg[k] = 0.09 * float(k % 43) - 1.7
    }
    for (p in range(npos)) {
        for (i in range(qd)) {
            let h = i / hs
            let ii = i % hs
            qwant[p * qs + i] = qg[p * 2 * qd + h * 2 * hs + ii]
            gwant[p * qd + i] = qg[p * 2 * qd + h * 2 * hs + hs + ii]
        }
    }
    var bqg = buf_upload(dev, qg)
    var bq = buf_fill(dev, npos * qs, 0.0)
    var bgt = buf_fill(dev, npos * qd, 0.0)
    var ka = DnDeintArgs(hs = uint(hs), qd = uint(qd), qs = uint(qs))
    let dran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_de)
        metal_set_buffer(enc, bqg, 0ul, 0)
        metal_set_buffer(enc, bq, 0ul, 1)
        metal_set_buffer(enc, bgt, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint((qd + 255) / 256), uint(npos), 1u), uint3(256u, 1u, 1u))
    }
    t |> success(dran, "dn deint dispatch: {err}")
    // the pad tail of each destination row is unwritten — compare only the written columns
    var qgot <- buf_download(bq, npos * qs, type<float>)
    var dbad = 0
    for (p in range(npos)) {
        for (i in range(qd)) {
            if (qgot[p * qs + i] != qwant[p * qs + i]) {
                dbad++
            }
        }
    }
    t |> equal(dbad, 0)
    t |> equal(buf_mismatch_exact(bgt, gwant), 0)
    // sigmul: x ⊙ σ(g) on the deinterleaved gate rows
    var xv : array<float>
    var swant : array<float>
    xv |> resize(npos * qd)
    swant |> resize(npos * qd)
    for (k in range(npos * qd)) {
        xv[k] = 0.06 * float(k % 39) - 1.1
        swant[k] = xv[k] / (1.0 + exp(-gwant[k]))
    }
    var sgot <- ew2_gate(t, dev, queue, "sigmul", pso_sg, xv, gwant, swant, false)   // the gate rows the deint compare held exact
    delete sgot
    metal_release(bqg)
    metal_release(bq)
    metal_release(bgt)
    metal_release(pso_de)
    metal_release(pso_sg)
    delete qg
    delete qwant
    delete gwant
    delete qgot
    delete xv
    delete swant
}

def private axpy_sig_gate(t : T?; dev, queue; npos, dim : int) {
    var err : string
    var pso = pipeline_from_source(dev, metal_axpy_sig_msl, metal_axpy_sig_msl_entry, metal_axpy_sig_msl_fastmath, err)
    t |> success(pso != null, "axpy_sig pipeline: {err}")
    if (pso == null) {
        return
    }
    let total = npos * dim
    var yv : array<float>
    var xv : array<float>
    var gv : array<float>
    var want : array<float>
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
    var by = buf_upload(dev, yv)
    var bx = buf_upload(dev, xv)
    var bg = buf_upload(dev, gv)
    var btot = buf_u32(dev, uint(total))
    var bdim = buf_u32(dev, uint(dim))
    var bufs <- [by, bx, bg, btot, bdim]
    let ran = run_compute_1d(queue, pso, bufs, uint(total), 256u, err)
    t |> success(ran, "axpy_sig dispatch: {err}")
    t |> equal(buf_mismatch_approx(by, want), 0)
    bufs |> clear()
    unsafe {
        delete bufs
    }
    metal_release(by)
    metal_release(bx)
    metal_release(bg)
    metal_release(btot)
    metal_release(bdim)
    metal_release(pso)
    delete yv
    delete xv
    delete gv
    delete want
}

// ===== local GPU plumbing (generic bodies instantiate only inside the static_if above) =====

def private buf_mismatch_exact(buf; want : array<auto(TT)>) : int {
    let p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        if (unsafe(p[k]) != want[k]) {
            bad++
        }
    }
    return bad
}

def private buf_mismatch_tol(buf; want : array<float>; tol : float) : int {
    let p = unsafe(reinterpret<float const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        let g = unsafe(p[k])
        if (abs(g - want[k]) > tol * max(1.0, max(abs(g), abs(want[k])))) {
            bad++
        }
    }
    return bad
}

def private buf_mismatch_approx(buf; want : array<float>) : int {
    let p = unsafe(reinterpret<float const?>(metal_buffer_contents(buf)))
    var bad = 0
    for (k in range(length(want))) {
        if (!fapprox(unsafe(p[k]), want[k])) {
            bad++
        }
    }
    return bad
}
