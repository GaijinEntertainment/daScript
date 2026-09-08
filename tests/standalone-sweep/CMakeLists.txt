# Two standalone sweeps over test_aot's corpus, one per backend. Opt-in, out of CI:
#   ninja standalone_sweep_aot / run_standalone_sweep_aot   -ctx: compile, link, launch all
#   ninja standalone_sweep_jit                              -lib: emit, load back, drive each
# -DDAS_STANDALONE_SWEEP_FILTER=<regex> narrows the corpus.

if(NOT TARGET daslang OR NOT DEFINED TEST_AOT_ALL_DAS)
    return()
endif()

set(DAS_STANDALONE_SWEEP_FILTER "" CACHE STRING
    "Regex narrowing the standalone sweep corpus (empty = every file test_aot covers)")

# DAS_AOT_CTX resolves against the tree root, not this subdirectory
set(_sweep_in "")
foreach(_f IN LISTS TEST_AOT_ALL_DAS)
    if(IS_ABSOLUTE ${_f})
        list(APPEND _sweep_in ${_f})
    else()
        list(APPEND _sweep_in ${PROJECT_SOURCE_DIR}/${_f})
    endif()
endforeach()
list(REMOVE_DUPLICATES _sweep_in)
list(FILTER _sweep_in EXCLUDE REGEX "/_")   # required helpers, not entry points
if(NOT DAS_STANDALONE_SWEEP_FILTER STREQUAL "")
    list(FILTER _sweep_in INCLUDE REGEX "${DAS_STANDALONE_SWEEP_FILTER}")
endif()

# `options no_aot`, a quote and require_module_now each leave a file with no standalone form;
# a stem another file already claimed would emit one namespace twice and collide at link
set(_sweep_files "")
set(_sweep_stems "")
set(_skipped 0)
foreach(_f IN LISTS _sweep_in)
    get_filename_component(_stem ${_f} NAME_WE)
    file(STRINGS ${_f} _no_form REGEX "^[ \t]*options[ \t].*no_aot|\\[no_aot\\]|qmacro|qmatch|qblock|quote\\(|require_module_now")
    if(_no_form OR (_stem IN_LIST _sweep_stems))
        math(EXPR _skipped "${_skipped} + 1")
        file(RELATIVE_PATH _rel ${PROJECT_SOURCE_DIR} ${_f})
        message(VERBOSE "standalone sweep: not swept - ${_rel}")
    else()
        list(APPEND _sweep_stems ${_stem})
        list(APPEND _sweep_files ${_f})
    endif()
endforeach()
list(LENGTH _sweep_files _sweep_count)
message(STATUS "standalone sweep: ${_sweep_count} files, ${_skipped} with no standalone form or a claimed stem")
list(JOIN _sweep_files "|" _sweep_files_arg)

### C++ tier
add_custom_target(standalone_sweep_aot_corpus)
set(SWEEP_CTX_GENERATED_SRC)
DAS_AOT_CTX("${_sweep_files}" SWEEP_CTX_GENERATED_SRC standalone_sweep_aot_corpus daslang)

# which files emitted an entry point is known only after the generate step
set(_contexts "${CMAKE_CURRENT_BINARY_DIR}/sweep_contexts.h")
add_custom_command(
    OUTPUT ${_contexts}
    COMMAND ${CMAKE_COMMAND} -DOUT=${_contexts} "-DFILES=${_sweep_files_arg}"
        -P ${CMAKE_CURRENT_SOURCE_DIR}/sweep_contexts.cmake
    DEPENDS standalone_sweep_aot_corpus ${CMAKE_CURRENT_SOURCE_DIR}/sweep_contexts.cmake
    COMMENT "Standalone sweep (C++): collecting the contexts that emitted"
    VERBATIM
)

add_executable(standalone_sweep_aot EXCLUDE_FROM_ALL
    "${CMAKE_CURRENT_SOURCE_DIR}/sweep_driver.c" ${_contexts} ${SWEEP_CTX_GENERATED_SRC})
target_link_libraries(standalone_sweep_aot PRIVATE libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
# the same corpus test_aot compiles, so the same include list
target_include_directories(standalone_sweep_aot PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}" ${TEST_AOT_INCLUDE_DIRS} ${DAS_CONFIG_INCLUDE_DIR})
set_target_properties(standalone_sweep_aot PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED ON LINKER_LANGUAGE CXX FOLDER tests)
SETUP_CPP11(standalone_sweep_aot)
add_dependencies(standalone_sweep_aot standalone_sweep_aot_corpus)

add_custom_target(run_standalone_sweep_aot
    COMMAND $<TARGET_FILE:standalone_sweep_aot>
    DEPENDS standalone_sweep_aot
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Standalone sweep (C++): launching every context"
    USES_TERMINAL
)

### JIT tier
if(NOT DAS_LLVM_DISABLED)
    add_custom_target(standalone_sweep_jit
        COMMAND ${CMAKE_COMMAND} -DDASLANG=$<TARGET_FILE:daslang> -DROOT=${PROJECT_SOURCE_DIR}
            -DOUT=${CMAKE_CURRENT_BINARY_DIR}/_lib "-DFILES=${_sweep_files_arg}"
            -P ${CMAKE_CURRENT_SOURCE_DIR}/sweep_jit.cmake
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Standalone sweep (JIT -lib): every file as a C-ABI library, loaded back"
        USES_TERMINAL
        VERBATIM
    )
endif()
