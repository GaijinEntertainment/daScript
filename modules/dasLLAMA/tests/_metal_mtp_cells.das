options gen2
options _dasllama_internal = true

module _metal_mtp_cells shared public

//! The Metal MTP/NextN rail's cells per model file, GPU against GPU
//! on the served image a block's `metal_form(image = true)` loads:
//! forced feed holds the verify rows to the plain step within a bar,
//! counting walks stay token-exact; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029,LINT019 - the public facade
require dasllama/dasllama_batch   // nolint:STYLE030,LINT019 - verify_batch_step
require dasllama/dasllama_mtp_gemma   // nolint:STYLE030,LINT019 - gemma_drafter_sidecar
require ?das_metal dasllama/dasllama_metal_mtp_gemma   // nolint:STYLE030,LINT019 - the drafter
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - the decode stats
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - attn_d_head_ok
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - the Metal prefill
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the das_metal halves
require math
require strings
require daslib/strings_boost   // nolint:STYLE030,LINT019 - split, the crown list's parse
require _model_tier
require _model_form   // with_ctx_cut: the cells' context on the form's one load
require _resident_feed   // nolint:STYLE029,LINT019 - sampled_stream, inside the das_metal halves
require ../performance/model_specs.das   // nolint:STYLE030,LINT019 - counting_prompt
require _metal_blob_twin   // metal_kernel_count - the census reader
require _format_census   // nolint:STYLE029,STYLE030,LINT019 - census_cell, das_metal halves
require _kernel_census   // nolint:STYLE029,STYLE030,LINT019 - kernel_census_reset_metal, inside the das_metal halves

//! the context every cell here cuts its form's model to: each of
//! the sixteen model files' largest
let METAL_MTP_CELLS_CTX = 512l

typedef private Coverage = table<string; int64>

//! every kernel's dispatches over one Metal prefill of `prompt` on a
//! fresh session, with the draft head's warm on or off
def private prefill_census(m : Model; prompt : array<int64>;
                           warm : bool) : Coverage {   // nolint:LINT012,LINT019 - das_metal half
    var cov : Coverage
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior = get_mtp_spec()
        set_mtp_spec(warm)
        var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        kernel_census_reset_metal()
        metal_prefill_eval(m, s, prompt)
        cov <- metal_kernel_coverage()
        set_mtp_spec(prior)
        delete s
    }
    return <- cov
}

//! whether a kernel name is a dispatch the head's warm must not add:
//! attention, routed or shared-expert, FFN activation
def private heavy_kernel(lk : string) : bool {
    let qk = find(lk, "qk_") >= 0 && find(lk, "rope") < 0
    //! a hyper-connection mixer's scatter logits ride the router GEMV
    let routed = find(lk, "moe") >= 0 && find(lk, "router") < 0
    return (qk || routed || find(lk, "attn") >= 0 || find(lk, "_av") >= 0 || find(lk, "swiglu") >= 0
        || find(lk, "rowstat") >= 0 || find(lk, "pexp") >= 0)
}

//! the draft head's prompt warm runs its K/V alone
//! (`ARCHITECTURE_GPU_MTP.md#mtp-verify-draft-warm`): it adds no
//! attention, routed or FFN dispatch; the rope count rising is the
//! control
def private warm_cell(t : T?; m : Model; tag : string; prompt : array<int64>) {
    var inscope off <- prefill_census(m, prompt, false)
    var inscope on <- prefill_census(m, prompt, true)
    var moved : array<string>
    var heavy = 0l
    var rope = 0l
    for (k, n in keys(on), values(on)) {
        let d = n - (off?[k] ?? 0l)
        continue if (d == 0l)
        moved |> push("{k}+{d}")
        let lk = to_lower(k)
        if (heavy_kernel(lk)) {
            heavy += d
        }
        if (find(lk, "rope") >= 0) {
            rope += d
        }
    }
    to_log(LOG_INFO, "metal warm census {tag}: the warm's extra dispatches {moved}\n")
    t |> success(rope > 0l, ("{tag} warm: the head's K rows were roped and stored ({rope} extra " +
        "rope dispatches)"))
    t |> equal(heavy, 0l, ("{tag} warm: no attention, routed, shared-expert or FFN activation " +
        "dispatch for the head ({moved})"))
    delete moved
}

def private gpu_prefill_pair(m : Model; var sa, sb : Session; prompt : array<int64>) {
    metal_prefill_eval(m, sa, prompt)
    metal_prefill_eval(m, sb, prompt)
}

struct private FfStats {
    steps : int64
    drafted : int64   //!< rounds the spec rail drafted and verified
    cold : int64      //!< rounds the rail declined: a plain step instead
    served : int64    //!< Metal decode steps over the pair: 0 the GPU declined
    flips : int64
    maxd : float
    flip_margin_max : float
    off_min : float = 1e30   //!< the one-step-off control: B's row to A's PREVIOUS step, smallest
}

//! one forced-feed step's distances into `st`: B's row against A's
//! step, and against A's previous step (`prev`) where one stands
def private ff_step_stats(var st : FfStats; sa, sb : Session; prev : array<float>;
                          tok, vocab : int64) {
    st.maxd = max(st.maxd, logits_maxdiff(sa.logits, sb.logits))
    if (!empty(prev)) {
        st.off_min = min(st.off_min, logits_maxdiff(prev, sb.logits))
    }
    if (argmax_of(sb.logits, vocab) != tok) {
        st.flips++
        st.flip_margin_max = max(st.flip_margin_max, top2_margin(sa.logits, vocab))
    }
}

//! The forced-feed pair: A steps plainly on the GPU, B runs the
//! speculative round with every draft forced to reject, both fed A's
//! greedy token; `verify = false` is the control (B steps plainly)
def private ff_pair(m : Model; prompt : array<int64>; nsteps : int64; verify : bool;
                    depth : int64 = 1l) : FfStats {   // nolint:LINT012,LINT019 - das_metal half
    var st = FfStats()   // nolint:LINT003,LINT019 - filled in the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = m.config
        let vocab = c.vocab_size
        with_dasllama_jobque() {
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(m, sa, sb, prompt)
            with_metal_rails(MetalRails(prefill = false, min_npos = 0l)) {
                let before = metal_decode_stats().decodes
                var tok = argmax_of(sa.logits, vocab)
                var one <- [tok]
                eval_(m, sa, one)
                eval_(m, sb, one)
                tok = argmax_of(sa.logits, vocab)
                let reject0 = get_mtp_force_reject_every()
                let depth0 = mtp_depth_pin()
                if (verify) {
                    set_mtp_force_reject_every(1l)
                    set_mtp_depth(depth)
                }
                var prev : array<float>
                for (_i in range64(nsteps)) {
                    ff_step_stats(st, sa, sb, prev, tok, vocab)
                    prev := sa.logits
                    one[0] = tok
                    eval_(m, sa, one)
                    if (verify) {
                        let nd0 = sb.mtp_drafted
                        mtp_spec_round(m, sb, tok)
                        if (sb.mtp_drafted == nd0) {
                            st.cold++
                        } else {
                            st.drafted++
                        }
                    } else {
                        eval_(m, sb, one)
                    }
                    st.steps++
                    tok = argmax_of(sa.logits, vocab)
                }
                set_mtp_force_reject_every(reject0)
                restore_mtp_depth(depth0)
                st.served = metal_decode_stats().decodes - before
                delete prev
                delete one
            }
            delete sa
            delete sb
        }
    }
    return st
}

var private g_last_accepted = 0l   //! the drafts the last speculative free-run accepted

//! top-k 1 makes every draw the penalized argmax, so a seeded
//! sampled stream is deterministic and the token-exact gate holds
let private SAMPLED_PARAMS = SamplingParams(temp = 0.7, top_k = 1l, penalty = 1.1)

//! what one free-running pair reports: the steps the Metal decode
//! served, the speculative side's drafts, and its parked draws
struct private FreeRun {
    served : int64
    drafted : int64
    parked : int64
}

//! the speculative side of a free-running pair: `n` tokens of `sb`'s
//! rounds at `depth` into `spec`, every accepted draft committed
def private spec_walk(m : Model; var sb : Session; n, depth : int64; sampled : bool;
                      sp : SamplingParams; var spec : array<int64>) : int64 {
    var parked = 0l
    let vocab = m.config.vocab_size
    let depth0 = mtp_depth_pin()
    set_mtp_depth(depth)
    g_last_accepted = 0l
    spec |> reserve(n)
    var tok = sampled ? sample_(sb, sp) : argmax_of(sb.logits, vocab)
    while (long_length(spec) < n) {
        spec |> push(tok)
        let a = mtp_spec_round(m, sb, tok)
        g_last_accepted += a
        for (i in range64(a)) {
            spec |> push(sb.mtp_vbatch[i + 1l])
        }
        if (sb.mtp_pre_drawn) {
            parked++
        }
        tok = sampled ? sample_(sb, sp) : argmax_of(sb.logits, vocab)
    }
    restore_mtp_depth(depth0)
    spec |> resize(n)
    return parked
}

//! Free-running pair on the GPU, greedy or through the session
//! sampler: a token-exact gate only on a counting prompt
def private free_run_pair(m : Model; prompt : array<int64>; n : int64;
                          var plain, spec : array<int64>; depth : int64; sampled : bool;
                          kdt : KVDtype) : FreeRun {   // nolint:LINT012,LINT019 - das_metal half
    var fr = FreeRun()   // nolint:LINT003,LINT019 - counted in the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vocab = m.config.vocab_size
        var sp = SAMPLED_PARAMS
        with_dasllama_jobque() {
            var sa = make_run_state(m.config, kdt, kdt)
            var sb = make_run_state(m.config, kdt, kdt)
            gpu_prefill_pair(m, sa, sb, prompt)
            with_metal_rails(MetalRails(prefill = false, min_npos = 0l)) {
                let before = metal_decode_stats().decodes
                var one <- [argmax_of(sa.logits, vocab)]
                eval_(m, sa, one)
                eval_(m, sb, one)
                if (sampled) {
                    set_seed_(sa, 4242)
                    set_seed_(sb, 4242)
                    sb.spec_params = unsafe(addr(sp))
                }
                var tok = sampled ? sample_(sa, sp) : argmax_of(sa.logits, vocab)
                for (_i in range64(n)) {
                    plain |> push(tok)
                    one[0] = tok
                    eval_(m, sa, one)
                    tok = sampled ? sample_(sa, sp) : argmax_of(sa.logits, vocab)
                }
                fr.parked = spec_walk(m, sb, n, depth, sampled, sp, spec)
                sb.spec_params = null
                fr.drafted = sb.mtp_drafted
                fr.served = metal_decode_stats().decodes - before
                delete one
            }
            delete sa
            delete sb
        }
    }
    return fr
}

//! the GPU-vs-GPU forced-feed envelope: two Metal rails on the
//! same feeds differ by kernel spelling only; the one-step decode
//! gate's bar
let private FF_TOL = 0.5
//! the plain-MoE bar (q30's): the rows and single GEMV forms round
//! the router's logits apart, and a near-tie flips an expert
let private MOE_FF_TOL = 2.0
//! the nine-row kq mul_mm and the half-x small-batch forms stage x
//! to half: a wider envelope than the f32-x GEMV forms', flips still
//! zero
let private MM_TOL = 1.5
let private FF_STEPS = 48l
let private COUNT_STEPS = 40l
//! the leading tokens the 26B's drafted counting run shares with the
//! plain one at draft depth 2 and past it: a verify row flips at a
//! near-tie under the routed-expert bar
let private G26_COUNT_AGREE = 32l

//! the carrier is served, so a decline or a rail that never drafts
//! fails; then the tolerance gate with its flip telemetry
def private ff_cell(t : T?; m : Model; tag, pn : string; prompt : array<int64>; depth : int64 = 1l;
                    tol : float = FF_TOL) {
    let st = ff_pair(m, prompt, FF_STEPS, true, depth)
    if (st.served == 0l) {
        t |> failure("{tag} {pn}: the metal decode declined a served carrier's steps")
    } elif (st.drafted == 0l) {
        t |> failure(("{tag} {pn}: the spec rail never engaged ({st.cold} cold rounds) - nothing " +
            "verified"))
    } else {
        t |> success(st.cold == 0l, ("{tag} {pn}: every round drafted+verified ({st.drafted} " +
            "drafted, {st.cold} cold)"))
        t |> success(st.maxd <= tol, ("{tag} {pn}: verify row0 vs plain step within {tol} over " +
            "{st.steps} forced-feed steps (maxd {st.maxd}, argmax flips {st.flips}, widest " +
            "flipped margin {st.flip_margin_max})"))
        t |> success(st.off_min > tol, ("{tag} {pn} control: verify row0 against the PREVIOUS " +
            "plain step lands outside {tol} at every step (smallest {st.off_min})"))
        to_log(LOG_INFO, ("metal mtp {tag} {pn} forced-feed: maxd={st.maxd} " +
            "flips={st.flips}/{st.steps} widest_margin={st.flip_margin_max} drafted={st.drafted} " +
            "cold={st.cold} one_step_off_min={st.off_min}\n"))
    }
}

//! whether any rows site reads a plane `pred` takes: q, k, v, o and
//! the dense FFN triple, and with `every_site` the rest the verify
//! runs - the shared expert's triple, deltanet and the classifier
def private any_rows_site(m : Model; every_site : bool; pred : block<(f : KqFmt) : bool>) : bool {
    return true if (every_site && invoke(pred, m.wcls_fmt))
    for (l in range64(long_length(m.wq_offs))) {
        for (
             f in [fmt_at(m.wq_fmt, l), fmt_at(m.wk_fmt, l), fmt_at(m.wv_fmt, l),
                   fmt_at(m.wo_fmt, l),
                fmt_at(m.w1_fmt, l), fmt_at(m.w3_fmt, l), fmt_at(m.w2_fmt, l)]) {
            return true if (invoke(pred, f))
        }
        continue if (!every_site)
        for (f in [fmt_at(m.dnqkv_fmt, l), fmt_at(m.dngate_fmt, l), fmt_at(m.dnout_fmt, l),
                fmt_at(m.dnbeta_fmt, l), fmt_at(m.dnalpha_fmt, l)]) {
            return true if (invoke(pred, f))
        }
        if (m.config.n_ff_shexp > 0l) {
            for (kind in [1, 2, 3]) {
                return true if (invoke(pred, shexp_plane(m, l, kind).f))
            }
        }
    }
    return false
}

//! whether any attention or dense FFN site reads a k4 / k5 / k6
//! plane, the formats the half-x leg pins crowns for
def private has_kq_rows_planes(m : Model) : bool =>
                   any_rows_site(m, false, $(f) => f == KqFmt.k4 || f == KqFmt.k5 || f == KqFmt.k6)

//! a superblock plane the box leaves on the small-batch forms past
//! eight rows (`ARCHITECTURE_GPU_MTP_DECODE_ROWS.md#kq-rows-crown`)
def private rides_small_batch(f : KqFmt) : bool {
    var rides = kq_sb(f)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        rides = rides && !metal_kq_rows_crowned(f)
    }
    return rides
}

//! whether any rows site the nine-row verify runs rides the
//! small-batch forms past eight rows
def private has_wide_rows_sites(m : Model) : bool =>
                   any_rows_site(m, true, $(f) => rides_small_batch(f))

//! whether a verify of two to eight rows reads x in half at some rows
//! site: the format's half-x forms serve on the box
def private has_half_x_rows_sites(m : Model) : bool {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return any_rows_site(m, true, $(f) => kq_sb(f) && metal_kq_half_on(f))
    } else {
        return false
    }
}

//! the counters a counting walk reads before its run
struct private WalkCounters {
    chained : int64
    wide : int64
    tiled : int64
}

def private walk_counters() : WalkCounters {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return WalkCounters(chained = metal_mtp_chained_rounds(), wide = metal_kq_mvb_wide_count(),
            tiled = metal_rows8_tiled_sites())
    } else {
        return WalkCounters()
    }
}

//! the walk's command-buffer chaining at depth 1 and its nine-row
//! dispatch forms at depth 8, read against the counters before it
def private walk_rail_asserts(t : T?; m : Model; tag, kind : string; depth, drafted : int64;
                              sampled : bool;
                              wc0 : WalkCounters) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let wc = walk_counters()
        let chained = wc.chained - wc0.chained
        let wide = wc.wide - wc0.wide
        if (depth == 1l) {
            //! a greedy depth-1 round chains on any table the device
            //! gathers; sampled rounds, the n-gram model and the
            //! assistant's CPU oracle drafts never chain
            let chains = (!sampled && emb_dev_gather_ok(m) && !has_ple_ngram(m.config)
                && !gemma_draft_on_cpu())
            t |> equal(chained, chains ? drafted : 0l, ("{tag} {kind} k1: every drafted round " +
                "{chains ? "rode" : "kept off"} one command buffer (table {m.emb_fmt}, {chained} " +
                "chained of {drafted} rounds)"))
        }
        if (depth == 8l) {
            //! nine verify rows are one dispatch a superblock plane:
            //! over two row tiles on the rows8 form where the box
            //! runs it, else on the wide small-batch form
            let kq = has_wide_rows_sites(m)
            let tiled = wc.tiled - wc0.tiled
            to_log(LOG_INFO, ("{tag} {kind} k8: {wide} K-quant dispatches past eight rows, " +
                "{tiled} sites over row tiles (small-batch rows sites: {kq}, the rows8 form on: " +
                "{metal_rows8_on()})\n"))
            t |> success(kq ? wide + tiled > 0l : wide == 0l, ("{tag} {kind} k8: the verify's " +
                "nine rows rode one dispatch a superblock site - a tiled rows8 site or a wide " +
                "small-batch one - and no wide one elsewhere ({wide} past-eight dispatches, " +
                "{tiled} tiled sites, small-batch sites {kq})"))
            t |> success(metal_rows8_on() || tiled == 0l, ("{tag} {kind} k8: no site took the " +
                "row tiles on a box that does not run the rows8 form ({tiled} tiled sites)"))
        }
    }
}

//! whether the fused attention kernel takes the model's head shapes
def private fused_attn_shape(c : Config) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return attn_d_head_ok(c.head_size) && (c.head_size_swa == 0l
                                               || attn_d_head_ok(c.head_size_swa))
    } else {
        return false
    }
}

//! the verify's attention form over a counting prompt past 64 tokens:
//! fused where the head shape takes it, else the chunked pair
def private walk_attn_asserts(t : T?; m : Model; tag, kind : string; count : array<int64>;
                              depth : int64; kdt : KVDtype) {
    let c = m.config
    let fused_shape = fused_attn_shape(c)
    //! the mirror codec's stamps
    let cs = (kdt == KVDtype.q8_0 ? "Q8"
        : (kdt == KVDtype.tq4 ? "Tq4" : (kdt == KVDtype.f32 ? "F32" : "F16")))
    let chunked = metal_kernel_count("MetalSqAttnPartB{cs}_metal_sq_attn_part_b")
    let fused = (metal_kernel_count("MetalSqAttnD{cs}_metal_sq_attn_d")
        + metal_kernel_count("MetalSqAttnDW{cs}_metal_sq_attn_d")
        + metal_kernel_count("MetalSqAttnDX{cs}_metal_sq_attn_d"))
    to_log(LOG_INFO, ("{tag} {kind} k{depth} {kdt}: prompt {long_length(count)} tokens, head " +
        "{c.head_size}, fused shape {fused_shape}: {fused} fused dispatches, {chunked} chunked\n"))
    if (long_length(count) > 64l && fused_shape) {
        t |> success(fused > 0l && chunked == 0l, ("{tag} {kind} k{depth}: the verify's rows " +
            "took the fused attention under DASLLAMA_METAL_ATTN_D pinned on ({fused} fused " +
            "dispatches, {chunked} chunked)"))
    } elif (long_length(count) > 64l) {
        t |> success(chunked > 0l, ("{tag} {kind} k{depth}: a shape the fused kernel does not " +
            "carry keeps the verify on the chunked pair ({chunked} dispatches)"))
    }
}

//! the free run's own gates: engaged, the arm's depth reached, the
//! plain run's tokens shared, the sampled walk's parked draws
def private walk_stream_asserts(t : T?; tag, kind : string; depth, agree : int64; sampled : bool;
                                fr : FreeRun; plain, spec : array<int64>) {
    t |> success(fr.drafted > 0l, ("{tag} {kind} k{depth}: the spec rail engaged ({fr.drafted} " +
        "drafts) - a plain fallback would pass vacuously"))
    let positions = mtp_pos_report()
    t |> success(find(positions, "p{depth} ") >= 0, ("{tag} {kind} k{depth}: a round drafted " +
        "position {depth} - the depth the arm names was reached ({positions})"))
    let same = common_prefix_len(plain, spec)
    to_log(LOG_INFO, ("{tag} {kind} k{depth}: the speculative free-run shares {same} of the " +
        "plain one's {long_length(plain)} tokens\n"))
    t |> success(long_length(plain) == long_length(spec) && same >= min(agree, long_length(plain)),
        ("{tag} {kind} k{depth}: the speculative free-run shares the plain free-run's first " +
        "{agree} of {COUNT_STEPS} tokens (shares {same})"))
    t |> success(sampled == (fr.parked > 0l), ("{tag} {kind} k{depth}: the sampled walk parks " +
        "the next draw ({fr.parked} rounds); the argmax walk parks none"))
}

//! answers the rounds the spec rail drafted - the draft-quality
//! witness a crowned leg holds against the plain run's; `attn_form`
//! off on a leg that pins the verify onto another attention form
def private count_cell(t : T?; m : Model; tag : string; count : array<int64>; depth : int64;
                       sampled : bool = false; kdt : KVDtype = KVDtype.f16;
                       agree : int64 = COUNT_STEPS; attn_form : bool = true) : int64 {
    var plain, spec : array<int64>
    mtp_pos_reset()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        kernel_census_reset_metal()
    }
    let wc0 = walk_counters()
    var fr = FreeRun()
    with_metal_attn_d(true) $ {
        fr = free_run_pair(m, count, COUNT_STEPS, plain, spec, depth, sampled, kdt)
    }
    let kind = sampled ? "sampled" : "counting"
    if (fr.served > 0l) {
        walk_rail_asserts(t, m, tag, kind, depth, fr.drafted, sampled, wc0)
    }
    if (fr.served == 0l) {
        t |> failure("{tag} {kind} k{depth}: the metal decode declined a served carrier's steps")
    } else {
        if (attn_form) {
            walk_attn_asserts(t, m, tag, kind, count, depth, kdt)
        }
        log_gen_texts(m, "{tag} {kind} k{depth}", plain, spec)
        walk_stream_asserts(t, tag, kind, depth, agree, sampled, fr, plain, spec)
    }
    delete plain
    delete spec
    return fr.drafted
}

//! the half-x stamps' dispatches over the last walk: the k4, k5 and
//! k6 forms at two, four and eight rows
def private half_x_stamps() : int64 {
    return (metal_kernel_count("MetalKqMvW2K4H_metal_kq_mvw_k4")
        + metal_kernel_count("MetalKqMvW4K4H_metal_kq_mvw_k4")
        + metal_kernel_count("MetalKqMvW8K4H_metal_kq_mvw_k4")
        + metal_kernel_count("MetalKqMvB2K5H_metal_kq_mv_k5")
        + metal_kernel_count("MetalKqMvB4K5H_metal_kq_mv_k5")
        + metal_kernel_count("MetalKqMvB8K5H_metal_kq_mvb8")
        + metal_kernel_count("MetalKqMvB2K6H_metal_kq_mv_k6")
        + metal_kernel_count("MetalKqMvB4K6H_metal_kq_mv_k6")
        + metal_kernel_count("MetalKqMvB8K6H_metal_kq_mvb8_k6"))
}

//! The half-x leg at depth 1 under the k4 / k5 / k6 crowns, the MTP
//! head's q/k/v among the sites: its drafted rounds hold to the
//! uncrowned run's within a fifth. Shuts the drivers down mid-cell
def private half_crown_cell(t : T?; m : Model; tag : string; count : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let depth = 1l
        let prev_crowns = get_metal_tensor_crowns()
        let prev_pinned = metal_tensor_crowns_pinned()
        let prev_half = get_metal_kq_half()
        //! the plain leg runs with every half-x crown off
        let no_half <- [for (f in split(prev_crowns, ",")); f; where !empty(f)
                        && !(f |> starts_with(KQ_HALF_CROWN_PREFIX))]
        pin_metal_tensor_crowns(join(no_half, ","))
        set_metal_kq_half(false)
        //! both drivers share the device the decode release drops
        metal_prefill_shutdown()
        metal_decode_shutdown()
        let plain_drafted = count_cell(t, m, "{tag} plain-crowns", count, depth)
        set_metal_kq_half(true)
        pin_metal_tensor_crowns("{kq_half_crown(KqFmt.k4)},{kq_half_crown(KqFmt.k5)}," +
            "{kq_half_crown(KqFmt.k6)}")
        metal_prefill_shutdown()
        metal_decode_shutdown()
        let half_drafted = count_cell(t, m, "{tag} half-x", count, depth)
        let half_stamps = half_x_stamps()
        to_log(LOG_INFO, ("{tag} half-x leg: drafted {half_drafted} rounds under the half crowns " +
            "against {plain_drafted} without, {half_stamps} half-x dispatches\n"))
        if (has_kq_rows_planes(m)) {
            t |> success(half_stamps > 0l, ("{tag} half-x: the verify's K-quant rows sites took " +
                "the half-x forms under the crowns ({half_stamps} dispatches) - a leg that " +
                "dispatched none measured the f32 forms twice"))
        } else {
            to_log(LOG_INFO, ("{tag} half-x: no K-quant rows plane on this carrier, the half-x " +
                "dispatch count is not asserted\n"))
        }
        t |> success(half_drafted * 5l >= plain_drafted * 4l, ("{tag} half-x: the drafts under " +
            "the half crowns hold up to the plain run's ({half_drafted} vs {plain_drafted} " +
            "rounds) - a head fed a stale twin drafts garbage and the count collapses"))
        set_metal_kq_half(prev_half)
        if (prev_pinned) {
            pin_metal_tensor_crowns(prev_crowns)
        } else {
            unpin_metal_tensor_crowns()
            set_metal_tensor_crowns(prev_crowns)
        }
        metal_prefill_shutdown()
        metal_decode_shutdown()
    }
}

//! per-row logits comparison stats: flips against the plain top-2
//! margin
struct private RowStats {
    rows : int64
    flips : int64
    max_abs : float
    flip_margin_max : float
    plain_steps : int64     //!< the plain side's steps
    plain_decodes : int64   //!< the plain steps the Metal decode served
    verify_decodes : int64  //!< the verify side's Metal decodes: the control
}

//! one row's compare folded into the stats; the row's widest
//! difference back
def private compare_row(plain, store : array<float>; soff, n : int64; var st : RowStats;
                        tag : string; pos : int64; verbose : bool) : float {
    var mx = 0.0
    for (i in range64(n)) {
        mx = max(mx, nan_wide_diff(plain[i], store[soff + i]))
    }
    st.rows++
    st.max_abs = max(st.max_abs, mx)
    let ap = argmax_of(plain, 0l, n)
    let av = argmax_of(store, soff, n)
    if (ap != av) {
        st.flips++
        let mg = top2_margin(plain, n)
        st.flip_margin_max = max(st.flip_margin_max, mg)
        if (verbose) {
            feint("FLIP {tag} pos={pos}: plain {ap} vs verify {av} margin {mg} maxd {mx}\n")
        }
    }
    return mx
}

//! one round's verify rows against the plain steps' stored rows: each
//! row compared, its widest difference kept a row
def private vff_round_rows(store, verify : array<float>; vocab, nr, pos : int64; var st : RowStats;
                           var rowmax : array<float>; verbose : bool) {
    for (i in range64(nr)) {
        var plain : array<float>
        plain |> resize(vocab)
        for (v in range64(vocab)) {
            plain[v] = store[i * vocab + v]
        }
        let rmx = compare_row(plain, verify, i * vocab, vocab, st, "vrow{i}", pos + i, verbose)
        rowmax[i] = max(rowmax[i], rmx)
        if (verbose) {
            to_log(LOG_INFO, ("vff round pos={pos + i} row={i} maxd={rmx} " +
                "plain_top={argmax_of(plain, 0l, vocab)} " +
                "verify_top={argmax_of(verify, i * vocab, vocab)} " +
                "margin={top2_margin(plain, vocab)}\n"))
        }
        delete plain
    }
}

//! A's `nr` plain steps from `tok`, every step's logits recorded into
//! `store` and the fed tokens into `toks`; the next token back
def private vff_plain(m : Model; var sa : Session; var one, toks : array<int64>;
                      var store : array<float>; tok0, nr : int64;
                      var st : RowStats) : int64 {   // nolint:LINT012,LINT019 - das_metal half
    var tok = tok0
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let vocab = m.config.vocab_size
        for (i in range64(nr)) {
            toks[i] = tok
            one[0] = tok
            let d0 = metal_decode_stats().decodes
            eval_(m, sa, one)
            finish_pending_step()
            st.plain_decodes += metal_decode_stats().decodes - d0
            st.plain_steps++
            for (v in range64(vocab)) {
                store[i * vocab + v] = sa.logits[v]
            }
            tok = argmax_of(sa.logits, vocab)
        }
    }
    return tok
}

//! The batch-rail verify (one session at `nr` consecutive positions
//! through the batch driver's same-slab mode) against the plain GPU
//! step, forced-feed: each verify row against A's step there
def private vff_pair(m : Model; prompt : array<int64>; nrounds, nr : int64; var st : RowStats;
                     var served : int64&;
                     verbose : bool) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = m.config
        let vocab = c.vocab_size
        with_dasllama_jobque() {
            var sa = make_run_state(c, KVDtype.f16, KVDtype.f16)
            var sb = make_run_state(c, KVDtype.f16, KVDtype.f16)
            gpu_prefill_pair(m, sa, sb, prompt)
            with_metal_rails(MetalRails(prefill = false, min_npos = 0l)) {
                let before = metal_batch_decode_stats().steps
                var ws <- create_batch_workspace_(m)
                var tok = argmax_of(sa.logits, vocab)
                var one <- [tok]
                eval_(m, sa, one)
                eval_(m, sb, one)
                finish_pending_step()
                tok = argmax_of(sa.logits, vocab)
                var pos = sb.n_past
                var toks : array<int64>
                var store, rowmax : array<float>
                toks |> resize(nr)
                store |> resize(nr * vocab)
                rowmax |> resize(nr)
                for (_r in range64(nrounds)) {
                    break if (pos + nr + 1l >= c.seq_len)
                    tok = vff_plain(m, sa, one, toks, store, tok, nr, st)
                    let v0 = metal_decode_stats().decodes
                    let verified = verify_batch_step(m, ws, sb, toks, pos, nr)
                    st.verify_decodes += metal_decode_stats().decodes - v0
                    break if (!verified)
                    sb.n_past = pos + nr
                    vff_round_rows(store, sb.mtp_logits_b, vocab, nr, pos, st, rowmax, verbose)
                    pos += nr
                }
                served = metal_batch_decode_stats().steps - before
                to_log(LOG_INFO, "metal vff per-row maxd: {rowmax}\n")
                delete ws
                delete one
                delete toks
                delete store
                delete rowmax
            }
            delete sa
            delete sb
        }
    }
}

//! the same model, DISTINCT sessions: GPU single step vs GPU batch
//! step on identical tokens - the batch rail's own parity, so a
//! same-slab failure can be told from a rail failure
def private dff_pair(m : Model; prompts : array<array<int64>>; nsteps : int64; var st : RowStats;
                     var served : int64&;
                     verbose : bool) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let c = m.config
        let vocab = c.vocab_size
        let nb = long_length(prompts)
        with_dasllama_jobque() {
            var sa, sb : array<Session>
            sa |> reserve(nb)
            sb |> reserve(nb)
            //! a recurrent carrier: every session keeps a live mirror
            //! and a prefill mints one - the room comes before them
            dn_mirror_room(2l * nb + 1l)
            for (i in range64(nb)) {
                sa |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                sb |> emplace <| make_run_state(c, KVDtype.f16, KVDtype.f16)
                gpu_prefill_pair(m, sa[i], sb[i], prompts[i])
            }
            with_metal_rails(MetalRails(prefill = false, min_npos = 0l)) {
                let before = metal_batch_decode_stats().steps
                var ws <- create_batch_workspace_(m)
                var sessb : array<Session?>
                var cur : array<int64>
                sessb |> reserve(nb)
                cur |> reserve(nb)
                for (i in range64(nb)) {
                    sessb |> push(unsafe(addr(sb[i])))
                    cur |> push(argmax_of(sa[i].logits, vocab))
                }
                var one <- [0l]
                for (_step in range64(nsteps)) {
                    for (i in range64(nb)) {
                        one[0] = cur[i]
                        eval_(m, sa[i], one)
                        finish_pending_step()
                    }
                    eval_batch_(m, ws, sessb, cur)
                    for (i in range64(nb)) {
                        compare_row(sa[i].logits, sb[i].logits, 0l, vocab, st, "drow{i}",
                                    sb[i].n_past - 1l, verbose)
                        cur[i] = argmax_of(sa[i].logits, vocab)
                    }
                }
                served = metal_batch_decode_stats().steps - before
                sessb |> clear()
                unsafe {
                    delete sessb
                }
                delete ws
                delete one
                delete cur
            }
            delete sa
            delete sb
        }
    }
}

//! a carrier the file loads is a carrier the batch rail serves: a
//! decline fails the cell
def private dff_cell(t : T?; m : Model; tag : string; prose_a, prose_b : array<int64>; tol : float;
                     wide : bool = false) {
    for (nb in (wide ? [5l, 8l] : [2l, 4l])) {
        var inscope sub : array<array<int64>>
        sub |> reserve(nb)
        for (i in range64(nb)) {
            sub |> emplace <| clone_to_move((i & 1l) == 0l ? prose_a : prose_b)
        }
        var st = RowStats()
        var served = 0l
        dff_pair(m, sub, 24l, st, served, false)
        if (served == 0l) {
            t |> failure(("{tag} dff B={nb}: the metal batch rail declined distinct-session " +
                "steps on a carrier it serves"))
        } else {
            t |> success(st.max_abs <= tol, ("{tag} dff B={nb}: GPU batch step vs GPU single " +
                "step within {tol} over {st.rows} rows (maxd {st.max_abs}, argmax flips " +
                "{st.flips}, widest flipped margin {st.flip_margin_max})"))
            to_log(LOG_INFO, ("metal dff {tag} B={nb}: maxd={st.max_abs} " +
                "flips={st.flips}/{st.rows} widest_margin={st.flip_margin_max} steps={served}\n"))
        }
        delete sub
    }
}

//! a widened verify bar's control: the same `nr`-row verify with the
//! GEMV stage knocked out must land OUTSIDE the bar
def private vff_control_cell(t : T?; m : Model; tag : string; prompt : array<int64>; nr : int64;
                             bar : float) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var st = RowStats()
        var served = 0l
        let skip0 = get_metal_decode_skip()
        set_metal_decode_skip("gemv")
        vff_pair(m, prompt, 4l, nr, st, served, false)
        set_metal_decode_skip(skip0)
        if (served == 0l) {
            t |> failure(("{tag} vff{nr} control: the metal batch rail declined the same-slab " +
                "verify"))
        } else {
            t |> success(st.plain_steps > 0l && st.plain_decodes == st.plain_steps,
                ("{tag} vff{nr} control: every plain step was a Metal decode ({st.plain_decodes} " +
                "of {st.plain_steps})"))
            t |> equal(0l, st.verify_decodes, ("{tag} vff{nr} control: the batch verify counted " +
                "no single-row Metal decode"))
            t |> success(st.max_abs > bar, ("{tag} vff{nr} control: the {nr}-row verify with its " +
                "GEMV stage knocked out reds the {bar} bar (maxd {st.max_abs} over {st.rows} " +
                "rows)"))
        }
    }
}

//! the batch rail's decline table, as text
def private declines_text() : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return "{metal_batch_decode_declines()}"
    } else {
        return ""
    }
}

//! DASLLAMA_METAL_KQ_HALF as the box reads it
def private kq_half_knob() : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return get_metal_kq_half()
    } else {
        return true
    }
}

//! a served verify's gates: rows compared, every plain step a Metal
//! decode, and the rows within the bar, its control where widened
def private vff_served_asserts(t : T?; m : Model; tag : string; prompt : array<int64>; tol : float;
                               nr : int64; st : RowStats; served : int64) {
    //! two to eight rows on a half-x site read x in half, the
    //! nine-row mul_mm's envelope - read once the step armed the
    //! crowns
    let half = nr >= 2l && nr <= 8l && has_half_x_rows_sites(m)
    let bar = half ? max(tol, MM_TOL) : tol
    t |> success(st.rows > 0l, "{tag} vff: rows compared ({st.rows})")
    t |> success(st.plain_steps > 0l && st.plain_decodes == st.plain_steps,
        ("{tag} vff: every plain step was a Metal decode ({st.plain_decodes} of {st.plain_steps})"))
    t |> equal(0l, st.verify_decodes, ("{tag} vff: the batch verify counted no single-row Metal " +
        "decode (the plain count's control)"))
    let kq_half = kq_half_knob()
    t |> success(st.max_abs <= bar, ("{tag} vff: batch same-slab verify rows vs plain steps " +
        "within {bar} over {st.rows} rows (maxd {st.max_abs}, argmax flips {st.flips}, widest " +
        "flipped margin {st.flip_margin_max}, half-x sites {half}, DASLLAMA_METAL_KQ_HALF " +
        "{kq_half})"))
    to_log(LOG_INFO, ("metal vff {tag}: maxd={st.max_abs} flips={st.flips}/{st.rows} " +
        "widest_margin={st.flip_margin_max} steps={served} half_x={half}\n"))
    if (bar > tol) {
        vff_control_cell(t, m, tag, prompt, nr, bar)
    }
}

//! a decline fails the cell unless `expect_decline` names the reason
//! the carrier declines for
def private vff_cell(t : T?; m : Model; tag : string; prompt : array<int64>; tol : float;
                     nr : int64 = 4l; expect_decline : string = "") {
    var st = RowStats()
    var served = 0l
    let before = empty(expect_decline) ? 0l : metal_batch_decline_count(expect_decline)
    vff_pair(m, prompt, 12l, nr, st, served, false)
    if (expect_decline != "") {
        let after = metal_batch_decline_count(expect_decline)
        let by_key = declines_text()
        t |> success(served == 0l && after > before, ("{tag}: the metal batch rail declines this " +
            "graph as {expect_decline} ({before} -> {after}, served {served}; declines {by_key})"))
    } elif (served == 0l) {
        t |> failure(("{tag}: the metal batch rail declined the same-slab verify on a carrier it " +
            "serves (declines {declines_text()})"))
    } else {
        vff_served_asserts(t, m, tag, prompt, tol, nr, st, served)
    }
}

//! the nine-row arm: the kq carriers take the wider mul_mm bar, and a
//! bar wider than the standing one ships its control in the arm
def private vff9_cell(t : T?; m : Model; tag : string; prompt : array<int64>; tol : float;
                      expect_decline : string) {
    vff_cell(t, m, tag, prompt, m.kquant_native ? max(tol, MM_TOL) : tol, 9l, expect_decline)
    if (m.kquant_native && empty(expect_decline)) {
        vff_control_cell(t, m, tag, prompt, 9l, MM_TOL)
    }
}

//! the parameters of one model file's `mtp_parity_metal` call
struct MtpParityRow {
    file : string    //!< the model file under the models folder
    tag : string     //!< the arms' suffix: `mtp-<stem>-<tag>`
    tol : float = FF_TOL   //!< the forced-feed bar
    blocks : int64   //!< past 0: the trunk blocks a cut's name claims
    held : string    //!< the comma list of arm stems a cut leaves on its whole file
    on_whole : bool  //!< the whole file's run: only the `held` arms
    family : string  //!< the family the cells run under: "" untagged
    head : string    //!< the NextN head file the block's form pins: "" the file's own
    served_only : bool   //!< the head-split hyper-connection shape: the dff arm, no census
}

//! whether the arm `stem` runs on this row: an arm of `held` runs on
//! the whole file alone (`on_whole`), every other arm on the row's
def private fixture_arm(t : T?; stem : string; p : MtpParityRow) : bool =>
                   (list_names(p.held, stem) == p.on_whole
    && arm_on(t, "{stem}-{p.tag}"))

//! the forced-feed arms: the plain-vs-plain control, the verify's row
//! 0 at depth 1, 2 and 4, the same-slab verify, the head's warm
def private ff_arms(t : T?; m : Model; p : MtpParityRow;
                    prose_a, prose_b : array<int64>) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (fixture_arm(t, "mtp-ctrl", p)) {
            let st = ff_pair(m, prose_a, 16l, false)
            if (st.served == 0l) {
                t |> failure("{p.tag} control: the metal decode declined a served carrier's steps")
            } else {
                t |> success(st.maxd == 0.0 && st.flips == 0l, ("{p.tag} control: plain-vs-plain " +
                    "forced feed is bit-identical (maxd {st.maxd}, flips {st.flips})"))
            }
        }
        if (fixture_arm(t, "mtp-ff", p)) {
            ff_cell(t, m, p.tag, "prose-a", prose_a, 1l, p.tol)
            ff_cell(t, m, p.tag, "prose-b", prose_b, 1l, p.tol)
        }
        //! a hyper-connection carrier's same-slab rows are one
        //! session, which the rail declines as `graph`
        if (fixture_arm(t, "mtp-vff", p)) {
            vff_cell(t, m, p.tag, prose_a, FF_TOL, 4l, m.config.hyper_conn ? "graph" : "")
        }
        if (fixture_arm(t, "mtp-warm", p)) {
            warm_cell(t, m, p.tag, prose_a)
        }
        if (fixture_arm(t, "mtp-ffk", p)) {
            ff_cell(t, m, "{p.tag} k2", "prose-a", prose_a, 2l, p.tol)
            ff_cell(t, m, "{p.tag} k4", "prose-b", prose_b, 4l, p.tol)
        }
    }
}

//! The packed tile stamp under the depth-1 counting walk: pinned on
//! it serves the verify and the walk still emits the plain walk's
//! tokens; pinned off, the control, it dispatches none
def private tile_pack_cell(t : T?; m : Model; tag : string; count : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        for (on in [true, false]) {
            with_metal_attn_tile_far(on ? 0l : -1l) {
                count_cell(t, m, "{tag} tile {on ? "on" : "off"}", count, 1l, false, KVDtype.f16,
                    COUNT_STEPS, !on)
            }
            let packed = metal_kernel_count("metal_sq_attn_tile_pk")
            if (on) {
                t |> success(packed > 0l, ("{tag}: the verify's rows took the packed tile stamp " +
                    "({packed} dispatches)"))
            } else {
                t |> equal(packed, 0l, "{tag} control: the tile form pinned off dispatches none")
            }
        }
    }
}

//! the Metal home's opening of a sampled stream: the prompt through
//! the Metal prefill and one eval - the mirror-building step a round
//! needs live
def private metal_sampled_open(m : Model; var s : Session; prompt : array<int64>) {
    metal_prefill_eval(m, s, prompt)
    var one <- [argmax_of(s.logits, m.config.vocab_size)]
    eval_(m, s, one)
    delete one
}

//! a seeded sampled stream token for token with the selection landed
//! and with the rows landed, every served step's selection landed
def private select_cell(t : T?; m : Model; tag : string; prompt : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_dasllama_jobque() {
            var sp = SamplingParams(temp = 0.7, top_p = 0.9)
            let sel0 = get_select_landing()
            with_metal_rails(MetalRails(prefill = false, min_npos = 0l)) {
                let served0 = metal_decode_stats().decodes
                set_select_landing(false)
                var rows <- sampled_stream(m, 40l, sp, 4242, KVDtype.f16) $(var s : Session) {
                    metal_sampled_open(m, s, prompt)
                }
                let served_rows = metal_decode_stats().decodes - served0
                set_select_landing(true)
                var sel <- sampled_stream(m, 40l, sp, 4242, KVDtype.f16) $(var s : Session) {
                    metal_sampled_open(m, s, prompt)
                }
                let served_sel = metal_decode_stats().decodes - served0 - served_rows
                set_select_landing(sel0)
                t |> success(served_rows > 0l && served_sel > 0l, ("{tag} select: both streams " +
                    "were device-served ({served_rows} and {served_sel} steps)"))
                selection_agrees(t, m, tag, rows, sel)
                delete rows
                delete sel
            }
        }
    }
}

//! the walk arms: counting at depth 1, 2, 4 and 8, the other mirror
//! codecs, the half-x crowns, the sampled walk, the selection
//! landing, the packed tile
def private walk_arms(t : T?; m : Model; p : MtpParityRow;
                      count : array<int64>) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (fixture_arm(t, "mtp-count", p)) {
            count_cell(t, m, p.tag, count, 1l)
            count_cell(t, m, p.tag, count, 2l)
            count_cell(t, m, p.tag, count, 4l)
        }
        if (fixture_arm(t, "mtp-codec", p)) {
            for (kdt in [KVDtype.q8_0, KVDtype.tq4, KVDtype.f32]) {
                let drafted = count_cell(t, m, "{p.tag} {kdt}", count, 2l, false, kdt)
                t |> success(drafted > 0l && g_last_accepted > 0l, ("{p.tag} {kdt}: the verify " +
                    "accepted drafts on the codec's mirror ({g_last_accepted} of {drafted})"))
            }
        }
        if (fixture_arm(t, "mtp-count8", p)) {
            count_cell(t, m, p.tag, count, 8l)
        }
        if (fixture_arm(t, "mtp-ffh", p)) {
            half_crown_cell(t, m, p.tag, count)
        }
        if (fixture_arm(t, "mtp-sampled", p)) {
            count_cell(t, m, p.tag, count, 1l, true)
            count_cell(t, m, p.tag, count, 2l, true)
        }
        if (fixture_arm(t, "mtp-select", p)) {
            select_cell(t, m, p.tag, count)
        }
        if (fixture_arm(t, "mtp-tile", p)) {
            tile_pack_cell(t, m, p.tag, count)
        }
    }
}

//! whether the served image `m` is the carrier the row names: the
//! cut's claimed blocks, a NextN head, the metal-blob flavor
def private parity_carrier_ok(t : T?; m : Model; p : MtpParityRow) : bool {
    if (p.served_only) {
        if (!m.metal_blob || m.config.n_layer_nextn <= 0l) {
            t |> failure(("{p.file}: the served image is not a metal-blob NextN carrier - the " +
                "Metal mode's load declined a carrier the box holds"))
            return false
        }
        return true
    }
    if (p.blocks > 0l) {
        cut_within(t, "{p.file}: ", m, p.blocks)
    }
    if (m.config.n_layer_nextn <= 0l) {
        t |> failure("{p.file}: no NextN draft head - wrong fixture")
        return false
    }
    if (!m.metal_blob) {
        t |> failure(("{p.file}: the served image is not the metal-blob flavor - the Metal " +
            "mode's load declined a carrier the box holds"))
        return false
    }
    return true
}

//! the row's arms on the served image `m`, the draft head's
//! speculation on for them and put back after
def private parity_arms(t : T?; m : Model;
                        p : MtpParityRow) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!parity_carrier_ok(t, m, p))
        if (!p.served_only && fixture_arm(t, "mtp-census", p)) {
            var inscope any_sites : array<string>
            census_cell(t, m, path_join(models_dir(), p.file), "{p.tag} served twin", any_sites)
        }
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)
        var inscope prose_a <- encode(m, PROSE_LIGHTHOUSE, true, false)
        var inscope prose_b <- encode(m, PROSE_RIVER, true, false)
        var inscope count <- encode(m, counting_prompt(1, 40), true, false)
        if (p.served_only && arm_on(t, "mtp-dff-{p.tag}")) {
            dff_cell(t, m, p.tag, prose_a, prose_b, p.tol)
        }
        ff_arms(t, m, p, prose_a, prose_b)
        walk_arms(t, m, p, count)
        set_mtp_spec(spec0)
    }
}

//! the MTP rail's arms for the row `p` on the served image `m`
//! (`metal_form(image = true)`, the row's head pinned), cut to
//! `METAL_MTP_CELLS_CTX`; `half_crown_cell` shuts the drivers down
[cold_path]
def mtp_parity_metal(t : T?; var m : Model; form : ModelForm;
                     p : MtpParityRow) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.served_image, p.file, "MTP")
                   || (!empty(p.family) && !family_on(t, p.family)))
        with_ctx_cut(m, METAL_MTP_CELLS_CTX) {
            parity_arms(t, m, p)
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! the arms the 35B cuts leave on their whole files: the sampled
//! selection, the codec rounds, and on the IQ4_XS cut the forced
//! feeds
let private HELD_35B = "mtp-select,mtp-codec"
let private HELD_35B_IQ4XS = "mtp-select,mtp-codec,mtp-ff,mtp-ffk"

def mtp_parity_qwen35_0_8b() : MtpParityRow => MtpParityRow(
    file = "Qwen3.5-0.8B-MTP-Q8_0.gguf", tag = "0.8b")

def mtp_parity_qwen35_4b() : MtpParityRow => MtpParityRow(
    file = "Qwen3.5-4B-MTP-Q8_0.gguf", tag = "4b", family = "qwen35")

def mtp_parity_qwen35_9b() : MtpParityRow => MtpParityRow(
    file = "Qwen3.5-9B-MTP-UD-Q5_K_XL.gguf", tag = "9b", family = "qwen35")

def mtp_parity_qwen36_27b() : MtpParityRow => MtpParityRow(
    file = "Qwen3.6-27B-MTP-Q4_K_M.gguf", tag = "27b")

def mtp_parity_qwen36_35b_cut() : MtpParityRow => MtpParityRow(
    file = "Qwen3.6-35B-A3B-4L-MTP-UD-Q4_K_M-local.gguf", tag = "35b", blocks = 4l, held = HELD_35B)

def mtp_parity_qwen36_35b_whole() : MtpParityRow => MtpParityRow(
    file = "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", tag = "35b", held = HELD_35B, on_whole = true)

//! the IQ4_XS carrier's MoE bar: its plain step and the verify's rows
//! sum the routed experts in two fused orders
def mtp_parity_qwen36_35b_iq4xs_whole() : MtpParityRow => MtpParityRow(
    file = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf", tag = "35b-iq4xs", tol = MOE_FF_TOL,
    held = HELD_35B_IQ4XS, on_whole = true)

def mtp_parity_qwen36_35b_iq4xs_cut() : MtpParityRow => MtpParityRow(
    file = "Qwen3.6-35B-A3B-4L-MTP-IQ4_XS-pure-local.gguf", tag = "35b-iq4xs", tol = MOE_FF_TOL,
    blocks = 4l, held = HELD_35B_IQ4XS)

//! the Q4_K_M trunk with its Q8_0 NextN head as a split shard: a
//! mixed-quant pair the head adjacency cannot name, so the form
//! pins it
def mtp_parity_qwen38_27b_split_head() : MtpParityRow => MtpParityRow(
    file = "Qwen3.8-27B-Q4_K_M.gguf", tag = "3.8-27b", head = "mtp-Qwen3.8-27B-Q8_0.gguf")

//! the hyper-connection carrier with its split `shared` head: the
//! wide-residual carry, the n-gram panel and the sigmoid deltanet
//! gate
def mtp_parity_qwen38_flash_next() : MtpParityRow => MtpParityRow(
    file = "Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/Qwen3.8-Flash-Next-UD-IQ4_XS-00001-of-00003.gguf",
    tag = "3.8fn", tol = MOE_FF_TOL,
    head = "Qwen3.8-Flash-Next-GGUF/MTP/mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf",
    served_only = true)

//! the parameters of one model file's `mtp_verify_metal` call
struct MtpVerifyRow {
    file : string    //!< the model file under the models folder
    tag : string     //!< the arms' suffix: `mtp-<stem>-<tag>`
    tol : float      //!< the dff / vff bar
    expect_decline : string   //!< the reason the batch rail declines the verify for: "" served
    count_agree : int64 = COUNT_STEPS   //!< the drafter walk's leading tokens shared past depth 1
    blocks : int64   //!< past 0: the trunk blocks a cut's name claims
    family : string  //!< the family the cells run under: "" untagged
}

//! the arms a verify row's form serves on `tag`: a block pushes the
//! served image form only where the arm filter reaches one
def mtp_verify_arms(tag : string) : array<string> {
    let arms = fixed_array("dff", "dff8", "vff", "vff1", "vff5", "vff9", "vffc", "count")
    return <- [for (arm in arms); "mtp-{arm}-{tag}"]
}

//! one row through the batch driver, plain and under the unfused
//! add+rms pin
def private vff1_cells(t : T?; m : Model; tag : string; prompt : array<int64>; tol : float;
                       expect_decline : string) {
    vff_cell(t, m, tag, prompt, tol, 1l, expect_decline)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let unfused0 = get_metal_batch_addrms_unfused()
        set_metal_batch_addrms_unfused(true)
        vff_cell(t, m, "{tag} unfused add+rms", prompt, tol, 1l, expect_decline)
        set_metal_batch_addrms_unfused(unfused0)
    }
}

//! the nine-row verify under a 64 MB idle cap, the drivers shut down
//! first so the arena comes back sized by the rows' own demand
def private vffc_cell(t : T?; m : Model; tag : string; prompt : array<int64>;
                      tol : float) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_decode_shutdown()
        metal_prefill_shutdown()
        let prev_cap = get_metal_decode_mirror_cap_mb()
        set_metal_decode_mirror_cap_mb(64)
        vff_cell(t, m, "{tag} ceiling 64MB", prompt, m.kquant_native ? max(tol, MM_TOL) : tol, 9l)
        set_metal_decode_mirror_cap_mb(prev_cap)
    }
}

//! the same-slab verify arms by row count
def private vff_arms(t : T?; m : Model; p : MtpVerifyRow; prompt : array<int64>) {
    if (arm_on(t, "mtp-vff-{p.tag}")) {
        vff_cell(t, m, p.tag, prompt, p.tol, 4l, p.expect_decline)
    }
    if (arm_on(t, "mtp-vff1-{p.tag}")) {
        vff1_cells(t, m, p.tag, prompt, p.tol, p.expect_decline)
    }
    if (arm_on(t, "mtp-vff5-{p.tag}")) {
        vff_cell(t, m, p.tag, prompt, p.tol, 5l, p.expect_decline)
    }
    if (arm_on(t, "mtp-vff9-{p.tag}")) {
        vff9_cell(t, m, p.tag, prompt, p.tol, p.expect_decline)
    }
    if (arm_on(t, "mtp-vffc-{p.tag}") && empty(p.expect_decline)) {
        vffc_cell(t, m, p.tag, prompt, p.tol)
    }
}

//! the assistant drafter's counting walk at depth 1, 2 and 4 (and the
//! sampled one below 4), attached live and detached after
def private drafter_count_arm(t : T?; var m : Model;
                              p : MtpVerifyRow) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let side = gemma_drafter_sidecar(path_join(models_dir(), p.file))
        if (empty(side)) {
            t |> skip("{p.tag}: no assistant drafter sidecar beside {p.file}")
            return
        }
        if (attach_gemma_drafter(m, side) != "") {
            t |> failure("{p.tag}: the drafter {side} beside the carrier did not attach")
            return
        }
        var count <- encode(m, counting_prompt(1, 40), true, false)
        let prior_trace = gemma_trace()
        let prior_cpu = gemma_draft_on_cpu()
        let prior_pre = gemma_carry_pre_norm()
        set_gemma_trace(arm_on(t, "mtp-count-trace"))
        set_gemma_draft_on_cpu(arm_on(t, "mtp-count-cpu"))
        set_gemma_carry_pre_norm(arm_on(t, "mtp-count-pre"))
        for (depth in [1l, 2l, 4l]) {
            //! one draft a round verifies on the single-row form
            let agree = depth == 1l ? COUNT_STEPS : p.count_agree
            count_cell(t, m, p.tag, count, depth, false, KVDtype.f16, agree)
            let st = mtp_drafter_stats(m)
            to_log(LOG_INFO, ("gemma drafter {p.tag} k{depth}: rounds={st.rounds} " +
                "drafted={st.drafted} accepted={st.accepted}\n"))
            if (depth < 4l && arm_on(t, "mtp-sampled-{p.tag}")) {
                count_cell(t, m, p.tag, count, depth, true, KVDtype.f16, agree)
            }
        }
        set_gemma_trace(prior_trace)
        set_gemma_draft_on_cpu(prior_cpu)
        set_gemma_carry_pre_norm(prior_pre)
        detach_gemma_drafter()
        delete count
    }
}

//! the verify row's arms on the served image `m`
def private verify_arms_on(t : T?; var m : Model; p : MtpVerifyRow) {
    if (p.blocks > 0l) {
        cut_within(t, "{p.file}: ", m, p.blocks)
    }
    if (!m.metal_blob) {
        t |> failure(("{p.file}: the served image is not the metal-blob flavor - the Metal " +
            "mode's load declined a carrier the box holds"))
        return
    }
    var inscope prose_a <- encode(m, PROSE_LIGHTHOUSE, true, false)
    var inscope prose_b <- encode(m, PROSE_RIVER, true, false)
    if (arm_on(t, "mtp-dff-{p.tag}")) {
        dff_cell(t, m, p.tag, prose_a, prose_b, p.tol)
    }
    if (arm_on(t, "mtp-dff8-{p.tag}")) {
        dff_cell(t, m, p.tag, prose_a, prose_b, p.tol, true)
    }
    vff_arms(t, m, p, prose_a)
    if (arm_on(t, "mtp-count-{p.tag}")) {
        drafter_count_arm(t, m, p)
    }
}

//! the batch rail's same-slab verify arms for the row `p` on the
//! served image `m` (`metal_form(image = true)`), cut to
//! `METAL_MTP_CELLS_CTX`; `vffc_cell` shuts the drivers down
[cold_path]
def mtp_verify_metal(t : T?; var m : Model; form : ModelForm;
                     p : MtpVerifyRow) {   // nolint:LINT012,LINT019 - das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (!metal_form_held(t, form, MetalWant.served_image, p.file, "MTP")
                   || (!empty(p.family) && !family_on(t, p.family)))
        var inscope arms <- mtp_verify_arms(p.tag)
        return if (!arms_on(t, arms))
        with_ctx_cut(m, METAL_MTP_CELLS_CTX) {
            verify_arms_on(t, m, p)
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}

//! the routed experts amplify any float-class difference at a
//! near-tie: the support matrix's gemma4moe Q4_K_M forced-step bar
def mtp_verify_gemma4_26b() : MtpVerifyRow => MtpVerifyRow(
    file = "gemma-4-26B-A4B-it-Q4_K_M.gguf", tag = "g26", tol = 24.0, count_agree = G26_COUNT_AGREE)

//! the 12B cut's bar: its rows run to a 1000 peak past the lifted
//! cap - the same-slab verify reads 3.54 on nine rows, its GEMV stage
//! knocked out 15.9 and past
let private G12_CUT_TOL = 6.0
//! the 26B cut's bar: the distinct sessions read 0.49 off the single
//! step and the same-slab verify 0.47
let private G26_CUT_TOL = 2.0

def mtp_verify_gemma4_26b_cut() : MtpVerifyRow => MtpVerifyRow(
    file = "gemma-4-26B-A4B-6L-Q4_K_M-local.gguf", tag = "g26", tol = G26_CUT_TOL, blocks = 6l,
    family = "gemma4moe")

//! the control carrier: a red elsewhere is the carrier, not the
//! same-slab plumbing
def mtp_verify_llama_1b() : MtpVerifyRow => MtpVerifyRow(
    file = "Llama-3.2-1B-Instruct-Q8_0.gguf", tag = "l1b", tol = 0.5)

def mtp_verify_gemma4_12b_cut() : MtpVerifyRow => MtpVerifyRow(
    file = "gemma-4-12B-12L-Q4_K_M-local.gguf", tag = "g12", tol = G12_CUT_TOL, blocks = 12l)

//! a same-slab verify's recurrent rows are one group walking the
//! session's mirror (`ARCHITECTURE_GPU_MTP.md#dn-group-stages`)
def mtp_verify_qwen38_27b() : MtpVerifyRow => MtpVerifyRow(
    file = "Qwen3.8-27B-Q4_K_M.gguf", tag = "q38", tol = 2.0)

//! the plain-MoE control the gemma-MoE cell is read against
def mtp_verify_qwen3_30b_a3b_cut() : MtpVerifyRow => MtpVerifyRow(
    file = "Qwen3-30B-A3B-7L-Q4_K_M-local.gguf", tag = "q30", tol = 2.0, blocks = 7l)
