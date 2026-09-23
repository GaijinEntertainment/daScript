# Test-runner targets, one per execution mode, invoked by CI lanes via
# `cmake --build build --target run_tests_<mode>`. Added from the root via
# add_subdirectory(tests) after daslang exists. The test_aot binary itself is
# built by tests/aot/CMakeLists.txt, include()d from the root earlier.
# Each mode has a primary target (fast path) and an _isolated variant
# (--isolated-mode, one context per test). CI chains them as the flaky-test
# retry: `ninja run_tests_X || ninja run_tests_X_isolated`.
# cmake has no OR/retry primitive inside a single target, so the `||` lives in CI.
# NOTE: --timeout is dastest's GLOBAL suite watchdog (wall clock for the whole
# run), not per test. The defaults below fit every lane whose interpreter sweep
# lands well inside them; a lane that needs more says so at configure time via
# -DDAS_TEST_TIMEOUT / -DDAS_TEST_TIMEOUT_ISOLATED rather than inflating the
# budget everywhere - a watchdog only catches a hang while it is still tight.
# The RelWithDebInfo Windows nightly is the lane that needs it: DAS_TRACK_ALLOC
# with the free list off roughly doubles the sweep, and the hosted runner adds
# another ~1.4x of its own variance on top.
if(NOT DAS_TOOLS_DISABLED)
    set(DAS_TEST_TIMEOUT 1800 CACHE STRING
        "dastest global suite watchdog, seconds, for run_tests_interpreter")
    set(DAS_TEST_TIMEOUT_ISOLATED 5400 CACHE STRING
        "dastest global suite watchdog, seconds, for run_tests_interpreter_isolated")

    set(_DAS_DASTEST  ${PROJECT_SOURCE_DIR}/dastest/dastest.das)
    set(_DAS_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
    set(_DAS_TEST_COMMON --color --failures-only --test ${_DAS_TEST_DIR})

    set(_DAS_TEST_HOSTS daslang)
    foreach(_host daslang-live watchdog dasbind_probe)
        if(TARGET ${_host})
            list(APPEND _DAS_TEST_HOSTS ${_host})
        endif()
    endforeach()

    add_das_target(run_tests_interpreter
        COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -- ${_DAS_TEST_COMMON} --timeout ${DAS_TEST_TIMEOUT}
        DEPENDS ${_DAS_TEST_HOSTS}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (interpreter)"
    )
    add_das_target(run_tests_interpreter_isolated
        COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -- ${_DAS_TEST_COMMON} --isolated-mode --timeout ${DAS_TEST_TIMEOUT_ISOLATED}
        DEPENDS ${_DAS_TEST_HOSTS}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (interpreter, isolated retry)"
    )

    if(NOT DAS_LLVM_DISABLED)
        # Sanitizer builds JIT-link without -fsanitize unless told; forward it.
        set(_DAS_JIT_LINKER_STRING "")
        if(DAS_USE_SANITIZER STREQUAL "address" OR DAS_USE_SANITIZER STREQUAL "asan")
            set(_DAS_JIT_LINKER_STRING "--jit-linker-string=-fsanitize=address")
        elseif(DAS_USE_SANITIZER STREQUAL "undefined" OR DAS_USE_SANITIZER STREQUAL "ubsan")
            set(_DAS_JIT_LINKER_STRING "--jit-linker-string=-fsanitize=undefined")
        elseif(DAS_USE_SANITIZER STREQUAL "thread" OR DAS_USE_SANITIZER STREQUAL "tsan")
            set(_DAS_JIT_LINKER_STRING "--jit-linker-string=-fsanitize=thread")
        endif()
        # A sanitizer build links its JIT dll with the compiler that built daslang: c++ is g++
        # on Ubuntu, whose asan/tsan runtime the clang-built binary cannot load.
        set(_DAS_JIT_PATH_TO_LINKER "")
        if(DAS_USE_SANITIZER AND NOT DAS_USE_SANITIZER STREQUAL "none")
            set(_DAS_JIT_PATH_TO_LINKER "--jit-path-to-linker=${CMAKE_CXX_COMPILER}")
        endif()
        set(_DAS_JIT_FLAGS ${_DAS_JIT_LINKER_STRING} ${_DAS_JIT_PATH_TO_LINKER} $<IF:$<CONFIG:Debug>,--jit-opt-level=0,--jit-opt-level=3>)

        # The sanitizer runtimes read these from the environment of the process under test, so
        # the target carries them - otherwise every caller repeats them and they drift apart.
        # detect_leaks=0 for the JIT sweep: it exits through exit(), running no destructors.
        set(_DAS_SAN_ENV ${DAS_SANITIZER_ENV})
        if(DAS_SANITIZER_ENV)
            list(APPEND _DAS_SAN_ENV "ASAN_OPTIONS=detect_leaks=0")
        endif()

        # A sanitizer lane sweeps tests/language only: the whole suite under asan/tsan/ubsan
        # costs ~10 minutes more without proportionate coverage.
        set(_DAS_JIT_TEST_DIR ${_DAS_TEST_DIR})
        if(DAS_USE_SANITIZER AND NOT DAS_USE_SANITIZER STREQUAL "none")
            set(_DAS_JIT_TEST_DIR ${_DAS_TEST_DIR}/language)
        endif()
        set(_DAS_JIT_COMMON --color --failures-only --test ${_DAS_JIT_TEST_DIR})

        set(DAS_TEST_JIT_TIMEOUT 1800 CACHE STRING
            "dastest global suite watchdog, seconds, for run_tests_jit")
        set(DAS_TEST_JIT_TIMEOUT_ISOLATED 3600 CACHE STRING
            "dastest global suite watchdog, seconds, for run_tests_jit_isolated")
        # Primary JIT sweep runs isolated-PARALLEL (2x-hw-thread worker
        # subprocesses, 4 files per batch): ~3x FASTER wall-clock than the sequential
        # sweep with an identical pass/fail set, and cold-cache parallel beats
        # warm sequential — which is what made the CI "Prewarm JIT cache" step
        # redundant. Safe because each test program owns its dll-cache path
        # (.jitted_scripts/<namespace>/<contenthash>) and each file runs in
        # exactly one batch. The _isolated retry stays batch-1 (one process per
        # test — max crash isolation, and a batch crash re-dispatches per-file).
        add_das_target(run_tests_jit
            COMMAND ${_DAS_SAN_ENV} $<TARGET_FILE:daslang> ${_DAS_DASTEST} -jit -- ${_DAS_JIT_FLAGS} ${_DAS_JIT_COMMON} --isolated-mode --batch 4 --timeout ${DAS_TEST_JIT_TIMEOUT}
            DEPENDS ${_DAS_TEST_HOSTS}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running tests (JIT, isolated-parallel)"
        )
        add_das_target(run_tests_jit_isolated
            COMMAND ${_DAS_SAN_ENV} $<TARGET_FILE:daslang> ${_DAS_DASTEST} -jit -- ${_DAS_JIT_FLAGS} ${_DAS_JIT_COMMON} --isolated-mode --timeout ${DAS_TEST_JIT_TIMEOUT_ISOLATED}
            DEPENDS ${_DAS_TEST_HOSTS}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running tests (JIT, isolated retry)"
        )
    endif()
endif()

# test_aot only exists when tests + AOT examples are enabled (tests/aot/CMakeLists.txt).
# The full binary is EXCLUDE_FROM_ALL; this target builds it on demand (nightly CI,
# preflight --full).
if(TARGET test_aot)
    add_das_target(run_tests_aot
        COMMAND ${DAS_SANITIZER_ENV} $<TARGET_FILE:test_aot> -use-aot ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --use-aot --color --failures-only --timeout 1800 --test ${PROJECT_SOURCE_DIR}/tests
        DEPENDS test_aot
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (AOT)"
    )
endif()

# Fast AOT run over tests/language only, against the subset binary that per-PR
# CI already builds as part of ALL. For local/debug use — PR CI treats the
# subset build itself as the gate and runs no AOT tests.
if(TARGET test_aot_subset)
    add_das_target(run_tests_aot_subset
        COMMAND ${DAS_SANITIZER_ENV} $<TARGET_FILE:test_aot_subset> -use-aot ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --use-aot --color --failures-only --timeout 1800 --test ${PROJECT_SOURCE_DIR}/tests/language
        DEPENDS test_aot_subset
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (AOT, tests/language subset)"
    )
endif()

# LLVM-AOT (LLVM-backend .o) mirror of run_tests_aot: test bodies bound from the
# statically-linked self-registering objects and run jitted. Opt-in, not in CI.
if(TARGET test_llvm_aot)
    add_das_target(run_tests_llvm_aot
        COMMAND ${DAS_SANITIZER_ENV} $<TARGET_FILE:test_llvm_aot> -use-aot ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --use-aot --color --failures-only --isolated-mode --batch 4 --timeout 1800 --test-list ${LLVM_AOT_TEST_LIST} --test ${PROJECT_SOURCE_DIR}/tests/
        DEPENDS test_llvm_aot
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (LLVM-AOT)"
    )
endif()

# Two standalone sweeps over the test corpus, one per backend. Opt-in, out of CI.
add_subdirectory(standalone-sweep)

if(NOT DAS_TOOLS_DISABLED)

set(_lane_lsan "LSAN_OPTIONS=suppressions=${PROJECT_SOURCE_DIR}/.github/lsan_suppressions.txt")
set(_lane_san_env ${_lane_lsan}
    "TSAN_OPTIONS=suppressions=${PROJECT_SOURCE_DIR}/.github/tsan_suppressions.txt"
    "UBSAN_OPTIONS=print_stacktrace=1")

function(das_lane_dastest name comment)
    set(_cmds)
    foreach(_f IN LISTS ARGN)
        list(APPEND _cmds COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/dastest/dastest.das --
             --color --failures-only --test ${PROJECT_SOURCE_DIR}/${_f})
    endforeach()
    add_custom_target(${name} ${_cmds} DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMENT "${comment}")
endfunction()

function(das_ci_sweeps out isolated)
    set(_deps)
    set(_jit run_tests_jit)
    set(_interp run_tests_interpreter)
    if(isolated)
        set(_jit run_tests_jit_isolated)
        set(_interp run_tests_interpreter_isolated)
    endif()
    if(TARGET ${_jit} AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND _deps ${_jit})
    endif()
    if(NOT DAS_USE_SANITIZER OR DAS_USE_SANITIZER STREQUAL "none")
        list(APPEND _deps ${_interp})
    endif()
    set(${out} ${_deps} PARENT_SCOPE)
endfunction()

das_ci_sweeps(_ci_sweeps OFF)

das_ci_sweeps(_ci_sweeps_isolated ON)

add_custom_target(run_tests_sweeps COMMENT "Running the per-PR sweeps")
if(NOT "${_ci_sweeps}" STREQUAL "")
    add_dependencies(run_tests_sweeps ${_ci_sweeps})
endif()

add_custom_target(run_tests_sweeps_isolated COMMENT "Running the per-PR sweeps")
if(NOT "${_ci_sweeps_isolated}" STREQUAL "")
    add_dependencies(run_tests_sweeps_isolated ${_ci_sweeps_isolated})
endif()

add_das_target(run_tests_static
    COMMAND $<TARGET_FILE:daslang_static> ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --color --failures-only --test ${PROJECT_SOURCE_DIR}/tests
    DEPENDS daslang_static
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Running tests (daslang_static)"
)

add_das_target(run_tests_serdeser
    COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --color --failures-only --test ${PROJECT_SOURCE_DIR}/tests --ser ${CMAKE_CURRENT_BINARY_DIR}/serialized.bin --timeout 1200
    COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --color --failures-only --test ${PROJECT_SOURCE_DIR}/tests --deser ${CMAKE_CURRENT_BINARY_DIR}/serialized.bin --timeout 1200
    DEPENDS daslang
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Running tests (serialize, then deserialize)"
)



add_custom_target(run_tests_slow COMMENT "Running the full AOT suite and the big C++ tests")
if(TARGET run_tests_aot)
    add_dependencies(run_tests_slow run_tests_aot)
endif()
if(TARGET test-big)
    add_dependencies(run_tests_slow test-big)
endif()

if(NOT _ci_backend)
    set(_ci_backend COMMAND ${CMAKE_COMMAND} -E true)
endif()
add_custom_target(run_backend_sweeps COMMENT "Running the nightly backend sweeps")
foreach(_t run_standalone_sweep_aot run_tests_llvm_aot standalone_sweep_jit)
    if(TARGET ${_t} AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        add_dependencies(run_backend_sweeps ${_t})
    endif()
endforeach()

file(GLOB_RECURSE _ci_ast_verify_files RELATIVE ${PROJECT_SOURCE_DIR} CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/*.das)
set(_ci_ast_verify_stamps)
foreach(_f IN LISTS _ci_ast_verify_files)
    get_filename_component(_name ${_f} NAME)
    if(_name MATCHES "^(cant_|failed_|invalid_)")
        continue()
    endif()
    set(_stamp ${CMAKE_CURRENT_BINARY_DIR}/ast_verify/${_f}.stamp)
    add_custom_command(
        OUTPUT ${_stamp}
        COMMAND $<TARGET_FILE:daslang> ./utils/ast-verify/main.das -- --file ${_f}
        COMMAND ${CMAKE_COMMAND} -E touch ${_stamp}
        DEPENDS daslang ${PROJECT_SOURCE_DIR}/${_f} ${PROJECT_SOURCE_DIR}/utils/ast-verify/main.das
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "ast-verify ${_f}"
    )
    list(APPEND _ci_ast_verify_stamps ${_stamp})
endforeach()
add_custom_target(check_ast_verify_tree
    DEPENDS ${_ci_ast_verify_stamps}
    COMMENT "Compiling tests/ with --ast-verify-batch"
)

endif()
