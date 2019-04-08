#pragma once

#include "daScript/daScript.h"

struct TestObjectFoo {
    int fooData;
    int propAdd13() {
        return fooData + 13;
    }
};

int *getPtr();

void testFields ( das::Context * ctx );
void test_das_string(const das::Block & block, das::Context * context);
vec4f new_and_init ( das::Context & context, das::SimNode_CallBase * call, vec4f * );
