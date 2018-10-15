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
    }
    
    Context::~Context()
    {
        _mm_free(linearAllocatorBase);
    }
}
