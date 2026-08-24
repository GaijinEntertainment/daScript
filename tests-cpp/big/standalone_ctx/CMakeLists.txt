# Big test: a standalone AOT context runs global initializers in declaration
# order (required modules first), then [init] functions.

set(STANDALONE_CTX_GEN "${CMAKE_CURRENT_BINARY_DIR}/_generated")
file(MAKE_DIRECTORY "${STANDALONE_CTX_GEN}")

add_custom_command(
    OUTPUT "${STANDALONE_CTX_GEN}/standalone_init_fixture.das.cpp"
           "${STANDALONE_CTX_GEN}/standalone_init_fixture.das.h"
    COMMAND $<TARGET_FILE:daslang>
        "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
        -- -ctx "${CMAKE_CURRENT_SOURCE_DIR}/standalone_init_fixture.das"
        "${STANDALONE_CTX_GEN}/"
    DEPENDS daslang
        "${CMAKE_CURRENT_SOURCE_DIR}/standalone_init_fixture.das"
        "${CMAKE_CURRENT_SOURCE_DIR}/standalone_init_dep.das"
        "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
        "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
        "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Standalone AOT: standalone_init_fixture.das"
    VERBATIM
)

add_executable(test_standalone_ctx
    test_standalone_ctx.cpp
    "${STANDALONE_CTX_GEN}/standalone_init_fixture.das.cpp")
target_link_libraries(test_standalone_ctx PRIVATE
    libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
target_include_directories(test_standalone_ctx PRIVATE "${STANDALONE_CTX_GEN}" ${NEED_MODULES_PATH})
SETUP_CPP11(test_standalone_ctx)
set_target_properties(test_standalone_ctx PROPERTIES FOLDER "tests-cpp/big")

add_test(NAME standalone_ctx COMMAND test_standalone_ctx
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(standalone_ctx PROPERTIES LABELS "big")
add_dependencies(test-big test_standalone_ctx)
