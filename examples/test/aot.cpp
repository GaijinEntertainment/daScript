#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "unitTest.h"

namespace das {
    namespace aot {
        namespace test_array {
            #include "unit_tests/array.das.main.inc"
        }
        namespace test_auto_infer {
            #include "unit_tests/auto_infer.das.main.inc"
        }
        namespace test_new_delete {
            #include "unit_tests/new_delete.das.main.inc"
        }
        namespace test_new_and_init {
            #include "unit_tests/new_and_init.das.main.inc"
        }
        void registerAot ( AotLibrary & aotLib )
        {
            {   using namespace test_array;
                #include "unit_tests/array.das.register.inc"
            }
            {   using namespace test_auto_infer;
                #include "unit_tests/auto_infer.das.register.inc"
            }
            {   using namespace test_new_delete;
                #include "unit_tests/new_delete.das.register.inc"
            }
            {   using namespace test_new_and_init;
                #include "unit_tests/new_and_init.das.register.inc"
            }
        }
    }
}
