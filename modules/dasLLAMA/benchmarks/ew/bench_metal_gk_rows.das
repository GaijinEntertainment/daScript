options gen2
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require metal/das_metal_boost  // GPU driver half, Apple builds only
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels  // the production stamps of the templates both homes stamp
require dasllama/dasllama_metal_prefill  // the window's stamps
require metal/msl_shader  // [metal_kernel]: the races' bench-side stamps
require dasllama/dasllama_metal_lens  // [metal_dispatch], compile_stamp: their builders and pipelines
require dasllama/dasllama_gpu_math  // rms_inv: the template bodies the bench-side stamps compile
require daslib/typemacro_boost  // [template_struct_instance]: the bench-side stamps' shells
require math

// The row kernels the Metal home stamps off the shared templates
// (`dasllama_gpu_kernels_common.das`), each timed alone at a
// qwen35moe / qwen4exp decode shape through its generated builder: the
// hyper-connection seams, the n-gram side input, the per-head q/k
// norm, the router select and the routed combine. A lone kernel's row
// is the median GPU time of one dispatch over `g_env_bench.lab_rounds`
// rounds (at least 9) of 200 back-to-back dispatches, with the rounds'
// cv - a row past 3% cv is VOID and reruns on a quiet box; it reads no
// output back and prints `timing-only`. Warmup: each row runs one
// round more and drops the first (`time_rows`), and the run opens on a
// `warm-up` row (hc_init at the seam shape, never reported) that ramps
// the clocks before the first timed row. A run that times no row - no
// Metal device, an interpreted run, a build with no Metal - fails. The
// kernels' numerics are the kernel cells'
// (`tests/test_metal_misc_kernels.das`,
// `tests/test_metal_prefill_kernels.das`); this file times them.
//
// The races time the shipped picks against their candidates at every
// row count the pick reads, an arm chosen by its index (`race_rows`,
// `race_arms`): a round times every arm's 200 dispatches in turn, the
// first round warms each arm and drops, an arm's row its best round
// with the median and cv beside. The shipped arm is the baseline,
// checked against a CPU reference in the process; another arm prints
// `bit-exact vs` the baseline or its difference from the reference,
// and an arm past `RACE_BOUND` prints the miss and no time. The races
// and their compare regions:
// - the gemma-4 MoE step with four slot loads in flight on and off
//   (`SLOT_X4`) at one and four rows (`MetalMoeCombG4` / `Hx`) and the
//   512-row window (`MetalMoeCombG4Pf`), each against a bench stamp
//   with the constant flipped (`g4_slot_race`): the stream rows;
// - the routed combine the same way at one and four rows
//   (`MetalMoeCombine` / `Sh`) and the window's reduce (`MetalMoeReduce`
//   / `Sh`, `comb_slot_race`): the combined rows;
// - the gemma-4 MoE tail, the one step against the unfused chain, at a
//   decode row and the window (`g4_tail_race`): the stream rows;
// - the select with and without the down-scale arm, no row named
//   (`select_ds_race`): the picks, then their weights;
// - the residual step at 1152, 2560, 3840, 4608, 5120, 5376 and 8192
//   (`ar_race`, `ar_arms`): the plain add + norm, the sandwich seam and
//   its no-norm form as one dispatch against the rms and the add (+
//   norm) - past the 4096-float slab against the split chain, the
//   plain step on the window and the decode rows' f16-twin stamps
//   too, and the seam against a wide-slab bench stamp where its slab
//   holds the row: the stream rows and, where the step stores one, the
//   f32 next norm;
// - the sandwich seam at 256, 128 and 1024 lanes over one and four rows
//   of the plain pipeline and 512 rows of the f16 twin's, at 1152, 2560,
//   3840 and 5376 (`seam_lane_race`): the stream rows and the f32 next
//   norm;
// - the side rows' projection, the bf16 stamps against the f32 rows, on
//   one weight buffer and on a ring of `PLE_RING` copies
//   (`ple_proj_race`): the logits.
//
//   bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/ew/bench_metal_gk_rows.das

let private REPS = 200
let private MIN_ROUNDS = 9

let private RACE_BOUND = 1e-4lf   //! a race arm's max |gpu - ref| over the compare region, over the region's max |ref|

//! the fixture's pattern value at element i
def private pat(i : int; scale : float) : float => scale * (0.11 * float(i % 47) - 2.5)

//! the first `n` floats of `buf` set to the fixture's pattern from element `base` on
def private fill_pattern(buf : MetalBuffer?; n : int; scale : float; base : int = 0) {
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    for (i in range(n)) {
        unsafe(p[i]) = pat(base + i, scale)
    }
}

//! a buffer of `n` floats, element i the fixture's pattern value
def private fill_buf(n : int; scale : float) : MetalBuffer? {
    var buf = metal_new_buffer(g_dev, uint64(n * 4))
    fill_pattern(buf, n, scale)
    return buf
}

//! the first `n` floats of `buf` read back
def private read_floats(buf : MetalBuffer?; n : int) : array<float> {
    let p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    return <- [for (i in range(n)); unsafe(p[i])]
}

//! the median, the best and the cv of a row's per-dispatch times
def private round_stats(times : array<double>) : tuple<med : double; best : double; cv : double> {
    var sorted := times
    sorted |> sort()
    var mean = 0.0lf
    for (v in times) {
        mean += v
    }
    mean /= double(length(times))
    var var2 = 0.0lf
    for (v in times) {
        var2 += (v - mean) * (v - mean)
    }
    return (med = sorted[length(sorted) / 2], best = sorted[0], cv = sqrt(var2 / double(length(times))) / mean)
}

typedef Reading = tuple<us : double; cv : double>

//! `reps` dispatches of arm `arm` of `body` on one timed encoder, waited on: the per-dispatch GPU time in us, or -1 on a failure
def private timed_dispatches(name : string; arm, reps : int; body : block<(enc : MetalComputeEncoder?; arm : int) : void>) : double {
    var err : string
    var ms = 0.0lf
    let ok = with_compute_encoder_timed(g_queue, err, ms) $(enc) {
        for (_i in range(reps)) {
            invoke(body, enc, arm)
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "{name}: arm {arm} dispatch failed: {err}\n")
        return -1.0lf
    }
    return ms * 1000.0lf / double(reps)
}

//! the per-dispatch GPU times in us of `narms` arms, a list an arm, interleaved: a round times each arm's REPS dispatches of `body`
//! in turn, and the first round warms every arm and drops; empty on a dispatch failure
def private race_arms(name : string; narms : int; body : block<(enc : MetalComputeEncoder?; arm : int) : void>) : array<array<double>> {
    let rounds = max(g_env_bench.lab_rounds, MIN_ROUNDS)
    var times : array<array<double>>
    times |> resize(narms)
    for (r in range(rounds + 1)) {
        for (a in range(narms)) {
            let us = timed_dispatches(name, a, REPS, body)
            if (us < 0.0lf) {
                var none : array<array<double>>
                return <- none
            }
            if (r > 0) {
                times[a] |> push(us)
            }
        }
    }
    return <- times
}

//! the median per-dispatch GPU time in us of `body` over the rounds (`race_arms` at one arm), and the rounds' cv
def private time_rows(name : string; body : block<(enc : MetalComputeEncoder?) : void>) : Reading {
    var times <- race_arms(name, 1) $(enc; _arm) {
        invoke(body, enc)
    }
    return (us = -1.0lf, cv = 1.0lf) if (empty(times))
    let st = round_stats(times[0])
    to_log(LOG_INFO, "{name}: {st.med} us a dispatch (cv {st.cv * 100.0lf}%{st.cv > 0.03lf ? " - VOID" : ""}) - timing-only\n")
    return (us = st.med, cv = st.cv)
}

//! arm `arm` of `body` dispatched once and waited on
def private dispatch_once(arm : int; body : block<(enc : MetalComputeEncoder?; arm : int) : void>) : bool => timed_dispatches("once", arm, 1, body) >= 0.0lf

struct private ArmCompare {
    err : double    //! max |gpu - ref| over the compare region, over its max |ref|
    text : string   //! the row's compare
}

//! arm `arm`'s compare over its compare region `got`: bit-exact vs the baseline arm's `base` where an arm past the baseline matches
//! it bit for bit, else its difference from the CPU reference `ref`
def private arm_compare(arm : int; base_name : string; got, base : array<float>; ref : array<double>) : ArmCompare {
    var dmax = 0.0lf
    var rmax = 0.0lf
    var same = arm > 0 && length(got) == length(base)
    for (g, r, i in got, ref, count()) {
        dmax = max(dmax, abs(double(g) - r))
        rmax = max(rmax, abs(r))
        if (same && unsafe(reinterpret<uint>(g)) != unsafe(reinterpret<uint>(base[i]))) {
            same = false
        }
    }
    let err = rmax > 0.0lf ? dmax / rmax : dmax
    if (same) {
        return ArmCompare(err = err, text = "bit-exact vs {base_name}")
    }
    return ArmCompare(err = err, text = "{err} <= {RACE_BOUND} vs CPU reference")
}

//! race arm `arm`'s row off the race's `times` (`race_arms`): its best round, the median and cv beside, and its compare - or the
//! miss and the bound alone
def private arm_row(name : string; times : array<array<double>>; arm : int; cmp : ArmCompare) : Reading {
    if (cmp.err > RACE_BOUND) {
        to_log(LOG_INFO, "{name}: MISS - {cmp.err} past the bound {RACE_BOUND} vs CPU reference\n")
        return (us = -1.0lf, cv = 1.0lf)
    }
    if (arm >= length(times)) {
        return (us = -1.0lf, cv = 1.0lf)
    }
    let st = round_stats(times[arm])
    to_log(LOG_INFO, "{name}: best {st.best} us a dispatch, median {st.med} (cv {st.cv * 100.0lf}%{st.cv > 0.03lf ? " - VOID" : ""}) - {cmp.text}\n")
    return (us = st.best, cv = st.cv)
}

//! a race's rows: each arm dispatched once off `reset` and its compare region (`read`) checked (`arm_compare`: arm 0, the baseline,
//! against the CPU reference `ref`), then the arms raced (`race_arms`), a row an arm
def private race_rows(title : string; names : array<string>; ref : array<double>; var rows : array<Reading>; reset : block<() : void>;
                      read : block<(var got : array<float>) : void>; body : block<(enc : MetalComputeEncoder?; arm : int) : void>) {
    let narms = length(names)
    var base : array<float>
    var cmps : array<ArmCompare>
    for (arm in range(narms)) {
        invoke(reset)
        if (!dispatch_once(arm, body)) {
            cmps |> push(ArmCompare(err = 1.0lf, text = "no dispatch"))
            continue
        }
        var inscope got : array<float>
        invoke(read, got)
        if (arm == 0) {
            base := got
        }
        cmps |> push(arm_compare(arm, names[0], got, base, ref))
    }
    var times <- race_arms(title, narms, body)
    rows |> reserve(length(rows) + narms)
    for (arm in range(narms)) {
        rows |> push(arm_row("race {title}: {names[arm]}", times, arm, cmps[arm]))
    }
}

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
            enc_moe_select(enc, bx, by, by, GkTopKArgs(ne = 256u, k = 8u, norm = 1u, lstride = 256u), 1l)
        },
        time_rows("moe_select ne=128 k=8 1 row, the down scales (gemma-4)") $(enc) {
            enc_moe_select_ds(enc, bx, by, by, bw, GkTopKArgs(ne = 128u, k = 8u, norm = 1u, lstride = 128u), 1l)
        },
        time_rows("moe_select_512 ne=512 k=10 1 row") $(enc) {
            enc_moe_select_512(enc, bx, by, by, GkTopKArgs(ne = 512u, k = 10u, norm = 1u, lstride = 512u), 1l)
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

let private PLE_RING = 8   //! the projection race's weight copies: 440 MB of f32 rows, 220 MB of bf16 words, past the M5 Max's 48 MB SLC

//! the projection fixture's weight at element i: the pattern value truncated to bf16, so the f32 rows and the bf16 words hold one matrix
def private ple_w(i : int) : float => unsafe(reinterpret<float>(unsafe(reinterpret<uint>(pat(i, 0.01))) & 0xFFFF0000u))

//! the side rows' projection operands: `ring` copies of the weight matrix as f32 rows and as bf16 words, the columns and the logits
struct private PleStep {
    bws : array<MetalBuffer?>   //! the f32 rows, copy i
    bbs : array<MetalBuffer?>   //! the same rows' bf16 words, copy i
    bx : MetalBuffer?           //! the columns, `dim` apart
    by : MetalBuffer?           //! the logits, column c's row r at c * ne + r
    ka : GkRouterArgs
    ring : int
    slot : int                  //! the copy the next dispatch reads
}

//! the projection race's arm on `s` in its `form` (0 the bf16 words a threadgroup a row, 1 the bf16 words blocked, 2 the f32 rows),
//! the next weight copy of the ring
def private ple_proj_arm(enc : MetalComputeEncoder?; var s : PleStep; form : int) {
    var k = s.ka
    let ne = int64(s.ka.ne)
    let np = int64(s.ka.ncols)
    if (form == 0) {
        enc_ple_proj_bf16_one(enc, s.bbs[s.slot], s.bx, s.by, k, ne, 1l)
    } elif (form == 1) {
        enc_ple_proj_bf16(enc, s.bbs[s.slot], s.bx, s.by, k, ne, np)
    } else {
        enc_moe_router(enc, s.bws[s.slot], s.bx, s.by, g_one, k, ne, np)
    }
    s.slot = (s.slot + 1) % s.ring
}

//! the projection's logits on the CPU in double off the fixture: column c's row r the dot of weight row r with column c
def private ple_proj_ref(ne, dim, np : int) : array<double> {
    var out : array<double>
    out |> reserve(np * ne)
    for (c in range(np)) {
        for (r in range(ne)) {
            var acc = 0.0lf
            for (e in range(dim)) {
                acc += double(ple_w(r * dim + e)) * double(pat(c * dim + e, 0.5))
            }
            out |> push(acc)
        }
    }
    return <- out
}

//! the E-series side rows' projection at the E2B shape (35 layers x 256 rows of 1536) over `np` columns, its weights on one buffer
//! or a ring of `ring` copies (dispatch i reads copy i mod `ring`): the shipped bf16 stamp for the column count - a threadgroup a row
//! at one column, the blocked stamp past it - the baseline, raced against the other bf16 stamp at one column and the f32 rows; the
//! compare region is the logits
def private ple_proj_race(np, ring : int; var rows : array<Reading>) {
    let ne = 35 * 256
    let dim = 1536
    let n = ne * dim
    var s = PleStep(bx = fill_buf(np * dim, 0.5), by = fill_buf(np * ne, 0.0), ka = moe_router_args(int64(ne), int64(dim), int64(np)),
        ring = ring)
    s.bws |> reserve(ring)
    s.bbs |> reserve(ring)
    for (_i in range(ring)) {
        s.bws |> push(metal_new_buffer(g_dev, uint64(n) * 4ul))
        s.bbs |> push(metal_new_buffer(g_dev, uint64(n) * 2ul))
    }
    var pw = unsafe(reinterpret<float?>(metal_buffer_contents(s.bws[0])))
    var pb = unsafe(reinterpret<uint16?>(metal_buffer_contents(s.bbs[0])))
    for (i in range(n)) {
        let w = ple_w(i)
        unsafe {
            pw[i] = w
            pb[i] = uint16(reinterpret<uint>(w) >> 16u)
        }
    }
    for (i in range(1, ring)) {
        unsafe {
            memcpy(metal_buffer_contents(s.bws[i]), metal_buffer_contents(s.bws[0]), int64(n) * 4l)
            memcpy(metal_buffer_contents(s.bbs[i]), metal_buffer_contents(s.bbs[0]), int64(n) * 2l)
        }
    }
    let forms = np == 1 ? fixed_array(0, 1, 2) : fixed_array(1, 2, 2)
    let names <- (np == 1 ? ["bf16 words, a threadgroup a row (the shipped stamp)", "bf16 words, blocked", "f32 rows"]
        : ["bf16 words, blocked (the shipped stamp)", "f32 rows"])
    let ref <- ple_proj_ref(ne, dim, np)
    race_rows("ple proj ne={ne} dim={dim} {np} cols, ring {ring}", names, ref, rows, $ {
        fill_pattern(s.by, np * ne, 0.0)
    }, $(var got) {
        got |> push_from(read_floats(s.by, np * ne))
    }, $(enc, a) {
        ple_proj_arm(enc, s, forms[a])
    })
    for (i in range(ring)) {
        metal_release(s.bws[i])
        metal_release(s.bbs[i])
    }
    metal_release(s.bx)
    metal_release(s.by)
}

//! the projection race over one weight buffer and over the ring, a decode column and a verify's four
def private ple_proj_rows : array<Reading> {
    var rows : array<Reading>
    rows |> reserve(10)
    for (ring in fixed_array(1, PLE_RING)) {
        for (np in fixed_array(1, 4)) {
            ple_proj_race(np, ring, rows)
        }
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

//! the gemma-4 step race stamps' shell: `MetalMoeCombG4T` a threadgroup a row, each stamp the shipped one it races with `SLOT_X4`
//! flipped
[ |> template_struct_instance]
class template BenchMoeCombG4T : MetalMoeCombG4T {
    [metal_kernel]
    def bench_moe_comb_g4 {
        comb_g4_row(gl_WorkGroupID.x)
    }
}

//! `MetalMoeCombG4Pf` with four slot loads in flight a lane
[metal_dispatch(name = "bench_enc_moe_comb_g4_pf_x4", pso = "g_bench_pso_moe_comb_g4_pf_x4", tgmem = "BenchMoeCombG4PfX4_bench_moe_comb_g4_msl_tgmem",
    tg = 1024, grid = "npos", params = "npos : int64")]
class BenchMoeCombG4PfX4 : BenchMoeCombG4T {
    override GATHERED = true
    override OUT_F32 = false
    override SLOT_X4 = true
}

//! `MetalMoeCombG4` with one slot's loads in flight a step
[metal_dispatch(name = "bench_enc_moe_comb_g4_x1", pso = "g_bench_pso_moe_comb_g4_x1", tgmem = "BenchMoeCombG4X1_bench_moe_comb_g4_msl_tgmem",
    tg = 1024, grid = "nst", params = "nst : int64")]
class BenchMoeCombG4X1 : BenchMoeCombG4T {
    override PRESUM_SWITCH = false
    override SLOT_X4 = false
}

//! `MetalMoeCombG4Hx` with one slot's loads in flight a step
[metal_dispatch(name = "bench_enc_moe_comb_g4_hx_x1", pso = "g_bench_pso_moe_comb_g4_hx_x1", tgmem = "BenchMoeCombG4HxX1_bench_moe_comb_g4_msl_tgmem",
    tg = 1024, grid = "nst", params = "nst : int64")]
class BenchMoeCombG4HxX1 : BenchMoeCombG4T {
    override HX = true
    override PRESUM_SWITCH = false
    override SLOT_X4 = false
}

//! the routed combine race stamps' shell: `GkMoeCombine` a thread an element, one slot's loads in flight a step
[ |> template_struct_instance]
class template BenchMoeCombineT : GkMoeCombine {
    override WINDOW = false
    override SLOT_X4 = false

    [metal_kernel]
    def bench_moe_combine {
        if (gl_GlobalInvocationID.x < pa.n) {
            comb_elem(gl_WorkGroupID.y, gl_GlobalInvocationID.x)
        }
    }
}

//! `MetalMoeCombine` with one slot's loads in flight a step
[metal_dispatch(name = "bench_enc_moe_combine_x1", pso = "g_bench_pso_moe_combine_x1", tgmem = "BenchMoeCombineX1_bench_moe_combine_msl_tgmem",
    tg = 256, grid = "dim/256, nst", params = "dim : int64, nst : int64")]
class BenchMoeCombineX1 : BenchMoeCombineT {
    override GATHERED = false
}

//! `MetalMoeCombineSh` with one slot's loads in flight a step
[metal_dispatch(name = "bench_enc_moe_combine_sh_x1", pso = "g_bench_pso_moe_combine_sh_x1", tgmem = "BenchMoeCombineShX1_bench_moe_combine_msl_tgmem",
    tg = 256, grid = "dim/256, nst", params = "dim : int64, nst : int64")]
class BenchMoeCombineShX1 : BenchMoeCombineT {
    override GATHERED = false
    override SH = true
}

//! `MetalMoeReduce` with one slot's loads in flight a step
[metal_dispatch(name = "bench_enc_moe_reduce_x1", pso = "g_bench_pso_moe_reduce_x1", tgmem = "BenchMoeReduceX1_bench_moe_combine_msl_tgmem",
    tg = 256, grid = "dim/256, npos", params = "dim : int64, npos : int64")]
class BenchMoeReduceX1 : BenchMoeCombineT {
}

//! `MetalMoeReduceSh` with one slot's loads in flight a step
[metal_dispatch(name = "bench_enc_moe_reduce_sh_x1", pso = "g_bench_pso_moe_reduce_sh_x1", tgmem = "BenchMoeReduceShX1_bench_moe_combine_msl_tgmem",
    tg = 256, grid = "dim/256, npos", params = "dim : int64, npos : int64")]
class BenchMoeReduceShX1 : BenchMoeCombineT {
    override SH = true
}

//! `MetalMoeSelect` with the down-scale arm compiled in, its row read at run time (`DSOFF_SWITCH`)
[metal_dispatch(name = "bench_enc_moe_select_dsw", pso = "g_bench_pso_moe_select_dsw", tgmem = "BenchMoeSelectDsw_metal_moe_select_msl_tgmem",
    tg = 32, grid = "nst", params = "nst : int64")]
class BenchMoeSelectDsw : MetalMoeSelectT {
    override DSCALE = true
}

//! `MetalMoeSelect512` with the down-scale arm compiled in, its row read at run time
[metal_dispatch(name = "bench_enc_moe_select_512_dsw", pso = "g_bench_pso_moe_select_512_dsw", tgmem = "BenchMoeSelect512Dsw_metal_moe_select_msl_tgmem",
    tg = 32, grid = "nst", params = "nst : int64")]
class BenchMoeSelect512Dsw : MetalMoeSelectT {
    override DEPTH = 16
    override DSCALE = true
}

var private g_bench_pso_moe_comb_g4_pf_x4 : MetalComputePipeline?
var private g_bench_pso_moe_comb_g4_x1 : MetalComputePipeline?
var private g_bench_pso_moe_comb_g4_hx_x1 : MetalComputePipeline?
var private g_bench_pso_moe_combine_x1 : MetalComputePipeline?
var private g_bench_pso_moe_combine_sh_x1 : MetalComputePipeline?
var private g_bench_pso_moe_reduce_x1 : MetalComputePipeline?
var private g_bench_pso_moe_reduce_sh_x1 : MetalComputePipeline?
var private g_bench_pso_moe_select_dsw : MetalComputePipeline?
var private g_bench_pso_moe_select_512_dsw : MetalComputePipeline?

//! the MoE race stamps' pipelines; false when one fails to compile
def private bench_moe_psos_init : bool {
    var ok = true
    g_bench_pso_moe_comb_g4_pf_x4 = compile_stamp(BenchMoeCombG4PfX4_bench_moe_comb_g4_msl, ok)
    g_bench_pso_moe_comb_g4_x1 = compile_stamp(BenchMoeCombG4X1_bench_moe_comb_g4_msl, ok)
    g_bench_pso_moe_comb_g4_hx_x1 = compile_stamp(BenchMoeCombG4HxX1_bench_moe_comb_g4_msl, ok)
    g_bench_pso_moe_combine_x1 = compile_stamp(BenchMoeCombineX1_bench_moe_combine_msl, ok)
    g_bench_pso_moe_combine_sh_x1 = compile_stamp(BenchMoeCombineShX1_bench_moe_combine_msl, ok)
    g_bench_pso_moe_reduce_x1 = compile_stamp(BenchMoeReduceX1_bench_moe_combine_msl, ok)
    g_bench_pso_moe_reduce_sh_x1 = compile_stamp(BenchMoeReduceShX1_bench_moe_combine_msl, ok)
    g_bench_pso_moe_select_dsw = compile_stamp(BenchMoeSelectDsw_metal_moe_select_msl, ok)
    g_bench_pso_moe_select_512_dsw = compile_stamp(BenchMoeSelect512Dsw_metal_moe_select_msl, ok)
    return ok
}

def private bench_moe_psos_release {
    release_handles(g_bench_pso_moe_comb_g4_pf_x4, g_bench_pso_moe_comb_g4_x1, g_bench_pso_moe_comb_g4_hx_x1, g_bench_pso_moe_combine_x1,
        g_bench_pso_moe_combine_sh_x1, g_bench_pso_moe_reduce_x1, g_bench_pso_moe_reduce_sh_x1, g_bench_pso_moe_select_dsw,
        g_bench_pso_moe_select_512_dsw)
}

let private COMB_GSTRIDE = 257   //! the shared gate's row stride in the router rows: 256 experts, then the gate logit
let private COMB_GOFF = 256

//! slot j of row p's down row: the selection rows' p k + j, or under `gathered` the bucket inverse's fixture map
def private slot_row_ref(p, j, k, np : int; gathered : bool) : int => gathered ? ((p * k + j) * 7) % (np * k) : p * k + j

//! the bucket inverse's fixture map over `n` slots (`slot_row_ref`'s gathered rows)
def private slot_map_buf(n : int) : MetalBuffer? {
    var buf = metal_new_buffer(g_dev, uint64(n * 4))
    var ip = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    for (i in range(n)) {
        unsafe(ip[i]) = uint((i * 7) % n)
    }
    return buf
}

//! the routed combine's rows after one dispatch, on the CPU in double off the fixture's pattern inputs: row p's k slots under their
//! weights, and under `sh` the shared row under its gate logit's sigmoid
def private comb_ref(dim, k, np : int; gathered, sh : bool) : array<double> {
    var out : array<double>
    out |> reserve(np * dim)
    for (p in range(np)) {
        let gl = double(pat(p * COMB_GSTRIDE + COMB_GOFF, 0.5))
        for (e in range(dim)) {
            var acc = 0.0lf
            for (j in range(k)) {
                acc += double(pat(p * 2 * k + k + j, 0.01)) * double(pat(slot_row_ref(p, j, k, np, gathered) * dim + e, 0.1))
            }
            if (sh) {
                acc += double(pat(p * dim + e, 0.25)) / (1.0lf + exp(-gl))
            }
            out |> push(acc)
        }
    }
    return <- out
}

//! the routed combine's operands at width `dim`, `k` slots a row over `np` rows
struct private CombStep {
    acc : MetalBuffer?     //! the combined rows
    binv : MetalBuffer?    //! row p's slot j's down row at p * k + j (the window's reduce)
    bsel : MetalBuffer?    //! the selection rows: k picks, then k weights
    bdn : MetalBuffer?     //! the slots' down rows
    bsh : MetalBuffer?     //! the shared expert's rows
    bgate : MetalBuffer?   //! the router rows' shared gate logit at `COMB_GOFF`
    kc : GkMoeCombArgs
    dim : int
    np : int
    gathered : bool        //! the window's reduce (`MetalMoeReduce`), else the decode combine (`MetalMoeCombine`)
    sh : bool              //! the shared-expert stamp
}

//! the combine slot race's arm `arm` on `s`: 0 the shipped stamp (`SLOT_X4` on), 1 the bench stamp (off)
def private comb_slot_arm(enc : MetalComputeEncoder?; s : CombStep; arm : int) {
    var k = s.kc
    let dim = int64(s.dim)
    let np = int64(s.np)
    if (s.gathered && s.sh) {
        if (arm == 0) {
            pf_enc_moe_reduce_sh(enc, s.acc, s.binv, s.bsel, s.bdn, s.bsh, s.bgate, k, dim, np)
        } else {
            bench_enc_moe_reduce_sh_x1(enc, s.acc, s.binv, s.bsel, s.bdn, s.bsh, s.bgate, k, dim, np)
        }
    } elif (s.gathered) {
        if (arm == 0) {
            pf_enc_moe_reduce(enc, s.acc, s.binv, s.bsel, s.bdn, k, dim, np)
        } else {
            bench_enc_moe_reduce_x1(enc, s.acc, s.binv, s.bsel, s.bdn, k, dim, np)
        }
    } elif (s.sh) {
        if (arm == 0) {
            enc_moe_combine_sh(enc, s.acc, s.bsel, s.bdn, s.bsh, s.bgate, k, dim, np)
        } else {
            bench_enc_moe_combine_sh_x1(enc, s.acc, s.bsel, s.bdn, s.bsh, s.bgate, k, dim, np)
        }
    } elif (arm == 0) {
        enc_moe_combine(enc, s.acc, s.bsel, s.bdn, k, dim, np)
    } else {
        bench_enc_moe_combine_x1(enc, s.acc, s.bsel, s.bdn, k, dim, np)
    }
}

//! the routed combine at width `dim`, `k` slots over `np` rows, raced with four slot loads in flight a step on - the shipped stamp,
//! the baseline - and off: the decode combine (`MetalMoeCombine` / `Sh`) or under `gathered` the window's reduce (`MetalMoeReduce` / `Sh`)
def private comb_slot_race(dim, k, np : int; gathered, sh : bool; var rows : array<Reading>) {
    var s = CombStep(acc = fill_buf(np * dim, 0.0), binv = slot_map_buf(np * k), bsel = fill_buf(np * 2 * k, 0.01),
        bdn = fill_buf(np * k * dim, 0.1), bsh = fill_buf(np * dim, 0.25), bgate = fill_buf(np * COMB_GSTRIDE, 0.5),
        kc = moe_comb_args(int64(dim), int64(k), int64(COMB_GSTRIDE), int64(COMB_GOFF)), dim = dim, np = np, gathered = gathered, sh = sh)
    let shipped = gathered ? (sh ? "MetalMoeReduceSh" : "MetalMoeReduce") : (sh ? "MetalMoeCombineSh" : "MetalMoeCombine")
    let ref <- comb_ref(dim, k, np, gathered, sh)
    race_rows("{shipped} dim={dim} k={k} {np} rows", ["SLOT_X4 on (the shipped stamp)", "SLOT_X4 off (bench stamp)"], ref, rows, $ {
        fill_pattern(s.acc, np * dim, 0.0)
    }, $(var got) {
        got |> push_from(read_floats(s.acc, np * dim))
    }, $(enc, a) {
        comb_slot_arm(enc, s, a)
    })
    for (buf in fixed_array(s.acc, s.binv, s.bsel, s.bdn, s.bsh, s.bgate)) {
        metal_release(buf)
    }
}

enum private G4Form {
    window      //! `MetalMoeCombG4Pf`: the bucket inverse's slots, no norm out
    decode      //! `MetalMoeCombG4`: the selection rows' slots, the next norm in f32
    decode_hx   //! `MetalMoeCombG4Hx`: ... and its f16 twin
}

//! the gemma-4 step's operands at width `dim`, `k` slots a row over `np` rows, laid out as `g4_tail_race`'s
struct private G4Step {
    bx : MetalBuffer?     //! the stream rows, updated in place
    binv : MetalBuffer?   //! row p's slot j's down row at p * k + j (the window)
    bsel : MetalBuffer?   //! the selection rows: k picks, then k weights
    bdn : MetalBuffer?    //! the slots' down rows
    bd : MetalBuffer?     //! the dense branch's rows
    bw : MetalBuffer?     //! the norms plane: the routed, dense and post-FFN norms, then the next norm
    by : MetalBuffer?     //! the next norm (the decode stamps)
    byh : MetalBuffer?    //! ... its f16 twin (`decode_hx`)
    kp : GkMoeCombArgs
    np : int
    form : G4Form
}

//! the step slot race's arm `arm` on `s`: 0 the shipped stamp, 1 the bench stamp with `SLOT_X4` flipped
def private g4_slot_arm(enc : MetalComputeEncoder?; s : G4Step; arm : int) {
    var k = s.kp
    let np = int64(s.np)
    if (s.form == G4Form.window) {
        if (arm == 0) {
            enc_moe_comb_g4_pf(enc, s.bx, s.binv, s.bsel, s.bdn, s.bd, s.bw, k, np)
        } else {
            bench_enc_moe_comb_g4_pf_x4(enc, s.bx, s.binv, s.bsel, s.bdn, s.bd, s.bw, k, np)
        }
    } elif (s.form == G4Form.decode) {
        if (arm == 0) {
            enc_moe_comb_g4(enc, s.bx, s.bsel, s.bdn, s.bd, s.bw, s.by, k, np)
        } else {
            bench_enc_moe_comb_g4_x1(enc, s.bx, s.bsel, s.bdn, s.bd, s.bw, s.by, k, np)
        }
    } elif (arm == 0) {
        enc_moe_comb_g4_hx(enc, s.bx, s.bsel, s.bdn, s.bd, s.bw, s.by, s.byh, k, np)
    } else {
        bench_enc_moe_comb_g4_hx_x1(enc, s.bx, s.bsel, s.bdn, s.bd, s.bw, s.by, s.byh, k, np)
    }
}

//! rsqrt(mean + eps) of the squares of `v`, in double
def private ref_rms_inv(v : array<double>; eps : double) : double {
    var ss = 0.0lf
    for (e in v) {
        ss += e * e
    }
    return 1.0lf / sqrt(ss / double(length(v)) + eps)
}

//! the step's stream rows after one dispatch, on the CPU in double off the fixture's pattern inputs
def private g4_step_ref(dim, k, np : int; ascale : double; gathered : bool) : array<double> {
    var out : array<double>
    out |> reserve(np * dim)
    for (p in range(np)) {
        var inscope rv <- [for (_e in range(dim)); 0.0lf]
        for (j in range(k)) {
            let wj = double(pat(p * 2 * k + k + j, 0.01))
            let row = slot_row_ref(p, j, k, np, gathered)
            for (e in range(dim)) {
                rv[e] += wj * double(pat(row * dim + e, 0.1))
            }
        }
        var inscope dv <- [for (e in range(dim)); double(pat(p * dim + e, 0.25))]
        let inv_r = ref_rms_inv(rv, 1e-6lf)
        let inv_d = ref_rms_inv(dv, 1e-6lf)
        var inscope cv <- [for (e in range(dim)); double(pat(e, 0.01)) * rv[e] * inv_r + double(pat(dim + e, 0.01)) * dv[e] * inv_d]
        let inv_c = ref_rms_inv(cv, 1e-6lf)
        for (e in range(dim)) {
            out |> push((double(pat(p * dim + e, 0.5)) + double(pat(2 * dim + e, 0.01)) * cv[e] * inv_c) * ascale)
        }
    }
    return <- out
}

//! the gemma-4 MoE step at the 26B's shape (dim 2816, eight slots) over `np` rows in its stamp's `form`, raced with four slot loads
//! in flight a lane on and off - the shipped stamp the baseline (off on the window, on at the decode rows); the compare region is
//! the stream rows the step updates
def private g4_slot_race(np : int; form : G4Form; var rows : array<Reading>) {
    let dim = 2816
    let k = 8
    var s = G4Step(bx = fill_buf(np * dim, 0.5), binv = slot_map_buf(np * k), bsel = fill_buf(np * 2 * k, 0.01),
        bdn = fill_buf(np * k * dim, 0.1), bd = fill_buf(np * dim, 0.25), bw = fill_buf(4 * dim, 0.01), by = fill_buf(np * dim, 0.0),
        byh = metal_new_buffer(g_dev, uint64(np * dim * 2)), kp = moe_g4_args(int64(dim), int64(k), 1e-6, 0.75, false), np = np, form = form)
    let win = form == G4Form.window
    let shipped = win ? "MetalMoeCombG4Pf" : (form == G4Form.decode ? "MetalMoeCombG4" : "MetalMoeCombG4Hx")
    let names <- (win ? ["SLOT_X4 off (the shipped stamp)", "SLOT_X4 on (bench stamp)"]
        : ["SLOT_X4 on (the shipped stamp)", "SLOT_X4 off (bench stamp)"])
    let ref <- g4_step_ref(dim, k, np, 0.75lf, win)
    race_rows("g4 MoE step {shipped} dim={dim} k={k} {np} rows", names, ref, rows, $ {
        fill_pattern(s.bx, np * dim, 0.5)
    }, $(var got) {
        got |> push_from(read_floats(s.bx, np * dim))
    }, $(enc, a) {
        g4_slot_arm(enc, s, a)
    })
    for (buf in fixed_array(s.bx, s.binv, s.bsel, s.bdn, s.bd, s.bw, s.by, s.byh)) {
        metal_release(buf)
    }
}

//! one router row's select on the CPU in double off the fixture's logits (pattern scale 1): the k picks by softmax probability, the
//! lowest expert on a tie, then their weights renormalized over the picks
def private select_ref(ne, k : int) : array<double> {
    var inscope used <- [for (_e in range(ne)); false]
    var picks, lw : array<double>
    picks |> reserve(2 * k)
    lw |> reserve(k)
    for (_j in range(k)) {
        var best = -1
        for (e in range(ne)) {
            if (!used[e] && (best < 0 || pat(e, 1.0) > pat(best, 1.0))) {
                best = e
            }
        }
        used[best] = true
        picks |> push(double(best))
        lw |> push(double(pat(best, 1.0)))
    }
    var sw = 0.0lf
    for (l in lw) {
        sw += exp(l - lw[0])
    }
    for (l in lw) {
        picks |> push(exp(l - lw[0]) / sw)
    }
    return <- picks
}

//! the select's operands over one router row of `ne` logits
struct private SelStep {
    blog : MetalBuffer?   //! the router row
    bsel : MetalBuffer?   //! the selection row: k picks, then k weights
    bds : MetalBuffer?    //! the down-scale row the bench stamps bind and, with no row named, never read
    ka : GkTopKArgs
}

//! the select race's arm `arm` on `s`: 0 the shipped plain stamp, 1 the bench stamp with the down-scale arm compiled in
def private select_arm(enc : MetalComputeEncoder?; s : SelStep; arm : int) {
    var k = s.ka
    if (s.ka.ne > 256u) {
        if (arm == 0) {
            enc_moe_select_512(enc, s.blog, s.bsel, s.bsel, k, 1l)
        } else {
            bench_enc_moe_select_512_dsw(enc, s.blog, s.bsel, s.bsel, s.bds, k, 1l)
        }
    } elif (arm == 0) {
        enc_moe_select(enc, s.blog, s.bsel, s.bsel, k, 1l)
    } else {
        bench_enc_moe_select_dsw(enc, s.blog, s.bsel, s.bsel, s.bds, k, 1l)
    }
}

//! the router select over one row of `ne` experts, `k` picks, raced on its plain stamp - the shipped one, the baseline - against the
//! stamp with the down-scale arm compiled in and no row named; the compare region is the picks, then their weights
def private select_ds_race(ne, k : int; var rows : array<Reading>) {
    var s = SelStep(blog = fill_buf(ne, 1.0), bsel = fill_buf(2 * k, 0.0), bds = fill_buf(ne, 1.0),
        ka = GkTopKArgs(ne = uint(ne), k = uint(k), norm = 1u, lstride = uint(ne), dsoff = GK_NO_DSCALE))
    let ref <- select_ref(ne, k)
    race_rows("moe_select ne={ne} k={k} 1 row", ["the shipped plain stamp", "the down-scale arm compiled in, no row named (bench stamp)"],
            ref, rows, $ {
        fill_pattern(s.bsel, 2 * k, 0.0)
    }, $(var got) {
        let ip = unsafe(reinterpret<uint?>(metal_buffer_contents(s.bsel)))
        let fp = unsafe(reinterpret<float?>(ip))
        got |> reserve(2 * k)
        for (j in range(2 * k)) {
            got |> push(j < k ? float(unsafe(ip[j])) : unsafe(fp[j]))
        }
    }, $(enc, a) {
        select_arm(enc, s, a)
    })
    for (buf in fixed_array(s.blog, s.bsel, s.bds)) {
        metal_release(buf)
    }
}

//! the MoE races: the gemma-4 step's and the routed combine's slot loads at the decode rows (one, a verify's four) and the 512-row
//! window, and the select's down-scale arm
def private moe_race_rows : array<Reading> {
    var rows : array<Reading>
    if (!bench_moe_psos_init()) {
        rows |> push((us = -1.0lf, cv = 1.0lf))
        return <- rows
    }
    rows |> reserve(26)
    for (np in fixed_array(1, 4)) {
        g4_slot_race(np, G4Form.decode, rows)
        g4_slot_race(np, G4Form.decode_hx, rows)
        comb_slot_race(2048, 8, np, false, false, rows)
        comb_slot_race(2048, 8, np, false, true, rows)
    }
    g4_slot_race(512, G4Form.window, rows)
    comb_slot_race(2048, 8, 512, true, false, rows)
    comb_slot_race(2048, 8, 512, true, true, rows)
    select_ds_race(256, 8, rows)
    select_ds_race(512, 10, rows)
    bench_moe_psos_release()
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

//! the lane race stamps' shell: the sandwich seam stamped as `MetalPreAddRms` is - its loops striding the threadgroup's own size, so
//! the threadgroup is the lane width - and dispatched at 128 lanes
[ |> template_struct_instance]
class template BenchPreAddRmsT : MetalAddRmsT {
    override PRE = true
    override PRE_SWITCH = false
    override OUT_F32 = true

    [metal_kernel]
    def bench_pre_add_rms {
        add_rms_row(gl_WorkGroupID.x)
    }
}

//! `MetalPreAddRms` at 128 and 1024 lanes
[metal_dispatch(name = "bench_enc_preaddrms_b_128", pso = "g_bench_pso_preaddrms", tgmem = "BenchPreAddRms_bench_pre_add_rms_msl_tgmem", tg = 128,
    grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "bench_enc_preaddrms_b_1024", pso = "g_bench_pso_preaddrms", tgmem = "BenchPreAddRms_bench_pre_add_rms_msl_tgmem",
    tg = 1024, grid = "nrows", params = "nrows : int64")]
class BenchPreAddRms : BenchPreAddRmsT {
}

//! `MetalPreAddRmsHX` at 128 and 1024 lanes
[metal_dispatch(name = "bench_enc_preaddrms_bhx_128", pso = "g_bench_pso_preaddrms_hx", tgmem = "BenchPreAddRmsHX_bench_pre_add_rms_msl_tgmem",
    tg = 128, grid = "nrows", params = "nrows : int64"),
    metal_dispatch(name = "bench_enc_preaddrms_bhx_1024", pso = "g_bench_pso_preaddrms_hx", tgmem = "BenchPreAddRmsHX_bench_pre_add_rms_msl_tgmem",
    tg = 1024, grid = "nrows", params = "nrows : int64")]
class BenchPreAddRmsHX : BenchPreAddRmsT {
    override HX = true
}

//! `MetalPreAddRms` with a slab as wide as the 31B's row (`WIDE_SLAB`), at 256 lanes: the whole row staged, no column re-read
//! joins `GkAddRmsRows`, the template both homes stamp
[metal_dispatch(name = "bench_enc_preaddrms_b_wide", pso = "g_bench_pso_preaddrms_wide", tgmem = "BenchPreAddRmsWide_bench_pre_add_rms_msl_tgmem",
    tg = 256, grid = "nrows", params = "nrows : int64")]
class BenchPreAddRmsWide : BenchPreAddRmsT {
    override SLAB = 5376
}

//! `MetalPreAddRmsHX` with the wide slab; joins `GkAddRmsRows`, the template both homes stamp
[metal_dispatch(name = "bench_enc_preaddrms_bhx_wide", pso = "g_bench_pso_preaddrms_hx_wide",
    tgmem = "BenchPreAddRmsHXWide_bench_pre_add_rms_msl_tgmem", tg = 256, grid = "nrows", params = "nrows : int64")]
class BenchPreAddRmsHXWide : BenchPreAddRmsT {
    override SLAB = 5376
    override HX = true
}

let private WIDE_SLAB = 5376   //! the wide bench stamps' slab, the row it serves whole

var private g_bench_pso_preaddrms : MetalComputePipeline?
var private g_bench_pso_preaddrms_hx : MetalComputePipeline?
var private g_bench_pso_preaddrms_wide : MetalComputePipeline?
var private g_bench_pso_preaddrms_hx_wide : MetalComputePipeline?

//! the sandwich seam's operands at width `dim` over `np` rows
struct private SeamStep {
    bx : MetalBuffer?   //! the stream rows, updated in place
    ba : MetalBuffer?   //! the branch rows, normed under the post norm before the add
    by : MetalBuffer?   //! the next norm, f32
    bh : MetalBuffer?   //! ... its f16 twin (`hx`)
    bw : MetalBuffer?   //! the norms plane: the post norm, then the next norm at `dim`
    ks : GkAddRmsArgs
    np : int
    hx : bool           //! the f16 twin's pipeline (`MetalPreAddRmsHX`), else `MetalPreAddRms`
}

//! the lane race's arm at `lanes` on `s`'s pipeline: 256 the shipped builders (the window's, and the decode rows' within the slab -
//! the one-row grid at one row), 1024 the decode rows' shipped builder past the slab, else the bench stamp, 128 the bench stamp
def private seam_lane_arm(enc : MetalComputeEncoder?; s : SeamStep; lanes : int) {
    var k = s.ks
    let np = int64(s.np)
    let win = s.np >= 64
    if (lanes == 1024 && !win && int64(k.dim) > ADD_RMS_MAX_DIM) {
        enc_residual_wide(enc, s.bx, s.ba, s.bw, s.by, s.hx ? s.bh : null, true, true, k, np)
    } elif (s.hx) {
        if (lanes == 128) {
            bench_enc_preaddrms_bhx_128(enc, s.bx, s.ba, s.bw, s.by, s.bh, k, np)
        } elif (lanes == 1024) {
            bench_enc_preaddrms_bhx_1024(enc, s.bx, s.ba, s.bw, s.by, s.bh, k, np)
        } elif (win) {
            pf_enc_preaddrms_bhx_c(enc, s.bx, s.ba, s.bw, s.by, s.bh, k, np)
        } else {
            enc_preaddrms_bhx_c(enc, s.bx, s.ba, s.bw, s.by, s.bh, k, np)
        }
    } elif (lanes == 128) {
        bench_enc_preaddrms_b_128(enc, s.bx, s.ba, s.bw, s.by, k, np)
    } elif (lanes == 1024) {
        bench_enc_preaddrms_b_1024(enc, s.bx, s.ba, s.bw, s.by, k, np)
    } elif (win) {
        pf_enc_preaddrms_b_c(enc, s.bx, s.ba, s.bw, s.by, k, np)
    } elif (s.np == 1) {
        enc_preaddrms_c(enc, s.bx, s.ba, s.bw, s.by, k)
    } else {
        enc_preaddrms_b_c(enc, s.bx, s.ba, s.bw, s.by, k, np)
    }
}

//! the seam's stream rows then its f32 next norm after one dispatch, on the CPU in double off the fixture's pattern inputs
def private seam_ref(dim, np : int; ascale : double) : array<double> {
    var xs, ys : array<double>
    xs |> reserve(np * dim)
    ys |> reserve(np * dim)
    for (r in range(np)) {
        var inscope av <- [for (e in range(dim)); double(pat(r * dim + e, 0.25))]
        let ainv = ref_rms_inv(av, 1e-6lf)
        var inscope vv <- [for (e in range(dim)); (double(pat(e, 0.01)) * av[e] * ainv + double(pat(r * dim + e, 0.5))) * ascale]
        let inv = ref_rms_inv(vv, 1e-6lf)
        for (e in range(dim)) {
            xs |> push(vv[e])
            ys |> push(double(pat(dim + e, 0.01)) * vv[e] * inv)
        }
    }
    xs |> push_from(ys)
    return <- xs
}

//! the sandwich seam (pre_add_rms; `hx` its f16 twin) at width `dim` over `np` rows, raced at 1024, 256 and 128 lanes on the same
//! pipeline - the shipped builder for the row count and width the baseline; the compare region is the stream rows and the f32 next
//! norm
def private seam_lane_race(dim, np : int; hx : bool; var rows : array<Reading>) {
    let n = np * dim
    var s = SeamStep(bx = fill_buf(n, 0.5), ba = fill_buf(n, 0.25), by = fill_buf(n, 0.0), bh = metal_new_buffer(g_dev, uint64(n * 2)),
        bw = fill_buf(2 * dim, 0.01), ks = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(dim), eps = 1e-6, ascale = 0.75, pre_on = 1u,
        pwoff = 0u), np = np, hx = hx)
    let win = np >= 64
    let past = !win && int64(dim) > ADD_RMS_MAX_DIM
    let lanes = past ? fixed_array(1024, 256, 128) : fixed_array(256, 128, 1024)
    let names <- (past ? ["1024 lanes (the shipped decode builder past the slab)", "256 lanes (the decode builder within the slab)",
        "128 lanes (bench stamp)"] : [win ? "256 lanes (the shipped window builder)" : "256 lanes (the shipped decode builder)",
        "128 lanes (bench stamp)", "1024 lanes (bench stamp)"])
    let ref <- seam_ref(dim, np, 0.75lf)
    race_rows("ar dim={dim} {np} rows sandwich seam pre_add_rms{hx ? " hx" : ""}", names, ref, rows, $ {
        fill_pattern(s.bx, n, 0.5)
    }, $(var got) {
        got |> push_from(read_floats(s.bx, n), read_floats(s.by, n))
    }, $(enc, a) {
        seam_lane_arm(enc, s, lanes[a])
    })
    for (buf in fixed_array(s.bx, s.ba, s.by, s.bh, s.bw)) {
        metal_release(buf)
    }
}

//! the lane race at the gemma-3-12b, E4B and gemma-4 31B widths: the decode rows (one, a verify's four) on the plain pipeline, the
//! 512-row window on the f16 twin's
def private seam_lane_rows : array<Reading> {
    var rows : array<Reading>
    if (!seam_bench_psos_init()) {
        rows |> push((us = -1.0lf, cv = 1.0lf))
        return <- rows
    }
    rows |> reserve(36)
    for (dim in fixed_array(1152, 2560, 3840, 5376)) {
        seam_lane_race(dim, 1, false, rows)
        seam_lane_race(dim, 4, false, rows)
        seam_lane_race(dim, 512, true, rows)
    }
    seam_bench_psos_release()
    return <- rows
}

//! the seam's bench stamps' pipelines (the lane stamps, the wide-slab stamps); false when one fails to compile
def private seam_bench_psos_init : bool {
    var ok = true
    g_bench_pso_preaddrms = compile_stamp(BenchPreAddRms_bench_pre_add_rms_msl, ok)
    g_bench_pso_preaddrms_hx = compile_stamp(BenchPreAddRmsHX_bench_pre_add_rms_msl, ok)
    g_bench_pso_preaddrms_wide = compile_stamp(BenchPreAddRmsWide_bench_pre_add_rms_msl, ok)
    g_bench_pso_preaddrms_hx_wide = compile_stamp(BenchPreAddRmsHXWide_bench_pre_add_rms_msl, ok)
    return ok
}

def private seam_bench_psos_release {
    release_handles(g_bench_pso_preaddrms, g_bench_pso_preaddrms_hx, g_bench_pso_preaddrms_wide, g_bench_pso_preaddrms_hx_wide)
}

//! a one-float buffer holding `v` (a residual add's scale)
def private scale_buf(v : float) : MetalBuffer? {
    var buf = metal_new_buffer(g_dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

//! the norms plane's row `r` at width `dim` alone in a buffer: the fixture's pattern from element r x dim on (the unfused chain's
//! norms, which bind a row at element zero)
def private norm_row_buf(dim, r : int) : MetalBuffer? {
    var buf = metal_new_buffer(g_dev, uint64(dim * 4))
    fill_pattern(buf, dim, 0.01, r * dim)
    return buf
}

//! the gemma-4 MoE tail's unfused chain's scratch beside the step's operands
struct private G4Tail {
    brt : MetalBuffer?    //! the routed rows, then their post norm
    bt : MetalBuffer?     //! the dense post norm, the branch sum, then its post-FFN norm
    bwr : MetalBuffer?    //! the routed post norm row
    bwd : MetalBuffer?    //! the dense post norm row
    bws : MetalBuffer?    //! the post-FFN norm row
    bwn : MetalBuffer?    //! the next norm row
    bosc : MetalBuffer?   //! the residual's scale (the window's add)
    udim : MetalBuffer?
    ueps : MetalBuffer?
    utot : MetalBuffer?
    kc : GkMoeCombArgs
}

//! the gemma-4 MoE tail's unfused chain on `s` with scratch `u`: the routed combine (the window: the reduce), the routed and dense
//! post norms, their add, the post-FFN norm, then the residual add with the next norm (the window: the scaled add alone)
def private g4_unfused_arm(enc : MetalComputeEncoder?; s : G4Step; u : G4Tail) {
    var kc = u.kc
    let dim = int64(s.kp.n)
    let np = int64(s.np)
    if (s.form == G4Form.window) {
        pf_enc_moe_reduce(enc, u.brt, s.binv, s.bsel, s.bdn, kc, dim, np)
        pf_enc_rms(enc, u.brt, 0ul, u.bwr, u.brt, u.udim, u.ueps, np)
        pf_enc_rms(enc, s.bd, 0ul, u.bwd, u.bt, u.udim, u.ueps, np)
        enc_add(enc, u.bt, 0ul, u.brt, 0ul, u.utot, np * dim)
        pf_enc_rms(enc, u.bt, 0ul, u.bws, u.bt, u.udim, u.ueps, np)
        enc_add(enc, s.bx, 0ul, u.bt, 0ul, u.utot, np * dim, u.bosc)
        return
    }
    var kr = GkAddRmsArgs(dim = uint(dim), add_on = 1u, eps = 1e-6, ascale = s.kp.ascale)
    enc_moe_combine(enc, u.brt, s.bsel, s.bdn, kc, dim, np)
    enc_rms_b(enc, u.brt, 0ul, u.bwr, u.brt, u.udim, u.ueps, np)
    enc_rms_b(enc, s.bd, 0ul, u.bwd, u.bt, u.udim, u.ueps, np)
    enc_add(enc, u.bt, 0ul, u.brt, 0ul, u.utot, np * dim)
    enc_rms_b(enc, u.bt, 0ul, u.bws, u.bt, u.udim, u.ueps, np)
    enc_add_rms_b_c(enc, s.bx, u.bt, u.bwn, s.by, kr, np)
}

//! the gemma-4 MoE layer's tail at the 26B's shape (dim 2816, eight slots) over `np` rows: the one step - `MetalMoeCombG4` at the
//! decode rows, `MetalMoeCombG4Pf` on the window, the shipped form and the baseline - raced against the unfused chain the engine
//! serves under `set_metal_moe_g4_unfused`; the compare region is the stream rows the tail updates
def private g4_tail_race(np : int; var rows : array<Reading>) {
    let dim = 2816
    let k = 8
    let form = np >= 64 ? G4Form.window : G4Form.decode
    var s = G4Step(bx = fill_buf(np * dim, 0.5), binv = slot_map_buf(np * k), bsel = fill_buf(np * 2 * k, 0.01),
        bdn = fill_buf(np * k * dim, 0.1), bd = fill_buf(np * dim, 0.25), bw = fill_buf(4 * dim, 0.01), by = fill_buf(np * dim, 0.0),
        byh = metal_new_buffer(g_dev, uint64(np * dim * 2)), kp = moe_g4_args(int64(dim), int64(k), 1e-6, 0.75, false), np = np, form = form)
    var u = G4Tail(brt = fill_buf(np * dim, 0.0), bt = fill_buf(np * dim, 0.0), bwr = norm_row_buf(dim, 0), bwd = norm_row_buf(dim, 1),
        bws = norm_row_buf(dim, 2), bwn = norm_row_buf(dim, 3), bosc = scale_buf(0.75), udim = uniform_u32(uint(dim)),
        ueps = uniform_f32(1e-6), utot = uniform_u32(uint(np * dim)), kc = moe_comb_args(int64(dim), int64(k)))
    let ref <- g4_step_ref(dim, k, np, 0.75lf, form == G4Form.window)
    race_rows("g4 MoE tail dim={dim} k={k} {np} rows", ["the one step (the shipped form)", "the unfused chain (6 dispatches)"], ref, rows, $ {
        fill_pattern(s.bx, np * dim, 0.5)
    }, $(var got) {
        got |> push_from(read_floats(s.bx, np * dim))
    }, $(enc, a) {
        if (a == 0) {
            g4_slot_arm(enc, s, 0)
        } else {
            g4_unfused_arm(enc, s, u)
        }
    })
    for (buf in fixed_array(s.bx, s.binv, s.bsel, s.bdn, s.bd, s.bw, s.by, s.byh, u.brt, u.bt, u.bwr, u.bwd, u.bws, u.bwn, u.bosc)) {
        metal_release(buf)
    }
    for (b in fixed_array(u.udim, u.ueps, u.utot)) {
        pool_release(g_upool, b, 4ul)
    }
}

//! the gemma-4 MoE tail race at a decode row and over the 512-row window
def private g4_tail_rows : array<Reading> {
    var rows : array<Reading>
    rows |> reserve(4)
    g4_tail_race(1, rows)
    g4_tail_race(512, rows)
    return <- rows
}

enum private ArForm {
    plain    //! the add + norm: x = x + a, its next norm out
    seam     //! the sandwich seam: x = (x + post(a)) . ascale, its next norm out
    nonorm   //! the seam with no norm stored
}

//! the residual step's operands at width `dim` over `np` rows, laid out as `SeamStep`'s, with the split chain's scratch
struct private ArStep {
    st : SeamStep
    bt : MetalBuffer?     //! the branch under its post norm (the split chain)
    bwp : MetalBuffer?    //! the post norm row alone
    bwn : MetalBuffer?    //! the next norm row alone
    bosc : MetalBuffer?   //! the residual's scale (the split no-norm seam's add)
    udim : MetalBuffer?
    ueps : MetalBuffer?
    utot : MetalBuffer?
    form : ArForm
}

//! the residual race's arms
enum private ArArm {
    fused    //! the one dispatch, the shipped form
    split    //! the branch's rms, then the add + norm (`nonorm`: the scaled add; `plain`: the add, then the norm)
    split3   //! the split chain `enc_residual` serves split: the branch's rms, the scaled add, the next norm
    wide     //! the one dispatch on the wide-slab bench stamp (`WIDE_SLAB`)
    lanes256 //! past the slab, the decode rows' one dispatch on the 256-lane builders the rows within the slab take
}

//! the next norm of `bx`'s rows into `by` (`bh` its f16 twin where `hx`) in the shape `enc_rms_form` dispatches it
def private ar_next_norm(enc : MetalComputeEncoder?; s : ArStep) {
    let t = s.st
    let np = int64(t.np)
    if (t.np >= 64) {
        pf_enc_rms_hx(enc, t.bx, 0ul, s.bwn, t.by, s.udim, s.ueps, t.bh, np)
    } elif (t.hx) {
        enc_rms_bhx(enc, t.bx, 0ul, s.bwn, t.by, s.udim, s.ueps, t.bh, np)
    } elif (t.np == 1) {
        enc_rms(enc, t.bx, 0ul, s.bwn, t.by, s.udim, s.ueps)
    } else {
        enc_rms_b(enc, t.bx, 0ul, s.bwn, t.by, s.udim, s.ueps, np)
    }
}

//! the add + norm in one dispatch: `ba`'s rows added onto the stream rows, the next norm under `bw` into `by` (`bh` its twin where
//! `hx`); `single` takes the one-row grid at one row
def private ar_add_rms(enc : MetalComputeEncoder?; s : ArStep; ba, bw : MetalBuffer?; var k : GkAddRmsArgs; single : bool) {
    let t = s.st
    let np = int64(t.np)
    if (t.np >= 64) {
        pf_enc_add_rms_bhx_c(enc, t.bx, ba, bw, t.by, t.bh, k, np)
    } elif (t.hx) {
        enc_add_rms_bhx_c(enc, t.bx, ba, bw, t.by, t.bh, k, np)
    } elif (single && t.np == 1) {
        enc_addrms_c(enc, t.bx, ba, bw, t.by, k)
    } else {
        enc_add_rms_b_c(enc, t.bx, ba, bw, t.by, k, np)
    }
}

//! the split arms on `s`: the branch's rms, then the add + norm (`split`), the scaled add and the next norm (`split3`), or the scaled
//! add alone (`nonorm`)
def private ar_split_arm(enc : MetalComputeEncoder?; s : ArStep; arm : ArArm; var k : GkAddRmsArgs) {
    let t = s.st
    let np = int64(t.np)
    let n = np * int64(k.dim)
    if (t.np >= 64) {
        pf_enc_rms(enc, t.ba, 0ul, s.bwp, s.bt, s.udim, s.ueps, np)
    } elif (t.np == 1 && arm == ArArm.split3) {
        enc_rms(enc, t.ba, 0ul, s.bwp, s.bt, s.udim, s.ueps)
    } else {
        enc_rms_b(enc, t.ba, 0ul, s.bwp, s.bt, s.udim, s.ueps, np)
    }
    if (s.form == ArForm.nonorm || arm == ArArm.split3) {
        enc_add(enc, t.bx, 0ul, s.bt, 0ul, s.utot, n, s.bosc)
        if (arm == ArArm.split3) {
            ar_next_norm(enc, s)
        }
    } else {
        ar_add_rms(enc, s, s.bt, s.bwn, k, false)
    }
}

//! the residual race's arm `arm` on `s`
def private ar_arm(enc : MetalComputeEncoder?; s : ArStep; arm : ArArm) {
    var k = s.st.ks
    let np = int64(s.st.np)
    let win = s.st.np >= 64
    let one = s.st.np == 1
    let t = s.st
    if (s.form == ArForm.plain) {
        k.ascale = 1.0
    }
    if (arm == ArArm.fused && !win && int64(k.dim) > ADD_RMS_MAX_DIM) {
        let plain = s.form == ArForm.plain
        enc_residual_wide(enc, t.bx, t.ba, plain ? s.bwn : t.bw, t.by, t.hx ? t.bh : null, !plain, s.form != ArForm.nonorm, k, np)
    } elif (s.form == ArForm.plain && arm == ArArm.split) {
        enc_add(enc, t.bx, 0ul, t.ba, 0ul, s.utot, np * int64(k.dim))
        ar_next_norm(enc, s)
    } elif (s.form == ArForm.plain) {
        ar_add_rms(enc, s, t.ba, s.bwn, k, true)
    } elif (arm == ArArm.wide) {
        if (t.hx) {
            bench_enc_preaddrms_bhx_wide(enc, t.bx, t.ba, t.bw, t.by, t.bh, k, np)
        } else {
            bench_enc_preaddrms_b_wide(enc, t.bx, t.ba, t.bw, t.by, k, np)
        }
    } elif (arm != ArArm.fused && arm != ArArm.lanes256) {
        ar_split_arm(enc, s, arm, k)
    } elif (s.form == ArForm.seam) {
        if (win) {
            pf_enc_preaddrms_bhx_c(enc, t.bx, t.ba, t.bw, t.by, t.bh, k, np)
        } elif (t.hx) {
            enc_preaddrms_bhx_c(enc, t.bx, t.ba, t.bw, t.by, t.bh, k, np)
        } elif (one) {
            enc_preaddrms_c(enc, t.bx, t.ba, t.bw, t.by, k)
        } else {
            enc_preaddrms_b_c(enc, t.bx, t.ba, t.bw, t.by, k, np)
        }
    } elif (win) {
        pf_enc_preadd_b_c(enc, t.bx, t.ba, t.bw, k, np)
    } elif (one) {
        enc_preadd_c(enc, t.bx, t.ba, t.bw, k)
    } else {
        enc_preadd_b_c(enc, t.bx, t.ba, t.bw, k, np)
    }
}

//! the race's arms at width `dim` in `form`, each with its row name: within the slab, the one dispatch against the pair it fuses;
//! past it, against the split chain the step served there before the slab's re-read, and the wide-slab stamp where it holds the row
struct private ArRace {
    arms : array<ArArm>
    names : array<string>
}

def private ar_arms(dim, np : int; form : ArForm) : ArRace {
    let past = int64(dim) > ADD_RMS_MAX_DIM
    var arms : array<ArArm>
    var names : array<string>
    if (form == ArForm.plain) {
        arms |> push(ArArm.fused)
        names |> push("add + norm, one dispatch (the shipped form)")
        if (past) {
            arms |> push(ArArm.split)
            names |> push("add + rms (the split chain)")
        }
    } elif (form == ArForm.nonorm) {
        arms |> push_from([ArArm.fused, ArArm.split])
        names |> push_from(["pre_add, one dispatch (the shipped form)", "rms + add{past ? " (the split chain)" : ""}"])
    } elif (!past) {
        arms |> push_from([ArArm.fused, ArArm.split])
        names |> push_from(["pre_add_rms, one dispatch (the shipped form)", "rms + add_rms"])
    } else {
        arms |> push_from([ArArm.fused, ArArm.split3])
        names |> push_from(["pre_add_rms, one dispatch (the shipped form: the slab, the columns past it re-read)", "rms + add + rms (the split chain)"])
        if (dim <= WIDE_SLAB) {
            arms |> push(ArArm.wide)
            names |> push("pre_add_rms on a {WIDE_SLAB}-float slab (bench stamp)")
        }
    }
    if (past && np < 64) {
        arms |> push(ArArm.lanes256)
        names |> push("one dispatch on the 256-lane decode builder")
    }
    return <- ArRace(arms <- arms, names <- names)
}

//! the plain step's stream rows then its next norm after one dispatch, on the CPU in double off the fixture's pattern inputs
def private ar_plain_ref(dim, np : int) : array<double> {
    var xs, ys : array<double>
    xs |> reserve(np * dim)
    ys |> reserve(np * dim)
    for (r in range(np)) {
        var inscope vv <- [for (e in range(dim)); double(pat(r * dim + e, 0.25)) + double(pat(r * dim + e, 0.5))]
        let inv = ref_rms_inv(vv, 1e-6lf)
        for (e in range(dim)) {
            xs |> push(vv[e])
            ys |> push(double(pat(dim + e, 0.01)) * vv[e] * inv)
        }
    }
    xs |> push_from(ys)
    return <- xs
}

//! the residual step at width `dim` over `np` rows in `form` (`ar_arms`): the plain add + norm, the sandwich seam and its no-norm
//! form, the one dispatch - the shipped form, the baseline, checked against the CPU - raced against the split chain; the window's
//! rows ride the f16 twin's pipelines, the decode rows too under `twin` (a rows site that carries the twin). The compare region
//! is the stream rows and, where the step stores one, the f32 next norm
def private ar_race(dim, np : int; form : ArForm; twin : bool; var rows : array<Reading>) {
    let n = np * dim
    var s = ArStep(st = SeamStep(bx = fill_buf(n, 0.5), ba = fill_buf(n, 0.25), by = fill_buf(n, 0.0), bh = metal_new_buffer(g_dev, uint64(n * 2)),
        bw = fill_buf(2 * dim, 0.01), ks = GkAddRmsArgs(dim = uint(dim), add_on = 1u, woff = uint(dim), eps = 1e-6, ascale = 0.75, pre_on = 1u,
        pwoff = 0u), np = np, hx = np >= 64 || twin), bt = fill_buf(n, 0.0), bwp = norm_row_buf(dim, 0), bwn = norm_row_buf(dim, 1),
        bosc = scale_buf(0.75), udim = uniform_u32(uint(dim)), ueps = uniform_f32(1e-6), utot = uniform_u32(uint(n)), form = form)
    var ref <- (form == ArForm.plain ? ar_plain_ref(dim, np) : seam_ref(dim, np, 0.75lf))
    let race <- ar_arms(dim, np, form)
    if (form == ArForm.nonorm) {
        ref |> resize(n)
    }
    let what = form == ArForm.plain ? "add + norm" : (form == ArForm.seam ? "sandwich seam" : "sandwich seam no norm out")
    race_rows("ar dim={dim} {np} rows {what}{s.st.hx ? " hx" : ""}", race.names, ref, rows, $ {
        fill_pattern(s.st.bx, n, 0.5)
    }, $(var got) {
        got |> push_from(read_floats(s.st.bx, n))
        if (form != ArForm.nonorm) {
            got |> push_from(read_floats(s.st.by, n))
        }
    }, $(enc, a) {
        ar_arm(enc, s, race.arms[a])
    })
    for (buf in fixed_array(s.st.bx, s.st.ba, s.st.by, s.st.bh, s.st.bw, s.bt, s.bwp, s.bwn, s.bosc)) {
        metal_release(buf)
    }
    for (b in fixed_array(s.udim, s.ueps, s.utot)) {
        pool_release(g_upool, b, 4ul)
    }
}

//! the residual step at the gemma-3-12b and E4B widths, past the slab at the two widths a stocked model could take next above
//! it, the gemma-4 31B's and twice the slab: the plain step at a decode row and a verify's four (and the 512-row window past the
//! slab), the sandwich seam and its no-norm form there and over the window; past the slab the plain step and the seam again on
//! the decode rows with the f16 twin beside
def private ar_rows : array<Reading> {
    var rows : array<Reading>
    if (!seam_bench_psos_init()) {
        rows |> push((us = -1.0lf, cv = 1.0lf))
        return <- rows
    }
    rows |> reserve(160)
    for (dim in fixed_array(1152, 2560, 3840, 4608, 5120, 5376, 8192)) {
        let past = int64(dim) > ADD_RMS_MAX_DIM
        for (np in fixed_array(1, 4)) {
            ar_race(dim, np, ArForm.plain, false, rows)
        }
        if (past) {
            ar_race(dim, 512, ArForm.plain, false, rows)
        }
        for (np in fixed_array(1, 4, 512)) {
            ar_race(dim, np, ArForm.seam, false, rows)
            ar_race(dim, np, ArForm.nonorm, false, rows)
        }
        if (past) {
            for (np in fixed_array(1, 4)) {
                ar_race(dim, np, ArForm.plain, true, rows)
                ar_race(dim, np, ArForm.seam, true, rows)
            }
        }
    }
    seam_bench_psos_release()
    return <- rows
}

//! the E-series side rows at the E2B shape (35 layers x 256): the act over a layer's side slice and the pre-step's finish, the
//! decode row and the 512-row window
def private ple_rows : array<Reading> {
    let ple = 256
    let nl = 35
    let all = nl * ple
    var rows : array<Reading>
    rows |> reserve(4)
    for (np in fixed_array(1, 512)) {
        var bg = fill_buf(np * ple, 0.5)
        var bs = fill_buf(np * all, 0.25)
        var bp = fill_buf(np * all, 0.1)
        var bpn = fill_buf(ple, 0.01)
        let kp = GkPleActArgs(nelem = uint(np * ple), sbase = uint(7 * ple), sstride = uint(all), slen = uint(ple))
        rows |> push(time_rows("ple act ple={ple} nl={nl} {np} rows") $(enc) {
            var k = kp
            enc_geglu_side(enc, bg, bs, k, int64(np * ple))
        })
        let ka = ple_fin_args(int64(ple), 1536l, 1e-6)
        rows |> push(time_rows("ple finish ple={ple} nl={nl} {np} rows") $(enc) {
            var k = ka
            enc_ple_finish(enc, bp, bpn, bs, k, int64(nl * np))
        })
        for (buf in fixed_array(bg, bs, bp, bpn)) {
            metal_release(buf)
        }
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
    rows |> push_from(ar_rows(), ple_rows())
    rows |> push_from(moe_race_rows(), seam_lane_rows())
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
