if(DAS_TOOLS_DISABLED)
    return()
endif()

add_das_target(run_tests_bench_updater
    COMMAND $<TARGET_FILE:daslang> ${PROJECT_SOURCE_DIR}/dastest/dastest.das --
            --color --failures-only --test ${CMAKE_CURRENT_SOURCE_DIR}/sql/tests/test_update_results.das
    DEPENDS daslang
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Testing the benchmark results updater"
)
