# Big test: the minimal runtime runs the same standalone contexts the full
# runtime does. Each example is its own tier - POD, heap, closures, output, and
# a table a macro baked at compile time - and each one is here because a tier
# that stops linking is a tier that silently left nano, which no other test
# would notice.

# nano decides the header search order for everything that links it, and a
# directory-level include_directories() is searched BEFORE any target's own.
# Inheriting tests-cpp's would put the full include/ ahead of the headers nano
# shadows, so this test would link nano while compiling against the full
# runtime's Context.
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")

set(NANO_CTX_GEN "${CMAKE_CURRENT_BINARY_DIR}/_generated")
file(MAKE_DIRECTORY "${NANO_CTX_GEN}")

set(NANO_CTX_EXAMPLES "${PROJECT_SOURCE_DIR}/examples/standalone")

# data a .das reads while it COMPILES - a change here changes the generated C++
set(NANO_CTX_DATA
    "${NANO_CTX_EXAMPLES}/05_compile_time_table/thermistor.csv"
    "${NANO_CTX_EXAMPLES}/05_compile_time_table/csv_bake.das")

set(NANO_CTX_GENERATED)
foreach(_pair "01_pure/pure_math.das" "02_heap/heap_demo.das"
              "03_closures/closures.das" "04_c_binding/blinker.das"
              "05_compile_time_table/thermometer.das")
    get_filename_component(_das_name "${_pair}" NAME)
    # only the compile-time-table example reads its data while compiling; the
    # other four must not regenerate when the calibration file changes
    if(_pair STREQUAL "05_compile_time_table/thermometer.das")
        set(_pair_data ${NANO_CTX_DATA})
    else()
        set(_pair_data "")
    endif()
    add_custom_command(
        OUTPUT "${NANO_CTX_GEN}/${_das_name}.cpp" "${NANO_CTX_GEN}/${_das_name}.h"
        COMMAND $<TARGET_FILE:daslang>
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            -- -ctx "${NANO_CTX_EXAMPLES}/${_pair}" "${NANO_CTX_GEN}/"
        DEPENDS daslang
            "${NANO_CTX_EXAMPLES}/${_pair}"
            ${_pair_data}
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Standalone AOT (nano): ${_das_name}"
        VERBATIM
    )
    list(APPEND NANO_CTX_GENERATED "${NANO_CTX_GEN}/${_das_name}.cpp")
endforeach()

add_executable(test_nano_ctx test_nano_ctx.cpp ${NANO_CTX_GENERATED})
target_include_directories(test_nano_ctx PRIVATE "${NANO_CTX_GEN}")
target_link_libraries(test_nano_ctx PRIVATE libDaScriptNano)
set_target_properties(test_nano_ctx PROPERTIES FOLDER "tests-cpp/big")

add_test(NAME nano_ctx COMMAND test_nano_ctx
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(nano_ctx PROPERTIES LABELS "big")
add_dependencies(test-big test_nano_ctx)
