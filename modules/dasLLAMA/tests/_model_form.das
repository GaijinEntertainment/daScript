options gen2
options _dasllama_internal = true

module _model_form shared public

require dastest/testing_boost   // T: a block's gate skips on it
require dasllama/dasllama_common   // QuantMode, Model
require dasllama/dasllama_kv_dtype   // KVDtype
require dasllama/dasllama_kqformat   // KqFmt, kq_dense_b32: the plane census
require daslib/fio   // base_name
require math   // min

//! a CPU form's K-quant native knob for its load (`set_kquant_native`, and `set_kq_q40_native` for `on_q40`), put back after
//! the load
enum KqNative {
    kept   //! the engine's setting as the load finds it
    on     //! the native K-quant planes
    off    //! the q8 requant of every K-quant tensor
    on_q40   //! the native K-quant planes and the native Q4_0 ones (`set_kq_q40_native`)
}

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
    //! the load asks the driver for a block drafter's round
    //! (`set_gpu_resident_block_rounds_`)
    block_rounds : bool
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
    //! the mirror context `ctx` asked whole (`set_gpu_ctx_strict`): the
    //! load declines where the card cannot hold it, else it shrinks
    strict_ctx : bool
    //! the CUDA home's window-fault seat pinned for the load
    //! (`set_cuda_window_fault_`): the driver arms without the window
    //! chain's planes; no knob off the CUDA home
    cuda_window_fault : bool
    //! the CUDA home's head-warm fault seat pinned through the form
    //! (`set_cuda_head_warm_fault_`): the NextN head's prompt warm, whose
    //! planes are taken at its first window, declines; no knob off the
    //! CUDA home
    cuda_head_warm_fault : bool
    //! the CUDA home's FP4-chain fault seat pinned through the form
    //! (`set_cuda_mmq_fault_`): the FP4 expert chain's planes, taken
    //! where the routed block's stacks are sized, read as having found
    //! no room; no knob off the CUDA home
    cuda_mmq_fault : bool
    //! a CPU form whose cells assert on its load alone: it loads on
    //! every box, where no device form runs too; on a load the body
    //! sees once a codec, true on the first codec's pass alone
    alone : bool
    //! a CPU form loaded through `load_model_uncached` (the product's
    //! load with no image) in place of `load_model_`
    uncached : bool
    //! the NextN head file (its name under the models folder) pinned for the
    //! load (`set_mtp_head_path`), the pin cleared after it: "" none
    mtp_head : string
    //! the load takes the f32 weight-scale plane (`set_wscale_f16(false)`),
    //! put back after it: else the engine's default scale
    wscale_f32 : bool
    //! DASLLAMA_IQ3S_SERVE pinned for the load (`set_iq3s_serve_pin`), put
    //! back after it: "" the environment's
    iq3s_serve : string
    //! a CPU form's kernel backend pinned for its load and its cells
    //! (`with_kernel_backend`), put back after them: "" none
    backend : string
    //! a CPU form's K-quant native knob for its load
    kq_native : KqNative
    //! a device form whose last cell drops the device model
    //! (`device_drop_cell`): the driver held armed up to that cell
    drops_device : bool
    //! a form on the Metal home (`metal_form`), on a das_metal build: no
    //! CPU load (`on_gpu` holds) and no whole-model driver's
    on_metal : bool
    //! a Metal form loads the file's served image (`load_model_cached`),
    //! else its blob twin (`blob_twin`)
    metal_image : bool
    //! a Metal form loads and runs under `MetalMode.required`, else
    //! `when_available`; a served image always does
    metal_required : bool
    //! the dense-FFN want pinned for the load (`set_gpu_want_dense_ffn`,
    //! read by a per-op load's walk): -1 the environment's, else 0 or 1
    dense_ffn : int = -1
    //! the resident routed layers the tier's want asks for the load, the
    //! prompt streaming every layer below them and the tier re-armed
    //! around it: 0 the want as found
    moe_layers : int64
    //! the Vulkan cooperative-matrix mode pinned for the load and its
    //! cells (`g_gpu.coopmat_mode`, a `COOPMAT_*` value): -1 the box's;
    //! no knob off Vulkan
    coopmat : int = -1
}

//! a device form on the environment's mirror codec
def device_form(ring : bool = true; gather : bool = true; regions : int64 = 0l; host : bool = false; split_forms : bool = false;
                ctx : int64 = 0l; host_layers : int64 = 0l; pf_stream : bool = true;
                pf_window_rows : int64 = 0l; mtp_depth : int64 = 0l; head_off : bool = false;
                mmq_off : bool = false; host_ctx : int64 = 0l; fuse_bisect : int64 = -1l;
                perop : bool = false; pool_slots : int64 = 0l; strict_ctx : bool = false;
                cuda_window_fault : bool = false; mtp_head : string = "";
                drops_device : bool = false; cuda_head_warm_fault : bool = false;
                block_rounds : bool = false; cuda_mmq_fault : bool = false; dense_ffn : int = -1;
                moe_layers : int64 = 0l; coopmat : int = -1) : ModelForm =>
    ModelForm(on_gpu = true, kv = KVDtype.f16, kv_asked = false, ring = ring, gather = gather,
        quant = QuantMode.q8, regions = regions, host = host, split_forms = split_forms, ctx = ctx,
        host_layers = host_layers, pf_stream = pf_stream, pf_window_rows = pf_window_rows,
        mtp_depth = mtp_depth, head_off = head_off, mmq_off = mmq_off, host_ctx = host_ctx,
        fuse_bisect = fuse_bisect, perop = perop, pool_slots = pool_slots, strict_ctx = strict_ctx,
        cuda_window_fault = cuda_window_fault, mtp_head = mtp_head, drops_device = drops_device,
        cuda_head_warm_fault = cuda_head_warm_fault, block_rounds = block_rounds,
        cuda_mmq_fault = cuda_mmq_fault, dense_ffn = dense_ffn, moe_layers = moe_layers,
        coopmat = coopmat)

//! a form on the Metal home: the file's blob twin (`blob_twin`: the q8 load turned into the blob the Metal drivers serve)
//! under `MetalMode.when_available` - `required` where asked - or, `image`, its served image (`load_model_cached` under
//! `MetalMode.required`, the metal-blob `.dlim` minted beside the file where none stands); the context cut to `ctx` (0 the
//! call's); the NextN head `mtp_head`, the f32 weight scale `wscale_f32` and the IQ3_S serving mode `iq3s_serve` pinned for
//! the load where asked. The mode holds through the form's cells
def metal_form(image : bool = false; required : bool = false; ctx : int64 = 0l;
               mtp_head : string = ""; wscale_f32 : bool = false;
               iq3s_serve : string = "") : ModelForm =>
    ModelForm(on_gpu = true, on_metal = true, metal_image = image, metal_required = image || required,
        kv = KVDtype.f16, ring = true, gather = true, quant = QuantMode.q8, ctx = ctx,
        pf_stream = true, mtp_head = mtp_head, wscale_f32 = wscale_f32, iq3s_serve = iq3s_serve)

//! the Metal mode a Metal form loads and runs under
def metal_form_mode(f : ModelForm) : MetalMode =>
    f.metal_required ? MetalMode.required : MetalMode.when_available

//! the Metal form a cell family runs on
enum MetalWant {
    blob_twin   //! the blob twin (`metal_form()`)
    any         //! either Metal form
    served_image   //! the served image (`metal_form(image = true)`)
}

//! whether `form` is the Metal form `want` names, else a failure of
//! `t` naming `file` and the cell family `cells`
def metal_form_held(t : T?; form : ModelForm; want : MetalWant; file, cells : string) : bool {
    if (want == MetalWant.blob_twin) {
        return true if (form.on_metal && !form.metal_image)
        t |> failure("{file}: the {cells} cells run on the blob twin (`metal_form()`)")
    } elif (want == MetalWant.any) {
        return true if (form.on_metal)
        t |> failure("{file}: the {cells} cells run on a Metal form (`metal_form`)")
    } else {
        return true if (form.metal_image)
        t |> failure(("{file}: the {cells} cells run on the served image form " +
            "(`metal_form(image = true)`)"))
    }
    return false
}

//! `m`'s weight planes whose tag `pred` takes, one per tag array
//! element: the dense q/k/v/o and FFN arrays under `dense`, the
//! routed stacks under `routed`; an empty array counts none
def sites_count(m : Model; dense, routed : bool; pred : block<(f : KqFmt) : bool>) : int64 {
    var n = 0l
    if (dense) {
        n += (tags_taken(m.wq_fmt, pred) + tags_taken(m.wk_fmt, pred) + tags_taken(m.wv_fmt, pred)
            + tags_taken(m.wo_fmt, pred) + tags_taken(m.w1_fmt, pred)
            + tags_taken(m.w2_fmt, pred) + tags_taken(m.w3_fmt, pred))
    }
    if (routed) {
        n += (tags_taken(m.we1_fmt, pred) + tags_taken(m.we2_fmt, pred)
            + tags_taken(m.we3_fmt, pred))
    }
    return n
}

def private tags_taken(a : array<KqFmt>; pred : block<(f : KqFmt) : bool>) : int64 {
    var n = 0l
    for (f in a) {
        n += invoke(pred, f) ? 1l : 0l
    }
    return n
}

//! `m`'s per-32 planes (`kq_dense_b32`), the dense ones and, under
//! `routed`, the routed stacks'
def per32_plane_count(m : Model; routed : bool) : int64 =>
    sites_count(m, true, routed, $(f : KqFmt) : bool => kq_dense_b32(f))

//! what a block's Metal pass does with an asked form
enum MetalPass {
    other   //! a form of another home: passed by
    skip    //! a Metal form on a build that lacks the home: a skipped cell of its own
    load    //! a Metal form the build serves: its load and its cells
}

//! what a block's Metal pass does with the asked form `f`, `lacks` what the build lacks for the Metal home (`metal_build_why`,
//! "" where it serves)
def metal_pass(f : ModelForm; lacks : string) : MetalPass {
    return MetalPass.other if (!f.on_metal)
    return empty(lacks) ? MetalPass.load : MetalPass.skip
}

//! the form `f` with its mirror asking the codec `kv` (`with_gpu_kv_dtype`): `device_form(...) |> asking(kv)`
def asking(f : ModelForm; kv : KVDtype) : ModelForm {
    var g = f
    g.kv = kv
    g.kv_asked = true
    return g
}

//! `m`'s sessions' context cut to at most `cap` for `body`, put back after it: a CPU form's cell that runs on a shorter context
//! than the block's
def with_ctx_cut(var m : Model; cap : int64; body : block<void>) {
    let seq = m.config.seq_len
    m.config.seq_len = min(seq, cap)
    invoke(body)
    m.config.seq_len = seq
}

//! a CPU form loaded in the mode `quant` past the CPU chain's q8: `QuantMode.fp32` the file's f32 chain, `QuantMode.q4_0` the q4
//! serving mode
def cpu_quant_form(quant : QuantMode) : ModelForm =>
    ModelForm(on_gpu = false, kv = KVDtype.f16, kv_asked = false, ring = true, gather = true,
        quant = quant, pf_stream = true)

//! the CPU form on the file's f32 chain
def f32_form() : ModelForm => cpu_quant_form(QuantMode.fp32)

//! a CPU form whose cells assert on its load alone (`alone`): it loads in the mode `quant`, through `load_model_uncached`
//! where `uncached`, with the NextN head `mtp_head` pinned where named, under the kernel backend `backend` pinned where named
//! (`g_env_engine.pin_backend` the environment's pin), the K-quant knob `kq_native` set, the f32 weight scale `wscale_f32` and
//! the IQ3_S serving mode `iq3s_serve` pinned where asked, on every box - on the q8 cached load
//! the CPU form answering the device forms where one ran
def cpu_cells_form(quant : QuantMode = QuantMode.q8; uncached : bool = false; mtp_head : string = "";
                   backend : string = ""; kq_native : KqNative = KqNative.kept;
                   wscale_f32 : bool = false; iq3s_serve : string = "") : ModelForm {
    var f = cpu_quant_form(quant)
    f.alone = true
    f.uncached = uncached
    f.mtp_head = mtp_head
    f.backend = backend
    f.kq_native = kq_native
    f.wscale_f32 = wscale_f32
    f.iq3s_serve = iq3s_serve
    return f
}

//! whether two CPU forms are one load: the same load mode, loader, head pin, backend pin, K-quant knob, weight scale and
//! IQ3_S serving mode
def same_cpu_load(a, b : ModelForm) : bool => (a.quant == b.quant && a.uncached == b.uncached && a.mtp_head == b.mtp_head
    && a.backend == b.backend && a.kq_native == b.kq_native && a.wscale_f32 == b.wscale_f32
    && a.iq3s_serve == b.iq3s_serve)

//! the pass of the CPU load `load` the body sees on the codec `kv`: the codec set, `alone` kept on the `first` codec's pass
//! alone - a cell that stands alone runs on one pass a block, however many codecs the device forms armed
def codec_pass(load : ModelForm; kv : KVDtype; first : bool) : ModelForm {
    var f = load
    f.kv = kv
    f.alone = load.alone && first
    return f
}

//! a form of the Vulkan or CUDA device home: on the GPU and not on Metal
def on_device_home(f : ModelForm) : bool => f.on_gpu && !f.on_metal

//! the driver's default form on the device: the environment's codec, the rings, the token gather, the load's own regions,
//! context and planes
def device_default(f : ModelForm) : bool => (on_device_home(f) && !f.kv_asked && f.ring && f.gather && f.regions == 0l && !f.host && !f.split_forms
    && f.ctx == 0l && f.mtp_depth == 0l && !f.head_off && !f.mmq_off && f.host_ctx == 0l && f.fuse_bisect < 0l
    && !f.perop && f.pool_slots == 0l && !f.strict_ctx && !f.cuda_window_fault && !f.cuda_head_warm_fault && empty(f.mtp_head)
    && !f.block_rounds && !f.cuda_mmq_fault && f.dense_ffn < 0 && f.moe_layers == 0l && f.coopmat < 0)

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
    return MainSide.cpu if (!f.on_gpu && same_cpu_load(f, cpu_quant_form(QuantMode.q8)) && p.ran && f.kv == p.kv)
    return MainSide.neither
}

//! whether the pass `form` is the CPU load `load`'s pass that stands alone
def alone_pass_of(form, load : ModelForm) : bool => !form.on_gpu && form.alone && same_cpu_load(form, load)

//! where a form a block was asked stands after the block's passes
enum FormMark {
    pending   //! no pass yet
    reached   //! its load reached `body`
    skipped   //! it skipped, or failed, loudly where it could not load
}

//! what a block owes a form it was asked, its passes done
enum FormOwed {
    nothing   //! the form reached `body` or skipped loudly
    skip      //! a loud skip of its own
    failure   //! a form lost: the block fails naming it
}

//! what a block owes the asked form `f` its passes left at `mark`, `device_ran` whether a device form reached `body`: a CPU
//! form not asked alone loads to answer the device forms, so where none ran it skips loudly; any other pending form - a
//! form asked alone runs on every box - is a failure
def form_owed(f : ModelForm; mark : FormMark; device_ran : bool) : FormOwed {
    return FormOwed.nothing if (mark != FormMark.pending)
    return FormOwed.skip if (!f.on_gpu && !f.alone && !device_ran)
    return FormOwed.failure
}

//! a device form that loads off the file's device lane where the
//! rail is on: every expert on the device (a lane bakes its
//! host-layers split, its pool, its resident routed layers, its dense
//! rail and its coopmat mode), no host planes, and no fault seat (the
//! seat's load streams the file)
def is_lane_form(f : ModelForm) : bool => (on_device_home(f) && !f.host && f.host_layers == 0l && !f.perop && f.pool_slots == 0l
    && !f.cuda_window_fault && empty(f.mtp_head) && f.dense_ffn < 0 && f.moe_layers == 0l && f.coopmat < 0)

//! a CPU form as its messages name it
def private cpu_form_name(f : ModelForm) : string {
    let uncached = f.uncached ? ", the uncached load" : ""
    let head = empty(f.mtp_head) ? uncached : "{uncached}, the NextN head {base_name(f.mtp_head)}"
    let pinned = empty(f.backend) ? head : "{head}, the {f.backend} backend pinned"
    let knob = f.kq_native == KqNative.kept ? pinned : "{pinned}, K-quant native {f.kq_native}"
    let loader = "{knob}{load_pins_note(f)}"
    return "the {f.quant} CPU form{loader}" if (f.quant != QuantMode.q8)
    return "the CPU form ({f.kv} sessions{loader})"
}

//! the load-time pins of `f` beyond its head, as a form's name words them
def private load_pins_note(f : ModelForm) : string {
    let scale = f.wscale_f32 ? ", the f32 weight scale" : ""
    return empty(f.iq3s_serve) ? scale : "{scale}, IQ3_S served as {f.iq3s_serve}"
}

//! a Metal form as its messages name it
def private metal_form_name(f : ModelForm) : string {
    let flavor = f.metal_image ? "served image" : "blob twin"
    let head = empty(f.mtp_head) ? "" : ", the NextN head {base_name(f.mtp_head)}"
    return "the Metal form (the {flavor}, {metal_form_mode(f)}{head}{load_pins_note(f)})"
}

//! what a form's load does about speculation, as `form_name` words it
def private speculation_note(f : ModelForm) : string
    => f.head_off ? ", the NextN head off the device" : (f.block_rounds ? ", a block drafter's round asked" : "")

//! the form as its messages name it
def form_name(f : ModelForm) : string {
    return cpu_form_name(f) if (!f.on_gpu)
    return f.on_metal ? metal_form_name(f) : device_form_name(f)
}

//! the load-time asks of `f`'s tier and drivers - the pool's slots, the resident routed layers, the dense rail, the coopmat
//! mode - as a form's name words them
def private tier_want_note(f : ModelForm) : string {
    let heat = f.pool_slots > 0l ? ", {f.pool_slots} pool slots" : ""
    let routed = f.moe_layers > 0l ? "{heat}, {f.moe_layers} resident routed layers" : heat
    let dense = f.dense_ffn >= 0 ? "{routed}, the dense FFN rail {f.dense_ffn > 0 ? "on" : "off"}" : routed
    return f.coopmat >= 0 ? "{dense}, coopmat mode {f.coopmat}" : dense
}

//! a device form as its messages name it
def private device_form_name(f : ModelForm) : string {
    let regions = f.regions > 0l ? ", {f.regions} regions" : ""
    let planes = f.host ? ", host planes" : ""
    let fuse = f.split_forms ? ", split forms" : ""
    let stream = f.pf_stream ? "" : ", the prompt stream off"
    let experts = f.host_layers > 0l ? ", {f.host_layers} routed layers on the host{stream}" : ""
    let window = f.pf_window_rows > 0l ? ", {f.pf_window_rows}-row prompt windows" : ""
    let depth = f.mtp_depth > 0l ? ", depth {f.mtp_depth}" : ""
    let speculation = speculation_note(f)
    let mmq = f.mmq_off ? ", the MMQ route off" : ""
    let host_ctx = f.host_ctx > 0l ? ", {f.host_ctx}-position host sessions" : ""
    let bisect = f.fuse_bisect >= 0l ? ", fuse bisect {f.fuse_bisect}" : ""
    let heat = tier_want_note(f)
    let whole = f.strict_ctx ? ", the context asked whole" : ""
    let window_fault = f.cuda_window_fault ? ", the window-fault seat" : ""
    let head_fault = f.cuda_head_warm_fault ? "{window_fault}, the head-warm fault seat" : window_fault
    let fault = f.cuda_mmq_fault ? "{head_fault}, the FP4-chain fault seat" : head_fault
    let pinned = empty(f.mtp_head) ? fault : "{fault}, the NextN head {base_name(f.mtp_head)}"
    let shape = "{f.ring ? "rings" : "flat"}, {f.gather ? "device gather" : "host embed"}"
    return "the per-op form ({f.kv} asked, the whole-model route off{heat})" if (f.perop)
    return "the device form ({f.kv} mirror, {shape}{regions}{planes}{fuse}{experts}{window}{depth}{speculation}{mmq}{host_ctx}{bisect}{heat}{whole}{pinned})"
}

//! where a block's seconds went, a line a form: under `DAS_LOG_LEVEL=info`
def log_form_seconds(name : string; load_usec, cells_usec : int) {
    to_log(LOG_INFO, "FORM {name}: load {float(load_usec) * 1e-6f:.3f} s, cells {float(cells_usec) * 1e-6f:.3f} s\n")
}

//! a form's `cells` on its model `m`, after a load that began at `t0`: one `FORM` line of both spans (`log_form_seconds`)
def form_cells_timed(name : string; t0 : int64; var m : auto(MODEL); form : string; cells : block<(var m : MODEL; form : string) : void>) {
    let loaded = get_time_usec(t0)
    let t1 = ref_time_ticks()
    invoke(cells, m, form)
    log_form_seconds(name, loaded, get_time_usec(t1))
}

//! The forms of one model file at `path`, a form at a time, behind the file's gate: where `available` - the gate, which
//! skips loudly on `t` - answers false nothing loads and the call answers false; else `load` builds the model of each
//! form of `forms`, in the list's order, `cells` runs on it, and it drops before the next form loads.
def model_once_at(t : T?; area, file, path : string; forms : array<string>; available : function<(t : T?; path : string) : bool>;
                  load : function<(path : string; form : string) : auto(MODEL)>;
                  cells : block<(var m : MODEL; form : string) : void>) : bool {
    return false if (!invoke(available, t, path))
    for (form in forms) {
        let t0 = ref_time_ticks()
        var inscope m <- invoke(load, path, form)
        form_cells_timed("{area} {file} {form}", t0, m, form, cells)
    }
    return true
}

//! the staging a form mints from (`staged_once_at`): none where the form has no staged read (`has` false), a copy of
//! `kept` where a later form mints from it `again`, else `kept` itself, moved out
def own_staging(var kept : auto(STG); has, again : bool) : STG -const {
    return <- default<STG> if (!has)
    return clone_to_move(kept) if (again)
    return <- kept
}

//! `model_once_at` for a file several of whose forms are minted from one staged read. `key` names the staged read a form
//! is minted from - "" for a form that loads on its own - `stage` reads the file once a key, and `load` builds a form's
//! model from a staging of its own: a copy of the kept read, or the kept read itself on the last form of its key, so the
//! cut or the poison one form writes into its staging reaches no other form's. The kept read drops where a form of
//! another key asks for its own.
def staged_once_at(t : T?; area, file, path : string; forms : array<string>; available : function<(t : T?; path : string) : bool>;
                   key : function<(form : string) : string>;
                   stage : function<(path : string; key : string) : auto(STG)>;
                   load : function<(path : string; form : string; var stg : STG) : auto(MODEL)>;
                   cells : block<(var m : MODEL; form : string) : void>) : bool {
    return false if (!invoke(available, t, path))
    var inscope kept = default<STG>
    var kept_key = ""
    for (i in range(length(forms))) {
        let form = forms[i]
        let t0 = ref_time_ticks()
        let k = invoke(key, form)
        if (!empty(k) && k != kept_key) {
            delete kept
            kept <- invoke(stage, path, k)
            kept_key = k
        }
        var again = false
        for (j in range(i + 1, length(forms))) {
            again ||= !empty(k) && invoke(key, forms[j]) == k
        }
        var inscope own <- own_staging(kept, !empty(k), again)
        if (!empty(k) && !again) {
            kept_key = ""
        }
        var inscope m <- invoke(load, path, form, own)
        form_cells_timed("{area} {file} {form}", t0, m, form, cells)
    }
    return true
}

//! a skipped subtest of its own named `what`: the run's skipped cells count it, and the verdict of `t`'s other cells stands
def skip_cell(t : T?; what : string) {
    t |> run(what) @(t : T?) {
        t |> skip(what)
    }
}

//! The forms of `forms` this box serves, in the list's order, for a block's form list: `why` is the family's account of
//! what the box lacks for a form - a device, a build's module - and "" where it serves. A form with a reason is a skipped
//! cell of its own on `t` (`skip_cell`), named `tag`, the form and the reason, and is left out, so its load never starts.
def private served_forms_by(t : T?; tag : string; forms : array<string>; why : block<(form : string) : string>) : array<string> {
    var served : array<string>
    for (form in forms) {
        let lacks = invoke(why, form)
        if (empty(lacks)) {
            served |> push(form)
        } else {
            skip_cell(t, "{tag}, the {form} form: {lacks}")
        }
    }
    return <- served
}

//! the forms of `forms` this box serves, by the family's reason `why` for each form (`served_forms_by`)
def served_forms(t : T?; tag : string; forms : array<string>; why : function<(form : string) : string>) : array<string> =>
    <- served_forms_by(t, tag, forms) $(form) => invoke(why, form)

//! the forms of `forms` this box serves, where one reason `lacks` - "" where the box serves them - stands for every form
def served_forms(t : T?; tag : string; forms : array<string>; lacks : string) : array<string> =>
    <- served_forms_by(t, tag, forms) $(_form) => lacks

//! `asked` added to a block's form list `forms` where `why` is "",
//! else each a skipped cell of `file` naming the form and `why`
def forms_or_skips(t : T?; var forms : array<ModelForm>; file : string;
                   asked : array<ModelForm>; why : string) {
    if (empty(why)) {
        forms |> push_from(asked)
        return
    }
    for (f in asked) {
        skip_cell(t, "{file}, {form_name(f)}: {why}")
    }
}

//! the forms of `asked` the box serves (`served_forms`: each of the others a skipped cell of its own), added to a block's
//! form list `forms`
def add_served_forms(t : T?; var forms : array<string>; tag : string; asked : array<string>; why : function<(form : string) : string>) {
    var inscope served <- served_forms(t, tag, asked, why)
    forms |> push_from(served)
}

//! the forms of `asked` the box serves where one reason `lacks` stands for every form, added to a block's form list `forms`
def add_served_forms(t : T?; var forms : array<string>; tag : string; asked : array<string>; lacks : string) {
    var inscope served <- served_forms(t, tag, asked, lacks)
    forms |> push_from(served)
}

//! the reason a form either of two seats serves lacks: "" where the first seat stands (`first` empty), else the second's
def either_why(first, second : string) : string => empty(first) ? "" : second

//! what the box lacks for a form the Metal home's cells read, "" on a das_metal build
def metal_build_why() : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return ""
    } else {
        return "das_metal not present"
    }
}

//! a cell of a block: `cell` on the form's model `tw`, as the subtest `name`
def tower_cell(t : T?; name : string; var tw : auto(TW); cell : auto(CELL)) {
    unsafe {   // nolint:STYLE025 - the capture by reference needs it: the lint reads the generic before its types are known
        t |> run(name) @capture(& tw)(t : T ?) {
            invoke(cell, t, tw)
        }
    }
}

//! a cell of a block that keeps values for a compare after the block: `cell` on the form's model `tw` and the kept
//! state `kept`, as the subtest `name`
def tower_cell(t : T?; name : string; var tw : auto(TW); var kept : auto(KEPT); cell : auto(CELL)) {
    unsafe {   // nolint:STYLE025 - the capture by reference needs it: the lint reads the generic before its types are known
        t |> run(name) @capture(& tw, & kept)(t : T ?) {
            invoke(cell, t, tw, kept)
        }
    }
}

//! a compare between a file's forms, after its block: `cell` on the kept state `kept`, as the subtest `name`
def kept_cell(t : T?; name : string; var kept : auto(KEPT); cell : auto(CELL)) {
    unsafe {   // nolint:STYLE025 - the capture by reference needs it: the lint reads the generic before its types are known
        t |> run(name) @capture(& kept)(t : T ?) {
            invoke(cell, t, kept)
        }
    }
}

//! the declaration a function makes that holds a load of a model file its block does not load: `why`, a string literal
//! `REVIEW.das` reads, says why the load cannot be a form of its model's `with_cpu_and_gpu_model` call; logged as it runs
def own_load(why : string) {
    to_log(LOG_INFO, "own load: {why}\n")
}
