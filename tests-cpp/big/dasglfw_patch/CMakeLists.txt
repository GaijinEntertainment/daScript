# modules/dasGlfw/patch_glfw.cmake over a one-file source and a patch the test writes, which edits
# that file and adds a second. Labelled small: a few cmake -P runs, so every per-PR ctest runs it.

find_package(Git QUIET)
if(GIT_FOUND)
    add_test(
        NAME dasglfw_patch
        COMMAND ${CMAKE_COMMAND}
                -DPATCH_SCRIPT=${PROJECT_SOURCE_DIR}/modules/dasGlfw/patch_glfw.cmake
                -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
                -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}/work
                -P ${CMAKE_CURRENT_SOURCE_DIR}/test_patch_glfw.cmake)
    set_tests_properties(dasglfw_patch PROPERTIES LABELS "small")
endif()
