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
        stackTop = (char *) _mm_malloc(stackSize, 16);
    }
    
    Context::~Context()
    {
        _mm_free(linearAllocatorBase);
        _mm_free(stackTop);
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
}
