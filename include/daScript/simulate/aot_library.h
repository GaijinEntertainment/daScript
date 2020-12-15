#pragma once

#include "daScript/simulate/simulate.h"

namespace das {
    typedef function<SimNode * (Context &)> AotFactory;
    typedef das_hash_map<uint64_t,AotFactory> AotLibrary;

    typedef void ( * RegisterAotFunctions ) ( AotLibrary & );

    struct AotListBase {
        AotListBase( RegisterAotFunctions prfn );
        static void registerAot ( AotLibrary & lib );

        AotListBase * tail = nullptr;
        static AotListBase * head;
        RegisterAotFunctions regFn;
    };
}

