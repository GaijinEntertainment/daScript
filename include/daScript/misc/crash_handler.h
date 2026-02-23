#pragma once
#include "daScript/misc/platform.h"

#ifndef DAS_USE_BASE_CRASH_HANDLER
#define DAS_USE_BASE_CRASH_HANDLER 0
#endif


#if DAS_USE_BASE_CRASH_HANDLER && defined(_WIN32)

#define DAS_CRASH_HANDLER_PLATFORM_SUPPORTED 1

#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#elif DAS_USE_BASE_CRASH_HANDLER && ((defined(__linux__) || defined(__APPLE__)) && !defined(__EMSCRIPTEN__))

#define DAS_CRASH_HANDLER_PLATFORM_SUPPORTED 1

#include <cstdio>
#include <signal.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#else

#define DAS_CRASH_HANDLER_PLATFORM_SUPPORTED 0

#endif


namespace das {

#define DAS_CRASH_HANDLER_MAX_STACK_FRAMES 64

#if !DAS_CRASH_HANDLER_PLATFORM_SUPPORTED

    inline void install_crash_handler() {}

#elif defined(_WIN32)

    inline void print_stack_trace(CONTEXT *ctx) {
        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();
        SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
        if (!SymInitialize(process, NULL, TRUE)) {
            fprintf(stderr, "SymInitialize failed: %lu\n", GetLastError());
            return;
        }
        STACKFRAME64 frame = {};
#if defined(_M_X64)
        frame.AddrPC.Offset = ctx->Rip;
        frame.AddrFrame.Offset = ctx->Rbp;
        frame.AddrStack.Offset = ctx->Rsp;
        DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
#elif defined(_M_ARM64)
        frame.AddrPC.Offset = ctx->Pc;
        frame.AddrFrame.Offset = ctx->Fp;
        frame.AddrStack.Offset = ctx->Sp;
        DWORD machineType = IMAGE_FILE_MACHINE_ARM64;
#else
        frame.AddrPC.Offset = ctx->Eip;
        frame.AddrFrame.Offset = ctx->Ebp;
        frame.AddrStack.Offset = ctx->Esp;
        DWORD machineType = IMAGE_FILE_MACHINE_I386;
#endif
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Mode = AddrModeFlat;

        alignas(SYMBOL_INFO) char symbolBuf[sizeof(SYMBOL_INFO) + 256];
        SYMBOL_INFO *symbol = (SYMBOL_INFO *)symbolBuf;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = 255;

        IMAGEHLP_LINE64 line = {};
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        fprintf(stderr, "Stack trace:\n");
        for (int i = 0; i < DAS_CRASH_HANDLER_MAX_STACK_FRAMES; i++) {
            if (!StackWalk64(machineType, process, thread, &frame, ctx, NULL,
                             SymFunctionTableAccess64, SymGetModuleBase64, NULL))
                break;
            if (frame.AddrPC.Offset == 0)
                break;
            DWORD64 displacement64 = 0;
            DWORD displacement32 = 0;
            if (SymFromAddr(process, frame.AddrPC.Offset, &displacement64, symbol)) {
                if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &displacement32, &line)) {
                    fprintf(stderr, "  [%2d] %s + 0x%llx (%s:%lu)\n", i, symbol->Name,
                            (unsigned long long)displacement64, line.FileName, line.LineNumber);
                } else {
                    fprintf(stderr, "  [%2d] %s + 0x%llx\n", i, symbol->Name,
                            (unsigned long long)displacement64);
                }
            } else {
                fprintf(stderr, "  [%2d] 0x%llx\n", i, (unsigned long long)frame.AddrPC.Offset);
            }
        }
        SymCleanup(process);
    }

    inline const char * exception_code_to_string(DWORD code) {
        switch (code) {
            case EXCEPTION_ACCESS_VIOLATION:       return "EXCEPTION_ACCESS_VIOLATION";
            case EXCEPTION_STACK_OVERFLOW:         return "EXCEPTION_STACK_OVERFLOW";
            case EXCEPTION_INT_DIVIDE_BY_ZERO:     return "EXCEPTION_INT_DIVIDE_BY_ZERO";
            case EXCEPTION_ILLEGAL_INSTRUCTION:    return "EXCEPTION_ILLEGAL_INSTRUCTION";
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:     return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            case EXCEPTION_FLT_OVERFLOW:           return "EXCEPTION_FLT_OVERFLOW";
            case EXCEPTION_FLT_UNDERFLOW:          return "EXCEPTION_FLT_UNDERFLOW";
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:  return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            case EXCEPTION_BREAKPOINT:             return "EXCEPTION_BREAKPOINT";
            case EXCEPTION_DATATYPE_MISALIGNMENT:  return "EXCEPTION_DATATYPE_MISALIGNMENT";
            case EXCEPTION_IN_PAGE_ERROR:          return "EXCEPTION_IN_PAGE_ERROR";
            default:                               return "UNKNOWN_EXCEPTION";
        }
    }

    inline LONG WINAPI crash_handler_callback(EXCEPTION_POINTERS *ep) {
        DWORD code = ep->ExceptionRecord->ExceptionCode;
        fprintf(stderr, "\nCRASH: %s (0x%08lX) at address 0x%llx\n",
                exception_code_to_string(code), (unsigned long)code,
                (unsigned long long)ep->ExceptionRecord->ExceptionAddress);
        if (code == EXCEPTION_ACCESS_VIOLATION && ep->ExceptionRecord->NumberParameters >= 2) {
            fprintf(stderr, "  %s address 0x%llx\n",
                    ep->ExceptionRecord->ExceptionInformation[0] ? "writing" : "reading",
                    (unsigned long long)ep->ExceptionRecord->ExceptionInformation[1]);
        }
        print_stack_trace(ep->ContextRecord);
        fflush(stderr);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    inline void install_crash_handler() {
        SetUnhandledExceptionFilter(crash_handler_callback);
    }

#elif (defined(__linux__) || defined(__APPLE__)) && !defined(__EMSCRIPTEN__)

    inline const char * signal_to_string(int sig) {
        switch (sig) {
            case SIGSEGV: return "SIGSEGV (Segmentation fault)";
            case SIGBUS:  return "SIGBUS (Bus error)";
            case SIGABRT: return "SIGABRT (Aborted)";
            case SIGFPE:  return "SIGFPE (Floating point exception)";
            case SIGILL:  return "SIGILL (Illegal instruction)";
            default:      return "UNKNOWN SIGNAL";
        }
    }

    inline void crash_signal_handler(int sig, siginfo_t *info, void * /*ucontext*/) {
        fprintf(stderr, "\nCRASH: %s (signal %d)", signal_to_string(sig), sig);
        if (info->si_addr)
            fprintf(stderr, " at address %p", info->si_addr);
        fprintf(stderr, "\n");

        void *frames[DAS_CRASH_HANDLER_MAX_STACK_FRAMES];
        int nframes = backtrace(frames, DAS_CRASH_HANDLER_MAX_STACK_FRAMES);
        if (nframes > 0) {
            fprintf(stderr, "Stack trace:\n");
            char **symbols = backtrace_symbols(frames, nframes);
            if (symbols) {
                for (int i = 0; i < nframes; i++) {
                    char *mangled_start = strchr(symbols[i], '(');
                    char *mangled_end = mangled_start ? strchr(mangled_start, '+') : nullptr;
                    if (mangled_start && mangled_end) {
                        *mangled_end = '\0';
                        int status = -1;
                        char *demangled = abi::__cxa_demangle(mangled_start + 1, nullptr, nullptr, &status);
                        if (status == 0 && demangled) {
                            *mangled_end = '+';
                            *mangled_start = '\0';
                            fprintf(stderr, "  [%2d] %s(%s+%s\n", i, symbols[i], demangled, mangled_end + 1);
                            free(demangled);
                        } else {
                            *mangled_end = '+';
                            fprintf(stderr, "  [%2d] %s\n", i, symbols[i]);
                        }
                    } else {
                        fprintf(stderr, "  [%2d] %s\n", i, symbols[i]);
                    }
                }
                free(symbols);
            } else {
                backtrace_symbols_fd(frames, nframes, STDERR_FILENO);
            }
        }
        fflush(stderr);

        signal(sig, SIG_DFL);
        raise(sig);
    }

    inline void install_crash_handler() {
        struct sigaction sa = {};
        sa.sa_sigaction = crash_signal_handler;
        sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGSEGV, &sa, nullptr);
        sigaction(SIGBUS,  &sa, nullptr);
        sigaction(SIGABRT, &sa, nullptr);
        sigaction(SIGFPE,  &sa, nullptr);
        sigaction(SIGILL,  &sa, nullptr);
    }

#else

    inline void install_crash_handler() {}

#endif

#undef DAS_CRASH_HANDLER_MAX_STACK_FRAMES

} // namespace das
