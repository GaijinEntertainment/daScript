# CI lane targets: verify formatting / regenerated-bindings freshness. Both run
# daslang interpreted (no LLVM), so define them before the DAS_LLVM_DISABLED
# early-return below.
add_custom_target(check_format
    COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/utils/das-fmt/dasfmt.das -- --path ${PROJECT_SOURCE_DIR}/ --verify --exclude-mask ${CMAKE_BINARY_DIR}
    DEPENDS daslang
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Verifying .das formatting (das-fmt --verify)"
)

add_custom_target(check_dasgen
    COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/utils/internal/dasgen/gen_bind.das
    COMMAND git diff --exit-code -- ${PROJECT_SOURCE_DIR}/include/daScript/builtin/
    DEPENDS daslang
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Checking generated builtin .inc files are up to date (dasgen)"
)

# Build daslang utilities as standalone executables using -exe mode.
# Each utility's entry .das is compiled via daslang -exe and the
# resulting binary is placed next to the daslang compiler binary
# (e.g. bin/Release/ on multi-config generators, bin/ on single-config).
# Requires LLVM for -exe compilation.

if(DAS_LLVM_DISABLED)
    return()
endif()

# JIT-linked util.exe pulls in libDaScriptDyn_runtime.so. When daslang is built
# with -fsanitize=*, the .so references sanitizer runtime helpers that the JIT
# link cmd (c++ without -fsanitize) can't satisfy. Forward the flag via
# --jit-linker-string so daslang's link cmd resolves the right runtime.
set(DAS_JIT_LINKER_STRING "")
if(DAS_USE_SANITIZER STREQUAL "address" OR DAS_USE_SANITIZER STREQUAL "asan")
    set(DAS_JIT_LINKER_STRING "--jit-linker-string=-fsanitize=address")
elseif(DAS_USE_SANITIZER STREQUAL "undefined" OR DAS_USE_SANITIZER STREQUAL "ubsan")
    set(DAS_JIT_LINKER_STRING "--jit-linker-string=-fsanitize=undefined")
elseif(DAS_USE_SANITIZER STREQUAL "thread" OR DAS_USE_SANITIZER STREQUAL "tsan")
    set(DAS_JIT_LINKER_STRING "--jit-linker-string=-fsanitize=thread")
endif()

# Entries are paths relative to utils/; the exe/target name is the basename.
# mcp/lsp never build as exe: their python keep-alive dev setup means an exe
# form would never be dogfooded.
set(DAS_UTILS
    aot
    dascov
    daspkg
    das-fmt
    detect-dupe
    lint
    internal/hygiene
    internal/jit
)

if(NOT DAS_SQLITE_DISABLED)
    list(APPEND DAS_UTILS benchctl)   # same gate on DAS_UTILS_SHIPPED_EXES below
endif()

# Collect daslib and LLVM daslib sources as dependencies. jit is a compiler-
# driver: edits anywhere under modules/dasLLVM/ (llvm_exe.das, llvm_jit_cli.das,
# llvm_jit_run.das, bindings/*.das) must trigger a jit.exe rebuild. Use a
# recursive glob so subdirs are covered.
file(GLOB DASLIB_SOURCES ${PROJECT_SOURCE_DIR}/daslib/*.das)
file(GLOB_RECURSE DASLLVM_DASLIB_SOURCES ${PROJECT_SOURCE_DIR}/modules/dasLLVM/*.das)

# daslang -exe appends `.exe` to the output path.
# On multi-config generators (Visual Studio), place next to daslang in
# bin/<Config>/. On single-config generators (Make/Ninja), place in bin/.
# Key on the GENERATOR, not the compiler: MSVC+Ninja (release CI) is
# single-config and its exes land in plain bin/.
get_property(_DAS_UTILS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(_DAS_UTILS_MULTI_CONFIG)
    set(UTIL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin/${CMAKE_CFG_INTDIR})
else()
    set(UTIL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
endif()

foreach(util_path ${DAS_UTILS})
    get_filename_component(util ${util_path} NAME)
    # dasfmt.das is das-fmt's published entry point — external CIs and the
    # shipped pre-commit hook invoke it by name, so it does not rename to main.das
    if(util STREQUAL "das-fmt")
        set(UTIL_MAIN ${PROJECT_SOURCE_DIR}/utils/${util_path}/dasfmt.das)
    else()
        set(UTIL_MAIN ${PROJECT_SOURCE_DIR}/utils/${util_path}/main.das)
    endif()

    # jit and lint are compiler-drivers: their standalone exes recompile
    # arbitrary daslang at runtime (lint compiles each target to walk its AST),
    # so they must register every builtin native module at startup — otherwise
    # targets requiring math/fio/ast_core/... fail to resolve.
    set(_util_extra_args "")
    if(util STREQUAL "jit" OR util STREQUAL "lint")
        set(_util_extra_args --jit-register-all-modules)
    endif()

    add_custom_command(
        OUTPUT  ${UTIL_BIN_DIR}/${util}.exe
        COMMAND daslang -exe -output ${UTIL_BIN_DIR}/${util} ${UTIL_MAIN}
                $<$<OR:$<BOOL:${DAS_JIT_LINKER_STRING}>,$<BOOL:${_util_extra_args}>>:-->
                ${_util_extra_args}
                ${DAS_JIT_LINKER_STRING}
        DEPENDS ${UTIL_MAIN} daslang ${DASLIB_SOURCES} ${DASLLVM_DASLIB_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Building ${util} executable (daslang -exe)"
    )

    add_custom_target(${util}_exe DEPENDS ${UTIL_BIN_DIR}/${util}.exe)
endforeach()

SET(TEST_MAIN ${PROJECT_SOURCE_DIR}/dastest/dastest.das)
file(GLOB DASTEST_SOURCES ${PROJECT_SOURCE_DIR}/dastest/*.das)

add_custom_command(
    OUTPUT  ${UTIL_BIN_DIR}/dastest.exe
    COMMAND daslang -exe -output ${UTIL_BIN_DIR}/dastest ${TEST_MAIN}
            $<$<BOOL:${DAS_JIT_LINKER_STRING}>:-->
            ${DAS_JIT_LINKER_STRING}
    DEPENDS ${TEST_MAIN} ${DASTEST_SOURCES} daslang ${DASLIB_SOURCES} ${DASLLVM_DASLIB_SOURCES}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Building dastest executable (daslang -exe)"
)
add_custom_target(dastest_exe DEPENDS ${UTIL_BIN_DIR}/dastest.exe)


add_custom_target(all_utils_exe ALL)
foreach(util_path ${DAS_UTILS})
    get_filename_component(util ${util_path} NAME)
    add_dependencies(all_utils_exe ${util}_exe)
endforeach()
add_dependencies(all_utils_exe dastest_exe)

# Single-config generators only: UTIL_BIN_DIR embeds CMAKE_CFG_INTDIR on
# multi-config, which install() cannot resolve — and release bundles are cut on
# single-config (Ninja) lanes. Each tool's exe-or-not decision is recorded in
# that tool's REVIEW.md.
if(NOT _DAS_UTILS_MULTI_CONFIG)
    set(DAS_UTILS_SHIPPED_EXES lint daspkg dascov detect-dupe das-fmt dastest)
    if(NOT DAS_SQLITE_DISABLED)
        list(APPEND DAS_UTILS_SHIPPED_EXES benchctl)   # same gate on DAS_UTILS above
    endif()
    foreach(util ${DAS_UTILS_SHIPPED_EXES})
        install(PROGRAMS ${UTIL_BIN_DIR}/${util}.exe DESTINATION ${DAS_INSTALL_BINDIR})
    endforeach()
endif()

# Prewarm the JIT dll cache for the whole test tree, except module-gated folders and gc
# (mirrors tests/.das_test: gc stays interp-only under -jit — heap_collect can't see
# jitted-frame locals; quote/qmacro folders prewarm fine now that jit_enabled triggers
# daslib/quote lowering). Reuses the jit_exe target built by the foreach above
# (bin/jit.exe, built with --jit-register-all-modules). Run explicitly:
#   ninja jit_cache_all_tests
add_custom_target(jit_cache_all_tests
    COMMAND ${UTIL_BIN_DIR}/jit.exe ${PROJECT_SOURCE_DIR}/tests --parallel 0 $<IF:$<CONFIG:Debug>,--jit-opt-level=0,--jit-opt-level=3> --exclude gc --exclude dasHV --exclude dasPUGIXML --exclude dasSQLITE --exclude sql_conformance --exclude stbimage --exclude dasGLTF --exclude live_host --exclude audio --exclude strudel --exclude strudel_device --exclude minfft
    DEPENDS jit_exe
    WORKING_DIRECTORY ${UTIL_BIN_DIR}
    COMMENT "JIT-compiling tests/ into the .jitted_scripts cache"
)

SET(DAS_UTILS_TO_TEST
    internal/arch-extract/tests
    dascov
    mcp
    lint
    detect-dupe
    find-dupe/tests
    gen1-to-gen2/tests
    internal/hygiene
    internal/preflight/tests
    internal/ast-fuzz
    internal/requirefix
    internal/test-release
)

FOREACH(_das ${DAS_UTILS_TO_TEST})
    LIST(APPEND _run_utils_tests
        COMMAND ${CMAKE_COMMAND} -E echo "Running tests ${_das}"
        COMMAND daslang ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --test ${PROJECT_SOURCE_DIR}/utils/${_das}
        DEPENDS tree_sitter_daslang
    )
ENDFOREACH()

ADD_CUSTOM_TARGET(run_utils_tests
    ${_run_utils_tests}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Running utils tests"
)

# watchdog: the supervisor as a standalone context on the full runtime - libDaScript and dasHV
# as static archives, no compiler, no shared module, no lock on the files it deploys (the shape of
# examples/standalone/06_full_runtime). Lands in bin/ and installs beside daslang.
if(NOT DAS_HV_DISABLED)
    set(WATCHDOG_GEN "${CMAKE_CURRENT_BINARY_DIR}/_watchdog_ctx_generated")
    file(MAKE_DIRECTORY "${WATCHDOG_GEN}")
    file(GLOB WATCHDOG_DAS_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/utils/watchdog/*.das")
    add_custom_command(
        OUTPUT "${WATCHDOG_GEN}/main.das.cpp" "${WATCHDOG_GEN}/main.das.h"
        COMMAND $<TARGET_FILE:daslang>
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            -- -ctx "${PROJECT_SOURCE_DIR}/utils/watchdog/main.das"
            "${WATCHDOG_GEN}/"
        DEPENDS daslang
            ${WATCHDOG_DAS_SOURCES}
            "${PROJECT_SOURCE_DIR}/utils/aot/main.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_standalone.das"
            "${PROJECT_SOURCE_DIR}/daslib/aot_cpp.das"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Standalone AOT (full runtime): watchdog"
        VERBATIM
    )
    add_executable(watchdog
        "${PROJECT_SOURCE_DIR}/utils/watchdog/main.cpp" "${WATCHDOG_GEN}/main.das.cpp")
    target_include_directories(watchdog PRIVATE
        "${WATCHDOG_GEN}"
        ${NEED_MODULES_PATH}
        "${PROJECT_SOURCE_DIR}/modules/dasHV/hv/$<CONFIG>/include")
    target_link_libraries(watchdog PRIVATE libDaScript ${SRC_LIBRARIES} libDasModuleHV)
    SETUP_CPP11(watchdog)
    SETUP_BIN_RPATH(watchdog)
    set_target_properties(watchdog PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
        FOLDER "utils")
    if(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET watchdog POST_BUILD
            COMMAND ${CMAKE_STRIP} $<TARGET_FILE:watchdog>
            COMMENT "Stripping watchdog")
    endif()
    INSTALL(TARGETS watchdog RUNTIME DESTINATION ${DAS_INSTALL_BINDIR})
    add_test(NAME watchdog_smoke
        COMMAND ${CMAKE_COMMAND}
            -DWATCHDOG=$<TARGET_FILE:watchdog>
            -DDASLANG=$<TARGET_FILE:daslang>
            -DDAS_ROOT=${PROJECT_SOURCE_DIR}
            -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}/watchdog_smoke
            -P ${PROJECT_SOURCE_DIR}/utils/watchdog/smoke_test.cmake)
    set_tests_properties(watchdog_smoke PROPERTIES LABELS "small" TIMEOUT 120)
    add_dependencies(test-small watchdog)
endif()
