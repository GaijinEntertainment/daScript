#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "unitTest.h"

// this is how we disable AOT
#if !USE_AOT

namespace das {
    namespace aot {
        void registerAot ( AotLibrary & ) {
        }
    }
}

#else

namespace das {
    namespace aot {

        void registerAot_1 ( AotLibrary & );
        void registerAot_2 ( AotLibrary & );
        void registerAot_3 ( AotLibrary & );
        void registerAot_4 ( AotLibrary & );

        void registerAot ( AotLibrary & lib ) {
            registerAot_1(lib);
            registerAot_2(lib);
            registerAot_3(lib);
            registerAot_4(lib);
        }
    }
}
#endif

