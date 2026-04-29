# Big test: thread-safety stress test for module init (issue #2027).
# Built in two link variants — static (libDaScript) and dynamic (libDaScriptDyn).

# ── Static-link variant ─────────────────────────────────────────────────────
add_executable(test_concurrent_init test_concurrent_init.cpp)
target_link_libraries(test_concurrent_init PRIVATE
    libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
target_include_directories(test_concurrent_init PRIVATE ${NEED_MODULES_PATH})
SETUP_CPP11(test_concurrent_init)
set_target_properties(test_concurrent_init PROPERTIES FOLDER "tests-cpp/big")

add_test(NAME concurrent_init COMMAND test_concurrent_init
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(concurrent_init PROPERTIES LABELS "big")

# ── Dynamic-link variant ────────────────────────────────────────────────────
add_executable(test_concurrent_init_dyn test_concurrent_init.cpp)
target_link_libraries(test_concurrent_init_dyn PRIVATE
    libDaScriptDyn ${SRC_LIBRARIES})
add_dependencies(test_concurrent_init_dyn libDaScriptDyn ${DAS_DYN_MODULES_LIBS})
target_include_directories(test_concurrent_init_dyn PRIVATE ${NEED_MODULES_PATH})
SETUP_CPP11(test_concurrent_init_dyn)
set_target_properties(test_concurrent_init_dyn PROPERTIES FOLDER "tests-cpp/big")

add_test(NAME concurrent_init_dyn COMMAND test_concurrent_init_dyn
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(concurrent_init_dyn PROPERTIES LABELS "big")

# Wire into the umbrella `test-big` target so `ninja test-big` builds these first.
add_dependencies(test-big test_concurrent_init test_concurrent_init_dyn)
