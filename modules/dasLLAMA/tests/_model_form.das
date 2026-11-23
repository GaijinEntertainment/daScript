options gen2
options _dasllama_internal = true

module _model_form shared public

require dastest/testing_boost   // T: a block's gate skips on it
require dasllama/dasllama_common   // QuantMode, Model
require dasllama/dasllama_kv_dtype   // KVDtype
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
    //! a CPU form's kernel backend pinned for its load and its cells
    //! (`with_kernel_backend`), put back after them: "" none
    backend : string
    //! a CPU form's K-quant native knob for its load
    kq_native : KqNative
    //! a device form whose last cell drops the device model
    //! (`device_drop_cell`): the driver held armed up to that cell
    drops_device : bool
}

//! a device form on the environment's mirror codec
def device_form(ring : bool = true; gather : bool = true; regions : int64 = 0l; host : bool = false; split_forms : bool = false;
                ctx : int64 = 0l; host_layers : int64 = 0l; pf_stream : bool = true;
                pf_window_rows : int64 = 0l; mtp_depth : int64 = 0l; head_off : bool = false;
                mmq_off : bool = false; host_ctx : int64 = 0l; fuse_bisect : int64 = -1l;
                perop : bool = false; pool_slots : int64 = 0l; strict_ctx : bool = false;
                cuda_window_fault : bool = false; mtp_head : string = "";
                drops_device : bool = false) : ModelForm =>
    ModelForm(on_gpu = true, kv = KVDtype.f16, kv_asked = false, ring = ring, gather = gather,
        quant = QuantMode.q8, regions = regions, host = host, split_forms = split_forms, ctx = ctx,
        host_layers = host_layers, pf_stream = pf_stream, pf_window_rows = pf_window_rows,
        mtp_depth = mtp_depth, head_off = head_off, mmq_off = mmq_off, host_ctx = host_ctx,
        fuse_bisect = fuse_bisect, perop = perop, pool_slots = pool_slots, strict_ctx = strict_ctx,
        cuda_window_fault = cuda_window_fault, mtp_head = mtp_head, drops_device = drops_device)

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
//! (`g_env_engine.pin_backend` the environment's pin), the K-quant knob `kq_native` set, on every box - on the q8 cached load
//! the CPU form answering the device forms where one ran
def cpu_cells_form(quant : QuantMode = QuantMode.q8; uncached : bool = false; mtp_head : string = "";
                   backend : string = ""; kq_native : KqNative = KqNative.kept) : ModelForm {
    var f = cpu_quant_form(quant)
    f.alone = true
    f.uncached = uncached
    f.mtp_head = mtp_head
    f.backend = backend
    f.kq_native = kq_native
    return f
}

//! whether two CPU forms are one load: the same load mode, loader, head pin, backend pin and K-quant knob
def same_cpu_load(a, b : ModelForm) : bool => (a.quant == b.quant && a.uncached == b.uncached && a.mtp_head == b.mtp_head
    && a.backend == b.backend && a.kq_native == b.kq_native)

//! the pass of the CPU load `load` the body sees on the codec `kv`: the codec set, `alone` kept on the `first` codec's pass
//! alone - a cell that stands alone runs on one pass a block, however many codecs the device forms armed
def codec_pass(load : ModelForm; kv : KVDtype; first : bool) : ModelForm {
    var f = load
    f.kv = kv
    f.alone = load.alone && first
    return f
}

//! the driver's default form on the device: the environment's codec, the rings, the token gather, the load's own regions,
//! context and planes
def device_default(f : ModelForm) : bool => (f.on_gpu && !f.kv_asked && f.ring && f.gather && f.regions == 0l && !f.host && !f.split_forms
    && f.ctx == 0l && f.mtp_depth == 0l && !f.head_off && !f.mmq_off && f.host_ctx == 0l && f.fuse_bisect < 0l
    && !f.perop && f.pool_slots == 0l && !f.strict_ctx && !f.cuda_window_fault && empty(f.mtp_head))

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
//! host-layers split and its pool), no host planes, and no fault seat
//! (the seat's load streams the file)
def is_lane_form(f : ModelForm) : bool => (f.on_gpu && !f.host && f.host_layers == 0l && !f.perop && f.pool_slots == 0l
    && !f.cuda_window_fault && empty(f.mtp_head))

//! a CPU form as its messages name it
def private cpu_form_name(f : ModelForm) : string {
    let uncached = f.uncached ? ", the uncached load" : ""
    let head = empty(f.mtp_head) ? uncached : "{uncached}, the NextN head {base_name(f.mtp_head)}"
    let pinned = empty(f.backend) ? head : "{head}, the {f.backend} backend pinned"
    let loader = f.kq_native == KqNative.kept ? pinned : "{pinned}, K-quant native {f.kq_native}"
    return "the {f.quant} CPU form{loader}" if (f.quant != QuantMode.q8)
    return "the CPU form ({f.kv} sessions{loader})"
}

//! the form as its messages name it
def form_name(f : ModelForm) : string {
    return cpu_form_name(f) if (!f.on_gpu)
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
    let whole = f.strict_ctx ? ", the context asked whole" : ""
    let fault = f.cuda_window_fault ? ", the window-fault seat" : ""
    let pinned = empty(f.mtp_head) ? fault : "{fault}, the NextN head {base_name(f.mtp_head)}"
    let shape = "{f.ring ? "rings" : "flat"}, {f.gather ? "device gather" : "host embed"}"
    return "the per-op form ({f.kv} asked, the whole-model route off)" if (f.perop)
    return "the device form ({f.kv} mirror, {shape}{regions}{planes}{fuse}{experts}{window}{depth}{head}{mmq}{host_ctx}{bisect}{heat}{whole}{pinned})"
}

//! where a block's seconds went, a line a form: under `DAS_LOG_LEVEL=info`
def log_form_seconds(name : string; load_usec, cells_usec : int) {
    to_log(LOG_INFO, "FORM {name}: load {float(load_usec) * 1e-6f:.3f} s, cells {float(cells_usec) * 1e-6f:.3f} s\n")
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
        let loaded = get_time_usec(t0)
        let t1 = ref_time_ticks()
        invoke(cells, m, form)
        log_form_seconds("{area} {file} {form}", loaded, get_time_usec(t1))
    }
    return true
}

//! the declaration a function makes that holds a load of a model file its block does not load: `why`, a string literal
//! `REVIEW.das` reads, says why the load cannot be a form of its model's `with_cpu_and_gpu_model` call; logged as it runs
def own_load(why : string) {
    to_log(LOG_INFO, "own load: {why}\n")
}
