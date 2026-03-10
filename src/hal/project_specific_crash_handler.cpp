#include "daScript/misc/platform.h"
#include "daScript/misc/crash_handler.h"
#include "daScript/simulate/simulate.h"

#if DAS_CRASH_HANDLER_PLATFORM_SUPPORTED && defined(_WIN32) && defined(_M_X64)

#include <cstring>

using namespace das;

// SEH-safe read of a uint32 from a given address. Returns 0 on fault.
static uint32_t safe_read_u32(uint64_t addr) {
    uint32_t result = 0;
    __try {
        result = *(uint32_t *)addr;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        result = 0;
    }
    return result;
}

// SEH-safe read of a pointer from a given address. Returns nullptr on fault.
static void * safe_read_ptr(uint64_t addr) {
    void * result = nullptr;
    __try {
        result = *(void **)addr;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        result = nullptr;
    }
    return result;
}

// Validate a candidate Context* by checking the magic number.
// Works with Context and any derived class.
static bool validate_context_ptr(void * ctxPtr) {
    // context_magic is the first non-vtable field in Context.
    // Layout: [vtable_ptr(8)] [context_magic(4)] ...
    uint64_t addr = (uint64_t)(uintptr_t)ctxPtr + offsetof(Context, context_magic);
    return safe_read_u32(addr) == Context::CONTEXT_MAGIC;
}

// Print the das stack walk for a validated Context. Separate function
// because C++ objects (std::string) cannot coexist with __try/__except.
static void print_das_stack_walk(void * ctxPtr) {
    auto * ctx = (Context *)ctxPtr;
    auto str = ctx->getStackWalk(nullptr, true, false);
    fprintf(stderr, "%s", str.c_str());
}

// Frame filter: only collect frames whose symbol name contains "SimNode".
static bool das_crash_frame_filter(const char * symbolName) {
    return strstr(symbolName, "SimNode") != nullptr;
}

// Extra-info callback: the frames array only contains SimNode frames
// (pre-filtered by das_crash_frame_filter). Probe each for Context* at [RBP+0],
// then call getStackWalk() on each unique Context found.
static void das_crash_extra_info(CrashFrame * frames, int frameCount) {
    static const int MAX_CONTEXTS = 16;
    void * contexts[MAX_CONTEXTS];
    int contextCount = 0;
    for (int i = 0; i < frameCount; i++) {
        // On x64 MSVC, Context* is at [RBP+0] for SimNode eval methods
        void * candidate = safe_read_ptr(frames[i].frameRbp);
        if (!candidate || !validate_context_ptr(candidate))
            continue;
        // Deduplicate
        bool found = false;
        for (int j = 0; j < contextCount; j++) {
            if (contexts[j] == candidate) { found = true; break; }
        }
        if (!found && contextCount < MAX_CONTEXTS) {
            contexts[contextCount++] = candidate;
        }
    }
    for (int i = 0; i < contextCount; i++) {
        fprintf(stderr, "\ndaslang stack (Context 0x%llx):\n",
            (unsigned long long)(uintptr_t)contexts[i]);
        print_das_stack_walk(contexts[i]);
    }
}

namespace das {
    void install_das_crash_handler() {
        set_crash_handler_extra_info(das_crash_frame_filter, das_crash_extra_info);
        install_crash_handler();
    }
}

#else

namespace das {
    void install_das_crash_handler() {
        install_crash_handler();
    }
}

#endif
