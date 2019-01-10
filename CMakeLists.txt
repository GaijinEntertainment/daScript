cmake_minimum_required (VERSION 2.6)
project (DAS)

INCLUDE(./CMakeCommon.txt)

find_package(BISON)
find_package(FLEX)

SETUP_COMPILER()

set(GEN_FILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
BISON_TARGET(DSParser parser/ds_parser.ypp ${GEN_FILE_DIR}/ds_parser.cpp)
FLEX_TARGET(DSScanner parser/ds_lexer.lpp  ${GEN_FILE_DIR}/ds_lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(DSScanner DSParser)

SET(PARSER_GENERATED_SRC
${BISON_DSParser_OUTPUTS}
${FLEX_DSScanner_OUTPUTS}
)

SET(PARSER_SRC
parser/ds_parser.ypp
parser/ds_lexer.lpp
)
SOURCE_GROUP_FILES("parser" PARSER_SRC)
SOURCE_GROUP_FILES("parser/generated" PARSER_GENERATED_SRC)

IF(MSVC)
	# Don't use precompiled headers with flex/bison generated files
	FOREACH(file ${PARSER_GENERATED_SRC})
		SET_SOURCE_FILES_PROPERTIES(${file} PROPERTIES COMPILE_FLAGS "/Y-" )
	ENDFOREACH()
ENDIF()

SET(AST_SRC
include/daScript/ast/compilation_errors.h
ast/ast.cpp
ast/ast_module.cpp
ast/ast_print.cpp
ast/ast_plot.cpp
ast/ast_infer_type.cpp
ast/ast_allocate_stack.cpp
ast/ast_const_folding.cpp
ast/ast_block_folding.cpp
ast/ast_unused.cpp
ast/ast_annotations.cpp
include/daScript/ast/ast.h
include/daScript/ast/ast_interop.h
include/daScript/ast/ast_handle.h
include/daScript/ast/ast_policy_types.h
)
SOURCE_GROUP_FILES("ast" AST_SRC)

SET(BUILTIN_SRC
builtin/module_builtin.h
builtin/module_builtin.cpp
builtin/module_builtin_runtime.cpp
builtin/module_builtin_vector.cpp
builtin/module_builtin_time.cpp
)
SOURCE_GROUP_FILES("module builtin" BUILTIN_SRC)

SET(MISC_SRC
include/daScript/misc/enums.h
include/daScript/misc/function_traits.h
include/daScript/misc/hal.h
include/daScript/misc/platform.h
include/daScript/misc/vectypes.h
include/daScript/misc/arraytype.h
include/daScript/misc/rangetype.h
precomp.cpp
)
SOURCE_GROUP_FILES("misc" MISC_SRC)

SET(SIMULATE_SRC
include/daScript/simulate/cast.h
include/daScript/simulate/hash.h
simulate/hash.cpp
simulate/debug_info.cpp
include/daScript/simulate/debug_info.h
include/daScript/simulate/interop.h
simulate/runtime_string.cpp
include/daScript/simulate/runtime_string.h
simulate/runtime_array.cpp
include/daScript/simulate/runtime_array.h
simulate/runtime_table.cpp
include/daScript/simulate/runtime_table.h
simulate/runtime_range.cpp
include/daScript/simulate/runtime_range.h
simulate/runtime_profile.cpp
include/daScript/simulate/runtime_profile.h
simulate/simulate.cpp
include/daScript/simulate/simulate.h
include/daScript/simulate/sim_policy.h
)
SOURCE_GROUP_FILES("simulate" SIMULATE_SRC)

include_directories(include ".")
ADD_LIBRARY(libDaScript ${AST_SRC} ${BUILTIN_SRC} ${MISC_SRC} ${SIMULATE_SRC} ${TEST_SRC} ${MAIN_SRC} ${PARSER_SRC} ${PARSER_GENERATED_SRC})
SETUP_CPP11(libDaScript)
ADD_PRECOMPILED_HEADER(libDaScript platform.h include/daScript/misc/platform.h)

add_custom_target(daScriptGenDir ${CMAKE_COMMAND} -E make_directory ${GEN_FILE_DIR})
add_dependencies(libDaScript daScriptGenDir)

# Stand alone command line compiler

SET(DASCRIPT_MAIN_SRC
daScript/main.cpp
)

include_directories(include lexer test)
add_executable(daScript ${DASCRIPT_MAIN_SRC} )
TARGET_LINK_LIBRARIES(daScript libDaScript)
ADD_DEPENDENCIES(daScript libDaScript)
SETUP_CPP11(daScript)
ADD_PRECOMPILED_HEADER(daScript platform.h include/daScript/misc/platform.h)

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
include/daScript/misc/platform.h
precomp.cpp
test/main.cpp
test/test_handles.cpp
)
SOURCE_GROUP_FILES("main" TEST_MAIN_SRC)

include_directories(include lexer test)
add_executable(daScriptTest ${UNIT_TEST_SRC} ${COMPILATION_FAIL_TEST_SRC} ${MIX_TEST_SRC}
    ${TEST_MAIN_SRC} ${OPTIMIZATION_SRC} ${RUNTIME_ERRORS_SRC})
TARGET_LINK_LIBRARIES(daScriptTest libDaScript)
ADD_DEPENDENCIES(daScriptTest libDaScript)
SETUP_CPP11(daScriptTest)
ADD_PRECOMPILED_HEADER(daScriptTest platform.h include/daScript/misc/platform.h)

# Profile

file(GLOB PROFILE_SRC
"profile/tests/*.das"
)
list(SORT PROFILE_SRC)
SOURCE_GROUP_FILES("profile" PROFILE_SRC)

SET(PROFILE_MAIN_SRC
include/daScript/misc/platform.h
precomp.cpp
profile/main.cpp
profile/test_profile.cpp
)
SOURCE_GROUP_FILES("main" PROFILE_MAIN_SRC)

include_directories(include lexer)
add_executable(daScriptProfile ${PROFILE_SRC} ${PROFILE_MAIN_SRC} )
TARGET_LINK_LIBRARIES(daScriptProfile libDaScript)
ADD_DEPENDENCIES(daScriptProfile libDaScript)
SETUP_CPP11(daScriptProfile)
ADD_PRECOMPILED_HEADER(daScriptProfile platform.h include/daScript/misc/platform.h)

