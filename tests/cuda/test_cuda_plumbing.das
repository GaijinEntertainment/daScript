options gen2
options indenting = 4
options no_aot   // the host gate calls the driver's late externs by name, and the AOT emitter spells a dasbind proxy as its mangled name, which no C++ compiler accepts (the dasbind tests are no_aot the same way)

// The binding's host gate: the driver and NVRTC load by name, a kernel text compiles to a cubin
// for this card, loads, launches with the struct-of-pointers parameter blob, computes the
// expected values, the second compile of the same text is a cache hit, and every handle comes
// back; then the boost's refusal arms, each answering null or false with the driver's or the
// compiler's text, or panicking by name. The file compiles everywhere; the device arms feint where
// no driver, NVRTC or device is.

require dastest/testing_boost public
require cuda/das_cuda_boost
require daslib/fio
require daslib/jobque_boost   // the worker-thread bind: a context of its own on another thread
require strings
require math   // min, clamp: the prewarm's worker count, computed beside it

let N = 1 << 16
let TPB = 256u

//! the kernel the gate launches - the one hand-written kernel text under the module's rule that every other
//! kernel is emitted; its parameter space is one by-value struct of pointers and one by-value args struct
let AXPY_SRC = "
struct KBufs \{ float* o; const float* __restrict__ y; \};
struct Args \{ float a; unsigned n; \};
extern \"C\" __global__ void axpy(KBufs b, Args ka) \{
    unsigned i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < ka.n) b.o[i] = ka.a * b.o[i] + b.y[i];
\}
"

struct AxpyParams {
    o : uint64
    y : uint64
    a : float
    n : uint
}

//! the cubin cache every cell compiles into: this run's own, never the shared one under the working directory
def private cubin_dir() : string => path_join(test_temp_dir(), "cubin")

[test(description = "The driver, NVRTC, the cubin cache and a struct-of-pointers launch work end to end.")]
def test_cuda_plumbing(t : T?) {
    t |> run("driver + nvrtc + cubin cache + struct-of-pointers launch") <| @(t : T?) {
        if (!cuda_available()) {
            feint("no CUDA driver on this box; plumbing checks skipped\n")
            return
        }
        if (!cuda_nvrtc_available()) {
            feint("no NVRTC on this box; plumbing checks skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; plumbing checks skipped\n")
                return
            }
            let base = cuda_live_object_count()
            t |> success(cuda_device_compute_capability(cc) >= 50, "a real compute capability: sm_{cuda_device_compute_capability(cc)} on {cuda_device_name(cc)}")
            t |> success(cuda_driver_version() >= 12000, "driver API {cuda_driver_version()}")
            t |> success(cuda_nvrtc_version() >= 12000, "nvrtc {cuda_nvrtc_version()}")

            var cerr : string
            let opts <- cuda_compile_options(cc)
            let cache_dir = cubin_dir()
            let key_path = cubin_cache_path(cache_dir, AXPY_SRC, opts)
            if (stat(key_path).is_reg) {
                remove(key_path)
            }
            var image1 <- cubin_cached(AXPY_SRC, "axpy.cu", opts, cerr, cache_dir)
            t |> success(!empty(image1), "nvrtc compiles the gate kernel to a cubin: {cerr}")
            t |> success(stat(key_path).is_reg, "the miss wrote the cubin to the cache directory")
            let image2 <- cubin_cached(AXPY_SRC, "axpy.cu", opts, cerr, cache_dir)
            t |> equal(length(image2), length(image1), "the hit reads the same bytes back")

            var lerr : string
            var mod = cuda_load_module(image1, lerr)
            t |> success(mod != null, "the cubin loads: {lerr}")
            let fn = cuda_get_function(mod, "axpy")
            t |> success(fn != null, "the entry point resolves: {cuda_last_error()}")
            t |> equal(cuda_get_function(mod, "no_such_kernel"), null, "a missing entry point reads null")
            t |> success(!empty(cuda_last_error()), "and names the driver's refusal: {cuda_last_error()}")

            var o, y : array<float>
            o |> resize(N)
            y |> resize(N)
            for (i in range(N)) {
                o[i] = float(i)
                y[i] = 1.0
            }
            let d_o = buf_upload(o)
            let d_y = buf_upload(y)
            t |> success(d_o != 0ul && d_y != 0ul, "device buffers allocate and fill: {cuda_last_error()}")
            t |> equal(cuda_device_bytes_allocated(), int64(2 * N * 4), "the byte counter reads both buffers")
            var params = AxpyParams(o = d_o, y = d_y, a = 2.0, n = uint(N))
            var rerr : string
            t |> success(run_compute_1d(fn, params, uint(N), TPB, rerr), "the launch and its sync: {rerr}")
            let got <- buf_download(d_o, N, type<float>)
            var bad = 0
            for (i in range(N)) {
                if (got[i] != 2.0 * float(i) + 1.0) {
                    bad++
                }
            }
            t |> equal(bad, 0, "every element reads a * o + y")

            let ev0 = cuda_event_create()
            let ev1 = cuda_event_create()
            t |> success(cuda_event_record(ev0) && run_compute_1d(fn, params, uint(N), TPB, rerr) && cuda_event_record(ev1), "a timed launch: {rerr}")
            t |> success(cuda_event_synchronize(ev1) && cuda_event_elapsed_ms(ev0, ev1) >= 0.0, "events time the launch: {cuda_event_elapsed_ms(ev0, ev1)} ms")
            cuda_event_destroy(ev0)
            cuda_event_destroy(ev1)

            cuda_mem_free(d_o)
            cuda_mem_free(d_y)
            cuda_release(mod)
            t |> equal(cuda_live_object_count(), base, "every handle and buffer of the gate came back")
            t |> equal(cuda_device_bytes_allocated(), 0l)
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test(description = "Device queries, register, occupancy and carveout queries, mapped host memory, memset, device-to-device copy and the PDL launch work.")]
def test_cuda_host_rails(t : T?) {
    t |> run("device queries, the kernel's register, occupancy and carveout queries, mapped host memory the device reads in place, memset, dtod copy, the PDL launch") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            feint("no CUDA driver with NVRTC on this box; host rails skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; host rails skipped\n")
                return
            }
            let base = cuda_live_object_count()
            t |> success(cuda_device_sm_count(cc) > 0, "the SM count: {cuda_device_sm_count(cc)}")
            let total = cuda_device_total_memory(cc)
            let free = cuda_device_free_memory()
            t |> success(total > 0ul && free > 0ul && free <= total, "the memory sizes: {free} of {total} bytes free")

            var cerr : string
            let opts <- cuda_compile_options(cc)
            var image <- cubin_cached(AXPY_SRC, "axpy.cu", opts, cerr, cubin_dir())
            var lerr : string
            var mod = cuda_load_module(image, lerr)
            let fn = cuda_get_function(mod, "axpy")
            t |> success(fn != null, "the gate kernel loads: {cerr} {lerr}")
            t |> success(cuda_function_regs(fn) > 0, "registers a thread: {cuda_function_regs(fn)}")
            t |> success(cuda_function_blocks_per_sm(fn, int(TPB), 0ul) > 0, "blocks an SM at {TPB} threads: {cuda_function_blocks_per_sm(fn, int(TPB), 0ul)}")
            t |> success(cuda_function_set_shared_carveout(fn, 50), "the shared carveout sets: {cuda_last_error()}")

            // y in mapped host memory, zeroed by the device's memset; o on the device, copied aside device to device
            let mapped = cuda_mem_alloc_host_mapped(uint64(N * 4))
            t |> success(mapped.host != null && mapped.dev != 0ul, "mapped host memory allocates: {cuda_last_error()}")
            t |> success(cuda_memset_d8(mapped.dev, 0u8, uint64(N * 4)), "the memset lands in the mapped block: {cuda_last_error()}")
            var o : array<float>
            o |> resize(N)
            for (i in range(N)) {
                o[i] = float(i)
            }
            let d_o = buf_upload(o)
            let d_copy = buf_fill(N, 0.0)
            t |> success(cuda_memcpy_dtod(d_copy, d_o, uint64(N * 4)), "the device-to-device copy: {cuda_last_error()}")
            t |> success(cuda_stream_synchronize(null), "the copies land")
            let copied <- buf_download(d_copy, N, type<float>)
            var bad = 0
            for (i in range(N)) {
                if (copied[i] != float(i)) {
                    bad++
                }
            }
            t |> equal(bad, 0, "the copy reads o back")
            var params = AxpyParams(o = d_o, y = mapped.dev, a = 2.0, n = uint(N))
            var rerr : string
            cuda_set_launch_pdl(true)
            t |> success(cuda_launch_pdl(), "the PDL attribute arms")
            let launched = run_compute_1d(fn, params, uint(N), TPB, rerr)
            cuda_set_launch_pdl(false)
            t |> success(launched, "the launch under PDL and its sync: {rerr}")
            let got <- buf_download(d_o, N, type<float>)
            bad = 0
            for (i in range(N)) {
                if (got[i] != 2.0 * float(i)) {
                    bad++
                }
            }
            t |> equal(bad, 0, "every element reads a * o + 0: the kernel read the zeroed mapped y")

            cuda_mem_free(d_o)
            cuda_mem_free(d_copy)
            cuda_mem_free_host(mapped.host)
            cuda_release(mod)
            t |> equal(cuda_live_object_count(), base, "every handle and buffer of the rails came back")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test(description = "Three launches captured on a stream replay as one graph launch twice with the parameters as captured.")]
def test_cuda_graph_replay(t : T?) {
    t |> run("three launches captured on a stream, replayed as one launch twice with the parameters as captured") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            feint("no CUDA driver or NVRTC on this box; graph checks skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; graph checks skipped\n")
                return
            }
            let base = cuda_live_object_count()
            var cerr : string
            let opts <- cuda_compile_options(cc)
            var image <- cubin_cached(AXPY_SRC, "axpy.cu", opts, cerr, cubin_dir())
            var lerr : string
            var mod = cuda_load_module(image, lerr)
            t |> success(mod != null, "the gate kernel loads: {lerr}")
            let fn = cuda_get_function(mod, "axpy")
            var o <- [for (i in range(N)); float(i)]
            var y <- [for (_i in range(N)); 1.0]
            let d_o = buf_upload(o)
            let d_y = buf_upload(y)
            var params = AxpyParams(o = d_o, y = d_y, a = 2.0, n = uint(N))
            let stream = cuda_stream_create()
            let blocks = (uint(N) + TPB - 1u) / TPB
            let exec = cuda_graph_capture(stream) {
                for (_k in range(3)) {
                    cuda_launch(fn, uint3(blocks, 1u, 1u), uint3(TPB, 1u, 1u), 0u, stream, unsafe(addr<void?>(params)), uint64(typeinfo sizeof(type<AxpyParams>)))
                }
            }
            t |> success(exec != null, "three launches capture into a graph: {cuda_last_error()}")
            t |> success(cuda_graph_launch(exec, stream) && cuda_graph_launch(exec, stream) && cuda_stream_synchronize(stream), "the graph replays twice: {cuda_last_error()}")
            let after <- buf_download(d_o, N, type<float>)
            var bad = 0
            for (i in range(N)) {
                var want = float(i)
                for (_k in range(6)) {
                    want = 2.0 * want + 1.0
                }
                if (after[i] != want) {
                    bad++
                }
            }
            t |> equal(bad, 0, "every element took the six captured applications")
            cuda_graph_destroy(exec)
            cuda_stream_destroy(stream)
            cuda_mem_free(d_o)
            cuda_mem_free(d_y)
            cuda_release(mod)
            t |> equal(cuda_live_object_count(), base, "the graph, the stream, the buffers and the module came back")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

//! the cubin cache's arms on a copy of the gate kernel no other cell compiles (the first line carries the clock), so the
//! files it overwrites are its own: a hit serves the file's bytes, an empty file is a miss, and two option sets of one
//! length key two files
[test(description = "A cubin cache hit reads the file, an empty file recompiles, and the compile options key the file.")]
def test_cuda_cubin_cache_arms(t : T?) {
    t |> run("the hit reads the file, an empty file recompiles, the options key the file") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            feint("no CUDA driver or NVRTC on this box; cache arms skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; cache arms skipped\n")
                return
            }
            let base = cuda_live_object_count()
            let src = "// cache arm {ref_time_ticks()}\n{AXPY_SRC}"
            let opts <- cuda_compile_options(cc)
            let cache_dir = cubin_dir()
            let key_path = cubin_cache_path(cache_dir, src, opts)
            var cerr : string
            let image <- cubin_cached(src, "axpy.cu", opts, cerr, cache_dir)
            t |> success(!empty(image), "the miss compiles: {cerr}")
            t |> equal(stat(key_path).size, uint64(length(image)), "the miss writes the cubin to its key's file")

            let marker = "not a cubin - the cache arm wrote these bytes over the cached one"
            t |> success(fwrite(key_path, marker), "the marker replaces the cached cubin")
            let served <- cubin_cached(src, "axpy.cu", opts, cerr, cache_dir)
            t |> equal(string(served), marker, "the hit serves the file's bytes, not a recompile")

            t |> success(fwrite(key_path, ""), "the cached cubin truncates to nothing")
            let recompiled <- cubin_cached(src, "axpy.cu", opts, cerr, cache_dir)
            t |> equal(length(recompiled), length(image), "an empty cache file is a miss - the text compiles again: {cerr}")
            t |> equal(stat(key_path).size, uint64(length(image)), "and the miss rewrites the file")

            var opts_a <- cuda_compile_options(cc)
            opts_a |> push("-DCACHE_ARM_VARIANT=1")
            var opts_b <- cuda_compile_options(cc)
            opts_b |> push("-DCACHE_ARM_VARIANT=2")
            let path_a = cubin_cache_path(cache_dir, src, opts_a)
            let path_b = cubin_cache_path(cache_dir, src, opts_b)
            t |> success(path_a != path_b, "two option sets of one length key two files")
            let image_a <- cubin_cached(src, "axpy.cu", opts_a, cerr, cache_dir)
            let image_b <- cubin_cached(src, "axpy.cu", opts_b, cerr, cache_dir)
            t |> success(!empty(image_a) && !empty(image_b), "both option sets compile: {cerr}")
            t |> success(stat(path_a).is_reg && stat(path_b).is_reg, "each option set's cubin lands in its own file")
            t |> equal(cuda_live_object_count(), base, "the cache arms hold no driver object")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test(description = "A syntax error answers the compiler log, a compute_ cubin is refused by name, and a compute_ PTX loads and resolves its entry.")]
def test_cuda_compile_arms(t : T?) {
    t |> run("a syntax error answers the compiler's log, a compute_ cubin is refused by name, a compute_ PTX loads and resolves its entry") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            feint("no CUDA driver or NVRTC on this box; compile arms skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; compile arms skipped\n")
                return
            }
            let base = cuda_live_object_count()
            var log : string
            let opts <- cuda_compile_options(cc)
            let bad <- cuda_compile("this is not C", "bad.cu", opts, CudaImage.cubin, log)
            t |> success(empty(bad), "a syntax error compiles to nothing")
            t |> success(starts_with(log, "nvrtcCompileProgram:") && find(log, "error") >= 0, "and answers the compiler's log: {log}")

            let compute_opts <- ["--gpu-architecture=compute_{cuda_device_compute_capability(cc)}"]
            let no_cubin <- cuda_compile(AXPY_SRC, "axpy.cu", compute_opts, CudaImage.cubin, log)
            t |> success(empty(no_cubin) && starts_with(log, "nvrtcGetCUBIN: no cubin"), "a cubin of a compute_ target is refused by name: {log}")

            let ptx <- cuda_compile(AXPY_SRC, "axpy.cu", compute_opts, CudaImage.ptx, log)
            t |> success(find(string(ptx), ".entry axpy") >= 0, "the PTX arm returns the PTX text: {log}")
            var lerr : string
            var mod = cuda_load_module(ptx, lerr)
            t |> success(mod != null && cuda_get_function(mod, "axpy") != null, "the PTX loads and resolves its entry: {lerr} {cuda_last_error()}")
            cuda_release(mod)
            t |> equal(cuda_live_object_count(), base, "the PTX module came back")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

//! the arm only a box without NVRTC reaches - every lane without the toolkit runs it
[test(description = "cuda_compile refuses by name where NVRTC does not load.")]
def test_cuda_compile_without_nvrtc(t : T?) {
    t |> run("cuda_compile refuses by name where NVRTC does not load") <| @(t : T?) {
        if (cuda_nvrtc_available()) {
            feint("NVRTC loads on this box; the no-NVRTC refusal is unreachable\n")
            return
        }
        var log : string
        let no_opts : array<string>
        let image <- cuda_compile(AXPY_SRC, "axpy.cu", no_opts, CudaImage.cubin, log)
        t |> success(empty(image) && starts_with(log, "cuda_compile: no NVRTC library loaded"), "the refusal names NVRTC: {log}")
    }
}

[test(description = "Module load, function_from_source, mem_alloc and launch refusals answer their text, release what they made and leave the counters balanced.")]
def test_cuda_module_function_arms(t : T?) {
    t |> run("module load, function_from_source, mem_alloc and launch refusals answer their text, release what they made and leave the counters") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            feint("no CUDA driver or NVRTC on this box; refusal arms skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; refusal arms skipped\n")
                return
            }
            let base = cuda_live_object_count()
            let bytes_base = cuda_device_bytes_allocated()
            var lerr : string
            var no_image : array<uint8>
            t |> success(cuda_load_module(no_image, lerr) == null && lerr == "cuda_load_module: empty image", "an empty image is refused by name: {lerr}")
            let garbage <- [for (i in range(64)); uint8(i)]
            t |> success(cuda_load_module(garbage, lerr) == null && starts_with(lerr, "cuModuleLoadDataEx"), "bytes the driver cannot load answer its text: {lerr}")

            var mod : CudaModule?
            var ferr : string
            let cache_dir = cubin_dir()
            var fn = function_from_source(cc, "this is not C", "bad", "axpy", mod, ferr, cache_dir = cache_dir)
            t |> success(fn == null && mod == null && starts_with(ferr, "nvrtcCompileProgram:"), "text that does not compile answers its log: {ferr}")
            fn = function_from_source(cc, AXPY_SRC, "axpy", "no_such_kernel", mod, ferr, cache_dir = cache_dir)
            t |> success(fn == null && mod == null && starts_with(ferr, "entry `no_such_kernel` missing"), "a missing entry is refused by name: {ferr}")
            t |> equal(cuda_live_object_count(), base, "the missing entry's module was released")
            fn = function_from_source(cc, AXPY_SRC, "axpy", "axpy", mod, ferr, 1u << 22u, cache_dir = cache_dir)
            t |> success(fn == null && mod == null && find(ferr, "bytes of dynamic shared memory refused") >= 0, "an opt-in past the device's limit is refused: {ferr}")
            t |> equal(cuda_live_object_count(), base, "the refused opt-in's module was released")

            fn = function_from_source(cc, AXPY_SRC, "axpy", "axpy", mod, ferr, cache_dir = cache_dir)
            t |> success(fn != null && mod != null, "the gate kernel resolves through function_from_source: {ferr}")
            var params = AxpyParams(n = 0u)
            var rerr : string
            t |> success(!run_compute_1d(fn, params, 1u, 1u, rerr, 1u << 22u), "run_compute_1d refuses shared memory past the opt-in limit")
            t |> success(starts_with(rerr, "cuFuncSetAttribute"), "before any launch, with the opt-in's text: {rerr}")
            let zero_grid = panic_text() $ {
                cuda_launch(fn, uint3(0u, 1u, 1u), uint3(1u, 1u, 1u), 0u, null, null, 0ul)
            }
            t |> success(find(zero_grid, "cuda_launch: zero launch dimension") >= 0, "a zero grid panics by name: {zero_grid}")

            t |> equal(cuda_mem_alloc(1ul << 60ul), 0ul, "an allocation the device cannot hold answers 0")
            t |> success(starts_with(cuda_last_error(), "cuMemAlloc"), "with the driver's text: {cuda_last_error()}")
            t |> equal(cuda_device_bytes_allocated(), bytes_base, "and counts no bytes")

            cuda_release(mod)
            t |> equal(cuda_live_object_count(), base, "every module of the refusal arms came back")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

[test(description = "A capture the legacy stream cannot begin never runs its block, and one invalidated by a context sync fails at its end and leaves the stream capturable.")]
def test_cuda_graph_capture_arms(t : T?) {
    t |> run("a capture the legacy stream cannot begin never runs the block; one a context sync invalidates fails at its end and leaves the stream capturable") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            feint("no CUDA driver or NVRTC on this box; capture arms skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; capture arms skipped\n")
                return
            }
            let base = cuda_live_object_count()
            var ran = false
            let unbegun = cuda_graph_capture(null) {
                ran = true
            }
            t |> success(unbegun == null && !ran, "the legacy stream does not capture, and the block never runs")
            t |> success(starts_with(cuda_last_error(), "cuStreamBeginCapture"), "the begin's text: {cuda_last_error()}")

            let stream = cuda_stream_create()
            var sync_refused = false
            let invalidated = cuda_graph_capture(stream) {
                sync_refused = !cuda_context_synchronize()
            }
            t |> success(sync_refused, "a context sync inside the capture is refused")
            t |> success(invalidated == null && starts_with(cuda_last_error(), "cuStreamEndCapture"), "the capture fails at its end: {cuda_last_error()}")
            let empty_graph = cuda_graph_capture(stream) {
                pass
            }
            t |> success(empty_graph != null, "the failed capture ended - the stream captures again: {cuda_last_error()}")
            cuda_graph_destroy(empty_graph)
            cuda_stream_destroy(stream)
            t |> equal(cuda_live_object_count(), base, "the stream and the graph came back")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

//! what a worker thread's context saw of the device, carried back without a string
struct WorkerBind {
    before : uint64
    bound : bool
    after : uint64
}

//! the fences and the thread bind a GPU tier rides: an untimed event recorded behind a long memset on one stream, queried without
//! a wait and waited on by a second stream; the context bound again on its creating thread, refused on null, and bound on a
//! worker thread whose device calls answer only after the bind
[test(description = "Untimed events fence two streams, the ready query never blocks, and a worker thread bound to the context can use the device.")]
def test_cuda_fences_and_thread_bind(t : T?) {
    t |> run("untimed events fence two streams, the ready query never blocks, a worker thread binds the context") <| @(t : T?) {
        if (!cuda_available()) {
            feint("no CUDA driver on this box; fence checks skipped\n")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                feint("no CUDA device on this box; fence checks skipped\n")
                return
            }
            let base = cuda_live_object_count()
            t |> equal(cuda_bind_context(null), false, "a null context binds nothing")
            t |> success(cuda_bind_context(cc), "the creating thread binds its own context again: {cuda_last_error()}")
            let s0 = cuda_stream_create()
            let s1 = cuda_stream_create()
            let fence = cuda_event_create_untimed()
            t |> success(s0 != null && s1 != null && fence != null, "two streams and an untimed event: {cuda_last_error()}")
            t |> success(cuda_event_ready(fence), "a never-recorded event reads ready")
            let bytes = 256ul << 20ul
            let d = cuda_mem_alloc(bytes)
            t |> success(d != 0ul, "a 256 MB block to memset: {cuda_last_error()}")
            var queued = true
            for (i in range(8)) {   // two gigabytes of memsets: the fence behind them cannot have landed by the time the host asks
                queued = queued && cuda_memset_d8(d, uint8(i), bytes, s0)
            }
            t |> success(queued && cuda_event_record(fence, s0), "eight memsets and the fence behind them queue on the first stream: {cuda_last_error()}")
            let early = cuda_event_ready(fence)
            t |> success(empty(cuda_last_error()), "the ready query answers {early} with no error while the memsets may still run")
            if (early) {
                feint("the eight memsets landed before the host asked - the not-ready arm is out of reach on this box\n")   // the driver makes no promise the queue is still busy; a fast card lands two gigabytes in under a millisecond
            } else {
                t |> success(!early, "the fence behind two gigabytes of memsets reads not-ready at once: the query's not-ready arm")
            }
            t |> success(cuda_stream_wait_event(s1, fence), "the second stream waits on the fence: {cuda_last_error()}")
            t |> success(cuda_stream_synchronize(s1), "the second stream drains: {cuda_last_error()}")
            t |> success(cuda_event_ready(fence), "... and the fence it waited on reads ready")
            t |> equal(cuda_event_elapsed_ms(fence, fence), -1.0, "an untimed event refuses an elapsed time")
            var saw = WorkerBind()
            with_channel(1) $(ch) {
                new_thread() <| @capture(= ch, = cc) {
                    let avail = cuda_available()
                    let before = avail ? cuda_device_free_memory() : 0ul
                    let bound = avail && cuda_bind_context(cc)
                    let after = bound ? cuda_device_free_memory() : 0ul
                    ch |> push_clone(WorkerBind(before = before, bound = bound, after = after))
                    ch |> notify_and_release
                }
                ch |> join()
                ch |> for_each_clone() $(r : WorkerBind#) {
                    saw = WorkerBind(before = r.before, bound = r.bound, after = r.after)
                }
            }
            t |> equal(saw.before, 0ul, "a worker thread's device call before any bind answers nothing")
            t |> success(saw.bound && saw.after > 0ul, "bound to the creating thread's context, the worker reads the device's free memory ({saw.after} bytes)")
            cuda_mem_free(d)
            cuda_event_destroy(fence)
            cuda_stream_destroy(s0)
            cuda_stream_destroy(s1)
            t |> equal(cuda_live_object_count(), base, "every stream, event and buffer of the gate came back")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

//! the panics that fire before any driver call, so every lane runs them
[test(description = "Freeing an unknown pointer and launching a null function each panic with a named message.")]
def test_cuda_boost_panics(t : T?) {
    t |> run("freeing an unknown pointer and launching a null function panic by name") <| @(t : T?) {
        let unknown_free = panic_text() $ {
            cuda_mem_free(0x1234ul)
        }
        t |> success(find(unknown_free, "is not a live cuda_mem_alloc result") >= 0, "an unknown device pointer panics: {unknown_free}")
        let null_fn = panic_text() $ {
            cuda_launch(null, uint3(1u, 1u, 1u), uint3(1u, 1u, 1u), 0u, null, null, 0ul)
        }
        t |> success(find(null_fn, "cuda_launch: null function") >= 0, "a null function panics: {null_fn}")
        t |> equal(cuda_device_bytes_allocated(), 0l, "and neither panic moved the byte counter")
    }
}

let STUB_NVRTC = 12080   //! the NVRTC version the stand-in prewarms key under
let STUB_DRIVER = 12090   //! the driver version they key under

//! the prewarm's stand-in compiler: a text holding `STUB_FAIL` answers no image and a log, any other
//! text the bytes `stub:<name>:<option count>`
def private stub_compile(source, name : string; compile_opts : array<string>; var log : string&) : array<uint8> {
    if (find(source, "STUB_FAIL") >= 0) {
        log = "stub: {name} refused"
        return <- array<uint8>()
    }
    return <- [for (b in "stub:{name}:{length(compile_opts)}"); uint8(b)]
}

def private stub_texts(stamp : string) : array<CudaPrewarmText> {
    return <- [
        CudaPrewarmText(name = "k_a", source = "{stamp} a", compile_opts <- ["--x"]),
        CudaPrewarmText(name = "k_b", source = "{stamp} b", compile_opts <- ["--x"]),
        CudaPrewarmText(name = "k_fail", source = "{stamp} STUB_FAIL", compile_opts <- ["--x"]),
        CudaPrewarmText(name = "k_a_again", source = "{stamp} a", compile_opts <- ["--x"])
    ]
}

def private stub_path(cache_dir : string; text : CudaPrewarmText; nvrtc : int = STUB_NVRTC; driver : int = STUB_DRIVER) : string =>
    cubin_cache_path(cache_dir, text.source, text.compile_opts, nvrtc, driver)

def private fresh_cache_dir(tag : string) : string => path_join(cubin_dir(), "{tag}_{ref_time_ticks()}")

[test(description = "A prewarm compiles only the texts with no cubin, reports a failure by name while the others land, and a second prewarm compiles none.")]
def test_cuda_cubin_prewarm(t : T?) {
    t |> run("the stub compiler's prewarm: misses compile once, a failure is named, the cubins are the compile's bytes") <| @(t : T?) {
        let cache_dir = fresh_cache_dir("prewarm")
        let texts <- stub_texts("// prewarm {ref_time_ticks()}")
        let first = cubin_prewarm(texts, cache_dir, STUB_NVRTC, STUB_DRIVER, @@stub_compile, 2)
        t |> equal(first.compiled, 2, "the two distinct good texts compile; the repeated text's key compiles once")
        t |> equal(first.cached, 0, "a fresh cache holds nothing")
        t |> equal(first.failed, 1, "the failing text is counted")
        t |> equal(first.workers, 2, "the worker cap holds")
        t |> equal(length(first.failures), 1, "and named")
        if (length(first.failures) == 1) {
            t |> equal(first.failures[0].name, "k_fail", "the failure carries the text's name")
            t |> equal(first.failures[0].log, "stub: k_fail refused", "and the compile's log")
        }
        t |> equal(fread(stub_path(cache_dir, texts[0])), "stub:k_a:1", "the cubin on disk is the compile's bytes, at the text's path")
        t |> success(!stat(stub_path(cache_dir, texts[2])).is_valid, "a failed text leaves no file")

        let second = cubin_prewarm(texts, cache_dir, STUB_NVRTC, STUB_DRIVER, @@stub_compile, 2)
        t |> equal(second.compiled, 0, "a second prewarm compiles nothing that landed")
        t |> equal(second.cached, 2, "it finds both cubins")
        t |> equal(second.failed, 1, "and the failing text fails again")

        let other_driver = cubin_prewarm(texts, cache_dir, STUB_NVRTC, STUB_DRIVER + 1, @@stub_compile, 2)
        t |> equal(other_driver.compiled, 2, "another driver version keys other files")
        let other_nvrtc = cubin_prewarm(texts, cache_dir, STUB_NVRTC + 1, STUB_DRIVER, @@stub_compile, 2)
        t |> equal(other_nvrtc.compiled, 2, "another NVRTC version keys other files")

        let none : array<CudaPrewarmText>
        let empty_run = cubin_prewarm(none, cache_dir, STUB_NVRTC, STUB_DRIVER, @@stub_compile)
        t |> equal(empty_run.compiled + empty_run.cached + empty_run.failed + empty_run.workers, 0,
            "no text: no job, nothing counted")
        let rung_opts <- cuda_compile_options(120, "a")
        t |> equal(rung_opts[0], "--gpu-architecture=sm_120a", "a card's options name its arch, the suffix after")
        let text_opts <- cuda_text_compile_options(120, "// __DAS_ARCH_A__ text")
        t |> equal(text_opts[0], "--gpu-architecture=sm_120a", "an arch-specific text's options take the suffix")
        let plain_opts <- cuda_text_compile_options(120, "// plain text")
        t |> equal(plain_opts[0], "--gpu-architecture=sm_120", "a plain text's options take none")
    }
}

[test(description = "One missing text under the default cap runs one job; many run one a live worker of the queue, at most CUDA_PREWARM_MAX_WORKERS.")]
def test_cuda_cubin_prewarm_workers(t : T?) {
    t |> run("the worker count: the missing texts, the live workers and the cap, the least of them") <| @(t : T?) {
        let stamp = "// workers {ref_time_ticks()}"
        let one <- [CudaPrewarmText(name = "only", source = "{stamp} only", compile_opts <- ["--x"])]
        let single = cubin_prewarm(one, fresh_cache_dir("prewarm_one"), STUB_NVRTC, STUB_DRIVER, @@stub_compile)
        t |> equal(single.workers, 1, "one missing text: one job")
        let many <- [for (i in range(100)); CudaPrewarmText(name = "k{i}", source = "{stamp} {i}", compile_opts <- ["--x"])]
        with_job_que() {
            let live = min(get_total_hw_jobs(), get_jobque_worker_limit())
            t |> equal(CUDA_PREWARM_MAX_WORKERS, 64, "the cap is 64 jobs")
            let expected = clamp(min(live, 64), 1, 100)
            let report = cubin_prewarm(many, fresh_cache_dir("prewarm_many"), STUB_NVRTC, STUB_DRIVER, @@stub_compile)
            t |> equal(report.workers, expected, "a job a live worker, at most the cap ({live} live)")
            t |> equal(report.compiled, 100, "every text compiles")
        }
    }
}

[test(description = "A prewarm whose cache folder is a regular file reports each compiled text failed with the write named in its log.")]
def test_cuda_cubin_prewarm_store_failure(t : T?) {
    t |> run("a cubin that does not write is a failure, named") <| @(t : T?) {
        let blocker = fresh_cache_dir("prewarm_blocker")
        mkdir_rec(cubin_dir())
        t |> success(fwrite(blocker, "a file where the cache folder goes"), "the cache folder's path is a regular file")
        let texts <- [CudaPrewarmText(name = "k_store", source = "// store {ref_time_ticks()}", compile_opts <- ["--x"])]
        let report = cubin_prewarm(texts, blocker, STUB_NVRTC, STUB_DRIVER, @@stub_compile)
        t |> equal(report.compiled, 0, "nothing lands")
        t |> equal(report.failed, 1, "the text that did not write is a failure")
        if (length(report.failures) == 1) {
            t |> success(find(report.failures[0].log, "did not write") >= 0, "its log names the write: {report.failures[0].log}")
        }
    }
}

[test(description = "A prewarm through NVRTC writes the cubin cubin_cached reads, compiles again over an empty file, and the cubin loads.")]
def test_cuda_cubin_prewarm_nvrtc(t : T?) {
    t |> run("the real compile's prewarm feeds the serial path") <| @(t : T?) {
        if (!cuda_available() || !cuda_nvrtc_available()) {
            t |> skip("no CUDA driver or NVRTC on this box; the NVRTC prewarm skipped")
            return
        }
        with_cuda_context() $(cc : CudaContext?) {
            if (cc == null) {
                t |> skip("no CUDA device on this box; the NVRTC prewarm skipped")
                return
            }
            let base = cuda_live_object_count()
            let src = "// prewarm arm {ref_time_ticks()}\n{AXPY_SRC}"
            let texts <- [CudaPrewarmText(name = "axpy", source = src, compile_opts <- cuda_compile_options(cc))]
            let cache_dir = cubin_dir()
            let report = cubin_prewarm(texts, cache_dir, cuda_nvrtc_version(), cuda_driver_version(), @@cuda_compile_cubin_unchecked)
            t |> equal(report.compiled, 1, "the text compiles on a worker: {report.failures}")
            let key_path = cubin_cache_path(cache_dir, src, texts[0].compile_opts)
            t |> success(stat(key_path).is_reg, "at the path cubin_cached reads")
            let marker = "the prewarm's file, served"
            t |> success(fwrite(key_path, marker), "the marker replaces the prewarmed cubin")
            var cerr : string
            let served <- cubin_cached(src, "axpy.cu", texts[0].compile_opts, cerr, cache_dir)
            t |> equal(string(served), marker, "cubin_cached serves the prewarm's file, compiling nothing")
            t |> success(fwrite(key_path, ""), "the file truncates, so the next prewarm compiles")
            let again = cubin_prewarm(texts, cache_dir, cuda_nvrtc_version(), cuda_driver_version(), @@cuda_compile_cubin_unchecked)
            t |> equal(again.compiled, 1, "an empty file is a miss")
            var image <- cubin_cached(src, "axpy.cu", texts[0].compile_opts, cerr, cache_dir)
            var lerr : string
            var mod = cuda_load_module(image, lerr)
            t |> success(mod != null, "the prewarmed cubin loads: {lerr}")
            cuda_release(mod)
            t |> equal(cuda_live_object_count(), base, "the prewarm holds no driver object")
        }
        t |> equal(cuda_log_leaks_if_any(), 0l)
    }
}

let INIT_MARK_ENV = "CUDA_PLUMBING_INIT_MARK"   //! a cell's mark file: each context that runs this file's [init] while it is set appends a byte

//! the mark: a byte appended to the file the cell names, outside every context's globals - a skip-init fork's globals
//! are uninitialized memory, so only a mark no global holds tells whether its [init] ran
[init]
def private mark_init() {
    let path = get_env_variable(INIT_MARK_ENV)
    return if (empty(path))
    fopen(path, "ab") $(f) {
        if (f != null) {
            fwrite(f, "i")
        }
    }
}

//! the contexts that ran this file's [init] since the cell's mark file was named
def private init_marks(path : string) : int {
    let st = stat(path)
    return st.is_reg ? int(st.size) : 0
}

//! `blk` with the mark file named for every context made meanwhile; unnamed after
def private with_init_mark(tag : string; blk : block<(path : string) : void>) {
    let path = path_join(fresh_cache_dir(tag), "init_marks")
    mkdir_rec(dir_name(path))
    set_env_variable(INIT_MARK_ENV, path)
    invoke(blk, path)
    set_env_variable(INIT_MARK_ENV, "")
}

//! one probe text prewarmed into a fresh folder: the bytes the stand-in left there
def private probe_prewarm(tag : string) : string => probe_prewarm_in(fresh_cache_dir(tag), tag)

def private probe_prewarm_in(cache_dir, tag : string) : string {
    let texts <- [CudaPrewarmText(name = "probe", source = "// {tag} {ref_time_ticks()}", compile_opts <- ["--x"])]
    cubin_prewarm(texts, cache_dir, STUB_NVRTC, STUB_DRIVER, @@stub_compile)
    return fread(stub_path(cache_dir, texts[0]))
}

//! one `new_job` of this context, after the prewarm
def private later_job() {
    with_wait_group(1) $(wg) {
        new_job() @() {
            wg |> notify_and_release
        }
    }
}

[test(description = "A prewarm on the caller's queue puts both fork-pool flags back from each start, runs outside a skip-init pool on forks that ran [init], and a later job of a caller with skip-init off runs on a fork that ran [init].")]
def test_cuda_cubin_prewarm_fork_pool(t : T?) {
    t |> run("the fork-pool flags come back as found, and no skip-init fork serves the caller's later jobs") <| @(t : T?) {
        with_init_mark("prewarm_marks_pool") $(marks) {
            with_job_que() {
                for (start in fixed_array((keep = false, skip = false), (keep = true, skip = false), (keep = true, skip = true))) {
                    set_jobque_fork_pool(start.keep, start.skip)
                    let before = init_marks(marks)
                    let probe = probe_prewarm("prewarm_queue")
                    t |> success(!empty(probe), "the prewarm ran its job on the caller's queue (from {start})")
                    if (!(start.keep && start.skip)) {
                        t |> success(init_marks(marks) > before, "outside a skip-init pool each job runs on a fresh fork that ran [init] (from {start})")
                    }
                    t |> equal(get_jobque_fork_pool(), start.keep, "the pool flag comes back (from {start})")
                    t |> equal(get_jobque_fork_skip_init(), start.skip, "the skip-init flag comes back (from {start})")
                    if (!start.skip) {
                        let ahead = init_marks(marks)
                        later_job()
                        t |> success(init_marks(marks) > ahead, "a later job runs on a fork that ran [init] (from {start})")
                    }
                }
                set_jobque_fork_pool(false, false)
            }
        }
    }
}

//! a record crossing to the prewarm's thread and back: a folder, then the marks its prewarm left
struct ProbeText {
    text : string
    marks : int
}

//! the probe prewarmed from a context of its own on another thread, whose fork pool is empty and pooled skip-init:
//! the marks its prewarm's forks left, counted on that thread around the prewarm alone
def private marks_from_skip_init_pool(cache_dir, marks : string) : int {
    var left = -1
    with_stream() $(var dirs : Stream?) {
        with_stream() $(var answers : Stream?) {
            dirs |> push_archive(ProbeText(text = cache_dir))
            dirs |> push_archive(ProbeText(text = marks))
            with_wait_group(1) $(wg) {
                new_thread() @() {
                    set_jobque_fork_pool(true, true)
                    var dir, mark : string
                    dirs |> try_pop_archive() $(var d : ProbeText&) {
                        dir = clone_string(d.text)
                    }
                    dirs |> try_pop_archive() $(var m : ProbeText&) {
                        mark = clone_string(m.text)
                    }
                    let before = init_marks(mark)
                    let probe = probe_prewarm_in(dir, "prewarm_init")
                    let grew = empty(probe) ? -1 : init_marks(mark) - before
                    answers |> push_archive(ProbeText(marks = grew))
                    dirs |> release
                    answers |> release
                    wg |> notify_and_release
                }
            }
            answers |> try_pop_archive() $(var answer : ProbeText&) {
                left = answer.marks
            }
        }
    }
    return left
}

[test(description = "A prewarm from a caller that pools skip-init forks runs its compile step on forks no [init] ran in, by a mark the [init] leaves outside every context's globals; the same prewarm from a caller that does not pool skip-init leaves marks.")]
def test_cuda_cubin_prewarm_skip_init(t : T?) {
    t |> run("the compile step's contexts ran no [init]") <| @(t : T?) {
        with_init_mark("prewarm_marks_skip") $(marks) {
            let before = init_marks(marks)
            let probe = probe_prewarm("prewarm_marked")
            t |> success(!empty(probe), "the control's prewarm ran its job")
            t |> success(init_marks(marks) > before, "the control: a caller that does not pool skip-init runs its jobs on forks that ran [init]")
            let left = marks_from_skip_init_pool(fresh_cache_dir("prewarm_init"), marks)
            t |> equal(left, 0, "the job's fork, out of the caller's skip-init pool, skipped the init script")
        }
    }
}
