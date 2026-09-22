// The forced-feed rig of the resident files: a prompt plus fed tokens through one session with every step's
// logits kept, the resident run against the CPU chain's on the same tokens, and the per-step bar with its
// one-step-off control - the llama, qwen2, gemma, MoE and shared-expert files run it on their own carriers.

options gen2
options _dasllama_internal = true

module _resident_feed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require math
require dasllama/dasllama_common   // Model, Session, forward_prefill, forward
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active / moe_gpu_resident_prefills: the driver's witnesses
require ?vulkan dasllama/dasllama_vulkan_common   // the resolved coopmat mode, where the module exists
require _model_tier   // logits_maxdiff, mirror_kv_dtype

//! the served tile arm, so a red names which prefill tile it came from
def tile_arm_tag() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return "coopmat mode {g_gpu.coopmat_mode}"
    } else {
        return "no vulkan module"
    }
}

def argmax_of(v : array<float>) : int64 {
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

let SUPPRESSED_LOGIT = -1.0e30   //! a pinned id's logit on both arms

//! the largest |logit| among the live ones: a suppressed id (`SUPPRESSED_LOGIT`) would set a relative bar to nonsense
def live_maxabs(v : array<float>) : float {
    var m = 0.0
    for (x in v) {
        let a = abs(x)
        if (a < -SUPPRESSED_LOGIT * 0.1 && a > m) {
            m = a
        }
    }
    return m
}

//! the prefill's logits, then each fed step's, through `s`
def feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : array<array<float>> {
    let n = long_length(prompt)
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    }
    return <- out
}

//! the two arms' logits rows: the resident driver's, then the CPU chain's
struct FeedPair {
    gpu : array<array<float>>
    cpu : array<array<float>>
}

//! `prompt` then `toks` through the resident driver armed on `tr` (the stayed-armed and served-prefill witnesses under `t`,
//! then `after_gpu`, the caller's own witnesses while the driver still holds the model), then the model dropped off the
//! device and the same tokens through the CPU chain
def resident_then_cpu_feed(t : T?; tr : Model; prompt, toks : array<int64>; after_gpu : block<void>) : FeedPair {
    var pair : FeedPair
    var sg <- create_session(tr, mirror_kv_dtype())
    let served0 = moe_gpu_resident_prefills()
    pair.gpu <- feed_logits(tr, sg, prompt, toks)
    t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
    t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {long_length(prompt)}-token prompt")
    invoke(after_gpu)
    delete sg
    moe_gpu_drop_model()   //! every later step is the CPU chain
    var sc <- create_session(tr, mirror_kv_dtype())
    pair.cpu <- feed_logits(tr, sc, prompt, toks)
    delete sc
    return <- pair
}

//! every row of `gpu` (the prefill's, then each fed step's) within `bar_rel` of the CPU chain's max live |logit| of the
//! same row, and each step's one-step-off compare (the previous step's CPU row, a disconnected mechanism) past the bar;
//! both argmaxes logged under `tag`, the two arms named `gpu_name` and `cpu_name` in every message
def feed_steps_within(t : T?; tag : string; nprompt : int64; gpu, cpu : array<array<float>>; toks : array<int64>; bar_rel : float; gpu_name : string = "resident"; cpu_name : string = "the CPU chain") {
    for (st in range64(long_length(cpu))) {
        let diff = logits_maxdiff(gpu[st], cpu[st])
        let bar = bar_rel * live_maxabs(cpu[st])
        let ag = argmax_of(gpu[st])
        let ac = argmax_of(cpu[st])
        let what = tag + (st == 0l ? "prefill ({nprompt} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
        to_log(LOG_INFO, "{what}: argmax cpu {ac} ({cpu[st][ac]}), {gpu_name} {ag} ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
        t |> success(diff <= bar, "{what} logits: {gpu_name} within {bar} of {cpu_name} (maxdiff {diff})")
        if (st > 0l) {
            let off = logits_maxdiff(gpu[st], cpu[st - 1l])
            to_log(LOG_INFO, "{what}: one-step-off control maxdiff {off} (bar {bar})\n")
            t |> success(off > bar, "{what} control: the bar discriminates a one-step-off compare (maxdiff {off} vs bar {bar})")
        }
    }
}
