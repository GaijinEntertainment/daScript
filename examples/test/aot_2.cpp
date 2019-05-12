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
        namespace test_global {
            #include "unit_tests/global.das.main.inc"
        }
        namespace test_handle {
            #include "unit_tests/handle.das.main.inc"
        }
        namespace test_include {
            #include "unit_tests/include.das.main.inc"
        }
        namespace test_init_script {
            #include "unit_tests/init_script.das.main.inc"
        }
        namespace test_infer_alias_and_alias_ctor {
            #include "unit_tests/infer_alias_and_alias_ctor.das.main.inc"
        }
        namespace test_infer_alias_argument {
            #include "unit_tests/infer_alias_argument.das.main.inc"
        }
        namespace test_infer_remove_ref_const {
            #include "unit_tests/infer_remove_ref_const.das.main.inc"
        }
        namespace test_int2_crash {
            #include "unit_tests/int2_crash.das.main.inc"
        }
        namespace test_int_types {
            #include "unit_tests/int_types.das.main.inc"
        }
        namespace test_lambda {
            #include "unit_tests/lambda.das.main.inc"
        }
        namespace test_local_reference {
            #include "unit_tests/local_reference.das.main.inc"
        }
        namespace test_make_local {
            #include "unit_tests/make_local.das.main.inc"
        }
        namespace test_mat_crash {
            #include "unit_tests/mat_crash.das.main.inc"
        }
        namespace test_mat_ctors {
            #include "unit_tests/mat_ctors.das.main.inc"
        }
        namespace test_math {
            #include "unit_tests/math.das.main.inc"
        }
        namespace test_method_semantic {
            #include "unit_tests/method_semantic.das.main.inc"
        }
        namespace test_module {
            #include "unit_tests/module.das.main.inc"
        }

        void registerAot_2 ( AotLibrary & aotLib )
        {
            {   using namespace test_global;
                #include "unit_tests/global.das.register.inc"
            }
            {   using namespace test_handle;
                #include "unit_tests/handle.das.register.inc"
            }
            {   using namespace test_include;
                #include "unit_tests/include.das.register.inc"
            }
            {   using namespace test_init_script;
                #include "unit_tests/init_script.das.register.inc"
            }
            {   using namespace test_infer_alias_and_alias_ctor;
                #include "unit_tests/infer_alias_and_alias_ctor.das.register.inc"
            }
            {   using namespace test_infer_alias_argument;
                #include "unit_tests/infer_alias_argument.das.register.inc"
            }
            {   using namespace test_infer_remove_ref_const;
                #include "unit_tests/infer_remove_ref_const.das.register.inc"
            }
            {   using namespace test_int2_crash;
                #include "unit_tests/int2_crash.das.register.inc"
            }
            {   using namespace test_int_types;
                #include "unit_tests/int_types.das.register.inc"
            }
            {   using namespace test_lambda;
                #include "unit_tests/lambda.das.register.inc"
            }
            {   using namespace test_local_reference;
                #include "unit_tests/local_reference.das.register.inc"
            }
            {   using namespace test_make_local;
                #include "unit_tests/make_local.das.register.inc"
            }
            {   using namespace test_mat_crash;
                #include "unit_tests/mat_crash.das.register.inc"
            }
            {   using namespace test_mat_ctors;
                #include "unit_tests/mat_ctors.das.register.inc"
            }
            {   using namespace test_math;
                #include "unit_tests/math.das.register.inc"
            }
            {   using namespace test_method_semantic;
                #include "unit_tests/method_semantic.das.register.inc"
            }
            {   using namespace test_module;
                #include "unit_tests/module.das.register.inc"
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

