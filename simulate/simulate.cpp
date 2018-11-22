#include "precomp.h"

#include "simulate.h"

namespace yzg
{
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
        cout << "\nCALL STACK:\n";
        char * sp = stackTop;
        while ( sp>=stackTop && sp <(stack+stackSize) ) {
            Prologue * pp = (Prologue *) sp;
            if ( pp->row && pp->col ) {
                cout << pp->info->name << " at line " << pp->row << "\n";
            } else {
                cout << pp->info->name << "\n";
            }
            for ( uint32_t i = 0; i != pp->info->argsSize; ++i ) {
                cout << "\t" << pp->info->args[i]->name
                << " : " << debug_type(pp->info->args[i])
                << " = \t" << debug_value(pp->arguments[i], pp->info->args[i]) << "\n";
            }
            sp += pp->info->stackSize;
        }
        cout << "\n";
    }
}
