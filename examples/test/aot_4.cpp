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
        namespace test_table {
            #include "unit_tests/table.das.main.inc"
        }
        namespace test_test_bool_if {
            #include "unit_tests/test_bool_if.das.main.inc"
        }
        namespace test_time {
            #include "unit_tests/time.das.main.inc"
        }
        namespace test_try_catch {
            #include "unit_tests/try-catch.das.main.inc"
        }
        namespace test_typeAlias {
            #include "unit_tests/typeAlias.das.main.inc"
        }
        namespace test_typename {
            #include "unit_tests/typename.das.main.inc"
        }
        namespace test_types {
            #include "unit_tests/types.das.main.inc"
        }
        namespace test_vec_ctors {
            #include "unit_tests/vec_ctors.das.main.inc"
        }
        namespace test_vec_index {
            #include "unit_tests/vec_index.das.main.inc"
        }
        namespace test_vec_swizzle {
            #include "unit_tests/vec_swizzle.das.main.inc"
        }
        namespace test_with {
            #include "unit_tests/with.das.main.inc"
        }

        void registerAot_4 ( AotLibrary & aotLib )
        {
            {   using namespace test_table;
                #include "unit_tests/table.das.register.inc"
            }
            {   using namespace test_test_bool_if;
                #include "unit_tests/test_bool_if.das.register.inc"
            }
            {   using namespace test_time;
                #include "unit_tests/time.das.register.inc"
            }
            {   using namespace test_try_catch;
                #include "unit_tests/try-catch.das.register.inc"
            }
            {   using namespace test_typeAlias;
                #include "unit_tests/typeAlias.das.register.inc"
            }
            {   using namespace test_typename;
                #include "unit_tests/typename.das.register.inc"
            }
            {   using namespace test_types;
                #include "unit_tests/types.das.register.inc"
            }
            {   using namespace test_vec_ctors;
                #include "unit_tests/vec_ctors.das.register.inc"
            }
            {   using namespace test_vec_index;
                #include "unit_tests/vec_index.das.register.inc"
            }
            {   using namespace test_vec_swizzle;
                #include "unit_tests/vec_swizzle.das.register.inc"
            }
            {   using namespace test_with;
                #include "unit_tests/with.das.register.inc"
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

