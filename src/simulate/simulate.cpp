#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/runtime_string.h"

// this is here for the default implementation of to_out and to_err
#include <iostream>

namespace das
{
    bool SimNode::evalBool ( Context & context ) {
        assert(0 && "we should never be here");
        return cast<bool>::to(eval(context));
    }
    
    float SimNode::evalFloat ( Context & context ) {
        assert(0 && "we should never be here");
        return cast<float>::to(eval(context));
    }
    
    int32_t SimNode::evalInt ( Context & context ) {
        assert(0 && "we should never be here");
        return cast<int32_t>::to(eval(context));
    }
    
    uint32_t SimNode::evalUInt ( Context & context ) {
        assert(0 && "we should never be here");
        return cast<uint32_t>::to(eval(context));
    }
    
    int64_t SimNode::evalInt64 ( Context & context ) {
        assert(0 && "we should never be here");
        return cast<int64_t>::to(eval(context));
    }
    
    uint64_t SimNode::evalUInt64 ( Context & context ) {
        assert(0 && "we should never be here");
        return cast<uint64_t>::to(eval(context));
    }
    
    char * SimNode::evalPtr ( Context & context ) {
        assert(0 && "we should never be here");
        return cast<char *>::to(eval(context));
    }
    
    // SimNode_MakeBlock
    
    vec4f SimNode_MakeBlock::eval ( Context & context )  {
        Block block;
        block.stackOffset = uint32_t(context.stackTop - context.stack);
        block.argumentsOffset = argStackTop ? uint32_t(context.stackTop + argStackTop - context.stack) : 0;
        block.body = subexpr;
        return cast<Block>::from(block);
    }
    
    // SimNode_Call
    
    vec4f SimNode_Call::eval ( Context & context ) {
		vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
        evalArgs(context, argValues);
        DAS_EXCEPTION_POINT;
        return context.call(fnIndex, argValues, nullptr, debug.line);
    }
    
    vec4f SimNode_CallAndCopyOrMove::eval ( Context & context ) {
        vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
        evalArgs(context, argValues);
        DAS_EXCEPTION_POINT;
        auto cmres = context.stackTop + stackTop;
        return context.call(fnIndex, argValues, cmres, debug.line);
    }

    // SimNode_Invoke
    
    vec4f SimNode_Invoke::eval ( Context & context )  {
        vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
        evalArgs(context, argValues);
        DAS_EXCEPTION_POINT;
        Block block = cast<Block>::to(argValues[0]);
        if ( nArguments>1 ) {
            return context.invoke(block, argValues + 1, nullptr);
        } else {
            return context.invoke(block, nullptr, nullptr);
        }
    }
    
    vec4f SimNode_InvokeAndCopyOrMove::eval ( Context & context )  {
        vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
        evalArgs(context, argValues);
        DAS_EXCEPTION_POINT;
        Block block = cast<Block>::to(argValues[0]);
        auto cmres = context.stackTop + stackTop;
        if ( nArguments>1 ) {
            return context.invoke(block, argValues + 1, cmres);
        } else {
            return context.invoke(block, nullptr, cmres);
        }
    }
    
    // SimNode_Debug
    
    vec4f SimNode_Debug::eval ( Context & context ) {
        vec4f res = subexpr->eval(context);
        DAS_EXCEPTION_POINT;
        stringstream ssw;
        if ( message ) ssw << message << " ";
        ssw << debug_type(typeInfo) << " = " << debug_value(res, typeInfo, PrintFlags::debugger)
            << " at " << debug.describe() << "\n";
        context.to_out(ssw.str().c_str());
        return res;
    }
    
    // SimNode_Assert
    
    vec4f SimNode_Assert::eval ( Context & context ) {
        if ( !subexpr->evalBool(context) ) {
            DAS_EXCEPTION_POINT;
            string error_message = "assert failed";
            if ( message )
                error_message = error_message + ", " + message;
            string error = reportError(context.debugInput, debug.line, debug.column, error_message );
            error = context.getStackWalk(false) + error;
            context.to_err(error.c_str());
            context.throw_error("assert failed");
        }
        return vec_setzero_ps();
    }
    
    // SimNode_TryCatch
    
    vec4f SimNode_TryCatch::eval ( Context & context ) {
        #if DAS_ENABLE_EXCEPTIONS
            try_block->eval(context);
            if ( context.stopFlags & EvalFlags::stopForThrow ) {
                context.stopFlags &= ~(EvalFlags::stopForThrow | EvalFlags::stopForReturn | EvalFlags::stopForBreak);
                catch_block->eval(context);
            }
        #else
            try {
                try_block->eval(context);
            } catch ( const runtime_error & ) {
                context.stopFlags &= ~(EvalFlags::stopForThrow | EvalFlags::stopForReturn | EvalFlags::stopForBreak);
                catch_block->eval(context);
            }
        #endif
        return vec_setzero_ps();
    }
    
    // SimNode_New
    
    vec4f SimNode_New::eval ( Context & context ) {
        if ( void * ptr = context.allocate(bytes) ) {
            memset ( ptr, 0, bytes );
            return cast<void *>::from(ptr);
        } else {
            context.throw_error("out of memory");
            return vec_setzero_ps();
        }
    }
    
    // SimNode_CopyRefValue
    
    vec4f SimNode_CopyRefValue::eval ( Context & context ) {
        auto pl = l->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pr = r->evalPtr(context);
        DAS_EXCEPTION_POINT;
        memcpy ( pl, pr, size );
        return vec_setzero_ps();
    }
    
    // SimNode_MoveRefValue
    
    vec4f SimNode_MoveRefValue::eval ( Context & context ) {
        auto pl = l->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pr = r->evalPtr(context);
        DAS_EXCEPTION_POINT;
        memcpy ( pl, pr, size );
        memset ( pr, 0, size );
        return vec_setzero_ps();
    }
    
    // SimNode_Block
    
    vec4f SimNode_Block::eval ( Context & context ) {
        for ( uint32_t i = 0; i!=total && !context.stopFlags; ++i )
            list[i]->eval(context);
        return vec_setzero_ps();
    }
    
    vec4f SimNode_ClosureBlock::eval ( Context & context ) {
        for ( uint32_t i = 0; i!=total && !context.stopFlags; ++i )
            list[i]->eval(context);
        if ( context.stopFlags & EvalFlags::stopForReturn ) {
            context.stopFlags &= ~EvalFlags::stopForReturn;
            return context.abiResult();
        } else {
            if ( needResult ) context.throw_error("end of block without return");
            return vec_setzero_ps();
        }
    }
    
    // SimNode_Let
    
    vec4f SimNode_Let::eval ( Context & context ) {
        for ( uint32_t i = 0; i!=total && !context.stopFlags; ++i )
            list[i]->eval(context);
        DAS_EXCEPTION_POINT;
        return subexpr ? subexpr->eval(context) : vec_setzero_ps();
    }
    
    // SimNode_IfThenElse
    
    vec4f SimNode_IfThenElse::eval ( Context & context ) {
        bool cmp = cond->evalBool(context);
        DAS_EXCEPTION_POINT;
        if ( cmp ) {
            return if_true->eval(context);
        } else if ( if_false ) {
            return if_false->eval(context);
        } else {
            return vec_setzero_ps();
        }
    }
    
    // SimNode_While
    
    vec4f SimNode_While::eval ( Context & context ) {
        while ( cond->evalBool(context) && !context.stopFlags ) {
            body->eval(context);
        }
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return vec_setzero_ps();
    }
    
    // Return
    
    vec4f SimNode_Return::eval ( Context & context ) {
        if ( subexpr ) context.abiResult() = subexpr->eval(context);
        context.stopFlags |= EvalFlags::stopForReturn;
        return vec_setzero_ps();
    }
    
    vec4f SimNode_ReturnAndCopy::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pl = context.abiCopyOrMoveResult();
        memcpy ( pl, pr, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return vec_setzero_ps();
    }
    
    vec4f SimNode_ReturnAndMove::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pl = context.abiCopyOrMoveResult();
        memcpy ( pl, pr, size);
        memset ( pr, 0, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return vec_setzero_ps();
    }

    vec4f SimNode_ReturnReference::eval ( Context & context ) {
        char * ref = subexpr->evalPtr(context);
        if ( context.stack<=ref && ref<context.stackTop ) {
            context.throw_error("reference bellow current function stack frame");
            return vec_setzero_ps();
        }
#if DAS_ENABLE_STACK_WALK
        auto pp = (Prologue *) context.stackTop;
        auto top = context.stackTop + pp->info->stackSize;
        if ( context.stackTop<=ref && ref<top ) {
            context.throw_error("reference to current function stack frame");
            return vec_setzero_ps();
        }
#endif
        context.abiResult() = cast<char *>::from(ref);
        context.stopFlags |= EvalFlags::stopForReturn;
        return vec_setzero_ps();
    }
    
    vec4f SimNode_ReturnAndCopyFromBlock::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto ba = (BlockArguments *) ( context.stackTop + argStackTop );
        auto pl = ba->copyOrMoveResult;
        memcpy ( pl, pr, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return vec_setzero_ps();
    }
    
    vec4f SimNode_ReturnAndMoveFromBlock::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto ba = (BlockArguments *) ( context.stackTop + argStackTop );
        auto pl = ba->copyOrMoveResult;
        memcpy ( pl, pr, size);
        memset ( pr, 0, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return vec_setzero_ps();
    }
    
    vec4f SimNode_ReturnReferenceFromBlock::eval ( Context & context ) {
        char * ref = subexpr->evalPtr(context);
        if ( context.stack<=ref && ref<context.invokeStackTop ) {
            context.throw_error("reference bellow current call chain stack frame");
            return vec_setzero_ps();
        }
        context.abiResult() = cast<char *>::from(ref);
        context.stopFlags |= EvalFlags::stopForReturn;
        return vec_setzero_ps();
    }
    
    // Context
    
    Context::Context(const string * lines, int las) {
        linearAllocatorSize = las;
        linearAllocator = linearAllocatorBase = (char *) _mm_malloc(linearAllocatorSize, 16);
        stack = (char *) _mm_malloc(stackSize, 16);
        stackTop = stack + stackSize;
        debugInput = lines;
    }
    
    Context::~Context() {
        _mm_free(linearAllocatorBase);
        _mm_free(stack);
    }
    
    void * Context::reallocate ( void * oldData, uint32_t oldSize, uint32_t size ) {
        if ( size <= oldSize ) return oldData;
        size = (size + 0x0f) & ~0x0f;
        oldSize = (oldSize + 0x0f) & ~0x0f;
        if ( oldData && (oldData == linearAllocator - oldSize) ) {
            uint32_t extra = size - oldSize;
            if ( linearAllocator - linearAllocatorBase + extra > linearAllocatorSize ) {
                throw_error("out of linear allocator space");
                return nullptr;
            }
            linearAllocator += extra;
            return oldData;
        } else {
            return allocate(size);
        }
    }
    
    void * Context::allocate ( uint32_t size ) {
        size = (size + 0x0f) & ~0x0f;
        if ( linearAllocator - linearAllocatorBase + size > linearAllocatorSize ) {
            throw_error("out of linear allocator space");
            return nullptr;
        }
        void * res = linearAllocator;
        linearAllocator += size;
        return res;
    }
    
    char * Context::allocateName ( const string & name ) {
        if ( name.empty() )
            return nullptr;
        auto size = name.length() + 1;
        if ( char * str = (char *) allocate(uint32_t(size+1)) ) {
            memcpy ( str, name.c_str(), size );
            return str;
        } else {
            return nullptr;
        }
    }
    
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#endif
    vec4f Context::invokeEx(const Block &block, vec4f * args, void * cmres, function<void (SimNode *)> && when) {
        char * saveSp = stackTop;
        char * saveISp = invokeStackTop;
        invokeStackTop = stackTop;
        stackTop = stack + block.stackOffset;
        assert ( stackTop >= stack && stackTop < stackTop + stackSize );
        BlockArguments * ba = nullptr;
        BlockArguments saveArguments;
        if ( block.argumentsOffset ) {
            ba = (BlockArguments *) ( stack + block.argumentsOffset );
            saveArguments = *ba;
            ba->arguments = args;
            ba->copyOrMoveResult = (char *) cmres;
        }
        when(block.body);
        if ( ba ) {
            *ba = saveArguments;
        }
        invokeStackTop = saveISp;
        stackTop = saveSp;
        assert ( stackTop >= stack && stackTop < stackTop + stackSize );
        return result;
    }
#ifdef _MSC_VER
#pragma warning(pop)
#endif

    vec4f Context::callEx(int fnIndex, vec4f *args, void * cmres, int line, function<void (SimNode *)> && when) {
        assert(fnIndex>=0 && fnIndex<totalFunctions && "function index out of range");
        auto & fn = functions[fnIndex];
        // PUSH
        char * top = invokeStackTop ? invokeStackTop : stackTop;
        if ( stack - ( top - fn.stackSize ) > stackSize ) {
            throw_error("stack overflow");
            return vec_setzero_ps();
        }
        char * pushStack = stackTop;
        char * pushInvokeStack = invokeStackTop;
        stackTop = top - fn.stackSize;
        assert ( stackTop >= stack && stackTop < stackTop + stackSize );
        invokeStackTop = nullptr;
        // fill prologue
        Prologue * pp = (Prologue *) stackTop;
        pp->arguments =     args;
        pp->copyOrMoveResult = (char *)cmres;
#if DAS_ENABLE_STACK_WALK
        pp->info =          fn.debug;
        pp->line =          line;
#endif
        // CALL
        when(fn.code);
        stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
        // POP
        invokeStackTop = pushInvokeStack;
        stackTop = pushStack;
        assert ( stackTop >= stack && stackTop < stackTop + stackSize );
        return result;
    }

    
    void Context::runInitScript ( void ) {
        for ( int i=0; i!=totalVariables && !stopFlags; ++i ) {
            auto & pv = globalVariables[i];
            if ( pv.init ) {
                pv.init->eval(*this);
            } else {
                memset ( cast<char *>::to(pv.value), 0, pv.size );
            }
        }
    }
    
    int Context::findFunction ( const char * name ) const {
        for ( int fni = 0; fni != totalFunctions; ++fni ) {
            if ( strcmp(functions[fni].name, name)==0 ) {
                return fni;
            }
        }
        return -1;
    }
    
    int Context::findVariable ( const char * name ) const {
        for ( int vni = 0; vni != totalVariables; ++vni ) {
            if ( strcmp(globalVariables[vni].name, name)==0 ) {
                return vni;
            }
        }
        return -1;
    }
    
    void Context::stackWalk() {
        auto str = getStackWalk();
        to_out(str.c_str());
    }
    
    string Context::getStackWalk( bool args ) {
        stringstream ssw;
    #if DAS_ENABLE_STACK_WALK
        ssw << "\nCALL STACK (sp=" << (stack + stackSize - stackTop) << "):\n";
        char * sp = stackTop;
        while ( sp>=stackTop && sp <(stack+stackSize) ) {
            int isp = int(stack + stackSize - sp);
            Prologue * pp = (Prologue *) sp;
            if ( pp->line ) {
                ssw << pp->info->name << " at line " << pp->line << " (sp=" << isp << ")\n";
            } else {
                ssw << pp->info->name << "(sp=" << isp << ")\n";
            }
            if ( args ) {
                for ( uint32_t i = 0; i != pp->info->argsSize; ++i ) {
                    ssw << "\t" << pp->info->args[i]->name
                        << " : " << debug_type(pp->info->args[i])
                        << " = \t" << debug_value(pp->arguments[i], pp->info->args[i], PrintFlags::stackwalker) << "\n";
                }
            }
            sp += pp->info->stackSize;
        }
        ssw << "\n";
    #else
        ssw << "\nCALL STACK TRACKING DISABLED:\n\n";
    #endif
        return ssw.str();
    }
    
    void Context::breakPoint(int, int ) const {
#ifdef _MSC_VER
		__debugbreak();
#else
        raise(SIGTRAP);
#endif
    }
    
    void Context::to_out ( const char * message ) {
        cout << message;
    }
    
    void Context::to_err ( const char * message ) {
        cerr << message;
    }
}
