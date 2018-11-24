#include "precomp.h"

#include "simulate.h"
#include "runtime_string.h"

#include <signal.h>

namespace yzg
{
    // Debug
    
    __m128 SimNode_Assert::eval ( Context & context )
    {
        if ( !cast<bool>::to(subexpr->eval(context)) ) {
            string error_message = "assert failed";
            if ( message )
                error_message = error_message + ", " + message;
            string error = reportError(context.debugInput, debug.line, debug.column, error_message );
            context.to_err(error.c_str());
            context.throw_error("assert failed");
        }
        return _mm_setzero_ps();
    }
    
    // Context
    
    Context::Context(const string * lines)
    {
        linearAllocator = linearAllocatorBase = (char *) _mm_malloc(linearAllocatorSize, 16);
        stack = (char *) _mm_malloc(stackSize, 16);
        stackTop = stack + stackSize;
        debugInput = lines;
    }
    
    Context::~Context()
    {
        _mm_free(linearAllocatorBase);
        _mm_free(stack);
    }
    
    void Context::runInitScript ( void )
    {
        for ( int i=0; i!=totalVariables; ++i ) {
            auto & pv = globalVariables[i];
            if ( pv.init ) {
                pv.init->eval(*this);
            } else {
                memset ( cast<void *>::to(pv.value), 0, pv.size );
            }
        }
    }
    
    int Context::findFunction ( const char * name ) const
    {
        for ( int fni = 0; fni != totalFunctions; ++fni ) {
            if ( strcmp(functions[fni].name, name)==0 ) {
                return fni;
            }
        }
        return -1;
    }
    
    int Context::findVariable ( const char * name ) const
    {
        for ( int vni = 0; vni != totalVariables; ++vni ) {
            if ( strcmp(globalVariables[vni].name, name)==0 ) {
                return vni;
            }
        }
        return -1;
    }
    
    void Context::stackWalk()
    {
        stringstream ssw;
    #if ENABLE_STACK_WALK
        ssw << "\nCALL STACK:\n";
        char * sp = stackTop;
        while ( sp>=stackTop && sp <(stack+stackSize) ) {
            Prologue * pp = (Prologue *) sp;
            if ( pp->line ) {
                ssw << pp->info->name << " at line " << pp->line << "\n";
            } else {
                ssw << pp->info->name << "\n";
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
    
    void Context::breakPoint(int column, int line) const
    {
        raise(SIGTRAP);
    }
    
    void Context::throw_error ( const char * message )
    {
        throw runtime_error(message);
    }
    
    void Context::to_out ( const char * message )
    {
        cout << message;
    }
    
    void Context::to_err ( const char * message )
    {
        cerr << message;
    }
}
