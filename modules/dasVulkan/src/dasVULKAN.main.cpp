#include "dasVULKAN.h"
#include <cstdio>
#include <vector>

#if defined(_WIN32)
// volk forward-declares the Win32 handle TYPES (HWND/HINSTANCE) but does not pull
// in <windows.h>, so GetModuleHandle is unavailable here. __ImageBase is the
// linker-provided base address of this module == its HINSTANCE; use it directly.
extern "C" char __ImageBase;
#endif

#if defined(__APPLE__)
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#endif

namespace das {

#if defined(__APPLE__)
// defined in dasVULKAN.metal.mm — attaches a CAMetalLayer to the NSWindow.
void * das_metal_layer_for_nswindow(void * ns_window);
#endif

// volk lifecycle — bound by hand; everything else dispatches through volk's
// loaded pointers and is generated.
#if defined(__APPLE__)
// volkInitialize() on macOS dlopen's only leaf names (resolved against the default
// dyld search, which excludes Homebrew) plus a hardcoded /usr/local/lib — so it
// misses the Apple-Silicon Homebrew prefix (/opt/homebrew/lib) and the LunarG SDK.
// Find the Vulkan loader at the known install locations and wire volk to it via
// volkInitializeCustom, so a stock `brew install vulkan-loader molten-vk` (or a
// VULKAN_SDK env) works with no DYLD_* override. The loader (not MoltenVK direct)
// is required: VK_KHR_portability_enumeration is a loader extension.
static int32_t das_volkInitialize() {
    if (volkInitialize() == VK_SUCCESS) {
        return int32_t(VK_SUCCESS);
    }
    char sdkPath[1024];
    const char * candidates[4];
    int n = 0;
    const char * sdk = getenv("VULKAN_SDK");
    if (sdk && snprintf(sdkPath, sizeof(sdkPath), "%s/lib/libvulkan.dylib", sdk) > 0) {
        candidates[n++] = sdkPath;
    }
    candidates[n++] = "/opt/homebrew/lib/libvulkan.dylib";
    candidates[n++] = "/usr/local/lib/libvulkan.dylib";
    candidates[n++] = "/opt/homebrew/lib/libvulkan.1.dylib";
    for (int i = 0; i < n; ++i) {
        void * module = dlopen(candidates[i], RTLD_NOW | RTLD_LOCAL);
        if (!module) {
            continue;
        }
        PFN_vkGetInstanceProcAddr gipa = (PFN_vkGetInstanceProcAddr) dlsym(module, "vkGetInstanceProcAddr");
        if (!gipa) {
            dlclose(module);
            continue;
        }
        volkInitializeCustom(gipa);
        return int32_t(VK_SUCCESS);
    }
    return int32_t(VK_ERROR_INITIALIZATION_FAILED);
}
#else
static int32_t das_volkInitialize() {
    return int32_t(volkInitialize());
}
#endif

static uint32_t das_volkGetInstanceVersion() {
    return volkGetInstanceVersion();
}

static void das_volkLoadInstance(VkInstance instance) {
    volkLoadInstance(instance);
}

static void das_volkLoadDevice(VkDevice device) {
    volkLoadDevice(device);
}

static void das_volkFinalize() {
    volkFinalize();
}

// The loader's vkGetInstanceProcAddr, as a raw pointer (null before volkInitialize).
// Used to hand the loader to GLFW (glfwInitVulkanLoader) so GLFW does not have to
// discover the loader itself — the discovery our das_volkInitialize already did on
// macOS then covers GLFW too. Platform-agnostic.
static void * das_vk_get_instance_proc_addr() {
    return (void *) vkGetInstanceProcAddr;
}

// GLFW exposes the native window/display handles but not glfwCreateWindowSurface,
// so bridge them to a VkSurfaceKHR via the platform surface extension here. Takes
// raw void* handles (HWND / X11 Window+Display) -- no GLFW dependency. Returns the
// surface as uint64 (0 on failure); the instance must have been created with the
// matching platform surface extension enabled and volkLoadInstance already called.
static uint64_t das_vk_surface_from_native(VkInstance instance, void * native_window, void * native_display) {
    VkSurfaceKHR surface = VK_NULL_HANDLE;
#if defined(_WIN32)
    VkWin32SurfaceCreateInfoKHR ci{};
    ci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    ci.hinstance = (HINSTANCE) &__ImageBase;
    ci.hwnd = (HWND) native_window;
    if (vkCreateWin32SurfaceKHR(instance, &ci, nullptr, &surface) != VK_SUCCESS) {
        surface = VK_NULL_HANDLE;
    }
#elif defined(__APPLE__)
    // native_window is the NSWindow (glfwGetCocoaWindow); bridge it to a
    // CAMetalLayer in the Objective-C++ TU, then create the MoltenVK surface.
    // The instance must enable VK_EXT_metal_surface (glfwGetRequiredInstanceExtensions
    // returns it on macOS) and volkLoadInstance must already have run.
    (void) native_display;
    if (void * layer = das_metal_layer_for_nswindow(native_window)) {
        VkMetalSurfaceCreateInfoEXT ci{};
        ci.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
        ci.pLayer = (const CAMetalLayer *) layer;
        if (vkCreateMetalSurfaceEXT(instance, &ci, nullptr, &surface) != VK_SUCCESS) {
            surface = VK_NULL_HANDLE;
        }
    }
#elif defined(__linux__)
    VkXlibSurfaceCreateInfoKHR ci{};
    ci.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    ci.dpy = (Display *) native_display;
    ci.window = (Window) (uintptr_t) native_window;
    if (vkCreateXlibSurfaceKHR(instance, &ci, nullptr, &surface) != VK_SUCCESS) {
        surface = VK_NULL_HANDLE;
    }
#else
    (void) instance; (void) native_window; (void) native_display;
#endif
    return (uint64_t) surface;
}

// A default debug-utils messenger callback: prints the diagnostic and returns
// VK_FALSE (do not abort the triggering call). Exposed as a raw void* so the das
// side can install it as VkDebugUtilsMessengerCreateInfoEXT.pfnUserCallback --
// the interpreter cannot hand a das function to Vulkan as a C callback.
static VKAPI_ATTR VkBool32 VKAPI_CALL das_vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT types,
    const VkDebugUtilsMessengerCallbackDataEXT * data, void * user) {
    (void) severity; (void) types; (void) user;
    fprintf(stderr, "[vulkan] %s\n",
        data && data->pMessage ? data->pMessage : "");
    fflush(stderr);
    return VK_FALSE;
}

static void * das_vk_debug_callback_ptr() {
    return (void *) das_vk_debug_callback;
}

enum OsVideoMemoryQuery : int32_t {
    OSVM_BUDGET = 0,
    OSVM_USAGE = 1,
    OSVM_ADAPTER_DEDICATED = 2,
    OSVM_ADAPTER_SHARED = 3,
};

#if defined(_WIN32)
extern "C" __declspec(dllimport) void * __stdcall LoadLibraryA(const char * name);
extern "C" __declspec(dllimport) void * __stdcall GetProcAddress(void * module, const char * name);
namespace {
    struct PdhFmtCounterValue { uint32_t CStatus; uint32_t pad; int64_t largeValue; };
    struct PdhFmtCounterValueItemA { char * szName; PdhFmtCounterValue FmtValue; };
    typedef int32_t (__stdcall * PfnPdhOpenQueryA)(const char *, uintptr_t, void **);
    typedef int32_t (__stdcall * PfnPdhAddEnglishCounterA)(void *, const char *, uintptr_t, void **);
    typedef int32_t (__stdcall * PfnPdhCollectQueryData)(void *);
    typedef int32_t (__stdcall * PfnPdhGetFormattedCounterArrayA)(void *, uint32_t, uint32_t *, uint32_t *, PdhFmtCounterValueItemA *);
    typedef int32_t (__stdcall * PfnPdhCloseQuery)(void *);
    const uint32_t PDH_FMT_LARGE_ = 0x00000400u;
    const int32_t PDH_MORE_DATA_ = (int32_t) 0x800007D2;

    uint64_t pdh_adapter_usage(uint32_t luid_lo, uint32_t luid_hi, const char * counter_path) {
        static PfnPdhOpenQueryA open_q = nullptr;
        static PfnPdhAddEnglishCounterA add_c = nullptr;
        static PfnPdhCollectQueryData collect = nullptr;
        static PfnPdhGetFormattedCounterArrayA get_arr = nullptr;
        static PfnPdhCloseQuery close_q = nullptr;
        static bool resolved = false;
        if (!resolved) {
            resolved = true;
            if (void * pdh = LoadLibraryA("pdh.dll")) {
                open_q = (PfnPdhOpenQueryA) GetProcAddress(pdh, "PdhOpenQueryA");
                add_c = (PfnPdhAddEnglishCounterA) GetProcAddress(pdh, "PdhAddEnglishCounterA");
                collect = (PfnPdhCollectQueryData) GetProcAddress(pdh, "PdhCollectQueryData");
                get_arr = (PfnPdhGetFormattedCounterArrayA) GetProcAddress(pdh, "PdhGetFormattedCounterArrayA");
                close_q = (PfnPdhCloseQuery) GetProcAddress(pdh, "PdhCloseQuery");
            }
        }
        if (!open_q || !add_c || !collect || !get_arr || !close_q) {
            return 0;
        }
        void * query = nullptr;
        if (open_q(nullptr, 0, &query) != 0 || !query) {
            return 0;
        }
        uint64_t total = 0;
        void * counter = nullptr;
        if (add_c(query, counter_path, 0, &counter) == 0 && collect(query) == 0) {
            uint32_t bytes = 0, count = 0;
            if (get_arr(counter, PDH_FMT_LARGE_, &bytes, &count, nullptr) == PDH_MORE_DATA_ && bytes > 0) {
                std::vector<char> buf(bytes);
                if (get_arr(counter, PDH_FMT_LARGE_, &bytes, &count, (PdhFmtCounterValueItemA *) buf.data()) == 0) {
                    char prefix[64];
                    snprintf(prefix, sizeof(prefix), "luid_0x%08x_0x%08x_phys", luid_hi, luid_lo);
                    const PdhFmtCounterValueItemA * items = (const PdhFmtCounterValueItemA *) buf.data();
                    for (uint32_t i = 0; i < count; ++i) {
                        const char * name = items[i].szName;
                        bool match = name != nullptr;
                        for (size_t k = 0; match && prefix[k]; ++k) {
                            const char a = name[k], b = prefix[k];
                            match = a != 0 && ((a >= 'A' && a <= 'Z') ? a + 32 : a) == b;
                        }
                        if (match && items[i].FmtValue.CStatus == 0 && items[i].FmtValue.largeValue > 0) {
                            total += (uint64_t) items[i].FmtValue.largeValue;
                        }
                    }
                }
            }
        }
        close_q(query);
        return total;
    }
    struct D3dkmtLuid { uint32_t LowPart; int32_t HighPart; };
    struct D3dkmtOpenAdapterFromLuid { D3dkmtLuid AdapterLuid; uint32_t hAdapter; };
    struct D3dkmtCloseAdapter { uint32_t hAdapter; };
    const uint32_t D3DKMT_SEGMENT_LOCAL = 0;
    struct D3dkmtQueryVideoMemoryInfo {
        void *   hProcess;                  // NULL = this process
        uint32_t hAdapter;
        uint32_t MemorySegmentGroup;
        uint64_t Budget;
        uint64_t CurrentUsage;
        uint64_t AvailableForReservation;
        uint64_t CurrentReservation;
        uint32_t PhysicalAdapterIndex;
    };
    typedef int32_t (__stdcall * PfnD3dkmtOpen)(D3dkmtOpenAdapterFromLuid *);
    typedef int32_t (__stdcall * PfnD3dkmtQuery)(D3dkmtQueryVideoMemoryInfo *);
    typedef int32_t (__stdcall * PfnD3dkmtClose)(const D3dkmtCloseAdapter *);
}
static uint64_t das_vk_os_video_memory(uint32_t luid_lo, uint32_t luid_hi, int32_t query) {
    static PfnD3dkmtOpen open_fn = nullptr;
    static PfnD3dkmtQuery query_fn = nullptr;
    static PfnD3dkmtClose close_fn = nullptr;
    static bool resolved = false;
    if (!resolved) {
        resolved = true;
        if (void * gdi = LoadLibraryA("gdi32.dll")) {
            open_fn = (PfnD3dkmtOpen) GetProcAddress(gdi, "D3DKMTOpenAdapterFromLuid");
            query_fn = (PfnD3dkmtQuery) GetProcAddress(gdi, "D3DKMTQueryVideoMemoryInfo");
            close_fn = (PfnD3dkmtClose) GetProcAddress(gdi, "D3DKMTCloseAdapter");
        }
    }
    if ((luid_lo == 0 && luid_hi == 0) || query < OSVM_BUDGET || query > OSVM_ADAPTER_SHARED) {
        return 0;
    }
    if (query == OSVM_ADAPTER_DEDICATED) {
        return pdh_adapter_usage(luid_lo, luid_hi, "\\GPU Adapter Memory(*)\\Dedicated Usage");
    }
    if (query == OSVM_ADAPTER_SHARED) {
        return pdh_adapter_usage(luid_lo, luid_hi, "\\GPU Adapter Memory(*)\\Shared Usage");
    }
    if (!open_fn || !query_fn || !close_fn) {
        return 0;
    }
    D3dkmtOpenAdapterFromLuid oa{};
    oa.AdapterLuid.LowPart = luid_lo;
    oa.AdapterLuid.HighPart = (int32_t) luid_hi;
    if (open_fn(&oa) != 0) {
        return 0;
    }
    D3dkmtQueryVideoMemoryInfo q{};
    q.hAdapter = oa.hAdapter;
    q.MemorySegmentGroup = D3DKMT_SEGMENT_LOCAL;
    const int32_t st = query_fn(&q);
    D3dkmtCloseAdapter ca{};
    ca.hAdapter = oa.hAdapter;
    close_fn(&ca);
    if (st != 0) {
        return 0;
    }
    return query == OSVM_BUDGET ? q.Budget : q.CurrentUsage;
}
#else
static uint64_t das_vk_os_video_memory(uint32_t luid_lo, uint32_t luid_hi, int32_t query) {
    (void) luid_lo; (void) luid_hi; (void) query;
    return 0;
}
#endif

Module_dasVULKAN::Module_dasVULKAN() : Module("vulkan") {
    ModuleLibrary lib(this);
    lib.addBuiltInModule();
    das_vulkan_init_generated(*this, lib);
    addExtern<DAS_BIND_FUN(das_volkInitialize)>(*this, lib, "volkInitialize",
        SideEffects::modifyExternal, "das_volkInitialize");
    addExtern<DAS_BIND_FUN(das_volkGetInstanceVersion)>(*this, lib, "volkGetInstanceVersion",
        SideEffects::accessExternal, "das_volkGetInstanceVersion");
    addExtern<DAS_BIND_FUN(das_volkLoadInstance)>(*this, lib, "volkLoadInstance",
        SideEffects::modifyExternal, "das_volkLoadInstance")->args({"instance"});
    addExtern<DAS_BIND_FUN(das_volkLoadDevice)>(*this, lib, "volkLoadDevice",
        SideEffects::modifyExternal, "das_volkLoadDevice")->args({"device"});
    addExtern<DAS_BIND_FUN(das_volkFinalize)>(*this, lib, "volkFinalize",
        SideEffects::modifyExternal, "das_volkFinalize");
    addExtern<DAS_BIND_FUN(das_vk_get_instance_proc_addr)>(*this, lib, "vk_get_instance_proc_addr",
        SideEffects::accessExternal, "das_vk_get_instance_proc_addr");
    addExtern<DAS_BIND_FUN(das_vk_surface_from_native)>(*this, lib, "vk_surface_from_native",
        SideEffects::modifyExternal, "das_vk_surface_from_native")->args({"instance", "native_window", "native_display"});
    addExtern<DAS_BIND_FUN(das_vk_debug_callback_ptr)>(*this, lib, "vk_debug_callback",
        SideEffects::accessExternal, "das_vk_debug_callback_ptr");
    addExtern<DAS_BIND_FUN(das_vk_os_video_memory)>(*this, lib, "vk_os_video_memory",
        SideEffects::accessExternal, "das_vk_os_video_memory")->args({"luid_lo", "luid_hi", "query"});
    verifyAotReady();
}

ModuleAotType Module_dasVULKAN::aotRequire(TextWriter & tw) const {
    tw << "#include <volk.h>\n";
    return ModuleAotType::cpp;
}

REGISTER_DYN_MODULE(Module_dasVULKAN, Module_dasVULKAN);

}

REGISTER_MODULE_IN_NAMESPACE(Module_dasVULKAN, das);
