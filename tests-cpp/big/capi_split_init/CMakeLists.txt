# Both smoke tests pull Module_Raster and Module_StbImage, which dasStbImage provides.
if(DAS_STBIMAGE_DISABLED)
    return()
endif()

# Pure-C ABI smoke test for split initialization and static module registration.
add_executable(test_capi_split_init test_capi_split_init.c)
target_link_libraries(test_capi_split_init PRIVATE
    libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
target_include_directories(test_capi_split_init PRIVATE ${NEED_MODULES_PATH})
SETUP_CPP11(test_capi_split_init)
set_target_properties(test_capi_split_init PROPERTIES
    FOLDER "tests-cpp/big"
    LINKER_LANGUAGE CXX)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(test_capi_split_init PRIVATE -Werror=strict-prototypes)
endif()

add_test(NAME capi_split_init COMMAND test_capi_split_init
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(capi_split_init PROPERTIES LABELS "big")
add_dependencies(test-big test_capi_split_init)

# C++ compatibility smoke: a namespaced host pulls separately compiled modules
# through their globally declared C-linkage registration entry points.
add_executable(test_cpp_module_pull test_cpp_module_pull.cpp)
target_link_libraries(test_cpp_module_pull PRIVATE
    libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
target_include_directories(test_cpp_module_pull PRIVATE ${NEED_MODULES_PATH})
SETUP_CPP11(test_cpp_module_pull)
set_target_properties(test_cpp_module_pull PROPERTIES FOLDER "tests-cpp/big")

add_test(NAME cpp_module_pull COMMAND test_cpp_module_pull
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(cpp_module_pull PROPERTIES LABELS "big")
add_dependencies(test-big test_cpp_module_pull)
