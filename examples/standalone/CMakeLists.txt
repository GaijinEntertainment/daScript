###########################################################
# Standalone-AOT examples built against libDaScriptNano.
#
# Each example is a .das compiled to C++ ahead of time plus a C++ main that
# links the generated code and nano - no daslang binary at run time, and no
# compiler in the linked program at all.
#
# SDK users build the same four targets from CMakeLists.standalone.cmake.
###########################################################

# nano decides the header search order for everything that links it, and a
# directory-level include_directories() is searched BEFORE any target's own.
# Inheriting the root's would put the full include/ ahead of the headers nano
# shadows - the program would link nano while compiling against the full
# runtime's Context, which is a mismatch no test would name.
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")

set(NANO_EXAMPLE_GEN "${CMAKE_CURRENT_BINARY_DIR}/_standalone_ctx_generated")
file(MAKE_DIRECTORY "${NANO_EXAMPLE_GEN}")

function(das_nano_example name dir das_file)
    # extra args are data files the .das reads AT COMPILE TIME - listing them here
    # is what makes editing calibration data rebuild the generated C++
    set(_gen_cpp "${NANO_EXAMPLE_GEN}/${das_file}.cpp")
    add_custom_command(
        OUTPUT "${_gen_cpp}" "${NANO_EXAMPLE_GEN}/${das_file}.h"
        COMMAND $<TARGET_FILE:daslang>
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            -- -ctx "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/${das_file}"
            "${NANO_EXAMPLE_GEN}/"
        DEPENDS daslang
            "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/${das_file}"
            ${ARGN}
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Standalone AOT: ${das_file}"
        VERBATIM
    )
    add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/main.cpp" "${_gen_cpp}")
    target_include_directories(${name} PRIVATE "${NANO_EXAMPLE_GEN}")
    target_link_libraries(${name} PRIVATE libDaScriptNano)
    set_target_properties(${name} PROPERTIES FOLDER "examples/standalone")
endfunction()

das_nano_example(standalone_01_pure       01_pure       pure_math.das)
das_nano_example(standalone_02_heap       02_heap       heap_demo.das)
das_nano_example(standalone_03_closures   03_closures   closures.das)
das_nano_example(standalone_04_c_binding  04_c_binding  blinker.das)
das_nano_example(standalone_05_table      05_compile_time_table  thermometer.das
    "${CMAKE_CURRENT_SOURCE_DIR}/05_compile_time_table/thermistor.csv"
    "${CMAKE_CURRENT_SOURCE_DIR}/05_compile_time_table/csv_bake.das")
