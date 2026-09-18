if(DAS_TOOLS_DISABLED)
    return()
endif()

function(das_lane_python name comment)
    set(_cmds)
    foreach(_args IN LISTS ARGN)
        string(REPLACE " " ";" _args "${_args}")
        list(APPEND _cmds COMMAND ${DAS_PYTHON} ${_args})
    endforeach()
    add_custom_target(${name} ${_cmds} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMENT "${comment}")
endfunction()

das_lane_python(check_workflow_matrix "Testing the workflow matrix and role conditions" "ci/test_ci_matrix.py")
das_lane_python(check_shipped_skills "Testing the shipped-skills gate" "ci/test_check_shipped_skills.py")
das_lane_python(check_md_ascii "Markdown ASCII gate" "ci/test_fix_md_ascii.py" "ci/fix_md_ascii.py --check")
das_lane_python(check_wheel_repack "Testing the pip wheel repack" "ci/test_wheel_build.py")

add_custom_target(check_bundle_smoke
COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/daslang_bundle
COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix ${CMAKE_BINARY_DIR}/daslang_bundle --config $<CONFIG> --strip
COMMAND bash ${PROJECT_SOURCE_DIR}/ci/smoke_test_bundle.sh ${CMAKE_BINARY_DIR}/daslang_bundle
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
COMMENT "Installing the release bundle and smoke-testing it"
DEPENDS daslang libDaScript libDaScript_runtime libDaScriptDyn libDaScriptDyn_runtime libUriParser libUriParserDyn
)

if(TARGET cpp-mcp)
    add_custom_target(make_cpp_mcp_bundle
        COMMAND bash ${PROJECT_SOURCE_DIR}/ci/make_cpp_mcp_bundle.sh ${PROJECT_SOURCE_DIR}/dist/cpp-mcp
        DEPENDS cpp-mcp
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Assembling the cpp-mcp bundle"
    )
    add_custom_target(check_cpp_mcp_bundle
        COMMAND bash ${PROJECT_SOURCE_DIR}/ci/smoke_test_cpp_mcp.sh ${PROJECT_SOURCE_DIR}/dist/cpp-mcp
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Smoke-testing the cpp-mcp bundle"
    )
    add_dependencies(check_cpp_mcp_bundle make_cpp_mcp_bundle)
endif()

if(TARGET daslang_static)
    add_custom_target(run_eastl_smoke
        COMMAND $<TARGET_FILE:daslang_static> examples/clargs/main.das -- --name ci -r 1
        DEPENDS daslang_static
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Smoke-running the EASTL-config daslang_static"
    )
endif()

function(das_lane_group name comment)
    set(_deps)
    set(_skipped)
    foreach(_t IN LISTS ARGN)
        if(TARGET ${_t})
            list(APPEND _deps ${_t})
        else()
            list(APPEND _skipped ${_t})
        endif()
    endforeach()
    if(_skipped)
        message(STATUS "${name}: this configure defines no ${_skipped}")
    endif()
    add_custom_target(${name} COMMENT "${comment}")
    if(_deps)
        add_dependencies(${name} ${_deps})
    endif()
endfunction()

set(DAS_EXTENDED_CORE
    check_workflow_matrix check_shipped_skills check_md_ascii check_wheel_repack
    check_example_games check_dasllama_site check_model_mint run_tests_bench_updater
    run_tests_pr_babysit check_dasgen run_tests_review_md run_tests_dastest
    check_lint_changed check_ast_verify_changed check_gated_das check_review_gates
    check_standalone_exes check_format check_format_exe run_utils_tests)
if(NOT WIN32)
    list(APPEND DAS_EXTENDED_CORE run_tests_dap_bridge)
endif()

set(DAS_EXTENDED_MODULES
    run_tests_dasllama_env_registry run_tests_llvm_env_registry run_tests_glfw_touch
    run_tests_dasllama_facade_lint run_tests_boulder_dash run_tests_daspkg
    run_tests_mcp_tools run_tests_dasweb_verify run_tests_dasllama_ladder
    run_tests_dasweb_buildd run_tests_dasweb_playground run_sequence_smoke
    run_tests_dasllama_server run_tests_serdeser run_tests_static)
if(APPLE)
    list(APPEND DAS_EXTENDED_MODULES run_tests_llvm_vector_math)
endif()

set(DAS_EXTENDED_NIGHTLY run_examples dry_run_tutorials)
if(NOT WIN32)
    list(APPEND DAS_EXTENDED_NIGHTLY check_doc_verify)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND DAS_EXTENDED_NIGHTLY check_nano_arm check_ast_verify_tree run_coverage)
endif()

das_lane_group(run_extended_core "Running the extended checks (core)" ${DAS_EXTENDED_CORE})
das_lane_group(run_extended_modules "Running the extended checks (modules)" ${DAS_EXTENDED_MODULES})
das_lane_group(run_extended_all "Running every extended check"
    ${DAS_EXTENDED_CORE} ${DAS_EXTENDED_MODULES} ${DAS_EXTENDED_NIGHTLY})

das_lane_group(run_build "Running the build lane"
    run_tests_sweeps test-small)
das_lane_group(run_build_nightly "Running the build lane's nightly sweeps"
    run_tests_slow run_backend_sweeps)
das_lane_group(run_site "Running the site lane's checks"
    run_imgui2rst run_vulkan2rst stage_doc_assets)
das_lane_group(run_nightly "Running everything CI runs only on the cron"
    run_build_nightly ${DAS_EXTENDED_NIGHTLY})
das_lane_group(run_all "Running every lane this configure can build"
    run_extended_all run_doc run_build run_build_nightly)

# A lane runs daslang against the tree's own shared modules and descriptors. Under a single
# ninja invocation a relink lands while another lane is reading that module, and its descriptor
# manifest (keyed on dep size and hash) goes stale mid-read. CI never sees this: it builds in
# one step and tests in the next. The edge goes on every leaf, not on the groups - a dependency
# of a group is a sibling of its members and ninja may still run them together.
add_custom_target(das_build_products COMMENT "Building what the lanes load at runtime")
add_dependencies(das_build_products daslang)
get_property(_das_modules GLOBAL PROPERTY DAS_SHARED_MODULE_TARGETS)
if(_das_modules)
    add_dependencies(das_build_products ${_das_modules})
endif()
foreach(_lane ${DAS_EXTENDED_CORE} ${DAS_EXTENDED_MODULES} ${DAS_EXTENDED_NIGHTLY}
              test-small test-big check_bundle_smoke
              run_tests_interpreter run_tests_interpreter_isolated
              run_tests_jit run_tests_jit_isolated
              run_tests_aot run_tests_aot_subset run_tests_llvm_aot
              run_tests_slow run_standalone_sweep_aot standalone_sweep_jit
              check_ast_verify_tree run_examples dry_run_tutorials
              check_docs_generated run_imgui2rst run_vulkan2rst stage_doc_assets)
    if(TARGET ${_lane})
        add_dependencies(${_lane} das_build_products)
    endif()
endforeach()
