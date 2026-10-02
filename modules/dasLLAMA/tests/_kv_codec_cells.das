// The K/V codec cells' rig, one carrier a file: a model loaded with the device mirror asked at a block codec, and the
// forms that hold the device to the CPU chain under it - the forced-feed bar, the three-chain distance form, the stored
// rows, and the per-op rails' form (`ARCHITECTURE_GPU_VULKAN_ATTN.md#vk-kv-block-codecs`,
// `ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block`). A file states its carrier's bars with the readings behind them.

options gen2
options _dasllama_internal = true

module _kv_codec_cells shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common   // Model, Session, kv_row_bytes
require dasllama/dasllama   // create_session, moe_gpu_drop_model, set_gpu_resident_route
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_tier   // moe_gpu_attn_on_gpu: the per-op attention block's layer marks
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, the resident prefill pin and its getter
require _model_tier public
require _resident_feed public

let CODEC_FED_STEPS = 8l
let CODEC_ROWS_CTRL = 0.2   //! a stored row held to the next one differs at 0.36-0.98 of its bytes: the stored-rows compares tell rows apart
//! the device's distance from the f16 chain, as a ratio of the CPU codec chain's own, at least: a driver that served its f16
//! rows in the codec's place would read near 0 (every resident carrier reads 0.95-1.01)
let CODEC_APPLIED_REL = 0.5

//! `blk` over `path` loaded with the whole-model route pinned on and the mirror asked at `kv` (the levers put back), inside the
//! engine's job queue. Skips without the JIT, the model, the armed tier, or where DASLLAMA_VK_KV32=1 armed f32 over the ask.
def with_codec_model(t : T?; path : string; kv : KVDtype; blk : block<(tr : Model) : void>) {
    return if (!resident_ready(t, path))
    var tr <- load_resident_at(path, kv)
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
def cell_codec_forced_feed(t : T?; path : string; n : int64; kv : KVDtype; bar_rel : float) {
    with_codec_model(t, path, kv) $(tr) {
        var prompt <- resident_prompt(n)
        var toks <- resident_feed(CODEC_FED_STEPS)
        var fed <- resident_then_cpu_feed(t, tr, prompt, toks) $ {
            served_every_call(t)
        }
        feed_steps_within(t, tr, "{kv} mirror: ", n, fed.gpu, fed.cpu, toks, bar_rel)
        delete fed
        delete prompt
        delete toks
    }
}

//! The distance form at a block codec, three chains over an `n`-token prompt and `CODEC_FED_STEPS` fed tokens: the device at
//! `kv`, the CPU at `kv`, the CPU at f16. The device sits no further from the f16 chain than `dist_rel` of the CPU codec
//! chain's own distance and no nearer than `CODEC_APPLIED_REL` of it, and the two codec chains within `pair_rel` of that
//! distance of each other; each upper bound's control, its second side one step off, lands outside it. With `cpu_prefill` the device's prompt runs on the CPU rails - its steps
//! read rows the CPU wrote and the driver uploaded - and the prompt's row, the CPU's on both codec chains, stays out of the sums.
[cold_path]
def cell_codec_distance(t : T?; path : string; n : int64; kv : KVDtype; cpu_prefill : bool; dist_rel, pair_rel : float) {
    with_codec_model(t, path, kv) $(tr) {
        codec_distance_over(t, tr, n, kv, cpu_prefill, dist_rel, pair_rel)
    }
}

//! `cell_codec_distance`'s three chains over a model the caller loaded with the mirror armed at `kv`, inside the engine's job
//! queue; the model's device state is dropped when it returns
[cold_path]
def codec_distance_over(t : T?; tr : Model; n : int64; kv : KVDtype; cpu_prefill : bool; dist_rel, pair_rel : float) {
    var prompt <- resident_prompt(n)
    var toks <- resident_feed(CODEC_FED_STEPS)
    let allowed0 = resident_prefill_allowed()
    set_resident_prefill_allowed(!cpu_prefill)
    var sg <- create_session(tr, kv)
    let served0 = moe_gpu_resident_prefills()
    var gpu <- feed_logits(tr, sg, prompt, toks)
    set_resident_prefill_allowed(allowed0)
    if (cpu_prefill) {
        t |> equal(moe_gpu_resident_prefills(), served0, "the prompt read on the CPU rails")
        var passes <- gpu_cpu_passes_()
        t |> success(length(passes) == 1 && passes[0].reason == "pinned_off", "the device served every call but the prompt the pin kept off it: {passes}")
        delete passes
    } else {
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the prompt")
        served_every_call(t)
    }
    t |> success(moe_gpu_resident_active(), "the driver stayed armed through the run")
    delete sg
    moe_gpu_drop_model()
    var sc <- create_session(tr, kv)
    var cpu <- feed_logits(tr, sc, prompt, toks)
    delete sc
    var sf <- create_session(tr, KVDtype.f16)
    var f16_rows <- feed_logits(tr, sf, prompt, toks)
    delete sf
    let what = "{kv}, {n} tokens{cpu_prefill ? ", the prompt on the CPU rails" : ""}"
    let dist = chain_distances(t, tr, what, gpu, cpu, f16_rows, cpu_prefill ? 1 : 0)
    t |> success(dist.cpu_ref > 0.0, "{what}: the codec moves the CPU chain off its f16 rows")
    t |> success(dist.dev_ref <= dist_rel * dist.cpu_ref, "{what}: the device reads no further from the f16 chain than {dist_rel} of the CPU codec chain's distance ({dist.dev_ref} against {dist.cpu_ref})")
    t |> success(dist.dev_ref_ctrl > dist_rel * dist.cpu_ref, "{what} control: the f16 rows one step off sit past that bound ({dist.dev_ref_ctrl} against {dist_rel * dist.cpu_ref})")
    t |> success(dist.dev_ref >= CODEC_APPLIED_REL * dist.cpu_ref, "{what}: the codec moves the device off the f16 rows as it moves the CPU chain ({dist.dev_ref} against {dist.cpu_ref})")
    t |> success(dist.dev_cpu <= pair_rel * dist.cpu_ref, "{what}: the two codec chains sit within {pair_rel} of the codec's distance from f16 ({dist.dev_cpu} against {dist.cpu_ref})")
    t |> success(dist.dev_cpu_ctrl > pair_rel * dist.cpu_ref, "{what} control: the CPU codec rows one step off sit past that bound ({dist.dev_cpu_ctrl} against {pair_rel * dist.cpu_ref})")
    delete gpu
    delete cpu
    delete f16_rows
    delete prompt
    delete toks
}

//! the stored-rows form's bars: the shares of bytes at which the device's rows may differ from the CPU cache's
struct StoredRowsBars {
    k : float                    //! the K rows
    v_step : float               //! the decode steps' V rows
    v_prefill_of_ctrl : float    //! the prefill's V rows, of the share a row held to the next one differs by
}

let private PLAN_WINDOW_ROWS = 512l   //! the prefill window the plan sizes the staged rows by (the engine's PF_WINDOW)
let private PLAN_PROBE_CTX = 4096l

//! the plan charges a block codec's scratch over the f16 plan's: both sides' f16 shadow of one layer's rows at the context,
//! and the window's staged float rows (layer 0's K/V row is the carrier's widest)
def private plan_charges_codec_scratch(t : T?; tr : Model; kv : KVDtype) {
    let kvd = layer_kv_dim(tr.config, 0l)
    let codec_plan = resident_plan(tr, PLAN_PROBE_CTX, kv, kv)
    let f16_plan = resident_plan(tr, PLAN_PROBE_CTX, KVDtype.f16, KVDtype.f16)
    let want = 2l * PLAN_PROBE_CTX * kvd * 2l + 2l * PLAN_WINDOW_ROWS * kvd * 4l
    t |> equal(codec_plan.scratch_bytes - f16_plan.scratch_bytes, want, "{kv}: the plan charges the shadow planes and the staged rows ({want} B at {PLAN_PROBE_CTX} positions of a {kvd}-wide row)")
}

//! The stored-rows form: the mirror brought down after a 40-token resident prefill and `CODEC_FED_STEPS` resident steps, layer
//! 0's bytes against the CPU cache's after the same tokens, within `bars`; a row held to the next one is the control. The
//! cell also holds the plan's scratch charge for the codec.
[cold_path]
def cell_codec_stored_rows(t : T?; path : string; kv : KVDtype; bars : StoredRowsBars) {
    with_codec_model(t, path, kv) $(tr) {
        plan_charges_codec_scratch(t, tr, kv)
        let npf = 40l
        let rows = npf + CODEC_FED_STEPS
        var prompt <- resident_prompt(npf)
        var toks <- resident_feed(CODEC_FED_STEPS)
        var sg <- create_session(tr, kv)
        let served0 = moe_gpu_resident_prefills()
        var gpu <- feed_logits(tr, sg, prompt, toks)
        t |> equal(moe_gpu_resident_prefills(), served0 + 1l, "the resident window chain served the prompt")
        served_every_call(t)
        moe_gpu_hydrate_session(tr, sg)
        var dev <- layer_rows(tr, sg, 0l, rows)
        delete sg
        moe_gpu_drop_model()
        var sc <- create_session(tr, kv)
        var cpu <- feed_logits(tr, sc, prompt, toks)
        var host <- layer_rows(tr, sc, 0l, rows)
        delete sc
        let rowb = kv_row_bytes(kv, layer_kv_dim(tr.config, 0l))
        let k_bytes = rows * rowb
        let k_diff = bytes_differing(dev, host, 0l, 0l, k_bytes)
        let v_prefill_diff = bytes_differing(dev, host, k_bytes, k_bytes, npf * rowb)
        let v_step_diff = bytes_differing(dev, host, k_bytes + npf * rowb, k_bytes + npf * rowb, CODEC_FED_STEPS * rowb)
        let k_ctrl = bytes_differing(dev, host, 0l, rowb, k_bytes - rowb)
        let v_ctrl = bytes_differing(dev, host, k_bytes, k_bytes + rowb, npf * rowb)
        to_log(LOG_INFO, "{kv} layer 0, {rows} rows of {rowb} bytes: K differs at {k_diff} of its bytes, the prefill's V at {v_prefill_diff}, the steps' V at {v_step_diff}; a row against the next - K {k_ctrl}, V {v_ctrl}\n")
        t |> success(k_diff <= bars.k, "{kv}: the device's K rows are the CPU cache's but for a quant on a rounding edge ({k_diff} of the bytes differ)")
        t |> success(v_step_diff <= bars.v_step, "{kv}: the decode steps' V rows are the CPU cache's but for a quant on a rounding edge ({v_step_diff} of the bytes differ)")
        t |> success(v_prefill_diff <= bars.v_prefill_of_ctrl * v_ctrl, "{kv}: the prefill's V rows sit by the CPU cache's ({v_prefill_diff} of the bytes differ against {v_ctrl} for a row held to the next)")
        t |> success(k_ctrl >= CODEC_ROWS_CTRL && v_ctrl >= CODEC_ROWS_CTRL, "{kv} control: a row held to the next one reads apart (K {k_ctrl}, V {v_ctrl})")
        delete dev
        delete host
        delete gpu
        delete cpu
        delete prompt
        delete toks
    }
}

//! the per-op form's bars
struct PeropBars {
    rows : float       //! the share of layer 0's cache bytes at which the device arm may differ from the CPU chain's
    dist_rel : float   //! the device's summed distance from the f16 chain, of the CPU codec chain's own
}

let PEROP_PREFILL_FLOOR = 32l   //! the shortest prompt the per-op device prefill takes (`attn_gpu_prefill_ready`)

//! The per-op rails under a session of `kv`, the whole-model route pinned off for the load: an `n`-token prompt and
//! `CODEC_FED_STEPS` fed steps. Layer 0's cache bytes within `bars.rows` of the CPU chain's (a row held to the next the control);
//! the device's summed distance from the CPU chain at f16 within `bars.dist_rel` of the CPU codec chain's own, the f16 rows one
//! step off past that bound. The census holds that the block's attention served the steps and the CPU chain's did not, and that
//! the device prefilled a prompt at `PEROP_PREFILL_FLOOR` or longer and no shorter one. Skips where the tier marked no
//! attention layer of the model for the block.
[cold_path]
def cell_perop_codec(t : T?; path : string; n : int64; kv : KVDtype; bars : PeropBars) {
    return if (!resident_ready(t, path))
    let route0 = gpu_resident_route_lever()
    set_gpu_resident_route(false)
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_resident_route(route0)
    t |> success(!moe_gpu_resident_active(), "the route lever kept the whole-model driver off the model")
    if (!moe_gpu_attn_on_gpu(0l)) {
        t |> skip("the per-op tier took no attention layer of this model on this device (the load log's GPU MoE tier lines say why)")
    } else {
        with_dasllama_jobque_() {
            perop_codec_compare(t, tr, n, kv, bars)
        }
    }
    moe_gpu_drop_model()
    delete tr
}

def private perop_codec_compare(t : T?; tr : Model; n : int64; kv : KVDtype; bars : PeropBars) {
    var prompt <- resident_prompt(n)
    var toks <- resident_feed(CODEC_FED_STEPS)
    var sg <- create_session(tr, kv)
    let attn0 = census("da_attn_f16_cls_spv")
    let prep0 = census("at_prep_cls_spv")
    var gpu <- feed_logits(tr, sg, prompt, toks)
    let attn1 = census("da_attn_f16_cls_spv")
    let prep1 = census("at_prep_cls_spv")
    let rows = n + CODEC_FED_STEPS
    var dev <- layer_rows(tr, sg, 0l, rows)
    delete sg
    moe_gpu_drop_model()
    var sc <- create_session(tr, kv)
    var cpu <- feed_logits(tr, sc, prompt, toks)
    var host <- layer_rows(tr, sc, 0l, rows)
    delete sc
    var sf <- create_session(tr, KVDtype.f16)
    var f16_rows <- feed_logits(tr, sf, prompt, toks)
    delete sf
    let what = "{kv}, {n} tokens on the per-op rails"
    if (attn0 != NO_CENSUS) {
        t |> success(attn1 - attn0 >= CODEC_FED_STEPS, "{what}: the device's attention block served the steps ({attn1 - attn0} dispatches)")
        t |> equal(census("da_attn_f16_cls_spv"), attn1, "{what} control: the CPU chains dispatch no device attention")
        if (n >= PEROP_PREFILL_FLOOR) {
            t |> success(prep1 > prep0, "{what}: the device prefilled the prompt ({prep1 - prep0} dispatches of its prep kernel)")
        } else {
            t |> equal(prep1, prep0, "{what}: a prompt under the device prefill's floor of {PEROP_PREFILL_FLOOR} prefills on the CPU")
        }
    }
    let rowb = kv_row_bytes(kv, layer_kv_dim(tr.config, 0l))
    let k_bytes = rows * rowb
    let k_diff = bytes_differing(dev, host, 0l, 0l, k_bytes)
    let v_diff = bytes_differing(dev, host, k_bytes, k_bytes, k_bytes)
    let k_ctrl = bytes_differing(dev, host, 0l, rowb, k_bytes - rowb)
    to_log(LOG_INFO, "{what}, layer 0: K differs at {k_diff} of its bytes, V at {v_diff}; a K row against the next {k_ctrl}\n")
    t |> success(k_diff <= bars.rows && v_diff <= bars.rows, "{what}: the cache's rows are the CPU chain's but for a quant on a rounding edge (K {k_diff}, V {v_diff} of the bytes differ)")
    t |> success(k_ctrl >= CODEC_ROWS_CTRL, "{what} control: a row held to the next one reads apart ({k_ctrl})")
    let dist = chain_distances(t, tr, what, gpu, cpu, f16_rows, 0)
    t |> success(dist.dev_ref <= bars.dist_rel * dist.cpu_ref, "{what}: the device reads no further from the f16 chain than {bars.dist_rel} of the CPU codec chain's distance ({dist.dev_ref} against {dist.cpu_ref})")
    t |> success(dist.dev_ref_ctrl > bars.dist_rel * dist.cpu_ref, "{what} control: the f16 rows one step off sit past that bound ({dist.dev_ref_ctrl} against {bars.dist_rel * dist.cpu_ref})")
    delete dev
    delete host
    delete gpu
    delete cpu
    delete f16_rows
    delete prompt
    delete toks
}
