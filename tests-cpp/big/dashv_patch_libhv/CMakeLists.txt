# modules/dasHV/patch_libhv.cmake over a fixture of the two hlog.c passages it anchors on. Labelled
# small: one cmake -P over a two-passage fixture, so every per-PR ctest runs it.

add_test(
    NAME dashv_patch_libhv
    COMMAND ${CMAKE_COMMAND}
            -DPATCH_SCRIPT=${PROJECT_SOURCE_DIR}/modules/dasHV/patch_libhv.cmake
            -DFIXTURE=${CMAKE_CURRENT_SOURCE_DIR}/pristine_hlog.c
            -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}/work
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_patch_libhv.cmake)
set_tests_properties(dashv_patch_libhv PROPERTIES LABELS "small")
