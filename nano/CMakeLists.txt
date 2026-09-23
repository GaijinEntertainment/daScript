###########################################################
# libDaScriptNano - the minimal daslang runtime.
#
# The whole mechanism is the include order below: nano/include comes BEFORE the
# regular include/, so a header nano shadows wins and every other header is the
# upstream file, byte for byte. That is why NANO_SHARED_SRC can list sources
# straight out of src/ - they compile against nano's Context without knowing it.
#
# Adding a source here is a decision, not a convenience: it must compile with no
# edit to the shared tree. If it needs one, the fix goes upstream (a carve, the
# way src/simulate/simulate_gc_pod.cpp and src/simulate/annotation_arguments.cpp
# were carved out) rather than into a fork.
###########################################################

# The daslang root: the tree in-repo, the SDK root once installed.
if(NOT DEFINED DASLANG_NANO_ROOT)
    get_filename_component(DASLANG_NANO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
endif()

set(NANO_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include(${CMAKE_CURRENT_SOURCE_DIR}/sources.cmake)

add_library(libDaScriptNano STATIC ${NANO_OWN_SRC} ${NANO_SHARED_SRC})

target_include_directories(libDaScriptNano BEFORE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${DASLANG_NANO_ROOT}/include>
    $<INSTALL_INTERFACE:nano/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(libDaScriptNano PUBLIC cxx_std_17)
set_target_properties(libDaScriptNano PROPERTIES FOLDER "nano")

if(MSVC)
    target_compile_options(libDaScriptNano PRIVATE /wd4100 /wd4127)
endif()

if(NOT DAS_TOOLS_DISABLED AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_custom_target(check_nano_arm
        COMMAND ${CMAKE_COMMAND} -S ${CMAKE_CURRENT_SOURCE_DIR}/arm -B ${CMAKE_BINARY_DIR}/nano-arm
            -G "${CMAKE_GENERATOR}"
            -DCMAKE_TOOLCHAIN_FILE=${PROJECT_SOURCE_DIR}/cmake/arm-none-eabi.cmake
            -DDASLANG_HOST_EXE=$<TARGET_FILE:daslang>
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/nano-arm
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Cross-compiling nano for cortex-m4"
    )
endif()
