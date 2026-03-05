### AOT test target
### Builds a standalone binary with AOT-compiled test files

# AOT for dastest/testing.das (library module used by all tests)
SET(AOT_TESTING_FILES
    dastest/testing.das
)

add_custom_target(test_aot_testing)
SET(TESTING_AOT_GENERATED_SRC)
DAS_AOT_LIB("${AOT_TESTING_FILES}" TESTING_AOT_GENERATED_SRC test_aot_testing daslang)

# AOT for daslib modules (library dependencies used by test files)
SET(AOT_DASLIB_MODULE_FILES
    daslib/archive.das
    daslib/ast_boost.das
    daslib/base64.das
    daslib/bool_array.das
    daslib/coroutines.das
    daslib/debug_eval.das
    daslib/decs.das
    daslib/faker.das
    daslib/fuzzer.das
    daslib/json.das
    daslib/json_boost.das
    daslib/live.das
    daslib/math_bits.das
    daslib/random.das
    daslib/regex.das
)

add_custom_target(test_aot_daslib_modules)
SET(DASLIB_MODULES_AOT_GENERATED_SRC)
DAS_AOT_LIB("${AOT_DASLIB_MODULE_FILES}" DASLIB_MODULES_AOT_GENERATED_SRC test_aot_daslib_modules daslang)

# AOT for individual test files
FILE(GLOB AOT_TEST_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/aot/*.das")

# AOT for algorithm test files
FILE(GLOB AOT_ALGORITHM_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/algorithm/*.das")

# AOT for apply test files
FILE(GLOB AOT_APPLY_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/apply/*.das")

# AOT for archive test files
FILE(GLOB AOT_ARCHIVE_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/archive/*.das")

# AOT for assert_once test files
FILE(GLOB AOT_ASSERT_ONCE_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/assert_once/*.das")

# AOT for async test files
FILE(GLOB AOT_ASYNC_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/async/*.das")

# AOT for bare_block test files
FILE(GLOB AOT_BARE_BLOCK_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/bare_block/*.das")

# AOT for base64 test files
FILE(GLOB AOT_BASE64_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/base64/*.das")

# AOT for bitfields test files
FILE(GLOB AOT_BITFIELDS_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/bitfields/*.das")

# AOT for bool_array test files
FILE(GLOB AOT_BOOL_ARRAY_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/bool_array/*.das")

# AOT for data_walker test files
FILE(GLOB AOT_DATA_WALKER_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/data_walker/*.das")

# AOT for debug test files
FILE(GLOB AOT_DEBUG_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/debug/*.das")

# AOT for debug_agent test files
FILE(GLOB AOT_DEBUG_AGENT_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/debug_agent/*.das")

# AOT for decs test files
FILE(GLOB AOT_DECS_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/decs/*.das")
# Exclude expect-error tests (they intentionally fail to compile)
# Exclude test_stages_extra (it is a module, compiled via DAS_AOT_LIB below)
list(FILTER AOT_DECS_FILES EXCLUDE REGEX "failed_|test_stages_extra")

# Decs test module files (libraries required by decs tests)
SET(AOT_DECS_MODULE_FILES
    tests/decs/test_stages_extra.das
)

# AOT for dynamic_cast_rtti test files
FILE(GLOB AOT_DYNAMIC_CAST_RTTI_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/dynamic_cast_rtti/*.das")

# AOT for fio test files
FILE(GLOB AOT_FIO_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/fio/*.das")

# AOT for functional test files
FILE(GLOB AOT_FUNCTIONAL_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/functional/*.das")

# AOT for handle_types test files
FILE(GLOB AOT_HANDLE_TYPES_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/handle_types/*.das")

# AOT for hash_map test files
FILE(GLOB AOT_HASH_MAP_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/hash_map/*.das")

# AOT for interfaces test files
FILE(GLOB AOT_INTERFACES_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/interfaces/*.das")
# Exclude expect-error tests and AOT codegen failures
list(FILTER AOT_INTERFACES_FILES EXCLUDE REGEX "failed_|test_missing_")

# AOT for jobque test files
FILE(GLOB AOT_JOBQUE_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/jobque/*.das")

# AOT for json test files
FILE(GLOB AOT_JSON_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/json/*.das")

# AOT for language test files
FILE(GLOB AOT_LANGUAGE_TEST_FILES RELATIVE ${PROJECT_SOURCE_DIR} "tests/language/*.das")
# Exclude expect-error tests and no_aot files
list(FILTER AOT_LANGUAGE_TEST_FILES EXCLUDE REGEX "failed_|cant_|invalid_")
# Exclude module files (they are compiled via DAS_AOT_LIB below)
list(FILTER AOT_LANGUAGE_TEST_FILES EXCLUDE REGEX "/_")

# Language test module files (libraries required by tests)
SET(AOT_LANGUAGE_MODULE_FILES
    tests/language/_glob.das
    tests/language/_helper_foo.das
    tests/language/_operators_derived.das
    tests/language/_operators_parent.das
)

add_custom_target(test_aot_algorithm)
SET(ALGORITHM_AOT_GENERATED_SRC)
DAS_AOT("${AOT_ALGORITHM_FILES}" ALGORITHM_AOT_GENERATED_SRC test_aot_algorithm daslang)

add_custom_target(test_aot_apply)
SET(APPLY_AOT_GENERATED_SRC)
DAS_AOT("${AOT_APPLY_FILES}" APPLY_AOT_GENERATED_SRC test_aot_apply daslang)

add_custom_target(test_aot_archive)
SET(ARCHIVE_AOT_GENERATED_SRC)
DAS_AOT("${AOT_ARCHIVE_FILES}" ARCHIVE_AOT_GENERATED_SRC test_aot_archive daslang)

add_custom_target(test_aot_assert_once)
SET(ASSERT_ONCE_AOT_GENERATED_SRC)
DAS_AOT("${AOT_ASSERT_ONCE_FILES}" ASSERT_ONCE_AOT_GENERATED_SRC test_aot_assert_once daslang)

add_custom_target(test_aot_async)
SET(ASYNC_AOT_GENERATED_SRC)
DAS_AOT("${AOT_ASYNC_FILES}" ASYNC_AOT_GENERATED_SRC test_aot_async daslang)

add_custom_target(test_aot_bare_block)
SET(BARE_BLOCK_AOT_GENERATED_SRC)
DAS_AOT("${AOT_BARE_BLOCK_FILES}" BARE_BLOCK_AOT_GENERATED_SRC test_aot_bare_block daslang)

add_custom_target(test_aot_base64)
SET(BASE64_AOT_GENERATED_SRC)
DAS_AOT("${AOT_BASE64_FILES}" BASE64_AOT_GENERATED_SRC test_aot_base64 daslang)

add_custom_target(test_aot_bitfields)
SET(BITFIELDS_AOT_GENERATED_SRC)
DAS_AOT("${AOT_BITFIELDS_FILES}" BITFIELDS_AOT_GENERATED_SRC test_aot_bitfields daslang)

add_custom_target(test_aot_bool_array)
SET(BOOL_ARRAY_AOT_GENERATED_SRC)
DAS_AOT("${AOT_BOOL_ARRAY_FILES}" BOOL_ARRAY_AOT_GENERATED_SRC test_aot_bool_array daslang)

add_custom_target(test_aot_data_walker)
SET(DATA_WALKER_AOT_GENERATED_SRC)
DAS_AOT("${AOT_DATA_WALKER_FILES}" DATA_WALKER_AOT_GENERATED_SRC test_aot_data_walker daslang)

add_custom_target(test_aot_debug)
SET(DEBUG_AOT_GENERATED_SRC)
DAS_AOT("${AOT_DEBUG_FILES}" DEBUG_AOT_GENERATED_SRC test_aot_debug daslang)

add_custom_target(test_aot_debug_agent)
SET(DEBUG_AGENT_AOT_GENERATED_SRC)
DAS_AOT("${AOT_DEBUG_AGENT_FILES}" DEBUG_AGENT_AOT_GENERATED_SRC test_aot_debug_agent daslang)

add_custom_target(test_aot_decs)
SET(DECS_AOT_GENERATED_SRC)
DAS_AOT("${AOT_DECS_FILES}" DECS_AOT_GENERATED_SRC test_aot_decs daslang)

add_custom_target(test_aot_decs_modules)
SET(DECS_MODULES_AOT_GENERATED_SRC)
DAS_AOT_LIB("${AOT_DECS_MODULE_FILES}" DECS_MODULES_AOT_GENERATED_SRC test_aot_decs_modules daslang)

add_custom_target(test_aot_dynamic_cast_rtti)
SET(DYNAMIC_CAST_RTTI_AOT_GENERATED_SRC)
DAS_AOT("${AOT_DYNAMIC_CAST_RTTI_FILES}" DYNAMIC_CAST_RTTI_AOT_GENERATED_SRC test_aot_dynamic_cast_rtti daslang)

add_custom_target(test_aot_fio)
SET(FIO_AOT_GENERATED_SRC)
DAS_AOT("${AOT_FIO_FILES}" FIO_AOT_GENERATED_SRC test_aot_fio daslang)

add_custom_target(test_aot_functional)
SET(FUNCTIONAL_AOT_GENERATED_SRC)
DAS_AOT("${AOT_FUNCTIONAL_FILES}" FUNCTIONAL_AOT_GENERATED_SRC test_aot_functional daslang)

add_custom_target(test_aot_handle_types)
SET(HANDLE_TYPES_AOT_GENERATED_SRC)
DAS_AOT("${AOT_HANDLE_TYPES_FILES}" HANDLE_TYPES_AOT_GENERATED_SRC test_aot_handle_types daslang)

add_custom_target(test_aot_hash_map)
SET(HASH_MAP_AOT_GENERATED_SRC)
DAS_AOT("${AOT_HASH_MAP_FILES}" HASH_MAP_AOT_GENERATED_SRC test_aot_hash_map daslang)

add_custom_target(test_aot_interfaces)
SET(INTERFACES_AOT_GENERATED_SRC)
DAS_AOT("${AOT_INTERFACES_FILES}" INTERFACES_AOT_GENERATED_SRC test_aot_interfaces daslang)

add_custom_target(test_aot_jobque)
SET(JOBQUE_AOT_GENERATED_SRC)
DAS_AOT("${AOT_JOBQUE_FILES}" JOBQUE_AOT_GENERATED_SRC test_aot_jobque daslang)

add_custom_target(test_aot_json)
SET(JSON_AOT_GENERATED_SRC)
DAS_AOT("${AOT_JSON_FILES}" JSON_AOT_GENERATED_SRC test_aot_json daslang)

add_custom_target(test_aot_language)
SET(LANGUAGE_AOT_GENERATED_SRC)
DAS_AOT("${AOT_LANGUAGE_TEST_FILES}" LANGUAGE_AOT_GENERATED_SRC test_aot_language daslang)

add_custom_target(test_aot_language_modules)
SET(LANGUAGE_MODULES_AOT_GENERATED_SRC)
DAS_AOT_LIB("${AOT_LANGUAGE_MODULE_FILES}" LANGUAGE_MODULES_AOT_GENERATED_SRC test_aot_language_modules daslang)

add_custom_target(test_aot_tests)
SET(TEST_AOT_GENERATED_SRC)
DAS_AOT("${AOT_TEST_FILES}" TEST_AOT_GENERATED_SRC test_aot_tests daslang)

SOURCE_GROUP_FILES("aot generated" TEST_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" TESTING_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" ALGORITHM_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" APPLY_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" ARCHIVE_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" ASSERT_ONCE_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" ASYNC_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" BARE_BLOCK_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" BASE64_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" BITFIELDS_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" BOOL_ARRAY_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" DATA_WALKER_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" DEBUG_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" DEBUG_AGENT_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" DECS_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" DYNAMIC_CAST_RTTI_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" FIO_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" FUNCTIONAL_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" HANDLE_TYPES_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" HASH_MAP_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" INTERFACES_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" JOBQUE_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" JSON_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" LANGUAGE_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" LANGUAGE_MODULES_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" DASLIB_MODULES_AOT_GENERATED_SRC)
SOURCE_GROUP_FILES("aot generated" DECS_MODULES_AOT_GENERATED_SRC)

# Build the test_aot executable using the same main.cpp as daslang
add_executable(test_aot ${DAS_DASCRIPT_MAIN_SRC}
    ${TEST_AOT_GENERATED_SRC}
    ${TESTING_AOT_GENERATED_SRC}
    ${ALGORITHM_AOT_GENERATED_SRC}
    ${APPLY_AOT_GENERATED_SRC}
    ${ARCHIVE_AOT_GENERATED_SRC}
    ${ASSERT_ONCE_AOT_GENERATED_SRC}
    ${ASYNC_AOT_GENERATED_SRC}
    ${BARE_BLOCK_AOT_GENERATED_SRC}
    ${BASE64_AOT_GENERATED_SRC}
    ${BITFIELDS_AOT_GENERATED_SRC}
    ${BOOL_ARRAY_AOT_GENERATED_SRC}
    ${DATA_WALKER_AOT_GENERATED_SRC}
    ${DEBUG_AOT_GENERATED_SRC}
    ${DEBUG_AGENT_AOT_GENERATED_SRC}
    ${DECS_AOT_GENERATED_SRC}
    ${DYNAMIC_CAST_RTTI_AOT_GENERATED_SRC}
    ${FIO_AOT_GENERATED_SRC}
    ${FUNCTIONAL_AOT_GENERATED_SRC}
    ${HANDLE_TYPES_AOT_GENERATED_SRC}
    ${HASH_MAP_AOT_GENERATED_SRC}
    ${INTERFACES_AOT_GENERATED_SRC}
    ${JOBQUE_AOT_GENERATED_SRC}
    ${JSON_AOT_GENERATED_SRC}
    ${LANGUAGE_AOT_GENERATED_SRC}
    ${LANGUAGE_MODULES_AOT_GENERATED_SRC}
    ${DASLIB_MODULES_AOT_GENERATED_SRC}
    ${DECS_MODULES_AOT_GENERATED_SRC}
)
TARGET_LINK_LIBRARIES(test_aot libDaScriptAot ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
ADD_DEPENDENCIES(test_aot libDaScriptAot
    test_aot_tests test_aot_testing
    test_aot_algorithm test_aot_apply test_aot_archive
    test_aot_assert_once test_aot_async test_aot_bare_block test_aot_base64
    test_aot_bitfields test_aot_bool_array
    test_aot_data_walker test_aot_debug
    test_aot_debug_agent test_aot_decs test_aot_dynamic_cast_rtti
    test_aot_fio test_aot_functional
    test_aot_handle_types test_aot_hash_map
    test_aot_interfaces
    test_aot_jobque test_aot_json
    test_aot_language test_aot_language_modules
    test_aot_daslib_modules test_aot_decs_modules
)
target_include_directories(test_aot PRIVATE ${NEED_MODULES_PATH} ${PROJECT_SOURCE_DIR}/modules/dasUnitTest)
SETUP_CPP11(test_aot)
SETUP_LTO(test_aot)
