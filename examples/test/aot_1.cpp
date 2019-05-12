#include "daScript/daScript.h"

#include "daScript/simulate/aot.h"

#include "unitTest.h"

// this is how we disable AOT
#if USE_AOT

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)   // unreferenced formal parameter
#pragma warning(disable:4189)   // local variable is initialized but not referenced
#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wwritable-strings"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunsequenced"
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
        namespace test_clone {
            #include "unit_tests/clone.das.main.inc"
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
        namespace test_deref_ptr_fun {
            #include "unit_tests/deref_ptr_fun.das.main.inc"
        }
        namespace test_dynamic_array {
            #include "unit_tests/dynamic_array.das.main.inc"
        }
        namespace test_enumdas {
            #include "unit_tests/enum.das.main.inc"
        }
        namespace test_finally {
            #include "unit_tests/finally.das.main.inc"
        }
        namespace test_foo_bar {
            #include "unit_tests/foo_bar.das.main.inc"
        }
        namespace test_for_loop {
            #include "unit_tests/for-loop.das.main.inc"
        }
        namespace test_func_addr {
            #include "unit_tests/func_addr.das.main.inc"
        }

        void registerAot_1 ( AotLibrary & aotLib )
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
            {   using namespace test_clone;
                #include "unit_tests/clone.das.register.inc"
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
            {   using namespace test_deref_ptr_fun;
                #include "unit_tests/deref_ptr_fun.das.register.inc"
            }
            {   using namespace test_dynamic_array;
                #include "unit_tests/dynamic_array.das.register.inc"
            }
            {   using namespace test_enumdas;
                #include "unit_tests/enum.das.register.inc"
            }
            {   using namespace test_finally;
                #include "unit_tests/finally.das.register.inc"
            }
            {   using namespace test_foo_bar;
                #include "unit_tests/foo_bar.das.register.inc"
            }
            {   using namespace test_for_loop;
                #include "unit_tests/for-loop.das.register.inc"
            }
            {   using namespace test_func_addr;
                #include "unit_tests/func_addr.das.register.inc"
            }
        }
    }
}

#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif

