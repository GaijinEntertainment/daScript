options gen2
options _dasllama_internal = true

module _cpu_gpu_model shared public

require dastest/testing_boost public
require daslib/jobque_boost
require dasllama/dasllama_common   // Model
require dasllama/dasllama   // load_model_, moe_gpu_drop_model
require dasllama/dasllama_gguf   // set_mtp_head_path: a form's head pin
require dasllama/dasllama_kv_dtype   // KVDtype
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_core   // resident_prefill_allowed: the pin a device form's cells leave as found
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, gpu_resident_prefills_
require dasllama/dasllama_gpu_tier   // set_gpu_ctx_max
require dasllama/dasllama_env   // the context pins, the image rail's switch
require dasllama/dasllama_image   // image_save_enabled, dlim_names_beside, load_model_uncached, the host_planes lever
require ?vulkan dasllama/dasllama_vulkan_common   // g_vk_fuse, the fuse bisect's pin: a device form's knobs
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_gathered: the gather cell's step rows
require dasllama/dasllama_kqformat   // kq_block32: the gather stamp's name
require ?cuda dasllama/dasllama_cuda_decode   // CUDA_MMQ_OFF: an MMQ-off device form's pin
require math   // min
require daslib/rtti   // this_context().last_exception: a fault-seat load's panic
require daslib/fio   // path_join
require _model_form public   // ModelForm and its pure parts: the knobs, names and pairing
require _model_tier public   // model_available, mirror_kv_dtype
require _resident_feed public   // resident_trunk, load_resident, the levers, feed_alone, the served witnesses
require _compares
require _gpu_tier_armed   // gpu_tier_why: the device forms skip where the tier is not armed
require _kv_codec_cells   // codec_env_why: a codec form the environment keeps off
require ?cuda _cuda_feed   // with_cuda_feed_served, with_cuda_counters_still: the CUDA home's feed witnesses
require _metal_blob_twin   // blob_twin, metal_home_seat: a Metal form's load and the home it puts back
require _kernel_census   // kernel_census_write: a GPU form's census file

//! a per-op form for each cell of `run` (`perop_next`): a cell a load, as the census counts a kernel as its command records
//! and a later session replays it
def perop_forms(run : PeropRun) : array<ModelForm> => <- [for (_c in run.cells); device_form(perop = true)]

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

//! `load` under the form's load-time pins, each put back after it: the head (`set_mtp_head_path`) where `f` names one, the
//! f32 weight scale where asked, the IQ3_S serving mode where named
def private load_pinned(f : ModelForm; load : block<() : Model>) : Model {
    let scale = get_wscale_f16()
    let iq3s = get_iq3s_serve_pin()
    if (f.wscale_f32) {
        set_wscale_f16(false)
    }
    if (!empty(f.iq3s_serve)) {
        set_iq3s_serve_pin(f.iq3s_serve)
    }
    if (!empty(f.mtp_head)) {
        set_mtp_head_path(path_join(models_dir(), f.mtp_head))
    }
    var m <- invoke(load)
    if (!empty(f.mtp_head)) {
        clear_mtp_head_path()
    }
    if (!empty(f.iq3s_serve)) {
        set_iq3s_serve_pin(iq3s)
    }
    set_wscale_f16(scale)
    return <- m
}

//! the image names a load of `path` writes beside it:
//! `dlim_names_beside`'s for a file; for a snapshot directory the
//! `.dlim` names beside it that open with its own name
//! (`image_path_for`)
def private lanes_beside(path : string) : array<string> {
    return <- dlim_names_beside(path) if (!stat(path).is_dir)
    let lead = "{base_name(path)}."
    var names : array<string>
    dir(dir_name(path)) $(name) {
        if (name |> starts_with(lead) && name |> ends_with(".dlim")) {
            names |> push(name)
        }
    }
    return <- names
}

//! every lane beside `path` that `before` does not name goes: the block leaves the folder as it found it. Where a name of
//! `before` is gone, a load adopted an image that stood there under its lane's name (`image_path_for`), and nothing goes:
//! false then, true where the folder is back to `before`
def private remove_minted_lanes(path : string; before : array<string>) : bool {
    var inscope now <- lanes_beside(path)
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
    let want = g_gpu_want
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
    let dense = gpu_dense_ffn_pin()
    if (form.dense_ffn >= 0) {
        set_gpu_want_dense_ffn(form.dense_ffn)   //! the dense walk marks the FFN triples at load
    }
    if (form.moe_layers > 0l) {
        want_routed_layers(t, form.moe_layers)
    }
    let out = device_form_seated(t, path, ctx, form, lane, body)
    pin_fuse_bisect(bisect)
    set_gpu_want_dense_ffn(dense)
    restore_mtp_depth(depth)
    restore_resident_levers(levers)
    restore_stream_pins(pins)
    if (form.moe_layers > 0l) {
        set_gpu_tier_want_(want)
        moe_gpu_tier_arm_()
        heat_reset()
    }
    return out
}

//! `device_form_run`'s load under the form's driver seat, a seat put back after the form's cells: the Vulkan coopmat mode,
//! the CUDA MMQ route or a CUDA fault seat; a skipped cell of its own where the build or the box lacks the seat's driver
def private device_form_seated(t : T?; path : string; ctx : int64; form : ModelForm; lane : bool;
                               body : block<(var m : Model; form : ModelForm) : void>) : tuple<ran : bool; kv : KVDtype> {
    var out = (ran = false, kv = form.kv)
    if (form.coopmat >= 0) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (g_gpu == null) {
                skip_cell(t, "{form_name(form)}: no Vulkan device armed - the coopmat mode is the Vulkan driver's")
            } else {
                let mode = g_gpu.coopmat_mode
                g_gpu.coopmat_mode = form.coopmat   //! the driver's prepare and the prompt's MoE setup read the mode at load
                out = device_form_pinned(t, path, ctx, form, lane, body)
                g_gpu.coopmat_mode = mode
            }
        } else {
            skip_cell(t, "{form_name(form)}: no Vulkan module on this build - the coopmat mode is the Vulkan driver's")
        }
    } elif (form.mmq_off) {
        static_if (typeinfo builtin_module_exists(cuda)) {
            with_cuda_mmq_pin(CUDA_MMQ_OFF) {
                out = device_form_pinned(t, path, ctx, form, lane, body)
            }
        } else {
            skip_cell(t, "{form_name(form)}: no CUDA module on this build - the MMQ route is the CUDA driver's")
        }
    } elif (form.cuda_window_fault) {
        static_if (typeinfo builtin_module_exists(cuda)) {
            out = device_form_pinned(t, path, ctx, form, lane, body)
        } else {
            skip_cell(t, "{form_name(form)}: no CUDA module on this build - the window-fault seat is the CUDA driver's")
        }
    } elif (form.cuda_head_warm_fault) {
        static_if (typeinfo builtin_module_exists(cuda)) {
            let was = set_cuda_head_warm_fault_(true)   // the head's planes are taken at its first prompt warm: the seat holds through the form's cells
            out = device_form_pinned(t, path, ctx, form, lane, body)
            set_cuda_head_warm_fault_(was)
        } else {
            skip_cell(t, "{form_name(form)}: no CUDA module on this build - the head-warm fault seat is the CUDA driver's")
        }
    } elif (form.cuda_mmq_fault) {
        static_if (typeinfo builtin_module_exists(cuda)) {
            let was = set_cuda_mmq_fault_(true)   // the chain's planes are taken where the routed stacks are sized, at the arm or after it: the seat holds through the form's cells
            out = device_form_pinned(t, path, ctx, form, lane, body)
            set_cuda_mmq_fault_(was)
        } else {
            skip_cell(t, "{form_name(form)}: no CUDA module on this build - the FP4-chain fault seat is the CUDA driver's")
        }
    } else {
        out = device_form_pinned(t, path, ctx, form, lane, body)
    }
    return out
}

//! the tier's want asking the last `resident` routed layers resident and the prompt streaming every layer below them, the
//! pool's slot ask kept, the tier re-armed on it and the heat counters reset before the load
def private want_routed_layers(t : T?; resident : int64) {
    var want = g_gpu_want
    want.moe_layers = resident
    want.moe_stream = -1l
    set_gpu_tier_want_(want)
    t |> success(moe_gpu_tier_arm_(), "the tier re-armed at {resident} resident routed layers")
    heat_reset()
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
    if (form.strict_ctx) {
        set_gpu_ctx_strict(true)
    }
    set_gpu_resident_block_rounds_(form.block_rounds)   //! every form writes the ask: the driver sizes the verify at load
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (form.split_forms) {
            g_vk_fuse = 0   //! the split one-row forms for the load; the levers put the knob back after it
        }
    }
    let t0 = ref_time_ticks()
    var dev <- resident_trunk(t, path, form.host_ctx > 0l ? form.host_ctx : ctx, "") $() : Model => <- load_pinned(form) $() : Model {
        return <- load_resident_lane(path, form.regions) if (lane)
        var seat : RdecDraftFn
        if (form.head_off) {
            seat = unset_rdec_draft()
        }
        var m <- form.cuda_window_fault ? faulted_load(t, form, path) : load_resident_as(form.host, path, form.regions, 0l, form.host_layers)
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
            var inscope lanes <- lanes_beside(path)
            t |> success(!empty(lanes), "{form_name(armed)} loaded through the image rail: the file's lane stands beside it")
        }
        let t1 = ref_time_ticks()
        g_drop_cells = 0
        let prefill_pin = resident_prefill_allowed()
        invoke(body, dev, armed)
        t |> equal(resident_prefill_allowed(), prefill_pin, "the cells of {form_name(armed)} left the resident prefill pin as they found it")
        set_resident_prefill_allowed(prefill_pin)   //! a cell that panicked under its pin: the later forms run unpinned
        log_form_seconds("{form_name(armed)}{lane ? ", off the lane" : ""}", get_time_usec(t0) - get_time_usec(t1), get_time_usec(t1))
        if (form.drops_device) {
            t |> equal(g_drop_cells, 1, "{form_name(armed)} ran the one drop cell it is marked for (`device_drop_cell`)")
        } else {
            t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through {form_name(armed)}")
        }
        served_every_call(t)
        kernel_census_write()
        moe_gpu_drop_model()
        out = (ran = true, kv = armed.kv)
    }
    delete dev
    return out
}

var private g_drop_cells = 0   //! the drop cells the running device form ran (`device_drop_cell`)

//! `cell`, which drops the device model, as the last cell of a device form marked `drops_device`: the driver held armed up to
//! it, one such cell a form; on any other form a failure, and `cell` does not run
def device_drop_cell(t : T?; form : ModelForm; cell : block<void>) {
    if (!form.on_gpu || !form.drops_device || g_drop_cells > 0) {
        t |> failure("a cell that drops the device model runs once, last, on a device form marked drops_device: {form_name(form)}, {g_drop_cells} run on it")
        return
    }
    t |> success(moe_gpu_resident_active(), "the whole-model driver stayed armed through {form_name(form)} up to its drop cell")
    g_drop_cells++
    invoke(cell)
}

//! the fault-seat form's load of `path`: the seat pinned on around it and put back whatever the load did (a seat left
//! pinned fails every later arm of the run), a panic inside it a failure; `device_form_run` runs the form on the CUDA build alone
def private faulted_load(t : T?; form : ModelForm; path : string) : Model {
    var m = Model()
    static_if (typeinfo builtin_module_exists(cuda)) {
        let was = set_cuda_window_fault_(true)
        try {
            delete m
            m <- load_resident_as(form.host, path, form.regions, 0l, form.host_layers)
        } recover {
            t |> success(false, "the load under {form_name(form)} panicked: {this_context().last_exception}")
        }
        set_cuda_window_fault_(was)
    }
    return <- m
}

//! a per-op form: `path` loaded with the whole-model route pinned off - the per-op rails - its sessions' context cut to `ctx`,
//! `body` once on it, the driver held off the model through the form, its device state dropped and the model gone before the
//! return
def private perop_form_load(t : T?; path : string; ctx : int64; form : ModelForm;
                            body : block<(var m : Model; form : ModelForm) : void>) : tuple<ran : bool; kv : KVDtype> {
    let levers = resident_levers()
    set_gpu_resident_route(false)
    moe_gpu_tier_arm_()   //! the form before dropped its model, which zeroes the tier's layer ask: the arm reads the want again
    let t0 = ref_time_ticks()
    var m <- load_model_(path, QuantMode.q8)
    restore_resident_levers(levers)
    m.config.seq_len = min(m.config.seq_len, ctx)
    t |> success(!moe_gpu_resident_active(), "the route lever kept the whole-model driver off the model through {form_name(form)}")
    let t1 = ref_time_ticks()
    invoke(body, m, form)
    log_form_seconds(form_name(form), get_time_usec(t0) - get_time_usec(t1), get_time_usec(t1))
    t |> success(!moe_gpu_resident_active(), "the whole-model driver stayed off through {form_name(form)}")
    kernel_census_write()
    moe_gpu_drop_model()
    delete m
    return (ran = true, kv = form.kv)
}

//! the block's Metal forms of `forms`, in the list's order, one load each (`metal_form_run`), each marked as it went; on a build
//! with no das_metal each is a skipped cell of its own and `body` never sees it (`metal_pass`)
def private metal_forms_run(t : T?; src : BlockSource; ctx : int64; forms : array<ModelForm>;
                            body : block<(var m : Model; form : ModelForm) : void>) {
    let lacks = metal_build_why()
    for (form, i in forms, count()) {
        let todo = metal_pass(form, lacks)
        continue if (todo == MetalPass.other)
        g_account.marks[i] = FormMark.skipped
        if (todo == MetalPass.skip) {
            skip_cell(t, "{src.name}, {form_name(form)}: {lacks}")
        } elif (metal_form_run(t, src, form.ctx > 0l ? form.ctx : ctx, form, body)) {
            g_account.marks[i] = FormMark.reached
        }
    }
}

//! one Metal form: the home's seat read, the form's mode set, the
//! load - the blob twin, or the served image, a skipped cell where
//! that image is no metal-blob on this box - its context cut to
//! `ctx`, `body` once on it, the census written, the seat put back,
//! the model closed with the drivers down and no Metal object left
//! live (`metal_form_close`); false where the load gave `body` no
//! model
def private metal_form_run(t : T?; src : BlockSource; ctx : int64; form : ModelForm;
                           body : block<(var m : Model; form : ModelForm) : void>) : bool {
    let path = src.path
    let file = src.name
    let seat = metal_home_seat()
    set_metal_mode(metal_form_mode(form))
    t |> equal(get_metal_mode(), metal_form_mode(form),
        "{file}, {form_name(form)}: the Metal mode took for the load")
    let t0 = ref_time_ticks()
    var m <- load_pinned(form) {
        return <- form.metal_image ? load_model_cached(path, QuantMode.q8) : blob_twin(t, path, ctx)
    }
    let served = m.metal_blob
    if (served) {
        m.config.seq_len = min(m.config.seq_len, ctx)
        let t1 = ref_time_ticks()
        invoke(body, m, form)
        log_form_seconds(form_name(form), get_time_usec(t0) - get_time_usec(t1), get_time_usec(t1))
        kernel_census_write()
    } elif (form.metal_image) {
        skip_cell(t,
            "{file}, {form_name(form)}: the served image is not the metal-blob flavor on this box")
    }
    restore_metal_home(seat)
    metal_form_close(t, m, "{file}, {form_name(form)}")
    return served
}

//! the CPU form `form` of `path` (its load mode and loader), loaded once under its backend pin (`backend_pinned`): `body` on it
//! once a codec of `kvs`; every CPU form of the block's that is this load marked as it went (`mark_load`)
def private cpu_form_run(t : T?; src : BlockSource; ctx : int64; form : ModelForm;
                         kvs : array<KVDtype>;
                         body : block<(var m : Model; form : ModelForm) : void>) {
    var ran = false
    backend_pinned(t, src.name, form) {
        cpu_form_load(t, src.path, ctx, form, kvs, body)
        ran = true
    }
    mark_load(form, ran ? FormMark.reached : FormMark.skipped)
}

//! what the running block was asked - its forms - and where each stands (`FormMark`): opened at each call
struct private BlockAccount {
    forms : array<ModelForm>
    marks : array<FormMark>
}

var private g_account : BlockAccount

//! the account of a block asked `forms`, each pending
def private account_open(forms : array<ModelForm>) {
    delete g_account
    g_account.forms := forms
    g_account.marks |> resize(length(forms))
}

//! every CPU form of the account that is the load `load` marked `mark`
def private mark_load(load : ModelForm; mark : FormMark) {
    for (f, i in g_account.forms, count()) {
        if (!f.on_gpu && same_cpu_load(f, load)) {
            g_account.marks[i] = mark
        }
    }
}

//! what the block owes each form it was asked (`form_owed`), `device_ran` whether a device form reached `body`: a loud skip
//! naming the reason, or a failure naming the form
def private forms_accounted(t : T?; file : string; device_ran : bool) {
    for (f, i in g_account.forms, count()) {
        let owed = form_owed(f, g_account.marks[i], device_ran)
        if (owed == FormOwed.skip) {
            skip_cell(t, "{file}, {form_name(f)}: no device form ran, and the form loads to answer them")
            g_account.marks[i] = FormMark.skipped
        } elif (owed == FormOwed.failure) {
            t |> failure("{file}, {form_name(f)}: the block was asked this form and neither ran it nor skipped it loudly")
        }
    }
}

//! the parity row's form `pf` of `file` among the block's forms and, where it did not skip, its pass reaching the row's cell
//! (`passes` the passes that did); else a failure naming it
def private parity_accounted(t : T?; file : string; pf : ModelForm; passes : int) {
    for (f, i in g_account.forms, count()) {
        continue if (!alone_pass_of(f, pf))
        return if (passes > 0 || g_account.marks[i] == FormMark.skipped)
        t |> failure("{file}, {form_name(pf)}: the parity row's pass never reached its cell")
        return
    }
    t |> failure("{file}, {form_name(pf)}: the parity row's form is not among the block's forms")
}

//! `run` with the form's kernel backend pinned
//! (`with_kernel_backend`), the backend and its pin put back after
//! it; `run` alone where the form names none. A pin the box refuses
//! skips the form loudly where DASLLAMA_PIN_BACKEND names that
//! backend - the run asks what the box cannot do - and fails it
//! otherwise, as a pin that does not take does; `name` the block's
//! model
def private backend_pinned(t : T?; name : string; f : ModelForm; run : block<void>) {
    if (empty(f.backend)) {
        invoke(run)
        return
    }
    let what = "{base_name(name)}, {form_name(f)}"
    let why = kernel_backend_pin_refusal(f.backend)
    if (!empty(why)) {
        if (f.backend == g_env_engine.pin_backend) {
            skip_cell(t, "{what}: DASLLAMA_PIN_BACKEND='{f.backend}' cannot be pinned on this box: {why}")
        } else {
            t |> failure("{what}: the pin cannot take: {why}")
        }
        return
    }
    let pinned = with_kernel_backend(f.backend, run)
    t |> success(pinned, "{what}: the pin took for the form's load and cells")
}

//! the form's load knobs held on its load `m`: the load ran on the pinned backend, and the K-quant knob - the Q4_0 one too where
//! the form asks it - held through the load as the form set it (`kq_load`, the engine's settings the load left - a box
//! profile's runtime section sets them at every load), no native K-quant plane loaded where it is off
def private load_knobs_held(t : T?; f : ModelForm; m : Model; kq_load : tuple<kq : bool; q40 : bool>) {
    if (!empty(f.backend)) {
        t |> equal(active_kernel_backend(), f.backend, "{form_name(f)} loaded on the pinned backend")
    }
    if (f.kq_native != KqNative.kept) {
        t |> equal(kq_load.kq, f.kq_native != KqNative.off, "{form_name(f)}: the K-quant native knob held through the load")
        t |> success(f.kq_native != KqNative.off || !m.kquant_native, "{form_name(f)}: no native K-quant plane loaded")
    }
    if (f.kq_native == KqNative.on_q40) {
        t |> success(kq_load.q40, "{form_name(f)}: the Q4_0 native knob held through the load")
    }
}

//! `cpu_form_run`'s load and cells: the K-quant knobs set for the load alone
def private cpu_form_load(t : T?; path : string; ctx : int64; form : ModelForm; kvs : array<KVDtype>;
                          body : block<(var m : Model; form : ModelForm) : void>) {
    let levers = resident_levers()
    if (pf_window_rows_pin() <= 0l) {
        set_pf_window_rows_pin(PF_WINDOW_ROWS)   //! the prompt window a load latches: the cells' own, as `load_resident` pins it
    }
    let kq_was = (kq = get_kquant_native(), q40 = get_kq_q40_native())
    if (form.kq_native != KqNative.kept) {
        set_kquant_native(form.kq_native != KqNative.off)
    }
    if (form.kq_native == KqNative.on_q40) {
        set_kq_q40_native(true)
    }
    let planes = host_planes_pin()
    set_host_planes(true)   //! an uncached load onto a discrete home keeps no plane on the host: the CPU chain needs them
    let t0 = ref_time_ticks()
    var cpu <- load_pinned(form) $() : Model => <- form.uncached ? load_model_uncached(path, form.quant) : load_model_(path, form.quant)
    restore_host_planes(planes)
    let kq_load = (kq = get_kquant_native(), q40 = get_kq_q40_native())
    if (form.kq_native != KqNative.kept) {
        set_kquant_native(kq_was.kq)
    }
    if (form.kq_native == KqNative.on_q40) {
        set_kq_q40_native(kq_was.q40)
    }
    restore_resident_levers(levers)
    moe_gpu_drop_model()   //! whatever device state the load installed goes: the form is the CPU chain's
    cpu.config.seq_len = min(cpu.config.seq_len, ctx)
    var f = form
    t |> success(!moe_gpu_resident_active(), "no driver is armed through {form_name(f)}")
    t |> success(!cpu.planes_trimmed, "{form_name(f)} holds its weight planes on the host")
    load_knobs_held(t, f, cpu, kq_load)
    let t1 = ref_time_ticks()
    for (kv, i in kvs, count()) {
        f = codec_pass(form, kv, i == 0)
        invoke(body, cpu, f)
    }
    log_form_seconds(form_name(f), get_time_usec(t0) - get_time_usec(t1), get_time_usec(t1))
    delete cpu
}

//! The model file `file` (its name under `models_dir()`) for the cells of the run area `area` (llm, vision, audio) - both
//! literals: `REVIEW.das` reads them to hold one load site an area and file across the folder - in each form of `forms`,
//! strictly one loaded model at a time. Each device form loads once with its knobs applied (its mirror cut to `ctx`, or to the
//! form's own context), `body` runs on it once, and the driver is held to the device: every call served, still armed at its
//! end; a form the environment cannot arm - every device form where the GPU tier is not armed - skips loudly as a cell of its
//! own and `body` never sees it. The first device form streams the file as a load with no image does; where the list holds
//! `LANE_FORMS` such forms or more and the image rail is on, each later one maps the file's device lane, minted once beside the
//! file and removed after the last form unless it stood there before (`../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`).
//! Then the CPU loads follow (`cpu_forms_run`): the CPU form answering the device forms where one ran, and on every box each
//! CPU form whose cells stand alone (`cpu_cells_form`); a load `body` sees once a codec marks `form.alone` on its first codec's
//! pass alone, so a cell that stands alone runs once a block where it reads `form.alone`. `body` collects what a compare
//! between forms reads - rows, sums, counts - into the caller's state, and marks there which forms it saw; those compares
//! follow the call, and a cell of one form alone may assert inside `body`. Every form of `forms` reaches `body` or skips
//! loudly - a CPU form not asked alone, where no device form ran, as a skip of its own - else the block fails naming it
//! (`forms_accounted`). False where no device form ran.
let private LANE_FORMS = 3   //! a mint costs a streamed load and a write, so it pays from the third form

def with_cpu_and_gpu_model(t : T?; area, file : string; ctx : int64; forms : array<ModelForm>;
                           body : block<(var m : Model; form : ModelForm) : void>) : bool =>
    with_cpu_and_gpu_model(t, area, file, ctx, forms, true, body)

//! the same where `cpu` says whether the CPU form answering the device forms loads: false for a block whose every compare reads
//! the device alone - a carrier the CPU form would load whole for no compare
def with_cpu_and_gpu_model(t : T?; area, file : string; ctx : int64; forms : array<ModelForm>; cpu : bool;
                           body : block<(var m : Model; form : ModelForm) : void>) : bool =>
    cpu_and_gpu_block(t, area, BlockSource(path = path_join(models_dir(), file), name = file),
        ctx, forms, cpu, body)

//! `with_cpu_and_gpu_model` over the Hugging Face snapshot directory
//! of `repo` (`hf_snapshot_dir`), a string literal `REVIEW.das` keys
//! the block by
def with_cpu_and_gpu_snapshot(t : T?; area, repo : string; ctx : int64; forms : array<ModelForm>;
                              cpu : bool;
                              body : block<(var m : Model; form : ModelForm) : void>) : bool {
    let src = BlockSource(path = hf_snapshot_dir(repo), name = repo, snapshot = true)
    return cpu_and_gpu_block(t, area, src, ctx, forms, cpu, body)
}

//! the snapshot block with the CPU form answering the device forms
def with_cpu_and_gpu_snapshot(t : T?; area, repo : string; ctx : int64; forms : array<ModelForm>;
                              body : block<(var m : Model; form : ModelForm) : void>) : bool =>
    with_cpu_and_gpu_snapshot(t, area, repo, ctx, forms, true, body)

//! where a block's model comes from: the path it loads, the name its
//! messages and gates read - a file under `models_dir()`, or a
//! snapshot's repo
struct private BlockSource {
    path : string
    name : string
    snapshot : bool
}

//! whether the block's model is here under the JIT: a file by
//! `model_available`, a snapshot by its repo's category and family
//! and by `snapshot_available`; else a loud skip
def private source_available(t : T?; src : BlockSource) : bool {
    return jit_model_available(t, src.path) if (!src.snapshot)
    return false if (!jit_only(t))
    let gate = category_gate(src.name, g_env_test.test_categories)
    let why = empty(gate.why) ? family_skip_reason(src.name, g_env_test.test_family) : gate.why
    let absent = "{src.name}: the snapshot is not in the Hugging Face cache"
    let lacks = empty(why) && empty(src.path) ? absent : why
    if (!empty(lacks)) {
        t |> skip(lacks)
        return false
    }
    return snapshot_available(t, src.path)
}

//! the block over `src` (`with_cpu_and_gpu_model`)
[arch(at = "../ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once")]
def private cpu_and_gpu_block(t : T?; area : string; src : BlockSource; ctx : int64;
                              forms : array<ModelForm>; cpu : bool;
                              body : block<(var m : Model; form : ModelForm) : void>) : bool {
    let path = src.path
    let file = src.name
    account_open(forms)
    if (!source_available(t, src)) {
        for (k in g_account.marks) {
            k = FormMark.skipped
        }
        return false
    }
    to_log(LOG_INFO, "{area}: {file} in {length(forms)} forms and the CPU's, one at a time\n")
    var inscope kvs : array<KVDtype>
    var inscope before <- lanes_beside(path)
    with_dasllama_jobque_() {
        delete kvs
        kvs <- device_forms_run(t, path, file, ctx, forms, body)
        if (remove_minted_lanes(path, before)) {
            var inscope after <- lanes_beside(path)
            t |> equal(length(after), length(before), "the block left the lanes beside {file} as it found them: {after}")
        }
        metal_forms_run(t, src, ctx, forms, body)
        cpu_forms_run(t, src, ctx, forms, cpu, kvs, body)
    }
    forms_accounted(t, file, !empty(kvs))
    return !empty(kvs)
}

//! the block's device forms of `forms`, one load each (`device_form_run`), each marked as it went: the codecs the forms that
//! ran armed, in order
def private device_forms_run(t : T?; path, file : string; ctx : int64; forms : array<ModelForm>;
                             body : block<(var m : Model; form : ModelForm) : void>) : array<KVDtype> {
    let tier_why = gpu_tier_why()
    let lane_forms = length([for (f in forms); f; where is_lane_form(f)])
    let rail = empty(tier_why) && lane_forms >= LANE_FORMS && g_env_engine.image && image_save_enabled()
    var kvs : array<KVDtype>
    var lane_forms_before = 0
    for (form, i in forms, count()) {
        continue if (!on_device_home(form))
        g_account.marks[i] = FormMark.skipped
        if (!empty(tier_why)) {
            skip_cell(t, "{file}, {form_name(form)}: {tier_why}")
            continue
        }
        let lane = rail && is_lane_form(form) && lane_forms_before > 0
        if (is_lane_form(form)) {
            lane_forms_before++
        }
        let armed_out = device_form_run(t, path, ctx, form, lane, body)
        if (armed_out.ran) {
            g_account.marks[i] = FormMark.reached
            if (!has_value(kvs, armed_out.kv)) {
                kvs |> push(armed_out.kv)
            }
        }
    }
    return <- kvs
}

//! the same with the model_specs parity row of `file` where `parity` (a bool): its CPU form (`parity_spec_form`) beside `forms`,
//! the row's cell (`parity_spec_cell`) on it, and `body` on that form only where another form of `forms` is the same load
//! (`parity_spec_spares`). A generic: the parity calls resolve where the caller instantiates it (`_::`), the caller requiring
//! `_parity_cells`, which requires this module
def with_cpu_and_gpu_model(t : T?; area, file : string; ctx : int64; forms : array<ModelForm>; cpu : bool = true; parity : auto(Flag);
                           body : block<(var m : Model; form : ModelForm) : void>) : bool {
    static_assert(typeinfo stripped_typename(parity) == "bool", "parity: whether the block runs the file's parity row")
    var all := forms
    let pf = parity ? _::parity_spec_form(file) : ModelForm()
    if (parity) {
        all |> push(pf)
    }
    var row_passes = 0
    let ran = with_cpu_and_gpu_model(t, area, file, ctx, all, cpu) $(var m, form) {
        if (parity) {
            row_passes += alone_pass_of(form, pf) ? 1 : 0
            _::parity_spec_cell(t, m, form, file)
            return if (!_::parity_spec_spares(form, all, file))
        }
        invoke(body, m, form)
    }
    if (parity) {
        parity_accounted(t, file, pf, row_passes)
    }
    delete all
    return ran
}

//! whether a CPU form of `forms` that is the load `f` asks its cells to stand alone (`alone`)
def private alone_asked(forms : array<ModelForm>; f : ModelForm) : bool {
    for (g in forms) {
        return true if (!g.on_gpu && g.alone && same_cpu_load(g, f))
    }
    return false
}

//! whether a CPU form before `forms[i]` is its load: one load a mode and loader
def private earlier_cpu_load(forms : array<ModelForm>; i : int) : bool {
    for (j in range(i)) {
        return true if (!forms[j].on_gpu && same_cpu_load(forms[j], forms[i]))
    }
    return false
}

//! the block's CPU loads, one a load mode and loader. The q8 load (`load_model_`) first: on every codec `kvs` the device forms
//! armed where they ran and `cpu`; else, where a form asks it alone, once on f16 sessions (on the first armed codec where a
//! device form ran). Then each other CPU load of `forms`, where a device form ran and `cpu` or one of its forms asks it alone,
//! once on the first armed codec (f16 where none ran)
def private cpu_forms_run(t : T?; src : BlockSource; ctx : int64; forms : array<ModelForm>;
                          cpu : bool; kvs : array<KVDtype>;
                          body : block<(var m : Model; form : ModelForm) : void>) {
    let ran = !empty(kvs) && cpu
    let first = empty(kvs) ? KVDtype.f16 : kvs[0]
    var main = cpu_quant_form(QuantMode.q8)
    main.alone = alone_asked(forms, main)
    if (ran) {
        cpu_form_run(t, src, ctx, main, kvs, body)
    } elif (main.alone) {
        cpu_form_run(t, src, ctx, main, [first], body)
    }
    for (f, i in forms, count()) {
        continue if (f.on_gpu || same_cpu_load(f, main) || earlier_cpu_load(forms, i))
        var load = f
        load.alone = alone_asked(forms, f)
        if (ran || load.alone) {
            cpu_form_run(t, src, ctx, load, [first], body)
        }
    }
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

//! a file's forced feeds on its default form: the prompts, the default form's rows - on the CUDA home under the driver's
//! counters (`cuda_feed_rows`), else the resident feed - the CPU form's on the default form's codec, the first under the
//! counters' control on the CUDA home, the f32 CPU form's where the block loads one, the MMQ-off form's control feed where
//! the block loads one, and what the device held (`carrier`); `window` the prompt window the feeds are sized against, `steps`
//! the resident feeds' fed steps
struct DenseFeeds {
    prompts : array<int64>
    cuda : bool
    window : int64
    steps : int64
    main : MainPair
    carrier : Held
    gpu : array<FeedRows>
    cpu : array<FeedRows>
    f32 : array<FeedRows>
    mmq : MmqOffFeed
}

//! the MMQ-off form's control feed (`mmq_off`): a 40-token prompt over one window on the CUDA home
struct MmqOffFeed {
    rows : FeedRows
    ran : bool
}

//! the MMQ-off form `form`'s control feed into `o`, the driver's counters held into `h` (`cuda_feed_rows`)
def mmq_off_rows(var o : MmqOffFeed; var h : Held; m : Model; form : ModelForm) {
    o.rows <- cuda_feed_rows(h, m, form, 40l, 1l)
    o.ran = true
}

//! where the MMQ-off control ran, its feed against the CPU form's first feed within `bar_rel` (`feed_rows_within`, `what` its
//! name), a missing twin a failure (`twin_collected`, `twin` its name)
def mmq_off_within(t : T?; o : MmqOffFeed; cpu : array<FeedRows>; twin, what : string; bar_rel : float) {
    if (o.ran && twin_collected(t, 0, length(cpu), twin)) {
        feed_rows_within(t, what, o.rows, cpu[0], bar_rel)
    }
}

//! the feeds `prompts` on the armed home over PF_WINDOW_ROWS-row windows and DENSE_FED_STEPS fed steps, a prompt past one
//! window held as a skip where the prompt window is not PF_WINDOW_ROWS rows
def dense_feeds(var c : DenseFeeds; prompts : array<int64>) {
    c.cuda = gpu_want_cuda()
    c.window = PF_WINDOW_ROWS
    c.steps = DENSE_FED_STEPS
    for (n in prompts) {
        if (n > PF_WINDOW_ROWS && !held_pf_window(c.carrier)) {
            continue
        }
        c.prompts |> push(n)
    }
}

//! one form's share of the feeds: the default form's rows - the file's family `arch` held off it where named - the CPU form's
//! on its codec, the f32 form's, the MMQ-off form's control feed; the side the form is
def dense_feeds_rows(var c : DenseFeeds; m : Model; form : ModelForm; arch : string = "") : MainSide =>
    dense_feeds_rows(c, m, form, arch) $(_i : int; _after : bool) {}

//! the same with `prefill_census` around each resident feed off the CUDA home, feed `i`'s: before its prefill (`after` false),
//! then between its prefill and its first fed step
def dense_feeds_rows(var c : DenseFeeds; m : Model; form : ModelForm; arch : string;
                     prefill_census : block<(i : int; after : bool) : void>) : MainSide {
    let side = main_side(c.main, form)
    if (form.on_gpu && form.mmq_off) {
        mmq_off_rows(c.mmq, c.carrier, m, form)
    } elif (side == MainSide.device) {
        if (!empty(arch)) {
            c.carrier |> held_equal(m.arch, arch, "the file is a {arch}-family model")
        }
        c.gpu |> reserve(length(c.prompts))
        for (n, i in c.prompts, count()) {
            if (c.cuda) {
                c.gpu |> emplace <| cuda_feed_rows(c.carrier, m, form, n, prompt_windows(n, c.window))
                continue
            }
            var inscope prompt <- resident_prompt(n)
            var inscope toks <- resident_feed(c.steps)
            invoke(prefill_census, i, false)
            c.gpu |> emplace <| feed_rows(m, form, prompt, toks) $ {
                invoke(prefill_census, i, true)
            }
        }
    } elif (side == MainSide.cpu) {
        c.cpu |> reserve(length(c.prompts))
        for (n, i in c.prompts, count()) {
            with_cpu_counters_still(c.carrier, c.cuda && i == 0) {
                c.cpu |> emplace <| resident_feed_rows(m, form, n, c.steps)
            }
        }
    } elif (!form.on_gpu && form.quant == QuantMode.fp32) {
        c.f32 |> reserve(length(c.prompts))
        for (n in c.prompts) {
            c.f32 |> emplace <| resident_feed_rows(m, form, n, c.steps)
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
        feeds_within(t, "{tag}{c.cuda ? " cuda" : ""}, ", c.gpu, c.cpu, bar_rel)
    }
}

//! the block of `file` on the driver's default form alone and the CPU form answering it: `rows` collects each form's share of
//! the feeds `c` the caller sized (`dense_feeds`), held after the block within `bar_rel` (`dense_feeds_within`, `file` its tag).
//! A loader `REVIEW.das` lists: it reads `file` at the call
def dense_feeds_block(t : T?; file : string; ctx : int64; var c : DenseFeeds; bar_rel : float;
                      rows : block<(m : Model; form : ModelForm) : void>) {
    let ran = with_cpu_and_gpu_model(t, "llm", file, ctx, [device_form()]) $(m, form) {
        invoke(rows, m, form)
    }
    if (ran) {
        dense_feeds_within(t, file, c, bar_rel)
    }
}

//! each device feed of `gpu` against its CPU twin of `cpu` within `bar_rel` (`feed_rows_within`), named
//! "{lead}{n} tokens{tail}: "; where `cpu_steps` is past 0 the twin's prompt row and first `cpu_steps` steps
//! (`feed_prefix`); a feed with no twin a failure (`twin_collected`)
def feeds_within(t : T?; lead : string; gpu, cpu : array<FeedRows>; bar_rel : float; tail : string = ""; cpu_steps : int64 = 0l) {
    for (f, i in gpu, count()) {
        let what = "{lead}{f.nprompt} tokens{tail}: "
        continue if (!twin_collected(t, i, length(cpu), what))
        if (cpu_steps > 0l) {
            var inscope twin <- feed_prefix(cpu[i], cpu_steps)
            feed_rows_within(t, what, f, twin, bar_rel)
        } else {
            feed_rows_within(t, what, f, cpu[i], bar_rel)
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
