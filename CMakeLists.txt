cmake_minimum_required (VERSION 2.6)
project (DAS)

INCLUDE(./CMakeCommon.txt)

IF(MSVC)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
ENDIF()

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

FIND_XXD()

SETUP_COMPILER()

set(GEN_FILE_DIR ${PROJECT_SOURCE_DIR}/generated)
BISON_TARGET(DSParser src/parser/ds_parser.ypp ${GEN_FILE_DIR}/ds_parser.cpp)
FLEX_TARGET(DSScanner src/parser/ds_lexer.lpp  ${GEN_FILE_DIR}/ds_lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(DSScanner DSParser)

SET(PARSER_GENERATED_SRC
${BISON_DSParser_OUTPUTS}
${FLEX_DSScanner_OUTPUTS}
)

SET(PARSER_SRC
src/parser/ds_parser.ypp
src/parser/ds_lexer.lpp
)
list(SORT PARSER_SRC)
SOURCE_GROUP_FILES("parser" PARSER_SRC)
SOURCE_GROUP_FILES("parser/generated" PARSER_GENERATED_SRC)

IF(MSVC)
    # Don't use precompiled headers with flex/bison generated files
    FOREACH(file ${PARSER_GENERATED_SRC})
        SET_SOURCE_FILES_PROPERTIES(${file} PROPERTIES COMPILE_FLAGS "/Y-" )
    ENDFOREACH()
ENDIF()

SET(VECMATH_SRC
include/vecmath/dag_vecMath.h
include/vecmath/dag_vecMathDecl.h
include/vecmath/dag_vecMath_common.h
include/vecmath/dag_vecMath_const.h
include/vecmath/dag_vecMath_neon.h
include/vecmath/dag_vecMath_pc_sse.h
)
list(SORT VECMATH_SRC)
SOURCE_GROUP_FILES("vecmath" VECMATH_SRC)

SET(AST_SRC
src/ast/ast.cpp
src/ast/ast_generate.cpp
src/ast/ast_simulate.cpp
src/ast/ast_typedecl.cpp
src/ast/ast_match.cpp
src/ast/ast_module.cpp
src/ast/ast_print.cpp
src/ast/ast_aot_cpp.cpp
src/ast/ast_plot.cpp
src/ast/ast_infer_type.cpp
src/ast/ast_allocate_stack.cpp
src/ast/ast_const_folding.cpp
src/ast/ast_block_folding.cpp
src/ast/ast_unused.cpp
src/ast/ast_annotations.cpp
src/ast/ast_export.cpp
src/ast/ast_parse.cpp
src/ast/ast_debug_info_helper.cpp
include/daScript/ast/compilation_errors.h
include/daScript/ast/ast.h
include/daScript/ast/ast_generate.h
include/daScript/ast/ast_match.h
include/daScript/ast/ast_interop.h
include/daScript/ast/ast_handle.h
include/daScript/ast/ast_policy_types.h
)
list(SORT AST_SRC)
SOURCE_GROUP_FILES("ast" AST_SRC)

SET(BUILTIN_SRC
src/builtin/module_builtin.h
src/builtin/module_builtin.cpp
src/builtin/module_builtin_runtime.cpp
src/builtin/module_builtin_vector.cpp
src/builtin/module_builtin_matrix.cpp
src/builtin/module_builtin_array.cpp
src/builtin/module_builtin_time.cpp
src/builtin/module_builtin_das.cpp
src/builtin/module_builtin_math.cpp
src/builtin/module_builtin_random.cpp
src/builtin/module_builtin_string.cpp
src/builtin/builtin.das.inc
src/builtin/builtin.das
)
list(SORT BUILTIN_SRC)
SOURCE_GROUP_FILES("module builtin" BUILTIN_SRC)
XXD(src/builtin/builtin.das)

IF(MSVC)
set_source_files_properties(src/builtin/module_builtin_vector.cpp PROPERTIES COMPILE_FLAGS /bigobj)
ENDIF()

SET(MISC_SRC
include/daScript/misc/enums.h
include/daScript/misc/function_traits.h
include/daScript/misc/hal.h
include/daScript/misc/platform.h
include/daScript/misc/vectypes.h
include/daScript/misc/arraytype.h
include/daScript/misc/rangetype.h
include/daScript/misc/string_writer.h
include/daScript/misc/type_name.h
src/misc/string_writer.cpp
)
list(SORT MISC_SRC)
SOURCE_GROUP_FILES("misc" MISC_SRC)

SET(SIMULATE_SRC
src/hal/performance_time.cpp
src/hal/debug_break.cpp
src/simulate/hash.cpp
src/simulate/debug_info.cpp
src/simulate/runtime_string.cpp
src/simulate/runtime_array.cpp
src/simulate/runtime_table.cpp
src/simulate/runtime_range.cpp
src/simulate/runtime_profile.cpp
src/simulate/simulate.cpp
src/simulate/simulate_visit.cpp
src/simulate/simulate_print.cpp
src/simulate/simulate_fn_hash.cpp
include/daScript/simulate/cast.h
include/daScript/simulate/hash.h
include/daScript/simulate/heap.h
include/daScript/simulate/buddy.h
src/simulate/heap.cpp
include/daScript/simulate/debug_info.h
include/daScript/simulate/interop.h
include/daScript/simulate/runtime_string.h
include/daScript/simulate/runtime_string_delete.h
include/daScript/simulate/runtime_array.h
include/daScript/simulate/runtime_table.h
include/daScript/simulate/runtime_range.h
include/daScript/simulate/runtime_profile.h
include/daScript/simulate/runtime_matrices.h
include/daScript/simulate/simulate.h
include/daScript/simulate/simulate_visit.h
include/daScript/simulate/simulate_visit_op.h
include/daScript/simulate/simulate_visit_op_undef.h
include/daScript/simulate/sim_policy.h
src/simulate/data_walker.cpp
include/daScript/simulate/data_walker.h
src/simulate/debug_print.cpp
include/daScript/simulate/debug_print.h
include/daScript/simulate/for_each.h
include/daScript/simulate/bind_enum.h
include/daScript/simulate/bin_serializer.h
src/simulate/bin_serializer.cpp
include/daScript/simulate/aot.h
include/daScript/simulate/aot_builtin.h
include/daScript/simulate/aot_builtin_math.h
include/daScript/simulate/aot_builtin_matrix.h
include/daScript/simulate/aot_builtin_random.h
include/daScript/simulate/aot_builtin_time.h
include/daScript/simulate/aot_builtin_string.h
)
list(SORT SIMULATE_SRC)
SOURCE_GROUP_FILES("simulate" SIMULATE_SRC)

SET(MAIN_SRC
include/daScript/daScript.h
include/daScript/das_config.h
)
SOURCE_GROUP_FILES("main" MAIN_SRC)

include_directories(include ".")
ADD_LIBRARY(libDaScript ${VECMATH_SRC} ${AST_SRC} ${BUILTIN_SRC} ${MISC_SRC} ${SIMULATE_SRC} 
    ${TEST_SRC} ${MAIN_SRC} ${PARSER_SRC} ${PARSER_GENERATED_SRC})
SETUP_CPP11(libDaScript)

add_custom_target(daScriptGenDir ${CMAKE_COMMAND} -E make_directory ${GEN_FILE_DIR})
add_dependencies(libDaScript daScriptGenDir)

# AOT compiler

SET(DASAOT_MAIN_SRC
examples/dasAot/main.cpp
examples/common/fileAccess.h
)
SOURCE_GROUP_FILES("main" DASAOT_MAIN_SRC)

include_directories(include lexer test)
add_executable(dasAot ${DASAOT_MAIN_SRC} )
TARGET_LINK_LIBRARIES(dasAot libDaScript libPathTracerHelper libDaScriptProfile libDaScriptTest)
ADD_DEPENDENCIES(dasAot libDaScript libPathTracerHelper libDaScriptProfile libDaScriptTest)
SETUP_CPP11(dasAot)

MACRO(DAS_AOT input genList)
    get_filename_component(input_src ${input} ABSOLUTE)
    get_filename_component(input_dir ${input_src} DIRECTORY)
    get_filename_component(input_name ${input} NAME)
	ADD_CUSTOM_COMMAND(
		DEPENDS ${input_src}
        DEPENDS dasAot
		OUTPUT ${input_src}.main.inc ${input_src}.register.inc
        COMMAND dasAot ${input_src} ${input_src}.main.inc ${input_src}.register.inc
		COMMENT "AOT precompiling ${input_src} absolute")
    list(APPEND ${genList} ${input_src}.main.inc)
    list(APPEND ${genList} ${input_src}.register.inc)
ENDMACRO()

# JSON

SET(JSON_MAIN
examples/json/json.cpp
examples/json/json.das.inc
examples/json/json.das
)
XXD(examples/json/json.das)

ADD_LIBRARY(libDaScriptJson ${JSON_MAIN})
SETUP_CPP11(libDaScriptJson)
ADD_DEPENDENCIES(libDaScriptJson libDaScript)

# PathTracer helper

SET(PATH_TRACER_HELPER_MAIN_SRC
examples/pathTracer/path_tracer_helper.h
examples/pathTracer/path_tracer_helper.cpp
)

ADD_LIBRARY(libPathTracerHelper ${PATH_TRACER_HELPER_MAIN_SRC})
SETUP_CPP11(libPathTracerHelper)
ADD_DEPENDENCIES(libPathTracerHelper libDaScript)

# PathTracer

SET(PATH_TRACER_MAIN_SRC
examples/pathTracer/main.cpp
examples/common/fileAccess.h
examples/pathTracer/path_tracer_helper.h
examples/pathTracer/toy_random.das
examples/pathTracer/toy_path_tracer.das
examples/pathTracer/aot.cpp
)
SOURCE_GROUP_FILES("source" PATH_TRACER_MAIN_SRC)

SET(PATH_TRACER_GEN_SRC)
DAS_AOT(examples/pathTracer/toy_path_tracer.das PATH_TRACER_GEN_SRC)
SOURCE_GROUP_FILES("generated" PATH_TRACER_GEN_SRC)

include_directories(include pathTracer)
add_executable(pathTracer ${PATH_TRACER_MAIN_SRC} ${PATH_TRACER_GEN_SRC})
TARGET_LINK_LIBRARIES(pathTracer libDaScript libPathTracerHelper)
IF (UNIX)
  TARGET_LINK_LIBRARIES(pathTracer pthread)
ENDIF()
ADD_DEPENDENCIES(pathTracer libDaScript libPathTracerHelper)
SETUP_CPP11(pathTracer)

# JSON Example

SET(JSON_MAIN_SRC
examples/json/main.cpp
examples/common/fileAccess.h
examples/json/test.das
examples/json/test_json.das
)

include_directories(include json)
add_executable(jsonTest ${JSON_MAIN_SRC} )
TARGET_LINK_LIBRARIES(jsonTest libDaScript libDaScriptJson)
ADD_DEPENDENCIES(jsonTest libDaScript libDaScriptJson)
SETUP_CPP11(jsonTest)

# Stand alone command line compiler

SET(DASCRIPT_MAIN_SRC
examples/daScript/main.cpp
examples/common/fileAccess.h
)
SOURCE_GROUP_FILES("main" DASCRIPT_MAIN_SRC)

include_directories(include lexer test)
add_executable(daScript ${DASCRIPT_MAIN_SRC} )
TARGET_LINK_LIBRARIES(daScript libDaScript)
ADD_DEPENDENCIES(daScript libDaScript)
SETUP_CPP11(daScript)

# Test module

SET(TEST_MAIN_LIB_SRC
examples/test/test_handles.cpp
examples/test/test_enum.cpp
examples/test/module_unitTest.h
examples/test/unitTest.h
)
SOURCE_GROUP_FILES("source" TEST_MAIN_LIB_SRC)

ADD_LIBRARY(libDaScriptTest ${TEST_MAIN_LIB_SRC})
SETUP_CPP11(libDaScriptTest)
ADD_DEPENDENCIES(libDaScriptTest libDaScript)

# Test

file(GLOB UNIT_TEST_SRC
"examples/test/unit_tests/*.das"
)
list(SORT UNIT_TEST_SRC)
SOURCE_GROUP_FILES("examples/test/unit" UNIT_TEST_SRC)

file(GLOB COMPILATION_FAIL_TEST_SRC
"examples/test/compilation_fail_tests/*.das"
)
list(SORT COMPILATION_FAIL_TEST_SRC)
SOURCE_GROUP_FILES("examples/test/compilation fail" COMPILATION_FAIL_TEST_SRC)

file(GLOB OPTIMIZATION_SRC
"examples/test/optimizations/*.das"
)
list(SORT OPTIMIZATION_SRC)
SOURCE_GROUP_FILES("examples/test/optimizations" OPTIMIZATION_SRC)

file(GLOB RUNTIME_ERRORS_SRC
"examples/test/runtime_errors/*.das"
)
list(SORT RUNTIME_ERRORS_SRC)
SOURCE_GROUP_FILES("examples/test/runtime_errors" RUNTIME_ERRORS_SRC)

file(GLOB MIX_TEST_SRC
"examples/test/*.das"
)
list(SORT MIX_TEST_SRC)
SOURCE_GROUP_FILES("examples/test/mix" MIX_TEST_SRC)

SET(TEST_MAIN_SRC
examples/test/main.cpp
examples/common/fileAccess.h
examples/test/unitTest.h
examples/test/aot.cpp
)
list(SORT TEST_MAIN_SRC)
SOURCE_GROUP_FILES("source" TEST_MAIN_SRC)

SET(TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/array.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/auto_infer.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/bin_serializer.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/block.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/block_access_function_arg.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/cast.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/copy_and_move_on_return.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/ctor.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/das_string.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/deref_ptr_fun.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/dynamic_array.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/enum.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/finally.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/foo_bar.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/for-loop.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/func_addr.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/global.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/handle.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/include.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/infer_alias_and_alias_ctor.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/infer_alias_argument.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/infer_remove_ref_const.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/int2_crash.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/int_types.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/lambda.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/local_reference.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/make_local.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/mat_crash.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/mat_ctors.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/math.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/method_semantic.das TEST_GENERATED_SRC)

DAS_AOT(examples/test/unit_tests/new_delete.das TEST_GENERATED_SRC)
DAS_AOT(examples/test/unit_tests/new_and_init.das TEST_GENERATED_SRC)
SOURCE_GROUP_FILES("generated" TEST_GENERATED_SRC)

include_directories(include lexer test)
add_executable(daScriptTest ${UNIT_TEST_SRC} ${COMPILATION_FAIL_TEST_SRC} ${MIX_TEST_SRC}
    ${TEST_MAIN_SRC} ${OPTIMIZATION_SRC} ${RUNTIME_ERRORS_SRC} ${TEST_GENERATED_SRC})
TARGET_LINK_LIBRARIES(daScriptTest libDaScript libDaScriptTest)
ADD_DEPENDENCIES(daScriptTest libDaScript libDaScriptTest)
SETUP_CPP11(daScriptTest)

# Profile module

SET(PROFILE_MAIN_LIB_SRC
examples/profile/test_profile.cpp
examples/profile/test_profile.h
)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_LIB_SRC)

ADD_LIBRARY(libDaScriptProfile ${PROFILE_MAIN_LIB_SRC})
SETUP_CPP11(libDaScriptProfile)
ADD_DEPENDENCIES(libDaScriptProfile libDaScript)

# Profile

file(GLOB PROFILE_SRC
"examples/profile/tests/*.das"
)
list(SORT PROFILE_SRC)
SOURCE_GROUP_FILES("profile" PROFILE_SRC)

SET(PROFILE_MAIN_SRC
examples/profile/main.cpp
examples/common/fileAccess.h
examples/profile/test_profile.h
examples/profile/aot.cpp
)
list(SORT PROFILE_MAIN_SRC)
SOURCE_GROUP_FILES("source" PROFILE_MAIN_SRC)

SET(PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/dict.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/exp.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/fib.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/native.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/nbodies.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/particles.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/primes.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/aos.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/panic.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/annotation.das PROFILE_GENERATED_SRC)
DAS_AOT(examples/profile/tests/make_struct.das PROFILE_GENERATED_SRC)
SOURCE_GROUP_FILES("generated" PROFILE_GENERATED_SRC)

include_directories(include lexer)
add_executable(daScriptProfile ${PROFILE_SRC} ${PROFILE_MAIN_SRC} ${PROFILE_GENERATED_SRC})
TARGET_LINK_LIBRARIES(daScriptProfile libDaScript libDaScriptProfile)
ADD_DEPENDENCIES(daScriptProfile libDaScript dasAot libDaScriptProfile)
SETUP_CPP11(daScriptProfile)

