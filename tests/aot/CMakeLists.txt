### AOT test target
### Builds a standalone binary with AOT-compiled test files

# AOT for individual test files
SET(AOT_TEST_FILES
    tests/aot/test_arithmetic.das
    tests/aot/test_strings.das
    tests/aot/test_lambdas.das
    tests/aot/test_structures.das
)

add_custom_target(test_aot_tests)
SET(TEST_AOT_GENERATED_SRC)
DAS_AOT("${AOT_TEST_FILES}" TEST_AOT_GENERATED_SRC test_aot_tests daslang)

SOURCE_GROUP_FILES("aot generated" TEST_AOT_GENERATED_SRC)

# Build the test_aot executable using the same main.cpp as daslang
add_executable(test_aot ${DAS_DASCRIPT_MAIN_SRC} ${TEST_AOT_GENERATED_SRC})
TARGET_LINK_LIBRARIES(test_aot libDaScriptAot ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(test_aot libDaScriptAot test_aot_tests)
target_include_directories(test_aot PRIVATE ${NEED_MODULES_PATH})
SETUP_CPP11(test_aot)
SETUP_LTO(test_aot)
