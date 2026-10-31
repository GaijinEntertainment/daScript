options gen2
options indenting = 4
options no_aot   // every verb here calls a cuda_driver late extern, a dasbind proxy the AOT emitter cannot spell; the module serves the interpreter and the JIT

// Host sugar over the cuda_driver externs: the availability probes, scoped context access,
// source -> module -> function through the shared cubin cache, device buffer helpers, a one-line
// 1D launch, the graph verbs, and the live-object registry the leak gate reads. Every driver
// object and device allocation made here is counted, since no das leak detector sees them.

module das_cuda_boost shared public

require cuda_driver public
require math
require daslib/fio
require daslib/sha_256
require daslib/strings_boost

//! the driver library loads and cuInit succeeds; false on a box with no NVIDIA driver
def public cuda_available() : bool {
    static_if (typeinfo builtin_module_exists(dasbind)) {
        return cuda_driver_library_loads() && cuInit(0u) == 0
    } else {
        return false
    }
}

//! the NVRTC library loads: kernel TEXT compiles here; a deployed box runs cubins without it
def public cuda_nvrtc_available() : bool => cuda_nvrtc_library_loads()

def public cuda_driver_version() : int {
    var v = 0
    cuDriverGetVersion(v)
    return v
}

//! major * 1000 + minor * 10, the toolkit's own spelling
def public cuda_nvrtc_version() : int {
    var major = 0
    var minor = 0
    nvrtcVersion(major, minor)
    return major * 1000 + minor * 10
}

def public cuda_device_count() : int {
    var n = 0
    cuDeviceGetCount(n)
    return n
}

//! the driver's text for a result code, copied into this context's heap
def public cuda_error_text(result : int) : string {
    return "" if (result == 0)
    var text : void?
    cuGetErrorString(result, text)
    let driver_text = unsafe(reinterpret<string>(text))
    return empty(driver_text) ? "CUDA error {result}" : "{clone_string(driver_text)} ({result})"
}

var private g_cuda_last_error : string

//! the driver's text for the last failing boost call; empty after a success
def public cuda_last_error() : string => g_cuda_last_error

def private cu_ok(result : int; what : string) : bool {
    if (result == 0) {
        g_cuda_last_error = ""
        return true
    }
    g_cuda_last_error = "{what}: {cuda_error_text(result)}"
    return false
}

//! the live-object registry the leak gate reads: kind -> count of handles and buffers not yet released
var private g_cuda_live : table<string; int>

def private track(kind : string) {
    g_cuda_live[kind] = (g_cuda_live?[kind] ?? 0) + 1
}

def private untrack(kind : string) {
    g_cuda_live[kind] = (g_cuda_live?[kind] ?? 0) - 1
}

def public cuda_live_object_count() : int64 {
    var n = 0l
    for (c in values(g_cuda_live)) {
        n += int64(c)
    }
    return n
}

def public cuda_live_object_report() : string {
    return build_string() $(w) {
        for (kind, c in keys(g_cuda_live), values(g_cuda_live)) {
            if (c != 0) {
                w |> write("{kind} x{c}\n")
            }
        }
    }
}

//! a context: the device's retained primary context, bound to the creating thread, where every later call runs
struct public CudaContext {
    ctx : void?
    device : int
}

def public cuda_create_context(ordinal : int = 0) : CudaContext? {
    var dev = 0
    return null if (!cu_ok(cuDeviceGet(dev, ordinal), "cuDeviceGet"))
    var ctx : void?
    cu_ok(cuDevicePrimaryCtxSetFlags(dev, CU_CTX_SCHED_SPIN), "cuDevicePrimaryCtxSetFlags")   // spin on sync: a yielding wait makes a token step's latency vary run to run
    return null if (!cu_ok(cuDevicePrimaryCtxRetain(ctx, dev), "cuDevicePrimaryCtxRetain"))
    if (!cu_ok(cuCtxSetCurrent(ctx), "cuCtxSetCurrent")) {
        cuDevicePrimaryCtxRelease(dev)
        return null
    }
    track("CudaContext")
    return new CudaContext(ctx = ctx, device = dev)
}

[arch(at = "../ARCHITECTURE.md#cuda-context-per-thread")]
def public cuda_bind_context(cc : CudaContext?) : bool => cc != null && cu_ok(cuCtxSetCurrent(cc.ctx), "cuCtxSetCurrent")

def public cuda_release(var cc : CudaContext?) {
    return if (cc == null)
    if (cu_ok(cuDevicePrimaryCtxRelease(cc.device), "cuDevicePrimaryCtxRelease")) {
        untrack("CudaContext")
    }
    unsafe {
        delete cc
    }
}

//! Scoped primary context on device `ordinal`: creates, binds to this thread, invokes, releases.
//! `cc` is null when the box has no driver or no device - the block decides whether that is a
//! skip or a failure.
def public with_cuda_context(blk : block<(cc : CudaContext?) : void>; ordinal : int = 0) {
    var cc = cuda_available() && cuda_device_count() > ordinal ? cuda_create_context(ordinal) : null
    invoke(blk, cc)
    cuda_release(cc)
}

def public cuda_device_name(cc : CudaContext?) : string {
    var buf : array<uint8>
    buf |> resize(256)
    cu_ok(cuDeviceGetName(unsafe(addr<void?>(buf[0])), 256, cc.device), "cuDeviceGetName")
    var n = 0
    while (n < 256 && buf[n] != 0u8) {
        n++
    }
    buf |> resize(n)
    return string(buf)
}

def public cuda_device_attribute(cc : CudaContext?; attribute : int) : int {
    var v = 0
    cu_ok(cuDeviceGetAttribute(v, attribute, cc.device), "cuDeviceGetAttribute")
    return v
}

//! compute capability as major * 10 + minor (sm_120 reads 120) - the NVRTC target
def public cuda_device_compute_capability(cc : CudaContext?) : int =>
    cuda_device_attribute(cc, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR) * 10 + cuda_device_attribute(cc, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR)

//! the device's streaming multiprocessors (a 5060 Ti reads 36): what a wave model and a per-SM scratch count by
def public cuda_device_sm_count(cc : CudaContext?) : int =>
    cuda_device_attribute(cc, CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT)

def public cuda_device_total_memory(cc : CudaContext?) : uint64 {
    var bytes = 0ul
    cu_ok(cuDeviceTotalMem(bytes, cc.device), "cuDeviceTotalMem")
    return bytes
}

def public cuda_device_free_memory() : uint64 {
    var free_bytes = 0ul
    var total_bytes = 0ul
    cu_ok(cuMemGetInfo(free_bytes, total_bytes), "cuMemGetInfo")
    return free_bytes
}

//! the image NVRTC hands back: a cubin needs an sm_ arch option, PTX takes either
enum public CudaImage {
    cubin
    ptx
}

//! NVRTC: source text to the `kind` image. Empty with the compile log in `log` on failure; on success
//! `log` carries the warnings.
def public cuda_compile(source, name : string; compile_opts : array<string>; kind : CudaImage; var log : string&) : array<uint8> {
    var image : array<uint8>
    log = ""
    if (!cuda_nvrtc_available()) {
        log = "cuda_compile: no NVRTC library loaded (cuda_nvrtc_available() is false)"
        return <- image
    }
    var prog : void?
    var r = nvrtcCreateProgram(prog, source, empty(name) ? "kernel.cu" : name, 0, null, null)
    if (r != 0) {
        log = "nvrtcCreateProgram: {nvrtcGetErrorString(r)}"
        return <- image
    }
    var opts := compile_opts
    r = nvrtcCompileProgram(prog, length(opts), empty(opts) ? null : unsafe(addr<void?>(opts[0])))
    var log_size = 0ul
    nvrtcGetProgramLogSize(prog, log_size)
    if (log_size > 1ul) {
        var buf : array<uint8>
        buf |> resize(int64(log_size))
        nvrtcGetProgramLog(prog, unsafe(addr<void?>(buf[0])))
        var n = int(log_size)
        while (n > 0 && (buf[n - 1] == 0u8 || buf[n - 1] == uint8('\n'))) {
            n--
        }
        buf |> resize(n)
        log = string(buf)
    }
    if (r != 0) {
        log = "nvrtcCompileProgram: {nvrtcGetErrorString(r)}{empty(log) ? "" : "\n"}{log}"
    } else {
        var size = 0ul
        let cubin = kind == CudaImage.cubin
        if (cubin) {
            nvrtcGetCUBINSize(prog, size)
        } else {
            nvrtcGetPTXSize(prog, size)
        }
        if (cubin && size == 0ul) {
            log = "nvrtcGetCUBIN: no cubin - the arch option names a compute_ target, a cubin needs sm_{empty(log) ? "" : "\n"}{log}"
        } else {
            image |> resize(int64(size))
            if (cubin) {
                nvrtcGetCUBIN(prog, unsafe(addr<void?>(image[0])))
            } else {
                nvrtcGetPTX(prog, unsafe(addr<void?>(image[0])))
            }
        }
    }
    nvrtcDestroyProgram(prog)
    return <- image
}

//! a loaded cubin or PTX image; a kernel function is the module's and dies with it
struct public CudaModule {
    module_ : void?
}

def public cuda_load_module(image : array<uint8>; var error : string&) : CudaModule? {
    error = ""
    if (empty(image)) {
        error = "cuda_load_module: empty image"
        return null
    }
    var bytes := image
    bytes |> push(0u8)
    var mod : void?
    if (!cu_ok(cuModuleLoadDataEx(mod, unsafe(addr<void?>(bytes[0])), 0u, null, null), "cuModuleLoadDataEx")) {
        error = g_cuda_last_error
        return null
    }
    track("CudaModule")
    return new CudaModule(module_ = mod)
}

def public cuda_release(var mod : CudaModule?) {
    return if (mod == null)
    if (cu_ok(cuModuleUnload(mod.module_), "cuModuleUnload")) {
        untrack("CudaModule")
    }
    unsafe {
        delete mod
    }
}

//! null with the driver's text in cuda_last_error() when the module has no such entry point
def public cuda_get_function(mod : CudaModule?; name : string) : void? {
    var fn : void?
    return cu_ok(cuModuleGetFunction(fn, mod.module_, name), "cuModuleGetFunction") ? fn : null
}

//! the dynamic shared memory a launch may ask for without the opt-in
let public CUDA_SHARED_BYTES_WITHOUT_OPT_IN = 49152u

//! opts `fn` in to `bytes` of dynamic shared memory, up to the device's MAX_SHARED_MEMORY_PER_BLOCK_OPTIN
def public cuda_function_set_max_dynamic_shared(fn : void?; bytes : int) : bool =>
    cu_ok(cuFuncSetAttribute(fn, CU_FUNC_ATTRIBUTE_MAX_DYNAMIC_SHARED_SIZE_BYTES, bytes), "cuFuncSetAttribute")

//! the kernel's share of the SM's unified cache as shared memory, in percent (100: every block the shared memory allows)
def public cuda_function_set_shared_carveout(fn : void?; percent : int) : bool =>
    cu_ok(cuFuncSetAttribute(fn, CU_FUNC_ATTRIBUTE_PREFERRED_SHARED_MEMORY_CARVEOUT, percent), "cuFuncSetAttribute")

//! the kernel's registers a thread, as the loaded binary holds it; 0 where the driver does not answer
def public cuda_function_regs(fn : void?) : int {
    var v = 0
    return cu_ok(cuFuncGetAttribute(v, CU_FUNC_ATTRIBUTE_NUM_REGS, fn), "cuFuncGetAttribute") ? v : 0
}

//! the blocks of `block_size` threads at `dynamic_smem` bytes one SM holds at once; 0 where the driver does not answer
def public cuda_function_blocks_per_sm(fn : void?; block_size : int; dynamic_smem : uint64) : int {
    var v = 0
    return cu_ok(cuOccupancyMaxActiveBlocksPerMultiprocessor(v, fn, block_size, dynamic_smem), "cuOccupancyMaxActiveBlocksPerMultiprocessor") ? v : 0
}

var private g_cuda_device_allocs : table<uint64; uint64>
var private g_cuda_device_bytes = 0l

//! device memory is a 64-bit device address, never a host pointer; 0 on failure
def public cuda_mem_alloc(bytes : uint64) : uint64 {
    var dptr = 0ul
    return 0ul if (!cu_ok(cuMemAlloc(dptr, bytes), "cuMemAlloc"))
    g_cuda_device_allocs[dptr] = bytes
    g_cuda_device_bytes += int64(bytes)
    track("CudaBuffer")
    return dptr
}

def public cuda_mem_free(dptr : uint64) {
    return if (dptr == 0ul)
    let bytes = g_cuda_device_allocs?[dptr] ?? 0ul
    if (bytes == 0ul) {
        panic("cuda_mem_free: {dptr} is not a live cuda_mem_alloc result")
    }
    return if (!cu_ok(cuMemFree(dptr), "cuMemFree"))
    g_cuda_device_allocs |> erase(dptr)
    g_cuda_device_bytes -= int64(bytes)
    untrack("CudaBuffer")
}

//! every buffer of a launch's set, each freed as `cuda_mem_free` frees one
def public cuda_mem_free(bufs : auto(TT)[]) {
    for (b in bufs) {
        cuda_mem_free(b)
    }
}

def public cuda_device_bytes_allocated() : int64 => g_cuda_device_bytes

//! pinned host memory the device reads and writes in place at `dev`: a kernel's inputs or its answer cross with no copy-engine
//! work (a memcpy node in a graph hops to the copy engine and back); null / 0 on failure; freed by cuda_mem_free_host
def public cuda_mem_alloc_host_mapped(bytes : uint64) : tuple<host : void?; dev : uint64> {
    var p : void?
    return (host = null, dev = 0ul) if (!cu_ok(cuMemHostAlloc(p, bytes, CU_MEMHOSTALLOC_DEVICEMAP), "cuMemHostAlloc"))
    var d = 0ul
    if (!cu_ok(cuMemHostGetDevicePointer(d, p, 0u), "cuMemHostGetDevicePointer")) {
        cuMemFreeHost(p)
        return (host = null, dev = 0ul)
    }
    track("CudaHostBuffer")
    return (host = p, dev = d)
}

def public cuda_mem_free_host(p : void?) {
    return if (p == null)
    if (cu_ok(cuMemFreeHost(p), "cuMemFreeHost")) {
        untrack("CudaHostBuffer")
    }
}

//! host to device; on `stream` the copy is ordered ahead of that stream's later launches (a pageable `src` is staged before
//! the call returns, so it may be freed), on the legacy stream (null) it is ordered only against other legacy-stream work -
//! a kernel on a non-blocking stream may run before the DMA lands
def public cuda_memcpy_htod(dst : uint64; src : void?; bytes : uint64; stream : void? = null) : bool {
    return true if (bytes == 0ul)
    return (stream != null
        ? cu_ok(cuMemcpyHtoDAsync(dst, src, bytes, stream), "cuMemcpyHtoDAsync")
        : cu_ok(cuMemcpyHtoD(dst, src, bytes), "cuMemcpyHtoD"))
}

def public cuda_memcpy_dtoh(dst : void?; src : uint64; bytes : uint64; stream : void? = null) : bool {
    return true if (bytes == 0ul)
    return (stream != null
        ? cu_ok(cuMemcpyDtoHAsync(dst, src, bytes, stream), "cuMemcpyDtoHAsync")
        : cu_ok(cuMemcpyDtoH(dst, src, bytes), "cuMemcpyDtoH"))
}

def public cuda_memcpy_dtod(dst : uint64; src : uint64; bytes : uint64; stream : void? = null) : bool {
    return true if (bytes == 0ul)
    return (stream != null
        ? cu_ok(cuMemcpyDtoDAsync(dst, src, bytes, stream), "cuMemcpyDtoDAsync")
        : cu_ok(cuMemcpyDtoD(dst, src, bytes), "cuMemcpyDtoD"))
}

def public cuda_memset_d8(dst : uint64; value : uint8; bytes : uint64; stream : void? = null) : bool {
    return true if (bytes == 0ul)
    return (stream != null
        ? cu_ok(cuMemsetD8Async(dst, value, bytes, stream), "cuMemsetD8Async")
        : cu_ok(cuMemsetD8(dst, value, bytes), "cuMemsetD8"))
}

def public cuda_stream_create() : void? {
    var s : void?
    return null if (!cu_ok(cuStreamCreate(s, CU_STREAM_NON_BLOCKING), "cuStreamCreate"))
    track("CudaStream")
    return s
}

def public cuda_stream_destroy(stream : void?) {
    return if (stream == null)
    if (cu_ok(cuStreamDestroy(stream), "cuStreamDestroy")) {
        untrack("CudaStream")
    }
}

def public cuda_stream_synchronize(stream : void?) : bool => cu_ok(cuStreamSynchronize(stream), "cuStreamSynchronize")

def public cuda_event_create() : void? {
    var e : void?
    return null if (!cu_ok(cuEventCreate(e, CU_EVENT_DEFAULT), "cuEventCreate"))
    track("CudaEvent")
    return e
}

def public cuda_event_destroy(event : void?) {
    return if (event == null)
    if (cu_ok(cuEventDestroy(event), "cuEventDestroy")) {
        untrack("CudaEvent")
    }
}

def public cuda_event_record(event : void?; stream : void? = null) : bool => cu_ok(cuEventRecord(event, stream), "cuEventRecord")

def public cuda_event_synchronize(event : void?) : bool => cu_ok(cuEventSynchronize(event), "cuEventSynchronize")

//! an event with no timestamp (`CU_EVENT_DISABLE_TIMING`): a fence between two streams, never timed
def public cuda_event_create_untimed() : void? {
    var e : void?
    return null if (!cu_ok(cuEventCreate(e, CU_EVENT_DISABLE_TIMING), "cuEventCreate"))
    track("CudaEvent")
    return e
}

//! whether the event's recorded work has completed, without waiting; a never-recorded event reads as ready
def public cuda_event_ready(event : void?) : bool {
    let r = cuEventQuery(event)
    return false if (r == CUDA_ERROR_NOT_READY)
    return cu_ok(r, "cuEventQuery")
}

//! `stream`'s later work waits for `event`'s recorded work, on the device - the host does not block
def public cuda_stream_wait_event(stream, event : void?) : bool => cu_ok(cuStreamWaitEvent(stream, event, 0u), "cuStreamWaitEvent")

//! -1 when the driver refuses (an event not yet recorded)
def public cuda_event_elapsed_ms(start, end : void?) : float {
    var ms = 0.0
    return cu_ok(cuEventElapsedTime(ms, start, end), "cuEventElapsedTime") ? ms : -1.0
}

def public cuda_context_synchronize() : bool => cu_ok(cuCtxSynchronize(), "cuCtxSynchronize")

//! the launches `blk` issues on `stream` captured into one executable graph (`cuda_graph_launch` replays them as one launch,
//! the kernels' parameters as captured - a per-step input rides a buffer the captured launches read); null with the driver's
//! text in cuda_last_error() when the capture or the instantiation fails. The graph object itself is released on the way out.
def public cuda_graph_capture(stream : void?; blk : block<void>) : void? {
    return null if (!cu_ok(cuStreamBeginCapture(stream, CU_STREAM_CAPTURE_MODE_THREAD_LOCAL), "cuStreamBeginCapture"))
    invoke(blk)
    var graph : void?
    return null if (!cu_ok(cuStreamEndCapture(stream, graph), "cuStreamEndCapture"))
    var exec : void?
    let ok = cu_ok(cuGraphInstantiateWithFlags(exec, graph, 0ul), "cuGraphInstantiateWithFlags")
    cuGraphDestroy(graph)
    return null if (!ok)
    track("CudaGraph")
    return exec
}

def public cuda_graph_launch(exec : void?; stream : void?) : bool => cu_ok(cuGraphLaunch(exec, stream), "cuGraphLaunch")

def public cuda_graph_destroy(exec : void?) {
    return if (exec == null)
    if (cu_ok(cuGraphExecDestroy(exec), "cuGraphExecDestroy")) {
        untrack("CudaGraph")
    }
}

//! the driver's CUlaunchConfig: the grid, the block, the shared bytes, the stream, then the attribute array and its count
struct private CuLaunchConfig {
    grid_x, grid_y, grid_z : uint
    block_x, block_y, block_z : uint
    shared_bytes : uint
    pad0 : uint
    stream : void?
    attrs : void?
    num_attrs : uint
    pad1 : uint
}

//! one CUlaunchAttribute: the id, its pad to 8, and the 64-byte value union (the first word the flag the serialization attribute reads)
struct private CuLaunchAttribute {
    id : uint
    pad : uint
    value : uint64[8]
}

let private CU_LAUNCH_ATTRIBUTE_PROGRAMMATIC_STREAM_SERIALIZATION = 6u

var private g_cuda_launch_pdl = false

//! only a stream whose every kernel came through the emitter may turn this on - the emitter's entry wait makes the early launch safe
[arch(at = "../ARCHITECTURE.md#cuda-programmatic-dependent-launch")]
def public cuda_set_launch_pdl(on : bool) {
    g_cuda_launch_pdl = on
}

def public cuda_launch_pdl() : bool => g_cuda_launch_pdl

//! one launch: `params` is the kernel's whole parameter space as one byte blob - the by-value structs in
//! parameter order at their natural alignment - handed to the driver through the launch's buffer extra,
//! so a das struct of (buffer addresses, args) is the launch argument
def public cuda_launch(fn : void?; grid, block_size : uint3; shared_bytes : uint; stream : void?; params : void?; params_len : uint64) : bool {
    if (fn == null) {
        panic("cuda_launch: null function")
    }
    if (grid.x == 0u || grid.y == 0u || grid.z == 0u || block_size.x == 0u || block_size.y == 0u || block_size.z == 0u) {
        panic("cuda_launch: zero launch dimension (grid {grid}, block {block_size})")
    }
    var len = params_len
    var extra : uint64[5]
    extra[0] = CU_LAUNCH_PARAM_BUFFER_POINTER
    extra[1] = intptr(params)
    extra[2] = CU_LAUNCH_PARAM_BUFFER_SIZE
    extra[3] = intptr(unsafe(addr<void?>(len)))
    extra[4] = CU_LAUNCH_PARAM_END
    let extra_p = params != null ? unsafe(addr<void?>(extra[0])) : null
    if (g_cuda_launch_pdl) {
        var attr = CuLaunchAttribute(id = CU_LAUNCH_ATTRIBUTE_PROGRAMMATIC_STREAM_SERIALIZATION)
        attr.value[0] = 1ul
        var cfg = CuLaunchConfig(grid_x = grid.x, grid_y = grid.y, grid_z = grid.z, block_x = block_size.x, block_y = block_size.y, block_z = block_size.z,
            shared_bytes = shared_bytes, stream = stream, attrs = unsafe(addr<void?>(attr)), num_attrs = 1u)
        return cu_ok(cuLaunchKernelEx(unsafe(addr<void?>(cfg)), fn, null, extra_p), "cuLaunchKernelEx")
    }
    return cu_ok(cuLaunchKernel(fn, grid.x, grid.y, grid.z, block_size.x, block_size.y, block_size.z, shared_bytes, stream, null, extra_p), "cuLaunchKernel")
}

//! the NVRTC options every kernel compiles with: the context's own sm target, C++17 and fast math;
//! `arch_suffix` is "a" for a kernel using the arch-specific tensor forms (FP4 on sm_120 needs it)
def public cuda_compile_options(cc : CudaContext?; arch_suffix : string = "") : array<string> =>
    <- [ "--gpu-architecture=sm_{cuda_device_compute_capability(cc)}{arch_suffix}", "--std=c++17", "--use_fast_math" ]

let public CUDA_CACHE_DIR = ".cache/daslang/cuda"

//! the cache key of a kernel text: every input the cubin depends on, so a toolkit or driver update,
//! another card or another option set never serves a stale image
def public cubin_cache_key(source : string; compile_opts : array<string>) : string =>
    sha256_hex("{cuda_nvrtc_version()}|{cuda_driver_version()}|{join(compile_opts, " ")}|{source}")

//! source text -> cubin through the cache in `cache_dir` (by default the shared one under the working
//! directory, the JIT object cache's home): a hit reads the file, a miss compiles and writes it. Empty
//! with `error` filled on a compile failure.
def public cubin_cached(source, name : string; compile_opts : array<string>; var error : string&; cache_dir : string = CUDA_CACHE_DIR) : array<uint8> {
    let path = path_join(cache_dir, "{cubin_cache_key(source, compile_opts)}.cubin")
    var image : array<uint8>
    let cached = stat(path)
    if (cached.is_reg && cached.size > 0ul) {
        image |> resize(int64(cached.size))
        fopen(path, "rb") $(f) {
            if (f == null || fread(f, image) != int(cached.size)) {
                image |> clear()
            }
        }
        return <- image if (!empty(image))
    }
    var log : string
    delete image
    image <- cuda_compile(source, name, compile_opts, CudaImage.cubin, log)
    if (empty(image)) {
        error = log
        return <- image
    }
    if (!stat(cache_dir).is_dir) {
        mkdir_rec(cache_dir)
    }
    // a process-unique temp file renamed into place: a reader of the final path never sees a partial cubin
    let tmp = "{path}.{ref_time_ticks()}.tmp"
    var written = false
    fopen(tmp, "wb") $(f) {
        if (f != null) {
            written = fwrite(f, image) == length(image)
        }
    }
    var rename_error : string
    if (!written || !rename(tmp, path, rename_error)) {
        remove(tmp, rename_error)
    }
    return <- image
}

//! source text -> loaded module, cubin for the context's device (cached in `cache_dir`); null with `error` filled on a compile or load failure
[arch(at="../ARCHITECTURE_EMIT.md#cuda-kernel-vocabulary-tiers")]
def public module_from_source(cc : CudaContext?; source, name : string; var error : string&; arch_suffix : string = "";
                              cache_dir : string = CUDA_CACHE_DIR) : CudaModule? {
    let suffix = !empty(arch_suffix) ? arch_suffix : (find(source, "__DAS_ARCH_A__") >= 0 ? "a" : "")
    var image <- cubin_cached(source, name, cuda_compile_options(cc, suffix), error, cache_dir)
    return null if (empty(image))
    return cuda_load_module(image, error)
}

//! false with the driver's text in cuda_last_error()
def private opt_in_dynamic_shared(fn : void?; shared_bytes : uint) : bool =>
    shared_bytes <= CUDA_SHARED_BYTES_WITHOUT_OPT_IN || cuda_function_set_max_dynamic_shared(fn, int(shared_bytes))

//! source text -> loaded module -> its entry point, the dynamic-shared opt-in applied where `shared_bytes` passes the
//! default: null with `error` filled (and `mod` null, the module released) when the text does not compile or load, the
//! entry is missing or the opt-in is refused; `mod` holds the module the function lives in, released by the caller
def public function_from_source(cc : CudaContext?; source, name, entry : string; var mod : CudaModule?&; var error : string&;
                                shared_bytes : uint = 0u; arch_suffix : string = ""; cache_dir : string = CUDA_CACHE_DIR) : void? {
    mod = module_from_source(cc, source, "{name}.cu", error, arch_suffix, cache_dir)
    return null if (mod == null)
    let fn = cuda_get_function(mod, entry)
    if (fn == null) {
        error = "entry `{entry}` missing: {cuda_last_error()}"
    } elif (!opt_in_dynamic_shared(fn, shared_bytes)) {
        error = "{shared_bytes} bytes of dynamic shared memory refused: {cuda_last_error()}"
    } else {
        return fn
    }
    cuda_release(mod)
    mod = null
    return null
}

//! upload a das array into fresh device memory; 0 with the driver's text in cuda_last_error() on failure.
//! `stream` is the stream whose kernels read the buffer: a copy on the legacy stream (null) only STAGES a pageable
//! array and returns, its DMA ordered after nothing a non-blocking stream launches, so a kernel there reads the old bytes
def public buf_upload(arr : array<auto(TT)>; stream : void? = null) : uint64 {
    let bytes = uint64(long_length(arr)) * uint64(typeinfo sizeof(arr[0]))
    let dptr = cuda_mem_alloc(bytes)
    if (dptr != 0ul && !cuda_memcpy_htod(dptr, unsafe(addr<void?>(arr[0])), bytes, stream)) {
        cuda_mem_free(dptr)
        return 0ul
    }
    return dptr
}

//! fresh device memory holding `nn` copies of `value`, the copy ordered on `stream` as `buf_upload`'s
def public buf_fill(nn : int; value : auto(TT); stream : void? = null) : uint64 {
    var arr : array<TT -const>
    arr |> resize(nn)
    for (v in arr) {
        v = value
    }
    return buf_upload(arr, stream)
}

//! copy `nn` elements of device memory into a fresh das array (`t` fixes TT; the parameter
//! position appends const to TT, so every use strips it back with -const)
[unused_argument(t)]
def public buf_download(dptr : uint64; nn : int; t : type<auto(TT)>) : array<TT -const> {
    var res : array<TT -const>
    res |> resize(nn)
    if (nn > 0) {
        cuda_memcpy_dtoh(unsafe(addr<void?>(res[0])), dptr, uint64(nn) * uint64(typeinfo sizeof(res[0])))
    }
    return <- res
}

//! One 1D launch of `fn` with `params` as the whole parameter space - the das struct of (buffer
//! addresses, args) the kernel declares in the same order - over ceil(threads / threads_per_block)
//! blocks, then a context sync. Returns false with `error` filled when the launch or the sync fails.
def public run_compute_1d(fn : void?; var params : auto(PA); threads, threads_per_block : uint; var error : string&; shared_bytes : uint = 0u) : bool {
    let blocks = (threads + threads_per_block - 1u) / threads_per_block
    if (!opt_in_dynamic_shared(fn, shared_bytes)) {
        error = cuda_last_error()
        return false
    }
    let launched = cuda_launch(fn, uint3(blocks, 1u, 1u), uint3(threads_per_block, 1u, 1u), shared_bytes, null,
        unsafe(addr<void?>(params)), uint64(typeinfo sizeof(type<PA>)))
    if (!launched || !cuda_context_synchronize()) {
        error = cuda_last_error()
        return false
    }
    return true
}

//! The leak gate every tests/cuda file closes on: driver objects and device memory are invisible to all six das leak
//! detectors, so the boost's counter is the only witness - the live-object report is logged when the count is non-zero,
//! and `t |> equal(cuda_log_leaks_if_any(), 0l)` keeps the gate one line while a red run names its leaks.
def public cuda_log_leaks_if_any : int64 {
    let live = cuda_live_object_count()
    if (live != 0l) {
        to_log(LOG_ERROR, "cuda live objects:\n{cuda_live_object_report()}")
    }
    return live
}
