#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// The nano Context. Everything here has a counterpart in src/runtime/context.cpp
// and src/simulate/simulate_exceptions.cpp; what is missing is missing because
// standalone AOT never reaches it - no debug agents, no code relocation, no
// cloning, no init/shutdown scripts (the generated constructor calls the init
// script itself), no GC roots.

namespace das {

    Context::Context(uint32_t stackSize, bool ph) : stack(stackSize) {
        code = make_shared<NodeAllocator>();
        constStringHeap = make_shared<ConstStringAllocator>();
        debugInfo = make_shared<DebugInfoAllocator>();
        ownStack = (stackSize != 0);
        persistent = ph;
    }

    Context::~Context() {
        freeGlobalsAndShared();
        if ( contextMutex ) {
            delete contextMutex;
            contextMutex = nullptr;
        }
    }

    void Context::setup(int totalVars, uint32_t globalStringHeapSize, CodeOfPolicies policies, AnnotationArgumentList options) {
        if ( policies.abi_stamp != CodeOfPolicies::expected_abi_stamp() ) {
            DAS_FATAL_ERROR("CodeOfPolicies ABI stamp mismatch at Context::setup: host wrote 0x%llx, this libDaScriptNano expects 0x%llx - the generated code and these headers came from different daslang builds",
                (unsigned long long) policies.abi_stamp,
                (unsigned long long) CodeOfPolicies::expected_abi_stamp());
        }
        verySafeContext = options.getBoolOption("very_safe_context",policies.very_safe_context);
        maxUnreservedSize = options.getUInt64Option("max_unreserved_size", policies.max_unreserved_size);
        persistent = options.getBoolOption("persistent_heap", policies.persistent_heap);
        if ( persistent ) {
            heap = make_unique<PersistentHeapAllocator>();
            stringHeap = make_unique<PersistentStringAllocator>();
        } else {
            heap = make_unique<LinearHeapAllocator>();
            stringHeap = make_unique<LinearStringAllocator>();
        }
        heap->setInitialSize ( options.getIntOption("heap_size_hint", policies.heap_size_hint) );
        heap->setLimit ( options.getUInt64OptionEx("heap_size_limit", "max_heap_allocated", policies.max_heap_allocated) );
        stringHeap->setInitialSize ( options.getIntOption("string_heap_size_hint", policies.string_heap_size_hint) );
        stringHeap->setLimit ( options.getUInt64OptionEx("string_heap_size_limit", "max_string_heap_allocated", policies.max_string_heap_allocated) );
        constStringHeap = make_shared<ConstStringAllocator>();
        totalVariables = totalVars;
        if ( globalStringHeapSize ) {
            constStringHeap->setInitialSize(globalStringHeapSize);
        }
        globalVariables = (GlobalVariable *) code->allocate( uint32_t(totalVars*sizeof(GlobalVariable)) );
        globalsSize = 0;
        sharedSize = 0;
    }

    void Context::freeGlobalsAndShared() {
        if ( globals && globalsOwner ) {
            das_aligned_free16(globals);
            globals = nullptr;
        }
        if ( shared && sharedOwner ) {
            das_aligned_free16(shared);
            shared = nullptr;
        }
    }

    void Context::allocateGlobalsAndShared() {
        freeGlobalsAndShared();
        globals = globalsSize ? (char *) das_aligned_alloc16(globalsSize) : nullptr;
        shared = (sharedOwner && sharedSize) ? (char *) das_aligned_alloc16(sharedSize) : nullptr;
        if ( shared ) memset(shared, 0, sharedSize);
        globalsOwner = true;
        sharedOwner = true;
    }

    SimFunction * Context::findFunction ( const char * name ) const {
        for ( int fni = 0; fni != totalFunctions; ++fni ) {
            if ( strcmp(functions[fni].name, name)==0 ) {
                return functions + fni;
            }
        }
        return nullptr;
    }

    SimFunction * Context::findFunction ( const char * name, bool & isUnique ) const {
        SimFunction * found = nullptr;
        isUnique = true;
        for ( int fni = 0; fni != totalFunctions; ++fni ) {
            if ( strcmp(functions[fni].name, name)==0 ) {
                if ( found ) {
                    isUnique = false;
                    return found;
                }
                found = functions + fni;
            }
        }
        return found;
    }

    int Context::findVariable ( const char * name ) const {
        for ( int vi = 0; vi != totalVariables; ++vi ) {
            if ( strcmp(globalVariables[vi].name, name)==0 ) {
                return vi;
            }
        }
        return -1;
    }

    void Context::to_out ( const LineInfo *, int level, const char * message ) {
        if ( message ) {
            das_to_stdout_level_prefix_text(level, getLogMarker(level), message);
        }
    }

    void Context::breakPoint(const LineInfo &, const char *, const char *) {
        os_debug_break();
    }

    void Context::throw_fatal_error ( const char * message, const LineInfo & at ) {
        const char * text = message ? message : "";
        size_t len = strlen(text);
        if ( len > EXCEPTION_MESSAGE_SIZE - 2 ) len = EXCEPTION_MESSAGE_SIZE - 2;
        memcpy(exceptionMessage, text, len);
        exceptionMessage[len] = '\n';
        exceptionMessage[len+1] = 0;
        exception = exceptionMessage;
        exceptionAt = at;
        if ( throwBuf ) {
#if defined(WIN64) || defined(_WIN64)
            //  "An invalid or unaligned stack was encountered during an unwind operation."
            //  is issued via longjmp - a known x64 issue; zeroing Frame disables unwinding
            ((_JUMP_BUFFER *)throwBuf)->Frame = 0;
#endif
            longjmp(*throwBuf,1);
        }
        to_err(&at, "\nunhandled exception\n");
        to_err(&at, exception);
        breakPoint(at, "exception", exception);
        exit(1);
    }

    void Context::rethrow () {
        if ( throwBuf ) {
#if defined(WIN64) || defined(_WIN64)
            ((_JUMP_BUFFER *)throwBuf)->Frame = 0;
#endif
            longjmp(*throwBuf,1);
        }
        to_err(nullptr, "\nunhandled exception\n");
        if ( exception ) to_err(nullptr, exception);
        breakPoint(exceptionAt, "exception", exception ? exception : "");
        exit(1);
    }

    void Context::throw_error ( const char * message ) {
        throw_fatal_error(message, LineInfo());
    }

    void Context::throw_error_ex ( DAS_FORMAT_STRING_PREFIX const char * message, ... ) {
        char buffer[EXCEPTION_MESSAGE_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf (buffer,EXCEPTION_MESSAGE_SIZE,message, args);
        va_end (args);
        throw_fatal_error(buffer, LineInfo());
    }

    void Context::throw_error_at ( const LineInfo * at, DAS_FORMAT_STRING_PREFIX const char * message, ... ) {
        char buffer[EXCEPTION_MESSAGE_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf (buffer,EXCEPTION_MESSAGE_SIZE,message, args);
        va_end (args);
        throw_fatal_error(buffer, at ? *at : LineInfo());
    }

    void Context::throw_error_at ( const LineInfo & at, DAS_FORMAT_STRING_PREFIX const char * message, ... ) {
        char buffer[EXCEPTION_MESSAGE_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf (buffer,EXCEPTION_MESSAGE_SIZE,message, args);
        va_end (args);
        throw_fatal_error(buffer, at);
    }

    void Context::throw_out_of_memory ( bool isStringHeap, uint64_t size, const LineInfo * at ) {
        throw_error_at(at, "out of %s memory, requested %llu bytes",
            isStringHeap ? "string heap" : "heap", (unsigned long long) size);
    }
}
