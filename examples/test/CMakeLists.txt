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

file(GLOB UNIT_TEST_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/examples/test/unit_tests/*.das"
)
list(SORT UNIT_TEST_SRC)
SOURCE_GROUP_FILES("unit" UNIT_TEST_SRC)

file(GLOB COMPILATION_FAIL_TEST_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/examples/test/compilation_fail_tests/*.das"
)
list(SORT COMPILATION_FAIL_TEST_SRC)
SOURCE_GROUP_FILES("compilation fail" COMPILATION_FAIL_TEST_SRC)

file(GLOB OPTIMIZATION_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/examples/test/optimizations/*.das"
)
list(SORT OPTIMIZATION_SRC)
SOURCE_GROUP_FILES("optimizations" OPTIMIZATION_SRC)

file(GLOB RUNTIME_ERRORS_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/examples/test/runtime_errors/*.das"
)
list(SORT RUNTIME_ERRORS_SRC)
SOURCE_GROUP_FILES("runtime_errors" RUNTIME_ERRORS_SRC)

file(GLOB_RECURSE MODULE_TEST_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/examples/test/module/*.*"
)
list(SORT MODULE_TEST_SRC)
SOURCE_GROUP_FILES("module" MODULE_TEST_SRC)

SET(TEST_MAIN_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/test/main.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/test/unitTest.h
)
list(SORT TEST_MAIN_SRC)
SOURCE_GROUP_FILES("source" TEST_MAIN_SRC)

add_custom_target(daScriptTestAot)
SET(TEST_GENERATED_SRC)
DAS_AOT("${UNIT_TEST_SRC}" TEST_GENERATED_SRC daScriptTestAot daslang)
UNITIZE_BUILD("examples/test/unit_tests" TEST_GENERATED_SRC)
#DAS_AOT("hello_world.das" TEST_GENERATED_SRC daScriptTestAot daslang)
SOURCE_GROUP_FILES("generated" TEST_GENERATED_SRC)

add_executable(daScriptTest ${TEST_GENERATED_SRC} ${UNIT_TEST_SRC} ${COMPILATION_FAIL_TEST_SRC} ${MODULE_TEST_SRC}
    ${TEST_MAIN_SRC} ${OPTIMIZATION_SRC} ${RUNTIME_ERRORS_SRC})
TARGET_LINK_LIBRARIES(daScriptTest libDaScriptTest libDaScriptAot Threads::Threads ${DAS_MODULES_LIBS})
TARGET_INCLUDE_DIRECTORIES(daScriptTest PRIVATE ${PROJECT_SOURCE_DIR}/examples/test)
ADD_DEPENDENCIES(daScriptTest libDaScriptTest libDaScriptAot ${DAS_MODULES_LIBS})
SETUP_CPP11(daScriptTest)
add_dependencies(daScriptTest daScriptTestAot)
SETUP_LTO(daScriptTest)
#target_precompile_headers(daScriptTest PUBLIC include/daScript/misc/platform.h)

SET(THREADS_MAIN_SRC
${CMAKE_CURRENT_SOURCE_DIR}/examples/test/test_threads.cpp
${CMAKE_CURRENT_SOURCE_DIR}/examples/test/unitTest.h
)

add_executable(daScriptTestThreads ${UNIT_TEST_SRC} ${THREADS_MAIN_SRC}
#    ${TEST_GENERATED_SRC}
)
TARGET_LINK_LIBRARIES(daScriptTestThreads libDaScriptTest libDaScriptAot Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(daScriptTestThreads libDaScriptTest libDaScriptAot ${DAS_MODULES_LIBS})
SETUP_CPP11(daScriptTestThreads)
#add_dependencies(daScriptTest daScriptTestAot)
SETUP_LTO(daScriptTestThreads)
