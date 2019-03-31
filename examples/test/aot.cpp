#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

namespace das {

    template <typename TT>
    struct das_new {
        static __forceinline TT * make ( Context * __context__ ) {
            char * data = __context__->heap.allocate( sizeof(TT) );
            memset ( data, 0, sizeof(TT) );
            return (TT *) data;
        }
    };

    namespace aot {
        namespace test_array {
            #include "unit_tests/array.das.main.inc"
        }
        namespace test_auto_infer {
            #include "unit_tests/auto_infer.das.main.inc"
        }
        void registerAot ( AotLibrary & aotLib )
        {
            {   using namespace test_array;
                #include "unit_tests/array.das.register.inc"
            }
            {   using namespace test_auto_infer;
                #include "unit_tests/auto_infer.das.register.inc"
            }
        }
    }
}
