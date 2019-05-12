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
        namespace test_named_call {
            #include "unit_tests/named_call.das.main.inc"
        }
        namespace test_new_and_init {
            #include "unit_tests/new_and_init.das.main.inc"
        }
        namespace test_new_delete {
            #include "unit_tests/new_delete.das.main.inc"
        }
        namespace test_new_with_init {
            #include "unit_tests/new_with_init.das.main.inc"
        }
        namespace test_operator_overload {
            #include "unit_tests/operator_overload.das.main.inc"
        }
        namespace test_operators {
            #include "unit_tests/operators.das.main.inc"
        }
        namespace test_override {
            #include "unit_tests/override.das.main.inc"
        }
        namespace test_return_reference {
            #include "unit_tests/return_reference.das.main.inc"
        }
        namespace test_rpipe {
            #include "unit_tests/rpipe.das.main.inc"
        }
        namespace test_static_if {
            #include "unit_tests/static_if.das.main.inc"
        }
        namespace test_string {
            #include "unit_tests/string.das.main.inc"
        }
        namespace test_string_builder {
            #include "unit_tests/string_builder.das.main.inc"
        }
        namespace test_structure {
            #include "unit_tests/struct.das.main.inc"
        }
        namespace test_structure_annotation {
            #include "unit_tests/structure_annotation.das.main.inc"
        }
        namespace test_swizze {
            #include "unit_tests/swizze.das.main.inc"
        }

        void registerAot_3 ( AotLibrary & aotLib )
        {
            {   using namespace test_named_call;
                #include "unit_tests/named_call.das.register.inc"
            }
            {   using namespace test_new_and_init;
                #include "unit_tests/new_and_init.das.register.inc"
            }
            {   using namespace test_new_delete;
                #include "unit_tests/new_delete.das.register.inc"
            }
            {   using namespace test_new_with_init;
                #include "unit_tests/new_with_init.das.register.inc"
            }
            {   using namespace test_operator_overload;
                #include "unit_tests/operator_overload.das.register.inc"
            }
            {   using namespace test_operators;
                #include "unit_tests/operators.das.register.inc"
            }
            {   using namespace test_override;
                #include "unit_tests/override.das.register.inc"
            }
            {   using namespace test_return_reference;
                #include "unit_tests/return_reference.das.register.inc"
            }
            {   using namespace test_rpipe;
                #include "unit_tests/rpipe.das.register.inc"
            }
            {   using namespace test_static_if;
                #include "unit_tests/static_if.das.register.inc"
            }
            {   using namespace test_string;
                #include "unit_tests/string.das.register.inc"
            }
            {   using namespace test_string_builder;
                #include "unit_tests/string_builder.das.register.inc"
            }
            {   using namespace test_structure;
                #include "unit_tests/struct.das.register.inc"
            }
            {   using namespace test_structure_annotation;
                #include "unit_tests/structure_annotation.das.register.inc"
            }
            {   using namespace test_swizze;
                #include "unit_tests/swizze.das.register.inc"
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

