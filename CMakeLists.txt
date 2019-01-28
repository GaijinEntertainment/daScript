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

set(GEN_FILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
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
src/ast/ast_plot.cpp
src/ast/ast_infer_type.cpp
src/ast/ast_allocate_stack.cpp
src/ast/ast_const_folding.cpp
src/ast/ast_block_folding.cpp
src/ast/ast_unused.cpp
src/ast/ast_annotations.cpp
src/ast/ast_export.cpp
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
src/builtin/module_builtin_int64.cpp
src/builtin/module_builtin_runtime.cpp
src/builtin/module_builtin_vector.cpp
src/builtin/module_builtin_matrix.cpp
src/builtin/module_builtin_array.cpp
src/builtin/module_builtin_time.cpp
src/builtin/module_builtin_das.cpp
src/builtin/module_builtin_math.cpp
src/builtin/module_builtin_string.cpp
src/builtin/builtin.das.inc
src/builtin/builtin.das
)
list(SORT BUILTIN_SRC)
SOURCE_GROUP_FILES("module builtin" BUILTIN_SRC)
XXD(src/builtin/builtin.das)

SET(MISC_SRC
include/daScript/misc/enums.h
include/daScript/misc/function_traits.h
include/daScript/misc/hal.h
include/daScript/misc/platform.h
include/daScript/misc/vectypes.h
include/daScript/misc/arraytype.h
include/daScript/misc/rangetype.h
)
list(SORT MISC_SRC)
SOURCE_GROUP_FILES("misc" MISC_SRC)

SET(SIMULATE_SRC
src/simulate/hash.cpp
src/simulate/debug_info.cpp
src/simulate/runtime_string.cpp
src/simulate/runtime_array.cpp
src/simulate/runtime_table.cpp
src/simulate/runtime_range.cpp
src/simulate/runtime_profile.cpp
src/simulate/simulate.cpp
include/daScript/simulate/cast.h
include/daScript/simulate/hash.h
include/daScript/simulate/heap.h
include/daScript/simulate/debug_info.h
include/daScript/simulate/interop.h
include/daScript/simulate/runtime_string.h
include/daScript/simulate/runtime_array.h
include/daScript/simulate/runtime_table.h
include/daScript/simulate/runtime_range.h
include/daScript/simulate/runtime_profile.h
include/daScript/simulate/simulate.h
include/daScript/simulate/sim_policy.h
)
list(SORT SIMULATE_SRC)
SOURCE_GROUP_FILES("simulate" SIMULATE_SRC)

SET(MAIN_SRC
include/daScript/daScript.h
)
SOURCE_GROUP_FILES("main" MAIN_SRC)

include_directories(include ".")
ADD_LIBRARY(libDaScript ${VECMATH_SRC} ${AST_SRC} ${BUILTIN_SRC} ${MISC_SRC} ${SIMULATE_SRC} 
	${TEST_SRC} ${MAIN_SRC} ${PARSER_SRC} ${PARSER_GENERATED_SRC})
SETUP_CPP11(libDaScript)

add_custom_target(daScriptGenDir ${CMAKE_COMMAND} -E make_directory ${GEN_FILE_DIR})
add_dependencies(libDaScript daScriptGenDir)

# JSON

SET(JSON_MAIN
json/json.cpp
json/json.das.inc
json/json.das
)
XXD(json/json.das)

ADD_LIBRARY(libDaScriptJson ${JSON_MAIN})
SETUP_CPP11(libDaScriptJson)
ADD_DEPENDENCIES(libDaScriptJson libDaScript)

# JSON Example

SET(JSON_MAIN_SRC
json/main.cpp
json/test.das
)

include_directories(include json)
add_executable(jsonTest ${JSON_MAIN_SRC} )
TARGET_LINK_LIBRARIES(jsonTest libDaScript libDaScriptJson)
ADD_DEPENDENCIES(jsonTest libDaScript libDaScriptJson)
SETUP_CPP11(jsonTest)

# Stand alone command line compiler

SET(DASCRIPT_MAIN_SRC
daScript/main.cpp
)

include_directories(include lexer test)
add_executable(daScript ${DASCRIPT_MAIN_SRC} )
TARGET_LINK_LIBRARIES(daScript libDaScript)
ADD_DEPENDENCIES(daScript libDaScript)
SETUP_CPP11(daScript)

# Test

file(GLOB UNIT_TEST_SRC
"test/unit_tests/*.das"
)
list(SORT UNIT_TEST_SRC)
SOURCE_GROUP_FILES("test/unit" UNIT_TEST_SRC)

file(GLOB COMPILATION_FAIL_TEST_SRC
"test/compilation_fail_tests/*.das"
)
list(SORT COMPILATION_FAIL_TEST_SRC)
SOURCE_GROUP_FILES("test/compilation fail" COMPILATION_FAIL_TEST_SRC)

file(GLOB OPTIMIZATION_SRC
"test/optimizations/*.das"
)
list(SORT OPTIMIZATION_SRC)
SOURCE_GROUP_FILES("test/optimizations" OPTIMIZATION_SRC)

file(GLOB RUNTIME_ERRORS_SRC
"test/runtime_errors/*.das"
)
list(SORT RUNTIME_ERRORS_SRC)
SOURCE_GROUP_FILES("test/runtime_errors" RUNTIME_ERRORS_SRC)

file(GLOB MIX_TEST_SRC
"test/*.das"
)
list(SORT MIX_TEST_SRC)
SOURCE_GROUP_FILES("test/mix" MIX_TEST_SRC)

SET(TEST_MAIN_SRC
test/main.cpp
test/test_handles.cpp
)
list(SORT TEST_MAIN_SRC)
SOURCE_GROUP_FILES("main" TEST_MAIN_SRC)

include_directories(include lexer test)
add_executable(daScriptTest ${UNIT_TEST_SRC} ${COMPILATION_FAIL_TEST_SRC} ${MIX_TEST_SRC}
    ${TEST_MAIN_SRC} ${OPTIMIZATION_SRC} ${RUNTIME_ERRORS_SRC})
TARGET_LINK_LIBRARIES(daScriptTest libDaScript)
ADD_DEPENDENCIES(daScriptTest libDaScript)
SETUP_CPP11(daScriptTest)

# Profile

file(GLOB PROFILE_SRC
"profile/tests/*.das"
)
list(SORT PROFILE_SRC)
SOURCE_GROUP_FILES("profile" PROFILE_SRC)

SET(PROFILE_MAIN_SRC
profile/main.cpp
profile/test_profile.cpp
profile/test_profile.h
)
list(SORT PROFILE_MAIN_SRC)
SOURCE_GROUP_FILES("main" PROFILE_MAIN_SRC)

include_directories(include lexer)
add_executable(daScriptProfile ${PROFILE_SRC} ${PROFILE_MAIN_SRC} )
TARGET_LINK_LIBRARIES(daScriptProfile libDaScript)
ADD_DEPENDENCIES(daScriptProfile libDaScript)
SETUP_CPP11(daScriptProfile)

