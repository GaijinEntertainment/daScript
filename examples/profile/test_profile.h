#pragma once

#include "daScript/daScript.h"

struct Object {
    das::float3   pos;  
    das::float3   vel;  
};

typedef std::vector<Object> ObjectArray;

int AddOne(int a);

int testDict(das::Array & arr);
float testExpLoop(int count);
int testFibR(int n);
int testFibI(int n);

