cmake_minimum_required (VERSION 2.6)
project (YZG)

INCLUDE(./CMakeCommon.txt)

SETUP_COMPILER()

SET(AST_SRC
ast/ast.cpp
ast/ast.hpp
ast/builtin.cpp
)
SOURCE_GROUP_FILES("ast" AST_SRC)

SET(LEXER_SRC
lexer/reader.cpp
lexer/reader.hpp
)
SOURCE_GROUP_FILES("lexer" LEXER_SRC)

SET(MISC_SRC
misc/enums.h
misc/function_traits.h
misc/platform.h
misc/vectypes.h
)
SOURCE_GROUP_FILES("misc" MISC_SRC)

SET(SIMULATE_SRC
simulate/cast.h
simulate/debug_info.cpp
simulate/debug_info.hpp
simulate/interop.h
simulate/runtime_string.cpp
simulate/runtime_string.hpp
simulate/simulate.cpp
simulate/simulate.hpp
)
SOURCE_GROUP_FILES("simulate" SIMULATE_SRC)

SET(TEST_SRC
test/profile_array_of_structures.yzg
test/profile_array_of_structures_vec.yzg
test/test_1.yzg
test/test_2.yzg
test/test_3.yzg
test/test_ref.yzg
test/try_catch.yzg
test/type_string.yzg
)
SOURCE_GROUP_FILES("test" TEST_SRC)

SET(MAIN_SRC
yzg/main.cpp
)

include_directories(yzg PUBLIC ast lexer misc simulate test yzg)

add_executable(yzg ${AST_SRC} ${LEXER_SRC} ${MISC_SRC} ${SIMULATE_SRC} ${TEST_SRC} ${MAIN_SRC})
SETUP_CPP11(yzg)

