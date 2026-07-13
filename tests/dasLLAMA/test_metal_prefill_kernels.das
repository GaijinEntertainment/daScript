options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_quant  // nolint:STYLE030 — used only inside the Apple static_if half; off-Apple lint sees the branch compiled out
require ?das_metal dasllama/dasllama_metal_prefill  // nolint:STYLE030 — the Phase-6 prefill kernel set (Apple builds)
require ?das_metal metal/das_metal_boost  // nolint:STYLE030 — device/pipeline/buffer plumbing + the leak gate
require dasllama/dasllama_lcpp_mulmm  // nolint:STYLE030 — the verbatim mul_mm kernel (the lcpp GEMM path), used only in the Apple half
require math

// The Phase-6 GPU prefill kernels vs their dasLLAMA CPU twins on synthetic data. Quantize gets a
// bit-exact scenario (planted ±127 per 32-block makes every scale exactly 1.0, so quants are
// round(x) of integer values) plus an arbitrary-data scenario (scales compare within tolerance;
// quants may sit on a rounding boundary, so one quantum of slack). rmsnorm / rope / swiglu /
// attention compare within the shared fp tolerance (reduction order, hardware exp vs the CPU's
// exp/exp4 — all inside 1e-4 relative); the residual add is bit-exact (one fadd both sides).
// Off-Apple (or with no Metal device) everything feints cleanly.

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

// quantize shape
let QN = 64                             // row width (2 blocks)
let QTOK = 5

def private approx(x, y : float) : bool {
    return abs(x - y) <= 1e-4 * max(1.0, max(abs(x), abs(y)))
}

// The tiled sgmat attention trio (QK^T -> softmax -> P.V) vs a direct causal-GQA oracle at one
// (head_size, heads, kv_heads, npos) shape. Buffers carry np32 rows like the driver's (whole-tile
// sgmat stores + staging reads walk the padded rows); rows >= npos are pad. GENERIC (untyped
// dev/queue) so it instantiates only inside the Apple static_if.
def private attn_trio_gate(t : T?; dev, queue; head_size, n_heads, n_kv_heads, npos : int) {
    let kv_mul = n_heads / n_kv_heads
    let qd = n_heads * head_size
    let kv_dim = n_kv_heads * head_size
    let np32 = (npos + 31) / 32 * 32
    var err : string
    var pso_sc = pipeline_from_source(dev, metal_attn_qk_msl, metal_attn_qk_msl_entry, metal_attn_qk_msl_fastmath, err)
    t |> success(pso_sc != null, "qk pipeline: {err}")
    var pso_sm = pipeline_from_source(dev, metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, err)
    t |> success(pso_sm != null, "softmax pipeline: {err}")
    var pso_av = pipeline_from_source(dev, metal_attn_av_msl, metal_attn_av_msl_entry, metal_attn_av_msl_fastmath, err)
    t |> success(pso_av != null, "av pipeline: {err}")
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
    var bqd = buf_u32(dev, uint(qd))
    var bkvd = buf_u32(dev, uint(kv_dim))
    var bhs = buf_u32(dev, uint(head_size))
    var bkvm = buf_u32(dev, uint(kv_mul))
    var bnp = buf_u32(dev, uint(npos))
    var bnp32 = buf_u32(dev, uint(np32))
    var bscale = buf_f32(dev, scale)
    let aran = with_compute_encoder(queue, err) $(enc : MetalComputeEncoder?) {
        metal_set_pipeline(enc, pso_sc)
        metal_set_buffer(enc, baq, 0ul, 0)
        metal_set_buffer(enc, bak, 0ul, 1)
        metal_set_buffer(enc, batt, 0ul, 2)
        metal_set_buffer(enc, bqd, 0ul, 3)
        metal_set_buffer(enc, bkvd, 0ul, 4)
        metal_set_buffer(enc, bhs, 0ul, 5)
        metal_set_buffer(enc, bkvm, 0ul, 6)
        metal_set_buffer(enc, bnp, 0ul, 7)
        metal_set_buffer(enc, bnp32, 0ul, 8)
        metal_set_buffer(enc, bscale, 0ul, 9)
        metal_dispatch_threadgroups(enc, uint3(uint(np32 / 32), uint(np32 / 32), uint(n_heads)), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_sm)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_buffer(enc, bnp32, 0ul, 1)
        metal_dispatch_threadgroups(enc, uint3(uint(npos), uint(n_heads), 1u), uint3(128u, 1u, 1u))
        metal_set_pipeline(enc, pso_av)
        metal_set_buffer(enc, batt, 0ul, 0)
        metal_set_buffer(enc, bav, 0ul, 1)
        metal_set_buffer(enc, bxb, 0ul, 2)
        metal_set_buffer(enc, bqd, 0ul, 3)
        metal_set_buffer(enc, bkvd, 0ul, 4)
        metal_set_buffer(enc, bhs, 0ul, 5)
        metal_set_buffer(enc, bkvm, 0ul, 6)
        metal_set_buffer(enc, bnp, 0ul, 7)
        metal_set_buffer(enc, bnp32, 0ul, 8)
        metal_dispatch_threadgroups(enc, uint3(uint(np32 / 32), uint(head_size / 32), uint(n_heads)), uint3(128u, 1u, 1u))
    }
    t |> success(aran, "attention dispatches (hs={head_size}): {err}")
    t |> equal(buf_mismatch_approx(bxb, axb_want), 0)
    metal_release(baq)
    metal_release(bak)
    metal_release(bav)
    metal_release(batt)
    metal_release(bxb)
    metal_release(bqd)
    metal_release(bkvd)
    metal_release(bhs)
    metal_release(bkvm)
    metal_release(bnp)
    metal_release(bnp32)
    metal_release(bscale)
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
    t |> run("phase-6 prefill kernels == dasLLAMA CPU twins") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            with_metal_device() <| $(dev : MetalDevice?) {
                if (dev == null) {
                    feint("no Metal device on this box; GPU kernel checks skipped\n")
                    return
                }
                var queue = metal_new_command_queue(dev)
                var err : string

                // ===== quantize: exact scenario (planted ±127 -> d == 1.0) =====
                var pso_quant = pipeline_from_source(dev, metal_q8_quant_msl, metal_q8_quant_msl_entry, metal_q8_quant_msl_fastmath, err)
                t |> success(pso_quant != null, "quant pipeline: {err}")
                var qx : array<float>
                qx |> resize(QTOK * QN)
                for (k in range(QTOK * QN)) {
                    qx[k] = float(k * 37 % 231 - 115)           // integers in [-115, 115]
                }
                for (b in range(QTOK * QN / 32)) {
                    qx[b * 32 + b % 32] = (b % 2 == 0) ? 127.0 : -127.0     // plant the amax
                }
                var want_q : array<int8>
                var want_s : array<float>
                want_q |> resize(QTOK * QN)
                want_s |> resize(QTOK * QN / 32)
                quantize_q8_0_into(qx, int64(QTOK * QN), want_q, want_s, 0l, 0l)    // one flat row == same per-32-block math
                var bqx = buf_upload(dev, qx)
                var bxq = buf_fill(dev, QTOK * QN, int8(0))
                var bxs = buf_fill(dev, QTOK * QN / 32, 0.0)
                var bqn = buf_u32(dev, uint(QN))
                var bqblk = buf_u32(dev, uint(QTOK * QN / 32))
                var qbufs <- [bqx, bxq, bxs, bqn, bqblk]
                let qran = run_compute_1d(queue, pso_quant, qbufs, uint(QTOK * QN / 32), 64u, err)
                t |> success(qran, "quant dispatch: {err}")
                t |> equal(buf_mismatch_exact(bxq, want_q), 0)      // d == 1.0: quants bit-exact
                t |> equal(buf_mismatch_exact(bxs, want_s), 0)      // scales exactly 1.0 both sides
                // arbitrary data: scales within tolerance, quants within one rounding quantum
                for (k in range(QTOK * QN)) {
                    qx[k] = 0.37 * float(k % 53) - 9.5 + 0.013 * float(k % 7)
                }
                quantize_q8_0_into(qx, int64(QTOK * QN), want_q, want_s, 0l, 0l)
                buf_store(bqx, qx)
                let qran2 = run_compute_1d(queue, pso_quant, qbufs, uint(QTOK * QN / 32), 64u, err)
                t |> success(qran2, "quant dispatch 2: {err}")
                let got_q <- buf_download(bxq, QTOK * QN, type<int8>)
                let got_s <- buf_download(bxs, QTOK * QN / 32, type<float>)
                var bad = 0
                for (k in range(QTOK * QN)) {
                    if (abs(int(got_q[k]) - int(want_q[k])) > 1) {
                        bad++
                    }
                }
                for (k in range(QTOK * QN / 32)) {
                    if (!approx(got_s[k], want_s[k])) {
                        bad++
                    }
                }
                t |> equal(bad, 0)
                qbufs |> clear()
                unsafe {
                    delete qbufs
                }
                metal_release(bqx)
                metal_release(bxq)
                metal_release(bxs)
                metal_release(bqn)
                metal_release(bqblk)
                metal_release(pso_quant)

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
                        rmsnorm(addr(rwant[r * DIM]), reinterpret<float const?>(addr(rx[r * DIM])),
                            reinterpret<float const?>(addr(rw[0])), int64(DIM), EPS)
                    }
                }
                var brx = buf_upload(dev, rx)
                var brw = buf_upload(dev, rw)
                var bry = buf_fill(dev, NROWS * DIM, 0.0)
                var brdim = buf_u32(dev, uint(DIM))
                var breps = buf_f32(dev, EPS)
                let rran = with_compute_encoder(queue, err) <| $(enc : MetalComputeEncoder?) {
                    metal_set_pipeline(enc, pso_rms)
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
                            reinterpret<float const?>(addr(rcos[p * half])), reinterpret<float const?>(addr(rsin[p * half])))
                    }
                }
                var brv = buf_upload(dev, rv)
                var brc = buf_upload(dev, rcos)
                var brs = buf_upload(dev, rsin)
                var brn = buf_u32(dev, uint(QD))
                var brhs = buf_u32(dev, uint(HEAD_SIZE))
                var brnp = buf_u32(dev, uint(NPOS * QD / 2))
                var ropebufs <- [brv, brc, brs, brn, brhs, brnp]
                let roperan = run_compute_1d(queue, pso_rope, ropebufs, uint(NPOS * QD / 2), 64u, err)
                t |> success(roperan, "rope dispatch: {err}")
                t |> equal(buf_mismatch_approx(brv, rvwant), 0)
                ropebufs |> clear()
                unsafe {
                    delete ropebufs
                }
                metal_release(brv)
                metal_release(brc)
                metal_release(brs)
                metal_release(brn)
                metal_release(brhs)
                metal_release(brnp)
                metal_release(pso_rope)

                // ===== attention: the tiled sgmat trio vs the direct causal-GQA oracle, at the
                // classic small shape (hs 32 — one QK k-slab) AND the llama-3B head shape
                // (hs 128 — the QK kernel's two-k-slab path; 2 heads keep the oracle cheap)
                attn_trio_gate(t, dev, queue, HEAD_SIZE, N_HEADS, N_KV_HEADS, NPOS)
                attn_trio_gate(t, dev, queue, 128, 2, 1, NPOS)

                // ===== swiglu + residual add =====
                var pso_sw = pipeline_from_source(dev, metal_swiglu_msl, metal_swiglu_msl_entry, metal_swiglu_msl_fastmath, err)
                t |> success(pso_sw != null, "swiglu pipeline: {err}")
                var pso_add = pipeline_from_source(dev, metal_add_msl, metal_add_msl_entry, metal_add_msl_fastmath, err)
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
                var addbufs <- [bsg, bsu, btot]
                let addran = run_compute_1d(queue, pso_add, addbufs, uint(total), 64u, err)
                t |> success(addran, "add dispatch: {err}")
                t |> equal(buf_mismatch_exact(bsg, add_want), 0)
                addbufs |> clear()
                unsafe {
                    delete addbufs
                }
                metal_release(bsg)
                metal_release(bsu)
                metal_release(btot)
                metal_release(pso_sw)
                metal_release(pso_add)

                // ===== the verbatim llama.cpp mul_mm — the resident path's default GEMM =====
                mulmm_gate(t, dev, queue, 96, 128, 64)      // 3 k-blocks, 2 output tiles, 2 token tiles
                mulmm_gate(t, dev, queue, 64, 192, 32)      // 2 k-blocks, 3 output tiles, 1 token tile

                metal_release(queue)
            }
            t |> equal(metal_live_object_count(), 0l)
        } else {
            feint("das_metal is not built on this platform; nothing to compare\n")
        }
    }
}

// The verbatim llama.cpp mul_mm kernel — the lcpp GEMM path's exact PSO — on exact-arithmetic
// data: block_q8_0 W blobs x raw f32 X rows vs a CPU reference, BIT-exact (quants in [-8, 7],
// pow2 scales: every staged half, every product, and every f32 partial sum is exactly
// representable, so any deviation is a real bug). The kargs image is built here independently
// of the driver — it doubles as the layout oracle for uniform_mm_args.
def private mulmm_gate(t : T?; dev, queue; k, d, m : int) {
    var err : string
    var pso = pipeline_from_source(dev, LCPP_MUL_MM_Q8_MSL, LCPP_MUL_MM_Q8_ENTRY, true, err)
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
    // the ggml_metal_kargs_mul_mm image (88 bytes; plain 2D GEMM)
    var ba = metal_new_buffer(dev, 88ul)
    let pc = metal_buffer_contents(ba)
    var pi = unsafe(reinterpret<int?>(pc))
    var pu = unsafe(reinterpret<uint64?>(pc))
    var ps = unsafe(reinterpret<int16?>(pc))
    unsafe {
        pi[0] = k                           // ne00
        pi[1] = 1                           // ne02
        pu[1] = uint64(nb * 34)             // nb01
        pu[2] = uint64(nb * 34 * d)         // nb02
        pu[3] = uint64(nb * 34 * d)         // nb03
        pi[8] = 1                           // ne12
        pi[9] = 0                           // struct padding
        pu[5] = 4ul                         // nb10
        pu[6] = uint64(k * 4)               // nb11
        pu[7] = uint64(k * 4 * m)           // nb12
        pu[8] = uint64(k * 4 * m)           // nb13
        pi[18] = d                          // ne0
        pi[19] = m                          // ne1
        ps[40] = int16(1)                   // r2
        ps[41] = int16(1)                   // r3
    }
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
        metal_set_buffer(enc, ba, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_threadgroup_memory_length(enc, 6144ul, 0)
        metal_dispatch_threadgroups(enc, uint3(uint(m / 32), uint(d / 64), 1u), uint3(32u, 4u, 1u))
    }
    t |> success(ran, "mul_mm dispatch {k}x{d}x{m}: {err}")
    t |> equal(buf_mismatch_exact(by, want), 0)
    metal_release(ba)
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

// ===== local GPU plumbing (generic bodies instantiate only inside the static_if above) =====

def private buf_upload(dev; arr : array<auto(TT)>) : auto {
    var buf = metal_new_buffer(dev, uint64(length(arr)) * uint64(typeinfo sizeof(arr[0])))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(length(arr))) {
        unsafe(p[k]) = arr[k]
    }
    return buf
}

def private buf_store(buf; arr : array<auto(TT)>) {
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(length(arr))) {
        unsafe(p[k]) = arr[k]
    }
}

def private buf_fill(dev; nn : int; value : auto(TT)) : auto {
    var buf = metal_new_buffer(dev, uint64(nn) * uint64(typeinfo sizeof(value)))
    var p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(nn)) {
        unsafe(p[k]) = value
    }
    return buf
}

def private buf_u32(dev; v : uint) : auto {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private buf_f32(dev; v : float) : auto {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def private buf_download(buf; nn : int; t : type<auto(TT)>) : array<TT -const> {
    var out : array<TT -const>
    out |> resize(nn)
    let p = unsafe(reinterpret<TT -const?>(metal_buffer_contents(buf)))
    for (k in range(nn)) {
        out[k] = unsafe(p[k])
    }
    return <- out
}

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
