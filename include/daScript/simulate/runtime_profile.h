#pragma once

#include "daScript/simulate/simulate.h"

namespace das
{
    // profile(count,category,block) -> float time in sec
    float builtin_profile ( int32_t count, char * category, const Block & block, Context * context );
}

