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
require dasllama/dasllama_image
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active / gpu_resident_prefills_: the driver's witnesses
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_: the calls the driver handed to the CPU rails, by reason
require dasllama/dasllama_kv_dtype   // kv_row_bytes / kv_layer_kbase: the head's K row
require ?vulkan dasllama/dasllama_vulkan_common   // the resolved coopmat mode, where the module exists
require ?vulkan dasllama/dasllama_vulkan_decode   // the greedy chain's mode pin and its hits and misses
require ?cuda dasllama/dasllama_cuda_decode   // ... the CUDA home's
require strings   // ends_with, slice: a Vulkan stamp name respelled as the CUDA home's census name
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_sampling   // sample_, set_seed_: the sampled speculative stream
require dasllama/dasllama_blocks   // set_moe_host_first / moe_host_first_pin: the stream twin's claim order
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 — pf_window_rows / the pins: read under the vulkan static_if, unused in the darwin world
require _compares   // argmax_of: the row's peak a red names; logits_of: the session's row copied
require _model_tier   // logits_maxdiff, mirror_kv_dtype, live_maxabs, SUPPRESSED_LOGIT
require _format_census   // census_plane, SiteCensus: a plane's tag against the file's tensor type
require dasllama/dasllama_gguf   // parse_gguf_meta
require daslib/fio   // fopen, fmap
require daslib/f16_cvt   // f16_to_f32: the stored f16 rows' distance
require math   // abs

//! the served tile arm, so a red names which prefill tile it came from
def tile_arm_tag() : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu != null ? "coopmat mode {g_gpu.coopmat_mode}" : "no vulkan device armed"   // the CUDA home leaves the Vulkan state down
    } else {
        return "no vulkan module"
    }
}

let PF_WINDOW_ROWS = 2048l   //! the prompt window the cells size their prompts against: `load_resident` pins it where no file pinned one, since the engine's own pick (the wide window where the plan fits) would run a two-window prompt as one

//! the engine's prefill window is `rows` (the default `PF_WINDOW_ROWS`, or the window a file pins), where the vulkan module exists: a cell sizing its prompt past the window holds it
def assert_pf_window(t : T?; rows : int64 = PF_WINDOW_ROWS) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (pf_window_rows() != rows) {   //! the knob or a pin moved the window: the cell's prompt is sized against `rows`, so it skips rather than reads another shape
            t |> skip("the prompt window is {pf_window_rows()} rows (DASLLAMA_GPU_PF_ROWS / set_pf_window_rows_pin), not the {rows} this cell sizes its prompt past")
            return false
        }
    }
    return true
}

//! `n` ramp ids `base + ((i + salt) * stride) % span`: a prompt or a fed run no tokenizer is needed for
def ramp_ids(n, salt, base, stride, span : int64) : array<int64> => <- [for (i in range64(n)); base + ((i + salt) * stride) % span]

//! of the row's max |logit|: the resident rigs' split bar - the window's GEMMs split k by the row count and two window splits of
//! one prompt reduce its attention in two orders, so two row counts round apart (the 0.5B's own device-vs-CPU rounding reads
//! 0.30-0.50 of a 9.5-18 max logit, `test_gpu_resident_qwen2.das`'s bar)
let SPLIT_BAR_REL = 0.06

//! the logits now, then after each of `toks` stepped one at a time on `s` at its own position (`eval_` advancing it)
def steps_after(tr : Model; var s : Session; toks : array<int64>) : array<array<float>> {
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    out |> emplace <| logits_of(s, tr.config.vocab_size)
    var one : array<int64>
    one |> resize(1)
    for (tok in toks) {
        one[0] = tok
        eval_(tr, s, one)
        out |> emplace <| logits_of(s, tr.config.vocab_size)
    }
    return <- out
}

//! a two-token prompt on `tr` from position zero takes the window chain, never the token command's peel (`rdec_peel_fits`
//! declines the model: `why`), the prefill counted and every call served
def assert_short_window_takes_chain(t : T?; tr : Model; why : string) {
    var prompt <- resident_prompt(2l)
    var s <- create_session(tr, mirror_kv_dtype())
    let lever = vulkan_prefill_peel()
    set_vulkan_prefill_peel(true)   //! the peel's lever at its default: the decline is the model's, not the lever's
    let peels0 = moe_gpu_resident_peels()
    let pf0 = gpu_resident_prefills_()
    forward_prefill(tr, s, prompt, 2l, 0l)
    set_vulkan_prefill_peel(lever)
    t |> equal(moe_gpu_resident_peels() - peels0, 0l, "a two-token prompt on {why} takes the chain, not the peel")
    t |> equal(gpu_resident_prefills_() - pf0, 1l, "... and the driver counted its prefill")
    served_every_call(t)
    delete s
    delete prompt
}

//! both sides' argmax rows decoded under `tag`, the readable form of a logits compare (`log_gen_texts` over each row's argmax)
def log_argmax_rows(tr : Model; tag : string; got, want : array<array<float>>) {
    log_gen_texts(tr, tag, [for (r in want); argmax_of(r)], [for (r in got); argmax_of(r)])
}

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

//! the CUDA home asked for (`DASLLAMA_CUDA=1` beside `DASLLAMA_GPU=1`); false after the loud skip
def cuda_home_asked(t : T?) : bool {
    if (!gpu_want_cuda()) {
        t |> skip("the CUDA home is not asked for - needs DASLLAMA_CUDA=1 beside DASLLAMA_GPU=1")
        return false
    }
    return true
}

//! `file`'s path for a cell on the CUDA home: the home asked for and the model ready (`resident_ready`), else "" after the loud skip
def cuda_model_path(t : T?; file : string) : string {
    return "" if (!cuda_home_asked(t))
    let path = path_join(models_dir(), file)
    return resident_ready(t, path) ? path : ""
}

//! the levers a resident load pins, as the cell found them: the whole-model route's lever (never `gpu_want_resident`, which
//! folds the environment's knob in and would pin the lever off under DASLLAMA_GPU_RESIDENT=0), the mirror regions asked
//! for, the context pins, the host-layers pin, the prompt-window pin, and the Vulkan fuse pin where the module exists
struct ResidentLevers {
    route : bool
    regions : int64
    ctx_max : int64
    ctx_strict : bool
    host_layers : int64
    pf_rows : int64
    fuse : int
}

def resident_levers() : ResidentLevers {
    let pins = gpu_ctx_pins()
    var l = ResidentLevers(route = gpu_resident_route_lever(), regions = gpu_resident_regions_want(), ctx_max = pins.ctx_max, ctx_strict = pins.strict,
                           host_layers = gpu_host_layers_pin(), pf_rows = pf_window_rows_pin(), fuse = -1)
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
    set_gpu_host_layers(l.host_layers)
    set_pf_window_rows_pin(l.pf_rows)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        g_vk_fuse = l.fuse
    }
}

//! the streamed prompt's levers the host-experts cells pin, as a cell found them (`restore_stream_pins`): the prompt stream, the
//! rare rail's arena, the meet's host share, the pool's DMA fill and share, the decode meet, the rare rail's one-pass and batched
//! arms, and the tier want's slot ask; a file pins the subset its cells shape and puts every lever back through the one record
struct StreamPins {
    pf_stream : int
    arena : int
    host_first : int64
    hot_dma : int
    hot_share : float
    dec_meet : int
    onepass : int
    batched : int
    heat : int64
}

def stream_pins() : StreamPins => StreamPins(pf_stream = gpu_want_pf_stream_pin(), arena = rare_arena_pin(), host_first = moe_host_first_pin(),
    hot_dma = gpu_hot_dma_pin(), hot_share = gpu_hot_share_pin(), dec_meet = gpu_dec_meet_pin(), onepass = rare_onepass_pin(), batched = rare_batched_pin(), heat = g_gpu_want.heat)

def restore_stream_pins(was : StreamPins) {
    set_gpu_want_pf_stream(was.pf_stream)
    set_rare_arena(was.arena)
    set_moe_host_first(was.host_first)
    set_gpu_hot_dma(was.hot_dma)
    set_gpu_hot_share(was.hot_share)
    set_gpu_dec_meet(was.dec_meet)
    set_rare_onepass(was.onepass)
    set_rare_batched(was.batched)
    want_heat(was.heat)
}

//! the pool's slot ask routed through the tier's want (`set_gpu_tier_want_`, `g_gpu_want` the getter): `slots` a layer asked
def want_heat(slots : int64) {
    var want = g_gpu_want
    want.heat = slots
    set_gpu_tier_want_(want)
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
    restore_gpu_kv_dtype(kv_pin)
}

//! where a cell pinned a mirror codec, the armed mirror holds it
def mirror_codec_held(t : T?) {
    let asked = gpu_kv_dtype_pin()
    if (asked.pinned) {
        t |> equal(mirror_kv_dtype(), asked.dt, "the mirror armed at the codec the cell asks")
    }
}

//! a resident load's admission: false after a skip with the driver's decline (`note` after it) where the plan does not fit
//! the card's room; else the driver asserted armed (`armed` the assert's text - any other decline reds it) and the pinned codec held
def resident_admitted(t : T?; armed, note : string) : bool {
    if (!moe_gpu_resident_active() && moe_gpu_resident_memory_decline()) {
        t |> skip("the residency plan does not fit this card ({gpu_resident_decline_()}){note}")
        return false
    }
    t |> success(moe_gpu_resident_active(), armed)
    mirror_codec_held(t)
    return true
}

//! the resident trunk at `path` (`load` the load, its levers its own): the gate (`resident_ready`), the admission
//! (`resident_admitted`, `note` after a memory decline's skip), the context cut to `ctx` - the sessions size their caches to the
//! mirror's cap, not the model's max; an empty Model (`n_layers` 0) after the skip that says why
def resident_trunk(t : T?; path : string; ctx : int64; note : string; load : block<() : Model>) : Model {
    return <- Model() if (!resident_ready(t, path))
    var tr <- invoke(load)
    if (!resident_admitted(t, "the whole-model driver armed on the model (the load log names any decline)", note)) {
        delete tr
        return <- Model()
    }
    tr.config.seq_len = min(tr.config.seq_len, ctx)
    return <- tr
}

//! holds that every layer's dense q/k/v/o and FFN planes read the file's own format (`census_plane` against `path`'s tensor
//! types: no q8 copy serves any of them) and that every layer carries tag `f` on at least one of them
def dense_planes_tagged(t : T?; tr : Model; path : string; f : KqFmt) {
    let fh = fopen(path, "rb")
    if (fh == null) {
        t |> failure("cannot open {path}")
        return
    }
    fmap(fh) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var dense = SiteCensus(site = "dense planes")
        var bare = 0
        let names = fixed_array("attn_q", "attn_k", "attn_v", "attn_output", "ffn_gate", "ffn_up", "ffn_down")
        for (l in range64(tr.config.n_layers)) {
            let tags = fixed_array(fmt_at(tr.wq_fmt, l), fmt_at(tr.wk_fmt, l), fmt_at(tr.wv_fmt, l), fmt_at(tr.wo_fmt, l), fmt_at(tr.w1_fmt, l),
                fmt_at(tr.w3_fmt, l), fmt_at(tr.w2_fmt, l))
            for (i in range(7)) {
                census_plane(dense, m, "blk.{l}.{names[i]}.weight", tags[i])
            }
            bare += tags |> has_value(f) ? 0 : 1
        }
        t |> equal(dense.copies, 0, "every dense plane of the {tr.config.n_layers} layers reads the file's own format ({dense.first})")
        t |> equal(bare, 0, "every layer carries the {f} tag on a dense plane")
    }
    fclose(fh)
}

let NO_CENSUS = -1l   //! the census count with no vulkan module: a census witness stands down

//! the coverage census's count of a kernel's dispatches so far; NO_CENSUS with no vulkan module
def census(kernel : string) : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {   // the CUDA home stamps the same stems under `_cuda`: a stamp this home has no leaf of reads NO_CENSUS
            let cuda_name = (kernel |> ends_with("_spv")) ? "{slice(kernel, 0, length(kernel) - 4)}_cuda" : kernel
            return cud_kernel_census_of(cuda_name)
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_kernel_coverage_of(kernel)
    }
    return NO_CENSUS
}

//! `path` loaded at q8 with the whole-model route pinned on - at `regions` mirror regions where it is past 0, the context
//! pinned and asked whole at `strict_ctx` positions where it is past 0, else at the caller's own context pins (its strict
//! ask included) - and `host_layers` routed layers' experts asked onto the host (0: the plan's own count, pinned so the
//! environment's ask does not reach the load) whatever an earlier cell left; every lever put back after
def load_resident(path : string; regions : int64 = 0l; strict_ctx : int64 = 0l; host_layers : int64 = 0l; image : bool = false) : Model {
    let levers = resident_levers()
    if (pf_window_rows_pin() <= 0l) {
        set_pf_window_rows_pin(PF_WINDOW_ROWS)
    }
    if (regions > 0l) {
        set_gpu_resident_regions_(regions)
    }
    if (strict_ctx > 0l) {
        set_gpu_ctx_max(strict_ctx)
        set_gpu_ctx_strict(true)
    }
    set_gpu_host_layers(host_layers)
    set_gpu_resident_route(true)
    var tr <- Model()
    if (image) {
        tr <- load_model_cached(path, QuantMode.q8)
    } else {
        tr <- load_model_(path, QuantMode.q8)
    }
    restore_resident_levers(levers)
    return <- tr
}

//! `load_resident` with the K/V mirror asked at `kv` for the load, the ask put back as it was found
def load_resident_at(path : string; kv : KVDtype; regions : int64 = 0l; strict_ctx : int64 = 0l) : Model {
    let pin = gpu_kv_dtype_pin()
    set_gpu_kv_dtype(kv)
    var tr <- load_resident(path, regions, strict_ctx)
    restore_gpu_kv_dtype(pin)
    return <- tr
}

//! the served witness: no call of the armed driver passed to the CPU rails since the model armed
def served_every_call(t : T?) {
    var passes <- gpu_cpu_passes_()
    t |> equal(length(passes), 0, "the device served every call: {passes}")
    delete passes
}

//! the CUDA driver's served-position count across the process's models (`cud_rdec_served_total`, which a model drop inside the
//! cell leaves standing); -1 off the CUDA home, or with no cuda module. A compare whose device side is a CUDA call reads it before
//! and after, so its rows are shown to have run on the CUDA driver and not on the CPU rails (`served_every_call` reads an empty
//! pass list, which a run that never reached the driver reads too)
def cuda_positions_served() : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_served_total() if (gpu_want_cuda())
    }
    return -1l
}

//! the CUDA driver's window-chain count (`cud_rdec_stats().windows`); -1 off the CUDA home, or with no cuda module
def cuda_windows_run() : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_stats().windows if (gpu_want_cuda())
    }
    return -1l
}

//! on the CUDA home, the driver served at least `n` positions since `c0` (a `cuda_positions_served` reading taken before the
//! compare's device rows): asserted under `what`; off the home (`c0` -1) nothing is asserted
def cuda_served_since(t : T?; c0, n : int64; what : string) {
    return if (c0 < 0l)
    let served = cuda_positions_served() - c0
    t |> success(served >= n, "{what}: the CUDA driver served {served} positions (at least {n} asked) - the device rows ran on the CUDA driver")
}

//! the counter's control on the CUDA home: no position served since `c0`; off the home nothing is asserted
def cuda_served_none(t : T?; c0 : int64; what : string) {
    return if (c0 < 0l)
    t |> equal(cuda_positions_served(), c0, "{what}: the CUDA driver served no position")
}

//! the counter's bounded control on the CUDA home: at most `n` positions served since `c0` (the rows the arm sends to the
//! device by design, `why`), none past them; off the home nothing is asserted
def cuda_served_at_most(t : T?; c0, n : int64; what, why : string) {
    return if (c0 < 0l)
    let served = cuda_positions_served() - c0
    t |> success(served <= n, "{what}: the CUDA driver served {served} positions, at most {n} ({why})")
}

//! how many calls the driver passed to the CPU rails for `reason` (the `RdecPass` name; "" = any reason) since the model armed
def pass_count(reason : string) : int64 {
    var passes <- gpu_cpu_passes_()
    var n = 0l
    for (p in passes) {
        if (empty(reason) || p.reason == reason) {
            n += p.count
        }
    }
    delete passes
    return n
}

//! how many calls the driver passed to the CPU rails for any reason since the model armed
def pass_total : int64 => pass_count("")

//! the driver passed calls to the CPU rails for `reason` alone since the model armed - `count` of them where past 0 - asserted
//! under `what` with every pass logged
def only_pass(t : T?; reason, what : string; count : int64 = 0l) {
    var passes <- gpu_cpu_passes_()
    t |> success(length(passes) == 1 && passes[0].reason == reason && (count <= 0l || passes[0].count == count), "{what}: {passes}")
    delete passes
}

//! the pick a device landed against the CPU logits of the same row, tie-aware: the pick is the CPU argmax, or its CPU logit sits
//! within `rel` of the CPU argmax's (a near-tie the two chains' rounding flips); both decoded and logged with their CPU logits
def pick_agrees(t : T?; tr : Model; what : string; pick : int64; cpu : array<float>; rel : float) : bool {
    let ac = argmax_of(cpu)
    let bar = row_bar(cpu, rel)
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
def draft_from(tr : Model; var s : Session; tok, row : int64; h0 : array<float>) : Draft {
    copy_floats(h0, 0l, s.mtp_h, 0l, long_length(h0))
    forward_mtp(tr, s, tok, row)
    return <- Draft(logits <- [for (x in s.logits); x], h <- [for (x in s.mtp_h); x])
}

//! `body` over a batch workspace for `tr` and the borrowed handles of `sa` and `sb`; the handles cleared before the array goes (a
//! delete of array<T?> frees the pointees), the workspace deleted after
def with_batch_rows(tr : Model; var sa, sb : Session; body : block<(var ws : BatchWorkspace; var sess : array<Session?>) : void>) {
    var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
    with_batch_rows(tr, sess, body)
    unsafe {
        delete sess
    }
}

//! `body` over a batch workspace for `tr` and the borrowed handles `sess` - any count; the handles cleared before `body`
//! returns to the caller (a delete of array<T?> frees the pointees), the workspace deleted after
def with_batch_rows(tr : Model; var sess : array<Session?>; body : block<(var ws : BatchWorkspace; var sess : array<Session?>) : void>) {
    var ws <- create_batch_workspace(tr)
    invoke(body, ws, sess)
    sess |> clear()
    delete ws
}

//! a session a prompt on codec `kv`, every session stepped `steps` batched steps on its feed's tokens: every row's logits a step,
//! the batched driver's own step count (`steps_of`) up by every step
def batched_logits_rows(t : T?; tr : Model; prompts, feeds : array<array<int64>>; kv : KVDtype; steps : int64;
                        steps_of : block<() : int64>) : array<array<float>> {
    let n = length(prompts)
    var sessions : array<Session>
    sessions |> reserve(n)
    for (p in prompts) {
        sessions |> emplace <| create_session(tr, kv)
        eval(tr, sessions[length(sessions) - 1], p)
    }
    var sess <- [for (i in range(n)); unsafe(addr(sessions[i]))]
    var rows : array<array<float>>
    rows |> reserve(int64(n) * steps)
    let steps0 = invoke(steps_of)
    with_batch_rows(tr, sess) $(var ws; var sx) {
        for (st in range64(steps)) {
            var toks <- [for (i in range(n)); feeds[i][st]]
            eval_batch(tr, ws, sx, toks)
            delete toks
            for (s in sessions) {
                rows |> push_clone(s.logits)
            }
        }
    }
    t |> equal(invoke(steps_of) - steps0, steps, "every batched step went through the batched driver")
    unsafe {
        delete sess
    }
    delete sessions
    return <- rows
}

//! speculative rounds on `s` while `more(round, committed)` holds: each round's token is `next(round)`, pushed onto `ids` with
//! the drafts the round accepted after it
def spec_rounds_into(tr : Model; var s : Session; var ids : array<int64>; more : block<(round, committed : int64) : bool>; next : block<(round : int64) : int64>) {
    var r = 0l
    while (invoke(more, r, long_length(ids))) {
        let tok = invoke(next, r)
        ids |> push(tok)
        let a = mtp_spec_round(tr, s, tok)
        for (i in range64(a)) {
            ids |> push(s.mtp_vbatch[i + 1l])   // nolint:PERF006 — the caller reserves `ids` for its rounds' worth of rows
        }
        r++
    }
}

//! a sampled speculative stream: the tokens the rounds committed, the rounds run, the drafts the walk drafted and accepted, the
//! device's selections it asked and the sampler's late landings it took
struct SampledStream {
    ids : array<int64>
    rounds : int64
    drafted : int64
    accepted : int64
    selections : int64
    late_landings : int64
}

//! a sampled speculative stream of `reply` tokens on `tr` at depth 2, seeded: `open` prefills the prompt onto the session it is
//! handed (each home's own prefill), then the first token and every round's next drawn with `sp` off the session; the
//! speculative gate and the depth put back after
def sampled_stream(tr : Model; reply : int64; var sp : SamplingParams; seed : int; kdt : KVDtype; open : block<(var s : Session) : void>) : SampledStream {
    var r : SampledStream
    r.ids |> reserve(reply + MTP_MAX_ROWS)
    var s <- create_session(tr, kdt)
    let spec0 = get_mtp_spec()
    let depth0 = mtp_depth_pin()
    set_mtp_spec(true)
    set_mtp_depth(2l)
    invoke(open, s)
    set_seed_(s, seed)
    s.spec_params = unsafe(addr(sp))
    let sel0 = sel_selections()
    let fb0 = sel_fallbacks()
    spec_rounds_into(tr, s, r.ids, $(round, committed) : bool { r.rounds = round; return committed < reply }, $(_round) => sample_(s, sp))
    r.ids |> resize(reply)
    r.selections = sel_selections() - sel0
    r.late_landings = sel_fallbacks() - fb0
    r.drafted = s.mtp_drafted
    r.accepted = s.mtp_accepted
    s.spec_params = null
    restore_mtp_depth(depth0)
    set_mtp_spec(spec0)
    delete s
    return <- r
}

//! the resident driver's sampled stream: the prompt prefilled through the engine's prefill on the mirror's codec
def sampled_stream(tr : Model; prompt : array<int64>; reply : int64; var sp : SamplingParams; seed : int) : SampledStream {
    return <- sampled_stream(tr, reply, sp, seed, mirror_kv_dtype()) $(var s : Session) {
        let np = long_length(prompt)
        forward_prefill(tr, s, prompt, np, 0l)
        s.n_past = np
    }
}

//! the selection landing's agreement, both homes' one reading: the rows-landed stream asked no selection, the selection served
//! the other's steps with no late landing, the two agree token for token (they share the device's logits and the seed and
//! part only through the sampler's input; `test_sampling_landed.das` holds the landed draw's equivalence to the row draw) and
//! the walk accepts the same drafts (a stale carry under the selection drafts garbage the walk rejects: the tokens stay alike
//! and the acceptance drops)
def selection_agrees(t : T?; tr : Model; tag : string; rows, sel : SampledStream) {
    log_gen_texts(tr, "{tag} the sampled stream: rows landed, the selection landed", rows.ids, sel.ids)
    t |> equal(rows.selections, 0l, "{tag} select: the rows-landed stream asked no selection")
    t |> success(sel.drafted > sel.rounds, "{tag} select: the rounds drafted past depth 1 ({sel.drafted} drafts over {sel.rounds} rounds)")
    t |> success(sel.selections > 0l, "{tag} select: the selection served the stream's steps ({sel.selections} selections over {sel.drafted} drafts)")
    t |> equal(sel.late_landings, 0l, "{tag} select: no step's sampler reached past the landing ({sel.late_landings} late landings)")
    t |> success(ids_equal(rows.ids, sel.ids), "{tag} select: the two streams agree token for token ({length(sel.ids)} tokens; {sel.accepted} of {sel.drafted} drafts accepted under the selection, {rows.accepted} of {rows.drafted} without)")
    t |> success(sel.accepted == rows.accepted && sel.drafted == rows.drafted, "{tag} select: the walk accepts the same drafts under the selection ({sel.accepted} of {sel.drafted} against {rows.accepted} of {rows.drafted})")
}

//! one speculative round of `tok` on `s` with its draft forced to reject, the reject cadence put back after; true where it accepted
def forced_reject_round(tr : Model; var s : Session; tok : int64) : bool {
    let reject0 = get_mtp_force_reject_every()
    set_mtp_force_reject_every(1l)
    var acc = 0l
    let hit = mtp_spec_eval(tr, s, tok, acc)
    set_mtp_force_reject_every(reject0)
    return hit
}

//! the head's K row `row` of the session's cache as bytes
def head_k_row_bytes(tr : Model; var s : Session; row : int64) : array<uint8> {
    let c = tr.config
    let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, c.n_layers))
    let kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    return <- [for (i in range64(rowb)); unsafe(kp[row * rowb + i])]
}

//! the bar a CPU row `cpu` holds a device row to: `rel` of its largest live |logit| - a suppressed id and a NaN scale no bar, so a
//! NaN in the reference row reads past the bar its finite elements set
def row_bar(cpu : array<float>; rel : float) : float => rel * live_maxabs(cpu)

//! `got` within `bar` of `want` - its largest difference, a NaN read past the bar - asserted under `what`; the difference back
def row_within(t : T?; what : string; got, want : array<float>; bar : float) : float {
    let diff = logits_maxdiff(got, want)
    t |> success(diff <= bar, "{what}: within {bar} (maxdiff {diff})")
    return diff
}

//! `row_within` with each side's argmax and its decoded piece logged under `what` first, so a red names what the rows said
def row_within_say(t : T?; tr : Model; what : string; got, want : array<float>; bar : float) : float {
    let pg = argmax_of(got)
    let pw = argmax_of(want)
    to_log(LOG_INFO, "{what}: argmax {pg} '{piece(tr, pg)}' ({got[pg]}) against {pw} '{piece(tr, pw)}' ({want[pw]})\n")
    return row_within(t, what, got, want, bar)
}

//! the control that gives a row bar its teeth: `got` against `control`, a row one token or step off, past `bars` bars and
//! finite (a NaN difference reads past every bar and proves nothing); the difference back
def control_past(t : T?; what : string; got, control : array<float>; bar : float; bars : float = 1.0) : float {
    let off = logits_maxdiff(got, control)
    let finite = is_finite(off) && off < 1e30
    t |> success(finite && off > bars * bar, "{what} control: the row one off misses by {bars} bars (maxdiff {off}, bar {bar})")
    return off
}

//! a tq4 quantizer's step is sixteen times q8_0's, and two passes a float's last bit apart land a quant apart on a rounding
//! edge: two window splits of one prompt read 0.8-2.0 apart at a 12 max logit and the split q/k norm against the fused one
//! 1.1-3.5 at 13-15, while a carrier whose rows agree bit for bit has a row one off 1.9 away. The share is card-dependent: 0.09-0.44
//! on the RTX 5060 Ti, 0.73 on the RTX PRO 4500 (900 tokens cut at 300); the bar sits halfway from that to the row one off's 1.0
let TQ4_CTRL_SHARE = 0.85

//! `got` no further from `want` than `TQ4_CTRL_SHARE` of its distance from `control`: no share of the peak holds a tq4 mirror.
//! A NaN in `got` or `want` reads an infinite difference, past every share
def tq4_within(t : T?; got, want, control : array<float>; what : string) {
    let diff = logits_maxdiff(got, want)
    let off = logits_maxdiff(got, control)
    t |> success(diff < INFINITE_DIFF && off > 0.0 && diff <= TQ4_CTRL_SHARE * off, "{what}: within {TQ4_CTRL_SHARE} of the distance to the row one off (maxdiff {diff}, the row one off {off})")
    to_log(LOG_INFO, "{what}: maxdiff {diff}, the row one off {off}\n")
}

//! every row r of `gpu` within `rel` of the largest live |value| of `cpu[r]` (`live_maxabs`: a NaN and the suppressed-id floor left out), and the control row `cpu[r + ctrl_off]` - where one exists -
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
        let bar = row_bar(cpu[r], rel)
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

//! `rows_within_control` on one row: `gpu` within `rel` of `cpu`'s scale, and `off` - the row one away - past the same bar,
//! `tail` naming the control in every message
def row_within_ctrl(t : T?; what : string; gpu, cpu, off : array<float>; rel : float; tail : string) {
    var g <- [ <- clone_to_move(gpu)]
    var c <- [ <- clone_to_move(cpu), <- clone_to_move(off)]
    rows_within_control(t, what, g, c, rel, 1l, 1.0, $(_r) => tail)
    delete g
    delete c
}

//! the prefill's logits, then each fed step's, through `s`; `after_prefill` runs between the prefill and the first step
def feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>; after_prefill : block<void>) : array<array<float>> {
    let n = long_length(prompt)
    var out : array<array<float>>
    out |> reserve(long_length(toks) + 1l)
    forward_prefill(tr, s, prompt, n, 0l)
    invoke(after_prefill)
    out |> emplace <| logits_of(s, tr.config.vocab_size)
    for (st in range64(long_length(toks))) {
        forward(tr, s, toks[st], n + st)
        out |> emplace <| logits_of(s, tr.config.vocab_size)
    }
    return <- out
}

//! the prefill's logits, then each fed step's, through `s`
def feed_logits(tr : Model; var s : Session; prompt : array<int64>; toks : array<int64>) : array<array<float>> => <- feed_logits(tr, s, prompt, toks) $ {}

//! `feed_logits` on a fresh session of codec `kv`, the session gone after
def feed_alone(tr : Model; kv : KVDtype; prompt, toks : array<int64>) : array<array<float>> {
    var s <- create_session(tr, kv)
    var rows <- feed_logits(tr, s, prompt, toks)
    delete s
    return <- rows
}

//! the first `count` ids of ordinary prose through the model's own tokenizer: on a junk prompt gemma's
//! logits swing twenty points between the CPU chain's own prefill and decode, so a bar on it measures nothing
def prose_tokens(tr : Model; count : int64) : array<int64> {
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
struct ProseFeed {
    prompt : array<int64>
    toks : array<int64>
}

def prose_feed(tr : Model; n, steps : int64) : ProseFeed {
    var ids <- prose_tokens(tr, n + steps)
    var f <- ProseFeed(prompt <- [for (i in range64(n)); ids[i]], toks <- [for (i in range64(n, n + steps)); ids[i]])
    delete ids
    return <- f
}

//! one load's run in `ring_vs_flat`: the logits rows, whether the driver armed the rings, and the mirror's bytes on the card
struct private RingRun {
    rows : array<array<float>>
    slack : int64
    mirror : int64
    window : int64
}

//! `path` loaded at mirror codec `kv` under the ring pin `ring` (`with_gpu_kv_ring`) and the session cap `ctx`, the `n` prose ids
//! and eight fed steps through it; `ok` false after the skip `resident_trunk` names
def private ring_run(t : T?; path : string; kv : KVDtype; ring : int; n, ctx : int64) : tuple<ok : bool; run : RingRun> {
    var tr <- resident_trunk(t, path, ctx, "") $() : Model {
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
        let c0 = cuda_positions_served()
        let w0 = cuda_windows_run()
        run.rows <- feed_alone(tr, kv, feed.prompt, feed.toks)
        served_every_call(t)
        let load = ring != 0 ? "the ringed load" : "the flat load"
        cuda_served_since(t, c0, n + 8l, load)
        if (w0 >= 0l) {   // the CUDA home: the prompt ran as the window chains its rows take at the pinned window
            let rows = pf_window_rows()
            t |> equal(cuda_windows_run() - w0, (n + rows - 1l) / rows, "{load}: the {n} prose ids ran as {(n + rows - 1l) / rows} window chain(s) of {rows} rows on the CUDA driver")
        }
        run.slack = gpu_resident_kv_ring_slack()
        run.mirror = gpu_resident_mirror_bytes()
        run.window = tr.config.sliding_window
    }
    moe_gpu_drop_model()
    delete tr
    return <- (ok = ok, run <- run)
}

//! The sliding layers' rings against the flat mirror on one driver: `n` prose ids past the window and eight fed steps on a ringed load
//! and a flat one (`set_gpu_kv_ring`), at mirror codec `kv` under the session cap `ctx` - the logits bit for bit, since a ring moves
//! where a row sits and never which rows a query reads or their order
def ring_vs_flat(t : T?; n : int64; path : string; kv : KVDtype; ctx : int64) {
    return if (!resident_ready(t, path))
    with_dasllama_jobque_() {
        var inscope ringed <- ring_run(t, path, kv, 1, n, ctx)
        var inscope flat <- ring_run(t, path, kv, 0, n, ctx)
        if (ringed.ok && flat.ok) {
            t |> success(n > ringed.run.window && ringed.run.window > 0l, "the {n}-token prompt runs past the {ringed.run.window}-position window")
            t |> success(ringed.run.slack > 0l && flat.run.slack == 0l, "the ringed load armed its rings (slack {ringed.run.slack}), the flat load none ({flat.run.slack})")
            t |> success(ringed.run.mirror < flat.run.mirror, "the rings charge the card less mirror ({ringed.run.mirror >> 20l} MB) than every row ({flat.run.mirror >> 20l} MB)")
            for (st in range(length(ringed.run.rows))) {
                let what = st == 0 ? "the prompt's logits" : "step {st - 1}"
                check_exact(t, "{what}: the ringed mirror against the flat one, bit for bit", ringed.run.rows[st], flat.run.rows[st])
                check_exceeds(t, "{what} control: the flat row a step off", ringed.run.rows[st], flat.run.rows[st > 0 ? st - 1 : 1], BAR_EXACT)
            }
        }
    }
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
    let served0 = gpu_resident_prefills_()
    let c0 = cuda_positions_served()
    pair.gpu <- feed_logits(tr, sg, prompt, toks, after_prefill)
    t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
    t |> equal(gpu_resident_prefills_(), served0 + 1l, "the resident window chain served the {long_length(prompt)}-token prompt")
    cuda_served_since(t, c0, long_length(prompt) + long_length(toks), "the resident feed")
    invoke(after_gpu)
    delete sg
    moe_gpu_drop_model()
    pair.cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
    return <- pair
}

//! the same with no seat between the resident prefill and its first step
def resident_then_cpu_feed(t : T?; tr : Model; prompt, toks : array<int64>; after_gpu : block<void>) : FeedPair => <- resident_then_cpu_feed(t, tr, prompt, toks, $ {}, after_gpu)

//! the stream twin's bar, of the row's largest live logit: the device's chain over its share of the routed experts against the host's
//! f32 sums over the same share reads 0.4 to 1.0 of the rows' 7-12 max logits on both homes (Qwen1.5-MoE Q8_0 and gpt-oss on the
//! 5060 Ti) and 1.30 of 8.0 on gpt-oss's CUDA prefill window on the RTX PRO 4500 (driver 595), under the CPU chain's fifth; the
//! chain's biased-act count is the form's own witness beside it
let STREAM_TWIN_BAR_REL = 0.18

//! `path` loaded twice with `host_layers` routed layers' experts on the host - the prompt stream pinned on, then off - the streamed
//! windows' rows against the host's sums' within `bar_rel` of the row's largest live logit, the device claiming first
//! (`set_moe_host_first(0)`) so the chain carries most of the rows; the pins put back, a declined model skipped
def stream_twin_within(t : T?; path : string; host_layers, n, steps : int64; bar_rel : float; tag : string) {
    let was_pf = gpu_want_pf_stream_pin()
    let was_first = moe_host_first_pin()
    set_moe_host_first(0l)
    var prompt <- resident_prompt(n)
    var toks <- resident_feed(steps)
    set_gpu_want_pf_stream(1)
    with_dasllama_jobque_() {   // the stream's readers and the model drop ask the queue, as the forced feed's does around its whole run
        var tr_on <- load_resident(path, 1l, 0l, host_layers)
        let admitted = resident_admitted(t, "the whole-model driver armed on the model with the prompt stream on (the load log names any decline)", "")
        let c0 = cuda_positions_served()
        var on <- device_rows_if(tr_on, prompt, toks, admitted)
        if (admitted) {
            cuda_served_since(t, c0, n + steps, "{tag}the streamed prompt's load")
        }
        moe_gpu_drop_model()
        delete tr_on
        if (admitted) {
            set_gpu_want_pf_stream(0)
            var tr <- load_resident(path, 1l, 0l, host_layers)
            let off_admitted = resident_admitted(t, "the whole-model driver armed on the model with the prompt stream off", "")
            var off <- device_rows_if(tr, prompt, toks, off_admitted)
            moe_gpu_drop_model()
            if (off_admitted) {
                feed_steps_within(t, tr, tag, n, on, off, toks, bar_rel, "the streamed windows", "the host's sums")
            }
            delete off
            delete tr
        }
        delete on
    }
    set_gpu_want_pf_stream(was_pf)
    set_moe_host_first(was_first)
    delete prompt
    delete toks
}

//! `prompt` then `toks` through the armed driver on a session of its own (the caller holds the job queue); no rows where `go` is off
def private device_rows_if(tr : Model; prompt, toks : array<int64>; go : bool) : array<array<float>> {
    var rows : array<array<float>>
    return <- rows if (!go)
    var sg <- create_session(tr, mirror_kv_dtype())
    var fed <- feed_logits(tr, sg, prompt, toks)
    delete sg
    return <- fed
}

//! the device's summed distance from the file's f32 chain, as a ratio of the CPU q8 chain's own: the Qwen3.5 mixed twin reads
//! 0.91 (3.42 against 3.77)
let F32_DIST_REL = 1.15

//! `prompt` then `toks` through the file at `path` loaded again at `QuantMode.fp32` on the CPU, the device state dropped: the file's
//! f32 chain, the reference where the CPU q8 chain's activation quantization rounds the device and itself to either side of it
def f32_chain_rows(path : string; prompt, toks : array<int64>) : array<array<float>> {
    var f32_model <- load_model_(path, QuantMode.fp32)
    moe_gpu_drop_model()
    var rows <- feed_alone(f32_model, mirror_kv_dtype(), prompt, toks)
    delete f32_model
    return <- rows
}

//! the same-or-better witness: the device no further from the f32 chain's rows than `ratio` of the CPU q8 chain's own
//! summed distance, the f32 rows one step off past that bound; `tr` decodes the logged picks
def f32_chain_witness(t : T?; tr : Model; what : string; fed : FeedPair; f32_rows : array<array<float>>; ratio : float = F32_DIST_REL) {
    let dist = chain_distances(t, tr, what, fed.gpu, fed.cpu, f32_rows, 0, "the f32 chain", "the CPU q8 chain")
    t |> success(dist.dev_ref <= ratio * dist.cpu_ref, "{what}: the device sits no further from the f32 chain ({dist.dev_ref}) than {ratio} of the CPU q8 chain's own distance ({dist.cpu_ref})")
    t |> success(dist.dev_ref_ctrl > ratio * dist.cpu_ref, "{what} control: the f32 rows one step off sit past that bound ({dist.dev_ref_ctrl})")
}

//! the forced-feed form: an `n`-token `resident_prompt` and `steps` fed tokens through `resident_then_cpu_feed` (`after_gpu`
//! the caller's witnesses), every row held by `feed_steps_within` at `bar_rel` under `tag`
def forced_feed_over(t : T?; tr : Model; tag : string; n, steps : int64; bar_rel : float; after_gpu : block<void>) {
    forced_feed_over(t, tr, tag, n, steps, bar_rel, 0l, after_gpu)
}

//! the same over the prompt `resident_prompt(n, salt)`
def forced_feed_over(t : T?; tr : Model; tag : string; n, steps : int64; bar_rel : float; salt : int64; after_gpu : block<void>) {
    var prompt <- resident_prompt(n, salt)
    var toks <- resident_feed(steps)
    var fed <- resident_then_cpu_feed(t, tr, prompt, toks, after_gpu)
    feed_steps_within(t, tr, tag, n, fed.gpu, fed.cpu, toks, bar_rel)
    delete fed
    delete prompt
    delete toks
}

//! every row of `gpu` (the prefill's, then each fed step's) within `bar_rel` of the CPU chain's max live |logit| of the
//! same row, and each step's one-step-off compare (the previous step's CPU row, a disconnected mechanism) past the bar;
//! both argmaxes logged under `tag` with the piece `tr` decodes them to, the two arms named `gpu_name` and `cpu_name` in every message
def feed_steps_within(t : T?; tr : Model; tag : string; nprompt : int64; gpu, cpu : array<array<float>>; toks : array<int64>; bar_rel : float; gpu_name : string = "resident"; cpu_name : string = "the CPU chain") {
    for (st in range64(long_length(cpu))) {
        let bar = row_bar(cpu[st], bar_rel)
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

//! the summed distance of `n` bytes of f16 rows in `a` from byte `a_at` to `b`'s from byte `b_at`, of `b`'s summed magnitude
def f16_rows_distance(a, b : array<uint8>; a_at, b_at, n : int64) : float {
    var dist = 0.0
    var mag = 0.0
    for (i in range64(n / 2l)) {
        let x = f16_to_f32(uint(a[a_at + 2l * i]) | (uint(a[a_at + 2l * i + 1l]) << 8u))
        let y = f16_to_f32(uint(b[b_at + 2l * i]) | (uint(b[b_at + 2l * i + 1l]) << 8u))
        dist += abs(x - y)
        mag += abs(y)
    }
    return mag > 0.0 ? dist / mag : (dist > 0.0 ? INFINITE_DIFF : 0.0)
}

let private CHAIN_STEPS = 24l   //! the greedy chain cell's fed steps a walk

//! one walk of `steps` steps after `prompt` on a fresh session under the greedy chain's mode `mode`: each step fed the last
//! step's argmax, every `off_every`-th one a token off it (0 = never); the fed tokens and every step's logits
struct ChainWalk {
    toks : array<int64>
    rows : array<array<float>>
}

//! the armed discrete home's greedy chain: whether this build and ask have one, its mode pin, the pin's setter and its counts
def chain_home : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return true if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return !gpu_want_cuda()
    }
    return false
}

def chain_pin : int {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_chain_pin() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_chain_pin()
    }
    return 0
}

def chain_mode_set(mode : int) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {
            cud_rdec_chain_mode_set(mode)
            return
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vk_rdec_chain_mode_set(mode)
    }
}

def chain_stats : tuple<hits : int64; misses : int64> {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cud_rdec_chain_stats() if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_chain_stats()
    }
    return (hits = 0l, misses = 0l)
}

def chain_walk(tr : Model; prompt : array<int64>; steps, off_every : int64; mode : int) : ChainWalk {
    var w : ChainWalk
    if (chain_home()) {
        let pin = chain_pin()
        chain_mode_set(mode)
        var s <- create_session(tr, mirror_kv_dtype())
        eval(tr, s, prompt)
        let vocab = tr.config.vocab_size
        var first <- logits_of(s, vocab)
        var tok = argmax_of(first)
        delete first
        var one : array<int64>
        one |> resize(1)
        w.toks |> reserve(steps)
        w.rows |> reserve(steps)
        for (i in range64(steps)) {
            if (off_every > 0l && i % off_every == off_every - 1l) {
                tok = (tok + 1l) % vocab
            }
            w.toks |> push(tok)
            one[0] = tok
            eval_(tr, s, one)
            var row <- logits_of(s, vocab)
            tok = argmax_of(row)
            w.rows |> emplace(row)
        }
        delete one
        delete s
        chain_mode_set(pin)
    }
    return <- w
}

//! the greedy chain (`../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain`) on the carrier at `path`, on the armed discrete home
//! (`chain_home`): a walk fed the argmax and
//! one fed a token off it every third step, each with the chain off and forced on - the chained walk's tokens and every step's
//! logits bit for bit the plain walk's, every step after the first a hit on the argmax walk, and on the other every off-pick
//! step a miss (its state put back) and every other step a hit
//! The adaptive mode (-1) runs both walks too: the same tokens and
//! logits, each miss but the last backing off the next step's chain.
def greedy_chain_cell(t : T?; path : string) {
    if (!resident_ready(t, path)) return
    var tr <- load_resident(path, 1l)
    t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the model (the load log names any decline)")
    if (chain_home()) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!gpu_want_cuda() && g_gpu.xfer_fam < 0) {   //! the chain lands its pick behind the transfer queue, which a one-family device (an iGPU) has none of
                t |> skip("the greedy chain asks a transfer queue: this device has one queue family")
                delete tr
                return
            }
        }
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(40l)
            for (off in fixed_array(0l, 3l)) {
                let c0 = cuda_positions_served()
                var plain <- chain_walk(tr, prompt, CHAIN_STEPS, off, 0)
                let st0 = chain_stats()
                var chained <- chain_walk(tr, prompt, CHAIN_STEPS, off, 1)
                let st1 = chain_stats()
                let hits = st1.hits - st0.hits
                let misses = st1.misses - st0.misses
                let offs = off > 0l ? CHAIN_STEPS / off : 0l
                let what = off > 0l ? "the walk fed off the pick every {off}rd step" : "the argmax walk"
                to_log(LOG_INFO, "{path} {what}: {hits} chained steps taken, {misses} discarded\n")
                log_gen_texts(tr, "{path} {what} (plain, chained)", plain.toks, chained.toks)
                var differ = 0
                for (i in range64(CHAIN_STEPS)) {
                    differ += chained.toks[i] != plain.toks[i] ? 1 : 0
                    t |> equal(logits_maxdiff(chained.rows[i], plain.rows[i]), 0.0, "{what}, step {i}: the chained run's logits are the plain run's bit for bit")
                }
                t |> equal(differ, 0, "{what}: the chained run's tokens are the plain run's")
                t |> equal(misses, offs, "{what}: every off-pick step discarded its chained step, and no other did")
                t |> equal(hits, CHAIN_STEPS - 1l - offs, "{what}: every other step after the first took its chained step")
                let sa0 = chain_stats()
                var adaptive <- chain_walk(tr, prompt, CHAIN_STEPS, off, -1)
                let sa1 = chain_stats()
                log_gen_texts(tr, "{path} {what} (plain, adaptive)", plain.toks, adaptive.toks)
                for (i in range64(CHAIN_STEPS)) {
                    t |> equal(adaptive.toks[i], plain.toks[i], "{what}, step {i}: adaptive token")
                    t |> equal(logits_maxdiff(adaptive.rows[i], plain.rows[i]), 0.0,
                        "{what}, step {i}: the adaptive run's logits are the plain run's")
                }
                //! the last walk's settle-dropped chain counts nothing; off every
                //! 3rd, each miss (len 0 -> 1) skips the next step, the one after
                //! hits and resets it: misses 2/5/../23, hits 1/4/../22
                let backed = offs > 0l ? offs - 1l : 0l
                t |> equal(sa1.misses - sa0.misses, offs,
                    "{what}: adaptive, every off-pick step a miss")
                t |> equal(sa1.hits - sa0.hits, CHAIN_STEPS - 1l - offs - backed,
                    "{what}: adaptive, the step after each miss but the last unchained")
                served_every_call(t)
                cuda_served_since(t, c0, 3l * CHAIN_STEPS, "{what}: the three walks")
                delete plain
                delete chained
                delete adaptive
            }
            delete prompt
        }
    } else {
        t |> skip("no discrete GPU home on this build: the greedy chain is the Vulkan and CUDA drivers'")
    }
    delete tr
}

let private PEEL_OPEN = 40l
let private PEEL_STEPS = 4l

//! after a `PEEL_OPEN`-row opening, tails of 1, 2, 5 and 8 rows as one prefill call peel through the token commands - the in-order
//! rows command past one row, every recurrent layer stepping the rows one after another in the region's slot - (a peel and a
//! prefill counted, the recurrent state at the tail's end on a recurrent model), and four fed steps read within `rel` of the
//! same tail stepped a row at a time on a second session, the row one step off the control; one row past the per-row planes
//! takes the chain, as does the eight-row tail with the peel's lever off under `lever_off` - the chain's leg over the same rows,
//! the peel counter's control; the plan charges the per-row planes at `RDEC_NROW_MAX` rows whatever the region count
def peel_cell(t : T?; var tr : Model; rel : float; tag : string; lever_off : bool) {
    let at_once = rdec_token_n_rows()
    t |> success(at_once >= 8l, "the token commands step eight rows or more at once on the model ({at_once})")
    var cap = 0l
    let plan = resident_plan_pick(tr, mirror_kv_dtype(), cap, false)
    t |> success(plan.scratch_bytes >= tr.config.vocab_size * 4l * RDEC_NROW_MAX, "the plan charges the per-row planes at {RDEC_NROW_MAX} rows whatever the region count ({plan.scratch_bytes} B of scratch)")
    let recurrent = tr.config.recr_mask != 0ul
    var opening <- resident_prompt(PEEL_OPEN)
    var feed <- resident_feed(at_once + 1l)
    let lever = vulkan_prefill_peel()
    var legs <- [1l, 2l, 5l, 8l, at_once + 1l]
    if (lever_off) {
        legs |> push(-8l)   // the eight-row tail with the lever off
    }
    for (k in legs) {
        let rows = abs(k)
        let peels = k > 0l && k <= at_once
        set_vulkan_prefill_peel(k > 0l)
        var tail <- [for (i in range64(rows)); feed[i]]
        var steps <- ramp_ids(PEEL_STEPS, rows, 2500l, 71l, 4500l)
        var sa <- create_session(tr, mirror_kv_dtype())
        eval(tr, sa, opening)
        let peels0 = moe_gpu_resident_peels()
        let pf0 = gpu_resident_prefills_()
        let c0 = cuda_positions_served()
        forward_prefill(tr, sa, tail, rows, PEEL_OPEN)   // the prefill call a scheduler's chunk takes (`eval` steps a one-token tail)
        t |> equal(moe_gpu_resident_peels() - peels0, peels ? 1l : 0l, "a tail of {rows} rows {peels ? "peels through the token commands" : k < 0l ? "takes the window chain with the lever off" : "takes the window chain"}")
        t |> equal(gpu_resident_prefills_() - pf0, 1l, "the tail of {rows} counted one served prefill")
        if (recurrent) {
            t |> equal(sa.dn_pos, PEEL_OPEN + rows, "the recurrent state stands at the tail's end")
        }
        sa.n_past = PEEL_OPEN + rows   // `forward_prefill` leaves the position to its caller
        var got <- steps_after(tr, sa, steps)
        var sb <- create_session(tr, mirror_kv_dtype())
        eval(tr, sb, opening)
        var inscope one : array<int64>
        one |> resize(1)
        for (i in range64(rows)) {
            one[0] = tail[i]
            eval_(tr, sb, one)
        }
        var want <- steps_after(tr, sb, steps)
        let what = "{tag}a tail of {rows} rows{k < 0l ? " with the lever off" : ""}"
        cuda_served_since(t, c0, rows + PEEL_STEPS, "{what}: the tail and its steps")
        rows_within_control(t, what, got, want, rel, 1l, 1.0, $(r) => r == 0l ? "the tail's last row" : "step {r}")
        log_argmax_rows(tr, "{what}: the row-at-a-time rows, then the tail's", got, want)
        served_every_call(t)
        delete sa
        delete sb
        delete got
        delete want
        delete tail
        delete steps
    }
    set_vulkan_prefill_peel(lever)
    delete legs
    delete opening
    delete feed
}

//! `peel_cell` on a recurrent carrier: the five tails, no lever-off leg
def recurrent_peel_cell(t : T?; var tr : Model; rel : float) {
    peel_cell(t, tr, rel, "", false)
}
