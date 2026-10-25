options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_common shared public

require dasllama/dasllama_lint public
require cuda/das_cuda_boost public
require dasllama/dasllama_env

//! the device state the CUDA home's dispatch rail reads: the primary context, one stream, the warp as the subgroup
struct CudGpu {
    ready : bool
    tried : bool             //! one arm attempt a process: a box with no driver answers once
    cc : CudaContext?
    name : string
    sm_count : int
    sm : int                 //! the compute capability as major x 10 + minor (sm_120 reads 120)
    subgroup_size : int64    //! a warp
    rows_per_wg : int64      //! subgroups a workgroup at `CUD_WG_X` - the GEMV ladders' output rows a workgroup
    stream : void?
}

let CUD_WG_X = 64u   //! the kernel workgroup width the GEMV ladders size against (the Vulkan home's `WG_X`)
let FP4_RUNG_SM = 120   //! the first compute capability with the FP4 tensor rung (the block-scaled mma, an `sm_120a` text): below it the FP4 leaf has no kernel

var g_cud : CudGpu

def cud_init() : bool {
    return true if (g_cud.ready)
    return false if (g_cud.tried)
    g_cud.tried = true
    if (!cuda_available()) {
        to_log(LOG_INFO, "dasLLAMA cuda tier: no CUDA driver on this box\n")
        return false
    }
    if (!cuda_nvrtc_available()) {
        to_log(LOG_WARNING, "dasLLAMA cuda tier: the driver loads but NVRTC does not - kernels cannot compile\n")
        return false
    }
    var cc = cuda_create_context(0)
    if (cc == null) {
        to_log(LOG_WARNING, "dasLLAMA cuda tier: no context: {cuda_last_error()}\n")
        return false
    }
    g_cud.cc = cc
    g_cud.name = cuda_device_name(cc)
    g_cud.sm_count = cuda_device_sm_count(cc)
    g_cud.sm = cuda_device_compute_capability(cc)
    g_cud.subgroup_size = 32l
    g_cud.rows_per_wg = int64(CUD_WG_X / 32u)
    g_cud.stream = cuda_stream_create()
    g_cud.ready = true
    cuda_set_launch_pdl(g_cud.sm >= 90 && g_env_vulkan.cuda_pdl != 0l)   // every kernel of the home comes through the emitter, which puts the entry wait in each
    to_log(LOG_INFO, "dasLLAMA cuda tier: device ready ({g_cud.name}, {g_cud.sm_count} SMs, sm_{g_cud.sm}, driver {cuda_driver_version()}, nvrtc {cuda_nvrtc_version()}, programmatic dependent launch {cuda_launch_pdl() ? "on" : "off"})\n")
    return true
}

var private g_cud_proc_bufs : array<uint64>   //! device buffers a family holds for the process (the kq grid codebooks): the shutdown frees them

//! a device buffer the shutdown frees
def cud_proc_buf(b : uint64) {
    g_cud_proc_bufs |> push(b)
}

//! the device epoch: a shutdown moves it, so a holder of a process buffer knows its buffer went with the last device
def cud_epoch() : int => g_cud_epoch

//! every loaded kernel module released and the device state dropped; a class rebuilds its functions on its next ensure
def cud_shutdown() {
    return if (!g_cud.ready)
    for (b in g_cud_proc_bufs) {
        cuda_mem_free(b)
    }
    g_cud_proc_bufs |> clear()
    for (m in g_cud_modules) {
        cuda_release(m)
    }
    g_cud_modules |> clear()
    g_cud_epoch++
    cuda_stream_destroy(g_cud.stream)
    g_cud.stream = null
    cuda_release(g_cud.cc)
    g_cud.cc = null
    g_cud.ready = false
    g_cud.tried = false
}

def cud_sync() : bool => g_cud.ready && cuda_stream_synchronize(g_cud.stream)

var private g_cud_modules : array<CudaModule?>   //! every kernel module loaded under the live device, released at shutdown
var private g_cud_epoch = 0   //! bumped at shutdown: a class built under an older epoch rebuilds

struct CudClass {
    ready : bool
    epoch : int
    fns : array<void?>        //! one per [cuda_dispatch] kernel, by ordinal
    names : array<string>     //! ... the census name each launches under
    smem : array<uint>        //! ... the dynamic shared bytes its launch passes
    blocks : array<uint3>     //! ... its block
}

def cud_class_init(var c : CudClass) {
    c.ready = true
    c.epoch = g_cud_epoch
}

def cud_fn_made(c : CudClass; ord : int) : bool =>
    c.ready && c.epoch == g_cud_epoch && ord < length(c.fns) && c.fns[ord] != null

//! the kernel's function loaded from its cached cubin into slot `ord`; false with the reason logged when it does not compile or load
def cud_class_fn(var c : CudClass; ord : int; text, entry : string; smem : uint64; blk : uint3; kernel : string) : bool {
    if (c.epoch != g_cud_epoch) {
        c.fns |> clear()
        c.epoch = g_cud_epoch
    }
    while (length(c.fns) <= ord) {
        c.fns |> push(null)
        c.names |> push("")
        c.smem |> push(0u)
        c.blocks |> push(uint3(1u, 1u, 1u))
    }
    var err : string
    var mod : CudaModule?
    let fn = function_from_source(g_cud.cc, text, kernel, entry, mod, err, uint(smem))
    if (fn == null) {
        to_log(LOG_ERROR, "dasLLAMA cuda tier: {kernel}: {err}\n")
        return false
    }
    g_cud_modules |> push(mod)
    c.fns[ord] = fn
    c.names[ord] = kernel
    c.smem[ord] = uint(smem)
    c.blocks[ord] = blk
    let threads = int(blk.x * blk.y * blk.z)
    cuda_function_set_shared_carveout(fn, 100)   // every block the shared memory allows: the tiles' blocks an SM are the latency hiding the home has
    to_log(LOG_INFO, "dasLLAMA cuda tier: {kernel}: {cuda_function_regs(fn)} registers a thread, {smem} bytes of dynamic shared memory, {cuda_function_blocks_per_sm(fn, threads, smem)} blocks of {threads} threads an SM\n")
    return true
}

var private g_cud_blob : array<uint8>   //! the launch's parameter blob, rebuilt a launch; the driver copies it at the launch call

//! one launch of the class's kernel `ord`: `bufs` the device addresses in binding order, `pc` the args struct, `groups` blocks
def cud_launch(c : CudClass; ord : int; stream : void?; bufs : uint64 const[]; var pc : auto(PT)&; groups : uint) {
    if (!cud_fn_made(c, ord)) {
        panic("dasLLAMA cuda tier: kernel slot {ord} launched before its ensure")
    }
    let nb = length(bufs)
    let pbytes = typeinfo sizeof(type<PT>)
    let len = (nb * 8 + pbytes + 7) / 8 * 8
    g_cud_blob |> resize(len)
    unsafe {
        memcpy(addr<void?>(g_cud_blob[0]), addr<void?>(bufs[0]), int64(nb * 8))
        memcpy(addr<void?>(g_cud_blob[nb * 8]), addr<void?>(pc), int64(pbytes))
    }
    for (i in range(nb * 8 + pbytes, len)) {
        g_cud_blob[i] = 0u8
    }
    if (!cuda_launch(c.fns[ord], uint3(groups, 1u, 1u), c.blocks[ord], c.smem[ord], stream, unsafe(addr<void?>(g_cud_blob[0])), uint64(len))) {
        panic("dasLLAMA cuda tier: {c.names[ord]} launch failed: {cuda_last_error()}")
    }
    g_cud_census[c.names[ord]] = (g_cud_census?[c.names[ord]] ?? 0) + 1
}

var private g_cud_census : table<string; int>
var private g_cud_ensures : table<string; function<() : bool>>   //! every [cuda_dispatch] kernel's ensure, seeded at program start: the coverage universe, and the compile gate's reach

//! every [cuda_dispatch] kernel joins the coverage universe at program start (the macro generates an [init] calling this)
def cud_census_seed(kernel : string; ensure : function<() : bool>) {
    g_cud_ensures[kernel] = ensure
}

//! every seeded kernel's ensure run: the names whose function did not compile or load (the home's NVRTC compile gate)
def cud_ensure_all : array<string> {
    var failed <- [for (name in cud_kernel_names()); name; where !invoke(g_cud_ensures[name])]
    return <- failed
}

//! the launches `kernel` took this process; a name nothing seeded panics, so a misspelt key cannot read as a zero count
def cud_kernel_coverage_of(kernel : string) : int {
    if (!key_exists(g_cud_ensures, kernel)) {
        panic("dasLLAMA cuda tier: no kernel named {kernel} in the census")
    }
    return g_cud_census?[kernel] ?? 0
}

def cud_kernel_names : array<string> {
    var names <- [for (k in keys(g_cud_ensures)); k]
    names |> sort()
    return <- names
}

//! the milliseconds `blk`'s launches take on the device stream, event to event
def cud_time_ms(blk : block<void>) : float {
    let e0 = cuda_event_create()
    let e1 = cuda_event_create()
    cuda_event_record(e0, g_cud.stream)
    invoke(blk)
    cuda_event_record(e1, g_cud.stream)
    cuda_event_synchronize(e1)
    let ms = cuda_event_elapsed_ms(e0, e1)
    cuda_event_destroy(e0)
    cuda_event_destroy(e1)
    return ms
}
