#pragma once

#include "daScript/misc/vectypes.h"

struct Object {
    das::float3   pos;
    das::float3   vel;
};

typedef vector<Object> ObjectArray;
