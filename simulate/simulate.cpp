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
            SimNode_Call * call = *(SimNode_Call **) sp;
            if ( call ) {
                if ( debugInput ) {
                    int col, row;
                    positionToRowCol ( *debugInput, call->debug, col, row );
                    cout << functions[call->fnIndex].name << " at line " << row << "\n";
                } else {
                    cout << functions[call->fnIndex].name << "\n";
                }
                if ( FuncInfo * info = functions[call->fnIndex].debug ) {
                    for ( uint32_t i = 0; i != info->argsSize; ++i ) {
                        cout << "\t" << info->args[i]->name
                        << " : " << debug_type(info->args[i])
                        << " = \t" << debug_value(call->argValues[i], info->args[i]) << "\n";
                    }
                }
                sp += functions[call->fnIndex].stackSize;
            } else {
                cout << "external_function_call\n";
                return;
            }
        }
        cout << "!!! corrupt stack !!!\n\n";
    }
}
