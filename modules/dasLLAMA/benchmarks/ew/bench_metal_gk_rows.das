options gen2
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require metal/das_metal_boost  // GPU driver half, Apple builds only
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels  // the production stamps of the templates both homes stamp
require dasllama/dasllama_metal_prefill  // the window's stamps
require math

// The row kernels the Metal home stamps off the shared templates
// (`dasllama_gpu_kernels_common.das`), each timed alone at a
// qwen35moe / qwen4exp decode shape through its generated builder: the
// hyper-connection seams, the n-gram side input, the per-head q/k
// norm, the router select and the routed combine. One instrument, no
// race: a kernel's row is the median GPU time of one dispatch over
// `g_env_bench.lab_rounds` rounds (at least 9) of 200 back-to-back
// dispatches, with the rounds' cv - a row past 3% cv is VOID and
// reruns on a quiet box. Warmup: each row runs one round more and
// drops the first (`time_rows`), and the run opens on a `warm-up` row
// (hc_init at the seam shape, never reported) that ramps the clocks
// before the first timed row. A run that times no row - no Metal
// device, an interpreted run, a build with no Metal - fails. The
// kernels' numerics are the kernel cells'
// (`tests/test_metal_misc_kernels.das`,
// `tests/test_metal_prefill_kernels.das`); this file times them.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/ew/bench_metal_gk_rows.das

let private REPS = 200
let private MIN_ROUNDS = 9

//! a buffer of `n` floats, element i the fixture's pattern value
def private fill_buf(n : int; scale : float) : MetalBuffer? {
    var buf = metal_new_buffer(g_dev, uint64(n * 4))
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    for (i in range(n)) {
        unsafe(p[i]) = scale * (0.11 * float(i % 47) - 2.5)
    }
    return buf
}

//! the median per-dispatch GPU time in us of `body` over the rounds, and the rounds' cv
def private time_rows(name : string; body : block<(enc : MetalComputeEncoder?) : void>) : tuple<us : double; cv : double> {
    let rounds = max(g_env_bench.lab_rounds, MIN_ROUNDS)
    var times : array<double>
    times |> reserve(rounds + 1)
    for (_r in range(rounds + 1)) {
        var err : string
        var ms = 0.0lf
        let ok = with_compute_encoder_timed(g_queue, err, ms) $(enc) {
            for (_i in range(REPS)) {
                invoke(body, enc)
            }
        }
        if (!ok) {
            to_log(LOG_ERROR, "{name}: dispatch failed: {err}\n")
            return (us = -1.0lf, cv = 1.0lf)
        }
        times |> push(ms * 1000.0lf / double(REPS))
    }
    times |> erase(0)   //! the first round warms the pipeline
    var sorted := times
    sorted |> sort()
    let med = sorted[length(sorted) / 2]
    var mean = 0.0lf
    for (v in times) {
        mean += v
    }
    mean /= double(length(times))
    var var2 = 0.0lf
    for (v in times) {
        var2 += (v - mean) * (v - mean)
    }
    let cv = sqrt(var2 / double(length(times))) / mean
    to_log(LOG_INFO, "{name}: {med} us a dispatch (cv {cv * 100.0lf}%{cv > 0.03lf ? " - VOID" : ""})\n")
    return (us = med, cv = cv)
}

typedef Reading = tuple<us : double; cv : double>

//! the hyper-connection seams, the n-gram side input, the per-head q/k norm, the router select, the routed combine and the deltanet
//! out-norm at a qwen35moe / qwen4exp shape
def private seam_rows : array<Reading> {
    let dim = 2048
    let hc = 4
    let hcd = hc * dim
    var bx = fill_buf(512 * hcd, 1.0)
    var by = fill_buf(512 * hcd, 0.5)
    var bw = fill_buf(4 * hcd + hcd * 4, 0.01)
    var bz = fill_buf(512 * hcd, 0.25)
    time_rows("warm-up") $(enc) {   //! the clocks ramp before the first timed row
        enc_hc_init(enc, bx, by, GkHcInitArgs(dim = uint(dim), hc = uint(hc), rows = 512u), int64(512 * hcd))
    }
    var rows <- [
        time_rows("hc_norm dim={dim} hc={hc} 1 row") $(enc) {
            enc_hc_norm(enc, bx, by, bw, GkHcNormArgs(dim = uint(dim), hc = uint(hc), woff = 0u, row0 = 0u, eps = 1e-6), int64(hc))
        },
        time_rows("hc_init dim={dim} hc={hc} 512 rows") $(enc) {
            enc_hc_init(enc, bx, by, GkHcInitArgs(dim = uint(dim), hc = uint(hc), rows = 512u), int64(512 * hcd))
        },
        time_rows("hc_mix dim={dim} hc={hc} 1 row") $(enc) {
            enc_hc_mix(enc, bx, bz, by, GkHcMixArgs(dim = uint(dim), hc = uint(hc), rows = 1u), int64(dim))
        },
        time_rows("hc_combine dim={dim} hc={hc} 1 row") $(enc) {
            enc_hc_combine(enc, by, bx, bz, GkHcCombArgs(dim = uint(dim), hc = uint(hc), rows = 1u), int64(hcd))
        },
        time_rows("ple_gate dim={dim} hc={hc} 1 row, the ring") $(enc) {
            enc_ple_gate(enc, bx, by, bz, bx, by, bw, GkPleGateArgs(dim = uint(dim), hc = uint(hc), nrows = 10u, pos0 = 17u, eps = 1e-6,
                inv_sq = 1.0 / sqrt(float(dim)), goff = uint(hcd), coff = uint(2 * hcd)), int64(hc))
        },
        time_rows("ple_conv dim={dim} hc={hc} 1 row, the ring") $(enc) {
            enc_ple_conv(enc, by, bz, bx, bw, bx, GkPleConvArgs(dim = uint(dim), hc = uint(hc), kern = 4u, dil = 3u, rows = 1u, pos0 = 17u), int64(hcd))
        },
        time_rows("qk_norm hs=256 q 16 k 2 1 row") $(enc) {
            enc_qk_norm(enc, by, by, bw, qk_norm_args(16u, 2u, 256u, 20u * 256u, 16u * 256u, 16u * 256u, 256u, 1e-6), 18l, 1l)
        },
        time_rows("moe_select ne=256 k=8 1 row") $(enc) {
            enc_moe_select(enc, bx, by, by, bw, GkTopKArgs(ne = 256u, k = 8u, norm = 1u, dsoff = GK_NO_DSCALE, lstride = 256u), 1l)
        },
        time_rows("moe_select ne=128 k=8 1 row, the down scales (gemma-4)") $(enc) {
            enc_moe_select_ds(enc, bx, by, by, bw, GkTopKArgs(ne = 128u, k = 8u, norm = 1u, dsoff = 0u, lstride = 128u), 1l)
        },
        time_rows("moe_select_512 ne=512 k=10 1 row") $(enc) {
            enc_moe_select_512(enc, bx, by, by, bw, GkTopKArgs(ne = 512u, k = 10u, norm = 1u, dsoff = GK_NO_DSCALE, lstride = 512u), 1l)
        },
        time_rows("moe_combine dim={dim} k=8 1 row") $(enc) {
            enc_moe_combine(enc, bz, bw, bx, moe_comb_args(int64(dim), 8l), int64(dim), 1l)
        },
        time_rows("dn_gate nvh=32 ds=128 1 row") $(enc) {
            enc_dn_gate(enc, bz, by, bw, GkDnGateArgs(nvh = 32u, di = 4096u, ds = 128u, eps = 1e-6), 32l, 1l)
        },
        time_rows("dn_gate nvh=32 ds=128 512 rows") $(enc) {
            enc_dn_gate(enc, bz, by, bw, GkDnGateArgs(nvh = 32u, di = 4096u, ds = 128u, eps = 1e-6), 32l, 512l)
        }
    ]
    metal_release(bx)
    metal_release(by)
    metal_release(bw)
    metal_release(bz)
    return <- rows
}

//! the q/k norm, rope and K/V store at a qwen3 shape: the split pair it replaced on the one row and on four rows, the fused pass on
//! both mirrors and four rows, and the window's fused pass with its q twin
def private qkn_rows : array<Reading> {
    let nh = 32
    let nkv = 8
    let hs = 128
    let qd = nh * hs
    let kvd = nkv * hs
    let qkvd = qd + 2 * kvd
    let npf = 512
    var bw = fill_buf(4 * hs, 0.01)
    var bqkv = fill_buf(4 * qkvd, 1.0)
    var bkm = fill_buf(64 * kvd, 0.0)
    var bvm = fill_buf(64 * kvd, 0.0)
    var bcs = fill_buf(4 * hs, 0.01)
    var brt = metal_new_buffer(g_dev, uint64(4 * 16))
    var prt = unsafe(reinterpret<uint4?>(metal_buffer_contents(brt)))
    for (r in range(4)) {
        unsafe(prt[r]) = uint4(uint(r * kvd), uint(r * kvd), 0u, uint(r + 1))
    }
    var bpq = fill_buf(npf * qd, 1.0)
    var bpk = fill_buf(npf * kvd, 1.0)
    var bph = metal_new_buffer(g_dev, uint64(npf * qd * 2))
    var bpcs = fill_buf(npf * hs / 2, 0.01)
    var pf_err : string
    var pso_pf = pipeline_from_source(g_dev, metal_pf_qkn_rope_hx_msl, metal_pf_qkn_rope_hx_msl_entry, metal_pf_qkn_rope_hx_msl_fastmath, pf_err)
    var qkn = GkQknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2), neox = 1u, nh = uint(nh), kwoff = uint(hs),
        eps = 1e-6, kvrow = uint(qkvd), qrow = uint(qkvd), cosrow = uint(hs / 2))
    var pfa = GkQknRopeArgs(kvd = uint(kvd), hs = uint(hs), half = uint(hs / 2), neox = 1u, nh = uint(nh), kwoff = uint(hs), eps = 1e-6,
        win = 1u, kvrow = uint(kvd), qrow = uint(qd), cosrow = uint(hs / 2), nreal = uint(npf), hscale = 0.088)
    var rka = gk_rope_kv_args(int64(qd), int64(kvd), int64(hs), int64(qd / 2), int64((qd + kvd) / 2), true, false, int64(hs),
        0l, 0l, 4l, int64(qkvd), int64(qkvd), int64(hs / 2), 0l)
    var rka1 = gk_rope_kv_args(int64(qd), int64(kvd), int64(hs), int64(qd / 2), int64((qd + kvd) / 2), true, false, int64(hs))
    var nka = qk_norm_args(uint(nh), uint(nkv), uint(hs), uint(qkvd), uint(qd), uint(qd), uint(hs), 1e-6)
    var rows <- [
        time_rows("qkn split f32 hs={hs} q {nh} k {nkv} 1 row (qk_norm + rope_store)") $(enc) {
            enc_qk_norm(enc, bqkv, bqkv, bw, nka, int64(nh + nkv), 1l)
            enc_rpst32_c(enc, bqkv, bqkv, bqkv, uint64((qd + kvd) * 4), bkm, 0ul, bvm, 0ul, bw, bcs, 0ul, bcs, rka1, int64((qd + kvd) / 2))
        },
        time_rows("qkn fused f32 hs={hs} q {nh} k {nkv} 1 row") $(enc) {
            enc_qkn_rope32(enc, bqkv, bqkv, bkm, 0ul, bvm, 0ul, bcs, 0ul, bw, bcs, 0ul, bw, bqkv, qkn, int64(nh + nkv), int64(hs / 2))
        },
        time_rows("qkn fused f16 hs={hs} q {nh} k {nkv} 1 row") $(enc) {
            enc_qkn_rope16(enc, bqkv, bqkv, bkm, 0ul, bvm, 0ul, bcs, 0ul, bw, bcs, 0ul, bw, bqkv, qkn, int64(nh + nkv), int64(hs / 2))
        },
        time_rows("qkn split f16 hs={hs} q {nh} k {nkv} 4 rows (qk_norm + rope_store_b)") $(enc) {
            enc_qk_norm(enc, bqkv, bqkv, bw, nka, int64(nh + nkv), 4l)
            enc_rpstb16_c(enc, bqkv, bqkv, bqkv, 0ul, bkm, 0ul, bvm, 0ul, bw, bcs, 0ul, bcs, brt, 0ul, rka, int64((qd + kvd) / 2), 4l)
        },
        time_rows("qkn fused f16 hs={hs} q {nh} k {nkv} 4 rows") $(enc) {
            enc_qkn_rope_b16(enc, bqkv, bqkv, bkm, 0ul, bvm, 0ul, bcs, 0ul, bw, bcs, 0ul, bw, brt, 0ul, bqkv, qkn, int64(nh + 2 * nkv), 4l, int64(hs / 2))
        },
        time_rows("pf qkn hx hs={hs} q {nh} k {nkv} {npf} rows (one fused pass)") $(enc) {
            enc_pf_qkn_hx_pso(enc, pso_pf, bpq, bpk, bpk, 0ul, bpk, 0ul, bpcs, 0ul, bw, bpcs, bpcs, bph, bpk, pfa, int64(nh + nkv), int64(npf), int64(hs / 2))
        }
    ]
    for (buf in fixed_array(bw, bqkv, bkm, bvm, bcs, brt, bpq, bpk, bph, bpcs)) {
        metal_release(buf)
    }
    metal_release(pso_pf)
    return <- rows
}

//! the router GEMV at a qwen3moe shape: the decode stamp (a threadgroup an expert row) and the window's (a simdgroup a row)
def private router_rows : array<Reading> {
    let ne = 256
    let dim = 2048
    let np = 512
    var bw = fill_buf(ne * dim, 0.01)
    var bx = fill_buf(np * dim, 0.5)
    var by = fill_buf(np * ne, 0.0)
    var rb = fill_buf(ne, 0.1)
    var k1 = moe_router_args(int64(ne), int64(dim), 1l)
    var kn = moe_router_args(int64(ne), int64(dim), int64(np))
    var k1g = moe_router_args(int64(ne + 1), int64(dim), 1l)
    var rows <- [
        time_rows("router ne={ne} dim={dim} 1 col") $(enc) {
            enc_moe_router(enc, bw, bx, by, rb, k1, int64(ne), 1l)
        },
        time_rows("router ne={ne} dim={dim} {np} cols") $(enc) {
            enc_moe_router_b(enc, bw, bx, by, rb, kn, int64(ne), int64(np))
        },
        time_rows("router + shared gate ne={ne}+1 dim={dim} 1 col") $(enc) {
            enc_moe_router(enc, bw, bx, by, rb, k1g, int64(ne + 1), 1l)
        },
        time_rows("routed combine + gated shared row dim={dim} k=8 1 row") $(enc) {
            enc_moe_combine_sh(enc, bx, by, bw, bx, by, moe_comb_args(int64(dim), 8l, int64(ne + 1), int64(ne)), int64(dim), 1l)
        }
    ]
    for (buf in fixed_array(bw, bx, by, rb)) {
        metal_release(buf)
    }
    return <- rows
}

//! the E-series side rows' projection at the E2B shape (35 layers x 256 rows of 1536): the router GEMV's f32 rows against the bf16
//! word form off the file's own rows, a decode column and a verify's four
def private ple_proj_rows : array<Reading> {
    let ne = 35 * 256
    let dim = 1536
    var bw = fill_buf(ne * dim, 0.01)
    var bx = fill_buf(4 * dim, 0.5)
    var by = fill_buf(4 * ne, 0.0)
    var rows : array<Reading>
    rows |> reserve(4)
    for (np in fixed_array(1, 4)) {
        let ka = moe_router_args(int64(ne), int64(dim), int64(np))
        rows |> push(time_rows("ple proj f32 rows ne={ne} dim={dim} {np} cols") $(enc) {
            var k = ka
            enc_moe_router(enc, bw, bx, by, g_one, k, int64(ne), int64(np))
        })
        rows |> push(time_rows("ple proj bf16 rows ne={ne} dim={dim} {np} cols") $(enc) {
            var k = ka
            enc_ple_proj_bf16(enc, bw, bx, by, k, int64(ne), int64(np))
        })
        if (np == 1) {
            rows |> push(time_rows("ple proj bf16 rows ne={ne} dim={dim} 1 col, a threadgroup a row") $(enc) {
                var k = ka
                enc_ple_proj_bf16_one(enc, bw, bx, by, k, int64(ne), 1l)
            })
        }
    }
    for (buf in fixed_array(bw, bx, by)) {
        metal_release(buf)
    }
    return <- rows
}

//! the Pocket frame's row GEMV at the backbone's 1024: the bare dot and the normed, modulated SiLU over the slab's f32 rows
def private pk_rows : array<Reading> {
    let n = 1024
    var bw = fill_buf(n * n + 4 * n, 0.01)
    var bx = fill_buf(n, 0.5)
    var by = fill_buf(n, 0.0)
    var bmod = fill_buf(3 * n, 0.1)
    var ka = GkPkGemvArgs(nin = uint(n), nout = uint(n), stride = uint(n), eps = 1e-6, boff = uint(n * n), lwoff = uint(n * n + n),
        lboff = uint(n * n + 2 * n))
    var rows <- [
        time_rows("pk gemv dot nin={n} nout={n}") $(enc) {
            enc_pk_gemv_f32(enc, bw, bx, by, ka, int64(n))
        },
        time_rows("pk gemv ln silu nin={n} nout={n}") $(enc) {
            enc_pk_gemv_f32_ln_silu(enc, bw, bx, by, bmod, ka, int64(n))
        }
    ]
    for (buf in fixed_array(bw, bx, by, bmod)) {
        metal_release(buf)
    }
    return <- rows
}

//! the deltanet conv front at the qwen35moe shape over one group: the conv and the L2 pass it carried against the one fused pass,
//! at a decode row, a verify's four rows and a 512-row window
def private dn_rows : array<Reading> {
    let kd = 2048
    let ds = 128
    let cd = 2 * kd + 4096
    var bx = fill_buf(512 * cd, 0.1)
    var bh = fill_buf(cd * 3, 0.1)
    var bw = fill_buf(cd * 4, 0.05)
    var by = fill_buf(512 * cd, 1.0)
    var rows : array<Reading>
    rows |> reserve(18)
    for (np in fixed_array(1, 4, 8, 16, 64, 512)) {
        var bg = metal_new_buffer(g_dev, 32ul)
        var gp = unsafe(reinterpret<uint?>(metal_buffer_contents(bg)))
        for (k in range(8)) {
            unsafe(gp[k]) = k == 1 ? uint(np) : 0u
        }
        var ka = DnArgs(cd = uint(cd), dconv = 4u, npos = uint(np), kd = uint(kd), ds = uint(ds), eps = 1e-6, qscale = 0.088)
        var kn = dn_conv_args(ka)
        rows |> push(time_rows("dn conv + l2 cd={cd} {np} rows") $(enc) {
            enc_dn_conv(enc, bx, bh, bw, by, bg, ka, int64(cd), int64(np), 1l)
            enc_dn_l2norm(enc, by, ka, int64(2 * kd / ds), int64(np))
        })
        rows |> push(time_rows("dn conv fused pb1 cd={cd} {np} rows") $(enc) {
            enc_dn_conv_n(enc, bx, by, bh, bw, bg, kn, dn_conv_n_wgs(int64(cd), int64(np)), 1l)
        })
        rows |> push(time_rows("dn conv fused pb4 cd={cd} {np} rows") $(enc) {
            enc_dn_conv_n4(enc, bx, by, bh, bw, bg, kn, dn_conv_n_wgs(int64(cd), int64(np), 4l), 1l)
        })
        metal_release(bg)
    }
    for (buf in fixed_array(bx, bh, bw, by)) {
        metal_release(buf)
    }
    return <- rows
}

//! the two dispatches a gated attention layer retires once its q and gate stay in the [q | gate] projection rows, at the qwen35
//! shape (eight heads of 256): the deinterleave and the sigmoid epilogue, at a decode row and a verify's four rows
def private gate_rows : array<Reading> {
    let hs = 256
    let qd = 8 * hs
    var bqg = fill_buf(4 * 2 * qd, 0.1)
    var bq = fill_buf(4 * 3 * qd, 1.0)
    var bg = fill_buf(4 * qd, 1.0)
    var by = fill_buf(4 * qd, 1.0)
    var rows : array<Reading>
    rows |> reserve(2)
    for (np in fixed_array(1, 4)) {
        var bt = metal_new_buffer(g_dev, 4ul)
        var tp = unsafe(reinterpret<uint?>(metal_buffer_contents(bt)))
        unsafe(tp[0]) = uint(np * qd)
        rows |> push(time_rows("gated deint + sigmul qd={qd} {np} rows") $(enc) {
            enc_dn_deint(enc, bqg, bq, bg, DnDeintArgs(hs = uint(hs), qd = uint(qd), qs = uint(3 * qd)), int64(qd), int64(np))
            enc_sigmul(enc, by, 0ul, bg, 0ul, bt, int64(np * qd))
        })
        metal_release(bt)
    }
    for (buf in fixed_array(bqg, bq, bg, by)) {
        metal_release(buf)
    }
    return <- rows
}

//! the gemma-4 MoE layer's tail at the 26B's shape (dim 2816, eight slots): the unfused chain it replaced - the routed combine, the
//! routed and dense post norms, their add, the post-FFN norm and the residual add with the next norm (the window: the add alone) -
//! against the one step, at a decode row and a 512-row window
def private g4_tail_rows : array<Reading> {
    let dim = 2816
    let k = 8
    let np = 512
    var bx = fill_buf(np * dim, 0.5)
    var bd = fill_buf(np * dim, 0.25)
    var brt = fill_buf(np * dim, 0.0)
    var bdn = fill_buf(np * k * dim, 0.1)
    var bsel = fill_buf(np * 2 * k, 0.01)
    var binv = metal_new_buffer(g_dev, uint64(np * k * 4))
    var ip = unsafe(reinterpret<uint?>(metal_buffer_contents(binv)))
    for (i in range(np * k)) {
        unsafe(ip[i]) = uint((i * 7) % (np * k))
    }
    var bw = fill_buf(4 * dim, 0.01)
    var udim = uniform_u32(uint(dim))
    var ueps = uniform_f32(1e-6)
    var utot = uniform_u32(uint(np * dim))
    var ka = moe_g4_args(int64(dim), int64(k), 1e-6, 0.75, false, true)
    var kp = moe_g4_args(int64(dim), int64(k), 1e-6, 0.75, false, false)
    var kc = moe_comb_args(int64(dim), int64(k))
    var rows <- [
        time_rows("g4 MoE tail dim={dim} k={k} 1 row, unfused (6 dispatches)") $(enc) {
            enc_moe_combine(enc, brt, bsel, bdn, kc, int64(dim), 1l)
            enc_rms(enc, brt, 0ul, bw, brt, udim, ueps)
            enc_rms(enc, bd, 0ul, bw, bd, udim, ueps)
            enc_add(enc, bd, 0ul, brt, 0ul, udim, int64(dim))
            enc_rms(enc, bd, 0ul, bw, bd, udim, ueps)
            var kr = GkAddRmsArgs(dim = uint(dim), add_on = 1u, eps = 1e-6, ascale = 1.0)
            enc_addrms_c(enc, bx, bd, bw, bd, kr)
        },
        time_rows("g4 MoE step dim={dim} k={k} 1 row") $(enc) {
            enc_moe_comb_g4(enc, bx, bsel, bdn, bd, bw, bd, ka, 1l)
        },
        time_rows("g4 MoE tail dim={dim} k={k} {np} rows, unfused (6 dispatches)") $(enc) {
            pf_enc_moe_reduce(enc, brt, binv, bsel, bdn, kc, int64(dim), int64(np))
            pf_enc_rms(enc, brt, 0ul, bw, brt, udim, ueps, int64(np))
            pf_enc_rms(enc, bd, 0ul, bw, bd, udim, ueps, int64(np))
            enc_add(enc, bd, 0ul, brt, 0ul, utot, int64(np * dim))
            pf_enc_rms(enc, bd, 0ul, bw, bd, udim, ueps, int64(np))
            enc_add(enc, bx, 0ul, bd, 0ul, utot, int64(np * dim))
        },
        time_rows("g4 MoE step dim={dim} k={k} {np} rows (the window's stamp)") $(enc) {
            enc_moe_comb_g4_pf(enc, bx, binv, bsel, bdn, bd, bw, kp, int64(np))
        }
    ]
    for (buf in fixed_array(bx, bd, brt, bdn, bsel, binv, bw)) {
        metal_release(buf)
    }
    for (u in fixed_array(udim, ueps, utot)) {
        pool_release(g_upool, u, 4ul)
    }
    return <- rows
}

//! the classifier epilogue at gemma-4's vocab: the final softcap and two suppressed ids over one row and four rows, one dispatch
def private cls_epi_rows : array<Reading> {
    let vocab = 262144
    var by = fill_buf(4 * vocab, 1.0)
    var bsupp = metal_new_buffer(g_dev, 8ul)
    var sp = unsafe(reinterpret<uint?>(metal_buffer_contents(bsupp)))
    unsafe {
        sp[0] = 255999u
        sp[1] = 258880u
    }
    var rows : array<Reading>
    rows |> reserve(2)
    for (nr in fixed_array(1, 4)) {
        rows |> push(time_rows("cls epilogue vocab={vocab} cap 30 two pins {nr} rows") $(enc) {
            enc_cls_epi_rows(enc, by, bsupp, int64(vocab), int64(nr), 30.0, 2l)
        })
    }
    metal_release(by)
    metal_release(bsupp)
    return <- rows
}

//! the residual step's rows at width `dim`: the plain add + norm, the sandwich seam as the pair and as one dispatch, the seam with no
//! norm stored, each at a decode row, a verify's four rows and a window of `np`
def private ar_dim_rows(dim, np : int; var rows : array<Reading>) {
    var bx = fill_buf(np * dim, 0.5)
    var ba = fill_buf(np * dim, 0.25)
    var by = fill_buf(np * dim, 0.0)
    var bh = metal_new_buffer(g_dev, uint64(np * dim * 2))
    var bw = fill_buf(2 * dim, 0.01)
    var udim = uniform_u32(uint(dim))
    var ueps = uniform_f32(1e-6)
    var utot = uniform_u32(uint(dim))
    var utot4 = uniform_u32(uint(4 * dim))
    var utotn = uniform_u32(uint(np * dim))
    let kp = GkAddRmsArgs(dim = uint(dim), add_on = 1u, eps = 1e-6, ascale = 1.0)
    let ks = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(dim), eps = 1e-6, ascale = 0.75, pre_on = 1u, pwoff = 0u)
    rows |> push(time_rows("ar dim={dim} 1 row: add + norm") $(enc) {
        var k = kp
        enc_addrms_c(enc, bx, ba, bw, by, k)
    })
    rows |> push(time_rows("ar dim={dim} 1 row: sandwich seam, rms + add_rms") $(enc) {
        var k = kp
        enc_rms(enc, ba, 0ul, bw, ba, udim, ueps)
        enc_addrms_c(enc, bx, ba, bw, by, k)
    })
    rows |> push(time_rows("ar dim={dim} 1 row: sandwich seam, pre_add_rms") $(enc) {
        var k = ks
        enc_preaddrms_c(enc, bx, ba, bw, by, k)
    })
    rows |> push(time_rows("ar dim={dim} 1 row: sandwich seam no norm out, rms + add") $(enc) {
        enc_rms(enc, ba, 0ul, bw, ba, udim, ueps)
        enc_add(enc, bx, 0ul, ba, 0ul, utot, int64(dim))
    })
    rows |> push(time_rows("ar dim={dim} 1 row: sandwich seam no norm out, pre_add") $(enc) {
        var k = ks
        enc_preadd_c(enc, bx, ba, bw, k)
    })
    rows |> push(time_rows("ar dim={dim} 4 rows: add + norm") $(enc) {
        var k = kp
        enc_add_rms_b_c(enc, bx, ba, bw, by, k, 4l)
    })
    rows |> push(time_rows("ar dim={dim} 4 rows: sandwich seam, rms + add_rms") $(enc) {
        var k = kp
        enc_rms_b(enc, ba, 0ul, bw, ba, udim, ueps, 4l)
        enc_add_rms_b_c(enc, bx, ba, bw, by, k, 4l)
    })
    rows |> push(time_rows("ar dim={dim} 4 rows: sandwich seam, pre_add_rms") $(enc) {
        var k = ks
        enc_preaddrms_b_c(enc, bx, ba, bw, by, k, 4l)
    })
    rows |> push(time_rows("ar dim={dim} 4 rows: sandwich seam no norm out, rms + add") $(enc) {
        enc_rms_b(enc, ba, 0ul, bw, ba, udim, ueps, 4l)
        enc_add(enc, bx, 0ul, ba, 0ul, utot4, int64(4 * dim))
    })
    rows |> push(time_rows("ar dim={dim} 4 rows: sandwich seam no norm out, pre_add") $(enc) {
        var k = ks
        enc_preadd_b_c(enc, bx, ba, bw, k, 4l)
    })
    rows |> push(time_rows("ar dim={dim} {np} rows: sandwich seam, rms + add_rms hx") $(enc) {
        var k = kp
        pf_enc_rms(enc, ba, 0ul, bw, ba, udim, ueps, int64(np))
        pf_enc_add_rms_bhx_c(enc, bx, ba, bw, by, bh, k, int64(np))
    })
    rows |> push(time_rows("ar dim={dim} {np} rows: sandwich seam, pre_add_rms hx") $(enc) {
        var k = ks
        enc_preaddrms_bhx_c(enc, bx, ba, bw, by, bh, k, int64(np))
    })
    rows |> push(time_rows("ar dim={dim} {np} rows: sandwich seam no norm out, rms + add") $(enc) {
        pf_enc_rms(enc, ba, 0ul, bw, ba, udim, ueps, int64(np))
        enc_add(enc, bx, 0ul, ba, 0ul, utotn, int64(np * dim))
    })
    rows |> push(time_rows("ar dim={dim} {np} rows: sandwich seam no norm out, pre_add") $(enc) {
        var k = ks
        enc_preadd_b_c(enc, bx, ba, bw, k, int64(np))
    })
    for (buf in fixed_array(bx, ba, by, bh, bw)) {
        metal_release(buf)
    }
    for (u in fixed_array(udim, ueps, utot, utot4, utotn)) {
        pool_release(g_upool, u, 4ul)
    }
}

//! the residual step at the gemma-3-12b and E4B widths (`ar_dim_rows`), and the E-series pre-step's finish over 42 layers of 256
def private ar_rows : array<Reading> {
    let np = 512
    var rows : array<Reading>
    rows |> reserve(32)
    for (dim in fixed_array(2560, 3840)) {
        ar_dim_rows(dim, np, rows)
    }
    let nl = 42
    let ple = 256
    var bproj = fill_buf(np * nl * ple, 0.5)
    var bside = fill_buf(np * nl * ple, 0.25)
    var bpn = fill_buf(ple, 0.01)
    for (npos in fixed_array(1, np)) {
        rows |> push(time_rows("ple finish {nl} layers x {ple} {npos} rows") $(enc) {
            var k = ple_finish_args(int64(ple), 2560l, 1e-6)
            enc_ple_finish(enc, bside, bproj, bpn, k, int64(nl * npos))
        })
    }
    for (buf in fixed_array(bproj, bside, bpn)) {
        metal_release(buf)
    }
    return <- rows
}

def private run_gk_rows(b : B?) {
    if (!metal_common_init() || !metal_decode_init() || !metal_prefill_pso_init()) {
        b |> failure("no Metal device: no row timed")
        return
    }
    var rows <- seam_rows()
    rows |> push_from(qkn_rows(), router_rows(), pk_rows(), dn_rows())
    rows |> push_from(gate_rows(), cls_epi_rows(), g4_tail_rows(), ple_proj_rows())
    rows |> push_from(ar_rows())
    var nmeas = 0
    var nvoid = 0
    for (r in rows) {
        if (r.us > 0.0lf) {
            nmeas++
            if (r.cv > 0.03lf) {
                nvoid++
            }
        }
    }
    b |> success(nmeas == length(rows), "every kernel dispatched")
    if (nvoid > 0) {
        to_log(LOG_WARNING, "{nvoid} of {nmeas} rows VOID (cv past 3%): rerun on a quiet box\n")
    }
}

[benchmark]
def bench_metal_gk_rows(b : B?) {
    if (!jit_enabled()) {
        b |> failure("interpreted: dasLLAMA benchmarks are JIT-only, no row timed")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_gk_rows(b)
    } else {
        b |> failure("this build carries no Metal: no row timed")
    }
}
