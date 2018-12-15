#pragma once

#include "simulate.h"

namespace yzg
{
    // profile(count,category,block) -> float time in sec
    float builtin_profile ( int32_t count, char * category, Block block, Context * context );
}

