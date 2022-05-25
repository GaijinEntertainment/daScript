#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/debug_print.h"
#include "daScript/misc/fpe.h"
#include "daScript/misc/debug_break.h"

#include <stdarg.h>

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

    DAS_THREAD_LOCAL StackAllocator *SharedStackGuard::lastContextStack = nullptr;

    SimNode * SimNode::copyNode ( Context &, NodeAllocator * code ) {
        auto prefix = ((NodePrefix *)this) - 1;
#ifndef NDEBUG
        DAS_ASSERTF(prefix->magic==0xdeadc0de,"node was allocated on the heap without prefix");
#endif
        char * newNode;
        if ( code->prefixWithHeader ) {
            newNode = code->allocate(prefix->size + sizeof(NodePrefix));
            memcpy ( newNode, ((char *)this) - sizeof(NodePrefix), sizeof(NodePrefix));
            newNode += sizeof(NodePrefix);
        } else {
            newNode = code->allocate(prefix->size);
        }
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

    vec4f SimNode_Jit::eval ( Context & context ) {
        auto result = func(&context, context.abiArg, context.abiCMRES);
        context.result = result;
        return result;
    }

    vec4f SimNode_NOP::eval ( Context & ) {
        return v_zero();
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

    vec4f SimNode_DeleteClassPtr::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto pStruct = (char **) subexpr->evalPtr(context);
        pStruct = pStruct + total - 1;
        auto sizeOf = sizeexpr->evalInt(context);
        for ( uint32_t i=0; i!=total; ++i, pStruct-- ) {
            if ( *pStruct ) {
                context.heap->free(*pStruct, sizeOf);
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
                SimFunction ** fnMnh = (SimFunction **) pLambda->capture;
                SimFunction * simFunc = fnMnh[1];
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
                context.last_exception = context.exception;
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
                context.last_exception = context.exception;
                context.exception = nullptr;
                catch_block->eval(context);
            }
            context.throwBuf = JB;
        #endif
        return v_zero();
    }

#if DAS_DEBUGGER
    vec4f SimNodeDebug_TryCatch::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto aa = context.abiArg; auto acm = context.abiCMRES;
        char * EP, * SP;
        context.stack.watermark(EP,SP);
        #if DAS_ENABLE_EXCEPTIONS
            try {
                DAS_SINGLE_STEP(context,try_block->debugInfo,false);
                try_block->eval(context);
            } catch ( const dasException & ) {
                context.abiArg = aa;
                context.abiCMRES = acm;
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
            if ( !setjmp(ev) ) {
                DAS_SINGLE_STEP(context,try_block->debugInfo,false);
                try_block->eval(context);
            } else {
                context.throwBuf = JB;
                context.abiArg = aa;
                context.abiCMRES = acm;
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
        if ( pl != pr ) {
            memcpy ( pl, pr, size );
            memset ( pr, 0, size );
        }
        return v_zero();
    }

    // SimNode_ForBase

    void SimNode_ForBase::allocateFor ( NodeAllocator * code, uint32_t t ) {
        totalSources = t;
        auto bytes = code->allocate( totalSources * ( sizeof(SimNode*) + sizeof(uint32_t)*2 ) );
        sources = (SimNode **) (bytes);
        strides = (uint32_t *) (bytes + totalSources * sizeof(SimNode *));
        stackTop = (uint32_t *) (bytes + totalSources * sizeof(SimNode *) + totalSources * sizeof(uint32_t));
    }

    SimNode * SimNode_ForBase::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_ForBase * that = (SimNode_ForBase *) SimNode_Block::copyNode(context, code);
        if ( totalSources ) {
            auto bytes = code->allocate( totalSources * ( sizeof(SimNode*) + sizeof(uint32_t)*2 ) );
            auto newSources = (SimNode **) (bytes);
            memcpy ( newSources, that->sources, totalSources*sizeof(SimNode *));
            that->sources = newSources;
            auto newStrides = (uint32_t *) (bytes + totalSources * sizeof(SimNode *));
            memcpy( newStrides, that->strides, totalSources*sizeof(uint32_t));
            that->strides = newStrides;
            auto newStackTop = (uint32_t *) (bytes + totalSources * sizeof(SimNode *) + totalSources * sizeof(uint32_t));
            memcpy ( newStackTop, that->stackTop, totalSources * sizeof(uint32_t));
            that->stackTop = newStackTop;
        } else {
            sources = nullptr;
            strides = nullptr;
            stackTop = nullptr;
        }
        return that;
    }

    // SimNode_ForWithIterator

    void SimNode_ForWithIteratorBase::allocateFor ( NodeAllocator * code, uint32_t t ) {
        totalSources = t;
        auto bytes = code->allocate( totalSources * ( sizeof(SimNode*) + sizeof(uint32_t) ) );
        source_iterators = (SimNode **) (bytes);
        stackTop = (uint32_t *) (bytes + totalSources * sizeof(SimNode *));
    }

    SimNode * SimNode_ForWithIteratorBase::copyNode ( Context & context, NodeAllocator * code ) {
        SimNode_ForWithIteratorBase * that = (SimNode_ForWithIteratorBase *) SimNode_Block::copyNode(context, code);
        if ( totalSources ) {
            auto bytes = code->allocate( totalSources * ( sizeof(SimNode*) + sizeof(uint32_t) ) );
            auto new_source_iterators = (SimNode **) (bytes);
            memcpy ( new_source_iterators, that->source_iterators, totalSources*sizeof(SimNode *));
            that->source_iterators = new_source_iterators;
            auto newStackTop = (uint32_t *) (bytes + totalSources * sizeof(SimNode *));
            memcpy ( newStackTop, that->stackTop, totalSources * sizeof(uint32_t));
            that->stackTop = newStackTop;
        } else {
            source_iterators = nullptr;
            stackTop = nullptr;
        }
        return that;
    }

    vec4f SimNode_ForWithIteratorBase::eval ( Context & context ) {
        // note: this is the 'slow' version, to which we fall back when there are too many sources
        DAS_PROFILE_NODE
        int totalCount = int(totalSources);
        vector<char *> pi(totalCount);
        for ( int t=0; t!=totalCount; ++t ) {
            pi[t] = context.stack.sp() + stackTop[t];
        }
        vector<Iterator *> sources(totalCount);
        for ( int t=0; t!=totalCount; ++t ) {
            vec4f ll = source_iterators[t]->eval(context);
            sources[t] = cast<Iterator *>::to(ll);
        }
        bool needLoop = true;
        SimNode ** __restrict tail = list + total;
        for ( int t=0; t!=totalCount; ++t ) {
            sources[t]->isOpen = true;
            needLoop = sources[t]->first(context, pi[t]) && needLoop;
            if ( context.stopFlags ) goto loopend;
        }
        if ( !needLoop ) goto loopend;
        for ( int i=0; !context.stopFlags; ++i ) {
            SimNode ** __restrict body = list;
        loopbegin:;
            for (; body!=tail; ++body) {
                (*body)->eval(context);
                DAS_PROCESS_LOOP_FLAGS(break);
            }
            for ( int t=0; t!=totalCount; ++t ){
                if ( !sources[t]->next(context, pi[t]) ) goto loopend;
                if ( context.stopFlags ) goto loopend;
            }
        }
    loopend:
        evalFinal(context);
        for ( int t=0; t!=totalCount; ++t ) {
            sources[t]->close(context, pi[t]);
        }
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

#if DAS_DEBUGGER

    vec4f SimNodeDebug_ForWithIteratorBase::eval ( Context & context ) {
        DAS_PROFILE_NODE
        auto totalCount = int(totalSources);
        vector<char *> pi(totalCount);
        for ( int t=0; t!=totalCount; ++t ) {
            pi[t] = context.stack.sp() + this->stackTop[t];
        }
        vector<Iterator *> sources(totalCount);
        for ( int t=0; t!=totalCount; ++t ) {
            vec4f ll = this->source_iterators[t]->eval(context);
            sources[t] = cast<Iterator *>::to(ll);
        }
        bool needLoop = true;
        SimNode ** __restrict tail = this-> list + this->total;
        for ( int t=0; t!=totalCount; ++t ) {
            sources[t]->isOpen = true;
            needLoop = sources[t]->first(context, pi[t]) && needLoop;
            if ( context.stopFlags ) goto loopend;
        }
        if ( !needLoop ) goto loopend;
        for ( int i=0; !context.stopFlags; ++i ) {
            SimNode ** __restrict body = this->list;
        loopbegin:;
            for (; body!=tail; ++body) {
                DAS_SINGLE_STEP(context,(*body)->debugInfo,true);
                (*body)->eval(context);
                DAS_PROCESS_LOOP_FLAGS(break);
            }
            for ( int t=0; t!=totalCount; ++t ){
                if ( !sources[t]->next(context, pi[t]) ) goto loopend;
                if ( context.stopFlags ) goto loopend;
            }
        }
    loopend:
        this->evalFinal(context);
        for ( int t=0; t!=totalCount; ++t ) {
            sources[t]->close(context, pi[t]);
        }
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }

#endif

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
            that->fnPtr = context.fnByMangledName(fnPtr->mangledNameHash);
            // printf("CALL %p -> %p\n", fnPtr, fnPtr );
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
        evalFinalSingleStep(context);
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
        evalFinalSingleStep(context);
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
        evalFinalSingleStep(context);
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
        evalFinalSingleStep(context);
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
        if ( pl != pr ) {
            memcpy ( pl, pr, size);
            memset ( pr, 0, size);
        }
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
        if ( pl != pr ) {
            memcpy ( pl, pr, size);
            memset ( pr, 0, size);
        }
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

    struct DebugAgentInstance {
        DebugAgentPtr   debugAgent;
        ContextPtr      debugAgentContext;
    };

    static std::recursive_mutex g_DebugAgentMutex;
    static das_safe_map<string, DebugAgentInstance>   g_DebugAgents;
    static DAS_THREAD_LOCAL bool g_isInDebugAgentCreation = false;

    template <typename TT>
    void for_each_debug_agent ( const TT & lmbd ) {
        std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
        for ( auto & it : g_DebugAgents ) {
            lmbd ( it.second.debugAgent );
        }
    }

    template <typename TT>
    void for_each_debug_agent_pair ( const TT & lmbd ) {
        std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
        for ( auto & it : g_DebugAgents ) {
            lmbd ( it.first, it.second.debugAgent );
        }
    }

    void dapiReportContextState ( Context & ctx, const char * category, const char * name, const TypeInfo * info, void * data ) {
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onVariable(&ctx,category,name,(TypeInfo*)info,data);
        });
    }

    Context::Context(uint32_t stackSize, bool ph) : stack(stackSize) {
        code = make_shared<NodeAllocator>();
        constStringHeap = make_shared<ConstStringAllocator>();
        debugInfo = make_shared<DebugInfoAllocator>();
        ownStack = (stackSize != 0);
        persistent = ph;
        for_each_debug_agent([&]( const DebugAgentPtr & pAgent ){
            pAgent->onCreateContext(this);
        });
    }

    void Context::strip() {
        stringHeap.reset();
        heap.reset();
        stack.strip();
        if ( globals && globalsOwner ) {
            das_aligned_free16(globals);
            globals = nullptr;
        }
        if ( shared && sharedOwner ) {
            das_aligned_free16(shared);
            shared = nullptr;
        }
    }

    void Context::logMemInfo(TextWriter & tw) {
        uint64_t bytesTotal = 0, bytesUsed = 0;
    // context
        tw << "Context: 0x" << HEX << intptr_t(this) << DEC << " '" << name << "' use_count = " << use_count() << "\n";
        bytesTotal = bytesUsed = sizeof(Context);
    // stringHeap
        if ( stringHeap ) {
            tw << "\tstringHeap: " << stringHeap->bytesAllocated() << " of " << stringHeap->totalAlignedMemoryAllocated()
                << ", depth = " << stringHeap->depth() << "\n";
            bytesTotal += stringHeap->totalAlignedMemoryAllocated();
            bytesUsed += stringHeap->bytesAllocated();
        }
    // constStringHeap
        if ( constStringHeap ) {
            tw << "\tconstStringHeap: " << constStringHeap->bytesAllocated() << " of " << constStringHeap->totalAlignedMemoryAllocated()
                << ", depth = " << constStringHeap->depth() << "\n";
            bytesTotal += constStringHeap->totalAlignedMemoryAllocated();
            bytesUsed += constStringHeap->bytesAllocated();
        }
    // heap
        if ( heap ) {
            tw << "\theap: " << heap->bytesAllocated() << " of " << heap->totalAlignedMemoryAllocated()
                << ", depth = " << heap->depth() << "\n";
            bytesTotal += heap->totalAlignedMemoryAllocated();
            bytesUsed += heap->bytesAllocated();
        }
    // code
        if ( code ) {
            tw << "\tcode: " << code->bytesAllocated() << " of " << code->totalAlignedMemoryAllocated()
                << ", depth = " << code->depth() << "\n";
            tw << "\t\ttableMN[" << tabMnLookup.size() << "]\n";
            tw << "\t\ttableGMN[" << tabGMnLookup.size() << "]\n";
            tw << "\t\ttableAd[" << tabAdLookup.size() << "]\n";
            int aotf = 0;
            for ( int i=0; i!=totalFunctions; ++i ) {
                if ( functions[i].aotFunction ) aotf++;
            }
            if ( aotf>0 ) {
                int64_t saot = int64_t(sizeof(SimNode_CallBase));
                tw << "\tAOT " << aotf << " of " << totalFunctions << ", " << aotf << " x sizeof(SimNode_Aot = " << saot << ") = " << aotf*saot << "\n";
            }
            bytesTotal += code->totalAlignedMemoryAllocated();
            bytesUsed += code->bytesAllocated();
        }
    // debugInfo
        if ( debugInfo ) {
            tw << "\tdebugInfo: " << debugInfo->bytesAllocated() << " of " << debugInfo->totalAlignedMemoryAllocated()
                << ", depth = " << debugInfo->depth() << "\n";
            bytesTotal += debugInfo->totalAlignedMemoryAllocated();
            bytesUsed += debugInfo->bytesAllocated();
        }
    // stack
        if ( stack.bottom() ) {
            tw << "\tstack: " << stack.size() << "\n";
            bytesTotal += stack.size();
            bytesUsed += stack.size();
        }
    // functions
        //tw << "\functions table: " << totalFunctions*sizeof(SimFunction) << "\n";
        //bytesTotal += totalFunctions*sizeof(SimFunction);
        //bytesUsed += totalFunctions*sizeof(SimFunction);
    // globals
        //tw << "\tglobals table: " << totalVariables*sizeof(GlobalVariable) << "\n";
        //bytesTotal += totalVariables*sizeof(GlobalVariable);
        //bytesUsed += totalVariables*sizeof(GlobalVariable);
    // globals data
        if ( globals ) {
            tw << "\tglobal data: " << globalsSize << "\n";
            bytesTotal += globalsSize;
            bytesUsed += globalsSize;
        }
    // shared
        if ( shared ) {
            tw << "\tshared data: " << sharedSize << "\n";
            bytesTotal += sharedSize;
            bytesUsed += sharedSize;
        }
    // total
        tw << "-----------------------------------------------------------------\n";
        tw << "  total " << bytesTotal << ", wasted "
            << ((bytesTotal - bytesUsed)*100/bytesTotal) << "% ("
            << (bytesTotal - bytesUsed) << ")\n";
    }

    void Context::makeWorkerFor(const Context & ctx)
    {
        if (code == ctx.code)
            return;

        code = ctx.code;
        constStringHeap = ctx.constStringHeap;
        debugInfo = ctx.debugInfo;
        thisProgram = ctx.thisProgram;
        thisHelper = ctx.thisHelper;
        category.value = ctx.category.value;

        // globals (on condition that all context globals are read-only)
        globals = ctx.globals;
        globalsOwner = false;
        annotationData = ctx.annotationData;
        globalsSize = ctx.globalsSize;
        globalInitStackSize = ctx.globalInitStackSize;
        globalVariables = ctx.globalVariables;
        totalVariables = ctx.totalVariables;

        // shared
        sharedSize = ctx.sharedSize;
        shared = ctx.shared;
        sharedOwner = false;
        // functions
        functions = ctx.functions;
        totalFunctions = ctx.totalFunctions;

        // mangled name table
        tabMnLookup = ctx.tabMnLookup;
        tabGMnLookup = ctx.tabGMnLookup;
        tabAdLookup = ctx.tabAdLookup;
    }

    uint64_t Context::getSharedMemorySize() const {
        uint64_t mem = 0;
        mem += code ? code->totalAlignedMemoryAllocated() : 0;
        mem += constStringHeap ? constStringHeap->totalAlignedMemoryAllocated() : 0;
        mem += debugInfo ? debugInfo->totalAlignedMemoryAllocated() : 0;
        mem += sharedSize;
        return mem;
    }

    uint64_t Context::getUniqueMemorySize() const {
        uint64_t mem = 0;
        mem += globalsSize;
        mem += stack.size();
        mem += heap ? heap->totalAlignedMemoryAllocated() : 0;
        mem += stringHeap ? stringHeap->totalAlignedMemoryAllocated() : 0;
        return mem;
    }


    Context::Context(const Context & ctx, uint32_t category_): stack(ctx.stack.size()) {
        persistent = ctx.persistent;
        code = ctx.code;
        constStringHeap = ctx.constStringHeap;
        debugInfo = ctx.debugInfo;
        thisProgram = ctx.thisProgram;
        thisHelper = ctx.thisHelper;
        name = "clone of " + ctx.name;
        category.value = category_;
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
        tabGMnLookup = ctx.tabGMnLookup;
        tabAdLookup = ctx.tabAdLookup;
        // register
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onCreateContext(this);
        });
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
        category.value |= uint32_t(ContextCategory::dead);
        // register
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onDestroyContext(this);
        });
        // shutdown
        runShutdownScript();
        // and free memory
        if ( globals && globalsOwner ) {
            das_aligned_free16(globals);
        }
        if ( shared && sharedOwner ) {
            das_aligned_free16(shared);
        }
    }

    struct SimNodeRelocator : SimVisitor {
        shared_ptr<NodeAllocator>   newCode;
        Context * context = nullptr;
        int totalNodes = 0;
        virtual SimNode * visit ( SimNode * node ) override {
            totalNodes ++;
            return node->copyNode(*context, newCode.get());
        }
    };

    void Context::relocateCode( bool pwh ) {
        SimNodeRelocator rel;
        rel.context = this;
        rel.newCode = make_shared<NodeAllocator>();
        rel.newCode->customGrow = [&](int ) { return 4000; };   // because SimNode_Aot is 80 bytes
        uint32_t codeSize = uint32_t(code->bytesAllocated());
        if ( code->prefixWithHeader && !pwh ) {
            // printf("[REL] %i adjusting\n", code->totalNodesAllocated);
            codeSize -= code->totalNodesAllocated * uint32_t(sizeof(NodePrefix));
        } else {
            // printf("[REL] %i not adjusting\n", code->totalNodesAllocated);
        }
        rel.newCode->prefixWithHeader = pwh;
        rel.newCode->setInitialSize(codeSize);
        SimFunction * oldFunctions = functions;
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
        // relocate mangle-name lookup
        for ( auto & kv : tabMnLookup ) {
            auto fn = kv.second;
            if ( fn!=nullptr ) {
                if ( fn>=oldFunctions && fn<(oldFunctions+totalFunctions) ) {
                    ptrdiff_t index = fn - oldFunctions;
                    kv.second = functions + index;
                    DAS_ASSERT(fn->mangledNameHash == kv.second->mangledNameHash);
                    DAS_ASSERT(kv.second>=functions && kv.second<(functions+totalFunctions));
                    // printf("%3i - MNH 0x%8x: %s [move %p -> %p]\n", i, fn->mangledNameHash, fn->name, fn, kv.second );
                }
            }
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
        // relocate functions
        for ( int i=0; i!=totalFunctions; ++i ) {
            auto & fn = functions[i];
            fn.code = fn.code->visit(rel);
        }
        // swap the code
        rel.newCode->totalNodesAllocated = rel.totalNodes;
        // we need small repro of this happening. disabling the assert until i find one
        // DAS_ASSERTF(rel.newCode->depth()<=1,"after code relocation all code should be on one page");
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
        if ( false && aotInitScript ) {
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
        }
        abiArg = nullptr;
        stack.pop(EP,SP);
        if ( !aotInitScript ) {
            for ( int j=0; j!=totalFunctions && !stopFlags; ++j ) {
                auto & pf = functions[j];
                if ( pf.debugInfo->flags & FuncInfo::flag_init ) {
                    callOrFastcall(&pf, nullptr, 0);
                }

            }
        }
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

    bool Context::runShutdownScript ( ) {
        DAS_ASSERTF(insideContext==0,"can't run init script on the locked context");
        if ( shutdown ) return false;
        shutdown = true;
        return runWithCatch([&](){
            for ( int j=0; j!=totalFunctions && !stopFlags; ++j ) {
                auto & pf = functions[j];
                if ( pf.debugInfo->flags & FuncInfo::flag_shutdown ) {
                    callOrFastcall(&pf, nullptr, 0);
                }
            }
        });
    }

    vector<SimFunction *> Context::findFunctions ( const char * fnname ) const {
        vector<SimFunction *> res;
        for ( auto & kv : tabMnLookup ) {
            auto fn = kv.second;
            if ( fn!=nullptr && strcmp(fn->name, fnname)==0 ) {
                res.push_back(fn);
            }
        }
        return res;
    }

    SimFunction * Context::findFunction ( const char * fnname ) const {
        for ( auto & kv : tabMnLookup ) {
            auto fn = kv.second;
            if ( fn!=nullptr && strcmp(fn->name, fnname)==0 ) {
                return fn;
            }
        }
        return nullptr;
    }

    SimFunction * Context::findFunction ( const char * fnname, bool & isUnique ) const {
        int candidates = 0;
        SimFunction * found = nullptr;
        for ( auto & kv : tabMnLookup ) {
            auto fn = kv.second;
            if ( fn!=nullptr && strcmp(fn->name, fnname)==0 ) {
                found = fn;
                candidates++;
            }
        }
        isUnique = candidates == 1;
        return found;
    }

    int Context::findVariable ( const char * fnname ) const {
        for ( int vni = 0; vni != totalVariables; ++vni ) {
            if ( strcmp(globalVariables[vni].name, fnname)==0 ) {
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
        dapiStackWalk ( walker, *this, at ? *at : LineInfo() );
        ssw << "\n";
    #else
        ssw << "\nCALL STACK TRACKING DISABLED:\n\n";
    #endif
        return ssw.str();
    }

    void tickSpecificDebugAgent ( const char * name ) {
        std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
        auto it = g_DebugAgents.find(name);
        if ( it != g_DebugAgents.end() ) {
            it->second.debugAgent->onTick();
        }
    }

    void tickDebugAgent ( ) {
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onTick();
        });
    }

    void collectDebugAgentState ( Context & ctx ) {
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onCollect( &ctx );
        });
    }

    bool multiline_log = true;

    void toLog ( int level, const char * text ) {
        bool any = false;
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            any |= pAgent->onLog(int(level), text);
        });
        if ( !any ) {
            const char * marker = "";

            if ( level >= LogLevel::error )
                marker = "[E] ";
            else if ( level >= LogLevel::warning )
                marker = "[W] ";
            else if ( level >= LogLevel::info )
                marker = "[I] ";
            else if ( level >= LogLevel::debug )
                marker = "";
            else if ( level >= LogLevel::trace )
                marker = "";

            if ( level>=LogLevel::warning ) {
                fprintf(stderr, multiline_log ? "%s%s\n" : "%s%s", marker, text);
                fflush(stderr);
            } else {
                fprintf(stdout, multiline_log ? "%s%s\n" : "%s%s", marker, text);
                fflush(stdout);
            }
        }
    }

    void installDebugAgent ( DebugAgentPtr newAgent, const char * category, LineInfoArg * at, Context * context ) {
        if ( !category ) context->throw_error_at(*at, "need to specify category");
        std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
        auto it = g_DebugAgents.find(category);
        if ( it != g_DebugAgents.end() ) {
            DebugAgent * oldAgentPtr = it->second.debugAgent.get();
            for ( auto & ap : g_DebugAgents ) {
                ap.second.debugAgent->onUninstall(oldAgentPtr);
            }
        }
        g_DebugAgents[category] = {
            newAgent,
            context->shared_from_this()
        };
        DebugAgent * newAgentPtr = newAgent.get();
        for ( auto & ap : g_DebugAgents ) {
            ap.second.debugAgent->onInstall(newAgentPtr);
        }
    }

    Context & getDebugAgentContext ( const char * category, LineInfoArg * at, Context * context ) {
        if ( !category ) context->throw_error_at(*at, "need to specify category");
        std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
        auto it = g_DebugAgents.find(category);
        if ( it == g_DebugAgents.end() ) context->throw_error_at(*at, "can't get debug agent '%s'", category);
        return *it->second.debugAgentContext;
    }

    bool hasDebugAgentContext ( const char * category, LineInfoArg * at, Context * context ) {
        if ( !category ) context->throw_error_at(*at, "need to specify category");
        std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
        auto it = g_DebugAgents.find(category);
        return it != g_DebugAgents.end();
    }

    void lockDebugAgent ( const TBlock<void> & blk, Context * context, LineInfoArg * line ) {
        std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
        context->invoke(blk, nullptr, nullptr, line);
    }
}

das::Context* get_clone_context( das::Context * ctx, uint32_t category );//link time resolved dependencies

namespace das
{

    void forkDebugAgentContext ( Func exFn, Context * context, LineInfoArg * lineinfo ) {
        g_isInDebugAgentCreation = true;
        shared_ptr<Context> forkContext;
        bool realPersistent = context->persistent;
        context->persistent = true;
        forkContext.reset(get_clone_context(context, uint32_t(ContextCategory::debug_context)));
        context->persistent = realPersistent;
        g_isInDebugAgentCreation = false;
        vec4f args[1];
        args[0] = cast<Context *>::from(context);
        SimFunction * fun = exFn.PTR;
        forkContext->callOrFastcall(fun, args, lineinfo);
    }

    bool isInDebugAgentCreation() {
        return g_isInDebugAgentCreation;
    }

    void shutdownDebugAgent() {
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
           for ( auto & ap : g_DebugAgents ) {
               ap.second.debugAgent->onUninstall(pAgent.get());
           }
        });
        das_safe_map<string,DebugAgentInstance> agents;
        {
            std::lock_guard<std::recursive_mutex> guard(g_DebugAgentMutex);
            swap(agents, g_DebugAgents);
        }
    }

    void Context::triggerHwBreakpoint ( void * addr, int index ) {
        singleStepMode = true;
        hwBpAddress = addr;
        hwBpIndex = index;
    }

    void Context::breakPoint(const LineInfo & at, const char * reason, const char * text) {
        if ( debugger ) {
            bool any = false;
            for_each_debug_agent([&](const DebugAgentPtr & pAgent){
                pAgent->onBreakpoint(this, at, reason, text);
                any = true;
            });
            if ( any ) return;
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
        const int PRINT_BUFFER_SIZE = 8192;
        char buffer[PRINT_BUFFER_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf (buffer,PRINT_BUFFER_SIZE,message, args);
        va_end (args);
        throw_fatal_error(buffer, at);
    }

    void Context::throw_error_ex ( const char * message, ... ) {
        const int PRINT_BUFFER_SIZE = 8192;
        char buffer[PRINT_BUFFER_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf (buffer,PRINT_BUFFER_SIZE,message, args);
        va_end (args);
        throw_fatal_error(buffer, LineInfo());
    }

    void Context::throw_error ( const char * message ) {
        throw_fatal_error(message, LineInfo());
    }

    void Context::throw_fatal_error ( const char * message, const LineInfo & at ) {
        exceptionMessage = message;
        exception = exceptionMessage.c_str();
        exceptionAt = at;
#if DAS_ENABLE_EXCEPTIONS
        if ( breakOnException ) breakPoint(at, "exception", message);
        throw dasException(message ? message : "", at);
#else
        if ( throwBuf ) {
            if ( alwaysStackWalkOnException ) stackWalk(nullptr, false, false);
            if ( breakOnException ) breakPoint(at, "exception", message);
            longjmp(*throwBuf,1);
        } else {
            to_err("\nunhandled exception\n");
            if ( exception ) {
                string msg = exceptionAt.describe() + ": " + exception;
                to_err(msg.c_str());
                to_err("\n");
            }
            stackWalk(nullptr, false, false);
            breakPoint(at, "exception", message);
        }
#endif
#if !defined(_MSC_VER) || (_MSC_VER>1900)
        exit(0);
#endif
    }

    void Context::rethrow () {
#if DAS_ENABLE_EXCEPTIONS
        throw dasException(exception ? exception : "", exceptionAt);
#else
        if ( throwBuf ) {
            longjmp(*throwBuf,1);
        } else {
            to_err("\nunhandled exception\n");
            if ( exception ) {
                string msg = exceptionAt.describe() + ": " + exception;
                to_err(msg.c_str());
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
        runVisitor(&collector);
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

    void Context::instrumentCallback ( const LineInfo & at ) {
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onInstrument(this, at);
        });
    }

    void Context::instrumentFunctionCallback ( SimFunction * sim, bool entering ) {
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onInstrumentFunction(this, sim, entering);
        });
    }

    void Context::bpcallback( const LineInfo & at ) {
        for_each_debug_agent([&](const DebugAgentPtr & pAgent){
            pAgent->onSingleStep(this, at);
        });
    }

    void Context::runVisitor ( SimVisitor * vis ) const {
        for ( int gvi=0; gvi!=totalVariables; ++gvi ) {
            const auto & gv = globalVariables[gvi];
            if ( gv.init ) gv.init->visit(*vis);
        }
        for ( int fni=0; fni!=totalFunctions; ++fni ) {
            const auto & fn = functions[fni];
            if ( fn.code ) fn.code->visit(*vis);
        }
    }
}

//workaround compiler bug in MSVC 32 bit
#if defined(_MSC_VER) && !defined(__clang__) && INTPTR_MAX == INT32_MAX
vec4i VECTORCALL v_ldu_ptr(const void * a) {return v_seti_x((int32_t)a);}
#endif
