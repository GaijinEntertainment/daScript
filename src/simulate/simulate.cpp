#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/debug_print.h"
#include "daScript/misc/fpe.h"
#include "daScript/misc/debug_break.h"

#include <stdarg.h>
#include <mutex>

namespace das
{
    bool PointerDimIterator::first ( Context &, char * _value ) {
        char ** value = (char **) _value;
        if ( data != data_end ) {
            *value = *data;
            return true;
        } else {
            return false;
        }
    }

    bool PointerDimIterator::next  ( Context &, char * _value ) {
        char ** value = (char **) _value;
        if ( ++data != data_end ) {
            *value = *data;
            return true;
        } else {
            return false;
        }
    }

    void PointerDimIterator::close ( Context & context, char * _value ) {
        if ( _value ) {
            char ** value = (char **) _value;
            *value = nullptr;
        }
        context.heap->free((char *)this, size);
    }

    // this is here to occasionally investigate untyped evaluation paths
    #define WARN_SLOW_CAST(TYPE)
    // #define WARN_SLOW_CAST(TYPE)    DAS_ASSERTF(0, "internal perofrmance issue, casting eval to eval##TYPE" );

    StackAllocator *SharedStackGuard::lastContextStack = nullptr;

    SimNode * SimNode::copyNode ( Context &, NodeAllocator * code ) {
        auto prefix = ((NodePrefix *)this) - 1;
#ifndef NDEBUG
        DAS_ASSERTF(prefix->magic==0xdeadc0de,"node was allocated on the heap without prefix");
#endif
        char * newNode = code->allocate(prefix->size);
        memcpy ( newNode, (char *)this, prefix->size );
        return (SimNode *) newNode;
    }

    bool SimNode::evalBool ( Context & context ) {
        WARN_SLOW_CAST(Bool);
        return cast<bool>::to(eval(context));
    }

    float SimNode::evalFloat ( Context & context ) {
        WARN_SLOW_CAST(Float);
        return cast<float>::to(eval(context));
    }

    double SimNode::evalDouble(Context & context) {
        WARN_SLOW_CAST(Double);
        return cast<double>::to(eval(context));
    }

    int32_t SimNode::evalInt ( Context & context ) {
        WARN_SLOW_CAST(Int);
        return cast<int32_t>::to(eval(context));
    }

    uint32_t SimNode::evalUInt ( Context & context ) {
        WARN_SLOW_CAST(UInt);
        return cast<uint32_t>::to(eval(context));
    }

    int64_t SimNode::evalInt64 ( Context & context ) {
        WARN_SLOW_CAST(Int64);
        return cast<int64_t>::to(eval(context));
    }

    uint64_t SimNode::evalUInt64 ( Context & context ) {
        WARN_SLOW_CAST(UInt64);
        return cast<uint64_t>::to(eval(context));
    }

    char * SimNode::evalPtr ( Context & context ) {
        WARN_SLOW_CAST(Ptr);
        return cast<char *>::to(eval(context));
    }

    vec4f SimNode_DeleteStructPtr::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pStruct = (char **) subexpr->evalPtr(context);
        pStruct = pStruct + total - 1;
        for ( uint32_t i=0; i!=total; ++i, pStruct-- ) {
            if ( *pStruct ) {
                if ( persistent ) {
                    das_aligned_free16(*pStruct);
                } else if ( isLambda ) {
                    context.heap->free(*pStruct - 16, structSize + 16);
                } else {
                    context.heap->free(*pStruct, structSize);
                }
                *pStruct = nullptr;
            }
        }
        return v_zero();
    }

    vec4f SimNode_DeleteLambda::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pLambda = (Lambda *) subexpr->evalPtr(context);
        pLambda = pLambda + total - 1;
        for ( uint32_t i=0; i!=total; ++i, pLambda-- ) {
            if ( pLambda->capture ) {
                int32_t * fnIndex = (int32_t *) pLambda->capture;
                SimFunction * simFunc = context.getFunction(fnIndex[1]-1);
                if (!simFunc) context.throw_error_at(debugInfo, "lambda finalizer is a null function");
                vec4f argValues[1] = {
                    cast<void *>::from(pLambda->capture)
                };
                context.call(simFunc, argValues, 0);
                pLambda->capture = nullptr;
            }
        }
        return v_zero();
    }

    vec4f SimNode_Swizzle::eval ( Context & context ) {
        DAS_PROFILE_NODE
        union {
            vec4f   res;
            float   val[4];
        } R, S;
        S.res = value->eval(context);
        R.val[0] = S.val[fields[0]];
        R.val[1] = S.val[fields[1]];
        R.val[2] = S.val[fields[2]];
        R.val[3] = S.val[fields[3]];
        return R.res;
    }

    // SimNode_MakeBlock

    vec4f SimNode_MakeBlock::eval ( Context & context )  {
        DAS_PROFILE_NODE
        Block * block = (Block *) ( context.stack.sp() + stackTop );
        block->stackOffset = context.stack.spi();
        block->argumentsOffset = argStackTop ? (context.stack.spi() + argStackTop) : 0;
        block->body = subexpr;
        block->aotFunction = nullptr;
        block->functionArguments = context.abiArguments();
        block->info = info;
        return cast<Block *>::from(block);
    }

    // SimNode_Debug

    void das_debug ( Context * context, TypeInfo * typeInfo, const char * FILE, int LINE, vec4f res, const char * message ) {
        TextWriter ssw;
        if ( message ) ssw << message << " ";
        ssw << debug_type(typeInfo) << " = " << debug_value(res, typeInfo, PrintFlags::debugger)
        << " at " << FILE << ":" << LINE << "\n";
        context->to_out(ssw.str().c_str());
    }

    vec4f SimNode_Debug::eval ( Context & context ) {
        DAS_PROFILE_NODE
        FPE_DISABLE;
        vec4f res = subexpr->eval(context);
        TextWriter ssw;
        if ( message ) ssw << message << " ";
        ssw << debug_type(typeInfo) << " = " << debug_value(res, typeInfo, PrintFlags::debugger)
            << " at " << debugInfo.describe() << "\n";
        context.to_out(ssw.str().c_str());
        return res;
    }

    // SimNode_Assert

    vec4f SimNode_Assert::eval ( Context & context ) {
        DAS_PROFILE_NODE
        if ( !subexpr->evalBool(context) ) {
            string error_message = "assert failed";
            if ( message )
                error_message = error_message + ", " + message;
            string error = reportError(debugInfo, error_message, "", "");
#ifdef NDEBUG
            error = context.getStackWalk(&debugInfo, false, false) + error;
#else
            error = context.getStackWalk(&debugInfo, true, true) + error;
#endif
            context.to_err(error.c_str());
            context.throw_error_at(debugInfo,"assert failed");
        }
        return v_zero();
    }

    // SimNode_TryCatch

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4611)
#endif

    vec4f SimNode_TryCatch::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto aa = context.abiArg; auto acm = context.abiCMRES;
        char * EP, * SP;
        context.stack.watermark(EP,SP);
        #if DAS_ENABLE_EXCEPTIONS
            try {
                try_block->eval(context);
            } catch ( const dasException & ) {
                context.abiArg = aa;
                context.abiCMRES = acm;
                context.stack.pop(EP,SP);
                context.stopFlags = 0;
                context.exception = nullptr;
                catch_block->eval(context);
            }
        #else
            jmp_buf ev;
            jmp_buf * JB = context.throwBuf;
            context.throwBuf = &ev;
            if ( !setjmp(ev) ) {
                try_block->eval(context);
            } else {
                context.throwBuf = JB;
                context.abiArg = aa;
                context.abiCMRES = acm;
                context.stack.pop(EP,SP);
                context.stopFlags = 0;
                context.exception = nullptr;
                catch_block->eval(context);
            }
            context.throwBuf = JB;
        #endif
        return v_zero();
    }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    // SimNode_CopyRefValue

    vec4f SimNode_CopyRefValue::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pl = l->evalPtr(context);
        auto pr = r->evalPtr(context);
        memcpy ( pl, pr, size );
        return v_zero();
    }

    // SimNode_MoveRefValue

    vec4f SimNode_MoveRefValue::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pl = l->evalPtr(context);
        auto pr = r->evalPtr(context);
        memcpy ( pl, pr, size );
        memset ( pr, 0, size );
        return v_zero();
    }

    // SimNode_CallBase

    SimNode * SimNode_CallBase::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_CallBase * that = (SimNode_CallBase *) SimNode::copyNode(context, code);
        if ( nArguments ) {
            SimNode ** newArguments = (SimNode **) code->allocate(nArguments * sizeof(SimNode *));
            memcpy ( newArguments, that->arguments, nArguments * sizeof(SimNode *));
            that->arguments = newArguments;
            if ( that->types ) {
                TypeInfo ** newTypes = (TypeInfo **) code->allocate(nArguments * sizeof(TypeInfo **));
                memcpy ( newTypes, that->types, nArguments * sizeof(TypeInfo **));
                that->types = newTypes;
            }
        }
        if ( fnPtr ) {
            that->fnPtr = context.getFunction(fnIndex);
        }
        return that;
    }

    // SimNode_Final

    SimNode * SimNode_Final::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_Final * that = (SimNode_Final *) SimNode::copyNode(context, code);
        if ( totalFinal ) {
            SimNode ** newList = (SimNode **) code->allocate(totalFinal * sizeof(SimNode *));
            memcpy ( newList, that->finalList, totalFinal*sizeof(SimNode *));
            that->finalList = newList;
        }
        return that;
    }

    // SimNode_Block

    SimNode * SimNode_Block::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_Block * that = (SimNode_Block *) SimNode_Final::copyNode(context, code);
        if ( total ) {
            SimNode ** newList = (SimNode **) code->allocate(total * sizeof(SimNode *));
            memcpy ( newList, that->list, total*sizeof(SimNode *));
            that->list = newList;
        }
        if ( totalLabels ) {
            uint32_t * newLabels = (uint32_t *) code->allocate(totalLabels * sizeof(uint32_t));
            memcpy ( newLabels, that->labels, totalLabels*sizeof(uint32_t));
            that->labels = newLabels;
        }
        return that;
    }

    vec4f SimNode_Block::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        evalFinal(context);
        return v_zero();
    }

#if DAS_DEBUGGER

    vec4f SimNodeDebug_Block::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            DAS_SINGLE_STEP(context,(*body)->debugInfo,false);
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        evalFinal(context);
        return v_zero();
    }

#endif

    vec4f SimNode_BlockNF::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        return v_zero();
    }

#if DAS_DEBUGGER

    vec4f SimNodeDebug_BlockNF::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            DAS_SINGLE_STEP(context,(*body)->debugInfo,false);
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        return v_zero();
    }

#endif

    vec4f SimNode_ClosureBlock::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        evalFinal(context);
        if ( context.stopFlags & EvalFlags::stopForReturn ) {
            context.stopFlags &= ~EvalFlags::stopForReturn;
            return context.abiResult();
        } else {
            if ( needResult ) context.throw_error_at(debugInfo,"end of block without return");
            return v_zero();
        }
    }

#if DAS_DEBUGGER

    vec4f SimNodeDebug_ClosureBlock::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        for (SimNode ** __restrict body = list; body!=tail; ++body) {
            DAS_SINGLE_STEP(context,(*body)->debugInfo,false);
            (*body)->eval(context);
            if ( context.stopFlags ) break;
        }
        evalFinal(context);
        if ( context.stopFlags & EvalFlags::stopForReturn ) {
            context.stopFlags &= ~EvalFlags::stopForReturn;
            return context.abiResult();
        } else {
            if ( needResult ) context.throw_error_at(debugInfo,"end of block without return");
            return v_zero();
        }
    }

#endif

// SimNode_BlockWithLabels

    vec4f SimNode_BlockWithLabels::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        SimNode ** __restrict body = list;
    loopbegin:;
        for (; body!=tail; ++body) {
            (*body)->eval(context);
            { if ( context.stopFlags ) {
                if (context.stopFlags&EvalFlags::jumpToLabel && context.gotoLabel<totalLabels) {
                    body=list+labels[context.gotoLabel];
                    if ( body>=list && body<tail ) {
                        context.stopFlags &= ~EvalFlags::jumpToLabel;
                        goto loopbegin;
                    } else {
                        context.throw_error_at(debugInfo, "jump to label %i failed", context.gotoLabel);
                    }
                }
                goto loopend;
            } }
        }
    loopend:;
        evalFinal(context);
        return v_zero();
    }

#if DAS_DEBUGGER

    vec4f SimNodeDebug_BlockWithLabels::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        SimNode ** __restrict body = list;
    loopbegin:;
        for (; body!=tail; ++body) {
            DAS_SINGLE_STEP(context,(*body)->debugInfo,false);
            (*body)->eval(context);
            { if ( context.stopFlags ) {
                if (context.stopFlags&EvalFlags::jumpToLabel && context.gotoLabel<totalLabels) {
                    body=list+labels[context.gotoLabel];
                    if ( body>=list && body<tail ) {
                        context.stopFlags &= ~EvalFlags::jumpToLabel;
                        goto loopbegin;
                    } else {
                        context.throw_error_at(debugInfo, "jump to label %i failed", context.gotoLabel);
                    }
                }
                goto loopend;
            } }
        }
    loopend:;
        evalFinal(context);
        return v_zero();
    }

#endif

    // SimNode_Let

    vec4f SimNode_Let::eval ( Context & context ) {
        DAS_PROFILE_NODE
        for ( uint32_t i = 0; i!=total && !context.stopFlags; ) {
            list[i++]->eval(context);
        }
        return v_zero();
    }

    // SimNode_While

    vec4f SimNode_While::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        while ( cond->evalBool(context) && !context.stopFlags ) {
            SimNode ** __restrict body = list;
        loopbegin:;
            for (; body!=tail; ++body) {
                (*body)->eval(context);
                DAS_PROCESS_LOOP_FLAGS(break);
            }
        }
    loopend:;
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

#if DAS_DEBUGGER

    vec4f SimNodeDebug_While::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        while ( cond->evalBool(context) && !context.stopFlags ) {
            SimNode ** __restrict body = list;
        loopbegin:;
            for (; body!=tail; ++body) {
                DAS_SINGLE_STEP(context,(*body)->debugInfo,true);
                (*body)->eval(context);
                DAS_PROCESS_LOOP_FLAGS(break);
            }
        }
    loopend:;
        evalFinal(context);
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

#endif

    // Return

    vec4f SimNode_Return::eval ( Context & context ) {
        DAS_PROFILE_NODE
        if ( subexpr ) context.abiResult() = subexpr->eval(context);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnNothing::eval ( Context & context ) {
        DAS_PROFILE_NODE
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnConst::eval ( Context & context ) {
        DAS_PROFILE_NODE
        context.abiResult() = value;
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnConstString::eval ( Context & context ) {
        DAS_PROFILE_NODE
        context.abiResult() = cast<char *>::from(value);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnRefAndEval::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pl = context.abiCopyOrMoveResult();
        DAS_ASSERT(pl);
        auto pR = ((char **)(context.stack.sp() + stackTop));
        *pR = pl;
        subexpr->evalPtr(context);;
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnAndCopy::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pr = subexpr->evalPtr(context);
        auto pl = context.abiCopyOrMoveResult();
        DAS_ASSERT(pl);
        memcpy ( pl, pr, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnAndMove::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pr = subexpr->evalPtr(context);
        auto pl = context.abiCopyOrMoveResult();
        DAS_ASSERT(pl);
        memcpy ( pl, pr, size);
        memset ( pr, 0, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnReference::eval ( Context & context ) {
        DAS_PROFILE_NODE
        char * ref = subexpr->evalPtr(context);
        if ( context.stack.bottom()<=ref && ref<context.stack.sp()) {
            context.throw_error_at(debugInfo,"reference bellow current function stack frame");
            return v_zero();
        }
#if DAS_ENABLE_STACK_WALK
        auto pp = (Prologue *) context.stack.sp();
        auto top = context.stack.sp() + pp->info->stackSize;
        if ( context.stack.sp()<=ref && ref<top ) {
            context.throw_error_at(debugInfo,"reference to current function stack frame");
            return v_zero();
        }
#endif
        context.abiResult() = cast<char *>::from(ref);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnRefAndEvalFromBlock::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto ba = (BlockArguments *) ( context.stack.sp() + argStackTop );
        auto pl = ba->copyOrMoveResult;
        DAS_ASSERT(pl);
        auto pR = ((char **)(context.stack.sp() + stackTop));
        *pR = pl;
        subexpr->evalPtr(context);;
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnAndCopyFromBlock::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pr = subexpr->evalPtr(context);
        auto ba = (BlockArguments *) ( context.stack.sp() + argStackTop );
        auto pl = ba->copyOrMoveResult;
        memcpy ( pl, pr, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnAndMoveFromBlock::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pr = subexpr->evalPtr(context);
        auto ba = (BlockArguments *) ( context.stack.sp() + argStackTop );
        auto pl = ba->copyOrMoveResult;
        memcpy ( pl, pr, size);
        memset ( pr, 0, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnReferenceFromBlock::eval ( Context & context ) {
        DAS_PROFILE_NODE
        char * ref = subexpr->evalPtr(context);
        if ( context.stack.bottom()<=ref && ref<context.stack.ap() ) {
            context.throw_error_at(debugInfo,"reference bellow current call chain stack frame");
            return v_zero();
        }
        context.abiResult() = cast<char *>::from(ref);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnLocalCMRes::eval ( Context & context ) {
        DAS_PROFILE_NODE
        SimNode ** __restrict tail = list + total;
        SimNode ** __restrict body = list;
        for (; body!=tail; ++body) {
            (*body)->eval(context);
            if (context.stopFlags) break;
        }
        context.abiResult() = cast<char *>::from(context.abiCopyOrMoveResult());
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    // Context

    static std::mutex g_DebugAgentMutex;
    static DebugAgentPtr g_DebugAgent;
    static unique_ptr<Context>   g_DebugAgentContext;
    static bool                  g_isInDebugAgentCreation;

    Context::Context(uint32_t stackSize, bool ph) : stack(stackSize) {
        code = make_shared<NodeAllocator>();
        constStringHeap = make_shared<ConstStringAllocator>();
        debugInfo = make_shared<DebugInfoAllocator>();
        ownStack = (stackSize != 0);
        persistent = ph;
        // register
        std::lock_guard<std::mutex> guard(g_DebugAgentMutex);
        if ( g_DebugAgent ) g_DebugAgent->onCreateContext(this);
    }

    Context::Context(const Context & ctx): stack(ctx.stack.size()) {
        persistent = ctx.persistent;
        code = ctx.code;
        constStringHeap = ctx.constStringHeap;
        debugInfo = ctx.debugInfo;
        thisProgram = ctx.thisProgram;
        thisHelper = ctx.thisHelper;
        ownStack = (ctx.stack.size() != 0);
        if ( persistent ) {
            heap = make_smart<PersistentHeapAllocator>();
            stringHeap = make_smart<PersistentStringAllocator>();
        } else {
            heap = make_smart<LinearHeapAllocator>();
            stringHeap = make_smart<LinearStringAllocator>();
        }
        // heap
        heap->setInitialSize(ctx.heap->getInitialSize());
        stringHeap->setInitialSize(ctx.stringHeap->getInitialSize());
        stringHeap->setIntern(ctx.stringHeap->isIntern());
        // globals
        annotationData = ctx.annotationData;
        globalsSize = ctx.globalsSize;
        globalInitStackSize = ctx.globalInitStackSize;
        globalVariables = ctx.globalVariables;
        totalVariables = ctx.totalVariables;
        if ( ctx.globals ) {
            globals = (char *) das_aligned_alloc16(globalsSize);
        }
        // shared
        sharedSize = ctx.sharedSize;
        shared = ctx.shared;
        sharedOwner = false;
        // functoins
        functions = ctx.functions;
        totalFunctions = ctx.totalFunctions;
        // mangled name table
        tabMnLookup = ctx.tabMnLookup;
        tabMnMask = ctx.tabMnMask;
        tabMnRot = ctx.tabMnRot;
        tabMnSize = ctx.tabMnSize;
        // global mangled name table
        tabGMnLookup = ctx.tabGMnLookup;
        tabGMnMask = ctx.tabGMnMask;
        tabGMnRot = ctx.tabGMnRot;
        tabGMnSize = ctx.tabGMnSize;
        // annotation data table
        tabAdLookup = ctx.tabAdLookup;
        tabAdMask = ctx.tabAdMask;
        tabAdRot = ctx.tabAdRot;
        tabAdSize = ctx.tabAdSize;
        // register
        std::lock_guard<std::mutex> guard(g_DebugAgentMutex);
        if ( g_DebugAgent ) g_DebugAgent->onCreateContext(this);
        // now, make it good to go
        restart();
        if ( stack.size() > globalInitStackSize ) {
            runInitScript();
        } else {
            auto ssz = max ( int(stack.size()), 16384 ) + globalInitStackSize;
            StackAllocator init_stack(ssz);
            SharedStackGuard init_guard(*this, init_stack);
            runInitScript();
        }
        restart();
    }

    Context::~Context() {
        // unregister
        {
            std::lock_guard<std::mutex> guard(g_DebugAgentMutex);
            if ( g_DebugAgent ) g_DebugAgent->onDestroyContext(this);
        }
        // and free memory
        if ( globals ) {
            das_aligned_free16(globals);
        }
        if ( shared && sharedOwner ) {
            das_aligned_free16(shared);
        }
    }

    uint64_t Context::getSharedMemorySize() const {
        uint64_t mem = 0;
        mem += code->totalAlignedMemoryAllocated();
        mem += constStringHeap->totalAlignedMemoryAllocated();
        mem += debugInfo->totalAlignedMemoryAllocated();
        return mem;
    }

    uint64_t Context::getUniqueMemorySize() const {
        uint64_t mem = 0;
        mem += globalsSize;
        mem += stack.size();
        mem += heap->totalAlignedMemoryAllocated();
        mem += stringHeap->totalAlignedMemoryAllocated();
        return mem;
    }

    struct SimNodeRelocator : SimVisitor {
        shared_ptr<NodeAllocator>   newCode;
        Context * context = nullptr;
        virtual SimNode * visit ( SimNode * node ) override {
            return node->copyNode(*context, newCode.get());
        }
    };

    void Context::relocateCode() {
        SimNodeRelocator rel;
        rel.context = this;
        rel.newCode = make_shared<NodeAllocator>();
        rel.newCode->prefixWithHeader = false;
        uint32_t codeSize = uint32_t(code->bytesAllocated()) - code->totalNodesAllocated * uint32_t(sizeof(NodePrefix));
        rel.newCode->setInitialSize(codeSize);
        if ( totalFunctions ) {
            SimFunction * newFunctions = (SimFunction *) rel.newCode->allocate(totalFunctions*sizeof(SimFunction));
            memcpy ( newFunctions, functions, totalFunctions*sizeof(SimFunction));
            for ( int i=0; i!=totalFunctions; ++i ) {
                newFunctions[i].name = rel.newCode->allocateName(functions[i].name);
                newFunctions[i].mangledName = rel.newCode->allocateName(functions[i].mangledName);
            }
            functions = newFunctions;
        }
        if ( totalVariables ) {
            GlobalVariable * newVariables = (GlobalVariable *) rel.newCode->allocate(totalVariables*sizeof(GlobalVariable));
            memcpy ( newVariables, globalVariables, totalVariables*sizeof(GlobalVariable));
            for ( int i=0; i!=totalVariables; ++i ) {
                newVariables[i].name = rel.newCode->allocateName(globalVariables[i].name);
            }
            globalVariables = newVariables;
        }
        // relocate functions
        for ( int i=0; i!=totalFunctions; ++i ) {
            auto & fn = functions[i];
            fn.code = fn.code->visit(rel);
        }
        // relocate variables
        if ( totalVariables ) {
            for ( int j=0; j!=totalVariables; ++j ) {
                auto & var = globalVariables[j];
                if ( var.init) {
                    var.init = var.init->visit(rel);
                }
            }
        }
        // relocate mangle-name lookup
        if ( tabMnLookup ) {
            uint32_t * newMnLookup = (uint32_t *) rel.newCode->allocate(tabMnSize * sizeof(uint32_t));
            memcpy ( newMnLookup, tabMnLookup, tabMnSize * sizeof(uint32_t));
            tabMnLookup = newMnLookup;
        }
        // relocate global mangle-name lookup
        if ( tabGMnLookup ) {
            uint32_t * newGMnLookup = (uint32_t *) rel.newCode->allocate(tabGMnSize * sizeof(uint32_t));
            memcpy ( newGMnLookup, tabGMnLookup, tabGMnSize * sizeof(uint32_t));
            tabGMnLookup = newGMnLookup;
        }
        // relocate annotation data lookup
        if ( tabAdLookup ) {
            uint64_t * newAdLookup = (uint64_t *) rel.newCode->allocate(tabAdSize * sizeof(uint64_t));
            memcpy ( newAdLookup, tabAdLookup, tabAdSize * sizeof(uint64_t));
            tabAdLookup = newAdLookup;
        }
        // swap the code
        DAS_ASSERTF(rel.newCode->depth()<=1,"after code relocation all code should be on one page");
        code = rel.newCode;
    }

    char * Context::intern(const char * str) {
        if ( !str ) return nullptr;
        uint32_t len = uint32_t(strlen(str));
        char * ist = constStringHeap->intern(str,len);
        if ( !ist ) ist = stringHeap->intern(str,len);
        return ist ? ist : stringHeap->allocateString(str,len);
    }

    class SharedDataWalker : public DataWalker {
    public:
        virtual void beforeArray ( Array * pa, TypeInfo * ) override {
            pa->shared = true;
        }
        virtual void beforeTable ( Table * pa, TypeInfo * ) override {
            pa->shared = true;
        }
    };

    void Context::runInitScript ( ) {
        DAS_ASSERTF(insideContext==0,"can't run init script on the locked context");
        char * EP, *SP;
        if(!stack.push(globalInitStackSize,EP,SP)) {
            throw_error("stack overflow in the initialization script");
            return;
        }
        vec4f args[2] = {
            cast<void *>::from(this),
            cast<bool>::from(sharedOwner)   // only init shared if we are the owner
        };
        abiArg = args;
        abiCMRES = nullptr;
        if ( aotInitScript ) {
            aotInitScript->eval(*this);
        } else {
#if DAS_ENABLE_STACK_WALK
            FuncInfo finfo;
            memset(&finfo, 0, sizeof(finfo));
            finfo.name = (char *) "Context::runInitScript";
            // TODO: init arguments?
#endif
            for ( int i=0; i!=totalVariables && !stopFlags; ++i ) {
                auto & pv = globalVariables[i];
                if ( pv.init ) {
                    if ( sharedOwner || !pv.shared ) {
#if DAS_ENABLE_STACK_WALK
                        finfo.stackSize = globalInitStackSize;
                        Prologue * pp = (Prologue *)stack.sp();
                        pp->info = &finfo;
                        pp->arguments = nullptr;    // TODO: args
                        pp->cmres = nullptr;
                        pp->line = &pv.init->debugInfo;
#endif
                        pv.init->eval(*this);
                    }
                } else {
                    memset ( globals + pv.offset, 0, pv.size );
                }
            }
            for ( int j=0; j!=totalFunctions && !stopFlags; ++j ) {
                auto & pf = functions[j];
                if ( pf.debugInfo->flags & FuncInfo::flag_init ) {
                    call(&pf, nullptr, 0);
                }

            }
        }
        abiArg = nullptr;
        stack.pop(EP,SP);
        // now, share the data
        if ( sharedOwner && shared ) {
            SharedDataWalker sdw;
            for ( int i=0; i!=totalVariables; ++i ) {
                auto & pv = globalVariables[i];
                if ( pv.init && pv.shared ) {
                    sdw.walk(shared + pv.offset, pv.debugInfo);
                }
            }
        }
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
        int candidates = 0;
        SimFunction * found = nullptr;
        for ( int fni = 0; fni != totalFunctions; ++fni ) {
            if ( strcmp(functions[fni].name, name)==0 ) {
                found = functions + fni;
                candidates++;
            }
        }
        isUnique = candidates == 1;
        return found;
    }

    int Context::findVariable ( const char * name ) const {
        for ( int vni = 0; vni != totalVariables; ++vni ) {
            if ( strcmp(globalVariables[vni].name, name)==0 ) {
                return vni;
            }
        }
        return -1;
    }

    void Context::stackWalk( const LineInfo * at, bool showArguments, bool showLocalVariables ) {
        auto str = getStackWalk(at, showArguments, showLocalVariables);
        to_out(str.c_str());
    }

    class StackWalkerTextWriter : public StackWalker {
    public:
        StackWalkerTextWriter ( TextWriter & tw, Context * ctx ) : ssw(tw), context(ctx) {}
        virtual bool canWalkArguments () override {
            return showArguments;
        }
        virtual bool canWalkVariables () override {
            return showLocalVariables;
        }
        virtual bool canWalkOutOfScopeVariables() override {
            return showOutOfScope;
        }
        virtual void onCallAOT ( Prologue *, const char * fileName ) override {
            ssw << fileName << ", AOT";
        }
        virtual void onCallAt ( Prologue *, FuncInfo * info, LineInfo * at ) override {
            ssw << info->name << " from " << at->describe();
        }
        virtual void onCall ( Prologue *, FuncInfo * info ) override {
            ssw << info->name;
        }
        virtual void onAfterPrologue ( Prologue * pp, char * SP ) override {
            ssw << "(sp=" << (context->stack.top() - SP)
                << ",sptr=0x" << HEX  << intptr_t(SP) << DEC;
            if ( pp->cmres ) {
                ssw << ",cmres=0x" << HEX << intptr_t(pp->cmres) << DEC;
            }
            ssw << ")\n";
        }
        virtual void onArgument ( FuncInfo * info, int i, VarInfo * field, vec4f arg ) override {
            ssw << "\t" << info->fields[i]->name
                << " : " << debug_type(field)
                << " = \t" << debug_value(arg, field, PrintFlags::stackwalker) << "\n";
        }
        virtual void onBeforeVariables ( ) override {
            ssw << "--> local variables\n";
        }
        virtual void onVariable ( FuncInfo *, LocalVariableInfo * lv, void * addr, bool inScope ) override {
            ssw << "\t" << lv->name
                << " : " << debug_type(lv);
            string location;
            if ( !inScope ) {
            } else if ( lv->cmres ) {
                location = "CMRES";
            } else if ( lv->isRefValue( ) ) {
                location = "ref *(sp + " + to_string(lv->stackTop) + ")";
            } else {
                location = "sp + " + to_string(lv->stackTop);
            }
            if ( addr ) {
                ssw << " = \t" << debug_value(addr, lv, PrintFlags::stackwalker)
                    << " at " << location
                    << " 0x" << HEX << intptr_t(addr) << DEC
                    << "\n";
            } else {
                if ( !inScope ) {
                    ssw << "\t// variable out of scope\n";
                } else {
                    ssw << "\t// variable was optimized out\n";
                }
            }
        }
        virtual bool onAfterCall ( Prologue * ) override {
            return !stackTopOnly;
         }
    public:
        bool showArguments = true;
        bool showLocalVariables = true;
        bool showOutOfScope = true;
        bool stackTopOnly = false;
    protected:
        TextWriter & ssw;
        Context * context;
    };

    string Context::getStackWalk ( const LineInfo * at, bool showArguments, bool showLocalVariables, bool showOutOfScope, bool stackTopOnly ) {
        FPE_DISABLE;
        TextWriter ssw;
    #if DAS_ENABLE_STACK_WALK
        ssw << "\n";
        if ( at ) {
            ssw << "from " << at->describe() << "\n";
        }
        char * sp = stack.ap();
        ssw << "CALL STACK (sp=" << (stack.top() - stack.ap())
            << ",sptr=0x" << HEX  << intptr_t(sp) << DEC << "):\n";
        auto walker = make_smart<StackWalkerTextWriter> ( ssw, this );
        walker->showArguments = showArguments;
        walker->showLocalVariables =  showLocalVariables;
        walker->showOutOfScope = showOutOfScope;
        walker->stackTopOnly = stackTopOnly;
        dapiStackWalk ( walker, *this, *at );
        ssw << "\n";
    #else
        ssw << "\nCALL STACK TRACKING DISABLED:\n\n";
    #endif
        return ssw.str();
    }

    void tickDebugAgent ( ) {
        std::lock_guard<std::mutex> guard(g_DebugAgentMutex);
        if ( g_DebugAgent ) g_DebugAgent->onTick();
    }

    void installDebugAgent ( DebugAgentPtr newAgent ) {
        std::lock_guard<std::mutex> guard(g_DebugAgentMutex);
        if ( g_DebugAgent ) g_DebugAgent->onUninstall(g_DebugAgent.get());
        g_DebugAgent = newAgent;
        if ( g_DebugAgent ) g_DebugAgent->onInstall(g_DebugAgent.get());
    }

    Context & getDebugAgentContext ( ) {
        return *g_DebugAgentContext;
    }

}

das::Context* get_clone_context( das::Context * ctx );//link time resolved dependencies

namespace das
{

    void forkDebugAgentContext ( Func exFn, Context * context, LineInfoArg * lineinfo ) {
        g_isInDebugAgentCreation = true;
        unique_ptr<Context> forkContext;
        bool realPersistent = context->persistent;
        context->persistent = true;
        forkContext.reset(get_clone_context(context));
        context->persistent = realPersistent;
        g_isInDebugAgentCreation = false;
        vec4f args[1];
        args[0] = cast<Context *>::from(context);
        SimFunction * fun = forkContext->getFunction(exFn.index-1);
        forkContext->callOrFastcall(fun, args, lineinfo);
        swap ( g_DebugAgentContext, forkContext );
    }

    bool isInDebugAgentCreation()
    {
        return g_isInDebugAgentCreation;
    }

    void shutdownDebugAgent() {
        installDebugAgent(nullptr);
        g_DebugAgentContext.reset();
    }

    void Context::breakPoint(const LineInfo & at) {
        if ( debugger ) {
            std::lock_guard<std::mutex> guard(g_DebugAgentMutex);
            if ( g_DebugAgent ) {
                g_DebugAgent->onBreakpoint(this, at);
                return;
            }
        }
        os_debug_break();
    }

    void Context::to_out ( const char * message ) {
        if (message) {
            fprintf(stdout, "%s", message);
            fflush(stdout);
        }
    }

    void Context::to_err ( const char * message ) {
        if (message) {
            fprintf(stderr, "%s", message);
            fflush(stderr);
        }
    }

    void Context::throw_error_at ( const LineInfo & at, const char * message, ... ) {
        const int PRINT_BUFFER_SIZE = 1024;
        char buffer[PRINT_BUFFER_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf (buffer,PRINT_BUFFER_SIZE,message, args);
        va_end (args);
        lastError = at.describe() + ": " + buffer;
        throw_error(lastError.c_str());
    }

    void Context::throw_error_ex ( const char * message, ... ) {
        const int PRINT_BUFFER_SIZE = 1024;
        char buffer[PRINT_BUFFER_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf (buffer,PRINT_BUFFER_SIZE,message, args);
        va_end (args);
        lastError = buffer;
        throw_error(lastError.c_str());
    }

    void Context::throw_error ( const char * message ) {
        exception = message;
#if DAS_ENABLE_EXCEPTIONS
        throw dasException(message ? message : "");
#else
        if ( throwBuf ) {
            longjmp(*throwBuf,1);
        } else {
            to_err("\nunhandled exception\n");
            if ( exception ) {
                to_err(exception);
                to_err("\n");
            }
            stackWalk(nullptr, false, false);
            os_debug_break();
        }
#endif
#if !defined(_MSC_VER) || (_MSC_VER>1900)
        exit(0);
#endif
    }

    void Context::rethrow () {
#if DAS_ENABLE_EXCEPTIONS
        throw dasException(exception ? exception : "");
#else
        if ( throwBuf ) {
            longjmp(*throwBuf,1);
        } else {
            to_err("\nunhandled exception\n");
            if ( exception ) {
                to_err(exception);
                to_err("\n");
            }
            stackWalk(nullptr, false, false);
            os_debug_break();
        }
#endif
#if !defined(_MSC_VER) || (_MSC_VER>1900)
        exit(0);
#endif
    }

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4611)
#endif

    vec4f Context::evalWithCatch ( SimNode * node ) {
        auto aa = abiArg;
        auto acm = abiCMRES;
        auto atba = abiThisBlockArg;
        char * EP, * SP;
        stack.watermark(EP,SP);
#if DAS_ENABLE_EXCEPTIONS
        try {
            return node->eval(*this);
        } catch ( const dasException & ) {
            /*
             to_err("\nunhandled exception\n");
             if ( exception ) {
             to_err(exception);
             to_err("\n");
             }
             stackWalk(nullptr, false, false);
             */
            abiArg = aa;
            abiCMRES = acm;
            abiThisBlockArg = atba;
            stack.pop(EP,SP);
            return v_zero();
        }
#else
        jmp_buf ev;
        jmp_buf * JB = throwBuf;
        throwBuf = &ev;
        if ( !setjmp(ev) ) {
            return node->eval(*this);
        } else {
            abiArg = aa;
            abiCMRES = acm;
            abiThisBlockArg = atba;
            stack.pop(EP,SP);
            throwBuf = JB;
            return v_zero();
        }
#endif
    }

    bool Context::runWithCatch ( const callable<void()> & subexpr ) {
        auto aa = abiArg;
        auto acm = abiCMRES;
        auto atba = abiThisBlockArg;
        char * EP, * SP;
        stack.watermark(EP,SP);
#if DAS_ENABLE_EXCEPTIONS
        try {
            subexpr();
            return true;
        } catch ( const dasException & ) {
            /*
             to_err("\nunhandled exception\n");
             if ( exception ) {
             to_err(exception);
             to_err("\n");
             }
             stackWalk(nullptr, false, false);
             */
            abiArg = aa;
            abiCMRES = acm;
            abiThisBlockArg = atba;
            stack.pop(EP,SP);
            return false;
        }
#else
        jmp_buf ev;
        jmp_buf * JB = throwBuf;
        throwBuf = &ev;
        if ( !setjmp(ev) ) {
            subexpr();
        } else {
            abiArg = aa;
            abiCMRES = acm;
            abiThisBlockArg = atba;
            stack.pop(EP,SP);
            throwBuf = JB;
            return false;
        }
        throwBuf = JB;
        return true;
#endif
    }

    vec4f Context::evalWithCatch ( SimFunction * fnPtr, vec4f * args, void * res ) {
        auto aa = abiArg;
        auto acm = abiCMRES;
        auto atba = abiThisBlockArg;
        char * EP, * SP;
        stack.watermark(EP,SP);
#if DAS_ENABLE_EXCEPTIONS
        try {
            return callWithCopyOnReturn(fnPtr, args, res, 0);
        } catch ( const dasException & ) {
            /*
            to_err("\nunhandled exception\n");
            if ( exception ) {
                to_err(exception);
                to_err("\n");
            }
            stackWalk(nullptr, false, false);
            */
            abiArg = aa;
            abiCMRES = acm;
            abiThisBlockArg = atba;
            stack.pop(EP,SP);
            return v_zero();
        }
#else
        jmp_buf ev;
        jmp_buf * JB = throwBuf;
        throwBuf = &ev;
        if ( !setjmp(ev) ) {
            return callWithCopyOnReturn(fnPtr, args, res, 0);
        } else {
            abiArg = aa;
            abiCMRES = acm;
            abiThisBlockArg = atba;
            stack.pop(EP,SP);
            throwBuf = JB;
            return v_zero();
        }
#endif
    }

    struct FileInfoCollector : SimVisitor {
        virtual void preVisit ( SimNode * node ) override {
            SimVisitor::preVisit(node);
            if ( auto fi = node->debugInfo.fileInfo ) {
                allFiles.insert(fi);
            }
        }
        das_hash_set<FileInfo *>    allFiles;
    };

    vector<FileInfo *> Context::getAllFiles() const {
        vector<FileInfo *> allFiles;
        FileInfoCollector collector;
        for ( int gvi=0; gvi!=totalVariables; ++gvi ) {
            const auto & gv = globalVariables[gvi];
            if ( gv.init ) gv.init->visit(collector);
        }
        for ( int fni=0; fni!=totalFunctions; ++fni ) {
            const auto & fn = functions[fni];
            if ( fn.code ) fn.code->visit(collector);
        }
        for ( auto & it : collector.allFiles ) {
            allFiles.push_back(it);
        }
        sort ( allFiles.begin(), allFiles.end(), [&]( FileInfo * a, FileInfo * b ){
            return a->name > b->name;
        });
        return allFiles;
    }

    void Context::resetProfiler() {
#if DAS_ENABLE_PROFILER
        auto allFiles = getAllFiles();
        for ( auto fi : allFiles ) {
            fi->profileData.clear();
        }
#endif
    }

    void Context::collectProfileInfo( TextWriter & tout ) {
#if DAS_ENABLE_PROFILER
        uint64_t totalGoo = 0;
        auto allFiles = getAllFiles();
        for ( auto info : allFiles ) {
            for ( auto counter : info->profileData ) {
                totalGoo += counter;
            }
        }
        tout << "\nPROFILING RESULTS:\n";
        for ( auto fi : allFiles ) {
            tout << fi->name << "\n";
            bool newLine = true;
            int  line = 0;
            char txt[2];
            txt[1] = 0;
            int col = 0;
            for ( uint32_t i = 0; i!=fi->sourceLength; ++i ) {
                if ( newLine ) {
                    line ++;
                    col = 0;
                    newLine = false;
                    char total[20];
                    if ( fi->profileData.size()>size_t(line) && fi->profileData[line] ) {
                        uint64_t samples = fi->profileData[line];
                        snprintf(total, 20, "%-6.2f", samples*100.1/totalGoo);
                        tout << total;
                    } else {
                        tout << "      ";
                    }
                }
                txt[0] = fi->source[i];
                if (txt[0] == '\n') {
                    newLine = true;
                }
                if (txt[0] == '\t') {
                    if (col % 4 == 0) {
                        tout << "    ";
                        col += 4;
                    } else {
                        while (col % 4) {
                            tout << " ";
                            col++;
                        }
                    }
                } else {
                    tout << txt;
                    col++;
                }
            }
        }

#else
        tout << "\nPROFILER IS DISABLED\n";
#endif
    }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    string getLinesAroundCode ( const char* st, int ROW, int TAB ) {
        TextWriter text;
        int col=0, row=1;
        auto it = st;
        while ( *it ) {
            auto CH = *it++;
            if ( CH=='\t' ) {
                int tcol = (col + TAB) & ~(TAB-1);
                while ( col < tcol ) {
                    if ( row>=ROW-3 && row<=ROW+3 ) text << " ";
                    col ++;
                }
                continue;
            } else if ( CH=='\n' ) {
                row++;
                col=0;
                if ( row>=ROW-3 && row<=ROW+3 ) {
                    text << ((row==ROW) ? "\n->  " : "\n    ");
                }
            } else {
                if ( row>=ROW-3 && row<=ROW+3 ) text << CH;
            }
            col ++;
        }
        return text.str();
    }

    void Context::bpcallback( const LineInfo & at ) {
        std::lock_guard<std::mutex> guard(g_DebugAgentMutex);
        if ( g_DebugAgent ) g_DebugAgent->onSingleStep(this, at);
    }

}

//workaround compiler bug in MSVC 32 bit
#if defined(_MSC_VER) && !defined(__clang__) && INTPTR_MAX == INT32_MAX
vec4i VECTORCALL v_splats_ptr(const void * a) {return v_splatsi((int32_t)a);}
#endif
