// The forced-feed rig of the resident files: a prompt plus fed tokens through one session with every step's
// logits kept, the resident run against the CPU chain's on the same tokens, and the per-step bar with its
// one-step-off control - the llama, qwen2, gemma, MoE and shared-expert files run it on their own carriers - and
// the resident rig under it: the cell's gate, the route-pinned load with its levers put back, the ramp ids.

options gen2
options _dasllama_internal = true

module _resident_feed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common   // Model, Session, forward_prefill, forward
require dasllama/dasllama   // create_session, moe_gpu_drop_model
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active / moe_gpu_resident_prefills: the driver's witnesses
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_: the calls the driver handed to the CPU rails, by reason
require dasllama/dasllama_kv_dtype   // kv_row_bytes / kv_layer_kbase: the head's K row
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

//! `n` ramp ids `base + ((i + salt) * stride) % span`: a prompt or a fed run no tokenizer is needed for
def ramp_ids(n, salt, base, stride, span : int64) : array<int64> => <- [for (i in range64(n)); base + ((i + salt) * stride) % span]

//! the resident cells' `n`-id prompt, shifted `salt` ids along the ramp
def resident_prompt(n : int64; salt : int64 = 0l) : array<int64> => <- ramp_ids(n, salt, 1000l, 37l, 5000l)

//! the resident cells' `n` fed tokens
def resident_feed(n : int64) : array<int64> => <- ramp_ids(n, 0l, 2000l, 53l, 4000l)

//! the JIT, the model at `path` present and in tier, and an armed GPU tier; false after the loud skip that names the missing one
def resident_ready(t : T?; path : string) : bool {
    return false if (!jit_model_available(t, path))
    if (!moe_gpu_tier_installed() || !gpu_want_auto()) {
        t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
        return false
    }
    return true
}

//! the levers a resident load pins, as the cell found them: the whole-model route's lever (never `gpu_want_resident`, which
//! folds the environment's knob in and would pin the lever off under DASLLAMA_GPU_RESIDENT=0), the mirror regions asked
//! for, and the Vulkan fuse pin where the module exists
struct ResidentLevers {
    route : bool
    regions : int64
    ctx_max : int64
    ctx_strict : bool
    fuse : int
}

def resident_levers() : ResidentLevers {
    let pins = gpu_ctx_pins()
    var l = ResidentLevers(route = gpu_resident_route_lever(), regions = gpu_resident_regions_want(), ctx_max = pins.ctx_max, ctx_strict = pins.strict, fuse = -1)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        l.fuse = g_vk_fuse
    }
    return l
}

def restore_resident_levers(l : ResidentLevers) {
    set_gpu_resident_route(l.route)
    set_gpu_resident_regions_(l.regions)
    set_gpu_ctx_max(l.ctx_max)
    set_gpu_ctx_strict(l.ctx_strict)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        g_vk_fuse = l.fuse
    }
}

//! the K/V mirror's codec ask put back as `gpu_kv_dtype_pin` read it
def restore_kv_pin(pin : tuple<pinned : bool; dt : KVDtype>) {
    if (pin.pinned) {
        set_gpu_kv_dtype(pin.dt)
    } else {
        reset_gpu_kv_dtype()
    }
}

//! `body` with the K/V mirror asked at `kv`, the ask put back as it was found - the pin for a rig that loads its own model
//! and makes its sessions on the armed codec; a skip where the environment arms another codec over the ask
def with_gpu_kv_dtype(t : T?; kv : KVDtype; body : block<void>) {
    let kv_pin = gpu_kv_dtype_pin()
    set_gpu_kv_dtype(kv)
    if (resident_mirror_dtype() == kv) {
        invoke(body)
    } else {
        t |> skip("the environment arms a {resident_mirror_dtype()} mirror over the {kv} asked (DASLLAMA_VK_KV32=1) - the cell needs that unset")
    }
    restore_kv_pin(kv_pin)
}

let NO_CENSUS = -1l   //! the census count with no vulkan module: a census witness stands down

//! the coverage census's count of a kernel's dispatches so far; NO_CENSUS with no vulkan module
def census(kernel : string) : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_kernel_coverage_of(kernel)
    } else {
        return NO_CENSUS
    }
}

//! `path` loaded at q8 with the whole-model route pinned on - at `regions` mirror regions where it is past 0, the context
//! pinned and asked whole at `strict_ctx` positions where it is past 0, else at the caller's own context pin, never strict -
//! whatever an earlier cell left; every lever put back after
def load_resident(path : string; regions : int64 = 0l; strict_ctx : int64 = 0l) : Model {
    let levers = resident_levers()
    if (regions > 0l) {
        set_gpu_resident_regions_(regions)
    }
    if (strict_ctx > 0l) {
        set_gpu_ctx_max(strict_ctx)
    }
    set_gpu_ctx_strict(strict_ctx > 0l)
    set_gpu_resident_route(true)
    var tr <- load_model_(path, QuantMode.q8)
    restore_resident_levers(levers)
    return <- tr
}

//! `load_resident` with the K/V mirror asked at `kv` for the load, the ask put back as it was found
def load_resident_at(path : string; kv : KVDtype; regions : int64 = 0l; strict_ctx : int64 = 0l) : Model {
    let pin = gpu_kv_dtype_pin()
    set_gpu_kv_dtype(kv)
    var tr <- load_resident(path, regions, strict_ctx)
    restore_kv_pin(pin)
    return <- tr
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

//! the head's K row `row` of the session's cache as bytes
def head_k_row_bytes(tr : Model; var s : Session; row : int64) : array<uint8> {
    let c = tr.config
    let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, c.n_layers))
    let kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    return <- [for (i in range64(rowb)); unsafe(kp[row * rowb + i])]
}

//! `got` within `bar` of `want` - its largest difference, a NaN read past the bar - asserted under `what`; the difference back
def row_within(t : T?; what : string; got, want : array<float>; bar : float) : float {
    let diff = logits_maxdiff(got, want)
    t |> success(diff <= bar, "{what}: within {bar} (maxdiff {diff})")
    return diff
}

//! the control that gives a row bar its teeth: `got` against `control`, a row one token or step off, past `bars` bars; the difference back
def control_past(t : T?; what : string; got, control : array<float>; bar : float; bars : float = 1.0) : float {
    let off = logits_maxdiff(got, control)
    t |> success(off > bars * bar, "{what} control: the row one off misses by {bars} bars (maxdiff {off}, bar {bar})")
    return off
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
        let diff = row_within(t, tag, gpu[r], cpu[r], bar)
        if (diff > worst) {
            worst = diff
            worst_row = r
            worst_bar = bar
        }
        let cr = r + ctrl_off
        if (cr >= 0l && cr < long_length(cpu)) {
            let off = control_past(t, tag, gpu[r], cpu[cr], bar, ctrl_bars)
            if (tight_row < 0l || off * tight_bar < tight_off * bar) {
                tight_off = off
                tight_bar = bar
                tight_row = r
            }
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
        let bar = bar_rel * live_maxabs(cpu[st])
        let ag = argmax_of(gpu[st])
        let ac = argmax_of(cpu[st])
        let what = tag + (st == 0l ? "prefill ({nprompt} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
        let diff = row_within(t, "{what} logits, {gpu_name} against {cpu_name}", gpu[st], cpu[st], bar)
        to_log(LOG_INFO, "{what}: argmax cpu {ac} '{decode_(tr, [ac])}' ({cpu[st][ac]}), {gpu_name} {ag} '{decode_(tr, [ag])}' ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
        let ctrl = st > 0l ? st - 1l : 1l   // the row one step off: the step before, or the step after for the prompt's row
        if (ctrl < long_length(cpu)) {
            let off = control_past(t, "{what} one-step-off", gpu[st], cpu[ctrl], bar)
            to_log(LOG_INFO, "{what}: one-step-off control maxdiff {off} (bar {bar})\n")
        }
    }
}

//! three chains' summed row distances - the device and the CPU chain each from a reference chain, and from each other -
//! each `_ctrl` the same compare with its second side a row one step off
struct ChainDistances {
    dev_ref : float
    cpu_ref : float
    dev_cpu : float
    dev_ref_ctrl : float
    dev_cpu_ctrl : float
}

//! The distances over rows [first, ..) of three chains - the device, the CPU chain `cpu_name` and the reference `ref_name` -
//! every row logged with each chain's argmax decoded. A NaN in a row reads as an infinite distance, which would pass any
//! ratio of two sums, so every sum is asserted finite.
def chain_distances(t : T?; tr : Model; what : string; gpu, cpu, ref_chain : array<array<float>>; first : int; ref_name : string = "f16"; cpu_name : string = "the CPU codec chain") : ChainDistances {
    var dist : ChainDistances
    for (st in range(first, length(ref_chain))) {
        let off = st > 0 ? st - 1 : 1
        let dd = logits_maxdiff(gpu[st], ref_chain[st])
        let dc = logits_maxdiff(cpu[st], ref_chain[st])
        let dp = logits_maxdiff(gpu[st], cpu[st])
        let af = argmax_of(ref_chain[st])
        let ac = argmax_of(cpu[st])
        let ag = argmax_of(gpu[st])
        to_log(LOG_INFO, "{what}, row {st}: the device {dd} from {ref_name}, {cpu_name} {dc}, the two {dp} apart; argmax {ref_name} {af} '{decode_(tr, [af])}', {cpu_name} {ac} '{decode_(tr, [ac])}', device {ag} '{decode_(tr, [ag])}'\n")
        dist.dev_ref += dd
        dist.cpu_ref += dc
        dist.dev_cpu += dp
        dist.dev_ref_ctrl += logits_maxdiff(gpu[st], ref_chain[off])
        dist.dev_cpu_ctrl += logits_maxdiff(gpu[st], cpu[off])
    }
    to_log(LOG_INFO, "{what}: summed distances - the device from {ref_name} {dist.dev_ref}, {cpu_name} from {ref_name} {dist.cpu_ref}, the two apart {dist.dev_cpu}; one step off - the device from {ref_name} {dist.dev_ref_ctrl}, from {cpu_name} {dist.dev_cpu_ctrl}\n")
    let finite = (dist.dev_ref < INFINITE_DIFF && dist.cpu_ref < INFINITE_DIFF && dist.dev_cpu < INFINITE_DIFF
        && dist.dev_ref_ctrl < INFINITE_DIFF && dist.dev_cpu_ctrl < INFINITE_DIFF)
    t |> success(finite, "{what}: every chain's rows are finite (a NaN row reads as an infinite distance)")
    return dist
}

//! layer `l`'s first `rows` K rows then V rows as `s` holds them
def layer_rows(tr : Model; var s : Session; l, rows : int64) : array<uint8> {
    let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(tr.config, l))
    var bytes : array<uint8>
    bytes |> resize(2l * rows * rowb)
    unsafe {
        memcpy(addr(bytes[0]), kv_layer_kbase(s, l, tr.config.seq_len), rows * rowb)
        memcpy(addr(bytes[rows * rowb]), kv_layer_vbase(s, l, tr.config.seq_len), rows * rowb)
    }
    return <- bytes
}

//! the share of `n` bytes at which `a` from byte `a_at` differs from `b` from byte `b_at`
def bytes_differing(a, b : array<uint8>; a_at, b_at, n : int64) : float {
    var sa <- copy_span(a, a_at, n)
    var sb <- copy_span(b, b_at, n)
    let differing = mismatch_exact(sa, sb)
    delete sa
    delete sb
    return float(differing) / float(n)
}
