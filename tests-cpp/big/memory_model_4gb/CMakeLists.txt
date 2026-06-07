# Big test: shoe-allocator 4 GB-boundary regression (das::Deck 64-bit migration).
# Allocates a real >4 GB chunk to prove total*size no longer wraps to 0, so it is
# local-only (`ninja test-big`), not part of CI.

add_executable(test_memory_model_4gb test_memory_model_4gb.cpp)
target_link_libraries(test_memory_model_4gb PRIVATE
    libDaScript ${SRC_LIBRARIES} ${DAS_MODULES_LIBS})
target_include_directories(test_memory_model_4gb PRIVATE ${NEED_MODULES_PATH})
SETUP_CPP11(test_memory_model_4gb)
set_target_properties(test_memory_model_4gb PROPERTIES FOLDER "tests-cpp/big")

add_test(NAME memory_model_4gb COMMAND test_memory_model_4gb
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties(memory_model_4gb PROPERTIES LABELS "big")

add_dependencies(test-big test_memory_model_4gb)
