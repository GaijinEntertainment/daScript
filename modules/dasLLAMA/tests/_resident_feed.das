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
require dasllama/dasllama_gpu_knee   // nolint:STYLE030,LINT019 - VK_VENDOR_AMD: `home_rounds_alike`'s device fact, inside the Vulkan half
require dasllama/dasllama_kv_dtype   // kv_row_bytes / kv_layer_kbase: the head's K row
require ?vulkan dasllama/dasllama_vulkan_common   // the resolved coopmat mode, where the module exists
require ?vulkan dasllama/dasllama_vulkan_decode   // the greedy chain's mode pin and its hits and misses
require ?cuda dasllama/dasllama_cuda_decode   // ... the CUDA home's
require ?cuda dasllama/dasllama_kqformat   // KQ_FMT_COUNT: the rows leaf's knee a format
require strings   // ends_with, slice: a Vulkan stamp name respelled as the CUDA home's census name
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_sampling   // sample_, set_seed_: the sampled speculative stream
require dasllama/dasllama_blocks   // set_moe_host_first / moe_host_first_pin: the stream twin's claim order
require dasllama/dasllama_env   // nolint:STYLE030,LINT019 — pf_window_rows / the pins: read under the vulkan static_if, unused in the darwin world
require _compares   // argmax_of: the row's peak a red names; logits_of: the session's row copied
require _model_tier   // logits_maxdiff, mirror_kv_dtype, live_maxabs, SUPPRESSED_LOGIT
require _gpu_tier_armed   // gpu_tier_armed: resident_ready's armed-tier skip
require _model_form   // skip_cell: a held skip as a skipped subtest of its own
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

//! the window chains a prompt of `n` rows takes at a `rows`-row
//! prompt window
def prompt_windows(n : int64; rows : int64 = PF_WINDOW_ROWS) : int64 => (n + rows - 1l) / rows

//! the engine's prefill window is `rows` (the default `PF_WINDOW_ROWS`, or the window a file pins), where the vulkan module exists: a cell sizing its prompt past the window holds it
def assert_pf_window(t : T?; rows : int64 = PF_WINDOW_ROWS) : bool {
    let why = pf_window_why(rows)
    if (!empty(why)) {
        t |> skip(why)
        return false
    }
    return true
}

//! `assert_pf_window` held into `h`: false after the held skip
def held_pf_window(var h : Held; rows : int64 = PF_WINDOW_ROWS) : bool {
    let why = pf_window_why(rows)
    if (!empty(why)) {
        h |> held_skip(why)
        return false
    }
    return true
}

let TWO_WINDOWS = PF_WINDOW_ROWS + 88l   //! a prompt two windows long, the seam's rows past the first

//! the cells' prompts: 40 tokens, and two windows (`TWO_WINDOWS`) where the prompt window is the cells' own
//! (`held_pf_window`, its skip held into `h`)
def window_prompts(var h : Held) : array<int64> => <- held_pf_window(h) ? [40l, TWO_WINDOWS] : [40l]

//! why a cell sizing its prompt past a `rows`-row window skips: the
//! knob or a pin moved the window, so it reads another shape; ""
//! where the window is `rows` or no vulkan module reads it
def pf_window_why(rows : int64) : string {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return ("the prompt window is {pf_window_rows()} rows (DASLLAMA_GPU_PF_ROWS / " +
            "set_pf_window_rows_pin), not the {rows} this cell sizes its prompt past") if (pf_window_rows() != rows)
    }
    return ""
}

//! `n` ramp ids `base + ((i + salt) * stride) % span`: a prompt or a fed run no tokenizer is needed for
def ramp_ids(n, salt, base, stride, span : int64) : array<int64> => <- [for (i in range64(n)); base + ((i + salt) * stride) % span]

//! of the row's max |logit|: the resident rigs' split bar - the window's GEMMs split k by the row count and two window splits of
//! one prompt reduce its attention in two orders, so two row counts round apart (the 0.5B's own device-vs-CPU rounding reads
//! 0.30-0.50 of a 9.5-18 max logit, `test_gpu_resident_qwen2.das`'s bar)
let SPLIT_BAR_REL = 0.06
let private FAR_ROW_TOK = 1234l   //! the token the far row's step takes
//! ... on a device whose compiler rounds a shader at a time (`home_rounds_alike`), where the row command's residual GEMV
//! walks its K rotated and the rows command's tiles do not: the gemma-4-E2B's peel on the Radeon 8060S reads a 1-row tail bit
//! for bit, a 5-row tail under the bar above, and one step of a 2-row tail past it, the row one step off well past this
//! bar
let SPLIT_BAR_REL_APART = 0.09

//! the split bar this device's two commands hold: `SPLIT_BAR_REL`, or `SPLIT_BAR_REL_APART` where they round apart
def split_bar_rel() : float => home_rounds_alike() ? SPLIT_BAR_REL : SPLIT_BAR_REL_APART

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
    return gpu_tier_armed(t)
}

//! why the run arms no CUDA home; "" where it asks for one
//! (`DASLLAMA_CUDA=1` beside `DASLLAMA_GPU=1`)
def cuda_home_why() : string {
    return "" if (gpu_want_cuda())
    return "the CUDA home is not asked for - needs DASLLAMA_CUDA=1 beside DASLLAMA_GPU=1"
}

//! why a cell the Vulkan home alone carries (`what`, the cell's reason) does not run: the CUDA home is asked; "" where it
//! is not
def vulkan_home_why(what : string) : string => gpu_want_cuda() ? "the CUDA home is asked: {what}" : ""

//! the Vulkan home's own cell runs (`vulkan_home_why`); false after the loud skip
def vulkan_home_asked(t : T?; what : string) : bool {
    let why = vulkan_home_why(what)
    if (!empty(why)) {
        t |> skip(why)
        return false
    }
    return true
}

//! the CUDA home asked for (`cuda_home_why`); false after the loud
//! skip
def cuda_home_asked(t : T?) : bool {
    let why = cuda_home_why()
    if (!empty(why)) {
        t |> skip(why)
        return false
    }
    return true
}

//! the home ask's value a red names, where a cell picks its bar or
//! its expected value by the home
def cuda_knob() : string => "DASLLAMA_CUDA={gpu_want_cuda() ? 1 : 0}"

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
    block_rounds : bool
}

def resident_levers() : ResidentLevers {
    let pins = gpu_ctx_pins()
    var l = ResidentLevers(route = gpu_resident_route_lever(), regions = gpu_resident_regions_want(), ctx_max = pins.ctx_max, ctx_strict = pins.strict,
                           host_layers = gpu_host_layers_pin(), pf_rows = pf_window_rows_pin(), fuse = -1,
                           block_rounds = gpu_resident_block_rounds_want())
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
    set_gpu_resident_block_rounds_(l.block_rounds)
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

//! the streamed prompt's pins a host-experts cell takes, the levers
//! as found returned (`restore_stream_pins`): the meet's host share
//! `host_first`, the prompt stream and the rare rail's arena where
//! `pf_stream` and `arena` are 0 or 1, and under `plain_steps` the
//! fed steps on the plain rail - the pool's DMA fill, no decode meet,
//! no heat slot
def pin_stream(host_first : int64; pf_stream : int = -1; arena : int = -1;
               plain_steps : bool = true) : StreamPins {
    let was = stream_pins()
    if (pf_stream >= 0) {
        set_gpu_want_pf_stream(pf_stream)
    }
    if (arena >= 0) {
        set_rare_arena(arena)
    }
    set_moe_host_first(host_first)
    if (plain_steps) {
        set_gpu_hot_dma(1)
        set_gpu_dec_meet(0)
        want_heat(0l)
    }
    return was
}

//! `body` with the K/V mirror asked at `kv`, the ask put back as it was found - the pin for a rig that loads its own model
//! and makes its sessions on the armed codec; a skip where the environment arms another codec over the ask
def with_gpu_kv_dtype(t : T?; kv : KVDtype; body : block<void>) {
    let kv_pin = gpu_kv_want_pin()
    set_gpu_kv_dtype(kv)
    if (resident_mirror_dtype() == kv) {
        invoke(body)
    } else {
        t |> skip("the environment arms a {resident_mirror_dtype()} mirror over the {kv} asked (DASLLAMA_VK_KV32=1) - the cell needs that unset")
    }
    restore_gpu_kv_want(kv_pin)
}

//! where a cell pinned a mirror codec, the armed mirror holds it
def mirror_codec_held(t : T?) {
    held_on(t) $(var h) {
        mirror_codec_held(h)
    }
}

//! `mirror_codec_held` read now, held into `h`
def mirror_codec_held(var h : Held) {
    let asked = gpu_kv_want_pin()
    if (asked.pinned) {
        h |> held_equal(mirror_kv_dtype(), asked.value,
            "the mirror armed at the codec the cell asks")
    }
}

//! `mirror_codec_held` on the model `tr`: the codec the cell asks as `tr` serves it (`kv_codec_for` - a Q/K/V-biased model
//! serves a tq4 ask at q8_0, the serving rule)
def mirror_codec_held(var h : Held; tr : Model) {
    let asked = gpu_kv_want_pin()
    if (asked.pinned) {
        h |> held_equal(mirror_kv_dtype(), kv_codec_for(tr.config, asked.value),
            "the mirror armed at the codec the cell asks, as the model serves it")
    }
}

//! a resident load's admission: false after a skip with the driver's decline (`note` after it) where the plan does not fit
//! the card's room; else the driver asserted armed (`armed` the assert's text - any other decline reds it) and the pinned codec held
def resident_admitted(t : T?; armed, note : string) : bool {
    return false if (!resident_fits(t, armed, note))
    mirror_codec_held(t)
    return true
}

//! `resident_admitted` on the model `tr` the load made: the pinned codec held as `tr` serves it (`mirror_codec_held(h, tr)`)
def resident_admitted(t : T?; armed, note : string; tr : Model) : bool {
    return false if (!resident_fits(t, armed, note))
    held_on(t) $(var h) {
        mirror_codec_held(h, tr)
    }
    return true
}

//! the admission's skip where the plan does not fit the card's room, else the driver asserted armed
def private resident_fits(t : T?; armed, note : string) : bool {
    if (!moe_gpu_resident_active() && moe_gpu_resident_memory_decline()) {
        t |> skip("the residency plan does not fit this card ({gpu_resident_decline_()}){note}")
        return false
    }
    t |> success(moe_gpu_resident_active(), armed)
    return true
}

//! the resident trunk at `path` (`load` the load, its levers its own): the gate (`resident_ready`), the admission
//! (`resident_admitted`, `note` after a memory decline's skip), the context cut to `ctx` - the sessions size their caches to the
//! mirror's cap, not the model's max; an empty Model (`n_layers` 0) after the skip that says why
def resident_trunk(t : T?; path : string; ctx : int64; note : string; load : block<() : Model>) : Model {
    return <- Model() if (!resident_ready(t, path))
    var tr <- invoke(load)
    if (!resident_admitted(t, "the whole-model driver armed on the model (the load log names any decline)", note, tr)) {
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

//! the coverage census's count of a kernel's dispatches so far - on the Vulkan home a class slot's, under the kernel's own
//! name and under the stamp its pipe slot serves on this device where that is another (a small-slab twin, or an alternate of
//! the box's set: `vkd_slot_kernel`), so a count holds whatever set serves; NO_CENSUS with no vulkan module
def census(kernel : string) : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {   // the CUDA home stamps the same stems under `_cuda`: a stamp this home has no leaf of reads NO_CENSUS
            let cuda_name = (kernel |> ends_with("_spv")) ? "{slice(kernel, 0, length(kernel) - 4)}_cuda" : kernel
            return cud_kernel_census_of(cuda_name)
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let own = vk_kernel_coverage_of(kernel)
        let served = g_gpu != null ? vkd_slot_kernel(kernel) : kernel
        return served == kernel ? own : own + vk_kernel_coverage_of(served)
    }
    return NO_CENSUS
}

//! the census count `now` reads past `c0`, the count before the run; NO_CENSUS where `c0` stood down
def census_delta(c0, now : int64) : int64 => c0 == NO_CENSUS ? NO_CENSUS : now - c0

//! `kernel`'s census count past `c0`, the count before the run (`census_delta`)
def census_since(kernel : string; c0 : int64) : int64 => census_delta(c0, census(kernel))

//! the summed census count so far of the stamps `keys` on the armed home, or NO_CENSUS (a stamp the home has no leaf of reads none)
def stamps_census(keys : array<string>) : int64 {
    var n = 0l
    for (k in keys) {
        let c = census(k)
        return NO_CENSUS if (c == NO_CENSUS && !gpu_want_cuda())
        n += max(c, 0l)
    }
    return n
}

//! the census names of the stamps of low-bit member `f` (tq2, tq1, iq1s, iq1m): the decode GEMV and its N-column leaves, the
//! batch tiles; tq2's rotated N-column leaves beside them, and the 1-bit grid members' cm2e tile
def private lowbit_stamps(f : KqFmt) : array<string> {
    var keys <- ["kq_gemv_{f}_cls_spv", "kq_gemv_{f}_n_cls_spv", "kq_gemv_{f}_n4_cls_spv", "kq_batch_{f}_cls_spv",
        "kq_batch_{f}_cm2l_cls_spv", "kq_batch_{f}_cm2m_cls_spv", "kq_batch_{f}_cm2s_cls_spv", "kq_batch_{f}_khr_cls_spv"]
    if (f == KqFmt.tq2) {
        keys |> push_from(["kq_gemv_tq2_nr_cls_spv", "kq_gemv_tq2_n4r_cls_spv"])
    } elif (f == KqFmt.iq1s || f == KqFmt.iq1m) {
        keys |> push("kq_batch_{f}_cm2e_cls_spv")
    }
    return <- keys
}

//! every stamp's census count so far of low-bit member `f` on the armed home, or NO_CENSUS: no other plane format moves it, so a
//! rise is the member's planes' own
def lowbit_census(f : KqFmt) : int64 {
    var inscope keys <- lowbit_stamps(f)
    return stamps_census(keys)
}

//! whether the armed home counts its stamps' dispatches: the CUDA home on a dasCUDA build, any other home on a dasVulkan build
def home_has_census() : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return true if (gpu_want_cuda())
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return true
    }
    return false
}

//! `run`, one form's share of a cell on a carrier of low-bit member `f` (`what` naming the carrier), under the member's census:
//! on a device form the stamps rose; on a CPU form, the control, they stayed put; neither where the home counts no stamp
def lowbit_census_ran(t : T?; f : KqFmt; form : ModelForm; what : string; run : block<void>) {
    let c0 = lowbit_census(f)
    invoke(run)
    return if (!home_has_census())
    let ran = lowbit_census(f) - c0
    to_log(LOG_INFO, "{what}, {form_name(form)}: the {f} stamps ran {ran} dispatches\n")
    if (form.on_gpu) {
        t |> success(ran > 0l, "{what}: the device form ran the {f} stamps ({ran} dispatches)")
    } else {
        t |> equal(ran, 0l, "{what} control: the CPU form {form_name(form)} ran no {f} stamp")
    }
}

//! how a resident cell's model loads: `served` is the product's load with no image (on a discrete home the gguf streams onto
//! the card and no weight plane stays on the host), `host` keeps the planes beside the device's copy, `image` takes the rail
enum ResidentLoad {
    served
    host
    image
}

//! `path` loaded at q8 with the whole-model route pinned on - at `regions` mirror regions where it is past 0, the context
//! pinned and asked whole at `strict_ctx` positions where it is past 0, else at the caller's own context pins (its strict
//! ask included) - and `host_layers` routed layers' experts asked onto the host (0: the plan's own count, pinned so the
//! environment's ask does not reach the load) whatever an earlier cell left; every lever put back after
def private load_resident_by(path : string; regions, strict_ctx, host_layers : int64; how : ResidentLoad) : Model {
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
    if (how == ResidentLoad.image) {
        tr <- load_model_cached(path, QuantMode.q8)
    } elif (how == ResidentLoad.host) {
        tr <- load_model_(path, QuantMode.q8)
    } else {
        tr <- load_model_uncached(path, QuantMode.q8)
    }
    restore_resident_levers(levers)
    return <- tr
}

//! the resident load of a cell whose every compare is the device's: the model as the product loads it with no image
def load_resident(path : string; regions : int64 = 0l; strict_ctx : int64 = 0l; host_layers : int64 = 0l) : Model =>
    <- load_resident_by(path, regions, strict_ctx, host_layers, ResidentLoad.served)

//! the resident load through the image rail, as a load with the rail on takes it: the file's device lane mapped - minted
//! beside the file where none stands - and, under the home's trim, no host copy of the weight planes
def load_resident_lane(path : string; regions : int64 = 0l) : Model =>
    <- load_resident_by(path, regions, 0l, 0l, ResidentLoad.image)

//! the resident load of a cell that runs the CPU chain on the same model, or reads its planes: the weight planes stay on the
//! host beside the device's copy (`image`: through the image rail instead, as the run's levers leave the lane)
def load_resident_host(path : string; regions : int64 = 0l; strict_ctx : int64 = 0l; host_layers : int64 = 0l; image : bool = false) : Model =>
    <- load_resident_by(path, regions, strict_ctx, host_layers, image ? ResidentLoad.image : ResidentLoad.host)

//! `load_resident_host` where `host`, else `load_resident`: a rig whose cells split on whether they read the host's copy
def load_resident_as(host : bool; path : string; regions : int64 = 0l; strict_ctx : int64 = 0l;
                     host_layers : int64 = 0l) : Model =>
    <- load_resident_by(path, regions, strict_ctx, host_layers,
        host ? ResidentLoad.host : ResidentLoad.served)

//! `load_resident`, or `load_resident_host` under `host`, with the K/V mirror asked at `kv` for the load, the ask put back
def load_resident_at(path : string; kv : KVDtype; regions : int64 = 0l; strict_ctx : int64 = 0l; host : bool = false) : Model {
    let pin = gpu_kv_want_pin()
    set_gpu_kv_dtype(kv)
    var tr <- load_resident_by(path, regions, strict_ctx, 0l, host ? ResidentLoad.host : ResidentLoad.served)
    restore_gpu_kv_want(pin)
    return <- tr
}

//! the served witness: no call of the armed driver passed to the CPU rails since the model armed
def served_every_call(t : T?) {
    held_on(t) $(var h) {
        h |> held_served()
    }
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
    cuda_served_within(t, cuda_positions_served() - c0, n, what)
}

//! what a CUDA served-count check says under `what`: the `served` positions against the `n` asked
def private cuda_served_text(what : string; served, n : int64) : string
    => "{what}: the CUDA driver served {served} positions (at least {n} asked) - the device rows ran on the CUDA driver"

//! a stored count of positions the CUDA driver served is at least the `n` asked: asserted under `what`
def cuda_served_within(t : T?; served, n : int64; what : string) {
    t |> success(served >= n, cuda_served_text(what, served, n))
}

//! `cuda_served_since` read now into `h`: off the home (`c0` -1) nothing is held
def held_cuda_served(var h : Held; c0, n : int64; what : string) {
    return if (c0 < 0l)
    let served = cuda_positions_served() - c0
    h |> held_fact(served >= n, cuda_served_text(what, served, n))
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
    held_on(t) $(var h) {
        h |> held_only_pass(reason, what, count)
    }
}

//! the pick a device landed against the CPU logits of the same row, tie-aware: the pick is the CPU argmax, or its CPU logit sits
//! within `rel` of the CPU argmax's (a near-tie the two chains' rounding flips); both decoded and logged with their CPU logits
def pick_agrees(t : T?; tr : Model; what : string; pick : int64; cpu : array<float>; rel : float) : bool =>
    pick_agrees(t, what, pick, cpu, rel) $(id) => decode_(tr, [id])

//! `pick_agrees` with no model at hand: `piece` decodes the logged ids
def pick_agrees(t : T?; what : string; pick : int64; cpu : array<float>; rel : float;
                piece : block<(id : int64) : string>) : bool {
    let ac = argmax_of(cpu)
    let bar = row_bar(cpu, rel)
    let in_bar = pick >= 0l && pick < long_length(cpu) && cpu[pick] >= cpu[ac] - bar
    to_log(LOG_INFO, "{what}: device pick {pick} '{invoke(piece, pick)}' (CPU logit {pick >= 0l && pick < long_length(cpu) ? cpu[pick] : 0.0}), CPU argmax {ac} '{invoke(piece, ac)}' ({cpu[ac]}), tie bar {bar}\n")
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
    var inscope h : Held
    var rows <- batched_logits_rows(h, tr, prompts, feeds, kv, steps, steps_of)
    held_within(t, h)
    return <- rows
}

//! `batched_logits_rows` with its step count held for the asserts after a model's block
def batched_logits_rows(var h : Held; tr : Model; prompts, feeds : array<array<int64>>; kv : KVDtype; steps : int64;
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
    h |> held_equal(invoke(steps_of) - steps0, steps, "every batched step went through the batched driver")
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
//! part only through the sampler's input; `test_mtp_sampled_walk.das` holds the landed draw's equivalence to the row draw) and
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

//! the head's K row `row` of the session's cache overwritten with one byte value, and that row as bytes: the baseline of a
//! cell that proves a later store wrote the row - a row no store wrote holds whatever its memory held before
def head_k_row_marked(tr : Model; var s : Session; row : int64) : array<uint8> {
    let c = tr.config
    let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, c.n_layers))
    var kp = kv_layer_kbase(s, c.n_layers, c.seq_len)
    for (i in range64(rowb)) {
        unsafe(kp[row * rowb + i]) = 0xA5u8
    }
    return <- head_k_row_bytes(tr, s, row)
}

//! whether the armed home rounds two programs of one float expression alike - two shaders, or a shader and the CPU: false on
//! an AMD Vulkan device, whose compiler contracts and re-associates a shader's arithmetic one shader at a time (the kernel
//! cells' `vk_rounds_alike`), so a model cell that is bit for bit elsewhere holds a bar with its control there
def home_rounds_alike() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu == null || g_gpu.vendor != VK_VENDOR_AMD
    } else {
        return true
    }
}

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

//! the launches of the CUDA home's rows tensor leaf stamps so far; -1 where the armed home has no rows leaf
def home_rows_leaf_launches : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {
            var names <- cud_kernel_names()
            var n = 0l
            for (name in names) {
                if (find(name, "_rows_") >= 0 && find(name, "mma") >= 0) {
                    n += int64(cud_kernel_coverage_of(name))
                }
            }
            delete names
            return n
        }
    }
    return -1l
}

//! `body` with the CUDA home's rows tensor leaf pinned - on: every format's knee written back as its getter read it; off:
//! every knee at 0, the N-column GEMVs at every row count - each knee put back as found; `body` alone on a home with no
//! rows leaf
def with_rows_leaf(on : bool; body : block<() : void>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {
            var found <- [for (f in range(KQ_FMT_COUNT)); cuda_rows_mma_min(f)]
            for (f in range(KQ_FMT_COUNT)) {
                set_cuda_rows_mma_min(on ? found[f] : 0l, f)
            }
            invoke(body)
            for (f in range(KQ_FMT_COUNT)) {
                set_cuda_rows_mma_min(found[f], f)
            }
            delete found
            return
        }
    }
    invoke(body)
}

//! `got` nearer to `want` than to `control`, the row one off: no share of the peak holds a tq4 mirror, and no share of the
//! control's distance holds on every card. A NaN in `got` or `want` reads an infinite difference, past the control
def tq4_within(t : T?; got, want, control : array<float>; what : string) {
    let diff = logits_maxdiff(got, want)
    let off = logits_maxdiff(got, control)
    t |> success(diff < INFINITE_DIFF && off > 0.0 && diff < off, "{what}: nearer than the row one off (maxdiff {diff}, the row one off {off})")
    to_log(LOG_INFO, "{what}: maxdiff {diff}, the row one off {off}\n")
}

//! `got` against `want` under a tq4 mirror where the two forms split a prompt's windows apart: the difference is the 4-bit
//! codec's own noise - 0.06-0.86 of the row one off's distance on the Vulkan home of an RTX PRO 6000, 1.03 on its CUDA home
//! for a 42-token prompt cut at 40 - so the reading is logged and only a NaN reds it
def tq4_reported(t : T?; got, want, control : array<float>; what : string) {
    let diff = logits_maxdiff(got, want)
    let off = logits_maxdiff(got, control)
    t |> success(diff < INFINITE_DIFF, "{what}: finite (maxdiff {diff}; reported, the two forms split apart under a 4-bit mirror)")
    to_log(LOG_INFO, "{what}: maxdiff {diff}, the row one off {off} (reported)\n")
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

//! one ring load's run (`ring_rows`): the logits rows, the positions
//! and window chains the CUDA driver served for
//! them (-1 off the CUDA home) at the prompt window `pf_rows`, whether the driver armed the rings, the mirror's bytes on the card
struct RingRun {
    rows : array<array<float>>
    cuda : int64
    windows : int64
    pf_rows : int64
    slack : int64
    mirror : int64
    window : int64
}

//! `n` prose ids and eight fed steps through the armed driver on a session of codec `kv`, and what the load armed for them
def ring_rows(tr : Model; kv : KVDtype; n : int64) : RingRun {
    var run : RingRun
    var inscope feed <- prose_feed(tr, n, 8l)
    let c0 = cuda_positions_served()
    let w0 = cuda_windows_run()
    run.rows <- feed_alone(tr, kv, feed.prompt, feed.toks)
    run.cuda = c0 >= 0l ? cuda_positions_served() - c0 : -1l
    run.windows = w0 >= 0l ? cuda_windows_run() - w0 : -1l
    run.pf_rows = pf_window_rows()
    run.slack = gpu_resident_kv_ring_slack()
    run.mirror = gpu_resident_mirror_bytes()
    run.window = tr.config.sliding_window
    return <- run
}

//! one ring load's CUDA witnesses: the driver served the `n` prose
//! ids and eight steps, the prompt as the window chains its rows take
//! at the pinned window
def private ring_served_within(t : T?; tag, load : string; run : RingRun; n : int64) {
    if (run.cuda >= 0l) {
        cuda_served_within(t, run.cuda, n + 8l, "{tag}{load}")
    }
    if (run.windows >= 0l) {
        let windows = prompt_windows(n, run.pf_rows)
        t |> equal(run.windows, windows,
            "{tag}{load}: the {n} prose ids ran as {windows} window chain(s) of {run.pf_rows} " +
            "rows on the CUDA driver")
    }
}

//! The ringed load's run against the flat load's (`ring_rows` on each): the `n` prose ids past the window, the rings armed on the
//! ringed load alone and charging the card less mirror, the logits bit for bit - a ring moves where a row sits and never which
//! rows a query reads or their order - each row's control the flat row a step off; `tag` opens each message
def ring_runs_within(t : T?; tag : string; n : int64; ringed, flat : RingRun) {
    ring_served_within(t, tag, "the ringed load", ringed, n)
    ring_served_within(t, tag, "the flat load", flat, n)
    t |> success(n > ringed.window && ringed.window > 0l, "{tag}the {n}-token prompt runs past the {ringed.window}-position window")
    t |> success(ringed.slack > 0l && flat.slack == 0l, "{tag}the ringed load armed its rings (slack {ringed.slack}), the flat load none ({flat.slack})")
    t |> success(ringed.mirror < flat.mirror, "{tag}the rings charge the card less mirror ({ringed.mirror >> 20l} MB) than every row ({flat.mirror >> 20l} MB)")
    for (st in range(length(ringed.rows))) {
        let what = st == 0 ? "the prompt's logits" : "step {st - 1}"
        check_exact(t, "{tag}{what}: the ringed mirror against the flat one, bit for bit", ringed.rows[st], flat.rows[st])
        check_exceeds(t, "{tag}{what} control: the flat row a step off", ringed.rows[st], flat.rows[st > 0 ? st - 1 : 1], BAR_EXACT)
    }
}

//! one mirror codec's ring runs (`ring_rows`): the ringed load's
//! and the flat load's, each `_ran` where its form collected
struct RingPair {
    ringed : RingRun
    flat : RingRun
    ringed_ran : bool
    flat_ran : bool
}

//! a block's ring runs on two mirror codecs: the first (`main`) and
//! the second (`other`), each ringed and flat
struct RingRows {
    main : RingPair
    other : RingPair
}

//! one load's run into `p`, its ringed or its flat slot by `ringed`
def private ring_pair_into(var p : RingPair; m : Model; kv : KVDtype; ringed : bool; ids : int64) {
    if (ringed) {
        delete p.ringed
        p.ringed <- ring_rows(m, kv, ids)
        p.ringed_ran = true
    } else {
        delete p.flat
        p.flat <- ring_rows(m, kv, ids)
        p.flat_ran = true
    }
}

//! one ring form's `ids` prose ids into `r`: the `other` codec's pair
//! or the main one's, its ringed or its flat run by `ringed`
def ring_rows_into(var r : RingRows; m : Model; kv : KVDtype; other, ringed : bool; ids : int64) {
    if (other) {
        ring_pair_into(r.other, m, kv, ringed, ids)
    } else {
        ring_pair_into(r.main, m, kv, ringed, ids)
    }
}

def private ring_pair_within(t : T?; tag : string; ids : int64; p : RingPair; kv : KVDtype) {
    if (p.ringed_ran && p.flat_ran) {
        ring_runs_within(t, "{tag}, the rings on a {kv} mirror: ", ids, p.ringed, p.flat)
    }
}

//! the ring cells' compares under `tag`, each codec's ringed run
//! against its flat where both ran - `main_kv` then `other_kv`;
//! inside the job queue
def rings_within(t : T?; tag : string; ids : int64; r : RingRows; main_kv, other_kv : KVDtype) {
    ring_pair_within(t, tag, ids, r.main, main_kv)
    ring_pair_within(t, tag, ids, r.other, other_kv)
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
//! chain's biased-act count is the form's own witness beside it; on the Qwen1.5-MoE four-block cuts on the Vulkan home (RTX
//! PRO 4500) the worst row reads 0.373 against a 2.13 bar (Q4_K_M) and 0.205 against 2.14 (Q8_0), the one-step-off control
//! 15.2 at least
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

//! the same-or-better witness: the device's rows `gpu` no further from the f32 chain's rows than `ratio` of the CPU q8 chain's
//! (`cpu`) own summed distance, the f32 rows one step off past that bound; `piece` decodes the logged picks
def f32_chain_witness(t : T?; what : string; gpu, cpu, f32_rows : array<array<float>>; ratio : float; piece : block<(id : int64) : string>) {
    let dist = chain_distances(t, what, gpu, cpu, f32_rows, 0, "the f32 chain", "the CPU q8 chain", piece)
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
def feed_steps_within(t : T?; tag : string; nprompt : int64; gpu, cpu : array<array<float>>; toks : array<int64>; bar_rel : float; gpu_name, cpu_name : string; piece : block<(id : int64) : string>) {
    for (st in range64(long_length(cpu))) {
        let bar = row_bar(cpu[st], bar_rel)
        let ag = argmax_of(gpu[st])
        let ac = argmax_of(cpu[st])
        let what = tag + (st == 0l ? "prefill ({nprompt} tokens)" : "step {st - 1l} (fed {toks[st - 1l]})") + " [{tile_arm_tag()}]"
        let diff = row_within(t, "{what} logits, {gpu_name} against {cpu_name}", gpu[st], cpu[st], bar)
        to_log(LOG_INFO, "{what}: argmax cpu {ac} '{invoke(piece, ac)}' ({cpu[st][ac]}), {gpu_name} {ag} '{invoke(piece, ag)}' ({gpu[st][ag]}); logits maxdiff {diff} (bar {bar})\n")
        let ctrl = st > 0l ? st - 1l : 1l   // the row one step off: the step before, or the step after for the prompt's row
        if (ctrl < long_length(cpu)) {
            let off = control_past(t, "{what} one-step-off", gpu[st], cpu[ctrl], bar)
            to_log(LOG_INFO, "{what}: one-step-off control maxdiff {off} (bar {bar})\n")
        }
    }
}

//! `feed_steps_within` with every argmax decoded to the piece `tr` reads it as
def feed_steps_within(t : T?; tr : Model; tag : string; nprompt : int64; gpu, cpu : array<array<float>>; toks : array<int64>; bar_rel : float; gpu_name : string = "resident"; cpu_name : string = "the CPU chain") {
    feed_steps_within(t, tag, nprompt, gpu, cpu, toks, bar_rel, gpu_name, cpu_name) $(id) => decode_(tr, [id])
}

//! `feed_steps_within` with no model at hand: the argmaxes by their ids
def feed_steps_within(t : T?; tag : string; nprompt : int64; gpu, cpu : array<array<float>>; toks : array<int64>; bar_rel : float; gpu_name : string = "resident"; cpu_name : string = "the CPU chain") {
    feed_steps_within(t, tag, nprompt, gpu, cpu, toks, bar_rel, gpu_name, cpu_name) $(_id) => ""
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
def chain_distances(t : T?; tr : Model; what : string; gpu, cpu, ref_chain : array<array<float>>; first : int; ref_name : string = "f16"; cpu_name : string = "the CPU codec chain") : ChainDistances =>
    chain_distances(t, what, gpu, cpu, ref_chain, first, ref_name, cpu_name) $(id) => decode_(tr, [id])

//! `chain_distances` with `piece` decoding the logged argmaxes
def chain_distances(t : T?; what : string; gpu, cpu, ref_chain : array<array<float>>; first : int; ref_name, cpu_name : string; piece : block<(id : int64) : string>) : ChainDistances {
    var dist : ChainDistances
    for (st in range(first, length(ref_chain))) {
        let off = st > 0 ? st - 1 : 1
        let dd = logits_maxdiff(gpu[st], ref_chain[st])
        let dc = logits_maxdiff(cpu[st], ref_chain[st])
        let dp = logits_maxdiff(gpu[st], cpu[st])
        let af = argmax_of(ref_chain[st])
        let ac = argmax_of(cpu[st])
        let ag = argmax_of(gpu[st])
        to_log(LOG_INFO, "{what}, row {st}: the device {dd} from {ref_name}, {cpu_name} {dc}, the two {dp} apart; argmax {ref_name} {af} '{invoke(piece, af)}', {cpu_name} {ac} '{invoke(piece, ac)}', device {ag} '{invoke(piece, ag)}'\n")
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

//! the greedy chain (`../ARCHITECTURE_GPU_VULKAN_LANDING.md#vk-greedy-chain`) on `tr`, armed at one mirror region, on the armed
//! discrete home (`chain_home`), inside the engine's job queue: a walk fed the argmax and one fed a token off it every third step,
//! each with the chain off and forced on - the chained walk's tokens and every step's logits bit for bit the plain walk's, every
//! step after the first a hit on the argmax walk, and on the other every off-pick step a miss (its state put back) and every
//! other step a hit. The adaptive mode (-1) runs both walks too: the same tokens and logits, each miss but the last backing off
//! the next step's chain. Its asserts held
def greedy_chain_rows(var h : Held; tr : Model; path : string) {
    if (!chain_home()) {
        h |> held_skip("no discrete GPU home on this build: the greedy chain is the Vulkan and CUDA drivers'")
        return
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!gpu_want_cuda() && g_gpu.xfer_fam < 0) {   //! the chain lands its pick behind the transfer queue, which a one-family device (an iGPU) has none of
            h |> held_skip("the greedy chain asks a transfer queue: this device has one queue family")
            return
        }
    }
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
            h |> held_same("{what}, step {i}: the chained run's logits are the plain run's", chained.rows[i], plain.rows[i])
        }
        h |> held_equal(differ, 0, "{what}: the chained run's tokens are the plain run's")
        h |> held_equal(misses, offs, "{what}: every off-pick step discarded its chained step, and no other did")
        h |> held_equal(hits, CHAIN_STEPS - 1l - offs, "{what}: every other step after the first took its chained step")
        let sa0 = chain_stats()
        var adaptive <- chain_walk(tr, prompt, CHAIN_STEPS, off, -1)
        let sa1 = chain_stats()
        log_gen_texts(tr, "{path} {what} (plain, adaptive)", plain.toks, adaptive.toks)
        for (i in range64(CHAIN_STEPS)) {
            h |> held_equal(adaptive.toks[i], plain.toks[i], "{what}, step {i}: adaptive token")
            h |> held_same("{what}, step {i}: the adaptive run's logits are the plain run's", adaptive.rows[i], plain.rows[i])
        }
        //! the last walk's settle-dropped chain counts nothing; off every
        //! 3rd, each miss (len 0 -> 1) skips the next step, the one after
        //! hits and resets it: misses 2/5/../23, hits 1/4/../22
        let backed = offs > 0l ? offs - 1l : 0l
        h |> held_equal(sa1.misses - sa0.misses, offs,
            "{what}: adaptive, every off-pick step a miss")
        h |> held_equal(sa1.hits - sa0.hits, CHAIN_STEPS - 1l - offs - backed,
            "{what}: adaptive, the step after each miss but the last unchained")
        h |> held_served()
        h |> held_cuda_served(c0, 3l * CHAIN_STEPS, "{what}: the three walks")
        delete plain
        delete chained
        delete adaptive
    }
    delete prompt
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

//! the armed home's kernel-skip setter (`set_cuda_kernel_skip` under the CUDA ask, else `set_vulkan_kernel_skip`)
def private home_kernel_skip(frags : string) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {
            set_cuda_kernel_skip(frags)
            return
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_kernel_skip(frags)
    }
}

//! the kernel-skip setter drops the commands the home already recorded, on the resident trunk `tr` loaded off `path`, armed by
//! the caller, inside the caller's job queue: a prompt and a step record the step's commands; a step with no skip is the control
//! (`skip_stem`'s census moves exactly where `other`'s does - both rise on a home that records each step, neither where it
//! replays); a step under a skip naming `skip_stem` counts no dispatch of it and dispatches `other` again (the commands recorded
//! anew, never the old ones replayed); the skip cleared, a step dispatches `skip_stem` again. `skip_stem` and `other` are census
//! stems (`<stem>_spv`, the CUDA home's `<stem>_cuda`)
def kernel_skip_rows(t : T?; var tr : Model; path, skip_stem, other : string) {
    let ks = "{skip_stem}_spv"
    let ko = "{other}_spv"
    t |> success(census(ks) >= 0l && census(ko) >= 0l, "the home stamps both census kernels ({ks}: {census(ks)}, {ko}: {census(ko)})")
    var s <- create_session(tr)
    var prompt <- resident_prompt(40l)
    eval_(tr, s, prompt)
    var one <- [3000l]
    eval_(tr, s, one)
    let c0k = census(ks)
    let c0o = census(ko)
    eval_(tr, s, one)
    let ctl_k = census(ks) - c0k
    let ctl_o = census(ko) - c0o
    to_log(LOG_INFO, "{path} kernel skip: the unskipped step dispatched {ks} {ctl_k} and {ko} {ctl_o} time(s)\n")
    t |> equal(ctl_k > 0l, ctl_o > 0l, "control: with no skip {ks} dispatches ({ctl_k}) exactly where {ko} does ({ctl_o})")
    home_kernel_skip(skip_stem)
    let k0 = census(ks)
    let o0 = census(ko)
    eval_(tr, s, one)
    t |> equal(census(ks) - k0, 0l, "under the skip {ks} is not dispatched")
    t |> success(census(ko) > o0, "the recorded commands were dropped: the skipped step dispatched {ko} anew ({census(ko) - o0})")
    home_kernel_skip("")
    let k1 = census(ks)
    eval_(tr, s, one)
    t |> success(census(ks) > k1, "the skip cleared: the next step dispatches {ks} again ({census(ks) - k1})")
    delete one
    delete prompt
    delete s
}

//! what one check of a cell holds off a loaded model for the asserts after the model's block (`held_within`): a condition read
//! (`fact`), two readings `equal` compares (`pair`), a logits row against its reference within a share of the reference's peak
//! with its control past the bar (`row`), the tq4 mirror's nearer-than-the-control form (`near`), two rows bit for bit
//! (`same`), or a skip - the cell the armed home does not carry
enum HeldKind {
    fact
    pair
    row
    near
    same
    skip
}

struct HeldCheck {
    kind : HeldKind
    ok : bool
    got_s : string
    want_s : string
    msg : string
    rel : float
    got : array<float>
    want : array<float>
    control : array<float>
    counted : bool   //! a skip of a cell of its own: a skipped subtest wherever `held_within` would skip the whole test
}

//! a cell's readings in the order it read them: the block collects them, `held_within` asserts them once the model is gone
struct Held {
    checks : array<HeldCheck>
}

def held_fact(var h : Held; ok : bool; msg : string) {
    h.checks |> emplace(HeldCheck(kind = HeldKind.fact, ok = ok, msg = msg))
}

//! `got` equal to `want`, both read as their text
def held_equal(var h : Held; got, want : auto(TT); msg : string) {
    h.checks |> emplace(HeldCheck(kind = HeldKind.pair, got_s = "{got}", want_s = "{want}", msg = msg))
}

//! `cut_claim` (`_model_tier.das`) collected into `h`
def cut_held(var h : Held; m : Model; blocks : int64; arch : string = "") {
    let c = cut_claim(m, blocks, arch)
    h |> held_fact(c.ok, c.msg)
}

def held_skip(var h : Held; msg : string; counted : bool = false) {
    h.checks |> emplace(HeldCheck(kind = HeldKind.skip, msg = msg, counted = counted))
}

//! `held_skip` of a cell of its own: `held_within` registers it as a skipped subtest (`skip_cell`) also where its
//! `mark_skips` would skip the whole test
def held_cell_skip(var h : Held; msg : string) {
    held_skip(h, msg, counted = true)
}

//! `got` within `rel` of `want`'s peak, `control` - the row one off - past the bar
def held_row(var h : Held; what : string; got, want, control : array<float>; rel : float) {
    h.checks |> emplace(HeldCheck(kind = HeldKind.row, msg = what, rel = rel, got := got, want := want, control := control))
}

//! `got` nearer `want` than `control` (`tq4_within`)
def held_near(var h : Held; what : string; got, want, control : array<float>) {
    h.checks |> emplace(HeldCheck(kind = HeldKind.near, msg = what, got := got, want := want, control := control))
}

//! `got` bit for bit `want`
def held_same(var h : Held; what : string; got, want : array<float>) {
    h.checks |> emplace(HeldCheck(kind = HeldKind.same, msg = what, got := got, want := want))
}

//! the served witness read now: no call of the armed driver passed to the CPU rails since the model armed
def held_served(var h : Held; what : string = "the device served every call") {
    var passes <- gpu_cpu_passes_()
    h |> held_equal(length(passes), 0, "{what}: {passes}")
    delete passes
}

//! the passes read now: calls passed to the CPU rails for `reason` alone - `count` of them where past 0 (`only_pass`)
def held_only_pass(var h : Held; reason, what : string; count : int64 = 0l) {
    var passes <- gpu_cpu_passes_()
    h |> held_fact(length(passes) == 1 && passes[0].reason == reason && (count <= 0l || passes[0].count == count), "{what}: {passes}")
    delete passes
}

//! `body` collects into a recorder of its own, and its checks are asserted on `t` when it ends
def held_on(t : T?; body : block<(var h : Held) : void>) {
    var inscope h : Held
    invoke(body, h)
    held_within(t, h)
}

//! every held check asserted on `t`, `tag` opening each message; a held skip skips `t` where `mark_skips`, else - and for a
//! `held_cell_skip` always - it is a skipped subtest of its own (`skip_cell`), the verdict of the other cells read off the
//! same block standing
def held_within(t : T?; h : Held; tag : string = ""; mark_skips : bool = true) {
    for (c in h.checks) {
        if (c.kind == HeldKind.fact) {
            t |> success(c.ok, "{tag}{c.msg}")
        } elif (c.kind == HeldKind.pair) {
            t |> equal(c.got_s, c.want_s, "{tag}{c.msg}")
        } elif (c.kind == HeldKind.row) {
            let bar = row_bar(c.want, c.rel)
            let diff = row_within(t, "{tag}{c.msg}", c.got, c.want, bar)
            let off = control_past(t, "{tag}{c.msg}", c.got, c.control, bar)
            to_log(LOG_INFO, "{tag}{c.msg}: maxdiff {diff}, the row one off {off}, bar {bar}\n")
        } elif (c.kind == HeldKind.near) {
            tq4_within(t, c.got, c.want, c.control, "{tag}{c.msg}")
        } elif (c.kind == HeldKind.same) {
            let diff = logits_maxdiff(c.got, c.want)
            t |> success(diff == 0.0, "{tag}{c.msg}: bit for bit (maxdiff {diff})")
        } elif (c.counted || !mark_skips) {
            skip_cell(t, "{tag}{c.msg}")
        } else {
            t |> skip("{tag}{c.msg}")
        }
    }
}

//! `blk` with the CUDA home's MMQ route pinned to `v` (`set_cuda_mmq_pin`), the pin put back to what `cuda_mmq_pin` read
//! before; `blk` alone in a build with no CUDA module
def with_cuda_mmq_pin(v : int64; blk : block<() : void>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        let was = cuda_mmq_pin()
        set_cuda_mmq_pin(v)
        invoke(blk)
        set_cuda_mmq_pin(was)
    } else {
        invoke(blk)
    }
}

//! `body` under the CUDA home's MMQ route pinned `CUDA_MMQ_WINNERS` - the route the CUDA cells measure - the pin put back after;
//! `body` alone off the CUDA home
def with_cuda_mmq_winners(body : block<() : void>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (gpu_want_cuda()) {
            with_cuda_mmq_pin(CUDA_MMQ_WINNERS, body)
            return
        }
    }
    invoke(body)
}

//! `body` with the CUDA home's far row pinned off (`set_cuda_attn_tile_far_pin`) - a one-row step attends on the row form at
//! every key count - the pin put back to what `cuda_attn_tile_far_pin` read before; `body` alone in a build with no CUDA module
def with_cuda_far_row_off(body : block<() : void>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        let was = cuda_attn_tile_far_pin()
        set_cuda_attn_tile_far_pin(0l)
        invoke(body)
        set_cuda_attn_tile_far_pin(was)
    } else {
        invoke(body)
    }
}

//! the CUDA token step's far row on the loaded model: after a 40-id prompt one step on the decode tile form (the far count
//! pinned to the first key), on the row form (pinned off), and on the tile form over a token one off - the logits each, and the
//! steps the tile form served; nothing on a build without the CUDA module
[unused_argument(h, m)]
def cuda_far_row_rows(var h : Held; m : Model) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        var prompt <- resident_prompt(40l)
        var rows : array<float>[3]
        var far : int64[3]
        let far_pin = cuda_attn_tile_far_pin()
        for (i in range(3)) {
            set_cuda_attn_tile_far_pin(i == 1 ? 0l : 1l)
            var sc <- create_session(m, mirror_kv_dtype())
            forward_prefill(m, sc, prompt, 40l, 0l)
            let f0 = cud_rdec_far_tokens()
            forward(m, sc, FAR_ROW_TOK + (i == 2 ? 1l : 0l), 40l)
            far[i] = cud_rdec_far_tokens() - f0
            rows[i] := sc.logits
            delete sc
        }
        set_cuda_attn_tile_far_pin(far_pin)
        h |> held_row("a step's logits on the decode tile form against the row form", rows[0], rows[1], rows[2], SPLIT_BAR_REL)
        h |> held_equal(far[0], 1l, "the far count pinned to the first key: the step's flat layers took the tile form")
        h |> held_equal(far[1], 0l, "control: the far count pinned off, the row form serves the step")
        for (i in range(3)) {
            delete rows[i]
        }
        delete prompt
    }
}
