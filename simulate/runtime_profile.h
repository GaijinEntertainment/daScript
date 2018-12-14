#pragma once

#include "simulate.h"

namespace yzg
{
    // profile(count,category,block) -> float time in sec
    __m128 builtin_profile ( Context & context, SimNode_Call *, __m128 * args );
}

