// The block drafter's walks (DFlash2 and DFlash files), one body for every trunk and home: `test_dflash_drafter.das` runs them
// on the trunks it loads, `test_gpu_resident_gemma4_e2b.das` and `test_gpu_resident_qwen3.das` on the forms their load-once
// blocks load - the lossless greedy walk against the plain one, the three sampled acceptances against the plain sampled walk, the drafter planes' disk-type tags, and the discrete
// homes' legs (the device selector and walk against the host's, a stop id, an embedding call). Each walk attaches the drafter
// on the arm it asks, asserts on `t`, and puts every knob it set back.

options gen2
options stack = 524288
options _dasllama_internal = true

module _dflash_cells shared public

//! the block drafter's walks on a trunk its caller loaded; an entry attaches its drafter, loads no trunk

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama   // the public facade - attach_block_drafter, generate_drafted, embed
require dasllama/dasllama_common   // generate, generate_mtp_greedy, make_run_state
require dasllama/dasllama_dflash   // the subject: the DFlash2 block drafter's loader, seams and CPU arm
require dasllama/dasllama_sampling   // generate_, generate_mtp_sampled, set_seed_: the sampled walks
require dasllama/dasllama_gguf   // gguf_header_read, gguf_find_tensor: the drafter file's disk types against the loader's tags
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_verifies: the discrete walks' verifies on the resident driver
require ?das_metal dasllama/dasllama_metal_dflash   // nolint:STYLE030,LINT019 - the device selector's counters, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the decode drivers' counts, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_decode   // nolint:STYLE030,LINT019 - metal_decode_shutdown, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030,LINT019 - metal_prefill_stats, metal_prefill_shutdown, inside the das_metal half
require ?das_metal metal/das_metal_boost   // nolint:STYLE030,LINT019 - metal_log_leaks_if_any, inside the das_metal half
require daslib/fio
require daslib/random   // random_seed, random_float: the draws a seeded walk spends
require math   // min, max, abs
require _model_tier   // _model_tier::models_dir() + model_available() presence/size-tier gate
require _compares   // first_mismatch
require _resident_feed   // served_every_call, cuda_served_since: the discrete walks' served witnesses
require _model_form   // device_form, asking, with_ctx_cut: the drafter form and the CPU drafter scope
require ?vulkan dasllama/dasllama_vulkan_dflash   // nolint:STYLE030,LINT019 - the discrete cell's static_if reads the Vulkan arm
require ?cuda dasllama/dasllama_cuda_dflash   // nolint:STYLE030,LINT019 - ... and the CUDA arm

let PROMPT = "Write a complete, well-commented Python module that implements a token-bucket rate limiter with per-key buckets, a sliding-window variant, a thread-safe wrapper, and a pytest test suite. Write at least 600 lines. Do not stop early."
let STEPS = 24l
let HOST_CTX = 4096l   //! the context a trunk off the image rail is held to: a flat session's host K/V is the model's whole context (18 GB a session at the 27B's 262144), and the walks here run under two hundred positions

let E2B_TRUNK_FILE = "gemma-4-E2B-it-Q8_0.gguf"
let E2B_DRAFTER_FILE = "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-Q8_0.gguf"
let E2B_DRAFTER_K4_FILE = "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-Q4_K_M-local.gguf"   //! Q4_K planes, Q6_K at two layers' v and down
let E2B_DRAFTER_NV4_FILE = "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-NVFP4-pure-local.gguf"   //! every weight plane NVFP4, no `.scale` beside any
let QWEN3_4B_TRUNK_FILE = "Qwen3-4B-Q4_K_M.gguf"
let QWEN3_4B_DRAFTER_FILE = "drafters/Qwen3-4B-DFlash-b16-GGUF/Qwen3-4B-DFlash-b16-Q8_0.gguf"   //! a DFlash file: no selector, its drafts the trunk classifier's
let QWEN3_4B_DRAFTER_K4_FILE = "drafters/Qwen3-4B-DFlash-b16-GGUF/Qwen3-4B-DFlash-b16-Q4_K_M.gguf"   //! Q4_K planes, Q6_K at two layers' v and down
let G26_DRAFTER_FILE = "drafters/gemma-4-26B-A4B-it-DFlash-GGUF/gemma-4-26B-A4B-it-DFlash-Q8_0.gguf"   //! the routed 26B's regular DFlash drafter
let G26_DRAFTER_K4_FILE = "drafters/gemma-4-26B-A4B-it-DFlash-GGUF/gemma-4-26B-A4B-it-DFlash-Q4_K_M.gguf"
let COMPLETE_STEPS = 64l
let COUNT_PROMPT = "Count from one to forty in words, one number a line."   //! a walk with no near tie: a discrete home's rows and its one-row step round apart
let DISCRETE_CTX = 1024l
let DISCRETE_MIRROR_CTX = 8192l   //! the mirror's positions asked whole: an uncapped mirror takes the card's room, and the drafter's planes and ring find none

//! every weight plane of the drafter at `path` tagged with the format of its disk type
def planes_tag_disk_types(t : T?; path : string; tag : string) {
    var dr <- load_dflash_drafter(path)
    t |> equal(dr.why, "", "{tag}: the drafter loads")
    if (dr.ok) {
        var planes <- [("fc.weight", dr.fc)]
        if (dr.v2) {
            planes |> push_from([("selector_hidden.weight", dr.sel_h), ("selector_predecessor.weight", dr.sel_prev),
                                 ("selector_successor.weight", dr.sel_next)])
        }
        planes |> reserve(4l + 9l * dr.n_layers)
        for (l in range64(dr.n_layers)) {
            planes |> push(("blk.{l}.attn_q.weight", dr.wq[l]))
            planes |> push(("blk.{l}.attn_k.weight", dr.wk[l]))
            planes |> push(("blk.{l}.attn_v.weight", dr.wv[l]))
            planes |> push(("blk.{l}.attn_output.weight", dr.wo[l]))
            planes |> push(("blk.{l}.ffn_gate.weight", dr.w1[l]))
            planes |> push(("blk.{l}.ffn_up.weight", dr.w3[l]))
            planes |> push(("blk.{l}.ffn_down.weight", dr.w2[l]))
            if (dr.v2) {
                planes |> push(("blk.{l}.attn_conv_proj.weight", dr.aconv_proj[l]))
                planes |> push(("blk.{l}.ffn_conv_proj.weight", dr.fconv_proj[l]))
            }
        }
        var off_tag = ""
        gguf_header_read(path) $(m; _bytes) {
            for (p in planes) {
                let gt = m.tensors[gguf_find_tensor(m, p._0)].ggml_type
                if (kq_ggml_type(p._1.fmt) != gt && empty(off_tag)) {
                    off_tag = "{p._0}: tagged {p._1.fmt} over disk type {ggml_type_name(gt)}"
                }
            }
        }
        t |> equal(off_tag, "", "{tag}: all {length(planes)} planes tagged with their disk type's format")
        delete planes
    }
    delete dr
}

//! the drafter file `drafter` on the CPU arm over `m`: its planes held to their disk types where `tags`, then the lossless walk
//! (`walk_text(chat)`), the drafter detached after it
def cpu_drafter_lossless(t : T?; m : Model; drafter, tag : string; tags : bool; chat : bool = false) {
    let path = path_join(_model_tier::models_dir(), drafter)
    return if (!model_available(t, path))
    if (tags) {
        planes_tag_disk_types(t, path, tag)
    }
    lossless_walk(t, m, drafter, false, tag, walk_text(chat), chat)
    detach_dflash_drafter()
}

//! a walk's prompt ids: `text` raw, or as one user turn of the model's chat template with thinking off (`chat`) - the form a
//! routed gemma-4 trunk needs, whose logits sit at near-ties past a raw prompt
def walk_prompt(m : Model; text : string; chat : bool) : array<int64> {
    return <- encode(m, text, true) if (!chat)
    var turn <- create_chat_renderer(m, "")
    set_thinking(turn, false)
    add_user(turn, text)
    var ids <- render_turn(m, turn)
    delete turn
    return <- ids
}

//! the text a CPU or Metal lossless walk continues: the prose prompt, or for the chat form the counting prompt - the chat
//! form is the routed gemma-4 trunk's, whose logits sit at near-ties past prose, so its exact compare takes a continuation
//! the prompt's own text dictates
def walk_text(chat : bool) : string => chat ? COUNT_PROMPT : PROMPT

//! a lossless walk's served witness: `walk`'s ids back, the caller's counters read around it
typedef WalkServed = block<(spec : bool; walk : block<() : array<int64>>) : array<int64>>

//! the plain greedy walk against the speculative one on `m`, the drafter file `drafter` attached on the asked arm: the ids
//! equal, drafts accepted; the accepted count back for the arms' compare, the drafter left attached for the caller's witnesses,
//! the arm's knob put back as found; `served` runs each walk (`spec` false for the plain one) inside the caller's counter readings
def lossless_walk(t : T?; m : Model; drafter : string; device : bool; tag : string; text : string; chat : bool; served : WalkServed) : int64 {
    let device0 = dflash_device_knob()
    set_dflash_device(device)
    let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter))
    set_dflash_device(device0)
    t |> equal(why, "", "{tag}: the drafter attaches")
    return 0l if (!empty(why))
    t |> equal(dflash_on_device(), device, "{tag}: the pass runs on the {device ? "device" : "CPU"}")
    let prompt <- walk_prompt(m, text, chat)
    let total = long_length(prompt) - 1l + STEPS
    var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    let plain <- invoke(served, false, $() : array<int64> {
        return <- generate(m, s1, prompt, int(total))
    })
    var s2 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var nd = 0l
    var na = 0l
    let spec <- invoke(served, true, $() : array<int64> {
        return <- generate_mtp_greedy(m, s2, prompt, total, nd, na)
    })
    t |> equal(length(spec), length(plain), "{tag}: the walks are the same length")
    var same = 0l
    while (same < long_length(plain) && same < long_length(spec) && plain[same] == spec[same]) {
        same++
    }
    to_log(LOG_INFO, "{tag} plain text: {eyeball_text(m, plain)}\n")
    to_log(LOG_INFO, "{tag} speculative text: {eyeball_text(m, spec)}\n")
    t |> equal(same, long_length(plain), "{tag}: speculative == plain token for token (both walks decoded above)")
    let off = min(length(spec), length(plain)) - 1
    t |> success(off > 0 && first_mismatch(spec, plain, 0, 1, off) >= 0, "{tag}: the control - the speculative walk against the plain walk one step on misses")
    t |> success(na > 0l && nd >= na, "{tag}: drafts accepted {na} of {nd}")
    delete s1
    delete s2
    return na
}

def lossless_walk(t : T?; m : Model; drafter : string; device : bool; tag : string; text : string = PROMPT; chat : bool = false) : int64 {
    return lossless_walk(t, m, drafter, device, tag, text, chat) $(_spec; walk) {
        return <- invoke(walk)
    }
}

//! `a` and `b` recorded drafts id for id, neither empty, each side's drafts decoded in the log
def same_drafts(t : T?; m : Model; a, b : array<int64>; what : string) {
    to_log(LOG_INFO, "{what}, the first side's drafts: {eyeball_text(m, a)}\n")
    to_log(LOG_INFO, "{what}, the second side's drafts: {eyeball_text(m, b)}\n")
    var same = 0
    while (same < length(a) && same < length(b) && a[same] == b[same]) {
        same++
    }
    t |> success(!empty(a) && same == length(a) && same == length(b), "{what} id for id ({same} of {length(a)} / {length(b)} equal)")
}

let SAMPLED_PARAMS = SamplingParams(temp = 0.7)
let SAMPLED_SEED = 4242
let SAMPLED_STEPS = 64l
let DRAWS_LIMIT = 1024

//! the speculative sampled walk on `m` under `accept`, the drafter at `draft_temp`, seeded at SAMPLED_SEED
def spec_sampled(m : Model; prompt : array<int64>; accept : MtpAccept; draft_temp : float) : MtpSampledWalk {
    var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var w <- spec_sampled_on(m, s, prompt, accept, draft_temp)
    delete s
    return <- w
}

//! the same on the caller's session `s`, whose random stream the caller reads after it
def spec_sampled_on(m : Model; var s : Session; prompt : array<int64>; accept : MtpAccept; draft_temp : float) : MtpSampledWalk {
    set_mtp_accept(accept)
    set_dflash_draft_temp(draft_temp)
    set_seed_(s, SAMPLED_SEED)
    return <- generate_mtp_sampled(m, s, prompt, SAMPLED_PARAMS, SAMPLED_STEPS)
}

//! the draws past SAMPLED_SEED that leave a session's random stream at `rng`; -1 past `limit`
def draws_spent(rng : int4; limit : int) : int {
    var at = random_seed(SAMPLED_SEED)
    for (n in range(limit + 1)) {
        return n if (at == rng)
        random_float(at)
    }
    return -1
}

//! the device selector's ended passes so far (0 off the Metal build)
def device_selections() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_dflash_selected().selected
    } else {
        return 0l
    }
}

//! the device selector's passes that drew a sampled round's drafts so far (0 off the Metal build)
def device_soft_picks() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_dflash_soft_picked()
    } else {
        return 0l
    }
}

//! the times the attached drafter's ring grew its dense layers: the Metal driver's count (`metal`), else the discrete home's
def private ring_grows(metal : bool) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_dflash_ring_grows() if (metal)
    }
    return dflash_device_stats().grows
}

//! the sampled rounds whose pass held with the verify committed behind it so far (0 off the Metal build)
def sampled_ahead_rounds() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_verify_ahead_rounds()
    } else {
        return 0l
    }
}

//! the verify-ahead level, the prior one back (0 off the Metal build)
def set_verify_ahead(level : int) : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior = get_metal_verify_ahead()
        set_metal_verify_ahead(level)
        return prior
    } else {
        return 0
    }
}

//! the device selector's draw of a sampled round's drafts on or off (nothing off the Metal build)
def set_device_soft_pick(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_dflash_soft_pick(on)
    }
}

//! the exact walk against the plain one of its seed: the plain walk spends one draw an id, the exact walk one a judged row -
//! its first id, each accepted draft, each round's next id - and its first id, drawn off the plain walk's prefill row by the
//! seed's first draw, is the plain walk's; where `tokens_hold` the two are the same ids, elsewhere the shared ids are logged
def private exact_holds_plain(t : T?; m : Model; tag : string; tokens_hold : bool;
                              plain : array<int64>; plain_rng : int4;
                              exact : MtpSampledWalk; exact_rng : int4) {
    let same = common_prefix_len(plain, exact.ids)
    let plain_text = eyeball_text(m, plain)
    let exact_text = eyeball_text(m, exact.ids)
    to_log(LOG_INFO, "{tag} plain sampled text: {plain_text}\n")
    to_log(LOG_INFO, "{tag} exact sampled text: {exact_text}\n")
    t |> equal(draws_spent(plain_rng, DRAWS_LIMIT), length(plain), "{tag}: the plain walk spends one draw an id")
    t |> equal(draws_spent(exact_rng, DRAWS_LIMIT), int(1l + exact.accepted + exact.rounds),
        "{tag}: the exact walk spends the first id's draw, one an accepted draft and one a round for the id after its run - none on a draft it rejects ({length(exact.ids)} ids, {exact.accepted} accepted, {exact.rounds} rounds)")
    t |> success(same > 0l, "{tag}: the exact walk's first id is the plain walk's - both drawn off "
        + "one prefill row by the seed's first draw")
    if (tokens_hold) {
        t |> equal(length(exact.ids), length(plain), "{tag}: the exact walk is the plain walk's length")
        t |> equal(same, long_length(plain), "{tag}: exact == the plain sampled walk token for token (plain '{decode(m, plain)}', spec '{decode(m, exact.ids)}')")
    } else {
        to_log(LOG_INFO, "{tag}: exact == the plain sampled walk for {same} of {length(plain)} "
            + "ids - plain '{plain_text}', exact '{exact_text}'\n")
    }
    t |> success(exact.accepted > 0l, "{tag}: exact accepted {exact.accepted} of {exact.drafted}")
}

//! the three acceptances over the drafter file `drafter` on `m`, the pass on the asked arm, against the plain sampled walk at the
//! same seed: exact spends a draw a judged row with drafts accepted and opens on the plain walk's first id - where `tokens_hold`
//! it equals the plain walk token for token, elsewhere the ids the two share are logged; each rejection walk (point-mass drafts,
//! drafts drawn at the stream's temperature) accepts some drafts and rejects some - its stream parts from the plain one at its
//! first rejection, so its rate is not the exact walk's to compare (the closed form is test_mtp_accept_walks.das's), the three
//! rates logged; on the device arm the drawn drafts drawn by the device selector, and with its soft pick off drawn on the host -
//! the same stream
def sampled_modes_walk(t : T?; m : Model; drafter : string; device : bool; tag : string;
                       tokens_hold : bool = false) {
    set_dflash_device(device)
    let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter))
    t |> equal(why, "", "{tag}: the drafter attaches")
    return if (!empty(why))
    let prompt <- encode(m, PROMPT, true)
    var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_seed_(s1, SAMPLED_SEED)
    var plain : array<int64>
    generate_(m, s1, prompt, SAMPLED_PARAMS, SAMPLED_STEPS) $(id : int64; _piece : string) : bool {
        plain |> push(id)
        return true
    }
    let knob = get_mtp_accept()
    let temp0 = get_dflash_draft_temp()
    let sel_before_exact = device_selections()
    var s2 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var inscope exact <- spec_sampled_on(m, s2, prompt, MtpAccept.exact, 0.0)
    let exact_rng = s2.rng
    delete s2
    let sel_exact = device_selections() - sel_before_exact
    exact_holds_plain(t, m, tag, tokens_hold, plain, s1.rng, exact, exact_rng)
    var stops <- [exact.ids[long_length(exact.ids) / 2l]]
    let stop_at = find_index(exact.ids, stops[0])
    set_mtp_accept(MtpAccept.exact)
    var s3 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_seed_(s3, SAMPLED_SEED)
    var inscope stopped <- generate_mtp_sampled(m, s3, prompt, SAMPLED_PARAMS, SAMPLED_STEPS, stops)
    t |> equal(length(stopped.ids), stop_at + 1, "{tag}: a stop id ends the exact walk on its first occurrence, kept as the last id (the stopless walk ran {length(exact.ids)})")
    t |> success(length(stopped.ids) == stop_at + 1 && stopped.ids[stop_at] == stops[0], "{tag}: ... the stop the stream's last id")
    delete s3
    delete stops
    var inscope point <- spec_sampled(m, prompt, MtpAccept.rejection, 0.0)
    t |> success(point.accepted > 0l && point.accepted < point.drafted,
        "{tag}: point-mass drafts accept some and reject some ({point.accepted} of {point.drafted}; exact {exact.accepted} of {exact.drafted})")
    let sel_before_soft = device_selections()
    let soft_before = device_soft_picks()
    var inscope soft <- spec_sampled(m, prompt, MtpAccept.rejection, -1.0)
    let sel_soft = device_selections() - sel_before_soft
    let soft_drawn = device_soft_picks() - soft_before
    t |> success(soft.accepted > 0l && soft.accepted < soft.drafted,
        "{tag}: drafts drawn at the stream's temperature (the knob's default) accept some and reject some ({soft.accepted} of {soft.drafted}; exact {exact.accepted} of {exact.drafted})")
    to_log(LOG_INFO, "{tag}: tokens a round exact {float(exact.accepted + exact.rounds) / float(max(exact.rounds, 1l))}, point {float(point.accepted + point.rounds) / float(max(point.rounds, 1l))}, drawn {float(soft.accepted + soft.rounds) / float(max(soft.rounds, 1l))}\n")
    to_log(LOG_INFO, "{tag}: drawn drafts' stream '{decode(m, soft.ids)}'\n")
    if (device && dflash_on_device()) {
        t |> success(sel_exact > 0l, "{tag}: the exact walk's greedy drafts end on the device selector ({sel_exact} passes)")
        t |> success(soft_drawn > 0l && soft_drawn == sel_soft, "{tag}: drawn drafts end on the device selector, which draws them ({soft_drawn} of {sel_soft} passes)")
        t |> success(sampled_ahead_rounds() > 0l, "{tag}: the sampled rounds' passes held with their verifies committed behind them ({sampled_ahead_rounds()})")
        let ahead0 = set_verify_ahead(0)
        let ahead_before = sampled_ahead_rounds()
        var inscope waited <- spec_sampled(m, prompt, MtpAccept.rejection, -1.0)
        t |> equal(sampled_ahead_rounds(), ahead_before, "{tag}: the verify-ahead knob off, no sampled round holds its pass")
        set_verify_ahead(ahead0)
        var same_waited = 0l
        while (same_waited < long_length(soft.ids) && same_waited < long_length(waited.ids) && soft.ids[same_waited] == waited.ids[same_waited]) {
            same_waited++
        }
        t |> success(same_waited == long_length(soft.ids) && length(soft.ids) == length(waited.ids) && soft.accepted == waited.accepted && soft.drafted == waited.drafted,
            "{tag}: the held sampled rounds draw the waited rounds' stream - equal for {same_waited} of {length(soft.ids)} ids, accepted {soft.accepted} / {waited.accepted} of {soft.drafted} / {waited.drafted}")
        set_device_soft_pick(false)
        let sel_before_host = device_selections()
        var inscope host_drawn <- spec_sampled(m, prompt, MtpAccept.rejection, -1.0)
        t |> equal(device_selections() - sel_before_host, 0l, "{tag}: the soft pick knob off, drawn drafts stand the device selector down")
        set_device_soft_pick(true)
        var same_soft = 0l
        while (same_soft < long_length(soft.ids) && same_soft < long_length(host_drawn.ids) && soft.ids[same_soft] == host_drawn.ids[same_soft]) {
            same_soft++
        }
        t |> success(same_soft == long_length(soft.ids) && length(soft.ids) == length(host_drawn.ids) && soft.accepted == host_drawn.accepted,
            "{tag}: the device's draws are the host's from the same rng - the streams equal for {same_soft} of {length(soft.ids)} ids, accepted {soft.accepted} / {host_drawn.accepted}")
    }
    set_mtp_accept(knob)
    set_dflash_draft_temp(temp0)
    detach_dflash_drafter()
    delete plain
    delete s1
}

//! `walk` on the discrete trunk, served on the device: no call passed to the CPU rails, on the CUDA home the driver serving at
//! least the walk's decode positions, and a speculative walk's verifies on the resident driver of either home
def discrete_trunk_served(t : T?; spec : bool; what : string; walk : block<() : array<int64>>) : array<int64> {
    let c0 = cuda_positions_served()
    let v0 = moe_gpu_resident_verifies()
    var ids <- invoke(walk)
    served_every_call(t)
    cuda_served_since(t, c0, STEPS - 2l, what)
    if (spec) {
        t |> success(moe_gpu_resident_verifies() > v0, "{what}: the speculative walk's verifies ran on the resident driver ({moe_gpu_resident_verifies() - v0})")
    }
    return <- ids
}

//! the lossless walk over the counting prompt on the discrete trunk, each walk's trunk served on the device
def discrete_lossless_walk(t : T?; m : Model; drafter, tag : string; chat : bool = false) : int64 {
    return lossless_walk(t, m, drafter, true, tag, COUNT_PROMPT, chat) $(spec; walk) {
        return <- discrete_trunk_served(t, spec, "{tag}, the {spec ? "speculative" : "plain"} walk", walk)
    }
}

//! one drafter file's legs on the discrete trunk `m`: the counting walk the plain walk's with every pass on the device
//! selector and its walk; the walk knob off, the host's walk over the landed lists drafts the same ids; the selector knob
//! off, the host's selector over the landed logits drafts them too; every walk's trunk served on the device. A DFlash file
//! (`dflash2` false) carries no selector: its pass ends at the top-k, with no walk for either side
def discrete_drafter_legs(t : T?; m : Model; drafter, tag : string; chat : bool = false; dflash2 : bool = true) {
    return if (!model_available(t, path_join(_model_tier::models_dir(), drafter)))
    let selector0 = dflash_device_selector()
    let record0 = dflash_record_on()
    set_dflash_device_selector(true, true)
    set_dflash_record(true)
    let dev = discrete_lossless_walk(t, m, drafter, "{tag} device arm", chat)
    var inscope dev_drafts <- dflash_recorded_drafts()
    let c = dflash_device_stats()
    to_log(LOG_INFO, "{tag}: the device arm committed {c.commits} times over {c.passes} passes\n")
    t |> success(c.passes > 0l, "{tag}: the pass ran on the device ({c.passes} passes)")
    t |> equal(c.selected, c.passes, "{tag}: every pass ended on the device's top-k")
    if (dflash2) {
        t |> equal(c.walked, c.passes, "{tag}: every pass's selector walk ran on the device")
    } else {
        t |> equal(c.walked, 0l, "{tag}: a file with no selector has no walk for the device")
    }
    let ctrl_prompt <- encode(m, COUNT_PROMPT, true)
    var s0 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    let plain <- discrete_trunk_served(t, false, "{tag}: the control's plain walk") $() : array<int64> {
        return <- generate(m, s0, ctrl_prompt, int(long_length(ctrl_prompt) - 1l + STEPS))
    }
    delete s0
    let cp = dflash_device_stats()
    t |> equal(cp.passes, c.passes,
        "{tag}: the control - a plain walk of {length(plain)} ids with the drafter attached runs no pass (its rows' context still commits: {cp.commits - c.commits} commits)")
    detach_dflash_drafter()
    set_dflash_device_selector(true, false)
    set_dflash_record(true)
    let host_walk = discrete_lossless_walk(t, m, drafter, "{tag} device arm, the host's walk", chat)
    var inscope host_walk_drafts <- dflash_recorded_drafts()
    let cw = dflash_device_stats()
    t |> success(cw.selected == cw.passes && cw.walked == 0l, "{tag}: the walk knob off, the lists land for the host's walk ({cw.walked} walked on the device)")
    detach_dflash_drafter()
    t |> equal(host_walk, dev, "{tag}: the host's walk accepts the device walk's count")
    same_drafts(t, m, dev_drafts, host_walk_drafts, "{tag}: the device walk's drafts are the host walk's")
    set_dflash_device_selector(false, true)
    set_dflash_record(true)
    let host_sel = discrete_lossless_walk(t, m, drafter, "{tag} device arm, the host's selector", chat)
    var inscope host_sel_drafts <- dflash_recorded_drafts()
    t |> equal(dflash_device_stats().selected, 0l, "{tag}: the selector knob off, no pass ends on the device's top-k")
    detach_dflash_drafter()
    set_dflash_device_selector(selector0.topk, selector0.walk)
    set_dflash_record(record0)
    t |> equal(host_sel, dev, "{tag}: the host's selector accepts the device selector's count")
    same_drafts(t, m, dev_drafts, host_sel_drafts, "{tag}: the device selector's drafts are the host selector's")
}

//! the stop id on the discrete trunk `m` (the resident verify seat's accept walk): a greedy completion drafted through the facade
//! with the plain stream's middle id as a stop ends on that id's first occurrence, the stopless drafted stream (the plain
//! stream's ids) running past it - the control
def discrete_stop_leg(t : T?; m : Model; drafter, tag : string) {
    return if (!model_available(t, path_join(_model_tier::models_dir(), drafter)))
    let prompt <- encode(m, COUNT_PROMPT, true)
    var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var inscope plain_ids : array<int64>
    generate(m, s1, prompt, SamplingParams(temp = 0.0), COMPLETE_STEPS) $(id, _piece) {
        plain_ids |> push(id)
        return true
    }
    delete s1
    let device0 = dflash_device_knob()
    set_dflash_device(true)
    t |> equal(attach_block_drafter(m, path_join(_model_tier::models_dir(), drafter)), "", "{tag}: the drafter attaches through the facade")
    set_dflash_device(device0)
    t |> equal(dflash_on_device(), true, "{tag}: the pass runs on the device")
    var s2 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var inscope drafted_ids, no_stops : array<int64>
    generate_drafted(m, s2, prompt, SamplingParams(temp = 0.0), COMPLETE_STEPS, no_stops) $(id, _piece) {
        drafted_ids |> push(id)
        return true
    }
    t |> success(s2.mtp_accepted > 0l, "{tag}: the stopless drafted stream accepted drafts ({s2.mtp_accepted} of {s2.mtp_drafted})")
    t |> equal(first_mismatch(drafted_ids, plain_ids), -1, "{tag}: the stopless drafted stream is the plain stream ('{decode(m, plain_ids)}')")
    delete s2
    let stop_id = plain_ids[length(plain_ids) / 2]
    var first = 0
    while (plain_ids[first] != stop_id) {
        first++
    }
    var s3 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var inscope stops <- [stop_id]
    var inscope stopped_ids : array<int64>
    generate_drafted(m, s3, prompt, SamplingParams(temp = 0.0), COMPLETE_STEPS, stops) $(id, _piece) {
        return false if (id == stop_id)
        stopped_ids |> push(id)
        return true
    }
    log_gen_texts(m, "{tag} (stopped, plain)", stopped_ids, plain_ids)
    t |> equal(length(stopped_ids), first, "{tag}: the stopped stream emits the ids before the stop's first occurrence ({first})")
    t |> success(first < length(plain_ids) - 1, "{tag}: the stop sits inside the budget (the control: the stopless stream ran past it)")
    delete s3
    detach_block_drafter()
}

let EMBED_TEXT_REPS = 24   //! the embedding call's text: the counting prompt this many times, rows past every earlier prefill's

//! an embedding call on the discrete trunk `m` with the drafter attached (a hidden-only prefill): it commits no drafter context
//! and leaves the capture block whole - the vector finite, and the counting walk after it still the plain walk's; a plain prefill
//! with the drafter attached committing context is the control
def discrete_embed_leg(t : T?; m : Model; drafter, tag : string) {
    return if (!model_available(t, path_join(_model_tier::models_dir(), drafter)))
    let device0 = dflash_device_knob()
    set_dflash_device(true)
    t |> equal(attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter)), "", "{tag}: the drafter attaches")
    set_dflash_device(device0)
    t |> equal(dflash_on_device(), true, "{tag}: the pass runs on the device")
    let ctrl_prompt <- encode(m, COUNT_PROMPT, true)
    let c0 = dflash_device_stats()
    var s0 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var inscope ctrl <- generate(m, s0, ctrl_prompt, int(long_length(ctrl_prompt)))
    delete s0
    let c1 = dflash_device_stats()
    t |> success(c1.commits > c0.commits, "{tag}: control - a plain prefill with the drafter attached commits its context ({c1.commits - c0.commits})")
    let text = build_string() $(var w) {
        for (_i in range(EMBED_TEXT_REPS)) {
            w |> write("{COUNT_PROMPT} ")
        }
    }
    let rows = length(encode(m, text, true))
    var inscope v <- embed(m, text)
    let c2 = dflash_device_stats()
    t |> equal(c2.commits, c1.commits, "{tag}: the embedding call over {rows} rows committed no drafter context")
    t |> success(!empty(v) && empty([for (x in v); x; where !(abs(x) <= 3.4e38)]), "{tag}: the embedding vector is finite ({length(v)} wide)")
    detach_dflash_drafter()
    discrete_lossless_walk(t, m, drafter, "{tag}, the walk after the embedding call")
    detach_dflash_drafter()
}
//! each drafter file of `drafters` on the discrete trunk `m` the caller loaded: its legs (`discrete_drafter_legs`), and under
//! `stop_embed` the stop and embedding legs after them
def discrete_drafter_cells(t : T?; m : Model; drafters : array<string>; tag : string; stop_embed : bool = false; chat : bool = false;
                           dflash2 : bool = true) {
    for (d in drafters) {
        discrete_drafter_legs(t, m, d, "{tag}, {base_name(d)}", chat, dflash2)
        if (stop_embed) {
            discrete_stop_leg(t, m, d, "{tag}, {base_name(d)}, a stop id")
            discrete_embed_leg(t, m, d, "{tag}, {base_name(d)}, an embedding call")
        }
    }
}

//! the discrete homes' drafter form: the f16 mirror asked DISCRETE_MIRROR_CTX positions whole, the host-cached sessions at
//! DISCRETE_CTX
def discrete_drafter_form() : ModelForm => device_form(ctx = DISCRETE_MIRROR_CTX, host_ctx = DISCRETE_CTX, strict_ctx = true) |> asking(KVDtype.f16)

//! `discrete_drafter_cells` as the cell `title` on the drafter form `m`, under the arm `arm` where one is named; a build with no
//! discrete home's drafter arm skips it, `tag` naming the cells
def discrete_drafter_cell(t : T?; m : Model; title, arm : string; drafters : array<string>; tag : string; stop_embed : bool = false;
                          chat : bool = false; dflash2 : bool = true) {
    static_if (typeinfo module_exists(dasllama_vulkan_dflash) || typeinfo module_exists(dasllama_cuda_dflash)) {
        unsafe {   //! the cell captures the loaded trunk by reference
            t |> run(title) @capture(& m, & drafters)(t : T ?) {
                return if (!empty(arm) && !arm_on(t, arm))
                discrete_drafter_cells(t, m, drafters, tag, stop_embed, chat, dflash2)
            }
        }
    } else {
        skip_cell(t, "{tag}: no discrete GPU home in this build")
    }
}

let DENSE_CTX = 8192l                  //! the dense cell's trunk context: a prompt past two of the drafter's windows fits
let private DENSE_TEXT_ITEMS = 520     //! ... the prompt's items, each a number no neighbour predicts, then the ask to list them again: the walk copies lines only a reading past the window still holds
let private DENSE_STEPS = 64l          //! ... and the ids each walk generates past it

//! the drafts a greedy speculative walk of DENSE_STEPS ids over `prompt` records on the device trunk `m` (`metal`: the Metal home's), the drafter file
//! attached on the device arm under the dense reading `dense` with new rings of `reserve` dense slots (0 = the window); the
//! times the session's ring grew into `grows`, the drafter detached after
def private dense_walk_drafts(t : T?; m : Model; drafter, tag : string; prompt : array<int64>; dense : bool; reserve : int64; metal : bool; var grows : int64&) : array<int64> {
    let dense_was = dflash_dense_knob()
    let reserve_was = dflash_dense_reserve()
    set_dflash_dense(dense)
    set_dflash_dense_reserve(reserve)
    let why = attach_dflash_drafter(m, path_join(_model_tier::models_dir(), drafter))
    restore_dflash_dense(dense_was)
    set_dflash_dense_reserve(reserve_was)
    t |> equal(why, "", "{tag}: the drafter attaches")
    return <- array<int64>() if (!empty(why))
    t |> success(dflash_on_device(), "{tag}: the pass runs on the device")
    set_dflash_record(true)
    var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    var nd = 0l
    var na = 0l
    var inscope ids <- generate_mtp_greedy(m, s, prompt, long_length(prompt) - 1l + DENSE_STEPS, nd, na)
    t |> success(nd > 0l, "{tag}: the walk drafted ({na} of {nd} accepted)")
    let walked = long_length(ids) - long_length(prompt)
    t |> success(walked >= DENSE_STEPS / 2l, "{tag}: the walk went on past the prompt ({walked} ids of {DENSE_STEPS} asked), no end of turn cutting it short")
    delete s
    grows = ring_grows(metal)
    var drafts <- dflash_recorded_drafts()
    detach_dflash_drafter()
    return <- drafts
}

//! the dense layers on the device trunk `m` (`metal`: the Metal home's) over a prompt of `items` lines past `grows_min` of the
//! drafter's windows, inside the context `ctx`: the walk whose ring grew to the context drafts id for id what a ring reserved
//! that wide drafts (the growth's copy, the layers' places); with every layer held to the window the drafts differ - the
//! control: the dense layer reads past the window
def dense_ring_leg(t : T?; m : Model; drafter, tag : string; items : int; ctx, grows_min : int64; metal : bool) {
    return if (!model_available(t, path_join(_model_tier::models_dir(), drafter)))
    let record0 = dflash_record_on()
    let device0 = dflash_device_knob()
    set_dflash_device(true)
    let text = build_string() $(var w) {
        for (i in range(items)) {
            w |> write("item {i + 1}: {(i * 7919 + 13) % 9973}\n")
        }
        w |> write("The same items again, in the same order:\nitem 1:")
    }
    let prompt <- encode(m, text, true)
    t |> success(long_length(prompt) > grows_min * DFLASH_RING && long_length(prompt) + DENSE_STEPS < ctx,
        "{tag}: the prompt of {length(prompt)} ids passes {grows_min} of the drafter's windows and fits the context of {ctx}")
    var grew = 0l
    var held = 0l
    var none = 0l
    var inscope grown <- dense_walk_drafts(t, m, drafter, "{tag}, a ring grown to the context", prompt, true, 0l, metal, grew)
    var inscope reserved <- dense_walk_drafts(t, m, drafter, "{tag}, a ring reserved for the context", prompt, true, ctx, metal, held)
    var inscope windowed <- dense_walk_drafts(t, m, drafter, "{tag}, every layer at the window", prompt, false, 0l, metal, none)
    set_dflash_record(record0)
    set_dflash_device(device0)
    t |> success(grew >= grows_min, "{tag}: the first walk's ring grew past the window ({grew} times, {grows_min} asked)")
    t |> success(held == 0l && none == 0l, "{tag}: the reserved ring and the windowed ring never grew ({held}, {none})")
    same_drafts(t, m, grown, reserved, "{tag}: the grown ring's drafts are the reserved ring's")
    var same = 0
    while (same < length(grown) && same < length(windowed) && grown[same] == windowed[same]) {
        same++
    }
    t |> success(!empty(windowed) && (same < length(grown) || same < length(windowed)),
        "{tag}: control - with every layer held to the window the drafts part from the dense reading's at draft {same}")
}

//! `dense_ring_leg` on a discrete home's trunk: a prompt past two windows inside DENSE_CTX
def discrete_dense_leg(t : T?; m : Model; drafter, tag : string) {
    dense_ring_leg(t, m, drafter, tag, DENSE_TEXT_ITEMS, DENSE_CTX, 2l, false)
}

let METAL_DENSE_ITEMS = 280   //! the Metal dense leg's prompt: past one window, inside HOST_CTX

//! the discrete homes' dense drafter form: the f16 mirror and the host-cached sessions both at DENSE_CTX, asked whole
def dense_drafter_form() : ModelForm => device_form(ctx = DENSE_CTX, strict_ctx = true) |> asking(KVDtype.f16)

//! `discrete_dense_leg` as the cell `title` on the dense drafter form `m`; a build with no discrete home's drafter arm skips it
def discrete_dense_cell(t : T?; m : Model; title, drafter, tag : string) {
    static_if (typeinfo module_exists(dasllama_vulkan_dflash) || typeinfo module_exists(dasllama_cuda_dflash)) {
        unsafe {   //! the cell captures the loaded trunk by reference
            t |> run(title) @capture(& m)(t : T ?) {
                discrete_dense_leg(t, m, drafter, tag)
            }
        }
    } else {
        skip_cell(t, "{tag}: no discrete GPU home in this build")
    }
}

//! `body` on the CPU form `m` with its sessions' context cut to HOST_CTX and the Metal mode off, both put back after it: the
//! CPU arm's drafter walks
def with_cpu_drafter_scope(var m : Model; body : block<void>) {
    let prior = get_metal_mode()
    set_metal_mode(MetalMode.off)
    with_ctx_cut(m, HOST_CTX, body)
    set_metal_mode(prior)
}

//! the Metal drivers' trunk calls: prefills, single-row decodes and batched steps (0 off the Metal build)
def metal_trunk_calls() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_prefill_stats().prefills + metal_decode_stats().decodes + metal_batch_decode_stats().steps
    } else {
        return 0l
    }
}

//! the lossless walk over the Metal-served trunk, each walk's trunk calls asserted rising on the Metal drivers
def metal_lossless_walk(t : T?; m : Model; drafter : string; device : bool; tag : string; chat : bool = false) : int64 {
    return lossless_walk(t, m, drafter, device, tag, walk_text(chat), chat) $(spec; walk) {
        let c0 = metal_trunk_calls()
        var ids <- invoke(walk)
        let c1 = metal_trunk_calls()
        t |> success(c1 > c0, "{tag}: the {spec ? "speculative" : "plain"} walk's trunk ran {c1 - c0} prefills and steps on the Metal device")
        return <- ids
    }
}

let QWEN3_4B_DEVICE_GAP = "a regular DFlash drafter's Metal device pass does not reproduce the CPU arm's drafts on this trunk - the walk parts from the plain walk at its third generated id, tokens doubled"

//! the lossless gate on the Metal device arm over the Metal-served trunk `m`, every walk over a chat turn: the device arm's
//! walk, where `joint` (a trunk the single-stream verify seat does not take, a gemma-4 shape) every block round run as the home's
//! joint round on the batch rail, and the CPU arm over the device's captures accepting the same count; a `device_gap` holds the
//! device arm and the count compare as a counted skip of their own naming it, the CPU arm's walk still lossless
def private metal_drafter_arms(t : T?; m : Model; drafter, tag : string; joint : bool; device_gap : string) {   // nolint:LINT012,LINT019 - the body is the das_metal half
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (!empty(device_gap)) {
            skip_cell(t, "{tag} device arm: {device_gap}")
            metal_lossless_walk(t, m, drafter, false, "{tag}, the CPU arm on the device's captures", true)
            detach_dflash_drafter()
            return
        }
        let rounds0 = metal_dflash_joint_rounds().rounds
        let dev = metal_lossless_walk(t, m, drafter, true, "{tag} device arm", true)
        let rounds = metal_dflash_joint_rounds().rounds - rounds0
        detach_dflash_drafter()
        if (joint) {
            t |> success(rounds > 0l, "{tag}: the block rounds ran as the home's joint rounds on the batch rail ({rounds})")
        }
        let cpu = metal_lossless_walk(t, m, drafter, false, "{tag}, the CPU arm on the device's captures", true)
        detach_dflash_drafter()
        t |> equal(dev, cpu, "{tag}: both arms accept the same count")
    }
}

//! the Metal decode and prefill drivers shut down: what a cell's leak check follows
def metal_drivers_down() {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_decode_shutdown()
        metal_prefill_shutdown()
    }
}

//! the Metal block drafter shut down, then the decode and prefill drivers (`metal_drivers_down`)
def metal_dflash_drivers_down() {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_dflash_shutdown()
    }
    metal_drivers_down()
}

//! `body` over a Metal-served trunk with the decode override Metal's, then the drivers down (`metal_drivers_down`) and
//! the override put back
def on_metal_trunk(body : block<void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let override0 = active_decode_override()
        select_decode_override("metal")
        invoke(body)
        metal_drivers_down()
        select_decode_override(override0)
    }
}

//! `on_metal_trunk` with the Metal block drafter shut down too, ahead of the other drivers
def on_metal_dflash_trunk(body : block<void>) {
    on_metal_trunk() {
        invoke(body)
        metal_dflash_drivers_down()
    }
}

//! the Metal home's drafter arms (`metal_drafter_arms`) as the cell `title` of a Metal form's trunk `m`, the form under
//! `MetalMode.required`: under the arm `arm` where one is named, with the drafter file present, the context cut to HOST_CTX; then the block
//! drafter and the decode and prefill drivers down, and no Metal object left (asserted under `tag`)
def metal_drafter_cell(t : T?; var m : Model; title, arm, drafter, tag : string; joint : bool;
                       device_gap : string = "") {
    unsafe {   //! the cell captures the loaded trunk by reference
        t |> run(title) @capture(& m)(t : T ?) {
            return if ((!empty(arm) && !arm_on(t, arm))
                || !model_available(t, path_join(_model_tier::models_dir(), drafter)))
            static_if (typeinfo builtin_module_exists(das_metal)) {
                with_ctx_cut(m, HOST_CTX) {
                    metal_drafter_arms(t, m, drafter, tag, joint, device_gap)
                }
                metal_dflash_drivers_down()
                t |> equal(metal_log_leaks_if_any(), 0l,
                    "{tag}: no leaked Metal objects after shutdown")
            }
        }
    }
}
