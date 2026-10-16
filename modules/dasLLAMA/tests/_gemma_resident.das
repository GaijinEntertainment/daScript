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

//! the rig's context: the resident mirror's cap and every session's KV cache - a model max of 131072 would take an
//! 8 GB mirror on a 4B and a 30 GB CPU cache on the 26B for prompts of 600
let GEMMA_RIG_CTX = 32768l

//! one gemma fixture: its file under models_dir() and the arm token that selects its cells
struct GemmaFile {
    file : string
    arm : string
    host_experts : bool   //! the load asks the rig's context whole on a card that cannot hold every expert stack beside it: the driver arms with the first routed layers' experts on the host
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

//! the first `count` ids of ordinary prose through the model's own tokenizer: on a junk prompt gemma's
//! logits swing twenty points between the CPU chain's own prefill and decode, so a bar on it measures nothing
def private prose_tokens(tr : Model; count : int64) : array<int64> {
    let para = "The river bends twice before it reaches the old mill, and in spring the water runs high enough to turn the wheel without help. The miller keeps a ledger of every sack that comes through the door, the name of the farm it came from, and the weight the scale showed. On market days the road fills with carts before sunrise, and the children sell bread from baskets at the gate. "
    var one <- encode_(tr, para, true, false)
    let reps = count / long_length(one) + 2l   // a repeat may tokenize a little shorter than the sum of its parts
    delete one
    let text = build_string() $(var w : StringBuilderWriter) {
        for (_i in range64(reps)) {
            w |> write(para)
        }
    }
    let ids <- encode_(tr, text, true, false)
    assert(long_length(ids) >= count, "the prose repeats cover the count")
    return <- [for (i in range64(count)); ids[i]]
}

//! a prose feed: the first `n` prose ids the prompt, the next `steps` the fed tokens
struct private ProseFeed {
    prompt : array<int64>
    toks : array<int64>
}

def private prose_feed(tr : Model; n, steps : int64) : ProseFeed {
    var ids <- prose_tokens(tr, n + steps)
    var f <- ProseFeed(prompt <- [for (i in range64(n)); ids[i]], toks <- [for (i in range64(n, n + steps)); ids[i]])
    delete ids
    return <- f
}

//! the fixture loaded on the armed driver, or an empty model (`n_layers` 0) after the skip that says why
def private gemma_model(t : T?; gf : GemmaFile) : Model {
    let path = path_join(models_dir(), gf.file)
    var tr <- resident_trunk(t, path, GEMMA_RIG_CTX, " - DASLLAMA_GPU_MIN_CTX=1024 arms the 12B Q8_0 file on a 16 GB card") $() : Model {
        var m <- load_resident(path, 0l, gf.host_experts ? GEMMA_RIG_CTX : 0l)
        if (gf.host_experts) {   //! the pinned cap decides the plan whatever the card: a memory decline is the form missing, never a skip
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
        let served0 = moe_gpu_resident_prefills()
        let gu0 = census("mx4_gemv_gu_cls_spv")
        let sum0 = census("mx4_gemv_sum_cls_spv")
        var rows <- feed_logits(tr, sg, feed.prompt, feed.toks) $ {}
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "{gf.file}: the resident window chain served the prompt")
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
//! (`f32_chain_witness`)
[cold_path, arch(at="../ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed")]
def gemma_forced_feed(t : T?; n : int64; gf : GemmaFile; exact : bool = false) {
    with_gemma_rig(t, gf) $(tr) {
        var inscope feed <- prose_feed(tr, n, 8l)
        var fed <- resident_then_cpu_feed(t, tr, feed.prompt, feed.toks) $ {
            served_every_call(t)
        }
        if (exact) {
            var f32_rows <- f32_chain_rows(path_join(models_dir(), gf.file), feed.prompt, feed.toks)
            feed_steps_within(t, tr, "", n, fed.gpu, f32_rows, feed.toks, GEMMA_LOGIT_BAR_REL, "resident", "the f32 chain")
            f32_chain_witness(t, tr, "{gf.file}: {n} tokens against the f32 chain", fed, f32_rows)
            delete f32_rows
        } else {
            feed_steps_within(t, tr, "", n, fed.gpu, fed.cpu, feed.toks, GEMMA_LOGIT_BAR_REL)
        }
        delete fed
    }
}

//! one load's run in `ring_vs_flat`: the logits rows, whether the driver armed the rings, and the mirror's bytes on the card
struct private RingRun {
    rows : array<array<float>>
    slack : int64
    mirror : int64
    window : int64
}

//! `path` loaded at mirror codec `kv` under the ring pin `ring` (`with_gpu_kv_ring`), the `n` prose ids and eight fed steps through
//! it; `ok` false after the skip `resident_trunk` names
def private ring_run(t : T?; path : string; kv : KVDtype; ring : int; n : int64) : tuple<ok : bool; run : RingRun> {
    var tr <- resident_trunk(t, path, GEMMA_RIG_CTX, "") $() : Model {
        var m <- Model()
        with_gpu_kv_ring(ring) {
            delete m
            m <- load_resident_at(path, kv)
        }
        return <- m
    }
    var run : RingRun
    let ok = tr.config.n_layers > 0l
    if (ok) {
        var inscope feed <- prose_feed(tr, n, 8l)
        run.rows <- feed_alone(tr, kv, feed.prompt, feed.toks)
        served_every_call(t)
        run.slack = gpu_resident_kv_ring_slack()
        run.mirror = gpu_resident_mirror_bytes()
        run.window = tr.config.sliding_window
    }
    moe_gpu_drop_model()
    delete tr
    return <- (ok = ok, run <- run)
}

//! The sliding layers' rings against the flat mirror on one driver: `n` prose ids past the window and eight fed steps on a load that
//! rings them and on one that keeps every row (`set_gpu_kv_ring`), at mirror codec `kv` - the logits bit for bit at the prompt and every
//! step, since a ring moves where a row sits and never which rows a query reads or their order; the ringed load arms its rings and charges
//! the card less mirror than the flat one, which arms none, and the row a step off is the control the compare tells apart
[cold_path]
def ring_vs_flat(t : T?; n : int64; gf : GemmaFile; kv : KVDtype) {
    return if (!arm_on(t, gf.arm))
    let path = path_join(models_dir(), gf.file)
    return if (!resident_ready(t, path))
    with_dasllama_jobque_() {
        var inscope ringed <- ring_run(t, path, kv, 1, n)
        var inscope flat <- ring_run(t, path, kv, 0, n)
        if (ringed.ok && flat.ok) {
            t |> success(n > ringed.run.window && ringed.run.window > 0l, "the {n}-token prompt runs past the {ringed.run.window}-position window")
            t |> success(ringed.run.slack > 0l && flat.run.slack == 0l, "the ringed load armed its rings (slack {ringed.run.slack}), the flat load none ({flat.run.slack})")
            t |> success(ringed.run.mirror < flat.run.mirror, "the rings charge the card less mirror ({ringed.run.mirror >> 20l} MB) than every row ({flat.run.mirror >> 20l} MB)")
            for (st in range(length(ringed.run.rows))) {
                let what = st == 0 ? "the prompt's logits" : "step {st - 1}"
                check_exact(t, "{what}: the ringed mirror against the flat one, bit for bit", ringed.run.rows[st], flat.run.rows[st])
                t |> success(logits_maxdiff(ringed.run.rows[st], flat.run.rows[st > 0 ? st - 1 : 1]) > 0.0, "{what} control: the row a step off differs")
            }
        }
    }
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
        var gap, gap_off : float[2]
        for (arm in range(2)) {
            if (arm == CPU) {
                moe_gpu_drop_model()
            }
            let name = arm == RESIDENT ? "resident" : "cpu"
            var a <- feed_alone(tr, mirror_kv_dtype(), window_prompt, window_steps)
            var b <- feed_alone(tr, mirror_kv_dtype(), token_prompt, token_steps)
            if (arm == RESIDENT) {
                served_every_call(t)
            }
            var worst = 0.0
            var worst_off = 0.0
            for (k in range64(long_length(a))) {
                let j = n - 1l + k
                let scale = live_maxabs(a[k])
                let rel = logits_maxdiff(a[k], b[j]) / scale
                if (rel > worst) {
                    worst = rel
                }
                let off = logits_maxdiff(a[k], b[j - 1l]) / scale   //! the control: the token command one position behind
                if (off > worst_off) {
                    worst_off = off
                }
                let ia = argmax_of(a[k])
                let ib = argmax_of(b[j])
                to_log(LOG_INFO, "{name}: position {n + k - 1l}: prefilled-in-one vs decoded-one-by-one logits maxdiff {logits_maxdiff(a[k], b[j])} of max |logit| {scale}, argmax {ia} ({a[k][ia]}) vs {ib} ({b[j][ib]}); one position off {off}\n")
            }
            gap[arm] = worst
            gap_off[arm] = worst_off
            delete a
            delete b
        }
        to_log(LOG_INFO, "{n} tokens: the resident paths sit {gap[RESIDENT]} of the max logit apart, the CPU paths {gap[CPU]} - ratio {gap[RESIDENT] / gap[CPU]}; one position off: resident {gap_off[RESIDENT]}, cpu {gap_off[CPU]}\n")
        t |> success(gap[RESIDENT] <= GEMMA_PATH_GAP_RATIO * gap[CPU], "the resident window chain and token command agree within {GEMMA_PATH_GAP_RATIO}x of the CPU chain's own two paths (resident {gap[RESIDENT]}, cpu {gap[CPU]})")
        //! the control: the compare sees a one-position shift on both arms - the shifted gap sits above the aligned one. It is not held past the bar: on the 12B and 31B the CPU chain's own paths sit 0.17 to 0.39 of the max logit apart, so 6x of that is wider than a one-position shift moves the logits (0.67 to 1.57)
        t |> success(gap_off[RESIDENT] > gap[RESIDENT] && gap_off[CPU] > gap[CPU], "control: one position behind, both arms read a wider gap than aligned (resident {gap_off[RESIDENT]} vs {gap[RESIDENT]}, cpu {gap_off[CPU]} vs {gap[CPU]})")
        delete toks
        delete window_prompt
        delete window_steps
        delete token_prompt
        delete token_steps
    }
}

//! log-softmax of `target` over the live logits (a pinned id at -1e30 drops out of the sum on its own); a NaN anywhere in the
//! row reads NaN - `hmax` and `hlse` fold with `max`, which drops a NaN operand, so the row is scanned first
def private target_logp(v : array<float>; target : int64) : float {
    for (x in v) {
        return x if (is_nan(x))
    }
    let m = max(SUPPRESSED_LOGIT, hmax(unsafe(addr<float const?>(v[0])), long_length(v)))
    return v[target] - m - float(log(hlse(v, m)))
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
        let served0 = moe_gpu_resident_prefills()
        let g = forced_nll(tr, sg, ids, n, steps)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {n}-token prompt")
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
        to_log(LOG_INFO, "{n} prefilled + {steps} teacher-forced [{tile_arm_tag()}]: PPL resident {ppl_g} (hits {g.hits}/{steps}), cpu {ppl_c} (hits {c.hits}/{steps}, one-off hits {c.off_hits}), one-off control {ppl_off}\n")
        t |> success(ppl_g <= ppl_c * ppl_ratio, "the resident perplexity {ppl_g} sits within {ppl_ratio}x of the CPU chain's {ppl_c}")
        t |> success(g.hits + hits_slack >= c.hits, "the resident argmax hits ({g.hits}) sit within {hits_slack} of the CPU chain's ({c.hits}) over {steps} positions")
        t |> success(ppl_off >= 3.0 * ppl_c, "control: the one-position-off targets read {ppl_off}, at least three times the CPU chain's {ppl_c}")
        t |> success(c.off_hits + hits_slack < c.hits, "control: the one-position-off targets hit {c.off_hits}, under the hits bar's reach ({c.hits} - {hits_slack})")
        delete ids
    }
}
