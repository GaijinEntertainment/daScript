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

set(NANO_OWN_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nano_context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nano_stubs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nano_string_writer.cpp
)

set(NANO_SHARED_SRC
    ${DASLANG_NANO_ROOT}/src/misc/hal.cpp
    ${DASLANG_NANO_ROOT}/src/misc/memory_model.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/annotation_arguments.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/aot_library.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/builtin_array_ops.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/builtin_runtime_ops.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/data_walker.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/debug_info.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/escape_string.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/heap.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/runtime_array.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/runtime_iterator.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/runtime_table.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/simulate_gc_pod.cpp
    ${DASLANG_NANO_ROOT}/src/simulate/standalone_ctx_utils.cpp
)

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
