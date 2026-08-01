options gen2
options _comment_hygiene = true

module dasllama_moe shared public

require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_convert   // the q8_0/q8_k row quantizers the expert dispatch feeds
require dasllama/dasllama_env       // env_flag: the two instrument gates
require math

// MoE expert routing and dispatch: the top-k router, the per-expert FFN accumulation, the shared
// expert, and the two opt-in decode instruments (cross-token expert reuse, GPU heat pinning).
// forward() never names any of this — the block kernels reach it through moe_ffn_core, which is
// why the whole subsystem lifts out of the hub without a hook.

// ===== cross-token expert reuse instrument (decode) =====
// DASLLAMA_EXPERT_REUSE=1 arms per-layer recording of decode expert picks — the data gate for
// the hot-expert decode cache (only cross-token reuse pays; per-token streaming loses).

let private REUSE_CAPS = fixed_array(8l, 16l, 32l, 64l)   // simulated per-layer LRU capacities

struct private ReuseLayer {
    @scratch freq : array<int64>    // per-expert pick count (static-pinning oracle input)
    @scratch prev : array<int64>    // previous token's picks
    prevn : int64
    @scratch lru : array<int64>     // the simulated LRU lists, flattened at cap-sum offsets
    lrun : int64[4]
    hits : int64[4]
    consec : int64         // picks already picked at the previous token
    accesses : int64
    tokens : int64
}

var private g_reuse_on = -1
var private g_reuse : array<ReuseLayer>

// ===== heat-cache policy (the live half of the reuse instrument) =====
// Decaying per-(layer, expert) pick counters drive an interval re-pin: every HEAT_REPIN tokens
// (staggered by layer so uploads spread) the layer's top-C ids go to the tier via advise.

let private HEAT_DECAY = 256l    // halve the counters every this many tokens (per layer)
let private HEAT_REPIN = 64l     // advise interval (per layer, layer-staggered)

struct private HeatLayer {
    @scratch cnt : array<float>     // decaying pick counters (n_expert)
    tokens : int64
    hits : int64           // routed picks the slot cache served
    picks : int64          // ... out of this many
}

var private g_heat : array<HeatLayer>
var private g_heat_want = -1l    // cached gpu_want_heat() (env read once)

//! Mean/per-layer heat-cache hit rate over the run (the split-FFN's coverage — the LRU sim in
//! expert_reuse_report is an ORACLE, this is what the live pins actually caught).
def heat_report(verbose : bool = false) {
    var hits = 0l
    var picks = 0l
    var nl = 0
    for (li in range(length(g_heat))) {
        let h & = unsafe(g_heat[li])
        continue if (h.picks == 0l)
        nl++
        hits += h.hits
        picks += h.picks
        if (verbose) {
            to_log(LOG_INFO, "heat L{li} hit {double(h.hits) * 100.0lf / double(h.picks):.1f}% ({h.hits}/{h.picks})\n")
        }
    }
    if (picks == 0l) {
        to_log(LOG_INFO, "heat: no data (cache never queried)\n")
        return
    }
    to_log(LOG_INFO, "heat MEAN hit {double(hits) * 100.0lf / double(picks):.1f}% ({hits}/{picks} picks over {nl} layers)\n")
}

[cold_path]   // one-time env resolution; heat_armed is a global test after the first call
def private load_heat_want() {
    g_heat_want = max(gpu_want_heat(), 0l)
}

def private heat_armed() : bool {
    if (g_heat_want < 0l) {
        load_heat_want()
    }
    return g_heat_want > 0l && moe_gpu_heat_on()
}

def private heat_advise_layer(t : Model; l : int64; h : HeatLayer) {
    let c = t.config
    // same fmt rule as the resident walk: a stack the tier has no kernels for (q51) never pools
    let hf1 = fmt_at(t.we1_fmt, l)
    let hf2 = fmt_at(t.we2_fmt, l)
    let hf3 = fmt_at(t.we3_fmt, l)
    if (!((hf1 == KqFmt.q8 || kq_sb(hf1)) && (hf2 == KqFmt.q8 || kq_sb(hf2))
            && (hf3 == KqFmt.q8 || kq_sb(hf3)))) {
        return
    }
    var ids <- [for (i in range(length(h.cnt))); int64(i)]
    ids |> sort() $(a, b) => h.cnt[a] > h.cnt[b]
    var want = min(g_heat_want, long_length(ids))
    while (want > 0l && h.cnt[ids[want - 1l]] <= 0.0) {   // never pin never-picked experts
        want--
    }
    if (want > 0l) {
        moe_gpu_heat_advise(l, t.we1_offs[l], t.we3_offs[l], t.we2_offs[l],
            int(moe_gpu_expert_fmt(fmt_at(t.we1_fmt, l))), int(moe_gpu_expert_fmt(fmt_at(t.we3_fmt, l))),
            int(moe_gpu_expert_fmt(fmt_at(t.we2_fmt, l))), c.dim, c.n_ff_exp, ids, want)
    }
    delete ids
}

[cold_path]   // opt-in bookkeeping behind the heat_moe gate
def private heat_note_and_advise(t : Model; l : int64; idx : array<int64>; k, ne : int64) {
    let li = int(l)
    if (length(g_heat) <= li) {
        g_heat |> resize(li + 1)
    }
    var h & = unsafe(g_heat[li])
    if (length(h.cnt) < int(ne)) {
        h.cnt |> resize(int(ne))
    }
    for (j in range64(k)) {
        h.cnt[idx[j]]++
    }
    h.tokens++
    if (h.tokens % HEAT_DECAY == 0l) {
        for (i in range(length(h.cnt))) {
            h.cnt[i] *= 0.5
        }
    }
    if ((h.tokens + l) % HEAT_REPIN == 0l) {
        heat_advise_layer(t, l, h)
    }
}

[cold_path]   // one-time env resolution; the accessor is a global test after the first call
def private load_expert_reuse() {
    g_reuse_on = env_flag("DASLLAMA_EXPERT_REUSE", false) ? 1 : 0
}

def private expert_reuse_armed() : bool {
    if (g_reuse_on < 0) {
        load_expert_reuse()
    }
    return g_reuse_on == 1
}

//! Arm/disarm the reuse counter programmatically (benches; the env is for probes/servers).
def set_expert_reuse(on : bool) {
    g_reuse_on = on ? 1 : 0
}

[cold_path]   // opt-in bookkeeping behind expert_reuse_armed
def private expert_reuse_note(l : int64; idx : array<int64>; k, ne : int64) {
    let li = int(l)
    if (length(g_reuse) <= li) {
        g_reuse |> resize(li + 1)
    }
    var r & = unsafe(g_reuse[li])
    if (length(r.freq) < int(ne)) {
        r.freq |> resize(int(ne))
        r.lru |> resize(120)   // sum of REUSE_CAPS
        r.prev |> resize(int(k))
    }
    for (j in range64(k)) {
        let id = idx[j]
        r.freq[id]++
        r.accesses++
        for (p in range64(r.prevn)) {
            if (r.prev[p] == id) {
                r.consec++
                break
            }
        }
        var base = 0l
        for (ci in range(4)) {
            let cap = REUSE_CAPS[ci]
            var found = -1l
            for (p in range64(r.lrun[ci])) {
                if (r.lru[base + p] == id) {
                    found = p
                    break
                }
            }
            if (found >= 0l) {
                r.hits[ci]++
            } else {
                if (r.lrun[ci] < cap) {
                    r.lrun[ci]++
                }
                found = r.lrun[ci] - 1l   // lands most-recent; the true LRU tail falls off when full
            }
            var p = found
            while (p > 0l) {
                r.lru[base + p] = r.lru[base + p - 1l]
                p--
            }
            r.lru[base] = id
            base += cap
        }
    }
    for (j in range64(k)) {
        r.prev[j] = idx[j]
    }
    r.prevn = k
    r.tokens++
}

//! Print the decode expert-reuse report accumulated under ``DASLLAMA_EXPERT_REUSE=1``: per layer
//! (``verbose`` only), simulated LRU hit rates at capacities 8/16/32/64, static top-C frequency
//! coverage (the whole-run pinning oracle), and consecutive-token overlap.
def expert_reuse_report(verbose : bool = true) {
    var nl = 0
    for (r in g_reuse) {
        if (r.accesses > 0l) {
            nl++
        }
    }
    var sum_hits : int64[4]
    var sum_cov : int64[4]
    var sum_consec = 0l
    var sum_acc = 0l
    var toks = 0l
    for (li in range(length(g_reuse))) {
        var r & = unsafe(g_reuse[li])
        if (r.accesses == 0l) {
            continue
        }
        toks = max(toks, r.tokens)
        // static top-C coverage: sort a frequency copy descending, prefix-sum at each capacity
        var f := r.freq
        f |> sort() $(a, b) => a > b
        var cov : int64[4]
        var acc = 0l
        var fi = 0
        for (ci in range(4)) {
            while (int64(fi) < min(REUSE_CAPS[ci], long_length(f))) {
                acc += f[fi]
                fi++
            }
            cov[ci] = acc
            sum_cov[ci] += acc
        }
        delete f
        let a = double(r.accesses)
        let ltag = li < 10 ? "L0" : "L"
        if (verbose) {
            to_log(LOG_INFO, "{ltag}{li} lru {double(r.hits[0]) * 100.0lf / a:.1f}/{double(r.hits[1]) * 100.0lf / a:.1f}/{double(r.hits[2]) * 100.0lf / a:.1f}/{double(r.hits[3]) * 100.0lf / a:.1f}%  top {double(cov[0]) * 100.0lf / a:.1f}/{double(cov[1]) * 100.0lf / a:.1f}/{double(cov[2]) * 100.0lf / a:.1f}/{double(cov[3]) * 100.0lf / a:.1f}%  consec {double(r.consec) * 100.0lf / a:.1f}%\n")
        }
        for (ci in range(4)) {
            sum_hits[ci] += r.hits[ci]
        }
        sum_consec += r.consec
        sum_acc += r.accesses
    }
    if (sum_acc == 0l) {
        to_log(LOG_INFO, "expert-reuse: no data (DASLLAMA_EXPERT_REUSE=1 not set, or no decode ran)\n")
        return
    }
    let a = double(sum_acc)
    to_log(LOG_INFO, "MEAN lru {double(sum_hits[0]) * 100.0lf / a:.1f}/{double(sum_hits[1]) * 100.0lf / a:.1f}/{double(sum_hits[2]) * 100.0lf / a:.1f}/{double(sum_hits[3]) * 100.0lf / a:.1f}%  top {double(sum_cov[0]) * 100.0lf / a:.1f}/{double(sum_cov[1]) * 100.0lf / a:.1f}/{double(sum_cov[2]) * 100.0lf / a:.1f}/{double(sum_cov[3]) * 100.0lf / a:.1f}%  consec {double(sum_consec) * 100.0lf / a:.1f}%  (caps {REUSE_CAPS[0]}/{REUSE_CAPS[1]}/{REUSE_CAPS[2]}/{REUSE_CAPS[3]}, tokens {toks}, layers-with-data {nl})\n")
    // the transfer-budget view: cache misses per token per layer at each capacity
    let tl = double(sum_acc) / (toks > 0l ? double(toks) : 1.0lf)
    to_log(LOG_INFO, "miss/tok/layer lru {(a - double(sum_hits[0])) / a * tl / double(nl > 0 ? nl : 1):.2f}/{(a - double(sum_hits[1])) / a * tl / double(nl > 0 ? nl : 1):.2f}/{(a - double(sum_hits[2])) / a * tl / double(nl > 0 ? nl : 1):.2f}/{(a - double(sum_hits[3])) / a * tl / double(nl > 0 ? nl : 1):.2f}\n")
}

//! Drop all accumulated expert-reuse data (phase separation in probes).
def expert_reuse_reset() {
    delete g_reuse
}

// Can the routed mx4 experts run as the fused 2-stage chain? Q8 pre-quant, mx4 expert planes,
// team mode + a backend mx4 rows core, 32-divisible spans — the fused_ffn_ok contract at MoE shape.
def private fused_moe_ok(t : Model) : bool {
    let c = t.config
    return (g_fused_decode && t.quant == QuantMode.q8 && t.experts_mx4
        && c.n_ff_exp % 32l == 0l && c.dim % 32l == 0l
        && get_jobque_team_mode() && kernel_backend_has_mx4_rows()
        && lanes_for_work(2l * c.n_expert_used * c.dim * c.n_ff_exp, g_gemv_lane_cap) > 1)
}

// Fused MoE expert chain: ONE team rendezvous runs [gate+up rows + bias + act + requant] ->
// [down rows + bias + weighted reduce] over all k routed experts. Bit-identical to the groupn
// sequence — same rows cores, same dot->bias->act element order, stage 1 reduces in slot order.
def private moe_experts_chain(t : Model; var s : Session; l : int64) { // nolint:STYLE038 — one fused per-expert pass: gather, GEMV and accumulate share hoisted row pointers across the whole walk
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    let nfe = c.n_ff_exp
    let ege = dim * nfe
    let biased = c.moe_exps_bias
    let ts_chain = ref_time_ticks()
    let mxrows = mx4_rows_fn()
    let is_gelu = c.ffn_act == FfnAct.gelu
    let is_oai = c.ffn_act == FfnAct.swiglu_oai
    let vec = g_act_vec
    let w1o = t.we1_offs[l]
    let w3o = t.we3_offs[l]
    let w2o = t.we2_offs[l]
    let web1o = t.web1_off
    let web3o = t.web3_off
    let web2o = t.web2_off
    let ng0 = nfe / 32l
    let ngrp0 = int(k * ng0)
    let ngrp1 = int(dim / 32l)
    let n0 = chain_stage_chunks(ngrp0, int(k * nfe), 2l * k * dim * nfe)
    let n1 = chain_stage_chunks(ngrp1, int(dim), k * nfe * dim)
    s.chain_stages |> resize(2)
    s.chain_stages[0] = int3(0, ngrp0, n0)
    s.chain_stages[1] = int3(0, ngrp1, n1)
    count_parallel_dispatch(min(n0, ngrp0) + min(n1, ngrp1))
    unsafe {
        var accp = addr(s.moe_acc[0])
        var gup = addr(s.moe_gu[0])
        var gu2p = addr(s.moe_gu2[0])
        var dnp = addr(s.moe_dn[0])
        var dqp = addr(s.moe_dq[0])
        var dsp = addr(s.moe_ds[0])
        let xqp = addr<int8 const?>(s.moe_xq[0])
        let xsp = addr<float const?>(s.moe_xs[0])
        let wnb = addr<uint8 const?>(t.mxq[0])
        let web = addr<uint8 const?>(t.mxs[0])
        let fbp = biased ? addr<float const?>(t.fblob[0]) : null
        let idxp = addr<int64 const?>(s.moe_idx[0])
        let wvp = addr<float const?>(s.moe_w[0])
        team_parallel_stages(s.chain_stages) <| @(stage : int; jb : int; je : int) {   // nolint:PERF026 — dispatch-API lambda capture, see attention_chain_decode
            unsafe {
                if (stage == 0) {
                    // gate+up rows of expert j, then bias -> act -> requant on exactly these rows
                    var g = int64(jb)
                    while (g < int64(je)) {
                        let j = g / ng0
                        let gEnd = min(int64(je) - j * ng0, ng0)
                        let r0 = (g % ng0) * 32l
                        let r1 = gEnd * 32l
                        let base = idxp[j] * ege
                        let w1 = w1o + base
                        let w3 = w3o + base
                        invoke(mxrows, gup + j * nfe, wnb + w1 / 2l, web + w1 / 32l, xqp, xsp, dim, r0, r1)
                        invoke(mxrows, gu2p + j * nfe, wnb + w3 / 2l, web + w3 / 32l, xqp, xsp, dim, r0, r1)
                        if (biased) {
                            let e = l * ne + idxp[j]
                            add_inplace(gup + j * nfe + r0, fbp + web1o + e * nfe + r0, r1 - r0)
                            add_inplace(gu2p + j * nfe + r0, fbp + web3o + e * nfe + r0, r1 - r0)
                        }
                        if (vec) {
                            if (is_gelu) {
                                geglu4(gup + j * nfe + r0, gu2p + j * nfe + r0, r1 - r0)
                            } elif (is_oai) {
                                swiglu_oai4(gup + j * nfe + r0, gu2p + j * nfe + r0, r1 - r0)
                            } else {
                                swiglu4(gup + j * nfe + r0, gu2p + j * nfe + r0, r1 - r0)
                            }
                        } else {
                            if (is_gelu) {
                                geglu(gup + j * nfe + r0, gu2p + j * nfe + r0, r1 - r0)
                            } elif (is_oai) {
                                swiglu_oai(gup + j * nfe + r0, gu2p + j * nfe + r0, r1 - r0)
                            } else {
                                swiglu(gup + j * nfe + r0, gu2p + j * nfe + r0, r1 - r0)
                            }
                        }
                        quantize_q8_0_into_ptr(gup + j * nfe + r0, r1 - r0, dqp, dsp, j * nfe + r0, (j * nfe + r0) / 32l)
                        g = j * ng0 + gEnd
                    }
                } else {
                    // every expert's down rows for this dim span, then the weighted sum in slot order
                    let r0 = int64(jb) * 32l
                    let r1 = int64(je) * 32l
                    for (j in range64(k)) {
                        let w2 = w2o + idxp[j] * ege
                        invoke(mxrows, dnp + j * dim, wnb + w2 / 2l, web + w2 / 32l, dqp + j * nfe, dsp + j * nfe / 32l, nfe, r0, r1)
                        if (biased) {
                            add_inplace(dnp + j * dim + r0, fbp + web2o + (l * ne + idxp[j]) * dim + r0, r1 - r0)
                        }
                    }
                    for (j in range64(k)) {
                        axpy(accp + r0, dnp + j * dim + r0, wvp[j], r1 - r0)
                    }
                }
            }
        }
    }
    prof_add("moe_chain", ts_chain)
}

// Routed experts, given the pre-normed input in s.xb and the top-k selection in s.moe_idx/s.moe_w:
// s.moe_acc = Σ_j w_j · FFN_e_j(xb). Shared by qwen2moe/gpt-oss (via moe_ffn_core) and gemma4
// (which folds its per-expert down scale into s.moe_w first).
def moe_experts_apply(t : Model; var s : Session; l : int64) { // nolint:STYLE037 — a flat dispatch table over expert weight format (q8/kq/q51/mx4) x placement; each arm is a distinct kernel call, not a branch to factor
    trace_tag(TRACE_TAG_MOE)
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    let k = c.n_expert_used
    if (expert_reuse_armed()) {
        expert_reuse_note(l, s.moe_idx, k, ne)
    }
    for (i in range64(dim)) {
        s.moe_acc[i] = 0.0
    }
    // Q8: quantize the shared activation ONCE (dedicated moe_xq/moe_xs so down-proj requant doesn't clobber it)
    let pre_q8 = t.quant == QuantMode.q8
    let layer_kq = pre_q8 && moe_layer_kq(t, l)
    if (pre_q8) {
        let ts_rq = ref_time_ticks()
        if (layer_kq && moe_pair_kform(fmt_at(t.we1_fmt, l), fmt_at(t.we3_fmt, l))) {
            // kq v2: kq gate/up stacks read the Q8_K-form scratch
            quantize_q8_k_into(s.xb, dim, s.kxq, s.kxs, s.kxbs, 0l, 0l, 0l)
            if (c.n_ff_shexp > 0l && !c.moe_dense_shexp) {
                // the q8 shexp downstream reads the hoisted moe_xq image — without this it silently drops out
                quantize_q8_0_into(s.xb, dim, s.moe_xq, s.moe_xs, 0l, 0l)
            }
        } else {
            quantize_q8_0_into(s.xb, dim, s.moe_xq, s.moe_xs, 0l, 0l)
        }
        prof_add("mm_requant", ts_rq)
    }
    let nfe = c.n_ff_exp
    let ege = dim * nfe
    if (pre_q8) {
        // fused-dispatch form: bit-exact vs the per-expert loop, but 3 fork/joins instead of 3k
        let ts_gu = ref_time_ticks()
        let biased = c.moe_exps_bias   // gpt-oss: each expert's biases fold into the workers' stores
        let f1 = fmt_at(t.we1_fmt, l)
        let f2 = fmt_at(t.we2_fmt, l)
        let f3 = fmt_at(t.we3_fmt, l)
        // GPU MoE tier: unbiased plain-q8 or all-K-quant stacks of offloaded layers dispatch on the GPU
        let gpu_moe = !biased && !t.experts_mx4 && c.ffn_act != FfnAct.swiglu_oai && moe_gpu_layer_on_gpu(l)
        // heat cache: non-resident layers partition the top-k against the tier's slot pools — hits ride the GPU FFN chain async, misses run the CPU groupn path concurrently
        let heat_moe = !gpu_moe && !biased && !t.experts_mx4 && c.ffn_act != FfnAct.swiglu_oai && heat_armed()
        var nhits = 0l
        var moe_chained = false   // the fused chain reduces inline — the nhits==0 axpy tail must skip
        if (heat_moe) {
            heat_note_and_advise(t, l, s.moe_idx, k, ne)
            s.heat_hit |> resize(int(k))
            s.heat_o1 |> resize(int(k))
            s.heat_o3 |> resize(int(k))
            s.heat_o2 |> resize(int(k))
            nhits = matmul_moe_gpu_heat_query(l, s.moe_idx, k, s.heat_hit, s.heat_o1, s.heat_o3, s.heat_o2)
            var hl & = unsafe(g_heat[int(l)])
            hl.hits += nhits
            hl.picks += k
        }
        if (gpu_moe) {
            // region triples (slot j = row j) + a k-row gathered image; gate+up/act/requant/down runs device-side into moe_dn
            s.moe_offs1 |> clear(); s.moe_offs1 |> reserve(k * 3l)
            s.moe_offs3 |> clear(); s.moe_offs3 |> reserve(k * 3l)
            s.moe_offs2 |> clear(); s.moe_offs2 |> reserve(k * 3l)
            for (j in range64(k)) {
                let base = s.moe_idx[j] * ege
                s.moe_offs1 |> push(t.we1_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs1 |> push(j)
                s.moe_offs1 |> push(1l)
                s.moe_offs3 |> push(t.we3_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs3 |> push(j)
                s.moe_offs3 |> push(1l)
                s.moe_offs2 |> push(t.we2_offs[l] + base)   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs2 |> push(j)
                s.moe_offs2 |> push(1l)
            }
            // kq layers gather the Q8_K image; q8 layers the Q8_0 one — the tier resolves the form per stack
            let nb = layer_kq ? dim / 256l : dim / 32l
            s.moe_gxq |> resize(k * dim)
            s.moe_gxs |> resize(k * nb)
            unsafe {
                var gq = addr(s.moe_gxq[0])
                var gs = addr(s.moe_gxs[0])
                let sxq = layer_kq ? addr<void?>(s.kxq[0]) : addr<void?>(s.moe_xq[0])
                let sxs = layer_kq ? addr<void?>(s.kxs[0]) : addr<void?>(s.moe_xs[0])
                for (j in range64(k)) {
                    memcpy(reinterpret<void?>(gq + j * dim), sxq, dim)
                    memcpy(reinterpret<void?>(gs + j * nb), sxs, nb * 4l)
                }
            }
            matmul_moe_gpu_ffn(s.moe_dn, s.moe_offs1, s.moe_offs3, s.moe_offs2, k,
                s.moe_gxq, s.moe_gxs, dim, nfe, k, int(moe_gpu_expert_fmt(f1)), int(moe_gpu_expert_fmt(f3)), int(moe_gpu_expert_fmt(f2)), c.ffn_act == FfnAct.gelu)
            prof_add("mm_moe", ts_gu)
        } elif (nhits > 0l) {
            // split-FFN: compacted hit regions (pool coordinates, rows 0..nhits) dispatch async
            let m = k - nhits
            s.moe_offs1 |> clear()
            s.moe_offs3 |> clear()
            s.moe_offs2 |> clear()
            for (i in range64(nhits)) {
                s.moe_offs1 |> push(s.heat_o1[i])   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs1 |> push(i)
                s.moe_offs1 |> push(1l)
                s.moe_offs3 |> push(s.heat_o3[i])   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs3 |> push(i)
                s.moe_offs3 |> push(1l)
                s.moe_offs2 |> push(s.heat_o2[i])   // nolint:STYLE012 — loop-append into reused scratch
                s.moe_offs2 |> push(i)
                s.moe_offs2 |> push(1l)
            }
            let nb = layer_kq ? dim / 256l : dim / 32l
            s.moe_gxq |> resize(int(nhits * dim))
            s.moe_gxs |> resize(int(nhits * nb))
            unsafe {
                var gq = addr(s.moe_gxq[0])
                var gs = addr(s.moe_gxs[0])
                let sxq = layer_kq ? addr<void?>(s.kxq[0]) : addr<void?>(s.moe_xq[0])
                let sxs = layer_kq ? addr<void?>(s.kxs[0]) : addr<void?>(s.moe_xs[0])
                for (i in range64(nhits)) {
                    memcpy(reinterpret<void?>(gq + i * dim), sxq, dim)
                    memcpy(reinterpret<void?>(gs + i * nb), sxs, nb * 4l)
                }
            }
            let ts_sub = ref_time_ticks()
            matmul_moe_gpu_ffn_begin(s.moe_offs1, s.moe_offs3, s.moe_offs2, nhits,
                s.moe_gxq, s.moe_gxs, dim, nfe, nhits, int(moe_gpu_expert_fmt(f1)), int(moe_gpu_expert_fmt(f3)), int(moe_gpu_expert_fmt(f2)), c.ffn_act == FfnAct.gelu)
            prof_add("heat_sub", ts_sub)   // dispatch cost ON the critical path (the split's tax)
            // the missed experts run the CPU groupn chain while the GPU crunches the hits
            var ts_down = ts_gu
            if (m > 0l) {
                var mi = 0l
                for (j in range64(k)) {
                    if (s.heat_hit[j] == int8(0)) {
                        s.moe_offs[mi * 2l] = t.we1_offs[l] + s.moe_idx[j] * ege
                        s.moe_offs[mi * 2l + 1l] = 0l
                        mi++
                    }
                }
                if (kq_sb(f1)) {
                    mm_at_kq_groupn(s.moe_gu, t, f1, s.moe_offs, m, s.kxq, s.kxs, s.kxbs, dim, nfe)
                } elif (f1 == KqFmt.q51) {
                    mm_at_q51_groupn(s.moe_gu, t, s.moe_offs, m, s.moe_xq, s.moe_xs, dim, nfe)
                } else {
                    mm_at_q8_groupn(s.moe_gu, t, s.moe_offs, m, s.moe_xq, s.moe_xs, dim, nfe)
                }
                mi = 0l
                for (j in range64(k)) {
                    if (s.heat_hit[j] == int8(0)) {
                        s.moe_offs[mi * 2l] = t.we3_offs[l] + s.moe_idx[j] * ege
                        s.moe_offs[mi * 2l + 1l] = 0l
                        mi++
                    }
                }
                if (kq_sb(f3)) {
                    mm_at_kq_groupn(s.moe_gu2, t, f3, s.moe_offs, m, s.kxq, s.kxs, s.kxbs, dim, nfe)
                } elif (f3 == KqFmt.q51) {
                    mm_at_q51_groupn(s.moe_gu2, t, s.moe_offs, m, s.moe_xq, s.moe_xs, dim, nfe)
                } else {
                    mm_at_q8_groupn(s.moe_gu2, t, s.moe_offs, m, s.moe_xq, s.moe_xs, dim, nfe)
                }
                prof_add("mm_moe", ts_gu)
                let ts_act = ref_time_ticks()
                gate_batch(s.moe_gu, s.moe_gu2, c.ffn_act, nfe, m)
                prof_add("act", ts_act)
                if (kq_sb(f2)) {
                    requant_rows_q8k_bs(s.moe_gu, nfe, m, s.moe_dq, s.moe_ds, s.moe_dbs)
                } else {
                    requant_rows_q8(s.moe_gu, nfe, m, s.moe_dq, s.moe_ds)
                }
                ts_down = ref_time_ticks()
                mi = 0l
                for (j in range64(k)) {
                    if (s.heat_hit[j] == int8(0)) {
                        s.moe_offs[mi * 2l] = t.we2_offs[l] + s.moe_idx[j] * ege
                        s.moe_offs[mi * 2l + 1l] = mi * nfe
                        mi++
                    }
                }
                if (kq_sb(f2)) {
                    mm_at_kq_groupn(s.moe_dn, t, f2, s.moe_offs, m, s.moe_dq, s.moe_ds, s.moe_dbs, nfe, dim)
                } elif (f2 == KqFmt.q51) {
                    mm_at_q51_groupn(s.moe_dn, t, s.moe_offs, m, s.moe_dq, s.moe_ds, nfe, dim)
                } else {
                    mm_at_q8_groupn(s.moe_dn, t, s.moe_offs, m, s.moe_dq, s.moe_ds, nfe, dim)
                }
            }
            s.moe_hdn |> resize(int(nhits * dim))
            prof_add("mm_moe_dn", ts_down)
            let ts_join = ref_time_ticks()
            matmul_moe_gpu_ffn_join(s.moe_hdn)
            prof_add("heat_wait", ts_join)   // GPU residue after the CPU misses — 0 = fully hidden
            let ts_hred = ref_time_ticks()
            var hi = 0l
            var mrow = 0l
            for (j in range64(k)) {   // original slot order — the weighted sum stays order-stable
                if (s.heat_hit[j] != int8(0)) {
                    axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.moe_hdn[hi * dim])), s.moe_w[j], dim)
                    hi++
                } else {
                    axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.moe_dn[mrow * dim])), s.moe_w[j], dim)
                    mrow++
                }
            }
            prof_add("moe_reduce", ts_hred)
        } elif (t.experts_mx4 && !kq_sb(f1) && f1 != KqFmt.q51 && !kq_sb(f3) && f3 != KqFmt.q51
                && !kq_sb(f2) && f2 != KqFmt.q51 && fused_moe_ok(t)) {
            // fused mx4 expert chain: 3 groupn fork/joins collapse to ONE team rendezvous
            moe_experts_chain(t, s, l)
            moe_chained = true
        } else {
            for (j in range64(k)) {
                let e = l * ne + s.moe_idx[j]
                s.moe_offs[j * 2l] = t.we1_offs[l] + s.moe_idx[j] * ege
                s.moe_offs[j * 2l + 1l] = 0l   // gates share the single hoisted xb image
                s.moe_boffs[j] = t.web1_off + e * nfe
            }
            if (kq_sb(f1)) {   // native K-quant expert stack reads the Q8_K-form scratch (never biased — detect guards that)
                mm_at_kq_groupn(s.moe_gu, t, f1, s.moe_offs, k, s.kxq, s.kxs, s.kxbs, dim, nfe)
            } elif (f1 == KqFmt.q51) {   // native Q5_1 stack reads the Q8_0-form image (never biased)
                mm_at_q51_groupn(s.moe_gu, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
            } elif (t.experts_mx4) {   // native 4-bit expert weights — half the Q8 traffic
                if (biased) {
                    mm_at_mx4_groupn(s.moe_gu, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
                } else {
                    mm_at_mx4_groupn(s.moe_gu, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
                }
            } elif (biased) {
                mm_at_q8_groupn(s.moe_gu, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
            } else {
                mm_at_q8_groupn(s.moe_gu, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
            }
            for (j in range64(k)) {
                let e = l * ne + s.moe_idx[j]
                s.moe_offs[j * 2l] = t.we3_offs[l] + s.moe_idx[j] * ege
                s.moe_offs[j * 2l + 1l] = 0l   // ups read the same hoisted xb image (explicit, not inherited from the gate pass)
                s.moe_boffs[j] = t.web3_off + e * nfe
            }
            if (kq_sb(f3)) {   // kq v2: Q8_K-form scratch
                mm_at_kq_groupn(s.moe_gu2, t, f3, s.moe_offs, k, s.kxq, s.kxs, s.kxbs, dim, nfe)
            } elif (f3 == KqFmt.q51) {
                mm_at_q51_groupn(s.moe_gu2, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
            } elif (t.experts_mx4) {
                if (biased) {
                    mm_at_mx4_groupn(s.moe_gu2, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
                } else {
                    mm_at_mx4_groupn(s.moe_gu2, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
                }
            } elif (biased) {
                mm_at_q8_groupn(s.moe_gu2, t, s.moe_offs, s.moe_boffs, k, s.moe_xq, s.moe_xs, dim, nfe)
            } else {
                mm_at_q8_groupn(s.moe_gu2, t, s.moe_offs, k, s.moe_xq, s.moe_xs, dim, nfe)
            }
            prof_add("mm_moe", ts_gu)
            let ts_act = ref_time_ticks()
            gate_batch(s.moe_gu, s.moe_gu2, c.ffn_act, nfe, k)
            prof_add("act", ts_act)
            if (kq_sb(f2)) {   // kq v2: the down stack reads Q8_K-form rows; q51/q8 the Q8_0 form
                requant_rows_q8k_bs(s.moe_gu, nfe, k, s.moe_dq, s.moe_ds, s.moe_dbs)
            } else {
                requant_rows_q8(s.moe_gu, nfe, k, s.moe_dq, s.moe_ds)   // books mm_requant
            }
            let ts_down = ref_time_ticks()
            for (j in range64(k)) {
                let e = l * ne + s.moe_idx[j]
                s.moe_offs[j * 2l] = t.we2_offs[l] + s.moe_idx[j] * ege
                s.moe_offs[j * 2l + 1l] = j * nfe   // each down reads its own requantized row
                s.moe_boffs[j] = t.web2_off + e * dim   // down bias inside the routed weight (ggml_add_id order)
            }
            if (kq_sb(f2)) {
                mm_at_kq_groupn(s.moe_dn, t, f2, s.moe_offs, k, s.moe_dq, s.moe_ds, s.moe_dbs, nfe, dim)
            } elif (f2 == KqFmt.q51) {
                mm_at_q51_groupn(s.moe_dn, t, s.moe_offs, k, s.moe_dq, s.moe_ds, nfe, dim)
            } elif (t.experts_mx4) {
                if (biased) {
                    mm_at_mx4_groupn(s.moe_dn, t, s.moe_offs, s.moe_boffs, k, s.moe_dq, s.moe_ds, nfe, dim)
                } else {
                    mm_at_mx4_groupn(s.moe_dn, t, s.moe_offs, k, s.moe_dq, s.moe_ds, nfe, dim)
                }
            } elif (biased) {
                mm_at_q8_groupn(s.moe_dn, t, s.moe_offs, s.moe_boffs, k, s.moe_dq, s.moe_ds, nfe, dim)
            } else {
                mm_at_q8_groupn(s.moe_dn, t, s.moe_offs, k, s.moe_dq, s.moe_ds, nfe, dim)
            }
            prof_add("mm_moe_dn", ts_down)
        }
        if (nhits == 0l && !moe_chained) {   // the split arm reduced its own row banks; the chain reduced inline
            let ts_red = ref_time_ticks()
            for (j in range64(k)) {   // slot order preserved — the sum is bit-identical
                axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.moe_dn[j * dim])), s.moe_w[j], dim)
            }
            prof_add("moe_reduce", ts_red)
        }
    } else {
        // fp32 reference path: the per-expert loop (q4_0 never routes here — MoE loads are q8/fp32)
        for (j in range64(k)) {
            let e = l * ne + s.moe_idx[j]
            let base = s.moe_idx[j] * ege
            let ts_gu = ref_time_ticks()
            mm(s.hb, t, t.we1_offs[l] + base, s.xb, s.xq, s.xs, dim, nfe)
            mm(s.hb2, t, t.we3_offs[l] + base, s.xb, s.xq, s.xs, dim, nfe)
            if (c.moe_exps_bias) {  // gpt-oss: this expert's gate/up biases before the activation
                add_bias(s.hb, 0l, t.fblob, t.web1_off + e * nfe, nfe)
                add_bias(s.hb2, 0l, t.fblob, t.web3_off + e * nfe, nfe)
            }
            prof_add("mm_moe", ts_gu)
            let ts_act = ref_time_ticks()
            gate_batch(s.hb, s.hb2, c.ffn_act, nfe, 1l)
            prof_add("act", ts_act)
            let ts_down = ref_time_ticks()
            mm(s.xb2, t, t.we2_offs[l] + base, s.hb, s.xq, s.xs, nfe, dim)
            if (c.moe_exps_bias) {  // ... and its down bias, inside the routed weight (ggml_add_id order)
                add_bias(s.xb2, 0l, t.fblob, t.web2_off + e * dim, dim)
            }
            prof_add("mm_moe_dn", ts_down)
            let ts_red = ref_time_ticks()
            axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.xb2[0])), s.moe_w[j], dim)
            prof_add("moe_reduce", ts_red)
        }
    }
}

// MoE FFN core (qwen2moe / gpt-oss): router logits on the rmsnormed activation in s.xb -> softmax /
// sigmoid select -> routed experts (moe_experts_apply) -> optional sigmoid-gated shared expert.
// Leaves the combined output in s.moe_acc; callers add the post-norm / residual / layer-out scale.
def moe_ffn_core(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let ne = c.n_expert
    trace_tag(TRACE_TAG_ROUTE)   // decode router GEMV — without this it rides the stale attn tag in traces
    let ts_route = ref_time_ticks()
    mm_fblob(s.moe_logits, t, t.router_off + l * ne * dim, s.xb, dim, ne)
    if (c.moe_router_bias) {
        add_bias(s.moe_logits, 0l, t.fblob, t.router_b_off + l * ne, ne)
    }
    var ebp : float const?
    if (c.moe_exp_probs) {
        ebp = unsafe(addr<float const?>(t.fblob[t.exp_probs_off + l * ne]))
    }
    moe_select(c, unsafe(addr(s.moe_logits[0])), s.moe_idx, s.moe_w, 0l, ebp)
    prof_add("moe_route", ts_route)
    // resident shared expert: submit FIRST, collect AFTER the routed experts, so it runs inside the CPU's own expert window — a synchronous per-layer submit is the heat-cache wash.
    let sh_gpu = (c.n_ff_shexp > 0l && !c.moe_dense_shexp && t.quant == QuantMode.q8
        && moe_gpu_shexp_on_gpu(l) && !moe_gpu_layer_on_gpu(l) && !heat_armed())
    let nsh = c.n_ff_shexp
    let she = dim * nsh
    if (sh_gpu) {
        let ts_shs = ref_time_ticks()
        quantize_q8_0_into(s.xb, dim, s.moe_xq, s.moe_xs, 0l, 0l)   // moe_experts_apply recomputes the same bits
        // the routed CPU path uses moe_offs (not the triples) — these are free at this gate
        s.moe_offs1 <- [t.wsh1_off + l * she, 0l, 1l]
        s.moe_offs3 <- [t.wsh3_off + l * she, 0l, 1l]
        s.moe_offs2 <- [t.wsh2_off + l * she, 0l, 1l]
        matmul_moe_gpu_ffn_begin(s.moe_offs1, s.moe_offs3, s.moe_offs2, 1l, s.moe_xq, s.moe_xs,
            dim, nsh, 1l, 0, 0, 0, c.ffn_act == FfnAct.gelu)
        prof_add("shexp_sub", ts_shs)
    }
    moe_experts_apply(t, s, l)
    if (sh_gpu) {
        let ts_shw = ref_time_ticks()
        trace_tag(TRACE_TAG_SHEXP)
        let g = c.moe_shexp_gated ? moe_shexp_gate(t, l, unsafe(addr(s.xb[0]))) : 1.0
        matmul_moe_gpu_ffn_join(s.xb2)
        axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.xb2[0])), g, dim)
        prof_add("shexp_wait", ts_shw)   // 0 = fully hidden behind the routed-expert window
    }
    // shared expert, gated by a sigmoid scalar from its per-layer gate vector (glm4moe: ungated, g = 1)
    if (c.n_ff_shexp > 0l && !sh_gpu) {
        let pre_q8 = t.quant == QuantMode.q8
        let ts_sh = ref_time_ticks()
        trace_tag(TRACE_TAG_SHEXP)
        let g = c.moe_shexp_gated ? moe_shexp_gate(t, l, unsafe(addr(s.xb[0]))) : 1.0
        if (pre_q8) {
            // shexp gate+up share the hoisted xb quants: one region-list dispatch, act on the halves
            s.ffn_offs[0] = t.wsh1_off + l * she
            s.ffn_offs[1] = 0l
            s.ffn_offs[2] = t.wsh3_off + l * she
            s.ffn_offs[3] = 0l
            mm_at_q8_groupn(s.ffn_gu, t, s.ffn_offs, 2l, s.moe_xq, s.moe_xs, dim, nsh)
            unsafe {
                gate_batch(addr(s.ffn_gu[0]), addr < float const? >(s.ffn_gu[nsh]), c.ffn_act, nsh, 1l)
            }
            quantize_q8_0_into(s.ffn_gu, nsh, s.xq, s.xs, 0l, 0l)
            mm_at_q8_pre(s.xb2, t, t.wsh2_off + l * she, s.xq, s.xs, nsh, dim)
        } else {
            mm(s.hb, t, t.wsh1_off + l * she, s.xb, s.xq, s.xs, dim, nsh)
            mm(s.hb2, t, t.wsh3_off + l * she, s.xb, s.xq, s.xs, dim, nsh)
            gate_batch(s.hb, s.hb2, c.ffn_act, nsh, 1l)
            mm(s.xb2, t, t.wsh2_off + l * she, s.hb, s.xq, s.xs, nsh, dim)
        }
        axpy(unsafe(addr(s.moe_acc[0])), unsafe(addr(s.xb2[0])), g, dim)
        prof_add("mm_shexp", ts_sh)
    }
}

// Config/Session-facing wrapper over the pointer core; off is 0 for decode, p*k for grouped prefill.
def moe_select(c : Config; var lg : float?; var idx : array<int64>; var w : array<float>; off : int64; bias : float const? = null) {
    unsafe {
        moe_select_core(c.moe_gate, c.n_expert, c.n_expert_used, c.norm_topk_prob, c.expert_weights_scale,
            lg, addr(idx[0]), addr(w[0]), off, bias)
    }
}

// pointer core — the threaded prefill router runs this in fork workers (scalars + raw pointers
// only; Config/Session stay on the caller). `bias` (exp_probs_b, DeepSeek-V3/glm4moe) skews the
// top-k PICK only — the stored weights stay the unbiased probs (llama.cpp build_moe_ffn order).
def moe_select_core(gate : MoeGate; ne, k : int64; norm_topk : bool; wscale : float; // nolint:STYLE037 — gate variant x bias x renorm are flat arms over one logit row, coupled through the running top-k state
                    var lg : float?; var idx : int64?; var w : float?; off : int64;
                    bias : float const? = null) {
    unsafe {   // the whole body indexes the logit-row pointer (in-place gate + top-k knockout)
    // probs — softmax_weight routes on the RAW biased logits, softmaxes only the selected top-k after
    if (gate == MoeGate.sigmoid) {
        var i = 0l
        while (i + 4l <= ne) {
            let e = exp4(clamp(-float4(lg[i], lg[i + 1l], lg[i + 2l], lg[i + 3l]), EXP4_LO, EXP4_HI))
            lg[i] = 1.0 / (1.0 + e.x)
            lg[i + 1l] = 1.0 / (1.0 + e.y)
            lg[i + 2l] = 1.0 / (1.0 + e.z)
            lg[i + 3l] = 1.0 / (1.0 + e.w)
            i += 4l
        }
        while (i < ne) {
            lg[i] = 1.0 / (1.0 + exp(clamp(-lg[i], -80.0, 80.0)))
            i++
        }
    } elif (gate == MoeGate.softmax) {
        var mx = lg[0]
        for (i in range64(ne)) {
            mx = max(mx, lg[i])
        }
        let m4 = float4(mx, mx, mx, mx)
        var sum4 = float4(0.0)
        var i = 0l
        while (i + 4l <= ne) {
            let e = exp4(max(float4(lg[i], lg[i + 1l], lg[i + 2l], lg[i + 3l]) - m4, EXP4_LO))
            lg[i] = e.x
            lg[i + 1l] = e.y
            lg[i + 2l] = e.z
            lg[i + 3l] = e.w
            sum4 += e
            i += 4l
        }
        var sum = sum4.x + sum4.y + sum4.z + sum4.w
        while (i < ne) {
            let e = exp(max(lg[i] - mx, -80.0))
            lg[i] = e
            sum += e
            i++
        }
        for (j in range64(ne)) {
            lg[j] /= sum
        }
    }
    // top-k by score, winner knocked out with -FLT_MAX; with a bias the pick compares lg[i]+bias[i] but stores UNBIASED lg[best]
    if (bias == null) {
        for (j in range64(k)) {
            var best = 0l
            for (i in range64(ne)) {
                if (lg[i] > lg[best]) {
                    best = i
                }
            }
            idx[off + j] = best
            w[off + j] = lg[best]
            lg[best] = -FLT_MAX
        }
    } else {
        for (j in range64(k)) {
            var best = 0l
            var bs = -FLT_MAX
            for (i in range64(ne)) {
                if (lg[i] == -FLT_MAX) {   // knocked out — a huge/inf file bias must not resurrect it
                    continue
                }
                let sc = lg[i] + bias[i]
                if (sc > bs) {
                    best = i
                    bs = sc
                }
            }
            idx[off + j] = best
            w[off + j] = lg[best]
            lg[best] = -FLT_MAX
        }
    }
    if (gate == MoeGate.softmax_weight) {  // gpt-oss: softmax over just the selected k logits
        softmax(w + off, k)
    }
    if (norm_topk) {   // renormalize the selected weights to sum 1 (llama.cpp clamps at f16-min)
        var ws = 0.0
        for (j in range64(k)) {
            ws += w[off + j]
        }
        ws = max(ws, 6.103515625e-5)
        for (j in range64(k)) {
            w[off + j] /= ws
        }
    }
    if (wscale != 0.0 && wscale != 1.0) {
        for (j in range64(k)) {
            w[off + j] *= wscale
        }
    }
    }
}

// The shared expert's scalar sigmoid gate: σ(gate_vec_l · x). One body for decode and prefill.
// Tuned dot on raw pointers, not an array_view loop — the block invoke + bounds-checked indexing
// was an optimization boundary around a plain dot (reassociates the sum: tolerance-level change).
def moe_shexp_gate(t : Model; l : int64; xp : float const?) : float {
    let dim = t.config.dim
    unsafe {
        return 1.0 / (1.0 + exp(-dot(addr(t.fblob[t.shexp_gate_off + l * dim]), xp, dim)))
    }
}
