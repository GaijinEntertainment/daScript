# Big test: a standalone AOT context runs global initializers in declaration
# order (required modules first), then [init] functions.

set(STANDALONE_CTX_GEN "${CMAKE_CURRENT_BINARY_DIR}/_generated")
file(MAKE_DIRECTORY "${STANDALONE_CTX_GEN}")

add_custom_command(
    OUTPUT "${STANDALONE_CTX_GEN}/standalone_init_fixture.das.cpp"
           "${STANDALONE_CTX_GEN}/standalone_init_fixture.das.h"
           "${CMAKE_CURRENT_SOURCE_DIR}/_standalone_init_fixture_c.das"
    COMMAND $<TARGET_FILE:daslang>
        "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
        -- -ctx "${CMAKE_CURRENT_SOURCE_DIR}/standalone_init_fixture.das"
        "${STANDALONE_CTX_GEN}/"
        # the bindings live beside the host that requires them; the library they name is the one
        # the shared target below builds out of the same generated .cpp
        -bindings "${CMAKE_CURRENT_SOURCE_DIR}/_standalone_init_fixture_c.das"
        -bindings_library "${CMAKE_CURRENT_BINARY_DIR}/standalone_init_fixture"
    DEPENDS daslang
        "${CMAKE_CURRENT_SOURCE_DIR}/standalone_init_fixture.das"
        "${CMAKE_CURRENT_SOURCE_DIR}/standalone_init_dep.das"
        "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
        "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
        "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
            "${PROJECT_SOURCE_DIR}/daslib/c_api_header.das"
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

# The same fixture driven through its C API, with the C half of the generated header compiled by
# a C compiler: a header that is not valid C, a layout that disagrees with das, or a thunk that
# loses a value fails here. The C++ twin above shares the generated source, so both APIs are
# proven against one emission.
add_executable(test_standalone_capi
    test_standalone_capi.c
    "${STANDALONE_CTX_GEN}/standalone_init_fixture.das.cpp")
target_link_libraries(test_standalone_capi PRIVATE
    libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
target_include_directories(test_standalone_capi PRIVATE "${STANDALONE_CTX_GEN}" ${NEED_MODULES_PATH})
set_target_properties(test_standalone_capi PROPERTIES
    FOLDER "tests-cpp/big"
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX)
SETUP_CPP11(test_standalone_capi)

add_test(NAME standalone_capi COMMAND test_standalone_capi
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(standalone_capi PROPERTIES LABELS "small")
add_dependencies(test-small test_standalone_capi)

# The same emission, loaded instead of linked: a shared library built from the generated .cpp
# exports the C entry points (default visibility; on Windows the header's <PREFIX>_API macro is
# what says so), and a daslang host reaches them through the bindings the same run wrote. Needs
# the shared runtime, since the host process already carries one.
if(TARGET libDaScriptDyn_runtime)
    add_library(standalone_init_fixture_shared SHARED
        "${STANDALONE_CTX_GEN}/standalone_init_fixture.das.cpp")
    target_link_libraries(standalone_init_fixture_shared PRIVATE
        libDaScriptDyn_runtime ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
    target_include_directories(standalone_init_fixture_shared PRIVATE
        "${STANDALONE_CTX_GEN}" ${NEED_MODULES_PATH})
    target_compile_definitions(standalone_init_fixture_shared PRIVATE
        STANDALONE_INIT_FIXTURE_SHARED STANDALONE_INIT_FIXTURE_BUILD)
    # the shared runtime hides its typeinfo (SETUP_DAS_SHARED_LIBRARY, CXX_VISIBILITY_PRESET
    # hidden), so a library carrying `class Standalone : public Context` cannot resolve
    # typeinfo for das::Context at load. No RTTI, no reference - the same rule a shared module obeys.
    target_compile_options(standalone_init_fixture_shared PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fno-rtti>
        $<$<CXX_COMPILER_ID:MSVC>:/GR->)
    set_target_properties(standalone_init_fixture_shared PROPERTIES
        PREFIX ""
        OUTPUT_NAME standalone_init_fixture
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        BUILD_RPATH "${PROJECT_SOURCE_DIR}/lib"
        FOLDER "tests-cpp/big")
    SETUP_CPP11(standalone_init_fixture_shared)

    add_test(NAME standalone_capi_dasbind
             COMMAND $<TARGET_FILE:daslang>
                     "${CMAKE_CURRENT_SOURCE_DIR}/test_standalone_bindings_host.das"
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    set_tests_properties(standalone_capi_dasbind PROPERTIES LABELS "small")
    add_dependencies(test-small standalone_init_fixture_shared)
endif()

# The generated header asserts every structure's size and each of its field offsets. Nothing
# proved those asserts FIRE, which is the whole reason they are emitted, so this compiles a host
# that packs its structures and requires the compiler to refuse it. Skipped on MSVC: the C flags
# and the _Static_assert spelling both differ there.
if(NOT MSVC)
    add_custom_command(
        OUTPUT "${STANDALONE_CTX_GEN}/standalone_layout_fixture.das.h"
               "${STANDALONE_CTX_GEN}/standalone_layout_fixture.das.cpp"
        COMMAND $<TARGET_FILE:daslang>
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            -- -ctx "${CMAKE_CURRENT_SOURCE_DIR}/standalone_layout_fixture.das"
            "${STANDALONE_CTX_GEN}/"
        DEPENDS daslang
            "${CMAKE_CURRENT_SOURCE_DIR}/standalone_layout_fixture.das"
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
            "${PROJECT_SOURCE_DIR}/daslib/c_api_header.das"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Standalone AOT: standalone_layout_fixture.das"
        VERBATIM
    )
    add_custom_target(standalone_layout_fixture_header
        DEPENDS "${STANDALONE_CTX_GEN}/standalone_layout_fixture.das.h")
    set_target_properties(standalone_layout_fixture_header PROPERTIES FOLDER "tests-cpp/big")

    add_test(NAME standalone_layout_assert COMMAND ${CMAKE_COMMAND}
             -DCC=${CMAKE_C_COMPILER}
             -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/test_standalone_layout_packed.c
             -DINC=${STANDALONE_CTX_GEN}
             -DOUT=${CMAKE_CURRENT_BINARY_DIR}/packed_host.o
             -P ${CMAKE_CURRENT_SOURCE_DIR}/expect_layout_assert.cmake)
    set_tests_properties(standalone_layout_assert PROPERTIES LABELS "small")
    add_dependencies(test-small standalone_layout_fixture_header)
endif()

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
            "${PROJECT_SOURCE_DIR}/daslib/c_api_header.das"
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
