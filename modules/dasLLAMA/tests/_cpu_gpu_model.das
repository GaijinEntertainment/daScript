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
require ?vulkan dasllama/dasllama_vulkan_common   // g_vk_fuse: a split-forms device form's knob
require math   // min
require daslib/fio   // path_join
require _model_tier public   // model_available, mirror_kv_dtype
require _resident_feed public   // resident_trunk, load_resident, the levers, feed_alone, the served witnesses
require _compares

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
}

//! a device form on the environment's mirror codec
def device_form(ring : bool = true; gather : bool = true; regions : int64 = 0l; host : bool = false; split_forms : bool = false;
                ctx : int64 = 0l) : ModelForm =>
    ModelForm(on_gpu = true, kv = KVDtype.f16, kv_asked = false, ring = ring, gather = gather, quant = QuantMode.q8, regions = regions,
        host = host, split_forms = split_forms, ctx = ctx)

//! a device form whose mirror asks the codec `kv`
def device_form(kv : KVDtype; ring : bool = true; gather : bool = true) : ModelForm =>
    ModelForm(on_gpu = true, kv = kv, kv_asked = true, ring = ring, gather = gather, quant = QuantMode.q8)

//! a CPU form loaded in the mode `quant` past the CPU chain's q8: `QuantMode.fp32` the file's f32 chain, `QuantMode.q4_0` the q4
//! serving mode
def cpu_quant_form(quant : QuantMode) : ModelForm =>
    ModelForm(on_gpu = false, kv = KVDtype.f16, kv_asked = false, ring = true, gather = true, quant = quant)

//! the CPU form on the file's f32 chain
def f32_form() : ModelForm => cpu_quant_form(QuantMode.fp32)

//! the driver's default form on the device: the environment's codec, the rings, the token gather, the load's own regions,
//! context and planes
def device_default(f : ModelForm) : bool => (f.on_gpu && !f.kv_asked && f.ring && f.gather && f.regions == 0l && !f.host && !f.split_forms
    && f.ctx == 0l)

//! the form as its messages name it
def form_name(f : ModelForm) : string {
    return "the {f.quant} CPU form" if (!f.on_gpu && f.quant != QuantMode.q8)
    return "the CPU form ({f.kv} sessions)" if (!f.on_gpu)
    let regions = f.regions > 0l ? ", {f.regions} regions" : ""
    let planes = f.host ? ", host planes" : ""
    let fuse = f.split_forms ? ", split forms" : ""
    return "the device form ({f.kv} mirror, {f.ring ? "rings" : "flat"}, {f.gather ? "device gather" : "host embed"}{regions}{planes}{fuse})"
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
    let levers = resident_levers()
    let ctx = form.ctx > 0l ? form.ctx : call_ctx
    set_gpu_ctx_max(ctx)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (form.split_forms) {
            g_vk_fuse = 0   //! the split one-row forms for the load; the levers put the knob back after it
        }
    }
    let t0 = ref_time_ticks()
    var dev <- resident_trunk(t, path, ctx, "") $() : Model {
        if (lane) {
            return <- load_resident_lane(path, form.regions)
        }
        return <- load_resident_as(form.host, path, form.regions)
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
//! (`../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`). Then the CPU form loads once and `body` runs on it once a
//! distinct codec of the device forms that ran, and each other CPU load mode in `forms` (`cpu_quant_form`) loads once after it
//! for one more run on the first of those codecs. `body` collects what a compare between forms reads - rows, sums,
//! counts - into the caller's state, and marks there which forms it saw; those compares follow the call, and a cell of one
//! form alone may assert inside `body`. False after the loud skip that says why no
//! device form ran.
let private LANE_FORMS = 3   //! a mint costs a streamed load and a write, so it pays from the third form

def with_cpu_and_gpu_model(t : T?; area, file : string; ctx : int64; forms : array<ModelForm>;
                           body : block<(var m : Model; form : ModelForm) : void>) : bool {
    let path = path_join(models_dir(), file)
    return false if (!resident_ready(t, path))
    to_log(LOG_INFO, "{area}: {file} in {length(forms)} forms and the CPU's, one at a time\n")
    var kvs : array<KVDtype>
    let streamed_forms = length([for (f in forms); f; where f.on_gpu && !f.host])
    let rail = streamed_forms >= LANE_FORMS && g_env_engine.image && image_save_enabled()
    var inscope before <- dlim_names_beside(path)
    with_dasllama_jobque_() {
        var streamed_before = 0
        for (form in forms) {
            continue if (!form.on_gpu)
            let lane = rail && !form.host && streamed_before > 0
            if (!form.host) {
                streamed_before++
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
        if (!empty(kvs)) {
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
}

//! `prompt` then `toks` through a fresh session of the form's codec
def feed_rows(m : Model; form : ModelForm; prompt, toks : array<int64>) : FeedRows {
    var f : FeedRows
    f.nprompt = long_length(prompt)
    f.toks := toks
    let p0 = gpu_resident_prefills_()
    let c0 = cuda_positions_served()
    f.rows <- feed_alone(m, form.kv, prompt, toks)
    f.prefills = gpu_resident_prefills_() - p0
    f.cuda = c0 >= 0l ? cuda_positions_served() - c0 : -1l
    return <- f
}

//! `n` ids of prose prefilled and `steps` fed after them, off one form
def prose_feed_rows(m : Model; form : ModelForm; n : int64; steps : int64 = 8l) : FeedRows {
    var inscope feed <- prose_feed(m, n, steps)
    return <- feed_rows(m, form, feed.prompt, feed.toks)
}

//! the device form's feed against a CPU form's (`cpu_name` the chain it ran): the device served the prompt as one window chain
//! (and every position on the CUDA home, where that home serves), and every row sits within `bar_rel` of the CPU row's largest
//! live logit, its one-step-off control outside
def feed_rows_within(t : T?; tag : string; gpu, cpu : FeedRows; bar_rel : float; cpu_name : string = "the CPU chain") {
    t |> equal(gpu.prefills, 1l, "{tag}the resident window chain served the {gpu.nprompt}-token prompt")
    t |> equal(cpu.prefills, 0l, "{tag}{cpu_name} took no resident window chain")
    if (gpu.cuda >= 0l) {
        t |> equal(gpu.cuda, gpu.nprompt + long_length(gpu.toks), "{tag}the CUDA driver served every position of the feed")
    }
    feed_steps_within(t, tag, gpu.nprompt, gpu.rows, cpu.rows, gpu.toks, bar_rel, "resident", cpu_name)
}
