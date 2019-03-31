#pragma once

#include "daScript/daScript.h"

struct TestObjectFoo {
    int fooData;
    int propAdd13() {
        return fooData + 13;
    }
};

int *getPtr();
