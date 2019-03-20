#pragma once

#include "daScript/misc/vectypes.h"

struct Object {
    das::float3   pos;  float pad1;
    das::float3   vel;  float pad2;
};

typedef std::vector<Object> ObjectArray;

int AddOne(int a);
