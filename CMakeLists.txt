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
ast/ast.cpp
ast/ast.h
ast/builtin.cpp
)
SOURCE_GROUP_FILES("ast" AST_SRC)

SET(MISC_SRC
misc/enums.h
misc/function_traits.h
misc/platform.h
misc/vectypes.h
misc/precomp.h
misc/precomp.cpp
)
SOURCE_GROUP_FILES("misc" MISC_SRC)

SET(SIMULATE_SRC
simulate/cast.h
simulate/debug_info.cpp
simulate/debug_info.h
simulate/interop.h
simulate/runtime_string.cpp
simulate/runtime_string.h
simulate/simulate.cpp
simulate/simulate.h
)
SOURCE_GROUP_FILES("simulate" SIMULATE_SRC)

SET(TEST_SRC
test/profile_array_of_structures.das
test/profile_array_of_structures_vec.das
)
SOURCE_GROUP_FILES("test" TEST_SRC)

SET(MAIN_SRC
yzg/main.cpp
)

include_directories(yzg PUBLIC ast lexer misc simulate test yzg)

add_executable(yzg ${AST_SRC} ${MISC_SRC} ${SIMULATE_SRC} ${TEST_SRC} ${MAIN_SRC} ${PARSER_SRC} ${PARSER_GENERATED_SRC})
SETUP_CPP11(yzg)
ADD_PRECOMPILED_HEADER(yzg misc/precomp.h misc/precomp.h)

add_custom_target(yzgGenDir ${CMAKE_COMMAND} -E make_directory ${GEN_FILE_DIR})
add_dependencies(yzg yzgGenDir)
