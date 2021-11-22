file(GLOB UNIT_TEST_SRC
"unit_tests/*.das"
)
list(SORT UNIT_TEST_SRC)
SOURCE_GROUP_FILES("unit" UNIT_TEST_SRC)

file(GLOB COMPILATION_FAIL_TEST_SRC
"compilation_fail_tests/*.das"
)
list(SORT COMPILATION_FAIL_TEST_SRC)
SOURCE_GROUP_FILES("compilation fail" COMPILATION_FAIL_TEST_SRC)

file(GLOB OPTIMIZATION_SRC
"optimizations/*.das"
)
list(SORT OPTIMIZATION_SRC)
SOURCE_GROUP_FILES("optimizations" OPTIMIZATION_SRC)

file(GLOB RUNTIME_ERRORS_SRC
"runtime_errors/*.das"
)
list(SORT RUNTIME_ERRORS_SRC)
SOURCE_GROUP_FILES("runtime_errors" RUNTIME_ERRORS_SRC)

file(GLOB MIX_TEST_SRC
"*.das"
)
list(SORT MIX_TEST_SRC)
SOURCE_GROUP_FILES("mix" MIX_TEST_SRC)

file(GLOB_RECURSE MODULE_TEST_SRC
"module/*.*"
)
list(SORT MODULE_TEST_SRC)
SOURCE_GROUP_FILES("module" MODULE_TEST_SRC)

SET(TEST_MAIN_SRC
main.cpp
unitTest.h
)
list(SORT TEST_MAIN_SRC)
SOURCE_GROUP_FILES("source" TEST_MAIN_SRC)

add_custom_target(daScriptTestAot)
SET(TEST_GENERATED_SRC)
DAS_AOT_DIR("${UNIT_TEST_SRC}" TEST_GENERATED_SRC daScriptTestAot daScript)
UNITIZE_BUILD("examples/test/unit_tests" TEST_GENERATED_SRC)
#DAS_AOT("hello_world.das" TEST_GENERATED_SRC daScriptTestAot dasAot)
SOURCE_GROUP_FILES("generated" TEST_GENERATED_SRC)

add_executable(daScriptTest ${UNIT_TEST_SRC} ${COMPILATION_FAIL_TEST_SRC} ${MIX_TEST_SRC} ${MODULE_TEST_SRC}
    ${TEST_MAIN_SRC} ${OPTIMIZATION_SRC} ${RUNTIME_ERRORS_SRC} ${TEST_GENERATED_SRC} ${AOT_GENERATED_SRC})
TARGET_LINK_LIBRARIES(daScriptTest libDaScript libDaScriptTest libDaScriptProfile libDasModuleUriparser libUriParser Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(daScriptTest libDaScript libDaScriptTest libDaScriptProfile libDasModuleUriparser libUriParser ${DAS_MODULES_LIBS})
TARGET_INCLUDE_DIRECTORIES(daScriptTest PUBLIC ${PROJECT_SOURCE_DIR}/examples/test)
SETUP_CPP11(daScriptTest)
add_dependencies(daScriptTest daScriptTestAot dasAotStub)
SETUP_LTO(daScriptTest)
#target_precompile_headers(daScriptTest PUBLIC include/daScript/misc/platform.h)

SET(THREADS_MAIN_SRC
test_threads.cpp
unitTest.h
)

add_executable(daScriptTestThreads ${UNIT_TEST_SRC} ${THREADS_MAIN_SRC} ${TEST_GENERATED_SRC} ${AOT_GENERATED_SRC})
TARGET_LINK_LIBRARIES(daScriptTestThreads libDaScript libDaScriptTest libDaScriptProfile libDasModuleUriparser libUriParser Threads::Threads ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(daScriptTestThreads libDaScript libDaScriptTest libDaScriptProfile libDasModuleUriparser libUriParser ${DAS_MODULES_LIBS})
TARGET_INCLUDE_DIRECTORIES(daScriptTestThreads PUBLIC ${PROJECT_SOURCE_DIR}/examples/test)
SETUP_CPP11(daScriptTestThreads)
add_dependencies(daScriptTest daScriptTestAot dasAotStub)
SETUP_LTO(daScriptTestThreads)
