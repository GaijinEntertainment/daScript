CMAKE_TEXT_XXD(libDaScriptTest examples/test/unit_test.das)

SET(TEST_MAIN_LIB_SRC
examples/test/bytecode.cpp
examples/test/bytecode.h
examples/test/test_handles.cpp
examples/test/test_enum.cpp
examples/test/module_unitTest.h
examples/test/unitTest.h
examples/test/unit_test.das.inc
examples/test/unit_test.das
)
SOURCE_GROUP_FILES("source" TEST_MAIN_LIB_SRC)


ADD_MODULE_CPP(UnitTest)
ADD_MODULE_LIB(libDaScriptTest UnitTest ${TEST_MAIN_LIB_SRC})
SETUP_CPP11(libDaScriptTest)
ADD_PROJECT_XXD_DEPENDS(libDaScriptTest)
