// The forced-feed rig of the resident files: a prompt plus fed tokens through one session with every step's
// logits kept, the resident run against the CPU chain's on the same tokens, and the per-step bar with its
// one-step-off control - the llama, qwen2, gemma, MoE and shared-expert files run it on their own carriers.

options gen2
options _dasllama_internal = true

module _resident_feed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common   // Model, Session, forward_prefill, forward
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active / moe_gpu_resident_prefills: the driver's witnesses
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_: the calls the driver handed to the CPU rails, by reason
require ?vulkan dasllama/dasllama_vulkan_common   // the resolved coopmat mode, where the module exists
require _model_tier   // logits_maxdiff, mirror_kv_dtype, live_maxabs, SUPPRESSED_LOGIT

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

//! the served witness: no call of the armed driver passed to the CPU rails since the model armed
def served_every_call(t : T?) {
    var passes <- gpu_cpu_passes_()
    t |> equal(length(passes), 0, "the device served every call: {passes}")
    delete passes
}

//! how many calls the driver passed to the CPU rails for `reason` (the `RdecPass` name) since the model armed
def pass_count(reason : string) : int64 {
    var passes <- gpu_cpu_passes_()
    var n = 0l
    for (p in passes) {
        if (p.reason == reason) {
            n += p.count
        }
    }
    delete passes
    return n
}

//! the pick a device landed against the CPU logits of the same row, tie-aware: the pick is the CPU argmax, or its CPU logit sits
//! within `rel` of the CPU argmax's (a near-tie the two chains' rounding flips); both decoded and logged with their CPU logits
def pick_agrees(t : T?; tr : Model; what : string; pick : int64; cpu : array<float>; rel : float) : bool {
    let ac = argmax_of(cpu)
    let bar = rel * live_maxabs(cpu)
    let in_bar = pick >= 0l && pick < long_length(cpu) && cpu[pick] >= cpu[ac] - bar
    to_log(LOG_INFO, "{what}: device pick {pick} '{decode_(tr, [pick])}' (CPU logit {pick >= 0l && pick < long_length(cpu) ? cpu[pick] : 0.0}), CPU argmax {ac} '{decode_(tr, [ac])}' ({cpu[ac]}), tie bar {bar}\n")
    t |> success(pick == ac || in_bar, "{what}: the device pick is the CPU argmax, or its CPU logit is within {bar} of the CPU argmax's (a near-tie)")
    return pick == ac
}

//! a draft's two outputs: the logits and the head's own hidden `forward_mtp` leaves in the session
struct Draft {
    logits : array<float>
    h : array<float>
}

//! `forward_mtp` of `tok` at head row `row` from the carry `h0`, through whichever seat the process has installed
[cold_path]
def draft_from(tr : Model; var s : Session; tok, row : int64; h0 : array<float>) : Draft {
    copy_floats(h0, 0l, s.mtp_h, 0l, long_length(h0))
    forward_mtp(tr, s, tok, row)
    return <- Draft(logits <- [for (x in s.logits); x], h <- [for (x in s.mtp_h); x])
}

//! every row r of `gpu` within `rel` of the max |value| of `cpu[r]`, and the control row `cpu[r + ctrl_off]` - where one exists -
//! past `ctrl_bars` bars of the same row: the widest miss and the tightest control logged with their rows; `row_tag` names a row
def rows_within_control(t : T?; what : string; gpu, cpu : array<array<float>>; rel : float; ctrl_off : int64; ctrl_bars : float; row_tag : block<(r : int64) : string>) {
    var worst = 0.0
    var worst_row = -1l
    var worst_bar = 0.0
    var tight_off = 0.0
    var tight_bar = 0.0
    var tight_row = -1l
    let n = long_length(gpu)
    for (r in range64(n)) {
        let tag = "{what} {invoke(row_tag, r)}"
        let bar = rel * logits_maxabs(cpu[r])
        let diff = logits_maxdiff(gpu[r], cpu[r])
        if (diff > worst) {
            worst = diff
            worst_row = r
            worst_bar = bar
        }
        t |> success(diff <= bar, "{tag}: the device row within {bar} of the CPU row (maxdiff {diff})")
        let cr = r + ctrl_off
        if (cr >= 0l && cr < long_length(cpu)) {
            let off = logits_maxdiff(gpu[r], cpu[cr])
            if (tight_row < 0l || off * tight_bar < tight_off * bar) {
                tight_off = off
                tight_bar = bar
                tight_row = r
            }
            t |> success(off > ctrl_bars * bar, "{tag} control: the CPU row {ctrl_off} away misses the device row by {ctrl_bars} bars (maxdiff {off}, bar {bar})")
        }
    }
    to_log(LOG_INFO, "{what}: {n} rows, the widest miss {worst} at row {worst_row} (bar {worst_bar}); the tightest control {tight_off} at row {tight_row} (bar {tight_bar}, {tight_bar > 0.0 ? tight_off / tight_bar : 0.0} bars)\n")
}

//! the prefill's logits, then each fed step's, through `s`; `after_prefill` runs between the prefill and the first step
def feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>; after_prefill : block<void>) : array<array<float>> {
    let n = long_length(prompt)
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    invoke(after_prefill)
    out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out |> emplace <| [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    }
    return <- out
}

//! the prefill's logits, then each fed step's, through `s`
def feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : array<array<float>> {
    return <- feed_logits(tr, s, prompt, toks) $ {}
}

//! the two arms' logits rows: the resident driver's, then the CPU chain's
struct FeedPair {
    gpu : array<array<float>>
    cpu : array<array<float>>
}

//! `prompt` then `toks` through the resident driver armed on `tr` (`after_prefill` between the resident prefill and its
//! first step, then the stayed-armed and served-prefill witnesses under `t`, then `after_gpu`, the caller's own witnesses
//! while the driver still holds the model), then the model dropped off the device and the same tokens through the CPU chain
def resident_then_cpu_feed(t : T?; tr : Model; prompt, toks : array<int64>; after_prefill : block<void>; after_gpu : block<void>) : FeedPair {
    var pair : FeedPair
    var sg <- create_session(tr, mirror_kv_dtype())
    let served0 = moe_gpu_resident_prefills()
    pair.gpu <- feed_logits(tr, sg, prompt, toks, after_prefill)
    t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
    t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the {long_length(prompt)}-token prompt")
    invoke(after_gpu)
    delete sg
    moe_gpu_drop_model()
    var sc <- create_session(tr, mirror_kv_dtype())
    pair.cpu <- feed_logits(tr, sc, prompt, toks)
    delete sc
    return <- pair
}

//! the same with no seat between the resident prefill and its first step
def resident_then_cpu_feed(t : T?; tr : Model; prompt, toks : array<int64>; after_gpu : block<void>) : FeedPair {
    return <- resident_then_cpu_feed(t, tr, prompt, toks, $ {}, after_gpu)
}

//! every row of `gpu` (the prefill's, then each fed step's) within `bar_rel` of the CPU chain's max live |logit| of the
//! same row, and each step's one-step-off compare (the previous step's CPU row, a disconnected mechanism) past the bar;
//! both argmaxes logged under `tag` with the piece `tr` decodes them to, the two arms named `gpu_name` and `cpu_name` in every message
def feed_steps_within(t : T?; tr : Model; tag : string; nprompt : int64; gpu, cpu : array<array<float>>; toks : array<int64>; bar_rel : float; gpu_name : string = "resident"; cpu_name : string = "the CPU chain") {
    for (st in range64(long_length(cpu))) {
        let diff = logits_maxdiff(gpu[st], cpu[st])
        let bar = bar_rel * live_maxabs(cpu[st])
        let ag = argmax_of(gpu[st])
        let ac = argmax_of(cpu[st])
        let what = tag + (st == 0l ? "prefill ({nprompt} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
        to_log(LOG_INFO, "{what}: argmax cpu {ac} '{decode_(tr, [ac])}' ({cpu[st][ac]}), {gpu_name} {ag} '{decode_(tr, [ag])}' ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
        t |> success(diff <= bar, "{what} logits: {gpu_name} within {bar} of {cpu_name} (maxdiff {diff})")
        let ctrl = st > 0l ? st - 1l : 1l   // the row one step off: the step before, or the step after for the prompt's row
        if (ctrl < long_length(cpu)) {
            let off = logits_maxdiff(gpu[st], cpu[ctrl])
            to_log(LOG_INFO, "{what}: one-step-off control maxdiff {off} (bar {bar})\n")
            t |> success(off > bar, "{what} control: the bar discriminates a one-step-off compare (maxdiff {off} vs bar {bar})")
        }
    }
}
