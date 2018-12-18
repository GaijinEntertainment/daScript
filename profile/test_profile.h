#pragma once

#include "vectypes.h"

struct Object {
    yzg::float3   pos;
    yzg::float3   vel;
};

typedef std::vector<Object> ObjectArray;
