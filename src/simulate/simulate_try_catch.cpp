#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/simulate_nodes.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4611)
#endif

namespace das {
    // SimNode_TryCatch

    vec4f WIN_EH_NO_ASAN SimNode_TryCatch::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto aa = context.abiArg; auto acm = context.abiCMRES; auto fcd = context.fastCallDepth;
        auto atba = context.abiThisBlockArg;
        char * EP, * SP;
        context.stack.watermark(EP,SP);
        #if DAS_ENABLE_EXCEPTIONS
            try {
                try_block->eval(context);
            } catch ( const dasException & ) {
                context.abiArg = aa;
                context.abiCMRES = acm;
                context.abiThisBlockArg = atba;
                context.fastCallDepth = fcd;
                context.stack.pop(EP,SP);
                context.stopFlags = 0;
                context.last_exception = context.exception;
                context.exception = nullptr;
                catch_block->eval(context);
            }
        #else
            jmp_buf ev;
            jmp_buf * JB = context.throwBuf;
            context.throwBuf = &ev;
            if ( !das_setjmp(ev) ) {
                try_block->eval(context);
            } else {
                context.throwBuf = JB;
                context.abiArg = aa;
                context.abiCMRES = acm;
                context.abiThisBlockArg = atba;
                context.fastCallDepth = fcd;
                context.stack.pop(EP,SP);
                context.stopFlags = 0;
                context.last_exception = context.exception;
                context.exception = nullptr;
                catch_block->eval(context);
            }
            context.throwBuf = JB;
        #endif
        return v_zero();
    }

#if DAS_DEBUGGER
    vec4f WIN_EH_NO_ASAN SimNodeDebug_TryCatch::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto aa = context.abiArg; auto acm = context.abiCMRES; auto fcd = context.fastCallDepth;
        auto atba = context.abiThisBlockArg;
        char * EP, * SP;
        context.stack.watermark(EP,SP);
        #if DAS_ENABLE_EXCEPTIONS
            try {
                DAS_SINGLE_STEP(context,try_block->debugInfo,false);
                try_block->eval(context);
            } catch ( const dasException & ) {
                context.abiArg = aa;
                context.abiCMRES = acm;
                context.abiThisBlockArg = atba;
                context.fastCallDepth = fcd;
                context.stack.pop(EP,SP);
                context.stopFlags = 0;
                context.last_exception = context.exception;
                context.exception = nullptr;
                DAS_SINGLE_STEP(context,catch_block->debugInfo,false);
                catch_block->eval(context);
            }
        #else
            jmp_buf ev;
            jmp_buf * JB = context.throwBuf;
            context.throwBuf = &ev;
            if ( !das_setjmp(ev) ) {
                DAS_SINGLE_STEP(context,try_block->debugInfo,false);
                try_block->eval(context);
            } else {
                context.throwBuf = JB;
                context.abiArg = aa;
                context.abiCMRES = acm;
                context.abiThisBlockArg = atba;
                context.fastCallDepth = fcd;
                context.stack.pop(EP,SP);
                context.stopFlags = 0;
                context.last_exception = context.exception;
                context.exception = nullptr;
                DAS_SINGLE_STEP(context,catch_block->debugInfo,false);
                catch_block->eval(context);
            }
            context.throwBuf = JB;
        #endif
        return v_zero();
    }
#endif
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
