# `daslang -lib` end-to-end integration test: a C host compiled against the generated header,
# linked against the produced library, run. See run_capi.cmake for the scenarios.

if(NOT TARGET daslang OR NOT DAS_LLVM_INCLUDED)
    # -lib is emitted by the LLVM backend; skip when LLVM is disabled.
    return()
endif()

add_test(
    NAME lib_capi
    COMMAND ${CMAKE_COMMAND}
        -DDASLANG=$<TARGET_FILE:daslang>
        -DSRC_DAS=${CMAKE_CURRENT_SOURCE_DIR}/fixture.das
        -DHOST_C=${CMAKE_CURRENT_SOURCE_DIR}/host.c
        -DC_COMPILER=${CMAKE_C_COMPILER}
        -DGENERATOR=${CMAKE_GENERATOR}
        -DBUILD_TYPE=$<CONFIG>
        "-DRUNTIME_DLLS=$<TARGET_FILE:libDaScriptDyn_runtime>$<SEMICOLON>$<TARGET_FILE:libDaScriptDyn>"
        -DSTATIC_RT=$<TARGET_FILE:libDaScript_runtime>
        -DWORKDIR=${CMAKE_CURRENT_BINARY_DIR}/wk
        -P ${CMAKE_CURRENT_SOURCE_DIR}/run_capi.cmake
)
# Label `small` is what CI's `ctest -L small` step picks up; the others are for local selection.
set_tests_properties(lib_capi PROPERTIES LABELS "small;lib-capi;integration")
