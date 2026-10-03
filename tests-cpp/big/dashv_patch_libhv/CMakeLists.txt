# modules/dasHV/patch_libhv.cmake over a fixture of the logger passages it anchors on. Labelled
# small: one cmake -P over a logger fixture, so every per-PR ctest runs it.

add_test(
    NAME dashv_patch_libhv
    COMMAND ${CMAKE_COMMAND}
            -DPATCH_SCRIPT=${PROJECT_SOURCE_DIR}/modules/dasHV/patch_libhv.cmake
            -DFIXTURE=${CMAKE_CURRENT_SOURCE_DIR}/pristine_hlog.c
            -DWORK_DIR=${CMAKE_CURRENT_BINARY_DIR}/work
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_patch_libhv.cmake)
set_tests_properties(dashv_patch_libhv PROPERTIES LABELS "small")

if(TARGET libDasModuleHV)
    add_executable(test_hv_file_pump test_file_pump.cpp)
    target_link_libraries(test_hv_file_pump PRIVATE doctest)
    target_compile_features(test_hv_file_pump PRIVATE cxx_std_17)
    add_test(NAME hv_file_pump COMMAND test_hv_file_pump)
    set_tests_properties(hv_file_pump PROPERTIES LABELS "small")
    add_dependencies(test-small test_hv_file_pump)
    add_executable(test_hv_parser_limits test_parser_limits.cpp)
    target_compile_definitions(test_hv_parser_limits PRIVATE HV_STATICLIB)
    target_include_directories(test_hv_parser_limits SYSTEM PRIVATE ${DAS_HV_DIR}/hv/$<CONFIG>/include)
    target_link_libraries(test_hv_parser_limits PRIVATE doctest ${HV_LIBRARIES} ${CMAKE_DL_LIBS})
    das_link_openssl(test_hv_parser_limits)
    target_compile_features(test_hv_parser_limits PRIVATE cxx_std_17)
    add_dependencies(test_hv_parser_limits LIBHV)
    add_test(NAME hv_parser_limits COMMAND test_hv_parser_limits)
    set_tests_properties(hv_parser_limits PROPERTIES LABELS "small")
    add_dependencies(test-small test_hv_parser_limits)
endif()

if(TARGET libDasModuleHV)
    add_executable(hv_tls_fixture tls_fixture.cpp)
    target_compile_definitions(hv_tls_fixture PRIVATE HV_STATICLIB)
    target_include_directories(hv_tls_fixture SYSTEM PRIVATE ${DAS_HV_DIR}/hv/$<CONFIG>/include)
    target_link_libraries(hv_tls_fixture PRIVATE ${HV_LIBRARIES} ${CMAKE_DL_LIBS})
    das_link_openssl(hv_tls_fixture)
    target_compile_features(hv_tls_fixture PRIVATE cxx_std_17)
    add_dependencies(hv_tls_fixture LIBHV)
    add_dependencies(daslang hv_tls_fixture)
endif()

if(APPLE AND TARGET hv_tls_fixture)
    target_link_libraries(hv_tls_fixture PRIVATE "-framework Security" "-framework CoreFoundation")
    target_link_libraries(test_hv_parser_limits PRIVATE "-framework Security" "-framework CoreFoundation")
endif()
