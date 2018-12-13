cmake_minimum_required (VERSION 2.6)
project (YZG)

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
ast/compilation_errors.h
ast/ast.cpp
ast/ast.h
ast/ast_interop.h
ast/builtin.cpp
ast/builtin_runtime.cpp
)
SOURCE_GROUP_FILES("ast" AST_SRC)

SET(MISC_SRC
misc/enums.h
misc/function_traits.h
misc/platform.h
misc/vectypes.h
misc/arraytype.h
misc/rangetype.h
misc/precomp.h
misc/precomp.cpp
)
SOURCE_GROUP_FILES("misc" MISC_SRC)

SET(SIMULATE_SRC
simulate/cast.h
simulate/hash.h
simulate/hash.cpp
simulate/debug_info.cpp
simulate/debug_info.h
simulate/interop.h
simulate/runtime_string.cpp
simulate/runtime_string.h
simulate/runtime_array.cpp
simulate/runtime_array.h
simulate/runtime_table.cpp
simulate/runtime_table.h
simulate/runtime_range.cpp
simulate/runtime_range.h
simulate/simulate.cpp
simulate/simulate.h
)
SOURCE_GROUP_FILES("simulate" SIMULATE_SRC)

include_directories(libDaScript PUBLIC ast misc simulate)
ADD_LIBRARY(libDaScript ${AST_SRC} ${MISC_SRC} ${SIMULATE_SRC} ${TEST_SRC} ${MAIN_SRC} ${PARSER_SRC} ${PARSER_GENERATED_SRC})
SETUP_CPP11(libDaScript)
ADD_PRECOMPILED_HEADER(libDaScript misc/precomp.h misc/precomp.h)

add_custom_target(daScriptGenDir ${CMAKE_COMMAND} -E make_directory ${GEN_FILE_DIR})
add_dependencies(libDaScript daScriptGenDir)

# Stand alone command line compiler

SET(DASCRIPT_MAIN_SRC
daScript/main.cpp
)

include_directories(daScript PUBLIC ast lexer misc simulate test)
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

file(GLOB MIX_TEST_SRC
"test/*.das"
)
list(SORT MIX_TEST_SRC)
SOURCE_GROUP_FILES("test/mix" MIX_TEST_SRC)

SET(TEST_MAIN_SRC
test/main.cpp
)
SOURCE_GROUP_FILES("main" TEST_MAIN_SRC)

include_directories(daScriptTest PUBLIC ast lexer misc simulate test)
add_executable(daScriptTest ${UNIT_TEST_SRC} ${COMPILATION_FAIL_TEST_SRC} ${MIX_TEST_SRC} ${TEST_MAIN_SRC} )
TARGET_LINK_LIBRARIES(daScriptTest libDaScript)
ADD_DEPENDENCIES(daScriptTest libDaScript)
SETUP_CPP11(daScriptTest)

# Profile

SET(PROFILE_SRC
profile/profile_array_of_structures_vec.das
profile/profile_try_catch.das
)
SOURCE_GROUP_FILES("profile" PROFILE_SRC)

SET(PROFILE_MAIN_SRC
profile/main.cpp
)

include_directories(daScriptProfile PUBLIC ast lexer misc simulate test)
add_executable(daScriptProfile ${PROFILE_SRC} ${PROFILE_MAIN_SRC} )
TARGET_LINK_LIBRARIES(daScriptProfile libDaScript)
ADD_DEPENDENCIES(daScriptProfile libDaScript)
SETUP_CPP11(daScriptProfile)

