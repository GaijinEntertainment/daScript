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
        namespace test_global {
            #include "unit_tests/global.das.main.inc"
        }
        namespace test_handle {
            #include "unit_tests/handle.das.main.inc"
        }
        namespace test_include {
            #include "unit_tests/include.das.main.inc"
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
            {   using namespace test_global;
                #include "unit_tests/global.das.register.inc"
            }
            {   using namespace test_handle;
                #include "unit_tests/handle.das.register.inc"
            }
            {   using namespace test_include;
                #include "unit_tests/include.das.register.inc"
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
        }
    }
}

#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

