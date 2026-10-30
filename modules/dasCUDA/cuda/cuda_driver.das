options gen2
options indenting = 4
options no_aot   // a late extern is a dasbind proxy the AOT emitter spells by its mangled name, which no C++ compiler accepts; the module serves the interpreter and the JIT, as dasOpenGL's externs do

// The CUDA driver API and NVRTC as late externs over the libraries the driver and the toolkit
// install: no header, no SDK at build time, no C++ module. A das name drops the `_v2` suffix the
// library exports under; a handle is a `void?`, a device address a `uint64`, a result an `int`.
// The library lists name every toolkit major the binding knows, newest first.

module cuda_driver shared public

require dasbind public
require daslib/fio
require daslib/strings_boost

let public CUDA_DRIVER_LIBRARY = "nvcuda"
let public CUDA_DRIVER_LIBRARY_LINUX = "libcuda.so.1;libcuda.so"
let public NVRTC_LIBRARY = "nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0"
let public NVRTC_LIBRARY_LINUX = "libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so"

let public CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT = 16
let public CU_DEVICE_ATTRIBUTE_L2_CACHE_SIZE = 38
let public CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR = 75
let public CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR = 76
let public CU_FUNC_ATTRIBUTE_NUM_REGS = 4
let public CU_FUNC_ATTRIBUTE_MAX_DYNAMIC_SHARED_SIZE_BYTES = 8
let public CU_FUNC_ATTRIBUTE_PREFERRED_SHARED_MEMORY_CARVEOUT = 9   //! the percent of the SM's unified cache the kernel asks as shared memory
let public CU_SHAREDMEM_CARVEOUT_DEFAULT = -1   //! the carveout value that leaves the split to the driver
let public CU_CTX_SCHED_SPIN = 1u   //! the host spins on a sync instead of yielding to the OS scheduler
let public CU_MEMHOSTALLOC_DEVICEMAP = 2u   //! pinned host memory the device addresses in place
let public CU_STREAM_NON_BLOCKING = 1u
let public CU_EVENT_DEFAULT = 0u
let public CU_EVENT_DISABLE_TIMING = 2u   //! an event with no timestamp: the cheapest fence between two streams
let public CUDA_ERROR_NOT_READY = 600       //! cuEventQuery's answer while the event's work is still in flight
let public CU_LAUNCH_PARAM_END = 0ul
let public CU_LAUNCH_PARAM_BUFFER_POINTER = 1ul
let public CU_LAUNCH_PARAM_BUFFER_SIZE = 2ul

[extern(cdecl, late, name="cuInit", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuInit(flags : uint) : int => 0

[extern(cdecl, late, name="cuDriverGetVersion", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDriverGetVersion(var version : int&) : int => 0

[extern(cdecl, late, name="cuDeviceGetCount", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDeviceGetCount(var count : int&) : int => 0

[extern(cdecl, late, name="cuDeviceGet", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDeviceGet(var device : int&; ordinal : int) : int => 0

[extern(cdecl, late, name="cuDeviceGetName", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDeviceGetName(name : void?; len : int; device : int) : int => 0

[extern(cdecl, late, name="cuDeviceGetAttribute", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDeviceGetAttribute(var value : int&; attribute : int; device : int) : int => 0

[extern(cdecl, late, name="cuDeviceTotalMem_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDeviceTotalMem(var bytes : uint64&; device : int) : int => 0

[extern(cdecl, late, name="cuDevicePrimaryCtxRetain", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDevicePrimaryCtxRetain(var ctx : void?&; device : int) : int => 0

[extern(cdecl, late, name="cuDevicePrimaryCtxSetFlags_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDevicePrimaryCtxSetFlags(device : int; flags : uint) : int => 0

[extern(cdecl, late, name="cuDevicePrimaryCtxRelease_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuDevicePrimaryCtxRelease(device : int) : int => 0

[extern(cdecl, late, name="cuCtxSetCurrent", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuCtxSetCurrent(ctx : void?) : int => 0

[extern(cdecl, late, name="cuCtxSynchronize", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuCtxSynchronize() : int => 0

[extern(cdecl, late, name="cuMemGetInfo_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemGetInfo(var free_bytes : uint64&; var total_bytes : uint64&) : int => 0

[extern(cdecl, late, name="cuModuleLoadDataEx", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuModuleLoadDataEx(var module_ : void?&; image : void?; num_options : uint; option_keys : void?; option_values : void?) : int => 0

[extern(cdecl, late, name="cuModuleUnload", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuModuleUnload(module_ : void?) : int => 0

[extern(cdecl, late, name="cuModuleGetFunction", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuModuleGetFunction(var fn : void?&; module_ : void?; name : string) : int => 0

[extern(cdecl, late, name="cuFuncSetAttribute", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuFuncSetAttribute(fn : void?; attribute : int; value : int) : int => 0

[extern(cdecl, late, name="cuFuncGetAttribute", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuFuncGetAttribute(var value : int&; attribute : int; fn : void?) : int => 0

[extern(cdecl, late, name="cuOccupancyMaxActiveBlocksPerMultiprocessor", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuOccupancyMaxActiveBlocksPerMultiprocessor(var blocks : int&; fn : void?; block_size : int; dynamic_smem : uint64) : int => 0

[extern(cdecl, late, name="cuMemAlloc_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemAlloc(var dptr : uint64&; bytes : uint64) : int => 0

[extern(cdecl, late, name="cuMemFree_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemFree(dptr : uint64) : int => 0

[extern(cdecl, late, name="cuMemHostAlloc", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemHostAlloc(var ptr : void?&; bytes : uint64; flags : uint) : int => 0

[extern(cdecl, late, name="cuMemHostGetDevicePointer_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemHostGetDevicePointer(var dptr : uint64&; p : void?; flags : uint) : int => 0

[extern(cdecl, late, name="cuMemFreeHost", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemFreeHost(ptr : void?) : int => 0

[extern(cdecl, late, name="cuMemcpyHtoD_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemcpyHtoD(dst : uint64; src : void?; bytes : uint64) : int => 0

[extern(cdecl, late, name="cuMemcpyDtoH_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemcpyDtoH(dst : void?; src : uint64; bytes : uint64) : int => 0

[extern(cdecl, late, name="cuMemcpyDtoD_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemcpyDtoD(dst : uint64; src : uint64; bytes : uint64) : int => 0

[extern(cdecl, late, name="cuMemcpyHtoDAsync_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemcpyHtoDAsync(dst : uint64; src : void?; bytes : uint64; stream : void?) : int => 0

[extern(cdecl, late, name="cuMemcpyDtoHAsync_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemcpyDtoHAsync(dst : void?; src : uint64; bytes : uint64; stream : void?) : int => 0

[extern(cdecl, late, name="cuMemcpyDtoDAsync_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemcpyDtoDAsync(dst : uint64; src : uint64; bytes : uint64; stream : void?) : int => 0

[extern(cdecl, late, name="cuMemsetD8_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemsetD8(dst : uint64; value : uint8; bytes : uint64) : int => 0

[extern(cdecl, late, name="cuMemsetD8Async", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuMemsetD8Async(dst : uint64; value : uint8; bytes : uint64; stream : void?) : int => 0

[extern(cdecl, late, name="cuStreamCreate", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuStreamCreate(var stream : void?&; flags : uint) : int => 0

[extern(cdecl, late, name="cuStreamDestroy_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuStreamDestroy(stream : void?) : int => 0

[extern(cdecl, late, name="cuStreamSynchronize", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuStreamSynchronize(stream : void?) : int => 0

[extern(cdecl, late, name="cuEventCreate", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuEventCreate(var event : void?&; flags : uint) : int => 0

[extern(cdecl, late, name="cuEventDestroy_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuEventDestroy(event : void?) : int => 0

[extern(cdecl, late, name="cuEventRecord", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuEventRecord(event : void?; stream : void?) : int => 0

[extern(cdecl, late, name="cuEventSynchronize", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuEventSynchronize(event : void?) : int => 0

[extern(cdecl, late, name="cuEventQuery", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuEventQuery(event : void?) : int => 0

[extern(cdecl, late, name="cuStreamWaitEvent", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuStreamWaitEvent(stream : void?; event : void?; flags : uint) : int => 0

[extern(cdecl, late, name="cuEventElapsedTime_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuEventElapsedTime(var ms : float&; start : void?; end : void?) : int => 0

let public CU_STREAM_CAPTURE_MODE_THREAD_LOCAL = 1u

[extern(cdecl, late, name="cuStreamBeginCapture_v2", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuStreamBeginCapture(stream : void?; mode : uint) : int => 0

[extern(cdecl, late, name="cuStreamEndCapture", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuStreamEndCapture(stream : void?; var graph : void?&) : int => 0

[extern(cdecl, late, name="cuGraphInstantiateWithFlags", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuGraphInstantiateWithFlags(var exec : void?&; graph : void?; flags : uint64) : int => 0

[extern(cdecl, late, name="cuGraphLaunch", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuGraphLaunch(exec : void?; stream : void?) : int => 0

[extern(cdecl, late, name="cuGraphExecDestroy", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuGraphExecDestroy(exec : void?) : int => 0

[extern(cdecl, late, name="cuGraphDestroy", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuGraphDestroy(graph : void?) : int => 0

[extern(cdecl, late, name="cuLaunchKernel", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuLaunchKernel(fn : void?; grid_x : uint; grid_y : uint; grid_z : uint; block_x : uint; block_y : uint; block_z : uint;
                          shared_bytes : uint; stream : void?; kernel_params : void?; extra : void?) : int => 0

//! the extended launch: `config` a CUlaunchConfig (the grid, the block, the shared bytes, the stream and the launch attributes)
[extern(cdecl, late, name="cuLaunchKernelEx", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuLaunchKernelEx(config : void?; fn : void?; kernel_params : void?; extra : void?) : int => 0

[extern(cdecl, late, name="cuGetErrorString", windows_library="nvcuda", linux_library="libcuda.so.1;libcuda.so")]
def public cuGetErrorString(result : int; var text : void?&) : int => 0

[extern(cdecl, late, name="nvrtcVersion", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcVersion(var major : int&; var minor : int&) : int => 0

[extern(cdecl, late, name="nvrtcCreateProgram", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcCreateProgram(var prog : void?&; src : string; name : string; num_headers : int; headers : void?; include_names : void?) : int => 0

[extern(cdecl, late, name="nvrtcCompileProgram", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcCompileProgram(prog : void?; num_options : int; option_strings : void?) : int => 0

[extern(cdecl, late, name="nvrtcGetProgramLogSize", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcGetProgramLogSize(prog : void?; var size : uint64&) : int => 0

[extern(cdecl, late, name="nvrtcGetProgramLog", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcGetProgramLog(prog : void?; log : void?) : int => 0

[extern(cdecl, late, name="nvrtcGetPTXSize", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcGetPTXSize(prog : void?; var size : uint64&) : int => 0

[extern(cdecl, late, name="nvrtcGetPTX", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcGetPTX(prog : void?; ptx : void?) : int => 0

[extern(cdecl, late, name="nvrtcGetCUBINSize", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcGetCUBINSize(prog : void?; var size : uint64&) : int => 0

[extern(cdecl, late, name="nvrtcGetCUBIN", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcGetCUBIN(prog : void?; cubin : void?) : int => 0

[extern(cdecl, late, name="nvrtcDestroyProgram", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcDestroyProgram(var prog : void?&) : int => 0

[extern(cdecl, late, name="nvrtcGetErrorString", windows_library="nvrtc64_140_0;nvrtc64_130_0;nvrtc64_120_0", linux_library="libnvrtc.so.14;libnvrtc.so.13;libnvrtc.so.12;libnvrtc.so")]
def public nvrtcGetErrorString(result : int) : string => ""

//! the driver library resolves on this box: the process can reach a device at all
def public cuda_driver_library_loads() : bool {
    let names = get_running_platform_name() == "windows" ? CUDA_DRIVER_LIBRARY : CUDA_DRIVER_LIBRARY_LINUX
    return __dasbind_get_library(names) != null
}

//! the NVRTC library resolves on this box: kernel text can be compiled here; a box without it still loads cubins
def public cuda_nvrtc_library_loads() : bool {
    let names = get_running_platform_name() == "windows" ? NVRTC_LIBRARY : NVRTC_LIBRARY_LINUX
    let cuda_path = get_env_variable("CUDA_PATH")
    // probe CUDA_PATH first: dasbind caches the bare list's answer, a miss included, and once the full path
    // loaded the module the bare names the late externs bind through resolve to it
    if (!empty(cuda_path)) {
        let sub = get_running_platform_name() == "windows" ? "/bin/x64/" : "/lib64/"
        for (name in split(names, ";")) {
            break if (__dasbind_get_library("{cuda_path}{sub}{name}") != null)
        }
    }
    return __dasbind_get_library(names) != null
}
