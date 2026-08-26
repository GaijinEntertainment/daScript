###########################################################
# Standalone-AOT examples on libDaScriptNano, built against
# an installed daslang SDK.
#
#   cmake -DCMAKE_PREFIX_PATH=<sdk-root> <this-directory>
#   cmake --build . --config Release
#
# Each example compiles its .das to C++ with the SDK's daslang, then links
# that C++ and nano - a program with no compiler in it and no daslang binary
# at run time.
###########################################################

cmake_minimum_required(VERSION 3.16)
project(daslang_standalone_examples CXX)

find_package(DAS REQUIRED)

# The SDK root, two levels up from lib/cmake/DAS/.
get_filename_component(DAS_SDK_ROOT "${DAS_DIR}/../../.." ABSOLUTE)
message(STATUS "daslang SDK root: ${DAS_SDK_ROOT}")

# nano ships as sources; build it here with this project's flags.
add_subdirectory("${DAS_SDK_ROOT}/nano" nano_build)

# nano decides the header search order for everything that links it, and a
# directory-level include_directories() is searched BEFORE any target's own.
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")

set(NANO_EXAMPLE_GEN "${CMAKE_CURRENT_BINARY_DIR}/_standalone_ctx_generated")
file(MAKE_DIRECTORY "${NANO_EXAMPLE_GEN}")

function(das_nano_example name dir das_file)
    set(_gen_cpp "${NANO_EXAMPLE_GEN}/${das_file}.cpp")
    add_custom_command(
        OUTPUT "${_gen_cpp}" "${NANO_EXAMPLE_GEN}/${das_file}.h"
        COMMAND $<TARGET_FILE:DAS::daslang>
            "${DAS_SDK_ROOT}/utils/aot/main.das"
            -- -ctx "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/${das_file}"
            "${NANO_EXAMPLE_GEN}/"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/${das_file}"
        COMMENT "Standalone AOT: ${das_file}"
        VERBATIM
    )
    add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/main.cpp" "${_gen_cpp}")
    target_include_directories(${name} PRIVATE "${NANO_EXAMPLE_GEN}")
    target_link_libraries(${name} PRIVATE libDaScriptNano)
endfunction()

das_nano_example(standalone_01_pure       01_pure       pure_math.das)
das_nano_example(standalone_02_heap       02_heap       heap_demo.das)
das_nano_example(standalone_03_closures   03_closures   closures.das)
das_nano_example(standalone_04_c_binding  04_c_binding  blinker.das)
