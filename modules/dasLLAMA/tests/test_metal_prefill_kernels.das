options gen2
options stack = 524288   // every dasLLAMA program root takes this budget
options _function_length = 0        // per-kernel gates: hand-bound buffers/uniforms + dispatch + oracle compare, one flat run each
options _cyclomatic_complexity = 0  // same — the arm ladders follow the kernel's own binding/variant axes
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/math_bits           // float<->uint bits — the moe_wscale sel-buffer pun
require dasllama/dasllama_math
require dasllama/dasllama_convert  // used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require ?das_metal dasllama/dasllama_metal_prefill  // the Phase-6 prefill kernel set (Apple builds)
require ?das_metal dasllama/dasllama_metal_kernels  // the Wave C MoE routing kernel set (Apple builds)
require ?das_metal metal/das_metal_boost  // device/pipeline/buffer plumbing + the leak gate
require ?das_metal _metal_kernel_common   // shared buf helpers + fixtures; only the tag-less mismatch compares stay local
require llvm/daslib/f16_cvt  // f32_to_f16: the q51 gates hand-pack f16 d/m scale halfwords
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

def private approx(x, y : float) : bool {
    return abs(x - y) <= 1e-4 * max(1.0, max(abs(x), abs(y)))
}

// The tiled sgmat attention trio (QK^T -> softmax -> P.V) vs a direct causal-GQA oracle at one
// (head_size, heads, kv_heads, npos) shape. Buffers carry np32 rows like the driver's (whole-tile
// sgmat stores + staging reads walk the padded rows); rows >= npos are pad. GENERIC (untyped
// dev/queue) so it instantiates only inside the Apple static_if.
def private attn_trio_gate(t : T?; dev, queue; head_size, n_heads, n_kv_heads, npos : int; mm : bool) {
    let kv_mul = n_heads / n_kv_heads
    let qd = n_heads * head_size
    let kv_dim = n_kv_heads * head_size
    let np32 = (npos + 31) / 32 * 32
    var err : string
    var pso_sc = (mm
        ? pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, err))
    t |> success(pso_sc != null, "qk pipeline (mm={mm}): {err}")
    var pso_sm = pipeline_from_source(dev, metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, err)
    t |> success(pso_sm != null, "softmax pipeline: {err}")
    var pso_av = (mm
        ? pipeline_from_source(dev, metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, err))
    t |> success(pso_av != null, "av pipeline (mm={mm}): {err}")
    let scale = 1.0 / sqrt(float(head_size))
    var aq : array<float>
    var ak : array<float>
    var av : array<float>
    var axb_want : array<float>
    aq |> resize(np32 * qd)
    ak |> resize(np32 * kv_dim)
    av |> resize(np32 * kv_dim)
    axb_want |> resize(npos * qd)
    for (k in range(npos * qd)) {
        aq[k] = 0.13 * float(k % 31) - 1.9
    }
    for (k in range(npos * kv_dim)) {
        ak[k] = 0.17 * float(k % 23) - 1.8
        av[k] = 0.09 * float(k % 37) - 1.6
    }
    var prow : array<float>
    prow |> resize(npos)
    for (h in range(n_heads)) {
        let kvhoff = (h / kv_mul) * head_size
        for (qi in range(npos)) {
            let cnt = qi + 1
            var m = -1.0e30
            for (j in range(cnt)) {
                var dot = 0.0
                for (d in range(head_size)) {
                    dot += aq[qi * qd + h * head_size + d] * ak[j * kv_dim + kvhoff + d]
                }
                prow[j] = dot * scale
                m = max(m, prow[j])
            }
            var sum = 0.0
            for (j in range(cnt)) {
                prow[j] = exp(prow[j] - m)
                sum += prow[j]
            }
            for (d in range(head_size)) {
                var acc = 0.0
                for (j in range(cnt)) {
                    acc += (prow[j] / sum) * av[j * kv_dim + kvhoff + d]
                }
                axb_want[qi * qd + h * head_size + d] = acc
            }
        }
    }
    var baq = buf_upload(dev, aq)
    var bak = buf_upload(dev, ak)
    var bav = buf_upload(dev, av)
    var batt = buf_fill(dev, n_heads * np32 * np32, 0.0)
    var bxb = buf_fill(dev, np32 * qd, 0.0)
    // ONE AttnArgs serves all three stages (each ignores the fields it does not read):
    // qoff = 0 / qrows = np32 (fresh start), window/softcap/hass at their llama-shape identities
    var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kv_dim), head_size = uint(head_size),
        kv_mul = uint(kv_mul), npos = uint(npos), np32 = uint(np32), scale = scale,
        qoff = 0u, qrows = uint(np32), window = 0u, softcap = 0.0, hass = 0u)
    var bcap0 = buf_f32(dev, 0.0)   // stands in for the sink buffer slot (hass = 0: never read)
    let aran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_sc)
        metal_set_threadgroup_memory_length(enc, mm ? metal_attn_qk_mm_msl_tgmem : metal_attn_qk_msl_tgmem, 0)
        metal_set_buffer(enc, baq, 0ul, 0)
        metal_set_buffer(enc, bak, 0ul, 1)
        metal_set_buffer(enc, batt, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(np32 / 32), uint(np32 / (mm ? 64 : 32)), uint(n_heads)), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_sm)
        metal_set_threadgroup_memory_length(enc, metal_attn_softmax_msl_tgmem, 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 1)
        metal_set_buffer(enc, bcap0, 0ul, 20)   // sink identity (hass = 0)
        metal_dispatch_threadgroups(enc, uint3(uint(npos), uint(n_heads), 1u), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_av)
        metal_set_threadgroup_memory_length(enc, mm ? metal_attn_av_mm_msl_tgmem : metal_attn_av_msl_tgmem, 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_buffer(enc, bav, 0ul, 1)
        metal_set_buffer(enc, bxb, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(np32 / 32), uint(head_size / (mm ? 64 : 32)), uint(n_heads)), uint3(128u, 1u, 1u))
    }
    t |> success(aran, "attention dispatches (hs={head_size}, mm={mm}): {err}")
    // the mm pair stages HALF (llama.cpp's f32_f32 mul_mm does the same) — the envelope is the
    // half-precision score shift through softmax, ~1% of the convex-combination output
    t |> equal(buf_mismatch_tol(bxb, axb_want, mm ? 2e-2 : 1e-4), 0)
    metal_release(baq)
    metal_release(bak)
    metal_release(bav)
    metal_release(batt)
    metal_release(bxb)
    metal_release(bcap0)
    metal_release(pso_sc)
    metal_release(pso_sm)
    metal_release(pso_av)
    delete aq
    delete ak
    delete av
    delete axb_want
    delete prow
}

// The trio under a uniform span (media rows): every query attends [window floor, uend) with
// uend = qoff + npos — the eval_embd non-causal contract. The continuation shape (qoff > 0) and
// a sliding layer (window > 0) exercise the two bounds independently: the upper bound is
// uniform, the window floor stays query-based (a floor derived from uend would flatten it).
def private attn_trio_span_gate(t : T?; dev, queue; head_size, n_heads, n_kv_heads, npos, qoff, window : int; mm : bool) {
    let kv_mul = n_heads / n_kv_heads
    let qd = n_heads * head_size
    let kv_dim = n_kv_heads * head_size
    let nk = qoff + npos                    // key rows: existing context + the span chunk
    let nkp = (nk + 63) / 64 * 64           // 64-padded key extent (the driver's nk64)
    let qp = (npos + 31) / 32 * 32          // padded query rows (continuation: != nkp)
    var err : string
    var pso_sc = (mm
        ? pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, err))
    t |> success(pso_sc != null, "span qk pipeline (mm={mm}): {err}")
    var pso_sm = pipeline_from_source(dev, metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, err)
    t |> success(pso_sm != null, "span softmax pipeline: {err}")
    var pso_av = (mm
        ? pipeline_from_source(dev, metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, err)
        : pipeline_from_source(dev, metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, err))
    t |> success(pso_av != null, "span av pipeline (mm={mm}): {err}")
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
        aq[k] = 0.11 * float(k % 29) - 1.7
    }
    for (k in range(nk * kv_dim)) {
        ak[k] = 0.15 * float(k % 27) - 1.9
        av[k] = 0.07 * float(k % 41) - 1.4
    }
    var prow : array<float>
    prow |> resize(nk)
    for (h in range(n_heads)) {
        let kvhoff = (h / kv_mul) * head_size
        for (qi in range(npos)) {
            let ap = qoff + qi
            let jlo = window > 0 && ap + 1 > window ? ap + 1 - window : 0
            var m = -1.0e30
            for (j in range(jlo, nk)) {
                var dot = 0.0
                for (d in range(head_size)) {
                    dot += aq[qi * qd + h * head_size + d] * ak[j * kv_dim + kvhoff + d]
                }
                prow[j] = dot * scale
                m = max(m, prow[j])
            }
            var sum = 0.0
            for (j in range(jlo, nk)) {
                prow[j] = exp(prow[j] - m)
                sum += prow[j]
            }
            for (d in range(head_size)) {
                var acc = 0.0
                for (j in range(jlo, nk)) {
                    acc += (prow[j] / sum) * av[j * kv_dim + kvhoff + d]
                }
                axb_want[qi * qd + h * head_size + d] = acc
            }
        }
    }
    var baq = buf_upload(dev, aq)
    var bak = buf_upload(dev, ak)
    var bav = buf_upload(dev, av)
    var batt = buf_fill(dev, n_heads * qp * nkp, 0.0)
    var bxb = buf_fill(dev, qp * qd, 0.0)
    var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kv_dim), head_size = uint(head_size),
        kv_mul = uint(kv_mul), npos = uint(nk), np32 = uint(nkp), scale = scale,
        qoff = uint(qoff), qrows = uint(qp), window = uint(window), softcap = 0.0, hass = 0u,
        uend = uint(nk))
    var bcap0 = buf_f32(dev, 0.0)   // stands in for the sink buffer slot (hass = 0: never read)
    let aran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_sc)
        metal_set_threadgroup_memory_length(enc, mm ? metal_attn_qk_mm_msl_tgmem : metal_attn_qk_msl_tgmem, 0)
        metal_set_buffer(enc, baq, 0ul, 0)
        metal_set_buffer(enc, bak, 0ul, 1)
        metal_set_buffer(enc, batt, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(qp / 32), uint(nkp / (mm ? 64 : 32)), uint(n_heads)), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_sm)
        metal_set_threadgroup_memory_length(enc, metal_attn_softmax_msl_tgmem, 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 1)
        metal_set_buffer(enc, bcap0, 0ul, 20)   // sink identity (hass = 0)
        metal_dispatch_threadgroups(enc, uint3(uint(npos), uint(n_heads), 1u), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_av)
        metal_set_threadgroup_memory_length(enc, mm ? metal_attn_av_mm_msl_tgmem : metal_attn_av_msl_tgmem, 0)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_buffer(enc, bav, 0ul, 1)
        metal_set_buffer(enc, bxb, 0ul, 2)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 3)
        metal_dispatch_threadgroups(enc, uint3(uint(qp / 32), uint(head_size / (mm ? 64 : 32)), uint(n_heads)), uint3(128u, 1u, 1u))
    }
    t |> success(aran, "span attention dispatches (hs={head_size}, qoff={qoff}, w={window}, mm={mm}): {err}")
    t |> equal(buf_mismatch_tol(bxb, axb_want, mm ? 2e-2 : 1e-4), 0)
    metal_release(baq)
    metal_release(bak)
    metal_release(bav)
    metal_release(batt)
    metal_release(bxb)
    metal_release(bcap0)
    metal_release(pso_sc)
    metal_release(pso_sm)
    metal_release(pso_av)
    delete aq
    delete ak
    delete av
    delete axb_want
    delete prow
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
                var pso_rms = pipeline_from_source(dev, metal_rmsnorm_msl, metal_rmsnorm_msl_entry, metal_rmsnorm_msl_fastmath, err)
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
                    metal_set_threadgroup_memory_length(enc, metal_rmsnorm_msl_tgmem, 0)
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

                // ===== rope (NORM pairs, table-driven) — the q-width dispatch =====
                var pso_rope = pipeline_from_source(dev, metal_rope_msl, metal_rope_msl_entry, metal_rope_msl_fastmath, err)
                t |> success(pso_rope != null, "rope pipeline: {err}")
                let half = HEAD_SIZE / 2
                var rv : array<float>
                var rcos : array<float>
                var rsin : array<float>
                var rvwant : array<float>
                rv |> resize(NPOS * QD)
                rvwant |> resize(NPOS * QD)
                rcos |> resize(NPOS * half)
                rsin |> resize(NPOS * half)
                for (k in range(NPOS * QD)) {
                    rv[k] = 0.11 * float(k % 41) - 2.2
                    rvwant[k] = rv[k]
                }
                for (k in range(NPOS * half)) {
                    rcos[k] = cos(0.01 * float(k % 97))
                    rsin[k] = sin(0.01 * float(k % 97))
                }
                unsafe {
                    for (p in range(NPOS)) {
                        rope_scaled_tab(addr(rvwant[p * QD]), int64(HEAD_SIZE), int64(QD),
                            addr<float const?>(rcos[p * half]), addr<float const?>(rsin[p * half]))
                    }
                }
                var brv = buf_upload(dev, rv)
                var brc = buf_upload(dev, rcos)
                var brs = buf_upload(dev, rsin)
                var brn = buf_u32(dev, uint(QD))
                var brhs = buf_u32(dev, uint(HEAD_SIZE))
                var brnp = buf_u32(dev, uint(NPOS * QD / 2))
                var brnx = buf_u32(dev, 0u)
                var ropebufs <- [brv, brc, brs, brn, brhs, brnp, brnx, brhs]   // trailing brhs = rot (full rope)
                let roperan = run_compute_1d(queue, pso_rope, ropebufs, uint(NPOS * QD / 2), 64u, err)
                t |> success(roperan, "rope dispatch: {err}")
                t |> equal(buf_mismatch_approx(brv, rvwant), 0)
                ropebufs |> clear()
                unsafe {
                    delete ropebufs
                }
                metal_release(brv)
                metal_release(brnx)

                // ===== the same kernel, NEOX pairing (j, j+half) — vs rope_scaled_neox_tab =====
                for (k in range(NPOS * QD)) {
                    rv[k] = 0.13 * float(k % 37) - 1.9
                    rvwant[k] = rv[k]
                }
                unsafe {
                    for (p in range(NPOS)) {
                        rope_scaled_neox_tab(addr(rvwant[p * QD]), int64(HEAD_SIZE), int64(QD),
                            addr<float const?>(rcos[p * half]), addr<float const?>(rsin[p * half]))
                    }
                }
                var brv2 = buf_upload(dev, rv)
                var brnx1 = buf_u32(dev, 1u)
                var ropebufs2 <- [brv2, brc, brs, brn, brhs, brnp, brnx1, brhs]
                let roperan2 = run_compute_1d(queue, pso_rope, ropebufs2, uint(NPOS * QD / 2), 64u, err)
                t |> success(roperan2, "rope NEOX dispatch: {err}")
                t |> equal(buf_mismatch_approx(brv2, rvwant), 0)
                ropebufs2 |> clear()
                unsafe {
                    delete ropebufs2
                }
                metal_release(brv2)
                metal_release(brnx1)

                // ===== partial NEOX (rot < head_size, Wave D) — vs rope_scaled_neox_tab_part;
                // the table rows are rot/2 wide, dims >= rot pass through untouched
                let prot = HEAD_SIZE / 2
                let phalf = prot / 2
                var rcosp : array<float>
                var rsinp : array<float>
                rcosp |> resize(NPOS * phalf)
                rsinp |> resize(NPOS * phalf)
                for (k in range(NPOS * phalf)) {
                    rcosp[k] = cos(0.02 * float(k % 89))
                    rsinp[k] = sin(0.02 * float(k % 89))
                }
                for (k in range(NPOS * QD)) {
                    rv[k] = 0.17 * float(k % 33) - 2.4
                    rvwant[k] = rv[k]
                }
                unsafe {
                    for (p in range(NPOS)) {
                        rope_scaled_neox_tab_part(addr(rvwant[p * QD]), int64(HEAD_SIZE), int64(prot),
                            int64(QD), addr<float const?>(rcosp[p * phalf]), addr<float const?>(rsinp[p * phalf]))
                    }
                }
                var brv3 = buf_upload(dev, rv)
                var brcp = buf_upload(dev, rcosp)
                var brsp = buf_upload(dev, rsinp)
                var brnx2 = buf_u32(dev, 1u)
                var brot = buf_u32(dev, uint(prot))
                var ropebufs3 <- [brv3, brcp, brsp, brn, brhs, brnp, brnx2, brot]
                let roperan3 = run_compute_1d(queue, pso_rope, ropebufs3, uint(NPOS * QD / 2), 64u, err)
                t |> success(roperan3, "rope partial-NEOX dispatch: {err}")
                t |> equal(buf_mismatch_approx(brv3, rvwant), 0)
                ropebufs3 |> clear()
                unsafe {
                    delete ropebufs3
                }
                delete rcosp
                delete rsinp
                metal_release(brv3)
                metal_release(brcp)
                metal_release(brsp)
                metal_release(brnx2)
                metal_release(brot)
                metal_release(brc)
                metal_release(brs)
                metal_release(brn)
                metal_release(brhs)
                metal_release(brnp)
                metal_release(pso_rope)

                // ===== attention: the tiled sgmat trio vs the direct causal-GQA oracle, at the
                // classic small shape (hs 32 — one QK k-slab) AND the llama-3B head shape
                // (hs 128 — the QK kernel's two-k-slab path; 2 heads keep the oracle cheap)
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, false)
                attn_trio_gate(t, dev, queue, 128, 2, 1, NPOS, false)
                // the ggml-geometry pair (hs % 64 shapes: llama-1B/3B classes + GQA)
                attn_trio_gate(t, dev, queue, 64, 4, 2, NPOS, true)
                attn_trio_gate(t, dev, queue, 128, 6, 2, 40, true)

                // ===== the uniform span (media rows): fresh, continuation, continuation+window =====
                attn_trio_span_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, 0, 0, false)
                attn_trio_span_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, 32, 0, false)
                attn_trio_span_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS, 32, 40, false)
                attn_trio_span_gate(t, dev, queue, 64, 4, 2, NPOS, 0, 0, true)
                attn_trio_span_gate(t, dev, queue, 64, 4, 2, NPOS, 32, 0, true)
                attn_trio_span_gate(t, dev, queue, 64, 4, 2, NPOS, 32, 40, true)

                // ===== swiglu + residual add =====
                var pso_sw = pipeline_from_source(dev, MetalSwiglu_metal_ew2_msl, MetalSwiglu_metal_ew2_msl_entry, MetalSwiglu_metal_ew2_msl_fastmath, err)
                t |> success(pso_sw != null, "swiglu pipeline: {err}")
                var pso_add = pipeline_from_source(dev, MetalAdd_metal_ew2_msl, MetalAdd_metal_ew2_msl_entry, MetalAdd_metal_ew2_msl_fastmath, err)
                t |> success(pso_add != null, "add pipeline: {err}")
                let total = NROWS * HIDDEN
                var sg : array<float>
                var su : array<float>
                var sg_want : array<float>
                sg |> resize(total)
                su |> resize(total)
                sg_want |> resize(total)
                for (k in range(total)) {
                    sg[k] = 0.21 * float(k % 43) - 4.3
                    su[k] = 0.31 * float(k % 17) - 2.5
                }
                for (k in range(total)) {
                    let vv = sg[k]
                    sg_want[k] = (vv / (1.0 + exp(-vv))) * su[k]     // the swiglu ptr-core formula
                }
                var bsg = buf_upload(dev, sg)
                var bsu = buf_upload(dev, su)
                var btot = buf_u32(dev, uint(total))
                var swbufs <- [bsg, bsu, btot]
                let swran = run_compute_1d(queue, pso_sw, swbufs, uint(total), 64u, err)
                t |> success(swran, "swiglu dispatch: {err}")
                t |> equal(buf_mismatch_approx(bsg, sg_want), 0)
                swbufs |> clear()
                unsafe {
                    delete swbufs
                }
                // add: x += y, bit-exact (one fadd on both sides) — seeded from the GPU gate
                // output, not the CPU one: the exp-envelope difference upstream would otherwise
                // masquerade as an add mismatch
                let sg_gpu <- buf_download(bsg, total, type<float>)
                var add_want : array<float>
                add_want |> resize(total)
                for (k in range(total)) {
                    add_want[k] = sg_gpu[k] + su[k]
                }
                var bone_add = buf_f32(dev, 1.0)   // the wave-B ascale slot at its identity (exact: *1.0)
                var addbufs <- [bsg, bsu, btot, bone_add]
                let addran = run_compute_1d(queue, pso_add, addbufs, uint(total), 64u, err)
                t |> success(addran, "add dispatch: {err}")
                t |> equal(buf_mismatch_exact(bsg, add_want), 0)
                addbufs |> clear()
                unsafe {
                    delete addbufs
                }
                // flat copy (the gemma4 V-from-K panel copy rides MetalCopyRow): dst = src, bit-exact
                var pso_cp = pipeline_from_source(dev, metal_copy_row_msl, metal_copy_row_msl_entry, metal_copy_row_msl_fastmath, err)
                t |> success(pso_cp != null, "copy_row pipeline: {err}")
                var bcp = buf_upload(dev, su)   // pre-filled — fully overwritten by the copy
                var cpbufs <- [bsg, bcp, btot]
                let cpran = run_compute_1d(queue, pso_cp, cpbufs, uint(total), 256u, err)
                t |> success(cpran, "copy_row dispatch: {err}")
                t |> equal(buf_mismatch_exact(bcp, add_want), 0)
                cpbufs |> clear()
                unsafe {
                    delete cpbufs
                }
                metal_release(bcp)
                metal_release(pso_cp)
                metal_release(bsg)
                metal_release(bsu)
                metal_release(btot)
                metal_release(bone_add)
                metal_release(pso_sw)
                metal_release(pso_add)

                // ===== the production mul_mm — the resident path's default GEMM =====
                mulmm_gate(t, dev, queue, 96, 128, 64)      // 3 k-blocks, 2 output tiles, 2 token tiles
                mulmm_gate(t, dev, queue, 64, 192, 32)      // 2 k-blocks, 3 output tiles, 1 token tile

                // ===== the classifier GEMV (logits-on-GPU) =====
                // exact-arithmetic data (quants in [-8, 7], pow2 scales, int x): every per-byte4
                // dot, scale product, and simd_sum partial is exactly representable — BIT-exact
                // vs the CPU reference. d = 21 exercises the row < ddim tail guard (grid pads to 24).
                gemv_gate(t, dev, queue, 96, 21)            // 3 blocks per row, guarded tail
                gemv_gate(t, dev, queue, 160, 64)           // 5 blocks per row, exact grid
                gemv_gate(t, dev, queue, 1120, 8)           // 35 blocks: exercises the 4-chunk main loop, not only the tail

                // ===== MoE routing kernels (Wave C) =====
                // select: exact-tie + odd-ne shapes vs the moe_select_core mirror; router /
                // expert-indexed q8 GEMV / combine: exact-arithmetic data, BIT-exact. The kq
                // expert twins share the q8 twin's slot/index math verbatim (dequant bodies are
                // the proven site GEMVs') — their numeric coverage rides the fam-qwen3moe row.
                moe_select_gate(t, dev, queue, 128, 8, 1, true)    // the qwen3moe shape + exact ties
                moe_select_gate(t, dev, queue, 60, 4, 1, false)    // ne not a lane multiple — pad lanes
                moe_select_gate(t, dev, queue, 256, 16, 1, false)  // the serve gate's caps
                moe_select_gate(t, dev, queue, 128, 8, 3, false)   // batch: one stream row per tg
                moe_select_gate(t, dev, queue, 32, 4, 1, false, 1)  // gpt-oss softmax_weight mode
                moe_router_gate(t, dev, queue, 64, 128)
                moe_gemv_gate(t, dev, queue, 96, 24, 5, 3, 1, false)   // shared x (the gate/up sites)
                moe_gemv_gate(t, dev, queue, 64, 32, 7, 4, 1, true)    // per-slot x rows (the down site)
                moe_gemv_gate(t, dev, queue, 64, 24, 5, 3, 3, false)   // batch streams, shared-x form
                moe_gemv_gate(t, dev, queue, 64, 24, 5, 3, 3, true)    // batch streams, down form
                moe_combine_gate(t, dev, queue, 200, 4)

                // ===== q51 expert kernels (the 26B-A4B Q4_K_M down stacks) =====
                // exact-arithmetic q51 planes (pow2 d, quarter m, int x) — BIT-exact both kernels;
                // n = 96 covers 3 blocks/row, the mulmm shapes cover padded CSR tails (ce % 32)
                moe_gemv_q51_gate(t, dev, queue, 96, 24, 5, 3, 1, false)   // gate/up site (shared x)
                moe_gemv_q51_gate(t, dev, queue, 64, 32, 7, 4, 1, true)    // down site (per-slot x)
                moe_gemv_q51_gate(t, dev, queue, 64, 24, 5, 3, 3, true)    // batch streams, down form
                moe_mulmm_q51_gate(t, dev, queue, 64, 64, [32, 40, 64])    // padded tail on expert 1
                moe_mulmm_q51_gate(t, dev, queue, 96, 128, [8, 96])        // 2 col tiles, tiny run + multi-tile

                // ===== mx4 expert GEMV (reworked to the q51 shape — gpt-oss's decode kernel) =====
                moe_gemv_mx4_gate(t, dev, queue, 96, 24, 5, 3, 1, false, false)   // gate/up (shared x)
                moe_gemv_mx4_gate(t, dev, queue, 64, 32, 7, 4, 1, true, true)     // down site + bias rows
                moe_gemv_mx4_gate(t, dev, queue, 64, 24, 5, 3, 3, true, true)     // batch streams, biased down

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
                moe_router_b_gate(t, dev, queue, 64, 7, 13, false)       // ne=7: row guard; np=13: stream clamp
                moe_router_b_gate(t, dev, queue, 64, 7, 13, true)
                moe_csr_gate(t, dev, queue, 13, 5, 2)
                moe_csr_gate(t, dev, queue, 40, 3, 4)                    // an expert crosses one 32-pad tile
                moe_reduce_gate(t, dev, queue, 96, 9, 2)

                // ===== prefill misc (PLE pre-step, bias epilogue, MTP cat, deltanet ba) =====
                ple_gather_q8_gate(t, dev, queue, 11, 3, 32, 5)
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

def private swiglu_oai_ref(g, u : double) : double {
    let x = min(g, 7.0lf)
    let y = clamp(u, -7.0lf, 7.0lf)
    return (x / (1.0lf + exp(-1.702lf * x))) * (y + 1.0lf)
}

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
    var want : array<float>
    want |> resize(total)
    for (i in range(total)) {
        want[i] = float(swiglu_oai_ref(double(g[i]), double(u[i])))
    }
    var bg = buf_upload(dev, g)
    var bu = buf_upload(dev, u)
    var btot = buf_u32(dev, uint(total))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bg, 0ul, 0)
        metal_set_buffer(enc, bu, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
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
    var cnt : array<uint>
    var basep : array<uint>
    cnt |> reserve(ne)
    basep |> reserve(ne)
    var pad = 0
    for (c in counts) {
        cnt |> push(uint(c))
        basep |> push(uint(pad))
        pad += (c + 31) / 32 * 32
    }
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
    var want : array<float>
    want |> resize(total)
    for (i in range(total)) {
        want[i] = i < live ? float(swiglu_oai_ref(double(g[i]), double(u[i]))) : g[i]
    }
    var bg = buf_upload(dev, g)
    var bu = buf_upload(dev, u)
    var btot = buf_u32(dev, uint(total))
    var bcnt = buf_upload(dev, cnt)
    var bbase = buf_upload(dev, basep)
    var bne = buf_u32(dev, uint(ne))
    var bnfe = buf_u32(dev, uint(nfe))
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bg, 0ul, 0)
        metal_set_buffer(enc, bu, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
        metal_set_buffer(enc, bcnt, 0ul, 3)
        metal_set_buffer(enc, bbase, 0ul, 4)
        metal_set_buffer(enc, bne, 0ul, 5)
        metal_set_buffer(enc, bnfe, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint((total / 4 + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
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
        var ss = 0.0lf
        for (c in range(dim)) {
            ss += double(x[r * dim + c]) * double(x[r * dim + c])
        }
        let inv = (1.0lf / sqrt(ss / double(dim) + double(eps))) * double(invd)
        for (c in range(dim)) {
            want[r * dim + c] = float(double(x[r * dim + c]) * inv * double(rs[c]))
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

// the batched prefill router GEMV (MetalMoeRouterB): logits[st][e] = w[e]·x[st] (+ rb[e]),
// 8 streams per tg column, pad streams clamp and their stores gate — BIT-exact fixture
def private moe_router_b_gate(t : T?; dev, queue; dim, ne, npos : int; hasb : bool) {
    let tag = "moe_router_b dim={dim} ne={ne} np={npos}{hasb ? " bias" : ""}"
    var err : string
    var pso = pipeline_from_source(dev, MetalMoeRouterB_metal_moe_router_msl, MetalMoeRouterB_metal_moe_router_msl_entry, MetalMoeRouterB_metal_moe_router_msl_fastmath, err)
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
        metal_set_buffer(enc, bnp, 0ul, 8)
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

// the atomics-free CSR pair (MetalMoeCount -> MetalMoeBucket) chained as production dispatches
// them: cnt, then padded-prefix basep + serial-order bkt/inv — all four outputs checked exactly
// (pad bkt slots stay at the 0xFFFFFFFF sentinel)
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
    var cntw : array<uint>
    var basew : array<uint>
    cntw |> resize(ne)
    basew |> resize(ne)
    for (e in range(ne)) {
        var n = 0u
        for (tt in range(total)) {
            if (int(sel[(tt / nk) * 2 * nk + tt % nk]) == e) {
                n++
            }
        }
        cntw[e] = n
    }
    var pad = 0
    for (e in range(ne)) {
        basew[e] = uint(pad)
        pad += (int(cntw[e]) + 31) / 32 * 32
    }
    var bktw : array<uint>
    var invw : array<uint>
    bktw |> resize(pad)
    invw |> resize(total)
    for (i in range(pad)) {
        bktw[i] = 0xFFFFFFFF
    }
    for (e in range(ne)) {
        var b = int(basew[e])
        for (tt in range(total)) {
            if (int(sel[(tt / nk) * 2 * nk + tt % nk]) == e) {
                bktw[b] = uint(tt)
                invw[tt] = uint(b)
                b++
            }
        }
    }
    var bsel = buf_upload(dev, sel)
    var bcnt = buf_fill(dev, ne, 0xFFFFFFFF)
    var bbase = buf_fill(dev, ne, 0xFFFFFFFF)
    var bbkt = buf_fill(dev, pad, 0xFFFFFFFF)
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
        metal_dispatch_threadgroups(enc, uint3(uint(ne), 1u, 1u), uint3(32u, 1u, 1u))
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

// q8 blob region shared by the ple/dn gates: pow2 scales + small-int quants (the common
// PLE stage 1 (MetalPleGatherQ8): token-row q8 dequant into the layer-major side panel,
// sqrt(ple) folded into the scale
def private ple_gather_q8_gate(t : T?; dev, queue; vocab, nlayers, ple, npos : int) {
    let tag = "ple_gather_q8 v={vocab} nl={nlayers} ple={ple} np={npos}"
    var err : string
    var pso = pipeline_from_source(dev, metal_ple_gather_q8_msl, metal_ple_gather_q8_msl_entry, metal_ple_gather_q8_msl_fastmath, err)
    t |> success(pso != null, "{tag}: pipeline: {err}")
    return if (pso == null)
    let rowlen = nlayers * ple
    var blob : array<uint8>
    var wdeq : array<float>
    q8_blob_fill(1, vocab * rowlen / 32, blob, wdeq)
    var tok : array<uint>
    tok |> resize(npos)
    for (p in range(npos)) {
        tok[p] = uint((p * 7 + 2) % vocab)
    }
    let sc0 = sqrt(float(ple))
    var want : array<float>
    want |> resize(nlayers * npos * ple)
    for (l in range(nlayers)) {
        for (p in range(npos)) {
            for (i in range(ple)) {
                want[(l * npos + p) * ple + i] = wdeq[int(tok[p]) * rowlen + l * ple + i] * sc0
            }
        }
    }
    var ka = PleArgs(npos = uint(npos), ple = uint(ple), nlayers = uint(nlayers), sc0 = sc0,
        inv_sd = 1.0, eps = 1.0e-6, inv_s2 = 1.0)
    var bw = buf_upload(dev, blob)
    var btok = buf_upload(dev, tok)
    var by = buf_fill(dev, nlayers * npos * ple, -1000.0)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   // half-scale view
        metal_set_buffer(enc, bw, 0ul, 1)   // byte view
        metal_set_buffer(enc, btok, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_dispatch_threadgroups(enc, uint3(uint(nlayers), uint(npos), 1u), uint3(uint(ple), 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
    t |> equal(buf_mismatch_approx(by, want), 0)
    metal_release(bw)
    metal_release(btok)
    metal_release(by)
    metal_release(pso)
    delete blob
    delete wdeq
    delete tok
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
    want |> resize(nlayers * npos * ple)
    for (l in range(nlayers)) {
        for (p in range(npos)) {
            var ss = 0.0lf
            for (i in range(ple)) {
                let v = double(proj[p * rowlen + l * ple + i]) * double(inv_sd)
                ss += v * v
            }
            let inv = 1.0lf / sqrt(ss / double(ple) + double(eps))
            for (i in range(ple)) {
                let v = double(proj[p * rowlen + l * ple + i]) * double(inv_sd)
                let yi = (l * npos + p) * ple + i
                want[yi] = float((double(side[yi]) + double(pnorm[i]) * (v * inv)) * double(inv_s2))
            }
        }
    }
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
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bx, 0ul, 0)
        metal_set_buffer(enc, bb, 0ul, 1)
        metal_set_buffer(enc, bn, 0ul, 2)
        metal_set_buffer(enc, btot, 0ul, 3)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
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
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, by, 0ul, 0)
        metal_set_buffer(enc, ba, 0ul, 1)
        metal_set_buffer(enc, bb, 0ul, 2)
        metal_set_buffer(enc, bd, 0ul, 3)
        metal_set_buffer(enc, btot, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "{tag}: encode: {err}")
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
    let nblk = n / 32
    var wq : array<int8>
    var ws : array<float>
    var xv : array<float>
    var want : array<float>
    wq |> resize(d * n)
    ws |> resize(d * nblk)
    xv |> resize(n)
    want |> resize(d)
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    for (i in range(d * n)) {
        wq[i] = int8((i * 7 + i / 31) % 16 - 8)
    }
    for (i in range(d * nblk)) {
        ws[i] = lut[(i / 5) % 4]
    }
    for (i in range(n)) {
        xv[i] = float((i * 3) % 9 - 4)
    }
    for (r in range(d)) {
        var acc = 0.0
        for (k in range(n)) {
            acc += float(wq[r * n + k]) * ws[r * nblk + k / 32] * xv[k]
        }
        want[r] = acc
    }
    // W blob: 34-byte blocks, built straight in the buffer (bound TWICE — half + byte views)
    var bw = metal_new_buffer(dev, uint64(d * nblk * 34))
    unsafe {
        var base = reinterpret<int8?>(metal_buffer_contents(bw))
        for (b in range(d * nblk)) {
            let off = b * 34
            var ph = reinterpret<float16?>(base + off)
            ph[0] = float16(ws[b])
            for (c in range(32)) {
                base[off + 2 + c] = wq[b * 32 + c]
            }
        }
    }
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, d, -1000.0)
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
        metal_dispatch_threadgroups(enc, uint3(uint((d + 3) / 4), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "gemv dispatch n={n} d={d}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bd)
    metal_release(pso)
    delete wq
    delete ws
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
    let nb = k / 32
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    var wq : array<int8>
    var ws : array<float>
    wq |> resize(d * k)
    ws |> resize(d * nb)
    for (i in range(d * k)) {
        wq[i] = int8((i * 7 + i / 31) % 16 - 8)
    }
    for (i in range(d * nb)) {
        ws[i] = lut[(i / 5) % 4]
    }
    // W blob: 34-byte blocks (half scale + 32 quants), built straight in the buffer
    var bw = metal_new_buffer(dev, uint64(d * nb * 34))
    var base = unsafe(reinterpret<int8?>(metal_buffer_contents(bw)))
    for (b in range(d * nb)) {
        let off = b * 34
        unsafe {
            var ph = reinterpret<float16?>(base + off)
            ph[0] = float16(ws[b])
            for (c in range(32)) {
                base[off + 2 + c] = wq[b * 32 + c]
            }
        }
    }
    // X: raw f32 rows, integer-times-pow2 values (exact in half)
    var xf : array<float>
    xf |> resize(m * k)
    for (i in range(m * k)) {
        xf[i] = float((i * 5 + i / 17) % 16 - 8) * lut[(i / 3) % 4]
    }
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
                acc += double(float(wq[j * k + c]) * ws[j * nb + c / 32]) * double(xf[r * k + c])
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
    delete wq
    delete ws
    delete xf
    delete want
}

// ===== MoE routing kernel gates (Wave C) =====

// The select kernel vs an in-test moe_select_core mirror (softmax + top-k + norm_topk renorm),
// one stream row per threadgroup ([k idx u32][k w f32] per row). idx must match EXACTLY — ties
// are exact-equal logits, so both sides agree by the lowest-index rule; w compares within the
// shared fp tolerance (hardware exp vs the CPU's exp).
def private moe_select_gate(t : T?; dev, queue; ne, k, nst : int; ties : bool; gmode : int = 0) {
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_select_msl, metal_moe_select_msl_entry, metal_moe_select_msl_fastmath, err)
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
    var widx : array<uint>
    var ww : array<float>
    widx |> reserve(nst * k)
    ww |> reserve(nst * k)
    for (s in range(nst)) {
        var p : array<float>
        p |> resize(ne)
        for (i in range(ne)) {
            p[i] = lg[s * ne + i]
        }
        if (gmode == 0) {   // softmax over all ne first; mode 1 selects on the raw logits
            var mx = p[0]
            for (i in range(ne)) {
                mx = max(mx, p[i])
            }
            var sum = 0.0
            for (i in range(ne)) {
                p[i] = exp(max(p[i] - mx, -80.0))
                sum += p[i]
            }
            for (i in range(ne)) {
                p[i] /= sum
            }
        }
        var ws = 0.0
        let base = length(ww)
        for (_j in range(k)) {
            var best = 0
            for (i in range(ne)) {
                if (p[i] > p[best]) {
                    best = i
                }
            }
            widx |> push(uint(best))   // reference built winner-by-winner (knockout between)
            ww |> push(p[best])
            ws += p[best]
            p[best] = -FLT_MAX
        }
        if (gmode == 1) {   // gpt-oss softmax_weight: softmax over just the selected k
            var mxw = ww[base]
            for (j in range(k)) {
                mxw = max(mxw, ww[base + j])
            }
            var sw = 0.0
            for (j in range(k)) {
                ww[base + j] = exp(max(ww[base + j] - mxw, -80.0))
                sw += ww[base + j]
            }
            for (j in range(k)) {
                ww[base + j] /= sw
            }
        } else {
            ws = max(ws, 6.103515625e-5)
            for (j in range(k)) {
                ww[base + j] /= ws
            }
        }
        delete p
    }
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
    var badidx = 0
    var badw = 0
    unsafe {
        var pi = reinterpret<uint?>(metal_buffer_contents(bsel))
        var pw = reinterpret<float?>(metal_buffer_contents(bsel))
        for (s in range(nst)) {
            for (j in range(k)) {
                if (pi[s * 2 * k + j] != widx[s * k + j]) {
                    badidx++
                }
                if (!approx(pw[s * 2 * k + k + j], ww[s * k + j])) {
                    badw++
                }
            }
        }
    }
    t |> equal(badidx, 0)
    t |> equal(badw, 0)
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

// The router GEMV on exact f32 data (integer w and x — every fma partial is exact), BIT-exact.
def private moe_router_gate(t : T?; dev, queue; n, ne : int) {
    var err : string
    var pso = pipeline_from_source(dev, MetalMoeRouter_metal_moe_router_msl, MetalMoeRouter_metal_moe_router_msl_entry, MetalMoeRouter_metal_moe_router_msl_fastmath, err)
    t |> success(pso != null, "moe router pipeline: {err}")
    if (pso == null) {
        return
    }
    var w : array<float>
    var xv : array<float>
    var want : array<float>
    w |> resize(ne * n)
    xv |> resize(n)
    want |> resize(ne)
    for (i in range(ne * n)) {
        w[i] = float((i * 7 + 3) % 15 - 7)
    }
    for (i in range(n)) {
        xv[i] = float((i * 5) % 11 - 5)
    }
    for (r in range(ne)) {
        var acc = 0.0
        for (c in range(n)) {
            acc += w[r * n + c] * xv[c]
        }
        want[r] = acc
    }
    var bw = buf_upload(dev, w)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, ne, -1000.0)
    var bn = buf_u32(dev, uint(n))
    var bd = buf_u32(dev, uint(ne))
    var bxs = buf_u32(dev, uint(n))   // per-stream x stride (one stream here)
    var bhb0 = buf_u32(dev, 0u)       // router bias off (identity)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, by, 0ul, 2)
        metal_set_buffer(enc, bn, 0ul, 3)
        metal_set_buffer(enc, bd, 0ul, 4)
        metal_set_buffer(enc, bxs, 0ul, 5)
        metal_set_buffer(enc, bhb0, 0ul, 6)
        metal_set_buffer(enc, bhb0, 0ul, 7)
        // slot 8 (ns) is a HOLE here — @template_gate = BATCHED erases it from the single stamp
        metal_dispatch_threadgroups(enc, uint3(uint((ne + 3) / 4), 1u, 1u), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "moe router dispatch n={n} ne={ne}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bn)
    metal_release(bd)
    metal_release(bxs)
    metal_release(bhb0)
    metal_release(pso)
    delete w
    delete xv
    delete want
}

// The expert-indexed q8 GEMV: a net-expert stack of [d x n] q8 rows (gemv_gate's exact-arithmetic
// data), sel rows pick k expert planes per stream, grid (rows, k slots, nst streams). perslot
// exercises the down site's per-slot x stride; nst > 1 the batch strides (ss/bxs/bys).
def private moe_gemv_gate(t : T?; dev, queue; n, d, net, k, nst : int; perslot : bool) {
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_gemv_q8_msl, metal_moe_gemv_q8_msl_entry, metal_moe_gemv_q8_msl_fastmath, err)
    t |> success(pso != null, "moe gemv pipeline: {err}")
    if (pso == null) {
        return
    }
    let nb = n / 32
    let lut = fixed_array(0.25, 0.5, 1.0, 2.0)
    var wq : array<int8>
    var ws : array<float>
    wq |> resize(net * d * n)
    ws |> resize(net * d * nb)
    for (i in range(net * d * n)) {
        wq[i] = int8((i * 7 + i / 31) % 16 - 8)
    }
    for (i in range(net * d * nb)) {
        ws[i] = lut[(i / 5) % 4]
    }
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
    var want : array<float>
    want |> resize(nst * k * d)
    for (s in range(nst)) {
        for (j in range(k)) {
            let e = int(sel[s * 2 * k + j])
            let x0 = (perslot ? s * k + j : s) * n
            for (r in range(d)) {
                var acc = 0.0
                for (c in range(n)) {
                    acc += float(wq[(e * d + r) * n + c]) * ws[(e * d + r) * nb + c / 32] * xv[x0 + c]
                }
                want[(s * k + j) * d + r] = acc
            }
        }
    }
    // the expert STACK blob: net*d*nb 34-byte blocks, expert-major (mulmm_gate's block layout)
    var bw = metal_new_buffer(dev, uint64(net * d * nb * 34))
    unsafe {
        var base = reinterpret<int8?>(metal_buffer_contents(bw))
        for (b in range(net * d * nb)) {
            let off = b * 34
            var ph = reinterpret<float16?>(base + off)
            ph[0] = float16(ws[b])
            for (c in range(32)) {
                base[off + 2 + c] = wq[b * 32 + c]
            }
        }
    }
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nst * k * d, -1000.0)
    var bsel = buf_upload(dev, sel)
    var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
        xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
        bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   // half-scale view
        metal_set_buffer(enc, bw, 0ul, 1)   // byte view
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_set_buffer(enc, bsel, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 3) / 4), uint(k), uint(nst)), uint3(128u, 1u, 1u))
    }
    t |> success(ran, "moe gemv dispatch n={n} d={d} net={net} k={k} nst={nst}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bx)
    metal_release(by)
    metal_release(bsel)
    metal_release(pso)
    delete wq
    delete ws
    delete sel
    delete xv
    delete want
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

// The expert-indexed q51 GEMV on exact-arithmetic q51 planes — moe_gemv_gate's shapes, BIT-exact.
def private moe_gemv_q51_gate(t : T?; dev, queue; n, d, net, k, nst : int; perslot : bool) {
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_gemv_q51_msl, metal_moe_gemv_q51_msl_entry, metal_moe_gemv_q51_msl_fastmath, err)
    t |> success(pso != null, "moe q51 gemv pipeline: {err}")
    if (pso == null) {
        return
    }
    let nb = n / 32
    var qp : array<uint8>
    var sp : array<uint8>
    var wref : array<float>
    q51_test_planes(net * d, n, qp, sp, wref)
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
    var want : array<float>
    want |> resize(nst * k * d)
    for (s in range(nst)) {
        for (j in range(k)) {
            let e = int(sel[s * 2 * k + j])
            let x0 = (perslot ? s * k + j : s) * n
            for (r in range(d)) {
                var acc = 0.0
                for (c in range(n)) {
                    acc += wref[((e * d + r) * nb) * 32 + c] * xv[x0 + c]
                }
                want[(s * k + j) * d + r] = acc
            }
        }
    }
    var bw = buf_upload(dev, qp)
    var bs = buf_upload(dev, sp)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nst * k * d, -1000.0)
    var bsel = buf_upload(dev, sel)
    var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
        xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
        bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = 0u)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bs, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_set_buffer(enc, bsel, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 3) / 4), uint(k), uint(nst)), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "moe q51 gemv dispatch n={n} d={d} net={net} k={k} nst={nst}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bs)
    metal_release(bx)
    metal_release(by)
    metal_release(bsel)
    metal_release(pso)
    delete qp
    delete sp
    delete wref
    delete sel
    delete xv
    delete want
}

// The expert-indexed mx4 GEMV on exact-arithmetic planes: nibble values from the signed
// doubled-e2m1 table, pow2 E8M0 scales, int x — BIT-exact (covers the bias arm too).
def private moe_gemv_mx4_gate(t : T?; dev, queue; n, d, net, k, nst : int; perslot, bias : bool) {
    var err : string
    var pso = pipeline_from_source(dev, metal_moe_gemv_mx4_msl, metal_moe_gemv_mx4_msl_entry, metal_moe_gemv_mx4_msl_fastmath, err)
    t |> success(pso != null, "moe mx4 gemv pipeline: {err}")
    if (pso == null) {
        return
    }
    let nb = n / 32
    let vt = fixed_array(0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 12.0,
                         0.0, -1.0, -2.0, -3.0, -4.0, -6.0, -8.0, -12.0)
    var etab : array<float>
    etab |> resize(256)
    for (ei in range(256)) {
        etab[ei] = unsafe(reinterpret<float>(ei < 2 ? (0x00200000u << uint(ei)) : (uint(ei - 1) << 23u)))
    }
    var qp : array<uint8>
    var sp : array<uint8>
    var wref : array<float>
    qp |> resize(net * d * nb * 16)
    sp |> resize(net * d * nb)
    wref |> resize(net * d * n)
    for (b in range(net * d * nb)) {
        let sc = 120 + (b * 3) % 12   // pow2 scales 2^-8 .. 2^3 — every product dyadic
        sp[b] = uint8(sc)
        for (i in range(16)) {
            let nl = (b * 7 + i * 5 + 1) % 16
            let nh = (b * 11 + i * 3 + 2) % 16
            qp[b * 16 + i] = uint8(nl | (nh << 4))
            wref[b * 32 + i] = vt[nl] * etab[sc]
            wref[b * 32 + 16 + i] = vt[nh] * etab[sc]
        }
    }
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
                want[(s * k + j) * d + r] = acc + (bias ? wb[e * d + r] : 0.0)
            }
        }
    }
    var bw = buf_upload(dev, qp)
    var bs = buf_upload(dev, sp)
    var bx = buf_upload(dev, xv)
    var by = buf_fill(dev, nst * k * d, -1000.0)
    var bsel = buf_upload(dev, sel)
    var bwb = buf_upload(dev, wb)
    var ka = MoeGemvArgs(ndim = uint(n), ddim = uint(d), eplane = uint(d * nb),
        xs = perslot ? uint(n) : 0u, ss = uint(2 * k),
        bxs = uint((perslot ? k : 1) * n), bys = uint(k * d), hasb = bias ? 1u : 0u)
    var vtf : array<float>
    vtf |> resize(16)
    for (i in range(16)) {
        vtf[i] = vt[i]
    }
    var bvt = buf_upload(dev, vtf)
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_threadgroup_memory_length(enc, metal_moe_gemv_mx4_msl_tgmem, 0)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bs, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_bytes(enc, unsafe(addr<void?>(ka)), uint64(typeinfo sizeof(ka)), 4)
        metal_set_buffer(enc, bsel, 0ul, 6)
        metal_set_buffer(enc, bwb, 0ul, 12)
        metal_set_buffer(enc, bvt, 0ul, 15)
        metal_dispatch_threadgroups(enc, uint3(uint((d + 3) / 4), uint(k), uint(nst)), uint3(64u, 1u, 1u))
    }
    t |> success(ran, "moe mx4 gemv dispatch n={n} d={d} net={net} k={k} nst={nst} bias={bias}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(bw)
    metal_release(bs)
    metal_release(bx)
    metal_release(by)
    metal_release(bsel)
    metal_release(bwb)
    metal_release(bvt)
    metal_release(pso)
    delete etab
    delete qp
    delete sp
    delete wref
    delete sel
    delete xv
    delete wb
    delete want
    delete vtf
}

// The gathered q51 mul_mm: ne experts with padded-CSR row runs (gather = 0 bucket-rows mode);
// exact-arithmetic planes (the A tile's f16(d*q + m) values are dyadic <= 63.25 — f16-exact),
// int x rows — every simdgroup accumulate is exact, so the whole padded y compares BIT-exact.
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
    // padded CSR: each expert's rows start at a 32-aligned base (the route prefix contract)
    var cnt : array<uint>
    var basep : array<uint>
    cnt |> reserve(ne)
    basep |> reserve(ne)
    var pad = 0
    for (c in range(ne)) {
        cnt |> push(uint(counts[c]))
        basep |> push(uint(pad))
        pad += (counts[c] + 31) / 32 * 32
    }
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
    // stage 2 mirror: per-head L2 over q/k + q pre-scale (double square-sum like the CPU)
    for (p in range(npos)) {
        for (blk in range(2 * nkh)) {
            var sum = 0.0lf
            for (j in range(ds)) {
                let v = cv[p * cd + blk * ds + j]
                sum += double(v * v)
            }
            let sc = (1.0 / max(sqrt(float(sum)), EPS)) * (blk < nkh ? qscale : 1.0)
            for (j in range(ds)) {
                cv[p * cd + blk * ds + j] *= sc
            }
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
            var ms = 0.0lf
            for (j in range(ds)) {
                let v = owant[p * di + h * ds + j]
                ms += double(v * v)
            }
            let inv = 1.0 / sqrt(float(ms) / float(ds) + EPS)
            for (j in range(ds)) {
                let v = owant[p * di + h * ds + j]
                let zv = zz[p * di + h * ds + j]
                gwant[p * di + h * ds + j] = (wn[j] * (v * inv)) * (zv / (1.0 + exp(-zv)))
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
    var bxv = buf_upload(dev, xv)
    var btot = buf_u32(dev, uint(npos * qd))
    let sran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_sg)
        metal_set_buffer(enc, bxv, 0ul, 0)
        metal_set_buffer(enc, bgt, 0ul, 1)
        metal_set_buffer(enc, btot, 0ul, 2)
        metal_dispatch_threadgroups(enc, uint3(uint((npos * qd + 63) / 64), 1u, 1u), uint3(64u, 1u, 1u))
    }
    t |> success(sran, "sigmul dispatch: {err}")
    t |> equal(buf_mismatch_approx(bxv, swant), 0)
    metal_release(bqg)
    metal_release(bq)
    metal_release(bgt)
    metal_release(bxv)
    metal_release(btot)
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
    let ran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, by, 0ul, 0)
        metal_set_buffer(enc, bx, 0ul, 1)
        metal_set_buffer(enc, bg, 0ul, 2)
        metal_set_buffer(enc, btot, 0ul, 3)
        metal_set_buffer(enc, bdim, 0ul, 4)
        metal_dispatch_threadgroups(enc, uint3(uint((total + 255) / 256), 1u, 1u), uint3(256u, 1u, 1u))
    }
    t |> success(ran, "axpy_sig dispatch: {err}")
    t |> equal(buf_mismatch_approx(by, want), 0)
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
        if (!approx(unsafe(p[k]), want[k])) {
            bad++
        }
    }
    return bad
}
