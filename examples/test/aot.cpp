#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "unitTest.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)    // unreferenced formal parameter
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wwritable-strings"
#endif

namespace das {
    namespace aot {
        namespace test_array {
            #include "unit_tests/array.das.main.inc"
        }
        namespace test_auto_infer {
            #include "unit_tests/auto_infer.das.main.inc"
        }
        namespace test_bin_serializer {
            #include "unit_tests/bin_serializer.das.main.inc"
        }
        namespace test_block {
            #include "unit_tests/block.das.main.inc"
        }
        namespace test_block_access_function_arg {
            #include "unit_tests/block_access_function_arg.das.main.inc"
        }
        namespace test_cast {
            #include "unit_tests/cast.das.main.inc"
        }
        namespace test_copy_and_move_on_return {
            #include "unit_tests/copy_and_move_on_return.das.main.inc"
        }
        namespace test_ctor {
            #include "unit_tests/ctor.das.main.inc"
        }
        namespace test_dasstring {
           #include "unit_tests/das_string.das.main.inc"
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
            {   using namespace test_bin_serializer;
                #include "unit_tests/bin_serializer.das.register.inc"
            }
            {   using namespace test_block;
                #include "unit_tests/block.das.register.inc"
            }
            {   using namespace test_block_access_function_arg;
                #include "unit_tests/block_access_function_arg.das.register.inc"
            }
            {   using namespace test_cast;
                #include "unit_tests/cast.das.register.inc"
            }
            {   using namespace test_copy_and_move_on_return;
                #include "unit_tests/copy_and_move_on_return.das.register.inc"
            }
            {   using namespace test_ctor;
                #include "unit_tests/ctor.das.register.inc"
            }
            {   using namespace test_dasstring;
                #include "unit_tests/das_string.das.register.inc"
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

#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

