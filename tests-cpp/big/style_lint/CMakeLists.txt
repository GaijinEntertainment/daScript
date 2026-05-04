# Big test: utils/lint must surface STYLE014/STYLE015 on the canonical
# comment-hygiene fixtures. Catches regressions in the option/flag plumbing
# and the scan_long_comment_blocks pass.

file(GLOB STYLE_LINT_FIXTURES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/utils/lint/tests/style014_*.das
    ${PROJECT_SOURCE_DIR}/utils/lint/tests/style015_*.das)

foreach(_fixture ${STYLE_LINT_FIXTURES})
    get_filename_component(_name ${_fixture} NAME_WE)
    add_test(
        NAME style_lint_${_name}
        COMMAND $<TARGET_FILE:daslang>
                ${PROJECT_SOURCE_DIR}/utils/lint/main.das
                -- --comment-hygiene true ${_fixture}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    set_tests_properties(style_lint_${_name} PROPERTIES
        LABELS "big;style_lint"
        PASS_REGULAR_EXPRESSION "STYLE014|STYLE015")
endforeach()

add_dependencies(test-big daslang)
