options gen2
options _dasllama_internal = true

module _cpu_gpu_model shared public

require dastest/testing_boost public
require daslib/jobque_boost
require dasllama/dasllama_common   // Model
require dasllama/dasllama   // load_model_, moe_gpu_drop_model
require dasllama/dasllama_kv_dtype   // KVDtype
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, gpu_resident_prefills_
require dasllama/dasllama_gpu_tier   // set_gpu_ctx_max
require dasllama/dasllama_env   // the context pins, the image rail's switch
require dasllama/dasllama_image   // image_save_enabled, dlim_names_beside
require ?vulkan dasllama/dasllama_vulkan_common   // g_vk_fuse, the fuse bisect's pin: a device form's knobs
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_gathered: the gather cell's step rows
require dasllama/dasllama_kqformat   // kq_block32: the gather stamp's name
require ?cuda dasllama/dasllama_cuda_decode   // CUDA_MMQ_OFF: an MMQ-off device form's pin
require math   // min
require daslib/fio   // path_join
require _model_tier public   // model_available, mirror_kv_dtype
require _resident_feed public   // resident_trunk, load_resident, the levers, feed_alone, the served witnesses
require _compares
require _kv_codec_cells   // codec_env_why: a codec form the environment keeps off
require ?cuda _cuda_feed   // with_cuda_feed_served, with_cuda_counters_still: the CUDA home's feed witnesses

//! one form of `with_cpu_and_gpu_model`: where the model loads and the knobs its load applies. `kv` is the K/V codec the form's
//! sessions take - the device mirror's as it armed, on a CPU form the codec of the device forms it answers
struct ModelForm {
    on_gpu : bool
    kv : KVDtype
    kv_asked : bool   //! a device form asks the mirror for `kv` (`with_gpu_kv_dtype`); else the environment's codec arms
    ring : bool       //! the sliding layers' K/V rings (`with_gpu_kv_ring`), else a flat mirror of every row
    gather : bool     //! the device gathers the token rows (`with_gpu_resident_embed`), else the host embeds them
    quant : QuantMode   //! a CPU form's load mode: q8 the CPU chain, fp32 the file's f32 chain, q4_0 the q4 serving mode
    regions : int64   //! a device form's mirror regions (`load_resident`'s `regions`): 0 the driver's own count
    host : bool       //! a device form keeps the weight planes on the host beside the device's copy (`load_resident_host`)
    split_forms : bool   //! a device form loads with the Vulkan fuse knob off - the split one-row forms; no knob off Vulkan
    ctx : int64       //! a device form's mirror context: 0 the call's
    host_layers : int64   //! routed layers' experts asked onto the host
    pf_stream : bool      //! a host-layers form's prompt streams, else keeps the host's sums
    pf_window_rows : int64   //! the prompt window pinned for the form: 0 the cells'
    //! the depth the load prepares verify rows for (`set_mtp_depth`):
    //! 0 the depth the call found
    mtp_depth : int64
    //! the NextN draft seat unset for the load (`unset_rdec_draft`):
    //! the plan keeps the head off the device
    head_off : bool
    //! the CUDA home's MMQ route pinned `CUDA_MMQ_OFF` for the load:
    //! the f16-fed cm2 tiles; no knob off the CUDA home
    mmq_off : bool
    //! the host-cached sessions' context where past 0, cut apart from
    //! the mirror's (`ctx`): 0 the mirror's
    host_ctx : int64
    //! DASLLAMA_VK_FUSE_BISECT pinned for the load (`vk_fuse_bisect_pin`):
    //! -1 the environment's; no knob off Vulkan
    fuse_bisect : int64 = -1l
    //! the whole-model route pinned off for the load: the per-op rails,
    //! the driver held off the model through the form
    perop : bool
    //! the hot expert pool's slots a layer asked through the tier's want
    //! (`want_heat`) for the load: 0 the want as the call found it
    pool_slots : int64
}

//! a device form on the environment's mirror codec
def device_form(ring : bool = true; gather : bool = true; regions : int64 = 0l; host : bool = false; split_forms : bool = false;
                ctx : int64 = 0l; host_layers : int64 = 0l; pf_stream : bool = true;
                pf_window_rows : int64 = 0l; mtp_depth : int64 = 0l; head_off : bool = false;
                mmq_off : bool = false; host_ctx : int64 = 0l; fuse_bisect : int64 = -1l;
                perop : bool = false; pool_slots : int64 = 0l) : ModelForm =>
    ModelForm(on_gpu = true, kv = KVDtype.f16, kv_asked = false, ring = ring, gather = gather,
        quant = QuantMode.q8, regions = regions, host = host, split_forms = split_forms, ctx = ctx,
        host_layers = host_layers, pf_stream = pf_stream, pf_window_rows = pf_window_rows,
        mtp_depth = mtp_depth, head_off = head_off, mmq_off = mmq_off, host_ctx = host_ctx,
        fuse_bisect = fuse_bisect, perop = perop, pool_slots = pool_slots)

//! a per-op form for each cell of `run` (`perop_next`): a cell a load, as the census counts a kernel as its command records
//! and a later session replays it
def perop_forms(run : PeropRun) : array<ModelForm> => <- [for (_c in run.cells); device_form(perop = true)]

//! a device form whose mirror asks the codec `kv` (`asking`), every other knob as `device_form`'s
def device_form(kv : KVDtype; ring : bool = true; gather : bool = true; ctx : int64 = 0l; pf_window_rows : int64 = 0l;
                regions : int64 = 0l; host : bool = false; host_ctx : int64 = 0l; perop : bool = false;
                split_forms : bool = false; host_layers : int64 = 0l; pf_stream : bool = true; mtp_depth : int64 = 0l;
                head_off : bool = false; mmq_off : bool = false; fuse_bisect : int64 = -1l;
                pool_slots : int64 = 0l) : ModelForm =>
    asking(device_form(ring = ring, gather = gather, regions = regions, host = host, split_forms = split_forms, ctx = ctx,
        host_layers = host_layers, pf_stream = pf_stream, pf_window_rows = pf_window_rows, mtp_depth = mtp_depth,
        head_off = head_off, mmq_off = mmq_off, host_ctx = host_ctx, fuse_bisect = fuse_bisect, perop = perop,
        pool_slots = pool_slots), kv)

//! the form `f` with its mirror asking the codec `kv` (`with_gpu_kv_dtype`)
def asking(f : ModelForm; kv : KVDtype) : ModelForm {
    var g = f
    g.kv = kv
    g.kv_asked = true
    return g
}

//! `base` asking each codec of `kvs` (`asking`) pushed onto `forms`; a codec the environment keeps off (`codec_env_why`) held
//! into `h` as a skip, `what` naming the cells it drops
def codec_forms(var forms : array<ModelForm>; var h : Held; kvs : array<KVDtype>; base : ModelForm;
                what : block<(kv : KVDtype) : string>) {
    for (kv in kvs) {
        let why = codec_env_why(kv)
        if (empty(why)) {
            forms |> push(asking(base, kv))
        } else {
            h |> held_skip("{invoke(what, kv)}: {why}")
        }
    }
}

//! a CPU form loaded in the mode `quant` past the CPU chain's q8: `QuantMode.fp32` the file's f32 chain, `QuantMode.q4_0` the q4
//! serving mode
def cpu_quant_form(quant : QuantMode) : ModelForm =>
    ModelForm(on_gpu = false, kv = KVDtype.f16, kv_asked = false, ring = true, gather = true,
        quant = quant, pf_stream = true)

//! the CPU form on the file's f32 chain
def f32_form() : ModelForm => cpu_quant_form(QuantMode.fp32)

//! the driver's default form on the device: the environment's codec, the rings, the token gather, the load's own regions,
//! context and planes
def device_default(f : ModelForm) : bool => (f.on_gpu && !f.kv_asked && f.ring && f.gather && f.regions == 0l && !f.host && !f.split_forms
    && f.ctx == 0l && f.mtp_depth == 0l && !f.head_off && !f.mmq_off && f.host_ctx == 0l && f.fuse_bisect < 0l
    && !f.perop && f.pool_slots == 0l)

//! which side of a block's default pair a form is (`main_side`)
enum MainSide {
    device   //! the driver's default form on the device
    cpu      //! the CPU form on the codec the default form armed
    neither
}

//! the driver's default form and the CPU form answering it: whether
//! the default form ran, and the codec its mirror armed
struct MainPair {
    ran : bool
    kv : KVDtype
}

//! the side of `p` the form `f` is, the default form marked as run
//! in `p` (its armed codec the CPU side's)
def main_side(var p : MainPair; f : ModelForm) : MainSide {
    if (device_default(f)) {
        p.ran = true
        p.kv = f.kv
        return MainSide.device
    }
    return MainSide.cpu if (!f.on_gpu && f.quant == QuantMode.q8 && p.ran && f.kv == p.kv)
    return MainSide.neither
}

//! a device form that loads off the file's device lane where the
//! rail is on: every expert on the device (a lane bakes its
//! host-layers split and its pool) and no host planes
def private is_lane_form(f : ModelForm) : bool => f.on_gpu && !f.host && f.host_layers == 0l && !f.perop && f.pool_slots == 0l

//! the form as its messages name it
def form_name(f : ModelForm) : string {
    return "the {f.quant} CPU form" if (!f.on_gpu && f.quant != QuantMode.q8)
    return "the CPU form ({f.kv} sessions)" if (!f.on_gpu)
    let regions = f.regions > 0l ? ", {f.regions} regions" : ""
    let planes = f.host ? ", host planes" : ""
    let fuse = f.split_forms ? ", split forms" : ""
    let stream = f.pf_stream ? "" : ", the prompt stream off"
    let experts = f.host_layers > 0l ? ", {f.host_layers} routed layers on the host{stream}" : ""
    let window = f.pf_window_rows > 0l ? ", {f.pf_window_rows}-row prompt windows" : ""
    let depth = f.mtp_depth > 0l ? ", depth {f.mtp_depth}" : ""
    let head = f.head_off ? ", the NextN head off the device" : ""
    let mmq = f.mmq_off ? ", the MMQ route off" : ""
    let host_ctx = f.host_ctx > 0l ? ", {f.host_ctx}-position host sessions" : ""
    let bisect = f.fuse_bisect >= 0l ? ", fuse bisect {f.fuse_bisect}" : ""
    let heat = f.pool_slots > 0l ? ", {f.pool_slots} pool slots" : ""
    let shape = "{f.ring ? "rings" : "flat"}, {f.gather ? "device gather" : "host embed"}"
    return "the per-op form ({f.kv} asked, the whole-model route off)" if (f.perop)
    return "the device form ({f.kv} mirror, {shape}{regions}{planes}{fuse}{experts}{window}{depth}{head}{mmq}{host_ctx}{bisect}{heat})"
}

//! where a block's seconds went, a line a form: under `DAS_LOG_LEVEL=info`
def private log_form_seconds(f : ModelForm; load_usec, cells_usec : int; how : string = "") {
    to_log(LOG_INFO, "FORM {form_name(f)}{how}: load {float(load_usec) * 1e-6f:.1f} s, cells {float(cells_usec) * 1e-6f:.1f} s\n")
}

//! every lane beside `path` that `before` does not name goes: the block leaves the folder as it found it. Where a name of
//! `before` is gone, a load adopted an image that stood there under its lane's name (`image_path_for`), and nothing goes:
//! false then, true where the folder is back to `before`
def private remove_minted_lanes(path : string; before : array<string>) : bool {
    var inscope now <- dlim_names_beside(path)
    return false if (!empty([for (name in before); name; where !has_value(now, name)]))
    for (name in now) {
        if (!has_value(before, name)) {
            remove(path_join(dir_name(path), name))
        }
    }
    return true
}

//! one device form: the load under the form's knobs, `body` once on it, the device witnesses, the model gone before
//! the return; `ran` false after the loud skip that says why the form did not arm
def private device_form_run(t : T?; path : string; ctx : int64; form : ModelForm; lane : bool;
                            body : block<(var m : Model; form : ModelForm) : void>) : tuple<ran : bool; kv : KVDtype> {
    let levers = resident_levers()
    let pins = stream_pins()
    if (form.pf_window_rows > 0l) {
        set_pf_window_rows_pin(form.pf_window_rows)   //! the cells read the window live
    }
    if (form.host_layers > 0l) {
        set_gpu_want_pf_stream(form.pf_stream ? 1 : 0)
    }
    if (form.pool_slots > 0l) {
        want_heat(form.pool_slots)   //! the plan seats the pool at load; the stream pins put the want back
    }
    let depth = mtp_depth_pin()
    if (form.mtp_depth > 0l) {
        set_mtp_depth(form.mtp_depth)   //! the driver prepares the verify rows at the knob at load
    }
    let bisect = fuse_bisect_found()
    if (form.fuse_bisect >= 0l) {
        pin_fuse_bisect(form.fuse_bisect)   //! the decode plan reads the bits at load
    }
    var out = (ran = false, kv = form.kv)
    if (form.mmq_off) {
        static_if (typeinfo builtin_module_exists(cuda)) {
            with_cuda_mmq_pin(CUDA_MMQ_OFF) {
                out = device_form_pinned(t, path, ctx, form, lane, body)
            }
        } else {
            skip_cell(t, "{form_name(form)}: no CUDA module on this build - the MMQ route is the CUDA driver's")
        }
    } else {
        out = device_form_pinned(t, path, ctx, form, lane, body)
    }
    pin_fuse_bisect(bisect)
    restore_mtp_depth(depth)
    restore_resident_levers(levers)
    restore_stream_pins(pins)
    return out
}

//! the fuse bisect's pin as found (`g_vk_fuse_bisect_pin`): -1 with no vulkan module
def private fuse_bisect_found() : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_vk_fuse_bisect_pin
    } else {
        return -1l
    }
}

//! the fuse bisect pinned at `bits` (-1 the environment's); nothing with no vulkan module
[unused_argument(bits)]
def private pin_fuse_bisect(bits : int64) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vk_fuse_bisect_pin(bits)
    }
}

//! `device_form_run` under the form's prompt pins
def private device_form_pinned(t : T?; path : string; ctx : int64; form : ModelForm; lane : bool;
                               body : block<(var m : Model; form : ModelForm) : void>
                               ) : tuple<ran : bool; kv : KVDtype> {
    var out = (ran = false, kv = form.kv)
    with_gpu_kv_ring(form.ring ? 1 : 0) {
        with_gpu_resident_embed(form.gather ? 1 : 0) {
            if (form.kv_asked) {
                with_gpu_kv_dtype(t, form.kv) {
                    out = device_form_load(t, path, ctx, form, lane, body)
                }
            } else {
                out = device_form_load(t, path, ctx, form, lane, body)
            }
        }
    }
    return out
}

def private device_form_load(t : T?; path : string; call_ctx : int64; form : ModelForm; lane : bool;
                             body : block<(var m : Model; form : ModelForm) : void>) : tuple<ran : bool; kv : KVDtype> {
    let ctx = form.ctx > 0l ? form.ctx : call_ctx
    return perop_form_load(t, path, ctx, form, body) if (form.perop)
    let levers = resident_levers()
    set_gpu_ctx_max(ctx)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (form.split_forms) {
            g_vk_fuse = 0   //! the split one-row forms for the load; the levers put the knob back after it
        }
    }
    let t0 = ref_time_ticks()
    var dev <- resident_trunk(t, path, form.host_ctx > 0l ? form.host_ctx : ctx, "") $() : Model {
        return <- load_resident_lane(path, form.regions) if (lane)
        var seat : RdecDraftFn
        if (form.head_off) {
            seat = unset_rdec_draft()
        }
        var m <- load_resident_as(form.host, path, form.regions, 0l, form.host_layers)
        if (form.head_off) {
            reinstall_rdec_draft(seat)
        }
        return <- m
    }
    restore_resident_levers(levers)
    var out = (ran = false, kv = form.kv)
    if (dev.config.n_layers > 0l) {
        var armed = form
        armed.kv = mirror_kv_dtype()
        if (lane) {
            var inscope lanes <- dlim_names_beside(path)
            t |> success(!empty(lanes), "{form_name(armed)} loaded through the image rail: the file's lane stands beside it")
        }
        let t1 = ref_time_ticks()
        invoke(body, dev, armed)
        log_form_seconds(armed, get_time_usec(t0) - get_time_usec(t1), get_time_usec(t1), lane ? ", off the lane" : "")
        t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through {form_name(armed)}")
        served_every_call(t)
        moe_gpu_drop_model()
        out = (ran = true, kv = armed.kv)
    }
    delete dev
    return out
}

//! a per-op form: `path` loaded with the whole-model route pinned off - the per-op rails - its sessions' context cut to `ctx`,
//! `body` once on it, the driver held off the model through the form, its device state dropped and the model gone before the
//! return
def private perop_form_load(t : T?; path : string; ctx : int64; form : ModelForm;
                            body : block<(var m : Model; form : ModelForm) : void>) : tuple<ran : bool; kv : KVDtype> {
    let levers = resident_levers()
    set_gpu_resident_route(false)
    let t0 = ref_time_ticks()
    var m <- load_model_(path, QuantMode.q8)
    restore_resident_levers(levers)
    m.config.seq_len = min(m.config.seq_len, ctx)
    t |> success(!moe_gpu_resident_active(), "the route lever kept the whole-model driver off the model through {form_name(form)}")
    let t1 = ref_time_ticks()
    invoke(body, m, form)
    log_form_seconds(form, get_time_usec(t0) - get_time_usec(t1), get_time_usec(t1))
    t |> success(!moe_gpu_resident_active(), "the whole-model driver stayed off through {form_name(form)}")
    moe_gpu_drop_model()
    delete m
    return (ran = true, kv = form.kv)
}

//! the CPU form of `path` in the load mode `quant`, loaded once: `body` on it once a codec of `kvs`
def private cpu_form_run(t : T?; path : string; ctx : int64; quant : QuantMode; kvs : array<KVDtype>;
                         body : block<(var m : Model; form : ModelForm) : void>) {
    let levers = resident_levers()
    if (pf_window_rows_pin() <= 0l) {
        set_pf_window_rows_pin(PF_WINDOW_ROWS)   //! the prompt window a load latches: the cells' own, as `load_resident` pins it
    }
    let t0 = ref_time_ticks()
    var cpu <- load_model_(path, quant)
    restore_resident_levers(levers)
    moe_gpu_drop_model()   //! whatever device state the load installed goes: the form is the CPU chain's
    cpu.config.seq_len = min(cpu.config.seq_len, ctx)
    var f = cpu_quant_form(quant)
    t |> success(!moe_gpu_resident_active(), "no driver is armed through {form_name(f)}")
    let t1 = ref_time_ticks()
    for (kv in kvs) {
        f.kv = kv
        invoke(body, cpu, f)
    }
    log_form_seconds(f, get_time_usec(t0) - get_time_usec(t1), get_time_usec(t1))
    delete cpu
}

//! The model file `file` (its name under `models_dir()`) for the cells of the run area `area` (llm, vision, audio) - both
//! literals: `REVIEW.das` reads them to hold one call an area and file across the folder - in each form of `forms`, strictly one
//! loaded model at a time. Each device form loads once with its knobs applied (its mirror cut to `ctx`, or to the form's own
//! context), `body` runs on it once, and the driver is held to the device: every call served, still armed at its end; a form the
//! environment cannot arm skips loudly and `body` never sees it. The first device form streams the file as a load with no
//! image does; where the list holds `LANE_FORMS` such forms or more and the image rail is on, each later one maps the file's
//! device lane, minted once beside the file and removed after the last form unless it stood there before
//! (`../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`). Then, unless the call asks none, the CPU form loads once and `body` runs on it once a
//! distinct codec of the device forms that ran, and each other CPU load mode in `forms` (`cpu_quant_form`) loads once after it
//! for one more run on the first of those codecs. `body` collects what a compare between forms reads - rows, sums,
//! counts - into the caller's state, and marks there which forms it saw; those compares follow the call, and a cell of one
//! form alone may assert inside `body`. False after the loud skip that says why no
//! device form ran.
let private LANE_FORMS = 3   //! a mint costs a streamed load and a write, so it pays from the third form

def with_cpu_and_gpu_model(t : T?; area, file : string; ctx : int64; forms : array<ModelForm>;
                           body : block<(var m : Model; form : ModelForm) : void>) : bool =>
    with_cpu_and_gpu_model(t, area, file, ctx, forms, true, body)

//! the same where `cpu` says whether the CPU form loads: false for a block whose every cell reads the device alone - a
//! carrier the CPU form would load whole for no compare
def with_cpu_and_gpu_model(t : T?; area, file : string; ctx : int64; forms : array<ModelForm>; cpu : bool;
                           body : block<(var m : Model; form : ModelForm) : void>) : bool {
    let path = path_join(models_dir(), file)
    return false if (!resident_ready(t, path))
    to_log(LOG_INFO, "{area}: {file} in {length(forms)} forms and the CPU's, one at a time\n")
    var kvs : array<KVDtype>
    let lane_forms = length([for (f in forms); f; where is_lane_form(f)])
    let rail = lane_forms >= LANE_FORMS && g_env_engine.image && image_save_enabled()
    var inscope before <- dlim_names_beside(path)
    with_dasllama_jobque_() {
        var lane_forms_before = 0
        for (form in forms) {
            continue if (!form.on_gpu)
            let lane = rail && is_lane_form(form) && lane_forms_before > 0
            if (is_lane_form(form)) {
                lane_forms_before++
            }
            let armed_out = device_form_run(t, path, ctx, form, lane, body)
            if (armed_out.ran && !has_value(kvs, armed_out.kv)) {
                kvs |> push(armed_out.kv)
            }
        }
        if (remove_minted_lanes(path, before)) {
            var inscope after <- dlim_names_beside(path)
            t |> equal(length(after), length(before), "the block left the lanes beside {file} as it found them: {after}")
        }
        if (!empty(kvs) && cpu) {
            cpu_form_run(t, path, ctx, QuantMode.q8, kvs, body)
            var done : array<QuantMode>
            for (form in forms) {
                if (!form.on_gpu && form.quant != QuantMode.q8 && !has_value(done, form.quant)) {
                    done |> push(form.quant)
                    cpu_form_run(t, path, ctx, form.quant, [kvs[0]], body)
                }
            }
            delete done
        }
    }
    let ran = !empty(kvs)
    delete kvs
    return ran
}

//! the driver's default form on the device, then the CPU form
def with_cpu_and_gpu_model(t : T?; area, file : string; ctx : int64; body : block<(var m : Model; form : ModelForm) : void>) : bool =>
    with_cpu_and_gpu_model(t, area, file, ctx, [device_form()], body)

//! a forced feed off one form: the prefill's row, then each fed step's, and what the device counted serving them
struct FeedRows {
    nprompt : int64
    toks : array<int64>
    rows : array<array<float>>
    prefills : int64   //! resident window chains the feed's prompt took
    cuda : int64       //! positions the CUDA driver served, -1 off the CUDA home
    pieces : table<int64; string>   //! every row's argmax decoded, for the compares after the load
}

//! `prompt` then `toks` through a fresh session of the form's
//! codec, inside the engine's job queue
def feed_rows(m : Model; form : ModelForm; prompt, toks : array<int64>) : FeedRows =>
    <- feed_rows(m, form, prompt, toks) $ {}

//! the same with `after_prefill` between the prefill and the first
//! fed step: a cell's reads of what the prefill alone moved
def feed_rows(m : Model; form : ModelForm; prompt, toks : array<int64>;
              after_prefill : block<void>) : FeedRows {
    var f : FeedRows
    f.nprompt = long_length(prompt)
    f.toks := toks
    let p0 = gpu_resident_prefills_()
    let c0 = cuda_positions_served()
    var s <- create_session(m, form.kv)
    f.rows <- feed_logits(m, s, prompt, toks, after_prefill)
    delete s
    f.prefills = gpu_resident_prefills_() - p0
    f.cuda = c0 >= 0l ? cuda_positions_served() - c0 : -1l
    for (row in f.rows) {
        keep_piece(f.pieces, m, argmax_of(row))
    }
    return <- f
}

//! the prompt's row and the first `steps` fed steps of `f`: a longer
//! CPU feed held against a shorter device feed
def feed_prefix(f : FeedRows; steps : int64) : FeedRows {
    var p = FeedRows(nprompt = f.nprompt, prefills = f.prefills, cuda = f.cuda,
        toks <- [for (i in range64(steps)); f.toks[i]])
    p.pieces := f.pieces
    p.rows |> reserve(steps + 1l)
    for (i in range64(steps + 1l)) {
        p.rows |> push_clone(f.rows[i])
    }
    return <- p
}

//! `n` ids of prose prefilled and `steps` fed after them, off one form
def prose_feed_rows(m : Model; form : ModelForm; n : int64; steps : int64 = 8l) : FeedRows {
    var inscope feed <- prose_feed(m, n, steps)
    return <- feed_rows(m, form, feed.prompt, feed.toks)
}

//! `n` ids of `resident_prompt` prefilled and `steps` of
//! `resident_feed` fed after them, off one form
def resident_feed_rows(m : Model; form : ModelForm; n, steps : int64) : FeedRows {
    var inscope prompt <- resident_prompt(n)
    var inscope toks <- resident_feed(steps)
    return <- feed_rows(m, form, prompt, toks)
}

//! `id` decoded, as one of two feeds' argmax pieces holds it; ""
//! where neither feed's argmax was `id`
def feed_piece(a, b : FeedRows; id : int64) : string => a.pieces?[id] ?? (b.pieces?[id] ?? "")

//! the device form's feed against a CPU form's (`cpu_name` the chain it ran): the device served the prompt as one window chain
//! (and every position on the CUDA home, where that home serves), and every row sits within `bar_rel` of the CPU row's largest
//! live logit, its one-step-off control outside
def feed_rows_within(t : T?; tag : string; gpu, cpu : FeedRows; bar_rel : float; cpu_name : string = "the CPU chain") {
    t |> equal(gpu.prefills, 1l, "{tag}the resident window chain served the {gpu.nprompt}-token prompt")
    t |> equal(cpu.prefills, 0l, "{tag}{cpu_name} took no resident window chain")
    if (gpu.cuda >= 0l) {
        t |> equal(gpu.cuda, gpu.nprompt + long_length(gpu.toks), "{tag}the CUDA driver served every position of the feed")
    }
    feed_steps_within(t, tag, gpu.nprompt, gpu.rows, cpu.rows, gpu.toks, bar_rel, "resident",
            cpu_name) $(id) {
        return feed_piece(gpu, cpu, id)
    }
}

//! the fed steps a CUDA home's feed takes: `cuda_feed_served` counts
//! the prompt and eight steps
let DENSE_FED_STEPS = 8l

//! `resident_feed_rows` of an `n`-id prompt on the CUDA home, the
//! driver's counters `with_cuda_feed_served` reads - every position,
//! `windows` window chains, the captured graph - held into `h`; no
//! rows on a build with no CUDA module
def cuda_feed_rows(var h : Held; m : Model; form : ModelForm; n, windows : int64) : FeedRows {
    var f : FeedRows
    static_if (typeinfo builtin_module_exists(cuda)) {
        with_cuda_feed_served(h, n, windows) {
            delete f
            f <- resident_feed_rows(m, form, n, DENSE_FED_STEPS)
        }
    }
    return <- f
}

//! `run`, a CPU form's feed, under the CUDA counters' control where
//! `on_cuda`: the driver's counters stay (`with_cuda_counters_still`)
def with_cpu_counters_still(var h : Held; on_cuda : bool; run : block<void>) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (on_cuda) {
            with_cuda_counters_still(h, run)
            return
        }
    }
    invoke(run)
}

let GATHER_PROMPT = 40l   //! the gather cell's prompt: one window
let GATHER_STEPS = 6l   //! ... and its fed steps

//! the one-row steps whose row the device gathered off the step's token id so far (NO_CENSUS when the module is absent)
def gathered_rows() : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_token_gathered()
    } else {
        return NO_CENSUS
    }
}

//! the census name of the token-table gather's stamp for a table in `f`
def emb_kernel(f : KqFmt) : string => f == KqFmt.q8 ? "emb_gather_cls_spv" : (kq_block32(f) ? "{f}_emb_cls_spv" : "kq_emb_{f}_cls_spv")

//! the gather cell's halves on `file`: the device's forced feed with what the window's gather and every fed step dispatched -
//! the table's form and the served witness held - and the CPU chain's feed of the same tokens
struct EmbGather {
    ran : bool
    h : Held
    gpu : FeedRows
    cpu : FeedRows
    census : bool
    kernel : string
    fmt : string
    pf_emb : int64
    st_rows : int64
}

//! the gather cell's device half on a form of `file`: the window's token rows and every fed step's row gathered on the device
//! off the table's plane (the tied classifier's where the table is tied)
def emb_gather_rows(var g : EmbGather; tr : Model; form : ModelForm; file : string) {
    g.h |> held_fact(tr.config.shared_weights && emb_kq(tr), "{file}: the table is tied and kept as the classifier's {tr.emb_fmt} plane")
    g.kernel = emb_kernel(tr.emb_fmt)
    g.fmt = "{tr.emb_fmt}"
    var prompt <- resident_prompt(GATHER_PROMPT)
    var toks <- resident_feed(GATHER_STEPS)
    let emb0 = census(g.kernel)
    var rows0 = 0l
    g.gpu <- feed_rows(tr, form, prompt, toks) $ {
        g.pf_emb = census(g.kernel) - emb0
        rows0 = gathered_rows()
    }
    g.st_rows = gathered_rows() - rows0
    g.census = emb0 != NO_CENSUS
    g.h |> held_fact(moe_gpu_resident_active(), "the whole-model driver stayed armed through the run")
    g.h |> held_served()
    g.ran = true
    delete prompt
    delete toks
}

//! the gather cell's compares: the device's feed against the CPU chain's at `bar_rel`, and where the census reads, the window's
//! token rows gathered off the tied plane and every fed step's row off its id
def emb_gather_within(t : T?; g : EmbGather; file : string; bar_rel : float) {
    return if (!g.ran)
    held_within(t, g.h)
    feed_rows_within(t, "{file} gather ", g.gpu, g.cpu, bar_rel)
    if (g.census) {
        t |> success(g.pf_emb >= 1l, "{file}: the window gathered its token rows off the tied {g.fmt} plane ({g.kernel}, {g.pf_emb} dispatches)")
        t |> equal(g.st_rows, GATHER_STEPS, "{file}: every fed step's row gathered on the device off its token id, no host row uploaded")
    }
}

//! a dense file's forced feeds on its default form: the prompts, the default form's rows - on the CUDA home under the driver's
//! counters (`cuda_feed_rows`), else the resident feed - the CPU form's on the default form's codec, the first under the
//! counters' control on the CUDA home, and what the device held (`carrier`)
struct DenseFeeds {
    prompts : array<int64>
    cuda : bool
    main : MainPair
    carrier : Held
    gpu : array<FeedRows>
    cpu : array<FeedRows>
}

//! the feeds `prompts` on the armed home, a prompt past one window held as a skip where the prompt window is not
//! PF_WINDOW_ROWS rows
def dense_feeds(var c : DenseFeeds; prompts : array<int64>) {
    c.cuda = gpu_want_cuda()
    for (n in prompts) {
        if (n > PF_WINDOW_ROWS && !held_pf_window(c.carrier)) {
            continue
        }
        c.prompts |> push(n)
    }
}

//! one form's share of the feeds: the default form's rows, the CPU form's on its codec; the side the form is
def dense_feeds_rows(var c : DenseFeeds; m : Model; form : ModelForm) : MainSide {
    let side = main_side(c.main, form)
    if (side == MainSide.device) {
        c.gpu |> reserve(length(c.prompts))
        for (n in c.prompts) {
            if (c.cuda) {
                c.gpu |> emplace <| cuda_feed_rows(c.carrier, m, form, n, prompt_windows(n))
            } else {
                c.gpu |> emplace <| resident_feed_rows(m, form, n, DENSE_FED_STEPS)
            }
        }
    } elif (side == MainSide.cpu) {
        c.cpu |> reserve(length(c.prompts))
        for (n, i in c.prompts, count()) {
            with_cpu_counters_still(c.carrier, c.cuda && i == 0) {
                c.cpu |> emplace <| resident_feed_rows(m, form, n, DENSE_FED_STEPS)
            }
        }
    }
    return side
}

//! the feeds' compares after the block: what the device held, then each feed's device rows against the CPU form's within
//! `bar_rel`, the device serving the prompt as one window chain (and every position on the CUDA home)
def dense_feeds_within(t : T?; tag : string; c : DenseFeeds; bar_rel : float) {
    return if (!c.main.ran)
    held_within(t, c.carrier, "{tag}: ", false)
    with_dasllama_jobque_() {
        for (f, i in c.gpu, count()) {
            let what = "{tag}{c.cuda ? " cuda" : ""}, {f.nprompt} tokens: "
            continue if (!twin_collected(t, i, length(c.cpu), what))
            feed_rows_within(t, what, f, c.cpu[i], bar_rel)
        }
    }
}

//! whether the device feed `i` has its CPU twin among the `ncpu` feeds the CPU form collected; false after a failure naming
//! the feed (`what`) where it has none - the CPU form runs on every codec a device form armed, so a missing twin is a
//! compare lost, never a skip
def twin_collected(t : T?; i, ncpu : int; what : string) : bool {
    return true if (i < ncpu)
    t |> failure("{what}the CPU form collected no twin of this device feed ({ncpu} collected)")
    return false
}
