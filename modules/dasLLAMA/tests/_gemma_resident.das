// The gemma cells of the whole-model resident driver, shared by the per-model test files
// (test_gpu_resident_gemma*.das): ONE model a file, so a process loads one carrier and no test
// inherits another model's device state. The GPU chains and the CPU chain are not bit-identical
// (GPU exp / f32 accumulation order, the prefill's f16 activation feed against the CPU's Q8
// activation blocks), so the gates are: the forced-feed logits-tolerance form (the same prose
// tokens fed to both arms, the logits within a bar of the CPU chain after the prefill and at every
// step, the one-step-off control past it), the agreement form (the window chain and the token
// command against each other on the same tokens, the CPU chain's own two paths the yardstick),
// and the perplexity form (teacher-forced positions after a prefill, the resident's perplexity
// within a ratio of the CPU chain's) for the models whose logits sit flat over whole stretches.

options gen2
options _dasllama_internal = true

module _gemma_resident shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common   // Model, Session, forward_prefill, forward
require dasllama/dasllama   // create_session, moe_gpu_drop_model; its umbrella fires each arch [init] registration (gemma3)

//! the CPU pre-step's side row of `tok` on a scratch session: the scaled embedding row, then `ple_pre_decode` (the device
//! pre-step cells' reference on either discrete home)
def cpu_side_row(tr : Model; tok : int64) : array<float> {
    var s <- make_run_state(tr.config)
    embed_text_rows(tr, [tok], s.x, 0l)
    ple_pre_decode(tr, s, tok)
    let n = tr.config.n_layers * tr.config.n_embd_per_layer
    var row <- [for (i in range64(n)); s.ple_inp[i]]
    delete s
    return <- row
}

//! the device's side row `dev` of token `tok` against the CPU pre-step's row at `rel` of its largest magnitude (the token command's
//! row lands within 1e-4 - the projection without the 1/sqrt(dim) reads the eps alone apart; the window's rows feed the projection
//! as f16 and land within 1e-3), another token's CPU row the control
def side_row_within(t : T?; dev : array<float>; tr : Model; tok : int64; what : string; rel : float) {
    var inscope cpu <- cpu_side_row(tr, tok)
    var inscope other <- cpu_side_row(tr, tok + 1l)
    let mx = logits_maxdiff(dev, cpu)   // a NaN on either side reads as infinite
    var big = 0.0   // the bar's scale: the CPU row's largest finite magnitude
    for (v in cpu) {
        big = max(big, abs(v))
    }
    let bar = rel * big
    to_log(LOG_INFO, "{moe_gpu_backend_name()} e2b pre-step ({what}): device side row against the CPU's: maxdiff {mx} (bar {bar}, largest {big})\n")
    t |> success(mx <= bar, "{what}: the device side row within {rel} of the CPU's largest ({mx} of {bar})")
    check_exceeds(t, "{what}: another token's CPU row against the device row", dev, other, Bar(kind = BarKind.rel_of_max, rel = rel))
}

//! the rig's context: the resident mirror's cap and every session's KV cache - a model max of 131072 would take an
//! 8 GB mirror on a 4B and a 30 GB CPU cache on the 26B for prompts of 600
let GEMMA_RIG_CTX = 32768l

//! one gemma fixture: its file under models_dir() and the arm token that selects its cells
struct GemmaFile {
    file : string
    arm : string
    host_experts : bool   //! the load asks the rig's context whole and at least one routed layer's experts onto the host: the driver arms with the first routed layers' experts on the host on any card
}

[init]
def private gemma_rig_init() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
    set_gpu_ctx_max(GEMMA_RIG_CTX)
}
require math
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require _model_tier public   // models_dir() + model_available() presence/size-tier gate, arm_on, live_maxabs, SUPPRESSED_LOGIT
require _resident_feed public   // resident_ready, load_resident, resident_then_cpu_feed, feed_steps_within, feed_logits
require _format_census   // census_cell: the routed files' every site in its own format
require _compares   // INFINITE_DIFF: the agreement form's control read finite
require _child_run   // run_child / child_cmd - the load-log child

//! the logits bar, a fraction of the step's max live |logit|. gemma-3-1b's logits move with the activation rounding itself: the CPU
//! chain's OWN prefill kernel and decode kernel land 0.06-0.10 of the max logit apart on the same
//! tokens (0.87 of 15.3 at eight tokens, 0.45 of 13.4 at forty), and the resident arms sit a further
//! factor off it (the prefill feeds the GEMMs f16 rows where the CPU quantizes Q8 blocks); the
//! one-step-off controls read 5 to 19, so the bar keeps its teeth
let GEMMA_LOGIT_BAR_REL = 0.16
//! the resident window chain and token command may sit this many times further apart than the CPU chain's own two paths on the same tokens (measured 1.6 to 4.8 on gemma-3-1b, by the prefill's activation feed)
let GEMMA_PATH_GAP_RATIO = 6.0
//! the resident driver's teacher-forced perplexity may exceed the CPU chain's by this ratio (12B Q8_0 measured 1.449 vs 1.516 over 150 positions, the reference exe 1.390)
let GEMMA_PPL_RATIO = 1.05
//! the routed 26B's ratio: a router near-tie flips whole positions, so either arm can land the farther one, and a kernel
//! recompile alone moves a 150-position cell - the Q4_K_M file read 1.29 to 1.84 of the CPU chain across the gpt-oss
//! arc's value-identical kernel rewrites, the IQ3_XXS file 0.68 to 1.71 - so the bar sits past that band (the readings: CLAUDE.md)
let GEMMA_MOE_PPL_RATIO = 2.0

//! the fixture loaded on the armed driver, or an empty model (`n_layers` 0) after the skip that says why
def private gemma_model(t : T?; gf : GemmaFile) : Model {
    let path = path_join(models_dir(), gf.file)
    var tr <- resident_trunk(t, path, GEMMA_RIG_CTX, " - DASLLAMA_GPU_MIN_CTX=1024 arms the 12B Q8_0 file on a 16 GB card") $() : Model {
        var m <- load_resident(path, 0l, gf.host_experts ? GEMMA_RIG_CTX : 0l, gf.host_experts ? 1l : 0l)
        if (gf.host_experts) {   //! the pinned cap and the one routed layer asked onto the host decide the plan whatever the card (a card past the rig's holds the stacks beside the cap): a memory decline is the form missing, never a skip
            t |> success(moe_gpu_resident_active(), "the whole-model driver armed with host experts ({gpu_resident_decline_()})")
        }
        return <- m
    }
    return <- tr if (tr.config.n_layers == 0l)
    if (gf.host_experts) {
        let host = gpu_resident_host_layers()
        t |> success(host > 0l && host < tr.config.n_layers, "the first {host} of {tr.config.n_layers} routed layers' experts serve on the host, the rest on the device")
    }
    t |> success(tr.config.pre_post_norm && tr.config.sliding_window > 0l && tr.config.ffn_act == FfnAct.gelu,
        "the fixture carries the sandwich norms, a sliding window and the GeGLU FFN (the arms this file gates)")
    return <- tr
}

//! the gemma cell's rig: the fixture on the armed driver (or the cell skipped, saying why - its arm filtered among the reasons),
//! `body` inside the engine's job queue, the model deleted after
def with_gemma_rig(t : T?; gf : GemmaFile; body : block<(tr : Model) : void>) {
    return if (!arm_on(t, gf.arm))
    var tr <- gemma_model(t, gf)
    if (tr.config.n_layers > 0l) {
        with_dasllama_jobque_() {
            invoke(body, tr)
        }
    }
    delete tr
}

//! the routed model's weight-format census on the armed driver's load: every site the file carries in its own format, the
//! expert stacks the native-MXFP4 planes exactly where `mx4` names them, then a 40-token prompt and two fed steps on the driver
//! with every call served, the steps' mx4 stamps counted where `mx4` and none dispatched where not
[cold_path]
def gemma_format_cell(t : T?; gf : GemmaFile; mx4 : bool) {
    with_gemma_rig(t, gf) $(tr) {
        census_cell(t, tr, path_join(models_dir(), gf.file), gf.file, ["attention q/k/v/o", "routed experts", "token table", "classifier"])
        t |> equal(tr.experts_mx4, mx4, "{gf.file}: the expert stacks load as the native-MXFP4 planes exactly where the file carries them")
        var inscope feed <- prose_feed(tr, 40l, 2l)
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = gpu_resident_prefills_()
        let gu0 = census("mx4_gemv_gu_cls_spv")
        let sum0 = census("mx4_gemv_sum_cls_spv")
        var rows <- feed_logits(tr, sg, feed.prompt, feed.toks) $ {}
        t |> equal(gpu_resident_prefills_(), served0 + 1l, "{gf.file}: the resident window chain served the prompt")
        served_every_call(t)
        if (gu0 != NO_CENSUS) {   //! the fed steps' routed sites on the mx4 stamps: the fused gate + up and the summed down
            let gu_n = census("mx4_gemv_gu_cls_spv") - gu0
            let sum_n = census("mx4_gemv_sum_cls_spv") - sum0
            if (mx4) {
                t |> success(gu_n > 0l && sum_n > 0l, "{gf.file}: the steps' routed gate + up ({gu_n}) and summed down ({sum_n}) on the mx4 stamps")
            } else {   //! the control: a file with no MXFP4 stack dispatches neither stamp
                t |> equal(gu_n + sum_n, 0l, "{gf.file}: no mx4 stack, no mx4 stamp dispatched (gate + up {gu_n}, summed down {sum_n})")
            }
        }
        for (r, i in rows, count()) {
            let bad = length([for (v in r); v; where !is_finite(v)])
            t |> equal(bad, 0, "{gf.file}: row {i}'s logits finite")
        }
        delete rows
        delete sg
    }
}

//! prefill `n` tokens + 8 fed steps on the resident driver with every call served, then the same on the all-CPU chain; the
//! prefill logits and every step's within the bar, the one-step-off control past it. `exact` holds the rows against the file's f32
//! chain (`f32_chain_rows`) where the CPU q8 chain is no reference: on the E2B Q4_K_M file the Q8_K activations make the feed
//! chaotic, so step 4's top logit reads 17.85 (arm64), 16.70 (x64) and 18.50 (x64 on the reference kernel bodies) where the f32
//! chain reads 19.14, and the reference build's quantized CPU path reads 17.8 to 20.1 by its attention form and batching, each
//! chain summing as far from the f32 rows - and adds that the device sits no further from that chain than the CPU q8 chain
//! (`f32_chain_witness`, within `f32_ratio` of the CPU q8 chain's distance, `f32_route` naming the route that ratio is for)
[cold_path, arch(at="../ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed")]
def gemma_forced_feed(t : T?; n : int64; gf : GemmaFile; exact : bool = false; f32_ratio : float = F32_DIST_REL; f32_route : string = "") {
    with_gemma_rig(t, gf) $(tr) {
        var inscope feed <- prose_feed(tr, n, 8l)
        var fed <- resident_then_cpu_feed(t, tr, feed.prompt, feed.toks) $ {
            served_every_call(t)
        }
        if (exact) {
            var f32_rows <- f32_chain_rows(path_join(models_dir(), gf.file), feed.prompt, feed.toks)
            feed_steps_within(t, tr, "", n, fed.gpu, f32_rows, feed.toks, GEMMA_LOGIT_BAR_REL, "resident", "the f32 chain")
            f32_chain_witness(t, tr, "{gf.file}: {n} tokens against the f32 chain{f32_route}", fed, f32_rows, f32_ratio)
            delete f32_rows
        } else {
            feed_steps_within(t, tr, "", n, fed.gpu, fed.cpu, feed.toks, GEMMA_LOGIT_BAR_REL)
        }
        delete fed
    }
}

//! `ring_vs_flat` on the gemma fixture `gf` under the rig's context
[cold_path]
def ring_vs_flat(t : T?; n : int64; gf : GemmaFile; kv : KVDtype) {
    return if (!arm_on(t, gf.arm))
    ring_vs_flat(t, n, path_join(models_dir(), gf.file), kv, GEMMA_RIG_CTX)
}

let RESIDENT_RING_ON = "dasLLAMA: resident driver - the sliding layers' K/V rows ride rings of their window plus"
let RESIDENT_RING_OFF = "dasLLAMA: resident driver - the sliding layers' K/V rows keep every position's row, no ring, as asked"
let RESIDENT_EMBED_OFF = "dasLLAMA: resident driver - the host embeds every token row as asked"

//! the whole-model driver's load log for `gf` - what to_log prints, which no in-process cell reads - from one resident load in a
//! child (`_resident_load_root.das`) at the context `ctx` asked whole (0 none) under `envs`; "" with the skip `resident_ready` names
def resident_load_log(t : T?; gf : GemmaFile; ctx : int64; envs : array<tuple<name : string; value : string>>) : string {
    let path = path_join(models_dir(), gf.file)
    return "" if (!resident_ready(t, path))
    var out : string
    let rc = run_child(child_cmd("_resident_load_root.das", envs, "\"{path}\" {ctx}", true), out)
    t |> equal(rc, 0, "the load root ran:\n{out}")
    t |> success(find(out, "RESIDENT_LOAD active=true") >= 0, "the whole-model driver armed in the child:\n{out}")
    return out
}

//! The resident driver's load-time announces off its log under both pin states: the ringed load names its rings, the flat one
//! (`DASLLAMA_GPU_KV_RING=0`) its every-row mirror as asked; with the token gather off (`DASLLAMA_VK_GPU_EMBED=0`) the host embed
//! says so, a gathering load saying nothing
[cold_path]
def ring_load_announces(t : T?; gf : GemmaFile) {
    return if (!arm_on(t, gf.arm))
    let ringed = resident_load_log(t, gf, 0l, [(name = "DASLLAMA_GPU_KV_RING", value = "1"), (name = "DASLLAMA_VK_GPU_EMBED", value = "1"),
        (name = "DAS_LOG_LEVEL", value = "info")])
    return if (empty(ringed))
    let flat = resident_load_log(t, gf, 0l, [(name = "DASLLAMA_GPU_KV_RING", value = "0"), (name = "DASLLAMA_VK_GPU_EMBED", value = "0"),
        (name = "DAS_LOG_LEVEL", value = "info")])
    t |> success(find(ringed, RESIDENT_RING_ON) >= 0 && find(ringed, RESIDENT_RING_OFF) < 0, "the ringed load names its rings alone:\n{ringed}")
    t |> success(find(flat, RESIDENT_RING_OFF) >= 0 && find(flat, RESIDENT_RING_ON) < 0, "the flat load names its every-row mirror alone:\n{flat}")
    t |> success(find(flat, RESIDENT_EMBED_OFF) >= 0, "the host embed under the gather pin says so:\n{flat}")
    t |> success(find(ringed, RESIDENT_EMBED_OFF) < 0, "the gathering load says nothing of the host embed")
}

//! the same tokens through the window chain at once and through the token command one at a time,
//! on one arm: the two GPU paths' logits at every shared position against each other, then the CPU
//! arm's own pair as the yardstick of how far a prefill kernel and a decode kernel sit apart on
//! this model by rounding alone
[cold_path]
def prefill_vs_decode(t : T?; n : int64; gf : GemmaFile) {
    with_gemma_rig(t, gf) $(tr) {
        let RESIDENT = 0
        let CPU = 1
        var toks <- prose_tokens(tr, n + 4l)
        var window_prompt <- [for (i in range64(n)); toks[i]]
        var window_steps <- [for (i in range64(n, n + 4l)); toks[i]]
        var token_prompt <- [toks[0]]
        var token_steps <- [for (i in range64(1l, n + 4l)); toks[i]]
        var gap, gap_off, gap_first : float[2]
        for (arm in range(2)) {
            if (arm == CPU) {
                moe_gpu_drop_model()
            }
            let name = arm == RESIDENT ? "resident" : "cpu"
            let c0 = cuda_positions_served()
            var a <- feed_alone(tr, mirror_kv_dtype(), window_prompt, window_steps)
            var b <- feed_alone(tr, mirror_kv_dtype(), token_prompt, token_steps)
            if (arm == RESIDENT) {
                served_every_call(t)
                cuda_served_since(t, c0, 2l * (n + 4l), "the window chain and the token command over {n + 4l} tokens each")
            }
            var worst = 0.0
            var worst_off = 0.0
            var worst_first = 0.0
            for (k in range64(long_length(a))) {
                let j = n - 1l + k
                let scale = live_maxabs(a[k])
                let rel = logits_maxdiff(a[k], b[j]) / scale
                if (rel > worst) {
                    worst = rel
                }
                let off = logits_maxdiff(a[k], b[j - 1l]) / scale   // the token command one position behind: logged, since a one-position shift moves prose logits less than the bar on the 12B and 31B
                if (off > worst_off) {
                    worst_off = off
                }
                let first = logits_maxdiff(a[k], b[0]) / scale   // the control: the token command's row at position 0 - the same window read every position off
                if (first > worst_first) {
                    worst_first = first
                }
                let ia = argmax_of(a[k])
                let ib = argmax_of(b[j])
                to_log(LOG_INFO, "{name}: position {n + k - 1l}: prefilled-in-one vs decoded-one-by-one logits maxdiff {logits_maxdiff(a[k], b[j])} of max |logit| {scale}, argmax {ia} ({a[k][ia]}) vs {ib} ({b[j][ib]}); one position off {off}, the first position {first}\n")
            }
            gap[arm] = worst
            gap_off[arm] = worst_off
            gap_first[arm] = worst_first
            delete a
            delete b
        }
        let bar = GEMMA_PATH_GAP_RATIO * gap[CPU]
        to_log(LOG_INFO, "{n} tokens: the resident paths sit {gap[RESIDENT]} of the max logit apart, the CPU paths {gap[CPU]} - ratio {gap[RESIDENT] / gap[CPU]} (bar {bar}); one position off: resident {gap_off[RESIDENT]}, cpu {gap_off[CPU]}; the first position: resident {gap_first[RESIDENT]}, cpu {gap_first[CPU]}\n")
        t |> success(gap[RESIDENT] <= bar, "the resident window chain and token command agree within {GEMMA_PATH_GAP_RATIO}x of the CPU chain's own two paths (resident {gap[RESIDENT]}, cpu {gap[CPU]})")
        // the control on the resident route: the window's rows against the token command's first row - the same window read every position off - land outside the bar; a one-position shift cannot (the CPU chain's own paths sit 0.17 to 0.39 apart on the 12B and 31B, and 6x of that is wider than one position moves prose logits, 0.67 to 1.57), so it is logged beside the aligned gap alone
        t |> success(is_finite(gap_first[RESIDENT]) && gap_first[RESIDENT] < INFINITE_DIFF && gap_first[RESIDENT] > bar, "control: on the resident route the window's rows against the token command's first row land outside the bar ({gap_first[RESIDENT]} against {bar})")
        t |> success(gap_off[RESIDENT] > gap[RESIDENT] && gap_off[CPU] > gap[CPU], "one position behind, both arms read a wider gap than aligned (resident {gap_off[RESIDENT]} vs {gap[RESIDENT]}, cpu {gap_off[CPU]} vs {gap[CPU]})")
        delete toks
        delete window_prompt
        delete window_steps
        delete token_prompt
        delete token_steps
    }
}

//! teacher-forced negative log-likelihood over `steps` positions after `n` prefilled ids: the sum over the
//! true next ids, the sum over the ids one position off (the control), and the argmax hits on each
def private forced_nll(tr : Model; var s : Session; ids : array<int64>; n, steps : int64) : tuple<nll : float; off : float; hits : int64; off_hits : int64> {
    var prompt <- [for (i in range64(n)); ids[i]]
    forward_prefill(tr, s, prompt, n, 0l)
    delete prompt
    var nll = 0.0
    var off = 0.0
    var hits = 0l
    var off_hits = 0l
    for (p in range64(n, n + steps)) {
        nll -= target_logp(s.logits, ids[p])
        off -= target_logp(s.logits, ids[p + 1l])
        let am = argmax_of(s.logits)
        if (am == ids[p]) {
            hits++
        }
        if (am == ids[p + 1l]) {
            off_hits++
        }
        if (p + 1l < n + steps) {
            forward(tr, s, ids[p], p)
        }
    }
    return (nll = nll, off = off, hits = hits, off_hits = off_hits)
}

//! the same prose through the resident driver and the CPU chain, scored as a perplexity over `steps`
//! teacher-forced positions after an `n`-id prefill: the resident within the ratio of the CPU chain's,
//! its argmax hits within `hits_slack`, and the one-off control at least three times the CPU chain's perplexity.
//! On the 12B the logits sit within a few points at whole stretches of positions (the reference exe's own
//! margins read 0.6 to 16 on this text), so a maxdiff bar on a single position measures the model, not the driver;
//! a routed model's near-tie flips move whole positions, so its slack is wider.
[cold_path]
def ppl_compare(t : T?; n, steps : int64; gf : GemmaFile; hits_slack : int64 = 3l; ppl_ratio : float = GEMMA_PPL_RATIO) {
    with_gemma_rig(t, gf) $(tr) {
        var ids <- prose_tokens(tr, n + steps + 1l)
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = gpu_resident_prefills_()
        let c0 = cuda_positions_served()
        let g = forced_nll(tr, sg, ids, n, steps)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(gpu_resident_prefills_(), served0 + 1l, "the resident window chain served the {n}-token prompt")
        cuda_served_since(t, c0, n + steps - 1l, "the {n}-id prefill and {steps - 1l} teacher-forced steps")
        served_every_call(t)
        delete sg
        moe_gpu_drop_model()   // every later step is the CPU chain
        var sc <- create_session(tr, mirror_kv_dtype())
        let c = forced_nll(tr, sc, ids, n, steps)
        delete sc
        let fsteps = float(steps)
        let ppl_g = exp(g.nll / fsteps)
        let ppl_c = exp(c.nll / fsteps)
        let ppl_off = exp(c.off / fsteps)
        let ppl_g_off = exp(g.off / fsteps)   // the device route's own control: its logits scored against the ids one position off
        to_log(LOG_INFO, "{n} prefilled + {steps} teacher-forced [{tile_arm_tag()}]: PPL resident {ppl_g} (hits {g.hits}/{steps}, one-off {ppl_g_off}), cpu {ppl_c} (hits {c.hits}/{steps}, one-off hits {c.off_hits}), one-off control {ppl_off}\n")
        t |> success(ppl_g <= ppl_c * ppl_ratio, "the resident perplexity {ppl_g} sits within {ppl_ratio}x of the CPU chain's {ppl_c}")
        t |> success(g.hits + hits_slack >= c.hits, "the resident argmax hits ({g.hits}) sit within {hits_slack} of the CPU chain's ({c.hits}) over {steps} positions")
        t |> success(is_finite(ppl_g_off) && ppl_g_off > ppl_c * ppl_ratio, "control on the device route: the resident logits scored against the ids one position off read {ppl_g_off}, outside {ppl_ratio}x of the CPU chain's {ppl_c}")
        t |> success(ppl_off >= 3.0 * ppl_c, "control: the one-position-off targets read {ppl_off}, at least three times the CPU chain's {ppl_c}")
        t |> success(c.off_hits + hits_slack < c.hits, "control: the one-position-off targets hit {c.off_hits}, under the hits bar's reach ({c.hits} - {hits_slack})")
        delete ids
    }
}
