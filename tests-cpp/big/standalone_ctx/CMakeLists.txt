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

# Two contexts with different C++ module sets in one binary - the dasHV + fio example and a
# fio-only fixture - share one module registry lifetime. Small-labelled: a second owner of the
# shutdown, or an unbalanced Initialize, fails at exit and nothing else would notice.
if(NOT DAS_HV_DISABLED)
    set(STANDALONE_MODULES_GENERATED)
    foreach(_das "${PROJECT_SOURCE_DIR}/examples/standalone/06_full_runtime/service_probe.das"
                 "${CMAKE_CURRENT_SOURCE_DIR}/standalone_modules_fixture.das")
        get_filename_component(_das_name "${_das}" NAME)
        add_custom_command(
            OUTPUT "${STANDALONE_CTX_GEN}/${_das_name}.cpp" "${STANDALONE_CTX_GEN}/${_das_name}.h"
            COMMAND $<TARGET_FILE:daslang>
                "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
                -- -ctx "${_das}" "${STANDALONE_CTX_GEN}/"
            DEPENDS daslang
                "${_das}"
                "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
                "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
                "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Standalone AOT: ${_das_name}"
            VERBATIM
        )
        list(APPEND STANDALONE_MODULES_GENERATED "${STANDALONE_CTX_GEN}/${_das_name}.cpp")
    endforeach()

    add_executable(test_standalone_modules test_standalone_modules.cpp ${STANDALONE_MODULES_GENERATED})
    target_link_libraries(test_standalone_modules PRIVATE
        libDaScript ${SRC_LIBRARIES} libDasModuleHV)
    target_include_directories(test_standalone_modules PRIVATE "${STANDALONE_CTX_GEN}" ${NEED_MODULES_PATH}
        "${PROJECT_SOURCE_DIR}/modules/dasHV/hv/$<CONFIG>/include")
    SETUP_CPP11(test_standalone_modules)
    set_target_properties(test_standalone_modules PROPERTIES FOLDER "tests-cpp/big")

    add_test(NAME standalone_modules COMMAND test_standalone_modules
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    # the host registered every module first: the generated code registers nothing and owns
    # no shutdown, so the host's own Initialize/Shutdown pair stays balanced
    add_test(NAME standalone_modules_host COMMAND test_standalone_modules --host-registers
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    # the host registered the builtin set but not dasHV: the generated code must stop the
    # program by name rather than initialize a second time and shut down a registry twice
    add_test(NAME standalone_modules_host_partial COMMAND test_standalone_modules --host-partial
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    set_tests_properties(standalone_modules_host_partial PROPERTIES WILL_FAIL TRUE)
    set_tests_properties(standalone_modules standalone_modules_host standalone_modules_host_partial
                         PROPERTIES LABELS "small")
    add_dependencies(test-small test_standalone_modules)
endif()
