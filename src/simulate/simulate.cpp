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
    
    vec4f SimNode_Swizzle::eval ( Context & context ) {
        union {
            vec4f   res;
            float   val[4];
        } R, S;
        S.res = value->eval(context);
        DAS_EXCEPTION_POINT;
        R.val[0] = S.val[fields[0]];
        R.val[1] = S.val[fields[1]];
        R.val[2] = S.val[fields[2]];
        R.val[3] = S.val[fields[3]];
        return R.res;
    }
    
    // SimNode_MakeBlock
    
    vec4f SimNode_MakeBlock::eval ( Context & context )  {
        Block block;
        block.stackOffset = context.stack.spi();
        block.argumentsOffset = argStackTop ? (context.stack.spi() + argStackTop) : 0;
        block.body = subexpr;
        return cast<Block>::from(block);
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
        return v_zero();
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
        return v_zero();
    }
    
    // SimNode_New
    
    vec4f SimNode_New::eval ( Context & context ) {
        if ( void * ptr = context.heap.allocate(bytes) ) {
            memset ( ptr, 0, bytes );
            return cast<void *>::from(ptr);
        } else {
            context.throw_error("out of heap");
            return v_zero();
        }
    }
    
    // SimNode_CopyRefValue
    
    vec4f SimNode_CopyRefValue::eval ( Context & context ) {
        auto pl = l->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pr = r->evalPtr(context);
        DAS_EXCEPTION_POINT;
        memcpy ( pl, pr, size );
        return v_zero();
    }
    
    // SimNode_MoveRefValue
    
    vec4f SimNode_MoveRefValue::eval ( Context & context ) {
        auto pl = l->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pr = r->evalPtr(context);
        DAS_EXCEPTION_POINT;
        memcpy ( pl, pr, size );
        memset ( pr, 0, size );
        return v_zero();
    }
    
    // SimNode_Block
    
    vec4f SimNode_Block::eval ( Context & context ) {
        for ( uint32_t i = 0; i!=total && !context.stopFlags; ++i )
            list[i]->eval(context);
        return v_zero();
    }
    
    vec4f SimNode_ClosureBlock::eval ( Context & context ) {
        for ( uint32_t i = 0; i!=total && !context.stopFlags; ++i )
            list[i]->eval(context);
        if ( context.stopFlags & EvalFlags::stopForReturn ) {
            context.stopFlags &= ~EvalFlags::stopForReturn;
            return context.abiResult();
        } else {
            if ( needResult ) context.throw_error("end of block without return");
            return v_zero();
        }
    }
    
    // SimNode_Let
    
    vec4f SimNode_Let::eval ( Context & context ) {
        for ( uint32_t i = 0; i!=total && !context.stopFlags; ++i )
            list[i]->eval(context);
        DAS_EXCEPTION_POINT;
        return subexpr ? subexpr->eval(context) : v_zero();
    }
    
    // SimNode_IfThenElse
    
    vec4f SimNode_IfThenElse::eval ( Context & context ) {
        bool cmp = cond->evalBool(context);
        DAS_EXCEPTION_POINT;
        if ( cmp ) {
            return if_true->eval(context);
        } else {
            return if_false->eval(context);
        }
    }
    
    vec4f SimNode_IfThen::eval ( Context & context ) {
        bool cmp = cond->evalBool(context);
        DAS_EXCEPTION_POINT;
        if ( cmp ) {
            return if_true->eval(context);
        } else {
            return v_zero();
        }
    }
    
    // SimNode_While
    
    vec4f SimNode_While::eval ( Context & context ) {
        while ( cond->evalBool(context) && !context.stopFlags ) {
            body->eval(context);
        }
        context.stopFlags &= ~EvalFlags::stopForBreak;
        return v_zero();
    }
    
    // Return
    
    vec4f SimNode_Return::eval ( Context & context ) {
        if ( subexpr ) context.abiResult() = subexpr->eval(context);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

	vec4f SimNode_ReturnConst::eval ( Context & context ) {
		context.abiResult() = value;
		context.stopFlags |= EvalFlags::stopForReturn;
		return v_zero();
	}

    vec4f SimNode_ReturnAndCopy::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pl = context.abiCopyOrMoveResult();
        memcpy ( pl, pr, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }
    
    vec4f SimNode_ReturnAndMove::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto pl = context.abiCopyOrMoveResult();
        memcpy ( pl, pr, size);
        memset ( pr, 0, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }

    vec4f SimNode_ReturnReference::eval ( Context & context ) {
        char * ref = subexpr->evalPtr(context);
        if ( context.stack.bottom()<=ref && ref<context.stack.sp()) {
            context.throw_error("reference bellow current function stack frame");
            return v_zero();
        }
#if DAS_ENABLE_STACK_WALK
        auto pp = (Prologue *) context.stack.sp();
        auto top = context.stack.sp() + pp->info->stackSize;
        if ( context.stack.sp()<=ref && ref<top ) {
            context.throw_error("reference to current function stack frame");
            return v_zero();
        }
#endif
        context.abiResult() = cast<char *>::from(ref);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }
    
    vec4f SimNode_ReturnAndCopyFromBlock::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto ba = (BlockArguments *) ( context.stack.sp() + argStackTop );
        auto pl = ba->copyOrMoveResult;
        memcpy ( pl, pr, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }
    
    vec4f SimNode_ReturnAndMoveFromBlock::eval ( Context & context ) {
        auto pr = subexpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        auto ba = (BlockArguments *) ( context.stack.sp() + argStackTop );
        auto pl = ba->copyOrMoveResult;
        memcpy ( pl, pr, size);
        memset ( pr, 0, size);
        context.abiResult() = cast<char *>::from(pl);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }
    
    vec4f SimNode_ReturnReferenceFromBlock::eval ( Context & context ) {
        char * ref = subexpr->evalPtr(context);
        if ( context.stack.bottom()<=ref && ref<context.stack.ap() ) {
            context.throw_error("reference bellow current call chain stack frame");
            return v_zero();
        }
        context.abiResult() = cast<char *>::from(ref);
        context.stopFlags |= EvalFlags::stopForReturn;
        return v_zero();
    }
    
    // Context
    
    Context::Context(const string * lines, uint32_t heapSize)
        :   heap(heapSize)
		,   code(64*1024)
		,   debugInfo(64*1024)
        ,   stack(16*1024)
	{
        debugInput = lines;
    }
    
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4324)
#pragma warning(disable:4701)
#endif
    vec4f Context::invokeEx(const Block &block, vec4f * args, void * cmres, function<void (SimNode *)> && when) {
		char * EP, *SP;
		stack.invoke(block.stackOffset,EP,SP);
        BlockArguments * ba = nullptr;
        BlockArguments saveArguments;
        if ( block.argumentsOffset ) {
            ba = (BlockArguments *) ( stack.bottom() + block.argumentsOffset );
            saveArguments = *ba;
            ba->arguments = args;
            ba->copyOrMoveResult = (char *) cmres;
        }
        when(block.body);
        if ( ba ) {
            *ba = saveArguments;
        }
		stack.pop(EP,SP);
        return result;
    }
#ifdef _MSC_VER
#pragma warning(pop)
#endif

    vec4f Context::callEx(int fnIndex, vec4f *args, void * cmres, int line, function<void (SimNode *)> && when) {
        assert(fnIndex>=0 && fnIndex<totalFunctions && "function index out of range");
        auto & fn = functions[fnIndex];
        // PUSH
		char * EP, *SP;
		if(!stack.push(fn.stackSize,EP,SP)) {
            throw_error("stack overflow");
            return v_zero();
        }
        // fill prologue
        vec4f * aa = abiArg;
        Prologue * pp           = (Prologue *) stack.sp();
        pp->arguments           = abiArg = args;
        pp->copyOrMoveResult    = (char *)cmres;
#if DAS_ENABLE_STACK_WALK
        pp->info                = fn.debug;
        pp->line                = line;
#endif
        // CALL
        when(fn.code);
        stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
        // POP
        abiArg = aa;
		stack.pop(EP,SP);
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
        ssw << "\nCALL STACK (sp=" << (stack.top() - stack.sp()) << "):\n";
        char * sp = stack.sp();
        while (  sp < stack.top() ) {
            Prologue * pp = (Prologue *) sp;
            if ( pp->line ) {
                ssw << pp->info->name << " at line " << pp->line << " (sp=" << (stack.top() - sp) << ")\n";
            } else {
                ssw << pp->info->name << "(sp=" << (stack.top() - sp) << ")\n";
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
