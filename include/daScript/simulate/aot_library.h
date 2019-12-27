#pragma once

#include "daScript/simulate/simulate.h"

namespace das {
    typedef function<SimNode * (Context &)> AotFactory;
    typedef das_hash_map<uint64_t,AotFactory> AotLibrary;

    struct AotListBase {
        AotListBase();
        static void registerAot ( AotLibrary & lib );
        virtual void registerAotFunctions ( AotLibrary & lib ) = 0;
        AotListBase * tail = nullptr;
        static AotListBase * head;
    };
}

