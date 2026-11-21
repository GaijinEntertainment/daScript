// The K/V codec cells' rig, one carrier a file: a model loaded with the device mirror asked at a block codec, and the
// forms that hold the device to the CPU chain under it - the forced-feed bar, the three-chain distance form, the stored
// rows, and the per-op rails' form (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`,
// `ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block`). A file states its carrier's bars with the readings behind them.

options gen2
options _dasllama_internal = true

module _kv_codec_cells shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common   // Model, Session, kv_row_bytes
require dasllama/dasllama   // create_session, moe_gpu_drop_model, set_gpu_resident_route
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_kv_dtype   // kv_block_codec: the stored-rows cell's f16 arm
require dasllama/dasllama_env   // the prompt window's pin: the plan probe's staged rows read the window
require dasllama/dasllama_gpu_tier   // moe_gpu_attn_on_gpu: the per-op attention block's layer marks
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, the resident prefill pin and its getter
require _model_tier public
require _resident_feed public

let CODEC_FED_STEPS = 8l
let CODEC_ROWS_CTRL = 0.2   //! a stored row held to the next one differs at 0.36-0.98 of its bytes: the stored-rows compares tell rows apart
//! the device's distance from the f16 chain, as a ratio of the CPU codec chain's own, at least: a driver that served its f16
//! rows in the codec's place would read near 0 (every resident carrier reads 0.95-1.01)
let CODEC_APPLIED_REL = 0.5

//! why the environment keeps the mirror off `kv`: the f32 mirror
//! arms under `DASLLAMA_VK_KV32=1` alone (the in-process ask is
//! refused by name), every block codec under it unset; "" where it
//! arms
def codec_env_why(kv : KVDtype) : string {
    let kv32 = g_env_vulkan.vk_kv32 != 0l
    return ("the f32 mirror arms under DASLLAMA_VK_KV32=1 alone - " +
        "the arm runs there") if (kv == KVDtype.f32 && !kv32)
    return ("DASLLAMA_VK_KV32=1 arms the f32 mirror over the {kv} asked - " +
        "a codec cell needs it unset") if (kv != KVDtype.f32 && kv32)
    return ""
}

//! `blk` over `file` (under models_dir()) loaded with the whole-model route pinned on and the mirror asked at `kv` (the levers put back), inside the
//! engine's job queue. Skips without the JIT, the model, the armed tier, or where DASLLAMA_VK_KV32=1 armed f32 over the ask.
def with_codec_model(t : T?; file : string; kv : KVDtype; blk : block<(tr : Model) : void>) {
    let path = path_join(models_dir(), file)
    return if (!resident_ready(t, path))
    var tr <- load_resident_at(path, kv, 0l, 0l, true)
    if (mirror_kv_dtype() == KVDtype.f32 && kv != KVDtype.f32) {
        t |> skip("DASLLAMA_VK_KV32=1 arms the f32 mirror over the {kv} asked - a codec cell needs it unset")
    } else {
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed (the load log names any decline)")
        t |> equal(mirror_kv_dtype(), kv, "the mirror armed at the asked codec")
        with_dasllama_jobque_() {
            invoke(blk, tr)
        }
    }
    moe_gpu_drop_model()
    delete tr
}

//! The forced-feed form at a block codec: an `n`-token prompt and `CODEC_FED_STEPS` fed tokens through the device and the CPU
//! chain, both on sessions of `kv`, every row within `bar_rel` of the CPU row's largest live logit, the row one step off past it.
[cold_path]
def cell_codec_forced_feed(t : T?; file : string; n : int64; kv : KVDtype; bar_rel : float) {
    with_codec_model(t, file, kv) $(tr) {
        forced_feed_over(t, tr, "{kv} mirror: ", n, CODEC_FED_STEPS, bar_rel) $ {
            served_every_call(t)
        }
    }
}

//! The distance form at a block codec, three chains over an `n`-token prompt and `CODEC_FED_STEPS` fed tokens: the device at
//! `kv`, the CPU at `kv`, the CPU at f16. The device sits no further from the f16 chain than `dist_rel` of the CPU codec
//! chain's own distance and no nearer than `CODEC_APPLIED_REL` of it, and the two codec chains within `pair_rel` of that
//! distance of each other; each upper bound's control, its second side one step off, lands outside it. With `cpu_prefill` the device's prompt runs on the CPU rails - its steps
//! read rows the CPU wrote and the driver uploaded - and the prompt's row, the CPU's on both codec chains, stays out of the sums.
[cold_path]
def cell_codec_distance(t : T?; file : string; n : int64; kv : KVDtype; cpu_prefill : bool; dist_rel, pair_rel : float) {
    with_codec_model(t, file, kv) $(tr) {
        codec_distance_over(t, tr, n, kv, cpu_prefill, dist_rel, pair_rel)
    }
}

//! `cell_codec_distance`'s three chains over a model the caller loaded with the mirror armed at `kv`, inside the engine's job
//! queue; the model's device state is dropped when it returns
def codec_distance_over(t : T?; tr : Model; n : int64; kv : KVDtype; cpu_prefill : bool; dist_rel, pair_rel : float) {
    var prompt <- resident_prompt(n)
    var toks <- resident_feed(CODEC_FED_STEPS)
    var gpu <- codec_device_rows(t, tr, n, kv, cpu_prefill)
    moe_gpu_drop_model()
    var cpu <- feed_alone(tr, kv, prompt, toks)
    var f16_rows <- feed_alone(tr, KVDtype.f16, prompt, toks)
    let what = "{kv}, {n} tokens{cpu_prefill ? ", the prompt on the CPU rails" : ""}"
    codec_distances_within(t, what, gpu, cpu, f16_rows, cpu_prefill ? 1 : 0, dist_rel, pair_rel) $(id) => decode_(tr, [id])
    delete gpu
    delete cpu
    delete f16_rows
    delete prompt
    delete toks
}

//! the distance form's device side on a model armed at `kv`: an `n`-token prompt and `CODEC_FED_STEPS` fed tokens on a session
//! of `kv` - with `cpu_prefill` the prompt on the CPU rails, the steps over the rows the driver uploaded, its pinned-off pass
//! by design - its witnesses asserted; the device's rows back. A caller whose load reads the served witness after a
//! `cpu_prefill` run resets the passes (`reset_gpu_cpu_passes`)
def codec_device_rows(t : T?; tr : Model; n : int64; kv : KVDtype; cpu_prefill : bool) : array<array<float>> {
    var prompt <- resident_prompt(n)
    var toks <- resident_feed(CODEC_FED_STEPS)
    let allowed0 = resident_prefill_allowed()
    set_resident_prefill_allowed(!cpu_prefill)
    var sg <- create_session(tr, kv)
    let served0 = gpu_resident_prefills_()
    var gpu <- feed_logits(tr, sg, prompt, toks)
    set_resident_prefill_allowed(allowed0)
    if (cpu_prefill) {
        t |> equal(gpu_resident_prefills_(), served0, "the prompt read on the CPU rails")
        only_pass(t, "pinned_off", "the device served every call but the prompt the pin kept off it")
    } else {
        t |> equal(gpu_resident_prefills_(), served0 + 1l, "the resident window chain served the prompt")
        served_every_call(t)
    }
    t |> success(moe_gpu_resident_active(), "the driver stayed armed through the run")
    delete sg
    delete prompt
    delete toks
    return <- gpu
}

//! the distance form's bounds on three chains' rows - the device's `gpu` and the CPU's `cpu` at the codec, the CPU's at f16 -
//! the rows before `first` out of the sums, `piece` decoding the logged picks: the device no further from the f16 chain than
//! `dist_rel` of the CPU codec chain's distance and no nearer than `CODEC_APPLIED_REL` of it, the two codec chains within
//! `pair_rel` of that distance, each upper bound's control - its second side one step off - outside it
def codec_distances_within(t : T?; what : string; gpu, cpu, f16_rows : array<array<float>>; first : int; dist_rel, pair_rel : float;
                           piece : block<(id : int64) : string>) {
    let dist = chain_distances(t, what, gpu, cpu, f16_rows, first, "f16", "the CPU codec chain", piece)
    t |> success(dist.cpu_ref > 0.0, "{what}: the codec moves the CPU chain off its f16 rows")
    t |> success(dist.dev_ref <= dist_rel * dist.cpu_ref, "{what}: the device reads no further from the f16 chain than {dist_rel} of the CPU codec chain's distance ({dist.dev_ref} against {dist.cpu_ref})")
    t |> success(dist.dev_ref_ctrl > dist_rel * dist.cpu_ref, "{what} control: the f16 rows one step off sit past that bound ({dist.dev_ref_ctrl} against {dist_rel * dist.cpu_ref})")
    t |> success(dist.dev_ref >= CODEC_APPLIED_REL * dist.cpu_ref, "{what}: the codec moves the device off the f16 rows as it moves the CPU chain ({dist.dev_ref} against {dist.cpu_ref})")
    t |> success(dist.dev_cpu <= pair_rel * dist.cpu_ref, "{what}: the two codec chains sit within {pair_rel} of the codec's distance from f16 ({dist.dev_cpu} against {dist.cpu_ref})")
    t |> success(dist.dev_cpu_ctrl > pair_rel * dist.cpu_ref, "{what} control: the CPU codec rows one step off sit past that bound ({dist.dev_cpu_ctrl} against {pair_rel * dist.cpu_ref})")
}

//! the stored-rows form's bars: the shares of bytes at which the device's rows may differ from the CPU cache's
struct StoredRowsBars {
    k : float                    //! the K rows
    v_step : float               //! the decode steps' V rows
    v_prefill_of_ctrl : float    //! the prefill's V rows, of the share a row held to the next one differs by
    f16_dist : float = 0.0       //! f16 rows: the prefill's K and V rows' distance of the CPU rows' magnitude (an ulp off flips every low byte)
}

let private PLAN_PROBE_CTX = 4096l

//! the plan charges a block codec's scratch over the f16 plan's: both sides' f16 shadow of one layer's rows at the context,
//! and the window's staged float rows (layer 0's K/V row is the carrier's widest)
def private plan_charges_codec_scratch(t : T?; tr : Model; kv : KVDtype) {
    let kvd = layer_kv_dim(tr.config, 0l)
    let pf_pin = pf_window_rows_pin()
    set_pf_window_rows_pin(PF_WINDOW_ROWS)   //! the staged rows' charge reads the window, which a pick an earlier load made would move
    let codec_plan = resident_plan(tr, PLAN_PROBE_CTX, kv, kv)
    let f16_plan = resident_plan(tr, PLAN_PROBE_CTX, KVDtype.f16, KVDtype.f16)
    set_pf_window_rows_pin(pf_pin)
    let want = kv_block_codec(kv) ? 2l * PLAN_PROBE_CTX * kvd * 2l + 2l * PF_WINDOW_ROWS * kvd * 4l : 0l
    t |> equal(codec_plan.scratch_bytes - f16_plan.scratch_bytes, want, "{kv}: the plan charges the shadow planes and the staged rows ({want} B at {PLAN_PROBE_CTX} positions of a {kvd}-wide row)")
}

//! The stored-rows form: the mirror brought down after a 40-token resident prefill and `CODEC_FED_STEPS` resident steps, layer
//! 0's bytes against the CPU cache's after the same tokens, within `bars` (f16 rows: the prefill's K and V rows as a distance of
//! the CPU rows' magnitude, `bars.f16_dist`); a row held to the next one is the control. The cell also holds the plan's scratch
//! charge for the codec (none for f16).
[cold_path]
def cell_codec_stored_rows(t : T?; file : string; kv : KVDtype; bars : StoredRowsBars) {
    with_codec_model(t, file, kv) $(tr) {
        var dev <- codec_stored_device(t, tr, kv)
        moe_gpu_drop_model()
        var host <- codec_stored_host(tr, kv)
        codec_stored_within(t, kv, layer_kv_dim(tr.config, 0l), dev, host, bars)
        delete dev
        delete host
    }
}

let private STORED_PROMPT = 40l   //! the stored-rows form's prompt

//! the stored-rows form's device side on a model armed at `kv`: the plan's scratch charge for the codec held, a 40-token resident
//! prefill and `CODEC_FED_STEPS` resident steps on a session of `kv`, the mirror brought down; layer 0's rows back
def codec_stored_device(t : T?; tr : Model; kv : KVDtype) : array<uint8> {
    plan_charges_codec_scratch(t, tr, kv)
    var prompt <- resident_prompt(STORED_PROMPT)
    var toks <- resident_feed(CODEC_FED_STEPS)
    var sg <- create_session(tr, kv)
    let served0 = gpu_resident_prefills_()
    var gpu <- feed_logits(tr, sg, prompt, toks)
    t |> equal(gpu_resident_prefills_(), served0 + 1l, "the resident window chain served the prompt")
    served_every_call(t)
    moe_gpu_hydrate_session(tr, sg)
    var dev <- layer_rows(tr, sg, 0l, STORED_PROMPT + CODEC_FED_STEPS)
    delete sg
    delete gpu
    delete prompt
    delete toks
    return <- dev
}

//! the stored-rows form's CPU side: the same tokens on a CPU session of `kv`, layer 0's rows back
def codec_stored_host(tr : Model; kv : KVDtype) : array<uint8> {
    var prompt <- resident_prompt(STORED_PROMPT)
    var toks <- resident_feed(CODEC_FED_STEPS)
    var sc <- create_session(tr, kv)
    var cpu <- feed_logits(tr, sc, prompt, toks)
    var host <- layer_rows(tr, sc, 0l, STORED_PROMPT + CODEC_FED_STEPS)
    delete sc
    delete cpu
    delete prompt
    delete toks
    return <- host
}

//! the stored-rows form's compares on layer 0's rows, `kvd` wide: the device's `dev` against the CPU cache's `host` within `bars`
//! (f16 rows: the prefill's K and V rows as a distance of the CPU rows' magnitude, `bars.f16_dist`), a row held to the next one
//! the control
def codec_stored_within(t : T?; kv : KVDtype; kvd : int64; dev, host : array<uint8>; bars : StoredRowsBars) {
    let npf = STORED_PROMPT
    let rows = npf + CODEC_FED_STEPS
    let rowb = kv_row_bytes(kv, kvd)
    let k_bytes = rows * rowb
    let k_diff = bytes_differing(dev, host, 0l, 0l, k_bytes)
    let v_prefill_diff = bytes_differing(dev, host, k_bytes, k_bytes, npf * rowb)
    let v_step_diff = bytes_differing(dev, host, k_bytes + npf * rowb, k_bytes + npf * rowb, CODEC_FED_STEPS * rowb)
    let k_ctrl = bytes_differing(dev, host, 0l, rowb, k_bytes - rowb)
    let v_ctrl = bytes_differing(dev, host, k_bytes, k_bytes + rowb, npf * rowb)
    to_log(LOG_INFO, "{kv} layer 0, {rows} rows of {rowb} bytes: K differs at {k_diff} of its bytes, the prefill's V at {v_prefill_diff}, the steps' V at {v_step_diff}; a row against the next - K {k_ctrl}, V {v_ctrl}\n")
    if (kv_block_codec(kv)) {
        t |> success(k_diff <= bars.k, "{kv}: the device's K rows are the CPU cache's but for a quant on a rounding edge ({k_diff} of the bytes differ)")
        t |> success(v_prefill_diff <= bars.v_prefill_of_ctrl * v_ctrl, "{kv}: the prefill's V rows sit by the CPU cache's ({v_prefill_diff} of the bytes differ against {v_ctrl} for a row held to the next)")
    } else {
        let k_dist = f16_rows_distance(dev, host, 0l, 0l, npf * rowb)
        let v_dist = f16_rows_distance(dev, host, k_bytes, k_bytes, npf * rowb)
        let k_dist_ctrl = f16_rows_distance(dev, host, 0l, rowb, (npf - 1l) * rowb)
        to_log(LOG_INFO, "{kv} layer 0: the prefill's K rows sit {k_dist} of the CPU rows' magnitude from them, the V rows {v_dist}; a K row against the next {k_dist_ctrl}\n")
        t |> success(k_dist <= bars.f16_dist && v_dist <= bars.f16_dist, "{kv}: the device prefill's K and V rows sit by the CPU cache's (K {k_dist}, V {v_dist} of the rows' magnitude; {k_dist_ctrl} for a row held to the next)")
    }
    t |> success(v_step_diff <= bars.v_step, "{kv}: the decode steps' V rows are the CPU cache's but for a quant on a rounding edge ({v_step_diff} of the bytes differ)")
    t |> success(k_ctrl >= CODEC_ROWS_CTRL && v_ctrl >= CODEC_ROWS_CTRL, "{kv} control: a row held to the next one reads apart (K {k_ctrl}, V {v_ctrl})")
}

//! a codec form's sides shared across a block's forms: the device's rows over a 40-token prompt on the CPU rails (`up`), and
//! layer 0's stored rows `kvd` wide on the device and on the CPU; `up_ran` says the CPU-rails form collected
struct CodecSides {
    up : array<array<float>>
    up_ran : bool
    stored_dev : array<uint8>
    stored_host : array<uint8>
    kvd : int64
}

//! the device side of the 40-token prompt on the CPU rails at `kv`, its pinned-off pass reset after it for the form's own witness
def codec_up_rows(t : T?; var s : CodecSides; m : Model; kv : KVDtype) {
    s.up <- codec_device_rows(t, m, 40l, kv, true)
    reset_gpu_cpu_passes()
    s.up_ran = true
}

//! the device side of layer 0's stored rows at `kv`
def codec_stored_device_rows(t : T?; var s : CodecSides; m : Model; kv : KVDtype) {
    s.stored_dev <- codec_stored_device(t, m, kv)
    s.kvd = layer_kv_dim(m.config, 0l)
}

//! the stored rows' compare as its own cell `title`
def codec_stored_cell(t : T?; title : string; kv : KVDtype; s : CodecSides; bars : StoredRowsBars) {
    unsafe {
        t |> run(title) @capture(& s)(t : T ?) {
            codec_stored_within(t, kv, s.kvd, s.stored_dev, s.stored_host, bars)
        }
    }
}

//! the per-op form's bars
struct PeropBars {
    rows : float       //! the share of layer 0's cache bytes at which the device arm may differ from the CPU chain's
    dist_rel : float   //! the device's summed distance from the f16 chain, of the CPU codec chain's own
}

let PEROP_PREFILL_FLOOR = 32l   //! the shortest prompt the per-op device prefill takes (`attn_gpu_prefill_ready`)

//! one per-op cell: an `n`-token prompt and `CODEC_FED_STEPS` fed steps on a session of `kv`, held to `bars`
struct PeropCell {
    n : int64
    kv : KVDtype
    bars : PeropBars
}

//! per-op cells a form each (`perop_forms`), run in order by the forms' loads (`perop_next`); `at` the cell the next load runs
struct PeropRun {
    cells : array<PeropCell>
    at : int
}

//! the next cell of `run` on its per-op form's load (`perop_codec_cells`)
def perop_next(t : T?; var run : PeropRun; tr : Model) {
    perop_codec_cells(t, tr, [run.cells[run.at]])
    run.at++
}

//! a per-op cell's device side: its rows, layer 0's cache bytes, and the attention block's and its prep kernel's dispatches
//! over the feed (NO_CENSUS with no census)
struct private PeropRows {
    gpu : array<array<float>>
    dev : array<uint8>
    attn : int64
    prep : int64
}

//! The per-op rails under block-codec sessions, on a model the caller loaded with the whole-model route pinned off: each
//! cell of `cells` feeds the device, then the device state goes and each cell's CPU chains run - at its codec and at f16 - on
//! the same model. Layer 0's cache bytes within `bars.rows` of the CPU chain's (a row held to the next the control); the
//! device's summed distance from the CPU chain at f16 within `bars.dist_rel` of the CPU codec chain's own, the f16 rows one
//! step off past that bound. The census holds that the block's attention served each cell's steps and the CPU chains did
//! not, and that the device prefilled a prompt at `PEROP_PREFILL_FLOOR` or longer and no shorter one. Skips where the tier
//! marked no attention layer of the model for the block.
[cold_path]
def perop_codec_cells(t : T?; tr : Model; cells : array<PeropCell>) {
    if (!moe_gpu_attn_on_gpu(0l)) {
        t |> skip("the per-op tier took no attention layer of this model on this device (the load log's GPU MoE tier lines say why)")
        return
    }
    var rows : array<PeropRows>
    rows |> reserve(length(cells))
    for (c in cells) {
        rows |> emplace <| perop_device(tr, c)
    }
    let attn_off = census("da_attn_f16_cls_spv")
    moe_gpu_drop_model()
    for (c, p in cells, rows) {
        perop_within(t, tr, c, p, attn_off)
    }
    delete rows
}

def private perop_device(tr : Model; c : PeropCell) : PeropRows {
    var prompt <- resident_prompt(c.n)
    var toks <- resident_feed(CODEC_FED_STEPS)
    var sg <- create_session(tr, c.kv)
    let attn0 = census("da_attn_f16_cls_spv")
    let prep0 = census("at_prep_cls_spv")
    var gpu <- feed_logits(tr, sg, prompt, toks)
    let attn = census_since("da_attn_f16_cls_spv", attn0)
    let prep = census_since("at_prep_cls_spv", prep0)
    var dev <- layer_rows(tr, sg, 0l, c.n + CODEC_FED_STEPS)
    delete sg
    delete prompt
    delete toks
    return <- PeropRows(gpu <- gpu, dev <- dev, attn = attn, prep = prep)
}

//! one per-op cell's CPU chains and compares, the device state gone: `attn_off` the attention block's census as it went
def private perop_within(t : T?; tr : Model; c : PeropCell; p : PeropRows; attn_off : int64) {
    var prompt <- resident_prompt(c.n)
    var toks <- resident_feed(CODEC_FED_STEPS)
    let rows = c.n + CODEC_FED_STEPS
    var sc <- create_session(tr, c.kv)
    var cpu <- feed_logits(tr, sc, prompt, toks)
    var host <- layer_rows(tr, sc, 0l, rows)
    delete sc
    var f16_rows <- feed_alone(tr, KVDtype.f16, prompt, toks)
    let what = "{c.kv}, {c.n} tokens on the per-op rails"
    if (p.attn != NO_CENSUS) {
        t |> success(p.attn >= CODEC_FED_STEPS, "{what}: the device's attention block served the steps ({p.attn} dispatches)")
        t |> equal(census("da_attn_f16_cls_spv"), attn_off, "{what} control: the CPU chains dispatch no device attention")
        if (c.n >= PEROP_PREFILL_FLOOR) {
            t |> success(p.prep > 0l, "{what}: the device prefilled the prompt ({p.prep} dispatches of its prep kernel)")
        } else {
            t |> equal(p.prep, 0l, "{what}: a prompt under the device prefill's floor of {PEROP_PREFILL_FLOOR} prefills on the CPU")
        }
    }
    let rowb = kv_row_bytes(c.kv, layer_kv_dim(tr.config, 0l))
    let k_bytes = rows * rowb
    let k_diff = bytes_differing(p.dev, host, 0l, 0l, k_bytes)
    let v_diff = bytes_differing(p.dev, host, k_bytes, k_bytes, k_bytes)
    let k_ctrl = bytes_differing(p.dev, host, 0l, rowb, k_bytes - rowb)
    to_log(LOG_INFO, "{what}, layer 0: K differs at {k_diff} of its bytes, V at {v_diff}; a K row against the next {k_ctrl}\n")
    t |> success(k_diff <= c.bars.rows && v_diff <= c.bars.rows, "{what}: the cache's rows are the CPU chain's but for a quant on a rounding edge (K {k_diff}, V {v_diff} of the bytes differ)")
    t |> success(k_ctrl >= CODEC_ROWS_CTRL, "{what} control: a row held to the next one reads apart ({k_ctrl})")
    let dist = chain_distances(t, tr, what, p.gpu, cpu, f16_rows, 0)
    t |> success(dist.dev_ref <= c.bars.dist_rel * dist.cpu_ref, "{what}: the device reads no further from the f16 chain than {c.bars.dist_rel} of the CPU codec chain's distance ({dist.dev_ref} against {dist.cpu_ref})")
    t |> success(dist.dev_ref_ctrl > c.bars.dist_rel * dist.cpu_ref, "{what} control: the f16 rows one step off sit past that bound ({dist.dev_ref_ctrl} against {c.bars.dist_rel * dist.cpu_ref})")
    delete host
    delete cpu
    delete f16_rows
    delete prompt
    delete toks
}
