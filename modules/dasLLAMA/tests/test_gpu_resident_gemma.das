options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration (gemma3)
require dasllama/dasllama   // create_session, moe_gpu_drop_model

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // the control arm (the model dropped off the device) prefills on the CPU by design
}
require math
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_tier_installed / gpu_want_auto: is the tier armed
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active: did the whole-model driver take the model
require ?vulkan dasllama/dasllama_vulkan_common   // PF_WINDOW, the resolved coopmat mode, where the module exists
require _model_tier   // models_dir() + model_available() presence/size-tier gate

// The whole-model resident driver on the gemma dense base under DASLLAMA_GPU=1: sandwich norms
// (the residual steps norm their add partner first), sliding-window layers beside global ones with
// their own rope class, the GeGLU FFN, the sqrt(dim) embed scale. The GPU chains and the CPU chain
// are not bit-identical (GPU exp / f32 accumulation order, and the prefill's f16 activation feed
// against the CPU's Q8 activation blocks), so the gate is the forced-feed logits-tolerance form: the
// same fixed tokens fed to both arms, the logits within a bar of the CPU chain after the prefill and
// at every step, and the one-step-off control that must exceed it. The cells past the 512-position
// sliding window make every sliding layer mask keys below its window on the prefill tiles and the
// decode chunks alike. The agreement cells hold the two GPU paths - the window chain and the token
// command - to each other on the same tokens, with the CPU chain's own two paths as the yardstick.

//! of the step's max |logit|. gemma-3-1b's logits move with the activation rounding itself: the CPU
//! chain's OWN prefill kernel and decode kernel land 0.06-0.10 of the max logit apart on the same
//! tokens (0.87 of 15.3 at eight tokens, 0.45 of 13.4 at forty), and the resident arms sit a further
//! factor off it (the prefill feeds the GEMMs f16 rows where the CPU quantizes Q8 blocks); the
//! one-step-off controls read 5 to 19, so the bar keeps its teeth
let private GEMMA_LOGIT_BAR_REL = 0.16
//! the resident window chain and token command may sit this many times further apart than the CPU chain's own two paths on the same tokens (measured 1.6 to 4.8 on gemma-3-1b, by the prefill's activation feed)
let private GEMMA_PATH_GAP_RATIO = 6.0
//! the resident driver's teacher-forced perplexity may exceed the CPU chain's by this ratio (12B Q8_0 measured 1.449 vs 1.516 over 150 positions, llama.cpp 1.390)
let private GEMMA_PPL_RATIO = 1.05
let private PF_WINDOW_ROWS = 512l     //! the engine's PF_WINDOW, asserted equal by the two-window cells: a prompt past it runs as sequential windows
let private GEMMA3_FILE = "gemma-3-1b-it-Q8_0.gguf"
let private GEMMA3_4B_FILE = "gemma-3-4b-it-Q8_0.gguf"
let private GEMMA2_FILE = "gemma-2-2b-it-Q8_0.gguf"   //! the attention logit softcap (50) on every score, a 4096 window on the even layers, no qk-norm   //! the dense base at width 2560, heads 8 x 256 over 4 kv heads, a 1024 sliding window
//! the gemma-4 dense carriers (large tier): the weightless v-norm, the V-from-K global layers at head 512 beside the sliding 256s,
//! the per-layer output scale, the final softcap and the suppressed ids. The Q8_0 file takes the forced-feed cells; the Q4_K_M file
//! takes the agreement cell alone - on it the CPU chain's own prefill and decode kernels land 12% of the max logit apart, so a
//! CPU-referenced bar there measures the model's sensitivity to the K-quant activation rounding, not the driver
let private GEMMA4_FILE = "gemma-4-12B-it-Q4_K_M.gguf"
//! the gemma-4 E-series carrier: per-layer embeddings (ple 256 on 35 layers), twenty layers sharing the K/V of the fifteen below, two dense FFN widths, a 512 sliding window
let private GEMMA4E_FILE = "gemma-4-E2B-it-Q8_0.gguf"
let private GEMMA4E4_FILE = "gemma-4-E4B-it-Q8_0.gguf"   //! the larger E-series carrier (large tier): 42 layers, eighteen sharing K/V
let private GEMMA4_Q8_FILE = "gemma-4-12B-it-Q8_0.gguf"

def private window_pin(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        t |> equal(PF_WINDOW, PF_WINDOW_ROWS, "the prompt is sized past the engine's own window")
    }
}

//! the served tile arm, so a red names which prefill tile it came from
def private tile_arm_tag() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return "coopmat mode {g_gpu.coopmat_mode}"
    } else {
        return "no vulkan module"
    }
}

def private argmax_of(v : array<float>) : int64 {
    var best = -1e30
    var bi = 0l
    for (i in range64(long_length(v))) {
        if (v[i] > best) {
            best = v[i]
            bi = i
        }
    }
    return bi
}

//! the largest |logit| among the live ones - a suppressed id sits at -1e30 on both arms and would set the bar to nonsense
def private live_maxabs(v : array<float>) : float {
    var m = 0.0
    for (x in v) {
        let a = abs(x)
        if (a < 1.0e29 && a > m) {
            m = a
        }
    }
    return m
}

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

//! the fixture loaded on the armed driver, or an empty model (`n_layers` 0) after the skip that says why
def private gemma_model(t : T?; file : string) : Model {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
        return <- Model()
    }
    let path = path_join(models_dir(), file)
    if (!model_available(t, path)) return <- Model()
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return <- Model()
    }
    var tr <- load_model_(path, QuantMode.q8)
    if (!moe_gpu_resident_active() && moe_gpu_resident_memory_decline()) {
        // the plan did not fit the card at the session's context: a shorter context (DASLLAMA_GPU_MIN_CTX) or a smaller card's file, never a driver fault
        t |> skip("the residency plan does not fit this card ({moe_gpu_resident_decline()}) - DASLLAMA_GPU_MIN_CTX=1024 arms the 12B Q8_0 file on a 16 GB card")
        delete tr
        return <- Model()
    }
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the model (the load log names any decline)")
    t |> success(tr.config.pre_post_norm && tr.config.sliding_window > 0l && tr.config.ffn_act == FfnAct.gelu,
        "the fixture carries the sandwich norms, a sliding window and the GeGLU FFN (the arms this file gates)")
    return <- tr
}

// prefill `prompt`, then feed `toks` one per step; the logits after the prefill and after each step
def private feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : array<array<float>> {
    let n = long_length(prompt)
    var rows : array<array<float>>
    rows |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    rows |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        rows |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    }
    return <- rows
}

// prefill `n` tokens + 8 fed steps on the resident driver, then the same on the all-CPU chain; the
// prefill logits and every step's within the bar, the one-step-off control past it.
def private gemma_forced_feed(t : T?; n : int64; file : string) {
    var tr <- gemma_model(t, file)
    if (tr.config.n_layers == 0l) {
        delete tr
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var ids <- prose_tokens(tr, n + 8l)
        var prompt <- [for (i in range64(n)); ids[i]]
        var toks <- [for (i in range64(n, n + 8l)); ids[i]]
        delete ids
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = moe_gpu_resident_prefills()
        var gpu <- feed_logits(tr, sg, prompt, toks)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {n}-token prompt")
        delete sg
        moe_gpu_drop_model()   // every later step is the CPU chain
        var sc <- create_session(tr, mirror_kv_dtype())
        var cpu <- feed_logits(tr, sc, prompt, toks)
        delete sc
        for (st in range64(long_length(cpu))) {
            let diff = logits_maxdiff(gpu[st], cpu[st])
            let bar = GEMMA_LOGIT_BAR_REL * live_maxabs(cpu[st])
            let ag = argmax_of(gpu[st])
            let ac = argmax_of(cpu[st])
            let what = (st == 0l ? "prefill ({n} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
            to_log(LOG_INFO, "{what}: argmax cpu {ac} ({cpu[st][ac]}), resident {ag} ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
            t |> success(diff <= bar, "{what} logits: resident within {bar} of the CPU chain (maxdiff {diff})")
            if (st > 0l) {   // the control: the previous step's CPU logits are a disconnected mechanism the bar must RED
                let off = logits_maxdiff(gpu[st], cpu[st - 1l])
                t |> success(off > bar, "{what} control: the bar discriminates a one-step-off compare (maxdiff {off} vs bar {bar})")
            }
        }
        delete gpu
        delete cpu
        delete prompt
        delete toks
    }
    delete tr
}

// the same tokens through the window chain at once and through the token command one at a time,
// on one arm: the two GPU paths' logits at every shared position against each other, then the CPU
// arm's own pair as the yardstick of how far a prefill kernel and a decode kernel sit apart on
// this model by rounding alone
def private prefill_vs_decode(t : T?; n : int64; file : string) {
    var tr <- gemma_model(t, file)
    if (tr.config.n_layers == 0l) {
        delete tr
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var toks <- prose_tokens(tr, n + 4l)
        var head <- [for (i in range64(n)); toks[i]]
        var one <- [toks[0]]
        var rest <- [for (i in range64(1l, n + 4l)); toks[i]]
        var tail <- [for (i in range64(n, n + 4l)); toks[i]]
        var gap : float[2]
        for (arm in range(2)) {
            if (arm == 1) {
                moe_gpu_drop_model()
            }
            let name = arm == 0 ? "resident" : "cpu"
            var sa <- create_session(tr, mirror_kv_dtype())
            var a <- feed_logits(tr, sa, head, tail)   // rows: after n tokens, then n+1 .. n+4
            delete sa
            var sb <- create_session(tr, mirror_kv_dtype())
            var b <- feed_logits(tr, sb, one, rest)    // rows: after 1 token, then 2 .. n+4
            delete sb
            var worst = 0.0
            for (k in range64(long_length(a))) {
                let j = n - 1l + k
                let rel = logits_maxdiff(a[k], b[j]) / live_maxabs(a[k])
                if (rel > worst) {
                    worst = rel
                }
                to_log(LOG_INFO, "{name}: position {n + k - 1l}: prefilled-in-one vs decoded-one-by-one logits maxdiff {logits_maxdiff(a[k], b[j])} of max |logit| {live_maxabs(a[k])}, argmax {argmax_of(a[k])} vs {argmax_of(b[j])}\n")
            }
            gap[arm] = worst
            delete a
            delete b
        }
        to_log(LOG_INFO, "{n} tokens: the resident paths sit {gap[0]} of the max logit apart, the CPU paths {gap[1]} - ratio {gap[0] / gap[1]}\n")
        t |> success(gap[0] <= GEMMA_PATH_GAP_RATIO * gap[1], "the resident window chain and token command agree within {GEMMA_PATH_GAP_RATIO}x of the CPU chain's own two paths (resident {gap[0]}, cpu {gap[1]})")
        delete toks
        delete head
        delete one
        delete rest
        delete tail
    }
    delete tr
}

// log-softmax of `target` over the live logits (a pinned id at -1e30 drops out of the sum on its own)
def private target_logp(v : array<float>; target : int64) : float {
    var m = -1.0e30
    for (x in v) {
        if (x > m) {
            m = x
        }
    }
    var sum = 0.0
    for (x in v) {
        sum += exp(x - m)
    }
    return v[target] - m - log(sum)
}

//! teacher-forced negative log-likelihood over `steps` positions after `n` prefilled ids: the sum over the
//! true next ids, the sum over the ids one position off (the control), and the argmax hits
def private forced_nll(tr : Model; var s : Session; ids : array<int64>; n, steps : int64) : tuple<nll : float; off : float; hits : int64> {
    var prompt <- [for (i in range64(n)); ids[i]]
    forward_prefill(tr, s, prompt, n, 0l)
    delete prompt
    var nll = 0.0
    var off = 0.0
    var hits = 0l
    for (p in range64(n, n + steps)) {
        nll -= target_logp(s.logits, ids[p])
        off -= target_logp(s.logits, ids[p + 1l])
        if (argmax_of(s.logits) == ids[p]) {
            hits++
        }
        if (p + 1l < n + steps) {
            forward(tr, s, ids[p], p)
        }
    }
    return (nll = nll, off = off, hits = hits)
}

// the same prose through the resident driver and the CPU chain, scored as a perplexity over `steps`
// teacher-forced positions after an `n`-id prefill: the resident within the ratio of the CPU chain's,
// its argmax hits within three, and the one-off control at least three times the CPU chain's perplexity.
// On the 12B the logits sit within a few points at whole stretches of positions (llama.cpp's own margins
// read 0.6 to 16 on this text), so a maxdiff bar on a single position measures the model, not the driver.
def private ppl_compare(t : T?; n, steps : int64; file : string) {
    var tr <- gemma_model(t, file)
    if (tr.config.n_layers == 0l) {
        delete tr
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        var ids <- prose_tokens(tr, n + steps + 1l)
        var sg <- create_session(tr, mirror_kv_dtype())
        let served0 = moe_gpu_resident_prefills()
        let g = forced_nll(tr, sg, ids, n, steps)
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {n}-token prompt")
        delete sg
        moe_gpu_drop_model()   // every later step is the CPU chain
        var sc <- create_session(tr, mirror_kv_dtype())
        let c = forced_nll(tr, sc, ids, n, steps)
        delete sc
        let fsteps = float(steps)
        let ppl_g = exp(g.nll / fsteps)
        let ppl_c = exp(c.nll / fsteps)
        let ppl_off = exp(c.off / fsteps)
        to_log(LOG_INFO, "{n} prefilled + {steps} teacher-forced [{tile_arm_tag()}]: PPL resident {ppl_g} (hits {g.hits}/{steps}), cpu {ppl_c} (hits {c.hits}/{steps}), one-off control {ppl_off}\n")
        t |> success(ppl_g <= ppl_c * GEMMA_PPL_RATIO, "the resident perplexity {ppl_g} sits within {GEMMA_PPL_RATIO}x of the CPU chain's {ppl_c}")
        t |> success(g.hits + 3l >= c.hits, "the resident argmax hits ({g.hits}) sit within three of the CPU chain's ({c.hits}) over {steps} positions")
        t |> success(ppl_off >= 3.0 * ppl_c, "control: the one-position-off targets read {ppl_off}, at least three times the CPU chain's {ppl_c}")
        delete ids
    }
    delete tr
}

[test]
def test_gpu_resident_gemma3_one_window(t : T?) {
    t |> run("gemma-3-1b: resident prefill + decode with sandwich norms and sliding layers on device vs the CPU chain, one window") @(t : T?) {
        if (!arm_on(t, "g3")) return
        gemma_forced_feed(t, 40l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: an eight-token prompt - the shortest window, where this model's logits move most with the rounding") @(t : T?) {
        if (!arm_on(t, "g3")) return
        gemma_forced_feed(t, 8l, GEMMA3_FILE)
    }
}

[test]
def test_gpu_resident_gemma3_past_window(t : T?) {
    t |> run("gemma-3-1b: a 520-token prompt - two prefill windows, the sliding window engaged on the last eight positions") @(t : T?) {
        if (!arm_on(t, "g3")) return
        window_pin(t)
        gemma_forced_feed(t, PF_WINDOW_ROWS + 8l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: a 600-token prompt - every sliding layer masks below its window on the prefill tiles, then the decode chunks past it") @(t : T?) {
        if (!arm_on(t, "g3")) return
        window_pin(t)
        gemma_forced_feed(t, PF_WINDOW_ROWS + 88l, GEMMA3_FILE)
    }
}

[test]
def test_gpu_resident_gemma3_4b(t : T?) {
    t |> run("gemma-3-4b: resident prefill + decode vs the CPU chain, forty tokens - the dense base at its wider geometry") @(t : T?) {
        if (!arm_on(t, "g3b")) return
        gemma_forced_feed(t, 40l, GEMMA3_4B_FILE)
    }
    t |> run("gemma-3-4b: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g3b")) return
        ppl_compare(t, 150l, 150l, GEMMA3_4B_FILE)
    }
}

[test]
def test_gpu_resident_gemma2(t : T?) {
    t |> run("gemma-2-2b: resident prefill + decode vs the CPU chain, forty tokens - the attention logit softcap on the 8-row tile and the token command") @(t : T?) {
        if (!arm_on(t, "g2")) return
        gemma_forced_feed(t, 40l, GEMMA2_FILE)
    }
    t |> run("gemma-2-2b: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g2")) return
        ppl_compare(t, 150l, 150l, GEMMA2_FILE)
    }
    t |> run("gemma-2-2b: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g2")) return
        prefill_vs_decode(t, 8l, GEMMA2_FILE)
    }
}

[test]
def test_gpu_resident_gemma4_dense(t : T?) {
    t |> run("gemma-4-12B Q8_0: resident prefill + decode - v-norm, V-from-K 512-heads, the layer scale, the capped and pinned logits - vs the CPU chain, one window") @(t : T?) {
        if (!arm_on(t, "g4q8")) return
        gemma_forced_feed(t, 40l, GEMMA4_Q8_FILE)
    }
    t |> run("gemma-4-12B Q8_0: 150 prose ids prefilled, 150 teacher-forced - the wide heads over up to 300 keys on the token command, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4q8")) return
        ppl_compare(t, 150l, 150l, GEMMA4_Q8_FILE)
    }
    t |> run("gemma-4-12B Q8_0: 520 ids prefilled - two windows, the second eight rows deep - then 80 teacher-forced past them, the 1024 window not yet engaged") @(t : T?) {
        if (!arm_on(t, "g4q8")) return
        window_pin(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4_Q8_FILE)
    }
}

[test]
def test_gpu_resident_gemma4_eseries(t : T?) {
    t |> run("gemma-4-E2B Q8_0: resident prefill + decode - the per-layer-embedding branch, twenty shared-KV layers, two FFN widths - vs the CPU chain, forty tokens") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        gemma_forced_feed(t, 40l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        ppl_compare(t, 150l, 150l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: 520 ids prefilled - two windows, the 512 sliding window engaged on the last eight - then 80 teacher-forced") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        window_pin(t)
        ppl_compare(t, PF_WINDOW_ROWS + 8l, 80l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E2B Q8_0: eight tokens through the window chain against the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g4e")) return
        prefill_vs_decode(t, 8l, GEMMA4E_FILE)
    }
    t |> run("gemma-4-E4B Q8_0: resident prefill + decode vs the CPU chain, forty tokens - the E-series arms on the larger carrier") @(t : T?) {
        if (!arm_on(t, "g4f")) return
        gemma_forced_feed(t, 40l, GEMMA4E4_FILE)
    }
    t |> run("gemma-4-E4B Q8_0: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        if (!arm_on(t, "g4f")) return
        ppl_compare(t, 150l, 150l, GEMMA4E4_FILE)
    }
}

[test]
def test_gpu_resident_gemma3_prefill_vs_decode(t : T?) {
    t |> run("gemma-3-1b: eight tokens through the window chain against the same eight through the token command, both arms") @(t : T?) {
        if (!arm_on(t, "g3")) return
        prefill_vs_decode(t, 8l, GEMMA3_FILE)
    }
    t |> run("gemma-3-1b: the same at forty tokens") @(t : T?) {
        if (!arm_on(t, "g3")) return
        prefill_vs_decode(t, 40l, GEMMA3_FILE)
    }
    t |> run("gemma-4-12B: eight tokens through the window chain against the token command, both arms - the wide heads' prefill kernel against their decode kernel") @(t : T?) {
        if (!arm_on(t, "g4k")) return
        prefill_vs_decode(t, 8l, GEMMA4_FILE)
    }
    t |> run("gemma-4-12B: the same at 300 tokens - the wide heads over many keys on both paths") @(t : T?) {
        if (!arm_on(t, "g4x")) return
        prefill_vs_decode(t, 300l, GEMMA4_FILE)
    }
}
