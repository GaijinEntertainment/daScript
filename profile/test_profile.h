#pragma once

#include "vectypes.h"

struct Object {
    das::float3   pos;
    das::float3   vel;
};

typedef std::vector<Object> ObjectArray;
