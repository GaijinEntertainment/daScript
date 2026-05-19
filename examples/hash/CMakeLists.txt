cmake_minimum_required(VERSION 3.16)
project(example_hash_bench CXX)

# Standalone bench — compares three hash table implementations on typical
# daslang key shapes:
#   * std::unordered_map / unordered_set      (chained baseline)
#   * das::daslang_hash_map / daslang_hash_set (in-tree, open addressing)
#   * absl::flat_hash_map / flat_hash_set     (Abseil SwissTable reference)
#
# Build:  cmake -S examples/hash -B build/example_hash_bench -DCMAKE_BUILD_TYPE=Release
#         cmake --build build/example_hash_bench -j
# Run:    ./build/example_hash_bench/example_hash_bench
#
# First configure clones Abseil (~2-3 min, cached in _deps/ afterwards).

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# --- Abseil (flat_hash_map / flat_hash_set baseline) ---------------------
include(FetchContent)
set(ABSL_PROPAGATE_CXX_STD ON CACHE BOOL "" FORCE)
set(ABSL_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    absl
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG        20240722.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(absl)

# --- bench executables ---------------------------------------------------
# Main bench: insert / churn / find x10 across {std, das, absl} x 5 key types.
add_executable(example_hash_bench bench_hash_map.cpp)
target_include_directories(example_hash_bench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(example_hash_bench PRIVATE
    absl::flat_hash_map
    absl::flat_hash_set
    absl::hash
    absl::strings)

# Hash-function isolation bench: find x10 only, on the 2x2 cross of
# {das_hash_map, absl::flat_hash_map} x {daslang_hash, absl::Hash}.
# Runs independently — `--target example_hash_func_bench` builds just this.
add_executable(example_hash_func_bench bench_hash_func.cpp)
target_include_directories(example_hash_func_bench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(example_hash_func_bench PRIVATE
    absl::flat_hash_map
    absl::hash
    absl::strings)

# Insert-only isolation bench: find x10, daslang_hash_{map,set} vs
# daslang_insert_only_hash_{map,set}. Build just this:
# `--target example_hash_insert_only_bench`.
add_executable(example_hash_insert_only_bench bench_hash_insert_only.cpp)
target_include_directories(example_hash_insert_only_bench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

if(MSVC)
    target_compile_options(example_hash_bench             PRIVATE /O2 /W4)
    target_compile_options(example_hash_func_bench        PRIVATE /O2 /W4)
    target_compile_options(example_hash_insert_only_bench PRIVATE /O2 /W4)
    # absl pulls in <Windows.h> indirectly via some platform headers; tame it
    target_compile_definitions(example_hash_bench             PRIVATE
        NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(example_hash_func_bench        PRIVATE
        NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(example_hash_insert_only_bench PRIVATE
        NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(example_hash_bench             PRIVATE -O3 -Wall -Wextra)
    target_compile_options(example_hash_func_bench        PRIVATE -O3 -Wall -Wextra)
    target_compile_options(example_hash_insert_only_bench PRIVATE -O3 -Wall -Wextra)
endif()
