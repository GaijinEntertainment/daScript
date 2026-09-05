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

    add_custom_target(run_tests_interpreter
        COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -- ${_DAS_TEST_COMMON} --timeout ${DAS_TEST_TIMEOUT}
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (interpreter)"
        USES_TERMINAL
    )
    add_custom_target(run_tests_interpreter_isolated
        COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -- ${_DAS_TEST_COMMON} --isolated-mode --timeout ${DAS_TEST_TIMEOUT_ISOLATED}
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (interpreter, isolated retry)"
        USES_TERMINAL
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
        set(_DAS_JIT_FLAGS ${_DAS_JIT_LINKER_STRING} $<IF:$<CONFIG:Debug>,--jit-opt-level=0,--jit-opt-level=3>)
        # Primary JIT sweep runs isolated-PARALLEL (2x-hw-thread worker
        # subprocesses, 4 files per batch): ~3x FASTER wall-clock than the sequential
        # sweep with an identical pass/fail set, and cold-cache parallel beats
        # warm sequential — which is what made the CI "Prewarm JIT cache" step
        # redundant. Safe because each test program owns its dll-cache path
        # (.jitted_scripts/<namespace>/<contenthash>) and each file runs in
        # exactly one batch. The _isolated retry stays batch-1 (one process per
        # test — max crash isolation, and a batch crash re-dispatches per-file).
        add_custom_target(run_tests_jit
            COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -jit -- ${_DAS_JIT_FLAGS} ${_DAS_TEST_COMMON} --isolated-mode --batch 4 --timeout 1800
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running tests (JIT, isolated-parallel)"
            USES_TERMINAL
        )
        add_custom_target(run_tests_jit_isolated
            COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -jit -- ${_DAS_JIT_FLAGS} ${_DAS_TEST_COMMON} --isolated-mode --timeout 3600
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running tests (JIT, isolated retry)"
            USES_TERMINAL
        )
    endif()
endif()

# test_aot only exists when tests + AOT examples are enabled (tests/aot/CMakeLists.txt).
# The full binary is EXCLUDE_FROM_ALL; this target builds it on demand (nightly CI,
# preflight --full).
if(TARGET test_aot)
    add_custom_target(run_tests_aot
        COMMAND $<TARGET_FILE:test_aot> -use-aot ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --use-aot --color --failures-only --timeout 1800 --test ${PROJECT_SOURCE_DIR}/tests
        DEPENDS test_aot
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (AOT)"
        USES_TERMINAL
    )
endif()

# Fast AOT run over tests/language only, against the subset binary that per-PR
# CI already builds as part of ALL. For local/debug use — PR CI treats the
# subset build itself as the gate and runs no AOT tests.
if(TARGET test_aot_subset)
    add_custom_target(run_tests_aot_subset
        COMMAND $<TARGET_FILE:test_aot_subset> -use-aot ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --use-aot --color --failures-only --timeout 1800 --test ${PROJECT_SOURCE_DIR}/tests/language
        DEPENDS test_aot_subset
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (AOT, tests/language subset)"
        USES_TERMINAL
    )
endif()

# LLVM-AOT (LLVM-backend .o) mirror of run_tests_aot: test bodies bound from the
# statically-linked self-registering objects and run jitted. Opt-in, not in CI.
if(TARGET test_llvm_aot)
    # -jit (fun->hash matches the --aot-object emit) AND --use-aot (linkCppAot binds
    # the .o as SimNode_Jit; run_jit no-ops). --exclude must match the corpus filter in
    # tests/aot/CMakeLists.txt: files with no emitted object would miss (a hard error now).
    add_custom_target(run_tests_llvm_aot
        COMMAND $<TARGET_FILE:test_llvm_aot> ${PROJECT_SOURCE_DIR}/dastest/dastest.das -jit -- --use-aot --color --failures-only --isolated-mode --batch 4 --timeout 1800 --exclude jit_fastpath --exclude typeinfo --exclude global_variables_solid --exclude llvm_code --exclude llvm_compile_only --exclude llvm_compile_only_client --exclude test_msl_ --exclude test_metal_ --test ${PROJECT_SOURCE_DIR}/tests/
        DEPENDS test_llvm_aot
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (LLVM-AOT)"
        USES_TERMINAL
    )
endif()
