#include "daScript/misc/platform.h"
#include "daScript/misc/crash_handler.h"
#include "daScript/simulate/simulate.h"

#include <cstring>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using namespace das;

// Separate function because C++ objects (std::string) cannot coexist with
// __try/__except on MSVC.
static void print_das_stack_walk(void * ctxPtr) {
    auto * ctx = (Context *)ctxPtr;
    string str;
    if (ctx->stack.empty()) {
        str = " (daslang stack empty - reset by recover before C++ overflow)\n";
    } else {
        str = ctx->getStackWalk(nullptr, false, false);
    }
    fprintf(stderr, "%s", str.c_str());
}

static bool das_crash_frame_filter(const char * symbolName) {
    return strstr(symbolName, "SimNode") != nullptr;
}

#if DAS_CRASH_HANDLER_PLATFORM_SUPPORTED

// ---- Platform-specific safe memory reads --------------------------------

#if defined(_MSC_VER)
// MSVC + clang-cl: SEH __try/__except (clang-cl defines _MSC_VER so it lands here).

static uint32_t safe_read_u32(uint64_t addr) {
    uint32_t result = 0;
    __try {
        result = *(uint32_t *)addr;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        result = 0;
    }
    return result;
}

static void * safe_read_ptr(uint64_t addr) {
    void * result = nullptr;
    __try {
        result = *(void **)addr;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        result = nullptr;
    }
    return result;
}

#elif defined(_WIN32)
// clang-mingw / gcc-mingw: SEH __try/__except is not enabled by default.
// VirtualQuery probes the page-protection state cheaply and never faults,
// so it gives us the same safety guarantee without needing SEH.

// Check a single MEMORY_BASIC_INFORMATION region for readability. Rejects
// PAGE_GUARD / PAGE_NOACCESS / PAGE_TARGETS_INVALID — accessing those raises
// EXCEPTION_GUARD_PAGE / EXCEPTION_ACCESS_VIOLATION which would defeat the
// whole point of the safe-read fallback. The protection-flag mask is
// 0xFFFF (not 0xFF) because PAGE_GUARD / PAGE_NOCACHE / PAGE_WRITECOMBINE
// live in the top byte of the DWORD and a too-narrow mask would silently
// strip them.
static bool is_region_readable(const MEMORY_BASIC_INFORMATION & mbi) {
    if (mbi.State != MEM_COMMIT) return false;
    if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) return false;
    const DWORD prot = mbi.Protect & 0xFF;
    return prot == PAGE_READONLY
        || prot == PAGE_READWRITE
        || prot == PAGE_EXECUTE_READ
        || prot == PAGE_EXECUTE_READWRITE
        || prot == PAGE_WRITECOPY
        || prot == PAGE_EXECUTE_WRITECOPY;
}

// Validate that [addr, addr+size) is fully covered by readable committed
// memory. A 4- or 8-byte read can straddle a page boundary (rare but real,
// e.g. unaligned reads near end-of-region), so when the first VirtualQuery's
// region ends inside [addr, addr+size) we requery for the second region and
// require it to also be readable. Without the second query a read could
// reach into the next page which may be unmapped / PAGE_GUARD / PAGE_NOACCESS
// and fault — exactly what this function exists to prevent.
static bool is_addr_readable(uint64_t addr, size_t size) {
    if (size == 0) return true;
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((LPCVOID)(uintptr_t)addr, &mbi, sizeof(mbi)) == 0) return false;
    if (!is_region_readable(mbi)) return false;
    const uintptr_t region_end = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
    const uintptr_t read_end = (uintptr_t)addr + size;
    if (read_end <= region_end) return true;
    // Read spans a region boundary — check the next region too.
    MEMORY_BASIC_INFORMATION mbi2;
    if (VirtualQuery((LPCVOID)region_end, &mbi2, sizeof(mbi2)) == 0) return false;
    if (!is_region_readable(mbi2)) return false;
    return read_end <= (uintptr_t)mbi2.BaseAddress + mbi2.RegionSize;
}

static uint32_t safe_read_u32(uint64_t addr) {
    if (!is_addr_readable(addr, sizeof(uint32_t))) return 0;
    return *(uint32_t *)(uintptr_t)addr;
}

static void * safe_read_ptr(uint64_t addr) {
    if (!is_addr_readable(addr, sizeof(void *))) return nullptr;
    return *(void **)(uintptr_t)addr;
}

#elif defined(__linux__) && !defined(__EMSCRIPTEN__)

#include <sys/mman.h>

// mincore() returns ENOMEM for unmapped ranges, 0 for mapped ones.
static bool is_addr_mapped(uint64_t addr, size_t size) {
    const uintptr_t PAGE = 4096;
    uintptr_t start = (uintptr_t)addr & ~(PAGE - 1);
    uintptr_t len = (((uintptr_t)addr + size - start) + PAGE - 1) & ~(PAGE - 1);
    unsigned char vec[8] = {};  // covers up to 8 pages
    if (len > sizeof(vec) * PAGE) return false;
    return mincore((void *)start, len, vec) == 0;
}

static uint32_t safe_read_u32(uint64_t addr) {
    if (!is_addr_mapped(addr, sizeof(uint32_t))) return 0;
    return *(uint32_t *)(uintptr_t)addr;
}

static void * safe_read_ptr(uint64_t addr) {
    if (!is_addr_mapped(addr, sizeof(void *))) return nullptr;
    return *(void **)(uintptr_t)addr;
}

#elif defined(__APPLE__) && !defined(__EMSCRIPTEN__)

#include <mach/mach.h>
#include <mach/mach_vm.h>

// mach_vm_read_overwrite() handles unmapped/unreadable pages gracefully and
// is safe to call from a signal handler (it's a Mach trap, not a POSIX call).
static uint32_t safe_read_u32(uint64_t addr) {
    uint32_t result = 0;
    mach_vm_size_t bytes_read = 0;
    mach_vm_read_overwrite(mach_task_self(),
        (mach_vm_address_t)addr, sizeof(uint32_t),
        (mach_vm_address_t)&result, &bytes_read);
    return (bytes_read == sizeof(uint32_t)) ? result : 0;
}

static void * safe_read_ptr(uint64_t addr) {
    void * result = nullptr;
    mach_vm_size_t bytes_read = 0;
    mach_vm_read_overwrite(mach_task_self(),
        (mach_vm_address_t)addr, sizeof(void *),
        (mach_vm_address_t)&result, &bytes_read);
    return (bytes_read == sizeof(void *)) ? result : nullptr;
}

#endif  // platform safe reads

// ---- Shared: context validation and das stack walk ----------------------

// Validate a candidate Context* by checking the magic number.
// context_magic is the first non-vtable field: layout [vtable(8)][magic(4)]...
static bool validate_context_ptr(void * ctxPtr) {
    uint64_t addr = (uint64_t)(uintptr_t)ctxPtr + offsetof(Context, context_magic);
    return safe_read_u32(addr) == Context::CONTEXT_MAGIC;
}

// Scan a pointer-aligned window around each collected frame pointer for a
// valid Context*.  The window covers both negative offsets (GCC/Clang spill
// slots on Linux/Mac) and [FP+0] (MSVC virtual-frame slot on Windows).
static void das_crash_extra_info(CrashFrame * frames, int frameCount) {
    static const int MAX_CONTEXTS = 16;
    void * contexts[MAX_CONTEXTS];
    int contextCount = 0;
    for (int i = 0; i < frameCount; i++) {
        const auto rbp = frames[i].frameRbp;
        for (int64_t off = -128; off <= 8; off += (int64_t)sizeof(void *)) {
            uint64_t probe = (uint64_t)((int64_t)rbp + off);
            void * candidate = safe_read_ptr(probe);
            if (!candidate || !validate_context_ptr(candidate))
                continue;
            bool found = false;
            for (int j = 0; j < contextCount; j++) {
                if (contexts[j] == candidate) {
                    found = true;
                    break;
                }
            }
            if (!found && contextCount < MAX_CONTEXTS) {
                contexts[contextCount++] = candidate;
                break;
            }
        }
    }
    for (int i = 0; i < contextCount; i++) {
        fprintf(stderr, "\ndaslang stack (Context 0x%llx):\n",
            (unsigned long long)(uintptr_t)contexts[i]);
        print_das_stack_walk(contexts[i]);
    }
}

#endif  // DAS_CRASH_HANDLER_PLATFORM_SUPPORTED

namespace das {
    void install_das_crash_handler() {
        #if DAS_CRASH_HANDLER_PLATFORM_SUPPORTED
        set_crash_handler_extra_info(das_crash_frame_filter, das_crash_extra_info);
        #endif
        install_crash_handler();
    }
}
