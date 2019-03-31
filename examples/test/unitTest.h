#pragma once

#include "daScript/daScript.h"

struct TestObjectFoo {
    int fooData;
    int propAdd13() {
        return fooData + 13;
    }
};

int *getPtr();

vec4f new_and_init ( das::Context & context, das::SimNode_CallBase * call, vec4f * );
