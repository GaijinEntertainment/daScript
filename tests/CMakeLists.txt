# Test-runner targets, one per execution mode, invoked by CI lanes via
# `cmake --build build --target run_tests_<mode>`. Added from the root via
# add_subdirectory(tests) after daslang exists. The test_aot binary itself is
# built by tests/aot/CMakeLists.txt, include()d from the root earlier.
# Each mode has a primary target (fast path, --timeout 900) and an _isolated
# variant (--isolated-mode --timeout 3600, one context per test). CI chains them
# as the flaky-test retry: `ninja run_tests_X || ninja run_tests_X_isolated`.
# cmake has no OR/retry primitive inside a single target, so the `||` lives in CI.
if(NOT DAS_TOOLS_DISABLED)
    set(_DAS_DASTEST  ${PROJECT_SOURCE_DIR}/dastest/dastest.das)
    set(_DAS_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
    set(_DAS_TEST_COMMON --color --failures-only --test ${_DAS_TEST_DIR})

    add_custom_target(run_tests_interpreter
        COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -- ${_DAS_TEST_COMMON} --timeout 900
        DEPENDS daslang
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (interpreter)"
    )
    add_custom_target(run_tests_interpreter_isolated
        COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -- ${_DAS_TEST_COMMON} --isolated-mode --timeout 3600
        DEPENDS daslang
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
        set(_DAS_JIT_FLAGS ${_DAS_JIT_LINKER_STRING} $<IF:$<CONFIG:Debug>,--jit-opt-level=0,--jit-opt-level=3>)
        add_custom_target(run_tests_jit
            COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -jit -- ${_DAS_JIT_FLAGS} ${_DAS_TEST_COMMON} --timeout 900
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running tests (JIT)"
        )
        add_custom_target(run_tests_jit_isolated
            COMMAND $<TARGET_FILE:daslang> ${_DAS_DASTEST} -jit -- ${_DAS_JIT_FLAGS} ${_DAS_TEST_COMMON} --isolated-mode --timeout 3600
            DEPENDS daslang
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running tests (JIT, isolated retry)"
        )
    endif()
endif()

# test_aot only exists when tests + AOT examples are enabled (tests/aot/CMakeLists.txt).
if(TARGET test_aot)
    add_custom_target(run_tests_aot
        COMMAND $<TARGET_FILE:test_aot> -use-aot ${PROJECT_SOURCE_DIR}/dastest/dastest.das -- --use-aot --color --failures-only --timeout 900 --test ${PROJECT_SOURCE_DIR}/tests
        DEPENDS test_aot
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running tests (AOT)"
    )
endif()
