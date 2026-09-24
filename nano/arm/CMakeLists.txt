cmake_minimum_required(VERSION 3.17)
project(nano_arm CXX)

if(NOT DASLANG_HOST_EXE)
    message(FATAL_ERROR "nano/arm: -DDASLANG_HOST_EXE=<host daslang> is required")
endif()
get_filename_component(DASLANG_NANO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
add_compile_options(-mcpu=cortex-m4 -mthumb -mfloat-abi=soft
    -Os -ffunction-sections -fdata-sections
    -fno-exceptions -fno-rtti -fno-threadsafe-statics)
add_link_options(-mcpu=cortex-m4 -mthumb -mfloat-abi=soft
    --specs=nosys.specs -Wl,--gc-sections)

set(NANO_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
include(${NANO_SOURCES_DIR}/sources.cmake)

add_library(nano_arm STATIC ${NANO_OWN_SRC} ${NANO_SHARED_SRC})
target_include_directories(nano_arm BEFORE PUBLIC
    ${NANO_SOURCES_DIR}/include
    ${DASLANG_NANO_ROOT}/include)

set(NANO_EXAMPLE_GEN ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(NANO_ARM_ELVES)

function(das_nano_example name dir das_file)
    set(_gen_cpp "${NANO_EXAMPLE_GEN}/${das_file}.cpp")
    add_custom_command(
        OUTPUT "${_gen_cpp}" "${NANO_EXAMPLE_GEN}/${das_file}.h"
        COMMAND "${DASLANG_HOST_EXE}" "${DASLANG_NANO_ROOT}/utils/aot/main.das"
            -- -cross_platform -ctx "${NANO_EXAMPLES_DIR}/${dir}/${das_file}"
            "${NANO_EXAMPLE_GEN}/"
        DEPENDS "${NANO_EXAMPLES_DIR}/${dir}/${das_file}"
            ${ARGN}
            "${DASLANG_NANO_ROOT}/utils/aot/main.das"
            "${DASLANG_NANO_ROOT}/daslib/aot_standalone.das"
            "${DASLANG_NANO_ROOT}/daslib/c_api_header.das"
            "${DASLANG_NANO_ROOT}/daslib/aot_cpp.das"
        WORKING_DIRECTORY "${DASLANG_NANO_ROOT}"
        COMMENT "Standalone AOT (cortex-m4): ${das_file}"
        VERBATIM
    )
    add_executable(${name} "${NANO_EXAMPLES_DIR}/${dir}/main.cpp" "${_gen_cpp}")
    target_include_directories(${name} PRIVATE "${NANO_EXAMPLE_GEN}")
    target_link_libraries(${name} PRIVATE nano_arm)
    list(APPEND NANO_ARM_ELVES $<TARGET_FILE:${name}>)
    set(NANO_ARM_ELVES "${NANO_ARM_ELVES}" PARENT_SCOPE)
endfunction()

set(NANO_EXAMPLES_DIR ${DASLANG_NANO_ROOT}/examples/standalone)
include(${NANO_EXAMPLES_DIR}/nano_examples.cmake)

add_custom_target(nano_arm_size ALL
    COMMAND ${CMAKE_COMMAND} -E echo "== cortex-m4, -Os, gc-sections"
    COMMAND ${CMAKE_SIZE} ${NANO_ARM_ELVES}
    DEPENDS ${NANO_ARM_ELVES}
    VERBATIM)
