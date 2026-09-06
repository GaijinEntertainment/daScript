###########################################################
# Standalone-AOT examples built against libDaScriptNano.
#
# Each example is a .das compiled to C++ ahead of time plus a C++ main that
# links the generated code and nano - no daslang binary at run time, and no
# compiler in the linked program at all.
#
# SDK users build 01-04 from CMakeLists.standalone.cmake; 05 and the full-runtime 06 are in-tree.
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

# Not a nano example: a standalone context on the full runtime, linking C++ modules (dasHV,
# fio) as static archives - it compiles nothing at run time and loads no shared module. It
# compiles against the full runtime's headers, so it takes them back explicitly after the nano
# clear above. It also runs as a small test: the generated constructor registering its modules,
# and the context tearing down cleanly, is what no nano example exercises.
if(NOT DAS_HV_DISABLED)
    set(_full_runtime_gen_cpp "${NANO_EXAMPLE_GEN}/service_probe.das.cpp")
    add_custom_command(
        OUTPUT "${_full_runtime_gen_cpp}" "${NANO_EXAMPLE_GEN}/service_probe.das.h"
        COMMAND $<TARGET_FILE:daslang>
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            -- -ctx "${CMAKE_CURRENT_SOURCE_DIR}/06_full_runtime/service_probe.das"
            "${NANO_EXAMPLE_GEN}/"
        DEPENDS daslang
            "${CMAKE_CURRENT_SOURCE_DIR}/06_full_runtime/service_probe.das"
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Standalone AOT (full runtime): service_probe.das"
        VERBATIM
    )
    add_executable(standalone_06_full_runtime
        "${CMAKE_CURRENT_SOURCE_DIR}/06_full_runtime/main.cpp" "${_full_runtime_gen_cpp}")
    target_include_directories(standalone_06_full_runtime PRIVATE
        "${NANO_EXAMPLE_GEN}"
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/3rdparty/fmt/include"
        ${NEED_MODULES_PATH}
        "${PROJECT_SOURCE_DIR}/modules/dasHV/hv/$<CONFIG>/include")
    if(DEFINED DAS_CONFIG_INCLUDE_DIR)
        target_include_directories(standalone_06_full_runtime PRIVATE ${DAS_CONFIG_INCLUDE_DIR})
    endif()
    target_link_libraries(standalone_06_full_runtime PRIVATE
        libDaScript ${SRC_LIBRARIES} libDasModuleHV)
    SETUP_CPP11(standalone_06_full_runtime)
    set_target_properties(standalone_06_full_runtime PROPERTIES FOLDER "examples/standalone")
    add_test(NAME standalone_full_runtime COMMAND standalone_06_full_runtime
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    set_tests_properties(standalone_full_runtime PROPERTIES LABELS "small")
    add_dependencies(test-small standalone_06_full_runtime)
endif()
