//
//  simulate.cpp
//  yzg
//
//  Created by Boris Batkin on 10/14/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "simulate.hpp"

namespace yzg
{
    Context::Context()
    {
        linearAllocator = linearAllocatorBase = (char *) _mm_malloc(linearAllocatorSize, 16);
        stack = (char *) _mm_malloc(stackSize, 16);
        stackTop = stack + stackSize;
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
        char * sp = stackTop;
        while ( sp>=stackTop && sp <(stack+stackSize) ) {
            SimNode_Call * call = *(SimNode_Call **) sp;
            if ( call ) {
                cout << functions[call->fnIndex].name << "\n";
                sp += functions[call->fnIndex].stackSize;
            } else {
                cout << "external_function_call\n";
                return;
            }
            if ( FuncInfo * info = functions[call->fnIndex].debug ) {
                for ( uint32_t i = 0; i != info->argsSize; ++i ) {
                    cout << "\t" << info->args[i]->name
                        << " : " << debug_type(info->args[i])
                        << " = \t" << debug_value(call->argValues[i], info->args[i]) << "\n";
                }
            }
        }
        cout << "corrupt stack\n";
    }
}
