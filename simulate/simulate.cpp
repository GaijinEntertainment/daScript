#include "precomp.h"

#include "simulate.h"
#include "runtime_string.h"

namespace yzg
{
    // SimNode_MakeBlock
    
    __m128 SimNode_MakeBlock::eval ( Context & context )  {
        Block block;
        block.stackTop = context.stackTop;
        block.body = subexpr;
        return cast<Block>::from(block);
    }
    
    // SimNode_Invoke
    
    __m128 SimNode_Invoke::eval ( Context & context )  {
        Block block = cast<Block>::to(subexpr->eval(context));
        YZG_EXCEPTION_POINT;
        return context.invoke(block);
    }
    
    // SimNode_At
    
    __m128 SimNode_At::eval ( Context & context )  {
        char * pValue = cast<char *>::to(value->eval(context));
        YZG_EXCEPTION_POINT;
        uint32_t idx = cast<uint32_t>::to(index->eval(context));
        YZG_EXCEPTION_POINT;
        if ( idx >= range ) {
            context.throw_error("index out of range");
            return _mm_setzero_ps();
        } else {
            return cast<char *>::from(pValue + idx*stride);
        }
    }
    
    // SimNode_Call
    
    void SimNode_Call::evalArgs ( Context & context, __m128 * argValues ) {
        for ( int i=0; i!=nArguments && !context.stopFlags; ++i ) {
            argValues[i] = arguments[i]->eval(context);
        }
    }
    
    __m128 SimNode_Call::eval ( Context & context ) {
        __m128 argValues[nArguments];
        evalArgs(context, argValues);
        YZG_EXCEPTION_POINT;
        return context.call(fnIndex, argValues, debug.line);
    }
    
    // SimNode_Debug
    
    __m128 SimNode_Debug::eval ( Context & context ) {
        __m128 res = subexpr->eval(context);
        YZG_EXCEPTION_POINT;
        stringstream ssw;
        if ( message ) ssw << message << " ";
        ssw << debug_type(typeInfo) << " = " << debug_value(res, typeInfo) << " at " << debug.describe() << "\n";
        context.to_out(ssw.str().c_str());
        return res;
    }
    
    // SimNode_Assert
    
    __m128 SimNode_Assert::eval ( Context & context ) {
        if ( !cast<bool>::to(subexpr->eval(context)) ) {
            YZG_EXCEPTION_POINT;
            string error_message = "assert failed";
            if ( message )
                error_message = error_message + ", " + message;
            string error = reportError(context.debugInput, debug.line, debug.column, error_message );
            context.to_err(error.c_str());
            context.throw_error("assert failed");
        }
        return _mm_setzero_ps();
    }
    
    // SimNode_TryCatch
    
    __m128 SimNode_TryCatch::eval ( Context & context ) {
        #if YZG_ENABLE_EXCEPTIONS
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
        return _mm_setzero_ps();
    }
    
    // SimNode_Ptr2Ref
    
    __m128 SimNode_Ptr2Ref::eval ( Context & context ) {
        __m128 ptr = subexpr->eval(context);
        YZG_EXCEPTION_POINT;
        void * p = cast<void *>::to(ptr);
        if ( p == nullptr ) {
            context.throw_error("dereferencing null pointer");
            return _mm_setzero_ps();
        }
        return ptr;
    }
    
    // SimNode_Ptr2Ref
    
    __m128 SimNode_NullCoalescingRef::eval ( Context & context ) {
        __m128 ptr = subexpr->eval(context);
        YZG_EXCEPTION_POINT;
        if ( cast<void *>::to(ptr) ) {
            return ptr;
        } else {
            return value->eval(context);
        }
    }
    
    // FIELD
    
    __m128 SimNode_FieldDeref::eval ( Context & context ) {
        __m128 rv = value->eval(context);
        YZG_EXCEPTION_POINT;
        if ( char * prv = cast<char *>::to(rv) ) {
            return cast<char *>::from( prv + offset );
        } else {
            context.throw_error("dereferencing nil pointer");
            return _mm_setzero_ps();
        }
    }
    
    // FIELD ?.
    
    __m128 SimNode_SafeFieldDeref::eval ( Context & context ) {
        __m128 rv = value->eval(context);
        YZG_EXCEPTION_POINT;
        if ( char * prv = cast<char *>::to(rv) ) {
            return cast<char *>::from( prv + offset );
        } else {
            return _mm_setzero_ps();
        }
    }
    
    // FIELD ?. ->
    
    __m128 SimNode_SafeFieldDerefPtr::eval ( Context & context ) {
        __m128 rv = value->eval(context);
        YZG_EXCEPTION_POINT;
        if ( char ** prv = cast<char **>::to(rv) ) {
            return cast<char *>::from( *(prv + offset) );
        } else {
            return _mm_setzero_ps();
        }
    }
    
    // SimNode_New
    
    __m128 SimNode_New::eval ( Context & context ) {
        if ( void * ptr = context.allocate(bytes) ) {
            memset ( ptr, 0, bytes );
            return cast<void *>::from(ptr);
        } else {
            context.throw_error("out of memory");
            return _mm_setzero_ps();
        }
    }
    
    // SimNode_CopyRefValue
    
    __m128 SimNode_CopyRefValue::eval ( Context & context ) {
        __m128 ll = l->eval(context);
        YZG_EXCEPTION_POINT;
        __m128 rr = r->eval(context);
        YZG_EXCEPTION_POINT;
        auto pl = cast<void *>::to(ll);
        auto pr = cast<void *>::to(rr);
        memcpy ( pl, pr, size );
        return ll;
    }
    
    // SimNode_MoveRefValue
    
    __m128 SimNode_MoveRefValue::eval ( Context & context ) {
        __m128 ll = l->eval(context);
        YZG_EXCEPTION_POINT;
        __m128 rr = r->eval(context);
        YZG_EXCEPTION_POINT;
        auto pl = cast<void *>::to(ll);
        auto pr = cast<void *>::to(rr);
        memcpy ( pl, pr, size );
        memset ( pr, 0, size );
        return ll;
    }
    
    // SimNode_Block
    
    __m128 SimNode_Block::eval ( Context & context ) {
        __m128 result = _mm_setzero_ps();
        for ( int i = 0; i!=total && !context.stopFlags; ++i )
            result = list[i]->eval(context);
        return result;
    }
    
    // SimNode_Let
    
    __m128 SimNode_Let::eval ( Context & context ) {
        for ( int i = 0; i!=total && !context.stopFlags; ++i )
            list[i]->eval(context);
        YZG_EXCEPTION_POINT;
        return subexpr ? subexpr->eval(context) : _mm_setzero_ps();
    }
    
    // SimNode_IfThenElse
    
    __m128 SimNode_IfThenElse::eval ( Context & context ) {
        bool cmp = cast<bool>::to(cond->eval(context));
        YZG_EXCEPTION_POINT;
        if ( cmp ) {
            return if_true->eval(context);
        } else if ( if_false ) {
            return if_false->eval(context);
        } else {
            return _mm_setzero_ps();
        }
    }
    
    // SimNode_While
    
    __m128 SimNode_While::eval ( Context & context ) {
        while ( cast<bool>::to(cond->eval(context)) && !context.stopFlags ) {
            body->eval(context);
        }
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return _mm_setzero_ps();
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
        void * result = linearAllocator;
        linearAllocator += size;
        return result;
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
    __m128 Context::invoke(const Block &block) {
        char * saveSp = stackTop;
        char * saveISp = invokeStackTop;
        invokeStackTop = stackTop;
        stackTop = block.stackTop;
        assert ( stackTop >= stack && stackTop < stackTop + stackSize );
        // cout << "invoke , stack at " << (context.stack + context.stackSize - context.stackTop) << endl;
        __m128 result = block.body->eval(*this);
        invokeStackTop = saveISp;
        stackTop = saveSp;
        assert ( stackTop >= stack && stackTop < stackTop + stackSize );
        return result;
    }

    __m128 Context::call ( int fnIndex, __m128 * args, int line ) {
        assert(fnIndex>=0 && fnIndex<totalFunctions && "function index out of range");
        auto & fn = functions[fnIndex];
        // PUSH
        char * top = invokeStackTop ? invokeStackTop : stackTop;
        if ( stack - ( top - fn.stackSize ) > stackSize ) {
            throw_error("stack overflow");
            return _mm_setzero_ps();
        }
        char * pushStack = stackTop;
        char * pushInvokeStack = invokeStackTop;
        stackTop = top - fn.stackSize;
        assert ( stackTop >= stack && stackTop < stackTop + stackSize );
        invokeStackTop = nullptr;
        // cout << "call " << fn.debug->name <<  ", stack at " << (stack + stackSize - stackTop) << endl;
        // fill prologue
        Prologue * pp = (Prologue *) stackTop;
        pp->result =        _mm_setzero_ps();
        pp->arguments =     args;
#if YZG_ENABLE_STACK_WALK
        pp->info =          fn.debug;
        pp->line =          line;
#endif
        // CALL
        fn.code->eval(*this);
        __m128 result = abiResult();
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
                memset ( cast<void *>::to(pv.value), 0, pv.size );
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
        stringstream ssw;
    #if YZG_ENABLE_STACK_WALK
        ssw << "\nCALL STACK (sp=" << (stack + stackSize - stackTop) << "):\n";
        char * sp = stackTop;
        while ( sp>=stackTop && sp <(stack+stackSize) ) {
            int isp = (stack + stackSize - sp);
            Prologue * pp = (Prologue *) sp;
            if ( pp->line ) {
                ssw << pp->info->name << " at line " << pp->line << " (sp=" << isp << ")\n";
            } else {
                ssw << pp->info->name << "(sp=" << isp << ")\n";
            }
            for ( uint32_t i = 0; i != pp->info->argsSize; ++i ) {
                ssw << "\t" << pp->info->args[i]->name
                    << " : " << debug_type(pp->info->args[i])
                    << " = \t" << debug_value(pp->arguments[i], pp->info->args[i]) << "\n";
            }
            sp += pp->info->stackSize;
        }
        ssw << "\n";
    #else
        ssw << "\nCALL STACK TRACKING DISABLED:\n\n";
    #endif
       to_out(ssw.str().c_str());
    }
    
    void Context::breakPoint(int column, int line) const {
        raise(SIGTRAP);
    }
    
    void Context::to_out ( const char * message ) {
        cout << message;
    }
    
    void Context::to_err ( const char * message ) {
        cerr << message;
    }
}
